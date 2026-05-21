#!/bin/bash
#
# setup_ns.sh — 创建 namespace、下发表项、测试连通性
#
# 用法:
#   ./setup_ns.sh          创建 namespace + 下发表项
#   ./setup_ns.sh ns       仅创建 namespace
#   ./setup_ns.sh prog     仅下发表项
#   ./setup_ns.sh cleanup  删除所有 namespace（并清理表项）
#   ./setup_ns.sh ping     测试 ping
#
# 前提: veth_setup.sh 已执行，model + switchd 已启动
#
# 拓扑:
#   ns_h0 [veth1, 10.0.0.1] <---> veth0  Switch Port 0
#   ns_h1 [veth3, 10.0.0.2] <---> veth2  Switch Port 1
#   ns_h2 [veth5, 10.0.0.3] <---> veth4  Switch Port 2
#
# veth 规则: veth<2*port> 留给 model, veth<2*port+1> 移入 namespace

set -e

# 确保 SDE 环境变量存在
SDE=${SDE:-/root/bf-sde-9.13.3}
SDE_INSTALL=${SDE_INSTALL:-$SDE/install}
export PATH=$SDE_INSTALL/bin:$PATH

# ============ 配置区 ============
# 格式: name:port:veth_host:ip:mac
HOSTS=(
    "ns_h0:0:veth1:10.0.0.1:00:aa:bb:cc:00:01"
    "ns_h1:1:veth3:10.0.0.2:00:aa:bb:cc:00:02"
    "ns_h2:2:veth5:10.0.0.3:00:aa:bb:cc:00:03"
)
PREFIX=24
MGID=1
P4_NAME="tna_arp_route"
# ================================

parse_host() {
    local IFS=':'
    local fields=($1)
    NAME="${fields[0]}"
    PORT="${fields[1]}"
    VETH="${fields[2]}"
    IP="${fields[3]}"
    MAC="${fields[4]}:${fields[5]}:${fields[6]}:${fields[7]}:${fields[8]}:${fields[9]}"
}

# ---------------------------------------------------------------------------
# 创建 namespace
# ---------------------------------------------------------------------------
do_ns() {
    echo "=== 创建 namespace 并配置 veth ==="

    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        local veth_sw="veth$((PORT * 2))"
        ip link set "$veth_sw" up 2>/dev/null || true
    done

    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        local veth_sw="veth$((PORT * 2))"

        ip netns add "$NAME"
        ip link set "$VETH" netns "$NAME"
        ip netns exec "$NAME" ip link set "$VETH" up
        ip netns exec "$NAME" ip link set "$VETH" address "$MAC"
        ip netns exec "$NAME" ip addr add "${IP}/${PREFIX}" dev "$VETH"
        ip netns exec "$NAME" sysctl -w "net.ipv6.conf.${VETH}.disable_ipv6=1" >/dev/null 2>&1 || true

        echo "  $NAME: $VETH ($IP/$PREFIX, MAC $MAC) <--> $veth_sw (port $PORT)"
    done

    echo ""
    echo "=== 配置静态 ARP ==="
    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        local my_name="$NAME" my_veth="$VETH"
        for other in "${HOSTS[@]}"; do
            parse_host "$other"
            [ "$my_name" = "$NAME" ] && continue
            ip netns exec "$my_name" ip neigh replace "$IP" lladdr "$MAC" dev "$my_veth"
        done
    done
    echo "  完成"

    echo ""
    echo "=== 状态 ==="
    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        echo "  $NAME ($IP):"
        ip netns exec "$NAME" ip addr show "$VETH" 2>/dev/null | grep inet | sed 's/^/    /'
    done
}

# ---------------------------------------------------------------------------
# 通过 BF Runtime 下发表项
# ---------------------------------------------------------------------------
do_prog() {
    echo ""
    echo "=== 通过 BF Runtime 下发表项 ==="

    # 设置 PYTHONPATH 以找到 bfrt_grpc 模块
    local PY_LIB
    PY_LIB=$(python3 -c "from distutils import sysconfig; print(sysconfig.get_python_lib(prefix='', standard_lib=True, plat_specific=True))")
    export PYTHONPATH=$($SDE_INSTALL/bin/sdepythonpath.py):$SDE_INSTALL/$PY_LIB/site-packages/tofino/bfrt_grpc:$SDE_INSTALL/$PY_LIB/site-packages:$SDE_INSTALL/$PY_LIB/site-packages/tofino:$PYTHONPATH

    # 从 HOSTS 数组构建 Python 参数列表
    local host_args=""
    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        host_args="$host_args --host $NAME $PORT $IP $MAC"
    done

    python3 "$SDE/setup_ns_prog.py" $host_args
}

# ---------------------------------------------------------------------------
# 清理
# ---------------------------------------------------------------------------
do_cleanup() {
    echo "=== 清理 namespace ==="
    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        if ip netns del "$NAME" 2>/dev/null; then
            echo "  已删除: $NAME"
        fi
    done
}

# ---------------------------------------------------------------------------
# Ping 测试
# ---------------------------------------------------------------------------
do_ping() {
    echo ""
    echo "=== Ping 测试 ==="
    local PASS=0 FAIL=0
    for entry in "${HOSTS[@]}"; do
        parse_host "$entry"
        local src_name="$NAME" src_veth="$VETH" src_ip="$IP"
        for other in "${HOSTS[@]}"; do
            parse_host "$other"
            [ "$src_name" = "$NAME" ] && continue
            printf "  %-8s -> %-8s (%s -> %s): " "$src_name" "$NAME" "$src_ip" "$IP"
            if ip netns exec "$src_name" ping -I "$src_veth" -c 3 -W 2 "$IP" >/dev/null 2>&1; then
                echo "OK"; PASS=$((PASS + 1))
            else
                echo "FAIL"; FAIL=$((FAIL + 1))
            fi
        done
    done
    echo ""
    echo "结果: $PASS 通过, $FAIL 失败"
    return $FAIL
}

# ---------------------------------------------------------------------------
case "${1:-}" in
    cleanup)  do_cleanup ;;
    ns)       do_ns ;;
    prog)     do_prog ;;
    ping)     do_ping ;;
    *)        do_ns; do_prog ;;
esac
