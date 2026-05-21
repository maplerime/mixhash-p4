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
 * @file pipe_mgr_exm_tof.h
 * @date
 *
 * Code/Definitions relating to Tofino Exact match.
 * Sort of a PD layer to the Exact match driver.
 */

/* Standard header includes */
#include <math.h>

/* Module header includes */

/* Local header includes  */
#include "pipe_mgr_int.h"

static inline vpn_id_t pipe_mgr_exm_tof_get_subword_vpn(
    rmt_virt_addr_t virt_addr) {
  return ((virt_addr >> (int)(log2(TOF_SRAM_UNIT_DEPTH))) &
          ((1 << TOF_EXM_SUBWORD_VPN_BITS) - 1));
}

static inline uint32_t pipe_mgr_exm_tof_get_ram_line(
    rmt_virt_addr_t virt_addr) {
  return (virt_addr & ((1 << (int)log2(TOF_SRAM_UNIT_DEPTH)) - 1));
}
