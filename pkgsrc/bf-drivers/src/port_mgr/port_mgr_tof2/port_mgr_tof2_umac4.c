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
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_log.h>
#include <port_mgr/port_mgr_dev.h>
#include <port_mgr/port_mgr_map.h>
#include "port_mgr_tof2_map.h"
#include "port_mgr_tof2_umac.h"
#include "port_mgr_tof2_umac4.h"
#include "port_mgr_tof2_microp.h"
#include "port_mgr_tof2_serdes.h"
#include "umac4c8_access.h"
#include "umac4_ctrs.h"
#include "eth400g_mac_rspec_access.h"
#include "eth400g_pcs_rspec_access.h"
#include "autogen-required-headers.h"

static uint32_t port_mgr_tof2_umac4_config_to_mode(bf_port_speed_t speed,
                                                   bf_fec_types_t fec,
                                                   uint32_t n_ch);
static void port_mgr_tof2_umac4_config_cmn(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch,
                                           uint64_t mode);

// tx/rx sds_mode handlers
// static
bf_status_t port_mgr_tof2_umac4_sds_mode_enc_mode_set(uint32_t ch,
                                                      uint32_t enc_mode,
                                                      uint32_t cfg_wd,
                                                      uint32_t *new_cfg_wd);
// static
bf_status_t port_mgr_tof2_umac4_sds_mode_map_set(uint32_t ch,
                                                 uint32_t phy_lane,
                                                 uint32_t cfg_wd,
                                                 uint32_t *new_cfg_wd);

// speed-specific configurations
static uint32_t port_mgr_tof2_umac4_txff_ctrl_cred_ini_calc(uint64_t mode);
static uint32_t port_mgr_tof2_umac4_txff_ctrl_chnl_mode_calc(uint64_t mode);
static uint32_t port_mgr_tof2_umac4_chnl_seq_calc(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch);
static uint32_t port_mgr_tof2_umac4_config_set(bf_dev_id_t dev_id,
                                               uint32_t umac4,
                                               uint32_t ch);
static void port_mgr_tof2_umac4_timestamp_offset_set(bf_dev_id_t dev_id,
                                                     uint32_t umac4,
                                                     uint32_t ch,
                                                     bf_port_speed_t speed);
static void port_mgr_tof2_umac4_get_speed_from_timestamp_offset(
    bf_dev_id_t dev_id, uint32_t umac4, uint32_t ch, bf_port_speed_t *speed);

// global data
static uint32_t initial_ch_seq[8] = {7, 3, 5, 1, 6, 2, 4, 0};
static uint32_t umac4_ch_used[BF_MAX_DEV_COUNT][33][8] = {{{0}}};

static int speed_enum_to_int(bf_port_speed_t speed) {
  if (speed == BF_SPEED_400G) return 400;
  if (speed == BF_SPEED_200G) return 200;
  if (speed == BF_SPEED_100G) return 100;
  if (speed == BF_SPEED_50G || speed == BF_SPEED_50G_CONS) return 50;
  if (speed == BF_SPEED_40G || speed == BF_SPEED_40G_R2) return 40;
  if (speed == BF_SPEED_25G) return 25;
  if (speed == BF_SPEED_10G) return 10;
  return 1;
}

static bf_port_speed_t speed_int_to_enum(uint64_t speed) {
  if (speed == 400) return BF_SPEED_400G;
  if (speed == 200) return BF_SPEED_200G;
  if (speed == 100) return BF_SPEED_100G;
  if (speed == 50) return BF_SPEED_50G;
  if (speed == 40) return BF_SPEED_40G;
  if (speed == 25) return BF_SPEED_25G;
  if (speed == 10) return BF_SPEED_10G;
  return BF_SPEED_1G;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_channel_add
 *
 * Update ch_used for the given UMAC and channel(s)
 ****************************************************************************/
void port_mgr_tof2_umac4_channel_add(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t ch,
                                     uint32_t n_ch) {
  umac4_ch_used[dev_id][umac][ch] = n_ch;
}

/*****************************************************************************
 * port_mgr_tof2_umac_channel_del
 *
 * Update ch_used for the given UMAC and channel(s)
 ****************************************************************************/
static void port_mgr_tof2_umac_channel_del(bf_dev_id_t dev_id,
                                           uint32_t umac,
                                           uint32_t ch,
                                           uint32_t n_ch) {
  uint32_t ch_idx;

  for (ch_idx = ch; ch_idx < (ch + n_ch); ch_idx++) {
    umac4_ch_used[dev_id][umac][ch_idx] = 0;
  }
  (void)dev_id;
}

/*****************************************************************************
 * port_mgr_tof2_umac_channel_get
 *
 * Return the # of UMAC channels required by the port defined on "ch"
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac_channel_get(bf_dev_id_t dev_id,
                                               uint32_t umac,
                                               uint32_t ch) {
  return umac4_ch_used[dev_id][umac][ch];
  (void)dev_id;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_am_period_set
 *
 *
 ****************************************************************************/
static void port_mgr_tof2_umac4_am_period_set(bf_dev_id_t dev_id,
                                              uint32_t umac4,
                                              uint32_t ch,
                                              bf_port_speed_t speed,
                                              bf_fec_types_t fec) {
  uint64_t reg64, fld64;

  if ((speed == BF_SPEED_40G_R2 || speed == BF_SPEED_50G_CONS) &&
      fec == BF_FEC_TYP_NONE) {
    // Set Rx AM period to 16383
    umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rd(
        dev_id, umac4, ch, &reg64, &fld64, true);
    fld64 &= ~(((1ull << 18ull) - 1) << 44ull);
    fld64 |= (16383ull << 44ull);
    umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_wr(
        dev_id, umac4, ch, &reg64, fld64, true);
    // Set Tx AM period to 65535
    umac4_chconfig310__chconfig31__macoverride_rd(
        dev_id, umac4, ch, &reg64, &fld64, true);
    fld64 &= ~((1ull << 20ull) - 1ull);
    fld64 |= 65535ull;
    umac4_chconfig310__chconfig31__macoverride_wr(
        dev_id, umac4, ch, &reg64, fld64, true);
  }
}

static void port_mgr_tof2_umac4_portspeed_save(bf_dev_id_t dev_id,
                                               uint32_t umac4,
                                               uint32_t ch,
                                               bf_port_speed_t speed) {
  uint32_t scratch_tmp = 0;
  // bit15   : valid
  // bit14:0 : speed
  uint32_t regval = (speed & 0x7FFFul) | (1ull << 15);

  eth400g_mac_rspec_scratch_bfportspeed_rmw(
      dev_id, umac4, ch, &scratch_tmp, regval);
}

static bool port_mgr_tof2_umac4_portspeed_restore(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch,
                                                  bf_port_speed_t *speed) {
  uint32_t scratch_tmp = 0, regval = 0;
  uint8_t valid;

  eth400g_mac_rspec_scratch_bfportspeed_get(
      dev_id, umac4, ch, &scratch_tmp, &regval, true);
  valid = (regval >> 15) & 1;
  *speed = regval & 0x7FFF;
  if (valid)
    return true;
  else
    return false;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_config
 *
 * Basic UMAC4 speed/fec and channel config (on port-add)
 ****************************************************************************/
void port_mgr_tof2_umac4_config(bf_dev_id_t dev_id,
                                uint32_t umac4,
                                uint32_t ch,
                                bf_port_speed_t speed,
                                bf_fec_types_t fec,
                                uint32_t n_lanes) {
  uint64_t mode, chmode0 = 0;
  uint32_t cred_ini, chnl_mode, chnl_seq, chnl_seq_reg;
  uint32_t sds_mode = 0, txff_ctrl = 0, unused_32b_fld = 0;
  uint32_t sd_mode_msk;
  uint32_t sd_mode;
  uint32_t sd_mode_56g = 0x88888888;  // upper bit=1 is "pam4" mode
  uint32_t sd_mode_28g = 0x00000000;  // upper bit=0 is "nrz" mode
  uint64_t n_ch = (uint64_t)n_lanes;
  uint64_t unused_fld;
  bf_serdes_encoding_mode_t enc_mode;

  bf_serdes_encoding_mode_get(speed, n_lanes, &enc_mode);
  port_mgr_tof2_umac4_channel_add(dev_id, umac4, ch, n_ch);

  // determine UMAC4 mode (speed + fec)
  mode = (uint64_t)port_mgr_tof2_umac4_config_to_mode(speed, fec, n_ch);

  // Read mode (to preserve any reserved bits)
  umac4_chmode0__chmode__mode_rd(
      dev_id, umac4, ch, &chmode0, &unused_fld, true);

  // assert the resets first
  umac4_chmode0__chmode__txswrst_wr(dev_id, umac4, ch, &chmode0, 0x1ull, false);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x1ull, true);

  // Set the new mode
  umac4_chmode0__chmode__mode_wr(dev_id, umac4, ch, &chmode0, mode, true);

  // then set the enables
  umac4_chmode0__chmode__txen_wr(dev_id, umac4, ch, &chmode0, 0x1ull, false);
  // TF2LAB-82, After add (before enable) rxswrst=0, rx_en=0
  // umac4_chmode0__chmode__rxen_wr(dev_id, umac4, ch, &chmode0, 0x1ull, false);
  umac4_chmode0__chmode__rxen_wr(dev_id, umac4, ch, &chmode0, 0x0ull, false);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x0ull, false);

  umac4_chmode0__chmode__txjabber_wr(
      dev_id, umac4, ch, &chmode0, 0x2400ull, false);
  umac4_chmode0__chmode__rxjabber_wr(
      dev_id, umac4, ch, &chmode0, 0x2400ull, false);
  umac4_chmode0__chmode__ifglen_wr(dev_id, umac4, ch, &chmode0, 0xCull, false);
  umac4_chmode0__chmode__ifgpacing_wr(
      dev_id, umac4, ch, &chmode0, 0x1ull, true);  // write all 64b

  port_mgr_tof2_umac4_config_cmn(dev_id, umac4, ch, mode);

  if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
    sd_mode = sd_mode_56g;
  } else {
    sd_mode = sd_mode_28g;
  }

  sd_mode_msk = ((0x88888888 >> (32 - (4 * n_ch))) << (4 * ch));

  // read current sds_mode
  eth400g_pcs_rspec_txsds_mode_mode_get(
      dev_id, umac4, &sds_mode, &unused_32b_fld, true);
  sds_mode &= ~sd_mode_msk;
  sds_mode |= (sd_mode & sd_mode_msk);
  eth400g_pcs_rspec_txsds_mode_mode_set(
      dev_id, umac4, &sds_mode, sds_mode, true);

  eth400g_pcs_rspec_rxsds_mode_mode_get(
      dev_id, umac4, &sds_mode, &unused_32b_fld, true);
  sds_mode &= ~sd_mode_msk;
  sds_mode |= (sd_mode & sd_mode_msk);
  eth400g_pcs_rspec_rxsds_mode_mode_set(
      dev_id, umac4, &sds_mode, sds_mode, true);

#ifndef DEVICE_IS_EMULATOR
  // on real HW, force sigok low until after DFE
  port_mgr_tof2_umac4_force_sigok_low_set(dev_id, umac4, ch, n_lanes);
#else
  // on EMU, force sigok hi since there are no actual serdes
  port_mgr_tof2_umac4_force_sigok_hi_set(dev_id, umac4, ch, n_lanes);
#endif

  // read txff first (to preserve any reserved bits)
  eth400g_mac_rspec_txff_ctrl_tx_flush_get(
      dev_id, umac4, ch, &txff_ctrl, &unused_32b_fld, true);

  // now build txff_ctrl word
  eth400g_mac_rspec_txff_ctrl_tx_flush_set(
      dev_id, umac4, ch, &txff_ctrl, 0, false);

  chnl_mode = port_mgr_tof2_umac4_txff_ctrl_chnl_mode_calc(mode);
  eth400g_mac_rspec_txff_ctrl_chnl_mode_set(
      dev_id, umac4, ch, &txff_ctrl, chnl_mode, false);

  cred_ini = port_mgr_tof2_umac4_txff_ctrl_cred_ini_calc(mode);
  eth400g_mac_rspec_txff_ctrl_cred_ini_set(
      dev_id, umac4, ch, &txff_ctrl, cred_ini, true);

  // Get channel sequence based on currently defined ports/speeds
  chnl_seq = port_mgr_tof2_umac4_chnl_seq_calc(dev_id, umac4, ch);
  eth400g_mac_rspec_chnl_seq_chnl_seq_rmw(
      dev_id, umac4, &chnl_seq_reg, chnl_seq);

  // Program timestamp offset values for the channel.
  port_mgr_tof2_umac4_timestamp_offset_set(dev_id, umac4, ch, speed);

  // config custom alignment marker period (if necessary -- 40G-R2)
  port_mgr_tof2_umac4_am_period_set(dev_id, umac4, ch, speed, fec);

  // Save BF_PORT_SPEED in eth400g_mac scratch register for HA
  port_mgr_tof2_umac4_portspeed_save(dev_id, umac4, ch, speed);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_enable
 *
 * Enable a UMAC4 channel
 ****************************************************************************/
void port_mgr_tof2_umac4_enable(bf_dev_id_t dev_id,
                                uint32_t umac4,
                                uint32_t ch) {
  uint64_t unused_fld, chmode0 = 0ull, mode;
#if 1
  uint32_t txff_ctrl = 0, txfifo_flush;
#endif
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  lld_err_t err;

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac4, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  // configure any new settings
  port_mgr_tof2_umac4_config_set(dev_id, umac4, ch);

  // in case FEC mode changed, re-evaluate mode
  mode = (uint64_t)port_mgr_tof2_umac4_config_to_mode(
      port_p->sw.speed, port_p->sw.fec, port_p->sw.n_lanes);

  // Read mode first (to preserve any reserved bits)
  umac4_chmode0__chmode__mode_rd(
      dev_id, umac4, ch, &chmode0, &unused_fld, true);

  // update chmode before de-asserting resets
  umac4_chmode0__chmode__mode_wr(dev_id, umac4, ch, &chmode0, mode, true);

  // un-reset
  umac4_chmode0__chmode__txswrst_wr(dev_id, umac4, ch, &chmode0, 0x0ull, false);
  // TF2LAB-82, after enable, rxswrst=0, rx_en=1
  umac4_chmode0__chmode__rxen_wr(dev_id, umac4, ch, &chmode0, 0x1ull, false);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x0ull, true);

  // read to preserve other fields
  eth400g_mac_rspec_txff_ctrl_tx_flush_get(
      dev_id, umac4, ch, &txff_ctrl, &txfifo_flush, true);

  // de-assert flush
  eth400g_mac_rspec_txff_ctrl_tx_flush_set(
      dev_id, umac4, ch, &txff_ctrl, 0, false);

  // assert ch_ena
  eth400g_mac_rspec_txff_ctrl_chnl_ena_set(
      dev_id, umac4, ch, &txff_ctrl, 1, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_disable
 *
 * Disable a UMAC4 channel
 * 1) Flush Txfifo and disable channel by setting bits [1:0] =2?b0 in register
 *  ETH400G_MAC_RSPEC_TXFF_CTRL_ADDRESS
 * 2) set flush = 1, chan enable =0
 * 3) Wait till fifo count is 0. (Poll bits [17:8] in
 *  ETH400G_MAC_RSPEC_TXFF_STATUS_ADDRESS
 * 4) Set flush =0
 * 5) Deassert bit 0(chnl_ena) in ETH400G_MAC_RSPEC_TXFF_CTRL_ADDRESS

 ****************************************************************************/
void port_mgr_tof2_umac4_disable(bf_dev_id_t dev_id,
                                 uint32_t umac4,
                                 uint32_t ch) {
  uint64_t unused_fld, chmode0 = 0ull;
  uint32_t txff_ctrl, txfifo_flush;
  uint32_t txff_status, txff_count;
  int max_wait = 10;

#ifndef DEVICE_IS_EMULATOR
  uint32_t n_ch = port_mgr_tof2_umac_channel_get(dev_id, umac4, ch);
  // on real HW, force sigok low. This will give the LP a chance to see
  // some faults and bring there side of the link down.
  port_mgr_tof2_umac4_force_sigok_low_set(dev_id, umac4, ch, n_ch);
#endif

  // disable ch interrupts
  port_mgr_tof2_umac4_ch_int_dis_all_set(dev_id, umac4, ch);

  // read to preserve other fields
  eth400g_mac_rspec_txff_ctrl_tx_flush_get(
      dev_id, umac4, ch, &txff_ctrl, &txfifo_flush, true);
  // assert flush
  eth400g_mac_rspec_txff_ctrl_tx_flush_set(
      dev_id, umac4, ch, &txff_ctrl, 1, true);
  // Wait till fifo count is 0. (Poll bits [17:8]
  do {
    eth400g_mac_rspec_txff_status_txff_count_get(
        dev_id, umac4, ch, &txff_status, &txff_count, true);
  } while ((txff_count != 0) && (--max_wait >= 0));
  if (txff_count != 0) {
    port_mgr_log(
        "UMAC: %d:%2d:%d : Warning: TxFifo not drained after 10 checks: "
        "txff_count=%08x",
        dev_id,
        umac4,
        ch,
        txff_count);
  }
  // de-assert ch_ena
  eth400g_mac_rspec_txff_ctrl_chnl_ena_set(
      dev_id, umac4, ch, &txff_ctrl, 0, true);

  // Read mode first (to preserve any reserved bits)
  umac4_chmode0__chmode__mode_rd(
      dev_id, umac4, ch, &chmode0, &unused_fld, true);

  // reset
  umac4_chmode0__chmode__txswrst_wr(dev_id, umac4, ch, &chmode0, 0x1ull, false);
  // TF2LAB-82, after disable, rxswrst=0-1-0, rx_en=0
  // umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x1ull,
  // true);
  umac4_chmode0__chmode__rxen_wr(dev_id, umac4, ch, &chmode0, 0x0ull, false);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x1ull, true);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x0ull, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_link_state_get
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_link_state_get(bf_dev_id_t dev_id,
                                        uint32_t umac4,
                                        uint32_t ch,
                                        bool *up) {
  uint64_t reg64 = 0, linkup = 0;

  umac4_chsts0__chsts__linkup_rd(dev_id, umac4, ch, &reg64, &linkup, true);
  /* Occasionally, the linkup bit (chsts.9) was set when the link was down, in
   * those situations one or more of the following bits were 0 (when link is up
   * all these bits should be '1'):
   *  [1] rxclkpresentall
   *  [2] rxsigokall
   *  [3] blocklockall
   *  [4] amlockall
   *  [5] aligned
   *  [6] nohiber
   *  [7] nolocalfault
   *  [8] noremotefault
   * So linkup condition is qualified checking that linkup bit (bit [9]) and
   * all other bits mentioned before are also '1'.
   * Note there is not need to validat  bit [0] txclkpresnetall.
   */
  *up = ((reg64 & 0x3FEull) == 0x3FEull) ? true : false;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_link_fault_get
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_link_fault_get(bf_dev_id_t dev_id,
                                        uint32_t umac4,
                                        uint32_t ch,
                                        bool *pcs_ready,
                                        bool *local_fault,
                                        bool *remote_fault) {
  uint64_t noremote_fault = 0;
  uint64_t nolocal_fault = 0;
  uint64_t aligned = 0;
  uint64_t reg64 = 0;

  umac4_chsts0__chsts__nolocalfault_rd(
      dev_id, umac4, ch, &reg64, &nolocal_fault, true);
  umac4_chsts0__chsts__noremotefault_rd(
      dev_id, umac4, ch, &reg64, &noremote_fault, false);
  umac4_chsts0__chsts__aligned_rd(dev_id, umac4, ch, &reg64, &aligned, false);
  *pcs_ready = (aligned != 0);
  if (aligned) {
    *local_fault = (nolocal_fault == 0);
    *remote_fault = (noremote_fault == 0 && nolocal_fault == 1);
  } else {
    *local_fault = false;
    *remote_fault = false;
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac4_get_pcs_status
 *
 * Return the pcs status of the port defined on umac3/ch
 ****************************************************************************/
void port_mgr_tof2_umac4_get_pcs_status(bf_dev_id_t dev_id,
                                        int umac4,
                                        int ch,
                                        bool *pcs_status,
                                        uint32_t *block_lock_per_pcs_lane,
                                        uint32_t *alig_marker_lock_per_pcs_lane,
                                        bool *hi_ber,
                                        bool *block_lock_all,
                                        bool *alignment_marker_lock_all) {
  uint64_t fd64a = 0;
  uint64_t reg64 = 0;

  umac4_chsts0__chsts__amlockall_rd(dev_id, umac4, ch, &reg64, &fd64a, true);
  *alignment_marker_lock_all = fd64a ? true : false;

  umac4_chsts0__chsts__nohiber_rd(dev_id, umac4, ch, &reg64, &fd64a, false);
  *hi_ber = fd64a ? false : true;

  umac4_chsts0__chsts__blocklockall_rd(
      dev_id, umac4, ch, &reg64, &fd64a, false);
  *block_lock_all = fd64a ? true : false;

  // pcs status should be True (which means "PCS is ready") if:
  //  rxclkpresentall && rxsigokall && blocklockall && amlockall && aligned &&
  //  nolocalfault
  *pcs_status = (reg64 & 0x00be) == 0x00be;

  // block lock per lane and alignment per lane not supported
  *block_lock_per_pcs_lane = 0;
  *alignment_marker_lock_all = 0;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sw_reset_tx_set
 *
 * Asert ro de-assert txswrst on a UMAC4 channel
 ****************************************************************************/
void port_mgr_tof2_umac4_sw_reset_tx_set(bf_dev_id_t dev_id,
                                         uint32_t umac4,
                                         uint32_t ch,
                                         bool assert_reset) {
  uint64_t chmode0, state = assert_reset ? 1ull : 0ull;

  umac4_chmode0__chmode__txswrst_rmw(dev_id, umac4, ch, &chmode0, state);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sw_reset_rx_set
 *
 * Asert ro de-assert txswrst on a UMAC4 channel
 ****************************************************************************/
void port_mgr_tof2_umac4_sw_reset_rx_set(bf_dev_id_t dev_id,
                                         uint32_t umac4,
                                         uint32_t ch,
                                         bool assert_reset) {
  uint64_t chmode0, state = assert_reset ? 1ull : 0ull;

  umac4_chmode0__chmode__rxswrst_rmw(dev_id, umac4, ch, &chmode0, state);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sw_reset_set
 *
 * Assert ro de-assert both txswrst AND rxswrst on a UMAC4 channel
 ****************************************************************************/
void port_mgr_tof2_umac4_sw_reset_set(bf_dev_id_t dev_id,
                                      uint32_t umac4,
                                      uint32_t ch,
                                      bool assert_reset) {
  port_mgr_tof2_umac4_sw_reset_tx_set(dev_id, umac4, ch, assert_reset);
  port_mgr_tof2_umac4_sw_reset_rx_set(dev_id, umac4, ch, assert_reset);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_txff_ctrl_cred_ini_calc
 *
 * Calculate the cred_ini field value from the speed
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac4_txff_ctrl_cred_ini_calc(uint64_t mode) {
  uint32_t cred_ini_val;

  if (mode >= 55) {
    cred_ini_val = 0x7f;  // 400G
  } else if (mode >= 52) {
    cred_ini_val = 0x3f;  // 200G
  } else if (mode >= 46) {
    cred_ini_val = 0x1f;  // 100G
  } else if (mode >= 37) {
    cred_ini_val = 0xf;  // 50G
  } else {
    cred_ini_val = 0xf;  // < 50G
  }
  return cred_ini_val;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_txff_ctrl_chnl_mode_calc
 *
 * Calculate the chnl_mode field value from the speed
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac4_txff_ctrl_chnl_mode_calc(uint64_t mode) {
  uint32_t chnl_mode_val;

  if (mode >= 55) {
    chnl_mode_val = 0x0;  // 400G
  } else if (mode >= 52) {
    chnl_mode_val = 0x1;  // 200G
  } else if (mode >= 46) {
    chnl_mode_val = 0x2;  // 100G
  } else if (mode >= 37) {
    chnl_mode_val = 0x3;  // 50G
  } else {
    chnl_mode_val = 0x3;  // < 50G
  }
  return chnl_mode_val;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_chnl_seq_calc
 *
 * Calculate the channel sequence based on all defined ports on this UMAC4s
 * 8x channels.
 *
 * We maintain a map of ports to channels,
 *   CH_USED_8 = 8
 *   CH_USED_4 = 4
 *   CH_USED_2 = 2
 *   CH_USED_1 = 1
 *   CH_USED_0 = 0
 *
 * INITIAL_CH_CFG = [7,3,5,1,6,2,4,0]
 *
 * Algorithm:
 *  chnl_seq = INITIAL_CH_CFG
 *  foreach ch {
 *    if ch_used[ch] != CH_USED_0
 *      for slot = 0 to 7
 *        if (chnl_seq[slot] >= ch) and (chnl_seq[slot] < (ch + ch_used[ch])
 *          chnl_seq[slot] = ch
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac4_chnl_seq_calc(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch) {
  uint32_t c, chnl_seq;
  uint32_t n_ch;
  uint32_t ch_seq[8];

  for (ch = 0; ch < 8; ch++) {
    ch_seq[ch] = initial_ch_seq[ch];
  }

  for (ch = 0; ch < 8; ch++) {
    n_ch = port_mgr_tof2_umac_channel_get(dev_id, umac4, ch);
    if (n_ch != 0) {
      for (c = 0; c < 8; c++) {
        if ((ch_seq[c] >= ch) && (ch_seq[c] < (ch + n_ch))) {
          ch_seq[c] = ch;
        }
      }
    }
  }
  // now convert ch_seq into the txff_ctrl.chnl_seq field
  chnl_seq = 0ull;
  for (c = 0; c < 8; c++) {
    uint32_t fld = ch_seq[c];
    chnl_seq = (chnl_seq << 3) | fld;
  }
  port_mgr_log("chnl_used: [%2d,%2d,%2d,%2d,%2d,%2d,%2d,%2d]",
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 0),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 1),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 2),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 3),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 4),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 5),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 6),
               port_mgr_tof2_umac_channel_get(dev_id, umac4, 7));
  port_mgr_log("chnl_seq : [%2d,%2d,%2d,%2d,%2d,%2d,%2d,%2d]",
               ch_seq[0],
               ch_seq[1],
               ch_seq[2],
               ch_seq[3],
               ch_seq[4],
               ch_seq[5],
               ch_seq[6],
               ch_seq[7]);
  port_mgr_log("chnl_seq : %08x", chnl_seq);

  return chnl_seq;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_config_cmn
 *
 * Configure parameters that are common to all speed/fec modes
 ****************************************************************************/
static void port_mgr_tof2_umac4_config_cmn(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch,
                                           uint64_t mode) {
  uint64_t maccfg = 0;
  uint64_t chconfig3 = 0;
  uint64_t fifo_thd, fifo_thd_scale, txrd_thresh;

  if (mode >= 55ull) {
    fifo_thd_scale = 1;  // 400G
  } else if (mode >= 52ull) {
    fifo_thd_scale = 2;  // 200G
  } else if (mode >= 46ull) {
    fifo_thd_scale = 4;  // 100G
  } else if (mode >= 37ull) {
    fifo_thd_scale = 8;  // 50G
  } else {
    fifo_thd_scale = 12;  // < 50G
  }
  fifo_thd =
      256 -
      16 * fifo_thd_scale;  // set almost full threshold at hf0, 16 entries left

  switch (mode) {
    case UMAC4_MODE_10GBASE_R:
    case UMAC4_MODE_10GBASE_R_FCFEC:
      txrd_thresh = 5ull;
      break;
    case UMAC4_MODE_25GBASE_R1:
    case UMAC4_MODE_25GBASE_R1_FCFEC:
    case UMAC4_MODE_25GBASE_R1_RSFEC:
    case UMAC4_MODE_25GBASE_R1_RSFEC_r1_5:
    case UMAC4_MODE_25GBASE_R1_RSFEC_r1_6:
      txrd_thresh = 3ull;
      break;
    default:
      txrd_thresh = 2ull;
      break;
  }
  //  lld_write_register 0 [format "0x%x" [expr {$base_addr +  0x8}]] 0x78200020
  //
  // Read mode first (to preserve any reserved bits)
  // umac4_maccfg0__maccfg__disfcsonerr_rd(dev_id, umac4, ch, &maccfg,
  // &unused_fld, true);
  umac4_maccfg0__maccfg__disfcsonerr_wr(
      dev_id, umac4, ch, &maccfg, 0x1ull, false);
  umac4_maccfg0__maccfg__txpadrunt_wr(
      dev_id, umac4, ch, &maccfg, 0x40ull, false);
  umac4_maccfg0__maccfg__txwrthresh_wr(
      dev_id, umac4, ch, &maccfg, fifo_thd, false);
  umac4_maccfg0__maccfg__txrdthresh_wr(
      dev_id, umac4, ch, &maccfg, txrd_thresh, false);
  umac4_maccfg0__maccfg__rxpadrunt_wr(
      dev_id, umac4, ch, &maccfg, 0x40ull, true);  // write it out

  //  lld_write_register 0 [format "0x%x" [expr {$base_addr + 0x18}]] 0x24000000
  //
  umac4_chconfig30__chconfig3__rxmaxfrmsize_rmw(
      dev_id, umac4, ch, &chconfig3, 0x2400ull);  // written out
}

/*****************************************************************************
 * port_mgr_tof2_umac4_de_config
 *
 * De-Configure a UMAC4 port being removed
 ****************************************************************************/
void port_mgr_tof2_umac4_de_config(bf_dev_id_t dev_id,
                                   uint32_t umac4,
                                   uint32_t ch) {
  uint32_t chnl_seq_reg, chnl_seq;
  uint32_t n_ch;
  uint64_t chmode0;

  n_ch = port_mgr_tof2_umac_channel_get(dev_id, umac4, ch);
  port_mgr_tof2_umac_channel_del(dev_id, umac4, ch, n_ch);

  // set mode to DISABLED
  umac4_chmode0__chmode__mode_rmw(
      dev_id, umac4, ch, &chmode0, UMAC4_MODE_RESET);

  // Get channel sequence based on currently defined ports/speeds
  chnl_seq = port_mgr_tof2_umac4_chnl_seq_calc(dev_id, umac4, ch);
  eth400g_mac_rspec_chnl_seq_chnl_seq_rmw(
      dev_id, umac4, &chnl_seq_reg, chnl_seq);

  // test: reset serdes mode to PAM4
  uint32_t sds_mode, unused_32b_fld,
      sd_mode_msk = ((0x88888888 >> (32 - (4 * n_ch))) << (4 * ch));
  uint32_t sd_mode = 0x88888888;

  // read current sds_mode
  eth400g_pcs_rspec_txsds_mode_mode_get(
      dev_id, umac4, &sds_mode, &unused_32b_fld, true);
  sds_mode &= ~sd_mode_msk;
  sds_mode |= (sd_mode & sd_mode_msk);
  eth400g_pcs_rspec_txsds_mode_mode_set(
      dev_id, umac4, &sds_mode, sds_mode, true);

  eth400g_pcs_rspec_rxsds_mode_mode_get(
      dev_id, umac4, &sds_mode, &unused_32b_fld, true);
  sds_mode &= ~sd_mode_msk;
  sds_mode |= (sd_mode & sd_mode_msk);
  eth400g_pcs_rspec_rxsds_mode_mode_set(
      dev_id, umac4, &sds_mode, sds_mode, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_mode_to_config
 *****************************************************************************/

static void port_mgr_tof2_umac4_mode_to_config(uint64_t mode,
                                               bf_port_speed_t *speed,
                                               bf_fec_types_t *fec,
                                               uint32_t *n_ch) {
  if (!speed || !fec || !n_ch) return;

  if (mode == UMAC4_MODE_400GBASE_R8_RSFEC) {
    *speed = BF_SPEED_400G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 8;
  } else if (mode == UMAC4_MODE_200GBASE_R4_RSFEC) {
    *speed = BF_SPEED_200G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 4;
  } else if (mode == UMAC4_MODE_200GBASE_R8_RSFEC) {
    *speed = BF_SPEED_200G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 8;
  } else if (mode == UMAC4_MODE_100GBASE_R2_RSFEC) {
    *speed = BF_SPEED_100G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_100GBASE_R2) {
    *speed = BF_SPEED_100G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_100GBASE_R4_RSFEC) {
    *speed = BF_SPEED_100G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 4;
  } else if (mode == UMAC4_MODE_100GBASE_R4) {
    *speed = BF_SPEED_100G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 4;
  } else if (mode == UMAC4_MODE_50GBASE_R1_RSFEC) {
    *speed = BF_SPEED_50G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_50GBASE_R2_RSFEC) {
    *speed = BF_SPEED_50G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_50GBASE_R2_FCFEC) {
    *speed = BF_SPEED_50G;
    *fec = BF_FEC_TYP_FC;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_50GBASE_R2) {
    *speed = BF_SPEED_50G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_40GBASE_R2_RSFEC) {
    *speed = BF_SPEED_40G_R2;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_40GBASE_R2_FCFEC) {
    *speed = BF_SPEED_40G_R2;
    *fec = BF_FEC_TYP_FC;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_40GBASE_R2) {
    *speed = BF_SPEED_40G_R2;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 2;
  } else if (mode == UMAC4_MODE_40GBASE_R4_FCFEC) {
    *speed = BF_SPEED_40G;
    *fec = BF_FEC_TYP_FC;
    *n_ch = 4;
  } else if (mode == UMAC4_MODE_40GBASE_R4) {
    *speed = BF_SPEED_40G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 4;
  } else if (mode == UMAC4_MODE_25GBASE_R1_RSFEC) {
    *speed = BF_SPEED_25G;
    *fec = BF_FEC_TYP_RS;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_25GBASE_R1_FCFEC) {
    *speed = BF_SPEED_25G;
    *fec = BF_FEC_TYP_FC;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_25GBASE_R1) {
    *speed = BF_SPEED_25G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_10GBASE_R_FCFEC) {
    *speed = BF_SPEED_10G;
    *fec = BF_FEC_TYP_FC;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_10GBASE_R) {
    *speed = BF_SPEED_10G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_1GBASE_X) {
    *speed = BF_SPEED_1G;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 1;
  } else if (mode == UMAC4_MODE_RESET) {
    *speed = BF_SPEED_NONE;
    *fec = BF_FEC_TYP_NONE;
    *n_ch = 0;
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac4_config_to_mode
 *****************************************************************************/
static uint32_t port_mgr_tof2_umac4_config_to_mode(bf_port_speed_t speed,
                                                   bf_fec_types_t fec,
                                                   uint32_t n_ch) {
  if ((speed == BF_SPEED_400G) && (fec == BF_FEC_TYP_RS) && (n_ch == 8)) {
    return UMAC4_MODE_400GBASE_R8_RSFEC;
  } else if ((speed == BF_SPEED_200G) && (fec == BF_FEC_TYP_RS) &&
             (n_ch == 4)) {
    return UMAC4_MODE_200GBASE_R4_RSFEC;
  } else if ((speed == BF_SPEED_200G) && (fec == BF_FEC_TYP_RS) &&
             (n_ch == 8)) {
    return UMAC4_MODE_200GBASE_R8_RSFEC;
  } else if ((speed == BF_SPEED_100G) && (fec == BF_FEC_TYP_RS) &&
             (n_ch == 2)) {
    return UMAC4_MODE_100GBASE_R2_RSFEC;
  } else if ((speed == BF_SPEED_100G) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 2)) {
    return UMAC4_MODE_100GBASE_R2;
  } else if ((speed == BF_SPEED_100G) && (fec == BF_FEC_TYP_RS) &&
             (n_ch == 4)) {
    return UMAC4_MODE_100GBASE_R4_RSFEC;
  } else if ((speed == BF_SPEED_100G) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 4)) {
    return UMAC4_MODE_100GBASE_R4;
  } else if ((speed == BF_SPEED_50G) && (fec == BF_FEC_TYP_RS) && (n_ch == 1)) {
    return UMAC4_MODE_50GBASE_R1_RSFEC;
  } else if ((speed == BF_SPEED_50G || speed == BF_SPEED_50G_CONS) &&
             (fec == BF_FEC_TYP_RS) && (n_ch == 2)) {
    return UMAC4_MODE_50GBASE_R2_RSFEC;
  } else if ((speed == BF_SPEED_50G || speed == BF_SPEED_50G_CONS) &&
             (fec == BF_FEC_TYP_FC) && (n_ch == 2)) {
    return UMAC4_MODE_50GBASE_R2_FCFEC;
  } else if ((speed == BF_SPEED_50G || speed == BF_SPEED_50G_CONS) &&
             (fec == BF_FEC_TYP_NONE) && (n_ch == 2)) {
    return UMAC4_MODE_50GBASE_R2;
  } else if ((speed == BF_SPEED_40G_R2) && (fec == BF_FEC_TYP_RS) &&
             (n_ch == 2)) {
    return UMAC4_MODE_40GBASE_R2_RSFEC;
  } else if ((speed == BF_SPEED_40G_R2) && (fec == BF_FEC_TYP_FC) &&
             (n_ch == 2)) {
    return UMAC4_MODE_40GBASE_R2_FCFEC;
  } else if ((speed == BF_SPEED_40G_R2) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 2)) {
    return UMAC4_MODE_40GBASE_R2;
  } else if ((speed == BF_SPEED_40G) && (fec == BF_FEC_TYP_FC) && (n_ch == 4)) {
    return UMAC4_MODE_40GBASE_R4_FCFEC;
  } else if ((speed == BF_SPEED_40G) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 4)) {
    return UMAC4_MODE_40GBASE_R4;
  } else if ((speed == BF_SPEED_25G) && (fec == BF_FEC_TYP_RS) && (n_ch == 1)) {
    return UMAC4_MODE_25GBASE_R1_RSFEC;
  } else if ((speed == BF_SPEED_25G) && (fec == BF_FEC_TYP_FC) && (n_ch == 1)) {
    return UMAC4_MODE_25GBASE_R1_FCFEC;
  } else if ((speed == BF_SPEED_25G) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 1)) {
    return UMAC4_MODE_25GBASE_R1;
  } else if ((speed == BF_SPEED_10G) && (fec == BF_FEC_TYP_FC) && (n_ch == 1)) {
    return UMAC4_MODE_10GBASE_R_FCFEC;
  } else if ((speed == BF_SPEED_10G) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 1)) {
    return UMAC4_MODE_10GBASE_R;
  } else if ((speed == BF_SPEED_1G) && (fec == BF_FEC_TYP_NONE) &&
             (n_ch == 1)) {
    return UMAC4_MODE_1GBASE_X;
  }
  return UMAC4_MODE_RESET;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_status_get
 *
 * Read umac4 chsts register
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_status_get(bf_dev_id_t dev_id,
                                           uint32_t umac,
                                           uint32_t channel,
                                           uint64_t *reg64,
                                           uint64_t *txclkpresentall,
                                           uint64_t *rxclkpresentall,
                                           uint64_t *rxsigokall,
                                           uint64_t *blocklockall,
                                           uint64_t *amlockall,
                                           uint64_t *aligned,
                                           uint64_t *nohiber,
                                           uint64_t *nolocalfault,
                                           uint64_t *noremotefault,
                                           uint64_t *linkup,
                                           uint64_t *hiser,
                                           uint64_t *fecdegser,
                                           uint64_t *rxamsf) {
  umac4_chsts0__chsts__txclkpresentall_rd(
      dev_id, umac, channel, reg64, txclkpresentall, true);
  umac4_chsts0__chsts__rxclkpresentall_rd(
      dev_id, umac, channel, reg64, rxclkpresentall, false);
  umac4_chsts0__chsts__rxsigokall_rd(
      dev_id, umac, channel, reg64, rxsigokall, false);
  umac4_chsts0__chsts__blocklockall_rd(
      dev_id, umac, channel, reg64, blocklockall, false);
  umac4_chsts0__chsts__amlockall_rd(
      dev_id, umac, channel, reg64, amlockall, false);
  umac4_chsts0__chsts__aligned_rd(dev_id, umac, channel, reg64, aligned, false);
  umac4_chsts0__chsts__nohiber_rd(dev_id, umac, channel, reg64, nohiber, false);
  umac4_chsts0__chsts__nolocalfault_rd(
      dev_id, umac, channel, reg64, nolocalfault, false);
  if (*nolocalfault) {
    umac4_chsts0__chsts__noremotefault_rd(
        dev_id, umac, channel, reg64, noremotefault, false);
  } else {
    // if local fault, then force remote fault status to no-fault
    *noremotefault = 1ull;
  }
  umac4_chsts0__chsts__linkup_rd(dev_id, umac, channel, reg64, linkup, false);
  umac4_chsts0__chsts__hiser_rd(dev_id, umac, channel, reg64, hiser, false);
  umac4_chsts0__chsts__fecdegser_rd(
      dev_id, umac, channel, reg64, fecdegser, false);
  umac4_chsts0__chsts__rxamsf_rd(dev_id, umac, channel, reg64, rxamsf, false);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_flowcontrol_config_set
 *
 * Apply flowcontrol (PFC/link pause) configs in  MAC
 ****************************************************************************/
void port_mgr_tof2_umac4_flowcontrol_config_set(bf_dev_id_t dev_id,
                                                uint32_t umac4,
                                                uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  lld_err_t err;
  uint64_t reg64, val;
  uint32_t reg32, fld32;

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac4, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  val = (port_p->sw.link_pause_tx) ? 1 : 0;
  umac4_maccfg0__maccfg__txfcen_rmw(dev_id, umac4, ch, &reg64, val);

  val = (port_p->sw.link_pause_rx) ? 1 : 0;
  umac4_maccfg0__maccfg__rxfcen_rmw(dev_id, umac4, ch, &reg64, val);

  // PFC TX config in MAC is bitmap - per priority enable/disable
  val = port_p->sw.pfc_pause_tx;
  umac4_maccfg0__maccfg__txpfcen_rmw(dev_id, umac4, ch, &reg64, val);

  // PFC RX config in MAC is single bit -no per priority enable/disable
  val = (port_p->sw.pfc_pause_rx) ? 1 : 0;
  umac4_maccfg0__maccfg__rxpfcen_rmw(dev_id, umac4, ch, &reg64, val);

  // Both PFC and pause frames should be filtered out by MAC always
  val = 1;
  umac4_maccfg0__maccfg__rxfilterfc_rmw(dev_id, umac4, ch, &reg64, val);

  umac4_maccfg0__maccfg__rxfilterpfc_rmw(dev_id, umac4, ch, &reg64, val);

  // read txff_ctrl from hw
  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_get(
      dev_id, umac4, ch, &reg32, &fld32, true);
  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_set(
      dev_id, umac4, ch, &reg32, 0, false);
  eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_set(
      dev_id, umac4, ch, &reg32, 0, true);

  // Program TX PFC XOFF config
  fld32 = port_p->sw.pfc_pause_tx;
  eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_rmw(
      dev_id, umac4, ch, &reg32, fld32);

  eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_rmw(
      dev_id,
      umac4,
      ch,
      &reg32,
      (port_p->sw.pfc_pause_tx || port_p->sw.pfc_pause_rx) ? 1 : 0);

  eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_rmw(
      dev_id,
      umac4,
      ch,
      &reg32,
      (port_p->sw.link_pause_tx || port_p->sw.link_pause_rx) ? 1 : 0);

  return;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_config_set
 *
 * Apply user configured MAC settings
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac4_config_set(bf_dev_id_t dev_id,
                                               uint32_t umac4,
                                               uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  lld_err_t err;
  uint64_t reg64, val;

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac4, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  // Program PFC/link pause settings in MAC
  port_mgr_tof2_umac4_flowcontrol_config_set(dev_id, umac4, ch);

  val = port_p->sw.ifg;
  umac4_chmode0__chmode__ifglen_rmw(dev_id, umac4, ch, &reg64, val);

  umac4_chconfig30__chconfig3__txdrainonfault_rmw(dev_id, umac4, ch, &reg64, 1);

  val = port_p->sw.preamble_length;
  umac4_chconfig30__chconfig3__txpreamble_rmw(dev_id, umac4, ch, &reg64, val);

  //?? no promiscuous mode setting?
  // val = port_p->sw.promiscuous_mode ? 1 : 0;

  val = port_p->sw.rx_mtu;
  umac4_chconfig30__chconfig3__rxmaxfrmsize_rmw(dev_id, umac4, ch, &reg64, val);

  // set max jabber size for tx and rx. Use same value for both
  val = port_p->sw.rx_max_jab_sz;
  umac4_chmode0__chmode__rxjabber_rmw(dev_id, umac4, ch, &reg64, val);

  val = port_p->sw.tx_mtu;
  umac4_chmode0__chmode__txjabber_rmw(dev_id, umac4, ch, &reg64, val);

#if 0
  // only seems to have 1 FC MAC addr (no local MAC addr)
  val = ((uint64_t)port_p->sw.fc_src_mac_addr[0] << 0ull) |
        ((uint64_t)port_p->sw.fc_src_mac_addr[1] << 8ull) |
        ((uint64_t)port_p->sw.fc_src_mac_addr[2] << 16ull) |
        ((uint64_t)port_p->sw.fc_src_mac_addr[3] << 24ull) |
        ((uint64_t)port_p->sw.fc_src_mac_addr[4] << 32ull) |
        ((uint64_t)port_p->sw.fc_src_mac_addr[5] << 40ull);
  umac4_chconfig40__chconfig4__macaddr_rmw(dev_id, umac4, ch, &reg64, val);

  val = ((uint64_t)port_p->sw.fc_dst_mac_addr[0] << 0ull) |
        ((uint64_t)port_p->sw.fc_dst_mac_addr[1] << 8ull) |
        ((uint64_t)port_p->sw.fc_dst_mac_addr[2] << 16ull) |
        ((uint64_t)port_p->sw.fc_dst_mac_addr[3] << 24ull) |
        ((uint64_t)port_p->sw.fc_dst_mac_addr[4] << 32ull) |
        ((uint64_t)port_p->sw.fc_dst_mac_addr[5] << 40ull);
  umac4_chconfig50__chconfig5__pausedest_rmw(dev_id, umac4, ch, &reg64, val);

  // not too sure of this
  val = port_p->sw.xoff_pause_time;
  umac4_chconfig50__chconfig5__pauserefresh_rmw(dev_id, umac4, ch, &reg64, val);

  val = port_p->sw.xon_pause_time;
  umac4_chconfig40__chconfig4__pauseontime_rmw(dev_id, umac4, ch, &reg64, val);
#endif

  //  val = port_p->sw.fc_corr_en ? 0 : 1;
  //  umac4_chconfig60__chconfig6__corrbyp_rmw(dev_id, umac4, ch, &reg64, val);

  val = port_p->sw.fc_ind_en ? 0 : 1;
  //  umac4_chconfig60__chconfig6__indibyp_rmw(dev_id, umac4, ch, &reg64, val);

  return 0;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_config_get
 *
 * Get hw configured MAC settings
 ****************************************************************************/
void port_mgr_tof2_umac4_config_get(bf_dev_id_t dev_id,
                                    uint32_t umac4,
                                    uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  lld_err_t err;
  uint64_t reg64, val;
  uint32_t _reg32, *reg_32 = &_reg32, fld = 0;
  bf_port_speed_t G;

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac4, ch, &dev_port);
  if (err != LLD_OK) {
    port_mgr_log(
        "%s Error: "
        "port_mgr_mac_hw_cfg_get: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        __func__,
        err,
        dev_id,
        umac4,
        ch);
    return;
  }

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  if (port_p == NULL) {
    port_mgr_log("Error: No port_p for dev_port=%x\n", dev_port);
    return;
  }
  lld_err_t ret;
  bool is_internal = false;
  ret = lld_sku_is_dev_port_internal(dev_id, dev_port, &is_internal);
  if (ret != LLD_OK) {
    port_mgr_log("Error: lld_sku_is_dev_port_internal failed for dev_port=%x\n",
                 dev_port);
  }
  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_get(
      dev_id, umac4, ch, reg_32, &fld, true);
  if ((fld != 0) || (is_internal == true)) {
    port_p->hw.lpbk_mode = BF_LPBK_PIPE;
    port_p->hw.loopback_enabled = true;
    port_p->hw.assigned = true;
    port_p->hw.enabled = true;
    if (is_internal != true) {
      port_mgr_tof2_umac4_get_speed_from_timestamp_offset(
          dev_id, umac4, ch, &G);
      port_p->hw.speed = G;
    } else {
      port_p->hw.speed = port_p->sw.speed;
    }
    return;
  }

  // Program PFC/link pause settings in MAC
  umac4_maccfg0__maccfg__txfcen_rd(dev_id, umac4, ch, &reg64, &val, true);
  port_p->hw.link_pause_tx = val;

  umac4_maccfg0__maccfg__rxfcen_rd(dev_id, umac4, ch, &reg64, &val, false);
  port_p->hw.link_pause_rx = val;

  // PFC TX config in MAC is bitmap - per priority enable/disable
  umac4_maccfg0__maccfg__txpfcen_rd(dev_id, umac4, ch, &reg64, &val, false);
  port_p->hw.pfc_pause_tx = val;

  // PFC RX config in MAC is single bit -no per priority enable/disable
  umac4_maccfg0__maccfg__rxpfcen_rd(dev_id, umac4, ch, &reg64, &val, false);
  port_p->hw.pfc_pause_rx = val;

  umac4_chmode0__chmode__ifglen_rd(dev_id, umac4, ch, &reg64, &val, true);
  port_p->hw.ifg = val;

  umac4_chmode0__chmode__rxjabber_rd(dev_id, umac4, ch, &reg64, &val, false);
  port_p->hw.rx_mtu = val;

  umac4_chmode0__chmode__txjabber_rd(dev_id, umac4, ch, &reg64, &val, false);
  port_p->hw.tx_mtu = val;

  umac4_chconfig30__chconfig3__txpreamble_rd(
      dev_id, umac4, ch, &reg64, &val, true);
  port_p->hw.preamble_length = val;

  umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(
      dev_id, umac4, ch, &reg64, &val, false);
  port_p->hw.rx_mtu = val;

  // Currently these are not prgrammed into hardware. So keep same.
  port_p->hw.fc_corr_en = port_p->sw.fc_corr_en;
  port_p->hw.fc_ind_en = port_p->sw.fc_ind_en;
  port_p->hw.xoff_pause_time = port_p->sw.xoff_pause_time;
  port_p->hw.xon_pause_time = port_p->sw.xon_pause_time;
  port_p->hw.promiscuous_mode = port_p->sw.promiscuous_mode;

  port_p->hw.txff_trunc_ctrl_size = port_p->sw.txff_trunc_ctrl_size;
  port_p->hw.txff_trunc_ctrl_en = port_p->sw.txff_trunc_ctrl_en;
  port_p->hw.txff_ctrl_crc_check_disable =
      port_p->sw.txff_ctrl_crc_check_disable;
  port_p->hw.txff_ctrl_fcs_insert_disable =
      port_p->sw.txff_ctrl_fcs_insert_disable;

  port_p->hw.fc_src_mac_addr[0] = port_p->sw.fc_src_mac_addr[0];
  port_p->hw.fc_src_mac_addr[1] = port_p->sw.fc_src_mac_addr[1];
  port_p->hw.fc_src_mac_addr[2] = port_p->sw.fc_src_mac_addr[2];
  port_p->hw.fc_src_mac_addr[3] = port_p->sw.fc_src_mac_addr[3];
  port_p->hw.fc_src_mac_addr[4] = port_p->sw.fc_src_mac_addr[4];
  port_p->hw.fc_src_mac_addr[5] = port_p->sw.fc_src_mac_addr[5];

  port_p->hw.fc_dst_mac_addr[0] = port_p->sw.fc_dst_mac_addr[0];
  port_p->hw.fc_dst_mac_addr[1] = port_p->sw.fc_dst_mac_addr[1];
  port_p->hw.fc_dst_mac_addr[2] = port_p->sw.fc_dst_mac_addr[2];
  port_p->hw.fc_dst_mac_addr[3] = port_p->sw.fc_dst_mac_addr[3];
  port_p->hw.fc_dst_mac_addr[4] = port_p->sw.fc_dst_mac_addr[4];
  port_p->hw.fc_dst_mac_addr[5] = port_p->sw.fc_dst_mac_addr[5];

  uint64_t _reg64, *pReg64 = &_reg64, ch_mode = 0;
  uint32_t reg32, chnl_ena = 0;
  umac4_chmode0__chmode__mode_rd(dev_id, umac4, ch, pReg64, &ch_mode, true);
  eth400g_mac_rspec_txff_ctrl_chnl_ena_get(
      dev_id, umac4, ch, &reg32, &chnl_ena, true);

  port_p->hw.speed = 0;
  port_mgr_tof2_umac4_mode_to_config(
      ch_mode, &port_p->hw.speed, &port_p->hw.fec, &port_p->hw.n_lanes);

  if (port_p->hw.speed) {
    // Distinguish between 40G-R2 special cases with 50G-R2 cases
    bf_port_speed_t hw_speed;
    bool valid = false;
    // bf_serdes_encoding_mode_t enc_mode = 0;
    switch (ch_mode) {
      case UMAC4_MODE_50GBASE_R2:        // UMAC4_MODE_40GBASE_R2
      case UMAC4_MODE_50GBASE_R2_FCFEC:  // UMAC4_MODE_40GBASE_R2_FCFEC
      case UMAC4_MODE_50GBASE_R2_RSFEC:  // UMAC4_MODE_40GBASE_R2_RSFEC
        port_mgr_tof2_umac4_get_speed_from_timestamp_offset(
            dev_id, umac4, ch, &G);
#if 0
        port_mgr_tof2_serdes_fw_lane_speed_get(
            dev_id, dev_port, 0, &G, &enc_mode);
#endif
        if (G == BF_SPEED_40G) {
          port_p->hw.speed = BF_SPEED_40G_R2;
          port_p->hw.n_lanes = 2;
          if (ch_mode == UMAC4_MODE_50GBASE_R2) {
            port_p->hw.fec = BF_FEC_TYP_NONE;
          } else if (ch_mode == UMAC4_MODE_50GBASE_R2_FCFEC) {
            port_p->hw.fec = BF_FEC_TYP_FC;
          } else if (ch_mode == UMAC4_MODE_50GBASE_R2_RSFEC) {
            port_p->hw.fec = BF_FEC_TYP_RS;
          }
        } else {
          valid = port_mgr_tof2_umac4_portspeed_restore(
              dev_id, umac4, ch, &hw_speed);
          if (valid) {
            switch (hw_speed) {
              case BF_SPEED_50G:
                port_p->hw.speed = BF_SPEED_50G;
                break;
              case BF_SPEED_50G_CONS:
                port_p->hw.speed = BF_SPEED_50G_CONS;
                break;
              default:
                port_mgr_log(
                    "Invalid speed:%#x read from eth400_mac scratch reg. "
                    "Defaulting to BF_SPEED_50G",
                    hw_speed);
                port_p->hw.speed = BF_SPEED_50G;
            }
          }
        }
        break;
      default:
        break;
    }

    port_p->hw.assigned = true;
    port_mgr_tof2_umac4_channel_add(dev_id, umac4, ch, port_p->hw.n_lanes);
  } else {
    port_p->hw.assigned = false;
  }

  if (chnl_ena && port_p->hw.speed) {
    port_p->hw.enabled = true;
  } else {
    port_p->hw.enabled = false;
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac4_interrupt_get
 *
 * Read and clear UMAC4 interrupts
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_interrupt_get(bf_dev_id_t dev_id,
                                              uint32_t umac,
                                              uint32_t channel,
                                              uint64_t *reg64) {
  uint64_t fld64;

  umac4_intcontrol0__intcontrol__intsts_rd(
      dev_id, umac, channel, reg64, &fld64, true);

  // write-1-to-clear what we read (only)
  umac4_intcontrol0__intcontrol__intclr_wr(
      dev_id, umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_interrupt_dn_up_get
 *
 * Read and clear UMAC4 interrupts
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_interrupt_dn_up_get(bf_dev_id_t dev_id,
                                                    uint32_t umac,
                                                    uint32_t channel,
                                                    uint64_t *dn,
                                                    uint64_t *up) {
  uint64_t reg64, fld64;

  umac4_intcontrol0__intcontrol__intsts_rd(
      dev_id, umac, channel, &reg64, &fld64, true);

  *dn = (fld64 >> 14) & 1ull;
  *up = (fld64 >> 15) & 1ull;

  // write-1-to-clear what we read (only)
  umac4_intcontrol0__intcontrol__intclr_wr(
      dev_id, umac, channel, &reg64, (fld64 & (3ull << 14)), true);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_chmode_get
 *
 * Read chmode register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_chmode_get(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch,
                                           uint64_t *reg64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__mode_rd(dev_id, umac4, ch, reg64, &unused_fld64, true);

  (void)unused_fld64;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_chmode_detail_get
 *
 * Read chmode register fields. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_chmode_detail_get(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch,
                                                  uint64_t *mode,
                                                  uint64_t *txswrst,
                                                  uint64_t *rxswrst,
                                                  uint64_t *txen,
                                                  uint64_t *txdrain,
                                                  uint64_t *rxen,
                                                  uint64_t *gmiilpbk,
                                                  uint64_t *txjabber,
                                                  uint64_t *rxjabber,
                                                  uint64_t *disfcs,
                                                  uint64_t *invfcs,
                                                  uint64_t *ignfcs,
                                                  uint64_t *stripfcs,
                                                  uint64_t *ifglen,
                                                  uint64_t *ifgpacing) {
  uint64_t _reg64, *reg64 = &_reg64, fld64;
  umac4_chmode0__chmode__mode_rd(dev_id, umac4, ch, reg64, &fld64, true);
  *mode = fld64;
  umac4_chmode0__chmode__txswrst_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txswrst = fld64;
  umac4_chmode0__chmode__rxswrst_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxswrst = fld64;
  umac4_chmode0__chmode__txen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txen = fld64;
  umac4_chmode0__chmode__txdrain_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txdrain = fld64;
  umac4_chmode0__chmode__rxen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxen = fld64;
  umac4_chmode0__chmode__gmiilpbk_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *gmiilpbk = fld64;
  umac4_chmode0__chmode__txjabber_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txjabber = fld64;
  umac4_chmode0__chmode__rxjabber_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxjabber = fld64;
  umac4_chmode0__chmode__disfcs_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *disfcs = fld64;
  umac4_chmode0__chmode__invfcs_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *invfcs = fld64;
  umac4_chmode0__chmode__ignfcs_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *ignfcs = fld64;
  umac4_chmode0__chmode__stripfcs_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *stripfcs = fld64;
  umac4_chmode0__chmode__ifglen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *ifglen = fld64;
  umac4_chmode0__chmode__ifgpacing_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *ifgpacing = fld64;
  return 0;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_maccfg_get
 *
 * Read maccfg register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_maccfg_get(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch,
                                           uint64_t *reg64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__disfcsonerr_rd(
      dev_id, umac4, ch, reg64, &unused_fld64, true);

  (void)unused_fld64;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_maccfg_get
 *
 * Read maccfg register fields. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_maccfg_detail_get(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch,
                                                  uint64_t *disfcsonerr,
                                                  uint64_t *txfcen,
                                                  uint64_t *rxfcen,
                                                  uint64_t *rxpfcen,
                                                  uint64_t *rxfctotx,
                                                  uint64_t *rxfilterfc,
                                                  uint64_t *rxfilterpfc,
                                                  uint64_t *txpadrunt,
                                                  uint64_t *txwrthresh,
                                                  uint64_t *txrdthresh,
                                                  uint64_t *txlfault,
                                                  uint64_t *txrfault,
                                                  uint64_t *txidle,
                                                  uint64_t *rxpadrunt,
                                                  uint64_t *rxlfault,
                                                  uint64_t *rxrfault,
                                                  uint64_t *rxidle,
                                                  uint64_t *statsclr,
                                                  uint64_t *txignorerx,
                                                  uint64_t *txpfcen) {
  uint64_t _reg64, *reg64 = &_reg64, fld64;

  umac4_maccfg0__maccfg__disfcsonerr_rd(dev_id, umac4, ch, reg64, &fld64, true);
  *disfcsonerr = fld64;
  umac4_maccfg0__maccfg__txfcen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txfcen = fld64;
  umac4_maccfg0__maccfg__rxfcen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxfcen = fld64;
  umac4_maccfg0__maccfg__rxpfcen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxpfcen = fld64;
  umac4_maccfg0__maccfg__rxfctotx_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxfctotx = fld64;
  umac4_maccfg0__maccfg__rxfilterfc_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxfilterfc = fld64;
  umac4_maccfg0__maccfg__rxfilterpfc_rd(
      dev_id, umac4, ch, reg64, &fld64, false);
  *rxfilterpfc = fld64;
  umac4_maccfg0__maccfg__txpadrunt_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txpadrunt = fld64;
  umac4_maccfg0__maccfg__txwrthresh_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txwrthresh = fld64;
  umac4_maccfg0__maccfg__txrdthresh_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txrdthresh = fld64;
  umac4_maccfg0__maccfg__txlfault_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txlfault = fld64;
  umac4_maccfg0__maccfg__txrfault_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txrfault = fld64;
  umac4_maccfg0__maccfg__txidle_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txidle = fld64;
  umac4_maccfg0__maccfg__rxpadrunt_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxpadrunt = fld64;
  umac4_maccfg0__maccfg__rxlfault_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxlfault = fld64;
  umac4_maccfg0__maccfg__rxrfault_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxrfault = fld64;
  umac4_maccfg0__maccfg__rxidle_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *rxidle = fld64;
  umac4_maccfg0__maccfg__statsclr_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *statsclr = fld64;
  umac4_maccfg0__maccfg__txignorerx_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txignorerx = fld64;
  umac4_maccfg0__maccfg__txpfcen_rd(dev_id, umac4, ch, reg64, &fld64, false);
  *txpfcen = fld64;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_chconfig30_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_chconfig30_get(bf_dev_id_t dev_id,
                                               uint32_t umac4,
                                               uint32_t ch,
                                               uint64_t *reg64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(
      dev_id, umac4, ch, reg64, &unused_fld64, true);

  (void)unused_fld64;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_chconfig30_get
 *
 * Read chconfig3 register fields. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_chconfig30_detail_get(bf_dev_id_t dev_id,
                                                      uint32_t umac4,
                                                      uint32_t ch,
                                                      uint64_t *ifgppm,
                                                      uint64_t *rxmaxfrmsize,
                                                      uint64_t *txpreamble,
                                                      uint64_t *txdrainonfault,
                                                      uint64_t *rxpreamble,
                                                      uint64_t *rxerrmask) {
  uint64_t _reg64, *reg64 = &_reg64, fld64;

  umac4_chconfig30__chconfig3__ifgppm_rd(
      dev_id, umac4, ch, reg64, &fld64, true);
  *ifgppm = fld64;
  umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(
      dev_id, umac4, ch, reg64, &fld64, false);
  *rxmaxfrmsize = fld64;
  umac4_chconfig30__chconfig3__txpreamble_rd(
      dev_id, umac4, ch, reg64, &fld64, false);
  *txpreamble = fld64;
  umac4_chconfig30__chconfig3__txdrainonfault_rd(
      dev_id, umac4, ch, reg64, &fld64, false);
  *txdrainonfault = fld64;
  umac4_chconfig30__chconfig3__rxerrmask_rd(
      dev_id, umac4, ch, reg64, &fld64, false);
  *rxerrmask = fld64;

  // hack until xml update
  *rxpreamble = (*reg64 >> 38ul) & 1ul;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sdcfg_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_sdcfg_get(bf_dev_id_t dev_id,
                                          uint32_t umac4,
                                          uint32_t ch,
                                          uint64_t *reg64) {
  uint64_t fld64;

  umac4_sdcfg0__sdcfg__serdeslpbk_rd(dev_id, umac4, ch, reg64, &fld64, true);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sdcfg_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_sdcfg_detail_get(bf_dev_id_t dev_id,
                                                 uint32_t umac4,
                                                 uint32_t ch,
                                                 uint64_t *serdeslpbk,
                                                 uint64_t *txremap,
                                                 uint64_t *sigokoverride,
                                                 uint64_t *rxremap,
                                                 uint64_t *txinv,
                                                 uint64_t *rxinv,
                                                 uint64_t *paceren,
                                                 uint64_t *pacerdiv,
                                                 uint64_t *txprbssel,
                                                 uint64_t *rxprbssel) {
  uint64_t reg64;

  port_mgr_tof2_umac4_sdcfg_get(dev_id, umac4, ch, &reg64);
  umac4_sdcfg0__sdcfg__serdeslpbk_rd(
      dev_id, umac4, ch, &reg64, serdeslpbk, false);
  umac4_sdcfg0__sdcfg__txremap_rd(dev_id, umac4, ch, &reg64, txremap, false);
  umac4_sdcfg0__sdcfg__sigokoverride_rd(
      dev_id, umac4, ch, &reg64, sigokoverride, false);
  umac4_sdcfg0__sdcfg__rxremap_rd(dev_id, umac4, ch, &reg64, rxremap, false);
  umac4_sdcfg0__sdcfg__txinv_rd(dev_id, umac4, ch, &reg64, txinv, false);
  umac4_sdcfg0__sdcfg__rxinv_rd(dev_id, umac4, ch, &reg64, rxinv, false);
  umac4_sdcfg0__sdcfg__paceren_rd(dev_id, umac4, ch, &reg64, paceren, false);
  umac4_sdcfg0__sdcfg__pacerdiv_rd(dev_id, umac4, ch, &reg64, pacerdiv, false);
  umac4_sdcfg0__sdcfg__txprbssel_rd(
      dev_id, umac4, ch, &reg64, txprbssel, false);
  umac4_sdcfg0__sdcfg__rxprbssel_rd(
      dev_id, umac4, ch, &reg64, rxprbssel, false);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sderrcfg_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_sderrcfg_get(bf_dev_id_t dev_id,
                                             uint32_t umac4,
                                             uint32_t ch,
                                             uint64_t *reg64) {
  uint64_t fld64;

  umac4_sderrcfg0__sderrcfg__txerrperiod_rd(
      dev_id, umac4, ch, reg64, &fld64, true);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sderrcfg_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_sderrcfg_detail_get(bf_dev_id_t dev_id,
                                                    uint32_t umac4,
                                                    uint32_t ch,
                                                    uint64_t *txerrperiod,
                                                    uint64_t *txerrburst) {
  uint64_t reg64;

  port_mgr_tof2_umac4_sderrcfg_get(dev_id, umac4, ch, &reg64);
  umac4_sderrcfg0__sderrcfg__txerrperiod_rd(
      dev_id, umac4, ch, &reg64, txerrperiod, false);
  umac4_sderrcfg0__sderrcfg__txerrburst_rd(
      dev_id, umac4, ch, &reg64, txerrburst, false);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sdsts_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_sdsts_get(bf_dev_id_t dev_id,
                                          uint32_t umac4,
                                          uint32_t ch,
                                          uint64_t *reg64) {
  uint64_t fld64;

  umac4_sdsts0__sdsts__txclkpresent_rd(dev_id, umac4, ch, reg64, &fld64, true);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sdsts_detail_get
 *
 * Read chconfig3 register. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_sdsts_detail_get(bf_dev_id_t dev_id,
                                                 uint32_t umac4,
                                                 uint32_t ch,
                                                 uint64_t *txclkpresent,
                                                 uint64_t *txclkrate,
                                                 uint64_t *rxclkpresent,
                                                 uint64_t *rxclkrate,
                                                 uint64_t *sigok,
                                                 uint64_t *rxprbserrcnt) {
  uint64_t reg64;

  port_mgr_tof2_umac4_sdsts_get(dev_id, umac4, ch, &reg64);
  umac4_sdsts0__sdsts__txclkpresent_rd(
      dev_id, umac4, ch, &reg64, txclkpresent, false);
  umac4_sdsts0__sdsts__txclkrate_rd(
      dev_id, umac4, ch, &reg64, txclkrate, false);
  umac4_sdsts0__sdsts__rxclkpresent_rd(
      dev_id, umac4, ch, &reg64, rxclkpresent, false);
  umac4_sdsts0__sdsts__rxclkrate_rd(
      dev_id, umac4, ch, &reg64, rxclkrate, false);
  umac4_sdsts0__sdsts__sigok_rd(dev_id, umac4, ch, &reg64, sigok, false);
  umac4_sdsts0__sdsts__rxprbserrcnt_rd(
      dev_id, umac4, ch, &reg64, rxprbserrcnt, false);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_eth400g_mac_txff_ctrl_get
 *
 * Read txff_ctrl register fields. Used by CLI to get port configuration details
 * for chcfg command
 ****************************************************************************/
bf_status_t port_mgr_tof2_eth400g_mac_txff_ctrl_get(bf_dev_id_t dev_id,
                                                    uint32_t umac4,
                                                    uint32_t ch,
                                                    uint32_t *reg32) {
  uint32_t unused_fld32;

  eth400g_mac_rspec_txff_ctrl_tx_flush_get(
      dev_id, umac4, ch, reg32, &unused_fld32, true);

  (void)unused_fld32;
  return BF_SUCCESS;
}

bf_status_t port_mgr_tof2_eth400g_mac_txff_ctrl_detail_get(
    bf_dev_id_t dev_id,
    uint32_t umac4,
    uint32_t ch,
    uint32_t *chnl_ena,
    uint32_t *tx_flush,
    uint32_t *chnl_mode,
    uint32_t *rx_xoff_mode,
    uint32_t *ovr_rx_pfcxoff,
    uint32_t *txrx_lpbk,
    uint32_t *val_rx_pfcxoff,
    uint32_t *cred_ini,
    uint32_t *min_thr) {
  uint32_t _reg32, *reg32 = &_reg32, fld;

  eth400g_mac_rspec_txff_ctrl_chnl_ena_get(
      dev_id, umac4, ch, reg32, &fld, true);
  *chnl_ena = fld;
  eth400g_mac_rspec_txff_ctrl_tx_flush_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *tx_flush = fld;
  eth400g_mac_rspec_txff_ctrl_chnl_mode_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *chnl_mode = fld;
  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *rx_xoff_mode = fld;
  eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *ovr_rx_pfcxoff = fld;
  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *txrx_lpbk = fld;
  eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *val_rx_pfcxoff = fld;
  eth400g_mac_rspec_txff_ctrl_cred_ini_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *cred_ini = fld;
  eth400g_mac_rspec_txff_ctrl_min_thr_get(
      dev_id, umac4, ch, reg32, &fld, false);
  *min_thr = fld;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_eth400g_mac_chnl_seq_get
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_eth400g_mac_chnl_seq_get(bf_dev_id_t dev_id,
                                                   uint32_t umac4,
                                                   uint32_t ch,
                                                   uint32_t *reg32) {
  uint32_t unused_fld32;

  eth400g_mac_rspec_chnl_seq_chnl_seq_get(
      dev_id, umac4, reg32, &unused_fld32, true);

  (void)ch;
  (void)unused_fld32;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_eth400g_serdes_mode_get
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_eth400g_serdes_mode_get(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch,
                                                  uint32_t *tx_sds_mode,
                                                  bool *tx_is_pam4,
                                                  uint32_t *tx_phys_ln,
                                                  uint32_t *rx_sds_mode,
                                                  bool *rx_is_pam4,
                                                  uint32_t *rx_phys_ln) {
  uint32_t tx, rx, unused_fld32;

  eth400g_pcs_rspec_txsds_mode_mode_get(
      dev_id, umac4, &tx, &unused_fld32, true);

  eth400g_pcs_rspec_rxsds_mode_mode_get(
      dev_id, umac4, &rx, &unused_fld32, true);

  *tx_sds_mode = tx;
  *tx_is_pam4 = ((tx >> (4 * ch)) & 0x8) ? true : false;
  *tx_phys_ln = ((tx >> (4 * ch)) & 0x7);

  *rx_sds_mode = rx;
  *rx_is_pam4 = ((rx >> (4 * ch)) & 0x8) ? true : false;
  *rx_phys_ln = ((rx >> (4 * ch)) & 0x7);

  (void)unused_fld32;
  return BF_SUCCESS;
}

#undef UMAC4_RMON_CTR
#define UMAC4_RMON_CTR(id) umac4_##id,

enum {
#include "umac4_rmon.h"
} umac4_rmon_counter_enum;

#undef UMAC4_RMON_CTR

uint32_t rmon_enum_xlat[] = {
    umac4_FramesRcvdOK,        // bf_mac_stat_FramesReceivedOK = 0,
    umac4_FramesRcvdAll,       // bf_mac_stat_FramesReceivedAll,
    umac4_FramesRcvdCRCError,  // bf_mac_stat_FramesReceivedwithFCSError,
    umac4_FramesRcvdError,     // bf_mac_stat_FrameswithanyError,
    umac4_OctetsRcvdOK,        // bf_mac_stat_OctetsReceivedinGoodFrames,
    umac4_OctetsRcvdAll,       // bf_mac_stat_OctetsReceived,
    umac4_FramesRcvdUnicast,  // bf_mac_stat_FramesReceivedwithUnicastAddresses,
    umac4_FramesRcvdMulticast,  // bf_mac_stat_FramesReceivedwithMulticastAddresses,
    umac4_FramesRcvdBroadcast,  // bf_mac_stat_FramesReceivedwithBroadcastAddresses,
    umac4_FramesRcvdPause,      // bf_mac_stat_FramesReceivedoftypePAUSE,
    umac4_FramesRcvdLenError,     // bf_mac_stat_FramesReceivedwithLengthError,
    umac4_Reserved,               // bf_mac_stat_FramesReceivedUndersized,
    umac4_FramesRcvdOversized,    // bf_mac_stat_FramesReceivedOversized,
    umac4_FramesRcvdFragments,    // bf_mac_stat_FragmentsReceived,
    umac4_FramesRcvdJabber,       // bf_mac_stat_JabberReceived,
    umac4_FramesRcvdPriPause,     // bf_mac_stat_PriorityPauseFrames,
    umac4_FramesRcvdCrcErrStomp,  // bf_mac_stat_CRCErrorStomped,
    umac4_FramesRcvdMaxFrmSizeVio,  // bf_mac_stat_FrameTooLong,
    umac4_FramesRcvdVLAN,           // bf_mac_stat_RxVLANFramesGood,
    umac4_FramesDropped,            // bf_mac_stat_FramesDroppedBufferFull,
    umac4_FramesRcvdSizeLT64,       // bf_mac_stat_FramesReceivedLength_lt_64,
    umac4_FramesRcvdSizeEQ64,       // bf_mac_stat_FramesReceivedLength_eq_64,
    umac4_FramesRcvdSize65to127,    // bf_mac_stat_FramesReceivedLength_65_127,
    umac4_FramesRcvdSize128to255,   // bf_mac_stat_FramesReceivedLength_128_255,
    umac4_FramesRcvdSize256to511,   // bf_mac_stat_FramesReceivedLength_256_511,
    umac4_FramesRcvdSize512to1023,  // bf_mac_stat_FramesReceivedLength_512_1023,
    umac4_FramesRcvdSize1024to1518,  // bf_mac_stat_FramesReceivedLength_1024_1518,
    umac4_FramesRcvdSize1519to2047,  // bf_mac_stat_FramesReceivedLength_1519_2047,
    umac4_FramesRcvdSize2048to4095,  // bf_mac_stat_FramesReceivedLength_2048_4095,
    umac4_FramesRcvdSize4096to8191,  // bf_mac_stat_FramesReceivedLength_4096_8191,
    umac4_FramesRcvdSize8192to9215,  // bf_mac_stat_FramesReceivedLength_8192_9215,
    umac4_FramesRcvdSizeGT9216,      // bf_mac_stat_FramesReceivedLength_9216,
    umac4_FramesXmitOK,              // bf_mac_stat_FramesTransmittedOK,
    umac4_FramesXmitAll,             // bf_mac_stat_FramesTransmittedAll,
    umac4_FramesXmitError,           // bf_mac_stat_FramesTransmittedwithError,
    umac4_OctetsXmitOK,           // bf_mac_stat_OctetsTransmittedwithouterror,
    umac4_OctetsXmitAll,          // bf_mac_stat_OctetsTransmittedTotal,
    umac4_FramesXmitUnicast,      // bf_mac_stat_FramesTransmittedUnicast,
    umac4_FramesXmitMulticast,    // bf_mac_stat_FramesTransmittedMulticast,
    umac4_FramesXmitBroadcast,    // bf_mac_stat_FramesTransmittedBroadcast,
    umac4_FramesXmitPause,        // bf_mac_stat_FramesTransmittedPause,
    umac4_FramesXmitPriPause,     // bf_mac_stat_FramesTransmittedPriPause,
    umac4_FramesXmitVLAN,         // bf_mac_stat_FramesTransmittedVLAN,
    umac4_FramesXmitSizeLT64,     // bf_mac_stat_FramesTransmittedLength_lt_64,
    umac4_FramesXmitSizeEQ64,     // bf_mac_stat_FramesTransmittedLength_eq_64,
    umac4_FramesXmitSize65to127,  // bf_mac_stat_FramesTransmittedLength_65_127,
    umac4_FramesXmitSize128to255,  // bf_mac_stat_FramesTransmittedLength_128_255,
    umac4_FramesXmitSize256to511,  // bf_mac_stat_FramesTransmittedLength_256_511,
    umac4_FramesXmitSize512to1023,  // bf_mac_stat_FramesTransmittedLength_512_1023,
    umac4_FramesXmitSize1024to1518,  // bf_mac_stat_FramesTransmittedLength_1024_1518,
    umac4_FramesXmitSize1519to2047,  // bf_mac_stat_FramesTransmittedLength_1519_2047,
    umac4_FramesXmitSize2048to4095,  // bf_mac_stat_FramesTransmittedLength_2048_4095,
    umac4_FramesXmitSize4096to8191,  // bf_mac_stat_FramesTransmittedLength_4096_8191,
    umac4_FramesSmitSize8192to9215,  // bf_mac_stat_FramesTransmittedLength_8192_9215,
    umac4_FramesXmitSizeGT9216,  // bf_mac_stat_FramesTransmittedLength_9216,
    umac4_FramesXmitPri0,        // bf_mac_stat_Pri0FramesTransmitted,
    umac4_FramesXmitPri1,        // bf_mac_stat_Pri1FramesTransmitted,
    umac4_FramesXmitPri2,        // bf_mac_stat_Pri2FramesTransmitted,
    umac4_FramesXmitPri3,        // bf_mac_stat_Pri3FramesTransmitted,
    umac4_FramesXmitPri4,        // bf_mac_stat_Pri4FramesTransmitted,
    umac4_FramesXmitPri5,        // bf_mac_stat_Pri5FramesTransmitted,
    umac4_FramesXmitPri6,        // bf_mac_stat_Pri6FramesTransmitted,
    umac4_FramesXmitPri7,        // bf_mac_stat_Pri7FramesTransmitted,
    umac4_FramesRcvdPri0,        // bf_mac_stat_Pri0FramesReceived,
    umac4_FramesRcvdPri1,        // bf_mac_stat_Pri1FramesReceived,
    umac4_FramesRcvdPri2,        // bf_mac_stat_Pri2FramesReceived,
    umac4_FramesRcvdPri3,        // bf_mac_stat_Pri3FramesReceived,
    umac4_FramesRcvdPri4,        // bf_mac_stat_Pri4FramesReceived,
    umac4_FramesRcvdPri5,        // bf_mac_stat_Pri5FramesReceived,
    umac4_FramesRcvdPri6,        // bf_mac_stat_Pri6FramesReceived,
    umac4_FramesRcvdPri7,        // bf_mac_stat_Pri7FramesReceived,
    umac4_XmitPri0Pause1US,      // bf_mac_stat_TransmitPri0Pause1USCount,
    umac4_XmitPri1Pause1US,      // bf_mac_stat_TransmitPri1Pause1USCount,
    umac4_XmitPri2Pause1US,      // bf_mac_stat_TransmitPri2Pause1USCount,
    umac4_XmitPri3Pause1US,      // bf_mac_stat_TransmitPri3Pause1USCount,
    umac4_XmitPri4Pause1US,      // bf_mac_stat_TransmitPri4Pause1USCount,
    umac4_XmitPri5Pause1US,      // bf_mac_stat_TransmitPri5Pause1USCount,
    umac4_XmitPri6Pause1US,      // bf_mac_stat_TransmitPri6Pause1USCount,
    umac4_XmitPri7Pause1US,      // bf_mac_stat_TransmitPri7Pause1USCount,
    umac4_RcvdPri0Pause1US,      // bf_mac_stat_ReceivePri0Pause1USCount,
    umac4_RcvdPri1Pause1US,      // bf_mac_stat_ReceivePri1Pause1USCount,
    umac4_RcvdPri2Pause1US,      // bf_mac_stat_ReceivePri2Pause1USCount,
    umac4_RcvdPri3Pause1US,      // bf_mac_stat_ReceivePri3Pause1USCount,
    umac4_RcvdPri4Pause1US,      // bf_mac_stat_ReceivePri4Pause1USCount,
    umac4_RcvdPri5Pause1US,      // bf_mac_stat_ReceivePri5Pause1USCount,
    umac4_RcvdPri6Pause1US,      // bf_mac_stat_ReceivePri6Pause1USCount,
    umac4_RcvdPri7Pause1US,      // bf_mac_stat_ReceivePri7Pause1USCount,
    umac4_RcvdStdPause1US,       // bf_mac_stat_ReceiveStandardPause1USCount,
    umac4_FramesRcvdTrunc,       // bf_mac_stat_FramesTruncated,
};

/*****************************************************************************
 * umac4_xlate_umac3_to_umac4
 ****************************************************************************/
uint32_t umac4_xlate_umac3_to_umac4(bf_rmon_counter_t ctr_id) {
  return rmon_enum_xlat[ctr_id];
}

/********************************************************************
 * umac4_to_umac3_ctr_copy
 * Copy UMAC4 counters in one buffer  to the corresponding UMAC3
 * counter entry in another buffer.
 ********************************************************************/
bf_status_t port_mgr_tof2_umac4_to_umac3_ctr_copy(uint64_t *umac4_ctr_array,
                                                  uint64_t *umac3_ctr_array) {
  uint32_t umac3_ctr_id;

  for (umac3_ctr_id = 0; umac3_ctr_id < BF_NUM_RMON_COUNTERS; umac3_ctr_id++) {
    uint32_t umac4_ctr_id = umac4_xlate_umac3_to_umac4(umac3_ctr_id);
    umac3_ctr_array[umac3_ctr_id] = umac4_ctr_array[umac4_ctr_id];
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_read_counter
 *
 * Sync read of UMAC4 RMON counter
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_read_counter(bf_dev_id_t dev_id,
                                             uint32_t umac4,
                                             uint32_t ch,
                                             bf_rmon_counter_t ctr_id,
                                             uint64_t *ctr_value) {
  uint32_t umac4_ctr_id;

  // translate ctr_id from umac3 to umac4
  umac4_ctr_id = umac4_xlate_umac3_to_umac4(ctr_id);

  // read the counter
  *ctr_value = umac4_ctrs_rmon_ctr_get(dev_id, umac4, ch, umac4_ctr_id);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_clear_counter
 *
 * Sync read of UMAC4 RMON counter
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_clear_counter(bf_dev_id_t dev_id,
                                              uint32_t umac4,
                                              uint32_t ch) {
  uint64_t clrmode, clr = 1;

  umac4_maccfg0__maccfg__statsclr_rmw(dev_id, umac4, ch, &clrmode, clr);
  clr = 0;
  umac4_maccfg0__maccfg__statsclr_rmw(dev_id, umac4, ch, &clrmode, clr);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sds_mode_enc_mode_set
 *
 * Set the encoding mode for a logical lane
 * 0=NRZ, 1=PAM4
 ****************************************************************************/
// static
bf_status_t port_mgr_tof2_umac4_sds_mode_enc_mode_set(uint32_t ch,
                                                      uint32_t enc_mode,
                                                      uint32_t cfg_wd,
                                                      uint32_t *new_cfg_wd) {
  uint32_t msk = (0x8 << (4 * ch));
  uint32_t wd;

  bf_sys_assert((enc_mode == 0) || (enc_mode == 1));

  wd = cfg_wd & ~msk;  // mask off current value
  wd |= ((enc_mode << 3) << (4 * ch));
  *new_cfg_wd = wd;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_sds_mode_map_set
 *
 * Set the logicla to physical map for a logical lane
 *
 ****************************************************************************/
// static
bf_status_t port_mgr_tof2_umac4_sds_mode_map_set(uint32_t ch,
                                                 uint32_t phy_lane,
                                                 uint32_t cfg_wd,
                                                 uint32_t *new_cfg_wd) {
  uint32_t msk = (0x7 << (4 * ch));
  uint32_t wd;

  wd = cfg_wd & ~msk;  // mask off current value
  wd |= (phy_lane << (4 * ch));
  *new_cfg_wd = wd;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_lane_map_set
 *
 * Program the lane remapping
 *
 * Also, program the rxsigok polling s/m here as it requires the physical
 * mapping to the bits in the tile register.
 *
 * Configure iotile serdes sigok polling on all macs
 * Make sure not to force sigok on mac_pcs ETH_RXSIGOK_CTRL register.
 *
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_POLL_CTRL_ADDRESS, 'h080060);
 *       if (in_tile_1_3(mac_id)) begin
 *          `wr_pcs(`ETH400G_PCS_RSPEC_ETH_RXSIGOK_BITSEL_ADDRESS, 'h76543210);
 *       end
 *       else begin
 *          `wr_pcs(`ETH400G_PCS_RSPEC_ETH_RXSIGOK_BITSEL_ADDRESS, 'h01234567);
 *       end
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_POLL_TIME_ADDRESS, 'h10100); //
 *enable, 256 cycles
 ****************************************************************************/
void port_mgr_tof2_umac4_lane_map_set(bf_dev_id_t dev_id,
                                      uint32_t umac4,
                                      uint32_t phys_tx_ln[8],
                                      uint32_t phys_rx_ln[8]) {
  uint32_t sds_mode = 0, unused_32b_fld, reg32, bitsel;
  uint32_t ch;
  uint32_t use_sig_ok, use_phy_ready, bit_to_use;
  uint32_t sd_mode_56g = 0x88888888;
  (void)use_sig_ok;
  (void)use_phy_ready;

  // read current Tx sds_mode
  eth400g_pcs_rspec_txsds_mode_mode_get(
      dev_id, umac4, &sds_mode, &unused_32b_fld, true);
  // test: OR in PAM4 mode to prevent clock glitch from corrupting UMAC
  //      logic when serdes TX_MODEx is first programmed
  sds_mode |= sd_mode_56g;

  for (ch = 0; ch < 8; ch++) {
    port_mgr_tof2_umac4_sds_mode_map_set(
        ch, phys_tx_ln[ch], sds_mode, &sds_mode);
  }
  eth400g_pcs_rspec_txsds_mode_mode_set(
      dev_id, umac4, &sds_mode, sds_mode, true);

  // read current Rx sds_mode
  eth400g_pcs_rspec_rxsds_mode_mode_get(
      dev_id, umac4, &sds_mode, &unused_32b_fld, true);
  // test: OR in PAM4 mode to prevent clock glitch from corrupting UMAC
  //      logic when serdes TX_MODEx is first programmed
  sds_mode |= sd_mode_56g;
  for (ch = 0; ch < 8; ch++) {
    port_mgr_tof2_umac4_sds_mode_map_set(
        ch, phys_rx_ln[ch], sds_mode, &sds_mode);
  }
  eth400g_pcs_rspec_rxsds_mode_mode_set(
      dev_id, umac4, &sds_mode, sds_mode, true);

  /***************************************************************************
   * Note:
   * The Credo register is defined as follows:
   * raw status bits set 0.
   *    Bit 7:0:  L_SIG_DETECT[7:0]
   *    Bit 15:8: L_R_PHY_READY[7:0]
   *
   * We'll try to use the phy_ready bits since they provide a better indication
   *of
   * a valid signal. If this causes problems, delete the "8 +" from the value
   * programmed into each field below and then the sig_detect bit will be used
   * instead.
   */
  bitsel = 0;
  use_sig_ok = 0;
  use_phy_ready = 8;
  bit_to_use = use_phy_ready;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set(
      dev_id, umac4, &bitsel, bit_to_use + 0, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set(
      dev_id, umac4, &bitsel, bit_to_use + 1, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set(
      dev_id, umac4, &bitsel, bit_to_use + 2, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set(
      dev_id, umac4, &bitsel, bit_to_use + 3, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_set(
      dev_id, umac4, &bitsel, bit_to_use + 4, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_set(
      dev_id, umac4, &bitsel, bit_to_use + 5, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_set(
      dev_id, umac4, &bitsel, bit_to_use + 6, false);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_set(
      dev_id, umac4, &bitsel, bit_to_use + 7, true);  // write to hw
  eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_rmw(
      dev_id, umac4, &reg32, 0x80060);
  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_rmw(
      dev_id, umac4, &reg32, 0x1000);
  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_rmw(
      dev_id, umac4, &reg32, 0x1);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_force_sigok_low_set
 *
 * Force sigok indication to PCS to be "0" for a set of lanes. This holds
 * the Rx state-machine in reset. This is cleared only after the serdes has
 * completed Rx equalization (DFE). This prevents the PCS from acting on
 * noise.
 ****************************************************************************/
void port_mgr_tof2_umac4_force_sigok_low_set(bf_dev_id_t dev_id,
                                             uint32_t umac4,
                                             uint32_t ch,
                                             uint32_t n_lanes) {
  uint32_t rxsigok_ctrl, rxsigok_val;
  uint32_t rxsigok_msk = ((0xFF >> (8 - n_lanes)) << ch);

  // on real HW, force sigok low until after DFE
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  rxsigok_val |= rxsigok_msk;
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set(
      dev_id, umac4, &rxsigok_ctrl, rxsigok_val, true);

  // clear any force hi setting
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  rxsigok_val &= ~rxsigok_msk;
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set(
      dev_id, umac4, &rxsigok_ctrl, rxsigok_val, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_force_sigok_hi_set
 *
 * Force sigok indication to PCS to be "1" for a set of lanes. This
 * is required on the emulator which does not support serdes.
 ****************************************************************************/
void port_mgr_tof2_umac4_force_sigok_hi_set(bf_dev_id_t dev_id,
                                            uint32_t umac4,
                                            uint32_t ch,
                                            uint32_t n_lanes) {
  uint32_t rxsigok_ctrl, rxsigok_val;
  uint32_t rxsigok_msk = ((0xFF >> (8 - n_lanes)) << ch);

  // clear any "force low" setting
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  rxsigok_val &= ~rxsigok_msk;
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set(
      dev_id, umac4, &rxsigok_ctrl, rxsigok_val, true);

  // force indication to be "ok"
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  rxsigok_val |= rxsigok_msk;
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set(
      dev_id, umac4, &rxsigok_ctrl, rxsigok_val, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_clear_forced_sigok_set
 *
 * Clear all forced indications, hi or low, to allow the real serdes
 * rxsigok indication to be used.
 ****************************************************************************/
void port_mgr_tof2_umac4_clear_forced_sigok_set(bf_dev_id_t dev_id,
                                                uint32_t umac4,
                                                uint32_t ch,
                                                uint32_t n_lanes) {
  uint32_t rxsigok_ctrl, rxsigok_val;
  uint32_t rxsigok_msk = ((0xFF >> (8 - n_lanes)) << ch);

  // clear any "force low" setting
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  rxsigok_val &= ~rxsigok_msk;
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set(
      dev_id, umac4, &rxsigok_ctrl, rxsigok_val, true);

  // clear any "force hi" setting
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  rxsigok_val &= ~rxsigok_msk;
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set(
      dev_id, umac4, &rxsigok_ctrl, rxsigok_val, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_forced_sigok_get
 *
 * return forced indications, hi and low, settings
 ****************************************************************************/
void port_mgr_tof2_umac4_forced_sigok_get(bf_dev_id_t dev_id,
                                          uint32_t umac4,
                                          uint32_t ch,
                                          uint32_t n_lanes,
                                          uint32_t *force_hi_raw_val,
                                          uint32_t *force_lo_raw_val,
                                          uint32_t *force_hi,
                                          uint32_t *force_lo) {
  uint32_t rxsigok_ctrl, rxsigok_val;
  uint32_t rxsigok_msk = ((0xFF >> (8 - n_lanes)) << ch);

  // "force low" setting
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  *force_lo_raw_val = rxsigok_ctrl;
  *force_lo = (rxsigok_val & rxsigok_msk) >> ch;

  // "force hi" setting
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get(
      dev_id, umac4, &rxsigok_ctrl, &rxsigok_val, true);
  *force_hi_raw_val = rxsigok_ctrl;
  *force_hi = (rxsigok_val & rxsigok_msk) >> ch;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_loopback_mac_near_set
 ****************************************************************************/
static void port_mgr_tof2_umac4_loopback_mac_near_set(bf_dev_id_t dev_id,
                                                      uint32_t umac4,
                                                      uint32_t ch,
                                                      bool en) {
  uint64_t reg64, fld64 = en ? 1 : 0;

  umac4_chmode0__chmode__gmiilpbk_rmw(dev_id, umac4, ch, &reg64, fld64);
  umac4_maccfg0__maccfg__txignorerx_rmw(dev_id, umac4, ch, &reg64, fld64);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_loopback_pcs_set
 ****************************************************************************/
static void port_mgr_tof2_umac4_loopback_pcs_set(bf_dev_id_t dev_id,
                                                 uint32_t umac4,
                                                 uint32_t ch,
                                                 bool en) {
  uint64_t reg64, fld64 = en ? 1 : 0;
  uint32_t ln, n_lanes;
  bf_serdes_encoding_mode_t enc_mode;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  lld_err_t err;

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac4, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  n_lanes = port_mgr_tof2_umac_channel_get(dev_id, umac4, ch);
  bf_serdes_encoding_mode_get(port_p->sw.speed, n_lanes, &enc_mode);
  if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
    // two PCS lanes per CH
    for (ln = 0; ln < n_lanes; ln++) {
      umac4_sdcfg0__sdcfg__serdeslpbk_rmw(
          dev_id, umac4, (ch + ln) * 2, &reg64, fld64);
      umac4_sdcfg0__sdcfg__sigokoverride_rmw(
          dev_id, umac4, (ch + ln) * 2, &reg64, en ? 3 : 0);
      umac4_sdcfg0__sdcfg__serdeslpbk_rmw(
          dev_id, umac4, ((ch + ln) * 2) + 1, &reg64, fld64);
      umac4_sdcfg0__sdcfg__sigokoverride_rmw(
          dev_id, umac4, ((ch + ln) * 2) + 1, &reg64, en ? 3 : 0);
    }
  } else {
    // one PCS lane per CH, but by twos, 0, 2, 4, ..
    for (ln = 0; ln < n_lanes; ln++) {
      umac4_sdcfg0__sdcfg__serdeslpbk_rmw(
          dev_id, umac4, (ch + ln) * 2, &reg64, fld64);
      umac4_sdcfg0__sdcfg__sigokoverride_rmw(
          dev_id, umac4, (ch + ln) * 2, &reg64, en ? 3 : 0);
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac4_loopback_min_latency_set
 ****************************************************************************/
static void port_mgr_tof2_umac4_loopback_min_latency_set(bf_dev_id_t dev_id,
                                                         uint32_t umac4,
                                                         uint32_t ch,
                                                         bool en) {
  uint32_t reg32, fld32 = en ? 1 : 0;
  uint32_t chnl_ena = 0;

  eth400g_mac_rspec_txff_ctrl_chnl_ena_get(
      dev_id, umac4, ch, &reg32, &chnl_ena, true);
  if (chnl_ena) {
    // lpbk needs to be set before chnl_ena goes from 0->1, so set chnl_ena=0
    // first
    // So, if chnl_ena is already set, de-assert it and re-assert afterwards
    // Otherwise, port is disabled and chnl_ena will go from 0 -> 1 on enable
    eth400g_mac_rspec_txff_ctrl_chnl_ena_rmw(dev_id, umac4, ch, &reg32, 0);
  }
  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_rmw(dev_id, umac4, ch, &reg32, fld32);
  if (chnl_ena) {
    // then renable the chnl
    eth400g_mac_rspec_txff_ctrl_chnl_ena_rmw(dev_id, umac4, ch, &reg32, 1);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac4_loopback_set
 *
 * Configure (or un-configure) one of the several loopback modes
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_loopback_set(bf_dev_id_t dev_id,
                                      uint32_t umac4,
                                      uint32_t ch,
                                      bf_loopback_mode_e mode) {
  // assert soft reset while changing modes
  port_mgr_tof2_umac4_sw_reset_set(dev_id, umac4, ch, true);

  if (mode == BF_LPBK_NONE) {
    // unconfigure all modes
    port_mgr_tof2_umac4_loopback_mac_near_set(dev_id, umac4, ch, false);
    port_mgr_tof2_umac4_loopback_pcs_set(dev_id, umac4, ch, false);
    port_mgr_tof2_umac4_loopback_min_latency_set(dev_id, umac4, ch, false);
  } else if (mode == BF_LPBK_MAC_NEAR) {
    port_mgr_tof2_umac4_loopback_mac_near_set(dev_id, umac4, ch, true);
  } else if (mode == BF_LPBK_MAC_FAR) {
    // not suported in tof2
  } else if (mode == BF_LPBK_PCS_NEAR) {
    port_mgr_tof2_umac4_loopback_pcs_set(dev_id, umac4, ch, true);
  } else if (mode == BF_LPBK_PIPE) {
    port_mgr_tof2_umac4_loopback_min_latency_set(dev_id, umac4, ch, true);
  }
  // release soft reset after changing modes
  port_mgr_tof2_umac4_sw_reset_set(dev_id, umac4, ch, false);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_txdrain_set
 ****************************************************************************/
void port_mgr_tof2_umac4_txdrain_set(bf_dev_id_t dev_id,
                                     uint32_t umac4,
                                     uint32_t ch,
                                     bool en) {
  uint64_t reg64, fld64 = en ? 1 : 0;

  umac4_chmode0__chmode__txdrain_rmw(dev_id, umac4, ch, &reg64, fld64);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_txdrain_get
 ****************************************************************************/
void port_mgr_tof2_umac4_txdrain_get(bf_dev_id_t dev_id,
                                     uint32_t umac4,
                                     uint32_t ch,
                                     bool *en) {
  uint64_t reg64, fld64;

  umac4_chmode0__chmode__txdrain_rd(dev_id, umac4, ch, &reg64, &fld64, true);

  *en = fld64 ? true : false;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_init
 *
 * Apply one-time UMAC4 configurations
 *
 * Reset and enable mdioci controller on all macs
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_CTRL_ADDRESS,
 *            {mdioci_clk_div[3:0], 2'b0, 1'b0, 1'b1}); // ~reset + en
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_CTRL_ADDRESS,
 *               {mdioci_clk_div[3:0], 2'b0, 1'b1, 1'b1}); // reset + en
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_CTRL_ADDRESS,
 *               {mdioci_clk_div[3:0], 2'b0, 1'b0, 1'b1}); // ~reset + en
 ****************************************************************************/
void port_mgr_tof2_umac4_init(bf_dev_id_t dev_id,
                              uint32_t umac4,
                              uint32_t clk_div) {
  uint32_t reg32, soft_reset = 0;

  port_mgr_log("UMAC4 mdioci init");
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_rmw(
      dev_id, umac4, &reg32, clk_div);
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_rmw(dev_id, umac4, &reg32, 1);

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_rmw(dev_id, umac4, &reg32, 0);
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_rmw(dev_id, umac4, &reg32, 1);
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_rmw(dev_id, umac4, &reg32, 0);

  port_mgr_log("UMAC4 soft reset");
  // this is a MAC-wide operation. it should only be done once
  eth400g_mac_rspec_eth_soft_reset_eth_swrst_rmw(dev_id, umac4, &soft_reset, 0);

  // TF2LAB-82, preset rxswrst=0, rx_en=0
  for (int ch = 0; ch < 8; ch++) {
    uint64_t chmode0;  // unused
    port_mgr_tof2_umac4_sw_reset_rx_set(dev_id, umac4, ch, false);
    umac4_chmode0__chmode__rxen_rmw(dev_id, umac4, ch, &chmode0, 0x0ull);
  }
  port_mgr_log("UMAC4 init done");
}

/*****************************************************************************
 * port_mgr_tof2_umac4_rs_fec_status_and_counters_get
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_rs_fec_status_and_counters_get(
    bf_dev_id_t dev_id,
    uint32_t umac4,
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
  umac4_rs_fec_ctr_t ctr;
  umac4_rs_fec_ln_ctr_t ln_ctr;

  umac4_ctrs_rs_fec_get(dev_id, umac4, ch, &ctr);
  *fec_corr_cnt = ctr.RSFEC_Correctable_CodeWords;
  *fec_uncorr_cnt = ctr.RSFEC_Uncorrectable_CodeWords;
  *fec_align_status = 0;
  *hi_ser = 0;

  umac4_ctrs_rs_fec_ln_get(dev_id, umac4, &ln_ctr);
  *fec_ser_lane_0 = ln_ctr.RSFEC_SERDES_0_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_1_Chx_Symbol_Errors;
  *fec_ser_lane_1 = ln_ctr.RSFEC_SERDES_2_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_3_Chx_Symbol_Errors;
  *fec_ser_lane_2 = ln_ctr.RSFEC_SERDES_4_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_5_Chx_Symbol_Errors;
  *fec_ser_lane_3 = ln_ctr.RSFEC_SERDES_6_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_7_Chx_Symbol_Errors;
  *fec_ser_lane_4 = ln_ctr.RSFEC_SERDES_8_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_9_Chx_Symbol_Errors;
  *fec_ser_lane_5 = ln_ctr.RSFEC_SERDES_10_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_11_Chx_Symbol_Errors;
  *fec_ser_lane_6 = ln_ctr.RSFEC_SERDES_12_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_13_Chx_Symbol_Errors;
  *fec_ser_lane_7 = ln_ctr.RSFEC_SERDES_14_Chx_Symbol_Errors +
                    ln_ctr.RSFEC_SERDES_15_Chx_Symbol_Errors;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_int_en_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_int_en_set(bf_dev_id_t dev_id,
                                    uint32_t umac4,
                                    bool on) {
  uint32_t reg32;

  eth400g_mac_rspec_mac_en0_mac_rmw(dev_id, umac4, &reg32, (on ? 1 : 0));
}

/*****************************************************************************
 * port_mgr_tof2_umac4_ch_int_en_all_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_ch_int_en_all_set(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch) {
  uint64_t reg64;

  umac4_intcontrol0__intcontrol__intena_rmw(
      dev_id, umac4, ch, &reg64, 0xffffull);
}
/*****************************************************************************
 * port_mgr_tof2_umac4_ch_int_dis_all_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_ch_int_dis_all_set(bf_dev_id_t dev_id,
                                            uint32_t umac4,
                                            uint32_t ch) {
  uint64_t reg64;

  umac4_intcontrol0__intcontrol__intena_rmw(dev_id, umac4, ch, &reg64, 0x0ull);
}

/*****************************************************************************
 *port_mgr_tof2_umac4_local_fault_int_en_set
 *
 * Note: UMAC4 does not support separate interrupts for local and remote
 * faults, so enabling either enables the one "Rx Fault" interrupt bit.
 ****************************************************************************/
void port_mgr_tof2_umac4_local_fault_int_en_set(bf_dev_id_t dev_id,
                                                uint32_t umac4,
                                                uint32_t ch,
                                                bool en) {
  uint64_t reg64, fld64, fld64_en = 0x4100ull;

  if (en) {  // if enabling, clear before
    umac4_intcontrol0__intcontrol__intclr_rmw(
        dev_id, umac4, ch, &reg64, fld64_en);
  }

  umac4_intcontrol0__intcontrol__intena_rd(
      dev_id, umac4, ch, &reg64, &fld64, true);
  if (en) {
    fld64 |= fld64_en;
  } else {
    fld64 &= ~fld64_en;
  }
  umac4_intcontrol0__intcontrol__intena_wr(
      dev_id, umac4, ch, &reg64, fld64, true);

  if (!en) {  // if disabling, clear after
    umac4_intcontrol0__intcontrol__intclr_rmw(
        dev_id, umac4, ch, &reg64, fld64_en);
  }
}

/*****************************************************************************
 *port_mgr_tof2_umac4_link_gain_int_en_set
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_link_gain_int_en_set(bf_dev_id_t dev_id,
                                              uint32_t umac4,
                                              uint32_t ch,
                                              bool en) {
  uint64_t reg64, fld64, fld64_en = 0x8000ull;

  if (en) {  // if enabling, clear before
    umac4_intcontrol0__intcontrol__intclr_rmw(
        dev_id, umac4, ch, &reg64, fld64_en);
  }

  umac4_intcontrol0__intcontrol__intena_rd(
      dev_id, umac4, ch, &reg64, &fld64, true);
  if (en) {
    fld64 |= fld64_en;
  } else {
    fld64 &= ~fld64_en;
  }
  umac4_intcontrol0__intcontrol__intena_wr(
      dev_id, umac4, ch, &reg64, fld64, true);

  if (!en) {  // if disabling, clear after
    umac4_intcontrol0__intcontrol__intclr_rmw(
        dev_id, umac4, ch, &reg64, fld64_en);
  }
}

/*****************************************************************************
 *port_mgr_tof2_umac4_remote_fault_int_en_set
 *
 *
 * Note: UMAC4 does not support separate interrupts for local and remote
 * faults, so enabling either enables the one "Rx Fault" interrupt bit.
 ****************************************************************************/
void port_mgr_tof2_umac4_remote_fault_int_en_set(bf_dev_id_t dev_id,
                                                 uint32_t umac4,
                                                 uint32_t ch,
                                                 bool en) {
  port_mgr_tof2_umac4_local_fault_int_en_set(dev_id, umac4, ch, en);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_tx_local_fault_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_tx_local_fault_set(bf_dev_id_t dev_id,
                                            uint32_t umac4,
                                            uint32_t ch,
                                            bool on) {
  uint64_t reg64;
  uint64_t fld64;

  umac4_maccfg0__maccfg__txlfault_rd(dev_id, umac4, ch, &reg64, &fld64, true);
  umac4_maccfg0__maccfg__txrfault_wr(dev_id, umac4, ch, &reg64, 0ull, false);
  umac4_maccfg0__maccfg__txidle_wr(dev_id, umac4, ch, &reg64, 0ull, false);
  umac4_maccfg0__maccfg__txlfault_wr(
      dev_id, umac4, ch, &reg64, (on ? 1ull : 0ull), true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_tx_remote_fault_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_tx_remote_fault_set(bf_dev_id_t dev_id,
                                             uint32_t umac4,
                                             uint32_t ch,
                                             bool on) {
  uint64_t reg64;
  uint64_t fld64;

  umac4_maccfg0__maccfg__txrfault_rd(dev_id, umac4, ch, &reg64, &fld64, true);
  umac4_maccfg0__maccfg__txlfault_wr(dev_id, umac4, ch, &reg64, 0ull, false);
  umac4_maccfg0__maccfg__txidle_wr(dev_id, umac4, ch, &reg64, 0ull, false);
  umac4_maccfg0__maccfg__txrfault_wr(
      dev_id, umac4, ch, &reg64, (on ? 1ull : 0ull), true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_tx_idle_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_tx_idle_set(bf_dev_id_t dev_id,
                                     uint32_t umac4,
                                     uint32_t ch,
                                     bool on) {
  uint64_t reg64;
  uint64_t fld64;

  umac4_maccfg0__maccfg__txidle_rd(dev_id, umac4, ch, &reg64, &fld64, true);
  umac4_maccfg0__maccfg__txlfault_wr(dev_id, umac4, ch, &reg64, 0ull, false);
  umac4_maccfg0__maccfg__txrfault_wr(dev_id, umac4, ch, &reg64, 0ull, false);
  umac4_maccfg0__maccfg__txidle_wr(
      dev_id, umac4, ch, &reg64, (on ? 1ull : 0ull), true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_tx_ignore_rx_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_tx_ignore_rx_set(bf_dev_id_t dev_id,
                                          uint32_t umac4,
                                          uint32_t ch,
                                          bool en) {
  uint64_t reg64, fld64 = en ? 1ull : 0ull;

  umac4_maccfg0__maccfg__txignorerx_rmw(dev_id, umac4, ch, &reg64, fld64);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_timestamp_offset_set
 *
 *
 ****************************************************************************/
static void port_mgr_tof2_umac4_timestamp_offset_set(bf_dev_id_t dev_id,
                                                     uint32_t umac4,
                                                     uint32_t ch,
                                                     bf_port_speed_t speed) {
  uint64_t reg64, ets_offset = 180;  // TODO - Value is not final.
  uint64_t txff_cnt_adj = 1024 * 512 / speed_enum_to_int(speed);

  // Program timestamp offset values for the channel.
  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_rmw(
      dev_id, umac4, ch, &reg64, ets_offset);
  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_rmw(
      dev_id, umac4, ch, &reg64, txff_cnt_adj);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_get_speed_from_timestamp_offset
 *
 *
 ****************************************************************************/
static void port_mgr_tof2_umac4_get_speed_from_timestamp_offset(
    bf_dev_id_t dev_id, uint32_t umac4, uint32_t ch, bf_port_speed_t *speed) {
  uint64_t reg64, speed_bits, txff_cnt_adj;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_get(
      dev_id, umac4, ch, &reg64, &txff_cnt_adj, true);
  speed_bits = 1024 * 512 / txff_cnt_adj;
  *speed = speed_int_to_enum(speed_bits);
}

void port_mgr_tof2_umac4_rx_enable_set(bf_dev_id_t dev_id,
                                       uint32_t umac4,
                                       uint32_t ch,
                                       bool rx_en) {
  uint64_t unused_fld, chmode0 = 0ull;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac4, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  // Read mode first (to preserve any reserved bits)
  umac4_chmode0__chmode__mode_rd(
      dev_id, umac4, ch, &chmode0, &unused_fld, true);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x1ull, true);
  if (rx_en) {
    umac4_chmode0__chmode__rxen_wr(dev_id, umac4, ch, &chmode0, 0x1ull, true);
  } else {
    umac4_chmode0__chmode__rxen_wr(dev_id, umac4, ch, &chmode0, 0x0ull, true);
  }
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac4, ch, &chmode0, 0x0ull, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac4_handle_interrupts
 *
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac4_handle_interrupts(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch,
                                                  bool *possible_state_chg) {
  uint64_t reg64, fld64;

  umac4_intcontrol0__intcontrol__intsts_rd(
      dev_id, umac4, ch, &reg64, &fld64, true);

  // write-1-to-clear what we read (only)
  umac4_intcontrol0__intcontrol__intclr_wr(
      dev_id, umac4, ch, &reg64, fld64, true);

  if (reg64 & (1ull << 0)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : TX Application FIFO Overflow, TX Packet "
        "Underflow, Tx Packing Overflow",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 1)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : TX Protocol Violation on SOF/EOF/VLD Input "
        "to Application FIFO",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 2)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : TX JABBER PKT      TX Packet Jabbered",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 3)) {
    port_mgr_log("%d: INT: umac%d : ch%d : TX serdes Fifo  overlfow/underflow",
                 dev_id,
                 umac4,
                 ch);
  }
  if (reg64 & (1ull << 4)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : RX PCS Gearbox Overflow", dev_id, umac4, ch);
  }
  if (reg64 & (1ull << 5)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : RX PCS Deskew Overflow", dev_id, umac4, ch);
  }
  if (reg64 & (1ull << 6)) {
    port_mgr_log("%d: INT: umac%d : ch%d : Rx PCS hiber, hiser error triggered",
                 dev_id,
                 umac4,
                 ch);
  }
  if (reg64 & (1ull << 7)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : RX PCS Sync Header Error, Block Error, "
        "Codeword Error or BIP Error",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 8)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : RX Fault (Ordered Set) Received (or "
        "Generated by RX PCS)",
        dev_id,
        umac4,
        ch);
    *possible_state_chg = true;
  }
  if (reg64 & (1ull << 9)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : Frame Dropped due to bad SFD/PREAMBLE",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 10)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : Invalid FCS on Received Packet or PCS Error "
        "character found inside of packet",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 11)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : Rx Packet Jabbered", dev_id, umac4, ch);
  }
  if (reg64 & (1ull << 12)) {
    port_mgr_log("%d: INT: umac%d : ch%d : Rx Runt Packet Filtered/Dropped",
                 dev_id,
                 umac4,
                 ch);
  }
  if (reg64 & (1ull << 13)) {
    port_mgr_log(
        "%d: INT: umac%d : ch%d : RX Application FIFO Overflow, RX unpacking "
        "Overflow",
        dev_id,
        umac4,
        ch);
  }
  if (reg64 & (1ull << 14)) {
    port_mgr_log("%d: INT: umac%d : ch%d : Link Lost", dev_id, umac4, ch);
    *possible_state_chg = true;
  }
  if (reg64 & (1ull << 15)) {
    port_mgr_log("%d: INT: umac%d : ch%d : Link Acquired", dev_id, umac4, ch);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac4_pcslcfg_get
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_pcslcfg_get(bf_dev_id_t dev_id,
                                     uint32_t umac4,
                                     uint32_t vl,
                                     uint64_t *amlock,
                                     uint64_t *blocklock,
                                     uint64_t *mapping,
                                     uint64_t *amperiod) {
  uint64_t reg64, fld64;

  umac4_pcslcfg0__pcslcfg__amlock_rd(dev_id, umac4, vl, &reg64, &fld64, true);
  *amlock = fld64;
  umac4_pcslcfg0__pcslcfg__blocklock_rd(
      dev_id, umac4, vl, &reg64, &fld64, false);
  *blocklock = fld64;
  umac4_pcslcfg0__pcslcfg__mapping_rd(dev_id, umac4, vl, &reg64, &fld64, false);
  *mapping = fld64;
  umac4_pcslcfg0__pcslcfg__amperiod_rd(
      dev_id, umac4, vl, &reg64, &fld64, false);
  *amperiod = fld64;
}

/****************************************************************************
 * port_mgr_tof2_umac4_get_1588_timestamp_tx
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_get_1588_timestamp_tx(bf_dev_id_t dev_id,
                                               uint32_t mac_blk,
                                               uint32_t ch,
                                               uint64_t *ts,
                                               bool *ts_valid,
                                               int *ts_id) {
  uint64_t data = 0;
  uint64_t reg = 0;

  if ((ts_valid == NULL) || (ts_id == NULL)) {
    return;
  }

  *ts_valid = false;
  eth400g_mac_rspec_cts_fifo_out_cts_get(
      dev_id, mac_blk, ch, &reg, &data, true);
  if (data & 0x8000000000000000ull) {
    *ts = (data & 0xFFFFFFFFFFFF);
    *ts_id = ((data >> 48) & 0x1FF);
    *ts_valid = true;
  }
  return;
}

/****************************************************************************
 * port_mgr_tof2_umac4_set_1588_timestamp_delta_tx
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_set_1588_timestamp_delta_tx(bf_dev_id_t dev_id,
                                                     uint32_t mac_blk,
                                                     uint16_t delta) {
  uint64_t data = delta;
  uint64_t reg;
  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set(
      dev_id, mac_blk, &reg, data, true);
  return;
}

/****************************************************************************
 * port_mgr_tof2_umac4_get_1588_timestamp_delta_tx
 *
 ****************************************************************************/
void port_mgr_tof2_umac4_get_1588_timestamp_delta_tx(bf_dev_id_t dev_id,
                                                     uint32_t mac_blk,
                                                     uint16_t *delta) {
  uint64_t data = 0;
  uint64_t reg;
  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get(
      dev_id, mac_blk, &reg, &data, true);
  *delta = data;
  return;
}
