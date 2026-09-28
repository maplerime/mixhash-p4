#!/bin/bash
# RDMA elephant+mice FCT test over the tofino-model path (netswan -> netswan1).
# Elephants: ib_write_bw -s 4096 -n 500 (2MB chunks, proven upper envelope:
#            rxe 6ms retrans timer < model RTT ~35ms, bigger windows collapse).
# Mice:      ib_write_bw -s 8192 -n 5 (perftest minimum) on control port 18600.
#
# usage: rdma_fct_test.sh <tag> <mode>   mode: mice | elephant | mixed
set -u
TAG=$1; MODE=$2
OUT=/tmp/rdma_fct_$TAG
N1=45.196.164.19
DEV=rxe0; GID=1
ELE_S=4096; ELE_N=500          # elephant chunk
ELE_PORTS=(18515 18516 18517 18518)
MICE_PORT_BASE=18600     # mouse k gets port 18600+k (one server per mouse)
MICE_N=${MICE_N:-16}     # rxe livelocks past ~4 concurrent QPs on this path
ELE_ACTIVE=${ELE_ACTIVE:-2}   # number of elephants to run in elephant/mixed mode
rm -rf $OUT; mkdir -p $OUT

# --- control-path bypass (ens7 direct): elephant ports + mice port ---
ensure_nat() {
    iptables -t nat -C OUTPUT -d 10.9.0.2/32 -p tcp --dport 18515:18699 \
        -j DNAT --to-destination $N1 2>/dev/null || \
    iptables -t nat -A OUTPUT -d 10.9.0.2/32 -p tcp --dport 18515:18699 \
        -j DNAT --to-destination $N1
    iptables -t nat -C POSTROUTING -o ens7 -p tcp -d $N1 --dport 18515:18699 \
        -j SNAT --to-source 45.196.164.29 2>/dev/null || \
    iptables -t nat -A POSTROUTING -o ens7 -p tcp -d $N1 --dport 18515:18699 \
        -j SNAT --to-source 45.196.164.29
}

# --- server-side one-shot restart loops, one background loop per port ---
start_servers() {
    local specs=""
    for p in "${ELE_PORTS[@]}"; do
        [ "$MODE" = mice ] && break
        specs="$specs while :; do ib_write_bw -s $ELE_S -n $ELE_N -d $DEV -x 2 -u 20 -p $p; done&"
    done
    for k in $(seq 0 $((MICE_N-1))); do
        [ "$MODE" = elephant ] && break
        specs="$specs while :; do ib_write_bw -s 8192 -n 5 -d $DEV -x 2 -u 20 -p $((MICE_PORT_BASE+k)); done&"
    done
    # NB: pkill must be a separate ssh call -- a single command line containing
    # both the pkill pattern and "ib_write_bw" kills its own shell (self-match).
    ssh root@$N1 "pkill -f '[i]b_write_bw' 2>/dev/null; sleep 1; echo cleaned"
    ssh root@$N1 "nohup bash -c '$specs wait' > /tmp/rdma_srv_$TAG.log 2>&1 & echo servers_up"
    sleep 1
}
stop_servers() { ssh root@$N1 "pkill -f '[i]b_write_bw' 2>/dev/null; echo servers_stopped"; }

elephant() {  # $1 = index (0-3)
    local p=${ELE_PORTS[$1]}
    local t0=$(date +%s.%N)
    timeout 120 ib_write_bw -s $ELE_S -n $ELE_N -d $DEV -x 1 -u 20 -p $p 10.9.0.2 \
        > $OUT/ele_$1.out 2>&1
    local rc=$?; local t1=$(date +%s.%N)
    echo "$t0 $t1 $rc" > $OUT/ele_$1.wall
}

mouse() {  # $1 = index
    local t0=$(date +%s.%N)
    local rc=1
    for try in 1 2 3; do
        timeout 120 ib_write_bw -s 8192 -n 5 -d $DEV -x 1 -u 20 -p $((MICE_PORT_BASE+$1)) 10.9.0.2 \
            > $OUT/mouse_$1.out 2>&1 && { rc=0; break; }
        sleep 1
    done
    local t1=$(date +%s.%N)
    echo "$t0 $t1 $rc $try" > $OUT/mouse_$1.wall
}

# --- capture RoCE on client side for retrans (PSN dup) analysis ---
start_cap() { nohup tcpdump -i veth1 -ne -U -w $OUT/roce.pcap 'udp port 4791' >/dev/null 2>&1 & echo $! > $OUT/cap.pid; }
stop_cap()  { kill $(cat $OUT/cap.pid) 2>/dev/null; }

ensure_nat
start_cap
start_servers
sleep 2

case $MODE in
mice)
    pids=()
    for k in $(seq 0 $((MICE_N-1))); do ( sleep $k; mouse $k ) & pids+=($!); done
    wait "${pids[@]}" ;;
elephant)
    pids=()
    for i in $(seq 0 $((ELE_ACTIVE-1))); do elephant $i & pids+=($!); done
    wait "${pids[@]}" ;;
mixed)
    pids=()
    for i in $(seq 0 $((ELE_ACTIVE-1))); do ( sleep $((i)); elephant $i ) & pids+=($!); done
    for k in $(seq 0 $((MICE_N-1))); do ( sleep $k; mouse $k ) & pids+=($!); done
    wait "${pids[@]}" ;;
*) echo "bad mode"; exit 1 ;;
esac

stop_cap
stop_servers
echo "=== $TAG/$MODE done -> $OUT ==="
