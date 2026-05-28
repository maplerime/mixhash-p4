#!/usr/bin/env python3
"""Learning daemon for tna_arp_route: receives digests and populates smac/dmac tables."""

import sys
import time
import signal
import argparse

import bfrt_grpc.client as gc

P4_NAME = "tna_arp_route"
GRPC_ADDR = "localhost:50052"

running = True


def signal_handler(sig, frame):
    global running
    running = False


def connect():
    interface = gc.ClientInterface(grpc_addr=GRPC_ADDR, client_id=0, device_id=0)
    interface.bind_pipeline_config(P4_NAME)
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)
    return interface, bfrt_info, target


def setup_learn_filter(bfrt_info):
    learn_filter = bfrt_info.learn_get("pipe.SwitchIngressDeparser.learn_digest")
    learn_filter.info.data_field_annotation_add("src_mac", "mac")
    learn_filter.info.data_field_annotation_add("src_ip", "ipv4")
    return learn_filter


def upsert_smac(smac_table, target, src_mac, src_ip, port):
    smac_table.info.key_field_annotation_add("hdr.ethernet.src_addr", "mac")
    smac_table.info.key_field_annotation_add("hdr.ipv4.src_addr", "ipv4")
    key = smac_table.make_key([
        gc.KeyTuple("hdr.ethernet.src_addr", src_mac),
        gc.KeyTuple("hdr.ipv4.src_addr", src_ip),
    ])
    data = smac_table.make_data([
        gc.DataTuple("port", port),
    ], "SwitchIngress.learn")
    try:
        smac_table.entry_mod(target, [key], [data])
    except Exception:
        try:
            smac_table.entry_add(target, [key], [data])
        except Exception as e:
            print("  smac add/mod error: %s" % e)


def upsert_dmac(dmac_table, target, dst_mac, port):
    dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")
    key = dmac_table.make_key([
        gc.KeyTuple("hdr.ethernet.dst_addr", dst_mac),
    ])
    data = dmac_table.make_data([
        gc.DataTuple("port", port),
    ], "SwitchIngress.forward")
    try:
        dmac_table.entry_mod(target, [key], [data])
    except Exception:
        try:
            dmac_table.entry_add(target, [key], [data])
        except Exception as e:
            print("  dmac add/mod error: %s" % e)


def main():
    parser = argparse.ArgumentParser(description="MAC learning daemon for tna_arp_route")
    parser.add_argument("-v", "--verbose", action="store_true", help="Print learned entries")
    parser.add_argument("-n", "--dry-run", action="store_true",
                        help="Only print digests, do not program tables")
    args = parser.parse_args()

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    print("Connecting to BF Runtime at %s ..." % GRPC_ADDR)
    interface, bfrt_info, target = connect()
    learn_filter = setup_learn_filter(bfrt_info)

    smac_table = bfrt_info.table_get("SwitchIngress.smac_table")
    dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")

    print("Learning daemon ready. Waiting for digests ...")
    count = 0
    while running:
        try:
            digest = interface.digest_get()
        except Exception as e:
            if not running:
                break
            print("digest_get error: %s" % e)
            time.sleep(0.5)
            continue

        data_list = learn_filter.make_data_list(digest)
        for d in data_list:
            dd = d.to_dict()
            src_mac = dd["src_mac"]
            src_ip = dd["src_ip"]
            port = dd["ingress_port"]
            count += 1

            if args.verbose or args.dry_run:
                print("  [%d] Learned: MAC=%s  IP=%s  port=%d" % (count, src_mac, src_ip, port))

            if not args.dry_run:
                upsert_smac(smac_table, target, src_mac, src_ip, port)
                upsert_dmac(dmac_table, target, src_mac, port)

    print("\nLearning daemon stopped. Processed %d digests." % count)
    interface._die = True


if __name__ == "__main__":
    main()
