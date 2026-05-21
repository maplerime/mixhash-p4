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

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <dvm/dvm_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <lld/bf_dev_if.h>
#include <lld/lld_interrupt_if.h>
#include "port_mgr_mac.h"

/** \brief port_mgr_tof1_init:
 *
 * Initialize port_mgr module
 */
void port_mgr_tof1_init(void) {
  lld_register_mac_int_poll_cb(port_mgr_mac_interrupt_poll);
  lld_register_mac_int_dump_cb(port_mgr_mac_interrupt_dump);
}
