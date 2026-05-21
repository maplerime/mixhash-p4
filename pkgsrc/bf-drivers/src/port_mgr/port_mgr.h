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

#ifndef port_mgr_h_included
#define port_mgr_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

// top-level logical abstraction(s)
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <port_mgr/bf_port_if.h>
#include "port_mgr_ha.h"
#include "port_mgr_logical_port.h"
#include "port_mgr_logical_dev.h"
#include "port_mgr_physical_dev.h"

// chip-type specific data
#include "port_mgr_tof1/port_mgr_tof1_physical_dev.h"

// device storage. Includes the logical (abstracted)
// device (ldev) as well as the underlying physical instantiation (pdev)
// (i.e. specific type of chip).
typedef struct port_mgr_dev_t {
  port_mgr_ldev_t ldev;
  port_mgr_pdev_t pdev;
} port_mgr_dev_t;

/** \typedef port_mgr_context_t:
 *
 */
typedef struct port_mgr_context_s {
  // per-device storage
  port_mgr_dev_t dev[BF_MAX_DEV_COUNT];

  // global (non device-spcific) callbacks
  bf_port_callback_t internal_sts_chg_cb;
  void *internal_sts_chg_userdata;

} port_mgr_context_t;

extern port_mgr_context_t *port_mgr_ctx;

#ifdef __cplusplus
}
#endif /* C++ */

#endif
