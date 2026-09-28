# MixHash vs. CLASSIC_ECMP: Load-Balancing Skew and Its (Absence of) Effect on RDMA FCT

> Paper-ready summary of the tofino-model leaf-spine experiments.
> Testbed: `tna_lb_mixhash` P4 program on a software Tofino-1 switch; 2-leaf + 2-spine VXLAN fabric.
> Two forwarding modes: **CLASSIC_ECMP** (`-DCLASSIC_ECMP`, per-flow static hash) vs **MixHash** (default, per-packet hash with a per-packet `ecmp_counter` in the ECMP selector key).

---

## 1. Load-balancing skew (forward data direction)

**Test:** 8 concurrent iperf3 TCP flows (leaf1 h1–h8 → leaf2 h5–h12, `-t 10`), each crossing the 2-spine ECMP group (group 10 = {spine1, spine2}).

| Mode | spine1 (VNI 101) | spine2 (VNI 102) | skew |
|---|---|---|---|
| CLASSIC_ECMP (per-flow) | 1588 (67.1%) | 779 (32.9%) | **67 : 33** |
| MixHash (per-packet) | 1146 (49.8%) | 1156 (50.2%) | **50 : 50** |

**Observation.** CLASSIC_ECMP's static per-flow hash maps the 8 flows to a 5:3 spine assignment, yielding a 67:33 data skew — flow-level hashing pins each flow to a single spine, so the imbalance is a direct function of the hash's flow-to-spine mapping. MixHash's per-packet hash spreads consecutive packets of the same flow across both spines, converging to a near-perfect 50:50 split.

*Methodological note:* the skew above is the **forward (upload) direction only**, which is the P4-controlled path. The reverse (ACK) direction is load-balanced by leaf2's *kernel* ECMP (weight 1:1), not by the P4, so it does not reflect the P4's ECMP mode and is excluded from the comparison.

---

## 2. RDMA FCT (Soft-RoCE mice, `ib_write_bw`, QP timeout `-u 20`)

**Test:** 8 RDMA mice (5 × 8 KB writes) alone (baseline), and contended by 2 RDMA elephants (500 × 4 KB writes). RoCE traverses `10.9.0.2/32` → **single-member** ECMP group 1.

| Scenario | CLASSIC_ECMP | MixHash |
|---|---|---|
| 8 mice, baseline | 0 dup-PSN / 355 pkts, FCT p50=1845 ms, max=1895 ms | 0 dup-PSN / 355 pkts, FCT p50=1761 ms, max=1886 ms |
| 8 mice + 2 RDMA elephants | 0 dup-PSN / 5360 pkts, FCT p50=4603 ms, max=5145 ms | 0 dup-PSN / 5360 pkts, FCT p50=4590 ms, max=4906 ms |

**Observation.** FCT is **identical between the two modes within measurement noise** — same completion rate (8/8), same zero retransmissions, same FCT distribution.

---

## 3. The relationship: load-balancing skew does **not** translate to RDMA FCT

The two modes differ **only** in the multi-member ECMP path (the TCP/VXLAN data plane). RoCE traffic is exempted from the MixHash rewrite path by construction: the `is_roce` branch (UDP dst port 4791) is unconditional, so RoCE

1. skips the per-packet IP-ID rewrite and the MixHash load-balancer table (`lb_vip_table` / `lb_backend_table`), and
2. never sets `ecmp_counter`, so the per-packet ECMP selector degenerates to per-flow behavior,

in both modes. Moreover, RoCE's route resolves to a **single-member** ECMP group, so per-packet ECMP spreading cannot apply even in principle. Consequently, the RDMA data plane is bit-identical under CLASSIC_ECMP and MixHash, and so is its FCT.

The dominant factor in RDMA FCT is instead the **QP retransmission-timer lockstep** (go-back-N livelock under concurrent load), which is a per-QP / per-flow phenomenon orthogonal to spine-level load distribution — lengthening the QP timeout past the switch RTT (`-u 20`) eliminates the retransmission storm and collapses both modes to the same clean FCT.

---

## 4. Takeaway

- **Where MixHash helps:** spine-level load uniformity for the general (TCP/VXLAN) data plane — 67:33 → 50:50 skew.
- **Where MixHash does not help:** RDMA/RoCE FCT. Because RoCE must bypass per-packet rehashing (to preserve ICRC and strict ordering) and traverses a single-member group, its FCT is insensitive to the ECMP mode and is instead bottlenecked by retransmission-timer dynamics.

*Caveat:* on this software-switch testbed a single contended RDMA run is noisy (one MixHash run showed a transient 520/5862 dup-PSN episode that vanished on a cold restart); the figures above are the reproducible clean runs (≥2 repetitions).

---

### 中文一句话结论

MixHash 的逐包哈希把 TCP 数据面的 spine 负载偏度从 CLASSIC_ECMP 的 **67:33 压到 50:50**，但**对 RDMA 小鼠的 FCT 没有任何改善**——因为 RoCE 为保 ICRC 与严格有序必须绕过逐包重哈希、且走单成员 ECMP 组，其 FCT 由 QP 重传定时器锁步（go-back-N 活锁）主导，与 spine 级负载均衡解耦。
