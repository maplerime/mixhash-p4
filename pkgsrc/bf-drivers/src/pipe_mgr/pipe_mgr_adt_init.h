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
 * @file pipe_exm_adt_mgr_init.h
 * @date
 *
 * Contains definitions for action data table manager related initialization.
 *
 */

/* Standard header includes */

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>

/* Local header includes */
#include "pipe_mgr_adt_mgr_int.h"

pipe_status_t pipe_mgr_adt_init_tbl_data(pipe_mgr_adt_t *adt,
                                         uint32_t num_tbl_instances,
                                         pipe_adt_tbl_info_t *adt_tbl_info);

pipe_status_t pipe_mgr_adt_init_stage_data(pipe_mgr_adt_data_t *adt_tbl_data,
                                           uint32_t num_rmt_info,
                                           rmt_tbl_info_t *rmt_info,
                                           pipe_tbl_ref_type_t ref_type,
                                           bool virtual_device);

pipe_status_t pipe_mgr_adt_cleanup_tbl(pipe_mgr_adt_t *adt);

pipe_status_t pipe_mgr_adt_init_ram_alloc_info(
    pipe_mgr_adt_stage_info_t *adt_stage_info, rmt_tbl_info_t *rmt_tbl_info);

pipe_status_t pipe_mgr_adt_add_ram_alloc_info(
    pipe_mgr_adt_stage_info_t *adt_stage_info, rmt_tbl_info_t *rmt_tbl_info);

pipe_status_t pipe_mgr_adt_tbl_init(bf_dev_id_t device_id,
                                    pipe_adt_tbl_hdl_t adt_tbl_hdl,
                                    profile_id_t profile_id,
                                    pipe_bitmap_t *pipe_bmp);

void pipe_mgr_adt_cleanup_ent_refs(pipe_mgr_adt_data_t *adt_tbl_data);

pipe_status_t pipe_mgr_adt_tbl_delete(bf_dev_id_t device_id,
                                      pipe_adt_tbl_hdl_t adt_tbl_hdl);

pipe_status_t pipe_mgr_adt_cleanup_all_tbl_data(pipe_mgr_adt_t *adt);

pipe_status_t pipe_mgr_adt_tbl_get_symmetric_mode(
    bf_dev_id_t dev_id,
    pipe_mat_tbl_hdl_t tbl_hdl,
    bool *symmetric,
    scope_num_t *num_scopes,
    scope_pipes_t *scope_pipe_bmp);

pipe_status_t pipe_mgr_adt_tbl_set_symmetric_mode(
    bf_dev_id_t dev_id,
    pipe_mat_tbl_hdl_t tbl_hdl,
    bool symmetric,
    scope_num_t num_scopes,
    scope_pipes_t *scope_pipe_bmp);
bool pipe_mgr_adt_tbl_is_ent_hdl_valid(pipe_mgr_adt_data_t *adt_tbl_data,
                                       pipe_adt_ent_hdl_t mat_ent_hdl);

pipe_status_t pipe_mgr_adt_get_first_placed_entry_handle(
    pipe_adt_tbl_hdl_t tbl_hdl, dev_target_t dev_tgt, int *entry_hdl);

pipe_status_t pipe_mgr_adt_get_next_placed_entry_handles(
    pipe_adt_tbl_hdl_t tbl_hdl,
    dev_target_t dev_tgt,
    pipe_adt_ent_hdl_t entry_hdl,
    int n,
    int *next_entry_handles);

pipe_status_t pipe_mgr_adt_get_first_programmed_entry_handle(
    pipe_adt_tbl_hdl_t tbl_hdl, dev_target_t dev_tgt, int *entry_hdl);

pipe_status_t pipe_mgr_adt_get_next_programmed_entry_handles(
    pipe_adt_tbl_hdl_t tbl_hdl,
    dev_target_t dev_tgt,
    pipe_adt_ent_hdl_t entry_hdl,
    int n,
    int *next_entry_handles);

pipe_status_t pipe_mgr_adt_shadow_mem_init(pipe_mgr_adt_t *adt);

pipe_status_t pipe_mgr_adt_stage_shadow_mem_init(
    pipe_mgr_adt_t *adt,
    pipe_mgr_adt_stage_info_t *adt_stage_info,
    pipe_bitmap_t *pipe_bmp,
    bf_dev_pipe_t pipe_id);
