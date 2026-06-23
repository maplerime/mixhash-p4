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

// Extend headers with ARP and L4 fields
header arp_ipv4_h {
    mac_addr_t   sender_hw_addr;
    ipv4_addr_t  sender_proto_addr;
    mac_addr_t   target_hw_addr;
    ipv4_addr_t  target_proto_addr;
}

// Reorder buffer resubmit digest (8 bytes = 64 bits)
const bit<3> REORDER_RESUBMIT_TYPE = 2;
const bit<8> REORDER_DIGEST_MAGIC = 8w0xFF;

header reorder_digest_h {
    bit<8>  type;
    bit<16> flow_hash;
    bit<16> seq;
    bit<8>  resubmit_count;
    bit<16> reserved;
}

struct local_header_t {
    ethernet_h       ethernet;
    ipv4_h           ipv4;
    arp_h            arp;
    arp_ipv4_h       arp_ipv4;
    tcp_h            tcp;
    udp_h            udp;
    reorder_digest_h reorder_digest;
}

struct learn_digest_t {
    mac_addr_t  src_mac;
    ipv4_addr_t src_ip;
    PortId_t    ingress_port;
}

struct metadata_t {
    bit<1>  routed;
    PortId_t ingress_port;
    ipv4_addr_t learn_src_ip;
    // L4 ports (for ECMP and LB hashing)
    bit<16> l4_src_port;
    bit<16> l4_dst_port;
    // ECMP
    bit<16> ecmp_group_id;
    bit<1>  ecmp_select;
    bit<16> ecmp_counter;
    // LB
    bit<1>  lb_hit;
    bit<16> lb_backend_index;
    // Reorder
    bit<1>  reorder_enabled;
}

// ---------------------------------------------------------------------------
// Ingress parser
// ---------------------------------------------------------------------------
parser SwitchIngressParser(
        packet_in pkt,
        out local_header_t hdr,
        out metadata_t ig_md,
        out ingress_intrinsic_metadata_t ig_intr_md) {

    state start {
        pkt.extract(ig_intr_md);
        transition select(ig_intr_md.resubmit_flag) {
            0 : parse_port_metadata;
            1 : parse_resubmit;
        }
    }

    state parse_port_metadata {
        pkt.advance(PORT_METADATA_SIZE);
        transition parse_ethernet;
    }

    state parse_resubmit {
        transition select(pkt.lookahead<bit<8>>()) {
            REORDER_DIGEST_MAGIC : parse_reorder_digest;
            default : parse_ethernet;
        }
    }

    state parse_reorder_digest {
        pkt.extract(hdr.reorder_digest);
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
        transition select(hdr.ipv4.protocol) {
            IP_PROTOCOLS_TCP : parse_tcp;
            IP_PROTOCOLS_UDP : parse_udp;
            default          : accept;
        }
    }

    state parse_tcp {
        pkt.extract(hdr.tcp);
        transition accept;
    }

    state parse_udp {
        pkt.extract(hdr.udp);
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
    Resubmit(REORDER_RESUBMIT_TYPE) reorder_resubmit;

    apply {
        if (ig_dprsr_md.resubmit_type == REORDER_RESUBMIT_TYPE) {
            reorder_resubmit.emit(hdr.reorder_digest);
        }

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
        pkt.emit(hdr.tcp);
        pkt.emit(hdr.udp);
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

    // ---- ECMP ActionSelector ----
    Hash<bit<16>>(HashAlgorithm_t.CRC16) ecmp_hash;
    ActionProfile(1024) ecmp_ap;
    ActionSelector(ecmp_ap, ecmp_hash, SelectorMode_t.FAIR, 128, 64) ecmp_selector;

    // ---- MixHash LB externs ----
    Hash<bit<32>>(HashAlgorithm_t.CRC32) flow_hash;
    Hash<bit<32>>(HashAlgorithm_t.CRC32) mix_hash;
    Register<bit<16>, bit<32>>(16384) flow_counter_reg;

    RegisterAction<bit<16>, bit<32>, bit<16>>(flow_counter_reg) counter_action = {
        void apply(inout bit<16> reg_val, out bit<16> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 1;
        }
    };

    // ---- Reorder buffer externs ----
    Hash<bit<16>>(HashAlgorithm_t.CRC16) reorder_hash;
    // Per-flow reorder control: [15:0]=expected, [23:16]=state
    // state: 0=uninit, 1=init
    Register<bit<32>, bit<16>>(4096) reorder_ctrl_reg;

    // RegisterAction for atomic increment (expected++, state=init)
    RegisterAction<bit<32>, bit<16>, bit<32>>(reorder_ctrl_reg) ctrl_inc = {
        void apply(inout bit<32> reg_val, out bit<32> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 32w0x00010001;
        }
    };

    // RegisterAction to undo incorrect increment
    RegisterAction<bit<32>, bit<16>, bit<32>>(reorder_ctrl_reg) ctrl_dec = {
        void apply(inout bit<32> reg_val, out bit<32> old_val) {
            old_val = reg_val;
            reg_val = reg_val - 32w0x00010001;
        }
    };

    // ---- Reorder enable (per ingress port) ----
    action enable_reorder() {
        ig_md.reorder_enabled = 1w1;
    }

    table reorder_enable_table {
        key = {
            ig_md.ingress_port : exact;
        }
        actions = {
            enable_reorder;
            @defaultonly nop;
        }
        const default_action = nop();
        size = 64;
    }

    // ---- SMAC learning ----
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

    // ---- DMAC forwarding ----
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

    action set_ecmp_group(bit<16> group_id) {
        ig_md.ecmp_group_id = group_id;
        ig_md.ecmp_select = 1w1;
    }

    table ipv4_route {
        key = {
            hdr.ipv4.dst_addr : lpm;
        }

        actions = {
            route;
            set_ecmp_group;
            @defaultonly nop;
        }

        const default_action = nop;
        size = 1024;
    }

    // ---- ECMP group table (ActionSelector) ----
    action set_nhop(mac_addr_t nhop_src_mac, mac_addr_t nhop_dst_mac, PortId_t nhop_port) {
        hdr.ethernet.src_addr = nhop_src_mac;
        hdr.ethernet.dst_addr = nhop_dst_mac;
        hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
        ig_tm_md.ucast_egress_port = nhop_port;
        ig_md.routed = 1;
    }

    table ecmp_group_table {
        key = {
            ig_md.ecmp_group_id : exact;
            hdr.ipv4.src_addr   : selector;
            hdr.ipv4.dst_addr   : selector;
            ig_md.l4_src_port   : selector;
            ig_md.l4_dst_port   : selector;
            ig_md.ecmp_counter  : selector;
        }

        actions = {
            set_nhop;
        }

        implementation = ecmp_selector;
        size = 64;
    }

    // ---- MixHash Load Balancer ----

    action lb_set_group() {
        ig_md.lb_hit = 1w1;
    }

    table lb_vip_table {
        key = {
            hdr.ipv4.dst_addr : exact;
        }

        actions = {
            lb_set_group;
            @defaultonly nop;
        }

        const default_action = nop;
        size = 256;
    }

    action lb_dnat(ipv4_addr_t be_ip, mac_addr_t be_mac, PortId_t be_port) {
        hdr.ipv4.dst_addr = be_ip;
        hdr.ethernet.dst_addr = be_mac;
        hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
        ig_tm_md.ucast_egress_port = be_port;
        ig_md.routed = 1;
    }

    table lb_backend_table {
        key = {
            ig_md.lb_backend_index : exact;
        }

        actions = {
            lb_dnat;
        }

        size = 4096;
    }

    apply {
        ig_md.ingress_port = ig_intr_md.ingress_port;
        ig_md.lb_hit = 1w0;
        ig_md.ecmp_select = 1w0;
        ig_md.routed = 1w0;
        ig_md.reorder_enabled = 1w0;

        if (hdr.arp.isValid()) {
            ig_dprsr_md.digest_type = 1;
            ig_md.learn_src_ip = hdr.arp_ipv4.sender_proto_addr;
            if (hdr.arp.opcode == 0x0001) {
                ig_tm_md.mcast_grp_a = 1;
            } else {
                dmac_table.apply();
            }
        } else if (hdr.ipv4.isValid()) {
            // Extract L4 ports for hashing
            if (hdr.tcp.isValid()) {
                ig_md.l4_src_port = hdr.tcp.src_port;
                ig_md.l4_dst_port = hdr.tcp.dst_port;
            } else if (hdr.udp.isValid()) {
                ig_md.l4_src_port = hdr.udp.src_port;
                ig_md.l4_dst_port = hdr.udp.dst_port;
            } else {
                ig_md.l4_src_port = 16w0;
                ig_md.l4_dst_port = 16w0;
            }

            // ================================================================
            // Reorder check
            // ================================================================
            bool is_resub = hdr.reorder_digest.isValid();
            bool do_reorder = is_resub;

            if (!is_resub) {
                reorder_enable_table.apply();
                if (ig_md.reorder_enabled == 1w1) {
                    do_reorder = true;
                }
            }

            if (do_reorder) {
                // ---- Reorder logic (inc always, undo if mismatch) ----
                bit<16> seq;
                bit<16> rhash;
                bit<8>  resub_count;

                if (is_resub) {
                    seq = hdr.reorder_digest.seq;
                    rhash = hdr.reorder_digest.flow_hash;
                    resub_count = hdr.reorder_digest.resubmit_count;
                } else {
                    seq = hdr.ipv4.identification;
                    rhash = reorder_hash.get({
                        hdr.ipv4.src_addr,
                        hdr.ipv4.dst_addr,
                        ig_md.l4_src_port,
                        ig_md.l4_dst_port
                    });
                    resub_count = 8w0;
                }

                // Atomic read + increment
                bit<32> old_ctrl = ctrl_inc.execute(rhash);
                bit<16> expected = old_ctrl[15:0];
                bit<8>  state    = old_ctrl[23:16];
                bool init = state != 8w0;

                if (!init) {
                    // First packet: increment was init, forward
                } else if (seq == expected) {
                    // In order: increment was correct, forward
                } else {
                    // Out of order or retry: undo increment
                    ctrl_dec.execute(rhash);

                    if (is_resub && resub_count >= 8w5) {
                        // Retry limit: drop
                        ig_md.routed = 1w1;
                        ig_dprsr_md.drop_ctl = 0x1;
                    } else {
                        // Resubmit
                        ig_md.routed = 1w1;
                        hdr.reorder_digest.type = REORDER_DIGEST_MAGIC;
                        hdr.reorder_digest.flow_hash = rhash;
                        hdr.reorder_digest.seq = seq;
                        if (is_resub) {
                            hdr.reorder_digest.resubmit_count =
                                (bit<8>)(resub_count + 8w1);
                        } else {
                            hdr.reorder_digest.resubmit_count = 8w0;
                        }
                        hdr.reorder_digest.reserved = 16w0;
                        ig_dprsr_md.resubmit_type = REORDER_RESUBMIT_TYPE;
                    }
                }

            } else {
                // ---- Sender mode ----
                ig_dprsr_md.digest_type = 1;
                ig_md.learn_src_ip = hdr.ipv4.src_addr;
                smac_table.apply();

                bit<32> fhash = flow_hash.get({
                    hdr.ipv4.src_addr,
                    hdr.ipv4.dst_addr,
                    ig_md.l4_src_port,
                    ig_md.l4_dst_port
                });
                bit<16> pkt_counter = counter_action.execute(fhash);
                hdr.ipv4.identification = pkt_counter;
                ig_md.ecmp_counter = (bit<16>)pkt_counter[2:0];

                lb_vip_table.apply();

                if (ig_md.lb_hit == 1w1) {
                    bit<32> mhash = mix_hash.get({
                        hdr.ipv4.src_addr,
                        hdr.ipv4.dst_addr,
                        ig_md.l4_src_port,
                        ig_md.l4_dst_port,
                        pkt_counter
                    });
                    ig_md.lb_backend_index = (bit<16>)mhash[15:0];
                    lb_backend_table.apply();
                } else {
                    ipv4_route.apply();
                    if (ig_md.ecmp_select == 1w1) {
                        ecmp_group_table.apply();
                    }
                }
            }

            // Unified L2 forwarding
            if (ig_md.routed == 1w0) {
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
