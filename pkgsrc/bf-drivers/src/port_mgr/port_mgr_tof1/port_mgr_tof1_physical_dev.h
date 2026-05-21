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

#ifndef port_mgr_tof1_physical_device_h_included
#define port_mgr_tof1_physical_device_h_included

#include <sys/time.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_dma_types.h>

#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <port_mgr/port_mgr_port_evt.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_serdes_if.h>

// FIXME
#define port_mgr_log_worthy(w, x, y, z) (0)

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct port_mgr_serdes_slice_t {
  // asymmetric mode parameters
  uint32_t tx_en;
  uint32_t tx_speed;
  uint32_t rx_en;
  uint32_t rx_speed;
} port_mgr_serdes_slice_t;

typedef struct port_mgr_serdes_t {
  int ring;
  int tx_sd;
  int rx_sd;
  bf_sds_tx_pll_clksel_t tx_pll_clk;  // Tx PLL clock source

  // Tx parameters
  bool tx_inv;
  int tx_eq_pre;
  int tx_eq_post;
  int tx_eq_atten;
  uint32_t tx_loop_bandwidth;

  // Rx parameters
  bool rx_inv;
  uint32_t rx_sig_ok_thresh;
  uint32_t rx_term;
  uint32_t rx_horz_eye;
  uint32_t rx_vert_eye;

  // Configured minimum "qualifying" eye heights at BER 1e06/1e10/1e12
  int cfgd_qualifying_eye_ht_1e06;
  int cfgd_qualifying_eye_ht_1e10;
  int cfgd_qualifying_eye_ht_1e12;

  // "Current" minimum "qualifying" eye heights at BER 1e06/1e10/1e12
  int qualifying_eye_ht_1e06;
  int qualifying_eye_ht_1e10;
  int qualifying_eye_ht_1e12;

  bf_sds_tof_dfe_ctrl_t dfe_ctrl;
  uint32_t hf_val;
  uint32_t lf_val;
  uint32_t dc_val;
  float pll_ovrclk;
} port_mgr_serdes_t;

/** \typedef port_mgr_mac_block_t:
 *
 */
typedef struct port_mgr_mac_block_s {
  int ch_in_use;       // bit maps of currently allocated channels
  uint32_t txff_ctrl;  // programmed when the MAC config is changed

  // interrupt info
  int comira_int_ch_mask;          // 0x0-0xF
  uint16_t comira_int_regs[0x40];  // mirrors Comira addr range 0x0000-0x003f

  // serdes node addresses associated with each mac channel
  // before swizzling due to board layout
  int sds_node[4];

  /* Serdes lane remap info passed along by the application (bf_pltfm pm)
     during device add */
  uint32_t tx_lane_map[4];
  uint32_t rx_lane_map[4];

  /* Serdes lane remap info read back from the hardware */
  uint32_t hw_tx_lane_map[4];
  uint32_t hw_rx_lane_map[4];

  port_mgr_serdes_t serdes[4];
  port_mgr_serdes_t hw_serdes[4];
  bf_sys_mutex_t mac_stats_mtx;
} port_mgr_mac_block_t;

#define TOF_MAX_MAC_BLOCKS 65  // including CPU MAC, for all 4 pipes
                               /** \typedef port_mgr_tof1_pdev_t:
                                *
                                */
typedef struct port_mgr_tof1_pdev_t {
  uint32_t pipe_log2phy[BF_PIPE_COUNT];
  port_mgr_mac_block_t mac_block[TOF_MAX_MAC_BLOCKS];
  port_mgr_serdes_slice_t slice[2][256];  // indexed by [ring][sd]
  uint8_t is_serdes[2][256];              // indexed by [ring][sd]
  bool
      disable_signal_ok_threshold_calibration;  // if thresh calibr. not desired
  void *aapl_hook;

  // firmware info
  uint32_t sbus_master_fw_ver;
  uint32_t sbus_master_fw_bld;
  uint32_t pcie_fw_ver;
  uint32_t pcie_fw_bld;
  uint32_t serdes_fw_ver;
  uint32_t serdes_fw_bld;
  uint32_t new_serdes_fw_ver;
  char sbus_master_fw[1024];  // serdes firmware file paths
  char pcie_fw[1024];         // ..from the dev_add profile
  char serdes_fw[1024];       // ..
  char new_serdes_fw[1024];
} port_mgr_tof1_pdev_t;

// int port_mgr_dev_ready(bf_dev_id_t dev_id);
// bool port_mgr_dev_is_locked(bf_dev_id_t dev_id);
char *spd_to_str(bf_port_speed_t speed);

uint64_t port_mgr_mac_stat_dma_msg_id_encode(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port);
void port_mgr_mac_stat_dma_msg_id_decode(uint64_t msg_id,
                                         bf_dev_id_t *dev_id,
                                         bf_dev_port_t *dev_port);
void port_mgr_init_mac_stats(bf_dev_id_t dev_id);
void port_mgr_free_mac_stats(bf_dev_id_t dev_id);
void port_mgr_tof1_set_default_all_ports_state(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
