/*************************************************************************
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
#include <tofino3/intrinsic_metadata.p4>
#elif __TARGET_TOFINO__ == 2
#include <tofino2/intrinsic_metadata.p4>
#else
#include <tofino/intrinsic_metadata.p4>
#endif
#include <tofino/constants.p4>

header_type ethernet_t {
  fields {
    dstAddr : 48;
    srcAddr : 48;
    etherType : 16;
  }
}
header ethernet_t ethernet;

parser start {
  return parse_ethernet;
}

parser parse_ethernet {
  extract(ethernet);
  return ingress;
}

header_type metadata_t {
  fields {
    do_ing_mirroring : 1;
    do_egr_mirroring : 1;
    ing_mir_ses : 10;
    egr_mir_ses : 10;
  }
}

metadata metadata_t md;

table p0 {
  reads   { ig_intr_md.ingress_port : exact; }
  actions { set_md; }
  default_action: set_md(0, 0, 0, 0, 0);
  size : 288;
}

action set_md(dest_port, ing_mir, ing_ses, egr_mir, egr_ses) {
  modify_field(ig_intr_md_for_tm.ucast_egress_port, dest_port);
  modify_field(md.do_ing_mirroring, ing_mir);
  modify_field(md.do_egr_mirroring, egr_mir);
  modify_field(md.ing_mir_ses, ing_ses);
  modify_field(md.egr_mir_ses, egr_ses);
}

table ing_mir {
  actions { do_ing_mir; }
  default_action : do_ing_mir;
  size : 1;
}
table egr_mir {
  actions { do_egr_mir; }
  default_action : do_egr_mir;
  size : 1;
}

//field_list no_fields {}

action do_ing_mir() {
  //clone_ingress_pkt_to_egress(md.ing_mir_ses, no_fields);
#if __TARGET_TOFINO__ == 2 ||  __TARGET_TOFINO__ == 3
  modify_field(ig_intr_md_for_mb.mirror_hash, 2);
  modify_field(ig_intr_md_for_mb.mirror_multicast_ctrl, 0);
  modify_field(ig_intr_md_for_mb.mirror_io_select, 0);
#endif
  clone_ingress_pkt_to_egress(md.ing_mir_ses);
}
action do_egr_mir() {
  //clone_egress_pkt_to_egress(md.egr_mir_ses, no_fields);
#if __TARGET_TOFINO__ == 2 ||  __TARGET_TOFINO__ == 3
  modify_field(eg_intr_md_for_mb.mirror_hash, 2);
  modify_field(eg_intr_md_for_mb.mirror_multicast_ctrl, 0);
  modify_field(eg_intr_md_for_mb.mirror_io_select, 1);
#endif
  clone_egress_pkt_to_egress(md.egr_mir_ses);
  modify_field(eg_intr_md_for_oport.drop_ctl, 1);
}

control ingress {
  if (0 == ig_intr_md.resubmit_flag) {
    apply(p0);
  }
  if (1 == md.do_ing_mirroring) {
    apply(ing_mir);
  }
}
control egress {
  if (1 == md.do_egr_mirroring) {
    apply(egr_mir);
  }
}
