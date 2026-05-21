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
 * @file pipe_mgr_stat_tbl_init.c
 * @date
 *
 *
 * Contains initialization code for statistics tables
 */

#ifndef PIPE_MGR_STAT_TBL_INIT_H
#define PIPE_MGR_STAT_TBL_INIT_H

/* Global header includes */

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>
#include <target-utils/map/map.h>

/* Local header includes */
#include "pipe_mgr_int.h"
#include "pipe_mgr_stat_mgr_int.h"

void pipe_mgr_stat_mgr_tbl_cleanup(bf_dev_id_t device_id,
                                   pipe_stat_tbl_hdl_t stat_tbl_hdl);

void pipe_mgr_stat_mgr_tbl_delete(bf_dev_id_t device_id,
                                  pipe_mgr_stat_tbl_t *stat_tbl);

pipe_status_t pipe_mgr_stat_tbl_get_symmetric_mode(
    bf_dev_id_t dev_id,
    pipe_mat_tbl_hdl_t tbl_hdl,
    bool *symmetric,
    scope_num_t *num_scopes,
    scope_pipes_t *scope_pipe_bmp);

pipe_status_t pipe_mgr_stat_tbl_set_symmetric_mode(
    bf_dev_id_t device_id,
    pipe_stat_tbl_hdl_t tbl_hdl,
    bool symmetric,
    scope_num_t num_scopes,
    scope_pipes_t *scope_pipe_bmp);
#endif
