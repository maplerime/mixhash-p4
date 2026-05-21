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

#ifndef TILE_SIM

#include <byteswap.h>
#include <target-sys/bf_sal/bf_sys_sem.h>
#include <bf_types/bf_types.h>

#include <dvm/bf_drv_intf.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <lld/lld_efuse.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_tof2_serdes_if.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr/port_mgr_intf.h"
#include "port_mgr/port_mgr.h"
#include "port_mgr/port_mgr_map.h"
#include "port_mgr_tof2_map.h"
#include <port_mgr/port_mgr_intf.h>
#include "port_mgr_tof2_port.h"
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr_logical_port.h>
#include <port_mgr/port_mgr_logical_dev.h>
#include <port_mgr/port_mgr_dev.h>
#include <port_mgr/port_mgr_physical_dev.h>
#include "autogen-required-headers.h"
#include "eth400g_mac_rspec_access.h"
#include "eth400g_pcs_rspec_access.h"

#include "credo_sd_access.h"
#endif  // NOT TILE_SIM
#include "port_mgr_tof2_serdes.h"
#include "port_mgr_tof2_serdes_defs.h"

bool bfn_sd_trace = false;
static uint32_t port_mgr_tof2_serdes_base_chnl_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port);

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_cmd_mutex
 ****************************************************************************/
bf_sys_mutex_t port_mgr_tof2_serdes_fw_cmd_mutex;

void port_mgr_tof2_serdes_fw_cmd_lock_init(void) {
  bf_sys_mutex_init(&port_mgr_tof2_serdes_fw_cmd_mutex);
}
static void port_mgr_tof2_serdes_fw_cmd_lock(void) {
  bf_sys_mutex_lock(&port_mgr_tof2_serdes_fw_cmd_mutex);
}
static void port_mgr_tof2_serdes_fw_cmd_unlock(void) {
  bf_sys_mutex_unlock(&port_mgr_tof2_serdes_fw_cmd_mutex);
}

/*****************************************************************************
 * port_mgr_tof2_bandgap_load
 ****************************************************************************/
#include "port_mgr_tof2_bandgap.c"

/*****************************************************************************
 * port_mgr_tof2_serdes_tile_rd
 *
 * Read a 16b value from a serdes register
 * NOTE: Need MAC_ID
 ****************************************************************************/
uint32_t port_mgr_tof2_serdes_tile_rd(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t ofs) {
  uint32_t mac_stn_id, bfn_addr, reg_val;
  port_mgr_err_t rc;
  uint32_t bfn_ofs = ((ofs & 0x7ff) | (((ofs >> 16) & 0xF) << 11)) << 2;
  lld_err_t lld_rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_stn_id, NULL, NULL);
  bf_sys_assert(rc == 0);  // should've been checked already

  // special-cases
  if ((mac_stn_id == 0) || (mac_stn_id == 39)) {
    if ((ofs >= 0xA0000) &&
        (ofs <= 0xA000F)) {  // CPU port (group8) FW dnld registers
      if (mac_stn_id == 0) {
        mac_stn_id = 35;
      } else {
        mac_stn_id = 37;
      }
    }
  } else if ((ofs >> 16) == 9) {  // group 9
    bf_dev_pipe_t logical_pipe_id = DEV_PORT_TO_PIPE(dev_port);
    bf_dev_pipe_t physical_pipe_id;

    lld_rc = lld_sku_map_pipe_id_to_phy_pipe_id(
        dev_id, logical_pipe_id, &physical_pipe_id);
    if (lld_rc != LLD_OK) {
      bf_sys_assert(0);
    }
    if (lld_efuse_get_die_rotated(dev_id)) {
      uint32_t __map[4] = {37, 38, 35, 36};
      mac_stn_id = __map[physical_pipe_id];
    } else {
      uint32_t __map[4] = {35, 36, 37, 38};
      mac_stn_id = __map[physical_pipe_id];
    }
  }

  bfn_addr = (3 << 24) | (mac_stn_id << 18) | bfn_ofs;
  lld_read_register(dev_id, bfn_addr, &reg_val);

  if (bfn_sd_trace)
    port_mgr_log("TRC : %d: %3d : --- : Rd : %08x : %08x",
                 dev_id,
                 dev_port,
                 bfn_addr,
                 reg_val);

  return reg_val;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tile_wr
 *
 * Write a 16b value to a serdes register
 ****************************************************************************/
void port_mgr_tof2_serdes_tile_wr(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  uint32_t ofs,
                                  uint32_t reg_val) {
  uint32_t mac_stn_id, bfn_addr;
  port_mgr_err_t rc;
  uint32_t bfn_ofs = ((ofs & 0x7ff) | (((ofs >> 16) & 0xF) << 11)) << 2;
  lld_err_t lld_rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_stn_id, NULL, NULL);
  bf_sys_assert(rc == 0);  // should've been checked already

  // special-cases
  if ((mac_stn_id == 0) || (mac_stn_id == 39)) {
    if ((ofs >= 0xA0000) &&
        (ofs <= 0xA000F)) {  // CPU port (group8) FW dnld registers
      if (mac_stn_id == 0) {
        mac_stn_id = 35;
      } else {
        mac_stn_id = 37;
      }
    }
  } else if ((ofs >> 16) == 9) {  // group 9
    bf_dev_pipe_t logical_pipe_id = DEV_PORT_TO_PIPE(dev_port);
    bf_dev_pipe_t physical_pipe_id = 0;

    lld_rc = lld_sku_map_pipe_id_to_phy_pipe_id(
        dev_id, logical_pipe_id, &physical_pipe_id);
    if (lld_rc != LLD_OK) {
      bf_sys_assert(0);
    }
    if (lld_efuse_get_die_rotated(dev_id)) {
      uint32_t __map[4] = {37, 38, 35, 36};
      mac_stn_id = __map[physical_pipe_id];
    } else {
      uint32_t __map[4] = {35, 36, 37, 38};
      mac_stn_id = __map[physical_pipe_id];
    }
  }

  bfn_addr = (3 << 24) | (mac_stn_id << 18) | bfn_ofs;

  if (bfn_sd_trace)
    port_mgr_log("TRC : %d: %3d : --- : Wr : %08x : %08x",
                 dev_id,
                 dev_port,
                 bfn_addr,
                 reg_val);

  lld_write_register(dev_id, bfn_addr, reg_val);
}

/*****************************************************************************
 * port_mgr_tof2_serdes_hw_mode_get
 *
 * Emergency back-up to port_mgr_tof2_serdes_mode_is_nrz/pam4 as it seems
 * FW takes some time to "realize" a lane is in a certain mode
 ****************************************************************************/
bf_serdes_encoding_mode_t port_mgr_tof2_serdes_hw_mode_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln) {
  uint32_t reg32 = 0;
  uint32_t tx_nrz_mode, pam4_en;

  credo_tx_0x41_pam4_en_get(dev_id, dev_port, ln, &reg32, &pam4_en, true);
  if (pam4_en) {
    return BF_SERDES_ENC_MODE_PAM4;
  }
  credo_tx_0xb0_tx_nrz_mode_get(
      dev_id, dev_port, ln, &reg32, &tx_nrz_mode, true);
  if (tx_nrz_mode) {
    return BF_SERDES_ENC_MODE_NRZ;
  }
  // return BF_SERDES_ENC_MODE_NONE;
  return BF_SERDES_ENC_MODE_NRZ;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_mode_is_nrz
 *
 * Determine encoding mode for the dev_port, PAM4 or NRZ
 ****************************************************************************/
bool port_mgr_tof2_serdes_mode_is_nrz(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t ln) {
  bf_serdes_encoding_mode_t enc_mode;
  uint32_t unused_G;
  bf_status_t rc;

  // first try based on logical port config
  rc = bf_port_encoding_mode_get(dev_id, dev_port, &enc_mode);
  if (rc == BF_SUCCESS) {
    if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
      return true;
    } else if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
      return false;
    }
  }
  // if SW config doesnt identify encoding mode, check current HW cfg
  port_mgr_tof2_serdes_fw_lane_speed_get(
      dev_id, dev_port, ln, &unused_G, &enc_mode);
  if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
    return true;
  } else {
    enc_mode = port_mgr_tof2_serdes_hw_mode_get(dev_id, dev_port, ln);
    if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
      return true;
    } else {
      return false;
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_serdes_mode_is_pam4
 *
 * Determine encoding mode for the dev_port, PAM4 or NRZ
 ****************************************************************************/
bool port_mgr_tof2_serdes_mode_is_pam4(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln) {
  bf_serdes_encoding_mode_t enc_mode;
  uint32_t unused_G;
  bf_status_t rc;

  // first try based on logical port config
  rc = bf_port_encoding_mode_get(dev_id, dev_port, &enc_mode);
  if (rc == BF_SUCCESS) {
    if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
      return true;
    } else if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
      return false;
    }
  }
  // if SW config doesnt identify encoding mode, check current HW cfg
  port_mgr_tof2_serdes_fw_lane_speed_get(
      dev_id, dev_port, ln, &unused_G, &enc_mode);
  if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
    return true;
  } else {
    enc_mode = port_mgr_tof2_serdes_hw_mode_get(dev_id, dev_port, ln);
    if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
      return true;
    } else {
      return false;
    }
  }
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_map_set
 *
 * Cache the serdes lane map in the physical_dev and optionally
 * program the serdes octal lane map into hw
 ****************************************************************************/
bf_status_t port_mgr_tof2_serdes_lane_map_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t tile_tx_ln[8],
                                              uint32_t tile_rx_ln[8]) {
  uint32_t reg32 = 0;
  bool is_cpu_port;
  uint32_t rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, NULL, NULL, &is_cpu_port);
  bf_sys_assert(rc == 0);

  credo_group8_0x0_tx_l_0_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[0]);
  credo_group8_0x0_tx_l_1_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[1]);
  credo_group8_0x0_tx_l_2_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[2]);
  credo_group8_0x0_tx_l_3_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[3]);
  if (!is_cpu_port) {
    credo_group8_0x1_tx_l_4_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[4]);
    credo_group8_0x1_tx_l_5_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[5]);
    credo_group8_0x1_tx_l_6_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[6]);
    credo_group8_0x1_tx_l_7_sel_rmw(dev_id, dev_port, 0, &reg32, tile_tx_ln[7]);
  }

  credo_group8_0x2_rx_l_0_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[0]);
  credo_group8_0x2_rx_l_1_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[1]);
  credo_group8_0x2_rx_l_2_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[2]);
  credo_group8_0x2_rx_l_3_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[3]);
  if (!is_cpu_port) {
    credo_group8_0x3_rx_l_4_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[4]);
    credo_group8_0x3_rx_l_5_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[5]);
    credo_group8_0x3_rx_l_6_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[6]);
    credo_group8_0x3_rx_l_7_sel_rmw(dev_id, dev_port, 0, &reg32, tile_rx_ln[7]);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_map_get
 *
 * Return the programmed serdes octal lane map
 ****************************************************************************/
bf_status_t port_mgr_tof2_serdes_lane_map_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t phys_tx_ln[8],
                                              uint32_t phys_rx_ln[8]) {
  uint32_t reg32 = 0;
  bool is_cpu_port;
  uint32_t rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, NULL, NULL, &is_cpu_port);
  if (rc != PORT_MGR_OK) return BF_INVALID_ARG;

  credo_group8_0x0_tx_l_0_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_tx_ln[0], true);
  credo_group8_0x0_tx_l_1_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_tx_ln[1], false);
  credo_group8_0x0_tx_l_2_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_tx_ln[2], false);
  credo_group8_0x0_tx_l_3_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_tx_ln[3], false);
  if (!is_cpu_port) {
    credo_group8_0x1_tx_l_4_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_tx_ln[4], true);
    credo_group8_0x1_tx_l_5_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_tx_ln[5], false);
    credo_group8_0x1_tx_l_6_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_tx_ln[6], false);
    credo_group8_0x1_tx_l_7_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_tx_ln[7], false);
  }

  credo_group8_0x2_rx_l_0_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_rx_ln[0], true);
  credo_group8_0x2_rx_l_1_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_rx_ln[1], false);
  credo_group8_0x2_rx_l_2_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_rx_ln[2], false);
  credo_group8_0x2_rx_l_3_sel_get(
      dev_id, dev_port, 0, &reg32, &phys_rx_ln[3], false);
  if (!is_cpu_port) {
    credo_group8_0x3_rx_l_4_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_rx_ln[4], true);
    credo_group8_0x3_rx_l_5_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_rx_ln[5], false);
    credo_group8_0x3_rx_l_6_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_rx_ln[6], false);
    credo_group8_0x3_rx_l_7_sel_get(
        dev_id, dev_port, 0, &reg32, &phys_rx_ln[7], false);
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_cpu_reset
 *
 *def soft_reset():
 *    chip.GROUP8_TOP[0].GROUP8_SW_RSTB_MAGIC = 0xAAA
 *    chip.GROUP8_TOP[0].GROUP8_SW_RSTB_MAGIC = 0x0
 *********************************************************************/
bf_status_t port_mgr_tof2_serdes_cpu_reset(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int ln) {
  uint32_t reg32 = 0;

  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_CPU_RESET);
  bf_sys_usleep(1000);
  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_NONE);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_soft_reset
 *
 *def soft_reset():
 *    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x888
 *    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x0
 *********************************************************************/
bf_status_t port_mgr_tof2_serdes_soft_reset(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            int ln) {
  uint32_t reg32 = 0;

  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_SOFT_RESET);
  bf_sys_usleep(1000);
  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_NONE);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_logic_reset
 *
 *def logic_reset():
 *    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x777
 *    chip.GROUP8_TOP[0].Reg0013_11_0 = 0x0
 *****************************************************************************/
bf_status_t port_mgr_tof2_serdes_logic_reset(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int ln) {
  uint32_t reg32;

  // reset with pam4_en=1
  credo_tx_0x41_pam4_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  bf_sys_usleep(1000);

  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_LOGIC_RESET);
  bf_sys_usleep(1000);
  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_NONE);

  // reset with pam4_en=0 (NRZ HW)
  credo_tx_0x41_pam4_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  bf_sys_usleep(1000);

  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_LOGIC_RESET);
  bf_sys_usleep(1000);
  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, ln, &reg32, CRDO_MAGIC_VAL_NONE);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_group_reset
 *
 * Note: Resets all lanes in the group regardless the "ln" specified.
 *
 *def group_reset(lane=None,t=0.1):
 *    soft_reset()
 *    time.sleep(t)
 *
 *    logic_reset()
 *    time.sleep(t)
 *
 *    chip.ANLT_TOP[lane].Tr_Reg0000_1 = 0
 *    chip.ANLT_TOP[lane].Tr_Reg0000_0 = 0
 *****************************************************************************/
bf_status_t port_mgr_tof2_serdes_group_reset(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int ln,
                                             int phase) {
  uint32_t reg32;

  if (phase == 0) {
    port_mgr_tof2_serdes_soft_reset(dev_id, dev_port, ln);
  } else if (phase == 1) {
    port_mgr_tof2_serdes_logic_reset(dev_id, dev_port, ln);
  } else if (phase == 2) {
    for (ln = 0; ln < 8; ln++) {
      credo_link_trng_0x0_training_en_rmw(dev_id, dev_port, ln, &reg32, 0);
      credo_link_trng_0x0_training_restart_sc_rmw(
          dev_id, dev_port, ln, &reg32, 0);
    }
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_reset_nrz_set
 *
 *def lr_nrz(lane_obj):
 *    lane_obj.TARGET_CTRL = 0x100
 *    lane_obj.NRZ_SM_RESET = 0x1
 *    time.sleep(.050)
 *    lane_obj.NRZ_SM_RESET = 0x0
 *    lane_obj.TARGET_CTRL = 0x002
 *****************************************************************************/
bf_status_t port_mgr_tof2_serdes_lane_reset_nrz_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    int ln) {
  uint32_t reg32 = 0;

  credo_rx_0x2_target_ctrl_rmw(dev_id, dev_port, ln, &reg32, 0x100);
  credo_rx_0x81_nrz_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 0x1);
  bf_sys_usleep(50000);
  credo_rx_0x81_nrz_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 0x0);
  credo_rx_0x2_target_ctrl_rmw(dev_id, dev_port, ln, &reg32, 0x002);
  return BF_SUCCESS;
}

/*****************************************************************************
* port_mgr_tof2_serdes_lane_reset_pam4_set
*
def lr_pam4(lane_obj):
    super_cal = lane_obj.MU_OFFSET_OW
    if super_cal != 0:
        lane_obj.MU_OFFSET_OWEN = 1
        lane_obj.MU_OFFSET_OW = 0
    updn  = (lane_obj.THETA2_UPDATE_MODE << 2)
    updn += (lane_obj.THETA3_UPDATE_MODE << 1)
    updn += (lane_obj.THETA4_UPDATE_MODE)
    if updn != 0:
        lane_obj.THETA2_UPDATE_MODE = 0
        lane_obj.THETA3_UPDATE_MODE = 0
        lane_obj.THETA4_UPDATE_MODE = 0
    lane_obj.Reg01D5_8 = 0
    lane_obj.THETA2_UPDATE_MODE = 0
    lane_obj.THETA3_UPDATE_MODE = 0
    lane_obj.THETA4_UPDATE_MODE = 0
    lane_obj.PAM4_SM_RESET = 0x1
    time.sleep(.050)
    lane_obj.PAM4_SM_RESET = 0x0

    if updn != 0:
        lane_obj.THETA2_UPDATE_MODE = (updn >> 2)
        lane_obj.THETA3_UPDATE_MODE = (updn >> 1) & 0x1
        lane_obj.THETA4_UPDATE_MODE = (updn & 0x1)
    lane_obj.MU_OFFSET_OWEN = 1
    if super_cal != 0:
        lane_obj.MU_OFFSET_OW = super_cal
    lane_obj.Reg01D5_8 = 1
*****************************************************************************/
bf_status_t port_mgr_tof2_serdes_lane_reset_pam4_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     int ln) {
  uint32_t reg32 = 0;
  uint32_t super_cal, t2, t3, t4, updn, reg41;

  credo_tx_0x87_mu_offset_ow_get(
      dev_id, dev_port, ln, &reg32, &super_cal, true);
  if (super_cal) {
    credo_tx_0x87_mu_offset_owen_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_tx_0x87_mu_offset_ow_rmw(dev_id, dev_port, ln, &reg32, 0);
  }
  credo_tx_0x41_theta2_update_mode_get(dev_id, dev_port, ln, &reg32, &t2, true);
  credo_tx_0x41_theta3_update_mode_get(
      dev_id, dev_port, ln, &reg32, &t3, false);
  credo_tx_0x41_theta4_update_mode_get(
      dev_id, dev_port, ln, &reg32, &t4, false);
  updn = (((t2 << 1) | t3) << 1) | t4;
  if (updn) {
    credo_tx_0x41_theta2_update_mode_set(
        dev_id, dev_port, ln, &reg32, 0, false);
    credo_tx_0x41_theta3_update_mode_set(
        dev_id, dev_port, ln, &reg32, 0, false);
    credo_tx_0x41_theta4_update_mode_set(dev_id, dev_port, ln, &reg32, 0, true);
  }
  reg41 = reg32;  // save

  //    lane_obj.Reg01D5_8 = 0
  credo_rx_0xd5_blwc_en_rmw(dev_id, dev_port, ln, &reg32, 0);

  reg32 = reg41;  // restore
  credo_tx_0x41_theta2_update_mode_set(dev_id, dev_port, ln, &reg32, 0, false);
  credo_tx_0x41_theta3_update_mode_set(dev_id, dev_port, ln, &reg32, 0, false);
  credo_tx_0x41_theta4_update_mode_set(dev_id, dev_port, ln, &reg32, 0, true);
  reg41 = reg32;  // save again

  credo_tx_0x0_pam4_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 1);
  bf_sys_usleep(50000);
  credo_tx_0x0_pam4_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
  if (updn) {
    reg32 = reg41;  // restore again
    credo_tx_0x41_theta2_update_mode_set(
        dev_id, dev_port, ln, &reg32, t2, false);
    credo_tx_0x41_theta3_update_mode_set(
        dev_id, dev_port, ln, &reg32, t3, false);
    credo_tx_0x41_theta4_update_mode_set(
        dev_id, dev_port, ln, &reg32, t4, true);
  }
  credo_tx_0x87_mu_offset_owen_rmw(dev_id, dev_port, ln, &reg32, 1);
  if (super_cal) {
    credo_tx_0x87_mu_offset_ow_rmw(dev_id, dev_port, ln, &reg32, super_cal);
  }
  // lane_obj.Reg01D5_8 = 1
  credo_rx_0xd5_blwc_en_rmw(dev_id, dev_port, ln, &reg32, 1);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_reset_set
 *
 *****************************************************************************/
bf_status_t port_mgr_tof2_serdes_lane_reset_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                int ln) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_lane_reset_nrz_set(dev_id, dev_port, ln);
  } else {
    port_mgr_tof2_serdes_lane_reset_pam4_set(dev_id, dev_port, ln);
  }
  return BF_SUCCESS;
}
/*****************************************************************************
 * port_mgr_tof2_serdes_fw_cmd_unprotected
 *
 * Lower level fun to perform actual FW cmd operation
 * Callers are assumed to have locked (shared) FW cmd
 * registers.
 *
 *def fw_cmd(cmd=0x0000, print_en=False):
 *    if print_en: print("Writing command : %d" % cmd)
 *    chip.MdioWr(0x40C1, cmd)
 *    loop_cnt = 0
 *    while (chip.MdioRd(0x40C1) == cmd):
 *        loop_cnt += 1
 *        if (loop_cnt > 1000):
 *            break
 *        continue
 *    return (chip.MdioRd(0x40C1) >> 8)
 */
bf_status_t port_mgr_tof2_serdes_fw_cmd_rtn_data_unprotected(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    int ln,
    uint32_t cmd,
    uint32_t *rtn_data,
    uint32_t detail) {
  uint32_t reg32 = 0, fld_val = 0, retries = 100;

  if (rtn_data) *rtn_data = 0;

  credo_group8_0xc2_fw_cmd_detail_set(
      dev_id, dev_port, ln, &reg32, detail, true);

  credo_group8_0xc1_fw_cmd_set(dev_id, dev_port, ln, &reg32, cmd, true);
  while (--retries != 0) {
    reg32 = 0;
    credo_group8_0xc1_fw_cmd_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val != cmd) break;
    // bf_sys_usleep(10000);
    bf_sys_usleep(100);
  }
  if (rtn_data) *rtn_data = fld_val;
  if (fld_val != cmd) return BF_SUCCESS;

  port_mgr_log("%d:%3d:%d : FW error: cmd=%04x : detail=%04x : rsp=%04x",
               dev_id,
               dev_port,
               ln,
               cmd,
               detail,
               fld_val);
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_cmd_unprotected
 *
 * Lower level fun to perform actual FW cmd operation
 * Callers are assumed to have locked (shared) FW cmd
 * registers.
 *
 *def fw_cmd(cmd=0x0000, print_en=False):
 *    if print_en: print("Writing command : %d" % cmd)
 *    chip.MdioWr(0x40C1, cmd)
 *    loop_cnt = 0
 *    while (chip.MdioRd(0x40C1) == cmd):
 *        loop_cnt += 1
 *        if (loop_cnt > 1000):
 *            break
 *        continue
 *    return (chip.MdioRd(0x40C1) >> 8)
 */
bf_status_t port_mgr_tof2_serdes_fw_cmd_unprotected(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    int ln,
                                                    uint32_t cmd,
                                                    uint32_t *rsp,
                                                    uint32_t detail) {
  uint32_t reg32 = 0, fld_val = 0, retries = 100;

  if (rsp) *rsp = 0;

  credo_group8_0xc2_fw_cmd_detail_set(
      dev_id, dev_port, ln, &reg32, detail, true);

  credo_group8_0xc1_fw_cmd_set(dev_id, dev_port, ln, &reg32, cmd, true);
  while (--retries != 0) {
    reg32 = 0;
    credo_group8_0xc1_fw_cmd_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val != cmd) break;
    // bf_sys_usleep(10000);
    bf_sys_usleep(100);
  }
  if (rsp) *rsp = fld_val >> 8;
  if (fld_val != cmd) return BF_SUCCESS;

  port_mgr_log("%d:%3d:%d : FW error: cmd=%04x : detail=%04x : rsp=%04x",
               dev_id,
               dev_port,
               ln,
               cmd,
               detail,
               fld_val);
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_cmd
 */
bf_status_t port_mgr_tof2_serdes_fw_cmd(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int ln,
                                        uint32_t cmd,
                                        uint32_t *rsp) {
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();
  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, ln, cmd, rsp, 0);
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_cmd_w_detail
 */
bf_status_t port_mgr_tof2_serdes_fw_cmd_w_detail(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 int ln,
                                                 uint32_t cmd,
                                                 uint32_t *rsp,
                                                 uint32_t detail) {
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();
  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, ln, cmd, rsp, detail);
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_cmd_w_rtn_data
 */
bf_status_t port_mgr_tof2_serdes_fw_cmd_w_rtn_data(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   int ln,
                                                   uint32_t cmd,
                                                   uint32_t *rsp,
                                                   uint32_t detail) {
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();
  rc = port_mgr_tof2_serdes_fw_cmd_rtn_data_unprotected(
      dev_id, dev_port, ln, cmd, rsp, detail);
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_debug_cmd
 *
 * def fw_debug_cmd(section=2, index=7, lane=0):
 *    timeout = 0.2
 *    result = 0
 *     cmd = 0xB000 + ((section & 0xf) << 4) + lane
 *    chip.MdioWr(0x40C2, index)  # fw_cmd_detail_addr = 0x40C2
 *    status = fw_cmd(cmd)  # fw_cmd_addr = 0x40C1
 *    if status != 0xB:
 *        # print("FW Debug CMD Section %d, Index %d, for Lane %s failed with
 *code 0x%04x" %(section, index, lane_name_list[lane],status))
 *        result = -1
 *    else:
 *        result = chip.MdioRd(0x40C2)  # fw_cmd_detail_addr = 0x40C2
 *
 *    return result
 */
bf_status_t port_mgr_tof2_serdes_fw_debug_cmd(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              int ln,
                                              uint32_t section,
                                              uint32_t index,
                                              uint32_t *result) {
  uint32_t cmd;
  uint32_t status;
  uint32_t reg32 = 0;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();

  cmd = 0xB000 + ((section & 0xf) << 4) + base_ln + ln;
  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, ln, cmd, &status, index);
  if (rc != BF_SUCCESS) {
    *result = 0;
  } else if (status != 0xB) {
    *result = 0;
    rc = BF_INVALID_ARG;
  } else {
    credo_group8_0xc2_fw_cmd_detail_get(
        dev_id, dev_port, ln, &reg32, result, true);
  }
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_reg_wr
 *
 *        chip.MdioWr(0x40C2, addr_list[0])  # fw_cmd_detail_addr = 0x9807
 *        chip.MdioWr(0x40C4, data)  # fw_cmd_status_addr = 0x98C7
 *        cmd_status = fw_cmd(0xe020)  # fw_cmd_addr = 0x9806
 *        if cmd_status != 0x000e:
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_reg_wr(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           int ln,
                                           uint32_t addr,
                                           uint32_t data) {
  uint32_t reg32 = 0, status;
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();

  credo_group8_0xc4_fw_reg_value_set(dev_id, dev_port, ln, &reg32, data, true);
  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, ln, 0xe020, &status, addr);
  if (rc != BF_SUCCESS) {
  } else if (status != 0x000e) {
    rc = BF_INVALID_ARG;
  }
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_reg_section_wr
 *
 *        chip.MdioWr(0x40C2, addr_list[0])  # fw_cmd_detail_addr = 0x9807
 *        chip.MdioWr(0x40C4, data)  # fw_cmd_status_addr = 0x98C7
 *        cmd_status = fw_cmd(0xe020)  # fw_cmd_addr = 0x9806
 *        if cmd_status != 0x000e:
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_reg_section_wr(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   int ln,
                                                   uint32_t addr,
                                                   uint32_t data,
                                                   uint32_t section) {
  uint32_t reg32 = 0, status;
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();

  credo_group8_0xc4_fw_reg_value_set(dev_id, dev_port, ln, &reg32, data, true);
  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, ln, 0xe020 + section, &status, addr);
  if (rc != BF_SUCCESS) {
  } else if (status != 0x000e) {
    rc = BF_INVALID_ARG;
  }
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_reg_section_rd
 *
 *  fw_cmd(0xe010+section, detail=reg_addr, expected_response=0xe)
 *  result[reg_addr] = chip.GROUP8_TOP[0].FW_REG_VALUE
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_reg_section_rd(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   int ln,
                                                   uint32_t addr,
                                                   uint32_t *data,
                                                   uint32_t section) {
  uint32_t reg32 = 0, status;
  bf_status_t rc;

  port_mgr_tof2_serdes_fw_cmd_lock();

  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, ln, 0xe010 + section, &status, addr);
  if (rc != BF_SUCCESS) {
  } else if (status != 0x000e) {
    rc = BF_INVALID_ARG;
  } else {
    credo_group8_0xc4_fw_reg_value_get(
        dev_id, dev_port, ln, &reg32, data, true);
  }
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_adapt_cnt_get
 *
 * def fw_adapt_cnt(group=None, lane=None):
 *    if not fw_loaded(print_en=0):
 *        result = -1
 *
 *    if gEncodingMode[group][lane][0].upper == 'NRZ':
 *        result = fw_debug_cmd(section=1, index=10, lane=lane)
 *    else:
 *        result = fw_debug_cmd(section=2, index=7, lane=lane)
 *
 *    return result
 */
bf_status_t port_mgr_tof2_serdes_fw_adapt_cnt_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  int ln,
                                                  uint32_t *cnt) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 1, 10, cnt);
  } else {
    port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 2, 7, cnt);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_readapt_cnt_get
 *
 *def fw_readapt_cnt(lane=None):
 *    if not fw_loaded(print_en=0):
 *        result = -1
 *    result = fw_debug_cmd(section=8, index=1, lane=lane)
 *
 *    return result
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_readapt_cnt_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    int ln,
                                                    uint32_t *cnt) {
  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 8, 1, cnt);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_link_lost_cnt_get
 *
 *def fw_link_lost_cnt(lane=None):
 *    if not fw_loaded(print_en=0):
 *        result = -1
 *    result = fw_debug_cmd(section=8, index=1, lane=lane)
 *
 *    return result
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_link_lost_cnt_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      int ln,
                                                      uint32_t *cnt) {
  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 8, 0, cnt);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_lane_speed_get
 *
 *def fw_lane_speed(lane=None):
 *    # [ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  0x08,  0x09, 0x0A ]
 *    speed_list = ['OFF', '10G', '20G', '25G', '26G', '28G', '50G', '07?',
 *'53G', '53G', '1G']
 *    mode_list = ['off', 'nrz', 'nrz', 'nrz', 'nrz', 'nrz', 'pam4', '07?',
 *'pam4', 'pam4', 'nrz']
 *    result = {}
 *    speed_index = (fw_debug_cmd(section=0, index=4, lane=lane) & 0xf)
 *    # speed_index = 3
 *    result = [mode_list[speed_index], speed_list[speed_index]]
 *    # result[ln] = speed_index
 *    return result
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_lane_speed_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    int ln,
    uint32_t *G,
    bf_serdes_encoding_mode_t *enc_mode) {
  uint32_t speed_index, speed_index_raw;
  uint32_t speed_list[] = {0, 10, 20, 25, 26, 28, 50, 0, 53, 53, 1};
  bf_serdes_encoding_mode_t mode_list[] = {
      BF_SERDES_ENC_MODE_NONE,
      BF_SERDES_ENC_MODE_NRZ,
      BF_SERDES_ENC_MODE_NRZ,
      BF_SERDES_ENC_MODE_NRZ,
      BF_SERDES_ENC_MODE_NRZ,
      BF_SERDES_ENC_MODE_NRZ,
      BF_SERDES_ENC_MODE_PAM4,
      BF_SERDES_ENC_MODE_NONE,
      BF_SERDES_ENC_MODE_PAM4,
      BF_SERDES_ENC_MODE_PAM4,
      BF_SERDES_ENC_MODE_NRZ,
  };
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_debug_cmd(
      dev_id, dev_port, ln, 0, 4, &speed_index_raw);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d:%d : fw_debug_cmd error : %d : getting lane speed",
                 dev_id,
                 dev_port,
                 ln,
                 rc);
  }
  speed_index = speed_index_raw & 0xF;
  if (speed_index < (sizeof(speed_list) / sizeof(speed_list[0]))) {
    *G = speed_list[speed_index];
    *enc_mode = mode_list[speed_index];
  } else {
    *G = 0;
    *enc_mode = BF_SERDES_ENC_MODE_NONE;
    return BF_INVALID_ARG;
  }

  return BF_SUCCESS;
}

/*****************************************************************************
def fw_ver(print_en=False):
    fw_cmd(cmd=0xF003, expected_response=0xf)
    high_word = chip.GROUP8_TOP[0].FW_CMD
    low_word = chip.GROUP8_TOP[0].FW_CMD_DETAIL
    ver_code = (high_word << 16) + low_word
    if print_en: print(
            "\n...FW Version : %02d.%02d.%02d\n" % (ver_code >> 8 & 0xFF,
ver_code >> 8 & 0xFF, ver_code & 0xFF))

    return ver_code
*/
bf_status_t port_mgr_tof2_serdes_fw_ver_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *fw_ver) {
  bf_status_t rc;
  uint32_t rsp;

  port_mgr_tof2_serdes_fw_cmd_lock();

  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, 0, 0xF003, &rsp, 0);
  if (rc == BF_SUCCESS) {
    uint32_t reg32 = 0, high_word, low_word;

    credo_group8_0xc1_fw_cmd_get(
        dev_id, dev_port, ln, &reg32, &high_word, true);
    credo_group8_0xc2_fw_cmd_detail_get(
        dev_id, dev_port, ln, &reg32, &low_word, true);

    if (fw_ver) *fw_ver = (high_word << 16) + low_word;
  }
  port_mgr_tof2_serdes_fw_cmd_unlock();

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_hash_get
 *
 * def fw_hash(print_en=False):
 *     chip.MdioWr(0x40C2, 0x0)
 *     fw_cmd(cmd=0xF000)
 *     high_word = chip.MdioRd(0x40C1) & 0xFF  # upper byte, only 8 bits are
 *valid
 *     low_word = chip.MdioRd(0x40C2)  # lower word
 *     hash_code = (high_word << 16) + low_word
 *     if print_en: print("\n...FW Hash Code : 0x%06X\n" % (hash_code))
 *     return hash_code
 */
bf_status_t port_mgr_tof2_serdes_fw_hash_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *hash_code) {
  bf_status_t rc;
  uint32_t rsp;

  port_mgr_tof2_serdes_fw_cmd_lock();

  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, 0, 0xF000, &rsp, 0);
  if (rc == BF_SUCCESS) {
    uint32_t reg32 = 0, fld_val, high_word, low_word;

    credo_group8_0xc1_fw_cmd_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
    high_word = fld_val & 0xff;

    credo_group8_0xc2_fw_cmd_detail_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    low_word = fld_val;

    if (hash_code) *hash_code = (high_word << 16) + low_word;
  }
  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_crc_get
 *
 * def fw_crc(print_en=False):
 *     fw_cmd(cmd=0xF001)
 *     crc_code = chip.MdioRd(0x40C2)
 *     if print_en: print("\n...FW CRC Code : 0x%06X\n" % (crc_code))
 *     return crc_code
 */
bf_status_t port_mgr_tof2_serdes_fw_crc_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *crc) {
  bf_status_t rc;
  uint32_t rsp;

  port_mgr_tof2_serdes_fw_cmd_lock();

  rc = port_mgr_tof2_serdes_fw_cmd_unprotected(
      dev_id, dev_port, 0, 0xF001, &rsp, 0);
  if (rc == BF_SUCCESS) {
    uint32_t reg32 = 0, fld_val;

    credo_group8_0xc2_fw_cmd_detail_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (crc) *crc = fld_val;
  }

  port_mgr_tof2_serdes_fw_cmd_unlock();
  return rc;
}
/*****************************************************************************
 * port_mgr_tof2_serdes_fw_file_info_get
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_file_info_get(unsigned char *buf_ptr,
                                                  uint32_t *hash_code,
                                                  uint32_t *crc) {
  uint32_t start = 4096;
  /*
    start = 4096
    file_hash_code = struct.unpack_from('>I', fw_data[start:start + 4])[0]
    file_crc_code = struct.unpack_from('>H', fw_data[start + 4:start + 6])[0]
  */
  *hash_code = bswap_32(*((uint32_t *)(buf_ptr + start)));
  *crc = bswap_16(*((uint16_t *)(buf_ptr + start + 4)));
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_running_info_get
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_running_info_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t *running_hash_code,
    uint32_t *running_crc) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_hash_get(dev_id, dev_port, 0, running_hash_code);
  if (rc == BF_SUCCESS) {
    rc = port_mgr_tof2_serdes_fw_crc_get(dev_id, dev_port, 0, running_crc);
  }
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_loaded_get
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_loaded_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               bool *loaded) {
  bf_status_t rc;
  uint32_t running_hash_code, running_crc;

  rc = port_mgr_tof2_serdes_fw_running_info_get(
      dev_id, dev_port, &running_hash_code, &running_crc);
  if ((rc != BF_SUCCESS) || (running_hash_code == 0) || (running_crc == 0)) {
    *loaded = false;
  } else {
    *loaded = true;
  }
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_load_from_buffer
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_load_from_buffer(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     unsigned char *buf_ptr) {
  uint32_t file_hash_code, entry_point, length, ram_addr;
  uint16_t file_crc_code, file_date_code;
  uint16_t *data_ptr;
  uint32_t sections, reg32 = 0;
  uint16_t check_sum;
  uint32_t i, j, start, data_ofs, status;
  int max_tries, try
    ;
  /*
    start = 4096
    file_hash_code = struct.unpack_from('>I', fw_data[start:start + 4])[0]
    file_crc_code = struct.unpack_from('>H', fw_data[start + 4:start + 6])[0]
    file_date_code = struct.unpack_from('>H', fw_data[start + 6:start + 8])[0]
    entryPoint = struct.unpack_from('>I', fw_data[start + 8:start + 12])[0]
    length = struct.unpack_from('>I', fw_data[start + 12:start + 16])[0]
    ramAddr = struct.unpack_from('>I', fw_data[start + 16:start + 20])[0]
    data = fw_data[start + 20:]
  */
  start = 4096;
  file_hash_code = bswap_32(*((uint32_t *)(buf_ptr + start)));
  file_crc_code = bswap_16(*((uint16_t *)(buf_ptr + start + 4)));
  file_date_code = bswap_16(*((uint16_t *)(buf_ptr + start + 6)));
  entry_point = bswap_32(*((uint32_t *)(buf_ptr + start + 8)));
  length = bswap_32(*((uint32_t *)(buf_ptr + start + 12)));
  ram_addr = bswap_32(*((uint32_t *)(buf_ptr + start + 16)));
  data_ptr = (uint16_t *)(buf_ptr + start + 20);
  data_ofs = 0;  // offset into buf_ptr (to compare against length)

  /*
    print "fw_load Hash Code : 0x%06x" % file_hash_code
    print "fw_load Date Code : 0x%02x (%04d-%02d-%02d)" % (file_date_code,
    d.year, d.month, d.day)
    print "fw_load  CRC Code : 0x%04x" % file_crc_code
    print "fw_load    Length : %d" % length
    print "fw_load     Entry : 0x%08x" % entryPoint
    print "fw_load       RAM : 0x%08x" % ramAddr
  */
  port_mgr_log("fw_load Hash Code : 0x%06x", file_hash_code);
  port_mgr_log("fw_load Date Code : 0x%08x", file_date_code);
  port_mgr_log("fw_load  CRC Code : 0x%04x", file_crc_code);
  port_mgr_log("fw_load    Length : %d", length);
  port_mgr_log("fw_load     Entry : 0x%08x", entry_point);
  port_mgr_log("fw_load       RAM : 0x%08x", ram_addr);
  if (length > 64 * 1024) {
    port_mgr_log("Length looks invalid. Fail.");
    return BF_INVALID_ARG;
  }
  /*
    dataPtr = 0
    sections = (length + 23) / 24
    # ===========Firmware unload start===========
    chip.MdioWr(0x40C0, 0xFFF0)
    time.sleep(.1)
    chip.MdioWr(0x4013, 0x0AAA)
    time.sleep(.1)
    chip.MdioWr(0x4013, 0x0000)
    time.sleep(.1)
  */
  sections = (length + 23) / 24;
  credo_group8_0xc0_fw_magic_set(dev_id, dev_port, 0, &reg32, 0xFFF0, true);
  bf_sys_usleep(100000);
  credo_group8_0x13_group8_cpuc_div_rmw(dev_id, dev_port, 0, &reg32, 0x0);
  credo_group8_0x13_group8_regc_div_rmw(dev_id, dev_port, 0, &reg32, 0x0);
  credo_group8_0x13_group8_sw_rstb_magic_rmw(
      dev_id, dev_port, 0, &reg32, 0x0AAA);
  bf_sys_usleep(100000);
  credo_group8_0x13_group8_sw_rstb_magic_rmw(dev_id, dev_port, 0, &reg32, 0x0);
  bf_sys_usleep(100000);

  /*
    if broadcast_mode == 1:  # broadcast download mode, use fixed delay
        time.sleep(0.1)
    else:
        start_time = time.time()
        checkTime = 0
        status = chip.MdioRd(0x40C0)
        while status != 0:
            status = chip.MdioRd(0x40C0)
            checkTime += 1
            if checkTime > 100000:
                print '\n...FW LOAD ERROR: : Wait for 0x40C0=0 Timed Out! FW2 =
    0x%X' % status,  # Wait for 0x40c0=0: 0.000432 sec
                break
        stop_time = time.time()
    chip.MdioWr(0x40C0, 0x0000)
    # ===========Firmware unload finish==========
  */
  max_tries = 1000;
  for (try = 0; try < max_tries; try ++) {
    credo_group8_0xc0_fw_magic_get(dev_id, dev_port, 0, &reg32, &status, true);
    if (status == 0) break;
    bf_sys_usleep(10000);
  }
  if (status != 0) {
    port_mgr_log("FW unload failed: status=%04x", status);
    return BF_INVALID_ARG;
  }
  credo_group8_0xc0_fw_magic_set(dev_id, dev_port, 0, &reg32, 0x0, true);

  /*
    if group == 8:
        chip.setPhyAddr(9)
    i = 0
    while i < sections:
        checkSum = 0x800c
        if i == 0: print checkSum
        chip.MdioWr(0x5000 + 12, ramAddr >> 16)
        chip.MdioWr(0x5000 + 13, (ramAddr & 0xFFFF))
        checkSum += (ramAddr >> 16) + (ramAddr & 0xFFFF)
        for j in range(12):
            if (dataPtr > length):
                mdioData = 0x0000
            else:
                mdioData = struct.unpack_from('>H', data[dataPtr:dataPtr +
    2])[0]
            chip.MdioWr(0x5000 + j, mdioData)
            checkSum += mdioData
            dataPtr += 2
            ramAddr += 2

        chip.MdioWr(0x5000 + 14, (~checkSum + 1) & 0xFFFF)
        chip.MdioWr(0x5000 + 15, 0x800c)

        if broadcast_mode == 1:
            time.sleep(wait)
        else:
            checkTime = 0
            status = chip.MdioRd(0x5000 + 15)
            while status == 0x800c:
                status = chip.MdioRd(0x5000 + 15)
                checkTime += 1
                if checkTime > 1000:
                    print '\n...FW LOAD ERROR: Write to Ram Timed Out! 0x5000 =
    %x' % status,
                    break
        i += 1
  */
  // <<< Note: not sure how to handle group 9 here >>>
  i = 0;
  while (i < sections) {
    uint16_t dval[12] = {0};
    check_sum = 0x800c;
    credo_fw_0xc_firmware_data_c_set(
        dev_id, dev_port, 0, &reg32, ram_addr >> 16, true);
    credo_fw_0xd_firmware_data_d_set(
        dev_id, dev_port, 0, &reg32, (ram_addr & 0xFFFF), true);
    check_sum += (ram_addr >> 16) + (ram_addr & 0xFFFF);
    for (j = 0; j < 12; j++) {
      if (data_ofs > length) {
        dval[j] = 0x0;
      } else {
        dval[j] = bswap_16(data_ptr[j]);
      }
      data_ofs += 2;
    }
    credo_fw_0x0_firmware_data_0_set(
        dev_id, dev_port, 0, &reg32, dval[0], true);
    credo_fw_0x1_firmware_data_1_set(
        dev_id, dev_port, 0, &reg32, dval[1], true);
    credo_fw_0x2_firmware_data_2_set(
        dev_id, dev_port, 0, &reg32, dval[2], true);
    credo_fw_0x3_firmware_data_3_set(
        dev_id, dev_port, 0, &reg32, dval[3], true);
    credo_fw_0x4_firmware_data_4_set(
        dev_id, dev_port, 0, &reg32, dval[4], true);
    credo_fw_0x5_firmware_data_5_set(
        dev_id, dev_port, 0, &reg32, dval[5], true);
    credo_fw_0x6_firmware_data_6_set(
        dev_id, dev_port, 0, &reg32, dval[6], true);
    credo_fw_0x7_firmware_data_7_set(
        dev_id, dev_port, 0, &reg32, dval[7], true);
    credo_fw_0x8_firmware_data_8_set(
        dev_id, dev_port, 0, &reg32, dval[8], true);
    credo_fw_0x9_firmware_data_9_set(
        dev_id, dev_port, 0, &reg32, dval[9], true);
    credo_fw_0xa_firmware_data_a_set(
        dev_id, dev_port, 0, &reg32, dval[0xa], true);
    credo_fw_0xb_firmware_data_b_set(
        dev_id, dev_port, 0, &reg32, dval[0xb], true);
    data_ptr += 12;
    ram_addr += 24;
    for (j = 0; j < 12; j++) {
      check_sum += dval[j];
    }
    credo_fw_0xe_firmware_data_e_set(
        dev_id, dev_port, 0, &reg32, (~check_sum + 1) & 0xFFFF, true);
    credo_fw_0xf_firmware_data_f_set(dev_id, dev_port, 0, &reg32, 0x800c, true);

    max_tries = 10000;
    for (try = 0; try < max_tries; try ++) {
      credo_fw_0xf_firmware_data_f_get(
          dev_id, dev_port, 0, &reg32, &status, true);
      if (status != 0x800c) break;
      bf_sys_usleep(100);
    }
    if (status == 0x800c) {
      port_mgr_log("FW load failed: Write to Ram Timed Out: status=%04x",
                   status);
      return BF_INVALID_ARG;
    }
    i++;
  }
  /*
      chip.MdioWr(0x5000 + 12, entryPoint >> 16)
      chip.MdioWr(0x5000 + 13, (entryPoint & 0xFFFF))
      checkSum = (entryPoint >> 16) + (entryPoint & 0xFFFF) + 0x4000
      chip.MdioWr(0x5000 + 14, (~checkSum + 1) & 0xFFFF)
      chip.MdioWr(0x5000 + 15, 0x4000)
      fw_file_ptr.close()
      if group == 8:
          chip.setPhyAddr(8)
      print("Done!"),
      time.sleep(.5)
  */
  credo_fw_0xc_firmware_data_c_set(
      dev_id, dev_port, 0, &reg32, entry_point >> 16, true);
  credo_fw_0xd_firmware_data_d_set(
      dev_id, dev_port, 0, &reg32, (entry_point & 0xFFFF), true);
  check_sum = (entry_point >> 16) + (entry_point & 0xFFFF) + 0x4000;
  credo_fw_0xe_firmware_data_e_set(
      dev_id, dev_port, 0, &reg32, (~check_sum + 1) & 0xFFFF, true);
  credo_fw_0xf_firmware_data_f_set(dev_id, dev_port, 0, &reg32, 0x4000, true);
  port_mgr_log("Done!\n");
  bf_sys_usleep(100000);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_broadcast_mode_set
 *
 */
bf_status_t port_mgr_tof2_serdes_broadcast_mode_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    bool en) {
  /*
  def broadcast_mode(Group=range(8), en=1):
      for group in Group:
          chip.setPhyAddr(group)
          if en == 1:
              chip.MdioWr(0x4014, 0x8888)
          else:
              chip.MdioWr(0x4014, 0x0000)
  */
  if (en) {
    port_mgr_tof2_serdes_tile_wr(dev_id, dev_port, 0x80014, 0x8888);
  } else {
    port_mgr_tof2_serdes_tile_wr(dev_id, dev_port, 0x80014, 0x0000);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_check_load_reqd
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_check_load_reqd(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t fw_hash_code,
                                                    uint32_t fw_crc,
                                                    bool *load_reqd) {
  bf_status_t rc;
  uint32_t running_hash_code = 0, running_crc = 0;

  *load_reqd = true;
  rc = port_mgr_tof2_serdes_fw_running_info_get(
      dev_id, dev_port, &running_hash_code, &running_crc);
  if (rc != BF_SUCCESS) return rc;

  if ((running_hash_code == fw_hash_code) && (running_crc == fw_crc)) {
    *load_reqd = false;
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_load_to_buffer
 *
 */
bf_status_t port_mgr_tof2_serdes_fw_load_to_buffer(char *fw_file_name,
                                                   uint8_t **fw_buffer_p,
                                                   uint32_t *fw_len,
                                                   uint32_t *fw_hash_code,
                                                   uint32_t *fw_crc) {
  unsigned char *buf_ptr;
  int buf_len = 0, item_cnt = -1;
  FILE *fp;

  fp = fopen(fw_file_name, "r");
  if (fp == NULL) return BF_INVALID_ARG;

  buf_ptr = bf_sys_malloc(128 * 1024);
  if (buf_ptr == NULL) {
    fclose(fp);
    return BF_NO_SYS_RESOURCES;
  }

  while (item_cnt) {
    item_cnt = fread(&buf_ptr[buf_len], 1, 1, fp);
    buf_len += item_cnt;
  }
  fclose(fp);
  *fw_buffer_p = buf_ptr;
  *fw_len = buf_len;

  /* get CRC and hash of the file for comparison to running code */
  port_mgr_tof2_serdes_fw_file_info_get(buf_ptr, fw_hash_code, fw_crc);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_mode_set
 *
 *
 * def bfn_set_lane_mode(mode='pam4', g=None, ln=None):
 *    set_grp(g)
 *    if mode !='PAM4':
 *        chip.PAM50[ln].PAM4_EN = 0
 *        chip.PAM50[ln].TX_PRBS_CLK_EN = 0
 *        chip.NRZ25[ln].TX_NRZ_MODE = 1
 *        chip.NRZ25[ln].TX_NRZ_PRBS_GEN_EN = 1
 *    else:
 *        chip.NRZ25[ln].TX_NRZ_MODE = 0
 *        chip.NRZ25[ln].TX_NRZ_PRBS_GEN_EN = 0
 *        chip.PAM50[ln].PAM4_EN = 1
 *        chip.PAM50[ln].TX_PRBS_CLK_EN = 1
 */
bf_status_t port_mgr_tof2_serdes_lane_mode_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               bf_serdes_encoding_mode_t mode) {
  uint32_t reg32 = 0;

  if (mode == BF_SERDES_ENC_MODE_NRZ) {
    credo_tx_0x41_pam4_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_tx_0xb0_tx_nrz_mode_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_tx_0xa0_tx_prbs_clk_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    // nrz prbs fix
    credo_tx_0xb0_tx_nrz_prbs_gen_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_tx_0xb0_tx_nrz_prbs_clk_en_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_tx_0xb0_tx_nrz_prbs_gen_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  } else {
    credo_tx_0x41_pam4_en_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_tx_0xb0_tx_nrz_mode_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_tx_0xb0_tx_nrz_prbs_clk_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_tx_0xb0_tx_nrz_prbs_gen_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    // pam4 prbs fix
    credo_tx_0xa0_tx_prbs_gen_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_tx_0xa0_tx_prbs_clk_en_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_tx_0xa0_tx_prbs_gen_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_mode_nrz_set
 */
bf_status_t port_mgr_tof2_serdes_lane_mode_nrz_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_lane_mode_set(
      dev_id, dev_port, ln, BF_SERDES_ENC_MODE_NRZ);
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_lane_mode_pam4_set
 */
bf_status_t port_mgr_tof2_serdes_lane_mode_pam4_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_lane_mode_set(
      dev_id, dev_port, ln, BF_SERDES_ENC_MODE_PAM4);
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_en_nrz_get
 *
 * def rx_checker_nrz(lane_obj, status = None):
 *     if status==None:
 *         return lane_obj.RX_PRBS_CHECK_EN
 *     else:
 *        lane_obj.RX_PRBS_CHECK_EN = status
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_en_nrz_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    bool *en) {
  uint32_t reg32;
  uint32_t fld_val;

  credo_rx_0x61_rx_prbs_check_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *en = (fld_val == 1) ? true : false;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_en_nrz_set
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_en_nrz_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    bool en) {
  uint32_t reg32;
  uint32_t fld_val = en ? 1 : 0;

  credo_rx_0x61_rx_prbs_check_en_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_en_pam4_get
 *
 * def rx_checker_pam4(lane_obj, status = None):
 *     if status==None:
 *        return lane_obj.PU_PRBS_SYNC_CHKR
 *     else:
 *         lane_obj.PU_PRBS_SYNC_CHKR = status
 *         lane_obj.PU_PRBS_CHKR = status
 *         lane_obj.RX_PRBS_AUTO_SYNC_EN = status
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_en_pam4_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool *en) {
  uint32_t reg32;
  uint32_t fld_val;

  credo_tx_0x43_pu_prbs_sync_chkr_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *en = (fld_val == 1) ? true : false;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_en_pam4_set
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_en_pam4_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool en) {
  uint32_t reg32;
  uint32_t fld_val = en ? 1 : 0;

  credo_tx_0x43_pu_prbs_sync_chkr_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  credo_tx_0x43_pu_prbs_chkr_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  credo_tx_0x43_rx_prbs_auto_sync_en_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_en_get
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_en_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool *en) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    return port_mgr_tof2_serdes_rx_prbs_en_nrz_get(dev_id, dev_port, ln, en);
  } else if (port_mgr_tof2_serdes_mode_is_pam4(dev_id, dev_port, ln)) {
    return port_mgr_tof2_serdes_rx_prbs_en_pam4_get(dev_id, dev_port, ln, en);
  }
  *en = false;
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_en_set
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_en_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool en) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    return port_mgr_tof2_serdes_rx_prbs_en_nrz_set(dev_id, dev_port, ln, en);
  } else if (port_mgr_tof2_serdes_mode_is_pam4(dev_id, dev_port, ln)) {
    return port_mgr_tof2_serdes_rx_prbs_en_pam4_set(dev_id, dev_port, ln, en);
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_mode_nrz_set
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_mode_nrz_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t mode) {
  uint32_t reg32 = 0;

  credo_rx_0x61_rx_prbs_mode_rmw(dev_id, dev_port, ln, &reg32, mode);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_mode_nrz_get
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_mode_nrz_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t *mode) {
  uint32_t reg32 = 0;

  credo_rx_0x61_rx_prbs_mode_get(dev_id, dev_port, ln, &reg32, mode, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_mode_pam4_set
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_mode_pam4_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t mode) {
  uint32_t reg32 = 0;

  credo_tx_0x43_prbs_mode_sel_rmw(dev_id, dev_port, ln, &reg32, mode);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_mode_pam4_get
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_mode_pam4_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t *mode) {
  uint32_t reg32 = 0;

  credo_tx_0x43_prbs_mode_sel_get(dev_id, dev_port, ln, &reg32, mode, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_mode_set
 *
 *  def rx_prbs_mode_nrz(lane_obj, pat=None):
 *    #nrz_prbs_pat  = ['PRBS9-NRZ', 'PRBS15-NRZ', 'PRBS23-NRZ', 'PRBS31-NRZ']
 *    nrz_prbs_pat  = ['PRBS9', 'PRBS15', 'PRBS23', 'PRBS31']
 *    if pat == None:
 *        checker = rx_checker_nrz(lane_obj)
 *        pat = lane_obj.RX_PRBS_MODE
 *        pat_sel = nrz_prbs_pat[pat]
 *        return checker, pat_sel
 *    elif type(pat) == int:
 *        rx_checker_nrz(lane_obj, 1)
 *        lane_obj.RX_PRBS_MODE = pat
 *    elif type(pat) == str:
 *        val = nrz_prbs_pat.index(pat)
 *        rx_checker_nrz(lane_obj, 1)
 *        lane_obj.RX_PRBS_MODE = val
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_mode_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t mode) {
  if (mode > PORT_MGR_PRBS_NRZ_MODE_NONE) return BF_INVALID_ARG;

  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    if (mode == PORT_MGR_PRBS_NRZ_MODE_NONE) {  // functional mode
      // disable Rx PRBS
      port_mgr_tof2_serdes_rx_prbs_en_nrz_set(dev_id, dev_port, ln, false);
      // pattern doesnt matter
    } else {
      port_mgr_tof2_serdes_rx_prbs_en_nrz_set(dev_id, dev_port, ln, true);
      port_mgr_tof2_serdes_rx_prbs_mode_nrz_set(dev_id, dev_port, ln, mode);
    }
  } else {
    if (mode == PORT_MGR_PRBS_PAM4_MODE_NONE) {  // functional mode
      // disable Rx PRBS
      port_mgr_tof2_serdes_rx_prbs_en_pam4_set(dev_id, dev_port, ln, false);
      // pattern doesnt matter
    } else {
      port_mgr_tof2_serdes_rx_prbs_en_pam4_set(dev_id, dev_port, ln, true);
      port_mgr_tof2_serdes_rx_prbs_mode_pam4_set(dev_id, dev_port, ln, mode);
    }
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_en_nrz_set
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_en_nrz_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    bool en) {
  uint32_t reg32 = 0, fld_val = en ? 1 : 0;

  credo_tx_0xa0_tx_prbs_gen_en_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  credo_tx_0xa0_tx_pam4_test_en_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  credo_tx_0xa0_tx_prbs_clk_en_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  credo_tx_0xa0_tx_test_data_src_rmw(dev_id, dev_port, ln, &reg32, fld_val);
  // poll for squelch bit (A0[13] == 1b)
  if (en == 0) {  // only if not in PRBS mode
    credo_tx_0xa0_tx_pam4_test_en_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val != en) {
      port_mgr_log(
          "%d:%3d:%d WARNIN: A0[13] = %d after "
          "port_mgr_tof2_serdes_tx_prbs_en_nrz_set ! A0=%04x",
          dev_id,
          dev_port,
          ln,
          en,
          reg32);
    }
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_en_nrz_get
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_en_nrz_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    bool *en) {
  uint32_t reg32 = 0, fld_val = 0;
  uint32_t b1_of_4, b2_of_4, b3_of_4, b4_of_4;

  // first read from hw to cache reg 0xa0 value in reg32
  credo_tx_0xa0_tx_prbs_gen_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  b1_of_4 = fld_val;
  credo_tx_0xa0_tx_pam4_test_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, false);
  b2_of_4 = fld_val;
  credo_tx_0xa0_tx_prbs_clk_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, false);
  b3_of_4 = fld_val;
  credo_tx_0xa0_tx_test_data_src_get(
      dev_id, dev_port, ln, &reg32, &fld_val, false);
  b4_of_4 = fld_val;

  // make sure all necessary bits are enabled
  *en = (b1_of_4 && b2_of_4 && b3_of_4 && b4_of_4);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_en_pam4_set
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_en_pam4_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool en) {
  // same as NRZ
  port_mgr_tof2_serdes_tx_prbs_en_nrz_set(dev_id, dev_port, ln, en);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_en_pam4_get
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_en_pam4_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool *en) {
  // same as NRZ
  port_mgr_tof2_serdes_tx_prbs_en_nrz_get(dev_id, dev_port, ln, en);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_mode_nrz_set
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_mode_nrz_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t tx_pat) {
  uint32_t reg32 = 0;

  // disable
  port_mgr_tof2_serdes_tx_prbs_en_nrz_set(dev_id, dev_port, ln, false);
  // change pattern
  credo_tx_0xa0_tx_prbs_mode_rmw(dev_id, dev_port, ln, &reg32, tx_pat);
  // enable
  port_mgr_tof2_serdes_tx_prbs_en_nrz_set(dev_id, dev_port, ln, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_mode_nrz_get
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_mode_nrz_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t *tx_pat) {
  uint32_t reg32 = 0, fld_val;

  credo_tx_0xa0_tx_prbs_mode_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  *tx_pat = fld_val;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_mode_pam4_set
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_mode_pam4_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t tx_pat) {
  // same as NRZ
  port_mgr_tof2_serdes_tx_prbs_mode_nrz_set(dev_id, dev_port, ln, tx_pat);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_mode_pam4_get
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_mode_pam4_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_tof2_prbs_mode_t *tx_pat) {
  port_mgr_tof2_serdes_tx_prbs_mode_nrz_get(dev_id, dev_port, ln, tx_pat);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_prbs_mode_select_set
 */
bf_status_t port_mgr_tof2_serdes_prbs_mode_select_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    bf_serdes_encoding_mode_t enc_mode,
    bool tx_prbs_mode,  // false=functional
    bool rx_prbs_mode,  // false=functional
    port_mgr_tof2_prbs_mode_t tx_pat,
    port_mgr_tof2_prbs_mode_t rx_pat) {
  if ((enc_mode != BF_SERDES_ENC_MODE_NRZ) &&
      (enc_mode != BF_SERDES_ENC_MODE_PAM4)) {
    return BF_INVALID_ARG;
  }
  if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
    if (tx_prbs_mode) {
      port_mgr_tof2_serdes_tx_prbs_mode_nrz_set(dev_id, dev_port, ln, tx_pat);
      port_mgr_tof2_serdes_tx_prbs_en_nrz_set(dev_id, dev_port, ln, true);
    } else {
      port_mgr_tof2_serdes_tx_prbs_en_nrz_set(dev_id, dev_port, ln, false);
    }
    if (rx_prbs_mode) {
      port_mgr_tof2_serdes_rx_prbs_mode_nrz_set(dev_id, dev_port, ln, rx_pat);
      port_mgr_tof2_serdes_rx_prbs_en_nrz_set(dev_id, dev_port, ln, true);
    } else {
      port_mgr_tof2_serdes_rx_prbs_en_nrz_set(dev_id, dev_port, ln, false);
    }
  } else {
    if (tx_prbs_mode) {
      port_mgr_tof2_serdes_tx_prbs_mode_pam4_set(dev_id, dev_port, ln, tx_pat);
      port_mgr_tof2_serdes_tx_prbs_en_pam4_set(dev_id, dev_port, ln, true);
    } else {
      port_mgr_tof2_serdes_tx_prbs_en_pam4_set(dev_id, dev_port, ln, false);
    }
    if (rx_prbs_mode) {
      port_mgr_tof2_serdes_rx_prbs_mode_pam4_set(dev_id, dev_port, ln, rx_pat);
      port_mgr_tof2_serdes_rx_prbs_en_pam4_set(dev_id, dev_port, ln, true);
    } else {
      port_mgr_tof2_serdes_rx_prbs_en_pam4_set(dev_id, dev_port, ln, false);
    }
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_prbs_cfg_get
 */
bf_status_t port_mgr_tof2_serdes_tx_prbs_cfg_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t *tx_cfg) {
  uint32_t fld_val = 0;
  credo_tx_0xa0_tx_prbs_mode_get(dev_id, dev_port, ln, tx_cfg, &fld_val, 1);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_scale_set
 */
bf_status_t port_mgr_tof2_serdes_tx_scale_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint32_t post2,
                                              uint32_t post1,
                                              uint32_t main,
                                              uint32_t pre1,
                                              uint32_t pre2) {
  uint32_t reg32 = 0;

  credo_tx_0xaf_tx_post2_scale_rmw(dev_id, dev_port, ln, &reg32, post2);
  credo_tx_0xaf_tx_post1_scale_rmw(dev_id, dev_port, ln, &reg32, post1);
  credo_tx_0xaf_tx_main_scale_rmw(dev_id, dev_port, ln, &reg32, main);
  credo_tx_0xaf_tx_pre1_scale_rmw(dev_id, dev_port, ln, &reg32, pre1);
  credo_tx_0xaf_tx_pre2_scale_rmw(dev_id, dev_port, ln, &reg32, pre2);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_auto_sel_set
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_auto_sel_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t post2,
                                                 uint32_t post1,
                                                 uint32_t main,
                                                 uint32_t pre1,
                                                 uint32_t pre2) {
  uint32_t reg32 = 0;

  credo_tx_0xaf_tx_pre2_auto_sel_rmw(dev_id, dev_port, ln, &reg32, pre2);
  credo_tx_0xaf_tx_pre1_auto_sel_rmw(dev_id, dev_port, ln, &reg32, pre1);
  credo_tx_0xaf_tx_main_auto_sel_rmw(dev_id, dev_port, ln, &reg32, main);
  credo_tx_0xaf_tx_post1_auto_sel_rmw(dev_id, dev_port, ln, &reg32, post1);
  credo_tx_0xaf_tx_post2_auto_sel_rmw(dev_id, dev_port, ln, &reg32, post2);
  return BF_SUCCESS;
}

static uint32_t int_to_twos(int32_t val, uint32_t fld_width) {
  return (((1 << fld_width) + val) & ((1 << fld_width) - 1));
}
// def twos_to_int(twos_val, bitWidth):
//    return twos_val - int((twos_val << 1) & 2**bitWidth)
static int32_t twos_to_int(uint32_t val, uint32_t fld_width) {
  return (val - (int32_t)((val << 1) & (1 << fld_width)));
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_taps_set
 *
 *        if tap1  != None: lane_obj.TX_PRE_2  = int_to_twos(tap1, 8)
 *        if tap2  != None: lane_obj.TX_PRE_1  = int_to_twos(tap2, 8)
 *        if tap3  != None: lane_obj.TX_MAIN  = int_to_twos(tap3, 8)
 *        if tap4  != None: lane_obj.TX_POST_1  = int_to_twos(tap4, 8)
 *        if tap5  != None: lane_obj.TX_POST_2  = int_to_twos(tap5, 8)
 *        if tap6  != None: lane_obj.Reg00B1_15_12  = int_to_twos(tap6, 4)
 *        if tap7  != None: lane_obj.Reg00B1_11_8  = int_to_twos(tap7, 4)
 *        if tap8  != None: lane_obj.Reg00B1_7_4  = int_to_twos(tap8, 4)
 *        if tap9  != None: lane_obj.Reg00B1_3_0  = int_to_twos(tap9, 4)
 *        if tap10 != None: lane_obj.Reg00B2_15_12 = int_to_twos(tap10,4)
 *        if tap11 != None: lane_obj.Reg00B2_11_8 = int_to_twos(tap11,4)
 */
bf_status_t port_mgr_tof2_serdes_tx_taps_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             int32_t pre2,
                                             int32_t pre1,
                                             int32_t main,
                                             int32_t post1,
                                             int32_t post2) {
  uint32_t reg32 = 0;

  credo_tx_0xa5_tx_pre_2_rmw(
      dev_id, dev_port, ln, &reg32, int_to_twos(pre2, 8));
  credo_tx_0xa7_tx_pre_1_rmw(
      dev_id, dev_port, ln, &reg32, int_to_twos(pre1, 8));
  credo_tx_0xab_tx_post_1_rmw(
      dev_id, dev_port, ln, &reg32, int_to_twos(post1, 8));
  credo_tx_0xad_tx_post_2_rmw(
      dev_id, dev_port, ln, &reg32, int_to_twos(post2, 8));
  /* Moved to the end so when un-squelching the pre and post taps are
   * pre-positioned before the main tap is changed from "0" */
  credo_tx_0xa9_tx_main_rmw(dev_id, dev_port, ln, &reg32, int_to_twos(main, 8));
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_taps_set
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_taps_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             int32_t *pre2,
                                             int32_t *pre1,
                                             int32_t *main,
                                             int32_t *post1,
                                             int32_t *post2) {
  uint32_t reg32 = 0, fld32;

  // if Link-training, get tx taps from B3-B4
  credo_tx_0xa0_coef_kr_training_sel_get(
      dev_id, dev_port, ln, &reg32, &fld32, true);
  if (fld32) {
    credo_tx_0xb4_kr_sm_coef_m2_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *pre2 = twos_to_int(fld32, 6);
    credo_tx_0xb4_kr_sm_coef_m1_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *pre1 = twos_to_int(fld32, 6);
    credo_tx_0xb3_kr_sm_coef_0_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *main = twos_to_int(fld32, 6);
    credo_tx_0xb3_kr_sm_coef_1_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *post1 = twos_to_int(fld32, 6);
    *post2 = 0;  // note: there is no POST2 tap in annlt mode
  } else {
    credo_tx_0xa5_tx_pre_2_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *pre2 = twos_to_int(fld32, 8);

    credo_tx_0xa7_tx_pre_1_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *pre1 = twos_to_int(fld32, 8);

    credo_tx_0xa9_tx_main_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *main = twos_to_int(fld32, 8);

    credo_tx_0xab_tx_post_1_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *post1 = twos_to_int(fld32, 8);

    credo_tx_0xad_tx_post_2_get(dev_id, dev_port, ln, &reg32, &fld32, true);
    *post2 = twos_to_int(fld32, 8);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_graycode_set
 */
bf_status_t port_mgr_tof2_serdes_graycode_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              bool tx_en,
                                              bool rx_en) {
  uint32_t reg32 = 0;

  credo_tx_0xaf_tx_graycode_en_rmw(dev_id, dev_port, ln, &reg32, tx_en ? 1 : 0);
  credo_tx_0x42_rx_graycode_en_rmw(dev_id, dev_port, ln, &reg32, rx_en ? 1 : 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_precode_set
 */
bf_status_t port_mgr_tof2_serdes_precode_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             bool tx_en,
                                             bool rx_en) {
  uint32_t reg32 = 0;

  credo_tx_0xaf_tx_precode_en_rmw(dev_id, dev_port, ln, &reg32, tx_en ? 1 : 0);
  credo_tx_0x42_rx_precode_en_rmw(dev_id, dev_port, ln, &reg32, rx_en ? 1 : 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_precode_get
 */
bf_status_t port_mgr_tof2_serdes_precode_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             bool *tx_en,
                                             bool *rx_en) {
  uint32_t rx_val32, tx_val32, reg32 = 0;

  credo_tx_0xaf_tx_precode_en_get(
      dev_id, dev_port, ln, &reg32, &tx_val32, true);
  credo_tx_0x42_rx_precode_en_get(
      dev_id, dev_port, ln, &reg32, &rx_val32, true);
  *tx_en = tx_val32 == 0 ? false : true;
  *rx_en = rx_val32 == 0 ? false : true;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_precode_set
 */
bf_status_t port_mgr_tof2_serdes_fw_precode_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool phy_mode_tx_en,
                                                bool phy_mode_rx_en,
                                                bool anlt_mode_tx_en,
                                                bool anlt_mode_rx_en) {
  uint32_t reg32 = 0, tx_val, rx_val, combined_msk, combined_val;
  bf_status_t rc1, rc2;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  // PHY mode
  rc1 = port_mgr_tof2_serdes_fw_reg_section_rd(
      dev_id, dev_port, ln, 181, &reg32, 0);
  tx_val = phy_mode_tx_en ? (1 << (base_ln + ln)) : 0;
  rx_val = phy_mode_rx_en ? (1 << (base_ln + ln)) : 0;
  combined_msk = (1 << (base_ln + ln)) | (1 << (base_ln + ln + 8));
  combined_val = (tx_val << 8) | rx_val;
  reg32 = (reg32 & ~combined_msk) | combined_val;
  rc1 = port_mgr_tof2_serdes_fw_reg_section_wr(
      dev_id, dev_port, ln, 181, reg32, 0);
  // AN mode
  rc1 = port_mgr_tof2_serdes_fw_reg_section_rd(
      dev_id, dev_port, ln, 182, &reg32, 0);
  tx_val = anlt_mode_tx_en ? (1 << (base_ln + ln)) : 0;
  rx_val = anlt_mode_rx_en ? (1 << (base_ln + ln)) : 0;
  combined_msk = (1 << (base_ln + ln)) | (1 << (base_ln + ln + 8));
  combined_val = (tx_val << 8) | rx_val;
  reg32 = (reg32 & ~combined_msk) | combined_val;
  rc2 = port_mgr_tof2_serdes_fw_reg_section_wr(
      dev_id, dev_port, ln, 182, reg32, 0);

  return rc1 ? rc1 : rc2;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fw_precode_get
 */
bf_status_t port_mgr_tof2_serdes_fw_precode_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool *phy_mode_tx_en,
                                                bool *phy_mode_rx_en,
                                                bool *anlt_mode_tx_en,
                                                bool *anlt_mode_rx_en) {
  uint32_t reg32 = 0, rx_val, tx_val;
  bf_status_t rc;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  // PHY mode
  rc = port_mgr_tof2_serdes_fw_reg_section_rd(
      dev_id, dev_port, ln, 181, &reg32, 0);
  rx_val = (reg32 >> (base_ln + ln)) & 1;
  tx_val = (reg32 >> (base_ln + ln + 8)) & 1;
  *phy_mode_rx_en = rx_val ? true : false;
  *phy_mode_tx_en = tx_val ? true : false;

  // AN mode
  rc = port_mgr_tof2_serdes_fw_reg_section_rd(
      dev_id, dev_port, ln, 182, &reg32, 0);
  rx_val = (reg32 >> (base_ln + ln)) & 1;
  tx_val = (reg32 >> (base_ln + ln + 8)) & 1;
  *anlt_mode_rx_en = rx_val ? true : false;
  *anlt_mode_tx_en = tx_val ? true : false;
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_msblsb_en
 */
bf_status_t port_mgr_tof2_serdes_msblsb_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            bool tx_en,
                                            bool rx_en) {
  uint32_t reg32 = 0;

  credo_tx_0xaf_tx_swap_msb_lsb_rmw(
      dev_id, dev_port, ln, &reg32, tx_en ? 1 : 0);
  credo_tx_0x43_rx_swap_msb_lsb_rmw(
      dev_id, dev_port, ln, &reg32, rx_en ? 1 : 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ac_couple_get
 */
bf_status_t port_mgr_tof2_serdes_ac_couple_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               bool *ac_en) {
  uint32_t reg32 = 0, fld_val = 0;

  *ac_en = false;
  credo_rx_0xe7_rx_ac_couple_en_get(dev_id, dev_port, ln, &reg32, &fld_val, 1);
  if (fld_val == 0) {
    *ac_en = false;
    return BF_SUCCESS;
  }
  credo_rx_0xdd_rsvd_0n01dd_11_09_get(
      dev_id, dev_port, ln, &reg32, &fld_val, 1);
  if (fld_val) {
    *ac_en = false;
  } else {
    *ac_en = true;
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ac_couple_en
 */
bf_status_t port_mgr_tof2_serdes_ac_couple_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               bool ac_en) {
  uint32_t reg32 = 0;

  credo_rx_0xe7_rx_ac_couple_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  if (ac_en) {
    credo_rx_0xdd_rsvd_0n01dd_11_09_rmw(dev_id, dev_port, ln, &reg32, 0);
  } else {
    credo_rx_0xdd_rsvd_0n01dd_11_09_rmw(dev_id, dev_port, ln, &reg32, 1);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_init_log_to_phy_reg_range
 *
 * ### Extend TX, for Log-to-Phy Mapping
 * chip.GROUP8_TOP[0].TX_REG_RNG_0_EN    = 1      #wregBits(0x4083, bits=[15,
 *14, 13, 12], val=0xE) chip.GROUP8_TOP[0].TX_REG_RNG_1_EN    = 1
 * chip.GROUP8_TOP[0].TX_REG_RNG_2_EN    = 1
 * chip.GROUP8_TOP[0].TX_REG_RNG_0_START = 0xF3   #wregBits(0x4083,
 *bits=range(9)[::-1], val=0xF3)
 * chip.GROUP8_TOP[0].TX_REG_RNG_0_END   = 0xFF   #wregBits(0x4084,
 *bits=range(9)[::-1], val=0xFF)
 * chip.GROUP8_TOP[0].TX_REG_RNG_1_START = 0xE9   #wregBits(0x4085,
 *bits=range(9)[::-1], val=0xE9)
 * chip.GROUP8_TOP[0].TX_REG_RNG_1_END   = 0xEB   #wregBits(0x4086,
 *bits=range(9)[::-1], val=0xEB)
 * chip.GROUP8_TOP[0].TX_REG_RNG_2_START = 0xA0   #wregBits(0x4087,
 *bits=range(9)[::-1], val=0xA0)
 * chip.GROUP8_TOP[0].TX_REG_RNG_2_END   = 0xDB   #wregBits(0x4088,
 *bits=range(9)[::-1], val=0xDB)
 * ### configure RX mapping range, for Log-to-Phy Mapping
 * chip.GROUP8_TOP[0].RX_REG_RNG_0_EN    = 1      #wregBits(0x408B, bits=[15,
 *14, 13, 12], val=0xF) chip.GROUP8_TOP[0].RX_REG_RNG_1_EN    = 1
 * chip.GROUP8_TOP[0].RX_REG_RNG_2_EN    = 1
 * chip.GROUP8_TOP[0].RX_REG_RNG_3_EN    = 1
 * chip.GROUP8_TOP[0].RX_REG_RNG_1_END   = 0x9F   #wregBits(0x408E,
 *bits=range(9)[::-1], val=0x9F)
 * chip.GROUP8_TOP[0].RX_REG_RNG_2_START = 0xEC
 * chip.GROUP8_TOP[0].RX_REG_RNG_2_END   = 0xF2   #wregBits(0x4090,
 *bits=range(9)[::-1], val=0xF2)
 * chip.GROUP8_TOP[0].RX_REG_RNG_3_START = 0xDC   #wregBits(0x4091,
 *bits=range(9)[::-1], val=0xDC)
 * chip.GROUP8_TOP[0].RX_REG_RNG_3_END   = 0xE8   #wregBits(0x4092,
 *bits=range(9)[::-1], val=0xE8)
 *
 */
bf_status_t port_mgr_tof2_serdes_init_log_to_phy_reg_range(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port) {
  uint32_t reg32 = 0, ln = 0;

  credo_group8_0x8b_rx_reg_rng_0_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x8b_rx_reg_rng_1_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x8b_rx_reg_rng_2_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x8b_rx_reg_rng_3_en_rmw(dev_id, dev_port, ln, &reg32, 1);

  credo_group8_0x8e_rx__reg__rng_1_end_rmw(dev_id, dev_port, ln, &reg32, 0x9f);
  credo_group8_0x8f_rx__reg__rng_2_start_rmw(
      dev_id, dev_port, ln, &reg32, 0xec);
  credo_group8_0x90_rx__reg__rng_2_end_rmw(dev_id, dev_port, ln, &reg32, 0xf3);
  credo_group8_0x91_rx__reg__rng_3_start_rmw(
      dev_id, dev_port, ln, &reg32, 0xdc);
  credo_group8_0x92_rx__reg__rng_3_end_rmw(dev_id, dev_port, ln, &reg32, 0xE8);

  credo_group8_0x83_tx__reg__rng_0_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x83_tx__reg__rng_1_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x83_tx__reg__rng_2_en_rmw(dev_id, dev_port, ln, &reg32, 1);

  credo_group8_0x83_tx__reg__rng_0_start_rmw(
      dev_id, dev_port, ln, &reg32, 0xF4);
  credo_group8_0x84_tx__reg__rng_0_end_rmw(dev_id, dev_port, ln, &reg32, 0xFF);
  credo_group8_0x85_tx__reg__rng_1_start_rmw(
      dev_id, dev_port, ln, &reg32, 0xE9);
  credo_group8_0x86_tx__reg__rng_1_end_rmw(dev_id, dev_port, ln, &reg32, 0xEB);
  credo_group8_0x87_tx__reg__rng_2_start_rmw(
      dev_id, dev_port, ln, &reg32, 0xA0);
  credo_group8_0x88_tx__reg__rng_2_end_rmw(dev_id, dev_port, ln, &reg32, 0xDB);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_init_lane_for_an
 *
 * def bfn_init_lane_for_fw(mode='pam4', input_mode='ac', lane=None, group =
 *None, TX_pat=3, RX_pat=3):
 */
bf_status_t port_mgr_tof2_serdes_init_lane_for_an(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln) {
  uint32_t reg32, phys_tx_ln[8], phys_rx_ln[8];
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  port_mgr_tof2_serdes_lane_map_get(dev_id, dev_port, phys_tx_ln, phys_rx_ln);

  // make sure AN is enabled
  credo_lane_slice_0x0_tx_an_or_lt_en_rmw(
      dev_id, dev_port, phys_tx_ln[base_ln + ln], &reg32, 1);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_init_lane_for_fw
 *
 * def bfn_init_lane_for_fw(mode='pam4', input_mode='ac', lane=None, group =
 *None, TX_pat=3, RX_pat=3):
 */
bf_status_t port_mgr_tof2_serdes_init_lane_for_fw(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    bf_serdes_encoding_mode_t enc_mode,
    port_mgr_tof2_prbs_mode_t tx_pat,
    port_mgr_tof2_prbs_mode_t rx_pat) {
  bool tx_prbs_mode, rx_prbs_mode;
  uint32_t reg32 = 0;

  if ((enc_mode != BF_SERDES_ENC_MODE_NRZ) &&
      (enc_mode != BF_SERDES_ENC_MODE_PAM4)) {
    return BF_INVALID_ARG;
  }
  port_mgr_tof2_serdes_tx_scale_set(dev_id, dev_port, ln, 0, 0, 1, 0, 0);
  port_mgr_tof2_serdes_msblsb_set(dev_id, dev_port, ln, false, false);

  port_mgr_tof2_serdes_lane_mode_set(dev_id, dev_port, ln, enc_mode);
  credo_tx_0x7b_blw_invert_pol_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_rx_0x7c_nrz_blwc_en_rmw(dev_id, dev_port, ln, &reg32, 0);

  if (tx_pat == PORT_MGR_PRBS_NRZ_MODE_NONE) {
    tx_prbs_mode = false;
  } else {
    tx_prbs_mode = true;
  }
  if (rx_pat == PORT_MGR_PRBS_NRZ_MODE_NONE) {
    rx_prbs_mode = false;
  } else {
    rx_prbs_mode = true;
  }
  port_mgr_tof2_serdes_prbs_mode_select_set(dev_id,
                                            dev_port,
                                            ln,
                                            enc_mode,
                                            tx_prbs_mode,  // false=functional
                                            rx_prbs_mode,  // false=functional
                                            tx_pat,
                                            rx_pat);

  if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
    port_mgr_tof2_serdes_graycode_set(dev_id, dev_port, ln, false, false);
  } else {
    port_mgr_tof2_serdes_graycode_set(dev_id, dev_port, ln, true, true);
  }

  return BF_SUCCESS;
}

int b13_max_tries = 0;

/*****************************************************************************
 * port_mgr_tof2_serdes_un_config_ln
 *
 * Issue FW commands to deactivate either PAM4 or NRZ config on this lane
 */
bf_status_t port_mgr_tof2_serdes_un_config_ln(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln) {
  uint32_t rsp;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  // force Tx to output "0"
  port_mgr_tof2_serdes_tx_taps_set(dev_id, dev_port, ln, 0, 0, 0, 0, 0);

  port_mgr_tof2_serdes_fw_cmd_w_detail(
      dev_id, dev_port, ln, 0x90D0 + base_ln + ln, &rsp, 0);
  // Only one should be sufficient
  // port_mgr_tof2_serdes_fw_cmd_w_detail(
  //    dev_id, dev_port, ln, 0x90C0 + base_ln + ln, &rsp, 0);

#ifdef BF_TOFINO2_SERDES_TEST
  // poll for squelch bit (A0[13] == 1b)
  for (int i = 0; i < 1000000; i++) {
    uint32_t reg32 = 0, fld_val = 0;

    credo_tx_0xa0_tx_pam4_test_en_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val) break;
    if (b13_max_tries < i) {
      b13_max_tries = i;
      port_mgr_log("%d:%3d:%d A0[13] max polls increases to %d ..",
                   dev_id,
                   dev_port,
                   ln,
                   b13_max_tries);
    }
  }
#endif

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_config_ln_an
 *
 *   # AN mode settings
 *   wreg(0x310, basepage&0xffff, lane=ln)
 *   wreg(0x311, (basepage>>16)&0xffff, lane=ln)
 *   wreg(0x312, (basepage>>32)&0xffff, lane=ln)
 *   if consort==0:
 *       wregBits(0x600, [12], 1, lane=ln)
 *   else:
 *       wregBits(0x600, [12], 0, lane=ln)
 *       wreg(0x3f4, consort, lane=ln)
 *   wregBits(0x600, [10], 1 if is_loop else 0, lane=ln)
 *
 */
bf_status_t port_mgr_tof2_serdes_config_ln_an(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint64_t basepage,
                                              uint32_t consortium_np_47_16,
                                              bool is_loop) {
  bf_status_t rc;
  uint32_t rsp, config_detail;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);
  uint32_t reg32;
  uint32_t adv_15_0, adv_31_16, adv_47_32;
  uint32_t phys_tx_ln[8], phys_rx_ln[8];

  port_mgr_tof2_serdes_lane_map_get(dev_id, dev_port, phys_tx_ln, phys_rx_ln);

  adv_15_0 = (uint32_t)((basepage >> 0) & 0xffffull);
  adv_31_16 = (uint32_t)((basepage >> 16) & 0xffffull);
  adv_47_32 = (uint32_t)((basepage >> 32) & 0xffffull);

  // program advertisement
  credo_an_lt_0x10_an_selector_field_rmw(
      dev_id, dev_port, ln, &reg32, adv_15_0);
  credo_an_lt_0x11_an_d_31_16_rmw(dev_id, dev_port, ln, &reg32, adv_31_16);
  credo_an_lt_0x12_an_d_47_32_rmw(dev_id, dev_port, ln, &reg32, adv_47_32);
  if (consortium_np_47_16 == 0) {
    credo_an_rsvd_0x0_arg_aneg_ieee_mode_s_rmw(dev_id, dev_port, ln, &reg32, 1);
  } else {
    uint32_t consortium_np_31_16, consortium_np_47_32;

    consortium_np_31_16 = consortium_np_47_16 & 0xffff;
    consortium_np_47_32 = consortium_np_47_16 >> 16;
    credo_an_rsvd_0x0_arg_aneg_ieee_mode_s_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_an_lt_0xf4_an_np_uf_31_16_rmw(
        dev_id, dev_port, ln, &reg32, consortium_np_31_16);
    credo_an_lt_0xf5_an_np_uf__47_32_rmw(
        dev_id, dev_port, ln, &reg32, consortium_np_47_32);
  }
  if (is_loop) {
    credo_an_rsvd_0x0_arg_dis_nonce_match_s_rmw(
        dev_id, dev_port, ln, &reg32, 1);
  } else {
    credo_an_rsvd_0x0_arg_dis_nonce_match_s_rmw(
        dev_id, dev_port, ln, &reg32, 0);
  }

  config_detail = 0x100;

  rc = port_mgr_tof2_serdes_fw_cmd_w_detail(
      dev_id, dev_port, ln, 0x8000 + base_ln + ln, &rsp, config_detail);
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_config_ln_nrz
 *
 * def bfn_init_lane_for_fw(mode='pam4', input_mode='ac', lane=None, group =
 *None, TX_pat=3, RX_pat=3):
 */
bf_status_t port_mgr_tof2_serdes_config_ln_nrz(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_serdes_nrz_speed_t speed,
    port_mgr_tof2_prbs_mode_t tx_pat,
    port_mgr_tof2_prbs_mode_t rx_pat) {
  uint32_t rsp, config_detail;
  bf_status_t rc;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  port_mgr_tof2_serdes_init_lane_for_fw(
      dev_id, dev_port, ln, BF_SERDES_ENC_MODE_NRZ, tx_pat, rx_pat);
  if (speed == 1) {
    config_detail = 0x0A;
  } else if (speed == 10) {
    config_detail = 0x1;
  } else if (speed == 20) {
    config_detail = 0x2;
  } else if (speed == 25) {
    config_detail = 0x0;
  } else {
    config_detail = 0x0;
  }
  rc = port_mgr_tof2_serdes_fw_cmd_w_detail(
      dev_id, dev_port, ln, 0x80C0 + base_ln + ln, &rsp, config_detail);

  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_config_ln_pam4
 *
 */
bf_status_t port_mgr_tof2_serdes_config_ln_pam4(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    port_mgr_serdes_pam4_speed_t speed,
    port_mgr_tof2_prbs_mode_t tx_pat,
    port_mgr_tof2_prbs_mode_t rx_pat) {
  uint32_t rsp;
  bf_status_t rc = BF_SUCCESS;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  port_mgr_tof2_serdes_init_lane_for_fw(
      dev_id, dev_port, ln, BF_SERDES_ENC_MODE_PAM4, tx_pat, rx_pat);

  // hack
  // poll for squelch bit (A0[13] == 1b)
  {
    uint32_t reg32 = 0, fld_val = 0;

    credo_tx_0xa0_tx_pam4_test_en_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val) {
      port_mgr_log(
          "%d:%3d:%d WARNIN: A0[13] = 1 after "
          "port_mgr_tof2_serdes_init_lane_for_fw ! A0=%04x",
          dev_id,
          dev_port,
          ln,
          reg32);
    }
  }

  rc = port_mgr_tof2_serdes_fw_cmd_w_detail(
      dev_id, dev_port, ln, 0x80D0 + base_ln + ln, &rsp, 0);
  // hack
  bf_sys_usleep(3000);  // give FW 1ms to complete
                        //

  (void)speed;
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_sig_detect_nrz_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_sig_detect_nrz_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    bool *sig_detect,
                                                    bool *phy_ready) {
  uint32_t reg32 = 0, fld_val = 0;

  credo_rx_0x2e_nrz_read_sig_det_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *sig_detect = (fld_val == 0) ? false : true;
  credo_rx_0x2e_nrz_read_phy_ready_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *phy_ready = (fld_val == 0) ? false : true;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_sig_detect_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_sig_detect_pam4_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool *sig_detect,
                                                     bool *phy_ready) {
  uint32_t reg32 = 0, fld_val = 0;

  credo_tx_0x6a_read_sig_det_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  *sig_detect = (fld_val == 0) ? false : true;
  credo_tx_0x6a_rx_read_phy_ready_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *phy_ready = (fld_val == 0) ? false : true;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_sig_detect_get
 *
 */
bf_status_t port_mgr_tof2_serdes_sig_detect_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool *sig_detect,
                                                bool *phy_ready) {
  if (port_mgr_tof2_serdes_mode_is_pam4(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_sig_detect_pam4_get(
        dev_id, dev_port, ln, sig_detect, phy_ready);
  } else {
    port_mgr_tof2_serdes_sig_detect_nrz_get(
        dev_id, dev_port, ln, sig_detect, phy_ready);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_an_done_get
 *
 * "Add AN resolved and lane speed switch finished feature ( Register 800C7
 * upper 8 bits) for ANLT, This is the same register as FW_ADPT_DONE, but
 * use upper 8 bits. 1 bit per lane. So you can use this value to decide
 * when your ANLT traffic send out." -- added in 1.00.25
 */
bf_status_t port_mgr_tof2_serdes_an_done_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             bool *an_done) {
  uint32_t reg32 = 0, fld_val = 0;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  credo_group8_0xc7_fw_status_msb_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *an_done = ((fld_val >> (base_ln + ln)) & 1) ? true : false;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_adapt_done_get
 *
 * def adapt_done(chip,ln):
 *  v = reg_rd(0x40C7)
 *  if ((v >> ln) & 1):
 *    return 1
 *  return 0
 *
 */
bf_status_t port_mgr_tof2_serdes_adapt_done_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool *adapt_done) {
  uint32_t reg32 = 0, fld_val = 0;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);

  credo_group8_0xc7_fw_adpt_done_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *adapt_done = ((fld_val >> (base_ln + ln)) & 1) ? true : false;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_temperature_start_set
 *
 *def temp_sensor_start(auto=0):
 *    base = 0x4B00
 *    chip.MdioWr(0x4d00, 0x5d81)
 *    if(auto):
 *        chip.MdioWr(base+0x3a, 0x3f)
 *    else:
 *        chip.MdioWr(base+0x3a, 0x7)
 *    chip.MdioWr(base + 0x3e, 0x0054)  # set clock
 *    time.sleep(1)
 *    chip.MdioWr(base + 0x37, 0x0)  # reset sensor
 *    time.sleep(1)
 */
bf_status_t port_mgr_tof2_serdes_temperature_start_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       bool auto_) {
  uint32_t reg32 = 0;

  /* pd_cal_fcal is bit 15, which is "0" in their hard-coded register value
   * So, init reg32 to the desired value and "set" bit 15 to "0" */
  reg32 = 0x5d81;
  credo_rsvd3_0x0_pd_cal_fcal_set(dev_id, dev_port, 0, &reg32, 0, true);
  if (auto_) {
    reg32 = 0x3f;
  } else {
    reg32 = 0x07;
  }
  // bf_sys_usleep(200);

  /* _rstb is bit 2, which is "1" in both the above options */
  credo_sensor_0x3a_tsensor_rstb_set(dev_id, dev_port, 0, &reg32, 1, true);
  // bf_sys_usleep(200);

  /* tsensor_clk_sel is bit 15, which is "0" in their hard-coded register
   * value*/
  reg32 = 0x0054;
  credo_sensor_0x3e_tsensor_clk_sel_set(dev_id, dev_port, 0, &reg32, 0, true);
  // bf_sys_usleep(1000000);
  bf_sys_usleep(100000);

  /* tsensor_run is bit 3, which is "0" in their hard-coded register value*/
  reg32 = 0x0000;
  credo_sensor_0x37_tsensor_run_set(dev_id, dev_port, 0, &reg32, 0, true);
  // bf_sys_usleep(1000000);
  return BF_SUCCESS;
}

/*****************************************************************************
* port_mgr_tof2_serdes_temperature_get
*
*
def temp_sensor_read(auto=0):
    base = 0x4B00
    Yds = 237.7
    Kds = 79.925
    time1 = time.time()
    time2 = time.time()
    if auto == 1:
        rdy = 0
        while (rdy == 0):  # wait for rdy
            value = chip.MdioRd(0x4859)
            rdy = value >> 12
            time2 = time.time()
            if (time2-time1) >= 5:
                print 'AutoReadTsensor test timeout2...'
        realVal = (value&0x0fff) * Yds / 4096 - Kds
        print('tempsensor%d: %d,realVal:%f' % (0, value, realVal))
    else:
        addr = [base + 0x39, base + 0x3a, base + 0x3b, base + 0x3c]
        chip.MdioWr(base + 0x37, 0xc)  # set no ack
        rdy = chip.MdioRd(base + 0x38) >> 8
        while (rdy == 0):  # wait for rdy
            rdy = chip.MdioRd(base + 0x38) >> 8
            time2 = time.time()
            if (time2 - time1) >= 5:
                print 'ReadTSensor test timeout.2..'
                break
        value = chip.MdioRd(addr[0])
        realVal = value * Yds / 4096 - Kds
        print('tempsensor%d: %d,realVal:%f' % (0, value, realVal))
    return realVal
*/
bf_status_t port_mgr_tof2_serdes_temperature_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 bool auto_,
                                                 float *temp) {
  float Yds = 237.7;
  float Kds = 79.925;
  uint32_t reg32 = 0, fld_val = 0;
  int tries = 5000;  // 5s
  uint32_t value, rdy = 0;

  if (auto_) {
    while (!rdy) {
      credo_top_pll_0x59_tsensor_auto_result_get(
          dev_id, dev_port, 0, &reg32, &fld_val, true);
      rdy = fld_val >> 12;
      if (!rdy) {
        bf_sys_usleep(1000);
        tries--;
        if (tries <= 0) {
          return BF_INVALID_ARG;
        }
      }
    }
    *temp = (fld_val & 0x0fff) * Yds / 4096 - Kds;
  } else {
    reg32 = 0x000C;
    credo_sensor_0x37_tsensor_run_set(dev_id, dev_port, 0, &reg32, 1, true);
    bf_sys_usleep(1000);
    credo_sensor_0x38_tsensor_ready_get(
        dev_id, dev_port, 0, &reg32, &fld_val, true);
    rdy = fld_val >> 8;
    tries = 10000;
    while (!rdy) {
      // bf_sys_usleep(1000);
      tries--;
      if (tries <= 0) {
        return BF_INVALID_ARG;
      }
      credo_sensor_0x38_tsensor_ready_get(
          dev_id, dev_port, 0, &reg32, &fld_val, true);
      rdy = fld_val >> 8;
    }
    credo_sensor_0x39_tsensor_manual_result_get(
        dev_id, dev_port, 0, &reg32, &fld_val, true);
    value = fld_val;
    *temp = value * Yds / 4096 - Kds;
  }
  return BF_SUCCESS;
}

bf_status_t port_mgr_tof2_serdes_fw_temperature_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    float *temp) {
  bf_status_t rc;
  uint32_t reg32;

  rc = port_mgr_tof2_serdes_fw_reg_section_rd(
      dev_id, dev_port, 0, 187, &reg32, 0);
  *temp = (float)reg32;
  return rc;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_check_en_nrz_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_check_en_nrz_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, bool *en) {
  uint32_t reg32 = 0;
  uint32_t fld_val;

  credo_rx_0x61_rx_prbs_check_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *en = (fld_val == 0) ? false : true;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_err_cnt_nrz_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_err_cnt_nrz_get(bf_dev_id_t dev_id,
                                                         bf_dev_port_t dev_port,
                                                         uint32_t ln,
                                                         uint32_t *err_cnt) {
  uint32_t reg32 = 0, hi, lo;  //, hi2;

  credo_rx_0x66_rx_nrz_prbs_read_err_high_get(
      dev_id, dev_port, ln, &reg32, &hi, true);
  credo_rx_0x67_rx_nrz_prbs_read_err_low_get(
      dev_id, dev_port, ln, &reg32, &lo, true);
  *err_cnt = (hi << 16) | lo;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_err_nrz_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_err_nrz_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     uint32_t *err_cnt) {
  bool en;

  port_mgr_tof2_serdes_rx_prbs_check_en_nrz_get(dev_id, dev_port, ln, &en);
  if (!en) {
    *err_cnt = 0;
  } else {
    port_mgr_tof2_serdes_rx_prbs_err_cnt_nrz_get(dev_id, dev_port, ln, err_cnt);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_check_en_pam4_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_check_en_pam4_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, bool *en) {
  uint32_t reg32 = 0;
  uint32_t fld_val;

  credo_tx_0x43_pu_prbs_sync_chkr_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *en = (fld_val == 0) ? false : true;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_err_cnt_pam4_get
 *
 *def latch_data_pam4(lane_obj):
 *    lane_obj.READOUT_SYNC_EN = 0
 *    lane_obj.READOUT_CAPTURE = 1
 *    lane_obj.READOUT_SYNC_EN = 1
 *
 *def latch_data_release_pam4(lane_obj):
 *    lane_obj.READOUT_SYNC_EN = 0
 *    lane_obj.READOUT_CAPTURE = 0
 *    lane_obj.Reg006E_7_0 = 0
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_err_cnt_pam4_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t *err_cnt) {
  uint32_t reg32 = 0, hi, lo;

  credo_tx_0x6e_readout_sync_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_tx_0x6e_readout_capture_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_tx_0x6e_readout_sync_en_rmw(dev_id, dev_port, ln, &reg32, 1);

  credo_tx_0x50_prbs_read_sync_err_cntr_msb_get(
      dev_id, dev_port, ln, &reg32, &hi, true);
  credo_tx_0x51_prbs_read_sync_err_cntr_lsb_get(
      dev_id, dev_port, ln, &reg32, &lo, true);

  credo_tx_0x6e_readout_sync_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_tx_0x6e_readout_capture_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_tx_0x6e_pam4_prbs_chk_phase_en_rmw(dev_id, dev_port, ln, &reg32, 0);

  *err_cnt = (hi << 16) | lo;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_err_pam4_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_err_pam4_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t *err_cnt) {
  bool en;

  port_mgr_tof2_serdes_rx_prbs_check_en_pam4_get(dev_id, dev_port, ln, &en);
  if (!en) {
    *err_cnt = 0;
  } else {
    port_mgr_tof2_serdes_rx_prbs_err_cnt_pam4_get(
        dev_id, dev_port, ln, err_cnt);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_prbs_err_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_prbs_err_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t *err_cnt) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_rx_prbs_err_nrz_get(dev_id, dev_port, ln, err_cnt);
  } else {
    port_mgr_tof2_serdes_rx_prbs_err_pam4_get(dev_id, dev_port, ln, err_cnt);
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_prbs_rst_pam4_set
 *
 * def prbs_rst_pam4(lane_obj):
 *     wait = 0.001
 *     lane_obj.PRBS_SYNC_CNTR_RESET = 0
 *     time.sleep(wait)
 *     lane_obj.PRBS_SYNC_CNTR_RESET = 1
 *     time.sleep(wait)
 *     lane_obj.PRBS_SYNC_CNTR_RESET = 0
 *
 */
bf_status_t port_mgr_tof2_serdes_prbs_rst_pam4_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln) {
  uint32_t reg32 = 0;

  credo_tx_0x43_prbs_sync_cntr_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_tx_0x43_prbs_sync_cntr_reset_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_tx_0x43_prbs_sync_cntr_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_prbs_rst_nrz_set
 *
 * def prbs_rst_nrz(lane_obj):
 *     wait = 0.001
 *     lane_obj.RX_PRBS_COUNT_RESET = 0
 *     time.sleep(wait)
 *     lane_obj.RX_PRBS_COUNT_RESET = 1
 *     time.sleep(wait)
 *     lane_obj.RX_PRBS_COUNT_RESET = 0
 */
bf_status_t port_mgr_tof2_serdes_prbs_rst_nrz_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln) {
  uint32_t reg32 = 0;

  credo_rx_0x61_rx_prbs_count_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rx_0x61_rx_prbs_count_reset_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_rx_0x61_rx_prbs_count_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_prbs_rst_set
 *
 */
bf_status_t port_mgr_tof2_serdes_prbs_rst_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_prbs_rst_nrz_set(dev_id, dev_port, ln);
  } else {
    port_mgr_tof2_serdes_prbs_rst_pam4_set(dev_id, dev_port, ln);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_polarity_nrz_set
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_polarity_nrz_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool inv) {
  uint32_t val, reg32 = 0;

  val = inv ? 0 : 1;  // sense is reversed
  credo_tx_0xa0_tx_ana_out_flip_rmw(dev_id, dev_port, ln, &reg32, val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_polarity_pam4_set
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_polarity_pam4_set(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      bool inv) {
  uint32_t val, reg32 = 0;

  val = inv ? 0 : 1;  // sense is reversed
  credo_tx_0xa0_tx_ana_out_flip_rmw(dev_id, dev_port, ln, &reg32, val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_polarity_set
 *
 *          tx_pn = 1 - tx_pn # 1 == NO INVERT
 *          chip.PAM50[ln].tx_pol_pam4(val=tx_pn)
 *          chip.PAM50[ln].rx_pol_pam4(val=rx_pn)
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_polarity_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 bool inv) {
  port_mgr_log("%d:%3d:%d Serdes Tx (hw) Polarity set: %d",
               dev_id,
               dev_port,
               ln,
               inv ? 1 : 0);
  // always set both PAM4 and NRZ
  port_mgr_tof2_serdes_tx_polarity_nrz_set(dev_id, dev_port, ln, inv);
  port_mgr_tof2_serdes_tx_polarity_pam4_set(dev_id, dev_port, ln, inv);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_polarity_nrz_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_polarity_nrz_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool *inv) {
  uint32_t val, reg32 = 0;

  credo_tx_0xa0_tx_ana_out_flip_get(dev_id, dev_port, ln, &reg32, &val, true);
  *inv = (val == 0) ? true : false;  // sense is reversed
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_polarity_pam4_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_polarity_pam4_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      bool *inv) {
  uint32_t val, reg32 = 0;

  credo_tx_0xa0_tx_ana_out_flip_get(dev_id, dev_port, ln, &reg32, &val, true);
  *inv = (val == 0) ? true : false;  // sense is reversed
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_polarity_get
 *
 *          tx_pn = 1 - tx_pn # 1 == NO INVERT
 *          chip.PAM50[ln].tx_pol_pam4(val=tx_pn)
 *          chip.PAM50[ln].rx_pol_pam4(val=rx_pn)
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_polarity_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 bool *inv) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_tx_polarity_nrz_get(dev_id, dev_port, ln, inv);
  } else {
    port_mgr_tof2_serdes_tx_polarity_pam4_get(dev_id, dev_port, ln, inv);
  }
  port_mgr_log("%d:%3d:%d Serdes Tx (hw) Polarity get: %d",
               dev_id,
               dev_port,
               ln,
               *inv ? 1 : 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_polarity_nrz_set
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_polarity_nrz_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool inv) {
  uint32_t val, reg32 = 0;

  val = inv ? 1 : 0;
  credo_rx_0x61_rx_pol_flip_rmw(dev_id, dev_port, ln, &reg32, val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_polarity_pam4_set
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_polarity_pam4_set(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      bool inv) {
  uint32_t val, reg32 = 0;

  val = inv ? 1 : 0;
  credo_tx_0x43_rx_data_flip_rmw(dev_id, dev_port, ln, &reg32, val);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_polarity_set
 *
 *          tx_pn = 1 - tx_pn # 1 == NO INVERT
 *          chip.PAM50[ln].tx_pol_pam4(val=tx_pn)
 *          chip.PAM50[ln].rx_pol_pam4(val=rx_pn)
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_polarity_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 bool inv) {
  port_mgr_log("%d:%3d:%d Serdes Rx (hw) Polarity set: %d",
               dev_id,
               dev_port,
               ln,
               inv ? 1 : 0);
  // always set both PAM4 and NRZ
  port_mgr_tof2_serdes_rx_polarity_nrz_set(dev_id, dev_port, ln, inv);
  port_mgr_tof2_serdes_rx_polarity_pam4_set(dev_id, dev_port, ln, inv);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_polarity_nrz_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_polarity_nrz_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     bool *inv) {
  uint32_t val, reg32 = 0;

  credo_rx_0x61_rx_pol_flip_get(dev_id, dev_port, ln, &reg32, &val, true);
  *inv = (val == 0) ? false : true;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_polarity_pam4_get
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_polarity_pam4_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      bool *inv) {
  uint32_t val, reg32 = 0;

  credo_tx_0x43_rx_data_flip_get(dev_id, dev_port, ln, &reg32, &val, true);
  *inv = (val == 0) ? false : true;
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_polarity_get
 *
 *          tx_pn = 1 - tx_pn # 1 == NO INVERT
 *          chip.PAM50[ln].tx_pol_pam4(val=tx_pn)
 *          chip.PAM50[ln].rx_pol_pam4(val=rx_pn)
 *
 *
 */
bf_status_t port_mgr_tof2_serdes_rx_polarity_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 bool *inv) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_rx_polarity_nrz_get(dev_id, dev_port, ln, inv);
  } else {
    port_mgr_tof2_serdes_rx_polarity_pam4_get(dev_id, dev_port, ln, inv);
  }
  port_mgr_log("%d:%3d:%d Serdes Rx (hw) Polarity get: %d",
               dev_id,
               dev_port,
               ln,
               *inv ? 1 : 0);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ppm_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ppm_nrz_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             int32_t *ppm) {
  uint32_t reg32 = 0, fld_val;

  credo_rx_0x73_rx_read_freq_err_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  *ppm = (fld_val & 0x7FF);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ppm_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ppm_pam4_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              int32_t *ppm) {
  uint32_t reg32 = 0, fld_val;

  credo_tx_0x73_read_freq_acc_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  *ppm = (fld_val & 0x7FF);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ppm_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ppm_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         int32_t *ppm) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_ppm_nrz_get(dev_id, dev_port, ln, ppm);
  } else {
    port_mgr_tof2_serdes_ppm_pam4_get(dev_id, dev_port, ln, ppm);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_of_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_of_nrz_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *of) {
  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 1, 4, of);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_of_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_of_pam4_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *of) {
  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 2, 4, of);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_of_get
 *
 */
bf_status_t port_mgr_tof2_serdes_of_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        uint32_t *of) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_of_nrz_get(dev_id, dev_port, ln, of);
  } else {
    port_mgr_tof2_serdes_of_pam4_get(dev_id, dev_port, ln, of);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_hf_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_hf_nrz_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *hf) {
  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 1, 5, hf);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_hf_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_hf_pam4_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *hf) {
  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 2, 5, hf);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_hf_get
 *
 */
bf_status_t port_mgr_tof2_serdes_hf_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        uint32_t *hf) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_hf_nrz_get(dev_id, dev_port, ln, hf);
  } else {
    port_mgr_tof2_serdes_hf_pam4_get(dev_id, dev_port, ln, hf);
  }
  return BF_SUCCESS;
}

/******************************************************************************
 * port_mgr_tof2_serdes_gray_bin_get
 *
 * Utility to convert graycode value to binary
 *
 *def Gray_Bin(gg=0): # up to 7-bit Gray Number
 *    bb1 = (gg & 0x40)
 *    bb2 = (gg ^ (bb1 >> 1)) & (0x20)
 *    bb3 = (gg ^ (bb2 >> 1)) & (0x10)
 *    bb4 = (gg ^ (bb3 >> 1)) & (0x8)
 *    bb5 = (gg ^ (bb4 >> 1)) & (0x4)
 *    bb6 = (gg ^ (bb5 >> 1)) & (0x2)
 *    bb7 = (gg ^ (bb6 >> 1)) & (0x1)
 *    bb = bb1+bb2+bb3+bb4+bb5+bb6+bb7
 *    return bb
 */
bf_status_t port_mgr_tof2_serdes_gray_bin_get(uint32_t gg, uint32_t *bb) {
  uint32_t bb1, bb2, bb3, bb4, bb5, bb6, bb7;

  bb1 = (gg & 0x40);
  bb2 = (gg ^ (bb1 >> 1)) & (0x20);
  bb3 = (gg ^ (bb2 >> 1)) & (0x10);
  bb4 = (gg ^ (bb3 >> 1)) & (0x8);
  bb5 = (gg ^ (bb4 >> 1)) & (0x4);
  bb6 = (gg ^ (bb5 >> 1)) & (0x2);
  bb7 = (gg ^ (bb6 >> 1)) & (0x1);
  *bb = bb1 + bb2 + bb3 + bb4 + bb5 + bb6 + bb7;

  return BF_SUCCESS;
}

/******************************************************************************
 * port_mgr_tof2_serdes_gray_bin_get
 *
 * Utility to convert graycode value to binary
 *
 * def Bin_Gray(bb):
 *    gg=bb ^ (bb >> 1)
 *    return gg
 */
bf_status_t port_mgr_tof2_serdes_bin_gray_get(uint32_t bb, uint32_t *gg) {
  *gg = bb ^ (bb >> 1);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_delta_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_delta_nrz_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *delta) {
  uint32_t reg32 = 0;

  credo_rx_0xd_delta_val_get(dev_id, dev_port, ln, &reg32, delta, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_delta_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_delta_pam4_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *delta) {
  uint32_t reg32 = 0;

  credo_tx_0x12_delta_ow_get(dev_id, dev_port, ln, &reg32, delta, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_delta_get
 *
 */
bf_status_t port_mgr_tof2_serdes_delta_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           uint32_t *delta) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_delta_nrz_get(dev_id, dev_port, ln, delta);
  } else {
    port_mgr_tof2_serdes_delta_pam4_get(dev_id, dev_port, ln, delta);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_edge_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_edge_nrz_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint32_t *edge1,
                                              uint32_t *edge2,
                                              uint32_t *edge3,
                                              uint32_t *edge4) {
  uint32_t reg32 = 0;

  credo_rsvd_0x2d_edge1_get(dev_id, dev_port, ln, &reg32, edge1, true);
  credo_rsvd_0x2d_edge2_get(dev_id, dev_port, ln, &reg32, edge2, false);
  credo_rsvd_0x2d_edge3_get(dev_id, dev_port, ln, &reg32, edge3, false);
  credo_rsvd_0x2d_edge4_get(dev_id, dev_port, ln, &reg32, edge4, false);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_edge_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_edge_pam4_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *edge1,
                                               uint32_t *edge2,
                                               uint32_t *edge3,
                                               uint32_t *edge4) {
  uint32_t reg32 = 0;

  credo_rsvd_0x2d_edge1_get(dev_id, dev_port, ln, &reg32, edge1, true);
  credo_rsvd_0x2d_edge2_get(dev_id, dev_port, ln, &reg32, edge2, false);
  credo_rsvd_0x2d_edge3_get(dev_id, dev_port, ln, &reg32, edge3, false);
  credo_rsvd_0x2d_edge4_get(dev_id, dev_port, ln, &reg32, edge4, false);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_edge_get
 *
 */
bf_status_t port_mgr_tof2_serdes_edge_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          uint32_t *edge1,
                                          uint32_t *edge2,
                                          uint32_t *edge3,
                                          uint32_t *edge4) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_edge_nrz_get(
        dev_id, dev_port, ln, edge1, edge2, edge3, edge4);
  } else {
    port_mgr_tof2_serdes_edge_pam4_get(
        dev_id, dev_port, ln, edge1, edge2, edge3, edge4);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_dfe_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_dfe_nrz_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *tap1,
                                             uint32_t *tap2,
                                             uint32_t *tap3) {
  uint32_t reg32 = 0;

  credo_rx_0x2b_rx_dfe_tap_1_curr_val_get(
      dev_id, dev_port, ln, &reg32, tap1, true);
  credo_rx_0x2c_rx_dfe_tap_2_curr_val_get(
      dev_id, dev_port, ln, &reg32, tap2, true);
  credo_rx_0x2c_rx_dfe_tap_3_curr_val_get(
      dev_id, dev_port, ln, &reg32, tap3, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_dfe_pam4_get
 *
 *    ths_sel_list = [x for x in range(12)]
 *    ths_list = []
 *    wait = 0.01
 *    for val in ths_sel_list:
 *        lane_obj.THS_SEL = val
 *        time.sleep(wait)
 *        readout = lane_obj.Reg002F_15_4
 *        readout2 = lane_obj.Reg002F_15_4
 *        if readout == readout2:
 *            result = ((float(readout) / 2048.0) + 1.0) % 2.0 - 1.0
 *            if print_en: print("\n%2d 0x%04X %6.3f"%(val, readout, result)),
 *        else:
 *            readout = lane_obj.Reg002F_15_4
 *            result = ((float(readout) / 2048.0) + 1.0) % 2.0 - 1.0
 *            if print_en: print("\n*%2d 0x%04X %6.3f"%(val, readout, result)),
 *        ths_list.append(result)
 *    f0 = (-3.0 / 16) * ((ths_list[0] - ths_list[2]) + (ths_list[3] -
 *ths_list[5]) + (ths_list[6] - ths_list[8]) + (ths_list[9] - ths_list[11]))
 *    f1 = (-3.0 / 20) * ((ths_list[0] + ths_list[1] + ths_list[2] - ths_list[9]
 *- ths_list[10] - ths_list[11]) + (1 / 3) * (ths_list[3] + ths_list[4] +
 *ths_list[5] - ths_list[6] - ths_list[7] - ths_list[8])
 */
bf_status_t port_mgr_tof2_serdes_dfe_pam4_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              float *f0,
                                              float *f1,
                                              float *ratio) {
  uint32_t ths_sel;
  float ths_list[12];
  uint32_t dfe_val[12];
  float result;

  port_mgr_tof2_serdes_fw_dfe_pam4_get(dev_id, dev_port, ln, dfe_val);

  for (ths_sel = 0; ths_sel < 12; ths_sel++) {
    // result = ((((float)readout)/2048.0) + 1.0) % 2.0 - 1.0;
    float temp = (((float)dfe_val[ths_sel]) / 2048.0) + 1.0;
    // manual "mod"
    while ((temp - 2.0) > 0.0) {
      temp -= 2.0;
    }
    result = temp - 1.0;
    // result = (temp - ((temp/2)*2)) - 1.0;
    ths_list[ths_sel] = result;
  }
  *f0 = (-3.0 / 16) *
        ((ths_list[0] - ths_list[2]) + (ths_list[3] - ths_list[5]) +
         (ths_list[6] - ths_list[8]) + (ths_list[9] - ths_list[11]));
  *f1 = (-3.0 / 20) * ((ths_list[0] + ths_list[1] + ths_list[2] - ths_list[9] -
                        ths_list[10] - ths_list[11]) +
                       (1 / 3) * (ths_list[3] + ths_list[4] + ths_list[5] -
                                  ths_list[6] - ths_list[7] - ths_list[8]));
  if (*f0 == 0.0) {
    *ratio = 0.0;
  } else {
    *ratio = *f1 / *f0;
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_f13_val_pam4_get
 *
 * No NRZ equivalent.
 */
bf_status_t port_mgr_tof2_serdes_f13_val_pam4_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *f13_val) {
  uint32_t reg32 = 0;

  credo_tx_0x4_dfe_init_val_get(dev_id, dev_port, ln, &reg32, f13_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_skef_val_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_skef_val_nrz_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *skef_val) {
  uint32_t reg32 = 0;

  credo_rx_0xdd_skef_val_get(dev_id, dev_port, ln, &reg32, skef_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_skef_val_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_skef_val_pam4_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t *skef_val) {
  uint32_t reg32 = 0;

  credo_rx_0xdd_skef_val_get(dev_id, dev_port, ln, &reg32, skef_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_skef_val_get
 *
 */
bf_status_t port_mgr_tof2_serdes_skef_val_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint32_t *skef_val) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_skef_val_nrz_get(dev_id, dev_port, ln, skef_val);
  } else {
    port_mgr_tof2_serdes_skef_val_pam4_get(dev_id, dev_port, ln, skef_val);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_dac_val_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_dac_val_nrz_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t *dac_val) {
  uint32_t reg32 = 0;

  credo_rx_0x7f_dac_sel_get(dev_id, dev_port, ln, &reg32, dac_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_dac_val_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_dac_val_pam4_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *dac_val) {
  uint32_t reg32 = 0;

  credo_tx_0x28_read_dac_sel_get(dev_id, dev_port, ln, &reg32, dac_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_dac_val_get
 *
 */
bf_status_t port_mgr_tof2_serdes_dac_val_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *dac_val) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_dac_val_nrz_get(dev_id, dev_port, ln, dac_val);
  } else {
    port_mgr_tof2_serdes_dac_val_pam4_get(dev_id, dev_port, ln, dac_val);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_eye_nrz_get
 *
 * dac_val = chip.NRZ25[ln].DAC_SEL
 * eye = (float(chip.NRZ25[ln].RX_READ_EM) / 2048.0) * (200 + (50.0 *
 *float(dac_val)))
 *
 */
bf_status_t port_mgr_tof2_serdes_eye_nrz_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             float *eye) {
  uint32_t reg32 = 0, dac_val, eye_em;

  port_mgr_tof2_serdes_dac_val_get(dev_id, dev_port, ln, &dac_val);
  credo_rx_0x2a_rx_read_em_get(dev_id, dev_port, ln, &reg32, &eye_em, true);
  *eye = ((((float)eye_em) / 2048.0) * (200 + (50.0 * (float)dac_val)));
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_eye_pam4_get
 *
 *def eye_pam4(lane_obj):
 *    DACQ_reg = lane_obj.READ_DAC_SEL
 *    eye_margin = []
 *    for eye_index in range(0, 3):
 *        result1 = 0xffff
 *        for y in range(0, 4):
 *            sel = 3 * y + eye_index
 *            lane_obj.MINUS_MARGIN_SEL = sel
 *            lane_obj.PLUS_MARGIN_SEL = sel
 *            plus_margin = lane_obj.Reg0032_15_4
 *            if (plus_margin > 0x7ff):
 *                plus_margin = plus_margin - 0x1000
 *            minus_margin = lane_obj.Reg0032_3_0S
 *            if (minus_margin > 0x7ff):
 *                minus_margin = minus_margin - 0x1000
 *            diff = plus_margin - minus_margin
 *            if (diff < result1):
 *                result1 = diff
 *            else:
 *                result1 = result1
 *        eye_margin.append((result1))
 *    em0, em1, em2 = map(lambda em: float(em) / 2048.0 * (100.0 + 50.0 *
 *float(DACQ_reg)), eye_margin)
 *    return em0, em1, em2
 *
 */
bf_status_t port_mgr_tof2_serdes_eye_pam4_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              float *eye_1,
                                              float *eye_2,
                                              float *eye_3) {
  uint32_t reg32 = 0, dacq_reg, eye_index, y;
  uint32_t eye_margin[3] = {0};

  credo_tx_0x28_read_dac_sel_get(dev_id, dev_port, ln, &reg32, &dacq_reg, true);
  for (eye_index = 0; eye_index < 3; eye_index++) {
    uint32_t result1 = 0xffff;

    for (y = 0; y < 5; y++) {
      uint32_t plus_margin = 0, minus_margin = 0, diff, minus_margin_msb,
               minus_margin_lsb;
      uint32_t sel = (3 * y) + eye_index;
      credo_tx_0x88_minus_margin_sel_rmw(dev_id, dev_port, ln, &reg32, sel);
      credo_tx_0x88_plus_margin_sel_rmw(dev_id, dev_port, ln, &reg32, sel);
      reg32 = 0;
      credo_tx_0x32_read_plus_margin_2c_get(
          dev_id, dev_port, ln, &reg32, &plus_margin, true);
      if (plus_margin > 0x7ff) {
        plus_margin = plus_margin - 0x1000;
      }
      reg32 = 0;
      credo_tx_0x32_read_minus_margin_2c_msb_get(
          dev_id, dev_port, ln, &reg32, &minus_margin_msb, true);
      credo_tx_0x33_read_minus_margin_2c_lsb_get(
          dev_id, dev_port, ln, &reg32, &minus_margin_lsb, true);
      minus_margin = (minus_margin_msb << 8) | minus_margin_lsb;
      if (minus_margin > 0x7ff) {
        minus_margin = minus_margin - 0x1000;
      }
      diff = plus_margin - minus_margin;
      if (diff < result1) {
        result1 = diff;
      }
    }
    eye_margin[eye_index] = result1;
  }
  // printf("em_1=%d : em_2=%d : em_3=%d\n",eye_margin[ 0 ],eye_margin[ 1 ],
  // eye_margin[ 2 ]);
  *eye_1 = (float)eye_margin[0] / 2048.0 * (100.0 + 50.0 * (float)dacq_reg);
  *eye_2 = (float)eye_margin[1] / 2048.0 * (100.0 + 50.0 * (float)dacq_reg);
  *eye_3 = (float)eye_margin[2] / 2048.0 * (100.0 + 50.0 * (float)dacq_reg);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_eye_fw_get
 *
 *        fw_debug_cmd(section=10, index=5, lane=lane)
 *        em = [rreg(0x5000+eye_index) for eye_index in range(3)]
 */
bf_status_t port_mgr_tof2_serdes_eye_pam4_fw_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 float *eye_1,
                                                 float *eye_2,
                                                 float *eye_3) {
  uint32_t reg32 = 0, res;
  uint32_t eye_ht[3] = {0};

  port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, ln, 10, 5, &res);
  credo_fw_0x0_firmware_data_0_get(
      dev_id, dev_port, ln, &reg32, &eye_ht[0], true);
  credo_fw_0x1_firmware_data_1_get(
      dev_id, dev_port, ln, &reg32, &eye_ht[1], true);
  credo_fw_0x2_firmware_data_2_get(
      dev_id, dev_port, ln, &reg32, &eye_ht[2], true);
  port_mgr_log(
      "port_mgr_tof2_serdes_eye_pam4_get: res=%04x eye_1=%04x  eye_2=%04x "
      "eye_3=%04x",
      res,
      eye_ht[0],
      eye_ht[1],
      eye_ht[2]);

  *eye_1 = (float)eye_ht[0];
  *eye_2 = (float)eye_ht[1];
  *eye_3 = (float)eye_ht[2];

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_eye_get
 *
 */
bf_status_t port_mgr_tof2_serdes_eye_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         float *eye_1,
                                         float *eye_2,
                                         float *eye_3) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_eye_nrz_get(dev_id, dev_port, ln, eye_1);
  } else {
    port_mgr_tof2_serdes_eye_pam4_fw_get(
        dev_id, dev_port, ln, eye_1, eye_2, eye_3);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_val_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_val_nrz_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t ctle_sel,
                                                  uint32_t *ctle_map_0,
                                                  uint32_t *ctle_map_1) {
  uint32_t reg32 = 0, fld_val, fld_val2, ctle_lsb;

  bf_sys_assert(ctle_sel < 8);

  if (ctle_sel == 0) {
    credo_rx_0x76_nrz_ctle_table_0_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 1) {
    credo_rx_0x76_nrz_ctle_table_1_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 2) {
    credo_rx_0x76_nrz_ctle_table_2_msb_get(
        dev_id, dev_port, ln, &reg32, &fld_val2, true);
    credo_rx_0x77_nrz_ctle_table_2_lsb_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    fld_val = fld_val | (fld_val2 << 2);
  } else if (ctle_sel == 3) {
    credo_rx_0x77_nrz_ctle_table_3_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 4) {
    credo_rx_0x77_nrz_ctle_table_4_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 5) {
    credo_rx_0x77_nrz_ctle_table_5_msb_get(
        dev_id, dev_port, ln, &reg32, &fld_val2, true);
    credo_rx_0x78_nrz_ctle_table_5_lsb_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    fld_val = fld_val | (fld_val2 << 4);
  } else if (ctle_sel == 6) {
    credo_rx_0x78_nrz_ctle_table_6_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 7) {
    credo_rx_0x78_nrz_ctle_table_7_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else {
    return BF_INVALID_ARG;
  }
  ctle_lsb = fld_val;

  credo_rx_0xd7_ctle_msb_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  // MSB "field" realy has MSB for both map0 (3) and map1 (2)
  *ctle_map_0 = ((ctle_lsb >> 3) & 7) | (((fld_val >> 1) & 1) << 3);
  *ctle_map_1 = ((ctle_lsb >> 0) & 7) | (((fld_val >> 0) & 1) << 3);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_val_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_val_pam4_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t ctle_sel,
                                                   uint32_t *ctle_map_0,
                                                   uint32_t *ctle_map_1) {
  uint32_t reg32 = 0, fld_val, fld_val2, ctle_lsb;

  bf_sys_assert(ctle_sel < 8);

  if (ctle_sel == 0) {
    credo_tx_0x48_pam4_ctle_table_0_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 1) {
    credo_tx_0x48_pam4_ctle_table_1_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 2) {
    credo_tx_0x48_pam4_ctle_table_2_msb_get(
        dev_id, dev_port, ln, &reg32, &fld_val2, true);
    credo_tx_0x49_pam4_ctle_table_2_lsb_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    fld_val = fld_val | (fld_val2 << 2);
  } else if (ctle_sel == 3) {
    credo_tx_0x49_pam4_ctle_table_3_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 4) {
    credo_tx_0x49_pam4_ctle_table_4_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 5) {
    credo_tx_0x49_pam4_ctle_table_5_msb_get(
        dev_id, dev_port, ln, &reg32, &fld_val2, true);
    credo_tx_0x4a_pam4_ctle_table_5_lsb_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    fld_val = fld_val | (fld_val2 << 4);
  } else if (ctle_sel == 6) {
    credo_tx_0x4a_pam4_ctle_table_6_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else if (ctle_sel == 7) {
    credo_tx_0x4a_pam4_ctle_table_7_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
  } else {
    return BF_INVALID_ARG;
  }
  ctle_lsb = fld_val;

  credo_rx_0xd7_ctle_msb_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  // MSB "field" realy has MSB for both map0 (3) and map1 (2)
  *ctle_map_0 = ((ctle_lsb >> 3) & 7) | (((fld_val >> 1) & 1) << 3);
  *ctle_map_1 = ((ctle_lsb >> 0) & 7) | (((fld_val >> 0) & 1) << 3);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_val_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_val_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint32_t ctle_sel,
                                              uint32_t *ctle_map_0,
                                              uint32_t *ctle_map_1) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_ctle_val_nrz_get(
        dev_id, dev_port, ln, ctle_sel, ctle_map_0, ctle_map_1);
  } else {
    port_mgr_tof2_serdes_ctle_val_pam4_get(
        dev_id, dev_port, ln, ctle_sel, ctle_map_0, ctle_map_1);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_over_val_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_over_val_nrz_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t *ctle_over_val) {
  uint32_t reg32 = 0;

  credo_rx_0x4e_rx_nrz_ctle_over_val_get(
      dev_id, dev_port, ln, &reg32, ctle_over_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_over_val_pam4_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_over_val_pam4_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t *ctle_over_val) {
  uint32_t reg32 = 0;

  credo_tx_0x21_rx_pam4_ctle_over_val_get(
      dev_id, dev_port, ln, &reg32, ctle_over_val, true);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_over_val_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_over_val_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t *ctle_over_val) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_ctle_over_val_nrz_get(
        dev_id, dev_port, ln, ctle_over_val);
  } else {
    port_mgr_tof2_serdes_ctle_over_val_pam4_get(
        dev_id, dev_port, ln, ctle_over_val);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_nrz_set
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_nrz_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint32_t ctle_over_val) {
  uint32_t reg32 = 0;

  credo_rx_0x4e_rx_nrz_ctle_over_val_rmw(
      dev_id, dev_port, ln, &reg32, ctle_over_val);
  credo_rx_0x4d_rx_nrz_ctle_over_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_pam4_set
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_pam4_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t ctle_over_val) {
  uint32_t reg32 = 0;

  credo_tx_0x21_rx_pam4_ctle_over_val_rmw(
      dev_id, dev_port, ln, &reg32, ctle_over_val);
  credo_tx_0x21_rx_pam4_ctle_over_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_set
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          uint32_t ctle_over_val) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_ctle_nrz_set(dev_id, dev_port, ln, ctle_over_val);
  } else {
    port_mgr_tof2_serdes_ctle_pam4_set(dev_id, dev_port, ln, ctle_over_val);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_agcgain_nrz_set
 *
 */
bf_status_t port_mgr_tof2_serdes_agcgain_nrz_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t ctle_gain_1,
                                                 uint32_t ctle_gain_2) {
  uint32_t reg32 = 0, gain_1, gain_2;

  port_mgr_tof2_serdes_bin_gray_get(ctle_gain_1, &gain_1);
  port_mgr_tof2_serdes_bin_gray_get(ctle_gain_2, &gain_2);

  credo_rx_0xd4_ctle_gain_1_rmw(dev_id, dev_port, ln, &reg32, gain_1);
  credo_rx_0xd4_ctle_gain_2_rmw(dev_id, dev_port, ln, &reg32, gain_2);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_agcgain_pam4_set
 *
 */
bf_status_t port_mgr_tof2_serdes_agcgain_pam4_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t ctle_gain_1,
                                                  uint32_t ctle_gain_2) {
  uint32_t reg32 = 0, gain_1, gain_2;

  port_mgr_tof2_serdes_bin_gray_get(ctle_gain_1, &gain_1);
  port_mgr_tof2_serdes_bin_gray_get(ctle_gain_2, &gain_2);

  credo_rx_0xd4_ctle_gain_1_rmw(dev_id, dev_port, ln, &reg32, gain_1);
  credo_rx_0xd4_ctle_gain_2_rmw(dev_id, dev_port, ln, &reg32, gain_2);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_agcgain_set
 *
 */
bf_status_t port_mgr_tof2_serdes_agcgain_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t ctle_gain_1,
                                             uint32_t ctle_gain_2) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_agcgain_nrz_set(
        dev_id, dev_port, ln, ctle_gain_1, ctle_gain_2);
  } else {
    port_mgr_tof2_serdes_agcgain_pam4_set(
        dev_id, dev_port, ln, ctle_gain_1, ctle_gain_2);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_gain_nrz_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_gain_nrz_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t *ctle_gain_1,
                                                   uint32_t *ctle_gain_2) {
  uint32_t reg32 = 0, gain_1, gain_2;

  credo_rx_0xd4_ctle_gain_1_get(dev_id, dev_port, ln, &reg32, &gain_1, true);
  credo_rx_0xd4_ctle_gain_2_get(dev_id, dev_port, ln, &reg32, &gain_2, true);

  port_mgr_tof2_serdes_gray_bin_get(gain_1, ctle_gain_1);
  port_mgr_tof2_serdes_gray_bin_get(gain_2, ctle_gain_2);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_gain_pam4_get
 *
 * (same as NRZ)
 */
bf_status_t port_mgr_tof2_serdes_ctle_gain_pam4_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *ctle_gain_1,
                                                    uint32_t *ctle_gain_2) {
  uint32_t reg32 = 0, gain_1, gain_2;

  credo_rx_0xd4_ctle_gain_1_get(dev_id, dev_port, ln, &reg32, &gain_1, true);
  credo_rx_0xd4_ctle_gain_2_get(dev_id, dev_port, ln, &reg32, &gain_2, true);

  port_mgr_tof2_serdes_gray_bin_get(gain_1, ctle_gain_1);
  port_mgr_tof2_serdes_gray_bin_get(gain_2, ctle_gain_2);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_ctle_gain_get
 *
 */
bf_status_t port_mgr_tof2_serdes_ctle_gain_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *ctle_gain_1,
                                               uint32_t *ctle_gain_2) {
  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    port_mgr_tof2_serdes_ctle_gain_nrz_get(
        dev_id, dev_port, ln, ctle_gain_1, ctle_gain_2);
  } else {
    port_mgr_tof2_serdes_ctle_gain_pam4_get(
        dev_id, dev_port, ln, ctle_gain_1, ctle_gain_2);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
* port_mgr_tof2_serdes_ffe_taps_pam4_get
*
        pol1 = lane_obj.POL_MUX1
        rx_ffe_k1_msb = Gray_Bin(lane_obj.DEGENMAIN1_SUM3)
        rx_ffe_k1_lsb = Gray_Bin(lane_obj.DEGENMAIN0_SUM3)
        rx_ffe_k1_bin = (1 - 2 * pol1) * (((rx_ffe_k1_msb << 4) + rx_ffe_k1_lsb)
& 0xFF)

        pol2 = lane_obj.POL_MUX2
        rx_ffe_k2_msb = Gray_Bin(lane_obj.DEGENMAIN1_SUM2)
        rx_ffe_k2_lsb = Gray_Bin(lane_obj.DEGENMAIN0_SUM2)
        rx_ffe_k2_bin = (1 - 2 * pol2) * (((rx_ffe_k2_msb << 4) + rx_ffe_k2_lsb)
& 0xFF)

        pol3 = lane_obj.POL_MUX3
        rx_ffe_k3_msb = Gray_Bin(lane_obj.DEGENMAIN1_SUM1)
        rx_ffe_k3_lsb = Gray_Bin(lane_obj.DEGENMAIN0_SUM1)
        rx_ffe_k3_bin = (1 - 2 * pol3) * (((rx_ffe_k3_msb << 4) + rx_ffe_k3_lsb)
& 0xFF)

        pol4 = lane_obj.POL_MUX4
        rx_ffe_k4_msb = Gray_Bin(lane_obj.DEGENSUM1_SUM1)
        rx_ffe_k4_lsb = Gray_Bin(lane_obj.DEGENSUM0_SUM1)
        rx_ffe_k4_bin = (1 - 2 * pol4) * (((rx_ffe_k4_msb << 4) + rx_ffe_k4_lsb)
& 0xFF)

        rx_ffe_s1_msb = Gray_Bin(lane_obj.DEGENSUM1_SUM3)
        rx_ffe_s1_lsb = Gray_Bin(lane_obj.DEGENSUM0_SUM3)
        rx_ffe_s1_bin = ((rx_ffe_s1_msb << 4) + rx_ffe_s1_lsb) & 0xFF

        rx_ffe_s2_msb = Gray_Bin(lane_obj.DEGENSUM1_SUM2)
        rx_ffe_s2_lsb = Gray_Bin(lane_obj.DEGENSUM0_SUM3)
        rx_ffe_s2_bin = ((rx_ffe_s2_msb << 4) + rx_ffe_s2_lsb) & 0xFF
        return rx_ffe_k1_bin, rx_ffe_k2_bin, rx_ffe_k3_bin, rx_ffe_k4_bin,
rx_ffe_s1_bin, rx_ffe_s2_bin

*/
bf_status_t port_mgr_tof2_serdes_ffe_taps_pam4_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   int32_t *k1,
                                                   int32_t *k2,
                                                   int32_t *k3,
                                                   int32_t *k4,
                                                   int32_t *s1,
                                                   int32_t *s2) {
  uint32_t reg32 = 0, fld_val, pol_mux, kx_msb, kx_lsb;

  // k1
  credo_rx_0xe0_pol_mux1_get(dev_id, dev_port, ln, &reg32, &pol_mux, true);

  credo_rx_0xe2_degenmain1_sum3_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_msb);

  credo_rx_0xe2_degenmain0_sum3_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_lsb);
  *k1 = (1 - 2 * pol_mux) * (((kx_msb << 4) + kx_lsb) & 0xFF);

  // k2
  credo_rx_0xe0_pol_mux2_get(dev_id, dev_port, ln, &reg32, &pol_mux, true);

  credo_rx_0xe3_degenmain1_sum2_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_msb);

  credo_rx_0xe3_degenmain0_sum2_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_lsb);
  *k2 = (1 - 2 * pol_mux) * (((kx_msb << 4) + kx_lsb) & 0xFF);

  // k3
  credo_rx_0xe0_pol_mux3_get(dev_id, dev_port, ln, &reg32, &pol_mux, true);

  credo_rx_0xe4_degenmain1_sum1_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_msb);

  credo_rx_0xe4_degenmain0_sum1_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_lsb);
  *k3 = (1 - 2 * pol_mux) * (((kx_msb << 4) + kx_lsb) & 0xFF);

  // k4
  credo_rx_0xe0_pol_mux4_get(dev_id, dev_port, ln, &reg32, &pol_mux, true);

  credo_rx_0xe4_degensum1_sum1_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_msb);

  credo_rx_0xe4_degensum0_sum1_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_lsb);
  *k4 = (1 - 2 * pol_mux) * (((kx_msb << 4) + kx_lsb) & 0xFF);

  // s1
  credo_rx_0xe2_degensum1_sum3_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_msb);
  credo_rx_0xe2_degensum0_sum3_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_lsb);

  *s1 = ((kx_msb << 4) + kx_lsb) & 0xFF;

  // s2
  credo_rx_0xe3_degensum1_sum2_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_msb);
  credo_rx_0xe3_degensum0_sum2_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  port_mgr_tof2_serdes_gray_bin_get(fld_val, &kx_lsb);

  *s2 = ((kx_msb << 4) + kx_lsb) & 0xFF;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_tx_rx_serial_loopback_nrz
 *
 *        if enable == 1:
 *          if phase == 0:
 *            chip.NRZ25[lane].tx_taps(0, 0, 15, -4, 0)
 *            chip.NRZ25[lane].Reg01E7_13 = 1
 *            chip.NRZ25[lane].Reg00FF_3 = 1
 *            chip.NRZ25[lane].Reg0100_0001_14 = 0
 *            #time.sleep(3)
 *          elif phase == 1:
 *            fw_reg(addr=0x8, data=0xffff-2**lane) # inactive FW
 *            chip.MdioWr(0x10b + 0x800 * lane, 0x0)
 *            chip.NRZ25[lane].NRZ_SM_CONT = 0
 *            chip.NRZ25[lane].NRZ_SM_CONT = 1
 *            chip.NRZ25[lane].agcgain(0, 0)
 *            chip.NRZ25[lane].ctle_nrz(7)
 *            #time.sleep(0.1)
 *          else:
 *            chip.NRZ25[lane].lane_reset()
 *
 *        else:
 *            ########################################################################################################################
 *            # After you enable the loopback, if you want to active the FW, you
 *need to set the enable=0 to active the FW
 *            ########################################################################################################################
 *            chip.NRZ25[lane].Reg01E7_13 = 0
 *            chip.NRZ25[lane].Reg00FF_3 = 0
 *            chip.NRZ25[lane].Reg0100_0001_14 = 1
 *            chip.GROUP8_TOP[0].fw_reg(chip, addr=0x8, data=0xffff) # active FW
 *            chip.NRZ25[lane].tx_taps(0, -8, 17, 0, 0)
 *
 */
bf_status_t port_mgr_tof2_serdes_tx_rx_serial_loopback_nrz(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t en,
    uint32_t phase) {
  uint32_t reg32 = 0, fld_val;
  if (en) {
    if (phase == 0) {
      // chip.NRZ25[lane].tx_taps(0, 0, 15, -4, 0)
      port_mgr_tof2_serdes_tx_taps_set(dev_id, dev_port, ln, 0, 0, 15, -4, 0);
      credo_rx_0xe7_rx_t2r_serial_lpbk_en_rmw(dev_id, dev_port, ln, &reg32, 1);
      credo_rsvd_0x3f_tx_t2r_serial_lpbk_en_rmw(
          dev_id, dev_port, ln, &reg32, 1);

      credo_rx_0x1_delta_adapt_en_get(
          dev_id, dev_port, ln, &reg32, &fld_val, true);
      fld_val &= ~0x1;  // force clear "bit 14"
      credo_rx_0x1_delta_adapt_en_set(
          dev_id, dev_port, ln, &reg32, fld_val, true);

    } else if (phase == 1) {
      port_mgr_tof2_serdes_fw_reg_wr(
          dev_id, dev_port, ln, 0x8, 0xffff & ~(1 << ln));
      reg32 = 0;  // write whole reg to 0
      credo_rx_0xb_nrz_bp1_en_set(dev_id, dev_port, ln, &reg32, 0, true);
      credo_rx_0xc_nrz_sm_cont_rmw(dev_id, dev_port, ln, &reg32, 0);
      credo_rx_0xc_nrz_sm_cont_rmw(dev_id, dev_port, ln, &reg32, 1);
      // chip.NRZ25[lane].agcgain(0, 0)
      // chip.NRZ25[lane].ctle_nrz(7)
      port_mgr_tof2_serdes_agcgain_set(dev_id, dev_port, ln, 0, 0);
      port_mgr_tof2_serdes_ctle_set(dev_id, dev_port, ln, 7);
    } else {
      port_mgr_tof2_serdes_lane_reset_set(dev_id, dev_port, ln);
    }
  } else {
    credo_rx_0xe7_rx_t2r_serial_lpbk_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x3f_tx_t2r_serial_lpbk_en_rmw(dev_id, dev_port, ln, &reg32, 0);

    credo_rx_0x1_delta_adapt_en_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    fld_val |= 0x1;  // force set "bit 14"
    credo_rx_0x1_delta_adapt_en_set(
        dev_id, dev_port, ln, &reg32, fld_val, true);

    port_mgr_tof2_serdes_fw_reg_wr(dev_id, dev_port, ln, 0x8, 0xffff);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fec_analyzer_tei_get
 *
 *def fec_analyzer_tei(lane=None):
 *    chip.FECANA[lane].READ_SEL = 4
 *    tei_l = chip.FECANA[lane].READ_DATA       # read data
 *    chip.FECANA[lane].READ_SEL = 5            # set reading data of TEi high
 *16 bit tei_h = chip.FECANA[lane].READ_DATA       # read data tei = tei_h *
 *65536 + tei_l               # combinate the data return tei
 */
bf_status_t port_mgr_tof2_serdes_fec_analyzer_tei_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t *tei) {
  uint32_t reg32 = 0, tei_l, tei_h;

  credo_fec_analyzer_0xd_read_sel_rmw(dev_id, dev_port, ln, &reg32, 4);
  credo_fec_analyzer_0x7_read_data_get(
      dev_id, dev_port, ln, &reg32, &tei_l, true);
  credo_fec_analyzer_0xd_read_sel_rmw(dev_id, dev_port, ln, &reg32, 5);
  credo_fec_analyzer_0x7_read_data_get(
      dev_id, dev_port, ln, &reg32, &tei_h, true);
  *tei = (tei_h << 16) | tei_l;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fec_analyzer_teo_get
 *
 *def fec_analyzer_teo(lane=None):
 *    chip.FECANA[lane].READ_SEL = 6       #set reading data of TEo low 16 bit
 *    teo_l = chip.FECANA[lane].READ_DATA  #read data
 *    chip.FECANA[lane].READ_SEL = 7       #set reading data of TEo high 16 bit
 *    teo_h = chip.FECANA[lane].READ_DATA  #read data
 *    teo = teo_h*65536+teo_l              #combinate the data
 *    return teo
 *
 */
bf_status_t port_mgr_tof2_serdes_fec_analyzer_teo_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t *teo) {
  uint32_t reg32 = 0, teo_l, teo_h;

  credo_fec_analyzer_0xd_read_sel_rmw(dev_id, dev_port, ln, &reg32, 6);
  credo_fec_analyzer_0x7_read_data_get(
      dev_id, dev_port, ln, &reg32, &teo_l, true);
  credo_fec_analyzer_0xd_read_sel_rmw(dev_id, dev_port, ln, &reg32, 7);
  credo_fec_analyzer_0x7_read_data_get(
      dev_id, dev_port, ln, &reg32, &teo_h, true);
  *teo = (teo_h << 16) | teo_l;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_fec_analyzer_init_set
 *
 *        chip.FECANA[ln].PRBS_CHK_CNTR_RESET = 0
 *        chip.FECANA[ln].RX_PRBS_FORCE_RELOAD = 1
 *        chip.FECANA[ln].RX_PRBS_AUTO_SYNC_EN = 1
 *        chip.FECANA[ln].PRBS_MISMATCH_THR = 0x10
 *        chip.FECANA[ln].PRBS_SYNC_THR = 0x2
 *        chip.FECANA[ln].PRBS_MODE = 0x3
 *        chip.FECANA[ln].SYM_SIZE = M
 *        chip.FECANA[ln].FRAM_SIZE = N
 *        chip.FECANA[ln].CORR_SIZE = T
 *        chip.FECANA[ln].TH_SIZE = T
 *        chip.FECANA[ln].CNT_CLR = 1
 *        chip.FECANA[ln].CNT_FREEZE = 0
 *        chip.FECANA[ln].FEC_CLK_EN = 1
 *        chip.FECANA[ln].FEC_ANA_EN = 1
 *        chip.FECANA[ln].CNT_CLR = 0
 *        chip.FECANA[ln].CTRL_TEO = err_type
 *        chip.FECANA[ln].CTRL_TEI = err_type
 *
 */
bf_status_t port_mgr_tof2_serdes_fec_analyzer_init_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       uint32_t ln,
                                                       uint32_t err_type,
                                                       uint32_t T,
                                                       uint32_t M,
                                                       uint32_t N) {
  uint32_t reg32 = 0;

  credo_fec_analyzer_0x9_fec_anz_prbs_chk_cntr_reset_rmw(
      dev_id, dev_port, ln, &reg32, 0);
  credo_fec_analyzer_0x9_fec_anz_rx_prbs_force_reload_rmw(
      dev_id, dev_port, ln, &reg32, 1);
  credo_fec_analyzer_0x9_fec_anz_rx_prbs_auto_sync_en_rmw(
      dev_id, dev_port, ln, &reg32, 1);
  credo_fec_analyzer_0x9_fec_anz_prbs_mismatch_thr_rmw(
      dev_id, dev_port, ln, &reg32, 0x10);
  credo_fec_analyzer_0x9_fec_anz_prbs_sync_thr_rmw(
      dev_id, dev_port, ln, &reg32, 2);
  credo_fec_analyzer_0x8_prbs_mode_rmw(dev_id, dev_port, ln, &reg32, 0x3);
  credo_fec_analyzer_0x0_sym_size_rmw(dev_id, dev_port, ln, &reg32, M);
  credo_fec_analyzer_0x4_frame_size_rmw(dev_id, dev_port, ln, &reg32, N);
  credo_fec_analyzer_0x5_corr_size_rmw(dev_id, dev_port, ln, &reg32, T);
  credo_fec_analyzer_0x5_th_size_rmw(dev_id, dev_port, ln, &reg32, T);
  credo_fec_analyzer_0x1_cnt_clr_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_fec_analyzer_0x1_cnt_freeze_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_fec_analyzer_0x1_fec_clk_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_fec_analyzer_0x1_fec_ana_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_fec_analyzer_0x1_cnt_clr_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_fec_analyzer_0xc_ctrl_teo_rmw(dev_id, dev_port, ln, &reg32, err_type);
  credo_fec_analyzer_0xb_ctrl_tei_rmw(dev_id, dev_port, ln, &reg32, err_type);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_power_on_self_test
 */
bf_status_t port_mgr_tof2_serdes_power_on_self_test(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t n_lanes) {
  uint32_t ln, reg32 = 0, fld_val = 0;
  uint32_t wr_val;

  for (ln = 0; ln < n_lanes; ln++) {
    credo_tx_0xa1_tx_test_pat_3_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val != 0xAAAA) {
      // access failure or device not reset prior to running POST
      // port_mgr_log("%d:%3d.%d: 0xA1 not 0xAAAA <%04x> , resetting..",
      //             dev_id,
      //             dev_port,
      //             ln,
      //             fld_val);
      reg32 = 0xAAAA;
      credo_tx_0xa1_tx_test_pat_3_set(
          dev_id, dev_port, ln, &reg32, 0xAAAA, true);
      credo_tx_0xa1_tx_test_pat_3_get(
          dev_id, dev_port, ln, &reg32, &fld_val, true);
      if (fld_val != 0xAAAA) {
        port_mgr_log("%d:%3d.%d: 0xA1 still not 0xAAAA <%04x> , FAILED..",
                     dev_id,
                     dev_port,
                     ln,
                     fld_val);
        return BF_HW_COMM_FAIL;
      }
    }

    wr_val = ~0xAAAA & 0xffff;
    credo_tx_0xa1_tx_test_pat_3_set(dev_id, dev_port, ln, &reg32, wr_val, true);
    credo_tx_0xa1_tx_test_pat_3_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val != wr_val) {
      port_mgr_log("%d:%3d.%d: 0xA1 still not 0x5555 <%04x> , FAILED..",
                   dev_id,
                   dev_port,
                   ln,
                   fld_val);
      return BF_HW_COMM_FAIL;
    }
    credo_tx_0xa1_tx_test_pat_3_set(dev_id, dev_port, ln, &reg32, 0xAAAA, true);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_sram_bist_grp
 */
bf_status_t port_mgr_tof2_serdes_sram_bist_grp(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port) {
  uint32_t ln = 0, reg32 = 0, fld_val = 0;
  uint32_t still_trying = 100;

  credo_group8_0x2f_sram_bist_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_group8_0x2f_sram_test_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  bf_sys_usleep(1000000);
  credo_group8_0x2f_sram_bist_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x2f_sram_test_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  while (--still_trying) {
    credo_group8_0x30_sram_bist_done_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val == 1) {
      break;
    }
    bf_sys_usleep(100000);
  }
  if (!still_trying) {
    port_mgr_log("%d:%3d.-: SRAM BIST timed out", dev_id, dev_port);
    return BF_INVALID_ARG;
  } else {
    bf_sys_usleep(100000);
    credo_group8_0x30_sram_bist_pass_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val == 0) {
      port_mgr_log("%d:%3d.-: SRAM BIST FAILED", dev_id, dev_port);
      return BF_INVALID_ARG;
    } else {
      port_mgr_log("%d:%3d.-: SRAM BIST PASSED", dev_id, dev_port);
    }
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rom_bist_grp
 */
bf_status_t port_mgr_tof2_serdes_rom_bist_grp(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port) {
  uint32_t ln = 0, reg32 = 0, fld_val = 0;
  uint32_t still_trying = 100;

  credo_group8_0x2f_rom_bist_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_group8_0x2f_rom_test_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  bf_sys_usleep(1000000);
  credo_group8_0x2f_rom_bist_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_group8_0x2f_rom_test_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  while (--still_trying) {
    credo_group8_0x30_rom_test_done_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    if (fld_val == 1) {
      break;
    }
    bf_sys_usleep(100000);
  }
  if (!still_trying) {
    port_mgr_log("%d:%3d.-: ROM BIST timed out", dev_id, dev_port);
    return BF_INVALID_ARG;
  } else {
    uint32_t r1, r2;
    bf_sys_usleep(100000);
    credo_group8_0x31_rom_test_result_31_16__get(
        dev_id, dev_port, ln, &reg32, &r1, true);
    credo_group8_0x32_rom_test_result_15_0__get(
        dev_id, dev_port, ln, &reg32, &r2, true);
    if ((r1 != 0xf804) || (r2 != 0x126f)) {
      port_mgr_log(
          "%d:%3d.-: ROM BIST FAILED <%04x_%04x>", dev_id, dev_port, r1, r2);
      return BF_INVALID_ARG;
    } else {
      port_mgr_log("%d:%3d.-: ROM BIST PASSED", dev_id, dev_port);
    }
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_eq_set
 *
 * Cache Tx Eq params in serdes struct and optionally apply the changes
 * to hw.
 ****************************************************************************/
bf_status_t port_mgr_tof2_serdes_tx_eq_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           int32_t pre2,
                                           int32_t pre1,
                                           int32_t main,
                                           int32_t post1,
                                           int32_t post2,
                                           bool apply) {
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    sd_p->pre2 = pre2;
    sd_p->pre1 = pre1;
    sd_p->main = main;
    sd_p->post1 = post1;
    sd_p->post2 = post2;
    if (apply) {
      port_mgr_tof2_serdes_tx_taps_set(
          dev_id, dev_port, ln, pre2, pre1, main, post1, post2);
    }
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_eq_get
 *
 * Retreive any or all the Tx Eq params from the serdes struct
 ****************************************************************************/
bf_status_t port_mgr_tof2_serdes_tx_eq_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           int32_t *pre2,
                                           int32_t *pre1,
                                           int32_t *main,
                                           int32_t *post1,
                                           int32_t *post2) {
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (pre2) *pre2 = sd_p->pre2;
    if (pre1) *pre1 = sd_p->pre1;
    if (main) *main = sd_p->main;
    if (post1) *post1 = sd_p->post1;
    if (post2) *post2 = sd_p->post2;
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_pol_inv_set
 *
 * Cache Tx polarity inversion state and optionally apply to hw
 */
bf_status_t port_mgr_tof2_serdes_tx_pol_inv_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool inv,
                                                bool apply) {
  bf_status_t rc = BF_SUCCESS;
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    // cache it
    sd_p->tx_inv = inv;
    if (apply) {
      // apply it
      rc = port_mgr_tof2_serdes_tx_polarity_set(dev_id, dev_port, ln, inv);
    }
    return rc;
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_pol_inv_get
 *
 * Retreive cached Tx polarity inversion state
 */
bf_status_t port_mgr_tof2_serdes_tx_pol_inv_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool *inv) {
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (inv) {
      *inv = sd_p->tx_inv;
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_pol_inv_set
 *
 * Cache Rx polarity inversion state and optionally apply to hw
 */
bf_status_t port_mgr_tof2_serdes_rx_pol_inv_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool inv,
                                                bool apply) {
  bf_status_t rc = BF_SUCCESS;
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    // cache it
    sd_p->rx_inv = inv;
    if (apply) {
      // apply it
      rc = port_mgr_tof2_serdes_rx_polarity_set(dev_id, dev_port, ln, inv);
    }
    return rc;
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_pol_inv_get
 *
 * Retreive cached Rx polarity inversion state
 */
bf_status_t port_mgr_tof2_serdes_rx_pol_inv_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                bool *inv) {
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (inv) {
      *inv = sd_p->rx_inv;
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_bandgap_set
 *
 * Cache Rx bandgap setting and optionally apply to hw
 */
bf_status_t port_mgr_tof2_serdes_rx_bandgap_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t rx_bg,
                                                bool apply) {
  bf_status_t rc = BF_SUCCESS;
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    // cache it
    sd_p->rx_bg = rx_bg;
    if (apply) {
      // apply it
      rc = port_mgr_tof2_serdes_rx_bandgap_hw_set(dev_id, dev_port, ln, rx_bg);
    }
    return rc;
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_bandgap_get
 *
 * Retreive cached Rx bandgap setting
 */
bf_status_t port_mgr_tof2_serdes_rx_bandgap_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *rx_bg) {
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (rx_bg) {
      *rx_bg = sd_p->rx_bg;
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_rx_bandgap_hw_get
 *
 * Retreive programmed Rx bandgap setting
 */
bf_status_t port_mgr_tof2_serdes_rx_bandgap_hw_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t *rx_bg) {
  port_mgr_tof2_serdes_t *sd_p;
  uint32_t reg32;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (rx_bg) {
      credo_rx_0xff_rx_vbg_get(dev_id, dev_port, ln, &reg32, rx_bg, true);
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_bandgap_set
 *
 * Cache Rx bandgap setting and optionally apply to hw
 */
bf_status_t port_mgr_tof2_serdes_tx_bandgap_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t tx_bg,
                                                bool apply) {
  bf_status_t rc = BF_SUCCESS;
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    // cache it
    sd_p->tx_bg = tx_bg;
    if (apply) {
      // apply it
      rc = port_mgr_tof2_serdes_tx_bandgap_hw_set(dev_id, dev_port, ln, tx_bg);
    }
    return rc;
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_bandgap_get
 *
 * Retreive cached Rx bandgap setting
 */
bf_status_t port_mgr_tof2_serdes_tx_bandgap_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *tx_bg) {
  port_mgr_tof2_serdes_t *sd_p;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (tx_bg) {
      *tx_bg = sd_p->tx_bg;
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_tx_bandgap_hw_get
 *
 * Retreive programmed Tx bandgap setting
 */
bf_status_t port_mgr_tof2_serdes_tx_bandgap_hw_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t *tx_bg) {
  port_mgr_tof2_serdes_t *sd_p;
  uint32_t reg32;

  sd_p = port_mgr_tof2_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  if (sd_p != NULL) {
    if (tx_bg) {
      credo_rsvd_0x3f_tx_vbg_get(dev_id, dev_port, ln, &reg32, tx_bg, true);
      return BF_SUCCESS;
    }
  }
  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof2_serdes_power_dn_set
 *
 * power up/dn serdes blocks
 */
bf_status_t port_mgr_tof2_serdes_power_dn_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              bool rx_off,
                                              bool tx_off,
                                              bool rx_bg_off,
                                              bool tx_bg_off) {
  uint32_t reg32 = 0, fld_val = 0;

  if (rx_bg_off) {
    credo_rx_0xff_pu_rx_bg_rmw(dev_id, dev_port, ln, &reg32, 0);
  } else {
    credo_rx_0xff_pu_rx_bg_rmw(dev_id, dev_port, ln, &reg32, 1);
  }

  if (tx_bg_off) {
    credo_rsvd_0x3f_pu_tx_bg_rmw(dev_id, dev_port, ln, &reg32, 0);
  } else {
    credo_rsvd_0x3f_pu_tx_bg_rmw(dev_id, dev_port, ln, &reg32, 1);
  }

  if (rx_off) {
    credo_group8_0xcf_firmware_15_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    reg32 |= (1 << ln);
    credo_group8_0xcf_firmware_15_set(
        dev_id, dev_port, ln, &reg32, reg32, true);

    credo_rx_0x81_nrz_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_tx_0x0_pam4_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xff_rx_vbg_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xff_pu_rx_rvdd_rmw(dev_id, dev_port, ln, &reg32, 0);
    // credo_rx_0xfd_pu_rx_pll_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xff_pu_agc_master_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xfe_pu_agc_1_master_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xfe_pu_agcdl_master_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xf8_pu_adc_master_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe7_pu_rx_agc_ln_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe7_pu_agcdl_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xf3_pu_rx_pll_intp_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xff_pu_rx_cp_vreg_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xf8_pu_intp_master_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xfc_rx_cp_vreg_1_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xfc_rx_cp_vreg_2_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe0_pu_degenmain_sum1_msb_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe0_pu_degenmain_sum1_lsb_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe0_pu_degenmain_sum2_msb_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe0_pu_degenmain_sum2_lsb_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe0_pu_degenmain_sum3_msb_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xe0_pu_degenmain_sum3_lsb_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xdd_pu_intp_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_fec_analyzer_0x1_fec_ana_en_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x31_pu_adc_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x31_pu_clkcomp_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x31_pu_clkcompreg_rmw(dev_id, dev_port, ln, &reg32, 0);
  } else {
    credo_group8_0xcf_firmware_15_get(
        dev_id, dev_port, ln, &reg32, &fld_val, true);
    reg32 &= ~(1 << ln);
    credo_group8_0xcf_firmware_15_set(
        dev_id, dev_port, ln, &reg32, reg32, true);

    credo_rx_0x81_nrz_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_tx_0x0_pam4_sm_reset_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rx_0xff_rx_vbg_rmw(
        dev_id, dev_port, ln, &reg32, 4);  // FIXME, re pgm rx bandgap
    credo_rx_0xff_pu_rx_rvdd_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xfd_pu_rx_pll_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xff_pu_agc_master_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xfe_pu_agc_1_master_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xfe_pu_agcdl_master_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xf8_pu_adc_master_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe7_pu_rx_agc_ln_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe7_pu_agcdl_rmw(
        dev_id, dev_port, ln, &reg32, 1);  // FIXME, nrz=0, pam4=1
    credo_rx_0xf3_pu_rx_pll_intp_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xff_pu_rx_cp_vreg_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xf8_pu_intp_master_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xfc_rx_cp_vreg_1_rmw(dev_id, dev_port, ln, &reg32, 4);
    credo_rx_0xfc_rx_cp_vreg_2_rmw(dev_id, dev_port, ln, &reg32, 4);
    credo_rx_0xe0_pu_degenmain_sum1_msb_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe0_pu_degenmain_sum1_lsb_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe0_pu_degenmain_sum2_msb_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe0_pu_degenmain_sum2_lsb_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe0_pu_degenmain_sum3_msb_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xe0_pu_degenmain_sum3_lsb_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rx_0xdd_pu_intp_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x31_pu_adc_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x31_pu_clkcomp_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x31_pu_clkcompreg_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_fec_analyzer_0x1_fec_ana_en_rmw(
        dev_id, dev_port, ln, &reg32, 1);  // required?
  }

  if (tx_off) {
    credo_rsvd_0x3f_tx_vbg_rmw(dev_id, dev_port, ln, &reg32, 0);
    // chip.NRZ25[ln].PU_TX_VREG = 0   #pu_rvdd_tx    //FIXME, note: set to "1"
    // on enable but never to "0"?
    // credo_rsvd_0x3f_pu_tx_rvdd_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x3a_pu_vdrv_ma_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x2b_pu_tx_drv_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x2a_pu_himode_vddr_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x3f_pu_tx_loop_vreg_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x3d_tx_vreg_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0x3d_tx_pll_vreg_rmw(dev_id, dev_port, ln, &reg32, 0);
    credo_rsvd_0xe_pu_acjtag_rmw(dev_id, dev_port, ln, &reg32, 0);
  } else {
    credo_rsvd_0x3f_tx_vbg_rmw(
        dev_id, dev_port, ln, &reg32, 0);  // FIXME, re pgm tx bandgap
    // chip.NRZ25[ln].PU_TX_VREG = 1   #pu_rvdd_tx    //FIXME
    credo_rsvd_0x3f_pu_tx_rvdd_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x3e_pu_tx_pll_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x3a_pu_vdrv_ma_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x2b_pu_tx_drv_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x2a_pu_himode_vddr_rmw(
        dev_id, dev_port, ln, &reg32, 0);  // leave at 0
    credo_rsvd_0x3f_pu_tx_loop_vreg_rmw(dev_id, dev_port, ln, &reg32, 1);
    credo_rsvd_0x3d_tx_vreg_rmw(dev_id, dev_port, ln, &reg32, 0x3);
    credo_rsvd_0x3d_tx_pll_vreg_rmw(dev_id, dev_port, ln, &reg32, 0x3);
    // credo_rsvd_0xe_pu_acjtag_rmw(dev_id, dev_port, ln, &reg32, 1); //note:
    // set to "0" on disable but not written in python on enable
  }

  return BF_SUCCESS;
}

#ifndef TILE_SIM

/** \brief tof-2 clkobs pad config
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
bf_status_t port_mgr_tof2_serdes_clkobs_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            bf_clkobs_pad_t pad,
                                            bf_sds_clkobs_clksel_t clk_src,
                                            int divider,
                                            int daisy_sel) {
  uint32_t val, clkobs, misc_clkpad_ctrl, reg32;
  bf_mac_block_id_t i, mac_block;
  int lane;
  uint32_t phys_tx_ln[8], phys_rx_ln[8];

  if (divider < 0 || divider > 3 || daisy_sel < 0 || daisy_sel > 1) {
    return BF_INVALID_ARG;
  }
  if ((pad != BF_CLKOBS_PAD_0) && (pad != BF_CLKOBS_PAD_1)) {
    return BF_INVALID_ARG;
  }
  if (bf_port_map_dev_port_to_mac(dev_id, dev_port, &mac_block, &lane) !=
      BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  if (port_mgr_tof2_port_lane_map_get(
          dev_id, dev_port, phys_tx_ln, phys_rx_ln) != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }

  val = 0;
  switch (clk_src) {
    case BF_SDS_RX_RECOVEREDCLK:
      clkobs = (phys_rx_ln[lane] & 0x7);
      break;
    case BF_SDS_TX_CLK:
      clkobs = (1 << 3) | (phys_tx_ln[lane] & 0x7);
      break;
    case BF_SDS_NONE_CLK:
      /* just deselect this MAC from driving the clk daisy chain */
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw(
          dev_id, mac_block, &reg32, val);
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw(
          dev_id, mac_block, &reg32, val);
      return BF_SUCCESS;
    default:
      return BF_INVALID_ARG;
  }

  // deselect any previous setting that might be driving clkobd_pad
  for (i = 1; i <= 32; i++) {
    if (i != mac_block) {
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw(dev_id, i, &reg32, val);
      eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw(dev_id, i, &reg32, val);
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
        dev_id, mac_block, &reg32, val);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_rmw(
        dev_id, mac_block, &reg32, 0);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_rmw(
        dev_id, mac_block, &reg32, clkobs);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_rmw(
        dev_id, mac_block, &reg32, divider);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_rmw(
        dev_id, mac_block, &reg32, val);
    misc_clkpad_ctrl &= ~(0x7UL << 4);
    misc_clkpad_ctrl |= ((daisy_sel | (1 << 2)) << 4);
  } else {
    eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw(
        dev_id, mac_block, &reg32, val);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_rmw(
        dev_id, mac_block, &reg32, 0);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_rmw(
        dev_id, mac_block, &reg32, clkobs);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_rmw(
        dev_id, mac_block, &reg32, divider);
    eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_rmw(
        dev_id, mac_block, &reg32, val);
    misc_clkpad_ctrl &= ~(0x7UL);
    misc_clkpad_ctrl |= (daisy_sel | (1 << 2));
  }
  lld_write_register(dev_id,
                     tof2_reg_device_select_misc_regs_clkpad_ctrl_address,
                     misc_clkpad_ctrl);
  return BF_SUCCESS;
}

/** \brief tof-2 clkobs drive strength config
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param drive_strength: Clock observation pad drive strength. (0 ~ 15)
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_clkobs_drive_strength_set(bf_dev_id_t dev_id,
                                                    int drive_strength) {
  uint32_t misc_clkpad_ctrl = 0;

  if ((drive_strength < 0) || (drive_strength > 15)) {
    return BF_INVALID_ARG;
  }

  /*
     register __device_select__misc_regs__clkpad_ctrl
             # Each 2 bit controls the clock divider of the clock observation
  pad: # -[0]   : Input enable   -[1]   : Pull-up   -[2]   : Pull-down   -[3] #
  : Strong pull  -[7:4] : Clock observation pad drive strength [15: 8] 10 :
  clkpad_ctrl
  */

  lld_read_register(dev_id,
                    tof2_reg_device_select_misc_regs_clkpad_ctrl_address,
                    &misc_clkpad_ctrl);

  misc_clkpad_ctrl &= ~(0xF000UL);
  misc_clkpad_ctrl |= (drive_strength << 12);

  lld_write_register(dev_id,
                     tof2_reg_device_select_misc_regs_clkpad_ctrl_address,
                     misc_clkpad_ctrl);
  return BF_SUCCESS;
}

#endif

/** \brief Return the logical channel (on a given octal) represented
 *         by lane 0 of the dev_port. This is required for FW cmds
 *         which add the logical lane into the command word.
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
static uint32_t port_mgr_tof2_serdes_base_chnl_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port) {
  uint32_t ch, rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, NULL, &ch, NULL);
  bf_sys_assert(rc == 0);
  return ch;
}

/** \brief Set Tx band gap (default=7)
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_tx_bandgap_hw_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t tx_bg_val) {
  uint32_t reg32 = 0;

  credo_rsvd_0x3f_tx_vbg_rmw(dev_id, dev_port, ln, &reg32, tx_bg_val);
  return BF_SUCCESS;
}

/** \brief Set Rx band gap (default=7)
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_rx_bandgap_hw_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t rx_bg_val) {
  uint32_t reg32 = 0;

  credo_rx_0xff_rx_vbg_rmw(dev_id, dev_port, ln, &reg32, rx_bg_val);
  return BF_SUCCESS;
}

/** \brief Return the AN status fields)
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_an_status_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t *lp_an_ability,
    uint32_t *link_status,
    uint32_t *an_ability,
    uint32_t *remote_fault,
    uint32_t *an_complete,
    uint32_t *page_rcvd,
    uint32_t *ext_np_status,
    uint32_t *parallel_detect_fault) {
  uint32_t reg32 = 0;

  // only 1 reg, read it in first
  credo_an_lt_0x1_an_complete_get(
      dev_id, dev_port, ln, &reg32, an_complete, true);

  // then pick the fields out
  credo_an_lt_0x1_parallel_detection_fault_get(
      dev_id, dev_port, ln, &reg32, parallel_detect_fault, false);
  credo_an_lt_0x1_extnp_status_get(
      dev_id, dev_port, ln, &reg32, ext_np_status, false);
  credo_an_lt_0x1_page_received_get(
      dev_id, dev_port, ln, &reg32, page_rcvd, false);
  credo_an_lt_0x1_an_complete_get(
      dev_id, dev_port, ln, &reg32, an_complete, false);
  credo_an_lt_0x1_remote_fault_get(
      dev_id, dev_port, ln, &reg32, remote_fault, false);
  credo_an_lt_0x1_an_ability_get(
      dev_id, dev_port, ln, &reg32, an_ability, false);
  credo_an_lt_0x1_link_status_get(
      dev_id, dev_port, ln, &reg32, link_status, false);
  credo_an_lt_0x1_lp_an_ability_get(
      dev_id, dev_port, ln, &reg32, lp_an_ability, false);

  return BF_SUCCESS;
}

/** \brief Return the AN lp base page
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_an_lp_base_page_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint64_t *lp_basepage) {
  uint32_t lp_15_00;
  uint32_t lp_31_16;
  uint32_t lp_47_32;
  uint32_t reg32 = 0;

  if (!lp_basepage) return BF_INVALID_ARG;

  credo_an_lt_0x13_an_lp_d_15_0_get(
      dev_id, dev_port, 0, &reg32, &lp_15_00, true);
  credo_an_lt_0x14_an_lp_d_31_16_get(
      dev_id, dev_port, 0, &reg32, &lp_31_16, true);
  credo_an_lt_0x15_an_lp_d_47_32_get(
      dev_id, dev_port, 0, &reg32, &lp_47_32, true);

  *lp_basepage = (lp_47_32 & 0xFFFFull) << 32ull |
                 (lp_31_16 & 0xFFFFull) << 16ull | (lp_15_00 & 0xFFFFull);

  return BF_SUCCESS;
}

/** \brief Return the AN lp pages
 *
 * Note: this function works with FW version 1.2.25 and later. It returns
 * the partner base page and also the first 2 next pages, if they were
 * negotiated.
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_an_lp_pages_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint64_t *lp_basepage,
                                                 uint64_t *lp_nextpage1,
                                                 uint64_t *lp_nextpage2) {
  uint32_t lp_15_00;
  uint32_t lp_31_16;
  uint32_t lp_47_32;
  uint32_t reg32 = 0;
  uint32_t result;
  bf_status_t rc;

  if (!lp_basepage || !lp_nextpage1 || !lp_nextpage2) return BF_INVALID_ARG;

  *lp_nextpage1 = 0;
  *lp_nextpage2 = 0;

  rc = port_mgr_tof2_serdes_fw_debug_cmd(dev_id, dev_port, 0, 3, 20, &result);
  if (rc != BF_SUCCESS) return rc;

  // read link partner base page
  credo_fw_0x0_firmware_data_0_get(
      dev_id, dev_port, 0, &reg32, &lp_15_00, true);
  credo_fw_0x1_firmware_data_1_get(
      dev_id, dev_port, 0, &reg32, &lp_31_16, true);
  credo_fw_0x2_firmware_data_2_get(
      dev_id, dev_port, 0, &reg32, &lp_47_32, true);

  *lp_basepage = (lp_47_32 & 0xFFFFull) << 32ull |
                 (lp_31_16 & 0xFFFFull) << 16ull | (lp_15_00 & 0xFFFFull);

  if (!(lp_15_00 & 0x8000)) return BF_SUCCESS;

  // read link partner first next page
  credo_fw_0x3_firmware_data_3_get(
      dev_id, dev_port, 0, &reg32, &lp_15_00, true);
  credo_fw_0x4_firmware_data_4_get(
      dev_id, dev_port, 0, &reg32, &lp_31_16, true);
  credo_fw_0x5_firmware_data_5_get(
      dev_id, dev_port, 0, &reg32, &lp_47_32, true);

  *lp_nextpage1 = (lp_47_32 & 0xFFFFull) << 32ull |
                  (lp_31_16 & 0xFFFFull) << 16ull | (lp_15_00 & 0xFFFFull);

  if (!(lp_15_00 & 0x8000)) return BF_SUCCESS;
  // read link partner second next page
  credo_fw_0x6_firmware_data_6_get(
      dev_id, dev_port, 0, &reg32, &lp_15_00, true);
  credo_fw_0x7_firmware_data_7_get(
      dev_id, dev_port, 0, &reg32, &lp_31_16, true);
  credo_fw_0x8_firmware_data_8_get(
      dev_id, dev_port, 0, &reg32, &lp_47_32, true);

  *lp_nextpage2 = (lp_47_32 & 0xFFFFull) << 32ull |
                  (lp_31_16 & 0xFFFFull) << 16ull | (lp_15_00 & 0xFFFFull);

  return BF_SUCCESS;
}

/** \brief Return the HCD
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 * \param hcd     : Speed negotiated (if any)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_an_hcd_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *hcd,
                                            bool *base_r_fec,
                                            bool *rs_fec) {
  uint32_t reg32 = 0, fld32;

  credo_an_lt_0xfc_read_back_base_200g_kr4_cr4_get(
      dev_id, dev_port, ln, &reg32, hcd, true);
  credo_an_lt_0x30_base_r_fec_negotiated_get(
      dev_id, dev_port, ln, &reg32, &fld32, true);
  *base_r_fec = fld32 ? true : false;
  credo_an_lt_0x30_rs_fec_negotiated__get(
      dev_id, dev_port, ln, &reg32, &fld32, false);
  *rs_fec = fld32 ? true : false;
  return BF_SUCCESS;
}

/** \brief Return the LT status fields
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_lt_status_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *readout_state,
                                               uint32_t *frame_lock,
                                               uint32_t *rx_trained,
                                               uint32_t *readout_training_state,
                                               uint32_t *training_failure,
                                               uint32_t *tx_training_data_en,
                                               uint32_t *sig_det,
                                               uint32_t *readout_txstate) {
  uint32_t reg32 = 0;

  // only 1 reg, read it in first
  credo_link_trng_0x4_readout_state_get(
      dev_id, dev_port, ln, &reg32, readout_state, true);

  // then pick the fields out
  credo_link_trng_0x4_frame_lock_get(
      dev_id, dev_port, ln, &reg32, frame_lock, false);
  credo_link_trng_0x4_rx_trained_get(
      dev_id, dev_port, ln, &reg32, rx_trained, false);
  credo_link_trng_0x4_readout_training_state_get(
      dev_id, dev_port, ln, &reg32, readout_training_state, false);
  credo_link_trng_0x4_training_failure_get(
      dev_id, dev_port, ln, &reg32, training_failure, false);
  credo_link_trng_0x4_tx_training_data_en_get(
      dev_id, dev_port, ln, &reg32, tx_training_data_en, false);
  credo_link_trng_0x4_sig_det_get(dev_id, dev_port, ln, &reg32, sig_det, false);
  credo_link_trng_0x4_readout_txstate_get(
      dev_id, dev_port, ln, &reg32, readout_txstate, false);

  return BF_SUCCESS;
}

/** \brief Get ISI info on NRZ lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_isi_nrz_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t isi_val[16]) {
  (void)dev_id;
  (void)dev_port;
  (void)ln;
  (void)isi_val;
  return BF_SUCCESS;
}

/** \brief FW info get
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_fw_info_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t mode,
                                             uint32_t index,
                                             int32_t n_vals,
                                             uint32_t *rtn_val,
                                             bool signed_vals) {
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);
  uint32_t reg32, rsp, cmd;
  bf_status_t rc;
  uint32_t val[16];

  mode |= 8;
  cmd = 0xB000 + ((mode & 0xF) << 4) + (base_ln + ln);
  rc = port_mgr_tof2_serdes_fw_cmd_w_detail(
      dev_id, dev_port, ln, cmd, &rsp, index);
  if (rc == BF_SUCCESS) {
    if (rsp == 0xb) {
      credo_fw_0x0_firmware_data_0_get(
          dev_id, dev_port, ln, &reg32, &val[0], true);
      credo_fw_0x1_firmware_data_1_get(
          dev_id, dev_port, ln, &reg32, &val[1], true);
      credo_fw_0x2_firmware_data_2_get(
          dev_id, dev_port, ln, &reg32, &val[2], true);
      credo_fw_0x3_firmware_data_3_get(
          dev_id, dev_port, ln, &reg32, &val[3], true);
      credo_fw_0x4_firmware_data_4_get(
          dev_id, dev_port, ln, &reg32, &val[4], true);
      credo_fw_0x5_firmware_data_5_get(
          dev_id, dev_port, ln, &reg32, &val[5], true);
      credo_fw_0x6_firmware_data_6_get(
          dev_id, dev_port, ln, &reg32, &val[6], true);
      credo_fw_0x7_firmware_data_7_get(
          dev_id, dev_port, ln, &reg32, &val[7], true);
      credo_fw_0x8_firmware_data_8_get(
          dev_id, dev_port, ln, &reg32, &val[8], true);
      credo_fw_0x9_firmware_data_9_get(
          dev_id, dev_port, ln, &reg32, &val[9], true);
      credo_fw_0xa_firmware_data_a_get(
          dev_id, dev_port, ln, &reg32, &val[10], true);
      credo_fw_0xb_firmware_data_b_get(
          dev_id, dev_port, ln, &reg32, &val[11], true);
      credo_fw_0xc_firmware_data_c_get(
          dev_id, dev_port, ln, &reg32, &val[12], true);
      credo_fw_0xd_firmware_data_d_get(
          dev_id, dev_port, ln, &reg32, &val[13], true);
      credo_fw_0xe_firmware_data_e_get(
          dev_id, dev_port, ln, &reg32, &val[14], true);
      credo_fw_0xf_firmware_data_f_get(
          dev_id, dev_port, ln, &reg32, &val[15], true);
      for (int i = 0; i < n_vals; i++) {
        if (signed_vals && (val[i] >= 0x8000)) {
          rtn_val[i] = val[i] - 0x10000;
        } else {
          rtn_val[i] = val[i];
        }
      }
    }
  }
  return BF_SUCCESS;
}

/** \brief Get DFE info (from FW) on PAM4 lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_fw_dfe_pam4_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t dfe_val[12]) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_info_get(
      dev_id, dev_port, ln, 10, 1, 12, dfe_val, true);
  return rc;
}

/** \brief Get ISI info on PAM4 lane
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_isi_pam4_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint32_t isi_val[16]) {
#if 1
  port_mgr_tof2_serdes_fw_info_get(
      dev_id, dev_port, ln, 10, 0, 16, isi_val, true);
#else
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);
  uint32_t reg32, rsp, cmd;
  bf_status_t rc;

  cmd = 0xB000 + ((10 & 0xF) << 4) + (base_ln + ln);
  rc = port_mgr_tof2_serdes_fw_cmd(dev_id, dev_port, ln, cmd, &rsp);
  if (rc == BF_SUCCESS) {
    if (rsp == 0xb) {
      credo_fw_0x0_firmware_data_0_get(
          dev_id, dev_port, ln, &reg32, &isi_val[0], true);
      credo_fw_0x1_firmware_data_1_get(
          dev_id, dev_port, ln, &reg32, &isi_val[1], true);
      credo_fw_0x2_firmware_data_2_get(
          dev_id, dev_port, ln, &reg32, &isi_val[2], true);
      credo_fw_0x3_firmware_data_3_get(
          dev_id, dev_port, ln, &reg32, &isi_val[3], true);
      credo_fw_0x4_firmware_data_4_get(
          dev_id, dev_port, ln, &reg32, &isi_val[4], true);
      credo_fw_0x5_firmware_data_5_get(
          dev_id, dev_port, ln, &reg32, &isi_val[5], true);
      credo_fw_0x6_firmware_data_6_get(
          dev_id, dev_port, ln, &reg32, &isi_val[6], true);
      credo_fw_0x7_firmware_data_7_get(
          dev_id, dev_port, ln, &reg32, &isi_val[7], true);
      credo_fw_0x8_firmware_data_8_get(
          dev_id, dev_port, ln, &reg32, &isi_val[8], true);
      credo_fw_0x9_firmware_data_9_get(
          dev_id, dev_port, ln, &reg32, &isi_val[9], true);
      credo_fw_0xa_firmware_data_a_get(
          dev_id, dev_port, ln, &reg32, &isi_val[10], true);
      credo_fw_0xb_firmware_data_b_get(
          dev_id, dev_port, ln, &reg32, &isi_val[11], true);
      credo_fw_0xc_firmware_data_c_get(
          dev_id, dev_port, ln, &reg32, &isi_val[12], true);
      credo_fw_0xd_firmware_data_d_get(
          dev_id, dev_port, ln, &reg32, &isi_val[13], true);
      credo_fw_0xe_firmware_data_e_get(
          dev_id, dev_port, ln, &reg32, &isi_val[14], true);
      credo_fw_0xf_firmware_data_f_get(
          dev_id, dev_port, ln, &reg32, &isi_val[15], true);
      for (int i = 0; i < 16; i++) {
        if (isi_val[i] >= 0x8000) {
          isi_val[i] -= 0x10000;
        }
      }
    }
  }
#endif
  return BF_SUCCESS;
}

/** \brief Get ISI info

 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_isi_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         uint32_t isi_vals[16]) {
  bf_status_t rc = BF_INVALID_ARG;

  if (port_mgr_tof2_serdes_mode_is_nrz(dev_id, dev_port, ln)) {
    rc = port_mgr_tof2_serdes_isi_nrz_get(dev_id, dev_port, ln, isi_vals);
  } else {
    rc = port_mgr_tof2_serdes_isi_pam4_get(dev_id, dev_port, ln, isi_vals);
  }
  return rc;
}

/** \brief Inject bit errors

 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_error_inject_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t n_errs) {
  bf_status_t rc = BF_INVALID_ARG;
  uint32_t reg32;
  int i;

  credo_tx_0xa0_tx_prbs_gen_err_rmw(dev_id, dev_port, ln, &reg32, 0);
  for (i = 0; i < (int)n_errs; i++) {
    credo_tx_0xa0_tx_prbs_gen_err_rmw(dev_id, dev_port, ln, &reg32, 1);
    bf_sys_usleep(10000);
    credo_tx_0xa0_tx_prbs_gen_err_rmw(dev_id, dev_port, ln, &reg32, 0);
    bf_sys_usleep(10000);
  }

  return rc;
}

/** \brief Retrieve the FW debug info for AN/LT

 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_an_lt_debug_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t *r_800c7,
                                                 uint32_t *r_800c8,
                                                 uint32_t *r_800c9,
                                                 uint32_t *r_800ca,
                                                 uint32_t *r_800cb) {
  uint32_t reg32;

  credo_group8_0xc7_fw_status_msb_get(
      dev_id, dev_port, ln, &reg32, r_800c7, true);
  credo_group8_0xc8_firmware_8_get(dev_id, dev_port, ln, &reg32, r_800c8, true);
  credo_group8_0xc9_firmware_9_get(dev_id, dev_port, ln, &reg32, r_800c9, true);
  credo_group8_0xca_firmware_10_get(
      dev_id, dev_port, ln, &reg32, r_800ca, true);
  credo_group8_0xcb_firmware_11_get(
      dev_id, dev_port, ln, &reg32, r_800cb, true);

  return BF_SUCCESS;
}

/** \brief Un-configure a serdes lane

 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      : logical lane in port (0-7)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t port_mgr_tof2_serdes_disable_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln) {
  // un-configure any existing lane settings
  // This will also squelch Tx output
  port_mgr_tof2_serdes_un_config_ln(dev_id, dev_port, ln);
  return BF_SUCCESS;
}

/*
typedef struct pll_info_t {
  float data_rate;
  float fvco;
  uint32_t pll_cap;
  float pll_n_float;
  uint32_t div4_en;
  uint32_t div2_bypass;
  float ref_clk;
  uint32_t pll_frac_en;
  uint32_t pll_frac_n;
} pll_info_t;
def get_lane_pll(lane_obj):
    ref_clk = 156.25
    tx_div4_en = lane_obj.TX_REFCLK_DIV4_EN
    tx_div2_bypass = lane_obj.TX_PLL_BYPASS_DIV2
    tx_pll_n = lane_obj.TX_PLL_N
    tx_pll_cap = lane_obj.TX_PLL_VCO_RANGE
    tx_10g_mode_en = lane_obj.TX_HALF_RATE_EN
    #tx_pll_frac_n = lane_obj.Reg00D9_3_0S

    tx_pll_frac_n = lane_obj.TX_PLL_N_FRAC
    tx_pll_frac_order = lane_obj.TX_PLL_N_FRAC_CTRL
    tx_pll_frac_en = lane_obj.TX_PLL_N_FRAC_EN

    rx_div4_en = lane_obj.RX_REFCLK_DIV4_EN
    rx_div2_bypass = lane_obj.RX_PLL_BYPASS_DIV2
    rx_pll_n = lane_obj.RX_PLL_N
    rx_pll_cap = lane_obj.RX_PLL_VCO_RANGE

    rx_pll_frac_n = lane_obj.RX_PLL_N_FRAC
    rx_pll_frac_order = lane_obj.RX_PLL_N_FRAC_CTRL
    rx_pll_frac_en = lane_obj.RX_PLL_N_FRAC_EN

    rx_ref_div2_en = lane_obj.RX_REFCLK_DIV2_EN
    tx_ref_div2_en = lane_obj.en_refclk_div2_tx

    tx_div_by_4 = 1.0 if tx_div4_en == 0 else 4.0
    rx_div_by_4 = 1.0 if rx_div4_en == 0 else 4.0
    tx_ref_by_2 = 1.0 if tx_ref_div2_en == 0 else 2.0
    rx_ref_by_2 = 1.0 if rx_ref_div2_en == 0 else 2.0
    tx_mul_by_2 = 1.0 if tx_div2_bypass == 1 else 2.0
    rx_mul_by_2 = 1.0 if rx_div2_bypass == 1 else 2.0

    pam4_mode_en = 1 if (lane_obj.TX_NRZ_MODE == 0 and lane_obj.PAM4_EN == 1)
else 0
    if pam4_mode_en:
        data_rate_to_fvco_ratio = 2.0
    else:
        if tx_10g_mode_en == 0:
            #print "enter NRZ25 mode"
            data_rate_to_fvco_ratio = 1.0
        else:
            data_rate_to_fvco_ratio = 0.5
    tx_pll_n_float = float(tx_pll_n) + float(tx_pll_frac_n / 1048575.0) if
tx_pll_frac_en else float(tx_pll_n)

    rx_pll_n_float = float(rx_pll_n) + float(rx_pll_frac_n / 65535.0) if
rx_pll_frac_en else float(rx_pll_n)
    tx_fvco = (ref_clk * tx_pll_n_float * 2.0 * tx_mul_by_2) / tx_div_by_4 /
tx_ref_by_2 / 1000.0
    rx_fvco = (ref_clk * rx_pll_n_float * 2.0 * rx_mul_by_2) / rx_div_by_4 /
rx_ref_by_2 / 1000.0

    tx_half_rate = lane_obj.HALF_RATE_SPEED_MODE
    rx_sub_rate = lane_obj.RX_SUB_RATE_MODE

    tx_data_rate = tx_fvco * data_rate_to_fvco_ratio/(2**tx_half_rate)
    rx_data_rate = rx_fvco * data_rate_to_fvco_ratio/(2**rx_sub_rate)

    tx_pll_params = tx_data_rate, tx_fvco, tx_pll_cap, tx_pll_n_float,
tx_div4_en, tx_div2_bypass, ref_clk, tx_pll_frac_en, tx_pll_frac_n
    rx_pll_params = rx_data_rate, rx_fvco, rx_pll_cap, rx_pll_n_float,
rx_div4_en, rx_div2_bypass, ref_clk, rx_pll_frac_en, rx_pll_frac_n
*/

bf_status_t port_mgr_tof2_serdes_pll_info_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              pll_info_t *tx_pll_info,
                                              pll_info_t *rx_pll_info) {
  uint32_t reg32, fld_val;
  float ref_clk = 156.25;
  uint32_t tx_pll_n_int, rx_pll_n_int;
  uint32_t tx_10g_mode_en, lsb, tx_nrz_mode, pam4_mode_en;
  uint32_t rx_ref_div2_en, tx_ref_div2_en;
  float data_rate_to_fvco_ratio;
  float tx_div_by_4, rx_div_by_4, tx_ref_by_2, rx_ref_by_2, tx_mul_by_2,
      rx_mul_by_2;
  uint32_t tx_half_rate, rx_sub_rate;

  tx_pll_info->ref_clk = ref_clk;
  rx_pll_info->ref_clk = ref_clk;

  credo_rsvd_0x3f_tx_refclk_div4_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_pll_info->div4_en = fld_val;
  credo_rsvd_0x3f_tx_pll_bypass_div2_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_pll_info->div2_bypass = fld_val;
  credo_rsvd_0x3e_tx_pll_n_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_pll_n_int = fld_val;
  credo_rsvd_0x1b_tx_pll_vco_range_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_pll_info->pll_cap = fld_val;
  credo_tx_0xb0_tx_half_rate_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_10g_mode_en = fld_val;
  credo_rsvd_0x18_tx_pll_n_frac_lsb_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  lsb = fld_val;
  credo_rsvd_0x19_tx_pll_n_frac_msb_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_pll_info->pll_frac_n = (fld_val << 16) | lsb;
  // credo_rsvd_0x17_tx_pll_n_frac_ctrl_get
  //
  credo_rsvd_0x17_tx_pll_n_frac_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_pll_info->pll_frac_en = fld_val;

  credo_rx_0xff_rx_refclk_div4_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_pll_info->div4_en = fld_val;
  credo_rx_0xf5_rx_pll_bypass_div2_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_pll_info->div2_bypass = fld_val;
  credo_rx_0xfd_rx_pll_n_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_pll_n_int = fld_val;
  credo_rx_0xf5_rx_pll_vco_range_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_pll_info->pll_cap = fld_val;
  credo_rx_0xf1_rx_pll_n_frac_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_pll_info->pll_frac_n = fld_val;
  // credo_rx_0xf0_rx_pll_n_frac_ctrl_get
  credo_rx_0xf0_rx_pll_n_frac_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_pll_info->pll_frac_en = fld_val;

  credo_rx_0xf4_rx_refclk_div2_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_ref_div2_en = fld_val;
  credo_rsvd_0x1a_tx_refclk_div2_en_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_ref_div2_en = fld_val;

  tx_div_by_4 = tx_pll_info->div4_en == 0 ? 1.0 : 4.0;
  rx_div_by_4 = rx_pll_info->div4_en == 0 ? 1.0 : 4.0;
  tx_ref_by_2 = tx_ref_div2_en == 0 ? 1.0 : 2.0;
  rx_ref_by_2 = rx_ref_div2_en == 0 ? 1.0 : 2.0;
  tx_mul_by_2 = tx_pll_info->div2_bypass == 1 ? 1.0 : 2.0;
  rx_mul_by_2 = rx_pll_info->div2_bypass == 1 ? 1.0 : 2.0;

  credo_tx_0xb0_tx_nrz_mode_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_nrz_mode = fld_val;
  credo_tx_0x41_pam4_en_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  pam4_mode_en = (tx_nrz_mode == 0) && (fld_val == 1) ? 1 : 0;
  if (pam4_mode_en) {
    data_rate_to_fvco_ratio = 2.0;
  } else {
    if (tx_10g_mode_en == 0) {
      data_rate_to_fvco_ratio = 1.0;
    } else {
      data_rate_to_fvco_ratio = 0.5;
    }
  }
  if (tx_pll_info->pll_frac_en) {
    tx_pll_info->pll_n_float =
        (float)tx_pll_n_int + ((float)tx_pll_info->pll_frac_n / 1048575.0);
  } else {
    tx_pll_info->pll_n_float = (float)tx_pll_n_int;
  }
  if (rx_pll_info->pll_frac_en) {
    rx_pll_info->pll_n_float =
        (float)rx_pll_n_int + ((float)rx_pll_info->pll_frac_n / 65535.0);
  } else {
    rx_pll_info->pll_n_float = (float)rx_pll_n_int;
  }
  /*
    tx_fvco = (ref_clk * tx_pll_n_float * 2.0 * tx_mul_by_2) / tx_div_by_4 /
    tx_ref_by_2 / 1000.0
    rx_fvco = (ref_clk * rx_pll_n_float * 2.0 * rx_mul_by_2) / rx_div_by_4 /
    rx_ref_by_2 / 1000.0

    tx_half_rate = lane_obj.HALF_RATE_SPEED_MODE
    rx_sub_rate = lane_obj.RX_SUB_RATE_MODE

    tx_data_rate = tx_fvco * data_rate_to_fvco_ratio/(2**tx_half_rate)
    rx_data_rate = rx_fvco * data_rate_to_fvco_ratio/(2**rx_sub_rate)
  */
  tx_pll_info->fvco = (ref_clk * tx_pll_info->pll_n_float * 2.0 * tx_mul_by_2) /
                      tx_div_by_4 / tx_ref_by_2 / 1000.0;
  rx_pll_info->fvco = (ref_clk * rx_pll_info->pll_n_float * 2.0 * rx_mul_by_2) /
                      rx_div_by_4 / rx_ref_by_2 / 1000.0;

  credo_tx_0xa0_speed_mode_get(dev_id, dev_port, ln, &reg32, &fld_val, true);
  tx_half_rate = fld_val;
  tx_pll_info->data_rate = (float)(tx_pll_info->fvco * data_rate_to_fvco_ratio /
                                   (1 << tx_half_rate));

  credo_rx_0x79_rx_sub_rate_mode_get(
      dev_id, dev_port, ln, &reg32, &fld_val, true);
  rx_sub_rate = fld_val;
  rx_pll_info->data_rate =
      (float)(rx_pll_info->fvco * data_rate_to_fvco_ratio / (1 << rx_sub_rate));

  return BF_SUCCESS;
}

/*
fw_eyemon_start_cmd = 0x1000
fw_eyemon_prog_cmd = 0x2000
fw_eyemon_read_cmd = 0x3000

def eye_mon_collect_with_plot(lane=0, depth=7, eye_mon_data=None):
    eye_mon_data_start_addr = 0x5000
    eye_mon_phase_range = 16
    fun_en = 1

    if eye_mon_data is None:
        eye_mon_data=numpy.zeros((127, eye_mon_phase_range*2+1))

    start_time=time.time()

    for pindex in range(0,eye_mon_phase_range*2+1):
        eye_mon_collection_start_word = fw_eyemon_start_cmd + (depth<<4)  + lane
        cnt=0
        eye_mon_curr_progress=0
        result=fw_cmd(eye_mon_collection_start_word, detail=pindex,
expected_response=None)
        pindex_output = (pindex<=eye_mon_phase_range) and
(pindex+eye_mon_phase_range) or (pindex-eye_mon_phase_range-1);

        while ((eye_mon_curr_progress & 0x00ff) < 100):
            result = fw_cmd(fw_eyemon_prog_cmd, expected_response=None)
            eye_mon_curr_progress = result & 0xff
            print("\r....Eye Mon Collection In Progress...%3d%% Phase: %d" %
(eye_mon_curr_progress,pindex)),

        for margin in range(64, -63, -16):
            chip.GROUP8_TOP[0].FW_CMD_DETAIL = margin & 0xffff
            chip.GROUP8_TOP[0].FW_CMD = (pindex&0xFF) | 0x3000
            while ((chip.GROUP8_TOP[0].FW_CMD)>>12)!=0:
                pass

            status=chip.GROUP8_TOP[0].FW_CMD
            if ((status>>8)&0xF) != 2:
                for i in range(16):
                    m=margin+i
                    if m<64:
                        eye_mon_data[63+m, pindex_output] = 0
            else:
                for i in range(16):
                    m=margin+i
                    if m<64:
                        eye_mon_data[63+m, pindex_output] =
chip.MdioRd(eye_mon_data_start_addr+i)

    stop_time=time.time()
    test_time = stop_time - start_time

    print ("Eye Mon Time: %2.3f sec"%(stop_time-start_time))
    eye_mon_data_text_file = open("eye_mon_data_txt.txt", "w")
    eye_mon_data_bin_file = open("eye_mon_data_bin.bin", "wb")

    for margin in range(0,127):
        eye_mon_data_text_file.write("\n")
        eye_mon_data_text_file.write("%4d"%(450-margin*8))
        for phase in range(eye_mon_phase_range*2+1):
            if eye_mon_data[margin, phase] == 255: # center of eye = 255
                eye_mon_data_text_file.write("    "%eye_mon_data[margin, phase])
            elif eye_mon_data[margin, phase] > 100: # center of eye = 255
                eye_mon_data_text_file.write("...."%eye_mon_data[margin, phase])
            elif eye_mon_data[margin, phase] > 70: # center of eye = 255
                eye_mon_data_text_file.write("oooo"%eye_mon_data[margin, phase])
            else:
                eye_mon_data_text_file.write("----"%eye_mon_data[margin, phase])

            bin_value = struct.pack("H", int(eye_mon_data[margin, phase]))
            eye_mon_data_bin_file.write(bin_value)

    eye_mon_data_text_file.close()
    eye_mon_data_bin_file.close()
    print("Done!")
*/

#define eye_mon_phase_range 16
#define phase_range (eye_mon_phase_range * 2 + 1)
uint8_t eye_mon_data[127 + 1][phase_range];
uint8_t eye_mon_plot_string[127 * phase_range * 5] = {0};

bf_status_t port_mgr_tof2_serdes_eye_plot_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              uint8_t **plot_data) {
  uint32_t reg32 = 0;
  uint32_t fld_val;
  uint32_t fw_eyemon_start_cmd = 0x1000;
  uint32_t fw_eyemon_prog_cmd = 0x2000;
  uint32_t fw_eyemon_read_cmd = 0x3000;
  uint32_t pindex, pindex_output, rsp;
  uint32_t rc, depth = 7, eye_mon_curr_progress;
  uint32_t base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);
  uint32_t ln_to_plot = base_ln + ln;
  int32_t m, margin;
  uint32_t last_chg = 0, last_prog = 0;

  memset(eye_mon_plot_string, 0, sizeof(eye_mon_plot_string));
  *plot_data = eye_mon_plot_string;

  for (pindex = 0; pindex < phase_range; pindex++) {
    rc = port_mgr_tof2_serdes_fw_cmd_w_rtn_data(
        dev_id,
        dev_port,
        ln,
        fw_eyemon_start_cmd + (depth << 4) + ln_to_plot,
        &rsp,
        pindex);
    port_mgr_log("PLOT: Start cmd: %04x : detail %04x : rsp %04x : rc %d",
                 fw_eyemon_start_cmd + (depth << 4) + base_ln,
                 pindex,
                 rsp,
                 rc);

    if (rc == BF_SUCCESS) {
      eye_mon_curr_progress = 0;
      pindex_output = (pindex <= eye_mon_phase_range)
                          ? (pindex + eye_mon_phase_range)
                          : (pindex - eye_mon_phase_range - 1);

      while ((eye_mon_curr_progress & 0x00ff) < 100) {
        bf_sys_usleep(10000);
        rc = port_mgr_tof2_serdes_fw_cmd_w_rtn_data(
            dev_id, dev_port, ln, fw_eyemon_prog_cmd, &rsp, pindex);
        if (rc != BF_SUCCESS) {
          // bf_sys_assert(0);
        }
        eye_mon_curr_progress = rsp & 0xff;
        if (eye_mon_curr_progress == last_prog) {
          last_chg++;
        } else {
          // port_mgr_log("PLOT: Prog cmd: %04x : detail %04x : rsp %04x : rc %d
          // : since chg %d",
          //         fw_eyemon_prog_cmd, pindex, rsp, rc, last_chg);
          last_chg = 0;
          last_prog = eye_mon_curr_progress;
        }
        if (last_chg > 500) {
          last_chg = 0;
          port_mgr_log("PLOT: Prog cmd: *** Timeout *** <rsp=%04x>", rsp);
          break;
        }
      }
      bf_sys_usleep(10000);
      for (margin = -63; margin < 64; margin += 16) {
        port_mgr_tof2_serdes_fw_cmd_lock();

        credo_group8_0xc2_fw_cmd_detail_set(
            dev_id, dev_port, ln, &reg32, margin & 0xffff, true);
        credo_group8_0xc1_fw_cmd_set(dev_id,
                                     dev_port,
                                     ln,
                                     &reg32,
                                     (pindex & 0xff) | fw_eyemon_read_cmd,
                                     true);
        // port_mgr_log("PLOT: Read cmd: %04x : detail %04x", (pindex & 0xff) |
        // fw_eyemon_read_cmd, margin & 0xffff);
        bf_sys_usleep(10000);

        bool ready = false;
        while (!ready) {
          credo_group8_0xc1_fw_cmd_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          ready = (fld_val >> 12) == 0;
        }
        credo_group8_0xc1_fw_cmd_get(
            dev_id, dev_port, ln, &reg32, &fld_val, true);
        if (((fld_val >> 8) & 0xf) == 3) {
          port_mgr_log("PLOT: Read cmd rsp: %04x", fld_val);
          if (margin == 64) {             // ignore
          } else if (fld_val == 0x307) {  // EM seems to have stopped?
            char *error_msg = "\n*** EM stopped unexpectedly ***\n";
            snprintf((char *)&eye_mon_plot_string[0],
                     strlen(error_msg) + 1,
                     "%s",
                     error_msg);
          } else {
            char *error_msg = "\n*** EM still in-progress ***\n";
            snprintf((char *)&eye_mon_plot_string[0],
                     strlen(error_msg) + 1,
                     "%s",
                     error_msg);
            bf_sys_usleep(10000);
          }
        }
        port_mgr_tof2_serdes_fw_cmd_unlock();

        if (((fld_val >> 8) & 0xf) != 2) {
          for (int i = 0; i < 16; i++) {
            m = margin + i;
            if (m < 64) {
              eye_mon_data[63 + m][pindex_output] = 0;
            }
          }
        } else {
          m = margin;
          if (m >= 64) {
            continue;
          }
          credo_fw_0x0_firmware_data_0_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 0][pindex_output] = fld_val;
          credo_fw_0x1_firmware_data_1_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 1][pindex_output] = fld_val;
          credo_fw_0x2_firmware_data_2_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 2][pindex_output] = fld_val;
          credo_fw_0x3_firmware_data_3_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 3][pindex_output] = fld_val;
          credo_fw_0x4_firmware_data_4_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 4][pindex_output] = fld_val;
          credo_fw_0x5_firmware_data_5_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 5][pindex_output] = fld_val;
          credo_fw_0x6_firmware_data_6_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 6][pindex_output] = fld_val;
          credo_fw_0x7_firmware_data_7_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 7][pindex_output] = fld_val;
          credo_fw_0x8_firmware_data_8_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 8][pindex_output] = fld_val;
          credo_fw_0x9_firmware_data_9_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 9][pindex_output] = fld_val;
          credo_fw_0xa_firmware_data_a_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 10][pindex_output] = fld_val;
          credo_fw_0xb_firmware_data_b_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 11][pindex_output] = fld_val;
          credo_fw_0xc_firmware_data_c_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 12][pindex_output] = fld_val;
          credo_fw_0xd_firmware_data_d_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 13][pindex_output] = fld_val;
          credo_fw_0xe_firmware_data_e_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 14][pindex_output] = fld_val;
          credo_fw_0xf_firmware_data_f_get(
              dev_id, dev_port, ln, &reg32, &fld_val, true);
          eye_mon_data[63 + m + 15][pindex_output] = fld_val;
        }
      }
    }
  }
  uint16_t len = 0;
  for (margin = 0; margin < 127; margin++) {
    len = strlen((char *)eye_mon_plot_string);
    snprintf((char *)&eye_mon_plot_string[len],
             sizeof(eye_mon_plot_string) - len,
             "\n%4d",
             450 - (margin * 8));

    for (int phase = 0; phase < phase_range; phase++) {
      char *chr;
      if (eye_mon_data[margin][phase] == 255) {
        chr = "    ";
      } else if (eye_mon_data[margin][phase] > 200) {
        chr = ". . ";
      } else if (eye_mon_data[margin][phase] > 150) {
        chr = "....";
      } else if (eye_mon_data[margin][phase] > 100) {
        chr = ":.:.";
      } else if (eye_mon_data[margin][phase] > 70) {
        chr = "::::";
      } else if (eye_mon_data[margin][phase] > 50) {
        chr = "iiii";
      } else if (eye_mon_data[margin][phase] > 35) {
        chr = "xxxx";
      } else {
        chr = "----";
      }

      len = strlen((char *)eye_mon_plot_string);
      snprintf((char *)&eye_mon_plot_string[len],
               sizeof(eye_mon_plot_string) - len,
               "%4s",
               chr);
    }
  }
  return BF_SUCCESS;
}

/** \brief port_mgr_tof2_serdes_tile_efuse_get

 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: used to determine tile
 * \param bank    : 0-2
 *
 * \return: 32-bit value read from tile efuse bank
 */
uint32_t port_mgr_tof2_serdes_tile_efuse_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             int bank) {
  uint32_t reg32;
  uint32_t data_h, data_l;

  credo_sensor_0xf9_magic_num_rmw(dev_id, dev_port, 0, &reg32, 0xAAAA);
  reg32 = 0;
  credo_sensor_0xff_rsvd_0903ff_15_01_set(dev_id, dev_port, 0, &reg32, 0, true);
  reg32 = 0;
  credo_sensor_0xfe_rsvd_0903fe_15_01_set(dev_id, dev_port, 0, &reg32, 0, true);
  reg32 = 0x103;
  credo_sensor_0xfd_load_set(dev_id, dev_port, 0, &reg32, 1, true);
  reg32 = bank;
  credo_sensor_0xfb_addr_pins_set(dev_id, dev_port, 0, &reg32, bank, true);
  reg32 = 1;
  credo_sensor_0xfc_rsvd_0903fc_15_01_set(dev_id, dev_port, 0, &reg32, 0, true);

  bf_sys_usleep(100000);

  reg32 = 0;
  credo_sensor_0xfc_rsvd_0903fc_15_01_set(dev_id, dev_port, 0, &reg32, 0, true);

  bf_sys_usleep(100000);

  credo_sensor_0xfa_data_out_high_get(
      dev_id, dev_port, 0, &reg32, &data_h, true);
  credo_sensor_0xf7_data_out_low_get(
      dev_id, dev_port, 0, &reg32, &data_l, true);

  reg32 = 0;
  credo_sensor_0xfe_rsvd_0903fe_15_01_set(dev_id, dev_port, 0, &reg32, 0, true);
  reg32 = 1;
  credo_sensor_0xff_rsvd_0903ff_15_01_set(dev_id, dev_port, 0, &reg32, 0, true);
  return ((data_h << 16) | data_l);
}

uint32_t port_mgr_tof2_serdes_a0_upper_bits_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln) {
  uint32_t reg32, f1, f2, f3;

  credo_tx_0xa0_tx_test_data_src_get(dev_id, dev_port, ln, &reg32, &f1, true);
  credo_tx_0xa0_tx_prbs_clk_en_get(dev_id, dev_port, ln, &reg32, &f2, false);
  credo_tx_0xa0_tx_pam4_test_en_get(dev_id, dev_port, ln, &reg32, &f3, false);
  return (((f1 << 2) | (f2 << 1) | f3) << 13);
}

void port_mgr_tof2_serdes_a0_upper_bits_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t x3_bits) {
  uint32_t reg32, f1;

  credo_tx_0xa0_tx_test_data_src_get(dev_id, dev_port, ln, &reg32, &f1, true);
  if (x3_bits & 4) {
    credo_tx_0xa0_tx_test_data_src_set(dev_id, dev_port, ln, &reg32, 1, false);
  } else {
    credo_tx_0xa0_tx_test_data_src_set(dev_id, dev_port, ln, &reg32, 0, false);
  }

  if (x3_bits & 2) {
    credo_tx_0xa0_tx_prbs_clk_en_set(dev_id, dev_port, ln, &reg32, 1, false);
  } else {
    credo_tx_0xa0_tx_prbs_clk_en_set(dev_id, dev_port, ln, &reg32, 0, false);
  }

  if (x3_bits & 1) {
    credo_tx_0xa0_tx_pam4_test_en_set(dev_id, dev_port, ln, &reg32, 1, true);
  } else {
    credo_tx_0xa0_tx_pam4_test_en_set(dev_id, dev_port, ln, &reg32, 0, true);
  }
  return;
}

/** \brief port_mgr_tof2_serdes_known_value_get

 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: used to determine tile
 *
 * \return: BF_SUUCESS    : tile returned expected value
 * \return: BF_INVALID_ARG: tile did not return expected value
 */
bf_status_t port_mgr_tof2_serdes_known_value_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port) {
  uint32_t reg32 = 0, fld_val = 0;

  credo_sensor_0xf1_delay_val_get(dev_id, dev_port, 0, &reg32, &fld_val, true);
  if (fld_val == 0xBAA) {
    return BF_SUCCESS;
  } else {
    port_mgr_log("%d:%2d : TILE READ ERROR: Expected 0x0BAA : got %04x",
                 dev_id,
                 dev_port,
                 fld_val);
    return BF_INVALID_ARG;
  }
}

bf_status_t port_mgr_tof2_serdes_delta_compute(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    bf_ha_port_reconcile_info_t *recon_info) {
  port_mgr_log("%s:%d Computing serdes delta for dev %d port %d",
               __func__,
               __LINE__,
               dev_id,
               dev_port);
  uint32_t phy_mac_block = 0, ch, umac, ln = 0;
  bool is_cpu_port;
  port_mgr_tof2_pdev_t *dev_p = port_mgr_dev_physical_dev_tof2_get(dev_id);

  port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &umac, &ch, &is_cpu_port);

  if (dev_p == NULL) {
    port_mgr_log(
        "%d:%2d : port_mgr_dev_physical_dev_tof2_get returned dev_p Null ",
        dev_id,
        dev_port);
    return BF_INVALID_ARG;
  }

  uint32_t num_lanes = port_mgr_tof2_get_num_lanes(dev_id, dev_port);
  if (num_lanes == 0) return BF_INVALID_ARG;

  if (!is_cpu_port) {
    port_mgr_umac4_t *umac4_p = &dev_p->umac4[umac - 1];
    for (ln = 0; ln < num_lanes; ln++) {
      if ((umac4_p->phys_tx_ln[ch + ln] != umac4_p->hw_phys_tx_ln[ch + ln]) ||
          (umac4_p->phys_rx_ln[ch + ln] != umac4_p->hw_phys_rx_ln[ch + ln])) {
        /* Indicates that there is a mismatch in the already programmed lane
           mapping and the one being replayed */
        recon_info->ca = BF_HA_CA_PORT_NONE;  // Needs cold boot.
        port_mgr_log(
            "%s:%d:%d:%d:%d Serdes phys_tx_ln change detected: %d (SW) : %d "
            "IMPORTANT: Needs cold boot. No action taken to avoid traffic "
            "disruptions "
            "(HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            phy_mac_block,
            umac4_p->phys_tx_ln[ch + ln],
            umac4_p->hw_phys_tx_ln[ch + ln]);
        port_mgr_log(
            "%s:%d:%d:%d:%d Serdes phys_rx_ln change detected: %d (SW) : %d "
            "IMPORTANT: Needs cold boot. No action taken to avoid traffic "
            "disruptions "
            "(HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            phy_mac_block,
            umac4_p->phys_rx_ln[ch + ln],
            umac4_p->hw_phys_rx_ln[ch + ln]);
        return BF_SUCCESS;
      }
    }
  } else {
    port_mgr_umac3_t *umac3_p = &dev_p->umac3[0];
    for (ln = 0; ln < num_lanes; ln++) {
      if ((umac3_p->phys_tx_ln[ch + ln] != umac3_p->hw_phys_tx_ln[ch + ln]) ||
          (umac3_p->phys_rx_ln[ch + ln] != umac3_p->hw_phys_rx_ln[ch + ln])) {
        /* Indicates that there is a mismatch in the already programmed lane
           mapping and the one being replayed */
        recon_info->ca = BF_HA_CA_PORT_NONE;  // Needs cold boot.
        port_mgr_log(
            "%s:%d:%d:%d:%d Serdes phys_tx_ln change detected: %d (SW) : %d "
            "IMPORTANT: Needs cold boot. No action taken to avoid traffic "
            "disruptions "
            "(HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            phy_mac_block,
            umac3_p->phys_tx_ln[ch + ln],
            umac3_p->hw_phys_tx_ln[ch + ln]);
        port_mgr_log(
            "%s:%d:%d:%d:%d Serdes phys_rx_ln change detected: %d (SW) : %d "
            "IMPORTANT: Needs cold boot. No action taken to avoid traffic "
            "disruptions "
            "(HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            phy_mac_block,
            umac3_p->phys_rx_ln[ch + ln],
            umac3_p->hw_phys_rx_ln[ch + ln]);
        return BF_SUCCESS;
      }
    }
  }

  // Get the serdes params in the hardware
  for (ln = 0; ln < num_lanes; ln++) {
    /* Compare all the params and if any of them differ mark the corrective
       action as FLAP*/
    if (!is_cpu_port) {
      if ((dev_p->umac4[umac - 1].hw_sd[ch + ln].rx_inv !=
           dev_p->umac4[umac - 1].sd[ch + ln].rx_inv) ||
          (dev_p->umac4[umac - 1].hw_sd[ch + ln].tx_inv !=
           dev_p->umac4[umac - 1].sd[ch + ln].tx_inv)) {
        recon_info->ca = BF_HA_CA_PORT_FLAP;
        port_mgr_log(
            "%s:%d:%d:%d: Serdes tx_inv change detected: %d (SW) : %d (HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            dev_p->umac4[umac - 1].sd[ch + ln].tx_inv,
            dev_p->umac4[umac - 1].hw_sd[ch + ln].tx_inv);
        port_mgr_log(
            "%s:%d:%d:%d: Serdes rx_inv change detected: %d (SW) : %d (HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            dev_p->umac4[umac - 1].sd[ch + ln].rx_inv,
            dev_p->umac4[umac - 1].hw_sd[ch + ln].rx_inv);
        return BF_SUCCESS;
      }
    } else {
      umac = 0;
      if ((dev_p->umac3[umac].hw_sd[ch + ln].rx_inv !=
           dev_p->umac3[umac].sd[ch + ln].rx_inv) ||
          (dev_p->umac3[umac].hw_sd[ch + ln].tx_inv !=
           dev_p->umac3[umac].sd[ch + ln].tx_inv)) {
        recon_info->ca = BF_HA_CA_PORT_FLAP;
        port_mgr_log(
            "%s:%d:%d:%d: Serdes tx_inv change detected: %d (SW) : %d (HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            dev_p->umac3[umac].sd[ch + ln].tx_inv,
            dev_p->umac3[umac].hw_sd[ch + ln].tx_inv);
        port_mgr_log(
            "%s:%d:%d:%d: Serdes rx_inv change detected: %d (SW) : %d (HW) ",
            __func__,
            __LINE__,
            dev_id,
            dev_port,
            dev_p->umac3[umac].sd[ch + ln].rx_inv,
            dev_p->umac3[umac].hw_sd[ch + ln].rx_inv);
        return BF_SUCCESS;
      }
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
