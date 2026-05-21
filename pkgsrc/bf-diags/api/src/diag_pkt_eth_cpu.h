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
 * @file diag_pkt_eth_cpu.h
 * @date
 *
 * Contains definitions of diag eth cpu pkt interface
 *
 */
#ifndef _DIAG_PKT_ETH_CPU_H
#define _DIAG_PKT_ETH_CPU_H

/* Module header includes */
#include "diag_common.h"

bf_status_t diag_eth_cpu_port_init(bf_dev_id_t dev_id,
                                   const char *eth_cpu_port_name);
bf_status_t diag_eth_cpu_port_deinit(bf_dev_id_t dev_id);

#endif
