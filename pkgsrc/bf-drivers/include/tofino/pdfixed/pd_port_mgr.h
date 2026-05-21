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

#ifndef _TOFINO_PDFIXED_PD_PORT_MGR_H
#define _TOFINO_PDFIXED_PD_PORT_MGR_H

#include <tofino/pdfixed/pd_common.h>
#include <port_mgr/bf_port_if.h>

/**
 * @brief Sets the maximum TX and RX MTU
 *
 * @param[in] dev_id Device identifier (0..BF_MAX_DEV_COUNT-1)
 * @param[in] port_id Port id
 * @param[in] tx_mtu  Maximum TX MTU
 * @param[in] rx_mtu  Maximum RX MTU
 *
 * @return BF_SUCCESS if success
 */
p4_pd_status_t p4_port_mgr_mtu_set(const bf_dev_id_t dev_id,
                                   const int port_id,
                                   const int tx_mtu,
                                   const int rx_mtu);

#endif
