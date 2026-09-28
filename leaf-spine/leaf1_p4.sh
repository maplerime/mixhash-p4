#!/bin/bash
# leaf1 (netswan): P4 in the forwarding path (real ECMP across spine1/spine2).
#
#   h1-h8  (10.100.1.11-18)  ->  P4 device ports 2,3,4,5,8,9,128,129
#                                (host veth = veth<2p+1> for p<=9; ports.json
#                                 jumps to 128/129 after port 9)
#   P4 device port 6  ->  veth13 + br101 + vxlan101 -> spine1 (netswan2, 45.196.164.22)
#   P4 device port 7  ->  veth15 + br102 + vxlan102 -> spine2 (netswan3, 45.196.164.24)
#
# The kernel ONLY does VXLAN encapsulation/decapsulation; all L3 (routing + ECMP)
# is done by the P4. Because the P4 does not answer ARP, gateways are statically
# ARP'd to SW_MAC on every host/next-hop that resolves a P4-owned IP.
#
# Prereqs (already done): spines + leaf2 run fabric.sh/l3_setup.sh; P4 model +
# switchd running; this script re-programs routes via lb_config.py at the end.
set -euo pipefail

LOCAL_IP=45.196.164.29
UNDERLAY=ens7
SW_MAC=02:09:00:00:00:ff
SPINE1=45.196.164.22   # spine1 vxlan101 (10.0.101.2)
SPINE2=45.196.164.24   # spine2 vxlan102 (10.0.102.2)

# device_port AND host-veth for each host h1..h8 (from ports.json).
# device_port is what lb_config.py needs; the host-side veth is NOT (2p+1)
# beyond port 9 — it jumps to veth21/veth23 for ports 128/129.
PORTS=(0 2 3 4 5 8 9 128 129)       # device_port (h1..h8)
VETHS=(0 5 7 9 11 17 19 21 23)      # host-side veth (h1..h8)

# ---- idempotent cleanup of our own namespaces/bridges ----
for i in 1 2 3 4 5 6 7 8; do ip netns del h$i 2>/dev/null || true; done
ip link del br101 2>/dev/null || true
ip link del br102 2>/dev/null || true
ip link del vxlan101 2>/dev/null || true
ip link del vxlan102 2>/dev/null || true

# ---- host namespaces h1-h8 ----
# MAC scheme: h<i> = 02:09:00:00:00:1<i>   (h1..h8 -> ...11..18)
for i in 1 2 3 4 5 6 7 8; do
    port=${PORTS[$i]}          # device port (2,3,4,5,8,9,128,129)
    veth=${VETHS[$i]}          # host side of model veth pair (from ports.json)
    mac=$(printf '02:09:00:00:00:1%d' "$i")
    ip netns add h$i
    ip link set veth$veth netns h$i
    ip netns exec h$i ip link set veth$veth name eth0
    ip netns exec h$i ip link set eth0 address "$mac"
    ip netns exec h$i ip link set eth0 up
    ip netns exec h$i ip addr add 10.100.1.$((10+i))/24 dev eth0
    ip netns exec h$i ip route add default via 10.100.1.254 dev eth0
    ip netns exec h$i ip neigh replace 10.100.1.254 lladdr $SW_MAC dev eth0
    echo "h$i -> device_port $port (veth$veth=eth0, $mac, 10.100.1.$((10+i)))"
done

# ---- spine1 tunnel: P4 port 6 (veth13) + vxlan101 ----
ip link add br101 type bridge stp_state 0
ip link add vxlan101 type vxlan id 101 local $LOCAL_IP dstport 4789 dev $UNDERLAY nolearning
ip link set vxlan101 mtu 1400
bridge fdb append 00:00:00:00:00:00 dev vxlan101 dst $SPINE1
ip link set vxlan101 master br101
ip link set veth13 master br101
ip link set vxlan101 up
ip link set veth13 up
ip link set br101 mtu 1400 up
# vlan-unaware bridge: 必须显式 vlan 1 (PVID), 否则永久表项命不中转发查找,
# decapsulated VXLAN 回程帧会被丢弃 (见 return-path drop 诊断)
bridge fdb add $SW_MAC dev veth13 vlan 1 master

# ---- spine2 tunnel: P4 port 7 (veth15) + vxlan102 ----
ip link add br102 type bridge stp_state 0
ip link add vxlan102 type vxlan id 102 local $LOCAL_IP dstport 4789 dev $UNDERLAY nolearning
ip link set vxlan102 mtu 1400
bridge fdb append 00:00:00:00:00:00 dev vxlan102 dst $SPINE2
ip link set vxlan102 master br102
ip link set veth15 master br102
ip link set vxlan102 up
ip link set veth15 up
ip link set br102 mtu 1400 up
bridge fdb add $SW_MAC dev veth15 vlan 1 master

echo "leaf1 bridges up: br101(veth13+vxlan101->$SPINE1)  br102(veth15+vxlan102->$SPINE2)"

# ---- program P4 forwarding via lb_config.py ----
export PYTHONPATH=$SDE_INSTALL/lib/python3.8/site-packages:$SDE_INSTALL/lib/python3.8/site-packages/tofino
cd "$(dirname "$0")/.."

# ECMP members: spine1 (device port 6), spine2 (device port 7)
python3 lb_config.py add-ecmp-member 101 --src-mac $SW_MAC --dst-mac 42:12:26:be:e6:b0 --port 6
python3 lb_config.py add-ecmp-member 102 --src-mac $SW_MAC --dst-mac a2:2a:dd:d4:f4:1d --port 7
python3 lb_config.py add-ecmp-group 10 --members 101 102

# host routes 10.100.1.11-18 -> device ports (use device_port, not veth index!)
for i in 1 2 3 4 5 6 7 8; do
    port=${PORTS[$i]}
    mac=$(printf '02:09:00:00:00:1%d' "$i")
    python3 lb_config.py add-route 10.100.1.$((10+i))/32 --src-mac $SW_MAC --dst-mac "$mac" --port "$port"
done

# ECMP route: everything for leaf2 (10.100.2.0/24) -> ECMP group 10
python3 lb_config.py add-ecmp-route 10.100.2.0/24 --group 10

# RDMA (RoCE) route: 10.9.0.2/32 (leaf2 rxe, enters P4 on device port 0/veth1)
# -> the SAME 2-spine group 10 so MixHash sprays per-packet (PSN -> ecmp_counter).
# Replaces the old single-member group 1 (which pinned RoCE to one spine).
python3 lb_config.py del-route 10.9.0.2/32 2>/dev/null || true
python3 lb_config.py add-ecmp-route 10.9.0.2/32 --group 10

# RDMA return path: leaf1's own rxe (10.9.0.1 on veth1 = device port 0).
# WITHOUT this, RoCE/ping replies to 10.9.0.1 arrive on ports 6/7 and are
# dropped by the P4 — the perftest exchange then hangs and every "cross-spine
# RDMA broken" symptom reappears even though forwarding is fine.
VETH1_MAC=$(cat /sys/class/net/veth1/address)
python3 lb_config.py add-route 10.9.0.1/32 --src-mac $SW_MAC --dst-mac "$VETH1_MAC" --port 0

echo "leaf1 P4 routes programmed"
