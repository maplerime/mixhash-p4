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


#ifndef port_mgr_tof3_physical_device_h_included
#define port_mgr_tof3_physical_device_h_included

#include <bf_types/bf_types.h>
#include "aw_if.h"

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#define TOF3_NUM_TMAC 65

#define TMAC_NUM_CH 8

// Logical serdes lane config
typedef struct port_mgr_tof3_serdes_t {
  bf_tf3_sd_t sd_cfg;
} port_mgr_tof3_serdes_t;

/** \typedef port_mgr_tmac_t:
 *
 */
typedef struct port_mgr_tmac_s {
  uint32_t ch_in_use;
  // indexed by logical lane
  port_mgr_tof3_serdes_t sd[TMAC_NUM_CH];
  // serdes lane map (i.e. swizzling)
  uint32_t phys_tx_ln[8];
  uint32_t phys_rx_ln[8];
  uint32_t max_serdes_per_mac;  // 4 or 8 depending on package
} port_mgr_tmac_t;

/** \typedef port_mgr_tof3_pdev_t:
 *
 */
typedef struct port_mgr_tof3_pdev_t {
  uint32_t pipe_log2phy[BF_PIPE_COUNT];
  port_mgr_tmac_t tmac[TOF3_NUM_TMAC];
  // note: should this be per-die?
  char *serdes_0_fw_path;
  char *serdes_1_32_fw_path;

} port_mgr_tof3_pdev_t;

#ifdef __cplusplus
}
#endif /* C++ */

#endif
