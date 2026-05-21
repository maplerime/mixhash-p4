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

#include <stdio.h>
#include <pipe_mgr/pipe_mgr_intf.h>
#include <tofino/pdfixed/pd_common.h>

p4_pd_status_t p4_pd_snapshot_timer_enable(pipe_snapshot_hdl_t hdl,
                                           bool disable) {
  bf_snapshot_ig_mode_t mode;
  bool old_val;
  uint32_t usec;
  bf_snapshot_cfg_get(hdl, &old_val, &usec, &mode);
  return bf_snapshot_cfg_set(hdl, disable, mode);
}

p4_pd_status_t p4_pd_snapshot_state_set(pipe_snapshot_hdl_t hdl,
                                        bf_snapshot_state_t state,
                                        uint32_t timer_usec) {
  return bf_snapshot_state_set(hdl, state, timer_usec);
}

p4_pd_status_t p4_pd_snapshot_state_get(pipe_snapshot_hdl_t hdl,
                                        bf_dev_pipe_t dev_pipe_id,
                                        bf_snapshot_state_t *state) {
  return bf_snapshot_pd_state_get(hdl, dev_pipe_id, (int *)state);
}

p4_pd_status_t p4_pd_snapshot_capture_trigger_fields_clr(
    pipe_snapshot_hdl_t hdl) {
  return bf_snapshot_capture_trigger_fields_clr(hdl);
}

p4_pd_status_t p4_pd_snapshot_field_in_scope(p4_pd_dev_target_t dev_tgt,
                                             uint8_t stage,
                                             bf_snapshot_dir_t dir,
                                             char *field_name,
                                             bool *field_exists) {
  bf_dev_id_t dev = dev_tgt.device_id;
  bf_dev_pipe_t dev_pipe_id = dev_tgt.dev_pipe_id;

  return bf_snapshot_field_in_scope(
      dev, dev_pipe_id, stage, dir, field_name, field_exists);
}

p4_pd_status_t p4_pd_snapshot_trigger_field_in_scope(p4_pd_dev_target_t dev_tgt,
                                                     uint8_t stage,
                                                     bf_snapshot_dir_t dir,
                                                     char *field_name,
                                                     bool *field_exists) {
  bf_dev_id_t dev = dev_tgt.device_id;
  bf_dev_pipe_t dev_pipe_id = dev_tgt.dev_pipe_id;

  return bf_snapshot_trigger_field_in_scope(
      dev, dev_pipe_id, stage, dir, field_name, field_exists);
}
