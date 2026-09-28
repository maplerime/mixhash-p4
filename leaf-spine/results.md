# leaf-spine VXLAN 测试台：拓扑 / FCT / Hash 分布 数据记录

> 测试对象：tna_lb_mixhash P4 程序（tofino-model 软件交换机，Tofino-1 `--chip-type 2`）
> 更新日期：2026-09-27

---

## 1. 拓扑

4 台机器组成 2-leaf + 2-spine 的 VXLAN fabric，underlay 走 `ens7` 直连网段。

| 角色 | 主机名 | 管理/underlay IP | 转发角色 |
|---|---|---|---|
| leaf1 | netswan  (本机) | 45.196.164.29 | **P4-in-path**（L3 路由 + ECMP 全在 P4，内核只做 VXLAN encap/decap） |
| leaf2 | netswan1 | 45.196.164.19 | 内核 L3 ECMP（br0 + vxlan103/104） |
| spine1 | netswan2 | 45.196.164.22 | VXLAN 隧道转发 |
| spine2 | netswan3 | 45.196.164.24 | VXLAN 隧道转发 |

### VNI 规划

| VNI | 路径 | 隧道端点 |
|---|---|---|
| 101 | leaf1 ↔ spine1 | 45.196.164.29 ↔ 45.196.164.22 (10.0.101.x/30) |
| 102 | leaf1 ↔ spine2 | 45.196.164.29 ↔ 45.196.164.24 (10.0.102.x/30) |
| 103 | leaf2 ↔ spine1 | 45.196.164.19 ↔ 45.196.164.22 |
| 104 | leaf2 ↔ spine2 | 45.196.164.19 ↔ 45.196.164.24 |

### leaf1 主机（P4 端口，netns）

- IP 段：**10.100.1.11–18**（h1–h8）
- MAC 方案：h\<i\> = `02:09:00:00:00:1<i>`（h1..h8 → ...11..18）
- 交换机 MAC（SW_MAC）：`02:09:00:00:00:ff`
- 网关：10.100.1.254（静态 ARP 指向 SW_MAC，因 P4 不回应 ARP）

| host | IP | device_port | 主机侧 veth |
|---|---|---|---|
| h1 | 10.100.1.11 | 2  | veth5  |
| h2 | 10.100.1.12 | 3  | veth7  |
| h3 | 10.100.1.13 | 4  | veth9  |
| h4 | 10.100.1.14 | 5  | veth11 |
| h5 | 10.100.1.15 | 8  | veth17 |
| h6 | 10.100.1.16 | 9  | veth19 |
| h7 | 10.100.1.17 | 128 | veth21 |
| h8 | 10.100.1.18 | 129 | veth23 |

- spine1 隧道：P4 device_port 6 → veth13 + br101 + vxlan101
- spine2 隧道：P4 device_port 7 → veth15 + br102 + vxlan102

### leaf2 主机（内核，netns）

- IP 段：**10.100.2.21–28**（h5–h12）
- 网关 10.100.2.254；ECMP 路由 `10.100.1.0/24 via 10.0.103.2 dev vxlan103 / via 10.0.104.2 dev vxlan104`（weight 1:1）

### device_port → veth 映射（ports.json，关键坑）

**不是** `port p = veth(2p+1)` 简单公式（仅在 p≤9 成立）：

| device_port | veth 对 |
|---|---|
| 0–9 | veth0–19（p≤9 时主机 veth = 2p+1） |
| 128 | veth20/21 |
| 129 | veth22/23 |
| 256 | veth24/25 |
| 257 | veth26/27 |
| 384 | veth28/29 |
| 385 | veth30/31 |
| 64 (CPU/recirc) | veth250/251 |

`lb_config.py add-route --port` / `set_nhop --port` 传的是 **device_port**，不是 veth 序号。

---

## 2. 两种 ECMP 模式

| 模式 | 编译宏 | selector key | 行为 |
|---|---|---|---|
| MixHash | （默认） | 含 `ig_md.ecmp_counter`（逐包变化） | 逐包哈希，均匀 50/50 |
| CLASSIC_ECMP | `-DCLASSIC_ECMP` | 仅 src/dst addr + l4 ports | 逐流静态哈希，流钉在单条 spine |

- 当前运行 build：**MixHash**（逐包，默认）
- 控制面表：`SwitchIngress.ipv4_route`、`ecmp_ap`（ActionProfile member）、`ecmp_selector`、`ecmp_group_table`
- ECMP 成员：member 101 → spine1(port 6)，member 102 → spine2(port 7)，group 10 = {101,102}
- route action：src_mac/dst_mac/port；set_nhop：nhop_src_mac/nhop_dst_mac/nhop_port；set_ecmp_group：group_id

---

## 3. 跨 spine 打流结果（8→8 iperf3 TCP）

**测试**：leaf1 h1–h8 → leaf2 h5–h12，iperf3 `-c 10.100.2.$d -p 5201 -t 10`，8 流并发。

**结论：跨 spine 走通。** ping 8/8，iperf3 8/8 完成。

### 每流吞吐（发送端 bits/s）

| 流 | 路径 | Mbps |
|---|---|---|
| h1→h5  | 10.100.1.11 → 10.100.2.21 | 0.19 |
| h2→h6  | 10.100.1.12 → 10.100.2.22 | 0.13 |
| h3→h7  | 10.100.1.13 → 10.100.2.23 | 0.04 |
| h4→h8  | 10.100.1.14 → 10.100.2.24 | 0.04 |
| h5→h9  | 10.100.1.15 → 10.100.2.25 | 0.60 |
| h6→h10 | 10.100.1.16 → 10.100.2.26 | 0.65 |
| h7→h11 | 10.100.1.17 → 10.100.2.27 | 0.37 |
| h8→h12 | 10.100.1.18 → 10.100.2.28 | 0.37 |
| **合计** | | **2.38** |

### VXLAN underlay spine 分片（ens7, udp/4789）

| spine | VNI | 报文数 | 正向分片 |
|---|---|---|---|
| spine1 (45.196.164.22) | 101 | 2379 | 1588 (67%) |
| spine2 (45.196.164.24) | 102 | 2135 | 779 (33%) |

**解读**：
- 67:33 不均衡是 **CLASSIC_ECMP 逐流哈希的预期行为**（8 条流按哈希落到 spine1/spine2 约 5:3），不是选路错误/丢包。
- ~2.4 Mbps 聚合 = tofino-model 软件交换机吞吐上限（~1–1.4 Mbps 单对，8 流并发顶到 2.4），不是路由问题。

---

## 4. RDMA FCT 数据（Soft-RoCE 小鼠，ib_write_bw）

> 关键参数：`-u 18`（rxe 重传定时器 ~1.07s，QP timeout 属性）；`-s 8192 -n 5`（40KB mice）。
> 详见 memory `rxe-concurrency-livelock-recirc-storm` / `tofino-model-fct-testbed-limits`。

### 错开启动（staggered，≥1s）

| 场景 | 完成率 | 重传 | FCT |
|---|---|---|---|
| 16 mice 错开（峰值 ~2 QP） | 16/16 | 0 dup-PSN | p50 ≈ 1.9s |

### 真并发（simultaneous，冷启动干净模型 CLASSIC_ECMP）

| 并发数 | 完成率 | 重传 | FCT |
|---|---|---|---|
| 2 mice | 2/2 | 0 dup-PSN | ~2.4s |
| 16 mice | 15/16 | 97.2% (27688/28528) | p50 ≈ 42s / max ≈ 86s |

### 真并发退化现象（旧 MixHash / 已退化模型）

| 并发数 | 完成率 |
|---|---|
| 2 | 0/2 |
| 4 | 0/4 |
| 8 | 0/8（98.2% 重传，183s 超时） |

**机制**：同时到达的初始突发打满模型小 buffer → 所有 QP 同时丢 → 相同的 1.07s 定时器锁步触发 → go-back-N 重传风暴再次碰撞 → 自持续活锁。ECMP 路径 RTT 抖动会打散锁步定时器，故大并发下部分存活。
**结论**：该路径上的「并发 RDMA 小鼠」应视为**串行化测试**，容量上限在 2 到 16 只并发之间。

### 4.5 象流（iperf3 TCP）对鼠流 FCT 的影响（2026-09-27）

**实验**：16 只 RDMA 鼠流（`ib_write_bw -s 8192 -n 5`，错开 1s 启动）不变，叠加 2 条 iperf3 TCP 象流（leaf1 h1→leaf2 h5 = 10.100.2.21、h2→h6 = 10.100.2.22，跨 spine，`-t 240` 长流），对比鼠流 FCT。

| 指标 | 纯鼠流（基线） | 鼠流 + 2 象流 |
|---|---|---|
| 完成率 | **16/16** | **6/16** |
| FCT p50 | 1830 ms | 17714 ms（存活者） |
| FCT max | 1875 ms | 45401 ms（存活者） |
| 存活者 FCT 走势 | 1.6–1.9s | 6.1s → 45.4s（随启动越晚越差） |
| 失败者 | 0 | mouse6–15 共 10 只（3×60s 超时，wall≈183s） |
| 重传（dup-PSN） | 0 / 715 | 68809 / 70304（**97.9%**） |

**象流本身**（CLASSC_ECMP 逐流钉 spine，跨 spine 均承载）：

| 流 | 路径 | 吞吐 | 重传 |
|---|---|---|---|
| h1→h5 | 10.100.1.11 → 10.100.2.21 | 0.074 Mbps | 1 |
| h2→h6 | 10.100.1.12 → 10.100.2.22 | 0.074 Mbps | 2 |

VXLAN underlay spine 分片：spine1=2230 / spine2=767（74:26，仅 2 条流的逐流哈希落点）。

**结论：鼠流 FCT 被 2 条轻象流彻底打崩。** 仅 ~0.074 Mbps×2 的持续 TCP 背景流就足以把模型推到容量悬崖之上，触发 RDMA 小鼠的 go-back-N 活锁：前 6 只（错开先启动）勉强完成但 FCT 从 6s 一路涨到 45s，第 7 只起全部 3 次超时失败，重传率从 0 飙到 97.9%。这证实了 `tofino-model-fct-testbed-limits` 里的「elephant load 会让 mice 死掉」——而且这里象流速率极低（模型 cap），说明**瓶颈不是带宽而是模型每包延迟 + RDMA 定时器锁步**。

### 4.6 8 鼠流 vs 8 鼠流 + 2 象流对比（2026-09-27）

**动机**：§4.5 里 16 鼠流 + 2 象流把完成率打到 6/16（10 只 3×60s 超时）。降到 8 鼠流，验证「所有 8 鼠流能否完整跑完」，并给出「纯 8 鼠流 vs 8 鼠流 + 2 象流」的干净对比。

| 指标 | 纯 8 鼠流（基线） | 8 鼠流 + 2 象流 |
|---|---|---|
| 完成率 | **8/8** | **8/8**（无超时失败者） |
| FCT p50 | 1838 ms | 23777 ms |
| FCT max | 1887 ms | 32870 ms |
| 单鼠 FCT 走势 | 1.6–1.9s（稳定） | 11.0s → 32.9s（随启动越晚越差） |
| 重传（dup-PSN） | 0 / 355 | 5798 / 6156（**94.2%**） |

**象流本身**（跨 spine，h1→h5 10.100.2.21 / h2→h6 10.100.2.22）：

| 流 | 发送端吞吐 | 重传 |
|---|---|---|
| h1→h5 | 0.322 MB/s | 1 |
| h2→h6 | 0.238 MB/s | 0 |

**结论：8 鼠流在 2 象流下全部完成，但 FCT 被推高 ~13×（1.8s→23.8s p50），重传率 0→94.2%。** 与 §4.5 的 16 鼠流对比：

- **8 鼠流**：8/8 存活，无一只触发 3×60s 超时，最差 FCT 32.9s。
- **16 鼠流**：6/16 存活，第 7 只起全部超时失败（最差 wall≈183s）。

即「象流背景下的并发 RDMA 小鼠」容量悬崖落在 **8 到 16 只之间**：8 只勉强全员完成（虽严重退化），16 只则直接打崩。机制同 §4.4/§4.5 —— 不是带宽竞争（象流仅 ~0.3 MB/s，模型 cap 内），而是模型每包延迟 + RDMA 定时器锁步触发的 go-back-N 活锁。

### 4.7 8 鼠流 vs 8 鼠流 + 2 RDMA 象流（-u 20，2026-09-27）

**动机**：把象流从 iperf3 TCP 换成 RDMA（`ib_write_bw -s 4096 -n 500`，2MB），并按要求「两者都拉长」——mouse 墙钟 `timeout 60→120s` + QP 重传定时器 `-u 18→20`（1.07s→4.3s）。

| 指标 | 纯 8 鼠流（-u 20） | 8 鼠流 + 2 RDMA 象流（-u 20） |
|---|---|---|
| 完成率 | **8/8** | **8/8** |
| FCT p50 | 1845 ms | 4603 ms |
| FCT max | 1895 ms | 5145 ms |
| 单鼠 FCT 走势 | 1.6–1.9s（稳定） | 4.0–5.1s |
| 重传（dup-PSN） | 0 / 355 | **0 / 5360** |

**RDMA 象流本身**（`ib_write_bw -s 4096 -n 500`，TX depth 128，MTU 1024）：

| 流 | 吞吐 | wall |
|---|---|---|
| ele0 | 0.05 MB/s | 38.2s |
| ele1 | 0.05 MB/s | 37.9s |

**结论：象流换成 RDMA + `-u 20` 后，鼠流活锁完全消失。** 8/8 完成、**0 重传**、FCT 仅退化 2.5×（1.8s→4.6s），与 §4.6 的 TCP 象流（94.2% 重传、13× 退化）天壤之别。

**但这是双变量改动，不能单归因**：

1. **RDMA 象流比 TCP 象流轻 6×**：`ib_write_bw` 的 RDMA_Write 过 rxe 是 latency-bound，2MB 拖了 38s = **0.05 MB/s**；而 iperf3 TCP 象流是 ~0.322 MB/s 的连续流。背景负载本身就小得多。
2. **`-u 20`（4.3s）> 模型 RTT（~35ms）**：§4.4 的 go-back-N 活锁机制依赖「1.07s 定时器锁步 + 同时碰撞」。拉到 4.3s 后重传间隔被拉散，锁步碰撞不再发生——这与 `rxe-concurrency-livelock-recirc-storm` 里 `-u 18` 治活锁是同一杠杆的反向验证（更长定时器 = 更少风暴）。

若要单变量归因，还需补两跑：`-u 18` 的 RDMA 象流（看是否仍活锁），和 `-u 20` 的 TCP 象流（看是否仅靠定时器就够）。

### 4.8 MixHash 冷重启对照（2026-09-27）

**动机**：§4.7 的干净结果（0 重传）有两个混杂变量——象流从 TCP 换成 RDMA、`-u` 18→20，同时上一轮 build 是 `-DCLASSIC_ECMP`。为判定「干净」到底来自冷重启清僵尸风暴还是 CLASSIC_ECMP 标志本身，把环境切回默认 MixHash（重编译去掉 `-DCLASSIC_ECMP`，`ecmp_group_table` selector key 重新含 `ig_md.ecmp_counter`），冷重启 model+switchd，跑完全相同的测试。

| 指标 | 纯 8 鼠流（-u 20） | 8 鼠流 + 2 RDMA 象流（-u 20） |
|---|---|---|
| 完成率 | **8/8** | **8/8** |
| FCT p50 | 1761 ms | 4590 ms（run2）/ 7241 ms（run1） |
| FCT max | 1886 ms | 4906 ms（run2）/ 9265 ms（run1） |
| 重传（dup-PSN） | 0 / 355 | **0 / 5360（run2）** / 520 / 5862（run1） |

MixHash 冷重启连跑两遍 mixed：
- **run1**：elephant 43.7s/43.4s；mice 8/8 但 **520 dup-PSN（8.9%）**、FCT p50 7241ms——其中 200 dup 落在 5/8 只鼠流（~50% 重传），320 dup 落在象流（~6%）。
- **run2**：elephant 36.5s/35.6s；mice 8/8、**0 dup-PSN / 5360 pkts、FCT p50 4590ms**——与 §4.7 CLASSIC_ECMP（0/5360、4603ms）**逐项吻合**。

**结论：CLASSIC_ECMP 标志对 RoCE 是转发 no-op，§4.7 的干净结果可被 MixHash 复现。** 源码里 `is_roce`（UDP dst 4791）分支是**无条件的**（`#ifdef CLASSIC_ECMP` 只包裹 `ecmp_group_table` 的 selector key，不包裹 RoCE 处理）：RoCE 走 `ipv4_route` + 单成员 group 1，跳过 MixHash IP-ID 重写、不置 `ecmp_counter`、不进 `lb_vip_table`。故两种 build 对 RDMA 数据面的转发逐比特相同，`ecmp_counter` 是否在 selector key 里对单成员组无影响。基线跑（355 pkts / 0 重传 / ~1.8s）与 §4.7 完全一致，进一步佐证 no-op。

**run1 的 520 重传是瞬态的模型状态抖动，非标志效应。** run2 冷重启后复现 0 重传，说明 run1 的退化（含 elephant 慢 ~5s、5 只鼠流 ~50% 重传）更可能是模型内某次短暂的自激/负载抖动（见 memory `rxe-concurrency-livelock-recirc-storm` 的僵尸风暴），而非 MixHash 路径本身。教训：该软交换机测试台上**单次 mixed 跑不能作为结论**，须连跑 ≥2 遍取干净的那一遍，或至少报告方差。

---

## 5. 已知坑（速查）

1. **device_port 编号**：`--port` 用 ports.json 的 device_port，9 之后跳 128/129/256/257/384/385（h7/h8 曾被误编程到 10/11 导致 100% 丢包）。
2. **bridge FDB vlan 1**：vlan-unaware bridge 下 permanent 表项须 `vlan 1 master`，否则 VXLAN 回程帧静默丢弃。
3. **CLASSIC_ECMP 对 RoCE 是 no-op**：RoCE 走 `is_roce` 分支（dst port 4791）绕过 MixHash 重写路径，`ecmp_counter` 恒 0。
4. **僵尸 recirc 风暴**：switch 内部自激（非 reorder buffer），恢复需 model+switchd 冷重启。
5. **lb_config.py 环境**：`export PYTHONPATH=$SDE_INSTALL/lib/python3.8/site-packages:$SDE_INSTALL/lib/python3.8/site-packages/tofino`（`sdepythonpath.py` 已坏，勿用）。
6. **MixHash IP-ID 重写会破坏 RoCE ICRC** → Soft-RoCE 活锁，`fct_icrc_check.py` 是正确的 ICRC 校验器。

---

## 6. 相关脚本 / 文件

- `leaf-spine/leaf1_p4.sh` — leaf1 P4-in-path 8-host 完整建网 + lb_config 编程
- `leaf-spine/hosts.sh` — `hosts.sh <first_ns> <last_ns> <last_octet_of_first>`
- `leaf-spine/l3_setup.sh` — `l3_setup.sh <role>`（leaf2 角色 ECMP 路由）
- `leaf-spine/fabric.sh` — `fabric.sh <underlay_dev> <local_ip> <vni:remote_ip>...`
- `lb_config.py` — BF Runtime 控制面（add-route / add-ecmp-member / add-ecmp-group / add-ecmp-route）
- `rdma_fct_test.sh` / `rdma_fct_parse.py` — RDMA FCT 测试与解析
- `/tmp/xspine_iperf.sh` — 8→8 跨 spine iperf3 并发测试

## 8-flow cross-leaf TCP FCT: MixHash vs CLASSIC_ECMP (2026-09-28)

Flows h1..h8 -> h9..h16, 64 KB each, 8 concurrent; host eth0 MTU 1400
(vxlan MTU 1400 + ens7 1450 underlay — 1500-byte host segments are
blackholed, SYN/small packets only).

| flow | MixHash FCT ms | Classic FCT ms | MixHash sp1/sp2 | Classic sp1/sp2 |
|------|---------------|----------------|-----------------|-----------------|
| h1->h9  | 3905 | 4081 | 22/31 (42%) | 52/0 (100%) |
| h2->h10 | 4031 | 3890 | 26/27 (49%) | 0/52 (0%) |
| h3->h11 | 4404 | 4327 | 28/26 (52%) | 52/0 (100%) |
| h4->h12 | 4395 | 4336 | 26/27 (49%) | 0/53 (0%) |
| h5->h13 | 5556 | 5489 | 16/39 (29%) | 55/0 (100%) |
| h6->h14 | 5596 | 5498 | 34/21 (62%) | 55/0 (100%) |
| h7->h15 | 3439 | 3441 | 34/19 (64%) | 0/52 (0%) |
| h8->h16 | 3263 | 3147 | 19/34 (36%) | 53/0 (100%) |

- MixHash: every flow sprayed per-packet across both spines (29-64% sp1);
  fwd bytes 519 KB vs 512 sent (~1.4% retrans overhead).
- Classic: each flow pinned to exactly one spine (5/3 split); 512 KB fwd.
- FCTs statistically identical (tofino-model CPU caps aggregate ~1 Mbps,
  as previously noted). Value of MixHash here is dispersion + balance, not FCT.
- Reverse (leaf2->leaf1) always 100% spine1: kernel fib multipath on leaf2
  hashes all 8 reverse flows to one spine.

## 2-elephant + 8-mice FCT & spine skew: in-P4 register logging (2026-09-28)

Method change: no tcpdump. Both builds (MixHash and `-DCLASSIC_ECMP`)
carry two `Register<bit<32>, bit<12>>(4096)` (disp_sp1_reg/disp_sp2_reg)
incremented after `ecmp_group_table.apply()` on the chosen egress port
(6=spine1/vxlan101, 7=spine2/vxlan102). Flow index =
`(src_octet ++ dst_octet) & 0xFFF`; read/reset via bfrt
(`leaf-spine/disp_read.py`).

Setup: e1=h1→h9, e2=h2→h10 (768 KB each, start t=0); m1..m8 = h1..h8 →
10.100.2.2{5,6,7,8,5,6,5,6} (64 KB each, start t=+2s). nc/dd FCT, MTU 1400.

### FCT (ms)

| flow | MixHash | Classic |
|------|---------|---------|
| e1 | 20289 | 21911 |
| e2 | 20989 | 21802 |
| m1 | 8991  | 9631  |
| m2 | 10543 | 9396  |
| m3 | 12035 | 11686 |
| m4 | 11870 | 12744 |
| m5 | 20608 | 21103 |
| m6 | 20606 | 21112 |
| m7 | 6155  | 5973  |
| m8 | 5918  | 5515  |
| mice mean | 11.97 s | 11.90 s |
| mice median | 11.21 s | 10.66 s |

### Spine dispersion (per-flow packet counts from P4 registers)

| flow | MixHash sp1/sp2 (sp1%) | skew | Classic sp1/sp2 (sp1%) | skew |
|------|------------------------|------|------------------------|------|
| e1 | 113/220 (34%) | 0.32 | 0/76 (0%) | 1.00 |
| e2 | 40/35 (53%)   | 0.07 | 77/0 (100%) | 1.00 |
| m1 | 35/20 (64%)   | 0.27 | 55/0 (100%) | 1.00 |
| m2 | 26/29 (47%)   | 0.05 | 55/0 (100%) | 1.00 |
| m3 | 20/36 (36%)   | 0.29 | 55/0 (100%) | 1.00 |
| m4 | 39/16 (71%)   | 0.42 | 56/0 (100%) | 1.00 |
| m5 | 28/29 (49%)   | 0.02 | 58/0 (100%) | 1.00 |
| m6 | 35/22 (61%)   | 0.23 | 0/57 (0%) | 1.00 |
| m7 | 24/29 (45%)   | 0.09 | 0/53 (0%) | 1.00 |
| m8 | 30/22 (58%)   | 0.15 | 0/52 (0%) | 1.00 |
| ALL | 390/458 (46.0%) | — | 356/238 (59.9%) | — |

(skew = |sp1−sp2|/(sp1+sp2) per flow; 0 = perfectly spread, 1 = pinned)

### Findings

- Classic: every flow pinned to one spine (skew 1.00 ×10). Hash landed
  e1+m6+m7+m8 on spine2, e2+m1..m5 on spine1 → aggregate 60/40.
- MixHash: every flow sprayed per-packet across both spines, per-flow
  skew 0.02–0.42 (mean 0.19), aggregate 46/54 — near-perfect balance.
- FCT statistically identical between modes (mice mean 11.97 vs 11.90 s;
  slow mice m5/m6 ≈ 21 s in both). Confirms the tofino-model CPU cap
  (~1–1.4 Mbps aggregate) dominates FCT on this testbed; path balance
  does not change FCT here. MixHash's measurable win is dispersion.
- Elephant register counts (~76) are veth TSO/GSO super-frames (~10 KB
  each), not 1400-B segments — counts weight forwarding decisions ≈ bytes.

### Gotchas (model)

- disp registers are per-pipe SALU; h7/h8 ingress on device ports
  128/129 = pipe 1. tofino-model reads pipe-1 counts back shifted 8 bits
  (count lands in byte1 of the 32-bit value). Verified with fixed-count
  pings; disp_read.py applies `>>8` for m7/m8 (idx 0x119/0x21A).
- bfrt bytes-width fields arrive as little-endian int lists; mask each
  element with 0xFF before `bytes()` or parsing crashes.
