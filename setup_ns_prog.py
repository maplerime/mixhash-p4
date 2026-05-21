#!/usr/bin/env python3
"""Populate switch tables for tna_arp_route via BF Runtime gRPC."""

import argparse
import sys

import bfrt_grpc.client as gc

MGID = 1


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--host", nargs=4, action="append",
                   metavar=("NAME", "PORT", "IP", "MAC"))
    args = p.parse_args()
    hosts = args.host

    if not hosts:
        print("  错误: 没有指定 host，请在 setup_ns.sh 的 HOSTS 中配置")
        sys.exit(1)

    # Convert to list of dicts
    hosts = [{"name": h[0], "port": int(h[1]), "ip": h[2], "mac": h[3]}
             for h in hosts]

    interface = gc.ClientInterface(grpc_addr="localhost:50052",
                                  client_id=0, device_id=0)
    interface.bind_pipeline_config("tna_arp_route")
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)

    # --- Clean up stale entries ---
    for tbl_name in ["SwitchIngress.smac_table",
                     "SwitchIngress.dmac_table",
                     "SwitchIngress.arp_table",
                     "SwitchIngress.ipv4_route"]:
        try:
            bfrt_info.table_get(tbl_name).entry_del(target, [])
        except Exception:
            pass

    mc_mgr = bfrt_info.table_get("$pre.node")
    mc_grp = bfrt_info.table_get("$pre.mgid")

    # Delete stale multicast entries
    try:
        for data, key in mc_grp.entry_get(target, [], flags={"from_hw": False}):
            mc_grp.entry_del(target, [key])
    except Exception:
        pass
    try:
        for data, key in mc_mgr.entry_get(target, [], flags={"from_hw": False}):
            mc_mgr.entry_del(target, [key])
    except Exception:
        pass

    # --- Create multicast group (all host ports) ---
    ports = [h["port"] for h in hosts]
    node_ids = []
    for i, port in enumerate(ports):
        nid = MGID * 256 + i + 1
        node_ids.append(nid)
        mc_mgr.entry_add(
            target,
            [mc_mgr.make_key([gc.KeyTuple("$MULTICAST_NODE_ID", nid)])],
            [mc_mgr.make_data([
                gc.DataTuple("$MULTICAST_RID", 0),
                gc.DataTuple("$MULTICAST_LAG_ID", int_arr_val=[]),
                gc.DataTuple("$DEV_PORT", int_arr_val=[port]),
            ])])

    mc_grp.entry_add(
        target,
        [mc_grp.make_key([gc.KeyTuple("$MGID", MGID)])],
        [mc_grp.make_data([
            gc.DataTuple("$MULTICAST_NODE_ID", int_arr_val=node_ids),
            gc.DataTuple("$MULTICAST_NODE_L1_XID_VALID",
                         bool_arr_val=[False] * len(node_ids)),
            gc.DataTuple("$MULTICAST_NODE_L1_XID",
                         int_arr_val=[0] * len(node_ids)),
            gc.DataTuple("$MULTICAST_ECMP_ID", int_arr_val=[]),
            gc.DataTuple("$MULTICAST_ECMP_L1_XID_VALID", bool_arr_val=[]),
            gc.DataTuple("$MULTICAST_ECMP_L1_XID", int_arr_val=[]),
        ])])
    print("  组播组 %d: 端口 %s" % (MGID, ports))

    # --- ARP table ---
    arp_table = bfrt_info.table_get("SwitchIngress.arp_table")
    arp_table.info.key_field_annotation_add(
        "hdr.arp_ipv4.target_proto_addr", "ipv4")

    # ARP request -> broadcast
    for h in hosts:
        arp_table.entry_add(
            target,
            [arp_table.make_key([
                gc.KeyTuple("hdr.arp.opcode", 0x0001),
                gc.KeyTuple("hdr.arp_ipv4.target_proto_addr", h["ip"]),
            ])],
            [arp_table.make_data([
                gc.DataTuple("mgid", MGID),
            ], "SwitchIngress.arp_broadcast")])
    print("  ARP table: request -> broadcast (mgid=%d)" % MGID)

    # ARP reply -> unicast
    for h in hosts:
        arp_table.entry_add(
            target,
            [arp_table.make_key([
                gc.KeyTuple("hdr.arp.opcode", 0x0002),
                gc.KeyTuple("hdr.arp_ipv4.target_proto_addr", h["ip"]),
            ])],
            [arp_table.make_data([
                gc.DataTuple("port", h["port"]),
            ], "SwitchIngress.arp_reply_unicast")])
    print("  ARP table: reply -> unicast")

    # --- SMAC table ---
    smac_table = bfrt_info.table_get("SwitchIngress.smac_table")
    smac_table.info.key_field_annotation_add("hdr.ethernet.src_addr", "mac")
    smac_table.info.key_field_annotation_add("hdr.ipv4.src_addr", "ipv4")

    for h in hosts:
        smac_table.entry_add(
            target,
            [smac_table.make_key([
                gc.KeyTuple("hdr.ethernet.src_addr", h["mac"]),
                gc.KeyTuple("hdr.ipv4.src_addr", h["ip"]),
            ])],
            [smac_table.make_data([
                gc.DataTuple("port", h["port"]),
            ], "SwitchIngress.learn")])
    print("  SMAC table: %d entries" % len(hosts))

    # --- DMAC table ---
    dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")
    dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")

    for h in hosts:
        dmac_table.entry_add(
            target,
            [dmac_table.make_key([
                gc.KeyTuple("hdr.ethernet.dst_addr", h["mac"]),
            ])],
            [dmac_table.make_data([
                gc.DataTuple("port", h["port"]),
            ], "SwitchIngress.forward")])
    print("  DMAC table: %d entries" % len(hosts))

    print("  表项下发完成!")
    interface._die = True


if __name__ == "__main__":
    main()
