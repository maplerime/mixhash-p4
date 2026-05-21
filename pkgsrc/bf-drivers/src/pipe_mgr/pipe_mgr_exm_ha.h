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
 * @file pipe_mgr_exm_ha.h
 * This file contains function/type declarations for exm HA purposes.
 */
#ifndef _PIPE_MGR_EXM_HA_H_
#define _PIPE_MGR_EXM_HA_H_

pipe_status_t pipe_mgr_exm_llp_restore_state(
    bf_dev_id_t dev_id,
    pipe_mat_tbl_hdl_t mat_tbl_hdl,
    pipe_mgr_move_list_t **move_head_p);

pipe_status_t pipe_mgr_exm_hlp_restore_state(bf_dev_id_t dev_id,
                                             pipe_mat_tbl_hdl_t mat_tbl_hdl,
                                             pipe_mgr_move_list_t *move_list,
                                             uint32_t *success_count);

pipe_status_t pipe_mgr_exm_update_sel_hlp_state(dev_target_t dev_tgt,
                                                pipe_mat_tbl_hdl_t tbl_hdl,
                                                pipe_mat_ent_hdl_t entry_hdl,
                                                pipe_sel_grp_hdl_t grp_hdl);

pipe_status_t pipe_mgr_exm_get_ha_reconc_report(
    dev_target_t dev_tgt,
    pipe_mat_tbl_hdl_t mat_tbl_handle,
    pipe_tbl_ha_reconc_report_t *ha_report);

pipe_status_t pipe_mgr_exm_hlp_compute_delta_changes(
    bf_dev_id_t dev_id,
    pipe_mat_tbl_hdl_t mat_tbl_hdl,
    pipe_mgr_move_list_t **move_head_p);

void pipe_mgr_exm_cleanup_hlp_ha_state(bf_dev_id_t device_id,
                                       pipe_mat_tbl_hdl_t mat_tbl_hdl);

void pipe_mgr_exm_cleanup_llp_ha_state(bf_dev_id_t device_id,
                                       pipe_mat_tbl_hdl_t mat_tbl_hdl);

#endif
