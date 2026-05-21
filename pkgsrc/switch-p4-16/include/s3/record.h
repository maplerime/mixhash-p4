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

#ifndef INCLUDE_S3_RECORD_H__
#define INCLUDE_S3_RECORD_H__

#include "bf_switch/bf_switch_types.h"

#ifdef __cplusplus
#include <set>
#include <string>
namespace smi {
namespace record {
void record_file_init(std::string record_file);
void record_file_clean(void);
void record_file_replay(std::string replay_file);
void record_comment_mode_set(bool on);
void record_add_create(const switch_object_type_t object_type,
                       const std::set<attr_w> &attrs,
                       switch_object_id_t object_id,
                       switch_status_t status);
void record_add_set(switch_object_id_t object_id,
                    const attr_w &attr,
                    switch_status_t status);
void record_add_get(switch_object_id_t object_id,
                    const switch_attr_id_t attr_id,
                    const attr_w &attr,
                    switch_status_t status);
void record_add_remove(switch_object_id_t object_id, switch_status_t status);
void record_add_notify(std::string notif);
void record_comment_mode_set(bool on);
bool record_comment_mode_get(void);
} /* namespace record */
} /* namespace smi */
#endif  /* __cplusplus */
#endif  // INCLUDE_S3_RECORD_H__
