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

#ifndef _TOFINO_BF_PAL_PLATFORM_INTF_H
#define _TOFINO_BF_PAL_PLATFORM_INTF_H

#include <bf_types/bf_types.h>

/**
 * @brief Get the type of the device (model or asic)
 * @param[in] dev_id Device id
 * @param[out] is_sw_model Pointer to bool flag to return true for model and
                      false for asic devices
 * @return Status of the API call
 */
bf_status_t bf_pal_pltfm_type_get(bf_dev_id_t dev_id, bool *is_sw_model);

#endif
