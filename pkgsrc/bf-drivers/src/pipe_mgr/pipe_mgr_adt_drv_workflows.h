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
 * @file pipe_exm_drv_workflows.h
 * @date
 *
 * Definitions for driver workflows used by action data table manager.
 */

/* Standard header includes */

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>

/* Local header includes */
#include "pipe_mgr_act_tbl.h"

pipe_status_t pipe_mgr_adt_program_entry(
    pipe_sess_hdl_t sess_hdl,
    pipe_mgr_adt_t *adt,
    pipe_mgr_adt_data_t *adt_tbl_data,
    pipe_mgr_adt_stage_info_t *adt_stage_info,
    pipe_adt_ent_idx_t entry_idx,
    uint8_t **shadow_ptr_arr,
    mem_id_t *mem_id_arr,
    uint32_t num_ram_units,
    bool update);
