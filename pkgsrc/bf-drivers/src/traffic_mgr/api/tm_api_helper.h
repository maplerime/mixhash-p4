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

/*
 */

#ifndef __TM_API_HELPER_H__
#define __TM_API_HELPER_H__

#include <stdint.h>
#include <stdbool.h>
#include <traffic_mgr/traffic_mgr_types.h>

int bf_tm_api_hlp_get_pool_details(bf_tm_app_pool_t pool, int *direction);
bool bf_tm_api_hlp_is_baf_dynamic(int baf);
void bf_tm_api_hlp_get_ppg_details(
    bf_dev_id_t, bf_tm_ppg_hdl, bool *, int *, int *, int *);

#endif
