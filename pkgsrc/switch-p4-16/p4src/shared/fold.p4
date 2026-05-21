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


// --------------------------------------------------------------------------------------------------------
// Fold - Use below table to 2-fold, 4-fold or arbitary-fold switch pipeline
//
// Pipeline folding involves sending packets from
// ingress pipeline X -> egress pipeline Y -> ingress pipeline Y -> egress pipeline Z ..
//
// To achieve above forwarding behavior two things are needed
// 1. Ability to send Packet from Port x in ingress pipeline X to Port y in egress pipeline Y
// 2. Ability to loopback packet on egress pipeline Y port y to ingress pipeline Y port y
//
// The below table helps achievethe first of these two tasks.
// Specifically it allows for below behavior
// fold_4_pipe:
// ===========
// On a 4-pipe system 1:1 (same port on ingress/egress pipes) forwarding across pipe <N=X> -> Pipe <N=X+1>,
// where N > 0 and N < 4. The value of N wraps around so N goes from 0,1,2,3 -> 0
//
// fold_2_pipe:
// ===========
// On a 2-pipe system 1:1 forwarding across pipe <N=X> -> Pipe <N=X+1>,
// where N > 0 and N < 2. The value of N wraps around so N goes from 0,1 -> 0
//
// set_egress_port: Arbitary/User defined forwarding/folding
// ===============
// Match on any incoming ingress port and send packet to user defined egress Pipe/Port
//
// The Fold table should be added at the end of Switching Ingress Pipeline to override port forwarding
// ---------------------------------------------------------------------------------------------------------

#define FOLD                                                                                \
    action fold_4_pipe() {                                                                  \
        ig_intr_md_for_tm.ucast_egress_port = (ig_intr_md.ingress_port + 0x80);             \
    }                                                                                       \
                                                                                            \
    action fold_2_pipe() {                                                                  \
        ig_intr_md_for_tm.ucast_egress_port = (ig_intr_md.ingress_port ^ 0x80);             \
    }                                                                                       \
                                                                                            \
    action set_egress_port(switch_port_t dev_port) {                                        \
        ig_intr_md_for_tm.ucast_egress_port = dev_port;                                     \
    }                                                                                       \
                                                                                            \
    table fold {                                                                            \
        key = { ig_intr_md.ingress_port : exact; }                                          \
        actions = {                                                                         \
                    fold_2_pipe;                                                            \
                    fold_4_pipe;                                                            \
                    set_egress_port;                                                        \
                    }                                                                       \
        size = MIN_TABLE_SIZE;                                                              \
        default_action = fold_4_pipe;                                                       \
    }
