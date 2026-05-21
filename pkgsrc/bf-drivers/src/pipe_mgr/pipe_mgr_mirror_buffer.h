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

#ifndef _PIPE_MGR_MIRROR_BUFFER_H_
#define _PIPE_MGR_MIRROR_BUFFER_H_

#include "pipe_mgr/pipe_mgr_mirror_intf.h"
#include "pipe_mgr_mirror_buffer_comm.h"
extern pipe_mgr_ctx_t *pipe_mgr_ctx;

typedef struct mirror_info_node_t {
  bool enable_ing;
  bool enable_egr;
  bf_dev_pipe_t pipe_id;
  uint16_t sid;
  pipe_mgr_mirror_session_info_t session_info;
} mirror_info_node_t;

typedef struct mirror_session_cache_ {
  bool ready;
  bf_map_t mir_sess;
  pipe_mgr_mutex_t mirror_lock;
} mirror_session_cache_t;

pipe_status_t pipe_mgr_mirror_buf_init(pipe_sess_hdl_t sess_hdl,
                                       bf_dev_id_t dev_id);
void pipe_mgr_mirror_buf_cleanup(bf_dev_id_t dev_id);
void pipe_mgr_free_mirr_info_node(mirror_info_node_t *node);
pipe_status_t pipe_mgr_mirror_buf_cfg_sessions(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_mirror_buf_mirror_session_validate(
    dev_target_t dev, uint16_t sid, bf_mirror_session_info_t *session_info);

pipe_status_t pipe_mgr_mirror_buf_mirror_session_set(
    pipe_sess_hdl_t sess_hdl,
    dev_target_t dev,
    uint16_t sid,
    bf_mirror_session_info_t *session_info,
    bool enable);
pipe_status_t pipe_mgr_mirror_buf_mirror_session_reset(pipe_sess_hdl_t sess_hdl,
                                                       dev_target_t dev,
                                                       uint16_t sid);
pipe_status_t pipe_mgr_mirr_sess_en_or_dis(pipe_sess_hdl_t sess_hdl,
                                           dev_target_t dev,
                                           bf_mirror_direction_e dir,
                                           uint16_t sid,
                                           bool enable);
pipe_status_t pipe_mgr_mirr_sess_pipe_vector_set(pipe_sess_hdl_t sess_hdl,
                                                 dev_target_t dev_target,
                                                 bf_mirror_id_t sid,
                                                 int logical_pipe_vector);
pipe_status_t pipe_mgr_mirr_sess_pipe_vector_get(pipe_sess_hdl_t sess_hdl,
                                                 dev_target_t dev_target,
                                                 bf_mirror_id_t sid,
                                                 int *logical_pipe_vector);

pipe_status_t pipe_mgr_mirror_ecc_correct(pipe_sess_hdl_t sess_hdl,
                                          bf_dev_id_t device_id,
                                          bf_dev_pipe_t pipe_id,
                                          uint16_t sid);
bf_mirror_type_e pipe_mgr_mirror_buf_mirror_type_get(dev_target_t dev_target,
                                                     bf_mirror_id_t sid);
pipe_status_t pipe_mgr_get_max_mirror_sessions(bf_dev_id_t device_id,
                                               bf_mirror_type_e mirror_type,
                                               bf_mirror_id_t *sid);
pipe_status_t pipe_mgr_get_base_mirror_session_id(bf_dev_id_t device_id,
                                                  bf_mirror_type_e mirror_type,
                                                  bf_mirror_id_t *sid);

pipe_status_t pipe_mgr_mirror_session_meta_flag_update(
    pipe_sess_hdl_t sess_hdl,
    dev_target_t dev_target,
    bf_mirror_id_t sid,
    bf_mirror_meta_flag_e mirror_flag,
    bool value);
pipe_status_t pipe_mgr_mirror_session_meta_flag_get(
    pipe_sess_hdl_t sess_hdl,
    dev_target_t dev_target,
    bf_mirror_id_t sid,
    bf_mirror_meta_flag_e mirror_flag,
    bool *value);

pipe_status_t pipe_mgr_mirror_session_pri_update(pipe_sess_hdl_t sess_hdl,
                                                 dev_target_t dev_target,
                                                 bf_mirror_id_t sid,
                                                 bool value);
pipe_status_t pipe_mgr_mirror_session_pri_get(pipe_sess_hdl_t sess_hdl,
                                              dev_target_t dev_target,
                                              bf_mirror_id_t sid,
                                              bool *value);

pipe_status_t pipe_mgr_mirror_session_coal_mode_update(pipe_sess_hdl_t sess_hdl,
                                                       dev_target_t dev_target,
                                                       bf_mirror_id_t sid,
                                                       bool value);

pipe_status_t pipe_mgr_mirror_session_coal_mode_get(pipe_sess_hdl_t sess_hdl,
                                                    dev_target_t dev_target,
                                                    bf_mirror_id_t sid,
                                                    bool *value);

pipe_status_t pipe_mgr_mirror_session_get(dev_target_t dev_target,
                                          bf_mirror_id_t sid,
                                          bf_mirror_session_info_t *s_info);

pipe_status_t pipe_mgr_mirror_session_enable_get(dev_target_t dev_target,
                                                 bf_mirror_id_t sid,
                                                 bool *session_enable);

pipe_status_t pipe_mgr_mirror_session_get_first(
    dev_target_t dev_target,
    bf_mirror_session_info_t *s_info,
    bf_mirror_id_t *sid,
    bf_dev_pipe_t *pipe_id);
pipe_status_t pipe_mgr_mirror_session_get_next(
    dev_target_t dev_target,
    bf_mirror_id_t cur_sid,
    bf_dev_pipe_t cur_pipe_id,
    bf_mirror_session_info_t *next_info,
    bf_mirror_id_t *next_sid,
    bf_dev_pipe_t *next_pipe_id);
pipe_status_t pipe_mgr_mirror_session_get_count(dev_target_t dev_target,
                                                uint32_t *count);
pipe_status_t pipe_mgr_mirror_buf_session_set_hw(pipe_sess_hdl_t sess_hdl,
                                                 rmt_dev_info_t *dev_info,
                                                 mirror_info_node_t *mirr_node);
pipe_status_t pipe_mgr_mirror_buf_init_one_mirror_session(
    pipe_sess_hdl_t shdl,
    rmt_dev_info_t *dev_info,
    bf_mirror_id_t sid,
    bf_dev_pipe_t pipe_tgt);
mirror_info_node_t *pipe_mgr_mirror_buf_get_session_by_pipe(
    bf_dev_id_t dev_id, bf_mirror_id_t sid, bf_dev_pipe_t pipe_id);
pipe_status_t pipe_mgr_mirror_buf_get_first_session(bf_dev_id_t dev_id,
                                                    bf_mirror_id_t *sid,
                                                    mirror_info_node_t *info);
pipe_status_t pipe_mgr_mirror_buf_get_next_session(bf_dev_id_t dev_id,
                                                   bf_mirror_id_t cur_sid,
                                                   bf_dev_pipe_t cur_pipe_id,
                                                   bf_mirror_id_t *next_sid,
                                                   mirror_info_node_t *info);
#endif  // _PIPE_MGR_MIRROR_BUFFER_H_
