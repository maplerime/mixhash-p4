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
 * @file pipe_mgr_meter_txn.h
 * @date
 *
 * Definition for Meter table manager transaction.
 */

#ifndef _PIPE_MGR_METER_MGR_TXN_H
#define _PIPE_MGR_METER_MGR_TXN_H

/* Standard header includes */

/* Module header includes */
#include "pipe_mgr/pipe_mgr_intf.h"

/* Local header includes */
#include "pipe_mgr_meter_mgr_int.h"

pipe_status_t pipe_mgr_meter_update_txn_state(
    pipe_mgr_meter_txn_state_t *txn_state,
    uint8_t pipe_id,
    pipe_meter_idx_t meter_idx);

pipe_status_t pipe_mgr_meter_backup_entry_info(
    pipe_mgr_meter_tbl_instance_t *meter_tbl_instance,
    pipe_meter_idx_t meter_idx);

pipe_status_t pipe_mgr_meter_mgr_backup_stage_idx_info(
    pipe_mgr_meter_tbl_stage_info_t *meter_tbl_stage_info,
    pipe_meter_idx_t meter_idx);

#endif  // _PIPE_MGR_METER_MGR_TXN_H
