#!/bin/bash
# 4 elephant + 32 mice (4 per sender, shuffled dsts) cross-leaf TCP FCT test.
# Elephants start at t=0 (768 KB each), mice at t=+DELAY (64 KB each).
# Flow layout (disp_idx collision-free, each dst receives exactly 4 mice):
#   e1 h1->h9(.21)  e2 h2->h10(.22) e3 h3->h11(.23) e4 h4->h12(.24)  [768 KB]
#   m01-m04  h1->.22 .23 .24 .25     m05-m08  h2->.23 .24 .25 .26
#   m09-m12  h3->.24 .25 .26 .27     m13-m16  h4->.25 .26 .27 .28
#   m17-m20  h5->.26 .27 .28 .21     m21-m24  h6->.27 .28 .21 .22
#   m25-m28  h7->.28 .21 .22 .23     m29-m32  h8->.21 .22 .23 .24  [64 KB]
# Usage: mice32ele4.sh <tag> [delay_s]
set -u
TAG=${1:?tag}
DELAY=${2:-2}
EKB=768
MKB=64
L2=root@45.196.164.19
OUT=/tmp/rct/fct_$TAG
mkdir -p $OUT

# dst last octet -> leaf2 netns
NS=""; dstns() { case $1 in
  21) echo h9;; 22) echo h10;; 23) echo h11;; 24) echo h12;;
  25) echo h13;; 26) echo h14;; 27) echo h15;; 28) echo h16;; esac; }

srv() { # netns port
  ssh $L2 "rm -f /tmp/rx_$2; setsid nohup ip netns exec $1 sh -c 'nc -l -p $2 | wc -c > /tmp/rx_$2' >/dev/null 2>&1 < /dev/null &"
}
# elephant receivers (dst port)
srv h9  24001; srv h10 24002; srv h11 24003; srv h12 24004
# mice receivers: mXX -> port 230XX on dst netns
MICE_DST=(22 23 24 25  23 24 25 26  24 25 26 27  25 26 27 28
          26 27 28 21  27 28 21 22  28 21 22 23  21 22 23 24)
for i in $(seq 1 32); do
  p=$((23000 + i)); d=${MICE_DST[$((i-1))]}
  srv $(dstns $d) $p
done
sleep 1
ssh $L2 'for n in h9 h10 h11 h12 h13 h14 h15 h16; do echo -n "$n:$(ip netns exec $n ss -tln | awk "{print \$4}" | grep -c "23\|24") "; done; echo servers-up'

export PYTHONPATH=/root/bf-sde-9.13.3/install/lib/python3.8/site-packages:/root/bf-sde-9.13.3/install/lib/python3.8/site-packages/tofino
python3 /tmp/rct/disp_read.py --set m32e4 --reset
sleep 1

cli() { # host ip port kb outfile
  timeout 150 ip netns exec $1 sh -c "t0=\$(date +%s%N); dd if=/dev/zero bs=1024 count=$4 2>/dev/null | nc -N -w 120 $2 $3; t1=\$(date +%s%N); echo \$(( (t1-t0)/1000000 ))" > $5 2>/dev/null &
}

# fire elephants
cli h1 10.100.2.21 24001 $EKB $OUT/e1 &
cli h2 10.100.2.22 24002 $EKB $OUT/e2 &
cli h3 10.100.2.23 24003 $EKB $OUT/e3 &
cli h4 10.100.2.24 24004 $EKB $OUT/e4 &
sleep $DELAY
# fire 32 mice: h1..h8, 4 each (dsts from MICE_DST above)
SENDERS=(h1 h2 h3 h4 h5 h6 h7 h8)
for i in $(seq 1 32); do
  s=$(( (i-1)/4 )); p=$((23000 + i)); d=${MICE_DST[$((i-1))]}
  n=$(printf "m%02d" $i)
  cli ${SENDERS[$s]} 10.100.2.$d $p $MKB $OUT/$n &
done
wait
echo clients-done

sleep 3
for p in 24001 24002 24003 24004 $(seq 23001 23032); do
  v=""
  for try in 1 2 3 4; do v=$(ssh $L2 "cat /tmp/rx_$p 2>/dev/null"); [ -n "$v" ] && break; sleep 2; done
  echo "$v" > $OUT/rx_$p
done

echo "=== FCT ($TAG: 4 elephants ${EKB}KB + 32 mice ${MKB}KB, mice at +${DELAY}s) ==="
printf "%-6s %10s %12s\n" flow FCT_ms rx_bytes
for e in e1 e2 e3 e4; do
  printf "%-6s %10s %12s\n" $e "$(cat $OUT/$e)" "$(cat $OUT/rx_2400${e#e})"
done
for i in $(seq 1 32); do
  n=$(printf "m%02d" $i)
  printf "%-6s %10s %12s\n" $n "$(cat $OUT/$n)" "$(cat $OUT/rx_$((23000 + i)))"
done
python3 - $OUT <<'EOF'
import sys
out = sys.argv[1]
def rd(f):
    try: return int(open(f"{out}/{f}").read().strip())
    except Exception: return None
es = [rd(f"e{i}") for i in (1,2,3,4)]
ms = [rd(f"m{i:02d}") for i in range(1,33)]
es = [x for x in es if x]; ms = [x for x in ms if x]
for name, v in (("elephant", es), ("mice", ms)):
    if v:
        print(f"{name}: n={len(v)} avg={sum(v)/len(v):.0f}ms max={max(v)}ms min={min(v)}ms")
EOF
echo "=== spine dispersion (P4 registers) ==="
python3 /tmp/rct/disp_read.py --set m32e4
