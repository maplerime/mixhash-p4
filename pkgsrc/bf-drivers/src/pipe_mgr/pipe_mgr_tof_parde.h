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

#ifndef __PIPE_MGR_TOF_PARDE_H__
#define __PIPE_MGR_TOF_PARDE_H__

#include <pipe_mgr/pipe_mgr_intf.h>
#include "pipe_mgr_db.h"

pipe_status_t pipe_mgr_parde_tof_port_add_egr(pipe_sess_hdl_t shdl,
                                              rmt_dev_info_t *dev_info,
                                              bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_tof_complete_port_mode_transition_wa(
    pipe_sess_hdl_t shdl, rmt_dev_info_t *dev_info, bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_tof_port_add_ing(pipe_sess_hdl_t shdl,
                                              rmt_dev_info_t *dev_info,
                                              bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_tof_port_rmv_egr(pipe_sess_hdl_t shdl,
                                              rmt_dev_info_t *dev_info,
                                              bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_tof_port_rmv_ing(pipe_sess_hdl_t shdl,
                                              rmt_dev_info_t *dev_info,
                                              bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_tof_device_add(pipe_sess_hdl_t shdl,
                                            rmt_dev_info_t *dev_info);

void pipe_mgr_parde_tof_device_rmv(rmt_dev_info_t *dev_info);

int pipe_mgr_parde_tof_speed_to_chan_cnt(bf_port_speeds_t speed);

pipe_status_t pipe_mgr_parde_tof_port_ena_one(pipe_sess_hdl_t shdl,
                                              rmt_dev_info_t *dev_info,
                                              uint8_t logical_pipe,
                                              int ipb_num,
                                              bool ing_0_egr_1);

pipe_status_t pipe_mgr_parde_tof_port_dis_one(pipe_sess_hdl_t shdl,
                                              rmt_dev_info_t *dev_info,
                                              uint8_t logical_pipe,
                                              int ipb_num,
                                              bool ing_0_egr_1);

pipe_status_t pipe_mgr_config_one_mem_tof(pipe_sess_hdl_t sess_hdl,
                                          rmt_dev_info_t *dev_info,
                                          uint8_t *data,
                                          int depth,
                                          int width,
                                          uint64_t addr,
                                          uint8_t log_pipe_mask);

pipe_status_t pipe_mgr_parser_config_tof(
    pipe_sess_hdl_t sess_hdl,
    rmt_dev_info_t *dev_info,
    uint8_t gress,
    pipe_bitmap_t pipe_bmp,
    uint64_t prsr_map,
    struct pipe_mgr_tof_prsr_bin_config *cfg);

#endif
