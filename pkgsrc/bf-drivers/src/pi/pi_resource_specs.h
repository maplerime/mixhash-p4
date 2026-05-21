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

#ifndef _PI_RESOURCE_SPECS_H_
#define _PI_RESOURCE_SPECS_H_

#include <PI/p4info.h>
#include <PI/pi.h>

#include <pipe_mgr/pipe_mgr_intf.h>

void convert_from_pipe_meter_spec(const pi_p4info_t *p4info,
                                  pi_p4_id_t meter_id,
                                  const pipe_meter_spec_t *pipe_meter_spec,
                                  pi_meter_spec_t *meter_spec);

void convert_to_pipe_meter_spec(const pi_p4info_t *p4info,
                                pi_p4_id_t meter_id,
                                const pi_meter_spec_t *meter_spec,
                                pipe_meter_spec_t *pipe_meter_spec);

void convert_to_counter_data(const pi_p4info_t *p4info,
                             pi_p4_id_t counter_id,
                             const pipe_stat_data_t *stat_data,
                             pi_counter_data_t *counter_data);

#endif  // _PI_RESOURCE_SPECS_H_
