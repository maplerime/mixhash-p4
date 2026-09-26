#!/bin/bash
# L3 ECMP conversion for the VXLAN leaf-spine fabric. usage: l3_setup.sh <role>
# roles: leaf1 leaf2 spine1 spine2
set -e
ROLE=$1
sysctl -w net.ipv4.ip_forward=1 >/dev/null
sysctl -w net.ipv4.fib_multipath_hash_policy=1 >/dev/null

case $ROLE in
leaf1)
    ip link set br0 type bridge stp_state 0
    ip link set vxlan101 nomaster
    ip link set vxlan102 nomaster
    ip addr add 10.100.1.254/24 dev br0
    ip addr add 10.0.101.1/30 dev vxlan101
    ip addr add 10.0.102.1/30 dev vxlan102
    ip route add 10.100.2.0/24 \
        nexthop via 10.0.101.2 dev vxlan101 weight 1 \
        nexthop via 10.0.102.2 dev vxlan102 weight 1
    for i in 1 2 3 4; do
        ip -n h$i addr flush dev eth0
        ip -n h$i addr add 10.100.1.$((10+i))/24 dev eth0
        ip -n h$i route add default via 10.100.1.254
    done
    ;;
leaf2)
    ip link set br0 type bridge stp_state 0
    ip link set vxlan103 nomaster
    ip link set vxlan104 nomaster
    ip addr add 10.100.2.254/24 dev br0
    ip addr add 10.0.103.1/30 dev vxlan103
    ip addr add 10.0.104.1/30 dev vxlan104
    ip route add 10.100.1.0/24 \
        nexthop via 10.0.103.2 dev vxlan103 weight 1 \
        nexthop via 10.0.104.2 dev vxlan104 weight 1
    for i in 5 6 7 8; do
        ip -n h$i addr flush dev eth0
        ip -n h$i addr add 10.100.2.$((16+i))/24 dev eth0
        ip -n h$i route add default via 10.100.2.254
    done
    ;;
spine1)
    ip link set br0 type bridge stp_state 0
    ip link set vxlan101 nomaster
    ip link set vxlan103 nomaster
    ip addr add 10.0.101.2/30 dev vxlan101
    ip addr add 10.0.103.2/30 dev vxlan103
    ip route add 10.100.1.0/24 via 10.0.101.1 dev vxlan101
    ip route add 10.100.2.0/24 via 10.0.103.1 dev vxlan103
    ;;
spine2)
    ip link set br0 type bridge stp_state 0
    ip link set vxlan102 nomaster
    ip link set vxlan104 nomaster
    ip addr add 10.0.102.2/30 dev vxlan102
    ip addr add 10.0.104.2/30 dev vxlan104
    ip route add 10.100.1.0/24 via 10.0.102.1 dev vxlan102
    ip route add 10.100.2.0/24 via 10.0.104.1 dev vxlan104
    ;;
*) echo "unknown role $ROLE"; exit 1;;
esac
echo "$ROLE L3 setup done"
