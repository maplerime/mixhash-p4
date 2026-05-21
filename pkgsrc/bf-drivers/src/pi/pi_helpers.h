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

#ifndef _PI_HELPERS_H__
#define _PI_HELPERS_H__

#include <PI/p4info.h>
#include <PI/pi.h>

#include "pi_state.h"

#include <pipe_mgr/pipe_mgr_intf.h>

#define PI_DEV_ID_TO_PIPE(dev_id) ((bf_dev_id_t)dev_id)
#define PI_DEV_ID_FROM_PIPE(dev_id) ((pi_dev_id_t)dev_id)

void convert_dev_tgt(pi_dev_tgt_t pi_dev_tgt, dev_target_t *pipe_mgr_dev_tgt);

void convert_pipe_dev_tgt(dev_target_t pipe_mgr_dev_tgt,
                          pi_dev_tgt_t *pi_dev_tgt);

void build_action_spec(const pi_action_data_t *action_data,
                       const pi_p4info_t *p4info,
                       pipe_action_data_spec_t *pipe_action_data_spec);

void build_pipe_action_spec_direct(const pi_p4info_t *p4info,
                                   pi_dev_id_t dev_id,
                                   pi_p4_id_t table_id,
                                   const pi_action_data_t *action_data,
                                   pipe_action_spec_t *pipe_action_spec);

void cleanup_pipe_action_spec(pipe_action_spec_t *pipe_action_spec);

void unbuild_action_spec(pi_p4_id_t action_id,
                         const pi_p4info_t *p4info,
                         const pipe_action_data_spec_t *pipe_action_data_spec,
                         pi_action_data_t *action_data);

void unbuild_pipe_action_spec_direct(const pi_p4info_t *p4info,
                                     pi_dev_id_t dev_id,
                                     pi_p4_id_t table_id,
                                     pi_p4_id_t action_id,
                                     const pipe_action_spec_t *pipe_action_spec,
                                     pi_action_data_t *action_data);

size_t retrieve_indirect_res_index(
    const pi_action_data_t *action_data,
    const pi_state_indirect_res_access_info_t *access_info);

void emit_indirect_res_index(
    size_t idx,
    pi_action_data_t *action_data,
    const pi_state_indirect_res_access_info_t *access_info);

typedef enum {
  INDIRECT_H_MBR = 0,
  INDIRECT_H_GRP = 1,
} indirect_h_type_t;

pi_indirect_handle_t indirect_h_make(pi_indirect_handle_t h,
                                     indirect_h_type_t type);
pi_indirect_handle_t indirect_h_clear(pi_indirect_handle_t h);
int indirect_h_is(pi_indirect_handle_t h, indirect_h_type_t type);

typedef struct _pipe_mgr_simple_cb_s _pipe_mgr_simple_cb_t;

_pipe_mgr_simple_cb_t *_pipe_mgr_simple_cb_init();
void _pipe_mgr_simple_cb_destroy(_pipe_mgr_simple_cb_t *cb_data);
void _pipe_mgr_simple_cb_wait(_pipe_mgr_simple_cb_t *cb_data);
void _pipe_mgr_simple_cb_notify(_pipe_mgr_simple_cb_t *cb_data);
void _pipe_mgr_simple_cb_fn(bf_dev_id_t device_id, void *cookie);

typedef struct {
  char *buf;
  size_t size;
  size_t capacity;
} entry_buffer_t;

char *entry_buffer_create(entry_buffer_t *ebuf);
char *entry_buffer_extend(entry_buffer_t *ebuf, size_t s);
void entry_buffer_destroy(entry_buffer_t *ebuf);

#endif  // _PI_HELPERS_H__
