#!/bin/bash
# 8-flow cross-leaf TCP FCT test: h1..h8 -> h9..h16, one flow per pair.
# Usage: tcp_fct_test.sh <tag> [KB_per_flow]
# Starts servers on netswan1 (leaf2), captures on both spines (ens8),
# fires 8 clients simultaneously, reports per-flow FCT + received bytes.
set -u
TAG=${1:?tag}
KB=${2:-64}
PORT_BASE=22000
L2=root@45.196.164.19
SP1=root@45.196.164.22
SP2=root@45.196.164.24
OUT=/tmp/rct/fct_$TAG
mkdir -p $OUT

# 1. servers on leaf2
for i in $(seq 1 8); do
  d=$((i+8)); p=$((PORT_BASE+i))
  ssh $L2 "rm -f /tmp/rx_$i; setsid nohup ip netns exec h$d sh -c 'nc -l -p $p | wc -c > /tmp/rx_$i' >/dev/null 2>&1 < /dev/null &"
done
sleep 1
ssh $L2 'for i in $(seq 1 8); do ip netns exec h$((i+8)) ss -tln | grep -q 2200$i || echo "MISSING server $i"; done; echo servers-up'

# 2. spine captures (traffic from leaf1 only, udp/4789)
for sp in $SP1 $SP2; do
  n=$(ssh $sp 'hostname')
  ssh $sp "rm -f /tmp/fct.pcap; setsid nohup tcpdump -i ens8 -w /tmp/fct.pcap 'udp port 4789 and host 45.196.164.29' >/dev/null 2>&1 < /dev/null &"
done
sleep 2

# 3. fire 8 clients simultaneously, each sends KB KB of zeros
for i in $(seq 1 8); do
  dst=10.100.2.$((20+i)); p=$((PORT_BASE+i))
  ip netns exec h$i sh -c "t0=\$(date +%s%N); dd if=/dev/zero bs=1024 count=$KB 2>/dev/null | nc -N -w 60 $dst $p; t1=\$(date +%s%N); echo \$(( (t1-t0)/1000000 ))" > $OUT/fct_h$i 2>/dev/null &
done
wait
echo clients-done

# 4. stop captures, collect everything
sleep 2
for sp in $SP1 $SP2; do ssh $sp 'pkill -x tcpdump'; done
sleep 1
scp -q $SP1:/tmp/fct.pcap $OUT/spine1.pcap
scp -q $SP2:/tmp/fct.pcap $OUT/spine2.pcap
for i in $(seq 1 8); do ssh $L2 "cat /tmp/rx_$i 2>/dev/null" > $OUT/rx_h$i; done

# 5. report
echo "=== FCT ($TAG, ${KB}KB/flow) ==="
printf "%-10s %10s %10s\n" flow FCT_ms rx_bytes
for i in $(seq 1 8); do
  f=$(cat $OUT/fct_h$i); r=$(cat $OUT/rx_h$i)
  printf "h%-9d %10s %10s\n" $i "$f" "$r"
done
echo "=== spine dispersion ==="
python3 /tmp/rct/tcp_disp.py $OUT/spine1.pcap $OUT/spine2.pcap
