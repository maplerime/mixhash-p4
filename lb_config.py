#!/usr/bin/env python3
"""Control plane for tna_lb_mixhash: configure VIPs, backends, and ECMP via BF Runtime gRPC.

Usage:
  eval $($SDE_INSTALL/bin/sdepythonpath.py)

  # LB: VIP and backends
  python3 lb_config.py add-vip 10.0.100.1
  python3 lb_config.py add-backend 0 --ip 10.0.1.1 --mac 00:aa:bb:cc:01:01 --port 8
  python3 lb_config.py add-backend 1 --ip 10.0.1.2 --mac 00:aa:bb:cc:01:02 --port 9

  # ECMP: route with multiple next-hops
  python3 lb_config.py add-ecmp-member 1 --src-mac 00:11:22:33:44:55 --dst-mac 00:aa:bb:cc:01:01 --port 8
  python3 lb_config.py add-ecmp-member 2 --src-mac 00:11:22:33:44:55 --dst-mac 00:aa:bb:cc:01:02 --port 9
  python3 lb_config.py add-ecmp-group 1 --members 1 2
  python3 lb_config.py add-ecmp-route 10.0.0.0/16 --group 1

  # Direct route (single next-hop)
  python3 lb_config.py add-route 10.0.1.0/24 --src-mac 00:11:22:33:44:55 --dst-mac 00:aa:bb:cc:01:01 --port 8

  # Reorder: enable on receiver ports
  python3 lb_config.py enable-reorder 8
  python3 lb_config.py enable-reorder 9
  python3 lb_config.py clear-reorder

  # List / Clear
  python3 lb_config.py list
  python3 lb_config.py clear
"""

import argparse
import sys

import bfrt_grpc.client as gc

P4_NAME = "tna_lb_mixhash"
GRPC_ADDR = "localhost:50052"


def connect():
    interface = gc.ClientInterface(grpc_addr=GRPC_ADDR, client_id=0, device_id=0)
    interface.bind_pipeline_config(P4_NAME)
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)
    return interface, bfrt_info, target


# ---------------------------------------------------------------------------
# LB: VIP and backend management
# ---------------------------------------------------------------------------

def cmd_add_vip(args):
    interface, bfrt_info, target = connect()
    vip_tbl = bfrt_info.table_get("SwitchIngress.lb_vip_table")
    vip_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")

    vip_tbl.entry_add(
        target,
        [vip_tbl.make_key([gc.KeyTuple("hdr.ipv4.dst_addr", args.vip)])],
        [vip_tbl.make_data([], "SwitchIngress.lb_set_group")])

    print("Added VIP: %s" % args.vip)
    interface._die = True


def cmd_add_backend(args):
    interface, bfrt_info, target = connect()
    be_tbl = bfrt_info.table_get("SwitchIngress.lb_backend_table")
    be_tbl.info.data_field_annotation_add("be_ip", "SwitchIngress.lb_dnat", "ipv4")
    be_tbl.info.data_field_annotation_add("be_mac", "SwitchIngress.lb_dnat", "mac")

    be_tbl.entry_add(
        target,
        [be_tbl.make_key([gc.KeyTuple("ig_md.lb_backend_index", args.index)])],
        [be_tbl.make_data([
            gc.DataTuple("be_ip", args.ip),
            gc.DataTuple("be_mac", args.mac),
            gc.DataTuple("be_port", args.port),
        ], "SwitchIngress.lb_dnat")])

    print("Added backend %d: %s (%s) -> port %d" % (args.index, args.ip, args.mac, args.port))
    interface._die = True


def cmd_del_vip(args):
    interface, bfrt_info, target = connect()
    vip_tbl = bfrt_info.table_get("SwitchIngress.lb_vip_table")
    vip_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")

    vip_tbl.entry_del(
        target,
        [vip_tbl.make_key([gc.KeyTuple("hdr.ipv4.dst_addr", args.vip)])])

    print("Deleted VIP: %s" % args.vip)
    interface._die = True


def cmd_del_backend(args):
    interface, bfrt_info, target = connect()
    be_tbl = bfrt_info.table_get("SwitchIngress.lb_backend_table")

    be_tbl.entry_del(
        target,
        [be_tbl.make_key([gc.KeyTuple("ig_md.lb_backend_index", args.index)])])

    print("Deleted backend %d" % args.index)
    interface._die = True


# ---------------------------------------------------------------------------
# ECMP: ActionProfile member, Selector group, route
# ---------------------------------------------------------------------------

def cmd_add_ecmp_member(args):
    interface, bfrt_info, target = connect()
    ap_tbl = bfrt_info.table_get("SwitchIngress.ecmp_ap")
    ap_tbl.info.data_field_annotation_add("nhop_dst_mac", "SwitchIngress.set_nhop", "mac")
    ap_tbl.info.data_field_annotation_add("nhop_src_mac", "SwitchIngress.set_nhop", "mac")

    ap_tbl.entry_add(
        target,
        [ap_tbl.make_key([gc.KeyTuple("$ACTION_MEMBER_ID", args.member_id)])],
        [ap_tbl.make_data([
            gc.DataTuple("nhop_src_mac", args.src_mac),
            gc.DataTuple("nhop_dst_mac", args.dst_mac),
            gc.DataTuple("nhop_port", args.port),
        ], "SwitchIngress.set_nhop")])

    print("Added ECMP member %d: %s -> port %d" % (args.member_id, args.dst_mac, args.port))
    interface._die = True


def cmd_del_ecmp_member(args):
    interface, bfrt_info, target = connect()
    ap_tbl = bfrt_info.table_get("SwitchIngress.ecmp_ap")

    ap_tbl.entry_del(
        target,
        [ap_tbl.make_key([gc.KeyTuple("$ACTION_MEMBER_ID", args.member_id)])])

    print("Deleted ECMP member %d" % args.member_id)
    interface._die = True


def cmd_add_ecmp_group(args):
    interface, bfrt_info, target = connect()
    sel_tbl = bfrt_info.table_get("SwitchIngress.ecmp_selector")

    members = args.members
    member_status = [True] * len(members)

    sel_tbl.entry_add(
        target,
        [sel_tbl.make_key([gc.KeyTuple("$SELECTOR_GROUP_ID", args.group_id)])],
        [sel_tbl.make_data([
            gc.DataTuple("$MAX_GROUP_SIZE", len(members)),
            gc.DataTuple("$ACTION_MEMBER_ID", int_arr_val=members),
            gc.DataTuple("$ACTION_MEMBER_STATUS", bool_arr_val=member_status),
        ])])

    print("Added ECMP group %d: members %s" % (args.group_id, members))
    interface._die = True


def cmd_del_ecmp_group(args):
    interface, bfrt_info, target = connect()
    sel_tbl = bfrt_info.table_get("SwitchIngress.ecmp_selector")

    sel_tbl.entry_del(
        target,
        [sel_tbl.make_key([gc.KeyTuple("$SELECTOR_GROUP_ID", args.group_id)])])

    print("Deleted ECMP group %d" % args.group_id)
    interface._die = True


# ---------------------------------------------------------------------------
# Route: direct or ECMP
# ---------------------------------------------------------------------------

def parse_prefix(prefix_str):
    """Parse '10.0.0.0/16' into (ip_str, prefix_len)."""
    parts = prefix_str.split("/")
    if len(parts) != 2:
        print("Error: prefix must be in format A.B.C.D/N")
        sys.exit(1)
    return parts[0], int(parts[1])


def cmd_add_route(args):
    interface, bfrt_info, target = connect()
    route_tbl = bfrt_info.table_get("SwitchIngress.ipv4_route")
    route_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")
    route_tbl.info.data_field_annotation_add("src_mac", "SwitchIngress.route", "mac")
    route_tbl.info.data_field_annotation_add("dst_mac", "SwitchIngress.route", "mac")

    ip_str, prefix_len = parse_prefix(args.prefix)
    route_tbl.entry_add(
        target,
        [route_tbl.make_key([
            gc.KeyTuple("hdr.ipv4.dst_addr", ip_str, prefix_len=prefix_len)])],
        [route_tbl.make_data([
            gc.DataTuple("src_mac", args.src_mac),
            gc.DataTuple("dst_mac", args.dst_mac),
            gc.DataTuple("port", args.port),
        ], "SwitchIngress.route")])

    print("Added route %s -> port %d" % (args.prefix, args.port))
    interface._die = True


def cmd_add_ecmp_route(args):
    interface, bfrt_info, target = connect()
    route_tbl = bfrt_info.table_get("SwitchIngress.ipv4_route")
    route_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")

    ip_str, prefix_len = parse_prefix(args.prefix)
    route_tbl.entry_add(
        target,
        [route_tbl.make_key([
            gc.KeyTuple("hdr.ipv4.dst_addr", ip_str, prefix_len=prefix_len)])],
        [route_tbl.make_data([
            gc.DataTuple("group_id", args.group),
        ], "SwitchIngress.set_ecmp_group")])

    print("Added ECMP route %s -> group %d" % (args.prefix, args.group))
    interface._die = True


def cmd_del_route(args):
    interface, bfrt_info, target = connect()
    route_tbl = bfrt_info.table_get("SwitchIngress.ipv4_route")
    route_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")

    ip_str, prefix_len = parse_prefix(args.prefix)
    route_tbl.entry_del(
        target,
        [route_tbl.make_key([
            gc.KeyTuple("hdr.ipv4.dst_addr", ip_str, prefix_len=prefix_len)])])

    print("Deleted route %s" % args.prefix)
    interface._die = True


# ---------------------------------------------------------------------------
# List and Clear
# ---------------------------------------------------------------------------

def cmd_list(args):
    interface, bfrt_info, target = connect()

    # List VIPs
    vip_tbl = bfrt_info.table_get("SwitchIngress.lb_vip_table")
    vip_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")
    print("=== VIPs ===")
    try:
        for data, key in vip_tbl.entry_get(target, [], flags={"from_hw": False}):
            k = key.to_dict()
            print("  %s" % k["hdr.ipv4.dst_addr"])
    except Exception:
        print("  (empty)")

    # List backends
    be_tbl = bfrt_info.table_get("SwitchIngress.lb_backend_table")
    be_tbl.info.data_field_annotation_add("be_ip", "SwitchIngress.lb_dnat", "ipv4")
    be_tbl.info.data_field_annotation_add("be_mac", "SwitchIngress.lb_dnat", "mac")
    print("\n=== Backends ===")
    try:
        for data, key in be_tbl.entry_get(target, [], flags={"from_hw": False}):
            k = key.to_dict()
            d = data.to_dict()
            print("  index=%d  %s  %s  port=%d" % (
                k["ig_md.lb_backend_index"],
                d["be_ip"], d["be_mac"], d["be_port"]))
    except Exception:
        print("  (empty)")

    # List ECMP members
    ap_tbl = bfrt_info.table_get("SwitchIngress.ecmp_ap")
    ap_tbl.info.data_field_annotation_add("nhop_dst_mac", "SwitchIngress.set_nhop", "mac")
    print("\n=== ECMP Members ===")
    try:
        for data, key in ap_tbl.entry_get(target, [], flags={"from_hw": False}):
            k = key.to_dict()
            d = data.to_dict()
            print("  member=%d  %s  port=%d" % (
                k["$ACTION_MEMBER_ID"], d["nhop_dst_mac"], d["nhop_port"]))
    except Exception:
        print("  (empty)")

    # List ECMP groups
    sel_tbl = bfrt_info.table_get("SwitchIngress.ecmp_selector")
    print("\n=== ECMP Groups ===")
    try:
        for data, key in sel_tbl.entry_get(target, [], flags={"from_hw": False}):
            k = key.to_dict()
            d = data.to_dict()
            print("  group=%d  members=%s" % (
                k["$SELECTOR_GROUP_ID"]["value"],
                d["$ACTION_MEMBER_ID"]))
    except Exception:
        print("  (empty)")

    # List routes
    route_tbl = bfrt_info.table_get("SwitchIngress.ipv4_route")
    route_tbl.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")
    print("\n=== Routes ===")
    try:
        for data, key in route_tbl.entry_get(target, [], flags={"from_hw": False}):
            k = key.to_dict()
            d = data.to_dict()
            action = d.get("action", "")
            if "route" in action.lower():
                print("  %s -> direct  port=%d" % (k["hdr.ipv4.dst_addr"], d.get("port", "?")))
            else:
                print("  %s -> ecmp group %d" % (k["hdr.ipv4.dst_addr"], d.get("group_id", "?")))
    except Exception:
        print("  (empty)")

    interface._die = True


def cmd_clear(args):
    interface, bfrt_info, target = connect()

    for tbl_name in ["SwitchIngress.lb_vip_table", "SwitchIngress.lb_backend_table",
                     "SwitchIngress.smac_table", "SwitchIngress.dmac_table",
                     "SwitchIngress.ipv4_route", "SwitchIngress.reorder_enable_table"]:
        try:
            bfrt_info.table_get(tbl_name).entry_del(target, [])
        except Exception:
            pass

    # Clear ECMP
    try:
        bfrt_info.table_get("SwitchIngress.ecmp_selector").entry_del(target, [])
    except Exception:
        pass
    try:
        bfrt_info.table_get("SwitchIngress.ecmp_ap").entry_del(target, [])
    except Exception:
        pass

    # Reset flow counters
    counter_reg = bfrt_info.table_get("SwitchIngress.flow_counter_reg")
    try:
        for i in range(16384):
            counter_reg.entry_mod(
                target,
                [counter_reg.make_key([gc.KeyTuple("$REGISTER_INDEX", i)])],
                [counter_reg.make_data([gc.DataTuple("$REGISTER_VAL", 0)])])
    except Exception:
        pass

    # Reset reorder registers
    for reg_name, size in [("SwitchIngress.reorder_ctrl_reg", 4096)]:
        try:
            reg = bfrt_info.table_get(reg_name)
            for i in range(size):
                reg.entry_mod(
                    target,
                    [reg.make_key([gc.KeyTuple("$REGISTER_INDEX", i)])],
                    [reg.make_data([gc.DataTuple("$REGISTER_VAL", 0)])])
        except Exception:
            pass

    print("Cleared all entries, counters, and reorder buffers")
    interface._die = True


# ---------------------------------------------------------------------------
# Reorder: enable/disable per port, clear reorder state
# ---------------------------------------------------------------------------

def cmd_enable_reorder(args):
    interface, bfrt_info, target = connect()
    tbl = bfrt_info.table_get("SwitchIngress.reorder_enable_table")

    tbl.entry_add(
        target,
        [tbl.make_key([gc.KeyTuple("ig_md.ingress_port", args.port)])],
        [tbl.make_data([], "SwitchIngress.enable_reorder")])

    print("Enabled reorder on ingress port %d" % args.port)
    interface._die = True


def cmd_disable_reorder(args):
    interface, bfrt_info, target = connect()
    tbl = bfrt_info.table_get("SwitchIngress.reorder_enable_table")

    tbl.entry_del(
        target,
        [tbl.make_key([gc.KeyTuple("ig_md.ingress_port", args.port)])])

    print("Disabled reorder on ingress port %d" % args.port)
    interface._die = True


def cmd_clear_reorder(args):
    interface, bfrt_info, target = connect()

    for reg_name, size in [("SwitchIngress.reorder_ctrl_reg", 4096)]:
        try:
            reg = bfrt_info.table_get(reg_name)
            for i in range(size):
                reg.entry_mod(
                    target,
                    [reg.make_key([gc.KeyTuple("$REGISTER_INDEX", i)])],
                    [reg.make_data([gc.DataTuple("$REGISTER_VAL", 0)])])
        except Exception as e:
            print("Warning: could not reset %s: %s" % (reg_name, e))

    print("Cleared reorder control registers")
    interface._die = True


def main():
    p = argparse.ArgumentParser(description="Load balancer config for tna_lb_mixhash")
    p.add_argument("--grpc", default=GRPC_ADDR, help="gRPC server address")
    sub = p.add_subparsers(dest="command")

    # --- LB: add-vip ---
    sp = sub.add_parser("add-vip", help="Add a VIP address")
    sp.add_argument("vip", help="VIP address")

    # --- LB: add-backend ---
    sp = sub.add_parser("add-backend", help="Add a backend server")
    sp.add_argument("index", type=int, help="Backend index")
    sp.add_argument("--ip", required=True, help="Backend real IP")
    sp.add_argument("--mac", required=True, help="Backend MAC")
    sp.add_argument("--port", required=True, type=int, help="Egress port")

    # --- LB: del-vip ---
    sp = sub.add_parser("del-vip", help="Delete a VIP address")
    sp.add_argument("vip", help="VIP address")

    # --- LB: del-backend ---
    sp = sub.add_parser("del-backend", help="Delete a backend")
    sp.add_argument("index", type=int, help="Backend index")

    # --- ECMP: add-ecmp-member ---
    sp = sub.add_parser("add-ecmp-member", help="Add an ECMP next-hop member")
    sp.add_argument("member_id", type=int, help="Unique member ID")
    sp.add_argument("--src-mac", required=True, help="Source MAC (switch MAC)")
    sp.add_argument("--dst-mac", required=True, help="Next-hop MAC")
    sp.add_argument("--port", required=True, type=int, help="Egress port")

    # --- ECMP: del-ecmp-member ---
    sp = sub.add_parser("del-ecmp-member", help="Delete an ECMP member")
    sp.add_argument("member_id", type=int, help="Member ID")

    # --- ECMP: add-ecmp-group ---
    sp = sub.add_parser("add-ecmp-group", help="Create an ECMP group with members")
    sp.add_argument("group_id", type=int, help="Group ID")
    sp.add_argument("--members", nargs="+", type=int, required=True,
                    help="Member IDs to include in this group")

    # --- ECMP: del-ecmp-group ---
    sp = sub.add_parser("del-ecmp-group", help="Delete an ECMP group")
    sp.add_argument("group_id", type=int, help="Group ID")

    # --- Route: add-route (direct) ---
    sp = sub.add_parser("add-route", help="Add a direct route (single next-hop)")
    sp.add_argument("prefix", help="IP prefix (e.g. 10.0.1.0/24)")
    sp.add_argument("--src-mac", required=True, help="Source MAC (switch MAC)")
    sp.add_argument("--dst-mac", required=True, help="Next-hop MAC")
    sp.add_argument("--port", required=True, type=int, help="Egress port")

    # --- Route: add-ecmp-route ---
    sp = sub.add_parser("add-ecmp-route", help="Add a route pointing to an ECMP group")
    sp.add_argument("prefix", help="IP prefix (e.g. 10.0.0.0/16)")
    sp.add_argument("--group", required=True, type=int, help="ECMP group ID")

    # --- Route: del-route ---
    sp = sub.add_parser("del-route", help="Delete a route")
    sp.add_argument("prefix", help="IP prefix (e.g. 10.0.1.0/24)")

    # --- list ---
    sub.add_parser("list", help="List all entries")

    # --- clear ---
    sub.add_parser("clear", help="Clear all entries and reset counters")

    # --- Reorder: enable-reorder ---
    sp = sub.add_parser("enable-reorder", help="Enable reorder on an ingress port")
    sp.add_argument("port", type=int, help="Ingress port number")

    # --- Reorder: disable-reorder ---
    sp = sub.add_parser("disable-reorder", help="Disable reorder on an ingress port")
    sp.add_argument("port", type=int, help="Ingress port number")

    # --- Reorder: clear-reorder ---
    sub.add_parser("clear-reorder", help="Reset reorder registers")

    args = p.parse_args()
    if not args.command:
        p.print_help()
        sys.exit(0)

    globals()["cmd_" + args.command.replace("-", "_")](args)


if __name__ == "__main__":
    main()
