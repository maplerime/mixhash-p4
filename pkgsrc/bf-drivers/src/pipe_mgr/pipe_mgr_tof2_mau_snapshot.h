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

/*!
 * @file pipe_mgr_tof2_mau_snapshot.h
 * @date
 *
 * Contains definitions of MAU snapshot
 *
 */
#ifndef _PIPE_MGR_TOF2_MAU_SNAPSHOT_H
#define _PIPE_MGR_TOF2_MAU_SNAPSHOT_H

#include <pipe_mgr/pipe_mgr_intf.h>
#include "pipe_mgr_mau_snapshot.h"

#define PIPE_MGR_TOF2_INVALID_LT_SRC 0xff

pipe_status_t pipe_mgr_snapshot_cfg_set_tof2(rmt_dev_info_t *dev_info,
                                             bf_dev_pipe_t pipe,
                                             dev_stage_t stage,
                                             bool timer_ig_enable,
                                             bool timer_eg_enable,
                                             bf_snapshot_ig_mode_t mode);
pipe_status_t pipe_mgr_snapshot_cfg_get_tof2(rmt_dev_info_t *dev_info,
                                             bf_dev_pipe_t pipe,
                                             dev_stage_t stage,
                                             bool *timer_ig_enable,
                                             bool *timer_eg_enable,
                                             bf_snapshot_ig_mode_t *mode);
pipe_status_t pipe_mgr_snapshot_timer_set_tof2(rmt_dev_info_t *dev_info,
                                               bf_dev_pipe_t pipe,
                                               dev_stage_t stage,
                                               uint64_t clocks);
pipe_status_t pipe_mgr_snapshot_timer_get_tof2(rmt_dev_info_t *dev_info,
                                               bf_dev_pipe_t pipe,
                                               dev_stage_t stage,
                                               uint64_t *clocks_now,
                                               uint64_t *clocks_trig);
pipe_status_t pipe_mgr_snapshot_capture_trigger_set_tof2(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe_idx,
    dev_stage_t stage,
    pipe_mgr_phv_spec_t *phv_spec);
pipe_status_t pipe_mgr_snapshot_fsm_state_set_tof2(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe_idx,
    dev_stage_t stage,
    bf_snapshot_dir_t dir,
    pipe_snapshot_fsm_state_t fsm_state);
pipe_status_t pipe_mgr_snapshot_fsm_state_get_tof2(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe,
    dev_stage_t stage,
    bf_snapshot_dir_t dir,
    pipe_snapshot_fsm_state_t *fsm_state);
pipe_status_t pipe_mgr_get_snapshot_captured_data_tof2(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe,
    dev_stage_t stage,
    bf_snapshot_dir_t dir,
    pipe_mgr_phv_spec_t *phv_spec,
    pipe_mgr_snapshot_capture_data_t *capture);
pipe_status_t pipe_mgr_snapshot_interrupt_clear_tof2(rmt_dev_info_t *dev_info,
                                                     bf_dev_pipe_t pipe,
                                                     dev_stage_t stage,
                                                     bf_snapshot_dir_t dir);
pipe_status_t pipe_mgr_snapshot_interrupt_get_tof2(rmt_dev_info_t *dev_info,
                                                   bf_dev_pipe_t pipe,
                                                   dev_stage_t stage,
                                                   bf_snapshot_dir_t dir,
                                                   bool *is_set);
pipe_status_t pipe_mgr_snapshot_captured_trigger_type_get_tof2(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe,
    dev_stage_t stage,
    int dir,
    bool *prev_stage_trig,
    bool *local_stage_trig,
    bool *timer_trig);

pipe_status_t pipe_mgr_snapshot_captured_thread_get_tof2(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe,
    dev_stage_t stage,
    int dir,
    bool *ingress,
    bool *egress,
    bool *ghost);

pipe_status_t pipe_mgr_snapshot_dp_reset_tof2(rmt_dev_info_t *dev_info,
                                              bf_dev_pipe_t pipe,
                                              dev_stage_t stage,
                                              int dir);
#endif
