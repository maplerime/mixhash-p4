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


#include <core.p4>
#include <tna.p4>

#include "common/headers.p4"
#include "common/util.p4"

struct custom_md_t {
  bit<16> md;
}

// ---------------------------------------------------------------------------
// Ingress parser
// ---------------------------------------------------------------------------
parser SwitchIngressParser(
        packet_in pkt,
        out header_t hdr,
        out custom_md_t ig_md,
        out ingress_intrinsic_metadata_t ig_intr_md) {

    TofinoIngressParser() tofino_parser;

    state start {
        tofino_parser.apply(pkt, ig_intr_md);
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
        transition select(hdr.ipv4.protocol) {
            IP_PROTOCOLS_TCP : parse_tcp;
            default : accept;
        }
    }

    state parse_tcp {
        pkt.extract(hdr.tcp);
        transition accept;
    }
}

// ---------------------------------------------------------------------------
// Ingress Deparser
// ---------------------------------------------------------------------------
control SwitchIngressDeparser(
         packet_out pkt,
         inout header_t hdr,
         in custom_md_t md,
         in ingress_intrinsic_metadata_for_deparser_t dprsr_md) {
    apply {
      pkt.emit(hdr);
    }
}

control SwitchIngress(
        inout header_t hdr,
        inout custom_md_t ig_md,
        in ingress_intrinsic_metadata_t ig_intr_md,
        in ingress_intrinsic_metadata_from_parser_t ig_prsr_md,
        inout ingress_intrinsic_metadata_for_deparser_t ig_dprsr_md,
        inout ingress_intrinsic_metadata_for_tm_t ig_tm_md) {

    Hash<bit<64>>(HashAlgorithm_t.IDENTITY) sel_hash;
    Hash<bit<64>>(HashAlgorithm_t.CRC64)   c_hash;

    ActionProfile(2048) example_action_selector_ap;
    ActionSelector(example_action_selector_ap, // action profile
                   sel_hash, // hash extern
                   SelectorMode_t.FAIR, // Selector algorithm
                   100, // max group size
                   100 // max number of groups
                   ) example_action_selector;

    action hit(PortId_t port) {
        ig_tm_md.ucast_egress_port = port;
    }

    action miss() {
        ig_dprsr_md.drop_ctl = 0x1; // Drop packet.
    }

    table forward {
        key = {
            ig_intr_md.ingress_port : exact;
            hdr.ethernet.src_addr[31:0] : selector;
        }

        actions = {
            hit;
            miss;
        }

        const default_action = miss;
        size = 512;
        implementation = example_action_selector;
    }

    action apply_hash() {
        hdr.ethernet.src_addr[31:0] = c_hash.get(
           {hdr.ipv4.src_addr,
            hdr.ipv4.dst_addr,
            hdr.tcp.src_port,
            hdr.tcp.dst_port})[31:0];
    }

    table tbl_hash {
        actions = {
            apply_hash;
        }
        const default_action = apply_hash();
    }

    apply {
      tbl_hash.apply();
      forward.apply();

      // No need for egress processing, skip it and use empty controls for egress.
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
