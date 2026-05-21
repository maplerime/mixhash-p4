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

#ifndef __PIPE_MGR_TOF_EBUF_H__
#define __PIPE_MGR_TOF_EBUF_H__

#include <pipe_mgr/pipe_mgr_intf.h>

#define BF_TOF_EBUF_DISPATCH_FIFO_NUM_ENTRIES 24

pipe_status_t pipe_mgr_ebuf_tof_dev_add(rmt_dev_info_t *dev_info);
void pipe_mgr_ebuf_tof_dev_rmv(rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ebuf_tof_set_port_cut_through(rmt_dev_info_t *dev_info,
                                                     bf_dev_port_t port,
                                                     bool cut_through_enabled);
pipe_status_t pipe_mgr_ebuf_tof_set_port_chnl_ctrl(rmt_dev_info_t *dev_info,
                                                   bf_dev_port_t port);
pipe_status_t pipe_mgr_ebuf_tof_disable_port_chnl(rmt_dev_info_t *dev_info,
                                                  bf_dev_port_t port_id);
pipe_status_t pipe_mgr_ebuf_tof_set_1588_timestamp_offset(
    pipe_sess_hdl_t sess_hdl, rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ebuf_tof_epb_set_100g_credits(pipe_sess_hdl_t sess_hdl,
                                                     rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ebuf_tof_wait_for_flush_all_chan(
    pipe_sess_hdl_t sess_hdl, rmt_dev_info_t *dev_info);

pipe_status_t ebuf_set_epb_prsr_port_chnl_ctrl_en_reg(rmt_dev_info_t *dev_info,
                                                      uint8_t logical_pipe,
                                                      bf_dev_port_t local_port,
                                                      bool enable);

pipe_status_t pipe_mgr_ebuf_tof_complete_port_mode_transition_wa(
    rmt_dev_info_t *dev_info, bf_dev_port_t port);

pipe_status_t pipe_mgr_ebuf_tof_get_port_counter(rmt_dev_info_t *dev_info,
                                                 bf_dev_port_t port_id,
                                                 uint64_t *value);

pipe_status_t pipe_mgr_ebuf_tof_get_port_bypass_counter(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id, uint64_t *value);

pipe_status_t pipe_mgr_ebuf_tof_get_port_100g_credits(rmt_dev_info_t *dev_info,
                                                      bf_dev_port_t port_id,
                                                      uint64_t *value);

#endif
