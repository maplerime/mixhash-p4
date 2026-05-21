#!/bin/bash
#
# test_arp_route_3hosts.sh
#
# 手动测试 tna_arp_route：三台主机通过 P4 交换机互通
# 每个 Host 放在独立的 network namespace 中，避免内核直接路由
#
# 拓扑：
#   ns_a [veth1, 10.0.0.1] <---> veth0  Switch Port 0
#   ns_b [veth5, 10.0.0.2] <---> veth4  Switch Port 2
#   ns_c [veth9, 10.0.0.3] <---> veth8  Switch Port 4
#

set -e

SDE=${SDE:-/root/bf-sde-9.13.3}
SDE_INSTALL=${SDE_INSTALL:-$SDE/install}

export PATH=$SDE_INSTALL/bin:$PATH
PYTHON_LIB_DIR=$(python3 -c "from distutils import sysconfig; print(sysconfig.get_python_lib(prefix='', standard_lib=True, plat_specific=True))")
export PYTHONPATH=$($SDE_INSTALL/bin/sdepythonpath.py):$SDE_INSTALL/$PYTHON_LIB_DIR/site-packages/tofino/bfrt_grpc:$SDE_INSTALL/$PYTHON_LIB_DIR/site-packages:$SDE_INSTALL/$PYTHON_LIB_DIR/site-packages/tofino:$PYTHONPATH

# ---- 配置 ----
HOST_A_MAC="00:aa:bb:cc:dd:01"
HOST_A_IP="10.0.0.1"
HOST_A_PORT=0
HOST_A_VETH="veth1"
HOST_A_NS="ns_a"

HOST_B_MAC="00:aa:bb:cc:dd:02"
HOST_B_IP="10.0.0.2"
HOST_B_PORT=2
HOST_B_VETH="veth5"
HOST_B_NS="ns_b"

HOST_C_MAC="00:aa:bb:cc:dd:03"
HOST_C_IP="10.0.0.3"
HOST_C_PORT=4
HOST_C_VETH="veth9"
HOST_C_NS="ns_c"

MGID=100

echo "============================================"
echo " tna_arp_route 三主机互通测试"
echo "============================================"

# ---- Step 1: 创建 veth 和 namespace ----
echo ""
echo "[Step 1] 创建 veth 接口和 network namespace..."

# 先清理旧 namespace（归还 veth）
for ns in $HOST_A_NS $HOST_B_NS $HOST_C_NS; do
    ip netns del $ns 2>/dev/null || true
done

# 跳过 veth_setup.sh，避免重建 veth 断开 model 连接
# veth 应在启动 model 之前已创建好
# $SDE_INSTALL/bin/veth_setup.sh 10

# 确保内部 veth（连 tofino model）在主 namespace 中 UP
for veth in veth0 veth2 veth4 veth6 veth8; do
    ip link set dev $veth up 2>/dev/null || true
done

# 创建 namespace，把外部 veth 移入
setup_host_ns() {
    local ns=$1
    local veth=$2
    local ip_addr=$3
    local mac=$4

    ip netns add $ns
    ip link set dev $veth netns $ns
    ip netns exec $ns ip link set dev $veth up
    ip netns exec $ns ip link set dev $veth address $mac
    ip netns exec $ns ip addr add ${ip_addr}/24 dev $veth
    ip netns exec $ns sysctl -w net.ipv6.conf.$veth.disable_ipv6=1 >/dev/null 2>&1 || true
}

setup_host_ns $HOST_A_NS $HOST_A_VETH $HOST_A_IP $HOST_A_MAC
setup_host_ns $HOST_B_NS $HOST_B_VETH $HOST_B_IP $HOST_B_MAC
setup_host_ns $HOST_C_NS $HOST_C_VETH $HOST_C_IP $HOST_C_MAC

# 设置静态 ARP（在 namespace 内部）
ip netns exec $HOST_A_NS ip neigh replace $HOST_B_IP lladdr $HOST_B_MAC dev $HOST_A_VETH
ip netns exec $HOST_A_NS ip neigh replace $HOST_C_IP lladdr $HOST_C_MAC dev $HOST_A_VETH
ip netns exec $HOST_B_NS ip neigh replace $HOST_A_IP lladdr $HOST_A_MAC dev $HOST_B_VETH
ip netns exec $HOST_B_NS ip neigh replace $HOST_C_IP lladdr $HOST_C_MAC dev $HOST_B_VETH
ip netns exec $HOST_C_NS ip neigh replace $HOST_A_IP lladdr $HOST_A_MAC dev $HOST_C_VETH
ip netns exec $HOST_C_NS ip neigh replace $HOST_B_IP lladdr $HOST_B_MAC dev $HOST_C_VETH

echo "  $HOST_A_NS: $HOST_A_VETH $HOST_A_IP/24 (MAC $HOST_A_MAC) -> port $HOST_A_PORT"
echo "  $HOST_B_NS: $HOST_B_VETH $HOST_B_IP/24 (MAC $HOST_B_MAC) -> port $HOST_B_PORT"
echo "  $HOST_C_NS: $HOST_C_VETH $HOST_C_IP/24 (MAC $HOST_C_MAC) -> port $HOST_C_PORT"

# ---- Step 2: 通过 BF Runtime API 下发表项 ----
echo ""
echo "[Step 2] 通过 BF Runtime 下发交换机表项..."

python3 <<'PYEOF'
import bfrt_grpc.client as gc
import sys

HOST_A_MAC = "00:aa:bb:cc:dd:01"
HOST_A_IP  = "10.0.0.1"
HOST_A_PORT = 0

HOST_B_MAC = "00:aa:bb:cc:dd:02"
HOST_B_IP  = "10.0.0.2"
HOST_B_PORT = 2

HOST_C_MAC = "00:aa:bb:cc:dd:03"
HOST_C_IP  = "10.0.0.3"
HOST_C_PORT = 4

MGID = 1

def mac_to_int(mac):
    return int(mac.replace(':', ''), 16)

try:
    interface = gc.ClientInterface(grpc_addr="localhost:50052", client_id=0, device_id=0)
    interface.bind_pipeline_config("tna_arp_route")
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)

    mc_mgr = bfrt_info.table_get("$pre.node")
    mc_grp = bfrt_info.table_get("$pre.mgid")

    # Clean up stale entries
    # Delete P4 tables first
    for tbl_name in ["SwitchIngress.smac_table", "SwitchIngress.dmac_table", "SwitchIngress.arp_table"]:
        try:
            bfrt_info.table_get(tbl_name).entry_del(target, [])
        except:
            pass
    # Delete all mgid entries (enumerate and delete individually)
    try:
        resp = mc_grp.entry_get(target, [], flags={'from_hw': False})
        for data, key in resp:
            mc_grp.entry_del(target, [key])
    except:
        pass
    # Delete all multicast nodes
    try:
        resp = mc_mgr.entry_get(target, [], flags={'from_hw': False})
        for data, key in resp:
            mc_mgr.entry_del(target, [key])
    except:
        pass

    # Create multicast nodes and group
    ports = [HOST_A_PORT, HOST_B_PORT, HOST_C_PORT]
    node_ids = []
    for i, port in enumerate(ports):
        nid = MGID * 256 + i + 1
        node_ids.append(nid)
        mc_mgr.entry_add(
            target,
            [mc_mgr.make_key([gc.KeyTuple('$MULTICAST_NODE_ID', nid)])],
            [mc_mgr.make_data([gc.DataTuple('$MULTICAST_RID', 0),
                               gc.DataTuple('$MULTICAST_LAG_ID', int_arr_val=[]),
                               gc.DataTuple('$DEV_PORT', int_arr_val=[port])])])

    mc_grp.entry_add(
        target,
        [mc_grp.make_key([gc.KeyTuple('$MGID', MGID)])],
        [mc_grp.make_data([gc.DataTuple('$MULTICAST_NODE_ID', int_arr_val=node_ids),
                           gc.DataTuple('$MULTICAST_NODE_L1_XID_VALID', bool_arr_val=[False, False, False]),
                           gc.DataTuple('$MULTICAST_NODE_L1_XID', int_arr_val=[0, 0, 0]),
                           gc.DataTuple('$MULTICAST_ECMP_ID', int_arr_val=[]),
                           gc.DataTuple('$MULTICAST_ECMP_L1_XID_VALID', bool_arr_val=[]),
                           gc.DataTuple('$MULTICAST_ECMP_L1_XID', int_arr_val=[])])])
    print("  组播组 %d 已创建，包含端口 %s" % (MGID, ports))

    # ARP table: request -> broadcast
    arp_table = bfrt_info.table_get("SwitchIngress.arp_table")
    arp_table.info.key_field_annotation_add("hdr.arp_ipv4.target_proto_addr", "ipv4")

    for ip in [HOST_A_IP, HOST_B_IP, HOST_C_IP]:
        arp_table.entry_add(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0001),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', ip)])],
            [arp_table.make_data([gc.DataTuple('mgid', MGID)],
                                 'SwitchIngress.arp_broadcast')])
    print("  ARP table: request -> broadcast (mgid=%d)" % MGID)

    # ARP table: reply -> unicast
    arp_table.entry_add(
        target,
        [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0002),
                             gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', HOST_A_IP)])],
        [arp_table.make_data([gc.DataTuple('port', HOST_A_PORT)],
                             'SwitchIngress.arp_reply_unicast')])
    arp_table.entry_add(
        target,
        [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0002),
                             gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', HOST_B_IP)])],
        [arp_table.make_data([gc.DataTuple('port', HOST_B_PORT)],
                             'SwitchIngress.arp_reply_unicast')])
    arp_table.entry_add(
        target,
        [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0002),
                             gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', HOST_C_IP)])],
        [arp_table.make_data([gc.DataTuple('port', HOST_C_PORT)],
                             'SwitchIngress.arp_reply_unicast')])
    print("  ARP table: reply -> unicast")

    # SMAC table: learn src MAC + IP -> port
    smac_table = bfrt_info.table_get("SwitchIngress.smac_table")
    smac_table.info.key_field_annotation_add("hdr.ethernet.src_addr", "mac")
    smac_table.info.key_field_annotation_add("hdr.ipv4.src_addr", "ipv4")

    for mac, ip, port in [(HOST_A_MAC, HOST_A_IP, HOST_A_PORT),
                          (HOST_B_MAC, HOST_B_IP, HOST_B_PORT),
                          (HOST_C_MAC, HOST_C_IP, HOST_C_PORT)]:
        smac_table.entry_add(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip)])],
            [smac_table.make_data([gc.DataTuple('port', port)],
                                  'SwitchIngress.learn')])
    print("  SMAC table: 3 entries learned")

    # DMAC table: dst MAC -> forward to port
    dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")
    dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")

    for mac, port in [(HOST_A_MAC, HOST_A_PORT),
                      (HOST_B_MAC, HOST_B_PORT),
                      (HOST_C_MAC, HOST_C_PORT)]:
        dmac_table.entry_add(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac)])],
            [dmac_table.make_data([gc.DataTuple('port', port)],
                                  'SwitchIngress.forward')])
    print("  DMAC table: 3 entries for forwarding")

    print("  所有表项下发完成!")
    interface._die = True

except Exception:
    import traceback
    traceback.print_exc()
    sys.exit(1)
PYEOF

if [ $? -ne 0 ]; then
    echo "表项下发失败！"
    exit 1
fi

# ---- Step 3: 测试连通性 ----
echo ""
echo "[Step 3] 测试连通性..."

PASS=0
FAIL=0

ping_test() {
    local src_ns=$1
    local src_veth=$2
    local dst_ip=$3
    local label=$4
    echo -n "  ping $label ($src_ns -> $dst_ip): "
    if ip netns exec $src_ns ping -I $src_veth -c 3 -W 2 $dst_ip > /dev/null 2>&1; then
        echo "OK"
        PASS=$((PASS+1))
    else
        echo "FAIL"
        FAIL=$((FAIL+1))
    fi
}

echo ""
ping_test $HOST_A_NS $HOST_A_VETH $HOST_B_IP "A->B"
ping_test $HOST_A_NS $HOST_A_VETH $HOST_C_IP "A->C"
ping_test $HOST_B_NS $HOST_B_VETH $HOST_A_IP "B->A"
ping_test $HOST_B_NS $HOST_B_VETH $HOST_C_IP "B->C"
ping_test $HOST_C_NS $HOST_C_VETH $HOST_A_IP "C->A"
ping_test $HOST_C_NS $HOST_C_VETH $HOST_B_IP "C->B"

echo ""
echo "============================================"
echo " 测试结果: $PASS 通过, $FAIL 失败"
echo "============================================"

# ---- 清理提示 ----
echo ""
echo "清理: ip netns del $HOST_A_NS $HOST_B_NS $HOST_C_NS"

exit $FAIL
