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

#ifndef DVM_INTF_H_INCLUDED
#define DVM_INTF_H_INCLUDED

/**
 * @file dvm_intf.h
 *
 * @brief Device manager interface APIs
 *
 */

/**
 * @brief Check whether the device is a virtual device or not.
 *
 * @param[in] dev_id Device identifier.
 *
 * @return true - if the device is a virtual device, false -otherwise.
 */
bool bf_drv_is_device_virtual(bf_dev_id_t dev_id);

#endif  // DVM_INTF_H
