#include <core.p4>
#if __TARGET_TOFINO__ == 3
#include <t3na.p4>
#elif __TARGET_TOFINO__ == 2
#include <t2na.p4>
#else
#include <tna.p4>
#endif

#include "common/headers.p4"
#include "common/util.p4"

// Extend the common header_t with ARP fields
header arp_ipv4_h {
    mac_addr_t   sender_hw_addr;
    ipv4_addr_t  sender_proto_addr;
    mac_addr_t   target_hw_addr;
    ipv4_addr_t  target_proto_addr;
}

struct local_header_t {
    ethernet_h   ethernet;
    ipv4_h       ipv4;
    arp_h        arp;
    arp_ipv4_h   arp_ipv4;
}

struct learn_digest_t {
    mac_addr_t  src_mac;
    ipv4_addr_t src_ip;
    PortId_t    ingress_port;
}

struct metadata_t {
    bit<1> routed;
    PortId_t ingress_port;
    ipv4_addr_t learn_src_ip;
}

// ---------------------------------------------------------------------------
// Ingress parser
// ---------------------------------------------------------------------------
parser SwitchIngressParser(
        packet_in pkt,
        out local_header_t hdr,
        out metadata_t ig_md,
        out ingress_intrinsic_metadata_t ig_intr_md) {

    TofinoIngressParser() tofino_parser;

    state start {
        tofino_parser.apply(pkt, ig_intr_md);
        transition parse_ethernet;
    }

    state parse_ethernet {
        pkt.extract(hdr.ethernet);
        transition select(hdr.ethernet.ether_type) {
            ETHERTYPE_IPV4 : parse_ipv4;
            ETHERTYPE_ARP  : parse_arp;
            default        : accept;
        }
    }

    state parse_ipv4 {
        pkt.extract(hdr.ipv4);
        transition accept;
    }

    state parse_arp {
        pkt.extract(hdr.arp);
        transition select(hdr.arp.opcode) {
            0x0001 : parse_arp_ipv4;
            0x0002 : parse_arp_ipv4;
            default : accept;
        }
    }

    state parse_arp_ipv4 {
        pkt.extract(hdr.arp_ipv4);
        transition accept;
    }
}

// ---------------------------------------------------------------------------
// Ingress Deparser
// ---------------------------------------------------------------------------
control SwitchIngressDeparser(
        packet_out pkt,
        inout local_header_t hdr,
        in metadata_t ig_md,
        in ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md) {

    Checksum() ipv4_checksum;
    Digest<learn_digest_t>() learn_digest;

    apply {
        hdr.ipv4.hdr_checksum = ipv4_checksum.update({
            hdr.ipv4.version,
            hdr.ipv4.ihl,
            hdr.ipv4.diffserv,
            hdr.ipv4.total_len,
            hdr.ipv4.identification,
            hdr.ipv4.flags,
            hdr.ipv4.frag_offset,
            hdr.ipv4.ttl,
            hdr.ipv4.protocol,
            hdr.ipv4.src_addr,
            hdr.ipv4.dst_addr});

        if (ig_dprsr_md.digest_type == 1) {
            learn_digest.pack({hdr.ethernet.src_addr,
                               ig_md.learn_src_ip,
                               ig_md.ingress_port});
        }

        pkt.emit(hdr.ethernet);
        pkt.emit(hdr.ipv4);
        pkt.emit(hdr.arp);
        pkt.emit(hdr.arp_ipv4);
    }
}

// ---------------------------------------------------------------------------
// Switch Ingress
// ---------------------------------------------------------------------------
control SwitchIngress(
        inout local_header_t hdr,
        inout metadata_t ig_md,
        in ingress_intrinsic_metadata_t ig_intr_md,
        in ingress_intrinsic_metadata_from_parser_t ig_prsr_md,
        inout ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md,
        inout ingress_intrinsic_metadata_for_tm_t ig_tm_md) {

    action nop() {}

    // ---- SMAC learning: record source MAC + source IP -> ingress port ----
    action learn(PortId_t port) {}

    table smac_table {
        key = {
            hdr.ethernet.src_addr : exact;
            hdr.ipv4.src_addr     : exact;
        }

        actions = {
            learn;
            @defaultonly nop;
        }

        const default_action = nop();
        size = 1024;
    }

    // ---- DMAC forwarding: forward based on learned dst MAC -> port ----
    action forward(PortId_t port) {
        ig_tm_md.ucast_egress_port = port;
    }

    action flood(MulticastGroupId_t mgid) {
        ig_tm_md.mcast_grp_a = mgid;
    }

    action drop() {
        ig_dprsr_md.drop_ctl = 0x1;
    }

    table dmac_table {
        key = {
            hdr.ethernet.dst_addr : exact;
        }

        actions = {
            forward;
            flood;
            @defaultonly drop;
        }

        const default_action = drop;
        size = 1024;
    }

    // ---- IPv4 routing (LPM) ----
    action route(mac_addr_t src_mac, mac_addr_t dst_mac, PortId_t port) {
        hdr.ethernet.src_addr = src_mac;
        hdr.ethernet.dst_addr = dst_mac;
        hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
        ig_tm_md.ucast_egress_port = port;
        ig_md.routed = 1;
    }

    table ipv4_route {
        key = {
            hdr.ipv4.dst_addr : lpm;
        }

        actions = {
            route;
            @defaultonly nop;
        }

        const default_action = nop;
        size = 1024;
    }

    apply {
        ig_md.ingress_port = ig_intr_md.ingress_port;

        if (hdr.arp.isValid()) {
            ig_dprsr_md.digest_type = 1;
            ig_md.learn_src_ip = hdr.arp_ipv4.sender_proto_addr;
            if (hdr.arp.opcode == 0x0001) {
                // ARP request → flood
                ig_tm_md.mcast_grp_a = 1;
            } else {
                // ARP reply → forward via DMAC lookup
                dmac_table.apply();
            }
        } else if (hdr.ipv4.isValid()) {
            ig_dprsr_md.digest_type = 1;
            ig_md.learn_src_ip = hdr.ipv4.src_addr;
            smac_table.apply();
            if (ipv4_route.apply().hit) {
                // routed: egress port already set by route action
            } else {
                dmac_table.apply();
            }
        } else {
            ig_dprsr_md.drop_ctl = 0x1;
        }

        ig_tm_md.bypass_egress = 1w1;
    }
}

Pipeline(SwitchIngressParser(),
         SwitchIngress(),
         SwitchIngressDeparser(),
         EmptyEgressParser(),
         EmptyEgress(),
         EmptyEgressDeparser()) pipe;

Switch(pipe) main;
