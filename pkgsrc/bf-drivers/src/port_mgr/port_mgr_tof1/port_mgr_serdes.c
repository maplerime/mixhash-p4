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

#include <stdint.h>
#include <stdbool.h>

#include <dvm/bf_drv_intf.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_serdes_if.h>
#include <port_mgr/port_mgr_serdes_sbus_map.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_dev.h>
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof1_map.h"
#include "port_mgr_serdes.h"
#include "port_mgr_tof1_port.h"
#include "port_mgr_av_sd.h"
#include "port_mgr_mac.h"

int next_fp = 0;
int dev_port_for_fp[65];

/*************************************************************
 * port_mgr_serdes_bcast_fw_load
 *
 * Note:
 *   For some as yet unknown reason, having the SBM enabled
 *   at the time of device-add can occasionally (1:2000) cause
 *   a corruption of one or more PCIe PMA slices IMEM. Since
 *   the SBM is not required for normal operation it is left
 *   disabled here as a precaution.
 *   The only two functions it seems to perform are,
 *
 *     1) Speeding up eye plotting
 *     2) Managing round-robin adaptive PCAL.
 *
 *   (1) is not a mission critical function, used mostly
 *   during development.
 *   (2) can be implemented in SW (in fact Avago recommends
 *   against using the SBM for this feature.
 *   Since the SBM is now unused, we also don't bother
 *   downloading its firmware.
 *************************************************************
 * Note:
 *   To eliminate a race condition between some PCIe initialization
 *   operations the SBus CLK s sped up by decreasing the
 *   CLK divider from 3 -> 1.
 *************************************************************/
int port_mgr_serdes_bcast_fw_load(bf_dev_id_t dev_id) {
  int ring;
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  // disable both SBMs first
  port_mgr_av_sd_sbus_wr(dev_id, 0, sbm, 0x1, 0x40);
  port_mgr_av_sd_sbus_wr(dev_id, 1, sbm, 0x1, 0x40);
  port_mgr_log("SDS: SBMs disabled. No FW upload");

  // Lower Sbus CLK speed (increase divider from 3 -> 5)
  port_mgr_av_sd_sbus_wr(dev_id, 0, 0xFE, 0xA, 0x5);
  port_mgr_av_sd_sbus_wr(dev_id, 1, 0xFE, 0xA, 0x5);
  port_mgr_log("SDS: SBus CLK divider set to 5 (x32 mode)");

  for (ring = 0; ring < port_mgr_num_sbus_rings_get(dev_id); ring++) {
    int rc;

    // port_mgr_av_sd_load_firmware(
    //    dev_id, ring, sbm, dev_p->sbus_master_fw_ver, dev_p->sbus_master_fw);

    rc = port_mgr_av_sd_load_firmware(
        dev_id, ring, AVAGO_BROADCAST, dev_p->serdes_fw_ver, dev_p->serdes_fw);
    if (rc != 0) {
      return rc;
    }
  }
  return 0;
}

/*************************************************************
 * port_mgr_serdes_ucast_fw_load
 *
 *************************************************************/
int port_mgr_serdes_ucast_fw_load(bf_dev_id_t dev_id) {
  int ring;
  int sd;
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  // disable both SBMs first
  port_mgr_av_sd_sbus_wr(dev_id, 0, sbm, 0x1, 0x40);
  port_mgr_av_sd_sbus_wr(dev_id, 1, sbm, 0x1, 0x40);
  port_mgr_log("SDS: SBMs disabled. No FW upload");

  // Lower Sbus CLK speed (increase divider from 3 -> 4)
  port_mgr_av_sd_sbus_wr(dev_id, 0, 0xFE, 0xA, 0x5);
  port_mgr_av_sd_sbus_wr(dev_id, 1, 0xFE, 0xA, 0x5);
  port_mgr_log("SDS: SBus CLK divider set to 5 (x32 mode)");

  for (ring = 0; ring < port_mgr_num_sbus_rings_get(dev_id); ring++) {
    // port_mgr_av_sd_load_firmware(
    //    dev_id, ring, sbm, dev_p->sbus_master_fw_ver, dev_p->sbus_master_fw);

    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_load_firmware(
            dev_id, ring, sd, dev_p->serdes_fw_ver, dev_p->serdes_fw);
      }
    }
  }
  return 0;
}

/*************************************************************
 * port_mgr_serdes_init
 *
 *************************************************************/
bf_status_t port_mgr_serdes_init(bf_dev_id_t dev_id) {
  int tcp_mode = 0;
  bf_status_t rc = BF_SUCCESS;

#ifdef AVAGO_EVAL_BOARD
  /* eval board uses aacs and connects over tcp */
  tcp_mode = 1;
#endif  // AVAGO_EVAL_BOARD

  /* Configure the default (un-swizzled) serdes address
   * map based on internal wiring of the specific chip
   */
  port_mgr_configure_sbus_map(dev_id);

  port_mgr_av_sd_init_aapl(dev_id, tcp_mode);

  /* enumerate all nodes to find the ETH/PCIe serdes types */
  port_mgr_av_sd_identify_serdes_node_types(dev_id);

  /* load the necessary firmware to ETH serdes */
  rc = port_mgr_serdes_bcast_fw_load(dev_id);
  if (rc != BF_SUCCESS) return rc;

  /* place all ETH serdes nodes in a known quiet state */
  port_mgr_av_sd_set_initial_state(dev_id);
  return rc;
}

/*************************************************************
 * port_mgr_serdes_init_warm_boot
 *
 *************************************************************/
void port_mgr_serdes_init_warm_boot(bf_dev_id_t dev_id) {
  int tcp_mode = 0;

#ifdef AVAGO_EVAL_BOARD
  /* eval board uses aacs and connects over tcp */
  tcp_mode = 1;
#endif  // AVAGO_EVAL_BOARD

  /* Configure the default (un-swizzled) serdes address
   * map based on internal wiring of the specific chip
   */
  port_mgr_configure_sbus_map(dev_id);

  port_mgr_av_sd_init_aapl(dev_id, tcp_mode);

  /* enumerate all nodes to find the ETH/PCIe serdes types */
  port_mgr_av_sd_identify_serdes_node_types(dev_id);
}

/*************************************************************
 * port_mgr_serdes_set_is_serdes
 *
 *************************************************************/
void port_mgr_serdes_set_is_serdes(bf_dev_id_t dev_id, int ring, int sd) {
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  if ((dev_p != NULL) && (ring < port_mgr_num_sbus_rings_get(dev_id)) &&
      (sd < port_mgr_num_sbus_nodes_get(dev_id, ring))) {
    dev_p->is_serdes[ring][sd] = 1;
  }
}

/*************************************************************
 * port_mgr_serdes_is_serdes
 *
 *************************************************************/
int port_mgr_serdes_is_serdes(bf_dev_id_t dev_id, int ring, int sd) {
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  if ((dev_p != NULL) && (ring < port_mgr_num_sbus_rings_get(dev_id)) &&
      (sd < port_mgr_num_sbus_nodes_get(dev_id, ring))) {
    return dev_p->is_serdes[ring][sd];
  }
  return 0;
}

/*************************************************************
 * port_mgr_serdes_is_eth_serdes
 *
 *************************************************************/
int port_mgr_serdes_is_eth_serdes(bf_dev_id_t dev_id, int ring, int sd) {
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  if ((dev_p != NULL) && (ring < port_mgr_num_sbus_rings_get(dev_id)) &&
      (sd < port_mgr_num_sbus_nodes_get(dev_id, ring))) {
    if (dev_p->is_serdes[ring][sd]) {
      port_mgr_sbus_ip_type_e ip_type;
      int inst, sub_inst;

      port_mgr_find_mac_info_for(dev_id, ring, sd, &ip_type, &inst, &sub_inst);
      if (ip_type == IP_TYPE_ETH_PMA) {
        bf_dev_port_t dev_port;
        lld_err_t err = lld_sku_map_mac_ch_to_dev_port_id(
            dev_id, inst, sub_inst, &dev_port);
        if (err) return 0;
        if (!DEV_PORT_VALIDATE(dev_port)) return 0;
        return 1;
      }
    }
  }
  return 0;
}
/**\get dev_port from ring and sd
 *
 */
static bf_status_t port_mgr_map_serdes_to_dev_port(bf_dev_id_t dev_id,
                                                   uint32_t ring,
                                                   uint32_t sd,
                                                   bf_dev_port_t *dev_port,
                                                   uint32_t *ch) {
  if (dev_port == NULL || ch == NULL) return BF_INVALID_ARG;
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  if ((dev_p != NULL) && ((int)ring < port_mgr_num_sbus_rings_get(dev_id)) &&
      ((int)sd < port_mgr_num_sbus_nodes_get(dev_id, ring))) {
    if (dev_p->is_serdes[ring][sd]) {
      port_mgr_sbus_ip_type_e ip_type;
      int inst, sub_inst;
      port_mgr_find_mac_info_for(dev_id, ring, sd, &ip_type, &inst, &sub_inst);
      if (ip_type == IP_TYPE_ETH_PMA) {
        // try to find the ch
        int ln;
        port_mgr_mac_block_t *mac_block_p =
            port_mgr_tof1_map_idx_to_mac_block(dev_id, inst);
        if (mac_block_p == NULL) {
          port_mgr_log("Error : invalid device id %d\n", dev_id);
          return BF_INVALID_ARG;
        }
        for (ln = 0; ln < 4; ln++) {
          if (mac_block_p->sds_node[mac_block_p->rx_lane_map[ln]] == (int)sd) {
            *ch = (uint32_t)ln;
            break;
          }
        }
        if (ln == 4) return BF_NOT_SUPPORTED;
        // at this point, we have the right *ch and mac_block
        lld_err_t err =
            lld_sku_map_mac_ch_to_dev_port_id(dev_id, inst, *ch, dev_port);
        if (err) return BF_NOT_SUPPORTED;
        if (!DEV_PORT_VALIDATE(*dev_port)) return BF_NOT_SUPPORTED;
        return BF_SUCCESS;
      }
    }
  }
  return BF_NOT_SUPPORTED;
}
/** \brief Return the DFE config info for the given ring/sd
 *
 * \param dev_id  : system-assigned device identifier (0..BF_MAX_DEV_COUNT-1)
 * \param ring    : serdes ring (0-1)
 * \param sd      : (serdes addr (0-254)
 * \return: BF_SUCCESS          : serdes register read successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port never added or port > 255
 * \return: BF_INVALID_ARG: lane > # of serdes lanes on this port (in this mode)
 *
 */
bf_status_t port_mgr_serdes_tof_dfe_cfg_get(bf_dev_id_t dev_id,
                                            uint32_t ring,
                                            uint32_t sd,
                                            uint32_t *dfe_ctrl,
                                            uint32_t *hf_val,
                                            uint32_t *lf_val,
                                            uint32_t *dc_val) {
  port_mgr_serdes_t *serdes_p;

  bf_status_t sts;
  bf_dev_port_t dev_port;
  uint32_t ln, ch;

  if (dfe_ctrl == NULL) return BF_INVALID_ARG;
  if (hf_val == NULL) return BF_INVALID_ARG;
  if (lf_val == NULL) return BF_INVALID_ARG;
  if (dc_val == NULL) return BF_INVALID_ARG;
  *dfe_ctrl = 0;
  *hf_val = 0;
  *lf_val = 0;
  *dc_val = 0;

#if 0
  serdes_p = port_mgr_tof1_map_ring_sd_to_hw_serdes(dev_id, ring, sd);
#endif
  sts = port_mgr_map_serdes_to_dev_port(dev_id, ring, sd, &dev_port, &ch);
  if (sts != BF_SUCCESS) return BF_INVALID_ARG;
  sts = port_mgr_map_dev_port_to_lane(dev_id, &dev_port, ch, &ln);
  if (sts != BF_SUCCESS) return BF_INVALID_ARG;
  serdes_p = port_mgr_map_port_lane_to_serdes(dev_id, dev_port, ln);

  if (serdes_p == NULL) return BF_INVALID_ARG;

  /*serdes_p = port_mgr_map_ring_sd_to_serdes(dev_id, ring, sd);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  */
  *dfe_ctrl = serdes_p->dfe_ctrl;
  *hf_val = serdes_p->hf_val;
  *lf_val = serdes_p->lf_val;
  *dc_val = serdes_p->dc_val;
  return BF_SUCCESS;
}

/** \brief Set the DFE config info for the given ring/sd
 *
 * \param dev_id  : system-assigned device identifier (0..BF_MAX_DEV_COUNT-1)
 * \param ring    : serdes ring (0-1)
 * \param sd      : (serdes addr (0-254)
 * \return: BF_SUCCESS          : serdes register read successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port never added or port > 255
 * \return: BF_INVALID_ARG: lane > # of serdes lanes on this port (in this mode)
 *
 */
bf_status_t port_mgr_serdes_tof_dfe_cfg_set(bf_dev_id_t dev_id,
                                            uint32_t ring,
                                            uint32_t sd,
                                            uint32_t dfe_ctrl,
                                            uint32_t hf_val,
                                            uint32_t lf_val,
                                            uint32_t dc_val) {
  port_mgr_serdes_t *serdes_p;

#if 0
  serdes_p = port_mgr_tof1_map_ring_sd_to_hw_serdes(dev_id, ring, sd);
#endif
  bf_status_t sts;
  bf_dev_port_t dev_port;
  uint32_t ln, ch;

  sts = port_mgr_map_serdes_to_dev_port(dev_id, ring, sd, &dev_port, &ch);
  if (sts != BF_SUCCESS) return BF_INVALID_ARG;
  sts = port_mgr_map_dev_port_to_lane(dev_id, &dev_port, ch, &ln);
  if (sts != BF_SUCCESS) return BF_INVALID_ARG;
  serdes_p = port_mgr_map_port_lane_to_serdes(dev_id, dev_port, ln);

  if (serdes_p == NULL) return BF_INVALID_ARG;
  /*
  serdes_p = port_mgr_map_ring_sd_to_serdes(dev_id, ring, sd);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  */
  serdes_p->dfe_ctrl = dfe_ctrl;
  serdes_p->hf_val = hf_val;
  serdes_p->lf_val = lf_val;
  serdes_p->dc_val = dc_val;
  return BF_SUCCESS;
}

/** \brief Return the DFE config info for the given ring/sd
 *
 * \param dev_id  : system-assigned device identifier (0..BF_MAX_DEV_COUNT-1)
 * \param ring    : serdes ring (0-1)
 * \param sd      : (serdes addr (0-254)
 *
 * \return: BF_SUCCESS          : serdes register read successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port never added or port > 255
 * \return: BF_INVALID_ARG: lane > # of serdes lanes on this port (in this mode)
 *
 */
bf_status_t port_mgr_serdes_tof_dfe_cfg_default_set(bf_dev_id_t dev_id,
                                                    uint32_t ring,
                                                    uint32_t sd) {
  port_mgr_serdes_tof_dfe_cfg_set(dev_id, ring, sd, 0, 0, 0, 0);
  return BF_SUCCESS;
}

/** \brief Dump the DFE state from a serdes slice. Note: this
 *         API ultimately calls printf to display the output.
 *
 * \param chip: int         : system-assigned device identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param port: int         : physical port (0-255)
 * \param lane: int         : logical lane w/in port (0-3, depending upon mode)
 * \return: BF_SUCCESS          : serdes register read successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port never added or port > 255
 * \return: BF_INVALID_ARG: lane > # of serdes lanes on this port (in this mode)
 *
 */
bf_status_t port_mgr_serdes_log_dfe(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_log_dfe_st(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

bf_status_t port_mgr_serdes_hw_cfg_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port) {
  int ln = 0;
  port_mgr_serdes_t *serdes_p, *hw_serdes_p;
  bf_dev_pipe_t pipe_id;
  int port_id, mac_block, ch, is_cpu_port;
  bf_status_t sts;
  port_mgr_mac_block_t *mac_block_p;
  bf_sds_rx_term_t rx_term;
  bool rx_los_en;
  float pll_ovrclk;

  /* This routine reads all 4 serdes channels in a MAC blk. Hence the dev port
     must be a multiple of 4. Just return immediately if not a multiple of 4*/
  if ((dev_port % lld_get_chnls_dev_port(dev_id, dev_port)) != 0) {
    return BF_SUCCESS;
  }

  sts = port_mgr_tof1_map_dev_port_to_all(
      dev_id, dev_port, &pipe_id, &port_id, &mac_block, &ch, &is_cpu_port);
  if (sts != 0) {
    return sts;
  }

  mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  if (!mac_block_p) return BF_INVALID_ARG;

  // Get the serdes params in the hardware
  for (ln = 0; ln < 4; ln++) {
    serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, dev_port, ln);
    if (!serdes_p) return BF_INVALID_ARG;
    hw_serdes_p = port_mgr_tof1_map_port_lane_to_hw_serdes_allow_unassigned(
        dev_id, dev_port, ln);
    if (!hw_serdes_p) return BF_INVALID_ARG;

    // Get the Tx PLL Refclk source for non CPU ports only as there is no
    // control for the CPU MAC
    if (dev_port != MAKE_DEV_PORT(0, 64)) {
      if (bf_serdes_tx_pll_clksel_get(
              dev_id, dev_port, ln, &hw_serdes_p->tx_pll_clk)) {
        port_mgr_log(
            "Error : Unable to get the tx pll clk source for dev %d dev_port "
            "%d : ln "
            ": %d",
            dev_id,
            dev_port,
            ln);
        return BF_HW_COMM_FAIL;
      }
    }

    // Get the TX parameters
    if (bf_serdes_tx_drv_inv_get(dev_id, dev_port, ln, &hw_serdes_p->tx_inv)) {
      port_mgr_log(
          "Error : Unable to get the tx inv param for dev % dev_port %d : ln : "
          "%d",
          dev_id,
          dev_port,
          ln);
      return BF_HW_COMM_FAIL;
    }

    if (bf_serdes_tx_drv_attn_get(dev_id,
                                  dev_port,
                                  ln,
                                  &hw_serdes_p->tx_eq_atten,
                                  &hw_serdes_p->tx_eq_post,
                                  &hw_serdes_p->tx_eq_pre)) {
      port_mgr_log(
          "Error : Unable to get the tx eq params for dev %d dev_port %d : ln "
          ": %d",
          dev_id,
          dev_port,
          ln);
      return BF_HW_COMM_FAIL;
    }

    // Get the RX parameters
    if (bf_serdes_rx_afe_inv_get(dev_id, dev_port, ln, &hw_serdes_p->rx_inv)) {
      port_mgr_log(
          "Error : Unable to get the rx inv param for dev %d dev_port %d : ln "
          ": %d",
          dev_id,
          dev_port,
          ln);
      return BF_HW_COMM_FAIL;
    }

    if (bf_serdes_rx_afe_los_thres_get(dev_id,
                                       dev_port,
                                       ln,
                                       &rx_los_en,
                                       (int *)&hw_serdes_p->rx_sig_ok_thresh)) {
      port_mgr_log(
          "Error : Unable to get the rx sig ok thresh for dev %d dev_port %d : "
          "ln : "
          "%d",
          dev_id,
          dev_port,
          ln);
      return BF_HW_COMM_FAIL;
    }
    hw_serdes_p->rx_sig_ok_thresh = ((hw_serdes_p->rx_sig_ok_thresh - 46) / 22);

    if (bf_serdes_rx_afe_term_get(dev_id, dev_port, ln, &rx_term)) {
      port_mgr_log(
          "Error : Unable to get the rx term for dev %d dev_port %d : ln : %d",
          dev_id,
          dev_port,
          ln);
      return BF_HW_COMM_FAIL;
    }
    hw_serdes_p->rx_term = rx_term;

    if (bf_serdes_pll_ovrclk_get_hw(dev_id, dev_port, ln, &pll_ovrclk)) {
      port_mgr_log(
          "Error : Unable to get the pll overclock config for dev %d dev_port "
          "%d : ln : %d",
          dev_id,
          dev_port,
          ln);
      return BF_HW_COMM_FAIL;
    }
    hw_serdes_p->pll_ovrclk = pll_ovrclk;
  }

  (void)rx_los_en;
  return BF_SUCCESS;
}

bf_status_t port_mgr_serdes_delta_compute(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    bf_ha_port_reconcile_info_t *recon_info) {
  port_mgr_log("%s:%d Computing serdes delta for dev %d port %d",
               __func__,
               __LINE__,
               dev_id,
               dev_port);
  int ln = 0;
  port_mgr_serdes_t *serdes_p, *hw_serdes_p;
  bf_dev_pipe_t pipe_id;
  int port_id, phy_mac_block, ch, is_cpu_port;
  bf_status_t sts;
  port_mgr_mac_block_t *mac_block_p;
  port_mgr_port_t *port_p;

  sts = port_mgr_tof1_map_dev_port_to_all(
      dev_id, dev_port, &pipe_id, &port_id, &phy_mac_block, &ch, &is_cpu_port);
  if (sts != 0) {
    return sts;
  }

  mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, phy_mac_block);
  if (!mac_block_p) return BF_INVALID_ARG;

  int num_lanes = port_mgr_tof1_get_num_lanes(dev_id, dev_port);
  if (num_lanes == 0) return BF_INVALID_ARG;

  for (ln = 0; ln < num_lanes; ln++) {
    if ((mac_block_p->tx_lane_map[ch + ln] !=
         mac_block_p->hw_tx_lane_map[ch + ln]) ||
        (mac_block_p->rx_lane_map[ch + ln] !=
         mac_block_p->hw_rx_lane_map[ch + ln])) {
      /* Indicates that there is a mismatch in the already programmed lane
         mapping and the one being replayed */
      recon_info->ca = BF_HA_CA_PORT_FLAP;
      port_mgr_log(
          "%s:%d:%d:%d:%d Serdes tx_lane_map change detected: %d (SW) : %d "
          "(HW) ",
          __func__,
          __LINE__,
          dev_id,
          dev_port,
          phy_mac_block,
          mac_block_p->tx_lane_map[ch + ln],
          mac_block_p->hw_tx_lane_map[ch + ln]);
      port_mgr_log(
          "%s:%d:%d:%d:%d Serdes rx_lane_map change detected: %d (SW) : %d "
          "(HW) ",
          __func__,
          __LINE__,
          dev_id,
          dev_port,
          phy_mac_block,
          mac_block_p->rx_lane_map[ch + ln],
          mac_block_p->hw_rx_lane_map[ch + ln]);
      return BF_SUCCESS;
    }
  }

  // Get the serdes params in the hardware
  for (ln = 0; ln < num_lanes; ln++) {
    serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, dev_port, ln);
    if (!serdes_p) return BF_INVALID_ARG;
    hw_serdes_p = port_mgr_tof1_map_port_lane_to_hw_serdes_allow_unassigned(
        dev_id, dev_port, ln);
    if (!hw_serdes_p) return BF_INVALID_ARG;

    /* Check if a port actually existed at this particular serdes lane.
       Otherwise we might end up comparing an unprogramed serdes lane cfg
       and flap the port unnecessarily */
    port_p =
        port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port + ln);
    if (port_p == NULL) {
      port_mgr_log("%s:%d Map dev %d port %d to port_p failed",
                   __func__,
                   __LINE__,
                   dev_id,
                   dev_port);
      return BF_INVALID_ARG;
    }
    if (!port_p->sw.assigned) continue;

    /* Compare all the params and if any of them differ mark the corrective
       action as FLAP*/
    if ((hw_serdes_p->tx_inv != serdes_p->tx_inv) ||
        (hw_serdes_p->rx_inv != serdes_p->rx_inv)) {
      recon_info->ca = BF_HA_CA_PORT_FLAP;
      port_mgr_log(
          "%s:%d:%d:%d: Serdes tx_inv change detected: %d (SW) : %d (HW) ",
          __func__,
          __LINE__,
          dev_id,
          dev_port,
          serdes_p->tx_inv,
          hw_serdes_p->tx_inv);
      port_mgr_log(
          "%s:%d:%d:%d: Serdes rx_inv change detected: %d (SW) : %d (HW) ",
          __func__,
          __LINE__,
          dev_id,
          dev_port,
          serdes_p->rx_inv,
          hw_serdes_p->rx_inv);
      return BF_SUCCESS;
    }

    if (bf_serdes_get_pll_div(dev_id, dev_port, hw_serdes_p->pll_ovrclk) !=
        bf_serdes_get_pll_div(dev_id, dev_port, serdes_p->pll_ovrclk)) {
      recon_info->ca = BF_HA_CA_PORT_FLAP;
      port_mgr_log(
          "%s:%d:%d:%d: Serdes pll_div change detected: %f (SW) : %f (HW) ",
          __func__,
          __LINE__,
          dev_id,
          dev_port,
          serdes_p->pll_ovrclk,
          hw_serdes_p->pll_ovrclk);
      return BF_SUCCESS;
    }
  }

  /* At this point all the hw serdes cfg is same as the replayed one. Hence
     mark the corrective action as NONE and proceed*/
  port_mgr_log("%s:%d:%d:%d No Serdes delta detected for port",
               __func__,
               __LINE__,
               dev_id,
               dev_port);
  recon_info->ca = BF_HA_CA_PORT_NONE;
  return BF_SUCCESS;
}

/** \brief Download the new serdes firmware for the port
 *
 * \param dev_id: int         : system-assigned device identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: int         : physical port (0-255)
 *
 * \return: BF_SUCCESS          : serdes firmware downloaded successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: dev_port never added or port > 255
 *
 */
bf_status_t port_mgr_serdes_firmware_upgrade(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t fw_ver,
                                             char *fw_path) {
  port_mgr_serdes_t *serdes_p;
  bf_status_t sts;
  int num_lanes = port_mgr_tof1_get_num_lanes(dev_id, dev_port);
  if (num_lanes == 0) return BF_INVALID_ARG;
  int ln;

  // Get the serdes params in the hardware
  for (ln = 0; ln < num_lanes; ln++) {
    serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
    if (serdes_p == NULL) {
      port_mgr_log("%s:%d Map dev %d port %d to serdes failed",
                   __func__,
                   __LINE__,
                   dev_id,
                   dev_port);
      return BF_INVALID_ARG;
    }

    bf_sys_assert(serdes_p->tx_sd == serdes_p->rx_sd);  // FIXME: Check added to
                                                        // catch the case of
                                                        // asymmetric serdes
    sts = bf_serdes_ucast_firmware_load(
        dev_id, serdes_p->ring, serdes_p->tx_sd, fw_ver, fw_path);
    if (sts != BF_SUCCESS) {
      return sts;
    }
  }

  return BF_SUCCESS;
}

/** \brief Apply the delta determined during config replay (Called during port
 *         enable in the delta push phase)
 *
 * \param dev_id: int         : system-assigned device identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: int         : physical port (0-255)
 *
 * \return: BF_SUCCESS          : serdes settings applied successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port never added or port > 255
 *
 */
bf_status_t port_mgr_serdes_delta_settings_apply(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port) {
  int ln = 0;
  port_mgr_serdes_t *serdes_p, *hw_serdes_p;
  bf_dev_pipe_t pipe_id;
  int port_id, phy_mac_block, ch, is_cpu_port;
  bf_status_t sts;
  port_mgr_mac_block_t *mac_block_p;
  uint32_t log_mac_block = 0;

  sts = port_mgr_tof1_map_dev_port_to_all(
      dev_id, dev_port, &pipe_id, &port_id, &phy_mac_block, &ch, &is_cpu_port);
  if (sts != 0) {
    return sts;
  }

  mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, phy_mac_block);
  if (!mac_block_p) return BF_INVALID_ARG;

  if (lld_sku_map_phy2log_mac_block(dev_id, phy_mac_block, &log_mac_block) !=
      0) {
    return BF_INVALID_ARG;
  }

  int num_lanes = port_mgr_tof1_get_num_lanes(dev_id, dev_port);
  if (num_lanes == 0) return BF_INVALID_ARG;

  for (ln = 0; ln < num_lanes; ln++) {
    if ((mac_block_p->tx_lane_map[ch + ln] !=
         mac_block_p->hw_tx_lane_map[ch + ln]) ||
        (mac_block_p->rx_lane_map[ch + ln] !=
         mac_block_p->hw_rx_lane_map[ch + ln])) {
      /* Indicates that there is a mismatch in the already programmed lane
         mapping and the one which was replayed */
      bf_mac_block_lane_map_t lane_map;
      lane_map.tx_lane[0] = mac_block_p->tx_lane_map[0];
      lane_map.tx_lane[1] = mac_block_p->tx_lane_map[1];
      lane_map.tx_lane[2] = mac_block_p->tx_lane_map[2];
      lane_map.tx_lane[3] = mac_block_p->tx_lane_map[3];
      lane_map.rx_lane[0] = mac_block_p->rx_lane_map[0];
      lane_map.rx_lane[1] = mac_block_p->rx_lane_map[1];
      lane_map.rx_lane[2] = mac_block_p->rx_lane_map[2];
      lane_map.rx_lane[3] = mac_block_p->rx_lane_map[3];
      lane_map.dev_port = dev_port;
      /*Clockwork fix :  We have to fill conn_id and chanl_id as below function
       * use that */

      port_mgr_port_t *port_p = NULL;
      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (!port_p) {
        return BF_INVALID_ARG;
      }

      lane_map.fp_conn_id = port_p->fp_conn_id;
      lane_map.fp_chnl_id = port_p->fp_chnl_id;

      sts = bf_port_lane_map_set(dev_id, log_mac_block, &lane_map);
      if (sts != BF_SUCCESS) {
        port_mgr_log(
            "Error: Unable to set the lane map for device: %d, port:%d, err: "
            "%s (%d) at "
            "%s:%d",
            dev_id,
            dev_port,
            bf_err_str(sts),
            sts,
            __func__,
            __LINE__);
        return sts;
      }
      return BF_SUCCESS;
    }
  }

  // Get the serdes params in the hardware
  for (ln = 0; ln < num_lanes; ln++) {
    serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, dev_port, ln);
    if (!serdes_p) return BF_INVALID_ARG;
    hw_serdes_p = port_mgr_tof1_map_port_lane_to_hw_serdes_allow_unassigned(
        dev_id, dev_port, ln);
    if (!hw_serdes_p) return BF_INVALID_ARG;

    if (hw_serdes_p->tx_inv != serdes_p->tx_inv) {
      sts = bf_serdes_tx_drv_inv_set(dev_id, dev_port, ln, serdes_p->tx_inv);
      if (sts != BF_SUCCESS) {
        port_mgr_log(
            "Error : Unable to set the tx inv param for device %d, port:%d at "
            "%s:%d",
            dev_id,
            dev_port,
            __func__,
            __LINE__);
        return sts;
      }
    }

    if (hw_serdes_p->rx_inv != serdes_p->rx_inv) {
      sts = bf_serdes_rx_afe_inv_set(dev_id, dev_port, ln, serdes_p->rx_inv);
      if (sts != BF_SUCCESS) {
        port_mgr_log(
            "Error : Unable to set the rx inv param for device %d, port:%d at "
            "%s:%d",
            dev_id,
            dev_port,
            __func__,
            __LINE__);
        return sts;
      }
    }
    if (bf_serdes_get_pll_div(dev_id, dev_port, hw_serdes_p->pll_ovrclk) !=
        bf_serdes_get_pll_div(dev_id, dev_port, serdes_p->pll_ovrclk)) {
      sts =
          bf_serdes_pll_ovrclk_set(dev_id, dev_port, ln, serdes_p->pll_ovrclk);
      if (sts != BF_SUCCESS) {
        port_mgr_log(
            "Error : Unable to set the rx inv param for device %d, port:%d at "
            "%s:%d",
            dev_id,
            dev_port,
            __func__,
            __LINE__);
        return sts;
      }
    }
  }

  return BF_SUCCESS;
}

/** \brief Get Transmit and Receive lane mapping in a port
 *
 * Gets the port (quad lane) based lane mapper config.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[out] tx_lane[4]   : TX physical lane # (0-3) for logical lane 0-3
 * \param[out] rx_lane[4]   : RX physical lane # (0-3) for logical lane 0-3
 *
 */
bf_status_t port_mgr_tof1_serdes_lane_map_get(
    bf_dev_id_t dev_id,
    bf_mac_block_id_t mac_block,
    bf_mac_block_lane_map_t *lane_map) {
  bf_dev_port_t dev_port;

  bf_status_t bf_status =
      bf_port_map_mac_to_dev_port(dev_id, mac_block, 0, &dev_port);
  if (bf_status != BF_SUCCESS) return bf_status;

  // Retrieves TX/RX lane mapper config
  port_mgr_mac_get_lane_remap(dev_id,
                              dev_port,
                              (int *)&lane_map->rx_lane[0],
                              (int *)&lane_map->rx_lane[1],
                              (int *)&lane_map->rx_lane[2],
                              (int *)&lane_map->rx_lane[3],
                              (int *)&lane_map->tx_lane[0],
                              (int *)&lane_map->tx_lane[1],
                              (int *)&lane_map->tx_lane[2],
                              (int *)&lane_map->tx_lane[3]);
  return BF_SUCCESS;
}

/** \brief Set Transmit and Receive lane mapping in a port
 *
 * Configures the port (quad lane) based lane mapper to map physical lanes
 * to logical lanes.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  mac_block    : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane_map:    : lane map struct ptr
 *
 */
bf_status_t port_mgr_tof1_serdes_lane_map_set(
    bf_dev_id_t dev_id,
    bf_mac_block_id_t mac_block,
    bf_mac_block_lane_map_t *lane_map) {
  // Implementation Notes:
  //
  // - Configures lane remapper inside MAC (0x5F02-Lane Remap RX,
  //  0x5F82 Lane Remap TX)
  // - Configure KR back channel interface mapping
  //  (ethsds_core_cntl.core_to_cntl[11:8])
  // - Note that if TX/RX of the same lane is not a logical pair,
  //  KR training needs to be done asymmetrically (Int 0x4.7:6)
  //
  /* Notes on KR back channel configuration:
  * See also JIRA Avago-248

  RX?s o_core_status[15:10] (req out) goes to TX?s i_core_to_cntl[15:10
  (req_ in) and TX?s o_core_status[9:8] (ack out) goes to
  RX?s i_core_to_cntl[9:8] (ack in).

  core_to_cntl  bits[11:8]

  This is the mux select to associate physical TX/RX pairs to logical pairs
  for KR Link Training. Physical and logical TX/RX pairing are different as
  TX and RX lanes can be remapped independently through registers
  Lane Remap TX and Lane Remap RX.

  Set bits[9:8]   to the channel number (0 to 3) of the TX logically paired
  to this PHY's RX
  Set bits[11:10] to the channel number (0 to 3) of the RX logically paired
  to this PHY's TX
  */
  bf_dev_port_t dev_port;
  bf_dev_port_t mac_ch_0_dev_port;
  uint32_t msk_check, phy_mac_block;
  int ln;
  port_mgr_mac_block_t *mac_block_p;
  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) return BF_INVALID_ARG;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;
  // convert logical mac-blk to physical
  lld_sku_map_log2phy_mac_block(dev_id, mac_block, &phy_mac_block);

  mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, phy_mac_block);
  if (mac_block_p == NULL) return BF_INVALID_ARG;

  // make sure all physical lanes are represented
  msk_check = (1 << lane_map->rx_lane[0]) | (1 << lane_map->rx_lane[1]) |
              (1 << lane_map->rx_lane[2]) | (1 << lane_map->rx_lane[3]);
  if (msk_check != 0xF) {
    port_mgr_log(
        "SDS : Error for mac-block %d. Rx-lanes not unique across channels "
        "%d/%2d : %d:-: Lane map: Rx: <%d, %d, %d, %d> : Tx: <%d, %d, "
        "%d, "
        "%d>",
        mac_block,
        lane_map->fp_conn_id,
        lane_map->fp_chnl_id,
        dev_id,
        lane_map->rx_lane[0],
        lane_map->rx_lane[1],
        lane_map->rx_lane[2],
        lane_map->rx_lane[3],
        lane_map->tx_lane[0],
        lane_map->tx_lane[1],
        lane_map->tx_lane[2],
        lane_map->tx_lane[3]);

    return BF_INVALID_ARG;
  }

  msk_check = (1 << lane_map->tx_lane[0]) | (1 << lane_map->tx_lane[1]) |
              (1 << lane_map->tx_lane[2]) | (1 << lane_map->tx_lane[3]);
  if (msk_check != 0xF) {
    port_mgr_log(
        "SDS : Error for mac-block %d. Tx-lanes not unique across channels "
        "%d/%2d : %d:-: Lane map: Rx: <%d, %d, %d, %d> : Tx: <%d, %d, "
        "%d, "
        "%d>",
        mac_block,
        lane_map->fp_conn_id,
        lane_map->fp_chnl_id,
        dev_id,
        lane_map->rx_lane[0],
        lane_map->rx_lane[1],
        lane_map->rx_lane[2],
        lane_map->rx_lane[3],
        lane_map->tx_lane[0],
        lane_map->tx_lane[1],
        lane_map->tx_lane[2],
        lane_map->tx_lane[3]);

    return BF_INVALID_ARG;
  }

  dev_port = lane_map->dev_port;
  mac_ch_0_dev_port = (dev_port & (~3));
  if ((lane_map->fp_conn_id - 1) < 65) {
    dev_port_for_fp[(lane_map->fp_conn_id - 1)] = mac_ch_0_dev_port;
  }

  port_mgr_log(
      "SDS : %d/%2d : %d:%3x:-: Lane map: Rx: <%d, %d, %d, %d> : Tx: <%d, %d, "
      "%d, "
      "%d>",
      lane_map->fp_conn_id,
      lane_map->fp_chnl_id,
      dev_id,
      mac_ch_0_dev_port,
      lane_map->rx_lane[0],
      lane_map->rx_lane[1],
      lane_map->rx_lane[2],
      lane_map->rx_lane[3],
      lane_map->tx_lane[0],
      lane_map->tx_lane[1],
      lane_map->tx_lane[2],
      lane_map->tx_lane[3]);
  /* Don't touch hardware during cfg replay */
  if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_CFG_REPLAY) {
    port_mgr_mac_set_lane_remap(dev_id,
                                mac_ch_0_dev_port,
                                lane_map->rx_lane[0],
                                lane_map->rx_lane[1],
                                lane_map->rx_lane[2],
                                lane_map->rx_lane[3],
                                lane_map->tx_lane[0],
                                lane_map->tx_lane[1],
                                lane_map->tx_lane[2],
                                lane_map->tx_lane[3]);
  }
  // set sbus addresses for tx and rx sides (possibly different)
  for (ln = 0; ln < 4; ln++) {
    port_mgr_serdes_t *serdes_p;

    serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, mac_ch_0_dev_port, ln);
    if (serdes_p == NULL) return BF_INVALID_ARG;

    serdes_p->rx_sd = mac_block_p->sds_node[lane_map->rx_lane[ln]];
    serdes_p->tx_sd = mac_block_p->sds_node[lane_map->tx_lane[ln]];

    mac_block_p->rx_lane_map[ln] = lane_map->rx_lane[ln];
    mac_block_p->tx_lane_map[ln] = lane_map->tx_lane[ln];
    /* Don't touch hardware during cfg replay*/
    if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_CFG_REPLAY) {
      // Configure kr-back-channel mux accordingly
      // just to set it to something sensible
      port_mgr_mac_kr_backchannel_mux_set(dev_id,
                                          mac_ch_0_dev_port + ln,  // FIXME
                                          lane_map->tx_lane[ln],
                                          lane_map->rx_lane[ln]);
    }
  }
  return BF_SUCCESS;
}
