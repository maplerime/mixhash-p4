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

#ifndef PIPE_MGR_STATS_MGR_TRACE_H
#define PIPE_MGR_STATS_MGR_TRACE_H

#include "pipe_mgr_stat_mgr_int.h"

#define PIPE_MGR_STAT_DBGCHK(inst, x)    \
  {                                      \
    if (!(x)) {                          \
      pipe_mgr_stat_trace_err_log(inst); \
      PIPE_MGR_DBGCHK(x);                \
    }                                    \
  }

void pipe_mgr_stat_mgr_trace_add(pipe_mgr_stat_tbl_t *stat_tbl,
                                 pipe_mgr_stat_tbl_instance_t *inst,
                                 pipe_mat_ent_hdl_t ent_hdl,
                                 dev_stage_t stage_id,
                                 pipe_stat_stage_ent_idx_t stage_idx,
                                 uint32_t deferred);
void pipe_mgr_stat_mgr_trace_def_add(pipe_mgr_stat_tbl_t *stat_tbl,
                                     pipe_mgr_stat_tbl_instance_t *inst,
                                     pipe_mat_ent_hdl_t ent_hdl,
                                     dev_stage_t stage_id,
                                     pipe_stat_stage_ent_idx_t stage_idx,
                                     bf_dev_pipe_t pipe_id);
void pipe_mgr_stat_mgr_trace_del(pipe_mgr_stat_tbl_t *stat_tbl,
                                 pipe_mgr_stat_tbl_instance_t *inst,
                                 pipe_mat_ent_hdl_t ent_hdl,
                                 dev_stage_t stage_id,
                                 pipe_stat_stage_ent_idx_t stage_idx,
                                 uint32_t deferred);
void pipe_mgr_stat_mgr_trace_def_del(pipe_mgr_stat_tbl_t *stat_tbl,
                                     pipe_mgr_stat_tbl_instance_t *inst,
                                     pipe_mat_ent_hdl_t ent_hdl,
                                     dev_stage_t stage_id,
                                     pipe_stat_stage_ent_idx_t stage_idx,
                                     bf_dev_pipe_t pipe_id);
void pipe_mgr_stat_mgr_trace_mov(pipe_mgr_stat_tbl_t *stat_tbl,
                                 pipe_mgr_stat_tbl_instance_t *inst,
                                 pipe_mat_ent_hdl_t ent_hdl,
                                 dev_stage_t dst_stage_id,
                                 pipe_stat_stage_ent_idx_t dst_stage_idx,
                                 dev_stage_t src_stage_id,
                                 pipe_stat_stage_ent_idx_t src_stage_idx,
                                 uint32_t deferred);
void pipe_mgr_stat_mgr_trace_def_mov(pipe_mgr_stat_tbl_t *stat_tbl,
                                     pipe_mgr_stat_tbl_instance_t *inst,
                                     pipe_mat_ent_hdl_t ent_hdl,
                                     dev_stage_t dst_stage_id,
                                     pipe_stat_stage_ent_idx_t dst_stage_idx,
                                     dev_stage_t src_stage_id,
                                     pipe_stat_stage_ent_idx_t src_stage_idx,
                                     bf_dev_pipe_t pipe_id);
void pipe_mgr_stat_mgr_trace_bar(pipe_mgr_stat_tbl_t *stat_tbl,
                                 pipe_mgr_stat_tbl_instance_t *inst,
                                 pipe_mgr_stat_barrier_state_t *bs);
void pipe_mgr_stat_mgr_trace_bar_ack(pipe_mgr_stat_tbl_t *stat_tbl,
                                     pipe_mgr_stat_tbl_instance_t *inst,
                                     bf_dev_pipe_t pipe_id,
                                     dev_stage_t stage_id,
                                     lock_id_t lock_id,
                                     bool deferred);
pipe_status_t pipe_mgr_stat_trace_str(pipe_mgr_stat_tbl_trace_entry_t *trace,
                                      char *buf,
                                      int buf_len);
void pipe_mgr_stat_trace_err_log(pipe_mgr_stat_tbl_instance_t *inst);
#endif
