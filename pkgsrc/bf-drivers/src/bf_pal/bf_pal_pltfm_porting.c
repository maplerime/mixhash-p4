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

// Standard includes
#include <string.h>

// Module includes
#include <bf_types/bf_types.h>
#include <tofino/bf_pal/bf_pal_pltfm_porting.h>
#include <bf_pm/bf_pm_intf.h>

// Local includes
#include "bf_pal_log.h"

bf_pal_pltfm_reg_interface_t pltfm_interface = {0};

bf_status_t bf_pal_pltfm_all_interface_set(bf_pal_pltfm_reg_interface_t *intf) {
  if (!intf) return BF_INVALID_ARG;

  // Copy all the function pointers to the global database
  memcpy(&pltfm_interface, intf, sizeof(pltfm_interface));

  return BF_SUCCESS;
}

bf_status_t bf_pal_pltfm_all_interface_get(bf_pal_pltfm_reg_interface_t *intf) {
  if (!intf) return BF_INVALID_ARG;

  // Copy all the function pointers from the global database
  memcpy(intf, &pltfm_interface, sizeof(*intf));

  return BF_SUCCESS;
}

bf_status_t bf_pal_pm_front_port_ready_for_bringup(
    bf_dev_id_t dev_id, bf_pal_front_port_handle_t *port_hdl, bool is_ready) {
  bf_status_t sts;

  sts = bf_pm_pltfm_front_port_ready_for_bringup(dev_id, port_hdl, is_ready);
  if (sts != BF_SUCCESS) {
    BF_PAL_ERROR(
        "Unable to mark port ready for bringup for dev : %d : front port : "
        "%d/%d : %s (%d)",
        dev_id,
        port_hdl->conn_id,
        port_hdl->chnl_id,
        bf_err_str(sts),
        sts);
    return sts;
  }

  return BF_SUCCESS;
}

bf_status_t bf_pal_pm_front_port_eligible_for_autoneg(
    bf_dev_id_t dev_id,
    bf_pal_front_port_handle_t *port_hdl,
    bool is_eligible) {
  bf_status_t sts;

  sts = bf_pm_pltfm_front_port_eligible_for_autoneg(
      dev_id, port_hdl, is_eligible);
  if (sts != BF_SUCCESS) {
    BF_PAL_ERROR(
        "Unable to mark port eligible for autoneg for dev : %d : front port : "
        "%d/%d : %s (%d)",
        dev_id,
        port_hdl->conn_id,
        port_hdl->chnl_id,
        bf_err_str(sts),
        sts);
    return sts;
  }

  return BF_SUCCESS;
}

bf_status_t bf_pal_pm_front_port_num_lanes_get(
    bf_dev_id_t dev_id, bf_pal_front_port_handle_t *port_hdl, int *num_lanes) {
  bf_status_t sts;

  sts = bf_pm_pltfm_front_port_num_lanes_get(dev_id, port_hdl, num_lanes);
  if (sts != BF_SUCCESS) {
    BF_PAL_ERROR(
        "Unable to get the number of MAC lanes for dev : %d : front port : "
        "%d/%d : %s (%d)",
        dev_id,
        port_hdl->conn_id,
        port_hdl->chnl_id,
        bf_err_str(sts),
        sts);
    return sts;
  }

  return BF_SUCCESS;
}
