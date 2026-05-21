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
 * @file pipe_exm_transaction.c
 * @date
 *
 * Exact-match table transaction handling.
 *
 */

/* Standard header includes */

/* Module header includes */

/* Local header includes */
#include "pipe_mgr_exm_tbl_mgr_int.h"
#include "cuckoo_move.h"

pipe_status_t pipe_mgr_exm_ent_hdl_txn_add(
    pipe_mgr_exm_tbl_t *exm_tbl,
    pipe_mgr_exm_tbl_data_t *exm_tbl_data,
    pipe_mgr_exm_stage_info_t *exm_stage_info,
    pipe_mat_ent_hdl_t mat_ent_hdl,
    pipe_mgr_exm_entry_info_t *entry_info,
    pipe_mgr_exm_operation_e operation,
    pipe_mat_ent_idx_t src_entry_idx,
    pipe_mat_ent_idx_t dst_entry_idx);
