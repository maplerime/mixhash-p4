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
 * @file pipe_mgr_dev_intf.h
 * @date
 *
 * Definitions for device management interface
 */

#ifndef _PIPE_MGR_DEV_INTF_H
#define _PIPE_MGR_DEV_INTF_H

/* Module header files */
#include <pipe_mgr/pipe_mgr_intf.h>
#include <dvm/bf_drv_intf.h>
#include <target-utils/third-party/cJSON/cJSON.h>

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

/********************************************
 * DEVICE MGMT RELATED API
 ********************************************/
struct bf_dma_info_s;

/* API to add a port to the device */
bf_status_t pipe_mgr_add_port(bf_dev_id_t dev_id,
                              bf_dev_port_t port_id,
                              bf_port_attributes_t *port_attrib,
                              bf_port_cb_direction_t direction);

/* API to remove a port from the device */
bf_status_t pipe_mgr_remove_port(bf_dev_id_t dev_id,
                                 bf_dev_port_t port_id,
                                 bf_port_cb_direction_t direction);

/********************************************
 * RMT TABLE MAPPING RELATED API
 ********************************************/

#endif /* _PIPE_MGR_DEV_INTF_H */
