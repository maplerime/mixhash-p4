#if __TARGET_TOFINO__ == 3
#include <tofino3/intrinsic_metadata.p4>
#endif
#if __TARGET_TOFINO__ == 2
#include <tofino2/intrinsic_metadata.p4>
#endif
#if __TARGET_TOFINO__ == 1
#include <tofino/intrinsic_metadata.p4>
#endif
#include <tofino/constants.p4>

parser start {
    return ingress;
}

action set_md(eg_port) {
    modify_field(ig_intr_md_for_tm.ucast_egress_port, eg_port);
}

table port_tbl {
    reads {
        ig_intr_md.ingress_port : exact;
    }
    actions {
        set_md;
    }
    default_action: set_md(0);
    size : 288;
}


control ingress {
    if (0 == ig_intr_md.resubmit_flag) {
        apply(port_tbl);
    }
}

control egress {
}
