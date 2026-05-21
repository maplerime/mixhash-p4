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

#ifndef port_mgr_physical_dev_h_included
#define port_mgr_physical_dev_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "port_mgr_tof1/port_mgr_tof1_physical_dev.h"
#include "port_mgr_tof2/port_mgr_tof2_physical_dev.h"
#include "port_mgr_tof3/port_mgr_tof3_physical_dev.h"

// Supported chip types
typedef enum {
  PORT_MGR_PDEV_TYPE_UNSPEC = 0,  // catch uninitialized refs
  PORT_MGR_PDEV_TYPE_TOF1 = 1,
  PORT_MGR_PDEV_TYPE_TOF2 = 2,
  PORT_MGR_PDEV_TYPE_TOF3 = 3,
} port_mgr_pdev_type_t;

// physical device (chip-specific)
typedef struct port_mgr_pdev_t {
  port_mgr_pdev_type_t pdev_type;
  union {
    port_mgr_tof1_pdev_t pdev_tof1;
    port_mgr_tof2_pdev_t pdev_tof2;
    port_mgr_tof3_pdev_t pdev_tof3;
  } u;
} port_mgr_pdev_t;

port_mgr_tof1_pdev_t *port_mgr_dev_physical_dev_get(bf_dev_id_t dev_id);
port_mgr_tof2_pdev_t *port_mgr_dev_physical_dev_tof2_get(bf_dev_id_t dev_id);
port_mgr_tof3_pdev_t *port_mgr_dev_physical_dev_tof3_get(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
