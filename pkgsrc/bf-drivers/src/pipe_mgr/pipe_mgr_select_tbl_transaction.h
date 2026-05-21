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
 * @file pipe_mgr_select_tbl_transaction.h
 * @date
 *
 *
 * Contains definitions relating to pipe mgr's selector table management
 */
#ifndef _PIPE_MGR_SELECT_TBL_TRANSACTION_H
#define _PIPE_MGR_SELECT_TBL_TRANSACTION_H

pipe_status_t pipe_mgr_sel_discard_all(sel_tbl_t *sel_tbl, sel_tbl_t *bsel_tbl);

pipe_status_t pipe_mgr_sel_restore_all(sel_tbl_t *sel_tbl, sel_tbl_t *bsel_tbl);

pipe_status_t pipe_mgr_sel_grp_backup_one_refcount(
    sel_tbl_info_t *sel_tbl_info,
    sel_tbl_t *sel_tbl,
    pipe_sel_grp_hdl_t sel_grp_hdl);

pipe_status_t pipe_mgr_sel_grp_backup_one(sel_tbl_t *sel_tbl,
                                          pipe_sel_grp_hdl_t sel_grp_hdl);

pipe_status_t pipe_mgr_sel_grp_mbr_backup_one(
    sel_tbl_info_t *sel_tbl_info,
    sel_tbl_t *sel_tbl,
    sel_grp_info_t *sel_grp,
    pipe_sel_grp_mbr_hdl_t grp_mbr_hdl);

pipe_status_t pipe_mgr_sel_backup_fallback_entry(sel_tbl_t *sel_tbl);

#endif
