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
 * @file pipe_mgr_exm_drv_workflows.h
 * @date
 *
 *
 * Contains definitions for the interface between the exact-match table
 * management code to the driver interface.
 */

#ifndef _PIPE_MGR_EXM_DRV_WORKFLOWS_H
#define _PIPE_MGR_EXM_DRV_WORKFLOWS_H

pipe_status_t pipe_mgr_exm_entry_program(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    pipe_mat_ent_idx_t stage_ent_idx,
    uint32_t num_ram_units,
    uint32_t version_valid_bits,
    uint8_t vv_word_index,
    bool is_stash,
    uint32_t stash_id,
    bool update);

pipe_status_t pipe_mgr_stash_match_addr_control_program(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    pipe_mat_ent_idx_t stage_ent_idx,
    uint32_t stash_id,
    struct pipe_mgr_mat_data *mat_data);

pipe_status_t pipe_mgr_stash_version_valid_program(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    uint32_t stash_id,
    bool valid);

pipe_status_t pipe_mgr_exm_program_default_entry(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    pipe_act_fn_hdl_t act_fn_hdl,
    pipe_action_data_spec_t *act_spec,
    pipe_mgr_indirect_ptrs_t *indirect_ptrs);

pipe_status_t pipe_mgr_exm_get_default_entry(
    pipe_sess_hdl_t sess_hdl,
    dev_target_t dev_tgt,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    pipe_act_fn_hdl_t *act_fn_hdl,
    pipe_action_data_spec_t *act_spec,
    pipe_mgr_indirect_ptrs_t *indirect_ptrs);

pipe_status_t pipe_mgr_exm_issue_push_instr(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    pipe_mat_ent_idx_t entry_idx);

pipe_status_t pipe_mgr_exm_issue_pop_instr(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    uint32_t ttl);

pipe_status_t pipe_mgr_exm_issue_lock_instr(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    lock_id_t lock_id,
    bool *stats_locked_p,
    bool *idle_locked_p);

pipe_status_t pipe_mgr_exm_issue_unlock_instr(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    lock_id_t lock_id,
    bool *stats_unlocked_p,
    bool *idle_unlocked_p);
#endif
