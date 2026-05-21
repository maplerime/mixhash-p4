/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2021 Intel Corporation
 *  All Rights Reserved.
 *
 *  This software and the related documents are Intel copyrighted materials,
 *  and your use of them is governed by the express license under which they
 *  were provided to you ("License"). Unless the License provides otherwise,
 *  you may not use, modify, copy, publish, distribute, disclose or transmit
 *  this software or the related documents without Intel's prior written
 *  permission.
 *
 *  This software and the related documents are provided as is, with no express
 *  or implied warranties, other than those that are expressly stated in the
 *  License.
 ******************************************************************************/


#if __TARGET_TOFINO__ == 3
#include <t3na.p4>
#else
#include <t2na.p4>
#endif

#include "common/headers.p4"
#include "common/util.p4"


struct metadata_t {}

// ---------------------------------------------------------------------------
// Ingress parser
// ---------------------------------------------------------------------------
parser SwitchIngressParser(
        packet_in pkt,
        out empty_header_t hdr,
        out metadata_t ig_md,
        out ingress_intrinsic_metadata_t ig_intr_md) {

    TofinoIngressParser() tofino_parser;

    state start {
        tofino_parser.apply(pkt, ig_intr_md);
        transition accept;
    }
}

// ---------------------------------------------------------------------------
// Ingress Deparser
// ---------------------------------------------------------------------------
control SwitchIngressDeparser(
        packet_out pkt,
        inout empty_header_t hdr,
        in metadata_t ig_md,
        in ingress_intrinsic_metadata_for_deparser_t ig_intr_dprsr_md) {
    apply { }
}

control SwitchIngress(
        inout empty_header_t hdr,
        inout metadata_t ig_md,
        in ingress_intrinsic_metadata_t ig_intr_md,
        in ingress_intrinsic_metadata_from_parser_t ig_intr_prsr_md,
        inout ingress_intrinsic_metadata_for_deparser_t ig_intr_dprsr_md,
        inout ingress_intrinsic_metadata_for_tm_t ig_intr_tm_md) {
    apply {
        ig_intr_tm_md.ucast_egress_port = ig_intr_md.ingress_port;
    }
}

// ---------------------------------------------------------------------------
// Egress parser
// ---------------------------------------------------------------------------
parser SwitchEgressParser(
        packet_in pkt,
        out header_t hdr,
        out empty_metadata_t eg_md,
        out egress_intrinsic_metadata_t eg_intr_md) {

    TofinoEgressParser() tofino_parser;

    state start {
        tofino_parser.apply(pkt, eg_intr_md);
        transition parse_ethernet;
    }

    state parse_ethernet {
        pkt.extract(hdr.ethernet);
        transition select (hdr.ethernet.ether_type) {
            ETHERTYPE_IPV4 : parse_ipv4;
            default : reject;
        }
    }

    state parse_ipv4 {
        pkt.extract(hdr.ipv4);
        transition accept;
    }
}


// ---------------------------------------------------------------------------
// Egress Deparser
// ---------------------------------------------------------------------------
control SwitchEgressDeparser(
        packet_out pkt,
        inout header_t hdr,
        in empty_metadata_t eg_md,
        in egress_intrinsic_metadata_for_deparser_t ig_intr_dprs_md) {
    apply {
        pkt.emit(hdr);
    }
}

control SwitchEgress(
        inout header_t hdr,
        inout empty_metadata_t eg_md,
        in egress_intrinsic_metadata_t eg_intr_md,
        in egress_intrinsic_metadata_from_parser_t eg_intr_md_from_prsr,
        inout egress_intrinsic_metadata_for_deparser_t ig_intr_dprs_md,
        inout egress_intrinsic_metadata_for_output_port_t eg_intr_oport_md) {

    // Create direct counters
    DirectCounter<bit<32>>(CounterType_t.PACKETS_AND_BYTES, true) direct_counter_teop;
    DirectCounter<bit<32>>(CounterType_t.PACKETS_AND_BYTES, true) direct_counter_teop2;
    Counter<bit<32>, bit<10>>(1024, CounterType_t.PACKETS_AND_BYTES, true) indirect_counter_teop;
    Counter<bit<32>, bit<10>>(1024, CounterType_t.PACKETS_AND_BYTES, true) indirect_counter_teop2;

    DirectCounter<bit<32>>(CounterType_t.PACKETS_AND_BYTES) direct_counter;

    action nop() { }

    action hit_src() {
        // Call direct counter. Note that no index parameter is required because
        // the index is implicitely generated from the entry of the the
        // associated match table.
        direct_counter.count();
    }

    table count_src {
        key = {
            hdr.ethernet.src_addr : ternary;
        }

        actions = {
            hit_src;
            @defaultonly nop;
        }

        size = 1024;
        const default_action = nop;
        // Associate this table with a direct counter
        counters = direct_counter;
    }

    action hit_src_teop() {
        hdr.ipv4.setInvalid();
        hdr.ethernet.ether_type = 0xf;
        direct_counter_teop.count();
    }

    table count_src_teop {
        key = {
            hdr.ethernet.src_addr : ternary;
        }

        actions = {
            hit_src_teop;
            @defaultonly nop;
        }

        size = 1024;
        const default_action = nop;
        // Associate this table with a direct counter
        counters = direct_counter_teop;
    }

    action hit_src_teop2() {
        hdr.ethernet.ether_type = hdr.ethernet.ether_type + 0xf0;
        direct_counter_teop2.count();
    }

    table count_src_teop2 {
        key = {
            hdr.ethernet.src_addr : ternary;
        }

        actions = {
            hit_src_teop2;
            @defaultonly nop;
        }

        size = 1024;
        const default_action = nop;
        // Associate this table with a direct counter
        counters = direct_counter_teop2;
    }

    action hit_src_teop3(bit<10> index) {
        hdr.ethernet.ether_type = hdr.ethernet.ether_type + 0xf00;
        indirect_counter_teop.count(index);
    }

    table count_src_teop3 {
        key = {
            hdr.ethernet.src_addr : ternary;
        }

        actions = {
            hit_src_teop3;
            @defaultonly nop;
        }

        size = 1024;
        const default_action = nop;
    }

    action hit_src_teop4(bit<10> index) {
        hdr.ethernet.ether_type = hdr.ethernet.ether_type + 0xf000;
        indirect_counter_teop2.count(index);
    }

    table count_src_teop4 {
        key = {
            hdr.ethernet.src_addr : ternary;
        }

        actions = {
            hit_src_teop4;
            @defaultonly nop;
        }

        size = 1024;
        const default_action = nop;
    }

    apply {
        count_src.apply();
        count_src_teop.apply();
        count_src_teop2.apply();
        count_src_teop3.apply();
        count_src_teop4.apply();
    }
}

Pipeline(SwitchIngressParser(),
         SwitchIngress(),
         SwitchIngressDeparser(),
         SwitchEgressParser(),
         SwitchEgress(),
         SwitchEgressDeparser()) pipe;

Switch(pipe) main;
