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
 * @file pipe_mgr_exm_tbl_dump.c
 * @date
 *
 * Exact match table manager dump definitions
 */

/* Standard header includes */

/* Module header includes */

/* Local header includes */

void pipe_mgr_exm_dump_tbl_info(ucli_context_t *uc,
                                uint8_t device_id,
                                pipe_mat_tbl_hdl_t exm_tbl_hdl);

void pipe_mgr_exm_dump_stage_info(ucli_context_t *uc,
                                  pipe_mgr_exm_stage_info_t *exm_stage_info);

void pipe_mgr_exm_dump_hashway_info(
    ucli_context_t *uc,
    pipe_mgr_exm_hash_way_data_t *exm_hash_way_data,
    uint8_t num_rams_in_wide_word,
    uint8_t num_entries_per_wide_word);

void pipe_mgr_exm_dump_entry_info(ucli_context_t *uc,
                                  uint8_t device_id,
                                  pipe_mat_tbl_hdl_t exm_tbl_hdl,
                                  pipe_mat_ent_hdl_t entry_hdl);

void pipe_mgr_exm_dump_phy_entry_info(ucli_context_t *uc,
                                      bf_dev_id_t device_id,
                                      pipe_mat_tbl_hdl_t exm_tbl_hdl,
                                      pipe_mat_ent_hdl_t entry_hdl);

void pipe_mgr_exm_tbl_dump_hash(ucli_context_t *uc,
                                bf_dev_id_t device_id,
                                pipe_mat_tbl_hdl_t exm_tbl_hdl);

void pipe_mgr_exm_tbl_dump_entries(ucli_context_t *uc,
                                   bf_dev_id_t device_id,
                                   pipe_mat_tbl_hdl_t mat_tbl_hdl,
                                   bool show_entry_handles);
void pipe_mgr_exm_entry_move_stats_dump(ucli_context_t *uc,
                                        bf_dev_id_t dev_id,
                                        pipe_mat_tbl_hdl_t mat_tbl_hdl);
void pipe_mgr_exm_entry_move_stats_clear(ucli_context_t *uc,
                                         bf_dev_id_t dev_id,
                                         pipe_mat_tbl_hdl_t mat_tbl_hdl);
