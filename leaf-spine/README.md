# leaf-spine VXLAN fabric (netswan / netswan1-3)

在 4 台机器的 45.196.164.x 网卡上搭建的 VXLAN leaf-spine 拓扑,
作为 tna_lb_mixhash 负载均衡实验的主机侧网络。

## 角色 / IP

| 角色     | 机器      | 管理 IP (VTEP)  | underlay dev | overlay      |
|----------|-----------|-----------------|--------------|--------------|
| leaf1    | netswan   | 45.196.164.29   | ens7         | br0, h1-h4   |
| leaf2    | netswan1  | 45.196.164.19   | ens8         | br0, h5-h8   |
| spine1   | netswan2  | 45.196.164.22   | ens8         | br0          |
| spine2   | netswan3  | 45.196.164.24   | ens8         | br0          |

## 链路 (VXLAN, UDP 4789, 点对点静态 FDB)

- VNI 101: leaf1 <-> spine1
- VNI 102: leaf1 <-> spine2
- VNI 103: leaf2 <-> spine1
- VNI 104: leaf2 <-> spine2

每台机器一个 Linux bridge (br0, STP 开启, forward_delay 4)。
4 桥成环, STP 阻塞一个口 (通常在某个 spine 上), 跨 leaf 流量必经另一 spine。

## 主机 (netns + veth)

同网段 10.100.0.0/24, MTU 1400:

- leaf1: h1=10.100.0.11 h2=.12 h3=.13 h4=.14
- leaf2: h5=10.100.0.21 h6=.22 h7=.23 h8=.24

## 搭建

    # leaf1 (netswan)
    bash fabric.sh ens7 45.196.164.29 101:45.196.164.22 102:45.196.164.24
    bash hosts.sh 1 4 11
    # leaf2 (netswan1)
    bash fabric.sh ens8 45.196.164.19 103:45.196.164.22 104:45.196.164.24
    bash hosts.sh 5 8 21
    # spine1 (netswan2)
    bash fabric.sh ens8 45.196.164.22 101:45.196.164.29 103:45.196.164.19
    # spine2 (netswan3)
    bash fabric.sh ens8 45.196.164.24 102:45.196.164.29 104:45.196.164.19

## 设计说明 (为什么是 VXLAN)

- 不能用 VLAN 子接口做 leaf-spine 链路: 机房交换机会学习内层主机 MAC,
  把跨 leaf 单播直接抄近路送到对端 leaf, 绕过 spine (实测验证)。
- VXLAN 封装后机房交换机只看到 VTEP 间 UDP 单播, 内层 MAC 拓扑完整保留。
- 前提: 机房网络需放通 UDP (4789)。2026-09-25 已放通。
- 配置非持久化, 重启后重跑脚本。
