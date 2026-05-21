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
 * @file diag_vlan.h
 * @date
 *
 * Contains definitions of vlan
 *
 */
#ifndef _DIAG_VLAN_H
#define _DIAG_VLAN_H

/* Module header includes */

#include <stdint.h>
#include "stdbool.h"
#include <bf_types/bf_types.h>
#include "diag_common.h"

#define DIAG_GEN_MC_INDEX(_vlan) (_vlan)
#define DIAG_GEN_VLAN_RID(_vlan) (_vlan)

diag_vlan_t *diag_int_get_vlan_info(bf_dev_id_t dev_id, int vlan_id);
bf_status_t diag_int_vlan_create(bf_dev_id_t dev_id, int vlan_id);
bf_status_t diag_int_vlan_destroy(bf_dev_id_t dev_id, int vlan_id);
bf_status_t diag_int_get_vlan_port_bitmap(bf_dev_id_t dev_id,
                                          int vlan_id,
                                          uint8_t *port_map,
                                          uint8_t *lag_map);

#endif
