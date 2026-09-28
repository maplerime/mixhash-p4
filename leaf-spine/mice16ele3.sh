#!/bin/bash
# 3 elephant + 16 mice (2 per sender, shuffled dsts) cross-leaf TCP FCT test.
# Elephants start at t=0 (768 KB each), mice at t=+DELAY (64 KB each).
# Flow layout (disp_idx collision-free, each dst receives exactly 2 mice):
#   e1 h1->h9(.21)  e2 h2->h10(.22) e3 h3->h11(.23)          [768 KB]
#   m01 h1->h12  m02 h1->h15  m03 h2->h13  m04 h2->h16
#   m05 h3->h14  m06 h3->h9   m07 h4->h15  m08 h4->h10
#   m09 h5->h16  m10 h5->h11  m11 h6->h9   m12 h6->h12
#   m13 h7->h10  m14 h7->h13  m15 h8->h11  m16 h8->h14       [64 KB]
# Usage: mice16ele3.sh <tag> [delay_s]
set -u
TAG=${1:?tag}
DELAY=${2:-2}
EKB=768
MKB=64
L2=root@45.196.164.19
OUT=/tmp/rct/fct_$TAG
mkdir -p $OUT

srv() { # netns port
  ssh $L2 "rm -f /tmp/rx_$2; setsid nohup ip netns exec $1 sh -c 'nc -l -p $2 | wc -c > /tmp/rx_$2' >/dev/null 2>&1 < /dev/null &"
}
# elephant receivers + mice receivers (dst netns port)
srv h9  24001; srv h10 24002; srv h11 24003
srv h12 23001; srv h15 23002; srv h13 23003; srv h16 23004
srv h14 23005; srv h9  23006; srv h15 23007; srv h10 23008
srv h16 23009; srv h11 23010; srv h9  23011; srv h12 23012
srv h10 23013; srv h13 23014; srv h11 23015; srv h14 23016
sleep 1
ssh $L2 'for n in h9 h10 h11 h12 h13 h14 h15 h16; do echo -n "$n:$(ip netns exec $n ss -tln | awk "{print \$4}" | grep -c "23\|24") "; done; echo servers-up'

export PYTHONPATH=/root/bf-sde-9.13.3/install/lib/python3.8/site-packages:/root/bf-sde-9.13.3/install/lib/python3.8/site-packages/tofino
python3 /tmp/rct/disp_read.py --set m16e3 --reset
sleep 1

cli() { # host ip port kb outfile
  timeout 150 ip netns exec $1 sh -c "t0=\$(date +%s%N); dd if=/dev/zero bs=1024 count=$4 2>/dev/null | nc -N -w 120 $2 $3; t1=\$(date +%s%N); echo \$(( (t1-t0)/1000000 ))" > $5 2>/dev/null &
}

# fire elephants
cli h1 10.100.2.21 24001 $EKB $OUT/e1 &
cli h2 10.100.2.22 24002 $EKB $OUT/e2 &
cli h3 10.100.2.23 24003 $EKB $OUT/e3 &
sleep $DELAY
# fire 16 mice
cli h1 10.100.2.24 23001 $MKB $OUT/m01 & cli h1 10.100.2.27 23002 $MKB $OUT/m02 &
cli h2 10.100.2.25 23003 $MKB $OUT/m03 & cli h2 10.100.2.28 23004 $MKB $OUT/m04 &
cli h3 10.100.2.26 23005 $MKB $OUT/m05 & cli h3 10.100.2.21 23006 $MKB $OUT/m06 &
cli h4 10.100.2.27 23007 $MKB $OUT/m07 & cli h4 10.100.2.22 23008 $MKB $OUT/m08 &
cli h5 10.100.2.28 23009 $MKB $OUT/m09 & cli h5 10.100.2.23 23010 $MKB $OUT/m10 &
cli h6 10.100.2.21 23011 $MKB $OUT/m11 & cli h6 10.100.2.24 23012 $MKB $OUT/m12 &
cli h7 10.100.2.22 23013 $MKB $OUT/m13 & cli h7 10.100.2.25 23014 $MKB $OUT/m14 &
cli h8 10.100.2.23 23015 $MKB $OUT/m15 & cli h8 10.100.2.26 23016 $MKB $OUT/m16 &
wait
echo clients-done

sleep 3
for p in 24001 24002 24003 23001 23002 23003 23004 23005 23006 23007 23008 23009 23010 23011 23012 23013 23014 23015 23016; do
  v=""
  for try in 1 2 3 4; do v=$(ssh $L2 "cat /tmp/rx_$p 2>/dev/null"); [ -n "$v" ] && break; sleep 2; done
  echo "$v" > $OUT/rx_$p
done

echo "=== FCT ($TAG: 3 elephants ${EKB}KB + 16 mice ${MKB}KB, mice at +${DELAY}s) ==="
printf "%-6s %10s %12s\n" flow FCT_ms rx_bytes
for e in e1 e2 e3; do
  printf "%-6s %10s %12s\n" $e "$(cat $OUT/$e)" "$(cat $OUT/rx_2400${e#e})"
done
for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16; do
  printf "m%-5s %10s %12s\n" $i "$(cat $OUT/m$i)" "$(cat $OUT/rx_230$i)"
done
python3 - $OUT <<'EOF'
import sys
out = sys.argv[1]
def rd(f):
    try: return int(open(f"{out}/{f}").read().strip())
    except Exception: return None
es = [rd(f"e{i}") for i in (1,2,3)]
ms = [rd(f"m{i:02d}") for i in range(1,17)]
es = [x for x in es if x]; ms = [x for x in ms if x]
for name, v in (("elephant", es), ("mice", ms)):
    if v:
        print(f"{name}: n={len(v)} avg={sum(v)/len(v):.0f}ms max={max(v)}ms min={min(v)}ms")
EOF
echo "=== spine dispersion (P4 registers) ==="
python3 /tmp/rct/disp_read.py --set m16e3
