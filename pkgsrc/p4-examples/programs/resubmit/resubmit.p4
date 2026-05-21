/*******************************************************************************
 * INTEL CONFIDENTIAL
 *
 * Copyright (c) 2021 Intel Corporation
 * All Rights Reserved.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 ******************************************************************************/

/*
 * This sample program hightlights the use of resubmit on Tofino
 * Not tested on BM (v1 or v2).
 */

#ifdef __TARGET_TOFINO__
#include <tofino/intrinsic_metadata.p4>
#endif

header_type ethernet_t {
    fields {
        dstAddr : 48;
        srcAddr : 48;
        etherType : 16;
    }
}

header ethernet_t ethernet;

parser start {
    extract(ethernet);
    return ingress;
}


header_type test_metadata_t {
    fields {
        field_A: 8;
        field_B: 8;
        field_C: 16;
    }
}

metadata test_metadata_t test_metadata;

field_list resubmit_fields {
    test_metadata.field_A;
    test_metadata.field_C;
}

action nop() {

}

/*
 * Processing regular packet
 */

action do_resubmit_with_fields() {
    modify_field(test_metadata.field_C, 0x1234);
    resubmit(resubmit_fields);
}

action do_resubmit() {
#if defined(BMV2TOFINO)
    resubmit_no_fields();
#else
    resubmit();
#endif
}


table l2_resubmit {
    reads {
        ethernet.dstAddr: exact;
    }
    actions {
        nop;
        do_resubmit;
        do_resubmit_with_fields;
    }
    size : 512;
}

/*
 * Processing resubmitted packet
 */

action nhop_set(port) {
    modify_field(ig_intr_md_for_tm.ucast_egress_port, port);
}

action nhop_set_with_type(port) {
    modify_field(ethernet.etherType, test_metadata.field_C);
    modify_field(ig_intr_md_for_tm.ucast_egress_port, port);
}

table l2_nhop {
    reads {
        ethernet.dstAddr: exact;
    }
    actions {
        nop;
        nhop_set;
        nhop_set_with_type;
    }
    size : 512;
}

/* Main control flow */
control ingress {
    if (0 == ig_intr_md.resubmit_flag) {
        apply(l2_resubmit);
    } else {
        apply(l2_nhop);
    }
}

control egress {

}
