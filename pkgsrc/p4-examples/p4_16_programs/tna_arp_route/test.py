#################################################################################
 #  P4 program test: tna_arp_route
 #
 #  Tests ARP broadcast, L2 forwarding (learned MAC-port), and IPv4 routing.
#################################################################################


import logging
import struct
import socket

from ptf import config
import ptf.testutils as testutils
from p4testutils.misc_utils import *
from bfruntime_client_base_tests import BfRuntimeTest
import bfrt_grpc.client as gc

logger = get_logger()
swports = get_sw_ports()


def build_arp_packet(eth_dst, eth_src, arp_opcode,
                     sender_mac, sender_ip, target_mac, target_ip):
    """Build an ARP packet."""
    eth_type = b'\x08\x06'
    pkt = (bytes.fromhex(eth_dst.replace(':', '')) +
           bytes.fromhex(eth_src.replace(':', '')) +
           eth_type +
           struct.pack('!HHBBH', 1, 0x0800, 6, 4, arp_opcode) +
           bytes.fromhex(sender_mac.replace(':', '')) +
           socket.inet_aton(sender_ip) +
           bytes.fromhex(target_mac.replace(':', '')) +
           socket.inet_aton(target_ip))
    return pkt


def setup_mc_group(bfrt_info, target, mgid, ports):
    """Helper: create a multicast group flooding to given ports."""
    mc_mgr = bfrt_info.table_get("$pre.node")
    mc_grp = bfrt_info.table_get("$pre.mgid")

    # Clean up stale entries first
    try:
        mc_grp.entry_del(target, [mc_grp.make_key([gc.KeyTuple('$MGID', mgid)])])
    except:
        pass
    try:
        prune_tbl.entry_del(target, [prune_tbl.make_key([gc.KeyTuple('$MULTICAST_L2_XID', mgid)])])
    except:
        pass

    node_ids = []
    for i, port in enumerate(ports):
        nid = mgid * 256 + i + 1
        node_ids.append(nid)
        try:
            mc_mgr.entry_del(target, [mc_mgr.make_key([gc.KeyTuple('$MULTICAST_NODE_ID', nid)])])
        except:
            pass
        mc_mgr.entry_add(
            target,
            [mc_mgr.make_key([gc.KeyTuple('$MULTICAST_NODE_ID', nid)])],
            [mc_mgr.make_data([gc.DataTuple('$MULTICAST_RID', 0),
                               gc.DataTuple('$MULTICAST_LAG_ID', int_arr_val=[]),
                               gc.DataTuple('$DEV_PORT', int_arr_val=[port])])])

    mc_grp.entry_add(
        target,
        [mc_grp.make_key([gc.KeyTuple('$MGID', mgid)])],
        [mc_grp.make_data([gc.DataTuple('$MULTICAST_NODE_ID', int_arr_val=node_ids),
                           gc.DataTuple('$MULTICAST_NODE_L1_XID_VALID', bool_arr_val=[False]*len(node_ids)),
                           gc.DataTuple('$MULTICAST_NODE_L1_XID', int_arr_val=[0]*len(node_ids)),
                           gc.DataTuple('$MULTICAST_ECMP_ID', int_arr_val=[]),
                           gc.DataTuple('$MULTICAST_ECMP_L1_XID_VALID', bool_arr_val=[]),
                           gc.DataTuple('$MULTICAST_ECMP_L1_XID', int_arr_val=[])])])
    return node_ids


def cleanup_mc_group(bfrt_info, target, mgid, node_ids):
    """Helper: remove a multicast group."""
    mc_mgr = bfrt_info.table_get("$pre.node")
    mc_grp = bfrt_info.table_get("$pre.mgid")
    mc_grp.entry_del(target, [mc_grp.make_key([gc.KeyTuple('$MGID', mgid)])])
    for nid in node_ids:
        mc_mgr.entry_del(target, [mc_mgr.make_key([gc.KeyTuple('$MULTICAST_NODE_ID', nid)])])


def get_data_ports():
    """Return swports for data plane ports only (exclude CPU/special)."""
    return [p for p in swports if p < 64]


def clean_p4_tables(bfrt_info, target):
    """Remove all entries from P4 tables to avoid stale data between tests."""
    mc_mgr = bfrt_info.table_get("$pre.node")
    mc_grp = bfrt_info.table_get("$pre.mgid")
    for tbl_name in ["SwitchIngress.arp_table", "SwitchIngress.smac_table",
                     "SwitchIngress.dmac_table", "SwitchIngress.ipv4_route"]:
        try:
            bfrt_info.table_get(tbl_name).entry_del(target, [])
        except Exception:
            pass
    try:
        for data, key in mc_grp.entry_get(target, [], flags={'from_hw': False}):
            mc_grp.entry_del(target, [key])
    except Exception:
        pass
    try:
        for data, key in mc_mgr.entry_get(target, [], flags={'from_hw': False}):
            mc_mgr.entry_del(target, [key])
    except Exception:
        pass


class ArpBroadcastTest(BfRuntimeTest):
    """@brief Test ARP request broadcast via multicast group."""

    def setUp(self):
        BfRuntimeTest.setUp(self, 0, "tna_arp_route")

    def runTest(self):
        data_ports = get_data_ports()
        ig_port = data_ports[0]
        target_ip = "10.0.0.2"
        sender_mac = "00:11:22:33:44:55"
        sender_ip = "10.0.0.1"

        bfrt_info = self.interface.bfrt_info_get("tna_arp_route")
        arp_table = bfrt_info.table_get("SwitchIngress.arp_table")
        arp_table.info.key_field_annotation_add("hdr.arp_ipv4.target_proto_addr", "ipv4")
        target = gc.Target(device_id=0, pipe_id=0xffff)

        clean_p4_tables(bfrt_info, target)

        # Setup multicast group 1 flooding to data ports only
        node_ids = setup_mc_group(bfrt_info, target, 1, data_ports)

        # ARP request for target_ip -> broadcast via mgid 1
        arp_table.entry_add(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0001),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', target_ip)])],
            [arp_table.make_data([gc.DataTuple('mgid', 1)],
                                 'SwitchIngress.arp_broadcast')])

        arp_pkt = build_arp_packet(
            "ff:ff:ff:ff:ff:ff", sender_mac, 1,
            sender_mac, sender_ip, "00:00:00:00:00:00", target_ip)

        logger.info("Sending ARP request on port %d", ig_port)
        testutils.send_packet(self, ig_port, arp_pkt)

        expected_ports = list(data_ports)
        testutils.verify_packets_any(self, arp_pkt, expected_ports)

        # Cleanup
        arp_table.entry_del(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0001),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', target_ip)])])
        cleanup_mc_group(bfrt_info, target, 1, node_ids)
    """@brief Test ARP reply unicast forwarding."""

    def setUp(self):
        BfRuntimeTest.setUp(self, 0, "tna_arp_route")

    def runTest(self):
        data_ports = get_data_ports()
        ig_port = data_ports[0]
        eg_port = data_ports[1]
        sender_mac = "00:aa:bb:cc:dd:ee"
        sender_ip = "10.0.0.2"
        target_mac = "00:11:22:33:44:55"
        target_ip = "10.0.0.1"

        bfrt_info = self.interface.bfrt_info_get("tna_arp_route")
        arp_table = bfrt_info.table_get("SwitchIngress.arp_table")
        arp_table.info.key_field_annotation_add("hdr.arp_ipv4.target_proto_addr", "ipv4")
        target = gc.Target(device_id=0, pipe_id=0xffff)

        clean_p4_tables(bfrt_info, target)

        arp_table.entry_add(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0002),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', target_ip)])],
            [arp_table.make_data([gc.DataTuple('port', eg_port)],
                                 'SwitchIngress.arp_reply_unicast')])

        arp_pkt = build_arp_packet(
            target_mac, sender_mac, 2,
            sender_mac, sender_ip, target_mac, target_ip)

        logger.info("Sending ARP reply on port %d, expecting on port %d", ig_port, eg_port)
        testutils.send_packet(self, ig_port, arp_pkt)
        testutils.verify_packet(self, arp_pkt, eg_port)

        arp_table.entry_del(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0002),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', target_ip)])])


class L2ForwardTest(BfRuntimeTest):
    """@brief Test L2 forwarding of regular IP packets based on learned MAC-port mappings.

    Simulates the learning process:
    1. Host A (MAC_A, IP_A) is on port 0
    2. Host B (MAC_B, IP_B) is on port 1
    3. Control plane "learns" by populating smac_table and dmac_table
    4. Send packet from A to B -> forwarded to port 1
    """

    def setUp(self):
        BfRuntimeTest.setUp(self, 0, "tna_arp_route")

    def runTest(self):
        data_ports = get_data_ports()
        port_a = data_ports[0]
        port_b = data_ports[1]
        mac_a = "00:11:22:33:44:01"
        ip_a = "10.0.0.1"
        mac_b = "00:11:22:33:44:02"
        ip_b = "10.0.0.2"

        bfrt_info = self.interface.bfrt_info_get("tna_arp_route")
        smac_table = bfrt_info.table_get("SwitchIngress.smac_table")
        smac_table.info.key_field_annotation_add("hdr.ethernet.src_addr", "mac")
        smac_table.info.key_field_annotation_add("hdr.ipv4.src_addr", "ipv4")
        dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")
        dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")
        target = gc.Target(device_id=0, pipe_id=0xffff)

        clean_p4_tables(bfrt_info, target)

        # "Learn" host A: MAC_A + IP_A -> port_a
        smac_table.entry_add(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac_a),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip_a)])],
            [smac_table.make_data([gc.DataTuple('port', port_a)],
                                  'SwitchIngress.learn')])

        # "Learn" host B: MAC_B + IP_B -> port_b
        smac_table.entry_add(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac_b),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip_b)])],
            [smac_table.make_data([gc.DataTuple('port', port_b)],
                                  'SwitchIngress.learn')])

        # Forward: dst MAC_A -> port_a
        dmac_table.entry_add(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_a)])],
            [dmac_table.make_data([gc.DataTuple('port', port_a)],
                                  'SwitchIngress.forward')])

        # Forward: dst MAC_B -> port_b
        dmac_table.entry_add(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_b)])],
            [dmac_table.make_data([gc.DataTuple('port', port_b)],
                                  'SwitchIngress.forward')])

        # Test: send packet A->B (dst MAC = MAC_B), expect on port_b
        pkt = testutils.simple_tcp_packet(
            eth_dst=mac_b, eth_src=mac_a,
            ip_dst=ip_b, ip_src=ip_a, ip_ttl=64)

        logger.info("Sending IP packet A->B on port %d, expecting on port %d", port_a, port_b)
        testutils.send_packet(self, port_a, pkt)
        testutils.verify_packet(self, pkt, port_b)

        # Test reverse: send packet B->A (dst MAC = MAC_A), expect on port_a
        pkt_rev = testutils.simple_tcp_packet(
            eth_dst=mac_a, eth_src=mac_b,
            ip_dst=ip_a, ip_src=ip_b, ip_ttl=64)

        logger.info("Sending IP packet B->A on port %d, expecting on port %d", port_b, port_a)
        testutils.send_packet(self, port_b, pkt_rev)
        testutils.verify_packet(self, pkt_rev, port_a)

        # Cleanup
        smac_table.entry_del(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac_a),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip_a)])])
        smac_table.entry_del(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac_b),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip_b)])])
        dmac_table.entry_del(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_a)])])
        dmac_table.entry_del(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_b)])])


class L2ForwardUnknownMacFloodTest(BfRuntimeTest):
    """@brief Test that packets to an unknown MAC are flooded via multicast."""

    def setUp(self):
        BfRuntimeTest.setUp(self, 0, "tna_arp_route")

    def runTest(self):
        data_ports = get_data_ports()
        ig_port = data_ports[0]
        mac_a = "00:11:22:33:44:01"
        ip_a = "10.0.0.1"
        mac_unknown = "00:99:88:77:66:55"

        bfrt_info = self.interface.bfrt_info_get("tna_arp_route")
        dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")
        dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")
        target = gc.Target(device_id=0, pipe_id=0xffff)

        # Clean up stale entries
        try:
            dmac_table.entry_del(target, [])
        except Exception:
            pass

        # Setup multicast group 2 for flooding
        node_ids = setup_mc_group(bfrt_info, target, 2, data_ports)

        # Unknown MAC -> flood
        dmac_table.entry_add(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_unknown)])],
            [dmac_table.make_data([gc.DataTuple('mgid', 2)],
                                  'SwitchIngress.flood')])

        pkt = testutils.simple_tcp_packet(
            eth_dst=mac_unknown, eth_src=mac_a,
            ip_dst="10.0.0.99", ip_src=ip_a, ip_ttl=64)

        logger.info("Sending packet to unknown MAC, expecting flood")
        testutils.send_packet(self, ig_port, pkt)

        expected_ports = list(data_ports)
        testutils.verify_packets_any(self, pkt, expected_ports)

        # Cleanup
        dmac_table.entry_del(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_unknown)])])
        cleanup_mc_group(bfrt_info, target, 2, node_ids)


class Ipv4RouteTest(BfRuntimeTest):
    """@brief Test IPv4 LPM routing with MAC rewrite and TTL decrement."""

    def setUp(self):
        BfRuntimeTest.setUp(self, 0, "tna_arp_route")

    def runTest(self):
        data_ports = get_data_ports()
        ig_port = data_ports[0]
        eg_port = data_ports[1]

        bfrt_info = self.interface.bfrt_info_get("tna_arp_route")
        route_table = bfrt_info.table_get("SwitchIngress.ipv4_route")
        route_table.info.key_field_annotation_add("hdr.ipv4.dst_addr", "ipv4")
        route_table.info.data_field_annotation_add("src_mac", "SwitchIngress.route", "mac")
        route_table.info.data_field_annotation_add("dst_mac", "SwitchIngress.route", "mac")
        target = gc.Target(device_id=0, pipe_id=0xffff)

        # Clean up stale entries
        try:
            route_table.entry_del(target, [])
        except:
            pass

        next_hop_mac = "00:aa:bb:cc:dd:01"
        router_mac = "00:00:00:00:00:01"

        # Route: 10.0.1.0/24 -> next_hop_mac, eg_port
        route_table.entry_add(
            target,
            [route_table.make_key([gc.KeyTuple('hdr.ipv4.dst_addr', "10.0.1.0", prefix_len=24)])],
            [route_table.make_data([gc.DataTuple('src_mac', router_mac),
                                    gc.DataTuple('dst_mac', next_hop_mac),
                                    gc.DataTuple('port', eg_port)],
                                   'SwitchIngress.route')])

        # Route: 10.0.0.0/16 -> same port (less specific)
        route_table.entry_add(
            target,
            [route_table.make_key([gc.KeyTuple('hdr.ipv4.dst_addr', "10.0.0.0", prefix_len=16)])],
            [route_table.make_data([gc.DataTuple('src_mac', router_mac),
                                    gc.DataTuple('dst_mac', "00:aa:bb:cc:dd:02"),
                                    gc.DataTuple('port', data_ports[2])],
                                   'SwitchIngress.route')])

        # Packet to 10.0.1.5 should match /24
        pkt = testutils.simple_tcp_packet(
            eth_dst=router_mac, eth_src="00:11:22:33:44:55",
            ip_dst="10.0.1.5", ip_src="192.168.1.1", ip_ttl=64)

        exp_pkt = testutils.simple_tcp_packet(
            eth_dst=next_hop_mac, eth_src=router_mac,
            ip_dst="10.0.1.5", ip_src="192.168.1.1", ip_ttl=63)

        logger.info("Sending IPv4 packet to 10.0.1.5 on port %d", ig_port)
        testutils.send_packet(self, ig_port, pkt)
        testutils.verify_packet(self, exp_pkt, eg_port)

        # Cleanup
        route_table.entry_del(target, [])

        # Cleanup dmac_table too (smac_table hit may have populated it via default)
        dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")
        dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")


class ArpAndL2ForwardCombinedTest(BfRuntimeTest):
    """@brief Combined test: ARP learning followed by L2 forwarding.

    1. Host A sends ARP request (broadcast)
    2. Host B sends ARP reply (unicast to A)
    3. Control plane learns MAC-IP-port bindings from ARP
    4. Host A sends IP packet to Host B -> L2 forwarded
    """

    def setUp(self):
        BfRuntimeTest.setUp(self, 0, "tna_arp_route")

    def runTest(self):
        data_ports = get_data_ports()
        port_a = data_ports[0]
        port_b = data_ports[1]
        mac_a = "00:11:22:33:44:01"
        ip_a = "10.0.0.1"
        mac_b = "00:11:22:33:44:02"
        ip_b = "10.0.0.2"

        bfrt_info = self.interface.bfrt_info_get("tna_arp_route")
        arp_table = bfrt_info.table_get("SwitchIngress.arp_table")
        arp_table.info.key_field_annotation_add("hdr.arp_ipv4.target_proto_addr", "ipv4")
        dmac_table = bfrt_info.table_get("SwitchIngress.dmac_table")
        dmac_table.info.key_field_annotation_add("hdr.ethernet.dst_addr", "mac")
        smac_table = bfrt_info.table_get("SwitchIngress.smac_table")
        smac_table.info.key_field_annotation_add("hdr.ethernet.src_addr", "mac")
        smac_table.info.key_field_annotation_add("hdr.ipv4.src_addr", "ipv4")
        target = gc.Target(device_id=0, pipe_id=0xffff)

        # Clean up stale entries from previous tests
        for tbl_name in ["SwitchIngress.arp_table", "SwitchIngress.smac_table",
                         "SwitchIngress.dmac_table"]:
            try:
                bfrt_info.table_get(tbl_name).entry_del(target, [])
            except Exception:
                pass

        # Setup broadcast group
        node_ids = setup_mc_group(bfrt_info, target, 1, data_ports)

        # Step 1: ARP request for ip_b -> broadcast
        arp_table.entry_add(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0001),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', ip_b)])],
            [arp_table.make_data([gc.DataTuple('mgid', 1)],
                                 'SwitchIngress.arp_broadcast')])

        # Step 2: ARP reply to ip_a -> unicast to port_a
        arp_table.entry_add(
            target,
            [arp_table.make_key([gc.KeyTuple('hdr.arp.opcode', 0x0002),
                                 gc.KeyTuple('hdr.arp_ipv4.target_proto_addr', ip_a)])],
            [arp_table.make_data([gc.DataTuple('port', port_a)],
                                 'SwitchIngress.arp_reply_unicast')])

        # Step 3: "Learn" bindings from ARP exchange
        smac_table.entry_add(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac_a),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip_a)])],
            [smac_table.make_data([gc.DataTuple('port', port_a)],
                                  'SwitchIngress.learn')])

        smac_table.entry_add(
            target,
            [smac_table.make_key([gc.KeyTuple('hdr.ethernet.src_addr', mac_b),
                                  gc.KeyTuple('hdr.ipv4.src_addr', ip_b)])],
            [smac_table.make_data([gc.DataTuple('port', port_b)],
                                  'SwitchIngress.learn')])

        dmac_table.entry_add(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_a)])],
            [dmac_table.make_data([gc.DataTuple('port', port_a)],
                                  'SwitchIngress.forward')])

        dmac_table.entry_add(
            target,
            [dmac_table.make_key([gc.KeyTuple('hdr.ethernet.dst_addr', mac_b)])],
            [dmac_table.make_data([gc.DataTuple('port', port_b)],
                                  'SwitchIngress.forward')])

        # --- Send ARP request from A (broadcast) ---
        arp_req = build_arp_packet(
            "ff:ff:ff:ff:ff:ff", mac_a, 1,
            mac_a, ip_a, "00:00:00:00:00:00", ip_b)

        logger.info("Step 1: ARP request from A (broadcast)")
        testutils.send_packet(self, port_a, arp_req)
        expected_ports = list(data_ports)
        testutils.verify_packets_any(self, arp_req, expected_ports)

        # --- Send ARP reply from B (unicast to A) ---
        arp_reply = build_arp_packet(
            mac_a, mac_b, 2,
            mac_b, ip_b, mac_a, ip_a)

        logger.info("Step 2: ARP reply from B (unicast to port %d)", port_a)
        testutils.send_packet(self, port_b, arp_reply)
        testutils.verify_packet(self, arp_reply, port_a)

        # --- Send IP packet A -> B (L2 forward) ---
        pkt = testutils.simple_tcp_packet(
            eth_dst=mac_b, eth_src=mac_a,
            ip_dst=ip_b, ip_src=ip_a, ip_ttl=64)

        logger.info("Step 3: IP packet A->B (L2 forward to port %d)", port_b)
        testutils.send_packet(self, port_a, pkt)
        testutils.verify_packet(self, pkt, port_b)

        # Cleanup
        arp_table.entry_del(target, [])
        smac_table.entry_del(target, [])
        dmac_table.entry_del(target, [])
        cleanup_mc_group(bfrt_info, target, 1, node_ids)
