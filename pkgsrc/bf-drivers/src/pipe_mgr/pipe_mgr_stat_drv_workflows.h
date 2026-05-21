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

#ifndef PIPE_MGR_STAT_DRV_WORKFLOWS_H
#define PIPE_MGR_STAT_DRV_WORKFLOWS_H

/* Module header includes */
#include "pipe_mgr/pipe_mgr_intf.h"
#include "pipe_mgr/pipe_mgr_intf.h"

/* Local header includes */
#include "pipe_mgr_stat_mgr_int.h"

pipe_status_t pipe_mgr_stat_mgr_ent_dump_drv_workflow(
    pipe_sess_hdl_t sess_hdl,
    dev_target_t dev_tgt,
    pipe_mgr_stat_tbl_t *stat_tbl,
    pipe_mgr_stat_tbl_instance_t *stat_tbl_instance,
    pipe_mat_tbl_hdl_t mat_tbl_hdl,
    pipe_mat_ent_hdl_t mat_ent_hdl,
    pipe_stat_ent_idx_t stat_ent_idx,
    pipe_mgr_stat_ent_worklist_t *worklist);

pipe_status_t pipe_mgr_stat_mgr_ent_write_drv_workflow(
    pipe_sess_hdl_t sess_hdl,
    bf_dev_pipe_t pipe_id,
    pipe_mgr_stat_tbl_t *stat_tbl,
    pipe_mgr_stat_tbl_instance_t *stat_tbl_instance,
    pipe_mgr_stat_ent_worklist_t *worklist,
    pipe_stat_data_t *stat_data);

pipe_status_t pipe_mgr_stat_tbl_dump_drv_workflow(
    pipe_sess_hdl_t sess_hdl,
    dev_target_t dev_tgt,
    pipe_mgr_stat_tbl_t *stat_tbl,
    pipe_mgr_stat_tbl_instance_t *stat_tbl_instance,
    pipe_mgr_stat_tbl_sync_cback_fn cback_fn,
    void *cookie);

pipe_mgr_stat_barrier_state_t *pipe_mgr_stat_mgr_alloc_barrier_state(
    pipe_bitmap_t *pipe_bmp, dev_stage_t stage_id, lock_id_t lock_id);
#endif
