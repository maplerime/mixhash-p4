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

#include <byteswap.h>
#include <target-sys/bf_sal/bf_sys_sem.h>
#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>

// #include <port_mgr/port_mgr_log.h>
#include "tof3-autogen-required-headers.h"
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_ha.h>
#include "port_mgr_tof3_serdes.h"
#include <port_mgr/port_mgr_intf.h>
#include "port_mgr_tof3_serdes_map.h"

void port_mgr_aw_access_rd32(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t offset,
                             uint32_t *r_data,
                             const char *fn) {
  (void)dev_id;
  (void)dev_port;
  (void)offset;
  (void)r_data;
  (void)fn;
}

void port_mgr_aw_access_wr32(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t offset,
                             uint32_t w_data,
                             const char *fn) {
  (void)dev_id;
  (void)dev_port;
  (void)offset;
  (void)w_data;
  (void)fn;
}

bf_status_t port_mgr_clkobs_ctrl_sel_clkena0(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t mac_block,
                                             uint32_t enable) {
  uint32_t reg, serdes_clkobs_ctrl;

  if (mac_block == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_clkobs_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_clkobs_ctrl);
    reg = reg_base + (stride * (mac_block - 1));
  }

  lld_subdev_read_register(dev_id, subdev_id, reg, &serdes_clkobs_ctrl);
  serdes_clkobs_ctrl = ((enable & 0x1ul) << 12) |
                       (serdes_clkobs_ctrl & (~(((uint32_t)0x1ul) << 12)));
  lld_subdev_write_register(dev_id, subdev_id, reg, serdes_clkobs_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_sel_clkena1(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t mac_block,
                                             uint32_t enable) {
  uint32_t reg, serdes_clkobs_ctrl;

  if (mac_block == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_clkobs_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_clkobs_ctrl);
    reg = reg_base + (stride * (mac_block - 1));
  }
  lld_subdev_read_register(dev_id, subdev_id, reg, &serdes_clkobs_ctrl);
  serdes_clkobs_ctrl = ((enable & 0x1ul) << 28) |
                       (serdes_clkobs_ctrl & (~(((uint32_t)0x1ul) << 28)));
  lld_subdev_write_register(dev_id, subdev_id, reg, serdes_clkobs_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_sel_clkobs1(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t mac_block,
                                             uint32_t clkobs) {
  uint32_t reg, serdes_clkobs_ctrl;

  if (mac_block == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_clkobs_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_clkobs_ctrl);
    reg = reg_base + (stride * (mac_block - 1));
  }

  lld_subdev_read_register(dev_id, subdev_id, reg, &serdes_clkobs_ctrl);
  serdes_clkobs_ctrl = ((clkobs & 0x1ful) << 16) |
                       (serdes_clkobs_ctrl & (~(((uint32_t)0x1ful) << 16)));
  lld_subdev_write_register(dev_id, subdev_id, reg, serdes_clkobs_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_sel_clkobs0(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t mac_block,
                                             uint32_t clkobs) {
  uint32_t reg, serdes_clkobs_ctrl;

  if (mac_block == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_clkobs_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_clkobs_ctrl);
    reg = reg_base + (stride * (mac_block - 1));
  }

  lld_subdev_read_register(dev_id, subdev_id, reg, &serdes_clkobs_ctrl);
  serdes_clkobs_ctrl =
      (clkobs & 0x1ful) | (serdes_clkobs_ctrl & (~((uint32_t)0x1ful)));
  lld_subdev_write_register(dev_id, subdev_id, reg, serdes_clkobs_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_sel_clkdiv0(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t mac_block,
                                             uint32_t clkdiv) {
  uint32_t reg, serdes_clkobs_ctrl;

  if (mac_block == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_clkobs_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_clkobs_ctrl);
    reg = reg_base + (stride * (mac_block - 1));
  }

  lld_subdev_read_register(dev_id, subdev_id, reg, &serdes_clkobs_ctrl);
  serdes_clkobs_ctrl = ((clkdiv & 0x3ul) << 8) |
                       (serdes_clkobs_ctrl & (~(((uint32_t)0x3ul) << 8)));
  lld_subdev_write_register(dev_id, subdev_id, reg, serdes_clkobs_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_sel_clkdiv1(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t mac_block,
                                             uint32_t clkdiv) {
  uint32_t reg, serdes_clkobs_ctrl;

  if (mac_block == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_clkobs_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_clkobs_ctrl);
    reg = reg_base + (stride * (mac_block - 1));
  }

  lld_subdev_read_register(dev_id, subdev_id, reg, &serdes_clkobs_ctrl);
  serdes_clkobs_ctrl = ((clkdiv & 0x3ul) << 24) |
                       (serdes_clkobs_ctrl & (~(((uint32_t)0x3ul) << 24)));
  lld_subdev_write_register(dev_id, subdev_id, reg, serdes_clkobs_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_ena_clkpad0(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t val) {
  uint32_t clkpad_ctrl;

  lld_subdev_read_register(
      dev_id,
      subdev_id,
      tof3_reg_device_select_misc_all_regs_misc_regs_clkpad_ctrl_address,
      &clkpad_ctrl);

  clkpad_ctrl =
      ((val & 0x1ul) << 2) | (clkpad_ctrl & (~(((uint32_t)0x1ul) << 2)));

  lld_subdev_write_register(
      dev_id,
      subdev_id,
      tof3_reg_device_select_misc_all_regs_misc_regs_clkpad_ctrl_address,
      clkpad_ctrl);

  return BF_SUCCESS;
}

bf_status_t port_mgr_clkobs_ctrl_ena_clkpad1(bf_dev_id_t dev_id,
                                             bf_subdev_id_t subdev_id,
                                             uint32_t val) {
  uint32_t clkpad_ctrl;

  lld_subdev_read_register(
      dev_id,
      subdev_id,
      tof3_reg_device_select_misc_all_regs_misc_regs_clkpad_ctrl_address,
      &clkpad_ctrl);

  clkpad_ctrl =
      ((val & 0x1ul) << 6) | (clkpad_ctrl & (~(((uint32_t)0x1ul) << 6)));

  lld_subdev_write_register(
      dev_id,
      subdev_id,
      tof3_reg_device_select_misc_all_regs_misc_regs_clkpad_ctrl_address,
      clkpad_ctrl);

  return BF_SUCCESS;
}

/** \brief tof-3 clkobs pad config
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param pad: BF_CLKOBS_PAD_0 or BF_CLKOBS_PAD_1
 * \param clk_src: BF_SDS_NONE_CLK or BF_SDS_RX_RECOVEREDCLK or BF_SDS_TX_CLK
 * \param divider: 0,1,2, 3 for div by 2,4,8,16 respectively
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 * \return: BF_HW_UPDATE_FAILED: failure to apply config
 */
bf_status_t port_mgr_tof3_serdes_clkobs_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            bf_clkobs_pad_t pad,
                                            bf_sds_clkobs_clksel_t clk_src,
                                            int divider) {
  uint32_t mac_block, val, daisy_sel;
  uint32_t clkobs, misc_clkpad_ctrl;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t phys_tx_ln, phys_rx_ln;
  bf_status_t rc;

  if (divider < 0 || divider > 3) {
    return BF_INVALID_ARG;
  }
  if ((pad != BF_CLKOBS_PAD_0) && (pad != BF_CLKOBS_PAD_1)) {
    return BF_INVALID_ARG;
  }

  rc = map_dev_port_to_macro(
      dev_id, dev_port, (uint32_t *)&subdev_id, &mac_block);
  if (rc != BF_SUCCESS) return rc;

  if (mac_block >= 17 && mac_block <= 32) {
    daisy_sel = 1;
  } else if (mac_block >= 1 && mac_block <= 16) {
    daisy_sel = 0;
  } else {
    daisy_sel = 2;
  }

  // the serdes lane registers are indexed by physical Tx-lane
  // so we need to retrieve the map
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_TX);
  phys_tx_ln = tf3_sd->physical_tx_lane;
  phys_rx_ln = tf3_sd->physical_rx_lane;

  val = 0;
  switch (clk_src) {
    case BF_SDS_RX_RECOVEREDCLK:
      clkobs = phys_rx_ln;
      break;
    case BF_SDS_TX_CLK:
      clkobs = phys_tx_ln + 8;
      break;
    case BF_SDS_NONE_CLK:
      /* just deselect this MAC from driving the clk daisy chain */
      if (pad == BF_CLKOBS_PAD_1) {
        port_mgr_clkobs_ctrl_sel_clkena1(dev_id, subdev_id, mac_block, val);
      } else {
        port_mgr_clkobs_ctrl_sel_clkena0(dev_id, subdev_id, mac_block, val);
      }
      return BF_SUCCESS;
    default:
      return BF_INVALID_ARG;
  }

  // deselect any previous setting that might be driving clkobd_pad
  for (uint i = 1; i <= 32; i++) {
    if (i != mac_block) {
      if (pad == BF_CLKOBS_PAD_1) {
        port_mgr_clkobs_ctrl_sel_clkena1(dev_id, subdev_id, i, val);
      } else {
        port_mgr_clkobs_ctrl_sel_clkena0(dev_id, subdev_id, i, val);
      }
    }
  }

  lld_subdev_read_register(
      dev_id,
      subdev_id,
      tof3_reg_device_select_misc_all_regs_misc_regs_clkpad_ctrl_address,
      &misc_clkpad_ctrl);

  val = 1;
  if (pad == BF_CLKOBS_PAD_1) {
    port_mgr_clkobs_ctrl_sel_clkobs1(dev_id, subdev_id, mac_block, clkobs);
    port_mgr_clkobs_ctrl_sel_clkdiv1(dev_id, subdev_id, mac_block, divider);
    port_mgr_clkobs_ctrl_sel_clkena1(dev_id, subdev_id, mac_block, val);
    misc_clkpad_ctrl &= ~(0x7UL << 4);
    misc_clkpad_ctrl |= ((daisy_sel | (1 << 2)) << 4) | (1 << 16);
  } else {
    port_mgr_clkobs_ctrl_sel_clkobs0(dev_id, subdev_id, mac_block, clkobs);
    port_mgr_clkobs_ctrl_sel_clkdiv0(dev_id, subdev_id, mac_block, divider);
    port_mgr_clkobs_ctrl_sel_clkena0(dev_id, subdev_id, mac_block, val);
    misc_clkpad_ctrl &= ~(0x7UL);
    misc_clkpad_ctrl |= (daisy_sel | (1 << 2)) | (1 << 16);
  }
  lld_subdev_write_register(
      dev_id,
      subdev_id,
      tof3_reg_device_select_misc_all_regs_misc_regs_clkpad_ctrl_address,
      misc_clkpad_ctrl);

  return BF_SUCCESS;
}
