#!/usr/bin/env python3
"""Setup multicast group for tna_arp_route via BF Runtime gRPC.

Automatically discovers all front-panel ports and creates multicast group 1
for ARP request flooding. No per-host configuration needed.

Usage:
  export SDE=/path/to/sde          # default: /usr/local/sde
  export SDE_INSTALL=$SDE/install
  eval $($SDE_INSTALL/bin/sdepythonpath.py)
  python3 setup_ns_prog.py
"""

import argparse
import sys

import bfrt_grpc.client as gc

MGID = 1


def get_dev_ports(bfrt_info, target):
    """Get all front-panel device ports from the $PORT table."""
    port_tbl = bfrt_info.table_get("$PORT")
    ports = []
    try:
        for data, key in port_tbl.entry_get(target, [], flags={"from_hw": False}):
            d = key.to_dict()
            port = d["$DEV_PORT"]
            if port < 64:
                ports.append(port)
    except Exception:
        pass
    return sorted(ports)


def main():
    p = argparse.ArgumentParser(
        description="Setup multicast group for tna_arp_route")
    p.add_argument("--ports", nargs="+", type=int, default=None,
                   help="Manual port list (default: auto-detect)")
    p.add_argument("--grpc", default="localhost:50052",
                   help="gRPC server address")
    args = p.parse_args()

    interface = gc.ClientInterface(grpc_addr=args.grpc,
                                  client_id=0, device_id=0)
    interface.bind_pipeline_config("tna_arp_route")
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)

    # --- Clean up stale table entries ---
    for tbl_name in ["SwitchIngress.smac_table",
                     "SwitchIngress.dmac_table",
                     "SwitchIngress.ipv4_route"]:
        try:
            bfrt_info.table_get(tbl_name).entry_del(target, [])
        except Exception:
            pass

    # --- Clean up stale multicast entries ---
    mc_mgr = bfrt_info.table_get("$pre.node")
    mc_grp = bfrt_info.table_get("$pre.mgid")
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

    # --- Get port list ---
    if args.ports:
        ports = sorted(args.ports)
    else:
        ports = get_dev_ports(bfrt_info, target)
        if not ports:
            print("  No ports detected. Use --ports to specify manually")
            sys.exit(1)

    # --- Create multicast group (all data ports) ---
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

    print("  Multicast group %d: ports %s (%d ports)" % (MGID, ports, len(ports)))
    print("  Done! ARP auto-flood, SMAC/DMAC auto-learned by daemon")
    interface._die = True


if __name__ == "__main__":
    main()
