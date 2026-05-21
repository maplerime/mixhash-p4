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

#ifndef _PIPE_MGR_TOF3_MIRROR_BUFFER_H
#define _PIPE_MGR_TOF3_MIRROR_BUFFER_H

#define PIPE_MGR_TOF3_MIRROR_SESSION_MAX 256
#define PIPE_MGR_TOF3_MIRROR_SLICE_MAX 4
#define PIPE_MGR_TOF3_MIRROR_COAL_SESSION_MAX 16

#define PIPE_MGR_TOF3_MIRROR_COAL_DEF_BASE_TIME 100  // usec

pipe_status_t pipe_mgr_tof3_mirror_buf_init(pipe_sess_hdl_t sess_hdl,
                                            bf_dev_id_t dev_id);
pipe_status_t pipe_mgr_tof3_mirror_buf_init_session(pipe_sess_hdl_t sess_hdl,
                                                    bf_mirror_id_t sid,
                                                    bf_dev_id_t dev_id,
                                                    pipe_bitmap_t pbm);
pipe_status_t pipe_mgr_tof3_mirror_buf_coal_session_update(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_id_t dev_id,
    bf_dev_pipe_t pipe_id,
    pipe_bitmap_t pbm,
    uint16_t sid,
    pipe_mgr_mirror_session_info_t *s_info,
    bool enable);
pipe_status_t pipe_mgr_tof3_mirror_buf_norm_session_update(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_id_t dev_id,
    bf_dev_pipe_t pipe_id,
    pipe_bitmap_t pbm,
    uint16_t sid,
    pipe_mgr_mirror_session_info_t *session_info,
    bool enable_ing,
    bool enable_egr);
pipe_status_t pipe_mgr_tof3_session_reset(pipe_sess_hdl_t shdl,
                                          rmt_dev_info_t *dev_info,
                                          uint16_t sid,
                                          bf_dev_pipe_t pipe_id);
#endif /* _PIPE_MGR_TOF3_MIRROR_BUFFER_H */
