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
#include <stdint.h>
#include <stdbool.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>

#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_serdes_if.h>
#include <port_mgr/port_mgr_ucli.h>
#include "port_mgr_serdes_diag.h"
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
//#include "port_mgr.h"
#include <port_mgr/port_mgr_map.h>
#include "port_mgr_tof1_map.h"
// for aim_printf
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

/** \brief Display the port stats like error, LOS, sigok, etc.
 *
 * \param dev_id             : system-assigned identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param dev_port           : encoded port identifier
 * \param display_ucli_cookie: ucli context (typecasted as (void *)) in which to
 *display
 *
 * \return Status of the API call
 */
bf_status_t port_diag_prbs_stats_display(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         void *display_ucli_cookie) {
  int num_lanes;
  int ln = 0;
  bf_status_t sts = BF_SUCCESS;

  // sts = bf_serdes_prbs_stats_banner_display(display_ucli_cookie);
  // if (sts != BF_SUCCESS) {
  // return sts;
  //}
  bf_port_num_lanes_get(dev_id, dev_port, &num_lanes);
  for (ln = 0; ln < num_lanes; ln++) {
    sts = sd_diag_prbs_stats_display(
        dev_id, dev_port, (uint32_t)ln, display_ucli_cookie);
    if (sts != BF_SUCCESS) {
      return sts;
    }
  }
  return BF_SUCCESS;
}

/**\display perf
 *
 */
bf_status_t port_diag_perf_display(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   int fp,
                                   int ch,
                                   void *display_ucli_cookie) {
  port_mgr_port_t *port_p;
  int n_lanes, state, ln;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }
  // see if its up
  bf_port_oper_state_get_no_side_effect(dev_id, dev_port, &state);
  if (state == 0) {  // down, skip it
    aim_printf(&uc->pvs, "The port is down.\n");
    return BF_INVALID_ARG;
  }
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  sd_perf_banner(uc);
  for (ln = 0; ln < n_lanes; ln++) {
    sd_diag_perf_display(
        dev_id, dev_port, fp, ch, (uint32_t)ln, display_ucli_cookie);
  }
  return BF_SUCCESS;
}

/**\plot eye
 *
 */
bf_status_t port_diag_plot_eye(bf_dev_id_t dev_id,
                               bf_dev_port_t dev_port,
                               void *display_ucli_cookie) {
  // sd_plot_eye(dev_id, dev_port, uc);
  int n_lanes, state, ln;
  port_mgr_port_t *port_p;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }
  // see if its up
  bf_port_oper_state_get_no_side_effect(dev_id, dev_port, &state);
  if (state == 0) {  // down, skip it
    aim_printf(&uc->pvs, "The port is down.\n");
    return BF_INVALID_ARG;
  }
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  for (ln = 0; ln < n_lanes; ln++) {
    sd_diag_plot_eye(dev_id, dev_port, (uint32_t)ln, display_ucli_cookie);
  }
  return BF_SUCCESS;
}
/**\set DFE
 *
 */
bf_status_t port_diag_dfe_set(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              uint32_t lane,
                              bool set_all_lane,
                              uint32_t dfe_ctrl,
                              uint32_t hf_val,
                              uint32_t lf_val,
                              uint32_t dc_val,
                              void *display_ucli_cookie) {
  int n_lanes;
  port_mgr_port_t *port_p;
  bf_status_t sts;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  if (set_all_lane == true) {
    int ln;
    for (ln = 0; ln < n_lanes; ln++) {
      sts = bf_serdes_dfe_config_set(dev_id,
                                     dev_port,
                                     ln,
                                     (bf_sds_tof_dfe_ctrl_t)dfe_ctrl,
                                     hf_val,
                                     lf_val,
                                     dc_val);
      if (sts != BF_SUCCESS) {
        aim_printf(&uc->pvs, "Set DFE fails.\n");
      }
    }
  } else {
    if ((int)lane >= n_lanes) {
      aim_printf(&uc->pvs,
                 "The port only has %d lanes, the range of lane is 0-%d.\n",
                 n_lanes,
                 n_lanes - 1);
      return BF_INVALID_ARG;
    }
    sts = bf_serdes_dfe_config_set(dev_id,
                                   dev_port,
                                   (int)lane,
                                   (bf_sds_tof_dfe_ctrl_t)dfe_ctrl,
                                   hf_val,
                                   lf_val,
                                   dc_val);
    if (sts != BF_SUCCESS) {
      aim_printf(&uc->pvs, "Set DFE fails.\n");
    }
  }
  return BF_SUCCESS;
}

/**\set TX eq
 *
 */
bf_status_t port_diag_set_tx_eq(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                uint32_t lane,
                                bool set_all_lane,
                                int pre,
                                int atten,
                                int post,
                                int slew,
                                void *display_ucli_cookie) {
  int n_lanes, ln;
  port_mgr_port_t *port_p;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }

  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  if (set_all_lane == true) {
    for (ln = 0; ln < n_lanes; ln++) {
      sd_diag_set_tx_eq(dev_id,
                        dev_port,
                        (uint32_t)ln,
                        pre,
                        atten,
                        post,
                        slew,
                        display_ucli_cookie);
    }
  } else {
    if ((int)lane >= n_lanes) {
      aim_printf(&uc->pvs,
                 "The port only has %d lanes, the range of lane is 0-%d.\n",
                 n_lanes,
                 n_lanes - 1);
      return BF_INVALID_ARG;
    }
    sd_diag_set_tx_eq(
        dev_id, dev_port, lane, pre, atten, post, slew, display_ucli_cookie);
  }
  return BF_SUCCESS;
}

/**\set rx inv
 *
 */
bf_status_t port_diag_rx_inv_set(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint32_t lane,
                                 bool set_all_lane,
                                 int polarity,
                                 void *display_ucli_cookie) {
  int n_lanes, ln;
  port_mgr_port_t *port_p;
  bf_status_t sts;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  if (set_all_lane == true) {
    for (ln = 0; ln < n_lanes; ln++) {
      sts = bf_serdes_set_rx_invert(
          dev_id, dev_port, ln, (bool)(polarity > 0 ? 1 : 0));
      if (sts != BF_SUCCESS) {
        aim_printf(&uc->pvs, "Set rx inv fails.\n");
      }
    }
  } else {
    if ((int)lane >= n_lanes) {
      aim_printf(&uc->pvs,
                 "The port only has %d lanes, the range of lane is 0-%d.\n",
                 n_lanes,
                 n_lanes - 1);
      return BF_INVALID_ARG;
    }
    sts = bf_serdes_set_rx_invert(
        dev_id, dev_port, (int)lane, (bool)(polarity > 0 ? 1 : 0));
    if (sts != BF_SUCCESS) {
      aim_printf(&uc->pvs, "Set rx inv fails.\n");
    }
  }
  return BF_SUCCESS;
}

/**\set tx inv
 *
 */
bf_status_t port_diag_tx_inv_set(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint32_t lane,
                                 bool set_all_lane,
                                 int polarity,
                                 void *display_ucli_cookie) {
  int n_lanes, ln;
  port_mgr_port_t *port_p;
  bf_status_t sts;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  if (set_all_lane == true) {
    for (ln = 0; ln < n_lanes; ln++) {
      sts = bf_serdes_set_tx_invert(
          dev_id, dev_port, ln, (bool)(polarity > 0 ? 1 : 0));
      if (sts != BF_SUCCESS) {
        aim_printf(&uc->pvs, "Set tx inv fails.\n");
      }
    }
  } else {
    if ((int)lane >= n_lanes) {
      aim_printf(&uc->pvs,
                 "The port only has %d lanes, the range of lane is 0-%d.\n",
                 n_lanes,
                 n_lanes - 1);
      return BF_INVALID_ARG;
    }
    sts = bf_serdes_set_tx_invert(
        dev_id, dev_port, (int)lane, (bool)(polarity > 0 ? 1 : 0));
    if (sts != BF_SUCCESS) {
      aim_printf(&uc->pvs, "Set tx inv fails.\n");
    }
  }
  return BF_SUCCESS;
}

/**\set dfe ical
 *
 */
bf_status_t port_diag_dfe_ical_set(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t lane,
                                   bool set_all_lane,
                                   void *display_ucli_cookie) {
  int n_lanes, ln;
  port_mgr_port_t *port_p;
  bf_status_t sts;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }

  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  if (set_all_lane == true) {
    for (ln = 0; ln < n_lanes; ln++) {
      sts = bf_serdes_start_dfe_ical(dev_id, dev_port, ln);
      if (sts == BF_SUCCESS) {
        aim_printf(&uc->pvs, "ICAL DFE started..\n");
      }
    }
  } else {
    if ((int)lane >= n_lanes) {
      aim_printf(&uc->pvs,
                 "The port only has %d lanes, the range of lane is 0-%d.\n",
                 n_lanes,
                 n_lanes - 1);
      return BF_INVALID_ARG;
    }
    sts = bf_serdes_start_dfe_ical(dev_id, dev_port, (int)lane);
    if (sts == BF_SUCCESS) {
      aim_printf(&uc->pvs, "ICAL DFE started..\n");
    }
  }
  return BF_SUCCESS;
}

/**\set dfe pcal
 *
 */
bf_status_t port_diag_dfe_pcal_set(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t lane,
                                   bool set_all_lane,
                                   void *display_ucli_cookie) {
  int n_lanes, ln;
  port_mgr_port_t *port_p;
  bf_status_t sts;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }

  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  if (set_all_lane == true) {
    for (ln = 0; ln < n_lanes; ln++) {
      sts = bf_serdes_start_dfe_pcal(dev_id, dev_port, ln);
      if (sts == BF_SUCCESS) {
        aim_printf(&uc->pvs, "PCAL DFE started..\n");
      }
    }
  } else {
    if ((int)lane >= n_lanes) {
      aim_printf(&uc->pvs,
                 "The port only has %d lanes, the range of lane is 0-%d.\n",
                 n_lanes,
                 n_lanes - 1);
      return BF_INVALID_ARG;
    }
    sts = bf_serdes_start_dfe_pcal(dev_id, dev_port, (int)lane);
    if (sts == BF_SUCCESS) {
      aim_printf(&uc->pvs, "PCAL DFE started..\n");
    }
  }
  return BF_SUCCESS;
}

/**\switch to transmit PRBS
 *
 */
bf_status_t port_diag_chg_to_prbs(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  void *display_ucli_cookie) {
  // sd_port_chg_to_prbs(dev_id, dev_port, uc);
  int n_lanes, ln;
  port_mgr_port_t *port_p;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  // see if its a valid port
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) {
    // aim_printf(&uc->pvs, "The port is invalid.\n");
    return BF_INVALID_ARG;
  }
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  for (ln = 0; ln < n_lanes; ln++) {
    sd_diag_chg_to_prbs(dev_id, dev_port, (uint32_t)ln, display_ucli_cookie);
  }
  return BF_SUCCESS;
}
