#!/bin/bash
# VXLAN leaf-spine fabric. usage: fabric.sh <underlay_dev> <local_ip> <vni:remote_ip> ...
set -e
DEV=$1; LOC=$2; shift 2
ip link add br0 type bridge stp_state 1 forward_delay 4
for spec in "$@"; do
    vni=${spec%%:*}; dst=${spec##*:}
    ip link add vxlan$vni type vxlan id $vni local $LOC dstport 4789 dev $DEV nolearning
    ip link set vxlan$vni mtu 1400
    bridge fdb append 00:00:00:00:00:00 dev vxlan$vni dst $dst
    ip link set vxlan$vni master br0
    ip link set vxlan$vni up
done
ip link set br0 mtu 1400 up
echo "fabric up: br0 + $*"
