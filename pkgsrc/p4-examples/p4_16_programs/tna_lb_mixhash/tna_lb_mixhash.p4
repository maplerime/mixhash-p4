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

// Reorder buffer recirculation header (carried in front of the packet).
// Unlike resubmit, recirculation preserves header edits and can loop many
// times, so the digest travels as an ordinary header detected by its magic.
const bit<16>  REORDER_MAGIC     = 16w0xBF01;
const bit<8>   REORDER_MAX_LOOPS = 8w16;
// All reorder state lives in ONE pipe's register copy (registers are
// per-pipe on Tofino). Packets that arrive on another pipe are funneled to
// this "owner" pipe first. For a single-pipe deployment owner pipe == the
// only pipe, so the funnel branch is never taken (zero overhead).
const bit<2>   REORDER_OWNER_PIPE = 2w0;
// Recirc port = DP-local port 68 of the owner pipe: (owner_pipe << 7) | 68.
// dev_port is bit<9> = {pipe[8:7], local_port[6:0]}; (0<<7)|68 = 68.
const PortId_t REORDER_RECIRC_PORT = 9w68;

header reorder_digest_h {
    bit<16> magic;         // = REORDER_MAGIC
    bit<16> flow_hash;
    bit<16> seq;
    bit<8>  recirc_count;
    bit<8>  reserved;
}

// RoCEv2 Base Transport Header (BTH, 12 bytes, follows UDP dst port 4791).
// ICRC covers the whole BTH (including PSN), so switches may only READ these
// fields — writing PSN breaks ICRC at the receiver exactly like the IP-ID
// rewrite (see mixhash-ipid-breaks-roce). PSN is the sender's per-packet
// sequence: rxe increments it once per packet AT THE SOURCE, and it travels
// hop-by-hop in-band. So reading PSN is the ICRC-safe way to give MixHash a
// per-packet value that is already "incremented only at the source".
header roce_bth_h {
    bit<8>  opcode;           // byte 0
    bit<8>  se_m_pad_tver;    // byte 1: SE | M | PadCount | TVer
    bit<16> pkey;             // bytes 2-3
    bit<8>  reserved8;        // byte 4
    bit<24> dest_qp;          // bytes 5-7
    bit<8>  a_reserved7;      // byte 8: AcknowledgeReq | Reserved
    bit<24> psn;              // bytes 9-11
}

struct local_header_t {
    ethernet_h       ethernet;
    ipv4_h           ipv4;
    arp_h            arp;
    arp_ipv4_h       arp_ipv4;
    tcp_h            tcp;
    udp_h            udp;
    roce_bth_h       roce_bth;
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
    // MixHash per-flow counter register index. The raw Hash.get() value must
    // NOT be passed straight to RegisterAction.execute(): that compiles to a
    // hash-addressed stateful ALU (ExecuteStatefulAluFromHash), whose address
    // distribution tofino-model rejects (vpn_range_check DISCARDING), so the
    // register would never update. Masking the hash into this metadata field
    // makes the SALU PHV-addressed, which works on both model and hardware.
    bit<14> flow_reg_idx;
    // Reorder register index: the CRC16 reorder hash masked to 12 bits (4096
    // entries = 2^12), routed through metadata for the same PHV-addressed SALU
    // reason as flow_reg_idx above.
    bit<12> reorder_reg_idx;
    // Spine dispersion log index: src/dst last octets of the 10.100.1.x ->
    // 10.100.2.x cross-leaf flows, concatenated then masked to 12 bits, routed
    // through metadata for the same PHV-addressed SALU reason.
    bit<12> disp_idx;
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
        pkt.advance(PORT_METADATA_SIZE);
        // Recirculated reorder packets arrive on the recirc port AND carry the
        // digest header (magic) in front; fresh packets start with Ethernet.
        transition select(ig_intr_md.ingress_port, pkt.lookahead<bit<16>>()) {
            (REORDER_RECIRC_PORT, REORDER_MAGIC) : parse_reorder_digest;
            default                              : parse_ethernet;
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
        transition select(hdr.udp.dst_port) {
            16w4791 : parse_roce_bth;   // RoCEv2
            default : accept;
        }
    }

    state parse_roce_bth {
        pkt.extract(hdr.roce_bth);
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

        // Digest header rides in front only when valid (recirculating);
        // it is stripped before the packet leaves to its final port.
        pkt.emit(hdr.reorder_digest);
        pkt.emit(hdr.ethernet);
        pkt.emit(hdr.ipv4);
        pkt.emit(hdr.tcp);
        pkt.emit(hdr.udp);
        pkt.emit(hdr.roce_bth);
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
    // Index width bit<14> (16384 entries): a bit<32> index forces the hash-
    // addressed stateful ALU path, which tofino-model discards (see note at
    // ig_md.flow_reg_idx).
    Register<bit<16>, bit<14>>(16384) flow_counter_reg;

    RegisterAction<bit<16>, bit<14>, bit<16>>(flow_counter_reg) counter_action = {
        void apply(inout bit<16> reg_val, out bit<16> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 1;
        }
    };

    // ---- Reorder buffer externs ----
    Hash<bit<16>>(HashAlgorithm_t.CRC16) reorder_hash;
    // Per-flow reorder state, split across TWO registers because Tofino-1
    // cannot pair a plain read and a plain write on the same register in one
    // pass ("non-mutually exclusive"), and RegisterAction has no PHV input so
    // it cannot seed a data value. The data-dependent seed (expected = seq+1
    // on the first packet) needs a plain write; the running advance needs an
    // atomic RegisterAction. The two live on different registers so their
    // accesses are disjoint branches:
    //
    //   reorder_seen_reg     : >0 once the flow has been seen (0 = unseen).
    //   reorder_expected_reg : next in-order PSN[15:0]; seeded seq+1, then +1
    //                          per accepted packet.
    //
    // The branch conditions each depend on a SINGLE stateful result (seen, or
    // old expected). A branch depending on both a plain-read result AND an
    // action result (e.g. seq == base + count) is rejected by the backend as
    // "condition expression too complex".
    //
    // 4096 entries = 2^12, so the index type is bit<12>: a wider index forces
    // a hash-addressed stateful ALU that tofino-model discards (see the
    // flow_reg_idx note in metadata_t).
    Register<bit<16>, bit<12>>(4096) reorder_seen_reg;
    Register<bit<16>, bit<12>>(4096) reorder_expected_reg;

    RegisterAction<bit<16>, bit<12>, bit<16>>(reorder_seen_reg) seen_inc = {
        void apply(inout bit<16> reg_val, out bit<16> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 16w1;
        }
    };

    RegisterAction<bit<16>, bit<12>, bit<16>>(reorder_expected_reg) expected_inc = {
        void apply(inout bit<16> reg_val, out bit<16> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 16w1;
        }
    };

    RegisterAction<bit<16>, bit<12>, bit<16>>(reorder_expected_reg) expected_dec = {
        void apply(inout bit<16> reg_val, out bit<16> old_val) {
            old_val = reg_val;
            reg_val = reg_val - 16w1;
        }
    };

    // Seed the expected value for the first packet of a flow. The seed must be a
    // CONSTANT so it compiles to constant SALU arithmetic ("add lo, lo, 1"):
    // tofino-model's stateful-ALU BlackBox does not execute PHV-operand SALU
    // instructions ("add lo, phv_lo, 1"), which the compiler emits when the body
    // reads ig_md.reorder_seq. A fresh reorder_expected_reg slot reads 0, so this
    // increment seeds expected=1; the demo therefore starts each flow at PSN 0.
    RegisterAction<bit<16>, bit<12>, bit<16>>(reorder_expected_reg) seed_expected = {
        void apply(inout bit<16> reg_val, out bit<16> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 16w1;
        }
    };

    // ---- Spine dispersion log (per ECMP member, per flow) ----
    // Counts packets forwarded via each ECMP member (device port 6 = spine1,
    // 7 = spine2) per cross-leaf flow, indexed by
    // ((src_octet ++ dst_octet) & 0xFFF) — e.g. h1->h9 (10.100.1.11 ->
    // 10.100.2.21) lands on 0xB15. Read from the control plane after a test
    // to measure per-flow spine dispersion and aggregate balance. Index via
    // metadata (PHV-addressed SALU — see flow_reg_idx note).
    Register<bit<32>, bit<12>>(4096) disp_sp1_reg;
    Register<bit<32>, bit<12>>(4096) disp_sp2_reg;

    RegisterAction<bit<32>, bit<12>, bit<32>>(disp_sp1_reg) disp_sp1_inc = {
        void apply(inout bit<32> reg_val, out bit<32> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 32w1;
        }
    };

    RegisterAction<bit<32>, bit<12>, bit<32>>(disp_sp2_reg) disp_sp2_inc = {
        void apply(inout bit<32> reg_val, out bit<32> old_val) {
            old_val = reg_val;
            reg_val = reg_val + 32w1;
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
#ifdef CLASSIC_ECMP
            // 基线模式: 逐流静态哈希, 不含逐包变化的字段
#else
            // Per-packet inputs MUST be header fields, not metadata written
            // earlier in the same ingress pass: the tofino-model ActionSelector
            // hashes only PV (header) operands reliably — ig_md.ecmp_counter
            // written just before apply() reads as stale/zero in the selector
            // hash, pinning every RoCE packet of a flow to one member.
            // RoCE: BTH PSN is the sender's per-packet sequence (ICRC-safe to
            // read, never written). Invalid when not RoCE → contributes 0.
            hdr.roce_bth.psn[15:0]  : selector;
            // Non-RoCE: sender mode stamps ipv4.identification with the
            // per-flow packet counter just above (a header write, hashed fine).
            hdr.ipv4.identification : selector;
#endif
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
            bool is_recirc = hdr.reorder_digest.isValid();
            bool do_reorder = is_recirc;

            if (!is_recirc) {
                reorder_enable_table.apply();
                if (ig_md.reorder_enabled == 1w1) {
                    do_reorder = true;
                }
            }

            if (do_reorder) {
                // ---- Reorder logic (inc always, undo if mismatch) ----
                bit<16> seq;
                bit<16> rhash;
                bit<8>  recirc_count;

                if (is_recirc) {
                    seq = hdr.reorder_digest.seq;
                    rhash = hdr.reorder_digest.flow_hash;
                    recirc_count = hdr.reorder_digest.recirc_count;
                } else {
                    // The per-packet sequence number is exactly what sender
                    // mode loads into ig_md.ecmp_counter: RoCE carries it in
                    // the (ICRC-covered, never-rewritten) BTH PSN — the IPv4
                    // identification field is left at 0 for RoCE, so reading
                    // it here would make every RoCE packet look identical.
                    // Non-RoCE flows keep using the per-flow packet counter
                    // that MixHash wrote into hdr.ipv4.identification.
                    if (hdr.udp.isValid() &&
                        (hdr.udp.dst_port == 16w4791)) {
                        seq = hdr.roce_bth.psn[15:0];
                    } else {
                        seq = hdr.ipv4.identification;
                    }
                    rhash = reorder_hash.get({
                        hdr.ipv4.src_addr,
                        hdr.ipv4.dst_addr,
                        ig_md.l4_src_port,
                        ig_md.l4_dst_port
                    });
                    recirc_count = 8w0;
                }

                // Narrow the CRC16 reorder hash to the 12-bit register index
                // and carry it in metadata so the register access is PHV-
                // addressed (not hash-addressed) — see the flow_reg_idx note.
                ig_md.reorder_reg_idx = (bit<12>)(rhash & 16w0x0FFF);

                // All reorder state lives in the owner pipe's register copy.
                // A fresh packet that landed on another pipe must NOT touch the
                // local (wrong) copy — funnel it to the owner pipe first, with
                // flow_hash/seq carried in the digest so no recompute is needed.
                // is_recirc packets are already on the owner pipe (they came
                // back through its recirc port). On a single-pipe deployment
                // owner_pipe == ingress pipe, so this branch is never taken.
                bool in_owner_pipe =
                    (ig_intr_md.ingress_port[8:7] == REORDER_OWNER_PIPE);

                if (!is_recirc && !in_owner_pipe) {
                    // Funnel to owner pipe (no register access on this pipe).
                    ig_md.routed = 1w1;
                    hdr.reorder_digest.setValid();
                    hdr.reorder_digest.magic = REORDER_MAGIC;
                    hdr.reorder_digest.flow_hash = rhash;
                    hdr.reorder_digest.seq = seq;
                    hdr.reorder_digest.recirc_count = 8w0;
                    hdr.reorder_digest.reserved = 8w0;
                    ig_tm_md.ucast_egress_port = REORDER_RECIRC_PORT;
                } else {
                    // On the owner pipe: seed (first packet) / compare /
                    // undo (out-of-order). `seen` is 0 only for the first
                    // packet of a flow, which seeds expected = seq+1 (a random
                    // RoCE PSN start then reads as in-order) and is accepted
                    // unconditionally. Later packets speculatively advance
                    // expected and undo on a mismatch. Each branch condition
                    // depends on a single stateful result (seen, or the old
                    // expected value).
                    bit<16> seen = seen_inc.execute(ig_md.reorder_reg_idx);

                    if (seen == 16w0) {
                        // First packet for this flow: seed expected = seq + 1.
                        // Do it through a RegisterAction so the write lands on
                        // the stateful-ALU path, atomically with expected_inc /
                        // expected_dec below (a plain register.write() would be
                        // applied at a different pipeline time and corrupt the
                        // running expected value on tofino-model).
                        seed_expected.execute(ig_md.reorder_reg_idx);
                        hdr.reorder_digest.setInvalid();
                    } else {
                        bit<16> old_expected =
                            expected_inc.execute(ig_md.reorder_reg_idx);
                        if (seq == old_expected) {
                            // In order: expected already advanced, strip digest.
                            hdr.reorder_digest.setInvalid();
                        } else {
                            // Out of order: undo the speculative advance.
                            expected_dec.execute(ig_md.reorder_reg_idx);
                            if (recirc_count >= REORDER_MAX_LOOPS) {
                                // Loop limit reached: give up and drop.
                                ig_md.routed = 1w1;
                                hdr.reorder_digest.setInvalid();
                                ig_dprsr_md.drop_ctl = 0x1;
                            } else {
                                // Recirculate: (re)attach digest and loop via port.
                                ig_md.routed = 1w1;
                                hdr.reorder_digest.setValid();
                                hdr.reorder_digest.magic = REORDER_MAGIC;
                                hdr.reorder_digest.flow_hash = rhash;
                                hdr.reorder_digest.seq = seq;
                                hdr.reorder_digest.recirc_count =
                                    (bit<8>)(recirc_count + 8w1);
                                hdr.reorder_digest.reserved = 8w0;
                                ig_tm_md.ucast_egress_port = REORDER_RECIRC_PORT;
                            }
                        }
                    }
                }

            } else {
                // ---- Sender mode ----
                ig_dprsr_md.digest_type = 1;
                ig_md.learn_src_ip = hdr.ipv4.src_addr;
                smac_table.apply();

                // RoCEv2 (UDP dst port 4791) validates ICRC over the BTH
                // (including PSN) and the invariant IP header — so we must
                // NOT rewrite IP identification or PSN. But the BTH PSN is
                // already the sender's per-packet sequence (rxe increments it
                // once per packet at the source, and it travels hop-by-hop),
                // so we READ it into ig_md.ecmp_counter — the one per-packet
                // hash input for ECMP/LB path selection at every hop — instead
                // of rewriting any field: per-packet MixHash without touching a
                // single ICRC-covered bit. With a single-member group this is
                // a no-op; with a multi-member group it sprays per-packet and
                // therefore REQUIRES the destination-side PSN reorder buffer.
                bool is_roce = hdr.udp.isValid() &&
                    (hdr.udp.dst_port == 16w4791);

                if (is_roce) {
                    ig_md.ecmp_counter = hdr.roce_bth.psn[15:0];
                } else {
                    bit<32> fhash = flow_hash.get({
                        hdr.ipv4.src_addr,
                        hdr.ipv4.dst_addr,
                        ig_md.l4_src_port,
                        ig_md.l4_dst_port
                    });
                    // Mask to the 16384-entry register size and go through
                    // metadata so the stateful ALU is PHV-addressed (see note
                    // in metadata_t) — hash-addressed execute is discarded by
                    // the tofino-model address distribution.
                    ig_md.flow_reg_idx = (bit<14>)(fhash & 0x3FFF);
                    bit<16> pkt_counter =
                        counter_action.execute(ig_md.flow_reg_idx);
                    hdr.ipv4.identification = pkt_counter;
                    ig_md.ecmp_counter = pkt_counter;
                }

                // Unified per-packet MixHash path: ECMP/LB hashing is driven
                // by ig_md.ecmp_counter at every hop (for RoCE that value is
                // the in-band BTH PSN, which travels hop-by-hop unchanged).
                lb_vip_table.apply();

                if (ig_md.lb_hit == 1w1) {
                    bit<32> mhash = mix_hash.get({
                        hdr.ipv4.src_addr,
                        hdr.ipv4.dst_addr,
                        ig_md.l4_src_port,
                        ig_md.l4_dst_port,
                        // header PVs, not metadata — same model hazard as the
                        // ecmp selector key (see ecmp_group_table comment)
                        hdr.roce_bth.psn[15:0],
                        hdr.ipv4.identification
                    });
                    ig_md.lb_backend_index = (bit<16>)mhash[15:0];
                    lb_backend_table.apply();
                } else {
                    ipv4_route.apply();
                    if (ig_md.ecmp_select == 1w1) {
                        if (ecmp_group_table.apply().hit) {
                            // Spine dispersion log: count this packet on the
                            // member (spine) the selector just chose. Flow
                            // index = (src_last_octet ++ dst_last_octet) &
                            // 0xFFF — pure concat+mask, same SALU complexity
                            // class as flow_reg_idx. The branch reads only PHV
                            // (egress port), never a stateful result, so it
                            // fits Tofino-1's dependency rules.
                            ig_md.disp_idx = (bit<12>)(
                                (hdr.ipv4.src_addr[7:0] ++
                                 hdr.ipv4.dst_addr[7:0]) & 16w0x0FFF);
                            if (ig_tm_md.ucast_egress_port == 6) {
                                disp_sp1_inc.execute(ig_md.disp_idx);
                            } else {
                                disp_sp2_inc.execute(ig_md.disp_idx);
                            }
                        }
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
