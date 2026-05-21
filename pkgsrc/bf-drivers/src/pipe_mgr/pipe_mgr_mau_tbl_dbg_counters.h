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
 * @file pipe_mgr_mau_tbl_dbg_counters.h
 * @date
 *
 * Contains definitions of MAU table dbg counter
 *
 */
#ifndef _PIPE_MGR_MAU_TBL_DBG_COUNTERS_H
#define _PIPE_MGR_MAU_TBL_DBG_COUNTERS_H

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>

/* ---- FUNCTONS ---- */

/**
 * Table debug counter DB init
 * @param dev_id The ASIC id.
 * @return Status of the API call.
 */
pipe_status_t pipe_mgr_tbl_dbg_counter_init(bf_dev_id_t dev);

/**
 * Table debug counter DB cleanup
 * @param dev_id The ASIC id.
 * @return Status of the API call.
 */
pipe_status_t pipe_mgr_tbl_dbg_counter_cleanup(bf_dev_id_t dev);

/**
 * Table debug counter type to string
 * @param type Counter type.
 * @param buf buffer to write the string
 * @return Status of the API call.
 */
char *pipe_mgr_tbl_dbg_counter_type_to_string(bf_tbl_dbg_counter_type_t type,
                                              char *buf);

/**
 * Table name to direction
 * @param dev      The ASIC id.
 * @param pipe     The pipe id.
 * @param tbl_name Table name.
 * @return direction
 */
int pipe_mgr_tbl_name_to_dir(bf_dev_id_t dev,
                             bf_dev_pipe_t pipe,
                             char *tbl_name);

#endif
