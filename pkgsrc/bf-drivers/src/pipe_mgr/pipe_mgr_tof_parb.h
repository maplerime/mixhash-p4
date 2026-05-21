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

#ifndef __PIPE_MGR_TOF_PARB_H__
#define __PIPE_MGR_TOF_PARB_H__

#include <pipe_mgr/pipe_mgr_intf.h>

pipe_status_t pipe_mgr_tof_parb_set_port_ingress_chnl_control(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_tof_parb_set_port_egress_chnl_control(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_tof_parb_enable_port_arb_priority_high(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_tof_parb_enable_port_arb_priority_normal(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_tof_parb_disable_chnl_control(rmt_dev_info_t *dev_info,
                                                     bf_dev_port_t port_id);
pipe_status_t pipe_mgr_tof_parb_init(pipe_sess_hdl_t sess_hdl,
                                     rmt_dev_info_t *dev_info);
#endif
