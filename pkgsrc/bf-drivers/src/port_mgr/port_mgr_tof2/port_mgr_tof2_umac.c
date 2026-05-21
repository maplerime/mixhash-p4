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

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_sku.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_log.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_logical_port.h>
#include <port_mgr/port_mgr_map.h>
#include "port_mgr_tof2_map.h"
#include "port_mgr_tof2_umac.h"
#include "port_mgr_tof2_umac3.h"
#include "port_mgr_tof2_umac4.h"
#include "port_mgr_tof2_microp.h"
#include "port_mgr_tof2_port.h"
#include "umac4c8_access.h"
#include "eth400g_mac_rspec_access.h"
#include "eth400g_pcs_rspec_access.h"
#include "eth100g_reg_rspec_access.h"
#include "autogen-required-headers.h"

// Set to "true" to program thru tv80
bool use_tv80_for_umac_access = false;
bool microp_init_done = false;
bool port_mgr_tof2_umac_is_cpu_port(bf_dev_id_t dev_id, uint32_t umac);

/*****************************************************************************
 * port_mgr_tof2_umac_config
 *
 * Basic speed/fec and channel config (on port-add)
 ****************************************************************************/
void port_mgr_tof2_umac_config(bf_dev_id_t dev_id,
                               uint32_t umac,
                               uint32_t ch,
                               bf_port_speed_t speed,
                               bf_fec_types_t fec,
                               uint32_t n_lanes) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_config(dev_id, umac, ch, speed, fec);
  } else {  // UMAC4
    port_mgr_tof2_umac4_config(dev_id, umac, ch, speed, fec, n_lanes);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_de_config
 *
 * Unconfigure a channel
 ****************************************************************************/
void port_mgr_tof2_umac_de_config(bf_dev_id_t dev_id,
                                  uint32_t umac,
                                  uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_de_config(dev_id, umac, ch);
  } else {  // UMAC4
    port_mgr_tof2_umac4_de_config(dev_id, umac, ch);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_enable
 *
 * Enable a UMAC channel
 ****************************************************************************/
void port_mgr_tof2_umac_enable(bf_dev_id_t dev_id, uint32_t umac, uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_enable(dev_id, umac, ch);
  } else {  // UMAC4
    port_mgr_tof2_umac4_enable(dev_id, umac, ch);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_disable
 *
 * Disable a UMAC channel
 ****************************************************************************/
void port_mgr_tof2_umac_disable(bf_dev_id_t dev_id,
                                uint32_t umac,
                                uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_disable(dev_id, umac, ch);
  } else {  // UMAC4
    port_mgr_tof2_umac4_disable(dev_id, umac, ch);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_link_state_get
 *
 * Return the operational state of the port defined on umac/ch
 ****************************************************************************/
void port_mgr_tof2_umac_link_state_get(bf_dev_id_t dev_id,
                                       uint32_t umac,
                                       uint32_t ch,
                                       bool *up) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_link_state_get(dev_id, umac, ch, up);
  } else {  // UMAC4
    port_mgr_tof2_umac4_link_state_get(dev_id, umac, ch, up);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_link_fault_get
 *
 * Return the link fault status of the port defined on umac/ch
 ****************************************************************************/
void port_mgr_tof2_umac_link_fault_get(bf_dev_id_t dev_id,
                                       uint32_t umac,
                                       uint32_t ch,
                                       bool *pcs_ready,
                                       bool *local_fault,
                                       bool *remote_fault) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    /* get link fault status from umac3 */
    port_mgr_tof2_umac3_link_fault_get(
        dev_id, umac, ch, pcs_ready, local_fault, remote_fault);
  } else {
    /* get link fault status from umac4 */
    port_mgr_tof2_umac4_link_fault_get(
        dev_id, umac, ch, pcs_ready, local_fault, remote_fault);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_get_pcs_status
 *
 * Return the pcs status of the port defined on umac/ch
 ****************************************************************************/
void port_mgr_tof2_umac_get_pcs_status(bf_dev_id_t dev_id,
                                       int umac,
                                       int ch,
                                       bool *pcs_status,
                                       uint32_t *block_lock_per_pcs_lane,
                                       uint32_t *alig_marker_lock_per_pcs_lane,
                                       bool *hi_ber,
                                       bool *block_lock_all,
                                       bool *alignment_marker_lock_all) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    /* get pcs status from umac3 */
    port_mgr_tof2_umac3_get_pcs_status(dev_id,
                                       umac,
                                       ch,
                                       pcs_status,
                                       block_lock_per_pcs_lane,
                                       alig_marker_lock_per_pcs_lane,
                                       hi_ber,
                                       block_lock_all,
                                       alignment_marker_lock_all);
  } else {
    /* get pcs status from umac4 */
    port_mgr_tof2_umac4_get_pcs_status(dev_id,
                                       umac,
                                       ch,
                                       pcs_status,
                                       block_lock_per_pcs_lane,
                                       alig_marker_lock_per_pcs_lane,
                                       hi_ber,
                                       block_lock_all,
                                       alignment_marker_lock_all);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_sw_reset_tx_set
 *
 * Assert or de-assert txswrst on a UMAC3/4 channel
 ****************************************************************************/
void port_mgr_tof2_umac_sw_reset_tx_set(bf_dev_id_t dev_id,
                                        uint32_t umac,
                                        uint32_t ch,
                                        bool assert_reset) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_sw_reset_tx_set(dev_id, umac, ch, assert_reset);
  } else {  // UMAC4
    port_mgr_tof2_umac4_sw_reset_tx_set(dev_id, umac, ch, assert_reset);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_sw_reset_rx_set
 *
 * Assert or de-assert rxswrst on a UMAC3/4 channel
 ****************************************************************************/
void port_mgr_tof2_umac_sw_reset_rx_set(bf_dev_id_t dev_id,
                                        uint32_t umac,
                                        uint32_t ch,
                                        bool assert_reset) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_sw_reset_rx_set(dev_id, umac, ch, assert_reset);
  } else {  // UMAC4
    port_mgr_tof2_umac4_sw_reset_rx_set(dev_id, umac, ch, assert_reset);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_sw_reset_set
 *
 * Assert or de-assert both txswrst AND rxswrst on a UMAC3/4 channel
 ****************************************************************************/
void port_mgr_tof2_umac_sw_reset_set(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t ch,
                                     bool assert_reset) {
  port_mgr_tof2_umac_sw_reset_tx_set(dev_id, umac, ch, assert_reset);
  port_mgr_tof2_umac_sw_reset_rx_set(dev_id, umac, ch, assert_reset);
}

/*****************************************************************************
 * port_mgr_tof2_umac_is_cpu_port
 *
 * Note: This function needs to be updated to support two different
 *       Tof2 SKUs,
 *
 * MAC_ID : CPU MAC is MAC_ID=0 for standard die and MAC_ID=39 (rotated die)
 * 400G MAC is 1-32 for 12p8t, 1-16 for 6p4t_std and 17-32 for 6p4t_rot
 * (rotated die)
 *
 ****************************************************************************/
bool port_mgr_tof2_umac_is_cpu_port(bf_dev_id_t dev_id, uint32_t umac) {
  (void)dev_id;
  if ((umac == PORT_MGR_TOF2_CPU_PORT_UMAC3) ||
      (umac == PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3)) {  // cpu port, UMAC3
    return true;
  }
  return false;
}

/*****************************************************************************
 * port_mgr_tof2_umac_read_counter
 *
 * Sync read of UMAC RMON counter
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_read_counter(bf_dev_id_t dev_id,
                                            uint32_t umac,
                                            uint32_t ch,
                                            bf_rmon_counter_t ctr_id,
                                            uint64_t *ctr_value) {
  bf_status_t rc;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    rc = port_mgr_tof2_umac3_read_counter(dev_id, umac, ch, ctr_id, ctr_value);
  } else {  // UMAC4
    rc = port_mgr_tof2_umac4_read_counter(dev_id, umac, ch, ctr_id, ctr_value);
  }
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_umac_clear_counter
 *
 * Sync read of UMAC RMON counter
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_clear_counter(bf_dev_id_t dev_id,
                                             uint32_t umac,
                                             uint32_t ch) {
  bf_status_t rc;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    rc = port_mgr_tof2_umac3_clear_counter(dev_id, umac, ch);
  } else {  // UMAC4
    rc = port_mgr_tof2_umac4_clear_counter(dev_id, umac, ch);
  }
  return rc;
}

/*****************************************************************************
 ****************************************************************************/
bf_status_t umac_ll_rd(bf_dev_id_t dev_id,
                       uint32_t umac,
                       uint32_t offset,
                       uint32_t *r_data) {
  bf_status_t rc;

  if (microp_init_done && use_tv80_for_umac_access) {
    uint32_t base;

    if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
      base = offsetof(tof2_reg, eth100g_regs);
    } else {
      uint32_t stride =
          offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
      base = offsetof(tof2_reg, eth400g_p1) + ((umac - 1) * stride);
    }
    offset -= base;  // microp works off local offset
    rc = port_mgr_tof2_microp_rd(dev_id, offset, r_data, umac);
    bf_sys_assert(rc == BF_SUCCESS);
  } else {
    rc = lld_read_register(dev_id, offset, r_data);
    bf_sys_assert(rc == BF_SUCCESS);
  }
  return rc;
}

/*****************************************************************************
 ****************************************************************************/
bf_status_t umac_ll_wr(bf_dev_id_t dev_id,
                       uint32_t umac,
                       uint32_t offset,
                       uint32_t w_data) {
  bf_status_t rc;

  if (microp_init_done && use_tv80_for_umac_access) {
    uint32_t base;

    if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
      base = offsetof(tof2_reg, eth100g_regs);
    } else {
      uint32_t stride =
          offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
      base = offsetof(tof2_reg, eth400g_p1) + ((umac - 1) * stride);
    }
    offset -= base;  // microp works off local offset
    rc = port_mgr_tof2_microp_wr(dev_id, offset, w_data, umac);
    bf_sys_assert(rc == BF_SUCCESS);
  } else {
    rc = lld_write_register(dev_id, offset, w_data);
    bf_sys_assert(rc == BF_SUCCESS);
  }
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_umac_lane_map_set
 *
 * Program the lane remapping
 ****************************************************************************/
void port_mgr_tof2_umac_lane_map_set(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t phys_tx_ln[8],
                                     uint32_t phys_rx_ln[8]) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_lane_map_set(dev_id, umac, phys_tx_ln, phys_rx_ln);
  } else {  // UMAC4
    port_mgr_tof2_umac4_lane_map_set(dev_id, umac, phys_tx_ln, phys_rx_ln);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_reset_set
 *
 * Reset Tx path in the umac.
 * One place this is required is after autonegotiatio. This is due to the
 * CLK change executed in the serdes tile based on the autoneg HCD. The CLK
 * change can corrupt the UMAC-to-serdes FIFO and mus be reset to ensure
 * proper operation.
 ****************************************************************************/
void port_mgr_tof2_umac_tx_reset_set(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    // UMAC3
    port_mgr_tof2_umac3_sw_reset_tx_set(dev_id, umac, ch, true);
    bf_sys_usleep(100);
    port_mgr_tof2_umac3_sw_reset_tx_set(dev_id, umac, ch, false);
  } else {  // UMAC4
    port_mgr_tof2_umac_sw_reset_tx_set(dev_id, umac, ch, true);
    port_mgr_tof2_umac_sw_reset_tx_set(dev_id, umac, ch, false);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_rx_reset_set
 *
 * Reset Rx path in the umac.
 * One place this is required is after autonegotiatio. This is due to the
 * CLK change executed in the serdes tile based on the autoneg HCD. The CLK
 * change can corrupt the UMAC-to-serdes FIFO and mus be reset to ensure
 * proper operation.
 ****************************************************************************/
void port_mgr_tof2_umac_rx_reset_set(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    // UMAC3
    port_mgr_tof2_umac3_sw_reset_rx_set(dev_id, umac, ch, true);
    bf_sys_usleep(100);
    port_mgr_tof2_umac3_sw_reset_rx_set(dev_id, umac, ch, false);
  } else {  // UMAC4
    port_mgr_tof2_umac_sw_reset_rx_set(dev_id, umac, ch, true);
    port_mgr_tof2_umac_sw_reset_rx_set(dev_id, umac, ch, false);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_sigovrd_set
 *
 * force rxsigok lo or "pass-thru"
 ****************************************************************************/
void port_mgr_tof2_umac_sigovrd_set(bf_dev_id_t dev_id,
                                    uint32_t umac,
                                    uint32_t ch,
                                    uint32_t n_lanes,
                                    bf_sigovrd_fld_t ovrd_val) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    // UMAC3
    if (ovrd_val == BF_SIGOVRD_PASS_THRU) {
      port_mgr_tof2_umac3_clear_forced_sigok_set(dev_id, umac, ch, n_lanes);
    } else if (ovrd_val == BF_SIGOVRD_FORCE_LO) {
      port_mgr_tof2_umac3_force_sigok_low_set(dev_id, umac, ch, n_lanes);
    } else if (ovrd_val == BF_SIGOVRD_FORCE_HI) {
      port_mgr_tof2_umac3_force_sigok_hi_set(dev_id, umac, ch, n_lanes);
    }
  } else {  // UMAC4
    if (ovrd_val == BF_SIGOVRD_PASS_THRU) {
      port_mgr_tof2_umac4_clear_forced_sigok_set(dev_id, umac, ch, n_lanes);
    } else if (ovrd_val == BF_SIGOVRD_FORCE_LO) {
      port_mgr_tof2_umac4_force_sigok_low_set(dev_id, umac, ch, n_lanes);
    } else if (ovrd_val == BF_SIGOVRD_FORCE_HI) {
      port_mgr_tof2_umac4_force_sigok_hi_set(dev_id, umac, ch, n_lanes);
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_loopback_set
 *
 * Configure (or un-configure) one of the several UMAC loopback modes
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_loopback_set(bf_dev_id_t dev_id,
                                            uint32_t umac,
                                            uint32_t ch,
                                            bf_loopback_mode_e mode) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_loopback_set(dev_id, umac, ch, mode);
  } else {
    port_mgr_tof2_umac4_loopback_set(dev_id, umac, ch, mode);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac_init
 *
 * Apply one-tie UMAC configurations.
 * Currently this includes,
 * - Reset and enable mdioci controller on all macs
 * - Configure iotile serdes sigok polling on all macs
 * - Place UMACs in known state
 * - Setup timestamp offsets for all MACs.
 *     Program eth400g_mac.eth_mac_ts_offset_ctrl
 *     Program eth100g_regs.eth100g_reg.eth_mac_ts_offset_ctrl
 *     Program eth100g_regs_rot.eth100g_reg.eth_mac_ts_offset_ctrl
 ****************************************************************************/
void port_mgr_tof2_umac_init(bf_dev_id_t dev_id, uint32_t clk_div) {
  uint32_t umac4, umac3;
  uint64_t reg64;
  uint64_t ts_offsets_100[2] = {49, 123};
  uint64_t ts_offsets_400[32] = {51,  54,  57,  60,  63,  66,  69,  72,
                                 119, 116, 113, 110, 107, 104, 101, 98,
                                 125, 128, 131, 134, 137, 140, 143, 146,
                                 45,  42,  39,  36,  33,  30,  27,  24};

  /* Get a bitmap of enabled physical pipes. */
  uint32_t phy_pipes = 0;
  uint32_t num_pipes = 0;
  lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  /* Three pipe SKUs (96T and 80T) need to access the fourth tile so init the
   * MACs to allow tile access. */
  if (num_pipes == 3 || num_pipes == 4) {
    phy_pipes = 0xF;
  } else {
    /* This is a two pipe SKU, we need to enable MACs for the two active pipes
     * so use the logical-to-physical mapping to cover rotated die cases. */
    for (bf_dev_pipe_t i = 0; i < num_pipes; ++i) {
      bf_dev_pipe_t phy_pipe = i;
      if (LLD_OK == lld_sku_map_pipe_id_to_phy_pipe_id(dev_id, i, &phy_pipe))
        phy_pipes |= 1 << phy_pipe;
    }
  }

  for (umac4 = 1; umac4 <= 32; umac4++) {
    unsigned int umac_phy_pipe = (umac4 - 1) / 8;
    if (phy_pipes & (1 << umac_phy_pipe))
      port_mgr_tof2_umac4_init(dev_id, umac4, clk_div);
  }
  port_mgr_tof2_umac3_init(dev_id, 0, clk_div);
  port_mgr_tof2_umac3_init(dev_id, 39, clk_div);

  /* Program the 400g UMAC4 blocks. */
  for (umac4 = 1; umac4 <= 32; ++umac4) {
    unsigned int umac_phy_pipe = (umac4 - 1) / 8;
    if (phy_pipes & (1 << umac_phy_pipe)) {
      eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_rmw(
          dev_id, umac4, &reg64, ts_offsets_400[umac4 - 1]);
      eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_rmw(
          dev_id, umac4, &reg64, 0x11c7);
    }
  }
  /* Program the 100g UMAC3 blocks. */
  umac3 = 0;
  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_rmw(
      dev_id, umac3, &reg64, ts_offsets_100[0]);
  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_rmw(
      dev_id, umac3, &reg64, 0x11c7);

  umac3 = 39;  // FIXME get a macro for rotated CPU UMAC3
  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_rmw(
      dev_id, umac3, &reg64, ts_offsets_100[1]);
  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_rmw(
      dev_id, umac3, &reg64, 0x11c7);
}

/*****************************************************************************
 * port_mgr_tof2_umac_forced_sigok_get
 *
 * Return the current settings of the RxSigOk force-hi and force-lo
 * registers.
 ****************************************************************************/
void port_mgr_tof2_umac_forced_sigok_get(bf_dev_id_t dev_id,
                                         uint32_t umac,
                                         uint32_t ch,
                                         uint32_t n_lanes,
                                         uint32_t *force_hi_raw_val,
                                         uint32_t *force_lo_raw_val,
                                         uint32_t *force_hi,
                                         uint32_t *force_lo) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_forced_sigok_get(dev_id,
                                         umac,
                                         ch,
                                         n_lanes,
                                         force_hi_raw_val,
                                         force_lo_raw_val,
                                         force_hi,
                                         force_lo);
  } else {
    port_mgr_tof2_umac4_forced_sigok_get(dev_id,
                                         umac,
                                         ch,
                                         n_lanes,
                                         force_hi_raw_val,
                                         force_lo_raw_val,
                                         force_hi,
                                         force_lo);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_rs_fec_status_and_counters_get
 *
 ****************************************************************************/
void port_mgr_tof2_umac_rs_fec_status_and_counters_get(
    bf_dev_id_t dev_id,
    uint32_t umac,
    uint32_t ch,
    bool *hi_ser,
    bool *fec_align_status,
    uint32_t *fec_corr_cnt,
    uint32_t *fec_uncorr_cnt,
    uint32_t *fec_ser_lane_0,
    uint32_t *fec_ser_lane_1,
    uint32_t *fec_ser_lane_2,
    uint32_t *fec_ser_lane_3,
    uint32_t *fec_ser_lane_4,
    uint32_t *fec_ser_lane_5,
    uint32_t *fec_ser_lane_6,
    uint32_t *fec_ser_lane_7) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_rs_fec_status_and_counters_get(dev_id,
                                                       umac,
                                                       ch,
                                                       hi_ser,
                                                       fec_align_status,
                                                       fec_corr_cnt,
                                                       fec_uncorr_cnt,
                                                       fec_ser_lane_0,
                                                       fec_ser_lane_1,
                                                       fec_ser_lane_2,
                                                       fec_ser_lane_3);

  } else {
    port_mgr_tof2_umac4_rs_fec_status_and_counters_get(dev_id,
                                                       umac,
                                                       ch,
                                                       hi_ser,
                                                       fec_align_status,
                                                       fec_corr_cnt,
                                                       fec_uncorr_cnt,
                                                       fec_ser_lane_0,
                                                       fec_ser_lane_1,
                                                       fec_ser_lane_2,
                                                       fec_ser_lane_3,
                                                       fec_ser_lane_4,
                                                       fec_ser_lane_5,
                                                       fec_ser_lane_6,
                                                       fec_ser_lane_7);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_dis_all_set
 *
 * Disable ALL UMAC leaf interrupts
 ****************************************************************************/
void port_mgr_tof2_umac_dis_all_set(bf_dev_id_t dev_id) {
  uint32_t umac4;
  uint32_t phy_pipes = 0;
  uint32_t num_pipes = 0;

  /* Get a bitmap of enabled physical pipes. */
  lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  for (bf_dev_pipe_t i = 0; i < num_pipes; ++i) {
    bf_dev_pipe_t phy_pipe = i;
    if (LLD_OK == lld_sku_map_pipe_id_to_phy_pipe_id(dev_id, i, &phy_pipe))
      phy_pipes |= 1 << phy_pipe;
  }

  for (umac4 = 1; umac4 <= 32; umac4++) {
    unsigned int umac_phy_pipe = (umac4 - 1) / 8;
    if (phy_pipes & (1 << umac_phy_pipe)) {
      for (uint32_t ch = 0; ch < 8; ch++) {
        // port_mgr_log("%d:%3d:- : Ch%d : disable UMAC leaf interrupts",

        port_mgr_tof2_umac4_ch_int_dis_all_set(dev_id, umac4, ch);
      }
    }
  }

  port_mgr_tof2_umac3_int_dis_all_set(dev_id, PORT_MGR_TOF2_CPU_PORT_UMAC3);
  port_mgr_tof2_umac3_int_dis_all_set(dev_id, PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3);
}

/*****************************************************************************
 * port_mgr_tof2_umac_int_en_set
 *
 * Enable UMAC to generate interrupts
 ****************************************************************************/
void port_mgr_tof2_umac_int_en_set(bf_dev_id_t dev_id, uint32_t umac, bool on) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_int_en_set(dev_id, umac, on);
  } else {  // UMAC4
    port_mgr_tof2_umac4_int_en_set(dev_id, umac, on);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_local_fault_int_en_set
 *
 * Enable/disable LF interrupt generation
 ****************************************************************************/
void port_mgr_tof2_umac_local_fault_int_en_set(bf_dev_id_t dev_id,
                                               uint32_t umac,
                                               uint32_t ch,
                                               bool en) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_local_fault_int_en_set(dev_id, umac, ch, en);
  } else {  // UMAC4
    port_mgr_tof2_umac4_local_fault_int_en_set(dev_id, umac, ch, en);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_remote_fault_int_en_set
 *
 * Enable/disable LF interrupt generation
 ****************************************************************************/
void port_mgr_tof2_umac_remote_fault_int_en_set(bf_dev_id_t dev_id,
                                                uint32_t umac,
                                                uint32_t ch,
                                                bool en) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_remote_fault_int_en_set(dev_id, umac, ch, en);
  } else {  // UMAC4
    port_mgr_tof2_umac4_remote_fault_int_en_set(dev_id, umac, ch, en);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_local_fault_set
 *
 * Force Tx /LF
 ****************************************************************************/
void port_mgr_tof2_umac_tx_local_fault_set(bf_dev_id_t dev_id,
                                           uint32_t umac,
                                           uint32_t ch,
                                           bool on) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_tx_local_fault_set(dev_id, umac, ch, on);
  } else {  // UMAC4
    port_mgr_tof2_umac4_tx_local_fault_set(dev_id, umac, ch, on);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_link_gain_int_en_set
 *
 * Enable/disable link gain interrupt generation
 ****************************************************************************/
void port_mgr_tof2_umac_link_gain_int_en_set(bf_dev_id_t dev_id,
                                             uint32_t umac,
                                             uint32_t ch,
                                             bool en) {
  if (!port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac4_link_gain_int_en_set(dev_id, umac, ch, en);
  }
  return;
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_remote_fault_set
 *
 * Force Tx /LF
 ****************************************************************************/
void port_mgr_tof2_umac_tx_remote_fault_set(bf_dev_id_t dev_id,
                                            uint32_t umac,
                                            uint32_t ch,
                                            bool on) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_tx_remote_fault_set(dev_id, umac, ch, on);
  } else {  // UMAC4
    port_mgr_tof2_umac4_tx_remote_fault_set(dev_id, umac, ch, on);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_idle_set
 *
 * Force Tx /LF
 ****************************************************************************/
void port_mgr_tof2_umac_tx_idle_set(bf_dev_id_t dev_id,
                                    uint32_t umac,
                                    uint32_t ch,
                                    bool on) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_tx_idle_set(dev_id, umac, ch, on);
  } else {  // UMAC4
    port_mgr_tof2_umac4_tx_idle_set(dev_id, umac, ch, on);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_rx_enable
 *
 * Enable/disable a UMAC RX channel
 ****************************************************************************/
void port_mgr_tof2_umac_rx_enable(bf_dev_id_t dev_id,
                                  uint32_t umac,
                                  uint32_t ch,
                                  bool rx_en) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_rx_enable_set(dev_id, umac, ch, rx_en);
  } else {  // UMAC4
    port_mgr_tof2_umac4_rx_enable_set(dev_id, umac, ch, rx_en);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_drain_set
 *
 * Enable/disable txdrain
 ****************************************************************************/
void port_mgr_tof2_umac_tx_drain_set(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t ch,
                                     bool en) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_txdrain_set(dev_id, umac, ch, en);
  } else {  // UMAC4
    port_mgr_tof2_umac4_txdrain_set(dev_id, umac, ch, en);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_flowcontrol_set
 *
 * Set PFC/link pause configs in UMAC
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_flowcontrol_set(bf_dev_id_t dev_id,
                                               uint32_t umac,
                                               uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_flowcontrol_config_set(dev_id, umac, ch);
  } else {  // UMAC4
    port_mgr_tof2_umac4_flowcontrol_config_set(dev_id, umac, ch);
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac_handle_interrupts
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_handle_interrupts(bf_dev_id_t dev_id,
                                                 uint32_t umac,
                                                 uint32_t ch,
                                                 bool *possible_state_chg) {
  bf_status_t err;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    err =
        port_mgr_tof2_umac3_handle_interrupts(dev_id, umac, possible_state_chg);
  } else {
    err = port_mgr_tof2_umac4_handle_interrupts(
        dev_id, umac, ch, possible_state_chg);
  }

  return err;
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_ignore_rx_set
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_tx_ignore_rx_set(bf_dev_id_t dev_id,
                                                uint32_t umac,
                                                uint32_t ch,
                                                bool en) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_tx_ignore_rx_set(dev_id, umac, ch, en);
  } else {
    port_mgr_tof2_umac4_tx_ignore_rx_set(dev_id, umac, ch, en);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac_tx_ignore_rx_set
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_config_get(bf_dev_id_t dev_id,
                                          uint32_t umac,
                                          uint32_t ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_config_get(dev_id, umac, ch);
  } else {
    port_mgr_tof2_umac4_config_get(dev_id, umac, ch);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac_channel_add
 *
 ****************************************************************************/
void port_mgr_tof2_umac_channel_add(bf_dev_id_t dev_id,
                                    uint32_t umac,
                                    uint32_t ch,
                                    uint32_t n_ch) {
  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_channel_add(dev_id, umac, ch, n_ch);
  } else {
    port_mgr_tof2_umac4_channel_add(dev_id, umac, ch, n_ch);
  }
}

void port_mgr_tof2_umac_hw_cfg_get(bf_dev_id_t dev_id,
                                   uint32_t umac,
                                   uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    port_mgr_tof2_umac3_config_get(dev_id, umac, ch);
  } else {
    port_mgr_tof2_umac4_config_get(dev_id, umac, ch);
  }

  lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac, ch, &dev_port);
  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  if (!port_p) return;
  if (!port_p->hw.enabled) return;

  if (port_p->hw.lpbk_mode == BF_LPBK_PIPE) {
    port_p->hw.oper_state = 1;
    port_p->sw.oper_state = port_p->hw.oper_state;
    return;
  }
  bool lnk_up = false;
  port_mgr_tof2_umac_link_state_get(dev_id, umac, ch, &lnk_up);
  /* We have no way to repopulate the sw.oper_state since that is not
     explicitly replayed by the user during cfg replay. Hence just set
     it equal to the value read from hardware */
  port_p->hw.oper_state = lnk_up;
  port_p->sw.oper_state = port_p->hw.oper_state;
}

/****************************************************************************
 * port_mgr_mac_get_1588_timestamp_tx
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_get_1588_timestamp_tx(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint64_t *ts,
                                                     bool *ts_valid,
                                                     int *ts_id) {
  if (!ts || !ts_valid || !ts_id) return BF_INVALID_ARG;
  uint32_t mac_blk = 0;
  uint32_t ch = 0;
  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  if (sts != BF_SUCCESS) return sts;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, mac_blk)) {
    port_mgr_tof2_umac3_get_1588_timestamp_tx(
        dev_id, mac_blk, ch, ts, ts_valid, ts_id);
  } else {  // UMAC4
    port_mgr_tof2_umac4_get_1588_timestamp_tx(
        dev_id, mac_blk, ch, ts, ts_valid, ts_id);
  }

  return sts;
}

/****************************************************************************
 * port_mgr_mac_set_1588_timestamp_delta_tx
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_set_1588_timestamp_delta_tx(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t delta) {
  uint32_t mac_blk = 0;
  uint32_t ch = 0;
  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  if (sts != BF_SUCCESS) return sts;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, mac_blk)) {
    port_mgr_tof2_umac3_set_1588_timestamp_delta_tx(dev_id, mac_blk, ch, delta);
  } else {  // UMAC4
    // For umac4 the offset register is not per channel
    port_mgr_tof2_umac4_set_1588_timestamp_delta_tx(dev_id, mac_blk, delta);
  }

  return sts;
}

/****************************************************************************
 * port_mgr_mac_get_1588_timestamp_delta_tx
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_get_1588_timestamp_delta_tx(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t *delta) {
  if (!delta) return BF_INVALID_ARG;
  uint32_t mac_blk = 0;
  uint32_t ch = 0;
  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  if (sts != BF_SUCCESS) return sts;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, mac_blk)) {
    port_mgr_tof2_umac3_get_1588_timestamp_delta_tx(dev_id, mac_blk, ch, delta);
  } else {  // UMAC4
    // For umac4 the offset register is not per channel
    port_mgr_tof2_umac4_get_1588_timestamp_delta_tx(dev_id, mac_blk, delta);
  }

  return sts;
}

/****************************************************************************
 * port_mgr_mac_set_1588_timestamp_delta_rx
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_set_1588_timestamp_delta_rx(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t delta) {
  uint32_t mac_blk = 0;
  uint32_t ch = 0;
  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  if (sts != BF_SUCCESS) return sts;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, mac_blk)) {
    port_mgr_tof2_umac3_set_1588_timestamp_delta_rx(dev_id, mac_blk, ch, delta);
  } else {  // UMAC4
    // NOT SUPPORTED YET
    return BF_INVALID_ARG;
  }

  return sts;
}

/****************************************************************************
 * port_mgr_mac_get_1588_timestamp_delta_rx
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac_get_1588_timestamp_delta_rx(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t *delta) {
  if (!delta) return BF_INVALID_ARG;
  uint32_t mac_blk = 0;
  uint32_t ch = 0;
  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  if (sts != BF_SUCCESS) return sts;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, mac_blk)) {
    port_mgr_tof2_umac3_get_1588_timestamp_delta_rx(dev_id, mac_blk, ch, delta);
  } else {  // UMAC4
    // NOT SUPPORTED YET
    return BF_INVALID_ARG;
  }

  return sts;
}
