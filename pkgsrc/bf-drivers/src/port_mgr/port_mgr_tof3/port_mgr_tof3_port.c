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

#include <target-sys/bf_sal/bf_sys_intf.h>
#include <dvm/bf_drv_intf.h>
#include <pipe_mgr/pktgen_intf.h>
#include <dvm/dvm_intf.h>
#include <tof3_regs/tof3_reg_drv.h>
#include <tof3_regs/tof3_mem_drv.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <port_mgr/bf_port_if.h>
//#include <port_mgr/bf_tof3_serdes_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_dev.h>
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_log.h>
#include <port_mgr/port_mgr_mac_stats.h>
#include <port_mgr/port_mgr_port.h>
#include "port_mgr_tof3_physical_dev.h"
#include "port_mgr_tof3_map.h"
#include "port_mgr_tof3_port.h"
#include "port_mgr_tof3_tmac.h"
#include "port_mgr_tof3_serdes.h"
#include <lld/bf_dev_if.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_interrupt_if.h>

//#include "tmac_ctrs.h"
#include "../port_mgr_mac_stats.h"

static void port_mgr_tof3_default_config(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         bf_port_attributes_t *port_attrib);

/** \brief port_mgr_tof3_port_add
 *         Add a Tofino2 port to the system
 *         note: dvm will call this API twice, first for ingress, then for
 *egress.
 *
 * [ PRE_ENABLE ]
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: dev_port
 *
 * \return: BF_SUCCESS     : port added successfully
 * \return: BF_INVALID_ARG : invalid dev_id
 * \return: BF_INVALID_ARG : invalid_dev_port
 * \return: BF_INVALID_ARG : resources in-use
 *
 */
bf_status_t port_mgr_tof3_port_add(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   bf_port_attributes_t *port_attrib,
                                   bf_port_cb_direction_t direction) {
  port_mgr_err_t sts;
  int tmac, ch, port_id;
  int is_cpu_port = false;
  port_mgr_port_t *port_p;
  bool is_valid;
  bf_status_t rc;
  int nserdes_per_mac;

  if (port_attrib == NULL)
    return BF_INVALID_ARG;

  if ((direction != BF_PORT_CB_DIRECTION_INGRESS) &&
      (direction != BF_PORT_CB_DIRECTION_EGRESS))
    return BF_INVALID_ARG;

  // Check for ports port_mgr really handles. This callback gets called for
  // any port_add, including recirc, etc. Just ignore those.
  sts = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                     &ch, &is_cpu_port);
  if (sts != PORT_MGR_OK) {
    return BF_SUCCESS;
  }

  port_mgr_log("PRT :%d:%3d:-: Add <spd=%d : n_lanes=%d : dir=%d>", dev_id,
               dev_port, port_attrib->port_speeds, port_attrib->n_lanes,
               direction);

// Hack until CPU port is supported.
#if defined(DEVICE_IS_EMULATOR)
  if (is_cpu_port)
    return BF_SUCCESS;
#endif

  // ignore non-MAC ports
  if ((port_id < 8) && (!is_cpu_port))
    return BF_SUCCESS;

  // validate speed/fec combination
  rc = bf_port_fec_type_validate(dev_id, dev_port, port_attrib->port_speeds,
                                 port_attrib->port_fec_types, &is_valid);
  if ((rc != BF_SUCCESS) || (!is_valid)) {
    return BF_INVALID_ARG;
  }

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  if (port_p == NULL)
    return BF_INVALID_ARG;

  if (direction == BF_PORT_CB_DIRECTION_EGRESS) {
    // verify resources are available
    port_mgr_tmac_t *tmac_p;
    nserdes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);
    uint32_t ln_needed, ch_needed;
    if (is_cpu_port) {
      ln_needed = port_p->sw.n_lanes / 2;
    } else {
      if (nserdes_per_mac == 8) {
        ln_needed = port_p->sw.n_lanes / 2;
      } else {
        ln_needed = port_p->sw.n_lanes;
      }
    }
    ch_needed = ((1 << ln_needed) - 1);

    tmac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
    if ((tmac_p->ch_in_use & (ch_needed << ch)) != 0) {
      return BF_INVALID_ARG;
    }
    tmac_p->ch_in_use |= (ch_needed << ch);
  }
  port_p->sw.speed = port_attrib->port_speeds;
  port_p->sw.fec = port_attrib->port_fec_types;

  switch (port_mgr_dev_ha_stage_get(dev_id)) {
  case PORT_MGR_HA_CFG_REPLAY:
    if (direction == BF_PORT_CB_DIRECTION_INGRESS) {
      /* During warm init, fill up the port_p->sw during EGRESS call only*/
      return BF_SUCCESS;
    }
    port_mgr_tof3_default_config(dev_id, dev_port, port_attrib);
    return BF_SUCCESS;
  case PORT_MGR_HA_DELTA_PUSH:
    /* dvm will call this API twice, first for egress, then for ingress.
     * The port will be added on the ingress
     */
    if (direction == BF_PORT_CB_DIRECTION_EGRESS) {
      return BF_SUCCESS;
    }
    /* verify required resources are uncommitted only when we are not in delta
       push phase because this would have already been populated during cfg
       replay. Hence if we check again during push delta, then this check will
       surely fail. We can assume that right things already exist in
       mac_block_p->ch_in_use
       and simply proceed*/
    break;
  case PORT_MGR_HA_NONE:
    /* dvm will call this API twice, first for egress, then for ingress.
     * The port will be added on the ingress
     */
    if (direction == BF_PORT_CB_DIRECTION_EGRESS) {
      return BF_SUCCESS;
    }
    port_mgr_tof3_default_config(dev_id, dev_port, port_attrib);
    nserdes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);
    int nserdes_per_lane = (nserdes_per_mac == 4) ? 1 : 2;
    port_mgr_tof3_tmac_config(dev_id, tmac, ch, port_attrib->port_speeds,
                              port_attrib->port_fec_types, port_attrib->n_lanes,
                              nserdes_per_lane);
    /* Software workaround for HW issue where TM datapath is not zeroed
       as part of port initialization. Below fix will zeroed the internal TM
       datapth */
    port_mgr_tof3_clear_pfc_data_path(dev_id, dev_port);
    break;
  case PORT_MGR_HA_DELTA_COMPUTE:
  case PORT_MGR_HA_MAX:
  default:
    port_mgr_log("Error : Invalid HA stage %d while trying to add a port",
                 port_mgr_dev_ha_stage_get(dev_id));
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

static void port_mgr_port_remove_basic_setup(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             port_mgr_port_t *port_p) {
  // reset port speed and fec mode
  port_p->sw.speed = BF_SPEED_NONE;
  port_p->sw.fec = BF_FEC_TYP_NONE;

  port_p->sw.assigned = 0;
  (void)dev_id;
  (void)dev_port;
}

/** \brief port_mgr_tof3_port_remove
 *         Remove a Tofino port from the system
 *         note: dvm will call this API twice, first for ingress, then for
 *egress.
 *
 * \param dev_id: int : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port: int : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 *
 * \return: BF_SUCCESS : port removed successfully
 *
 */
bf_status_t port_mgr_tof3_port_remove(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bf_port_cb_direction_t direction) {
  int tmac, ch, is_cpu_port, port_id;
  port_mgr_port_t *port_p;
  uint32_t nserdes_per_mac;
  uint32_t ln_needed, ch_needed;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p_allow_unassigned(dev_id);
  if (dev_p == NULL)
    return BF_INVALID_ARG;

  port_mgr_err_t err = port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, &port_id, &tmac, &ch, &is_cpu_port);
  if (err) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_DELTA_PUSH) {
    port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  } else {
    port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  }
  if (!port_p)
    return BF_INVALID_ARG;

#if 0
  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }
#endif

  if (direction == BF_PORT_CB_DIRECTION_INGRESS) {
    // release resources
    port_mgr_tmac_t *tmac_p;
    nserdes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);
    if (is_cpu_port) {
      ln_needed = port_p->sw.n_lanes / 2;
    } else {
      if (nserdes_per_mac == 8) {
        ln_needed = port_p->sw.n_lanes / 2;
      } else {
        ln_needed = port_p->sw.n_lanes;
      }
    }
    ch_needed = ((1 << ln_needed) - 1);

    tmac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
    if ((tmac_p->ch_in_use & (ch_needed << ch)) != (ch_needed << ch)) {
      return BF_INVALID_ARG; // somehow deleting more than one port
    }
    tmac_p->ch_in_use &= ~(ch_needed << ch);
  }

  switch (port_mgr_dev_ha_stage_get(dev_id)) {
  case PORT_MGR_HA_CFG_REPLAY:
    if (direction == BF_PORT_CB_DIRECTION_INGRESS) {
      /* During warm init, do nothing during the INGRESS call*/
      return BF_SUCCESS;
    }
    port_mgr_tof3_tmac_de_config(dev_id, tmac, ch, port_p->sw.speed,
                                 port_p->sw.n_lanes);

    return BF_SUCCESS;
  case PORT_MGR_HA_DELTA_PUSH:
    /* consider the case when a port is being deleted because it was already
       in the hw but was never replayed back. In this case the port_p->sw
       data structure would be invalid. Hence we need to go by the port_p->hw
       data strcuture */
    // if (!port_p->hw.assigned) {
    /* Indicates that the port was never added in the past as well. Not sure
       if this condition will ever occur */
    return BF_SUCCESS;
    //}
    if (direction == BF_PORT_CB_DIRECTION_INGRESS) {
      /* If the port was enabled in hw, then disable it before removing */
      // if (port_p->hw.enabled) { /* we check hw instead of sw*/
      //  port_mgr_port_disable(dev_id, dev_port);
      //}
      // update txff_ctrl, set force flush mode
      // mac_block_p->txff_ctrl = port_mgr_construct_txff_ctrl(dev_id,
      // dev_port);
      // port_mgr_mac_pgm_txff_ctrl(dev_id, mac_block);
      return BF_SUCCESS;
    } else {
      // determine freed resources (mac/serdes)
      // ch_reqd = port_mgr_ch_reqd_by_speed(dev_id, dev_port,
      // port_p->hw.speed);
    }
    break;
  case PORT_MGR_HA_NONE:
    // PORT-SEQ If direction == BF_PORT_CB_DIRECTION_INGRESS port-disable,
    // enable flush in MAC
    // PORT-SEQ If direction == BF_PORT_CB_DIRECTION_EGRESS disable MAC
    // channel , Disable Flush
    if (direction == BF_PORT_CB_DIRECTION_INGRESS) {
      // if enabled, disable before removing
      // if (port_p->sw.enabled) {
      //  port_mgr_port_disable(dev_id, dev_port);
      //}
      // update txff_ctrl, set force flush mode
      // mac_block_p->txff_ctrl = port_mgr_construct_txff_ctrl(dev_id,
      // dev_port);
      // port_mgr_mac_pgm_txff_ctrl(dev_id, mac_block);
      return BF_SUCCESS;
    } else {
      // determine freed resources (mac/serdes)
      port_mgr_tof3_tmac_de_config(dev_id, tmac, ch, port_p->sw.speed,
                                   port_p->sw.n_lanes);
      port_mgr_port_remove_basic_setup(dev_id, dev_port, port_p);
      // port_mgr_mac_stats_clear_historical_ctrs(&port_p->mac_stat_historical);
    }
    break;
  case PORT_MGR_HA_DELTA_COMPUTE:
  case PORT_MGR_HA_MAX:
  default:
    port_mgr_log("Error : Invalid HA stage %d while trying to remove a port",
                 port_mgr_dev_ha_stage_get(dev_id));
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_port_enable
 *         Enable a Tofino port. This kicks off the port
 *         bring-up sequence.
 *
 * [ PRE_ENABLE ]
 *
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: dev_port
 * \param enable  : true= Enable, false= Disable
 *
 * \return: BF_SUCCESS     : port enabled successfully
 * \return: BF_INVALID_ARG : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 *
 */
bf_status_t port_mgr_tof3_port_enable(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, bool enable) {

  port_mgr_log("PRT :%d:%3d:-: Raw %s", dev_id, dev_port,
               enable ? "Enable" : "Disable");

  // bf_status_t sts = BF_SUCCESS;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);
  if (!dev_p)
    return BF_INVALID_ARG;

  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p)
    return BF_INVALID_ARG;
  int ch, is_cpu_port, port_id;
  uint32_t tmac;

  port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, (int *)&tmac,
                               &ch, &is_cpu_port);

#if 0
  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }
#endif

  port_mgr_log("PRT :%d:%3d:-: %s", dev_id, dev_port,
               enable ? "Enable" : "Disable");

  if (!enable) {
    return port_mgr_tof3_port_disable(dev_id, dev_port);
  }

  switch (port_mgr_dev_ha_stage_get(dev_id)) {
  case PORT_MGR_HA_CFG_REPLAY:
    port_p->sw.enabled = 1;
    return BF_SUCCESS;
  case PORT_MGR_HA_DELTA_PUSH:
    // sts = port_mgr_serdes_delta_settings_apply(dev_id, dev_port);
    // if (sts != BF_SUCCESS) {
    //  port_mgr_log(
    //      "Error : Unable to apply serdes delta settings for port %d "
    //      "during port enable",
    //      dev_port);
    //  return sts;
    //}
    break;
  case PORT_MGR_HA_NONE:
    port_p->sw.enabled = 1;
    port_mgr_tof3_tmac_enable(dev_id, tmac, ch);
    /* Software workaround for HW issue where TM datapath is not zeroed
       as part of port initialization. Below fix will zeroed the internal TM
       datapth */
    port_mgr_tof3_clear_pfc_data_path(dev_id, dev_port);
    break;
  case PORT_MGR_HA_DELTA_COMPUTE:
  case PORT_MGR_HA_MAX:
  default:
    port_mgr_log("Error : Invalid HA stage %d while trying to enable a port",
                 port_mgr_dev_ha_stage_get(dev_id));
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_port_disable
 *         Disable a Tofino port. This brings the port down
 *         and disables the transmitter, bringing the link-
 *         partner down.
 *
 * \param dev_id: int         : system-assigned identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param port: int         : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 *
 * \return: BF_SUCCESS     : port disabled successfully
 * \return: BF_INVALID_ARG : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 *
 */
bf_status_t port_mgr_tof3_port_disable(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port) {
  // int ch_reqd;
  int mac_block, ch, is_cpu_port, port_id;
  // int num_lanes, ln;
  uint32_t tmac;
  port_mgr_port_t *port_p;
  //  bf_rmon_counter_array_t hw_ctrs;
  //  bf_status_t bf_status;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p_allow_unassigned(dev_id);
  if (!dev_p)
    return BF_INVALID_ARG;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  if (!port_p)
    return BF_INVALID_ARG;

  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_DELTA_PUSH) {
  } else {
    port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
    if (!port_p)
      return BF_INVALID_ARG;
  }

  port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &mac_block,
                               &ch, &is_cpu_port);
  tmac = mac_block;

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  switch (port_mgr_dev_ha_stage_get(dev_id)) {
  case PORT_MGR_HA_CFG_REPLAY:
    // port_mgr_port_disable_basic_setup(dev_id, dev_port, port_p);
    port_mgr_tof3_tmac_disable(dev_id, tmac, ch);
    return BF_SUCCESS;
  case PORT_MGR_HA_DELTA_PUSH:
    // ch_reqd = port_mgr_ch_reqd_by_speed(dev_id, dev_port,
    // port_p->hw.speed);
    // num_lanes = num_mac_chn_consumed(ch_reqd);

    // if (port_p->hw.enabled) { /* Here we check the hw value*/
    // first shut off the serdes
    // for (ln = 0; ln < num_lanes; ln++) {
    // turn off the PLLs and output driver
    // bf_serdes_set_rx_tx_and_tx_output_en_allow_unassigned(
    //    dev_id,
    //    dev_port,
    //    ln,
    //    false /*rx_en*/,
    //    false /*tx_en*/,
    //    false /*tx_output_en*/);
    // if (ln == 0) {
    // disable AN just in case
    //  bf_serdes_autoneg_stop(dev_id, dev_port);
    //}
    // bf_serdes_link_training_set(dev_id, dev_port, ln, false);
    //}
    //}
    break;
  case PORT_MGR_HA_NONE: {
#if RR_STATS_WRK
    tmac_rmon_ctr_t tmac_ctrs;
    bf_rmon_counter_array_t common_ctrs;
#endif

    // disconnect tmac from serdes Rx
    if (port_p->sw.enabled) {
      bf_port_force_sig_ok_low_set(dev_id, dev_port);
    }

#if RR_STATS_WRK
    // update historical counters before MAC ch disable (which clears the HW
    // counters)
    bf_status = bf_port_mac_stats_hw_only_sync_get(dev_id, dev_port, &hw_ctrs);
    if (bf_status != BF_SUCCESS) {
      port_mgr_log("%d:%3d: Error : %d : updating historical ctrs", dev_id,
                   dev_port, bf_status);
    } else {
      port_mgr_mac_stats_update_historical_ctrs(&hw_ctrs,
                                                &port_p->mac_stat_historical);
    }

    tmac = mac_block;
    tmac_ctrs_rmon_get(dev_id, tmac, ch, &tmac_ctrs);

    // Need to convert tmac to tmac counter array
    port_mgr_tof3_tmac_to_tmac_ctr_copy((uint64_t *)&tmac_ctrs,
                                        (uint64_t *)&common_ctrs);

    port_mgr_mac_stats_update_historical_ctrs(&common_ctrs,
                                              &port_p->mac_stat_historical);
#endif

    port_mgr_tof3_tmac_disable(dev_id, tmac, ch);

    if (port_p->sw.oper_state) {
      port_mgr_link_dn_actions(dev_id, dev_port);
    }
    port_p->sw.enabled = 0;
    port_p->sw.oper_state = 0;
  } break;
  case PORT_MGR_HA_DELTA_COMPUTE:
  case PORT_MGR_HA_MAX:
  default:
    port_mgr_log("Error : Invalid HA stage %d while trying to add a port",
                 port_mgr_dev_ha_stage_get(dev_id));
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_port_oper_state_get
 *         Get the operational stateof a tof3 port.
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param up      : operational state, false=down, true=up
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 *
 */
bf_status_t port_mgr_tof3_port_oper_state_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              bool *up) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    *up = false;
    return BF_INVALID_ARG;
  }
  // if port defined but not enabled, just call it down
  if (!port_p->sw.enabled) {
    *up = false;
    return BF_SUCCESS;
  }

  // Ignore recirc port.
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    *up = false;
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }
  port_mgr_tof3_tmac_link_state_get(dev_id, tmac, ch, up);
  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_port_loopback_set
 **************************************************************************/
bf_status_t port_mgr_tof3_port_loopback_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            bf_loopback_mode_e mode) {
  int mac, ch;
  bf_status_t rc;

  // Ignore recirc port.
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  switch (mode) {
  case BF_LPBK_NONE:
  case BF_LPBK_MAC_NEAR:
  case BF_LPBK_MAC_FAR:
  case BF_LPBK_PCS_NEAR:
  case BF_LPBK_PIPE:
    break;
  default:
    return BF_INVALID_ARG;
  }
  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &mac, &ch,
                                    NULL);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  rc = port_mgr_tof3_tmac_loopback_set(dev_id, mac, ch, mode);
  return rc;
}

static bf_status_t
port_mgr_tof3_set_default_port_state_itr(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, void *unused) {
  (void)unused;
  port_mgr_port_t *port_p =
      port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  if (port_p)
    port_mgr_set_default_port_state(dev_id, dev_port);
  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_default_config
 *
 * Set default config for port-add
 **************************************************************************/
static void port_mgr_tof3_default_config(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         bf_port_attributes_t *port_attrib) {
  port_mgr_port_t *port_p =
      port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);

  if (port_p == NULL) {
    port_mgr_log("PRT :%d:%3d port_p is NULL", dev_id, dev_port);
    return;
  }
  // init all sw config to defaults
  port_mgr_set_default_port_state(dev_id, dev_port);
  port_p->sw.tx_mtu = PORT_MGR_TOF3_MAX_RX_FRAME_SZ;
  port_p->sw.rx_mtu = PORT_MGR_TOF3_MAX_TX_FRAME_SZ;

  // configuration is valid
  port_p->sw.assigned = 1;

  // speed must be set in port for the txff_ctrl calculation (below)
  port_p->sw.speed = port_attrib->port_speeds;
  port_p->sw.fec = port_attrib->port_fec_types;

  // clear historical counter stats
  // port_mgr_tof3_mac_stats_clear_historical_ctrs(&port_p->mac_stat_historical);
}

/*****************************************************************************
 * port_mgr_tof3_port_read_counter
 *
 * Sync read of tmac RMON counter
 ****************************************************************************/
bf_status_t port_mgr_tof3_port_read_counter(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            bf_rmon_counter_t ctr_id,
                                            uint64_t *ctr_value) {
  int tmac, ch;
  int is_cpu_port = false;

  port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                               &is_cpu_port);

  return port_mgr_tof3_tmac_read_counter(dev_id, tmac, ch, ctr_id, ctr_value);
}

/**************************************************************************
 * port_mgr_tof3_port_iter
 *
 * iterate over all logical ports calling the passed fn for each
 **************************************************************************/
bf_status_t port_mgr_tof3_port_iter(bf_dev_id_t dev_id,
                                    port_mgr_tof3_iter_cb fn, void *userdata) {
  int rc, port;
  uint32_t pipe, num_pipes;
  bf_dev_port_t dev_port;

  rc = lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  if (rc != 0)
    return BF_INVALID_ARG;

  for (pipe = 0; pipe < num_pipes; pipe++) {
    for (port = 0; port < 72; port++) {
      dev_port = MAKE_DEV_PORT(pipe, port);
      fn(dev_id, dev_port, userdata);
    }
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_port_lane_map_set
 *
 * Save and program the lane map in both tmac and serdes. The tmac need only
 * be programmed once, at init. The serdes lane map would be lost if a group
 * reset is issued (which is required for new FW dnld). So we also save the
 * lane map here so the serdes can retreive it if needed.
 ****************************************************************************/
bf_status_t port_mgr_tof3_port_lane_map_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t phys_tx_ln[8],
                                            uint32_t phys_rx_ln[8]) {
  uint32_t ln, n_lanes;
  int mac, ch, port_id;
  int is_cpu_port;
  bf_status_t rc;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);

  if (!dev_p)
    return BF_INVALID_ARG;

  n_lanes = lld_get_num_serdes_per_mac(dev_id, dev_port);
  bool is_mac_112g_serdes_mode = (n_lanes == 8) ? false : true;

  // Check for ports port_mgr really handles. This callback gets called for
  // any port_add, including recirc, etc. Just ignore those.
  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &mac, &ch,
                                    &is_cpu_port);
  if (rc != PORT_MGR_OK) {
    return BF_SUCCESS;
  }

  // ignore non-MAC ports
  if ((port_id < 8) && (!is_cpu_port))
    return BF_SUCCESS;

  for (ln = 0; ln < n_lanes; ln++) {
    dev_p->tmac[mac].phys_tx_ln[ln] = phys_tx_ln[ln];
    dev_p->tmac[mac].phys_rx_ln[ln] = phys_rx_ln[ln];
  }
  dev_p->tmac[mac].max_serdes_per_mac = n_lanes;

  port_mgr_log("%d:%3d: mac:%0d: Serdes and Physical Lane Map:", dev_id,
               dev_port, mac);
  if ((is_cpu_port) || (is_mac_112g_serdes_mode)) {
    port_mgr_log("       -|-|-|-|3|2|1|0|");
    port_mgr_log("       -|-|-|-|%d|%d|%d|%d| Tx(phy)", phys_tx_ln[3],
                 phys_tx_ln[2], phys_tx_ln[1], phys_tx_ln[0]);
    port_mgr_log("       -|-|-|-|%d|%d|%d|%d| Rx(phy)", phys_rx_ln[3],
                 phys_rx_ln[2], phys_rx_ln[1], phys_rx_ln[0]);
  } else {
    port_mgr_log("       7|6|5|4|3|2|1|0|");
    port_mgr_log("       %d|%d|%d|%d|%d|%d|%d|%d| Tx(phy)", phys_tx_ln[7],
                 phys_tx_ln[6], phys_tx_ln[5], phys_tx_ln[4], phys_tx_ln[3],
                 phys_tx_ln[2], phys_tx_ln[1], phys_tx_ln[0]);
    port_mgr_log("       %d|%d|%d|%d|%d|%d|%d|%d| Rx(phy)", phys_rx_ln[7],
                 phys_rx_ln[6], phys_rx_ln[5], phys_rx_ln[4], phys_rx_ln[3],
                 phys_rx_ln[2], phys_rx_ln[1], phys_rx_ln[0]);
  }
  /* Don't touch hardware during cfg replay */
  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    return BF_SUCCESS;
  }

  // set phys lane map in tmac
  port_mgr_tof3_tmac_lane_map_set(dev_id, mac, phys_tx_ln, phys_rx_ln, n_lanes);

  return rc;
}

/*****************************************************************************
 * port_mgr_tof3_port_lane_map_get
 *
 * Retreive the lane map previously set (it is saved in the physical_device
 * struct with each tmac.
 ****************************************************************************/
bf_status_t port_mgr_tof3_port_lane_map_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t phys_tx_ln[8],
                                            uint32_t phys_rx_ln[8]) {
  int tmac, port_id, ln;
  int is_cpu_port;
  bf_status_t rc;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);

  // Check for ports port_mgr really handles. This callback gets called for
  // any port_add, including recirc, etc. Just ignore those.
  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    NULL, &is_cpu_port);
  if (rc != PORT_MGR_OK) {
    return BF_SUCCESS;
  }

  // ignore non-MAC ports
  if ((port_id < 8) && (!is_cpu_port))
    return BF_SUCCESS;

  for (ln = 0; ln < 8; ln++) {
    phys_tx_ln[ln] = dev_p->tmac[tmac].phys_tx_ln[ln];
    phys_rx_ln[ln] = dev_p->tmac[tmac].phys_rx_ln[ln];
  }
  return BF_SUCCESS;
  ;
}

/*****************************************************************************
 * port_mgr_tof3_get_num_lanes
 *
 * Internal function to return the number of "logical" lanes on a port.
 * Intention is to be called to determine "for loop" limits.
 *
 *****************************************************************************/
int port_mgr_tof3_get_num_lanes(bf_dev_id_t dev_id, bf_dev_port_t dev_port) {
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (!port_p) {
    return 0;
  } else {
    return port_p->sw.n_lanes;
  }
}

/*****************************************************************************
 * port_mgr_tof3_port_sigovrd_set
 *
 *****************************************************************************/
bf_status_t port_mgr_tof3_port_sigovrd_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t num_lanes,
                                           bf_sigovrd_fld_t ovrd_val) {
  bf_status_t rc;
  int tmac, ch;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != PORT_MGR_OK) {
    return BF_SUCCESS;
  }
  port_mgr_tof3_tmac_sigovrd_set(dev_id, tmac, ch, num_lanes, ovrd_val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_port_tx_reset_set
 *
 *****************************************************************************/
bf_status_t port_mgr_tof3_port_tx_reset_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port) {
  bf_status_t rc;
  int tmac, ch;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != PORT_MGR_OK) {
    return BF_SUCCESS;
  }
  port_mgr_tof3_tmac_tx_reset_set(dev_id, tmac, ch);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_port_rx_reset_set
 *
 *****************************************************************************/
bf_status_t port_mgr_tof3_port_rx_reset_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port) {
  bf_status_t rc;
  int tmac, ch;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != PORT_MGR_OK) {
    return BF_SUCCESS;
  }
  port_mgr_tof3_tmac_rx_reset_set(dev_id, tmac, ch);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_set_default_all_ports_state
 *
 *****************************************************************************/
bf_status_t port_mgr_tof3_set_default_all_ports_state(bf_dev_id_t dev_id) {
  bf_status_t rc = port_mgr_tof3_port_iter(
      dev_id, port_mgr_tof3_set_default_port_state_itr, NULL);
  if (BF_SUCCESS != rc)
    return rc;

  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);
  if (!dev_p)
    return BF_INVALID_ARG;
  for (int i = 0; i < TOF3_NUM_TMAC; ++i)
    dev_p->tmac[i].ch_in_use = 0;
  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_port_flowcontrol_set
 *
 **************************************************************************/
bf_status_t port_mgr_tof3_port_flowcontrol_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port) {
  int tmac, ch;
  bf_status_t rc;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  port_mgr_tof3_tmac_flowcontrol_set(dev_id, tmac, ch);

  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_port_mac_int_en_set
 *
 * Warning:
 * enable bit controls ALL channels on tmac. Must use Comira
 * reg to enable/disable specific channels.
 **************************************************************************/
bf_status_t port_mgr_tof3_port_mac_int_en_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port, bool on) {
  int tmac, ch;
  bf_status_t rc;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  port_mgr_tof3_tmac_int_en_set(dev_id, tmac, ch, on);

  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_port_local_fault_int_en_set
 *
 **************************************************************************/
bf_status_t port_mgr_tof3_port_local_fault_int_en_set(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      bool en) {
  int tmac, ch;
  bf_status_t rc;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  port_mgr_tof3_tmac_local_fault_int_en_set(dev_id, tmac, ch, en);

  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_port_remote_fault_int_en_set
 *
 **************************************************************************/
bf_status_t port_mgr_tof3_port_remote_fault_int_en_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       bool en) {
  int tmac, ch;
  bf_status_t rc;

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac, &ch,
                                    NULL);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  port_mgr_tof3_tmac_remote_fault_int_en_set(dev_id, tmac, ch, en);

  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_port_default_handler_for_mac_ints
 *
 **************************************************************************/
void port_mgr_tof3_port_default_handler_for_mac_ints(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port_unused, uint32_t reg,
    uint32_t set_int_bits_unused, void *userdata) {
#if 0  
  int state;
  bf_dev_port_t dev_port;
  uint32_t mac_stn_id, tmac, ch;

  /* Must compute dev_port based on the MAC stn_id in the register and
  * the associated channel number from polling each channels interrupt
  * registers */
  mac_stn_id = (reg >> 18) & 0x3F;              // ~0xff03ffff)
  if ((mac_stn_id > 0) && (mac_stn_id < 33)) {  // tmac
    uint32_t tmac = mac_stn_id;
    /* there is only a single interrupt bit associated with ALL channel
    * interrupts on a given tmac. So we have to poll each channel for
    * its interupts */
    for (ch = 0; ch < 8; ch++) {
      lld_err_t err;
      bool possible_state_chg = false;

      err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, tmac, ch, &dev_port);
      if (err) {
        port_mgr_log("%d: Error %d: converting tmac%d ch%d to dev_port",
                     dev_id,
                     err,
                     tmac,
                     ch);
        continue;
      } else {
        uint32_t mapped_ch;
        bf_status_t rc;

        /* We dont know yet which dev_ports are enabled. Not sure it matters.
        * If the channel is interupting we need to mask it annyway.
        */
        rc = port_mgr_map_dev_port_to_all(dev_id,
                                          dev_port,
                                          NULL,
                                          NULL,
                                          (int *)&tmac,
                                          (int *)&mapped_ch,
                                          NULL);
        // if (rc != BF_SUCCESS) continue;
        (void)mapped_ch;
        (void)rc;
      }
      port_mgr_tof3_tmac_handle_interrupts(
          dev_id, tmac, ch, &possible_state_chg);
      if (possible_state_chg) {
        /* determine whether or not the real port state has changed, and
        * if so, execute any necessary actions ("link-up-actions" or
        * "link-dn-actions") */
        bf_port_oper_state_get_and_issue_callbacks(dev_id, dev_port, &state);
        /* The "Fault" interrupt is a level.
        *  - if the port is now "up" enable the Fault interrupt.
        *  - If the port is now "down", mask the Fault interrupt
        */
        if (state) {  // up
          port_mgr_tof3_port_local_fault_int_en_set(dev_id, dev_port, true);
        } else {
          port_mgr_tof3_port_local_fault_int_en_set(dev_id, dev_port, false);
        }
      }
    }
  } else {// tmac
  }

#endif

  (void)dev_id;
  (void)reg;
  (void)set_int_bits_unused;
  (void)dev_port_unused;
  (void)userdata;
}

static uint32_t port_mgr_tof3_mac_interrupt_handle(
    bf_dev_id_t dev, bf_subdev_id_t subdev_id, uint32_t intr_address,
    uint32_t intr_status_val, uint32_t enable_hi_addr, uint32_t enable_lo_addr,
    void *userdata) {
  int ch = 0;
  int tmac = (intr_address >> 18) & 0x3F;

  while (ch < 4) {
    if (intr_status_val & (TF3_INTR_CH0_LINK_DOWN_RFAULT << ch)) {
      port_mgr_tof3_tmac_handle_interrupts(dev, subdev_id, ch, tmac,
                                           intr_status_val);
    }
    ch++;
  }

  port_mgr_submodule_log(
      PORT_MGR_SUBMOD_INTR,
      "%s dev %d int_addr 0x%x int_status_val 0x%x en_hi 0x%x en_lo 0x%x",
      __func__, dev, intr_address, intr_status_val, enable_hi_addr,
      enable_lo_addr);
  lld_subdev_write_register(dev, subdev_id, intr_address, intr_status_val);
  (void)enable_lo_addr;
  (void)userdata;
  return 0;
}

/**************************************************************************
 * port_mgr_tof3_port_register_default_handler_for_mac_ints
 *
 **************************************************************************/
bf_status_t
port_mgr_tof3_port_register_default_handler_for_mac_ints(bf_dev_id_t dev_id) {
  int row = 0, ret = 0;
  for (row = 0; row < 0x21; row++) {
    ret = lld_int_register_cb(
        dev_id, 0, offsetof(tof3_reg, eth400g[row].eth400g_mac.link_intr),
        &port_mgr_tof3_mac_interrupt_handle, NULL);
  }

  port_mgr_port_bind_int_bh_wakeup_callback(
      dev_id, port_mgr_port_default_int_bh_wakeup_cb);
  return ret;
}
/** \brief port_mgr_tof3_port_stats_clear
 *         Clear the hardware rmon stats
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 *
 */
bf_status_t port_mgr_tof3_port_mac_stats_clear(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  port_mgr_tof3_tmac_clear_stats(dev_id, tmac, ch);

  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_pcs_status_get
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param pcs: tof3 pcs struct
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 */
bf_status_t port_mgr_tof3_pcs_status_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         bf_tof3_pcs_status_t *pcs) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  port_mgr_tof3_tmac_pcs_status_get(dev_id, tmac, ch, pcs);
  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_fec_status_get
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param fec: tof3 fec struct
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 */
bf_status_t port_mgr_tof3_fec_status_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         bf_tof3_fec_status_t *fec) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  port_mgr_tof3_tmac_fec_status_get(dev_id, tmac, ch, fec);
  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_fec_status_get
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param fec: tof3 fec struct
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 */
bf_status_t port_mgr_tof3_port_tmac_reset_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t val) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  port_mgr_tof3_tmac_reset_set(dev_id, tmac, ch, val);
  return BF_SUCCESS;
}

/***************************************************************************
 *
 ***************************************************************************/
int port_mgr_tof3_tmac_num_lanes_per_ch(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port) {
  int num_lanes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);

  if ((dev_port >= lld_get_min_cpu_port(dev_id)) &&
      (dev_port <= lld_get_max_cpu_port(dev_id))) {
    return 2;
  } else if (num_lanes_per_mac == 8) {
    return 2;
  } else {
    return 1;
  }
}

bf_status_t port_mgr_tof3_port_fec_lane_symb_err_counter_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t n_ctrs,
    uint64_t symb_err[16]) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  // this is required to compute the offset of the proper counters in the array
  // because for osfp ports the channel numbers are compressed.
  //
  int lanes_per_ch = port_mgr_tof3_tmac_num_lanes_per_ch(dev_id, dev_port);
  port_mgr_tof3_tmac_fec_lane_symb_err_counter_get(dev_id, tmac, ch, n_ctrs,
                                                   symb_err, lanes_per_ch);
  return BF_SUCCESS;
}

/**************************************************************************
 * port_mgr_tof3_ignore_fault
 **************************************************************************/
bf_status_t port_mgr_tof3_ignore_fault(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, bool en) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  port_mgr_tof3_tmac_ignore_fault(dev_id, tmac, ch, en);
  return BF_SUCCESS;
}

/** \brief port_mgr_tof3_clear_pfc_data_path
 *  Software workaround for HW issue where TM datapath is not zeroed
 *  as part of port initialization. Below fix will zeroed the internal TM
 * datapth
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG : port > LLD_MAX_PORTS-1
 */
bf_status_t port_mgr_tof3_clear_pfc_data_path(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port) {
  int is_cpu_port, port_id;
  int tmac, ch;
  port_mgr_port_t *port_p;
  bf_status_t rc;

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  // if no port defined, its an error
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // Ignore recirc port
  bool recirc = false;
  rc = bf_recirculation_get(dev_id, dev_port, &recirc);
  if (rc == BF_SUCCESS && recirc) {
    return BF_SUCCESS;
  }

  rc = port_mgr_map_dev_port_to_all(dev_id, dev_port, NULL, &port_id, &tmac,
                                    &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  // make sure its a real mac
  if ((port_id < 8) && (!is_cpu_port)) {
    // ignore non-MAC ports
    return BF_SUCCESS;
  }

  port_mgr_tof3_tmac_clear_pfc_data_path(dev_id, tmac, ch);
  return BF_SUCCESS;
}
