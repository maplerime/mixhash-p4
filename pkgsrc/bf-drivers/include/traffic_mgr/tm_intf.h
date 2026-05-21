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

#ifndef __TM_INTF_H__
#define __TM_INTF_H__

#include <traffic_mgr/traffic_mgr_types.h>

bf_status_t bf_tm_init();

// Used for UT purposes only..
void bf_tm_set_ut_mode_as_model(bf_dev_id_t dev);
void bf_tm_set_ut_mode_as_asic(bf_dev_id_t dev);

#endif
