#!/bin/bash
# FCT test with rate-capped elephants so mice can complete.
# 4 elephants (8MB, capped -b 250K) + 16 mice (8KB) staggered 1/s.
# usage: fct_test2.sh <tag>
TAG=$1
OUT=/tmp/fct_$TAG
rm -rf $OUT; mkdir -p $OUT

# --- elephants: h1->h5 ... h4->h8, capped at 250Kbps each ---
for i in 1 2 3 4; do
    d=$((16+i+4))
    ( t0=$(date +%s.%N)
      ip netns exec h$i iperf3 -J -c 10.100.2.$d -p 5201 -n 8M -b 250K > $OUT/big_h$i.json 2>$OUT/big_h$i.err
      t1=$(date +%s.%N)
      echo "$t0 $t1" > $OUT/big_h$i.wall ) &
done

# --- mice: every 1s for 16s, rotating src/dst, port 5202 ---
for k in $(seq 0 15); do
    i=$(( k%4 + 1 ))
    d=$(( (i+k/4*2)%4 +5 ))
    dip=10.100.2.$((16+d))
    ( sleep $k
      t0=$(date +%s.%N)
      ip netns exec h$i iperf3 -J -c $dip -p 5202 -n 8K > $OUT/mice_${k}_h$i.json 2>$OUT/mice_${k}_h$i.err
      t1=$(date +%s.%N)
      echo "$t0 $t1 $i->$d" > $OUT/mice_${k}_h$i.wall ) &
done
wait
echo "=== $TAG done ==="
