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

#ifndef _PI_ALLOCATORS_H__
#define _PI_ALLOCATORS_H__

#include <PI/pi.h>

#include <pipe_mgr/pipe_mgr_intf.h>

void allocate_pipe_match_spec(pi_p4_id_t table_id,
                              const pi_p4info_t *p4info,
                              pipe_tbl_match_spec_t *pipe_match_spec);

void release_pipe_match_spec(pipe_tbl_match_spec_t *pipe_match_spec);

void allocate_pipe_action_data_spec(
    pi_p4_id_t action_id,
    const pi_p4info_t *p4info,
    pipe_action_data_spec_t *pipe_action_data_spec);

// allocate space to accomodate any action belonging to the table or action
// profile with the provided id
void allocate_pipe_action_data_spec_any(
    pi_p4_id_t id,
    const pi_p4info_t *p4info,
    pipe_action_data_spec_t *pipe_action_data_spec);

void release_pipe_action_data_spec(
    pipe_action_data_spec_t *pipe_action_data_spec);

void allocate_pi_match_key(pi_p4_id_t table_id,
                           const pi_p4info_t *p4info,
                           pi_match_key_t *match_key);

void release_pi_match_key(pi_match_key_t *match_key);

#endif  // _PI_ALLOCATORS_H__
