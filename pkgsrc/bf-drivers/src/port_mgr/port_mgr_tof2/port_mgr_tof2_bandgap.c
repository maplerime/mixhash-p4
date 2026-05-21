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

#include <math.h>
#include <byteswap.h>
#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_reg_if.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof2_map.h"
#include <port_mgr/port_mgr_intf.h>
#include "port_mgr_tof2_port.h"

#include <sys/stat.h>
#include <unistd.h>

#include <target-utils/third-party/cJSON/cJSON.h>
#include <ctx_json/ctx_json_utils.h>

typedef struct bg_cal_t {
  uint32_t tx_cal;
  uint32_t rx_cal;
} bg_cal_t;

// indexed by tile/grp/ln
bg_cal_t calibrated_bandgap_setting[4][9][8] = {{{{0}}}};

bf_status_t port_mgr_tof2_bg_cal_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     uint32_t ln,
                                     uint32_t *rx_bg,
                                     uint32_t *tx_bg) {
  uint32_t tile;
  uint32_t grp;
  uint32_t mac_stn_id;
  uint32_t base_ln;
  uint32_t rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_stn_id, NULL, NULL);
  bf_sys_assert(rc == 0);

  base_ln = port_mgr_tof2_serdes_base_chnl_get(dev_id, dev_port);
  rc =
      lld_sku_map_mac_stn_id_to_tile_and_group(dev_id, mac_stn_id, &tile, &grp);
  if (rc) {
    return BF_INVALID_ARG;
  }
  *rx_bg = calibrated_bandgap_setting[tile][grp][base_ln + ln].rx_cal;
  *tx_bg = calibrated_bandgap_setting[tile][grp][base_ln + ln].tx_cal;
  return BF_SUCCESS;
}

/* port_mgr_tof2_vsensor_all_disable

    chip.LANE[lane].TX_PLL_EST_EN = tp_list[0][2]
    chip.LANE[lane].TX_PLL_VTSTGROUP = tp_list[0][3]
    chip.LANE[lane].TX_PLL_TESTMODE = tp_list[0][4]
    chip.LANE[lane].RX_PLL_TEST_EN = tp_list[1][2]
    chip.LANE[lane].RX_PLL_VTSTGROUP = tp_list[1][3]
    chip.LANE[lane].RX_PLL_TESTMODE = tp_list[1][4]
    chip.LANE[lane].TX_TEST_GROUP_EN = tp_list[2][2]
    chip.LANE[lane].TX_VTSTGROUP = tp_list[2][3]
    chip.LANE[lane].TX_TESTMODE = tp_list[2][4]
    chip.LANE[lane].RX_TEST_GROUP_EN = tp_list[3][2]
    chip.LANE[lane].RX_VTSTGROUP = tp_list[3][3]
    chip.LANE[lane].RX_TESTMODE = tp_list[3][4]
*/
static void port_mgr_tof2_vsensor_disable(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln) {
  uint32_t reg32;

  credo_rsvd_0x3a_tx_pll_est_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x3a_tx_pll_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x3a_tx_pll_testmode_rmw(dev_id, dev_port, ln, &reg32, 0);

  credo_rx_0xf7_rx_pll_test_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rx_0xf7_rx_pll_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rx_0xf7_rx_pll_testmode_rmw(dev_id, dev_port, ln, &reg32, 0);

  credo_rsvd_0x29_tx_test_group_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x29_tx_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x29_tx_testmode_rmw(dev_id, dev_port, ln, &reg32, 0);

  credo_rsvd_0x2c_rx_test_group_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x2c_rx_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x2c_rx_testmode_rmw(dev_id, dev_port, ln, &reg32, 0);
}

static void port_mgr_tof2_vsensor_disable_all(bf_dev_id_t dev_id,
                                              uint32_t num_pipes) {
  uint32_t pipe_id, port_id;
  bf_dev_port_t dev_port;
  uint32_t ln, n_lanes;

  for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
    for (port_id = 8; port_id < 72; port_id += 8) {
      dev_port = MAKE_DEV_PORT(pipe_id, port_id);
      if ((port_id == 0) && (pipe_id > 0)) continue;
      if (port_id == 0) {
        dev_port = 2;  // make it the CPU Port
        n_lanes = 4;
      } else {
        n_lanes = 8;
      }

      for (ln = 0; ln < n_lanes; ln++) {
        port_mgr_tof2_vsensor_disable(dev_id, dev_port, ln);
      }
    }
  }
}

static void port_mgr_tof2_vsensor_tx_pll_disable(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln) {
  uint32_t reg32;

  credo_rsvd_0x3a_tx_pll_est_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x3a_tx_pll_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x3a_tx_pll_testmode_rmw(
      dev_id, dev_port, ln, &reg32, 0);  // PGATE
}

static void port_mgr_tof2_vsensor_rx_pll_disable(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln) {
  uint32_t reg32;

  credo_rx_0xf7_rx_pll_test_en_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rx_0xf7_rx_pll_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rx_0xf7_rx_pll_testmode_rmw(dev_id, dev_port, ln, &reg32, 0);  // PGATE
}

static void port_mgr_tof2_vsensor_tx_pll_enable(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln) {
  uint32_t reg32;

  credo_rsvd_0x3a_tx_pll_est_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  // credo_rsvd_0x3a_tx_pll_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rsvd_0x3a_tx_pll_testmode_rmw(
      dev_id, dev_port, ln, &reg32, 1);  // PGATE
}

static void port_mgr_tof2_vsensor_rx_pll_enable(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln) {
  uint32_t reg32;

  credo_rx_0xf7_rx_pll_test_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  // credo_rx_0xf7_rx_pll_vtstgroup_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_rx_0xf7_rx_pll_testmode_rmw(dev_id, dev_port, ln, &reg32, 1);  // PGATE
}

bf_status_t port_mgr_tof2_vsensor_bfn(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t ln,
                                      float *rtnd_mv) {
  uint32_t reg32, sensor_val;
  uint32_t mac_stn_id;
  port_mgr_err_t rc;
  uint32_t tile, grp;
  float mv;
  // int wait = 10000;
  lld_err_t lld_rc;

  rc = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_stn_id, NULL, NULL);
  if (rc != 0) {
    port_mgr_log("%d:%3d:%d : port_mgr_tof2_vsensor error: %d: from map to all",
                 dev_id,
                 dev_port,
                 ln,
                 rc);
    return rc;
  }
  lld_rc =
      lld_sku_map_mac_stn_id_to_tile_and_group(dev_id, mac_stn_id, &tile, &grp);
  if (lld_rc) {
    return BF_INVALID_ARG;
  }

  port_mgr_log(
      "%d:%3d:%d : port_mgr_tof2_vsensor : mac_stn_id=%d : tile=%d : grp=%d",
      dev_id,
      dev_port,
      ln,
      mac_stn_id,
      tile,
      grp);

  /*
     chip.TILE_TOP[0].PD_CAL_FCAL = 0
  */
  credo_rsvd3_0x0_pd_cal_fcal_rmw(dev_id, dev_port, ln, &reg32, 0);
  /*
    # Vsensor Auto Mode
    chip.TILE_TOP[0].TSENSOR_AUTO_EN = 1
    chip.TILE_TOP[0].VSENSOR_AUTO_EN = 1
    chip.TILE_TOP[0].VSENSOR1_AUTO_EN = 1
    chip.TILE_TOP[0].TSENSOR_RSTB = 1
    chip.TILE_TOP[0].VSENSOR_RSTB = 1
    chip.TILE_TOP[0].VSENSOR1_RSTB = 1
  */
  credo_sensor_0x3a_tsensor_auto_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor_auto_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor1_auto_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_tsensor_rstb_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor_rstb_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor1_rstb_rmw(dev_id, dev_port, ln, &reg32, 1);

  /* pre-set sel */
  credo_sensor_0xf6_vsensor_vin_sel_rmw(dev_id, dev_port, ln, &reg32, 8);
  credo_sensor_0xf3_vsensor1_vin_sel_rmw(dev_id, dev_port, ln, &reg32, 8);

  /* pre-set SDE=0 */
  credo_sensor_0xf3_vsensor1_sde_rmw(dev_id, dev_port, ln, &reg32, 0);

  // now power-dn both sensors
  credo_sensor_0xf6_pd_vsensor_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0xf3_pd_vsensor1_rmw(dev_id, dev_port, ln, &reg32, 1);

  /* pre-set rstn and run */
  credo_sensor_0xf6_vsensor_rstn_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_sensor_0xf6_vsensor_run_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_sensor_0xf3_vsensor1_rstn_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_sensor_0xf3_vsensor1_run_rmw(dev_id, dev_port, ln, &reg32, 0);
  /*
    chip.TILE_TOP[0].PD_VSENSOR1 = 0
    chip.TILE_TOP[0].VSENSOR1_VIN_SEL = 8
    time.sleep(wait)
  */
  credo_sensor_0xf3_pd_vsensor1_rmw(dev_id, dev_port, ln, &reg32, 0);
  bf_sys_usleep(10);

  /* de-assert rstn */
  credo_sensor_0xf3_vsensor1_rstn_rmw(dev_id, dev_port, ln, &reg32, 1);

  /* assert run */
  credo_sensor_0xf3_vsensor1_run_rmw(dev_id, dev_port, ln, &reg32, 1);

  int rdy = 0;
  int tout = 10000;
  while (!rdy && (tout-- > 0)) {
#if 1
    switch (grp) {
      case 0:
        credo_top_pll_0x50_sensor_0_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 1:
        credo_top_pll_0x51_sensor_1_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 2:
        credo_top_pll_0x52_sensor_2_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 3:
        credo_top_pll_0x53_sensor_3_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 4:
        credo_top_pll_0x54_sensor_4_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 5:
        credo_top_pll_0x55_sensor_5_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 6:
        credo_top_pll_0x56_sensor_6_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 7:
        credo_top_pll_0x57_sensor_7_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      case 8:
        credo_top_pll_0x58_sensor_8_get(
            dev_id, dev_port, ln, &reg32, &sensor_val, true);
        break;
      default:
        bf_sys_assert(0);
    }
#else
    uint32_t all_rdy = 0;
    uint32_t all_dones[9] = {0};
    {
      uint32_t s[9];
      int new_one = 0;
      credo_top_pll_0x50_sensor_0_get(
          dev_id, dev_port, ln, &reg32, &s[0], true);
      credo_top_pll_0x51_sensor_1_get(
          dev_id, dev_port, ln, &reg32, &s[1], true);
      credo_top_pll_0x52_sensor_2_get(
          dev_id, dev_port, ln, &reg32, &s[2], true);
      credo_top_pll_0x53_sensor_3_get(
          dev_id, dev_port, ln, &reg32, &s[3], true);
      credo_top_pll_0x54_sensor_4_get(
          dev_id, dev_port, ln, &reg32, &s[4], true);
      credo_top_pll_0x55_sensor_5_get(
          dev_id, dev_port, ln, &reg32, &s[5], true);
      credo_top_pll_0x56_sensor_6_get(
          dev_id, dev_port, ln, &reg32, &s[6], true);
      credo_top_pll_0x57_sensor_7_get(
          dev_id, dev_port, ln, &reg32, &s[7], true);
      credo_top_pll_0x58_sensor_8_get(
          dev_id, dev_port, ln, &reg32, &s[8], true);
      // port_mgr_log("%04x : %04x : %04x : %04x : %04x : %04x : %04x : %04x :
      // %04x",
      //             s[0], s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8]);
      sensor_val = s[grp];
      for (int g = 0; g < 8; g++) {
        all_rdy |= (((s[g] >> 12) & 1) << g);
        if (((s[g] >> 12) & 1)) {
          all_dones[g] = s[g];
          new_one = 1;
        }
      }
      if (new_one)
        port_mgr_log(
            "%04x : %04x : %04x : %04x : %04x : %04x : %04x : %04x : %04x",
            all_dones[0],
            all_dones[1],
            all_dones[2],
            all_dones[3],
            all_dones[4],
            all_dones[5],
            all_dones[6],
            all_dones[7],
            all_dones[8]);
    }
#endif
    rdy = (sensor_val >> 12) & 1;
  }
  if (!rdy) {
    return BF_INVALID_ARG;  // timeout
  }
  sensor_val = (sensor_val & 0x0fff);
  mv = ((sensor_val + 1.0) / 256.0) * 1.224;
  // port_mgr_log("auto,vh:%x,voltage sensor read data %f", sensor_val, mv);
  *rtnd_mv = mv * 1000;
  return BF_SUCCESS;
}

void port_mgr_tof2_bandgap_cal_org(bf_dev_id_t dev_id) {
  uint32_t pipe_id, port_id;
  bf_dev_port_t dev_port;
  uint32_t ln, n_lanes;
  uint32_t num_pipes = 0;
  lld_sku_get_num_active_pipes(dev_id, &num_pipes);

  port_mgr_log("port_mgr_tof2_bandgap_cal ..");

  port_mgr_tof2_vsensor_disable_all(dev_id, num_pipes);

  for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
    for (port_id = 8; port_id < 72; port_id += 8) {
      dev_port = MAKE_DEV_PORT(pipe_id, port_id);
      if ((port_id == 0) && (pipe_id > 0)) continue;
      if (port_id == 0) {
        dev_port = 2;  // make it the CPU Port
        n_lanes = 4;
      } else {
        n_lanes = 8;
      }

      for (ln = 0; ln < n_lanes; ln++) {
        bf_status_t rc;
        float mv;
        int bg_val;
        uint32_t mac_stn_id, ch;
        uint32_t reg32;

        // make sure its valid
        rc = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
        if (rc != BF_SUCCESS) continue;

        for (bg_val = 7; bg_val >= 0; bg_val--) {
          credo_rsvd_0x3f_tx_vbg_rmw(dev_id, dev_port, ln, &reg32, bg_val);

          port_mgr_tof2_vsensor_tx_pll_enable(dev_id, dev_port, ln);
          rc = port_mgr_tof2_vsensor_bfn(dev_id, dev_port, ln, &mv);
          port_mgr_tof2_vsensor_tx_pll_disable(dev_id, dev_port, ln);

          if (rc != BF_SUCCESS) {
            port_mgr_log("%d:%3d:%d : Tx bandgap_cal : failure : setting=%d",
                         dev_id,
                         dev_port,
                         ln,
                         bg_val);
          } else {
            port_mgr_log("%d:%3d:%d : Tx bandgap_cal : %fmv <%d>",
                         dev_id,
                         dev_port,
                         ln,
                         mv,
                         bg_val);
            if (mv < 755) break;
          }
        }
        if (bg_val < 0) bg_val = 0;
        port_mgr_log("%d:%3d:%d : Tx bandgap_cal : %fmv <%d> SETTING",
                     dev_id,
                     dev_port,
                     ln,
                     mv,
                     bg_val);

        for (bg_val = 7; bg_val >= 0; bg_val--) {
          credo_rx_0xff_rx_vbg_rmw(dev_id, dev_port, ln, &reg32, bg_val);

          port_mgr_tof2_vsensor_rx_pll_enable(dev_id, dev_port, ln);
          rc = port_mgr_tof2_vsensor_bfn(dev_id, dev_port, ln, &mv);
          port_mgr_tof2_vsensor_rx_pll_disable(dev_id, dev_port, ln);

          if (rc != BF_SUCCESS) {
            port_mgr_log("%d:%3d:%d : Rx bandgap_cal : failure : setting=%d",
                         dev_id,
                         dev_port,
                         ln,
                         bg_val);
          } else {
            port_mgr_log("%d:%3d:%d : Rx bandgap_cal : %fmv <%d>",
                         dev_id,
                         dev_port,
                         ln,
                         mv,
                         bg_val);
            if (mv < 755) break;
          }
        }
        if (bg_val < 0) bg_val = 0;
        port_mgr_log("%d:%3d:%d : Rx bandgap_cal : %fmv <%d> SETTING",
                     dev_id,
                     dev_port,
                     ln,
                     mv,
                     bg_val);
      }
    }
  }
}

bf_status_t port_mgr_tof2_vsensor_tile(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       float *rtnd_mv) {
  uint32_t reg32, sensor_val, s[8] = {0};
  float mv;
  int g;

  /*
     chip.TILE_TOP[0].PD_CAL_FCAL = 0
  */
  credo_rsvd3_0x0_pd_cal_fcal_rmw(dev_id, dev_port, ln, &reg32, 0);

  /*
    #chip.MdioWr((Vsensor_base_addr + 0x3f), 0x810d)
    chip.TILE_TOP[0].VSENSOR_CLK_SEL = 1
    chip.TILE_TOP[0].VSENSOR_CLK_CNTR = 0x10d
  */
  credo_sensor_0x3f_vsensor_clk_sel_rmw(dev_id, dev_port, ln, &reg32, 0x01);
  credo_sensor_0x3f_vsensor_clk_cntr_rmw(dev_id, dev_port, ln, &reg32, 0x10d);

  /*
    # Vsensor Auto Mode
    chip.TILE_TOP[0].TSENSOR_AUTO_EN = 1
    chip.TILE_TOP[0].VSENSOR_AUTO_EN = 1
    chip.TILE_TOP[0].VSENSOR1_AUTO_EN = 1
    chip.TILE_TOP[0].TSENSOR_RSTB = 1
    chip.TILE_TOP[0].VSENSOR_RSTB = 1
    chip.TILE_TOP[0].VSENSOR1_RSTB = 1
  */
  credo_sensor_0x3a_tsensor_auto_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor_auto_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor1_auto_en_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_tsensor_rstb_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor_rstb_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0x3a_vsensor1_rstb_rmw(dev_id, dev_port, ln, &reg32, 1);

  /* pre-set sel */
  credo_sensor_0xf6_vsensor_vin_sel_rmw(dev_id, dev_port, ln, &reg32, 8);
  credo_sensor_0xf3_vsensor1_vin_sel_rmw(dev_id, dev_port, ln, &reg32, 8);

  /* pre-set SDE=0 */
  credo_sensor_0xf3_vsensor1_sde_rmw(dev_id, dev_port, ln, &reg32, 0);

  // now power-dn both sensors
  credo_sensor_0xf6_pd_vsensor_rmw(dev_id, dev_port, ln, &reg32, 1);
  credo_sensor_0xf3_pd_vsensor1_rmw(dev_id, dev_port, ln, &reg32, 1);

  /* pre-set rstn and run */
  credo_sensor_0xf6_vsensor_rstn_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_sensor_0xf6_vsensor_run_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_sensor_0xf3_vsensor1_rstn_rmw(dev_id, dev_port, ln, &reg32, 0);
  credo_sensor_0xf3_vsensor1_run_rmw(dev_id, dev_port, ln, &reg32, 0);
  /*
    chip.TILE_TOP[0].PD_VSENSOR1 = 0
    chip.TILE_TOP[0].VSENSOR1_VIN_SEL = 8
    time.sleep(wait)
  */
  credo_sensor_0xf3_pd_vsensor1_rmw(dev_id, dev_port, ln, &reg32, 0);
  bf_sys_usleep(10);

  /* de-assert rstn */
  credo_sensor_0xf3_vsensor1_rstn_rmw(dev_id, dev_port, ln, &reg32, 1);

  /* assert run */
  credo_sensor_0xf3_vsensor1_run_rmw(dev_id, dev_port, ln, &reg32, 1);
  bf_sys_usleep(5000);

  int tout = 10000;
  uint32_t done[8] = {0};
  while (--tout > 0) {
    if (!done[0]) {
      credo_top_pll_0x50_sensor_0_get(
          dev_id, dev_port, ln, &reg32, &s[0], true);
      if ((s[0] >> 12) & 1) done[0] = 1;
    }
    if (!done[1]) {
      credo_top_pll_0x51_sensor_1_get(
          dev_id, dev_port, ln, &reg32, &s[1], true);
      if ((s[1] >> 12) & 1) done[1] = 1;
    }
    if (!done[2]) {
      credo_top_pll_0x52_sensor_2_get(
          dev_id, dev_port, ln, &reg32, &s[2], true);
      if ((s[2] >> 12) & 1) done[2] = 1;
    }
    if (!done[3]) {
      credo_top_pll_0x53_sensor_3_get(
          dev_id, dev_port, ln, &reg32, &s[3], true);
      if ((s[3] >> 12) & 1) done[3] = 1;
    }
    if (!done[4]) {
      credo_top_pll_0x54_sensor_4_get(
          dev_id, dev_port, ln, &reg32, &s[4], true);
      if ((s[4] >> 12) & 1) done[4] = 1;
    }
    if (!done[5]) {
      credo_top_pll_0x55_sensor_5_get(
          dev_id, dev_port, ln, &reg32, &s[5], true);
      if ((s[5] >> 12) & 1) done[5] = 1;
    }
    if (!done[6]) {
      credo_top_pll_0x56_sensor_6_get(
          dev_id, dev_port, ln, &reg32, &s[6], true);
      if ((s[6] >> 12) & 1) done[6] = 1;
    }
    if (!done[7]) {
      credo_top_pll_0x57_sensor_7_get(
          dev_id, dev_port, ln, &reg32, &s[7], true);
      if ((s[7] >> 12) & 1) done[7] = 1;
    }
    for (g = 0; g < 8; g++) {
      if (!done[g]) break;
    }
    if (g >= 8) {
      for (g = 0; g < 8; g++) {
        sensor_val = (s[g] & 0x0fff);
        mv = ((sensor_val + 1.0) / 256.0) * 1.224;
        rtnd_mv[g] = mv * 1000;
      }
      break;
    }
  }
  // Dump entire dataset
  port_mgr_log(
      "%d:%3d:%d: TOUT[s]   :: %5d :: %04x : %04x : %04x : %04x : %04x : %04x "
      ": %04x : %04x",
      dev_id,
      dev_port,
      ln,
      tout,
      s[0],
      s[1],
      s[2],
      s[3],
      s[4],
      s[5],
      s[6],
      s[7]);
  port_mgr_log(
      "%d:%3d:%d: TOUT[done]:: %5d :: %04x : %04x : %04x : %04x : %04x : %04x "
      ": %04x : %04x",
      dev_id,
      dev_port,
      ln,
      tout,
      done[0],
      done[1],
      done[2],
      done[3],
      done[4],
      done[5],
      done[6],
      done[7]);
  return BF_INVALID_ARG;  // timeout
}

typedef struct bg_sample_t {
  float mv;
  uint32_t bg_val;
} bg_sample_t;

// indexed by [pipe][port-id][ln][bg-val-0-7]
bg_sample_t tx_bg_sample[4][72][8][8] = {{{{{0}}}}};
bg_sample_t rx_bg_sample[4][72][8][8] = {{{{{0}}}}};

void port_mgr_tof2_bandgap_cal(bf_dev_id_t dev_id) {
  uint32_t pipe_id, port_id;
  bf_dev_port_t dev_port;
  uint32_t ln, n_lanes;
  bf_status_t rc;
  float mv[8];
  int bg_val;
  uint32_t tile, grp, mac_stn_id, ch;
  uint32_t reg32;
  uint32_t num_pipes = 0;
  lld_sku_get_num_active_pipes(dev_id, &num_pipes);

  port_mgr_tof2_vsensor_disable_all(dev_id, num_pipes);

  n_lanes = 8;
  for (ln = 0; ln < n_lanes; ln++) {
    for (bg_val = 7; bg_val >= 0; bg_val--) {
      // enable tx pll sensors
      for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        for (port_id = 8; port_id < 72; port_id += 8) {
          dev_port = MAKE_DEV_PORT(pipe_id, port_id);

          // make sure its valid
          rc = port_mgr_tof2_map_dev_port_to_all(
              dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
          if (rc != BF_SUCCESS) continue;

          credo_rsvd_0x3f_tx_vbg_rmw(dev_id, dev_port, ln, &reg32, bg_val);
          port_mgr_tof2_vsensor_tx_pll_enable(dev_id, dev_port, ln);
        }
      }
      bf_sys_usleep(20000);

      // read out the 8 per-tile sensor values
      for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        port_id = 8;
        dev_port = MAKE_DEV_PORT(pipe_id, port_id);

        // make sure its valid
        rc = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
        if (rc != BF_SUCCESS) continue;

        rc = port_mgr_tof2_vsensor_tile(dev_id, dev_port, ln, mv);
        for (port_id = 8; port_id < 72; port_id += 8) {
          dev_port = MAKE_DEV_PORT(pipe_id, port_id);

          // make sure its valid
          rc = port_mgr_tof2_map_dev_port_to_all(
              dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
          if (rc != BF_SUCCESS) continue;

          rc = lld_sku_map_mac_stn_id_to_tile_and_group(
              dev_id, mac_stn_id, &tile, &grp);
          if (rc) {
            continue;
          }

          tx_bg_sample[pipe_id][port_id][ln][bg_val].mv = mv[grp];
          tx_bg_sample[pipe_id][port_id][ln][bg_val].bg_val = bg_val;
          // port_mgr_log("%d:%3d:%d : Tx bandgap_cal : %fmv <%d>", dev_id,
          // dev_port, ln, mv[grp], bg_val);
        }
      }

      // disable sensors
      for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        for (port_id = 8; port_id < 72; port_id += 8) {
          dev_port = MAKE_DEV_PORT(pipe_id, port_id);

          // make sure its valid
          rc = port_mgr_tof2_map_dev_port_to_all(
              dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
          if (rc != BF_SUCCESS) continue;

          port_mgr_tof2_vsensor_tx_pll_disable(dev_id, dev_port, ln);
        }
      }
    }
    // find best setting
    for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
      for (port_id = 8; port_id < 72; port_id += 8) {
        float closest_mv_diff = 1000.0;
        float closest_mv = 1000.0;
        uint32_t closest_bg = 0;

        dev_port = MAKE_DEV_PORT(pipe_id, port_id);

        for (int val = 0; val < 8; val++) {
          if (fabs(tx_bg_sample[pipe_id][port_id][ln][val].mv - 750.0) <
              closest_mv_diff) {
            closest_mv_diff =
                fabs(tx_bg_sample[pipe_id][port_id][ln][val].mv - 750.0);
            closest_mv = tx_bg_sample[pipe_id][port_id][ln][val].mv;
            closest_bg = tx_bg_sample[pipe_id][port_id][ln][val].bg_val;
          }
        }
        rc = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
        if (rc != BF_SUCCESS) continue;

        rc = lld_sku_map_mac_stn_id_to_tile_and_group(
            dev_id, mac_stn_id, &tile, &grp);
        if (rc) {
          continue;
        }

        calibrated_bandgap_setting[tile][grp][ln].tx_cal = closest_bg;

        port_mgr_log(
            "%d:%3d:%d : tile=%d : grp=%d : Tx BG setting=%d : BG=%3.0fmv "
            "[%3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f]",
            dev_id,
            dev_port,
            ln,
            tile,
            grp,
            closest_bg,
            closest_mv,
            tx_bg_sample[pipe_id][port_id][ln][0].mv,
            tx_bg_sample[pipe_id][port_id][ln][1].mv,
            tx_bg_sample[pipe_id][port_id][ln][2].mv,
            tx_bg_sample[pipe_id][port_id][ln][3].mv,
            tx_bg_sample[pipe_id][port_id][ln][4].mv,
            tx_bg_sample[pipe_id][port_id][ln][5].mv,
            tx_bg_sample[pipe_id][port_id][ln][6].mv,
            tx_bg_sample[pipe_id][port_id][ln][7].mv);
      }
    }

    for (bg_val = 7; bg_val >= 0; bg_val--) {
      // enable rx pll sensors
      for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        for (port_id = 8; port_id < 72; port_id += 8) {
          dev_port = MAKE_DEV_PORT(pipe_id, port_id);

          // make sure its valid
          rc = port_mgr_tof2_map_dev_port_to_all(
              dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
          if (rc != BF_SUCCESS) continue;

          credo_rx_0xff_rx_vbg_rmw(dev_id, dev_port, ln, &reg32, bg_val);
          port_mgr_tof2_vsensor_rx_pll_enable(dev_id, dev_port, ln);
        }
      }

      bf_sys_usleep(20000);

      // read out the 8 per-tile sensor values
      for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        port_id = 8;
        dev_port = MAKE_DEV_PORT(pipe_id, port_id);

        // make sure its valid
        rc = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
        if (rc != BF_SUCCESS) continue;

        rc = port_mgr_tof2_vsensor_tile(dev_id, dev_port, ln, mv);
        for (port_id = 8; port_id < 72; port_id += 8) {
          dev_port = MAKE_DEV_PORT(pipe_id, port_id);

          // make sure its valid
          rc = port_mgr_tof2_map_dev_port_to_all(
              dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
          if (rc != BF_SUCCESS) continue;

          rc = lld_sku_map_mac_stn_id_to_tile_and_group(
              dev_id, mac_stn_id, &tile, &grp);
          if (rc) {
            continue;
          }

          rx_bg_sample[pipe_id][port_id][ln][bg_val].mv = mv[grp];
          rx_bg_sample[pipe_id][port_id][ln][bg_val].bg_val = bg_val;
          // port_mgr_log("%d:%3d:%d : Tx bandgap_cal : %fmv <%d>", dev_id,
          // dev_port, ln, mv[grp], bg_val);
        }
      }

      // disable sensors
      for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
        for (port_id = 8; port_id < 72; port_id += 8) {
          dev_port = MAKE_DEV_PORT(pipe_id, port_id);

          // make sure its valid
          rc = port_mgr_tof2_map_dev_port_to_all(
              dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
          if (rc != BF_SUCCESS) continue;

          port_mgr_tof2_vsensor_rx_pll_disable(dev_id, dev_port, ln);
        }
      }
    }
    // find best setting
    for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
      for (port_id = 8; port_id < 72; port_id += 8) {
        float closest_mv_diff = 1000.0;
        float closest_mv = 1000.0;
        uint32_t closest_bg = 0;

        dev_port = MAKE_DEV_PORT(pipe_id, port_id);

        for (int val = 0; val < 8; val++) {
          if (fabs(rx_bg_sample[pipe_id][port_id][ln][val].mv - 750.0) <
              closest_mv_diff) {
            closest_mv_diff =
                fabs(rx_bg_sample[pipe_id][port_id][ln][val].mv - 750.0);
            closest_mv = rx_bg_sample[pipe_id][port_id][ln][val].mv;
            closest_bg = rx_bg_sample[pipe_id][port_id][ln][val].bg_val;
          }
        }
        rc = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_stn_id, &ch, NULL);
        if (rc != BF_SUCCESS) continue;

        rc = lld_sku_map_mac_stn_id_to_tile_and_group(
            dev_id, mac_stn_id, &tile, &grp);
        if (rc) {
          continue;
        }
        calibrated_bandgap_setting[tile][grp][ln].rx_cal = closest_bg;

        port_mgr_log(
            "%d:%3d:%d : tile=%d : grp=%d : Rx BG setting=%d : BG=%3.0fmv "
            "[%3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f]",
            dev_id,
            dev_port,
            ln,
            tile,
            grp,
            closest_bg,
            closest_mv,
            rx_bg_sample[pipe_id][port_id][ln][0].mv,
            rx_bg_sample[pipe_id][port_id][ln][1].mv,
            rx_bg_sample[pipe_id][port_id][ln][2].mv,
            rx_bg_sample[pipe_id][port_id][ln][3].mv,
            rx_bg_sample[pipe_id][port_id][ln][4].mv,
            rx_bg_sample[pipe_id][port_id][ln][5].mv,
            rx_bg_sample[pipe_id][port_id][ln][6].mv,
            rx_bg_sample[pipe_id][port_id][ln][7].mv);
      }
    }
  }
}
