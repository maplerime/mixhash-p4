#!/bin/bash
# 5 elephant + 59 mice cross-leaf TCP FCT test ("8 mice per sender"; the 5
# elephant senders h1-h5 drop the mouse that would share their elephant's
# (src,dst) disp index, so they send 7 each and h6-h8 send 8 = 59 mice).
# Elephants start at t=0 (768 KB each), mice at t=+DELAY (64 KB each).
# Dst balance: .21-.25 receive 7 mice, .26-.28 receive 8.
# 64 concurrent flows / ~7.5 MB total: timeouts raised vs m32e4 (nc -w 260).
# Usage: mice59ele5.sh <tag> [delay_s]
set -u
TAG=${1:?tag}
DELAY=${2:-2}
EKB=768
MKB=64
L2=root@45.196.164.19
OUT=/tmp/rct/fct_$TAG
mkdir -p $OUT

dstns() { case $1 in
  21) echo h9;; 22) echo h10;; 23) echo h11;; 24) echo h12;;
  25) echo h13;; 26) echo h14;; 27) echo h15;; 28) echo h16;; esac; }

srv() { # netns port
  ssh $L2 "rm -f /tmp/rx_$2; setsid nohup ip netns exec $1 sh -c 'nc -l -p $2 | wc -c > /tmp/rx_$2' >/dev/null 2>&1 < /dev/null &"
}
# elephant receivers
srv h9  24001; srv h10 24002; srv h11 24003; srv h12 24004; srv h13 24005
# mice: mXX -> port 230XX; MICE_SENDER/MICE_DST indexed from 1
MICE_SENDER=(h1 h1 h1 h1 h1 h1 h1  h2 h2 h2 h2 h2 h2 h2  h3 h3 h3 h3 h3 h3 h3
             h4 h4 h4 h4 h4 h4 h4  h5 h5 h5 h5 h5 h5 h5  h6 h6 h6 h6 h6 h6 h6 h6
             h7 h7 h7 h7 h7 h7 h7 h7  h8 h8 h8 h8 h8 h8 h8 h8)
MICE_DST=(24 27 22 25 28 23 26  21 26 23 28 25 24 27  22 25 24 27 26 21 28
          23 28 25 26 21 27 22  26 21 28 24 27 22 23  27 22 25 21 28 23 26 24
          28 23 26 22 21 24 25 27  21 24 27 23 22 26 28 25)
NM=${#MICE_DST[@]}
for i in $(seq 1 $NM); do
  p=$((23000 + i)); d=${MICE_DST[$((i-1))]}
  srv $(dstns $d) $p
done
sleep 1
ssh $L2 'for n in h9 h10 h11 h12 h13 h14 h15 h16; do echo -n "$n:$(ip netns exec $n ss -tln | awk "{print \$4}" | grep -c "23\|24") "; done; echo servers-up'

export PYTHONPATH=/root/bf-sde-9.13.3/install/lib/python3.8/site-packages:/root/bf-sde-9.13.3/install/lib/python3.8/site-packages/tofino
python3 /tmp/rct/disp_read.py --set m59e5 --reset
sleep 1

cli() { # host ip port kb outfile
  timeout 280 ip netns exec $1 sh -c "t0=\$(date +%s%N); dd if=/dev/zero bs=1024 count=$4 2>/dev/null | nc -N -w 260 $2 $3; t1=\$(date +%s%N); echo \$(( (t1-t0)/1000000 ))" > $5 2>/dev/null &
}

# fire elephants
cli h1 10.100.2.21 24001 $EKB $OUT/e1 &
cli h2 10.100.2.22 24002 $EKB $OUT/e2 &
cli h3 10.100.2.23 24003 $EKB $OUT/e3 &
cli h4 10.100.2.24 24004 $EKB $OUT/e4 &
cli h5 10.100.2.25 24005 $EKB $OUT/e5 &
sleep $DELAY
# fire 59 mice
for i in $(seq 1 $NM); do
  s=${MICE_SENDER[$((i-1))]}; p=$((23000 + i)); d=${MICE_DST[$((i-1))]}
  n=$(printf "m%02d" $i)
  cli $s 10.100.2.$d $p $MKB $OUT/$n &
done
wait
echo clients-done

sleep 3
for p in 24001 24002 24003 24004 24005 $(seq 23001 $((23000 + NM))); do
  v=""
  for try in 1 2 3 4 5 6; do v=$(ssh $L2 "cat /tmp/rx_$p 2>/dev/null"); [ -n "$v" ] && break; sleep 2; done
  echo "$v" > $OUT/rx_$p
done

echo "=== FCT ($TAG: 5 elephants ${EKB}KB + ${NM} mice ${MKB}KB, mice at +${DELAY}s) ==="
printf "%-6s %10s %12s\n" flow FCT_ms rx_bytes
for e in e1 e2 e3 e4 e5; do
  printf "%-6s %10s %12s\n" $e "$(cat $OUT/$e)" "$(cat $OUT/rx_2400${e#e})"
done
for i in $(seq 1 $NM); do
  n=$(printf "m%02d" $i)
  printf "%-6s %10s %12s\n" $n "$(cat $OUT/$n)" "$(cat $OUT/rx_$((23000 + i)))"
done
python3 - $OUT <<'EOF'
import sys
out = sys.argv[1]
def rd(f):
    try: return int(open(f"{out}/{f}").read().strip())
    except Exception: return None
es = [rd(f"e{i}") for i in (1,2,3,4,5)]
ms = [rd(f"m{i:02d}") for i in range(1,60)]
es = [x for x in es if x]; ms = [x for x in ms if x]
for name, v in (("elephant", es), ("mice", ms)):
    if v:
        print(f"{name}: n={len(v)} avg={sum(v)/len(v):.0f}ms max={max(v)}ms min={min(v)}ms")
EOF
echo "=== spine dispersion (P4 registers) ==="
python3 /tmp/rct/disp_read.py --set m59e5
