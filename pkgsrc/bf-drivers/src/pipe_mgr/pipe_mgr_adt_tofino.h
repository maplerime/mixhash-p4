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
 * @file pipe_mgr_adt_tofino.h
 * @date
 *
 * Contains definitions for Tofino specific services for action data table
 * management exposed to the action data table manager.
 *
 */

/* Standard header includes */

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>

/* Local header includes */
#include "pipe_mgr_int.h"

#define TOF_ADT_VADDR_NUM_BITS 21

pipe_status_t pipe_adt_tof_generate_vaddr(
    pipe_mgr_adt_stage_info_t *adt_stage_info,
    pipe_adt_ent_idx_t adt_ent_idx,
    rmt_virt_addr_t *adt_virt_addr_p);

pipe_status_t pipe_mgr_adt_tof_encode_entry(
    pipe_mgr_adt_t *adt,
    pipe_mgr_adt_stage_info_t *adt_stage_info,
    pipe_act_fn_hdl_t act_fn_hdl,
    pipe_action_data_spec_t *act_data_spec,
    pipe_adt_ent_idx_t adt_ent_idx,
    uint8_t **shadow_ptr_arr);

void pipe_mgr_adt_tof_unbuild_virt_addr(rmt_virt_addr_t virt_addr,
                                        uint32_t adt_entry_width,
                                        uint32_t *ram_line,
                                        vpn_id_t *vpn,
                                        uint32_t *entry_position);
