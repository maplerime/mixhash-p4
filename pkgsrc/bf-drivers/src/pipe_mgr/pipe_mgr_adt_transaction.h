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
 * @file pipe_mgr_adt_transaction.h
 * @date
 *
 * Action data table transaction handling definitions.
 */

/* Standard header includes */

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>
#include <pipe_mgr/pipe_mgr_err.h>

/* Local header includes */

pipe_status_t pipe_mgr_adt_txn_commit(bf_dev_id_t dev_id,
                                      pipe_adt_tbl_hdl_t tbl_hdl,
                                      bf_dev_pipe_t *pipes_list,
                                      unsigned nb_pipes);

pipe_status_t pipe_mgr_adt_txn_abort(bf_dev_id_t dev_id,
                                     pipe_adt_tbl_hdl_t tbl_hdl,
                                     bf_dev_pipe_t *pipes_list,
                                     unsigned nb_pipes);
