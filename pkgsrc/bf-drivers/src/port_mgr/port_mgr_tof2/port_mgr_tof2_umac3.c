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
#include "port_mgr_tof2_umac3.h"
#include "port_mgr_tof2_microp.h"
#include "umac3c4_access.h"
#include "eth100g_reg_rspec_access.h"
#include "autogen-required-headers.h"

static uint32_t port_mgr_tof2_umac3_config_to_mode(bf_port_speed_t speed);
static void umac3_glbl__chmode__wr(bf_dev_id_t dev_id,
                                   uint32_t umac3,
                                   uint32_t ch,
                                   uint32_t *reg32,
                                   uint32_t speed_mode,
                                   uint32_t swreset,
                                   uint32_t chena);
/*static*/ void umac3_glbl__chmode__rd(bf_dev_id_t dev_id,
                                       uint32_t umac3,
                                       uint32_t ch,
                                       uint32_t *reg32,
                                       uint32_t *speed_mode,
                                       uint32_t *swreset,
                                       uint32_t *chena);
static void umac3_serdes_config_set(bf_dev_id_t dev_id,
                                    uint32_t umac3,
                                    uint32_t ch,
                                    uint32_t *serdes_config,
                                    uint32_t speed,
                                    uint32_t n_ch);
static void umac3_pcs_pma_and_fec_get(bf_port_speed_t speed,
                                      bf_fec_type_t fec,
                                      uint32_t *pma_mode,
                                      uint32_t *fec_mode);
static void umac3_glbl__chmode__swreset_rmw(bf_dev_id_t dev_id,
                                            uint32_t umac3,
                                            uint32_t ch,
                                            uint32_t *reg32,
                                            uint32_t swreset);
static void port_mgr_tof2_umac3_configure_fec(bf_dev_id_t dev_id,
                                              uint32_t umac3,
                                              uint32_t ch,
                                              bf_fec_types_t fec);
static void port_mgr_tof2_umac3_configure_appfifo(bf_dev_id_t dev_id,
                                                  uint32_t umac3,
                                                  uint32_t ch,
                                                  bf_port_speed_t speed);
static uint32_t port_mgr_tof2_umac3_txff_ctrl_cred_ini_calc(
    bf_port_speed_t speed);
static uint32_t port_mgr_tof2_umac3_txff_ctrl_chnl_mode_calc(
    bf_port_speed_t speed);
static uint32_t port_mgr_tof2_umac3_chnl_seq_calc(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch);
static void port_mgr_tof2_umac_channel_del(bf_dev_id_t dev_id,
                                           uint32_t umac,
                                           uint32_t ch,
                                           uint32_t n_ch);
static uint32_t port_mgr_tof2_umac_channel_get(bf_dev_id_t dev_id,
                                               uint32_t umac,
                                               uint32_t ch);
static uint32_t port_mgr_tof2_umac3_config_set(bf_dev_id_t dev_id,
                                               uint32_t umac3,
                                               uint32_t ch);
static void port_mgr_tof2_umac3_timestamp_offset_set(bf_dev_id_t dev_id,
                                                     uint32_t umac3,
                                                     uint32_t ch,
                                                     bf_port_speed_t speed);
// global data
static uint32_t initial_ch_seq[4] = {3, 1, 2, 0};
static uint32_t umac3_ch_used[BF_MAX_DEV_COUNT][33][8] = {{{0}}};

static int speed_enum_to_int(bf_port_speed_t speed) {
  if (speed == BF_SPEED_100G) return 100;
  if (speed == BF_SPEED_50G) return 50;
  if (speed == BF_SPEED_40G) return 40;
  if (speed == BF_SPEED_25G) return 25;
  if (speed == BF_SPEED_10G) return 10;
  return 1;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_config
 *
 * Basic UMAC3 (CPU port) speed/fec and channel config (on port-add)
 ****************************************************************************/
void port_mgr_tof2_umac3_config(bf_dev_id_t dev_id,
                                uint32_t umac3,
                                uint32_t ch,
                                bf_port_speed_t speed,
                                bf_fec_types_t fec) {
  uint32_t mode, chmode0 = 0;
  uint32_t n_ch;
  uint32_t unused_fld;
  uint32_t cred_ini, chnl_mode, chnl_seq, chnl_seq_reg;
  uint32_t mcmac_ctrl, pcs_mode, serdes_config;
  uint32_t txff_ctrl = 0;
  uint32_t mac_ctrl;
  uint32_t fec_mode, pma_mode;
  uint32_t fifoctrl0__ctrl1, slot2ch_map;

  n_ch = port_mgr_tof2_umac3_num_chnls_for_speed_calc(speed);
  port_mgr_tof2_umac3_channel_add(dev_id, umac3, ch, n_ch);

  // force sigok low until after DFE
  port_mgr_tof2_umac3_force_sigok_low_set(dev_id, umac3, ch, n_ch);

  // determine UMAC3 mode (speed)
  mode = port_mgr_tof2_umac3_config_to_mode(speed);

  // set chXmode (speed, ena, reset)
  umac3_glbl__chmode__wr(
      dev_id, umac3, ch, &chmode0, mode /*speed*/, 1 /*swreset*/, 1 /*ena*/);
  // reset MAC ch and retrieve mcmac.ctrl
  umac3_mcmac0__ctrl__swreset_rmw(dev_id, umac3, ch, &mcmac_ctrl, 1);

  // enable tx/rx
  umac3_mcmac0__ctrl__rxenable_wr(dev_id, umac3, ch, &mcmac_ctrl, 1, false);
  umac3_mcmac0__ctrl__txenable_wr(
      dev_id,
      umac3,
      ch,
      &mcmac_ctrl,
      1,
      true);  // dont clear reset and set tx/rx ena at the same time
  // clear MAC swreset
  umac3_mcmac0__ctrl__swreset_wr(dev_id, umac3, ch, &mcmac_ctrl, 0, true);

  // PCS, read current value
  umac3_hsmcpcs0__mode__pma_rd(dev_id, umac3, ch, &pcs_mode, &unused_fld, true);
  // determine PMA and FEC modes
  umac3_pcs_pma_and_fec_get(speed, fec, &pma_mode, &fec_mode);

  umac3_hsmcpcs0__mode__pma_wr(dev_id, umac3, ch, &pcs_mode, pma_mode, false);
  umac3_hsmcpcs0__mode__fec_wr(dev_id, umac3, ch, &pcs_mode, fec_mode, true);

  // config serdes data width
  umac3_serdes_config_set(dev_id, umac3, ch, &serdes_config, speed, n_ch);

  port_mgr_tof2_umac3_configure_fec(dev_id, umac3, ch, fec);

  port_mgr_tof2_umac3_configure_appfifo(dev_id, umac3, ch, speed);

  chnl_mode = port_mgr_tof2_umac3_txff_ctrl_chnl_mode_calc(speed);
  eth100g_reg_rspec_txff_ctrl_chnl_mode_set(
      dev_id, umac3, ch, &txff_ctrl, chnl_mode, false);

  cred_ini = port_mgr_tof2_umac3_txff_ctrl_cred_ini_calc(speed);
  eth100g_reg_rspec_txff_ctrl_cred_ini_set(
      dev_id, umac3, ch, &txff_ctrl, cred_ini, true);

  // Get channel sequence based on currently defined ports/speeds
  chnl_seq = port_mgr_tof2_umac3_chnl_seq_calc(dev_id, umac3, ch);
  eth100g_reg_rspec_chnl_seq_chnl_seq_rmw(
      dev_id, umac3, &chnl_seq_reg, chnl_seq);

  umac3_fifoctrl0__chmap0__slot0chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 0)) & 0x3);
  umac3_fifoctrl0__chmap0__slot1chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 1)) & 0x3);
  umac3_fifoctrl0__chmap0__slot2chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 2)) & 0x3);
  umac3_fifoctrl0__chmap0__slot3chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 3)) & 0x3);

  umac3_fifoctrl0__chmap1__slot4chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 0)) & 0x3);
  umac3_fifoctrl0__chmap1__slot5chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 1)) & 0x3);
  umac3_fifoctrl0__chmap1__slot6chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 2)) & 0x3);
  umac3_fifoctrl0__chmap1__slot7chmap_rmw(
      dev_id, umac3, &slot2ch_map, (chnl_seq >> (2 * 3)) & 0x3);

  umac3_fifoctrl0__ctrl1__s2chmapena_rmw(dev_id, umac3, &fifoctrl0__ctrl1, 1);

  port_mgr_tof2_umac3_timestamp_offset_set(dev_id, umac3, ch, speed);

  // BF_SPEED_40G
  if (speed == BF_SPEED_40G) {
    eth100g_reg_rspec_mac_ctrl_ins_ws_rmw(dev_id, umac3, &mac_ctrl, 1);
  } else {
    eth100g_reg_rspec_mac_ctrl_ins_ws_rmw(dev_id, umac3, &mac_ctrl, 0);
  }
  // FIXME test
  uint32_t txcrc = 0;
  eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis_rmw(
      dev_id, umac3, ch, &txcrc, 1);

#if 0
  umac4_chmode0__chmode__txjabber_wr(
      dev_id, umac4, ch, &chmode0, 0x2400ull, false);
  umac4_chmode0__chmode__rxjabber_wr(
      dev_id, umac4, ch, &chmode0, 0x2400ull, false);
  umac4_chmode0__chmode__ifglen_wr(dev_id, umac4, ch, &chmode0, 0xCull, false);
  umac4_chmode0__chmode__ifgpacing_wr(
      dev_id, umac4, ch, &chmode0, 0x1ull, true);  // write all 64b

  port_mgr_tof2_umac4_config_cmn(dev_id, umac4, ch, mode);
#endif
}

/*****************************************************************************
 * port_mgr_tof2_umac3_de_config
 *
 * De-Configure a UMAC3 port being removed
 ****************************************************************************/
void port_mgr_tof2_umac3_de_config(bf_dev_id_t dev_id,
                                   uint32_t umac3,
                                   uint32_t ch) {
  uint32_t chnl_seq_reg, chnl_seq;
  uint32_t n_ch;

  n_ch = port_mgr_tof2_umac_channel_get(dev_id, umac3, ch);
  port_mgr_tof2_umac_channel_del(dev_id, umac3, ch, n_ch);

  // Get channel sequence based on currently defined ports/speeds
  chnl_seq = port_mgr_tof2_umac3_chnl_seq_calc(dev_id, umac3, ch);
  eth100g_reg_rspec_chnl_seq_chnl_seq_rmw(
      dev_id, umac3, &chnl_seq_reg, chnl_seq);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_enable
 *
 * Enable a UMAC3 channel
 ****************************************************************************/
void port_mgr_tof2_umac3_enable(bf_dev_id_t dev_id,
                                uint32_t umac3,
                                uint32_t ch) {
  uint32_t reg32;

  // apply any config changes
  port_mgr_tof2_umac3_config_set(dev_id, umac3, ch);

  // de-assert txff_ctrl.flush
  eth100g_reg_rspec_txff_ctrl_tx_flush_rmw(dev_id, umac3, ch, &reg32, 0);

  // enable the txff_ctrl.chena
  eth100g_reg_rspec_txff_ctrl_chnl_ena_rmw(dev_id, umac3, ch, &reg32, 1);

  umac3_glbl__chmode__swreset_rmw(dev_id, umac3, ch, &reg32, 0);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_disable
 *
 * Disable a UMAC3 channel
 ****************************************************************************/
void port_mgr_tof2_umac3_disable(bf_dev_id_t dev_id,
                                 uint32_t umac3,
                                 uint32_t ch) {
  uint32_t reg32;
  uint32_t cnt, cnt1, cnt2, max_wait = 100;
  uint32_t n_ch = port_mgr_tof2_umac_channel_get(dev_id, umac3, ch);

  // force sigok low until after DFE
  port_mgr_tof2_umac3_force_sigok_low_set(dev_id, umac3, ch, n_ch);

  // assert txff_ctrl.flush
  eth100g_reg_rspec_txff_ctrl_tx_flush_rmw(dev_id, umac3, ch, &reg32, 1);

  // wait for txff_status counts to go to 0
  cnt = cnt1 = cnt2 = -1;
  do {
    eth100g_reg_rspec_txff_status_txff_count_get(
        dev_id, umac3, ch, &reg32, &cnt, true);
    cnt1 = cnt;
    if (cnt == 0) {
      eth100g_reg_rspec_txff_status_txappfifo_count_get(
          dev_id, umac3, ch, &reg32, &cnt, true);
      cnt2 = cnt;
    }
  } while ((cnt != 0) && (--max_wait != 0));
  if (cnt != 0) {
    port_mgr_log(
        "UMAC: %d:%2d:%d : Warning: TxFifo not drained after 100 checks: "
        "txff_cnt=%08x : txappfifo_cnt=%08x",
        dev_id,
        umac3,
        ch,
        cnt1,
        cnt2);
  }
  // disable the txff_ctrl.chena
  eth100g_reg_rspec_txff_ctrl_chnl_ena_rmw(dev_id, umac3, ch, &reg32, 0);

  // reset the MAC CH
  umac3_glbl__chmode__swreset_rmw(dev_id, umac3, ch, &reg32, 1);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_link_state_get
 *
 * return operational state of the port defined on umac3/ch
 ****************************************************************************/
void port_mgr_tof2_umac3_link_state_get(bf_dev_id_t dev_id,
                                        uint32_t umac3,
                                        uint32_t ch,
                                        bool *up) {
  uint32_t reg32 = 0, sts = 0;

  // read livelnkstat0 from hw
  umac3_glbl__livelnkstat0__chlinkup0_rd(dev_id, umac3, &reg32, &sts, true);

  if (ch == 0) {
    umac3_glbl__livelnkstat0__chlinkup0_rd(dev_id, umac3, &reg32, &sts, false);
  } else if (ch == 1) {
    umac3_glbl__livelnkstat0__chlinkup1_rd(dev_id, umac3, &reg32, &sts, false);
  } else if (ch == 2) {
    umac3_glbl__livelnkstat0__chlinkup2_rd(dev_id, umac3, &reg32, &sts, false);
  } else if (ch == 3) {
    umac3_glbl__livelnkstat0__chlinkup3_rd(dev_id, umac3, &reg32, &sts, false);
  }
  *up = (sts != 0);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_link_fault_get
 *
 * Return the link fault status of the port defined on umac3/ch
 ****************************************************************************/
void port_mgr_tof2_umac3_link_fault_get(bf_dev_id_t dev_id,
                                        uint32_t umac3,
                                        uint32_t ch,
                                        bool *pcs_ready,
                                        bool *local_fault,
                                        bool *remote_fault) {
  port_mgr_port_t *port_p = NULL;
  bf_dev_port_t dev_port;
  uint32_t fd32a = 0;
  uint32_t fd32b = 0;
  uint32_t reg32 = 0;
  uint32_t reg64 = 0;
  uint32_t fld64 = 0;
  lld_err_t err;

  if (pcs_ready) {
    // read pcsstatus flag in sts1 register from hw
    umac3_hsmcpcs0__sts1__pcsstatus_rd(dev_id, umac3, ch, &reg32, &fd32a, true);
    *pcs_ready = fd32a ? true : false;
  }

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac3, ch, &dev_port);
  if (err == LLD_OK) {
    port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  }
  if (!port_p) return;

  /* Check if MAC interrupts are enabled */
  eth100g_reg_rspec_mac_en0_mac_get(dev_id, umac3, &reg64, &fld64, true);

  if (fld64) {
    /* Interrupts are enabled, read local/remote fault cached values */
    if (local_fault) {
      if (port_p->lstate.local_fault) {
        *local_fault = true;
        port_p->lstate.local_fault = 0;
      } else {
        *local_fault = false;
      }
    }
    if (remote_fault) {
      if (port_p->lstate.local_fault) {
        *remote_fault = true;
        port_p->lstate.local_fault = 0;
      } else {
        *remote_fault = false;
      }
    }
    return;
  }

  /* Interrupts are disabled.
   * Read and clear the MAC link fault interrupt register.
   * Link fault interrupt registers:
   *  ch0 -> intstat3
   *  ch1 -> intstat4
   *  ch2 -> intstat5
   *  ch3 -> intstat6
   *
   * Local Fault   -> stat4
   * Remote Fault  -> stat5
   */

  if (ch == 0) {
    umac3_interrupts0__intstat3__stat4_rd(dev_id, umac3, &reg32, &fd32a, true);
    umac3_interrupts0__intstat3__stat5_rd(dev_id, umac3, &reg32, &fd32b, false);
    /* Clear interrupts bits */
    reg32 &= 0x030;
    umac3_interrupts0__intclr3__clr4_wr(dev_id, umac3, &reg32, fd32a, true);
  } else if (ch == 1) {
    umac3_interrupts0__intstat4__stat4_rd(dev_id, umac3, &reg32, &fd32a, true);
    umac3_interrupts0__intstat4__stat5_rd(dev_id, umac3, &reg32, &fd32b, false);
    /* Clear interrupts bits */
    reg32 &= 0x030;
    umac3_interrupts0__intclr4__clr4_wr(dev_id, umac3, &reg32, fd32a, true);
  } else if (ch == 2) {
    umac3_interrupts0__intstat5__stat4_rd(dev_id, umac3, &reg32, &fd32a, true);
    umac3_interrupts0__intstat5__stat5_rd(dev_id, umac3, &reg32, &fd32b, false);
    /* Clear interrupts bits */
    reg32 &= 0x030;
    umac3_interrupts0__intclr5__clr4_wr(dev_id, umac3, &reg32, fd32a, true);
  } else if (ch == 3) {
    umac3_interrupts0__intstat6__stat4_rd(dev_id, umac3, &reg32, &fd32a, true);
    umac3_interrupts0__intstat6__stat5_rd(dev_id, umac3, &reg32, &fd32b, false);
    /* Clear interrupts bits */
    reg32 &= 0x030;
    umac3_interrupts0__intclr6__clr4_wr(dev_id, umac3, &reg32, fd32a, true);
  }
  if (local_fault) {
    *local_fault = fd32a ? true : false;
  }
  if (remote_fault) {
    *remote_fault = fd32b ? true : false;
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac3_get_pcs_status
 *
 * Return the pcs status of the port defined on umac3/ch
 ****************************************************************************/
void port_mgr_tof2_umac3_get_pcs_status(bf_dev_id_t dev_id,
                                        int umac3,
                                        int ch,
                                        bool *pcs_status,
                                        uint32_t *block_lock_per_pcs_lane,
                                        uint32_t *alig_marker_lock_per_pcs_lane,
                                        bool *hi_ber,
                                        bool *block_lock_all,
                                        bool *alignment_marker_lock_all) {
  uint32_t fd32a = 0;
  uint32_t fd32b = 0;
  uint32_t reg32 = 0;
  uint32_t blocklock = 0;

  // read pcsstatus flag in sts1 register from hw
  umac3_hsmcpcs0__sts1__pcsstatus_rd(dev_id, umac3, ch, &reg32, &fd32a, true);
  *pcs_status = fd32a ? true : false;

  umac3_hsmcpcs0__sts1__blocklockall_rd(
      dev_id, umac3, ch, &reg32, &fd32a, false);
  *block_lock_all = fd32a ? true : false;

  umac3_hsmcpcs0__sts1__hiber_rd(dev_id, umac3, ch, &reg32, &fd32a, false);
  *hi_ber = fd32a ? true : false;

  umac3_hsmcpcs0__algnstat1__alignstatus_rd(
      dev_id, umac3, ch, &reg32, &fd32a, true);
  *alignment_marker_lock_all = fd32a ? true : false;

  umac3_hsmcpcs0__algnstat1__blocklock_rd(
      dev_id, umac3, ch, &reg32, &fd32a, false);
  blocklock = fd32a;

  umac3_hsmcpcs0__algnstat2__blocklock_rd(
      dev_id, umac3, ch, &reg32, &fd32a, true);
  *block_lock_per_pcs_lane = fd32a << 8 | blocklock;

  umac3_hsmcpcs0__algnstat3__amlock_rd(dev_id, umac3, ch, &reg32, &fd32a, true);
  umac3_hsmcpcs0__algnstat4__amlock_rd(dev_id, umac3, ch, &reg32, &fd32b, true);
  *alig_marker_lock_per_pcs_lane = fd32a | fd32b << 8;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_sw_reset_tx_set
 *
 * Assert or de-assert txswrst on a UMAC4 channel
 ****************************************************************************/
void port_mgr_tof2_umac3_sw_reset_tx_set(bf_dev_id_t dev_id,
                                         uint32_t umac3,
                                         uint32_t ch,
                                         bool assert_reset) {
  uint32_t reg32, state = assert_reset ? 1 : 0;

  umac3_glbl__chmode__swreset_rmw(dev_id, umac3, ch, &reg32, state);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_sw_reset_rx_set
 *
 * Assert ro de-assert txswrst on a UMAC4 channel
 ****************************************************************************/
void port_mgr_tof2_umac3_sw_reset_rx_set(bf_dev_id_t dev_id,
                                         uint32_t umac3,
                                         uint32_t ch,
                                         bool assert_reset) {
  uint32_t reg32, state = assert_reset ? 1 : 0;

  umac3_glbl__chmode__swreset_rmw(dev_id, umac3, ch, &reg32, state);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_sw_reset_set
 *
 * On UMAC3 we dont use separate tx and rx swrst's. This fn is just for
 * naming clarity
 ****************************************************************************/
void port_mgr_tof2_umac3_sw_reset_set(bf_dev_id_t dev_id,
                                      uint32_t umac3,
                                      uint32_t ch,
                                      bool assert_reset) {
  port_mgr_tof2_umac3_sw_reset_rx_set(dev_id, umac3, ch, assert_reset);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_config_to_mode
 *****************************************************************************/
static uint32_t port_mgr_tof2_umac3_config_to_mode(bf_port_speed_t speed) {
  if (speed == BF_SPEED_100G) {
    return UMAC3_MODE_100GBASE_R4;  // 9
  } else if (speed == BF_SPEED_50G) {
    return UMAC3_MODE_50GBASE_R2;
  } else if (speed == BF_SPEED_40G) {
    return UMAC3_MODE_40GBASE_R4;
  } else if (speed == BF_SPEED_25G) {
    return UMAC3_MODE_25GBASE_R1;
  } else if (speed == BF_SPEED_10G) {
    return UMAC3_MODE_10GBASE_R;
  } else if (speed == BF_SPEED_1G) {
    return UMAC3_MODE_1GBASE_X;
  }
  return UMAC3_MODE_RESET;  // 0
}

/*****************************************************************************
 * port_mgr_tof2_umac3_mode_to_config
 *****************************************************************************/
static void port_mgr_tof2_umac3_mode_to_config(uint64_t mode,
                                               bf_port_speed_t *speed,
                                               uint32_t fec_mode,
                                               bf_fec_types_t *fec,
                                               uint32_t *n_ch) {
  if (!speed || !fec || !n_ch) return;

  switch (fec_mode) {
    case UMAC3_FEC_FC:
      *fec = BF_FEC_TYP_FC;
      break;
    case UMAC3_FEC_RS:
      *fec = BF_FEC_TYP_RS;
      break;
    default:
    case UMAC3_FEC_NONE:
      *fec = BF_FEC_TYP_NONE;
      break;
  }

  if (mode == UMAC3_MODE_100GBASE_R4) {
    *speed = BF_SPEED_100G;
    *n_ch = 4;
  } else if (mode == UMAC3_MODE_50GBASE_R2) {
    *speed = BF_SPEED_50G;
    *n_ch = 2;
  } else if (mode == UMAC3_MODE_40GBASE_R4) {
    *speed = BF_SPEED_40G;
    *n_ch = 4;
  } else if (mode == UMAC3_MODE_25GBASE_R1) {
    *speed = BF_SPEED_25G;
    *n_ch = 1;
  } else if (mode == UMAC3_MODE_10GBASE_R) {
    *speed = BF_SPEED_10G;
    *n_ch = 1;
  } else if (mode == UMAC3_MODE_1GBASE_X) {
    *speed = BF_SPEED_1G;
    *n_ch = 1;
  } else if (mode == UMAC3_MODE_RESET) {
    *speed = BF_SPEED_NONE;
    *n_ch = 0;
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac3_num_chnls_for_speed_calc
 ****************************************************************************/
uint32_t port_mgr_tof2_umac3_num_chnls_for_speed_calc(uint32_t speed) {
  uint32_t n_chnls = 1;

  switch (speed) {
    case BF_SPEED_100G:
      n_chnls = 4;
      break;
    case BF_SPEED_50G:
      n_chnls = 2;
      break;
    case BF_SPEED_40G:
      n_chnls = 4;
      break;
    case BF_SPEED_25G:
    case BF_SPEED_10G:
    case BF_SPEED_1G:
      n_chnls = 1;
      break;
  }
  return n_chnls;
}

/*****************************************************************************
 * umac3_glbl__chmode__swreset_rmw
 ****************************************************************************/
static void umac3_glbl__chmode__swreset_rmw(bf_dev_id_t dev_id,
                                            uint32_t umac3,
                                            uint32_t ch,
                                            uint32_t *reg32,
                                            uint32_t swreset) {
  if (ch == 0) {
    umac3_glbl__ch0mode__swreset_rmw(dev_id, umac3, reg32, swreset);
  } else if (ch == 1) {
    umac3_glbl__ch1mode__swreset_rmw(dev_id, umac3, reg32, swreset);
  } else if (ch == 2) {
    umac3_glbl__ch2mode__swreset_rmw(dev_id, umac3, reg32, swreset);
  } else if (ch == 3) {
    umac3_glbl__ch3mode__swreset_rmw(dev_id, umac3, reg32, swreset);
  }
}

/*****************************************************************************
 * umac3_glbl__chmode__wr
 ****************************************************************************/
static void umac3_glbl__chmode__wr(bf_dev_id_t dev_id,
                                   uint32_t umac3,
                                   uint32_t ch,
                                   uint32_t *reg32,
                                   uint32_t speed_mode,
                                   uint32_t swreset,
                                   uint32_t chena) {
  uint32_t unused_fld32;

  if (ch == 0) {
    // get current settings
    umac3_glbl__ch0mode__speed_rd(dev_id, umac3, reg32, &unused_fld32, true);
    umac3_glbl__ch0mode__speed_wr(dev_id, umac3, reg32, speed_mode, false);
    umac3_glbl__ch0mode__swreset_wr(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch0mode__chena_wr(dev_id, umac3, reg32, chena, true);
  } else if (ch == 1) {
    umac3_glbl__ch1mode__speed_rd(dev_id, umac3, reg32, &unused_fld32, true);
    umac3_glbl__ch1mode__speed_wr(dev_id, umac3, reg32, speed_mode, false);
    umac3_glbl__ch1mode__swreset_wr(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch1mode__chena_wr(dev_id, umac3, reg32, chena, true);
  } else if (ch == 2) {
    umac3_glbl__ch2mode__speed_rd(dev_id, umac3, reg32, &unused_fld32, true);
    umac3_glbl__ch2mode__speed_wr(dev_id, umac3, reg32, speed_mode, false);
    umac3_glbl__ch2mode__swreset_wr(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch2mode__chena_wr(dev_id, umac3, reg32, chena, true);
  } else if (ch == 3) {
    umac3_glbl__ch3mode__speed_rd(dev_id, umac3, reg32, &unused_fld32, true);
    umac3_glbl__ch3mode__speed_wr(dev_id, umac3, reg32, speed_mode, false);
    umac3_glbl__ch3mode__swreset_wr(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch3mode__chena_wr(dev_id, umac3, reg32, chena, true);
  }
}

/*****************************************************************************
 * umac3_glbl__chmode__speed_rd
 ****************************************************************************/
/*static*/ void umac3_glbl__chmode__rd(bf_dev_id_t dev_id,
                                       uint32_t umac3,
                                       uint32_t ch,
                                       uint32_t *reg32,
                                       uint32_t *speed_mode,
                                       uint32_t *swreset,
                                       uint32_t *chena) {
  if (ch == 0) {
    umac3_glbl__ch0mode__speed_rd(dev_id, umac3, reg32, speed_mode, true);
    umac3_glbl__ch0mode__swreset_rd(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch0mode__chena_rd(dev_id, umac3, reg32, chena, true);
  } else if (ch == 1) {
    umac3_glbl__ch1mode__speed_rd(dev_id, umac3, reg32, speed_mode, true);
    umac3_glbl__ch1mode__swreset_rd(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch1mode__chena_rd(dev_id, umac3, reg32, chena, true);
  } else if (ch == 2) {
    umac3_glbl__ch2mode__speed_rd(dev_id, umac3, reg32, speed_mode, true);
    umac3_glbl__ch2mode__swreset_rd(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch2mode__chena_rd(dev_id, umac3, reg32, chena, true);
  } else if (ch == 3) {
    umac3_glbl__ch3mode__speed_rd(dev_id, umac3, reg32, speed_mode, true);
    umac3_glbl__ch3mode__swreset_rd(dev_id, umac3, reg32, swreset, false);
    umac3_glbl__ch3mode__chena_rd(dev_id, umac3, reg32, chena, true);
  }
}

/*****************************************************************************
 * umac3_serdes_config_set
 ****************************************************************************/
static void umac3_serdes_config_set(bf_dev_id_t dev_id,
                                    uint32_t umac3,
                                    uint32_t ch,
                                    uint32_t *serdes_config,
                                    uint32_t speed,
                                    uint32_t n_ch) {
  uint32_t ln, sd_width_mode = 1;  // default=25G;
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);

  switch (speed) {
    case BF_SPEED_40G:
    case BF_SPEED_10G:
      sd_width_mode = 2;
      break;
    case BF_SPEED_1G:
      sd_width_mode = 4;
      break;
    case BF_SPEED_100G:
    case BF_SPEED_50G:
    case BF_SPEED_25G:
      sd_width_mode = 1;
      break;
    default:
      bf_sys_assert(0);
  }

  for (ln = 0; ln < n_ch; ln++) {
    uint32_t rx_lane;  // umac_physical_rx_lane
    uint32_t tx_lane;  // umac_physical_tx_lane

    tx_lane = dev_p->umac3[0].phys_tx_ln[ch + ln];
    rx_lane = dev_p->umac3[0].phys_rx_ln[ch + ln];
    if (rx_lane == 0) {
      eth100g_reg_rspec_serdes_config_rx0_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    } else if (rx_lane == 1) {
      eth100g_reg_rspec_serdes_config_rx1_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    } else if (rx_lane == 2) {
      eth100g_reg_rspec_serdes_config_rx2_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    } else if (rx_lane == 3) {
      eth100g_reg_rspec_serdes_config_rx3_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    }

    if (tx_lane == 0) {
      eth100g_reg_rspec_serdes_config_tx0_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    } else if (tx_lane == 1) {
      eth100g_reg_rspec_serdes_config_tx1_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    } else if (tx_lane == 2) {
      eth100g_reg_rspec_serdes_config_tx2_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    } else if (tx_lane == 3) {
      eth100g_reg_rspec_serdes_config_tx3_mode_rmw(
          dev_id, umac3, serdes_config, sd_width_mode);
    }
  }
}

/*****************************************************************************
 * umac3_pcs_pma_and_fec_get
 ****************************************************************************/
static void umac3_pcs_pma_and_fec_get(bf_port_speed_t speed,
                                      bf_fec_type_t fec,
                                      uint32_t *pma_mode,
                                      uint32_t *fec_mode) {
  switch (speed) {
    case BF_SPEED_40G:
    case BF_SPEED_100G:
      *pma_mode = UMAC3_PMA_BASE_R4;
      break;
    case BF_SPEED_50G:
      *pma_mode = UMAC3_PMA_BASE_R2;
      break;
    case BF_SPEED_25G:
    case BF_SPEED_10G:
    case BF_SPEED_1G:
      *pma_mode = UMAC3_PMA_BASE_R1;
      break;
    default:
      bf_sys_assert(0);
  }

  switch (fec) {
    case BF_FEC_TYP_NONE:
      *fec_mode = UMAC3_FEC_NONE;
      break;
    case BF_FEC_TYP_FC:
      *fec_mode = UMAC3_FEC_FC;
      break;
    case BF_FEC_TYP_RS:
      *fec_mode = UMAC3_FEC_RS;
      break;
    default:
      bf_sys_assert(0);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac3_configure_fec
 ****************************************************************************/
static void port_mgr_tof2_umac3_configure_fec(bf_dev_id_t dev_id,
                                              uint32_t umac3,
                                              uint32_t ch,
                                              bf_fec_types_t fec) {
  uint32_t fecrs_dbgctl_disabled_fec = 1;
  uint32_t fecfc_cfg_enareq = 0;
  uint32_t reg32, unused_fld;

  switch (fec) {
    case BF_FEC_TYP_NONE:
      fecrs_dbgctl_disabled_fec = 1;
      fecfc_cfg_enareq = 0;
      break;
    case BF_FEC_TYP_FC:
      fecrs_dbgctl_disabled_fec = 1;
      fecfc_cfg_enareq = 1;
      break;
    case BF_FEC_TYP_RS:
      fecrs_dbgctl_disabled_fec = 0;
      fecfc_cfg_enareq = 0;
      break;
    default:
      bf_sys_assert(0);
  }
  /* Note: We cant use rmw in this case because we need to set a bit that Comira
   *        does not define in their xml file, "[8] enable_pcs_scrambler"
   */
  umac3_fecrs0__dbgctrl__disablefec_rd(
      dev_id, umac3, ch, &reg32, &unused_fld, true);
  reg32 |= (1 << 8);  // enable_pcs_scrambler
  umac3_fecrs0__dbgctrl__disablefec_wr(
      dev_id, umac3, ch, &reg32, fecrs_dbgctl_disabled_fec, true);

  umac3_fecfc0__cfg__enareq_rmw(dev_id, umac3, ch, &reg32, fecfc_cfg_enareq);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_configure_fec
 ****************************************************************************/
static void port_mgr_tof2_umac3_configure_appfifo(bf_dev_id_t dev_id,
                                                  uint32_t umac3,
                                                  uint32_t ch,
                                                  bf_port_speed_t speed) {
  uint32_t ch_msk = 0;
  uint32_t channel, portmapX_ena_setting;
  uint32_t reg32, unused_fld32;

  switch (speed) {
    case BF_SPEED_40G:
    case BF_SPEED_100G:
      ch_msk = 0xF;
      break;
    case BF_SPEED_50G:
      if (ch == 0) {
        ch_msk = 0x3;
      } else if (ch == 2) {
        ch_msk = 0xC;
      } else {
        bf_sys_assert(0);
      }
      break;
    case BF_SPEED_25G:
    case BF_SPEED_10G:
    case BF_SPEED_1G:
      ch_msk = (1 << ch);
      break;
    default:
      bf_sys_assert(0);
  }
  portmapX_ena_setting = 1;  // only ena=1 on the first channel, others =0

  // get current value of reg
  umac3_fifoctrl0__appfifoportmap0__portmap0_rd(
      dev_id, umac3, &reg32, &unused_fld32, true);

  for (channel = 0; channel < 4; channel++) {
    if ((1 << channel) & ch_msk) {
      uint32_t map_fld, en_fld;

      map_fld = (portmapX_ena_setting == 1) ? channel : 4;
      en_fld = portmapX_ena_setting;
      switch (channel) {
        case 0:
          umac3_fifoctrl0__appfifoportmap0__portmap0_wr(
              dev_id, umac3, &reg32, map_fld, false);
          umac3_fifoctrl0__appfifoportmap0__portmap0ena_wr(
              dev_id, umac3, &reg32, en_fld, false);
          break;
        case 1:
          umac3_fifoctrl0__appfifoportmap0__portmap1_wr(
              dev_id, umac3, &reg32, map_fld, false);
          umac3_fifoctrl0__appfifoportmap0__portmap1ena_wr(
              dev_id, umac3, &reg32, en_fld, false);
          break;
        case 2:
          umac3_fifoctrl0__appfifoportmap0__portmap2_wr(
              dev_id, umac3, &reg32, map_fld, false);
          umac3_fifoctrl0__appfifoportmap0__portmap2ena_wr(
              dev_id, umac3, &reg32, en_fld, false);
          break;
        case 3:
          umac3_fifoctrl0__appfifoportmap0__portmap3_wr(
              dev_id, umac3, &reg32, map_fld, false);
          umac3_fifoctrl0__appfifoportmap0__portmap3ena_wr(
              dev_id, umac3, &reg32, en_fld, false);
          break;
      }
      portmapX_ena_setting = 0;
    }
  }
  // extract fld value from new config wd
  umac3_fifoctrl0__appfifoportmap0__portmap0ena_rd(
      dev_id, umac3, &reg32, &portmapX_ena_setting, false);
  // write all that to hw
  umac3_fifoctrl0__appfifoportmap0__portmap0ena_wr(
      dev_id, umac3, &reg32, portmapX_ena_setting, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_txff_ctrl_cred_ini_calc
 *
 * Calculate the cred_ini field value from the speed
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac3_txff_ctrl_cred_ini_calc(
    bf_port_speed_t speed) {
  uint32_t cred_ini_val = 0;

  switch (speed) {
    case BF_SPEED_100G:
    case BF_SPEED_40G:
      cred_ini_val = 0x1f;  // 100G
      break;
    case BF_SPEED_50G:
      cred_ini_val = 0xf;  // 50G
      break;
    case BF_SPEED_25G:
    case BF_SPEED_10G:
    case BF_SPEED_1G:
      cred_ini_val = 0xf;  // < 50G
      break;
    default:
      bf_sys_assert(0);
  }
  return cred_ini_val;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_txff_ctrl_chnl_mode_calc
 *
 * Calculate the chnl_mode field value from the speed
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac3_txff_ctrl_chnl_mode_calc(
    bf_port_speed_t speed) {
  uint32_t chnl_mode_val = 0;

  switch (speed) {
    case BF_SPEED_40G:
    case BF_SPEED_100G:
      chnl_mode_val = 0;
      break;
    case BF_SPEED_50G:
      chnl_mode_val = 1;
      break;
    case BF_SPEED_25G:
    case BF_SPEED_10G:
    case BF_SPEED_1G:
      chnl_mode_val = 2;
      break;
    default:
      chnl_mode_val = 0;
      bf_sys_assert(0);
  }
  return chnl_mode_val;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_chnl_seq_calc
 *
 * Calculate the channel sequence based on all defined ports on this UMAC4s
 * 8x channels.
 *
 * We maintain a map of ports to channels,
 *   CH_USED_4 = 4
 *   CH_USED_2 = 2
 *   CH_USED_1 = 1
 *   CH_USED_0 = 0
 *
 * INITIAL_CH_CFG = [3,1,2,0]
 *
 * Algorithm:
 *  chnl_seq = INITIAL_CH_CFG
 *  foreach ch {
 *    if ch_used[ch] != CH_USED_0
 *      for slot = 0 to 3
 *        if (chnl_seq[slot] >= ch) and (chnl_seq[slot] < (ch + ch_used[ch])
 *          chnl_seq[slot] = ch
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac3_chnl_seq_calc(bf_dev_id_t dev_id,
                                                  uint32_t umac3,
                                                  uint32_t ch) {
  uint32_t c, chnl_seq;
  uint32_t n_ch;
  uint32_t ch_seq[4];

  for (ch = 0; ch < 4; ch++) {
    ch_seq[ch] = initial_ch_seq[ch];
  }

  for (ch = 0; ch < 4; ch++) {
    n_ch = port_mgr_tof2_umac_channel_get(dev_id, umac3, ch);
    if (n_ch != 0) {
      for (c = 0; c < 4; c++) {
        if ((ch_seq[c] >= ch) && (ch_seq[c] < (ch + n_ch))) {
          ch_seq[c] = ch;
        }
      }
    }
  }
  // now convert ch_seq into the txff_ctrl.chnl_seq field
  chnl_seq = 0ull;
  for (c = 0; c < 4; c++) {
    uint32_t fld = ch_seq[c];
    chnl_seq = (chnl_seq << 2) | fld;
  }
  port_mgr_log("chnl_used: [%2d,%2d,%2d,%2d]",
               port_mgr_tof2_umac_channel_get(dev_id, umac3, 0),
               port_mgr_tof2_umac_channel_get(dev_id, umac3, 1),
               port_mgr_tof2_umac_channel_get(dev_id, umac3, 2),
               port_mgr_tof2_umac_channel_get(dev_id, umac3, 3));
  port_mgr_log("chnl_seq : [%2d,%2d,%2d,%2d]",
               ch_seq[0],
               ch_seq[1],
               ch_seq[2],
               ch_seq[3]);
  port_mgr_log("chnl_seq : %08x", chnl_seq);

  return chnl_seq;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_channel_add
 *
 * Update ch_used for the given UMAC and channel(s)
 ****************************************************************************/
void port_mgr_tof2_umac3_channel_add(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t ch,
                                     uint32_t n_ch) {
  umac3_ch_used[dev_id][umac][ch] = n_ch;
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
    umac3_ch_used[dev_id][umac][ch_idx] = 0;
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
  return umac3_ch_used[dev_id][umac][ch];
  (void)dev_id;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_status_get
 *
 * Return detailed UMAC3 status
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac3_status_get(bf_dev_id_t dev_id,
                                           uint32_t umac3,
                                           uint32_t channel,
                                           uint32_t *reg32,
                                           uint32_t *ch0_link_sts,
                                           uint32_t *ch1_link_sts,
                                           uint32_t *ch2_link_sts,
                                           uint32_t *ch3_link_sts,
                                           uint32_t *ch0_rx_fault,
                                           uint32_t *ch1_rx_fault,
                                           uint32_t *ch2_rx_fault,
                                           uint32_t *ch3_rx_fault,
                                           uint32_t *ch0_sig_ok,
                                           uint32_t *ch1_sig_ok,
                                           uint32_t *ch2_sig_ok,
                                           uint32_t *ch3_sig_ok,
                                           uint32_t *ch0_tx_idle,
                                           uint32_t *ch1_tx_idle,
                                           uint32_t *ch2_tx_idle,
                                           uint32_t *ch3_tx_idle) {
  (void)channel;

  umac3_glbl__livelnkstat0__chlinkup0_rd(
      dev_id, umac3, reg32, ch0_link_sts, true);
  umac3_glbl__livelnkstat0__chlinkup1_rd(
      dev_id, umac3, reg32, ch1_link_sts, false);
  umac3_glbl__livelnkstat0__chlinkup2_rd(
      dev_id, umac3, reg32, ch2_link_sts, false);
  umac3_glbl__livelnkstat0__chlinkup3_rd(
      dev_id, umac3, reg32, ch3_link_sts, false);

  umac3_glbl__livelnkstat0__chmacflt0_rd(
      dev_id, umac3, reg32, ch0_rx_fault, false);
  umac3_glbl__livelnkstat0__chmacflt1_rd(
      dev_id, umac3, reg32, ch1_rx_fault, false);
  umac3_glbl__livelnkstat0__chmacflt2_rd(
      dev_id, umac3, reg32, ch2_rx_fault, false);
  umac3_glbl__livelnkstat0__chmacflt3_rd(
      dev_id, umac3, reg32, ch3_rx_fault, false);

  umac3_glbl__livelnkstat0__chsigstat0_rd(
      dev_id, umac3, reg32, ch0_sig_ok, false);
  umac3_glbl__livelnkstat0__chsigstat1_rd(
      dev_id, umac3, reg32, ch1_sig_ok, false);
  umac3_glbl__livelnkstat0__chsigstat2_rd(
      dev_id, umac3, reg32, ch2_sig_ok, false);
  umac3_glbl__livelnkstat0__chsigstat3_rd(
      dev_id, umac3, reg32, ch3_sig_ok, false);

  umac3_glbl__livelnkstat0__txidle0_rd(
      dev_id, umac3, reg32, ch0_tx_idle, false);
  umac3_glbl__livelnkstat0__txidle1_rd(
      dev_id, umac3, reg32, ch1_tx_idle, false);
  umac3_glbl__livelnkstat0__txidle2_rd(
      dev_id, umac3, reg32, ch2_tx_idle, false);
  umac3_glbl__livelnkstat0__txidle3_rd(
      dev_id, umac3, reg32, ch3_tx_idle, false);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_flowcontrol_config_set
 *
 * Apply flowcontrol (PFC/link pause) configs in  MAC
 ****************************************************************************/
void port_mgr_tof2_umac3_flowcontrol_config_set(bf_dev_id_t dev_id,
                                                uint32_t umac3,
                                                uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  uint32_t reg32, val;

  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac3, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  val = port_p->sw.link_pause_tx ? 1 : 0;
  umac3_mcmac0__txconfig__fcfrmgen_rmw(dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.pfc_pause_tx ? 1 : 0;
  umac3_mcmac0__txconfig__pfcfrmgen_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.link_pause_rx || port_p->sw.pfc_pause_rx) ? 1 : 0;
  umac3_mcmac0__rxconfig__enrxfcdec_rmw(dev_id, umac3, ch, &reg32, val);

  // Both PFC and pause frames should be filtered out by MAC always
  val = 1;
  umac3_mcmac0__rxconfig__filterpf_rmw(dev_id, umac3, ch, &reg32, val);

  // PFC TX config in MAC is bitmap - per priority enable/disable
  val = port_p->sw.pfc_pause_tx;
  umac3_mcmac0__txpfcvec__txpfcvec_rmw(dev_id, umac3, ch, &reg32, val);

  return;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_config_set
 *
 * Apply user configured MAC settings
 ****************************************************************************/
static uint32_t port_mgr_tof2_umac3_config_set(bf_dev_id_t dev_id,
                                               uint32_t umac3,
                                               uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  uint32_t reg32, fld32, val;
  uint32_t pma_mode = 0;
  uint32_t fec_mode = 0;
  uint32_t pcs_mode = 0;
  uint32_t unused_fld;

  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac3, ch, &dev_port);
  bf_sys_assert(err == LLD_OK);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  bf_sys_assert(port_p != NULL);

  umac3_hsmcpcs0__mode__pma_rd(dev_id, umac3, ch, &pcs_mode, &unused_fld, true);
  umac3_pcs_pma_and_fec_get(
      port_p->sw.speed, port_p->sw.fec, &pma_mode, &fec_mode);
  umac3_hsmcpcs0__mode__fec_wr(dev_id, umac3, ch, &pcs_mode, fec_mode, true);

  // Program PFC/link pause settings in MAC
  port_mgr_tof2_umac3_flowcontrol_config_set(dev_id, umac3, ch);

  umac3_mcmac0__txconfig__ifglength_rmw(
      dev_id, umac3, ch, &reg32, port_p->sw.ifg);

  umac3_mcmac0__txconfig__enautodrnonflt_rmw(dev_id, umac3, ch, &reg32, 1);

  // hack to set preamble_length since it is not defined in Comiras xml file
  // bits [13:11]
  umac3_mcmac0__txconfig__enautodrnonflt_rd(
      dev_id, umac3, ch, &reg32, &fld32, true);
  reg32 &= ~(7 << 11);
  reg32 |= (port_p->sw.preamble_length == 8)
               ? (0 << 11)
               : (port_p->sw.preamble_length == 4) ? (4 << 11) : (0 << 11);
  umac3_mcmac0__txconfig__enautodrnonflt_wr(
      dev_id, umac3, ch, &reg32, fld32, true);

  val = port_p->sw.promiscuous_mode ? 1 : 0;
  umac3_mcmac0__rxconfig__promiscuous_rmw(dev_id, umac3, ch, &reg32, val);

  // always set to NOT strip for now
  umac3_mcmac0__rxconfig__stripfcs_rmw(dev_id, umac3, ch, &reg32, 0);

  // hack to set preamble_length since it is not defined in Comiras xml file
  // bits [9:7]
  umac3_mcmac0__rxconfig__filterpf_rd(dev_id, umac3, ch, &reg32, &fld32, true);
  reg32 &= ~(7 << 7);
  reg32 |= (port_p->sw.preamble_length == 8)
               ? (0 << 7)
               : (port_p->sw.preamble_length == 4) ? (1 << 7) : (0 << 7);
  umac3_mcmac0__rxconfig__filterpf_wr(dev_id, umac3, ch, &reg32, fld32, true);

  val = port_p->sw.rx_mtu;
  umac3_mcmac0__maxfrmsize__maxfrmsize_rmw(dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.rx_max_jab_sz;
  umac3_mcmac0__maxrxjabsize__maxrxjabsize_rmw(dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.tx_mtu;
  umac3_mcmac0__maxtxjabsize__maxtxjabsize_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.mac_addr[1] << 8) | port_p->sw.mac_addr[0];
  umac3_mcmac0__macaddrlo__macaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.mac_addr[3] << 8) | port_p->sw.mac_addr[2];
  umac3_mcmac0__macaddrmid__macaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.mac_addr[5] << 8) | port_p->sw.mac_addr[4];
  umac3_mcmac0__macaddrhi__macaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.fc_src_mac_addr[1] << 8) | port_p->sw.fc_src_mac_addr[0];
  umac3_mcmac0__fcsaddrlo__fcsaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.fc_src_mac_addr[3] << 8) | port_p->sw.fc_src_mac_addr[2];
  umac3_mcmac0__fcsaddrmid__fcsaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.fc_src_mac_addr[5] << 8) | port_p->sw.fc_src_mac_addr[4];
  umac3_mcmac0__fcsaddrhi__fcsaddr_rmw(dev_id, umac3, ch, &reg32, val);

  //
  // val = port_p->sw.fc_dst_mac_addr

  val = (port_p->sw.fc_dst_mac_addr[1] << 8) | port_p->sw.fc_dst_mac_addr[0];
  umac3_mcmac0__fcdaddrlo__fcdaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.fc_dst_mac_addr[3] << 8) | port_p->sw.fc_dst_mac_addr[2];
  umac3_mcmac0__fcdaddrmid__fcdaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = (port_p->sw.fc_dst_mac_addr[5] << 8) | port_p->sw.fc_dst_mac_addr[4];
  umac3_mcmac0__fcdaddrhi__fcdaddr_rmw(dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.xoff_pause_time;
  umac3_mcmac0__xoffpausetime__xoffpausetime_rmw(
      dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.xon_pause_time;
  umac3_mcmac0__xonpausetime__xonpausetime_rmw(dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.fc_corr_en;
  umac3_fecfc0__cfg__enaerrcorr_rmw(dev_id, umac3, ch, &reg32, val);

  val = port_p->sw.fc_ind_en;
  umac3_fecfc0__cfg__enapcserr_rmw(dev_id, umac3, ch, &reg32, val);

  return 0;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_config_get
 *
 * Apply user configured MAC settings
 ****************************************************************************/
uint32_t port_mgr_tof2_umac3_config_get(bf_dev_id_t dev_id,
                                        uint32_t umac3,
                                        uint32_t ch) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  lld_err_t err;
  uint32_t reg32, fld32, val;

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac3, ch, &dev_port);
  if (err != LLD_OK) {
    port_mgr_log(
        "%s Error: "
        "port_mgr_mac_hw_cfg_get: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        __func__,
        err,
        dev_id,
        umac3,
        ch);
    return BF_INVALID_ARG;
  }

  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);
  if (port_p == NULL) {
    port_mgr_log("Error: No port_p for dev_port=%x\n", dev_port);
    return BF_INVALID_ARG;
  }

  // Program PFC/link pause settings in MAC
  umac3_mcmac0__txconfig__fcfrmgen_rd(dev_id, umac3, ch, &reg32, &val, true);
  port_p->hw.link_pause_tx = val;

  // umac3_mcmac0__txconfig__pfcfrmgen_rd(dev_id, umac3, ch, &reg32, &val,
  // true);
  // port_p->hw.pfc_pause_tx =val;

  // PFC TX config in MAC is bitmap - per priority enable/disable
  umac3_mcmac0__txpfcvec__txpfcvec_rd(dev_id, umac3, ch, &reg32, &val, true);
  port_p->hw.pfc_pause_tx = val;

  umac3_mcmac0__txconfig__ifglength_rd(dev_id, umac3, ch, &reg32, &val, true);
  port_p->hw.ifg = val;

  // hack to set preamble_length since it is not defined in Comiras xml file
  // bits [13:11]
  reg32 = (reg32 >> 11) & 0x7;
  if (reg32 == 0) {
    val = 8;
  } else if (reg32 == 4) {
    val = 4;
  } else {
    val = 99999;
  }
  port_p->hw.preamble_length = val;

  // hack to set preamble_length since it is not defined in Comiras xml file
  // bits [9:7]
  umac3_mcmac0__rxconfig__filterpf_rd(dev_id, umac3, ch, &reg32, &fld32, true);
  reg32 = (reg32 >> 7) & 1;
  if (reg32 == 0) {
    val = 8;
  } else if (reg32 == 1) {
    val = 4;
  } else {
    val = 99999;
  }
  if ((int)val != port_p->hw.preamble_length) {
    port_p->hw.preamble_length = 99998;  // indicate mismatch
  }
  umac3_mcmac0__maxfrmsize__maxfrmsize_rd(
      dev_id, umac3, ch, &reg32, &fld32, true);
  port_p->hw.rx_mtu = fld32;

  umac3_mcmac0__maxrxjabsize__maxrxjabsize_rd(
      dev_id, umac3, ch, &reg32, &fld32, true);
  port_p->hw.rx_max_jab_sz = fld32;

  umac3_mcmac0__maxtxjabsize__maxtxjabsize_rd(
      dev_id, umac3, ch, &reg32, &val, true);
  port_p->hw.tx_mtu = val;

  umac3_mcmac0__xoffpausetime__xoffpausetime_rd(
      dev_id, umac3, ch, &reg32, &val, true);
  port_p->hw.xoff_pause_time = val;

  umac3_mcmac0__xonpausetime__xonpausetime_rd(
      dev_id, umac3, ch, &reg32, &val, true);
  port_p->hw.xon_pause_time = val;

  // Currently these are not prgrammed into hardware. So keep same.
  port_p->hw.link_pause_rx = port_p->sw.link_pause_rx;
  port_p->hw.pfc_pause_rx = port_p->sw.pfc_pause_rx;
  port_p->hw.pfc_pause_tx = port_p->sw.pfc_pause_tx;

  port_p->hw.fc_corr_en = port_p->sw.fc_corr_en;
  port_p->hw.fc_ind_en = port_p->sw.fc_ind_en;
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

  uint32_t mode = 0, fec_mode, chnl_ena = 0;
  if (ch == 0) {
    // get current settings
    umac3_glbl__ch0mode__speed_rd(dev_id, umac3, &reg32, &mode, true);
  } else if (ch == 1) {
    umac3_glbl__ch1mode__speed_rd(dev_id, umac3, &reg32, &mode, true);
  } else if (ch == 2) {
    umac3_glbl__ch2mode__speed_rd(dev_id, umac3, &reg32, &mode, true);
  } else if (ch == 3) {
    umac3_glbl__ch3mode__speed_rd(dev_id, umac3, &reg32, &mode, true);
  }

  eth100g_reg_rspec_txff_ctrl_chnl_ena_get(
      dev_id, umac3, ch, &reg32, &chnl_ena, true);
  umac3_hsmcpcs0__mode__fec_rd(dev_id, umac3, ch, &reg32, &fec_mode, true);

  port_p->hw.speed = 0;
  port_mgr_tof2_umac3_mode_to_config(
      mode, &port_p->hw.speed, fec_mode, &port_p->hw.fec, &port_p->hw.n_lanes);

  if (port_p->hw.speed) {
    port_p->hw.assigned = true;
    port_mgr_tof2_umac3_channel_add(dev_id, umac3, ch, port_p->hw.n_lanes);
  } else {
    port_p->hw.assigned = false;
  }

  if (chnl_ena && port_p->hw.speed) {
    port_p->hw.enabled = true;
  } else {
    port_p->hw.enabled = false;
  }

  return 0;
}

/**********************************************************************
 * port_mgr_tof2_umac3_read_counter
 **********************************************************************/
bf_status_t port_mgr_tof2_umac3_read_counter(bf_dev_id_t dev_id,
                                             uint32_t umac3,
                                             uint32_t ch,
                                             bf_rmon_counter_t ctr_id,
                                             uint64_t *ctr_value) {
  uint32_t reg32 = 0, val = 0;
  uint32_t ctr_0, ctr_16, ctr_32, ctr_48;

  umac3_stats0__rdctrl__cntrnum_rmw(dev_id, umac3, &reg32, ctr_id);

  umac3_stats0__rdctrl__channum_rmw(dev_id, umac3, &reg32, ch);

  do {
    umac3_stats0__rdctrl__rsc_rd(dev_id, umac3, &reg32, &val, true);
  } while (val);
  val = 1;
  umac3_stats0__rdctrl__rsc_rmw(dev_id, umac3, &reg32, val);
  do {
    umac3_stats0__rdctrl__rsc_rd(dev_id, umac3, &reg32, &val, true);
  } while (val);

  umac3_stats0__rdata0__rdata_rd(dev_id, umac3, &reg32, &ctr_0, true);
  umac3_stats0__rdata1__rdata_rd(dev_id, umac3, &reg32, &ctr_16, true);
  umac3_stats0__rdata2__rdata_rd(dev_id, umac3, &reg32, &ctr_32, true);
  umac3_stats0__rdata3__rdata_rd(dev_id, umac3, &reg32, &ctr_48, true);

  *ctr_value = ctr_0 | (ctr_16 << 16) | ((uint64_t)ctr_32 << 32) |
               ((uint64_t)ctr_48 << 48);

  return BF_SUCCESS;
}

/**********************************************************************
 * port_mgr_tof2_umac3_clear_counter
 **********************************************************************/
bf_status_t port_mgr_tof2_umac3_clear_counter(bf_dev_id_t dev_id,
                                              uint32_t umac3,
                                              uint32_t ch) {
  uint32_t val = 0, reg32 = 0;

  val = 1;
  if (ch == 0) {
    umac3_stats0__statsrst__clr0_rmw(dev_id, umac3, &reg32, val);
  } else if (ch == 1) {
    umac3_stats0__statsrst__clr1_rmw(dev_id, umac3, &reg32, val);
  } else if (ch == 2) {
    umac3_stats0__statsrst__clr2_rmw(dev_id, umac3, &reg32, val);
  } else if (ch == 3) {
    umac3_stats0__statsrst__clr3_rmw(dev_id, umac3, &reg32, val);
  }

  val = 0;
  if (ch == 0) {
    umac3_stats0__statsrst__clr0_rmw(dev_id, umac3, &reg32, val);
  } else if (ch == 1) {
    umac3_stats0__statsrst__clr1_rmw(dev_id, umac3, &reg32, val);
  } else if (ch == 2) {
    umac3_stats0__statsrst__clr2_rmw(dev_id, umac3, &reg32, val);
  } else if (ch == 3) {
    umac3_stats0__statsrst__clr3_rmw(dev_id, umac3, &reg32, val);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_lane_map_set
 *
 * Program the lane remapping
 ****************************************************************************/
void port_mgr_tof2_umac3_lane_map_set(bf_dev_id_t dev_id,
                                      uint32_t umac3,
                                      uint32_t phys_tx_ln[8],
                                      uint32_t phys_rx_ln[8]) {
  uint32_t reg32 = 0, bitsel;

  umac3_serdesmux__laneremaprx0__remap0_rmw(
      dev_id, umac3, &reg32, phys_rx_ln[0]);
  umac3_serdesmux__laneremaprx0__remap1_rmw(
      dev_id, umac3, &reg32, phys_rx_ln[1]);
  umac3_serdesmux__laneremaprx0__remap2_rmw(
      dev_id, umac3, &reg32, phys_rx_ln[2]);
  umac3_serdesmux__laneremaprx0__remap3_rmw(
      dev_id, umac3, &reg32, phys_rx_ln[3]);

  umac3_serdesmux__laneremaptx0__remap0_rmw(
      dev_id, umac3, &reg32, phys_tx_ln[0]);
  umac3_serdesmux__laneremaptx0__remap1_rmw(
      dev_id, umac3, &reg32, phys_tx_ln[1]);
  umac3_serdesmux__laneremaptx0__remap2_rmw(
      dev_id, umac3, &reg32, phys_tx_ln[2]);
  umac3_serdesmux__laneremaptx0__remap3_rmw(
      dev_id, umac3, &reg32, phys_tx_ln[3]);

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
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set(
      dev_id, umac3, &bitsel, 8 + (phys_rx_ln[0] & 0x7), false);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set(
      dev_id, umac3, &bitsel, 8 + (phys_rx_ln[1] & 0x7), false);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set(
      dev_id, umac3, &bitsel, 8 + (phys_rx_ln[2] & 0x7), false);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set(
      dev_id, umac3, &bitsel, 8 + (phys_rx_ln[3] & 0x7), true);  // write to hw
  eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr_rmw(
      dev_id, umac3, &reg32, 0x80060);
  eth100g_reg_rspec_eth_mdioci_poll_time_poll_time_rmw(
      dev_id, umac3, &reg32, 0x0100);
  eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena_rmw(
      dev_id, umac3, &reg32, 0x1);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_force_sigok_low_set
 *
 * Force sigok indication to PCS to be "0" for a set of lanes. This holds
 * the Rx state-machine in reset. This is cleared only after the serdes has
 * completed Rx equalization (DFE). This prevents the PCS from acting on
 * noise.
 ****************************************************************************/
void port_mgr_tof2_umac3_force_sigok_low_set(bf_dev_id_t dev_id,
                                             uint32_t umac3,
                                             uint32_t ch,
                                             uint32_t n_ch) {
  uint32_t sigok_ctrl = 0, sigok_lo;
  uint32_t chnl_msk, rx_lane;
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);

  if (!dev_p) return;
  rx_lane = dev_p->umac3[0].phys_rx_ln[ch];
  // force sigok low until after DFE
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac3, &sigok_ctrl, &sigok_lo, true);
  chnl_msk = (((1 << n_ch) - 1) << rx_lane);
  sigok_lo |= chnl_msk;
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set(
      dev_id, umac3, &sigok_ctrl, sigok_lo, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_force_sigok_hi_set
 *
 * Force sigok indication to PCS to be "1" for a set of lanes. This is used
 * when the serdes are not expected to present a valid indication (such
 * as in MAC loopback modes where the serdes may not be programmed or on
 * an emulator where they may not be emulated.
 ****************************************************************************/
void port_mgr_tof2_umac3_force_sigok_hi_set(bf_dev_id_t dev_id,
                                            uint32_t umac3,
                                            uint32_t ch,
                                            uint32_t n_ch) {
  uint32_t sigok_ctrl = 0, sigok_hi;
  uint32_t chnl_msk, rx_lane;
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);

  if (!dev_p) return;
  rx_lane = dev_p->umac3[0].phys_rx_ln[ch];

  // force sigok low until after DFE
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get(
      dev_id, umac3, &sigok_ctrl, &sigok_hi, true);
  chnl_msk = (((1 << n_ch) - 1) << rx_lane);
  sigok_hi |= chnl_msk;
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set(
      dev_id, umac3, &sigok_ctrl, sigok_hi, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_clear_forced_sigok_set
 *
 * Clear all forced indications, hi or low, to allow the real serdes
 * rxsigok indication to be used.
 ****************************************************************************/
void port_mgr_tof2_umac3_clear_forced_sigok_set(bf_dev_id_t dev_id,
                                                uint32_t umac3,
                                                uint32_t ch,
                                                uint32_t n_ch) {
  uint32_t sigok_ctrl, sigok_val;
  uint32_t chnl_msk, rx_lane;
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);

  if (!dev_p) return;
  rx_lane = dev_p->umac3[0].phys_rx_ln[ch];

  // clear force sigok low
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac3, &sigok_ctrl, &sigok_val, true);
  chnl_msk = (((1 << n_ch) - 1) << rx_lane);
  sigok_val &= ~chnl_msk;
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set(
      dev_id, umac3, &sigok_ctrl, sigok_val, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_clear_forced_sigok_set
 *
 * Clear all forced indications, hi or low, to allow the real serdes
 * rxsigok indication to be used.
 ****************************************************************************/
void port_mgr_tof2_umac3_forced_sigok_get(bf_dev_id_t dev_id,
                                          uint32_t umac3,
                                          uint32_t ch,
                                          uint32_t n_lanes,
                                          uint32_t *force_hi_raw_val,
                                          uint32_t *force_lo_raw_val,
                                          uint32_t *force_hi,
                                          uint32_t *force_lo) {
  uint32_t sigok_ctrl, sigok_val;
  uint32_t chnl_msk = (((1 << n_lanes) - 1) << ch);
  uint32_t rx_lane;
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);

  if (!dev_p) return;
  rx_lane = dev_p->umac3[0].phys_rx_ln[ch];

  // force sigok lo
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get(
      dev_id, umac3, &sigok_ctrl, &sigok_val, true);
  *force_lo_raw_val = sigok_ctrl;
  *force_lo = (sigok_val & chnl_msk) >> rx_lane;

  // force sigok hi
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get(
      dev_id, umac3, &sigok_ctrl, &sigok_val, true);
  *force_hi_raw_val = sigok_ctrl;
  *force_hi = (sigok_val & chnl_msk) >> rx_lane;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_loopback_mac_near_set
 ****************************************************************************/
static void port_mgr_tof2_umac3_loopback_mac_near_set(bf_dev_id_t dev_id,
                                                      uint32_t umac3,
                                                      uint32_t ch,
                                                      bool en) {
  uint32_t reg32, fld32 = en ? 1 : 0;

  umac3_mcmac0__ctrl__maclpbk_rmw(dev_id, umac3, ch, &reg32, fld32);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_loopback_pcs_set
 ****************************************************************************/
static void port_mgr_tof2_umac3_loopback_pcs_set(bf_dev_id_t dev_id,
                                                 uint32_t umac3,
                                                 uint32_t ch,
                                                 bool en) {
  uint32_t reg32, fld32 = en ? 1 : 0;
  uint32_t ln, n_lanes;

  n_lanes = port_mgr_tof2_umac_channel_get(dev_id, umac3, ch);
  for (ln = 0; ln < n_lanes; ln++) {
    switch (ch + ln) {
      case 0:
        umac3_serdesmux__serdeslpbk__lpbken0_rmw(dev_id, umac3, &reg32, fld32);
        break;
      case 1:
        umac3_serdesmux__serdeslpbk__lpbken1_rmw(dev_id, umac3, &reg32, fld32);
        break;
      case 2:
        umac3_serdesmux__serdeslpbk__lpbken2_rmw(dev_id, umac3, &reg32, fld32);
        break;
      case 3:
        umac3_serdesmux__serdeslpbk__lpbken3_rmw(dev_id, umac3, &reg32, fld32);
        break;
      default:
        bf_sys_assert(0);
        break;
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac3_loopback_mac_far_set
 ****************************************************************************/
static void port_mgr_tof2_umac3_loopback_mac_far_set(bf_dev_id_t dev_id,
                                                     uint32_t umac3,
                                                     uint32_t ch,
                                                     bool en) {
  uint32_t reg32, fld32 = en ? 1 : 0;
  uint32_t ln, n_lanes;

  n_lanes = port_mgr_tof2_umac_channel_get(dev_id, umac3, ch);
  for (ln = 0; ln < n_lanes; ln++) {
    switch (ch + ln) {
      case 0:
        umac3_fifoctrl0__appfifolpbk__ench0_rmw(dev_id, umac3, &reg32, fld32);
        break;
      case 1:
        umac3_fifoctrl0__appfifolpbk__ench1_rmw(dev_id, umac3, &reg32, fld32);
        break;
      case 2:
        umac3_fifoctrl0__appfifolpbk__ench2_rmw(dev_id, umac3, &reg32, fld32);
        break;
      case 3:
        umac3_fifoctrl0__appfifolpbk__ench3_rmw(dev_id, umac3, &reg32, fld32);
        break;
      default:
        bf_sys_assert(0);
        break;
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac3_loopback_min_latency_set
 ****************************************************************************/
static void port_mgr_tof2_umac3_loopback_min_latency_set(bf_dev_id_t dev_id,
                                                         uint32_t umac3,
                                                         uint32_t ch,
                                                         bool en) {
  uint32_t reg32, fld32 = en ? 1 : 0;

  // needs to be set before chnl_ena goes from 0->1, so set chnl_ena=0 first
  eth100g_reg_rspec_txff_ctrl_chnl_ena_rmw(dev_id, umac3, ch, &reg32, 0);
  eth100g_reg_rspec_txff_ctrl_txrx_lpbk_rmw(dev_id, umac3, ch, &reg32, fld32);
  // then renable the chnl
  eth100g_reg_rspec_txff_ctrl_chnl_ena_rmw(dev_id, umac3, ch, &reg32, 1);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_handle_link_fault_interrupts
 ****************************************************************************/
static void port_mgr_tof2_umac3_handle_link_fault_interrupts(bf_dev_id_t dev_id,
                                                             uint32_t umac3,
                                                             uint32_t ch) {
  port_mgr_port_t *port_p = NULL;
  bool remote_fault = false;
  bool local_fault = false;
  bf_dev_port_t dev_port;
  uint32_t fld32a = 0;
  uint32_t fld32b = 0;
  uint32_t rg32 = 0;
  lld_err_t err;
  int state;

  /* read and clear link fault interrupts */
  if (ch == 0) {
    umac3_interrupts0__intstat3__stat4_rd(dev_id, umac3, &rg32, &fld32a, true);
    umac3_interrupts0__intstat3__stat5_rd(dev_id, umac3, &rg32, &fld32b, false);
    umac3_interrupts0__intclr3__clr4_wr(dev_id, umac3, &rg32, fld32a, true);
  } else if (ch == 1) {
    umac3_interrupts0__intstat4__stat4_rd(dev_id, umac3, &rg32, &fld32a, true);
    umac3_interrupts0__intstat4__stat5_rd(dev_id, umac3, &rg32, &fld32b, false);
    umac3_interrupts0__intclr4__clr4_wr(dev_id, umac3, &rg32, fld32a, true);
  } else if (ch == 2) {
    umac3_interrupts0__intstat5__stat4_rd(dev_id, umac3, &rg32, &fld32a, true);
    umac3_interrupts0__intstat5__stat5_rd(dev_id, umac3, &rg32, &fld32b, false);
    umac3_interrupts0__intclr5__clr4_wr(dev_id, umac3, &rg32, fld32a, true);
  } else if (ch == 3) {
    umac3_interrupts0__intstat6__stat4_rd(dev_id, umac3, &rg32, &fld32a, true);
    umac3_interrupts0__intstat6__stat5_rd(dev_id, umac3, &rg32, &fld32b, false);
    umac3_interrupts0__intclr6__clr4_wr(dev_id, umac3, &rg32, fld32a, true);
  }

  local_fault = fld32a ? true : false;
  remote_fault = fld32b ? true : false;

  if (local_fault || remote_fault) {
    err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, umac3, ch, &dev_port);
    if (err == LLD_OK) {
      bf_port_oper_state_get_and_issue_callbacks(dev_id, dev_port, &state);

      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (!port_p) return;

      port_p->lstate.local_fault |= fld32a;
      port_p->lstate.remote_fault |= fld32b;

      port_mgr_log("%d: INT: umac%d : ch%d : Local_fault=%d, Remote_fault=%d",
                   dev_id,
                   umac3,
                   ch,
                   local_fault,
                   remote_fault);
    }

    /* log no link-fault related interrupts */
    if (rg32 & 0x01) {
      port_mgr_log(
          "%d: INT: umac%d : ch%d : TX underrun interrupt", dev_id, umac3, ch);
    }

    if (rg32 & 0x02) {
      port_mgr_log(
          "%d: INT: umac%d : ch%d : TX jabber interrupt", dev_id, umac3, ch);
    }

    if (rg32 & 0x04) {
      port_mgr_log(
          "%d: INT: umac%d : ch%d : TX timestamp fifo overflow interrupt",
          dev_id,
          umac3,
          ch);
    }

    if (rg32 & 0x08) {
      port_mgr_log(
          "%d: INT: umac%d : ch%d : TX timestamp fifo available interrupt",
          dev_id,
          umac3,
          ch);
    }

    if (rg32 & 0x40) {
      port_mgr_log(
          "%d: INT: umac%d : ch%d : RX CRC error interrupt", dev_id, umac3, ch);
    }
  }
}
/*****************************************************************************
 * port_mgr_tof2_umac3_loopback_set
 *
 * Configure (or un-configure) one of the several loopback modes
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_loopback_set(bf_dev_id_t dev_id,
                                      uint32_t umac3,
                                      uint32_t ch,
                                      bf_loopback_mode_e mode) {
  if (mode == BF_LPBK_NONE) {
    // unconfigure all modes
    port_mgr_tof2_umac3_loopback_mac_near_set(dev_id, umac3, ch, false);
    port_mgr_tof2_umac3_loopback_pcs_set(dev_id, umac3, ch, false);
    port_mgr_tof2_umac3_loopback_min_latency_set(dev_id, umac3, ch, false);
    port_mgr_tof2_umac3_loopback_mac_far_set(dev_id, umac3, ch, false);
  } else if (mode == BF_LPBK_MAC_NEAR) {
    port_mgr_tof2_umac3_loopback_mac_near_set(dev_id, umac3, ch, true);
  } else if (mode == BF_LPBK_MAC_FAR) {
    port_mgr_tof2_umac3_loopback_mac_far_set(dev_id, umac3, ch, true);
  } else if (mode == BF_LPBK_PCS_NEAR) {
    port_mgr_tof2_umac3_loopback_pcs_set(dev_id, umac3, ch, true);
  } else if (mode == BF_LPBK_PIPE) {
    port_mgr_tof2_umac3_loopback_min_latency_set(dev_id, umac3, ch, true);
  }

  // assert soft reset after changing loopbacks
  port_mgr_tof2_umac3_sw_reset_set(dev_id, umac3, ch, true);
  bf_sys_usleep(5000);
  // release soft reset after changing loopbacks
  port_mgr_tof2_umac3_sw_reset_set(dev_id, umac3, ch, false);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_init
 *
 * Apply ont-time UMAC3 configurations
 *
 * Reset and enable mdioci controller on all macs
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_CTRL_ADDRESS,
 *            {mdioci_clk_div[3:0], 2'b0, 1'b0, 1'b1}); // ~reset + en
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_CTRL_ADDRESS,
 *               {mdioci_clk_div[3:0], 2'b0, 1'b1, 1'b1}); // reset + en
 *       `wr_pcs(`ETH400G_PCS_RSPEC_ETH_MDIOCI_CTRL_ADDRESS,
 *               {mdioci_clk_div[3:0], 2'b0, 1'b0, 1'b1}); // ~reset + en
 ****************************************************************************/
void port_mgr_tof2_umac3_init(bf_dev_id_t dev_id,
                              uint32_t umac3,
                              uint32_t clk_div) {
  uint32_t reg32;

  port_mgr_log("UMAC3 mdioci init");
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv_rmw(
      dev_id, umac3, &reg32, clk_div);
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en_rmw(dev_id, umac3, &reg32, 1);

  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_rmw(dev_id, umac3, &reg32, 0);
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_rmw(dev_id, umac3, &reg32, 1);
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_rmw(dev_id, umac3, &reg32, 0);

  port_mgr_log("UMAC3 threshold init");
  // perform some one time UMAC3 inits
  umac3_fifoctrl0__txfifoctrl0__txthreshold0_rmw(dev_id, umac3, &reg32, 4);
  umac3_fifoctrl0__txfifoctrl1__txthreshold1_rmw(dev_id, umac3, &reg32, 4);
  umac3_fifoctrl0__txfifoctrl2__txthreshold2_rmw(dev_id, umac3, &reg32, 4);
  umac3_fifoctrl0__txfifoctrl3__txthreshold3_rmw(dev_id, umac3, &reg32, 4);

  // defaults from tof1
  umac3_fifoctrl0__ctrl1__txfullthres4ch_rmw(dev_id, umac3, &reg32, 4);
  umac3_fifoctrl0__ctrl1__txfullthres2ch_rmw(dev_id, umac3, &reg32, 9);
  umac3_fifoctrl0__ctrl1__txfullthres1ch_rmw(dev_id, umac3, &reg32, 19);
  port_mgr_log("UMAC3 init done");
}

/*****************************************************************************
 * port_mgr_tof2_umac3_rs_fec_status_and_counters_get
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_rs_fec_status_and_counters_get(
    bf_dev_id_t dev_id,
    uint32_t umac3,
    uint32_t ch,
    bool *hi_ser,
    bool *fec_align_status,
    uint32_t *fec_corr_cnt,
    uint32_t *fec_uncorr_cnt,
    uint32_t *fec_ser_lane_0,
    uint32_t *fec_ser_lane_1,
    uint32_t *fec_ser_lane_2,
    uint32_t *fec_ser_lane_3) {
  uint32_t reg32, fld32, lo, hi;

  umac3_fecrs0__sts__hiser_rd(dev_id, umac3, ch, &reg32, &fld32, true);
  *hi_ser = (fld32 == 0) ? false : true;

  umac3_fecrs0__sts__alignstatus_rd(dev_id, umac3, ch, &reg32, &fld32, true);
  *fec_align_status = (fld32 == 0) ? false : true;

  umac3_fecrs0__corrcntlo__corrcnt_rd(dev_id, umac3, ch, &reg32, &lo, true);
  umac3_fecrs0__corrcnthi__corrcnt_rd(dev_id, umac3, ch, &reg32, &hi, true);
  *fec_corr_cnt = (hi << 16) | lo;

  umac3_fecrs0__uncorrcntlo__uncorrcnt_rd(dev_id, umac3, ch, &reg32, &lo, true);
  umac3_fecrs0__uncorrcnthi__uncorrcnt_rd(dev_id, umac3, ch, &reg32, &hi, true);
  *fec_uncorr_cnt = (hi << 16) | lo;

  umac3_fecrs0__serlane0lo__serlane0_rd(dev_id, umac3, ch, &reg32, &lo, true);
  umac3_fecrs0__serlane0hi__serlane0_rd(dev_id, umac3, ch, &reg32, &hi, true);
  *fec_ser_lane_0 = (hi << 16) | lo;

  umac3_fecrs0__serlane1lo__serlane1_rd(dev_id, umac3, ch, &reg32, &lo, true);
  umac3_fecrs0__serlane1hi__serlane1_rd(dev_id, umac3, ch, &reg32, &hi, true);
  *fec_ser_lane_1 = (hi << 16) | lo;

  umac3_fecrs0__serlane2lo__serlane2_rd(dev_id, umac3, ch, &reg32, &lo, true);
  umac3_fecrs0__serlane2hi__serlane2_rd(dev_id, umac3, ch, &reg32, &hi, true);
  *fec_ser_lane_2 = (hi << 16) | lo;

  umac3_fecrs0__serlane3lo__serlane3_rd(dev_id, umac3, ch, &reg32, &lo, true);
  umac3_fecrs0__serlane3hi__serlane3_rd(dev_id, umac3, ch, &reg32, &hi, true);
  *fec_ser_lane_3 = (hi << 16) | lo;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_int_dis_all_set
 *
 * This function disables all umac3 MAC interrupts, which are enabled by
 * default.
 ****************************************************************************/
void port_mgr_tof2_umac3_int_dis_all_set(bf_dev_id_t dev_id, uint32_t umac3) {
  uint32_t reg32 = 0xffff;
  uint32_t fld32 = 1;

  /* Disable all MAC interrupts */
  umac3_interrupts0__clrintenable0__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable1__clrinten8_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable2__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable3__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable4__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable5__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable6__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable7__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable8__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable9__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable10__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable11__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable12__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable13__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable14__clrinten0_wr(
      dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__clrintenable15__clrinten7_wr(
      dev_id, umac3, &reg32, fld32, true);

  /* Clear all interrupt status registers */
  umac3_interrupts0__intclr0__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr1__clr8_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr2__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr3__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr4__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr5__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr6__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr7__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr8__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr9__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr10__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr11__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr12__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr13__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr14__clr0_wr(dev_id, umac3, &reg32, fld32, true);
  umac3_interrupts0__intclr15__clr7_wr(dev_id, umac3, &reg32, fld32, true);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_int_en_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_int_en_set(bf_dev_id_t dev_id,
                                    uint32_t umac3,
                                    bool on) {
  uint32_t reg64;

  eth100g_reg_rspec_mac_en0_mac_rmw(dev_id, umac3, &reg64, (on ? 1ull : 0ull));
}

/*****************************************************************************
 *port_mgr_tof2_umac3_local_fault_int_en_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_local_fault_int_en_set(bf_dev_id_t dev_id,
                                                uint32_t umac3,
                                                uint32_t ch,
                                                bool en) {
  uint32_t fld32 = (en ? 1 : 0);
  uint32_t fld32_aux = 0;
  uint32_t reg32_aux = 0;
  uint32_t reg32 = 0;

  port_mgr_log("%d: INT: umac%d : ch%d : Local  Fault interrupt %s",
               dev_id,
               umac3,
               ch,
               en ? "enabled" : "disabled");

  if (en) {
    /* Enable local fault interrupt
     * Also enable RX fault interrupt in register setintenable0.
     */
    reg32_aux = 0x10;
    if (ch == 0) {
      umac3_interrupts0__intclr3__clr4_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten4_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable3__setinten4_rmw(
          dev_id, umac3, &reg32, fld32);
    } else if (ch == 1) {
      umac3_interrupts0__intclr4__clr4_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten5_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable4__setinten4_rmw(
          dev_id, umac3, &reg32, fld32);
    } else if (ch == 2) {
      umac3_interrupts0__intclr5__clr4_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten6_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable5__setinten4_rmw(
          dev_id, umac3, &reg32, fld32);
    } else if (ch == 3) {
      umac3_interrupts0__intclr6__clr4_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten7_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable6__setinten4_rmw(
          dev_id, umac3, &reg32, fld32);
    }
  } else {
    /* Disable local faul interrupt.
     * If both, local fault and remote fault interrupts are disabled, then
     * also disable RX fault interrupt in clrintenale0.
     */
    if (ch == 0) {
      umac3_interrupts0__clrintenable3__clrinten4_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable3__setinten5_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten4_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    } else if (ch == 1) {
      umac3_interrupts0__clrintenable4__clrinten4_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable4__setinten5_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten5_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    } else if (ch == 2) {
      umac3_interrupts0__clrintenable5__clrinten4_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable5__setinten5_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten6_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    } else if (ch == 3) {
      umac3_interrupts0__clrintenable6__clrinten4_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable6__setinten5_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten7_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    }
  }
}
/*****************************************************************************
 *port_mgr_tof2_umac3_remote_fault_int_en_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_remote_fault_int_en_set(bf_dev_id_t dev_id,
                                                 uint32_t umac3,
                                                 uint32_t ch,
                                                 bool en) {
  uint32_t fld32 = (en ? 1 : 0);
  uint32_t fld32_aux = 0;
  uint32_t reg32_aux = 0;
  uint32_t reg32 = 0;

  port_mgr_log("%d: INT: umac%d : ch%d : Remote Fault interrupt %s",
               dev_id,
               umac3,
               ch,
               en ? "enabled" : "disabled");
  if (en) {
    /* Enable remote fault interrupt
     * Also enable RX fault interrupt in register setintenable0.
     */
    reg32_aux = 0x20;
    if (ch == 0) {
      umac3_interrupts0__intclr3__clr5_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten4_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable3__setinten5_rmw(
          dev_id, umac3, &reg32, fld32);
    } else if (ch == 1) {
      umac3_interrupts0__intclr4__clr5_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten5_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable4__setinten5_rmw(
          dev_id, umac3, &reg32, fld32);
    } else if (ch == 2) {
      umac3_interrupts0__intclr5__clr5_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten6_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable5__setinten5_rmw(
          dev_id, umac3, &reg32, fld32);
    } else if (ch == 3) {
      umac3_interrupts0__intclr6__clr5_wr(dev_id, umac3, &reg32_aux, 1, true);
      umac3_interrupts0__setintenable0__setinten7_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable6__setinten5_rmw(
          dev_id, umac3, &reg32, fld32);
    }
  } else {
    /* Disable remote faul interrupt.
     * If both, local fault and remote fault, interrupts are disabled, then
     * also disable RX fault interrupt in clrintenale0.
     */
    if (ch == 0) {
      umac3_interrupts0__clrintenable3__clrinten5_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable3__setinten4_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten4_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    } else if (ch == 1) {
      umac3_interrupts0__clrintenable4__clrinten5_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable4__setinten4_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten5_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    } else if (ch == 2) {
      umac3_interrupts0__clrintenable5__clrinten5_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable5__setinten4_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten6_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    } else if (ch == 3) {
      umac3_interrupts0__clrintenable6__clrinten5_rmw(
          dev_id, umac3, &reg32, fld32);
      umac3_interrupts0__setintenable6__setinten4_rd(
          dev_id, umac3, &reg32, &fld32_aux, true);
      if (!fld32_aux) {
        umac3_interrupts0__clrintenable0__clrinten7_wr(
            dev_id, umac3, &reg32_aux, 1, true);
      }
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac3_tx_local_fault_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_tx_local_fault_set(bf_dev_id_t dev_id,
                                            uint32_t umac3,
                                            uint32_t ch,
                                            bool on) {
  uint32_t reg64;

  umac3_mcmac0__txdebug__txlfault_rmw(
      dev_id, umac3, ch, &reg64, (on ? 1ull : 0ull));
}

/*****************************************************************************
 * port_mgr_tof2_umac3_tx_remote_fault_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_tx_remote_fault_set(bf_dev_id_t dev_id,
                                             uint32_t umac3,
                                             uint32_t ch,
                                             bool on) {
  uint32_t reg64;

  umac3_mcmac0__txdebug__txrfault_rmw(
      dev_id, umac3, ch, &reg64, (on ? 1ull : 0ull));
}

/*****************************************************************************
 * port_mgr_tof2_umac3_tx_idle_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_tx_idle_set(bf_dev_id_t dev_id,
                                     uint32_t umac3,
                                     uint32_t ch,
                                     bool on) {
  uint32_t reg64;

  umac3_mcmac0__txdebug__txidle_rmw(
      dev_id, umac3, ch, &reg64, (on ? 1ull : 0ull));
}

/*****************************************************************************
 * port_mgr_tof2_umac3_handle_interrupts
 *
 *
 ****************************************************************************/
bf_status_t port_mgr_tof2_umac3_handle_interrupts(bf_dev_id_t dev_id,
                                                  uint32_t umac3,
                                                  bool *possible_state_chg) {
  uint32_t reg32;
  uint32_t fld32;

  if (!possible_state_chg) return BF_INVALID_ARG;
  if (umac3 > 0 && umac3 < 33) return BF_INVALID_ARG;

  *possible_state_chg = false;

  /* Read and clear umac3 interrupts */
  umac3_interrupts0__intstat0__stat0_rd(dev_id, umac3, &reg32, &fld32, true);
  umac3_interrupts0__intclr0__clr0_wr(dev_id, umac3, &reg32, fld32, true);

  if (reg32 & 0x0001) {
    port_mgr_log(
        "%d: INT: umac%d : ch0 : Fifoctrl RX sync interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0002) {
    port_mgr_log(
        "%d: INT: umac%d : ch1 : Fifoctrl RX sync interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0004) {
    port_mgr_log(
        "%d: INT: umac%d : ch2 : Fifoctrl RX sync interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0008) {
    port_mgr_log(
        "%d: INT: umac%d : ch3 : Fifoctrl RX sync interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0010) {
    port_mgr_log("%d: INT: umac%d : ch0 : RX fault interrupt", dev_id, umac3);
    port_mgr_tof2_umac3_handle_link_fault_interrupts(dev_id, umac3, 0);
    *possible_state_chg = true;
  }

  if (reg32 & 0x0020) {
    port_mgr_log("%d: INT: umac%d : ch1 : RX fault interrupt", dev_id, umac3);
    port_mgr_tof2_umac3_handle_link_fault_interrupts(dev_id, umac3, 1);
    *possible_state_chg = true;
  }

  if (reg32 & 0x0040) {
    port_mgr_log("%d: INT: umac%d : ch2 : RX fault interrupt", dev_id, umac3);
    port_mgr_tof2_umac3_handle_link_fault_interrupts(dev_id, umac3, 2);
    *possible_state_chg = true;
  }

  if (reg32 & 0x0080) {
    port_mgr_log("%d: INT: umac%d : ch3 : RX fault interrupt", dev_id, umac3);
    port_mgr_tof2_umac3_handle_link_fault_interrupts(dev_id, umac3, 3);
    *possible_state_chg = true;
  }

  if (reg32 & 0x0100) {
    port_mgr_log(
        "%d: INT: umac%d : --- : RX SigOk lane#0 interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0200) {
    port_mgr_log(
        "%d: INT: umac%d : --- : RX SigOk lane#1 interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0400) {
    port_mgr_log(
        "%d: INT: umac%d : --- : RX SigOk lane#2 interrupt", dev_id, umac3);
  }

  if (reg32 & 0x0800) {
    port_mgr_log(
        "%d: INT: umac%d : --- : RX SigOk lane#3 interrupt", dev_id, umac3);
  }

  if (reg32 & 0x1000) {
    port_mgr_log("%d: INT: umac%d : ch0 : TX idle interrupt", dev_id, umac3);
  }

  if (reg32 & 0x2000) {
    port_mgr_log("%d: INT: umac%d : ch1 : TX idle interrupt", dev_id, umac3);
  }

  if (reg32 & 0x4000) {
    port_mgr_log("%d: INT: umac%d : ch2 : TX idle interrupt", dev_id, umac3);
  }

  if (reg32 & 0x8000) {
    port_mgr_log("%d: INT: umac%d : ch3 : TX idle interrupt", dev_id, umac3);
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_umac3_timestamp_offset_set
 *
 *
 ****************************************************************************/
static void port_mgr_tof2_umac3_timestamp_offset_set(bf_dev_id_t dev_id,
                                                     uint32_t umac3,
                                                     uint32_t ch,
                                                     bf_port_speed_t speed) {
  uint64_t reg64, ets_offset = 917;  // TODO - Value is not final.
  uint64_t txff_cnt_adj = 1024 * 128 / speed_enum_to_int(speed);

  // Program timestamp offset values for the channel.
  eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_rmw(
      dev_id, umac3, ch, &reg64, ets_offset);
  eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_rmw(
      dev_id, umac3, ch, &reg64, txff_cnt_adj);
}

void port_mgr_tof2_umac3_rx_enable_set(bf_dev_id_t dev_id,
                                       uint32_t umac3,
                                       uint32_t ch,
                                       bool rx_en) {
  uint32_t mcmac_ctrl = 0;

  // reset MAC ch and retrieve mcmac.ctrl
  umac3_mcmac0__ctrl__swreset_rmw(dev_id, umac3, ch, &mcmac_ctrl, 1);
  if (rx_en) {
    umac3_mcmac0__ctrl__rxenable_wr(dev_id, umac3, ch, &mcmac_ctrl, 1, true);
  } else {
    umac3_mcmac0__ctrl__rxenable_wr(dev_id, umac3, ch, &mcmac_ctrl, 0, true);
  }
  // clear MAC swreset
  umac3_mcmac0__ctrl__swreset_wr(dev_id, umac3, ch, &mcmac_ctrl, 0, true);
}

void port_mgr_tof2_umac3_txdrain_set(bf_dev_id_t dev_id,
                                     uint32_t umac3,
                                     uint32_t ch,
                                     bool en) {
  uint32_t fld32 = en ? 1 : 0;
  uint32_t mcmac_ctrl = 0;

  umac3_mcmac0__ctrl__txdrn_rmw(dev_id, umac3, ch, &mcmac_ctrl, fld32);
}

/*****************************************************************************
 * port_mgr_tof2_umac3_tx_ignore_rx_set
 *
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_tx_ignore_rx_set(bf_dev_id_t dev_id,
                                          uint32_t umac3,
                                          uint32_t ch,
                                          bool en) {
  uint32_t mcmac_ctrl = 0;

  umac3_mcmac0__ctrl__faultovrd_rmw(dev_id, umac3, ch, &mcmac_ctrl, en ? 1 : 0);
}

/****************************************************************************
 * port_mgr_tof2_umac3_get_1588_timestamp_tx
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_get_1588_timestamp_tx(bf_dev_id_t dev_id,
                                               uint32_t mac_blk,
                                               uint32_t ch,
                                               uint64_t *ts,
                                               bool *ts_valid,
                                               int *ts_id) {
  uint32_t data;
  uint32_t reg;
  umac3_mcmac0__txtsinfo__tsid_rd(
      dev_id, mac_blk, ch, &reg, (uint32_t *)ts_id, true);
  umac3_mcmac0__txtsinfo__tsvld_rd(dev_id, mac_blk, ch, &reg, &data, true);
  *ts_valid = (bool)data;
  if (*ts_valid) {
    uint64_t v0, v1, v2, v3;
    umac3_mcmac0__tsv0__ts_rd(dev_id, mac_blk, ch, &reg, &data, true);
    v0 = data;
    umac3_mcmac0__tsv1__ts_rd(dev_id, mac_blk, ch, &reg, &data, true);
    v1 = data;
    umac3_mcmac0__tsv2__ts_rd(dev_id, mac_blk, ch, &reg, &data, true);
    v2 = data;
    umac3_mcmac0__tsv3__ts_rd(dev_id, mac_blk, ch, &reg, &data, true);
    v3 = data;
    *ts = ((v3 << 48) | (v2 << 32) | (v1 << 16) | (v0 << 0));
  }
}

/****************************************************************************
 * port_mgr_tof2_umac3_set_1588_timestamp_delta_tx
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_set_1588_timestamp_delta_tx(bf_dev_id_t dev_id,
                                                     uint32_t mac_blk,
                                                     uint32_t ch,
                                                     uint16_t delta) {
  uint32_t data = delta;
  uint32_t reg = 0;
  umac3_mcmac0__txtsdelta__txtsdelta_wr(dev_id, mac_blk, ch, &reg, data, true);
}

/****************************************************************************
 * port_mgr_tof2_umac3_get_1588_timestamp_delta_tx
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_get_1588_timestamp_delta_tx(bf_dev_id_t dev_id,
                                                     uint32_t mac_blk,
                                                     uint32_t ch,
                                                     uint16_t *delta) {
  uint32_t data = 0;
  uint32_t reg = 0;
  umac3_mcmac0__txtsdelta__txtsdelta_rd(dev_id, mac_blk, ch, &reg, &data, true);
  *delta = data;
}

/****************************************************************************
 * port_mgr_tof2_umac3_set_1588_timestamp_delta_rx
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_set_1588_timestamp_delta_rx(bf_dev_id_t dev_id,
                                                     uint32_t mac_blk,
                                                     uint32_t ch,
                                                     uint16_t delta) {
  uint32_t data = delta;
  uint32_t reg = 0;
  umac3_mcmac0__rxtsdelta__rxtsdelta_wr(dev_id, mac_blk, ch, &reg, data, true);
}

/****************************************************************************
 * port_mgr_tof2_umac3_get_1588_timestamp_delta_rx
 *
 ****************************************************************************/
void port_mgr_tof2_umac3_get_1588_timestamp_delta_rx(bf_dev_id_t dev_id,
                                                     uint32_t mac_blk,
                                                     uint32_t ch,
                                                     uint16_t *delta) {
  uint32_t data = 0;
  uint32_t reg = 0;
  umac3_mcmac0__rxtsdelta__rxtsdelta_rd(dev_id, mac_blk, ch, &reg, &data, true);
  *delta = data;
}
