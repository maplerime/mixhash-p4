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

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <tofino_regs/tofino.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <avago/aapl.h>

#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_serdes_if.h>
#include <port_mgr/port_mgr_intf.h>
//#include <port_mgr/port_mgr_mac.h>
#include "port_mgr_mac.h"
#include <port_mgr/port_mgr_serdes_sbus_map.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_dev.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof1_map.h"
#include "port_mgr_av_sd.h"
#include "port_mgr_serdes_diag.h"
#include "port_mgr_serdes.h"

// for aim_printf
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

static int bf_serdes_get_srds_speed_from_dev_speed(bf_port_speed_t speed) {
  switch (speed) {
    case BF_SPEED_1G:
      return 1;
    case BF_SPEED_10G:
    case BF_SPEED_40G:
      return 10;
    case BF_SPEED_25G:
    case BF_SPEED_50G:
    case BF_SPEED_100G:
      return 25;
    default:
      return speed;
  }
}

/** @brief Get PLL Divider value for a given speed
 *
 * @param[in]  speed   : Port Speed
 *
 * @return PLL Divider value for the given speed
 *
 */
uint8_t bf_serdes_pll_divider_get(bf_port_speed_t speed) {
  uint32_t bit_rate_code, data_width;
  int srds_speed = bf_serdes_get_srds_speed_from_dev_speed(speed);

  port_mgr_av_sd_encode_bitrate_and_width(
      srds_speed, &bit_rate_code, &data_width);

  return bit_rate_code;
}

/** @brief Get Data width for a given speed
 *
 * @param[in]  speed   : Port Speed
 *
 * @return Data Width for the given speed
 *
 */
uint8_t bf_serdes_data_width_get(bf_port_speed_t speed) {
  uint32_t bit_rate_code, data_width;
  int srds_speed = bf_serdes_get_srds_speed_from_dev_speed(speed);

  port_mgr_av_sd_encode_bitrate_and_width(
      srds_speed, &bit_rate_code, &data_width);

  return data_width;
}

/**
 * @file bf_serdes_if.c
 * \brief Details Serdes-level APIs.
 *
 */

static port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t lane) {
  if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_NONE) {
    // Called during warm init. Hence get the unassigned value
    return port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, dev_port, lane);
  }
  return port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
}

/**
 * @addtogroup lld-serdes-api
 * @{
 * This is a description of some APIs.
 */

/*
 * Function Types
 *
 *  *_set(): Set a parameter.  E.g. TX polarity inversion
 *           Initiate an action that completes as the function returns.
 *              E.g. 2D/3D eye measurement
 *
 *  *_get(): Get a parameter.  E.g. TX polarity inversion
 *           Retrieve register information.  E.g. error counters
 *
 *  *_run(): Trigger a remote procedure that does not complete immediately.
 *
 *  *_is_ready(): Check for completion of remote procedure.
 *              E.g. PLL calibration, RX EQ adaptation
 *
 *  *_is_valid(): Check to see if the input parameters are valid
 */

//---------------------------------------------------------------------
//  Section: SerDes Management Interface Control
//---------------------------------------------------------------------

/** \brief Set SerDes Management Interface Clock Source
 *
 * Set the clock source for the management interface.
 *
 * The clock source can be either the 156.25MHz ETH_REFCLK or the
 * 100MHz PCIE_REFCLK.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  clk_src  : Clock source selector
 *
 * \return: BF_SUCCESS           : Clock source set successfully
 * \return: BF_INVALID_ARG       : dev_id never added or dev_id >
 *                                  BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG       : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG       : lane > # of serdes lanes on this port (in
 *                                  this mode)
 * \return: BF_INVALID_ARG       : invalid clk_src
 *
 */
bf_status_t bf_serdes_mgmt_clksel_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bf_sds_mgmt_clk_clksel_t clk_src) {
  // Same as bf_serdes_set_spico_clk_source() with enum type for clk_src
  port_mgr_serdes_t *serdes_p;

  switch (clk_src) {
    case BF_SDS_MGMT_CLK_REFCLK:       /**< Source from external REFCLK */
    case BF_SDS_MGMT_CLK_REFCLK_DIV2:  /**< Debug Only. REFCLK/2 */
    case BF_SDS_MGMT_CLK_PCIECLK:      /**< Source from external PCIE CLK */
    case BF_SDS_MGMT_CLK_PCIECLK_DIV2: /**< Debug Only. PCIE CLK/2 */
      break;
    default:
      return BF_INVALID_ARG;
  }

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_spico_clk_source_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, clk_src);
  return BF_SUCCESS;
}

/** \brief Set SerDes Management Interface Clock Source
 *
 * Set the clock source for the management interface.
 *
 * The clock source can be either the 156.25MHz ETH_REFCLK or the
 * 100MHz PCIE_REFCLK.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  clk_src  : Clock source selector
 *
 * \return: BF_SUCCESS           : Spico clock source retrieved successfully
 * \return: BF_INVALID_ARG       : dev_id never added or dev_id >
 *                                  BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG       : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG       : lane > # of serdes lanes on this port (in
 *                                  this mode)
 * \return: BF_INVALID_ARG       : clk_src == NULL
 *
 */
bf_status_t bf_serdes_mgmt_clksel_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bf_sds_mgmt_clk_clksel_t *clk_src) {
  // Same as bf_serdes_get_spico_clk_source() with enum type for clk_src
  port_mgr_serdes_t *serdes_p;

  if (clk_src == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *clk_src = port_mgr_av_sd_spico_clk_source_get(
      dev_id, serdes_p->ring, serdes_p->tx_sd);
  return BF_SUCCESS;
}

/** \brief Set SerDes Management Bus Acess Method
 *
 * SerDes can be accessed either through SerDes Bus (SBUS) or through the
 * switch core.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  method   : BF_SDS_ACCESS_SBUS or BF_SDS_ACCESS_CORE
 *
 * \return: BF_SUCCESS      : serdes access method set successfully
 * \return: BF_INVALID_ARG  : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG  : lane > # of serdes lanes on this port (in this
 *                              mode)
 *
 */
bf_status_t bf_serdes_mgmt_access_method_set(bf_dev_id_t dev_id,
                                             bf_serdes_access_method_e method) {
  // Same as bf_serdes_set_access_method()
  //
  // I assumed bf_serdes_access_method_e is defined some where already
  // and did not define it again in this code example file
  int rc;

  rc = port_mgr_av_sd_access_fn_set(dev_id, method);
  return ((rc == 0) ? BF_SUCCESS : BF_INVALID_ARG);
}

/** \brief Get SerDes Management Bus Acess Method
 *
 * SerDes can be accessed either through SerDes Bus (SBUS) or through the
 * switch core.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[out] method   : BF_SDS_ACCESS_SBUS or BF_SDS_ACCESS_CORE
 *
 * \return: BF_SUCCESS      : serdes access method set successfully
 * \return: BF_INVALID_ARG  : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG  : lane > # of serdes lanes on this port (in this
 *                              mode)
 *
 */
bf_status_t bf_serdes_mgmt_access_method_get(
    bf_dev_id_t dev_id, bf_serdes_access_method_e *method) {
  // There is currently no get() function, may be useful to add a get for
  // debug.
  (void)dev_id;
  (void)method;

  return BF_SUCCESS;
}

/** \brief Set SerDes Management Interface Broadcast Mode
 *
 * Set SerDes management interface broadcast mode.  If enabled, the specified
 * SerDes lane will respond to broadcast write commands.
 *
 * Broadcast mode is only availabe if bus access is set to SerDes Bus
 * (BF_SDS_ACCESS_SBUS)
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  bcast_en : 1: Broadcast Enable. 0: Broadcast Disable
 *
 * \return: BF_SUCCESS         : broadcast enable set (or cleared) successfully
 * \return: BF_INVALID_ARG     : dev_id never added or dev_id >
 *                                  BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG     : port > LLD_MAX_PORTS-1
 * \return: BF_INVALID_ARG     : lane > 3
 *
 */
bf_status_t bf_serdes_mgmt_bcast_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool tx_dir,
                                     bool bcast_en) {
  // Same as bf_serdes_set_bcast_enable()
  bf_status_t bf_status;
  uint32_t data;

  bf_status =
      bf_serdes_mgmt_reg_get(dev_id, dev_port, lane, tx_dir, 253, &data);
  if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;

  data &= (~(1 << 0));
  data |= (bcast_en ? 0 : 1);  // clear or set IGNORE_BROADCAST
  bf_status = bf_serdes_mgmt_reg_set(dev_id, dev_port, lane, tx_dir, 253, data);
  if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get SerDes Management Interface Broadcast Mode
 *
 * Get SerDes management interface broadcast mode.  If enabled, the specified
 * SerDes lane will respond to broadcast write commands.
 *
 * Broadcast mode is only availabe if bus access is set to SerDes Bus
 * (BF_SDS_ACCESS_SBUS)
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] bcast_en : 1: Broadcast Enable. 0: Broadcast Disable
 *
 * \return: BF_SUCCESS         : broadcast enable set (or cleared) successfully
 * \return: BF_INVALID_ARG     : dev_id never added or dev_id >
 *                                  BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG     : port > LLD_MAX_PORTS-1
 * \return: BF_INVALID_ARG     : lane > 3
 *
 */
bf_status_t bf_serdes_mgmt_bcast_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool tx_dir,
                                     bool *bcast_en) {
  // There is currently no get() function, may be useful to add a get for
  // debug.
  bf_status_t bf_status;
  uint32_t data;

  bf_status =
      bf_serdes_mgmt_reg_get(dev_id, dev_port, lane, tx_dir, 253, &data);
  if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;
  *bcast_en = (data & 1) ? false : true;
  return BF_SUCCESS;
}

/** \brief Direct SerDes Register Write (Debug Only)
 *
 * This is an internal / debug only function.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_dir   : True=TX register space. False=RX register space
 * \param[in]  reg      : SerDes register address
 * \param[in]  data     : Value to write to register
 *
 * \return: BF_SUCCESS         : serdes register written successfully
 * \return: BF_INVALID_ARG     : dev_id never added or dev_id >
 *                                  BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG     : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG     : lane > # of serdes lanes on this port (in this
 *                                  mode)
 * \return: BF_INVALID_ARG     : reg > 255
 *
 */
bf_status_t bf_serdes_mgmt_reg_set(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   int lane,
                                   bool tx_dir,
                                   int reg,
                                   uint32_t data) {
  // Same as bf_serdes_set_reg()
  port_mgr_serdes_t *serdes_p;
  int sd;

  if (reg > 255) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  sd = tx_dir ? serdes_p->tx_sd : serdes_p->rx_sd;

  port_mgr_av_sd_sbus_wr(dev_id, serdes_p->ring, sd, reg, data);
  return BF_SUCCESS;
}

/** \brief Direct SerDes Register Read (Debug Only)
 *
 * This is an internal / debug only function.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_dir   : True=TX register space. False=RX register space
 * \param[in]  reg      : SerDes register address
 * \param[out] data     : Register value
 *
 * \return: BF_SUCCESS         : serdes register written successfully
 * \return: BF_INVALID_ARG     : dev_id never added or dev_id >
 *                                  BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG     : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG     : lane > # of serdes lanes on this port (in this
 *                                  mode)
 * \return: BF_INVALID_ARG        : data (pointer) NULL
 * \return: BF_INVALID_ARG     : reg > 255
 *
 */
bf_status_t bf_serdes_mgmt_reg_get(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   int lane,
                                   bool tx_dir,
                                   int reg,
                                   uint32_t *data) {
  port_mgr_serdes_t *serdes_p;
  int sd;

  // Same as bf_serdes_get_reg()
  if (reg > 255) return BF_INVALID_ARG;
  if (data == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  sd = tx_dir ? serdes_p->tx_sd : serdes_p->rx_sd;

  *data = port_mgr_av_sd_sbus_rd(dev_id, serdes_p->ring, sd, reg);
  return BF_SUCCESS;
}

/** \brief Interrupt SerDes Microcontroller (Debug Only)
 *
 * This is an internal / debug only function.
 *
 * This function issues an interrupt to the per lane microcontroller
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_dir    : True=TX register space. False=RX register space
 * \param[in]  interrupt : Interrupt Address (0-0xFF)
 * \param[in]  int_data  : Interrupt Data (0-0xFFFF)
 * \param[out] rtn_data  : Returned data or status
 *
 * \return: BF_SUCCESS     : Microcontroller interrupt issued successfully
 * \return: BF_INVALID_ARG : dev_id never added or dev_id >
 *                           BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG : lane > # of serdes lanes on this port (in
 *                           this mode)
 * \return: BF_INVALID_ARG : interrupt > 255
 * \return: BF_INVALID_ARG : int_data  > 0xffff
 * \return: BF_INVALID_ARG : rtn_data NULL
 *
 */
bf_status_t bf_serdes_mgmt_uc_int(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  int lane,
                                  bool tx_dir,
                                  int interrupt,
                                  uint32_t int_data,
                                  uint32_t *rtn_data) {
  // Same as bf_serdes_spico_int()
  port_mgr_serdes_t *serdes_p;
  int sd;

  if (interrupt > 255) return BF_INVALID_ARG;
  if (int_data > 0xffff) return BF_INVALID_ARG;
  if (rtn_data == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  sd = tx_dir ? serdes_p->tx_sd : serdes_p->rx_sd;

  *rtn_data =
      port_mgr_av_sd_spico_int(dev_id, serdes_p->ring, sd, interrupt, int_data);
  return BF_SUCCESS;
}

//---------------------------------------------------------------------
//  Section: Chip/Lane Level Functions
//---------------------------------------------------------------------

#if 0
int next_fp = 0;
int dev_port_for_fp[65];

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
bf_status_t port_mgr_tof1_serdes_lane_map_set(bf_dev_id_t dev_id,
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
bf_status_t port_mgr_tof1_serdes_lane_map_get(bf_dev_id_t dev_id,
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
#endif

/** \brief Set RX EQ Periodic Calibration Round Robin Limit
 *
 * Sets the number of lanes across the entire device that will run RX EQ
 * fine tuning concurrently.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 *
 * \param[in]  fine_tune_lane_cnt : Number of lanes running fine tuning (2..32)
 *                              for the whole device. Use even numbers only.
 *                              If odd number, -1 will be applied first.
 *                              If out of range, default of 8 will be used.
 *
 */
bf_status_t bf_dev_rx_eq_cal_rr_set(bf_dev_id_t dev_id,
                                    int fine_tune_lane_cnt) {
  // DEPRECATED
  // SBM has been disabled. RR PCAL must be implemented in SW
  (void)dev_id;
  (void)fine_tune_lane_cnt;
  return BF_INVALID_ARG;
}

/** \brief Get RX EQ Periodic Calibration Round Robin Limit
 *
 * Gets the number of lanes across the entire device that will run RX EQ
 * fine tuning concurrently.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] fine_tune_lane_cnt : Number of lanes running fine tuning (2..32)
 *                              for the whole device.
 *
 */
bf_status_t bf_dev_rx_eq_cal_rr_get(bf_dev_id_t dev_id,
                                    int *fine_tune_lane_cnt) {
  // DEPRECATED
  // SBM has been disabled. RR PCAL must be implemented in SW
  (void)dev_id;
  (void)fine_tune_lane_cnt;
  return BF_INVALID_ARG;
}

/** Refer to Notes related to using Alternate Reference Clock toward the end
 * of this file
 */
/** \brief Set CLKOBS Pad clock divider
 *
 *  Sets the clock divider for clock obervation pad.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  pad          : clkobs pad_id
 * \param[in]  divider      : can be 0: div by 1, 1:div by 2, 2:div by 4, 3:div
 *by 8 only
 * \param[in]  daisy_sel    : daisy chain select
 *             0: ethernet port 8-39+64 or macpll
 *             1: ethernet port 0-7 + 40-63 + pcie + corepll + pciepll
 *
 */
bf_status_t bf_serdes_clkobs_div_set(bf_dev_id_t dev_id,
                                     bf_clkobs_pad_t pad,
                                     int divider,
                                     bool daisy_sel) {
  uint32_t reg, val;

  if (divider > 3) {
    return BF_INVALID_ARG;
  }
  reg = offsetof(Tofino, device_select.misc_regs.refclk_pad_ctrl);
  lld_read_register(dev_id, reg, &val);

  if (pad == BF_CLKOBS_PAD_0) {
    val &= ~0x130ul;
    val |= (divider << 4);
    val |= ((daisy_sel) ? (1 << 8) : 0);
  } else if (pad == BF_CLKOBS_PAD_1) {
    val &= ~0x2c0ul;
    val |= (divider << 6);
    val |= ((daisy_sel) ? (1 << 9) : 0);
  } else {
    return BF_INVALID_ARG;
  }
  lld_write_register(dev_id, reg, val);
  return BF_SUCCESS;
}

/** \brief Set CLKOBS Pad Clock Source
 *
 *  Sets the clock source for clock obervation pad.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  pad          : clkobs pad_id
 * \param[in]  clk_source   : clock obs pad clock source
 * \param[in]  mac_block    : mac_block (for various tx and rx clk selections)
 * \param[in]  lane         : lane (if clk source is from a serdes lane)
 *
 */
bf_status_t bf_serdes_clkobs_clksel_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        bf_clkobs_pad_t pad,
                                        bf_sds_clkobs_clksel_t clk_src) {
  uint32_t reg, val, obs_ctrl;
  bf_mac_block_id_t mac_block;
  int lane;
  bf_mac_block_lane_map_t lane_map;

  if (bf_port_map_dev_port_to_mac(dev_id, dev_port, &mac_block, &lane) !=
      BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  /* TBD: disable other MACs' from driving this clkobs */
  if (lane >= 4) return BF_INVALID_ARG;
  if (mac_block >= 64) return BF_INVALID_ARG;

  if (port_mgr_tof1_serdes_lane_map_get(dev_id, mac_block, &lane_map) !=
      BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  switch (clk_src) {
    case BF_SDS_NONE_CLK:
      obs_ctrl = 0;
      break;
    case BF_SDS_RX_RECOVEREDCLK:
      obs_ctrl = 0x10 | (lane_map.rx_lane[lane] & 0x3);
      break;
    case BF_SDS_TX_CLK:
      obs_ctrl = 0x14 | (lane_map.tx_lane[lane] & 0x3);
      break;
    case BF_SDS_RX_PCS_FIFOCLK:
      obs_ctrl = 0x18 | (lane_map.rx_lane[lane] & 0x3);
      break;
    case BF_SDS_TX_PCS_FIFOCLK:
      obs_ctrl = 0x1C | (lane_map.tx_lane[lane] & 0x3);
      break;
    case BF_SDS_MACCLK:
      obs_ctrl = 0x20;
      break;
    case BF_SDS_HALF_MACCLK:
      obs_ctrl = 0x30;
      break;
    default:
      return BF_INVALID_ARG;
  }
  reg = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.eth_clkobs_ctrl);
  lld_read_register(dev_id, reg, &val);
  if (pad == BF_CLKOBS_PAD_0) {
    setp_eth_regs_eth_clkobs_ctrl_sel_clkobs0(&val, obs_ctrl);
  } else if (pad == BF_CLKOBS_PAD_1) {
    setp_eth_regs_eth_clkobs_ctrl_sel_clkobs1(&val, obs_ctrl);
  } else {
    return BF_INVALID_ARG;
  }
  lld_write_register(dev_id, reg, val);
  return BF_SUCCESS;
}

/** \brief Set TX PLL REFCLK Source
 *
 *  Sets the reference clock source for the TX PLL.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  clk_source   : TX PLL clock source
 *
 */
bf_status_t bf_serdes_tx_pll_clksel_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bf_sds_tx_pll_clksel_t clk_src) {
  // Implementation Notes:
  //
  //  There are 2 things to set.  To select between ETH/ALT_REFCLK, we need
  //  to access ethgpio{br,tl}.refclk_select.  Please see Excel worksheet
  //  refclk_select Mapping.
  //
  //  This setting affects both TX and RX.  TX uses reference clock directly
  //  and RX uses the refclk for frequency comparison purpose only (no
  //  directly impact if frequencies are the same between ETH/ALT_REFCLK).
  //
  //  There is not a separate bf_serdes_rx_cdr_clksel_set() function for
  //  this reason.  If we want to model HW direclty, we may want to have
  //  bf_serdes_refclksel_set/get() for ETH/ALT_REFCLk selection, then
  //  a seperate bf_serdes_tx_pll_clksel_set/get() for just TX PLL clksel.
  //
  //  If RXCLK/OFF/PCIECLK are selected,
  //  Call avago_serdes_set/get_tx_pll_clk_src()
  //
  //  After REFCLK is changed, we should re-cal the TX PLL.  This doesn't
  //  need to be done inside this function as PLL dividers may need to
  //  be changed by another API call before we re-cal.
  //
  //  Between ETH/ALT_REFCLK and RXCLK, refclk frequencies should be the same.
  //  However, phase is different and phase_cal is needed.
  //
  //  If OFF/PCIECLK is selected or was the previous state, then a
  //  frequency range recal + phase_cal are both needed.
  //
  port_mgr_serdes_t *serdes_p;
  int sd, rc;
  uint32_t reg, bit, val, clk_sel;
  bf_status_t bf_status;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  sd = serdes_p->tx_sd;

  switch (clk_src) {
    case BF_SDS_TX_PLL_ETH_REFCLK: /**< Source from external ETH_REFCLK */
    case BF_SDS_TX_PLL_ALT_REFCLK: /**< Source from external ALT_REFCLK */
      bf_status = port_mgr_map_port_lane_to_gpio_refclk(
          dev_id, dev_port, lane, &reg, &bit);
      if (bf_status != BF_SUCCESS) return bf_status;

      // read current so we can merge new clk_sel bit
      rc = lld_read_register(dev_id, reg, &val);
      if (rc != LLD_OK) return BF_INVALID_ARG;

      clk_sel = (clk_src == BF_SDS_TX_PLL_ETH_REFCLK) ? 0 : 1;
      val &= ~(1 << bit);  // mask off old value
      val |= (clk_sel << bit);
      rc = lld_write_register(dev_id, reg, val);
      if (rc != LLD_OK) return BF_INVALID_ARG;

      /* now make sure slice is set to REFCLK.
       *  note: Either BF_SDS_TX_PLL_ETH_REFCLK or BF_SDS_TX_PLL_ETH_REFCLK
       * will cause the av_sd fn to set REFCLK */
      rc = port_mgr_av_sd_tx_pll_clk_source_set(
          dev_id, serdes_p->ring, sd, BF_SDS_TX_PLL_ETH_REFCLK);
      if (rc != 0) {
        return BF_INVALID_ARG;
      }
      break;
    case BF_SDS_TX_PLL_RXCLK:   /**< Source from RX Recovered clock */
    case BF_SDS_TX_PLL_OFF:     /**< Debug Only. Tie clock input to GND */
    case BF_SDS_TX_PLL_PCIECLK: /**< Debug Only. Source from PCIe clock */
      rc = port_mgr_av_sd_tx_pll_clk_source_set(
          dev_id, serdes_p->ring, sd, clk_src);
      if (rc != 0) {
        return BF_INVALID_ARG;
      }
      break;
    default:
      return BF_INVALID_ARG;
  }
  serdes_p->tx_pll_clk = clk_src; /*Cache the clk source selected*/
  return BF_SUCCESS;
}

/** \brief Get TX PLL REFCLK Source
 *
 *  Gets the reference clock source for the TX PLL.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] clk_source   : TX PLL clock source
 *
 */
bf_status_t bf_serdes_tx_pll_clksel_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bf_sds_tx_pll_clksel_t *clk_src) {
  // See the set() function above
  port_mgr_serdes_t *serdes_p;
  int sd;

  if (clk_src == NULL) return BF_INVALID_ARG;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  sd = serdes_p->tx_sd;

  *clk_src = port_mgr_av_sd_tx_pll_clk_source_get(dev_id, serdes_p->ring, sd);
  /* av_sd fn returns BF_SDS_TX_PLL_ETH_REFCLK when the serdes
   * clk source is set to REFCLK */
  if (*clk_src == BF_SDS_TX_PLL_ETH_REFCLK) {
    uint32_t rc, reg, bit, val;
    bf_status_t bf_status;

    // note: above returns this for REFCLK. Need to figure
    // out here which refclk it really is
    bf_status = port_mgr_map_port_lane_to_gpio_refclk(
        dev_id, dev_port, lane, &reg, &bit);
    if (bf_status != BF_SUCCESS) return bf_status;

    // read current so we can merge new clk_sel bit
    rc = lld_read_register(dev_id, reg, &val);
    if (rc != LLD_OK) return BF_INVALID_ARG;

    if (val & (1u << bit)) {
      *clk_src = BF_SDS_TX_PLL_ALT_REFCLK;
    }
  }
  return BF_SUCCESS;
}

/** \brief Run SerDes Initialization
 *
 * Configures TX and RX sub-blocks inside the SerDes lane to bring the SerDes
 * to normal operation state.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  line_rate    : Line rate for this lane
 * \param[in]  init_rx      : if 1, initialize RX blocks
 * \param[in]  init_tx      : If 1, initialize TX blocks
 * \param[in]  tx_drv_en    : Enable TX driver after initialization
 * \param[in]  tx_phase_cal : TX data clock domain crossing clock phase cal
 *                              Needs to run once at start up
 */
bf_status_t bf_serdes_lane_init_run(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bf_sds_line_rate_mode_t line_rate,
                                    bool init_rx,
                                    bool init_tx,
                                    bool tx_drv_en,
                                    bool phase_cal) {
  // Implementation Notes:
  //  config->init_mode = AVAGO_INIT_ONLY
  //      Don't run loopback PRBS checking
  //  config->signal_ok_en = 0
  //      If we need it, let's turn it on through other APIs
  //      In sd16_spec11 p125, it mentioned that the electrical idle
  //      detector (signal_ok) is only reliable for PCIe Gen1/2 (up to 5Gbps)
  //      So it should be disabled for Ethernet speeds (> 10Gbps)
  //
  //  avago_serdes_set_tx_rx_enable() waits for PLL to be calibrated
  //      before it returns.  It checkes the ready signal.
  //
  //  Alternative approach is to start the TX/RX PLL calibration and
  //      exit function while spico FW run cal.  Then have a separate
  //      function above lane level to poll the pll_is_ready() for
  //      multiple lanes/ports.  This will reduce number of threads needed.
  //
  //  We can remove tx_drv_en from this function if it makes sense.
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_NONE) {
    /* Don't touch the harware during cfg replay */
    return BF_SUCCESS;
  }

  port_mgr_av_sd_init_serdes(dev_id,
                             serdes_p->ring,
                             serdes_p->tx_sd,
                             line_rate,
                             init_rx,
                             init_tx,
                             tx_drv_en,
                             phase_cal);
  return BF_SUCCESS;
}

/** \brief Get TX PLL Locked
 *
 * Checks if TX PLL has successfully calibrated and frequency locked
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] locked       : 1 if PLL locked.  0 if not locked
 * \return  1 if locked. 0 if not locked
 */
bf_status_t bf_serdes_tx_pll_lock_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bool *locked) {
  int rc;
  bool tx_pll_ready, rx_pll_ready;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_pll_lock_get(dev_id,
                                   serdes_p->ring,
                                   serdes_p->tx_sd,
                                   serdes_p->rx_sd,
                                   &tx_pll_ready,
                                   &rx_pll_ready);
  if (rc) {
    return BF_INVALID_ARG;
  }
  *locked = tx_pll_ready;
  return BF_SUCCESS;
}

/** \brief Get RX CDR Locked
 *
 * Checks if RX CDR has successfully calibrated and frequency locked
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] locked       : 1 if CDR locked.  0 if not locked
 *
 */
bf_status_t bf_serdes_rx_cdr_lock_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bool *locked) {
  int rc;
  bool tx_pll_ready, rx_pll_ready;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_pll_lock_get(dev_id,
                                   serdes_p->ring,
                                   serdes_p->tx_sd,
                                   serdes_p->rx_sd,
                                   &tx_pll_ready,
                                   &rx_pll_ready);
  if (rc) {
    return BF_INVALID_ARG;
  }
  *locked = rx_pll_ready;
  return BF_SUCCESS;
}

/** \brief Get RX and Tx PLL Locked status
 *
 * Checks if RX CDR has successfully calibrated and frequency locked
 * Checks if TX PLL has successfully calibrated and frequency locked
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] locked       : 1 if CDR locked.  0 if not locked
 *
 */
bf_status_t bf_serdes_rx_and_tx_lock_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         int lane,
                                         bool *rx_locked,
                                         bool *tx_locked) {
  int rc;
  bool tx_pll_ready, rx_pll_ready;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_pll_lock_get(dev_id,
                                   serdes_p->ring,
                                   serdes_p->tx_sd,
                                   serdes_p->rx_sd,
                                   &tx_pll_ready,
                                   &rx_pll_ready);
  if (rc) {
    return BF_INVALID_ARG;
  }
  *rx_locked = rx_pll_ready ? true : false;
  *tx_locked = tx_pll_ready ? true : false;
  return BF_SUCCESS;
}

/** \brief Get TX PLL Status
 *
 * Get detailed TX PLL status information including PLL lock, line rate
 * divider and PLL frequency
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] locked       : 1 if PLL locked.  0 if not locked
 * \param[out] div          : PLL feedback divider
 * \param[out] freq         : Frequency in MHz
 */
bf_status_t bf_serdes_tx_pll_status_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bool *locked,
                                        int *div,
                                        int *freq) {
  // Call avago_serdes_get_tx_pll_state()
  int rc;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_pll_state_get(
      dev_id, serdes_p->ring, serdes_p->tx_sd, locked, div, freq);
  if (rc) {
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Get RX CDR Status
 *
 * Get detailed RX CDR status information including PLL lock, line rate
 * divider and CDR frequency
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] locked       : 1 if CDR locked.  0 if not locked
 * \param[out] div          : CDR feedback divider
 * \param[out] freq         : Frequency in MHz
 */
bf_status_t bf_serdes_rx_cdr_status_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bool *locked,
                                        int *div,
                                        int *freq) {
  // Call avago_serdes_get_rx_pll_state()
  int rc;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_pll_state_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, locked, div, freq);
  if (rc) {
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Set SerDes Loopback Mode
 *
 * Selects between normal mode (no loopback), TX-to-RX serial (near end)
 * loopback and RX-to-TX parallel (far end) loopback modes.
 *
 * For RX-to-TX parallel loopback, this function will call
 * bf_serdes_tx_pll_clksel_set() to select RX recovered clock (RXCLK)
 * as reference clock.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  loopback_mode : SerDes loopback mode
 *
 * \see bf_serdes_tx_pll_clksel_set(), bf_serdes_tx_pll_clksel_get()
 *
 */
bf_status_t bf_serdes_lane_loopback_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bf_sds_loopback_t loopback_mode) {
  // Implementation Notes:
  //  See p49, section 1.12.4 Repeater mode in SerDes16_Spec_13 doc.
  //  It documented Int based procedure if we were to write our own code.
  //
  //  Call bf_serdes_tx_pll_clksel_get() first to determine if the
  //  refclk source needs to be changed.
  //
  //  We should not have to recal phase_cal for parallel (far end) loopback
  //  as data stays within the SerDes
  //
  //  AAPL Calls are avago_set/get_rx_input_loopback() to configure
  //  near end serial TX to RX loopback.  For far end RX to TX parallel
  //  loopback, AAPL calls are avago_serdes_set/get_tx_data_sel()
  //  and avago_serdes_set/get_tx_clk_source()

  //  BF_SDS_LB_OFF = 0,          /**< Normal Operation, no loopback */
  //  BF_SDS_LB_SER_TX_TO_RX = 1, /**< Serial TX to RX near end loopback */
  //  BF_SDS_LB_PAR_RX_TO_TX = 2, /**< Parallel RX to TX far end loopback */
  // int avago_serdes_set_rx_input_loopback(
  //    Aapl_t *aapl,               /**< [in] Pointer to Aapl_t structure. */
  //    uint addr,                  /**< [in] Device address number. */
  //    BOOL internal_loopback)
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  // cant sppt serdes loopback in asymmetric mode platforms
  if (serdes_p->tx_sd != serdes_p->rx_sd) return BF_INVALID_ARG;

  switch (loopback_mode) {
    case BF_SDS_LB_SER_TX_TO_RX:
      rc = port_mgr_av_sd_near_loopback_set(
          dev_id, serdes_p->ring, serdes_p->rx_sd, true);
      if (rc != 0) return BF_INVALID_ARG;
      break;
    case BF_SDS_LB_PAR_RX_TO_TX:
      rc = port_mgr_av_sd_far_loopback_set(
          dev_id, serdes_p->ring, serdes_p->rx_sd, true);
      if (rc != 0) return BF_INVALID_ARG;
      break;
    case BF_SDS_LB_OFF:
      // turn off any previously configured near loopback
      rc = port_mgr_av_sd_near_loopback_set(
          dev_id, serdes_p->ring, serdes_p->rx_sd, false);
      if (rc != 0) return BF_INVALID_ARG;
      // turn off any previously configured far loopback
      rc = port_mgr_av_sd_far_loopback_set(
          dev_id, serdes_p->ring, serdes_p->rx_sd, false);
      if (rc != 0) return BF_INVALID_ARG;
      break;
    default:
      return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Get SerDes Loopback Mode
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] loopback_mode : SerDes loopback mode
 *
 */
bf_status_t bf_serdes_lane_loopback_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bf_sds_loopback_t *loopback_mode) {
  port_mgr_serdes_t *serdes_p;
  bool en;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  // cant sppt serdes loopback in asymmetric mode platforms
  if (serdes_p->tx_sd != serdes_p->rx_sd) return BF_INVALID_ARG;

  port_mgr_av_sd_near_loopback_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, &en);
  if (en) {
    *loopback_mode = BF_SDS_LB_SER_TX_TO_RX;
  } else {
    *loopback_mode = BF_SDS_LB_OFF;
  }
  return BF_SUCCESS;
}

/** \brief Get Lane Status
 *
 * Quick overview of key lane health status
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] lane_status  : Lane Status Info
 */
bf_status_t bf_serdes_lane_status_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bf_sds_lane_status_t *lane_status) {
  (void)dev_id;
  (void)dev_port;
  (void)lane;
  (void)lane_status;

  return BF_SUCCESS;
}

/** @brief Get PLL Divider based on PLL overclock config
 *
 * @param[in]  dev_id   : Device identifier
 * @param[in]  dev_port : Port identifier
 * @param[in]  pll_ovrclk  : PLL overlock percent
 *
 * @return Status of the API call
 *
 */
uint32_t bf_serdes_get_pll_div(bf_dev_id_t dev_id,
                               bf_dev_port_t dev_port,
                               float pll_ovrclk) {
  uint32_t dflt_pll_div;
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) return 0;

  dflt_pll_div = bf_serdes_pll_divider_get(port_p->sw.speed);
  return ((dflt_pll_div * (pll_ovrclk + 100)) / 100);
}

/** @brief Get PLL Overclocking config from HW
 *
 * @param[in]  dev_id   : Device identifier
 * @param[in]  dev_port : Port identifier
 * @param[in]  lane  : Logical lane (within port) 0-3, depending upon mode
 * @param[out]  pll_ovrclk  : return pll overlock percent
 *
 * @return Status of the API call
 *
 */
bf_status_t bf_serdes_pll_ovrclk_get_hw(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        float *pll_ovrclk) {
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);
  port_mgr_port_t *port_p =
      port_mgr_map_dev_port_to_port(dev_id, dev_port + lane);
  bool locked;
  int div, freq, dflt_pll_div;

  if (pll_ovrclk == NULL) return BF_INVALID_ARG;
  if (port_p == NULL) {
    *pll_ovrclk = 0;
    return BF_SUCCESS;
  }
  if (serdes_p == NULL) return BF_INVALID_ARG;

  dflt_pll_div = bf_serdes_pll_divider_get(port_p->sw.speed);
  port_mgr_av_sd_tx_pll_state_get(
      dev_id, serdes_p->ring, serdes_p->tx_sd, &locked, &div, &freq);

  *pll_ovrclk = (((float)div * 100) / dflt_pll_div) - 100;
  return BF_SUCCESS;
}

/** @brief Get PLL Overclocking config
 *
 * @param[in]  dev_id   : Device identifier
 * @param[in]  dev_port : Port identifier
 * @param[in]  lane  : Logical lane (within port) 0-3, depending upon mode
 * @param[out]  pll_ovrclk  : return pll overlock percent
 *
 * @return Status of the API call
 *
 */
bf_status_t bf_serdes_pll_ovrclk_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     float *pll_ovrclk) {
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;
  if (pll_ovrclk == NULL) return BF_INVALID_ARG;

  *pll_ovrclk = serdes_p->pll_ovrclk;
  return BF_SUCCESS;
}

/** @brief Set PLL Overclocking config
 *
 * @param[in]  dev_id   : Device identifier
 * @param[in]  dev_port : Port identifier
 * @param[in]  lane  : Logical lane (within port) 0-3, depending upon mode
 * @param[in]  pll_ovrclk  : pll overlock percent
 *
 * @return Status of the API call
 *
 */
bf_status_t bf_serdes_pll_ovrclk_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     float pll_ovrclk) {
  uint32_t dflt_pll_div, pll_div_cfg;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return BF_INVALID_ARG;
  if (serdes_p == NULL) return BF_INVALID_ARG;

  dflt_pll_div = bf_serdes_pll_divider_get(port_p->sw.speed);
  pll_div_cfg = ((dflt_pll_div * (pll_ovrclk + 100)) / 100);
  if (pll_div_cfg > 180) {
    port_mgr_log(
        "bf_serdes_pll_ovrclk_set : Invalid pll_ovrclk(%f) config, pll_div > "
        "180\n",
        pll_ovrclk);
    return BF_INVALID_ARG;
  }

  serdes_p->pll_ovrclk = pll_ovrclk;
  return BF_SUCCESS;
}

//---------------------------------------------------------------------
//  Section: Transmit Path and Driver Functions
//---------------------------------------------------------------------

/** \brief Set Transmit Path Enable
 *
 * This function sets the TX path enable which also functions as a reset.
 * Some TX settings become effect when TX state goes from Disable to Enable
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_en    : 1: TX path enabled. 0: disabled
 *
 */
bf_status_t bf_serdes_tx_en_set(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                bool st) {
  bool tx_en, rx_en;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;

  // get rx_en state for rmw semantics
  port_mgr_av_sd_tx_rx_en_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, &tx_en, &rx_en);
  // now change rx_en state (on tx side)
  port_mgr_av_sd_tx_rx_en_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, st, rx_en);
  return BF_SUCCESS;
}

/** \brief Get Transmit Path Enable
 *
 * This function gets the TX path enable which also functions as a reset.
 * Some TX settings become effect when TX state goes from Disable to Enable
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] tx_en    : 1: TX path enabled. 0: disabled
 *
 */
bf_status_t bf_serdes_tx_en_get(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                bool *tx_en) {
  bool rx_en;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;

  // get rx_en state for rmw semantics
  port_mgr_av_sd_tx_rx_en_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, tx_en, &rx_en);
  return BF_SUCCESS;
}

/** \brief Set Transmit Output Enable
 *
 * This function sets the TX Output enable.  When disabled, output pins
 * P/N will drive to AVDD
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_drv_en    : 1: TX Output enabled.  0: disabled
 */
bf_status_t bf_serdes_tx_drv_en_set(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool tx_drv_en) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_drv_en_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, tx_drv_en);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get Transmit Output Enable
 *
 * This function gets the TX Output enable.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] tx_drv_en    : 1: TX Output enabled.  0: disabled
 */
bf_status_t bf_serdes_tx_drv_en_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool *tx_drv_en) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_drv_en_get(
      dev_id, serdes_p->ring, serdes_p->tx_sd, tx_drv_en);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Set Transmit Output Polarity Inversion
 *
 * This function sets the TX output polarity inversion.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_inv   : 1: invert P/N. 0: no inversion
 */
bf_status_t bf_serdes_tx_drv_inv_set_internal(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              int lane,
                                              bool tx_inv,
                                              bool allow_unassigned) {
  port_mgr_serdes_t *serdes_p;
  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) return BF_INVALID_ARG;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;

  if (allow_unassigned) {
    serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
        dev_id, dev_port, lane);
  } else {
    serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  }
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_log(
      "SDS :%d:%3d:%d: MAP: Tx inv=%d", dev_id, dev_port, lane, tx_inv ? 1 : 0);

  // save for FSM
  serdes_p->tx_inv = tx_inv ? true : false;
  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    /* Don't touch the hardware  during cfg replay */
    return BF_SUCCESS;
  }
  port_mgr_av_sd_tx_invert_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, serdes_p->tx_inv ? 1 : 0);
  return BF_SUCCESS;
}

/** \brief Set Transmit Output Polarity Inversion
 *
 * This function sets the TX output polarity inversion.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_inv   : 1: invert P/N. 0: no inversion
 */
bf_status_t bf_serdes_tx_drv_inv_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool tx_inv) {
  return bf_serdes_tx_drv_inv_set_internal(
      dev_id, dev_port, lane, tx_inv, false);
}

bf_status_t bf_serdes_tx_drv_inv_set_allow_unassigned(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      int lane,
                                                      bool tx_inv) {
  return bf_serdes_tx_drv_inv_set_internal(
      dev_id, dev_port, lane, tx_inv, true);
}

/** \brief Get Transmit Output Polarity Inversion
 *
 * This function gets the TX output polarity inversion.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] tx_inv   : 1: invert P/N. 0: no inversion
 */
bf_status_t bf_serdes_tx_drv_inv_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool *tx_inv) {
  port_mgr_serdes_t *serdes_p;
  BOOL inv;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  inv = port_mgr_av_sd_tx_invert_get(dev_id, serdes_p->ring, serdes_p->tx_sd);
  *tx_inv = (inv == TRUE) ? true : false;
  return BF_SUCCESS;
}

/** \brief Valid Check for Transmit EQ attenuation setting
 *
 * This funciton checks to see if the specified TX EQ attenuation settings
 * are valid.
 *
 * \param[in]  attn_main    : Main cursor setting (  0 to 23)
 * \param[in]  attn_post    : Post cursor setting (-31 to 31)
 * \param[in]  attn_pre     : Pre  cursor setting (-31 to 31)
 *
 * \return  1 if settings are valid. 0 if not.
 */
bf_status_t bf_serdes_tx_drv_attn_is_valid(int attn_main,
                                           int attn_post,
                                           int attn_pre) {
  if ((attn_main < 0) || (attn_main > 23)) return BF_INVALID_ARG;
  if ((attn_pre + attn_main + attn_post) > 32) return BF_INVALID_ARG;
  if ((attn_pre > 31) || (attn_pre < -31)) return BF_INVALID_ARG;
  if ((attn_post > 31) || (attn_post < -31)) return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/** \brief Set Transmit EQ based on Attenuation
 *
 * This function allows users to set transmit EQ attenuation settings directly.
 *
 * Range limits:
 *
 *  - attn_main
 *      - <= 23 for general applications
 *      - <= 16 for KR applications
 *  - attn_pre + attn_main + attn_post <= 32
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  attn_main    : Main cursor setting (  0 to 23)
 * \param[in]  attn_post    : Post cursor setting (-31 to 31)
 * \param[in]  attn_pre     : Pre  cursor setting (-31 to 31)
 *
 * \see bf_serdes_tx_drv_amp_set()
 */
bf_status_t bf_serdes_tx_drv_attn_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int attn_main,
                                      int attn_post,
                                      int attn_pre) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (!serdes_p) return BF_INVALID_ARG;

  bf_status_t bf_status;

  bf_status = bf_serdes_tx_drv_attn_is_valid(attn_main, attn_post, attn_pre);
  if (bf_status != BF_SUCCESS) return bf_status;
  // save for future use
  serdes_p->tx_eq_pre = attn_pre;
  serdes_p->tx_eq_atten = attn_main;
  serdes_p->tx_eq_post = attn_post;
  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    /* Don't touch the harware during cfg replay */
    return BF_SUCCESS;
  }
  // Configure Tx EQ parameters
  rc = port_mgr_av_sd_set_tx_eq(dev_id,
                                serdes_p->ring,
                                serdes_p->tx_sd,
                                serdes_p->tx_eq_pre,
                                serdes_p->tx_eq_atten,
                                serdes_p->tx_eq_post);

  if (rc) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Set Transmit EQ based on Attenuation
 *
 * This function allows users to set transmit EQ attenuation settings directly.
 *
 * Range limits:
 *
 *  - attn_main
 *      - <= 23 for general applications
 *      - <= 16 for KR applications
 *  - attn_pre + attn_main + attn_post <= 32
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  attn_main    : Main cursor setting (  0 to 23)
 * \param[in]  attn_post    : Post cursor setting (-31 to 31)
 * \param[in]  attn_pre     : Pre  cursor setting (-31 to 31)
 *
 * \see bf_serdes_tx_drv_amp_set_allow_unassigned()
 */
bf_status_t bf_serdes_tx_drv_attn_set_allow_unassigned(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       int lane,
                                                       int attn_main,
                                                       int attn_post,
                                                       int attn_pre) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    bf_status_t bf_status;

    bf_status = bf_serdes_tx_drv_attn_is_valid(attn_main, attn_post, attn_pre);
    if (bf_status != BF_SUCCESS) {
      return BF_INVALID_ARG;
    } else {
      int rc;

      // save for future use
      serdes_p->tx_eq_pre = attn_pre;
      serdes_p->tx_eq_atten = attn_main;
      serdes_p->tx_eq_post = attn_post;

      // Configure Tx EQ parameters
      rc = port_mgr_av_sd_set_tx_eq(dev_id,
                                    serdes_p->ring,
                                    serdes_p->tx_sd,
                                    serdes_p->tx_eq_pre,
                                    serdes_p->tx_eq_atten,
                                    serdes_p->tx_eq_post);
      if (rc) return BF_INVALID_ARG;
    }
  }
  return BF_SUCCESS;
}

/** \brief Get Transmit EQ based on Attenuation
 *
 * This function allows users to get transmit EQ attenuation settings directly.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out]  attn_main    : Main cursor settoutg (  0 to 23)
 * \param[out]  attn_post    : Post cursor settoutg (-31 to 31)
 * \param[out]  attn_pre     : Pre  cursor settoutg (-31 to 31)
 *
 * \see bf_serdes_tx_drv_amp_get()
 */
bf_status_t bf_serdes_tx_drv_attn_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int *attn_main,
                                      int *attn_post,
                                      int *attn_pre) {
  int rc;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;
  if ((attn_pre == NULL) || (attn_post == NULL) || (attn_main == NULL)) {
    return BF_INVALID_ARG;
  }

  rc = port_mgr_av_sd_get_tx_eq(
      dev_id, serdes_p->ring, serdes_p->tx_sd, attn_pre, attn_main, attn_post);

  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get Transmit EQ based on Attenuation
 *
 * This function allows users to get transmit EQ attenuation settings directly.
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port         : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane         : Logical lane within port (0..3, mode dependent)
 *
 * \param[out]  attn_main    : Main cursor settoutg (  0 to 23)
 * \param[out]  attn_post    : Post cursor settoutg (-31 to 31)
 * \param[out]  attn_pre     : Pre  cursor settoutg (-31 to 31)
 *
 * \see bf_serdes_tx_drv_amp_get()
 */
bf_status_t bf_serdes_tx_drv_attn_get_allow_unassigned(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       int lane,
                                                       int *attn_main,
                                                       int *attn_post,
                                                       int *attn_pre) {
  int rc;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
          dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;
  if ((attn_pre == NULL) || (attn_post == NULL) || (attn_main == NULL)) {
    return BF_INVALID_ARG;
  }

  rc = port_mgr_av_sd_get_tx_eq(
      dev_id, serdes_p->ring, serdes_p->tx_sd, attn_pre, attn_main, attn_post);

  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Set Transmit EQ based on Output Amplitude
 *
 * This function allows the user to configure output waveform based on
 * amplitude at various UI location relative to the edge transition.
 *
 *  - amp_pre  (amplitude 1  UI before edge transition, 962 to 156 mVppd)
 *  - amp_main (amplitude 1  UI after  edge transition, 962 to 364 mVppd)
 *  - amp_post (amplitude 2+ UI after  edge transition, 962 to 156 mVppd)
 *
 * This function translates amplitude setting into attenuation setting used by
 * the hardware.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  amp_main : Main-cursor amplitude
 * \param[in]  amp_post : Post-cursor amplitude
 * \param[in]  amp_pre  : Pre-cursor amplitude
 *
 * \see bf_serdes_tx_drv_attn_set()
 */
bf_status_t bf_serdes_tx_drv_amp_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int amp_main,
                                     int amp_post,
                                     int amp_pre) {
  // Implementation Notes:
  //
  //  Example Python Code (tested)
  //
  /*-----------------------------------------------------------------
  # Constants
  AMP_FULL = 962
  STEP     =  26

  attn_pre  = (amp_pre - amp_post) / STEP
  attn_main = (AMP_FULL - amp_main - amp_pre + amp_post) / STEP
  attn_post = (amp_main - amp_post) / STEP

  # Call attenuation setting checking here for API
  input_valid = tx_drv_attn_is_valid( attn_pre, attn_main, attn_post, False )

  if input_valid == True:
  # Perform write to the device
  pass
  -----------------------------------------------------------------*/
  bf_status_t bf_status;
  int attn_main;
  int attn_post;
  int attn_pre;
  int AMP_FULL = 962;
  int STEP = 26;

  attn_pre = (amp_pre - amp_post) / STEP;
  attn_main = (AMP_FULL - amp_main - amp_pre + amp_post) / STEP;
  attn_post = (amp_main - amp_post) / STEP;

  bf_status = bf_serdes_tx_drv_attn_set(
      dev_id, dev_port, lane, attn_main, attn_post, attn_pre);
  return bf_status;
}

/** \brief Get Transmit EQ based on Output Amplitude
 *
 * This functions gets the hardware setting (TX EQ attenuation) and converts
 * them to TX amplitude setting

 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] amp_main : Main-cursor amplitude
 * \param[out] amp_post : Post-cursor amplitude
 * \param[out] amp_pre  : Pre-cursor amplitude
 *
 * \see bf_serdes_tx_drv_attn_get()
 */
bf_status_t bf_serdes_tx_drv_amp_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int *amp_main,
                                     int *amp_post,
                                     int *amp_pre) {
  // Implementation Notes:
  //
  //  Example Python Code (tested)
  //
  /*-----------------------------------------------------------------
  # Constants
  AMP_FULL = 962
  STEP     =  26

  # Calls bf_serdes_tx_drv_attn_get()

  # Convert attn settings to amplitude
  amp_pre  = AMP_FULL - STEP * (attn_main + attn_post)
  amp_main = AMP_FULL - STEP * (attn_main + attn_pre)
  amp_post = AMP_FULL - STEP * (attn_main + attn_post + attn_pre)
  -----------------------------------------------------------------*/
  bf_status_t bf_status;
  int attn_main;
  int attn_post;
  int attn_pre;
  int AMP_FULL = 962;
  int STEP = 26;

  bf_status = bf_serdes_tx_drv_attn_get(
      dev_id, dev_port, lane, &attn_main, &attn_post, &attn_pre);
  if (bf_status != BF_SUCCESS) return bf_status;

  // Convert attn settings to amplitude
  *amp_pre = AMP_FULL - STEP * (attn_main + attn_post);
  *amp_main = AMP_FULL - STEP * (attn_main + attn_pre);
  *amp_post = AMP_FULL - STEP * (attn_main + attn_post + attn_pre);
  return BF_SUCCESS;
}

/** \brief Get Transmit Driver Status
 *
 * Get TX Driver status including: TX path enable, output enable,
 * TX EQ (by amplitude), output inversion.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] tx_status : TX Driver Status
 */
bf_status_t bf_serdes_tx_drv_status_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bf_sds_tx_drv_status_t *tx_status) {
  bf_status_t bf_status;

  bf_status = bf_serdes_tx_drv_amp_get(dev_id,
                                       dev_port,
                                       lane,
                                       &tx_status->amp_main,
                                       &tx_status->amp_post,
                                       &tx_status->amp_pre);
  if (bf_status != BF_SUCCESS) return bf_status;

  bf_status =
      bf_serdes_tx_drv_inv_get(dev_id, dev_port, lane, &tx_status->tx_inv);
  if (bf_status != BF_SUCCESS) return bf_status;

  bf_status =
      bf_serdes_tx_drv_en_get(dev_id, dev_port, lane, &tx_status->tx_drv_en);
  if (bf_status != BF_SUCCESS) return bf_status;

  bf_status = bf_serdes_tx_en_get(dev_id, dev_port, lane, &tx_status->tx_en);
  if (bf_status != BF_SUCCESS) return bf_status;
  return BF_SUCCESS;
}

//---------------------------------------------------------------------
//  Section: Receiver Analog Front End
//---------------------------------------------------------------------

/** \brief Set Receive Path Enable
 *
 * This function sets the RX path enable which also functions as a reset.
 * Some RX settings become effect when RX state goes from Disable to Enable
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  rx_en    : 1: RX path enabled. 0: disabled
 *
 */
bf_status_t bf_serdes_rx_en_set(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                bool st) {
  bool tx_en, rx_en;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;

  // get tx_en state for rmw semantics
  port_mgr_av_sd_tx_rx_en_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, &tx_en, &rx_en);
  // now change rx_en state (on rx side)
  port_mgr_av_sd_tx_rx_en_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, tx_en, st);
  return BF_SUCCESS;
}

/** \brief Get Receive Path Enable
 *
 * This function gets the RX path enable which also functions as a reset.
 * Some RX settings become effect when TX state goes from Disable to Enable
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_en    : 1: RX path enabled. 0: disabled
 *
 */
bf_status_t bf_serdes_rx_en_get(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                bool *rx_en) {
  bool tx_en;
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);

  if (serdes_p == NULL) return BF_INVALID_ARG;

  // get tx_en state for rmw semantics
  port_mgr_av_sd_tx_rx_en_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, &tx_en, rx_en);
  return BF_SUCCESS;
}

/** \brief Set Receive Input Polarity Inversion
 *
 * This function sets the RX input polarity inversion.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  rx_inv   : 1: invert P/N. 0: no inversion
 */
bf_status_t bf_serdes_rx_afe_inv_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool rx_inv) {
  port_mgr_serdes_t *serdes_p;
  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) return BF_INVALID_ARG;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_log(
      "SDS :%d:%3d:%d: MAP: Rx inv=%d", dev_id, dev_port, lane, rx_inv ? 1 : 0);

  // save for FSM
  serdes_p->rx_inv = rx_inv ? true : false;
  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    /* Don't touch the harware during cfg replay */
    return BF_SUCCESS;
  }
  port_mgr_av_sd_rx_invert_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_inv ? 1 : 0);
  return BF_SUCCESS;
}

/** \brief Get Receive Input Polarity Inversion
 *
 * This function gets the RX input polarity inversion.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_inv   : 1: invert P/N. 0: no inversion
 */
bf_status_t bf_serdes_rx_afe_inv_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool *rx_inv) {
  port_mgr_serdes_t *serdes_p;

  if (rx_inv == NULL) return BF_INVALID_ARG;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *rx_inv =
      port_mgr_av_sd_rx_invert_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Set Receive Input Termination
 *
 * Set RX AFE (analog front end) serial input buffer termination option.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  rx_term  : Termination option
 *
 */
bf_status_t bf_serdes_rx_afe_term_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bf_sds_rx_term_t rx_term) {
  port_mgr_serdes_t *serdes_p;

  switch (rx_term) {
    case BF_SDS_RX_TERM_GND:
    case BF_SDS_RX_TERM_AVDD:
    case BF_SDS_RX_TERM_FLOAT:
      break;
    default:
      return BF_INVALID_ARG;
  }

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  // save for FSM
  serdes_p->rx_term = rx_term;
  port_mgr_av_sd_rx_term_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_term);
  return BF_SUCCESS;
}

/** \brief Get Receive Input Termination
 *
 * Get RX AFE (analog front end) serial input buffer termination option.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_term  : Termination option
 *
 */
bf_status_t bf_serdes_rx_afe_term_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bf_sds_rx_term_t *rx_term) {
  port_mgr_serdes_t *serdes_p;

  if (rx_term == NULL) return BF_INVALID_ARG;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *rx_term =
      port_mgr_av_sd_rx_term_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Set Receive Loss of Signal Threshold
 *
 * Set RX input LOS threshold in DAC steps.  This function uses PCIe signal
 * detect circuit to detect LOS condition.
 *
 * Threshold range is
 * For A0, 0-15
 * For B0, 0-255
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 * \param[in]  rx_los_en    : LOS enable
 * \param[in]  rx_los_thres : LOS threshold DAC setting
 *
 */
bf_status_t bf_serdes_rx_afe_los_thres_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           bool rx_los_en,
                                           int rx_los_thres) {
  port_mgr_serdes_t *serdes_p;
  lld_err_t err;
  bf_sku_chip_part_rev_t rev_no;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;
  if (rx_los_thres < 0) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  serdes_p->rx_sig_ok_thresh = rx_los_thres;

  /* A0 (rev_no==0) requires custom bbgain settings
   *  B0 (rev_no==1) defaults recommended
   */
  err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
  if ((err == LLD_OK) && rev_no != 0) {
    port_mgr_av_sd_spico_int(dev_id,
                             serdes_p->ring,
                             serdes_p->rx_sd,
                             0x20,
                             0x0040 | (rx_los_thres << 8));
    port_mgr_av_sd_spico_int(
        dev_id, serdes_p->ring, serdes_p->rx_sd, 0x20, rx_los_en ? 0x20 : 0x0);
  } else {
    // For A0, cap at max
    if (rx_los_thres > 15) rx_los_thres = 15;
    serdes_p->rx_sig_ok_thresh = rx_los_thres;

    // save for FSM
    port_mgr_av_sd_signal_ok_thresh_set(
        dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_sig_ok_thresh);
  }
  return BF_SUCCESS;
}

/** \brief Get Receive Loss of Signal Threshold
 *
 * Get RX input LOS threshold in DAC steps
 *
 * Threshold range is (for A0):
 *    0-15
 * Threshold range is (for B0):
 *    0-255
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 * \param[out] rx_los_en    : LOS enable
 * \param[out] rx_los_thres : LOS threshold in DAC steps
 *
 */
bf_status_t bf_serdes_rx_afe_los_thres_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           bool *rx_los_en,
                                           int *rx_los_thres) {
  port_mgr_serdes_t *serdes_p;

  if (rx_los_thres == NULL) return BF_INVALID_ARG;
  if (rx_los_en == NULL) return BF_INVALID_ARG;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes_ha_mindful(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *rx_los_thres = port_mgr_av_sd_signal_ok_thresh_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd);

  *rx_los_en =
      port_mgr_av_sd_signal_ok_en_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Get Signal Detect Status
 *
 * If signal detect circuit is enabled, this function will report if a signal
 * loss condition (average amplitude below threshold) was recorded since the
 * last time this function was called.
 *
 * Upon calling this function, the sticky LOS signal will be reset.
 *
 * *Warning: This is intended as a debug feature as signal detect circuit is
 * not guaranted for non-PCIe Gen1/2 line rates.*
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_los   : Loss of signal indicator
 *
 */
bf_status_t bf_serdes_rx_afe_los_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool *rx_los) {
  port_mgr_serdes_t *serdes_p;
  BOOL los;

  if (rx_los == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  los = port_mgr_av_sd_los_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  *rx_los = los ? true : false;
  return BF_SUCCESS;
}

//---------------------------------------------------------------------
//  Section: Receiver Equalizer
//---------------------------------------------------------------------

/** \brief Set Receive Equalizer Parameters (Advanced)
 *
 * Sets specific RX EQ parameters.
 *
 * This is an advanced function reserved for internal/debug use.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  row      : row of INT 0x26, table 9.
 * \param[in]  col      : column of INT 0x26, table 9.
 * \param[in]  value    : Parameter value
 */
bf_status_t bf_serdes_rx_eq_param_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int row,
                                      int col,
                                      int value) {
  port_mgr_serdes_t *serdes_p;

  if ((row < 0) || (row > 15)) return BF_INVALID_ARG;
  if ((col < 0) || (col > 5)) return BF_INVALID_ARG;
  if ((value < -255) || (value > 255)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_param_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, row, col, value);
  return BF_SUCCESS;
}

/** \brief Get Receive Equalizer Parameters (Advanced)
 *
 * Gets specific RX EQ parameters.
 *
 * This is an advanced function reserved for internal/debug use.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  row      : row of INT 0x26, table 9.
 * \param[in]  col      : column of INT 0x26, table 9.
 * \param[out] value    : Parameter value
 */
bf_status_t bf_serdes_rx_eq_param_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int row,
                                      int col,
                                      int *value) {
  port_mgr_serdes_t *serdes_p;

  if ((row < 0) || (row > 15)) return BF_INVALID_ARG;
  if ((col < 0) || (col > 5)) return BF_INVALID_ARG;
  if (value == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_param_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, row, col, value);
  return BF_SUCCESS;
}

/** \brief RX EQ Calibration in Progress
 *
 * Check to see if PHY microcontroller (uC) is busy running RX EQ calibration.
 *
 * This function will check up to chk_cnt times with chk_wait (ms) delays in
 * between each check.  If uC busy is detected, the function will return
 * right away without finishing all the chk_cnt.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  chk_cnt  : Number of times to check
 * \param[in]  chk_wait : (ms) Time to wait between checks
 * \param[out] uc_busy  : 1: microcontroller is busy. 0: not busy
 *
 */
bf_status_t bf_serdes_rx_eq_cal_busy_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         int lane,
                                         int chk_cnt,
                                         int chk_wait,
                                         bool *uc_busy) {
  // Implementation Notes:
  //  Call avago_serdes_dfe_running()
  //  If chk_cnt = 0 or 1, check once and return.
  port_mgr_serdes_t *serdes_p;

  if (uc_busy == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_running_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, uc_busy);

  (void)chk_cnt;
  (void)chk_wait;
  return BF_SUCCESS;
}

/** \brief Set RX EQ CTLE Fixed Settings
 *
 * Sets fixed RX EQ CTLE settings.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  ctle_dc : DC boost (0..255)
 * \param[in]  ctle_lf : Peaking filter low frequency gain (0..15)
 * \param[in]  ctle_hf : Peaking filter high frequency gain (0..15)
 * \param[in]  ctle_bw : Peaking filter peaking frequency (0..7)
 *
 * \see bf_serdes_rx_eq_mode_set()
 */
bf_status_t bf_serdes_rx_eq_ctle_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int ctle_dc,
                                     int ctle_lf,
                                     int ctle_hf,
                                     int ctle_bw) {
  // Implementation Notes:
  //
  //  Calls avago_serdes_dfe_state_ext() with mode=AVAGO_DFE_MODE_CTLE and
  //  do_write=True
  //
  //  This in turn calls serdes_dfe_update_ctle() which modifies only
  //  these 4 CTLE parameters
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_eq_ctle_set(dev_id,
                                     serdes_p->ring,
                                     serdes_p->rx_sd,
                                     ctle_dc,
                                     ctle_lf,
                                     ctle_hf,
                                     ctle_bw);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get RX EQ CTLE Fixed Settings
 *
 * Gets live RX EQ CTLE settings.
 *
 * If this function is called after iCal, the calibrated CTLE results will
 * be returned.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] ctle_dc : DC boost (0..255)
 * \param[out] ctle_lf : Peaking filter low frequency gain (0..15)
 * \param[out] ctle_hf : Peaking filter high frequency gain (0..15)
 * \param[out] ctle_bw : Peaking filter peaking frequency (0..7)
 *
 * \see bf_serdes_rx_eq_mode_set()
 */
bf_status_t bf_serdes_rx_eq_ctle_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int *ctle_dc,
                                     int *ctle_lf,
                                     int *ctle_hf,
                                     int *ctle_bw) {
  // Implementation Notes:
  //  Calls avago_serdes_dfe_state_ext() with mode=AVAGO_DFE_MODE_CTLE and
  //  do_write=False.
  //
  //  This in turn calls serdes_dfe_update_ctle() which modifies only
  //  these 4 CTLE parameters
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_eq_ctle_get(dev_id,
                                     serdes_p->ring,
                                     serdes_p->rx_sd,
                                     ctle_dc,
                                     ctle_lf,
                                     ctle_hf,
                                     ctle_bw);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Set Specific RX EQ DFE Tap (Advanced)
 *
 * Set DFE Tap value for a specific tap.
 *
 * This is an advanced function for internal and debug use only.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  dfe_tap_num : DFE Tap Number (1 or higher)
 * \param[in]  dfe_tap_val : DFE Tap Value (-15..15)
 *
 * \see bf_serdes_rx_eq_dfe_set()
 */
bf_status_t bf_serdes_rx_eq_dfe_adv_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        int dfe_tap_num,
                                        int dfe_tap_val) {
  // Implementation Notes:
  //
  //  It may be easier to write to 0x26 column 0x3 rows 0x1..0xC directly
  //  than going through Avago API (avago_serdes_dfe_state_ext())
  //
  //  The purpose of this function is to not expose the number of DFE taps
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_tap_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, dfe_tap_num, dfe_tap_val);
  return BF_SUCCESS;
}

/** \brief Get Specific RX EQ DFE Tap (Advanced)
 *
 * Get DFE Tap value for a specific tap (tap 2 and beyond).
 * This is an advanced function for debug only.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  dfe_tap_num : DFE Tap Number (2 or higher)
 * \param[out] dfe_tap_val : DFE Tap Value (-15..15)
 *
 * \see bf_serdes_rx_eq_dfe_get()
 */
bf_status_t bf_serdes_rx_eq_dfe_adv_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        int dfe_tap_num,
                                        int *dfe_tap_val) {
  // Implementation Notes:
  //  It may be easier to read from 0x26 column 0x3 rows 0x1..0xC directly
  //  than going through Avago API (avago_serdes_dfe_state_ext())
  //
  //  The purpose of this function is to not expose the number of DFE taps
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_tap_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, dfe_tap_num, dfe_tap_val);
  return BF_SUCCESS;
}

/** \brief Set RX EQ DFE Tap
 *
 * Set DFE Tap values if RX EQ mode is one of FIX_DFE modes.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  dfe_gain   : DFE Gain (0..15)
 * \param[in]  dfe_tap[4] : DFE Taps 1 to 4 (-127..127). dfe_tap[0]=Tap 1
 *
 * \see bf_serdes_rx_eq_mode_set()
 */
bf_status_t bf_serdes_rx_eq_dfe_set(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    int dfe_gain,
                                    int dfe_tap[4]) {
  // Implementation Notes:
  //  The Avago API for modifying dfe tap is avago_serdes_dfe_state_ext().
  //  However, DFE tap should not be set until slicer offset (VOS in Avago
  //  code) has been calibrated.
  //
  //  If we read Int 0x26 dfe_status, dfe_status[7] means Input Offset
  //  correction complete (i.e. slicer offset).  We can check this bit first.
  //  If slicer has not been calibrated, we should:
  //      - Run slicer cal via bf_serdes_rx_eq_cal_adv_run(),
  //          cal_cmd=CAL_SLICER_ONLY)
  //      - Wait for cal to finish by calling bf_serdes_rx_eq_cal_busy()
  //      - Finall setting DFE taps.
  //
  //  This is being confirmed via Jiira AVAGO-268
  //
  //  Alternatively, we can return an error if dfe_status[7] has not been
  //  set.  If we do this, then user will have to call
  //  bf_serdes_rx_eq_cal_adv_run() separately, making usage a little more
  //  confusing.
  //
  //  Currently in UG I assume we will run slicer cal within this function.
  //
  //  Settings:
  //
  //      Set dfe_state.dfeGain       = dfe_gain
  //      Set dfe_state.dfeTAP1       = dfe_tap1
  //      Set dfe_state.dfeTAP[1:3]   = dfe_tap2:4
  //      Set dfe_state.dfeTAP[4:12]  = 0
  //
  //  We can change dfe_taps{1:4} to array dfe_tap[4] as well.
  //  Not sure how this might affect CLI.  Is it easy to support an array
  //  in CLI?
  port_mgr_serdes_t *serdes_p;
  bool vos_done;
  int tap;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_rx_eq_vos_done_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, &vos_done);
  if (!vos_done) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_gain_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, (uint32_t)dfe_gain);
  for (tap = 0; tap < 4; tap++) {
    bf_status_t bf_status;

    bf_status =
        bf_serdes_rx_eq_dfe_adv_set(dev_id, dev_port, lane, tap, dfe_tap[tap]);
    if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Get RX EQ DFE Tap
 *
 * Get DFE Tap values.
 *
 * If rx_eq_mode is ADP_DFE, this will retrieve adapted value.  If rx_eq_mode
 * is FIX_DFE, this will retrieve the fixed value.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] dfe_gain   : DFE Gain (0..15)
 * \param[in]  dfe_tap[4] : DFE Taps 1 to 4 (-127..127). dfe_tap[0]=Tap 1
 *
 * \see bf_serdes_rx_eq_mode_set()
 */
bf_status_t bf_serdes_rx_eq_dfe_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    int *dfe_gain,
                                    int dfe_taps[4]) {
  // Implementation Notes:
  //  Call avago_serdes_dfe_state_ext() with mode=AVAGO_DFE_MODE_DFE and
  //  do_write=False to write to fixed DFE parameters.
  //
  //      dfe_gain        = dfe_state.dfeGain
  //      dfe_taps[ 0]    = dfe_state.dfeTAP1
  //      dfe_taps[ 1:12] = dfe_state.dfeTAP[1:13]
  //      dfe_taps[13:15] = 0
  //
  //      dfe_taps[13:15] is to obscure the number of DFE taps in the
  //      Avago device
  //
  port_mgr_serdes_t *serdes_p;
  int tap;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_dfe_gain_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, (uint32_t *)dfe_gain);
  for (tap = 0; tap < 4; tap++) {
    bf_status_t bf_status;

    bf_status = bf_serdes_rx_eq_dfe_adv_get(
        dev_id, dev_port, lane, tap, &dfe_taps[tap]);
    if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Set RX EQ Calibration Parameters (Advanced)
 *
 * Set RX EQ calibration parameters if the default settings do not work well
 * for a given type of channel.
 *
 * This is a debug feature not recommended for general usage.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  ctle_dc_hint     : Hint for DC boost (0..255)
 * \param[in]  dfe_gain_range   : DFE gain range (bits 7:0)
 * \param[in]  pcal_loop_cnt    : Number of fine adjustment loops to run
 *                                  for each pCal run (1..15)
 *
 */
bf_status_t bf_serdes_rx_eq_cal_param_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int lane,
                                          int ctle_dc_hint,
                                          int dfe_gain_range,
                                          int pcal_loop_cnt) {
  // Implementation Notes:
  //  Call avago_serdes_set_dfe_state_ext() with mode=AVAGO_DFE_MODE_PARAM
  //  to configure the following:
  //
  //  Configures Int 0x26
  //      Int 0x26        param
  //      seed_DC         = ctle_dc_hint
  //      min_gainDFE     = dfe_gain_range[3:0]
  //      max_gainDFE     = dfe_gain_range[7:4]
  //      pCal_loop_count = pcal_loop_cnt
  //
  //  seed_HF/LF are not included here as they have been deprecated in
  //  avago_serdes_dfe_tune()
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_rx_eq_cal_param_set(dev_id,
                                     serdes_p->ring,
                                     serdes_p->rx_sd,
                                     ctle_dc_hint,
                                     dfe_gain_range,
                                     pcal_loop_cnt);
  return BF_SUCCESS;
}

/** \brief Get RX EQ Calibration Parameters (Advanced)
 *
 * Get RX EQ calibration parameters
 *
 * This is a debug feature not recommended for general usage.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] ctle_dc_hint     : Hint for DC boost (0..255)
 * \param[out] dfe_gain_range   : DFE gain range (bits 7:0)
 * \param[out] pcal_loop_cnt    : Number of fine adjustment loops to run
 *                                  for each pCal run (1..15)
 *
 */
bf_status_t bf_serdes_rx_eq_cal_param_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int lane,
                                          int *ctle_dc_hint,
                                          int *dfe_gain_range,
                                          int *pcal_loop_cnt) {
  // See notes from _set() above
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_rx_eq_cal_param_get(dev_id,
                                     serdes_p->ring,
                                     serdes_p->rx_sd,
                                     ctle_dc_hint,
                                     dfe_gain_range,
                                     pcal_loop_cnt);
  return BF_SUCCESS;
}

/** \brief Run RX EQ Calibration (Advanced)
 *
 * Run RX EQ Calibration.  This is the advanced version of
 * bf_serdes_rx_ical_run() and bf_serdes_rx_eq_pcal_run() with more control
 * over the adaptation behavior.
 *
 * This function is intended for internal use.  User may call this function
 * if a standard calibration setting does not work well for user's channel.
 *
 * If CTLE or DFE calibration is bypassed, fixed settings need to be specified
 * prior to calling this function via bf_serdes_rx_eq_ctle_set() and
 * bf_serdes_rx_eq_dfe_set().
 *
 * If special EQ tuning parameters are needed, set up special params using
 * bf_serdes_rx_eq_cal_param_set() prior to calling this function.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  cal_cmd  : Command to control the calibration routine
 * \param[in]  ctle_cal_cfg : 5 bits [4:0]. Default is 0x00
 *                          bit[0]: 1: Do not tune DC.    0: Tune DC
 *                          bit[1]: 1: Do not tune LF.    0: Tune LF
 *                          bit[2]: 1: Do not tune HF.    0: Tune HF
 *                          bit[3]: 1: Do not tune BW.    0: Tune BW
 *                          bit[4]: 1: Use DC tune hint.  0: Do not use Hint
 * \param[in]  dfe_fixed  : 1: Do not tune DFE.  0: tune DFE
 *
 * \see bf_serdes_rx_eq_ctle_set(), bf_serdes_rx_eq_dfe_set()
 * \see bf_serdes_rxeq_cal_param_set()
 * \see bf_serdes_rx_eq_ical_run(), bf_serdes_rx_eq_pcal_run()
 */
bf_status_t bf_serdes_rx_eq_cal_adv_run(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bf_sds_rx_cal_mode_t cal_cmd,
                                        int ctle_cal_cfg,
                                        int dfe_fixed) {
  // Implementation Notes:
  //  Call avago_serdes_dfe_tune()
  //
  //  Which 0x0A bits are active for D6 serdes can be confusing.  Here is
  //  a summary with clarifications from Jiira AVAGO-268
  //
  //  bit Name                Effective   mode_control    Input Param
  //  3:0 Cal Action          Yes         tune_mode (*1)  cal_cmd
  //  4   Reserved            No
  //  5   DVOS for CM4 only   No
  //  6   Bypass DFE          Yes         dfe_disable     dfe_fixed
  //  7   Fixed DC            Yes         fixed_dc        ctle_cal_cfg[0]
  //  8   Fixed LF            Yes         fixed_lf        ctle_cal_cfg[1]
  //  9   Fixed HF            Yes         fixed_hf        ctle_cal_cfg[2]
  //  10  Fixed BW            Yes         fixed_bw        ctle_cal_cfg[3]
  //  11  Seeded DC           Yes         seeded_dc       ctle_cal_cfg[4]
  //  12  Seeded LF           No
  //  13  Seeded HF           No
  //  14  Reserved            No
  //  15  Run VOS only        Yes         N/A (*2)        cal_cmd=CAL_SLICER
  //
  //  *1. avago_serdes_dfe_tune() tune_mode to cal_cmd mapping
  //      tune_mode       cal_cmd             Int 0x0A DATA   tuning_flags
  //      ICAL            ICAL_PCAL           b[3:0]=0x1      0
  //      ICAL_ONLY       ICAL_NO_PCAL        b[3:0]=0x1      1
  //      PCAL            PCAL_ONCE           b[3:0]=0x2
  //      START_ADAPTIVE  PCAL_CONT_START     b[3:0]=0x6
  //      STOP_ADAPTIVE   PCAL_CONT_STOP      b[3:0]=0x2      (**1)
  //      ENABLE_RR       PCAL_RR_ENABLE      b[3:0]=0xA
  //      DISABLE_RR      PCAL_RR_DISABLE     b[3:0]=0x8
  //      N/A             CAL_SLICER_ONLY     b[15:0]=0x80
  //
  //      **1. STOP_ADAPTIVE should be Int 0x26, DATA[3:0]=0x0.  However,
  //          in avago_serdes_dfe_tune(), AVAGO_DFE_STOP_ADAPTIVE is mapped
  //          to 0x0002, which is doing a one time pCal and tune CTLE and DFE.
  //          Opened Jiira AVAGO-268, #9 (in follow up comments)
  //
  //  2. For cal_cmd=CAL_SLICER_ONLY, write to Int 0x0A=0x80 directly.
  //      avago_serdes_dfe_tune() does not have this implemented
  //
  // See notes from _set() above
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_rx_eq_cal_adv_run(dev_id,
                                   serdes_p->ring,
                                   serdes_p->rx_sd,
                                   cal_cmd,
                                   ctle_cal_cfg,
                                   dfe_fixed);
  return BF_SUCCESS;
}

/** \brief Run RX EQ Initial Calibration
 *
 * Start RX EQ initial calibration.  This function performs the following
 * calibrations:
 *  - Slicer offset calibration
 *  - CTLE Calibration
 *  - DFE Calibration
 *
 * bf_serdes_rx_eq_ical_eye_get() can be called to check for completion and
 * if calibration was successful.
 *
 * If the user's channel cannot be calibrated well with standard ical settings,
 * bf_serdes_rx_eq_cal_adv_run() may be used for finer RX EQ cal control.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \see bf_serdes_rx_eq_ical_eye_get()
 * \see bf_serdes_rx_eq_cal_adv_run()
 */
bf_status_t bf_serdes_rx_eq_ical_run(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane) {
  // Implementation Notes:
  //  Call bf_serdes_rx_eq_cal_adv_run() with
  //      cal_cmd         = BF_SDS_RX_ICAL_PCAL,
  //      ctle_cal_cfg    = 0x00,
  //      dfe_fixed       = 0
  //
  //  For standard use, only support full calibration.  If a customer's
  //  channel requires special calibration, rx_eq_cal_adv_run() will
  //  be used.
  return bf_serdes_rx_eq_cal_adv_run(
      dev_id, dev_port, lane, BF_SDS_RX_ICAL_PCAL, 0, 0);
  return BF_SUCCESS;
}

/** \brief Get RX EQ Cal Observed Eye Height (Advanced)
 *
 * Gets the eye height observed by the EQ calibration routine following
 * bf_serdes_rx_eq_ical_run().
 *
 * This is an advanced function since the eye reported by adaptation will
 * typically be larger than that reported by 2D eye scan which spends more
 * time accumulating errors at the eye boundary.  So adp_eye cannot be
 * evaluated quantatively.  It is a rough indicator.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] cal_eye  : (mVppd) Eye height observed by cal algorithm
 *
 * \see bf_serdes_rx_eq_ical_run()
 */
bf_status_t bf_serdes_rx_eq_cal_eye_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        int *cal_eye) {
  // Implementation Notes:
  //  Call avago_serdes_eye_get() with configp->ec_eye_type ==
  //  AVAGO_EYE_HEIGHT_DVOS. This retrieves testLEV results through Int 0x26
  //
  //  This might be useful to determine if adaptation is successful or not.
  //  adp_eye will most likely report a larger eye height than doing the
  //  2D eye scan.
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_rx_eq_cal_eye_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, cal_eye);
  return BF_SUCCESS;
}

/** \brief RX Equalizer Calibration is Done
 *
 * Check to see if RX EQ iCal has completed and if calibration
 * is successful.  cal_good is only relavent if cal_done = 1.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  cal_good_thres : (mVppd) cal_good=1 if cal_eye is above
 *                              threshold.  125mVppd is recommended.
 * \param[out] cal_done :  1: Completed. 0: In progress
 * \param[out] cal_good :  1: cal_eye above threshold.
 *                         0: cal_eye below threshold.
 *                        -1: cal_eye is < 50mVppd
 * \param[out] cal_eye  : (mVppd) Eye height observed by cal algorithm
 *
 * \see bf_serdes_rx_eq_ical_run()
 */
bf_status_t bf_serdes_rx_eq_ical_eye_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         int lane,
                                         int cal_good_thres,
                                         bool *cal_done,
                                         bool *cal_good,
                                         int *cal_eye) {
  // Implementation Notes:
  //  The basic procedure is the following:
  //
  //  1. Call bf_serdes_rx_eq_cal_busy() to see if RX EQ cal has completed.
  //      - To do batch iCal support, it would make sense to call
  //          cal_busy() and have it return right away so that multiple
  //          lanes can be pulled without long delays in between
  //
  //  2. Check internal eye height as observed by the tuning algorithm.
  //      This can be done by calling bf_serdes_rx_eye_cal_get().
  //
  //  3. Determin if the calibrated eye (cal_eye) is acceptable or not.
  //
  //      In Jiira AVAGO-267 Question 4, a min eye height of 0x10 (16) is
  //      recommended.  I assume 0x10 is the testLEV setting, which
  //      translates into 16 DAC pts above 0V + 16 DAC pts below 0V =
  //      32 * 1000mVppd/256 DAC range = 125mVppd.
  //
  //      Note that cal_eye is the eye height seen by the cal routine.
  //      Cal routine does not spend a lot of time at each point, so the
  //      actual eye is much smaller than that.
  //
  //      Avago serdes' min eye height is around 50mVppd.  So if cal_eye is
  //      less than 50mVppd, then we definitely have a problem.
  //
  bf_status_t bf_status;
  bool uc_busy;
  int dac_range;

  bf_status =
      bf_serdes_rx_eq_cal_busy_get(dev_id, dev_port, lane, 1, 0, &uc_busy);
  if (bf_status != BF_SUCCESS) return bf_status;

  if (uc_busy) {
    *cal_done = false;
    return BF_SUCCESS;
  }
  *cal_done = true;

  bf_status = bf_serdes_rx_eq_cal_eye_get(dev_id, dev_port, lane, cal_eye);
  if (bf_status != BF_SUCCESS) return bf_status;

  dac_range = ((*cal_eye * 2) * 1000) / 256;
  if (dac_range >= cal_good_thres) {
    *cal_good = true;
  } else {
    *cal_good = false;
  }
  return BF_SUCCESS;
}

/** \brief Start/Stop RX EQ Periodic Calibration
 *
 * Start periodic cal (pCal) or doing a one time pCal, which is equivalent to
 * stopping the pCal.
 *
 * pCal is fine DFE adjustment to adjust for slow changing temperature and
 * voltage conditions.  It will not disrupt traffic.  Prior to running pCal,
 * RX EQ initial calibration (iCal) must first be run to make sure the EQ
 * is tuned for a given channel.
 *
 * There is a device wide round robin mechanism to run pCal for each lane.
 * The round robin mechanism is controlled by bf_dev_rx_eq_cal_rr_set()
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  cal_cont : 1: Continuous pCal  0: One time pCal
 *
 * \see bf_serdes_rx_eq_ical_run()
 * \see bf_dev_rx_eq_cal_rr_set()
 */
bf_status_t bf_serdes_rx_eq_pcal_run(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int cal_cont) {
  // Implementation Notes:
  //  If cal_cont=True,
  //      call bf_serdes_rx_eq_cal_adv_run() with
  //          cal_cmd         = BF_SDS_RX_PCAL_CONT_START,
  //          ctle_cal_cfg    = 0x00,
  //          dfe_fixed       = 0
  //      then call bf_serdes_rx_eq_cal_adv_run() again with
  //          cal_cmd         = BF_SDS_RX_PCAL_RR_ENABLE,
  //          ctle_cal_cfg    = 0x00,
  //          dfe_fixed       = 0
  //
  //  If cal_cont=False,
  //      call bf_serdes_rx_eq_cal_adv_run() with
  //          cal_cmd         = BF_SDS_RX_PCAL_ONCE,
  //          ctle_cal_cfg    = 0x00,
  //          dfe_fixed       = 0
  //      then call bf_serdes_rx_eq_cal_adv_run() again with
  //          cal_cmd         = BF_SDS_RX_PCAL_RR_DISABLE,
  //          ctle_cal_cfg    = 0x00,
  //          dfe_fixed       = 0
  bf_status_t bf_status;

  if (cal_cont) {
    bf_status = bf_serdes_rx_eq_cal_adv_run(
        dev_id, dev_port, lane, BF_SDS_RX_PCAL_CONT_START, 0, 0);
    if (bf_status != BF_SUCCESS) return bf_status;

    bf_status = bf_serdes_rx_eq_cal_adv_run(
        dev_id, dev_port, lane, BF_SDS_RX_PCAL_RR_ENABLE, 0, 0);
    if (bf_status != BF_SUCCESS) return bf_status;
  } else {
    bf_status = bf_serdes_rx_eq_cal_adv_run(
        dev_id, dev_port, lane, BF_SDS_RX_PCAL_ONCE, 0, 0);
    if (bf_status != BF_SUCCESS) return bf_status;

    bf_status = bf_serdes_rx_eq_cal_adv_run(
        dev_id, dev_port, lane, BF_SDS_RX_PCAL_RR_DISABLE, 0, 0);
    if (bf_status != BF_SUCCESS) return bf_status;
  }
  return BF_SUCCESS;
}

/** \brief Get RX Equalizer Status
 *
 * Get RX EQ CTLE/DFE settings for a port including:
 *      - RXEQ Mode
 *      - adp_done
 *      - adp_success
 *      - adp_eye
 *      - ctle settings (ctle_dc/lf/hf/bw)
 *      - dfe_taps
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_status : RX EQ status
 */
bf_status_t bf_serdes_rx_eq_status_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int lane,
                                       bf_sds_rx_eq_status_t *rx_status) {
  int dfe_gain;

  bf_status_t bf_status;

  bf_status = bf_serdes_rx_eq_ical_eye_get(dev_id,
                                           dev_port,
                                           lane,
                                           16,  // FIXME
                                           &rx_status->cal_done,
                                           &rx_status->cal_good,
                                           &rx_status->cal_eye);
  if (bf_status != BF_SUCCESS) return bf_status;

  bf_status = bf_serdes_rx_eq_ctle_get(dev_id,
                                       dev_port,
                                       lane,
                                       &rx_status->ctle_dc,
                                       &rx_status->ctle_lf,
                                       &rx_status->ctle_hf,
                                       &rx_status->ctle_bw);
  if (bf_status != BF_SUCCESS) return bf_status;

  bf_status = bf_serdes_rx_eq_dfe_get(
      dev_id, dev_port, lane, &dfe_gain, &rx_status->dfe_taps[0]);
  if (bf_status != BF_SUCCESS) return bf_status;

  return BF_SUCCESS;
}

//---------------------------------------------------------------------
//  Section: Eye Measurement
//---------------------------------------------------------------------

/** \brief Set RX Comparison Slicer Position (Advanced)
 *
 * This is an advanced debug function to check eye margin.
 * This function sets the RX comparison slicer's x, y position (phase, vertical
 * offset) and enables comparison between data slicer and comparison slicer.
 *
 * To observe error count, call bf_serdes_pat_rx_err_cnt_get() or
 *
 * \note
 * Continuous adaptation for this channel will be disabled to access the
 * comparison slicer while this function is enabled.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  offset_en : 1: Enable comparison slicer offset.
 *                         0: Disable (Normal Traffic).
 * \param[in]  pos_x     : Horizontal Position ( -32.. 31 phase setting)
 * \param[in]  pos_y     : Vertical Position   (-500..500 mV)
 *
 * \see bf_serdes_pat_rx_err_cnt_get()
 */
bf_status_t bf_serdes_rx_eye_offset_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        int offset_en,
                                        int pos_x,
                                        int pos_y) {
  // Implementation Notes:
  //
  //  If offset_en == 0:
  //      If continuous DFE adp is enabled, resume DFE adaptation.
  //          We might be able to get this indicator from dfe_status[6]
  //      Call avago_serdes_set_rx_cmp_mode() to set cmp_mode to
  //          AVAGO_SERDES_RX_CMP_MODE_OFF
  //      Set pos_x and pos_y to 0, 0 (ignore user input) through
  //      avago_serdes_step_phase() and avago_serdes_set_dac()
  //
  //  If offset_en == 1:
  //      If continuous DFE is enabled, pause DFE adaptation
  //      Wait for DFE adp to release microcontroller
  //      Call avago_serdes_set_rx_cmp_mode() to set cmp_mode to
  //          AVAGO_SERDES_RX_CMP_MODE_XOR
  //
  //      Call avago_serdes_get_phase_multiplier() to determine
  //      how many phase steps are in an UI.  If divider=2, number of
  //      steps in one UI doubles.  At full rate, 1 UI is 64 steps
  //
  //      Call avago_serdes_step_phase() to adjust pos_x.
  //      Call avago_serdes_set_dac() to adjust pos_y.
  //          See aapl->eye.c->serdes_get_dvos_height() on how to
  //          convert mV to dac setting
  //              - Full dac range is 256 and it corresponds to 900mV (AVDD)
  //              - Average the 4 slicers (even/odd, 0 and 180 degree)
  //                  + some small offset.
  //      Call avago_serdes_get_errors() once to clear accumulated errors.
  //          Spico FW does error accumulation when it changes phase.  For
  //          this debug function's purpose, we don't need it but we can't
  //          disable it.  So we clear the error before returning.
  //
  //  Since this is a debug function, we probably don't need to implement
  //  Get().
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  if (offset_en == 0) {
    rc = port_mgr_av_sd_rx_cmp_mode_set(
        dev_id, serdes_p->ring, serdes_p->rx_sd, BF_SDS_PAT_PATSEL_OFF);
    if (rc != 0) return BF_INVALID_ARG;

    rc = port_mgr_av_sd_offset_set(
        dev_id, serdes_p->ring, serdes_p->rx_sd, 0, 0);
    if (rc != 0) return BF_INVALID_ARG;
  } else {
    bf_status_t bf_status;

    // disable pcal
    bf_status = bf_serdes_rx_eq_cal_adv_run(
        dev_id, dev_port, lane, BF_SDS_RX_PCAL_CONT_STOP, 0, 0);
    if (bf_status != BF_SUCCESS) return bf_status;

    rc = port_mgr_av_sd_offset_step_set(
        dev_id, serdes_p->ring, serdes_p->rx_sd, pos_x, pos_y);
    if (rc != 0) return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Measure Vertical or Horizontal Eye Opening
 *
 * Perform Vertical or Horizontal eye scan to a particular BER.
 * Eye opening in mV or mUI is returned.
 *
 * The eye opening qualifying condition is that there can be up to
 * 1 bit error in 3/BER received bits.  E.g. for BER=1e-6, 1 bit error in
 * 3e6 received bits.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  meas_mode : Specify vertical or horizontal eye scan
 * \param[in]  meas_ber  : Specify 1e-6 or 1e-9 BER target
 * \param[out] meas_eye  : Measured eye height (mV) or eye width (mUI)
 *
 */
bf_status_t bf_serdes_rx_eye_get(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 int lane,
                                 bf_sds_rx_eye_meas_mode_t meas_mode,
                                 bf_sds_rx_eye_meas_ber_t meas_ber,
                                 int *meas_eye) {
  // Implementation Notes:
  //
  //  We can use avago_serdes_eye_get() or we can implement our own scan.
  //  avago_serdes_eye_get() does quite a bit more than we need and is
  //  more complicated.  It also does not have a batch mode.
  //
  //  If we do our own, see Implementation notes for
  //  bf_serdes_rx_eye_batch_get().  If we use AAPL, then see below:
  //
  //  Call avago_serdes_eye_get()
  //      set configp->ec_eye_type=AVAGO_EYE_WIDTH/HEIGHT
  //      set configp->ec_max_dwell_bits based on BER
  //          if 1e-6 BER, set to 3e6
  //          if 1e-9 BER, set to 3e9
  //      set configp->ec_min_dwell_bits to 1e3 bits
  //      set configp->ec_error_threshold to 2
  //          I think 2 is for up to 1 bit error
  //
  //  get datap->ed_width_mUI or datap->ed_height_mV
  //      If these are not filled in, then ed_width and ed_height.  We
  //      will need to convert them to mUI and mV.  AAPL generally assumes
  //      AVDD to be 1000mV.  Ours is 900mV.  So their conversion may be off.
  //
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_eye_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, meas_mode, meas_ber, meas_eye);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief 3D Eye Scan
 *
 * Perform 3D eye scan for link debug.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  meas_ber  : Specify 1e-6 or 1e-9 BER target
 * \param[out] meas_data : Memory allocated by user to store measured data
 */
bf_status_t bf_serdes_rx_eye_3d_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bf_sds_rx_eye_meas_ber_t meas_ber,
                                    char *eye_plot_data,
                                    int max_eye_plot_data) {
  // Implementation Notes:
  //  Call avago_serdes_eye_get()
  //      set configp->ec_eye_type=AVAGO_EYE_FULL
  //      set configp->ec_max_dwell_bits based on BER
  //          if 1e-6 BER, set to 3e6
  //          if 1e-9 BER, set to 3e9
  //      The rest of the settings can be default
  //
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_full_eye_get(dev_id,
                                      serdes_p->ring,
                                      serdes_p->rx_sd,
                                      meas_ber,
                                      eye_plot_data,
                                      max_eye_plot_data);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Save 3D Eye Scan Data
 *
 * Saves 3D eye scan data to a file
 *
 * \param[in]  meas_data : Memory allocated by user to store measured data
 * \param[in]  file_loc  : Save file location
 *
 * \see bf_serdes_rx_eye_3d_get()
 */
bf_status_t bf_serdes_rx_eye_3d_save(int *meas_data, int *file_loc) {
  // Implementation Notes:
  //  Not sure what is the right data type for file_loc

  (void)meas_data;
  (void)file_loc;

  return BF_SUCCESS;
}

//---------------------------------------------------------------------
//  Section: Pattern Generator / Checker
//---------------------------------------------------------------------

/** \brief Force inject Tx bit errors on a serdes slice
 *
 * Insert multiple single bit errors (as specified by num_bits) on the
 * transmit data output.  This is a debug feature to intentionally corrupts
 * transmit serial data.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  num_bits : Number or error bits to inject (0..65535)
 */
bf_status_t bf_serdes_tx_err_inj_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int num_bits) {
  //  Same as bf_serdes_set_tx_inject_error()
  //
  //  Probably calls avago_serdes_tx_inject_error() at some point
  port_mgr_serdes_t *serdes_p;

  if ((num_bits < 0) || (num_bits > 65535)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_tx_error_inject_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, num_bits);
  return BF_SUCCESS;
}

/** \brief Force inject Rx bit errors on a serdes slice
 *
 * Insert multiple single bit errors (as specified by num_bits) on the
 * receive data input.  This is a debug feture to intentionally corrupts
 * receive serial data.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  num_bits : Number or error bits to inject (0..65535)
 */
bf_status_t bf_serdes_rx_err_inj_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int num_bits) {
  //  Same as bf_serdes_set_rx_inject_error()
  //
  //  Probably calls avago_serdes_rx_inject_error() at some point
  port_mgr_serdes_t *serdes_p;

  if ((num_bits < 0) || (num_bits > 65535)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_rx_error_inject_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, num_bits);
  return BF_SUCCESS;
}

/** \brief Set Transmit Pattern Generator
 *
 * Sets TX Pattern Generator.  Supported modes are:
 *      - Normal Traffic:   Core data, PRBS off
 *      - PRBS Pattern:     PRBS-7/9/11/15/23/31
 *      - Fixed Pattern:    80b User defined fixed pattern
 *
 * To set 80b TX fixed pattern, call bf_serdes_tx_fixed_pat_set()
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_patsel : Select transmit data / PRBS pattern
 *
 * \see bf_serdes_tx_userpat_set()
 */
bf_status_t bf_serdes_tx_patsel_set(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bf_sds_pat_patsel_t tx_patsel) {
  //  Same as bf_serdes_set_tx_data_sel(), but without loopback.
  //  Loopback is set via bf_serdes_lane_loopback_set()
  port_mgr_serdes_t *serdes_p;

  switch (tx_patsel) {
    case BF_SDS_PAT_PATSEL_PRBS7:
    case BF_SDS_PAT_PATSEL_PRBS9:
    case BF_SDS_PAT_PATSEL_PRBS11:
    case BF_SDS_PAT_PATSEL_PRBS15:
    case BF_SDS_PAT_PATSEL_PRBS23:
    case BF_SDS_PAT_PATSEL_PRBS31:
    case BF_SDS_PAT_PATSEL_FIXED:
    case BF_SDS_PAT_PATSEL_OFF:
      break;
    default:
      return BF_INVALID_ARG;
  }

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_tx_data_sel_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, tx_patsel);
  return BF_SUCCESS;
}

/** \brief Get Transmit Pattern Generator
 *
 * Gets TX Pattern Generator.  Supported modes are:
 *      - Normal Traffic:   Core data, PRBS off
 *      - PRBS Pattern:     PRBS-7/9/11/15/23/31
 *      - Fixed Pattern:    80b User defined fixed pattern
 *
 * To get 80b TX fixed pattern, call bf_serdes_tx_fixed_pat_get()
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] tx_patsel : Select transmit data / PRBS pattern
 *
 * \see bf_serdes_tx_userpat_get()
 */
bf_status_t bf_serdes_tx_patsel_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bf_sds_pat_patsel_t *tx_patsel) {
  //  Same as bf_serdes_set_tx_data_gel(), but without loopback.
  //  Loopback is get via bf_serdes_lane_loopback_get()
  port_mgr_serdes_t *serdes_p;

  if (tx_patsel == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *tx_patsel =
      port_mgr_av_sd_tx_data_sel_get(dev_id, serdes_p->ring, serdes_p->tx_sd);
  return BF_SUCCESS;
}

/** \brief Set Transmit Fixed Pattern
 *
 * Sets 80b fixed transmit user pattern.  This is useful for PLL output jitter
 * measurement where a clock pattern is needed, or for debugs where a specific
 * data pattern (pulses or walking 1's pattern) is needed.
 *
 * The 80b fixed pattern is configured via four 32b words.  Only bits[19:0] of
 * each word is used.  Transmit order is LSBit first.  First bit transmitted
 * out serially is tx_fixed_pat[0][0].
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  tx_fixed_pat[4] : 80b fixed pattern in 4 32b words. Only
 *                               bits[19:0] are used.  LSBit transmitted first.
 */
bf_status_t bf_serdes_tx_fixed_pat_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int lane,
                                       int tx_fixed_pat[4]) {
  // Implementation Notes:
  //  Call avago_serdes_set_tx_user_data()
  int rc;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_fixed_pat_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, tx_fixed_pat);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get Transmit Fixed Pattern
 *
 * Gets 80b fixed transmit pattern.  This is useful for PLL output jitter
 * measurement where a clock pattern is needed, or for debugs where a specific
 * data pattern (pulses or walking 1's pattern) is needed.
 *
 * The 80b fixed pattern is stored in four 32b words.  Only bits[19:0] of
 * each word is used.  Transmit order is LSBit first.  First bit transmitted
 * out serially is tx_fixed_pat[0][0].
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] tx_fixed_pat[4] : 80b fixed pattern in 4 32b words. Only
 *                               bits[19:0] are used.  LSBit transmitted first.
 */
bf_status_t bf_serdes_tx_fixed_pat_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int lane,
                                       int tx_fixed_pat[4]) {
  // Implementation Notes:
  //  Call avago_serdes_get_tx_user_data()
  int rc;
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_fixed_pat_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, tx_fixed_pat);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Set Receive Pattern Checker
 *
 * Sets RX Pattern Checker.  Supported modes are:
 *      - Normal Traffic:       Core data, PRBS off
 *      - PRBS Pattern:         PRBS-7/9/11/15/23/31
 *      - Fixed Pattern:        80b Fixed Pattern
 *
 * The PRBS and Fixed pattern checker performs auto reseeding.  If errors are
 * detected for two consecutive words, a new seed (main data) will be used
 * to reseed the PRBS and fixed pattern checker.
 *
 * Fixed pattern uses incoming data to seed a reference, then compares
 * subsequent incoming data against it.
 *
 * \note    Continuous pCal needs to be disabled when RX pattern checker is
 *          enabled.  User needs to re-enable pCal by calling
 *          bf_serdes_rx_eq_pcal_run()
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[in]  rx_patsel : Receive data / PRBS pattern
 *
 * \see bf_serdes_rx_eq_pcal_run()
 */
bf_status_t bf_serdes_rx_patsel_set(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bf_sds_pat_patsel_t rx_patsel) {
  // Implementation Notes:
  //
  //  RX EQ pCal needs to be first disabled as both PRBS and Fixed pattern
  //  checkers needs to repurpose the data comparator.
  //      Call bf_serdes_rx_eq_pcal_run() with cal_cont=0
  //
  //  Call avago_serdes_set_rx_cmp_data() to configure the PRBS pattern.
  //
  //      Note that bf_sds_pat_patsel_t is not defined the same way as
  //      Avago_serdes_rx_cmp_data_t.  So we cannot pass it in as param data
  //      directly.  We can define them the same way, but it makes CLI
  //      interface a little harder to use in that 0 is not mission data
  //      (8 is mission data).
  //
  //  Call avago_serdes_set_rx_cmp_mode() to configure the inputs to the
  //  comparator.
  //      When PRBS is enabled,       mode =
  //      AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN
  //      When main data is selected, mode = AVAGO_SERDES_RX_CMP_MODE_OFF
  //      (As an aside, RX EQ Calibration uses AVAGO_SERDES_RX_CMP_MODE_XOR
  //      to compare main data with comparator slicer).
  //
  // bf_status_t bf_status;
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  // bf_status = bf_serdes_rx_eq_pcal_run(dev_id, dev_port, lane, 0);
  // if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_patsel_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, rx_patsel);
  port_mgr_log("rc=%d : port_mgr_av_sd_rx_patsel_set\n", rc);

  if (rc != 0) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_cmp_mode_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, rx_patsel);
  port_mgr_log("rc=%d : port_mgr_av_sd_rx_cmp_mode_set\n", rc);

  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get Receive Pattern Checker
 *
 * Gets RX Pattern Checker.  Supported modes are:
 *      - Normal Traffic:       Core data, PRBS off
 *      - PRBS Pattern:         PRBS-7/9/11/15/23/31
 *      - Fixed Pattern:        80b Fixed Pattern
 *
 * The PRBS and Fixed pattern checker performs auto reseeding.  If errors are
 * detected for two consecutive words, a new seed (main data) will be used
 * to reseed the PRBS and fixed pattern checker.
 *
 * Fixed pattern uses incoming data to seed a reference, then compares
 * subsequent incoming data against it.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_patsel : Receive data / PRBS pattern
 */
bf_status_t bf_serdes_rx_patsel_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bf_sds_pat_patsel_t *rx_patsel) {
  // Implementation Notes:
  //
  //  Call avago_serdes_get_rx_cmp_mode() to configure the inputs to the
  //  comparator.
  //      When PRBS is enabled,       mode =
  //      AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN
  //      When main data is selected, mode = AVAGO_SERDES_RX_CMP_MODE_OFF
  //      (As an aside, RX EQ Calibration uses AVAGO_SERDES_RX_CMP_MODE_XOR
  //      to compare main data with comparator slicer).
  //
  //  Call avago_serdes_get_rx_cmp_data() to configure the PRBS pattern.
  //
  //      Note that bf_sds_pat_patsel_t is not defined the same way as
  //      Avago_serdes_rx_cmp_data_t.  So we cannot pass it in as param data
  //      directly.  We can define them the same way, but it makes CLI
  //      interface a little harder to use in that 0 is not mission data
  //      (8 is mission data).
  //
  //  Same as bf_serdes_set_tx_data_gel(), but without loopback.
  //  Loopback is get via bf_serdes_lane_loopback_get()
  port_mgr_serdes_t *serdes_p;

  if (rx_patsel == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *rx_patsel =
      port_mgr_av_sd_rx_patsel_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Captured 80b of Received Data
 *
 * Capture 80b of received data pattern.  This is a debug function to see what
 * data is being received (no data, random or known pattern).  This function
 * is most useful if a known repeated pattern < 80b is sent.
 *
 * The 80b captured pattern is stored in four 32b words.  Only bits[19:0] of
 * each word is used.  Receive order is LSBit first.  First bit received
 * serially is rx_cap_pat[0][0].
 *
 * \note    Continuous pCal needs to be disabled when RX pattern checker is
 *          enabled.  User needs to re-enable pCal by calling
 *          bf_serdes_rx_eq_pcal_run()
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_cap_pat[4] : 80b captured pattern in 4 32b words. Only
 *                             bits[19:0] are used.  LSBit transmitted first.
 */
bf_status_t bf_serdes_rx_data_cap_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int rx_cap_pat[4]) {
  //  RX EQ pCal needs to be first disabled as rx_data_cap uses the
  //  data comparator.
  //      Call bf_serdes_rx_eq_pcal_run() with cal_cont=0
  //
  //  Call avago_serdes_get_rx_cmp_mode(),
  //      set mode=AVAGO_SERDES_RX_CMP_MODE_OFF
  //      If pCal is on, mode may be set to AVAGO_SERDES_RX_CMP_MODE_XOR
  //
  //  Call avago_serdes_get_rx_data() to do a capture (Int 0x1C) and read
  //      the 80b data
  bf_status_t bf_status;
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  bf_status = bf_serdes_rx_eq_pcal_run(dev_id, dev_port, lane, 0);

  if (bf_status != BF_SUCCESS) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_cmp_mode_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, BF_SDS_PAT_PATSEL_OFF);
  if (rc != 0) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_data_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, rx_cap_pat);
  if (rc != 0) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Get Receive Comparator Error Count
 *
 * Get RX comparator error count.
 *
 * For the error counter to activate, bf_serdes_rx_patsel_set() needs to be
 * called first to put the receive in PRBS or Fixed pattern checking mode.
 *
 * The error counter self-clears on read and saturates at 0xFFFF_FFFF.
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  port     : Physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param[in]  lane     : Logical lane within port (0..3, mode dependent)
 *
 * \param[out] rx_err_cnt : Accumulated error count, pegs at 0xffffffff
 *
 * \see bf_serdes_rx_patsel_set()
 */
bf_status_t bf_serdes_rx_err_cnt_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     uint32_t *rx_err_cnt) {
  // Same as bf_serdes_get_error_count(), which calls avago_get_errors()
  //   with reset flag = True
  port_mgr_serdes_t *serdes_p;

  if (rx_err_cnt == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *rx_err_cnt =
      port_mgr_av_sd_get_error_count(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Set or clear AN enable
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param en      : state to set an_en
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_en_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool en) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_autoneg_en_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, (en ? 1 : 0));
    if (rc == 0) return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set AN advertisement
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param base_pg : lower 48b contain base page advertsement
 * \param num_next_pg: # of next pages to be loaded
 * \param next_pg:  ptr to array of uint64_t values representing the
 *                  next page advertiements (in the lower 48b of each)
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes on this port (in this
 *mode)
 *
 */
bf_status_t bf_serdes_autoneg_advert_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         int lane,
                                         uint64_t base_pg,
                                         int num_next_pg,
                                         uint64_t *next_pg) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_autoneg_advert_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, base_pg, num_next_pg, next_pg);
    if (rc == 0) return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Start autoneg
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param base_pg : lower 48b contain base page advertsement
 * \param num_next_pg: # of next pages to be loaded
 * \param disable_nonce_match: debug flag, enables AN on loopback modules
 * \param disable_link_inhibit_timer: debug flag, LT can extend beyond 510ms
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_start(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    uint64_t base_pg,
                                    int num_next_pg,
                                    bool disable_nonce_match,
                                    bool disable_link_inhibit_timer) {
  port_mgr_serdes_t *serdes_p;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, /*lane*/ 0);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_autoneg_start(dev_id,
                                      serdes_p->ring,
                                      serdes_p->tx_sd,
                                      base_pg,
                                      num_next_pg,
                                      disable_nonce_match,
                                      disable_link_inhibit_timer);
    if (rc == 7) return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Stop autoneg
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 *
 * \return: BF_SUCCESS       : tx_output_en set successfully
 * \return: BF_INVALID_ARG   : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG   : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG   : lane > # of serdes lanes on this port (in this
 *mode)
 *
 */
bf_status_t bf_serdes_autoneg_stop(bf_dev_id_t dev_id, bf_dev_port_t dev_port) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, /*lane*/ 0);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_autoneg_stop(dev_id, serdes_p->ring, serdes_p->tx_sd);
  if (rc == 0) return BF_SUCCESS;

  return BF_INVALID_ARG;
}

/** \brief Assert restart_training
 *
 * \param dev_id: bf_dev_id_t  : system-assigned identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param port: bf_dev_port_t: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS       : tx_output_en set successfully
 * \return: BF_INVALID_ARG   : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG   : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG   : lane > # of serdes lanes on this port (in this
 *mode)
 *
 */
bf_status_t bf_serdes_link_training_restart(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            int lane) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_pmd_training_restart(
      dev_id, serdes_p->ring, serdes_p->tx_sd);
  if (rc == 0) return BF_SUCCESS;

  return BF_INVALID_ARG;
}

/** \brief Get AN_GOOD state
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane: logical lane (within port) 0-3, depending upon mode
 * \param st
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_an_good_get(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  bool *an_good) {
  port_mgr_serdes_t *serdes_p;
  uint32_t st;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, 0);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  st = port_mgr_av_sd_an_good_get(dev_id, serdes_p->ring, serdes_p->rx_sd);

  *an_good = (st == 0) ? false : true;
  return BF_SUCCESS;
}

/** \brief Get AN_COMPLETE state
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane: logical lane (within port) 0-3, depending upon mode
 * \param st
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_an_complete_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bool *an_cmplt) {
  port_mgr_serdes_t *serdes_p;
  uint32_t st;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, 0);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  st = port_mgr_av_sd_an_cmplt_get(dev_id, serdes_p->ring, serdes_p->rx_sd);

  *an_cmplt = (st == 0) ? false : true;
  return BF_SUCCESS;
}

/** \brief Get autoneg state
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane: logical lane (within port) 0-3, depending upon mode
 * \param st
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_st_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     bf_an_state_e *st) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, 0);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_autoneg_st_get(dev_id, serdes_p->ring, serdes_p->rx_sd);

    if (rc == -1) {
      return BF_INVALID_ARG;
    }
    *st = rc;

    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get autoneg hcd and fec resolution
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param st
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_hcd_fec_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          bf_port_speed_t *hcd,
                                          bf_fec_type_t *fec,
                                          uint32_t *av_hcd) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, 0);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_autoneg_hcd_fec_get(
        dev_id, serdes_p->ring, serdes_p->rx_sd, hcd, fec, av_hcd);
    if (rc == -1) {
      return BF_INVALID_ARG;
    }
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get link-training state
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param lt_st   :
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_link_training_st_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           bf_lt_state_e *lt_st) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_link_training_st_get(
        dev_id, serdes_p->ring, serdes_p->rx_sd, lt_st);
    if (rc == -1) {
      return BF_INVALID_ARG;
    }
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get link-training state (extended)
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param lt_st   :
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_link_training_st_extended_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    int lane,
                                                    int *failed,
                                                    int *in_prg,
                                                    int *rx_trnd,
                                                    int *frm_lk,
                                                    int *rmt_rq,
                                                    int *lcl_rq,
                                                    int *rmt_rcvr_rdy) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_link_training_st_extended_get(dev_id,
                                                 serdes_p->ring,
                                                 serdes_p->rx_sd,
                                                 failed,
                                                 in_prg,
                                                 rx_trnd,
                                                 frm_lk,
                                                 rmt_rq,
                                                 lcl_rq,
                                                 rmt_rcvr_rdy);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get all AN state variables
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param lt_st   :
 *
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_all_state(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bool *lp_base_pg_rdy,
                                        bool *lp_next_pg_rdy,
                                        bool *an_good,
                                        bool *an_complete,
                                        bool *an_failed) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_an_status_get(dev_id,
                               serdes_p->ring,
                               serdes_p->rx_sd,
                               lp_base_pg_rdy,
                               lp_next_pg_rdy,
                               an_good,
                               an_complete,
                               an_failed);
  return BF_SUCCESS;
}

/** \brief Get link-partners Base Page
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param lp_base_pg:
 *
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_lp_base_pg_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int lane,
                                             uint64_t *lp_base_pg) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *lp_base_pg =
      port_mgr_av_sd_lp_base_pg_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Get link-partners Next Page
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param lp_base_pg:
 *
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_lp_next_pg_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int lane,
                                             uint64_t *lp_next_pg) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *lp_next_pg =
      port_mgr_av_sd_lp_next_pg_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Get link-partners Next Page
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param lp_base_pg:
 *
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_autoneg_next_pg_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int lane,
                                          uint64_t next_pg) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_next_pg_set(dev_id, serdes_p->ring, serdes_p->tx_sd, next_pg);
  return BF_SUCCESS;
}

/** \brief Get the Tx equalization settings on a Tofino serdes lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param pre     : pre-cursor
 * \param atten   : attenuation
 * \param post    : post-cursor
 *
 * \return: BF_SUCCESS    : Tx Eq values set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port > LLD_MAX_PORTS-1
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: pre/post/atten NULL
 *
 */
bf_status_t bf_serdes_get_tx_eq(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                int *pre,
                                int *atten,
                                int *post) {
  port_mgr_serdes_t *serdes_p =
      port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);

  if ((pre == NULL) || (post == NULL) || (atten == NULL)) return BF_INVALID_ARG;

  if (serdes_p) {
    int rc;

    rc = port_mgr_av_sd_get_tx_eq(
        dev_id, serdes_p->ring, serdes_p->tx_sd, pre, atten, post);
    if (rc == 0) {
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/** \brief Set the Tx equalization parameters on a Tofino serdes lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: bf_dev_port_t : physical port # on dev_id
 *(0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param pre     : pre-cursor
 * \param atten   : attenuation
 * \param post    : post-cursor
 *
 * \return: BF_SUCCESS    : Tx Eq values set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: port > LLD_MAX_PORTS-1
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid pre/post/atten setting
 *
 */
bf_status_t bf_serdes_set_tx_eq(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                int pre,
                                int atten,
                                int post) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (!serdes_p) return BF_INVALID_ARG;
  int rc = 0;
  int pre_min, pre_max, atten_min, atten_max, post_min, post_max;

  port_mgr_av_sd_get_tx_eq_limits(dev_id,
                                  serdes_p->ring,
                                  serdes_p->tx_sd,
                                  &pre_min,
                                  &atten_min,
                                  &post_min,
                                  &pre_max,
                                  &atten_max,
                                  &post_max);
  if ((pre < pre_min) || (pre > pre_max)) return BF_INVALID_ARG;
  if ((atten < atten_min) || (atten > atten_max)) return BF_INVALID_ARG;
  if ((post < post_min) || (post > post_max)) return BF_INVALID_ARG;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  // save for future use
  serdes_p->tx_eq_pre = pre;
  serdes_p->tx_eq_atten = atten;
  serdes_p->tx_eq_post = post;
  // Configure Tx EQ parameters
  rc = port_mgr_av_sd_set_tx_eq(dev_id,
                                serdes_p->ring,
                                serdes_p->tx_sd,
                                serdes_p->tx_eq_pre,
                                serdes_p->tx_eq_atten,
                                serdes_p->tx_eq_post);
  if (rc) return BF_INVALID_ARG;
  return BF_SUCCESS;
}

/** \brief Check that the PLLs match the expected divider
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param divider : expected Tx/Rx divider (assumed to be the same)
 *
 * \return: BF_SUCCESS    : PLL state matches expected value
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: error from serdes API (PLL incorrect)
 *
 */
bf_status_t bf_serdes_get_pll_state(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    int expected_divider) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    int rc;

    rc = port_mgr_av_sd_check_tx_pll_state(
        dev_id, serdes_p->ring, serdes_p->tx_sd, expected_divider);
    if (rc == 0) {
      rc = port_mgr_av_sd_check_rx_pll_state(
          dev_id, serdes_p->ring, serdes_p->rx_sd, expected_divider);
      if (rc == 0) {
        return BF_SUCCESS;
      }
    }
  }
  return BF_INVALID_ARG;
}

/** \brief Get current state of tx_output_en from a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param en      : returned state of tx_output_en
 *
 * \return: BF_SUCCESS    : tx_output_en state retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: en == NULL
 *
 */
bf_status_t bf_serdes_get_tx_output_en(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int lane,
                                       bool *en) {
  port_mgr_serdes_t *serdes_p;

  if (en == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *en = port_mgr_av_sd_get_tx_output_en(
        dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set current state of tx_output_en on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param en      : state to set tx_output_en
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_set_tx_output_en(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int lane,
                                       bool en) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_set_tx_output_en(
        dev_id, serdes_p->ring, serdes_p->tx_sd, (en ? 1 : 0));
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set rx_en, tx_en, and tx_output_en in one shot
 *
 * \param dev_id       : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port     : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane         : logical lane (within port) 0-3, depending upon mode
 * \param rx_en        : state to set tx_output_en
 * \param tx_en        : state to set tx_output_en
 * \param tx_output_en : state to set tx_output_en
 *
 * \return: BF_SUCCESS    : tx_output_en set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_set_rx_tx_and_tx_output_en(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 int lane,
                                                 bool rx_en,
                                                 bool tx_en,
                                                 bool tx_output_en) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    if (serdes_p->tx_sd == serdes_p->rx_sd) {
      port_mgr_av_sd_set_rx_tx_and_tx_output_en(
          dev_id, serdes_p->ring, serdes_p->tx_sd, rx_en, tx_en, tx_output_en);
    } else {  // must do rx and tx separately
      return BF_INVALID_ARG;
    }
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

bf_status_t bf_serdes_set_rx_tx_and_tx_output_en_allow_unassigned(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    int lane,
    bool rx_en,
    bool tx_en,
    bool tx_output_en) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    if (serdes_p->tx_sd == serdes_p->rx_sd) {
      port_mgr_av_sd_set_rx_tx_and_tx_output_en(
          dev_id, serdes_p->ring, serdes_p->tx_sd, rx_en, tx_en, tx_output_en);
    } else {  // must do rx and tx separately
      return BF_INVALID_ARG;
    }
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Enable or disable the sig_ok threshold
 *
 **************************************************************
 * Note: The sig_ok threshold MUST BE DISABLED for DFE to work.
 **************************************************************
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param en    : enable or disable sig ok threshold
 *
 * \return: BF_SUCCESS    : signal_ok threshold enabled successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: thresh == NULL
 *
 */
bf_status_t bf_serdes_signal_ok_thresh_enable_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  int lane,
                                                  bool en) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  port_mgr_av_sd_spico_int(
      dev_id, serdes_p->ring, serdes_p->rx_sd, 0x20, en ? 0x20 : 0x0);
  return BF_SUCCESS;
}

/** \brief Get frequency lock status for a slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param flock : returned: 1=locked, 0=not locked
 *
 * \return: BF_SUCCESS    : signal_ok threshold enabled successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: thresh == NULL
 *
 */
bf_status_t bf_serdes_frequency_lock_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         int lane,
                                         bool *flock) {
  port_mgr_serdes_t *serdes_p;

  if (flock == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *flock = port_mgr_av_sd_frequency_lock_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd);
  return BF_SUCCESS;
}

/** \brief Get signal_ok threshold from a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param thresh: returned signal ok threshold
 *
 * \return: BF_SUCCESS    : signal_ok threshold retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: thresh == NULL
 *
 */
bf_status_t bf_serdes_get_signal_ok_thresh(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           int *thresh) {
  port_mgr_serdes_t *serdes_p;

  if (thresh == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *thresh = port_mgr_av_sd_signal_ok_thresh_get(
        dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set signal_ok threshold n a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param thresh: threshold to set (0-15)
 *
 * \return: BF_SUCCESS    : signal_ok threshold set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: thresh > 15
 *
 */
bf_status_t bf_serdes_set_signal_ok_thresh(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           int thresh) {
  port_mgr_serdes_t *serdes_p;

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  if ((thresh < 0) || (thresh > 15)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (!serdes_p) return BF_INVALID_ARG;

  // save for FSM
  serdes_p->rx_sig_ok_thresh = thresh;
  port_mgr_av_sd_signal_ok_thresh_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_sig_ok_thresh);
  return BF_SUCCESS;
}

/** \brief Get signal_ok status from a serdes slice. Note this API
 *         uses avago_serdes_get_signal_ok() which returns a latched
 *         status of LOS. NOT_LOS is defined as "signal_ok". To get
 *         the current status the API is called twice. This discards
 *         the latched LOS indication and returns the current status.
 *         If the latched LOS status is important then users should
 *         call port_mgr_port_check_los() prior to calling this API.
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param sig_ok: returned signal ok, 1=ok, 0=not ok (los)
 *
 * \return: BF_SUCCESS    : signal_ok status retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: sig_ok == NULL
 *
 */
bf_status_t bf_serdes_get_signal_ok(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool *sig_ok) {
  port_mgr_serdes_t *serdes_p;

  if (sig_ok == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *sig_ok =
        port_mgr_av_sd_signal_ok_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get LOS status from a serdes slice. Note this API
 *         uses avago_serdes_get_signal_ok() which returns a latched
 *         status of LOS. To get the current status call this API
 *         twice (or use the port_mgr_port_check_signal_ok API).
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param los   : returned LOS indication, 1=LOS, 0=no los
 *
 * \return: BF_SUCCESS    : LOS state retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t0
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: los == NULL
 *
 */
bf_status_t bf_serdes_get_los(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              int lane,
                              bool *los) {
  port_mgr_serdes_t *serdes_p;

  if (los == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *los = port_mgr_av_sd_los_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get LOS status from a serdes slice. Note this API
 *         uses avago_serdes_get_signal_ok() which returns a latched
 *         status of LOS. To get the current status call this API
 *         twice (or use the port_mgr_port_check_signal_ok API).
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param los   : returned LOS indication, 1=LOS, 0=no los
 *
 * \return: BF_SUCCESS    : LOS state retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t0
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: los == NULL
 *
 */
bf_status_t bf_serdes_elec_idle_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool *ei) {
  port_mgr_serdes_t *serdes_p;

  if (ei == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *ei = port_mgr_av_sd_elec_idle_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Start DFE ICAL (coarse tuning) on a serdes slice.
 *
 * [ POST_ENABLE ]
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE started successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_start_dfe_ical(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_start_dfe_ical(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Start DFE ICAL (coarse tuning) on a serdes slice.
 *
 * [ POST_ENABLE ]
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE started successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_start_dfe_ical_allow_unassigned(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_start_dfe_ical(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Start DFE PCAL (fine tuning) on a serdes slice.
 *
 * [ POST_ENABLE ]
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE started successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_start_dfe_pcal(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_start_dfe_pcal(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Start DFE PI CAL (phase interpolator cal) on a serdes slice.
 *
 * [ POST_ENABLE ]
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE started successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_start_dfe_pi_cal(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_start_dfe_pi_cal(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Start DFE adaptive (continuous tuning) on a serdes slice.
 *
 * [ POST_ENABLE ]
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE started successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_start_dfe_adaptive(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_start_dfe_adaptive(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Stop DFE adaptive (continuous tuning) on a serdes slice.
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE stopped successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_stop_dfe_adaptive(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_stop_dfe_adaptive(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Stop DFE (ICAL or PCAL) on a serdes slice.
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : DFE stopped successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_stop_dfe(bf_dev_id_t dev_id,
                               bf_dev_port_t dev_port,
                               int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_stop_dfe(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Check if DFE is running on a serdes slice.
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port   : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 * \param dfe_running: 0=not running, 1=running
 *
 * \return: BF_SUCCESS    : DFE running status retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: dfe_running == NULL
 *
 */
bf_status_t bf_serdes_get_dfe_running(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      bool *dfe_running) {
  port_mgr_serdes_t *serdes_p;

  if (dfe_running == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *dfe_running = port_mgr_av_sd_check_dfe_running(
        dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Check if DFE is running on a serdes slice.
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port   : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 * \param dfe_running: 0=not running, 1=running
 *
 * \return: BF_SUCCESS    : DFE running status retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: dfe_running == NULL
 *
 */
bf_status_t bf_serdes_get_dfe_running_allow_unassigned(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       int lane,
                                                       bool *dfe_running) {
  port_mgr_serdes_t *serdes_p;

  if (dfe_running == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *dfe_running = port_mgr_av_sd_check_dfe_running(
        dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get Tx polarity from a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param inv   : returned, 1=inverted, 0=not inverted
 *
 * \return: BF_SUCCESS    : Tx invert state retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: inv == NULL
 *
 */
bf_status_t bf_serdes_get_tx_invert(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool *inv) {
  port_mgr_serdes_t *serdes_p;

  if (inv == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *inv =
        port_mgr_av_sd_tx_invert_get(dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Tx polarity on a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param inv   : 1=invert, 0=dont invert
 *
 * \return: BF_SUCCESS    : Tx invert set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_set_tx_invert(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool inv) {
  port_mgr_serdes_t *serdes_p;
  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) return BF_INVALID_ARG;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (!serdes_p) return BF_INVALID_ARG;

  // save for FSM
  serdes_p->tx_inv = inv ? true : false;
  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    /* Don't touch the harware during cfg replay */
    return BF_SUCCESS;
  }
  port_mgr_av_sd_tx_invert_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, serdes_p->tx_inv ? 1 : 0);
  return BF_SUCCESS;
}

/** \brief Get Rx polarity from a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param inv   : returned, 1=inverted, 0=not inverted
 *
 * \return: BF_SUCCESS    : Rx invert state retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: inv == NULL
 *
 */
bf_status_t bf_serdes_get_rx_invert(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool *inv) {
  port_mgr_serdes_t *serdes_p;

  if (inv == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *inv =
        port_mgr_av_sd_rx_invert_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Rx polarity on a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param inv   : 1=invert, 0=dont invert
 *
 * \return: BF_SUCCESS i  : Rx invert set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_set_rx_invert(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool inv) {
  port_mgr_serdes_t *serdes_p;
  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) return BF_INVALID_ARG;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (!serdes_p) return BF_INVALID_ARG;
  // save for FSM
  serdes_p->rx_inv = inv ? true : false;

  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    /* Don't touch the harware during cfg replay */
    return BF_SUCCESS;
  }
  port_mgr_av_sd_rx_invert_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_inv ? 1 : 0);
  return BF_SUCCESS;
}

/** \brief Force inject Tx bit errors on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param num_bits: number of errorer bits to inject (0-65535)
 *
 * \return: BF_SUCCESS    : Error bits injected successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: num_bits > 65535 or negative
 *
 */
bf_status_t bf_serdes_set_tx_inject_error(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int lane,
                                          int num_bits) {
  port_mgr_serdes_t *serdes_p;

  if ((num_bits < 0) || (num_bits > 65535)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_tx_error_inject_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, num_bits);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Force inject Rx bit errors on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param num_bits: number of errorer bits to inject (0-65535)
 *
 * \return: BF_SUCCESS    : Error bits injected successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: num_bits > 65535 or negative
 *
 */
bf_status_t bf_serdes_set_rx_inject_error(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int lane,
                                          int num_bits) {
  port_mgr_serdes_t *serdes_p;

  if ((num_bits < 0) || (num_bits > 65535)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_rx_error_inject_set(
        dev_id, serdes_p->ring, serdes_p->rx_sd, num_bits);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get Tx data_sel mode from a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param data_sel: returned,
 *                : AVAGO_SERDES_TX_DATA_SEL_CORE
 *                : AVAGO_SERDES_TX_DATA_SEL_PRBS7
 *                : AVAGO_SERDES_TX_DATA_SEL_PRBS9
 *                : AVAGO_SERDES_TX_DATA_SEL_PRBS11
 *                : AVAGO_SERDES_TX_DATA_SEL_PRBS15
 *                : AVAGO_SERDES_TX_DATA_SEL_PRBS23
 *                : AVAGO_SERDES_TX_DATA_SEL_PRBS31
 *                : AVAGO_SERDES_TX_DATA_SEL_USER
 *                : AVAGO_SERDES_TX_DATA_SEL_LOOPBACK
 *
 * \return: BF_SUCCESS    : Tx data_sel retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: data_sel == NULL
 *
 */
bf_status_t bf_serdes_get_tx_data_sel(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int *data_sel) {
  port_mgr_serdes_t *serdes_p;

  if (data_sel == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *data_sel =
        port_mgr_av_sd_tx_data_sel_get(dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Tx data_sel mode on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param data_sel:
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS7    = 0, < PRBS7 (x^7+x^6+1) generator
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS9    = 1, < PRBS9 (x^9+x^5+1)
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS11   = 2, < PRBS11 (x^11+x^9+1)
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS15   = 3, < PRBS15 (x^15+x^14+1)
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS23   = 4, < PRBS23 (x^23+x^18+1)
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS31   = 5, < PRBS31 (x^31+x^28+1)
 *     AVAGO_SERDES_TX_DATA_SEL_PRBS13   = 6, < PRBS13 (x^13+x^12+x^2+x^1+1)
 *     AVAGO_SERDES_TX_DATA_SEL_USER     = 7, < User pattern generator
 *     AVAGO_SERDES_TX_DATA_SEL_CORE     = 8, < External data
 *     AVAGO_SERDES_TX_DATA_SEL_LOOPBACK = 9, < Parallel loopback from receiver
 *     AVAGO_SERDES_TX_DATA_SEL_PMD      = 10,< PMD training data
 *     AVAGO_SERDES_TX_DATA_SEL_AN       = 11 < Auto-negotiation data
 *
 * \return: BF_SUCCESS    : serdes register read successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid data_sel value
 *
 */
bf_status_t bf_serdes_set_tx_data_sel(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int data_sel) {
  port_mgr_serdes_t *serdes_p;

  if ((data_sel > AVAGO_SERDES_TX_DATA_SEL_AN) || (data_sel < 0))
    return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_tx_data_sel_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, data_sel);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get Rx cmp_sel mode from a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param cmp_sel : returned,
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS7
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS9
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS11
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS15
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS23
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS31
 *                : AVAGO_SERDES_RX_CMP_DATA_SELF_SEED
 *                : AVAGO_SERDES_RX_CMP_DATA_OFF
 *
 * \return: BF_SUCCESS    : Rx cmp_sel retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: cmp_sel == NULL
 *
 */
bf_status_t bf_serdes_get_rx_cmp_sel(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int *cmp_sel) {
  port_mgr_serdes_t *serdes_p;

  if (cmp_sel == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *cmp_sel =
        port_mgr_av_sd_get_rx_cmp_sel(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief bf_port_rx_cmp_sel_set
 *         Set Rx cmp_sel mode on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param cmp_sel : AVAGO_SERDES_RX_CMP_DATA_PRBS7
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS9
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS11
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS15
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS23
 *                : AVAGO_SERDES_RX_CMP_DATA_PRBS31
 *                : AVAGO_SERDES_RX_CMP_DATA_SELF_SEED
 *                : AVAGO_SERDES_RX_CMP_DATA_OFF
 *
 * \return: BF_SUCCESS    : Rx cmp_sel set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid cmp_sel value
 *
 */
bf_status_t bf_serdes_set_rx_cmp_sel(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int cmp_sel) {
  port_mgr_serdes_t *serdes_p;

  if ((cmp_sel > AVAGO_SERDES_RX_CMP_DATA_OFF) || (cmp_sel < 0))
    return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_set_rx_cmp_sel(
        dev_id, serdes_p->ring, serdes_p->rx_sd, cmp_sel);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get Rx cmp_mode mode from a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param cmp_mode: returned,
 *                : AVAGO_SERDES_RX_CMP_MODE_OFF
 *                : AVAGO_SERDES_RX_CMP_MODE_XOR
 *                : AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN
 *                : AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN
 *
 * \return: BF_SUCCESS    : Rx cmp_mode retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: cmp_mode == NULL
 *
 */
bf_status_t bf_serdes_get_rx_cmp_mode(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int *cmp_mode) {
  port_mgr_serdes_t *serdes_p;

  if (cmp_mode == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *cmp_mode =
        port_mgr_av_sd_get_rx_cmp_mode(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Rx cmp_mode mode on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param cmp_mode: AVAGO_SERDES_RX_CMP_MODE_OFF
 *                : AVAGO_SERDES_RX_CMP_MODE_XOR
 *                : AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN
 *                : AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN
 *
 * \return: BF_SUCCESS    : Rx cmp_mode set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid cmp_mode
 *
 */
bf_status_t bf_serdes_set_rx_cmp_mode(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int cmp_mode) {
  port_mgr_serdes_t *serdes_p;

  if ((cmp_mode != AVAGO_SERDES_RX_CMP_MODE_OFF) &&
      (cmp_mode != AVAGO_SERDES_RX_CMP_MODE_XOR) &&
      (cmp_mode != AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN) &&
      (cmp_mode != AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN)) {
    return BF_INVALID_ARG;
  }

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_set_rx_cmp_mode(
        dev_id, serdes_p->ring, serdes_p->rx_sd, cmp_mode);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get Rx term mode from a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param term  : returned,
 *              : AVAGO_SERDES_RX_TERM_AGND
 *              : AVAGO_SERDES_RX_TERM_AVDD
 *              : AVAGO_SERDES_RX_TERM_FLOAT
 *
 * \return: BF_SUCCESS    : Rx termination mode retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: term == NULL
 *
 */
bf_status_t bf_serdes_get_rx_term(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  int lane,
                                  int *term) {
  port_mgr_serdes_t *serdes_p;

  if (term == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *term = port_mgr_av_sd_rx_term_get(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Rx term mode on a serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param term  : AVAGO_SERDES_RX_TERM_AGND
 *              : AVAGO_SERDES_RX_TERM_AVDD
 *              : AVAGO_SERDES_RX_TERM_FLOAT
 *
 * \return: BF_SUCCESS    : Rx termination set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid term
 *
 */
bf_status_t bf_serdes_set_rx_term(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  int lane,
                                  int term) {
  port_mgr_serdes_t *serdes_p;

  if ((term != AVAGO_SERDES_RX_TERM_AGND) &&
      (term != AVAGO_SERDES_RX_TERM_AVDD) &&
      (term != AVAGO_SERDES_RX_TERM_FLOAT)) {
    return BF_INVALID_ARG;
  }

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (!serdes_p) return BF_INVALID_ARG;

  // save for FSM
  serdes_p->rx_term = term;
  port_mgr_av_sd_rx_term_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_term);
  return BF_SUCCESS;
}

/** \brief Get Tx PLL clk_src mode from a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param clk_src : returned,
 *                : AVAGO_SERDES_TX_PLL_REFCLK
 *                : AVAGO_SERDES_TX_PLL_RX_DIVX
 *                : AVAGO_SERDES_TX_PLL_OFF
 *                : AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK
 *                : AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK_DIV2
 *
 * \return: BF_SUCCESS    : Tx PLL clock source retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: clk_src == NULL
 *
 */
bf_status_t bf_serdes_get_tx_pll_clk_source(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            int lane,
                                            int *clk_src) {
  port_mgr_serdes_t *serdes_p;

  if (clk_src == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *clk_src = port_mgr_av_sd_tx_pll_clk_source_get(
        dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Tx PLL clk_src mode on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param clk_src : AVAGO_SERDES_TX_PLL_REFCLK
 *                : AVAGO_SERDES_TX_PLL_RX_DIVX
 *                : AVAGO_SERDES_TX_PLL_OFF
 *                : AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK
 *                : AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK_DIV2
 *
 * \return: BF_SUCCESS    : Tx PLL clock source set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid clk_src
 *
 */
bf_status_t bf_serdes_set_tx_pll_clk_source(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            int lane,
                                            int clk_src) {
  port_mgr_serdes_t *serdes_p;

  if ((clk_src != AVAGO_SERDES_TX_PLL_REFCLK) &&
      (clk_src != AVAGO_SERDES_TX_PLL_RX_DIVX) &&
      (clk_src != AVAGO_SERDES_TX_PLL_OFF) &&
      (clk_src != AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK) &&
      (clk_src != AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK_DIV2)) {
    return BF_INVALID_ARG;
  }

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_tx_pll_clk_source_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, clk_src);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Get Spico clk_src mode from a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param clk_src : returned,
 *                  AVAGO_SERDES_SPICO_REFCLK
 *                  AVAGO_SERDES_SPICO_PCIE_CORE_CLK
 *                  AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED
 *                  AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED
 *                  AVAGO_SERDES_SPICO_REFCLK_DIV2
 *                  AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2
 *                  AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED_DIV2
 *                  AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED_DIV2
 *
 * \return: BF_SUCCESS    : Spico clock source retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid  bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: clk_src == NULL
 *
 */
bf_status_t bf_serdes_get_spico_clk_source(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           int *clk_src) {
  port_mgr_serdes_t *serdes_p;

  if (clk_src == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    *clk_src = port_mgr_av_sd_spico_clk_source_get(
        dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set Spico clk_src mode on a serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param clk_src : AVAGO_SERDES_SPICO_REFCLK
 *                : AVAGO_SERDES_SPICO_PCIE_CORE_CLK
 *                : AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED
 *                : AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED
 *                : AVAGO_SERDES_SPICO_REFCLK_DIV2
 *                : AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2
 *                : AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED_DIV2
 *                : AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED_DIV2
 *
 * \return: BF_SUCCESS    : Spico clock source set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid  bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: invalid clk_src
 *
 */
bf_status_t bf_serdes_set_spico_clk_source(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int lane,
                                           int clk_src) {
  port_mgr_serdes_t *serdes_p;

  if ((clk_src != AVAGO_SERDES_SPICO_REFCLK) &&
      (clk_src != AVAGO_SERDES_SPICO_PCIE_CORE_CLK) &&
      (clk_src != AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED) &&
      (clk_src != AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED) &&
      (clk_src != AVAGO_SERDES_SPICO_REFCLK_DIV2) &&
      (clk_src != AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2) &&
      (clk_src != AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED_DIV2) &&
      (clk_src != AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED_DIV2)) {
    return BF_INVALID_ARG;
  }

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_spico_clk_source_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, clk_src);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set any one of several DFE parameters on a Tofino serdes slice
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param row     : row from below table
 * \param col     : col from below table
 * \param value   : value to set parameter to
 * <pre>
 *                    0x0       0x01  0x2   0x3     0x4
 *            0x0 - thresh_HF    d0e   HF  GAIN  dvos_d0e_lo*
 *            0x1 - thresh_LF    d0o   LF    2*  dvos_d0e_hi*
 *            0x2 - thresh_AGC   d1e   DC    3*  dvos_d0o_lo*
 *            0x3 - err_cnt_lo   d1o   BW    4*  dvos_d0o_hi*
 *            0x4 - err_cnt_hi   t1e   LB    5*  dvos_d1e_lo*
 *            0x5 - gainDFE_lo   t1o         6*  dvos_d1e_hi*
 *            0x6 - gainDFE_hi   t0e         7*  dvos_d1o_lo*
 *            0x7 - agc_gain_bnd t0o         8*  dvos_d1o_hi*
 *            0x8 - agc_eq_bnd   alpha       9*  tvos_d0e_lo*
 *            0x9 - thresh_lev               A** tvos_d0e_hi*
 *            0xA - dfe_state                B** tvos_d0o_lo*
 *            0xB - dfe_status               C** tvos_d0o_hi*
 *            0xC - d6_vos_only              D** tvos_d1e_lo*
 *            0xD - ctle_only                    tvos_d1e_hi*
 *            0xE - enable_dlev                  tvos_d1o_lo*
 *            0xF - run_coarse                   tvos_d1o_hi*
 * </pre>
 * \return: BF_SUCCESS    : DFE parameter set successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: row > 15 (or negative)
 * \return: BF_INVALID_ARG: col >  4 (or negative)
 *
 */
bf_status_t bf_serdes_set_dfe_param(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    int row,
                                    int col,
                                    int value) {
  port_mgr_serdes_t *serdes_p;

  if ((row < 0) || (row > 15)) return BF_INVALID_ARG;
  if ((col < 0) || (col > 4)) return BF_INVALID_ARG;
  if ((value < -255) || (value > 255)) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    port_mgr_av_sd_dfe_param_set(
        dev_id, serdes_p->ring, serdes_p->rx_sd, row, col, value);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Initialize a serdes slice. Configures and calibrates a single
 *         serdes slice with most basic parameters.
 *
 * Note: This API can ONLY be used if tx and rx serdes lanes are mapped
 *identically!
 *
 * \param dev_id    : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane      : logical lane (within port) 0-3, depending upon mode
 * \param reset     : 1=reset serdes first, 0=no reset
 * \param init_mode :
 * \param divider   : Tx/Rx divider (assumed to be the same)
 * \param data_width: 10/20/40
 * \param phase_cal : 1=perform phase calibration, 0=no phase calibration
 * \param output_en : 1=set tx_output_en, allowing signal to be seen externally
 *
 * \return: BF_SUCCESS    : serdes initialized successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: data_width not one of 10/20/40
 *
 */
bf_status_t bf_serdes_init(bf_dev_id_t dev_id,
                           bf_dev_port_t dev_port,
                           int lane,
                           int reset,
                           int init_mode,
                           int divider,
                           int data_width,
                           int phase_cal,
                           int output_en) {
  port_mgr_serdes_t *serdes_p;
  int speed = 0;

  if ((data_width != 10) && (data_width != 20) && (data_width != 40))
    return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  // HACK
  switch (divider) {
    case 8:
      if (data_width == 10) {  // 1G CPU port (data width=10b)
        speed = 1;
      } else {  // 1.25Ghz AN speed (data width=20b)
        speed = 125;
      }
      break;
    case 20:
      speed = 3125;
      break;
    case 66:
      speed = 10;
      break;
    case 165:
      speed = 25;
      break;
    default:
      bf_sys_assert(0);
  }

  if (serdes_p->tx_sd == serdes_p->rx_sd) {
    // reset only allowed for symmetric mode
    if (reset) {
      port_mgr_av_sd_reset(dev_id, serdes_p->ring, serdes_p->tx_sd, true, true);
    }
    // Program serdes in symmetric mode
    port_mgr_av_sd_pgm_symmetric(dev_id,
                                 serdes_p->ring,
                                 serdes_p->tx_sd,
                                 speed,
                                 output_en ? true : false,
                                 serdes_p->pll_ovrclk);
  } else {
    port_mgr_log(
        "SDS :%d:%3x:%d: Pgm ASym: spd=%s : div=%d : width=%d : rst=%d : "
        "drv=%d",
        dev_id,
        dev_port,
        lane,
        (speed == 125)
            ? "1.25Ghz"
            : (speed == 3125)
                  ? "3.125Ghz"
                  : (speed == 10) ? "10Ghz" : (speed == 25) ? "25Ghz" : "?",
        divider,
        data_width,
        reset,
        output_en);
    // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
    port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

    port_mgr_serdes_slice_t *rx_slice_p, *tx_slice_p;

    if (dev_p == NULL) return BF_INVALID_ARG;

    tx_slice_p = &dev_p->slice[serdes_p->ring][serdes_p->tx_sd];
    rx_slice_p = &dev_p->slice[serdes_p->ring][serdes_p->rx_sd];

    port_mgr_log(
        "SDS :%d:%3x:%d:      Tx : ring=%d : tx_sd=%d : rx_spd=%s : rx_en=%d",
        dev_id,
        dev_port,
        lane,
        serdes_p->ring,
        serdes_p->tx_sd,
        (tx_slice_p->rx_speed == 125)
            ? "1.25Ghz"
            : (tx_slice_p->rx_speed == 3125)
                  ? "3.125Ghz"
                  : (tx_slice_p->rx_speed == 10)
                        ? "10Ghz"
                        : (tx_slice_p->rx_speed == 25) ? "25Ghz" : "?",
        tx_slice_p->rx_en);

    // Program serdes in symmetric mode
    port_mgr_av_sd_pgm_asymmetric_tx(dev_id,
                                     serdes_p->ring,
                                     serdes_p->tx_sd,
                                     speed,
                                     tx_slice_p->rx_speed,
                                     tx_slice_p->rx_en,
                                     output_en ? true : false,
                                     serdes_p->pll_ovrclk);
    tx_slice_p->tx_speed = speed;
    tx_slice_p->tx_en = 1;

    port_mgr_log(
        "SDS :%d:%3x:%d:      Rx : ring=%d : rx_sd=%d : tx_spd=%s : tx_en=%d",
        dev_id,
        dev_port,
        lane,
        serdes_p->ring,
        serdes_p->rx_sd,
        (rx_slice_p->tx_speed == 125)
            ? "1.25Ghz"
            : (rx_slice_p->tx_speed == 3125)
                  ? "3.125Ghz"
                  : (rx_slice_p->tx_speed == 10)
                        ? "10Ghz"
                        : (rx_slice_p->tx_speed == 25) ? "25Ghz" : "?",
        rx_slice_p->tx_en);

    port_mgr_av_sd_pgm_asymmetric_rx(dev_id,
                                     serdes_p->ring,
                                     serdes_p->rx_sd,
                                     speed,
                                     rx_slice_p->tx_speed,
                                     rx_slice_p->tx_en,
                                     serdes_p->pll_ovrclk);
    rx_slice_p->rx_speed = speed;
    rx_slice_p->rx_en = 1;
  }

  (void)init_mode;
  (void)phase_cal;
  return BF_SUCCESS;
}

/** \brief Set some basic Tx/Rx serdes parameters.
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane:   : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS       : Parameters set successfully
 * \return: BF_INVALID_ARG   : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG   : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG   : lane > # of serdes lanes on this port (in this
 *mode)
 *
 */
bf_status_t bf_serdes_params_set_cmn(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bool do_tx_eq,
                                     bool do_thres) {
  port_mgr_serdes_t *serdes_p;
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);
  if (!dev_p) return BF_INVALID_ARG;
  int rc;

  if (!bf_ha_stage_is_valid(dev_id)) return BF_INVALID_ARG;

  if (port_mgr_dev_ha_stage_get(dev_id) == PORT_MGR_HA_CFG_REPLAY) {
    /* Don't touch the hardware  during cfg replay */
    return BF_SUCCESS;
  }

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  if (do_tx_eq) {
    // Configure Tx parameters
    rc = port_mgr_av_sd_set_tx_eq(dev_id,
                                  serdes_p->ring,
                                  serdes_p->tx_sd,
                                  serdes_p->tx_eq_pre,
                                  serdes_p->tx_eq_atten,
                                  serdes_p->tx_eq_post);
  } else {
    // reset tx-eq parms (for AN)
    rc = port_mgr_av_sd_set_tx_eq(
        dev_id, serdes_p->ring, serdes_p->tx_sd, 0, 0, 0);
  }
  if (rc) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_invert_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, serdes_p->tx_inv);
  if (rc) return BF_INVALID_ARG;

  // Configure Rx parameters
  rc = port_mgr_av_sd_rx_invert_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, serdes_p->rx_inv);
  if (rc) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_term_set(
      dev_id, serdes_p->ring, serdes_p->rx_sd, BF_SDS_RX_TERM_AVDD);
  if (rc) return BF_INVALID_ARG;

  // Cache the value so that the hw and the sw state are consistent
  serdes_p->rx_term = BF_SDS_RX_TERM_AVDD;

  if (do_thres) {
    lld_err_t err;
    bf_sku_chip_part_rev_t rev_no;

    /* A0 (rev_no==0) use sig_ok thresh=3
     *  B0 (rev_no==1) use calibrated threshold if not disabled
     */
    err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
    if ((err == LLD_OK) && (rev_no != 0) &&
        (dev_p->disable_signal_ok_threshold_calibration == false)) {
      port_mgr_av_sd_spico_int(
          dev_id, serdes_p->ring, serdes_p->rx_sd, 0x20, 0x20);
    } else {
      // enable sig_ok detection to wait for link-partner before running DFE
      // sig_ok_thresh should be set externally. hard code for now
      rc =
          port_mgr_av_sd_signal_ok_thresh_set(dev_id,
                                              serdes_p->ring,
                                              serdes_p->rx_sd,
                                              3 /*serdes_p->rx_sig_ok_thresh*/);
      if (rc) return BF_INVALID_ARG;
      // Cache the value so that the hw and the sw state are consistent
      serdes_p->rx_sig_ok_thresh = 3;
    }
    port_mgr_log("FSM :%d:%d:%3d: Tx loop bandwidth setting: %xh <%d>",
                 dev_id,
                 serdes_p->ring,
                 serdes_p->rx_sd,
                 serdes_p->tx_loop_bandwidth,
                 serdes_p->tx_loop_bandwidth);
    bf_serdes_tx_loop_bandwidth_set(
        dev_id, dev_port, lane, serdes_p->tx_loop_bandwidth);
  } else {
    port_mgr_av_sd_spico_int(dev_id, serdes_p->ring, serdes_p->rx_sd, 0x20, 0);
  }
  return BF_SUCCESS;
}

/** \brief Set basic Tx/Rx serdes parameters for Autonegotiation
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane:   : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS       : Parameters set successfully
 * \return: BF_INVALID_ARG   : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG   : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG   : lane > # of serdes lanes on this port (in this
 *mode)
 *
 */
bf_status_t bf_serdes_params_an_set(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    bool do_tx_eq) {
  bf_status_t bf_status;

  bf_status = bf_serdes_params_set_cmn(dev_id, dev_port, lane, do_tx_eq, false);
  return bf_status;
}

/** \brief Set basic Tx/Rx serdes parameters for non-AN ports
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane:   : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS       : Parameters set successfully
 * \return: BF_INVALID_ARG   : dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG   : invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG   : lane > # of serdes lanes on this port (in this
 *mode)
 *
 */
bf_status_t bf_serdes_params_set(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 int lane) {
  bf_status_t bf_status;

  bf_status = bf_serdes_params_set_cmn(dev_id, dev_port, lane, true, true);
  return bf_status;
}

/** \brief Initiate a temperature reading on the PMRO node
 *         A sensor reading can take several milliseconds to complete
 *         so the interface is broken into "start" adn "get" APIs.
 *         Call "start" to initiate a sensor measurement, then poll
 *         the "get" API until it returns something other than
 *         BF_NOT_READY.
 *
 * \param dev_id : system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param channel: Which sensor to read
 *                 0 : BF_SDS_MAIN_TEMP_SENSOR_CH
 *                 1 : BF_SDS_REMOTE_TEMP_SENSOR_0_CH
 *
 * \return: BF_SUCCESS    : Reading initiated
 * \return: BF_INVALID_ARG: invalid sensor
 * \return: BF_INVALID_ARG: invalid channel
 *
 */
bf_status_t bf_serdes_temperature_read_start(
    bf_dev_id_t dev_id, int sensor, bf_sds_temp_sensor_channel_t channel) {
  int rc, ring, sd;

  // only 1 sensor on tofino
  if (sensor != 0) return BF_INVALID_ARG;
  // each sensor has 2 "channels", main and remote
  if (channel > BF_SDS_REMOTE_TEMP_SENSOR_0_CH) return BF_INVALID_ARG;

  rc = port_mgr_find_addr_for(
      dev_id, IP_TYPE_TEMP_SENSOR, sensor, 0, &ring, &sd);
  if (rc != 0) return BF_INVALID_ARG;

  // initiate a temperature reading
  port_mgr_av_sd_temp_read_start(dev_id, ring, sd, channel);

  return BF_SUCCESS;
}

/** \brief Read a temperature sensor value from the PMRO node
 *
 * \param dev_id : system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param channel: Which sensor to read
 *                 0 : BF_SDS_MAIN_TEMP_SENSOR_CH
 *                 1 : BF_SDS_REMOTE_TEMP_SENSOR_0_CH
 * \param temp_mC: sensor value, in degrees mC
 *
 * \return: BF_SUCCESS    : Sensor value
 * \return: BF_NOT_READY  : Sensor read not complete yet
 * \return: BF_INVALID_ARG: invalid sensor
 * \return: BF_INVALID_ARG: invalid channel
 *
 */
bf_status_t bf_serdes_temperature_read_get(bf_dev_id_t dev_id,
                                           int sensor,
                                           bf_sds_temp_sensor_channel_t channel,
                                           uint32_t *temp_mC) {
  int rc, ring, sd;
  int temp;

  // only 1 sensor on tofino
  if (sensor != 0) return BF_INVALID_ARG;
  // each sensor has 2 "channels", main and remote
  if (channel > BF_SDS_REMOTE_TEMP_SENSOR_0_CH) return BF_INVALID_ARG;
  if (temp_mC == NULL) return BF_INVALID_ARG;

  rc = port_mgr_find_addr_for(
      dev_id, IP_TYPE_TEMP_SENSOR, sensor, 0, &ring, &sd);
  if (rc != 0) return BF_INVALID_ARG;

  // read the sensor raw value
  // bit[15] is the valid indication
  // if not valid, return BF_NOT_READY
  temp = port_mgr_av_sd_temp_read_get(dev_id, ring, sd, channel);

  // return temperature in mC or BF_NOT_READY
  *temp_mC = (uint32_t)temp;

  if (temp == -1) return BF_NOT_READY;

  return BF_SUCCESS;
}

/** \brief Initiate a voltage reading on the PMRO node
 *         A sensor reading can take several milliseconds to complete
 *         so the interface is broken into "start" adn "get" APIs.
 *         Call "start" to initiate a sensor measurement, then poll
 *         the "get" API until it returns something other than
 *         BF_NOT_READY.
 *
 * \param dev_id : system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param channel: Which sensor to read
 *                 0 : BF_SDS_MAIN_VOLT_SENSOR_CH
 *                 1 : BF_SDS_REMOTE_VOLT_SENSOR_0_CH
 *
 * \return: BF_SUCCESS    : Reading initiated
 * \return: BF_INVALID_ARG: invalid sensor
 * \return: BF_INVALID_ARG: invalid channel
 *
 */
bf_status_t bf_serdes_voltage_read_start(
    bf_dev_id_t dev_id, int sensor, bf_sds_voltage_sensor_channel_t channel) {
  int rc, ring, sd;

  // only 1 sensor on tofino
  if (sensor != 0) return BF_INVALID_ARG;
  // each sensor has 2 "channels", main and remote
  if (channel > BF_SDS_REMOTE_VOLT_SENSOR_0_CH) return BF_INVALID_ARG;

  rc = port_mgr_find_addr_for(
      dev_id, IP_TYPE_TEMP_SENSOR, sensor, 0, &ring, &sd);
  if (rc != 0) return BF_INVALID_ARG;

  // initiate a voltage reading
  port_mgr_av_sd_voltage_read_start(dev_id, ring, sd, channel);

  return BF_SUCCESS;
}

/** \brief Read a voltage sensor value from the PMRO node
 *
 * \param dev_id : system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param channel: Which sensor to read
 *                 0 : BF_SDS_MAIN_VOLTAGE_SENSOR_CH
 *                 1 : BF_SDS_REMOTE_VOLTAGE_SENSOR_0_CH
 * \param voltage_mV : sensor value, in mV
 *
 * \return: BF_SUCCESS    : Sensor value
 * \return: BF_NOT_READY  : Sensor read not complete yet
 * \return: BF_INVALID_ARG: invalid sensor
 * \return: BF_INVALID_ARG: invalid channel
 *
 */
bf_status_t bf_serdes_voltage_read_get(bf_dev_id_t dev_id,
                                       int sensor,
                                       bf_sds_voltage_sensor_channel_t channel,
                                       uint32_t *voltage_mV) {
  int rc, ring, sd;
  int voltage;

  // only 1 sensor on tofino
  if (sensor != 0) return BF_INVALID_ARG;
  // each sensor has 2 "channels", main and remote
  if (channel > BF_SDS_REMOTE_VOLT_SENSOR_0_CH) return BF_INVALID_ARG;

  rc = port_mgr_find_addr_for(
      dev_id, IP_TYPE_TEMP_SENSOR, sensor, 0, &ring, &sd);
  if (rc != 0) return BF_INVALID_ARG;

  voltage = port_mgr_av_sd_voltage_read_get(dev_id, ring, sd, channel);

  // return voltage in mV or BF_NOT_READY
  *voltage_mV = (uint32_t)voltage;  // in mV

  if (voltage == -1) return BF_NOT_READY;

  return BF_SUCCESS;
}

/** \brief Reset a serdes slice
 *
 * Caution: If lane uses asymmetric serdes (tx != rx) this
 *          API resets BOTH involved slices.
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param node_reset: perform a soft reset of the slice
 * \param microp_reset: perform a reset of the slices microprocessor
 *
 * \return: BF_SUCCESS    : reset(s) complete
 * \return: BF_INVALID_ARG: invalid port or lane
 *
 */
bf_status_t bf_serdes_reset(bf_dev_id_t dev_id,
                            bf_dev_port_t dev_port,
                            int lane,
                            bf_sds_reset_type_t reset_type) {
  port_mgr_serdes_t *serdes_p;
  bool node_reset = false;
  bool microp_reset = false;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  if (reset_type == BF_SDS_RESET_NODE) {
    node_reset = true;
  } else if (reset_type == BF_SDS_RESET_MICROPROCESSOR) {
    microp_reset = true;
  } else if (reset_type == BF_SDS_RESET_NODE_AND_MICROPROCESSOR) {
    node_reset = true;
    microp_reset = true;
  }

  if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_NONE) {
    /* Don't touch the harware during cfg replay */
    return BF_SUCCESS;
  }

  port_mgr_av_sd_reset(
      dev_id, serdes_p->ring, serdes_p->rx_sd, node_reset, microp_reset);

  // handle asymmetric case
  if (serdes_p->rx_sd != serdes_p->tx_sd) {
    port_mgr_av_sd_reset(
        dev_id, serdes_p->ring, serdes_p->tx_sd, node_reset, microp_reset);
  }
  return BF_SUCCESS;
}

/** \brief load the firmware version to a given serdes slice
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param fw_ver: version associated with firmware, for verification
 * \param fw_path: path to file containing firmware (if NULL use default)
 *
 * \return: BF_SUCCESS    : firmware loadad
 * \return: BF_INVALID_ARG: invalid dev_id
 *
 */
bf_status_t bf_serdes_ucast_firmware_load(
    bf_dev_id_t dev_id, int ring, int sd, uint32_t fw_ver, char *fw_path) {
  char *this_fw_path = fw_path;
  uint32_t this_fw_ver = fw_ver;

  if (fw_path == NULL) {
    // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
    port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

    if (dev_p == NULL) return BF_INVALID_ARG;

    /* use default firmware specified in profile at device_add */
    if (sd == sbm) {
      this_fw_path = dev_p->sbus_master_fw;
      this_fw_ver = dev_p->sbus_master_fw_ver;
    } else {
      this_fw_path = dev_p->serdes_fw;
      this_fw_ver = dev_p->serdes_fw_ver;
    }
  }
  port_mgr_av_sd_load_firmware(
      dev_id, ring, AVAGO_BROADCAST, this_fw_ver, this_fw_path);
  return BF_SUCCESS;
}

/** \brief load the firmware version to all serdes slices.
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param fw_ver: version associated with firmware, for verification
 * \param fw_path: path to file containing firmware (if NULL use default)
 *
 * \return: BF_SUCCESS    : firmware loadad
 * \return: BF_INVALID_ARG: invalid dev_id
 *
 */
bf_status_t bf_serdes_bcast_firmware_load(bf_dev_id_t dev_id,
                                          uint32_t fw_ver,
                                          char *fw_path) {
  int ring;
  char *this_fw_path = fw_path;
  uint32_t this_fw_ver = fw_ver;

  if (fw_path == NULL) {
    // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
    port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

    if (dev_p == NULL) return BF_INVALID_ARG;

    /* use default firmware specified in profile at device_add */
    this_fw_path = dev_p->serdes_fw;
    this_fw_ver = dev_p->serdes_fw_ver;
  }

  for (ring = 0; ring < port_mgr_num_sbus_rings_get(dev_id); ring++) {
    port_mgr_av_sd_load_firmware(
        dev_id, ring, AVAGO_BROADCAST, this_fw_ver, this_fw_path);
  }
  return BF_SUCCESS;
}

/** \brief load the firmware version to the sbus master
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param fw_ver: version associated with firmware, for verification
 * \param fw_path: path to file containing firmware (if NULL use default)
 *
 * \return: BF_SUCCESS    : firmware path set
 * \return: BF_INVALID_ARG: invalid dev_id
 *
 */
bf_status_t bf_serdes_sbm_firmware_load(bf_dev_id_t dev_id,
                                        uint32_t fw_ver,
                                        char *fw_path) {
  int ring;
  char *this_fw_path = fw_path;
  uint32_t this_fw_ver = fw_ver;

  if (fw_path == NULL) {
    // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
    port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

    if (dev_p == NULL) return BF_INVALID_ARG;

    /* use default firmware specified in profile at device_add */
    this_fw_path = dev_p->sbus_master_fw;
    this_fw_ver = dev_p->sbus_master_fw_ver;
  }

  for (ring = 0; ring < port_mgr_num_sbus_rings_get(dev_id); ring++) {
    port_mgr_av_sd_load_firmware(dev_id, ring, sbm, this_fw_ver, this_fw_path);
  }
  return BF_SUCCESS;
}

/** \brief Start/Stop transmission/reception of link-training frames
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param en    : false=disable LT, true=enable LT
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_link_training_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane,
                                        bool en) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  if (en) {
    bool hi_spd = false;
    port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
    if (port_p == NULL) return BF_INVALID_ARG;

    if ((port_p->sw.speed == BF_SPEED_10G) ||
        (port_p->sw.speed == BF_SPEED_40G) ||
        (port_p->sw.speed == BF_SPEED_1G)) {
      hi_spd = false;
    } else {
      hi_spd = true;
    }
    rc = port_mgr_av_sd_pmd_training_start(
        dev_id, serdes_p->ring, serdes_p->tx_sd, hi_spd);
  } else {
    rc = port_mgr_av_sd_pmd_training_stop(
        dev_id, serdes_p->ring, serdes_p->tx_sd);
  }
  if (rc != 0) return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/** \brief Set up for either clause 92 or clause 72 link-training
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param mode  : BF_CLAUSE_72_LINK_TRAINING
 *                BF_CLAUSE_92_LINK_TRAINING
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_link_training_mode_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int lane,
                                             bf_link_training_mode_t mode) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  if (mode == BF_CLAUSE_92_LINK_TRAINING) {
    port_mgr_av_sd_clause_92_training_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, lane);
  } else if (mode == BF_CLAUSE_72_LINK_TRAINING) {
    port_mgr_av_sd_clause_72_training_set(
        dev_id, serdes_p->ring, serdes_p->tx_sd, lane);
  } else {
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/** \brief Assert HCD link-status
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param hcd   : Negotiated (avago) hcd value
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_assert_hcd_link_status(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int lane,
                                             uint32_t hcd) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_assert_hcd_link_status(
      dev_id, serdes_p->ring, serdes_p->tx_sd, serdes_p->rx_sd, hcd);
  if (rc != 0) return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/** \brief Get rough estimate of eye quality
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param metric: rough metric of eye quality
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_eye_metric_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     uint32_t *metric) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_eye_metric_get(
      dev_id, serdes_p->ring, serdes_p->rx_sd, metric);
  if (rc != 0) return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/** \brief Set "delay cal"
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_delay_cal_set(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_delay_cal(dev_id, serdes_p->ring, serdes_p->rx_sd);
  if (rc != 0) return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/** \brief Set lane map and polarity (based on board configuration)
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param mac_block: Logical MAC block index (quad)
 * \param tx_chnl  : Tx MAC channel, 0-3
 * \param rx_chnl  : Rx MAC channel, 0-3
 * \param lane_info: ptr to board config for this quad/chnl
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_lane_info_set(bf_dev_id_t dev_id,
                                    bf_mac_block_id_t mac_block,
                                    uint32_t tx_chnl,
                                    uint32_t rx_chnl,
                                    bf_sds_lane_info_t *lane_info) {
  int rc, ring, tx_sd, rx_sd;
  bf_mac_block_id_t phy_mac_block;
  // convert logical mac-blk to physical
  lld_sku_map_log2phy_mac_block(dev_id, mac_block, &phy_mac_block);

  rc = port_mgr_find_addr_for(
      dev_id, IP_TYPE_ETH_PMA, phy_mac_block, tx_chnl, &ring, &tx_sd);
  if (rc != 0) return BF_INVALID_ARG;

  rc = port_mgr_find_addr_for(
      dev_id, IP_TYPE_ETH_PMA, phy_mac_block, rx_chnl, &ring, &rx_sd);
  if (rc != 0) return BF_INVALID_ARG;

  port_mgr_log(
      "SDS : %d:---:-: phy MAC=%d : Tx Ch=%d : Rx Ch=%d : Tx Sd=%d : Rx Sd=%d "
      ": "
      "tx=%d : rx=%d : tx_inv=%d : rx_inv=%d : "
      "pre=%d : atn=%d "
      ": post=%d",
      dev_id,
      phy_mac_block,
      tx_chnl,
      rx_chnl,
      tx_sd,
      rx_sd,
      lane_info->tx_phy_lane_id,
      lane_info->rx_phy_lane_id,
      lane_info->tx_phy_pn_swap ? 1 : 0,
      lane_info->rx_phy_pn_swap ? 1 : 0,
      lane_info->tx_pre,
      lane_info->tx_attn,
      lane_info->tx_post);

  // Configure Tx parameters
  rc = port_mgr_av_sd_set_tx_eq(dev_id,
                                ring,
                                tx_sd,
                                lane_info->tx_pre,
                                lane_info->tx_attn,
                                lane_info->tx_post);
  if (rc) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_tx_invert_set(
      dev_id, ring, tx_sd, lane_info->tx_phy_pn_swap ? 1 : 0);
  if (rc) return BF_INVALID_ARG;

  // Configure Rx parameters
  rc = port_mgr_av_sd_rx_invert_set(
      dev_id, ring, rx_sd, lane_info->rx_phy_pn_swap ? 1 : 0);
  if (rc) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_rx_term_set(dev_id, ring, rx_sd, BF_SDS_RX_TERM_AVDD);
  if (rc) return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/** \brief Setup PRBS on a serdes slice.
 *
 * [ POST_ENABLE ] / [PRE_ENABLE]
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port       : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 * \param prbs_speed : Speed in which PRBS test is to be set (25G or 10G)
 *
 * \return: BF_SUCCESS    : PRBS init successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_prbs_init(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                int lane,
                                bf_port_prbs_speed_t prbs_speed) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    /*FIXME : For the time being, this assumes symmetric serdes and hence we can
     * pass either tx_sd or rx_sd*/
    bf_sys_assert(serdes_p->tx_sd == serdes_p->rx_sd);
    sd_init_prbs(NULL,
                 dev_id,
                 serdes_p->ring,
                 serdes_p->tx_sd,
                 0 /*ELB*/,
                 prbs_speed == BF_PORT_PRBS_SPEED_10G ? 10 : 25);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Set PRBS mode on a serdes slice.
 *
 * [ POST_ENABLE ] / [PRE_ENABLE]
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port       : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 * \param prbs_mode  : Mode of the PRBS test (31, 23, 15, 13, 11, 9, 7)
 *
 * \return: BF_SUCCESS    : PRBS mode set successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_prbs_mode_set(bf_dev_id_t dev_id,
                                    bf_port_prbs_mode_t prbs_mode) {
  // Set the PRBS mode
  switch (prbs_mode) {
    case BF_PORT_PRBS_MODE_31:
      sd_set_prbs_mode(NULL, 31);
      break;
    case BF_PORT_PRBS_MODE_23:
      sd_set_prbs_mode(NULL, 23);
      break;
    case BF_PORT_PRBS_MODE_15:
      sd_set_prbs_mode(NULL, 15);
      break;
    case BF_PORT_PRBS_MODE_13:
      sd_set_prbs_mode(NULL, 13);
      break;
    case BF_PORT_PRBS_MODE_11:
      sd_set_prbs_mode(NULL, 11);
      break;
    case BF_PORT_PRBS_MODE_9:
      sd_set_prbs_mode(NULL, 9);
      break;
    case BF_PORT_PRBS_MODE_7:
      sd_set_prbs_mode(NULL, 7);
      break;
    default:
      return BF_INVALID_ARG;
  }
  (void)dev_id;
  return BF_SUCCESS;
}

/** \brief Get Eye Heights from vertical bathtub curve (VBTC)
 *
 * \param dev_id: system-assigned identifier *(0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: invalid dev_id
 * \return: BF_INVALID_ARG: op failed
 *
 */
bf_status_t bf_serdes_eye_height_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     int *eye_ht_1e06,
                                     int *eye_ht_1e10,
                                     int *eye_ht_1e12,
                                     int *eye_ht_1e15,
                                     int *eye_ht_1e17) {
  port_mgr_serdes_t *serdes_p;
  int rc;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  rc = port_mgr_av_sd_vbtc_get(dev_id,
                               serdes_p->ring,
                               serdes_p->rx_sd,
                               eye_ht_1e06,
                               eye_ht_1e10,
                               eye_ht_1e12,
                               eye_ht_1e15,
                               eye_ht_1e17);
  return (rc == 0 ? BF_SUCCESS : BF_INVALID_ARG);
}

/** \brief Returns user-specified minimum eye heights at 1e06, 1e10, and 1e12
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_eye_quality_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int *qualifying_eye_ht_1e06,
                                      int *qualifying_eye_ht_1e10,
                                      int *qualifying_eye_ht_1e12) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *qualifying_eye_ht_1e06 = serdes_p->qualifying_eye_ht_1e06;
  *qualifying_eye_ht_1e10 = serdes_p->qualifying_eye_ht_1e10;
  *qualifying_eye_ht_1e12 = serdes_p->qualifying_eye_ht_1e12;

  return BF_SUCCESS;
}

/** \brief Sets user-specified minimum eye heights at 1e06, 1e10, and 1e12
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_eye_quality_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      int qualifying_eye_ht_1e06,
                                      int qualifying_eye_ht_1e10,
                                      int qualifying_eye_ht_1e12) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  serdes_p->qualifying_eye_ht_1e06 = qualifying_eye_ht_1e06;
  serdes_p->qualifying_eye_ht_1e10 = qualifying_eye_ht_1e10;
  serdes_p->qualifying_eye_ht_1e12 = qualifying_eye_ht_1e12;

  return BF_SUCCESS;
}

/** \brief Sets user-specified minimum eye heights at 1e06, 1e10, and 1e12
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_eye_quality_configured_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 int lane,
                                                 int qualifying_eye_ht_1e06,
                                                 int qualifying_eye_ht_1e10,
                                                 int qualifying_eye_ht_1e12) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  // set configured values
  serdes_p->cfgd_qualifying_eye_ht_1e06 = qualifying_eye_ht_1e06;
  serdes_p->cfgd_qualifying_eye_ht_1e10 = qualifying_eye_ht_1e10;
  serdes_p->cfgd_qualifying_eye_ht_1e12 = qualifying_eye_ht_1e12;

  // also set initial "current" values
  serdes_p->qualifying_eye_ht_1e06 = qualifying_eye_ht_1e06;
  serdes_p->qualifying_eye_ht_1e10 = qualifying_eye_ht_1e10;
  serdes_p->qualifying_eye_ht_1e12 = qualifying_eye_ht_1e12;

  return BF_SUCCESS;
}

/** \brief Resets to user-specified minimum eye heights at 1e06, 1e10, and 1e12
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_eye_quality_reset(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  // reset to configured values
  serdes_p->qualifying_eye_ht_1e06 = serdes_p->cfgd_qualifying_eye_ht_1e06;
  serdes_p->qualifying_eye_ht_1e10 = serdes_p->cfgd_qualifying_eye_ht_1e10;
  serdes_p->qualifying_eye_ht_1e12 = serdes_p->cfgd_qualifying_eye_ht_1e12;

  return BF_SUCCESS;
}

/** \brief Set PRBS Error compare mode on a serdes slice.
 *
 * [ POST_ENABLE ] / [PRE_ENABLE]
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port       : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : PRBS Error Compare mode set successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_prbs_cmp_mode_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    sd_cmp_mode_prbs(dev_id, serdes_p->ring, serdes_p->rx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Turn off PRBS on a serdes slice.
 *
 * [ POST_ENABLE ] / [PRE_ENABLE]
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port       : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS    : Turning off PRBS successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_prbs_diag_off(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane) {
  port_mgr_serdes_t *serdes_p;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    /*FIXME : For the time being, this assumes symmetric serdes and hence we can
     * pass either tx_sd or rx_sd*/
    bf_sys_assert(serdes_p->tx_sd == serdes_p->rx_sd);
    sd_diag_off(dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Display stats banner .
 *
 * \param display_ucli_cookie: ucli context (typecasted as (void *)) in which to
 display

 * \return: BF_SUCCESS    : Printing stats banner successful
 * \return: BF_INVALID_ARG: ucli context is NULL
 */
bf_status_t bf_serdes_prbs_stats_banner_display(void *display_ucli_cookie) {
  if (display_ucli_cookie == NULL) return BF_INVALID_ARG;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  sd_print_banner(uc);
  return BF_SUCCESS;
}

/** \brief Get the stats like errors and eye_metric for a serdes slice.
 *
 * [ POST_ENABLE ] / [PRE ENABLE]
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port       : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 *
 * \param[out]  errors     : Errors seen on the serdes slice
 * \param[out]  eye_metric : Eye calculated for a serdes slice

 * \return: BF_SUCCESS    : Turning off PRBS successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_debug_stats_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      int lane,
                                      uint32_t *errors,
                                      uint32_t *eye_metric) {
  port_mgr_serdes_t *serdes_p;
  if (!errors) return BF_INVALID_ARG;
  if (!eye_metric) return BF_INVALID_ARG;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    /*FIXME : For the time being, this assumes symmetric serdes and hence we can
     * pass either tx_sd or rx_sd*/
    bf_sys_assert(serdes_p->tx_sd == serdes_p->rx_sd);
    sd_get_debug_stats_this(
        dev_id, serdes_p->ring, serdes_p->tx_sd, errors, eye_metric);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/** \brief Return a quick estimate of eye height
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_quick_eye_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    int lane,
                                    int *eye_ht) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *eye_ht =
      port_mgr_av_sd_map_eye_quick(dev_id, serdes_p->ring, serdes_p->rx_sd);

  return BF_SUCCESS;
}

/** \brief Return hardware address associated with dev_port/lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_hw_addr_get(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  int lane,
                                  bool tx_dir,
                                  uint32_t *hw_addr1, /*ring*/
                                  uint32_t *hw_addr2 /*sd*/) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  *hw_addr1 = serdes_p->ring;
  *hw_addr2 = (tx_dir) ? serdes_p->tx_sd : serdes_p->rx_sd;

  return BF_SUCCESS;
}

/** \brief Return hardware address associated with dev_port/lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param lane    : logical lane (within port) 0-3, depending upon mode
 * \param dfe_ctrl: BF_SDS_TOF_DFE_CTRL_DEFAULT
 *                  BF_SDS_TOF_DFE_CTRL_ICAL
 *                  BF_SDS_TOF_DFE_CTRL_PCAL
 *                  BF_SDS_TOF_DFE_CTRL_SEEDED_HF
 *                  BF_SDS_TOF_DFE_CTRL_SEEDED_LF
 *                  BF_SDS_TOF_DFE_CTRL_SEEDED_DC
 *                  BF_SDS_TOF_DFE_CTRL_FIXED_HF
 *                  BF_SDS_TOF_DFE_CTRL_FIXED_LF
 *                  BF_SDS_TOF_DFE_CTRL_FIXED_DC
 * \param hf_val  : used only for fixed or seeded hf
 * \param lf_val  : used only for fixed or seeded lf
 * \param dc_val  : used only for fixed or seeded dc
 *
 * \return: BF_SUCCESS,     port is valid and in-use
 * \return: BF_INVALID_ARG: port not valid
 */
bf_status_t bf_serdes_dfe_config_set(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int lane,
                                     bf_sds_tof_dfe_ctrl_t dfe_ctrl,
                                     uint32_t hf_val,
                                     uint32_t lf_val,
                                     uint32_t dc_val) {
  port_mgr_serdes_t *serdes_p;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p == NULL) return BF_INVALID_ARG;

  serdes_p->dfe_ctrl = dfe_ctrl;
  serdes_p->hf_val = hf_val;
  serdes_p->lf_val = lf_val;
  serdes_p->dc_val = dc_val;

  return BF_SUCCESS;
}

/** \brief Set loop bandwidth (bbgain)
 *
 * [ POST_ENABLE ] / [PRE_ENABLE]
 *
 * \param dev_id     : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port       : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane       : logical lane (within port) 0-3, depending upon mode
 * \param setting    : only tested values are 503, 511, 51b. 0=default,
 *-1=default
 *
 * \return: BF_SUCCESS    : PRBS Error Compare mode set successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t bf_serdes_tx_loop_bandwidth_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    int lane,
    bf_sds_tof_tx_loop_bandwidth_t setting) {
  port_mgr_serdes_t *serdes_p;
  bf_sds_tof_tx_loop_bandwidth_t setting_to_apply = setting;

  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p == NULL) {
    return BF_INVALID_ARG;
  }

  if (setting == BF_SDS_TOF_TX_LOOP_BANDWIDTH_DEFAULT) {
    lld_err_t err;
    bf_sku_chip_part_rev_t rev_no;

    /* A0 (rev_no==0) use sig_ok thresh=3
     *  B0 (rev_no==1) use calibrated threshold, just enable
     */
    err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
    if ((err == LLD_OK) && rev_no != 0) {
      setting_to_apply = BF_SDS_TOF_TX_LOOP_BANDWIDTH_B0_DEFAULT;
    } else {
      setting_to_apply = BF_SDS_TOF_TX_LOOP_BANDWIDTH_A0_DEFAULT;
    }
  }
  serdes_p->tx_loop_bandwidth = setting_to_apply;
  port_mgr_av_sd_tx_loop_bandwidth_set(
      dev_id, serdes_p->ring, serdes_p->tx_sd, setting_to_apply);
  return BF_SUCCESS;
}

/** \brief Disable calibration of signal ok detection threshold for the device
 *
 * \param[in]  dev_id       : System-assigned identifier (0..BFN_MAX_ASICS-1)
 *
 * \param[in]  true_or_false: Disable/Enable signal ok threshold calibration
 *                            for the whole device
 */
bf_status_t bf_serdes_signal_ok_thresh_calibration_disable(bf_dev_id_t dev_id,
                                                           bool true_or_false) {
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  dev_p->disable_signal_ok_threshold_calibration = true_or_false;
  return BF_SUCCESS;
}

/** \brief Get LOS status from a serdes slice. Note this API
 *         uses avago_serdes_get_signal_ok() which returns a latched
 *         status of LOS. To get the current status call this API
 *         twice (or use the port_mgr_port_check_signal_ok API).
 *
 * \param dev_id: system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param port  : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane  : logical lane (within port) 0-3, depending upon mode
 * \param los   : returned LOS indication, 1=LOS, 0=no los
 *
 * \return: BF_SUCCESS    : LOS state retrieved successfully
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid bf_dev_port_t0
 * \return: BF_INVALID_ARG: invalid lane
 * \return: BF_INVALID_ARG: los == NULL
 *
 */
bf_status_t bf_serdes_calibration_status_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int lane,
                                             bool *failed) {
  port_mgr_serdes_t *serdes_p;
  uint32_t cal_sts;

  if (failed == NULL) return BF_INVALID_ARG;

  serdes_p = port_mgr_map_port_lane_to_serdes(dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    cal_sts = port_mgr_av_sd_calibration_status_get(
        dev_id, serdes_p->ring, serdes_p->rx_sd);
    if (cal_sts != 0xbeef) {
      *failed = true;
    } else {
      *failed = false;
    }
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/**** Notes on using Alternate Reference Clock on Tofino ****/
/* Tofino has two blocks called Clock Observation Blocks (clkobs)
 *
 *
 * clkobs blocks control the generation of alternate reference clock
 *
 * each clkobs pad is a mux + clock divider
 * clkobs mux selects between the two clocks, one coming from
 * each MAC daisy chain. this selection is controlled by the per mac register
 * Tofino.macs.eth_reg.eth_clkobs_ctrl. EachMAC has this selection block that
 * produces the selected clock and presents to the next mac in the daisy chain.
 * This selection could be from various sources including the rx recovered
 * clock from any serdes. This particular selection allows the use of the
 * clock coming from a sync standard.
 * Clock selection selects the clock coming from the previous mac is the
 * selection is -[5:4] : Final Mux : Disable (00b). Ideally, only one MAC's
 * selection should be enabled to reach a particular clkobs pad. Otherwise,
 * the selected clock os the last MAC in the daisy chain would reach the
 * clkobs pad.
 *
 * The final selected clock could be further divided by. This is controlled
 * by the register Tofino.device_select.misc_regs.ref_clk_pad_ctrl
 * clkobs0 pad could be configured to carry the clock coming from
 * mac daisy chain 0 (MAC 8-39,64 or macpll)
 * or
 * mac daisy chain 1 (MAC 0-7, 40-63, pcie, corepll, pciepll)
 *
 * The clocks on clkobs pads are available on the pins of Tofine as output
 * clocks that can be fed to PLLs or similar clock synchronization devices.
 *
 * Ethernet Reference Clock or ALternate Reference Clock control consumption
 * of the reference clock for Transmission. Alternate Referenc Clock is input
 * to the Tofino. There are two of them supposed to be the exact same value,
 * each going to the half of the MACs. External PLL or similar clock
 * synchronization devices are supposed to be driving this clocks. The register
 * , Tofino.ethgpiobr(tl).gpio_common_regs.refclk_sel, selects the clock
 * to beused for each serdes connected to MAC TX lines.
 */
