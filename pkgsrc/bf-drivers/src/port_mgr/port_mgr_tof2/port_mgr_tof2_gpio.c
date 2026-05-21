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

#include <stdarg.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <dvm/dvm_intf.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include "port_mgr_tof2_map.h"
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_tof2_serdes_if.h>
#include <port_mgr/port_mgr_log.h>
#include "eth400g_mac_rspec_access.h"
#include "eth400g_pcs_rspec_access.h"

bf_status_t port_mgr_tof2_gpio_tile_test(bf_dev_id_t dev_id);

/** \brief tof-2 IO Tile init
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param clk_div : MDIO CLK divider value (0,1,3,7,15)
 *
 * Reset iotile 0 to 3 and set mdioci clk divider through gpio_iotiles (mac_id
 *35 to 38)
 * `wr_st_reg(`GPIO_IOTILE_RSPEC_MDIOCI_CTRL_ADDRESS, * {mdioci_clk_div[3:0],
 *2'b0, 1'b0, 1'b1}, * mac_id); // ~reset + en
 * `wr_st_reg(`GPIO_IOTILE_RSPEC_MDIOCI_CTRL_ADDRESS, * {mdioci_clk_div[3:0],
 *2'b0, 1'b1, 1'b1}, * mac_id); // reset + en
 * `wr_st_reg(`GPIO_IOTILE_RSPEC_MDIOCI_CTRL_ADDRESS, * {mdioci_clk_div[3:0],
 *2'b0, 1'b0, 1'b1}, * mac_id); // ~reset + en
 *
 */
bf_status_t port_mgr_tof2_gpio_tile_init(bf_dev_id_t dev_id, uint32_t clk_div) {
  uint32_t reg32 = 0;
  uint32_t soft_reset_reg;
  uint32_t misc_soft_reset_ofs;

  // Below can be used to debug tile access issues if reset-delays or
  // MDIO CLK speed is suspected
  //
  // port_mgr_tof2_gpio_tile_test(dev_id);
  // if (1) return 0;

  port_mgr_log("GPIO: %d:---: Tile init: Release MISC soft_reset", dev_id);
  misc_soft_reset_ofs = offsetof(tof2_reg, device_select.misc_regs.soft_reset);
  lld_read_register(dev_id, misc_soft_reset_ofs, &soft_reset_reg);
  soft_reset_reg = (soft_reset_reg & ~(0xf << 24));
  lld_write_register(dev_id, misc_soft_reset_ofs, soft_reset_reg);
  bf_sys_usleep(100);

  port_mgr_log("GPIO: %d:---: Tile init: clk_div = %d", dev_id, clk_div);

  lld_write_register(dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), 0);
  lld_write_register(dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), 0);
  lld_write_register(dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), 0);
  lld_write_register(dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), 0);
  bf_sys_usleep(100);

  setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_clkdiv(&reg32, clk_div);
  setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_en(&reg32, 1);

  setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_reset(&reg32, 0);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), reg32);
  bf_sys_usleep(100);

  setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_reset(&reg32, 1);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), reg32);
  bf_sys_usleep(100);

  setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_reset(&reg32, 0);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), reg32);
  lld_write_register(
      dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), reg32);
  bf_sys_usleep(100);

  return BF_SUCCESS;
}

/*
For unreset_delay=100us; unreset_delay < 1 second; unreset_delay *=10 {
  For mdio_clk = 3 ; mdio_clk < 16; mdio_clk = (mdio_clk << 1) | 1 {
    Data = Read( some_known_reg )
    If data != 0x0bad_0bad return // leave currenl delay and clk settings
  }
}
bf_status_t bf_tof2_serdes_tile_known_value_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port)
*/
bf_status_t port_mgr_tof2_gpio_tile_test(bf_dev_id_t dev_id) {
  uint32_t unreset_delay, mdio_clk_code;
  bf_status_t rc;
  bool fail_once = true;

  for (unreset_delay = 10 /*us*/; unreset_delay < 1000000 /* 1 second */;
       unreset_delay *= 10) {
    for (mdio_clk_code = 0x3; mdio_clk_code < 16;
         mdio_clk_code = ((mdio_clk_code << 1) | 1)) {
      uint32_t reg32 = 0;
      uint32_t soft_reset_reg;
      uint32_t misc_soft_reset_ofs;
      uint32_t clk_div = mdio_clk_code;

      port_mgr_log("GPIO: %d:---: Tile Test:", dev_id);
      port_mgr_log(
          "GPIO: %d:---: -- un-reset delay: %d", dev_id, unreset_delay);
      port_mgr_log(
          "GPIO: %d:---: -- mdio clk code : %d", dev_id, mdio_clk_code);

      port_mgr_log("GPIO: %d:---: Tile init: Release MISC soft_reset", dev_id);
      misc_soft_reset_ofs =
          offsetof(tof2_reg, device_select.misc_regs.soft_reset);
      lld_read_register(dev_id, misc_soft_reset_ofs, &soft_reset_reg);
      port_mgr_log(
          "GPIO: %d:---: soft_reset_reg = %08x", dev_id, soft_reset_reg);
      if (((soft_reset_reg >> 24) & 0xF) != 0xF) {
        port_mgr_log("GPIO: %d:---: WARNING: tiles not in reset !!", dev_id);
      }
      soft_reset_reg = (soft_reset_reg & ~(0xf << 24));
      lld_write_register(dev_id, misc_soft_reset_ofs, soft_reset_reg);
      bf_sys_usleep(unreset_delay);

      port_mgr_log("GPIO: %d:---: Tile init: clk_div = %d", dev_id, clk_div);

      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), 0);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), 0);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), 0);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), 0);
      bf_sys_usleep(unreset_delay);

      setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_clkdiv(&reg32, clk_div);
      setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_en(&reg32, 1);

      setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_reset(&reg32, 0);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), reg32);
      bf_sys_usleep(unreset_delay);

      setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_reset(&reg32, 1);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), reg32);
      bf_sys_usleep(unreset_delay);

      setp_tof2_gpio_iotile_rspec_mdioci_ctrl_mdioci_reset(&reg32, 0);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_bl.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_br.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tl.mdioci_ctrl), reg32);
      lld_write_register(
          dev_id, offsetof(tof2_reg, gpio_iotile_tr.mdioci_ctrl), reg32);
      bf_sys_usleep(unreset_delay);

      uint32_t num_pipes = 0;
      lld_sku_get_num_active_pipes(dev_id, &num_pipes);
      for (uint32_t pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        bf_dev_port_t dev_port;
        uint32_t port_id = 8;
        uint32_t mac_id, ch;

        dev_port = MAKE_DEV_PORT(pipe_id, port_id);
        // make sure its valid
        rc = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
        if (rc != 0) continue;

        /* test tile accessibility */
        rc = bf_tof2_serdes_tile_known_value_get(dev_id, dev_port);

        // force a failure to test code
        if (fail_once) {
          fail_once = false;
          rc = BF_INVALID_ARG;
        }

        if (rc == BF_SUCCESS) {
          port_mgr_log("GPIO: %d:---: Tile %d OK", dev_id, pipe_id);
          // return BF_SUCCESS;
        } else {
          port_mgr_log("GPIO: %d:---: Tile %d NOT accessible", dev_id, pipe_id);
          // break;
        }
      }

      if (unreset_delay >= 1000000) return BF_SUCCESS;

      port_mgr_log("GPIO: %d:---: Re-assert tile resets", dev_id);
      misc_soft_reset_ofs =
          offsetof(tof2_reg, device_select.misc_regs.soft_reset);
      lld_read_register(dev_id, misc_soft_reset_ofs, &soft_reset_reg);
      port_mgr_log(
          "GPIO: %d:---: soft_reset_reg = %08x", dev_id, soft_reset_reg);
      if (((soft_reset_reg >> 24) & 0xF) != 0x0) {
        port_mgr_log("GPIO: %d:---: WARNING: tiles not un-reset !!", dev_id);
      }
      soft_reset_reg = (soft_reset_reg | (0xf << 24));
      lld_write_register(dev_id, misc_soft_reset_ofs, soft_reset_reg);
      bf_sys_usleep(unreset_delay);
    }
  }
  return BF_INVALID_ARG;
}

bf_status_t bf_serdes_tof2_clkobs_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bf_clkobs_pad_t pad,
                                      bf_sds_clkobs_clksel_t clk_src,
                                      int divider,
                                      int daisy_sel) {
  uint32_t val, clkobs, misc_clkpad_ctrl;
  bf_mac_block_id_t i, mac_block;
  int lane;

  if (divider < 0 || divider > 3 || daisy_sel < 0 || daisy_sel > 1) {
    return BF_INVALID_ARG;
  }
  // if (pad != BF_CLKOBS_PAD_0 || pad != BF_CLKOBS_PAD_1) {
  if ((pad != BF_CLKOBS_PAD_0) && (pad != BF_CLKOBS_PAD_1)) {
    return BF_INVALID_ARG;
  }
  if (bf_port_map_dev_port_to_mac(dev_id, dev_port, &mac_block, &lane) !=
      BF_SUCCESS) {
    return BF_INVALID_ARG;
  }
  val = 0;
  switch (clk_src) {
    case BF_SDS_RX_RECOVEREDCLK:
      clkobs = 0;
      break;
    case BF_SDS_TX_CLK:
      clkobs = 0x4;
      break;
    case BF_SDS_NONE_CLK:
      /* just deselect this MAC from driving the clk daisy chain */
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw(
          dev_id, mac_block, &val, 1);
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw(
          dev_id, mac_block, &val, 1);
      return BF_SUCCESS;
    default:
      return BF_INVALID_ARG;
  }

  // deselect any previous setting that might be driving clkobd_pad
  for (i = 1; i <= 32; i++) {
    if (i != mac_block) {
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw(
          dev_id, mac_block, &val, 1);
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw(
          dev_id, mac_block, &val, 1);
    }
  }
  // eth400g_mac.eth_clkobs_ctrl; enable to drive a single daisy chain
  // write to eth400g_pcs.eth_clkobs_ctrl
  // write to misc.clkpad_ctrl
  lld_read_register(dev_id,
                    tof2_reg_device_select_misc_regs_clkpad_ctrl_address,
                    &misc_clkpad_ctrl);
  val = 1;
  if (pad == BF_CLKOBS_PAD_1) {
    eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw(
        dev_id, mac_block, &val, 1);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_rmw(
        dev_id, mac_block, &clkobs, 1);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_rmw(
        dev_id, mac_block, (uint32_t *)&divider, 1);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_rmw(
        dev_id, mac_block, &val, 1);
    misc_clkpad_ctrl &= ~(0x7UL << 4);
    misc_clkpad_ctrl |= ((daisy_sel | (1 << 2)) << 4);
  } else {
    eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw(
        dev_id, mac_block, &val, 1);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_rmw(
        dev_id, mac_block, &clkobs, 1);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_rmw(
        dev_id, mac_block, (uint32_t *)&divider, 1);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_rmw(
        dev_id, mac_block, &val, 1);
    misc_clkpad_ctrl &= ~(0x7UL);
    misc_clkpad_ctrl |= (daisy_sel | (1 << 2));
  }
  return BF_SUCCESS;
}
