#!/bin/bash
# Mice-only FCT test (no elephants): 32 x 8KB flows, one every 0.5s.
# usage: fct_mice.sh <tag>
TAG=$1
OUT=/tmp/fct_$TAG
rm -rf $OUT; mkdir -p $OUT

for k in $(seq 0 31); do
    i=$(( k%4 + 1 ))
    d=$(( k%4 + 5 ))
    dip=10.100.2.$((16+d))
    ( sleep $k
      t0=$(date +%s.%N)
      ip netns exec h$i iperf3 -J -c $dip -p 5202 -n 8K > $OUT/mice_${k}_h$i.json 2>$OUT/mice_${k}_h$i.err
      t1=$(date +%s.%N)
      echo "$t0 $t1 $i->$d" > $OUT/mice_${k}_h$i.wall ) &
done
wait
echo "=== $TAG mice-only done ==="
