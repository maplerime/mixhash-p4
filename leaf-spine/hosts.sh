#!/bin/bash
# add host namespaces to br0. usage: hosts.sh <first_ns> <last_ns> <last_octet_of_first>
set -e
FIRST=$1; LAST=$2; OCT=$3
for i in $(seq $FIRST $LAST); do
    ip netns add h$i
    ip link add veth-h$i type veth peer name eth0 netns h$i
    ip link set veth-h$i mtu 1400 master br0 up
    ip -n h$i link set lo up
    ip -n h$i link set eth0 mtu 1400 up
    ip -n h$i addr add 10.100.0.$((OCT + i - FIRST))/24 dev eth0
done
echo "hosts h$FIRST-h$LAST up"
