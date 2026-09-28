#!/bin/bash
# 2 elephant + 8 mice concurrent cross-leaf TCP FCT test.
# Elephants start at t=0 (768 KB each), mice at t=+DELAY (64 KB each).
# Usage: mice2ele8.sh <tag> [delay_s]
set -u
TAG=${1:?tag}
DELAY=${2:-2}
EKB=768
MKB=64
L2=root@45.196.164.19
SP1=root@45.196.164.22
SP2=root@45.196.164.24
OUT=/tmp/rct/fct_$TAG
mkdir -p $OUT

srv() { # netns port
  ssh $L2 "rm -f /tmp/rx_$2; setsid nohup ip netns exec $1 sh -c 'nc -l -p $2 | wc -c > /tmp/rx_$2' >/dev/null 2>&1 < /dev/null &"
}
# elephants
srv h9  24001
srv h10 24002
# mice: sender -> receiver:port
declare -A MS=( [h1]=10.100.2.25:23011 [h2]=10.100.2.26:23012 [h3]=10.100.2.27:23013
                [h4]=10.100.2.28:23014 [h5]=10.100.2.25:23015 [h6]=10.100.2.26:23016
                [h7]=10.100.2.25:23017 [h8]=10.100.2.26:23018 )
srv h13 23011   # h1 -> .25
srv h14 23012   # h2 -> .26
srv h15 23013   # h3 -> .27
srv h16 23014   # h4 -> .28
srv h13 23015   # h5 -> .25
srv h14 23016   # h6 -> .26
srv h13 23017   # h7 -> .25
srv h14 23018   # h8 -> .26
sleep 1
ssh $L2 'for n in h9 h10 h11 h12 h13 h14 h15 h16; do echo -n "$n:$(ip netns exec $n ss -tln | awk "{print \$4}" | grep -c "23\|24") "; done; echo servers-up'

# reset P4 dispersion registers (in-switch per-flow spine counters)
export PYTHONPATH=/root/bf-sde-9.13.3/install/lib/python3.8/site-packages:/root/bf-sde-9.13.3/install/lib/python3.8/site-packages/tofino
python3 /tmp/rct/disp_read.py --reset
sleep 1

cli() { # host ip port kb outfile
  timeout 120 ip netns exec $1 sh -c "t0=\$(date +%s%N); dd if=/dev/zero bs=1024 count=$4 2>/dev/null | nc -N -w 90 $2 $3; t1=\$(date +%s%N); echo \$(( (t1-t0)/1000000 ))" > $5 2>/dev/null &
}

# fire elephants
cli h1 10.100.2.21 24001 $EKB $OUT/e1 &
cli h2 10.100.2.22 24002 $EKB $OUT/e2 &
sleep $DELAY
# fire mice
for i in 1 2 3 4 5 6 7 8; do
  t=${MS[h$i]}; cli h$i ${t%:*} ${t#*:} $MKB $OUT/m$i &
done
wait
echo clients-done

sleep 3
for p in 24001 24002 23011 23012 23013 23014 23015 23016 23017 23018; do
  v=""
  for try in 1 2 3 4; do v=$(ssh $L2 "cat /tmp/rx_$p 2>/dev/null"); [ -n "$v" ] && break; sleep 2; done
  echo "$v" > $OUT/rx_$p
done

echo "=== FCT ($TAG: 2 elephants ${EKB}KB + 8 mice ${MKB}KB, mice at +${DELAY}s) ==="
printf "%-6s %10s %12s\n" flow FCT_ms rx_bytes
printf "e1     %10s %12s\n" "$(cat $OUT/e1)" "$(cat $OUT/rx_24001)"
printf "e2     %10s %12s\n" "$(cat $OUT/e2)" "$(cat $OUT/rx_24002)"
for i in 1 2 3 4 5 6 7 8; do
  p=${MS[h$i]#*:}
  printf "m%-5d %10s %12s\n" $i "$(cat $OUT/m$i)" "$(cat $OUT/rx_$p)"
done
echo "=== spine dispersion (P4 registers) ==="
python3 /tmp/rct/disp_read.py
