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

#include <stdio.h>
#include <dvm/bf_drv_intf.h>
#include <port_mgr/bf_port_if.h>
#include <tofino/pdfixed/pd_port_mgr.h>

p4_pd_status_t p4_port_mgr_mtu_set(const bf_dev_id_t dev_id,
                                   const int32_t port_id,
                                   const int32_t tx_mtu,
                                   const int32_t rx_mtu) {
  return bf_port_mtu_set(dev_id, port_id, tx_mtu, rx_mtu);
}
