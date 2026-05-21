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

#ifndef _PIPE_MGR_IDLE_SWEEP_H_
#define _PIPE_MGR_IDLE_SWEEP_H_

#include "pipe_mgr_idle.h"

void pipe_mgr_idle_sw_timer_cb(struct bf_sys_timer_s *timer, void *data);

pipe_status_t pipe_mgr_idle_entry_get_ttl(idle_tbl_stage_info_t *stage_info,
                                          pipe_mat_ent_hdl_t ent_hdl,
                                          uint32_t *ttl_p,
                                          uint32_t *init_ttl_p);

pipe_status_t pipe_mgr_idle_entry_get_init_ttl(
    idle_tbl_stage_info_t *stage_info,
    pipe_mat_ent_hdl_t ent_hdl,
    uint32_t *init_ttl_p);

pipe_status_t pipe_mgr_idle_entry_get_poll_state(
    idle_tbl_stage_info_t *stage_info,
    pipe_mat_ent_hdl_t ent_hdl,
    pipe_idle_time_hit_state_e *poll_state_p);

pipe_status_t pipe_mgr_idle_entry_set_poll_state(
    idle_tbl_stage_info_t *stage_info,
    pipe_mat_ent_hdl_t ent_hdl,
    pipe_idle_time_hit_state_e poll_state);

bool pipe_mgr_idle_entry_mdata_exists(idle_tbl_stage_info_t *stage_info,
                                      pipe_mat_ent_hdl_t ent_hdl);

pipe_status_t pipe_mgr_idle_entry_add_mdata(idle_tbl_stage_info_t *stage_info,
                                            pipe_mat_ent_hdl_t ent_hdl,
                                            uint32_t index,
                                            uint32_t new_ttl,
                                            uint32_t cur_ttl);

pipe_status_t pipe_mgr_idle_entry_del_mdata(idle_tbl_stage_info_t *stage_info,
                                            pipe_mat_ent_hdl_t ent_hdl,
                                            uint32_t index);

pipe_status_t pipe_mgr_idle_entry_move_mdata(idle_tbl_stage_info_t *stage_info,
                                             pipe_mat_ent_hdl_t ent_hdl,
                                             uint32_t src_index,
                                             uint32_t dest_index);

pipe_status_t pipe_mgr_idle_entry_set_mdata_ttl_dirty(
    idle_tbl_stage_info_t *stage_info,
    pipe_mat_ent_hdl_t ent_hdl,
    uint32_t new_ttl);

/* Accessed by CLI thread */
idle_entry_t *pipe_mgr_idle_entry_get_by_index(
    idle_tbl_stage_info_t *stage_info, uint32_t index);

bool pipe_mgr_idle_entry_mdata_get(idle_tbl_stage_info_t *stage_info,
                                   pipe_mat_ent_hdl_t ent_hdl,
                                   uint32_t *init_ttl_p,
                                   uint32_t *cur_ttl_p,
                                   idle_entry_location_t **ils_p);
void destroy_ils(idle_entry_location_t *ils);

pipe_status_t pipe_mgr_idle_process_task_list(idle_tbl_info_t *idle_tbl_info,
                                              idle_tbl_stage_info_t *stage_info,
                                              idle_task_list_t *tlist);
#endif
