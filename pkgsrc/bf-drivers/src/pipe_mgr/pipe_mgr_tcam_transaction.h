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
 * @file pipe_mgr_tcam_transaction.h
 * @date
 *
 * TCAM related definitions of pipeline manager
 */

#ifndef _PIPE_MGR_TCAM_TRANSACTION_H
#define _PIPE_MGR_TCAM_TRANSACTION_H

pipe_status_t pipe_mgr_tcam_default_entry_backup(
    tcam_pipe_tbl_t *tcam_pipe_tbl);

/** \brief pipe_mgr_tcam_entry_index_backup_one
 *        Backup a tcam entry in the backup tables
 *
 * \param tcam_tbl Pointer to the tcam table structure
 * \param index TCAM index to backup
 * \return pipe_status_t Status of the operation
 */
pipe_status_t pipe_mgr_tcam_entry_index_backup_one(tcam_tbl_t *tcam_tbl,
                                                   uint32_t index);

/* Backup all the instances of a ent-hdl based TCAM entry */
pipe_status_t pipe_mgr_tcam_entry_backup_one(tcam_pipe_tbl_t *tcam_pipe_tbl,
                                             pipe_mat_ent_hdl_t ent_hdl);

/** \brief pipe_mgr_tcam_restore_all
 *        Restore the state from the backup copies
 *
 * This function should be called during abort to restore the state from
 * backed up state
 *
 * \param tcam_pipe_tbl Pointer to the tcam table pipe info
 * \param btcam_pipe_tbl Pointer to the backup tcam table pipe info
 * \return pipe_status_t Status of the operation
 */
pipe_status_t pipe_mgr_tcam_restore_all(tcam_pipe_tbl_t *tcam_pipe_tbl,
                                        tcam_pipe_tbl_t *btcam_pipe_tbl);

/** \brief pipe_mgr_tcam_discard_all
 *        Restore the state from the backup copies
 *
 * This function should be called during commit to discard the state from
 * backed up state
 *
 * \param tcam_tbl_info Pointer to the tcam table info
 * \param btcam_tbl_info Pointer to the backup tcam table info
 * \return pipe_status_t Status of the operation
 */
pipe_status_t pipe_mgr_tcam_discard_all(tcam_pipe_tbl_t *btcam_pipe_tbl);

#endif
