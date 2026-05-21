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

#ifndef _PIPE_MGR_TOF_MIRROR_BUFFER_H
#define _PIPE_MGR_TOF_MIRROR_BUFFER_H

#define PIPE_MGR_TOF_MIRROR_SESSION_MAX 1024
#define PIPE_MGR_TOF_MIRROR_COAL_SESSION_MAX 8
#define PIPE_MGR_TOF_MIRROR_COAL_BASE_SID \
  (PIPE_MGR_TOF_MIRROR_SESSION_MAX - PIPE_MGR_TOF_MIRROR_COAL_SESSION_MAX)
#define PIPE_MGR_TOF_MIRROR_NEG_SID (PIPE_MGR_TOF_MIRROR_COAL_BASE_SID - 1)

#define PIPE_MGR_TOF_MIRROR_COAL_DEF_BASE_TIME 100  // usec

pipe_status_t pipe_mgr_tof_mirror_buf_init(pipe_sess_hdl_t sess_hdl,
                                           bf_dev_id_t dev_id);
pipe_status_t pipe_mgr_tof_mirror_buf_init_session(pipe_sess_hdl_t sess_hdl,
                                                   bf_mirror_id_t sid,
                                                   bf_dev_id_t dev_id,
                                                   pipe_bitmap_t pbm);
bool pipe_mgr_tof_mirror_buf_sid_is_coalescing(uint16_t sid);
pipe_status_t pipe_mgr_tof_mirror_buf_coal_session_update(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_id_t dev_id,
    pipe_bitmap_t pipes,
    uint16_t sid,
    pipe_mgr_mirror_session_info_t *s_info,
    bool enable);
pipe_status_t pipe_mgr_tof_mirror_buf_coal_session_read(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_id_t dev_id,
    bf_dev_pipe_t pipe_id,
    uint16_t sid,
    pipe_mgr_mirror_session_info_t *s_info,
    bool *enable,
    bool *session_valid);
pipe_status_t pipe_mgr_tof_mirror_buf_norm_session_update(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_id_t dev_id,
    pipe_bitmap_t pipes,
    uint16_t sid,
    pipe_mgr_mirror_session_info_t *session_info,
    bool enable_ing,
    bool enable_egr);
pipe_status_t pipe_mgr_tof_mirror_buf_norm_session_read(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_id_t dev_id,
    bf_dev_pipe_t pipe_id,
    uint16_t sid,
    pipe_mgr_mirror_session_info_t *session_info,
    bool *enable_ing,
    bool *enable_egr,
    bool *session_valid);
bool pipe_mgr_tof_ha_mirror_buf_cfg_compare(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t log_pipe,
    bf_mirror_id_t sid,
    pipe_mgr_mirror_session_info_t *sess_info,
    pipe_mgr_mirror_session_info_t *hw_sess_info);
#endif /* _PIPE_MGR_TOF_MIRROR_BUFFER_H */
