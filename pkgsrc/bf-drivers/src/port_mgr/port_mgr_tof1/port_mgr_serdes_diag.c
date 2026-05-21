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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <dvm/bf_drv_intf.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_serdes_if.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof1_map.h"
#include "port_mgr_mac.h"
#include "port_mgr_serdes.h"
#include "port_mgr_serdes_diag.h"
#include <port_mgr/port_mgr_serdes_sbus_map.h>
#include "port_mgr_av_sd.h"
#include <avago/avago_aapl.h>

// for aim_printf
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

void sd_scan(bf_dev_id_t dev_id, int scan_for_lp_only);
void sd_optimize_tx_eq_settings(void);
void sd_print_prbs_banner(ucli_context_t *uc);
void sd_dump_this_prbs(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd);
int aapl_escope_main(int argc, char **argv, Aapl_t *aapl);
void sd_tx_eq_all(bf_dev_id_t dev_id, int pre, int attn, int post);

uint32_t bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS31;

FILE *diag_fd;
int to_file = 1;

int g_num_devices = 0;
int g_num_rings = 0;

typedef struct optimum_val_t {
  float q_val;
  int pre;
  int post;
  int atten;
} optimum_val_t;

optimum_val_t best_tx_eq_solution[2][2][256] = {{{{0}}}};
Avago_serdes_eye_data_t best_eye[2][2][256] = {{{{0}}}};
int last_dev_id = 0;
int last_ring = 0;
int last_lp = 0;
Avago_serdes_eye_data_t last_eye;

void sd_escope(int argc, char **argv) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(0);
  aapl_escope_main(argc, argv, aapl);
}

void sd_set_prbs_mode(ucli_context_t *uc, uint32_t order) {
  switch (order) {
    case 31:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS31;
      break;
    case 23:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS23;
      break;
    case 15:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS15;
      break;
    case 13:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS13;
      break;
    case 11:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS11;
      break;
    case 9:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS9;
      break;
    case 7:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS7;
      break;
    default:
      bfn_prbs_mode = AVAGO_SERDES_TX_DATA_SEL_PRBS31;
      if (uc != NULL) {
        aim_printf(&uc->pvs,
                   "Valid PRBS modes are: 7, 9, 11, 13, 15, 23, 31\n");
        aim_printf(&uc->pvs, "PRBS mode reset to PRBS31\n");
      }
      break;
  }
  if (uc != NULL) {
    aim_printf(&uc->pvs, "PRBS mode set to PRBS%d\n", order);
  }
}

int sd_init_prbs31_10g(bf_dev_id_t dev_id, int ring, int sd, int ilb) {
  int rc;

  rc = port_mgr_av_sd_init(dev_id,
                           ring,
                           sd,
                           TRUE,  // FALSE /*no reset*/,
                           ilb ? AVAGO_PRBS31_ILB : AVAGO_PRBS31_ELB,
                           66 /*divider*/,
                           20 /*data_width*/,
                           TRUE,  // FALSE /*phase_cal*/,
                           TRUE /*output_en*/);

  // make sure rigt termination is set
  port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_AVDD);
  port_mgr_av_sd_pll_bbgain_set(dev_id, ring, sd, true);

  return rc;
}

int sd_init_prbs31_25g(bf_dev_id_t dev_id, int ring, int sd, int ilb) {
  int rc;

  rc = port_mgr_av_sd_init(dev_id,
                           ring,
                           sd,
                           TRUE,  // FALSE /*no reset*/,
                           ilb ? AVAGO_PRBS31_ILB : AVAGO_PRBS31_ELB,
                           165 /*divider*/,
                           40 /*data_width*/,
                           TRUE,  // FALSE /*phase_cal*/,
                           TRUE /*output_en*/);

  // make sure rigt termination is set
  port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_AVDD);
  port_mgr_av_sd_pll_bbgain_set(dev_id, ring, sd, false);

  return rc;
}

static int sd_init_core_data(bf_dev_id_t dev_id, int ring, int sd) {
  int rc;

  rc = port_mgr_av_sd_init(dev_id,
                           ring,
                           sd,
                           FALSE /*no reset*/,
                           AVAGO_CORE_DATA_ELB,
                           165 /*divider*/,
                           40 /*data_width*/,
                           FALSE /*phase_cal*/,
                           TRUE /*output_en*/);
  return rc;
}

void sd_init_prbs(
    ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd, int ilb, int gb) {
  int err;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
    if (gb == 10) {
      err = sd_init_prbs31_10g(dev_id, ring, sd, ilb);
    } else {
      err = sd_init_prbs31_25g(dev_id, ring, sd, ilb);
    }
    if (uc != NULL) {
      aim_printf(
          &uc->pvs,
          "Serdes: %d:%d:%d : PRBS%d %s <%dGb>: rc=%d\n",
          dev_id,
          ring,
          sd,
          (bfn_prbs_mode == AVAGO_SERDES_TX_DATA_SEL_PRBS31)
              ? 31
              : (bfn_prbs_mode == AVAGO_SERDES_TX_DATA_SEL_PRBS23)
                    ? 23
                    : (bfn_prbs_mode == AVAGO_SERDES_TX_DATA_SEL_PRBS15)
                          ? 15
                          : (bfn_prbs_mode == AVAGO_SERDES_TX_DATA_SEL_PRBS13)
                                ? 13
                                : (bfn_prbs_mode ==
                                   AVAGO_SERDES_TX_DATA_SEL_PRBS11)
                                      ? 11
                                      : (bfn_prbs_mode ==
                                         AVAGO_SERDES_TX_DATA_SEL_PRBS9)
                                            ? 9
                                            : (bfn_prbs_mode ==
                                               AVAGO_SERDES_TX_DATA_SEL_PRBS7)
                                                  ? 7
                                                  : -1,
          ilb ? "ILB" : "ELB",
          gb,
          err);
    }
    // make sure rigt termination is set
    port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_AVDD);
    bf_sys_usleep(40000);
  }
}

void sd_all_int(ucli_context_t *uc,
                bf_dev_id_t dev_id,
                uint32_t int_code,
                uint32_t int_data) {
  int ring, sd, n_dumped = 0;

  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  aim_printf(&uc->pvs, "Node |  INT 0x%04X  DATA 0x%08X", int_code, int_data);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        uint32_t val =
            port_mgr_av_sd_spico_int(dev_id, ring, sd, int_code, int_data);
        if ((n_dumped++ % 4) == 0) {
          aim_printf(&uc->pvs, "\n %d:%d:%3d |", dev_id, ring, sd);
        }
        aim_printf(&uc->pvs, " %08x |", val);
      }
    }
  }
}

void sd_all_sig_ok_thresh_en(bf_dev_id_t dev_id) {
  int ring, sd;

  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x20);
      }
    }
  }
}

void sd_all_sig_ok_thresh(bf_dev_id_t dev_id, int thresh) {
  int ring, sd;

  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x20);
        port_mgr_av_sd_spico_int(
            dev_id, ring, sd, 0x20, 0x0040 | (thresh << 8));
      }
    }
  }
}

void sd_all_swing(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;
  uint32_t swing_lo, swing_hi;

  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        uint32_t lo_los_neq_0, lo_sig_ok_eq_0, lo_ei_neq_0, calibrated_thresh;

        sd_swing(dev_id,
                 ring,
                 sd,
                 &swing_lo,
                 &swing_hi,
                 &lo_los_neq_0,
                 &lo_sig_ok_eq_0,
                 &lo_ei_neq_0,
                 &calibrated_thresh);
        aim_printf(&uc->pvs,
                   "%d:%d:%3d : %3d - %3d mV : LOS= %3d mv : sig_ok= %3d mv : "
                   "EI= %3d mv : Calibrated= %3d mv  <diff= %3d mv>\n",
                   dev_id,
                   ring,
                   sd,
                   swing_lo,
                   swing_hi,
                   lo_los_neq_0,
                   lo_sig_ok_eq_0,
                   lo_ei_neq_0,
                   calibrated_thresh,
                   calibrated_thresh - lo_ei_neq_0);
      }
    }
  }
}

void sd_init_all_prbs(ucli_context_t *uc, bf_dev_id_t dev_id, int ilb, int gb) {
  int ring, sd;

  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      sd_init_prbs(uc, dev_id, ring, sd, ilb, gb);
    }
  }
}

void sd_init_all_core_data(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd, err;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        err = sd_init_core_data(dev_id, ring, sd);
        aim_printf(&uc->pvs,
                   "Serdes: %d:%d:%d : CORE : rc=%d\n",
                   dev_id,
                   ring,
                   sd,
                   err);
        // make sure rigt termination is set
        port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_AVDD);
      }
    }
  }
}

void sd_switch_to_prbs(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd, err;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        // set Tx data sel
        err = port_mgr_av_sd_tx_data_sel_set(
            dev_id, ring, sd, BF_SDS_PAT_PATSEL_PRBS31);
        if (err) {
          aim_printf(&uc->pvs,
                     "Serdes: %d:%d:%d : Chg to PRBS : tx data sel : rc=%d\n",
                     dev_id,
                     ring,
                     sd,
                     err);
        }
        // set rx data sel
        err = port_mgr_av_sd_set_rx_cmp_sel(
            dev_id, ring, sd, AVAGO_SERDES_RX_CMP_DATA_PRBS31);
        if (err) {
          aim_printf(&uc->pvs,
                     "Serdes: %d:%d:%d : Chg to PRBS : rx cmp sel : rc=%d\n",
                     dev_id,
                     ring,
                     sd,
                     err);
          sd--;
          continue;  // retry
        }

        // set rx cmp mode
        err = port_mgr_av_sd_set_rx_cmp_mode(
            dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
        if (err) {
          aim_printf(&uc->pvs,
                     "Serdes: %d:%d:%d : Chg to PRBS : rx cmp mode : rc=%d\n",
                     dev_id,
                     ring,
                     sd,
                     err);
          sd--;
          continue;  // retry
        }

        // make sure rigt termination is set
        port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_AVDD);
      }
    }
  }
}

void sd_dump_all_vbtc(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  aim_printf(&uc->pvs,
             "+-+-+---+--------+--------+--------+--------+--------+\n");
  aim_printf(&uc->pvs,
             "| |r|   |        |        |        |        |        |\n");
  aim_printf(&uc->pvs,
             "|d|i|   |        |        |        |        |        |\n");
  aim_printf(&uc->pvs,
             "|e|n| s |        |        |        |        |        |\n");
  aim_printf(&uc->pvs,
             "|v|g| d | 1e-06  | 1e-10  | 1e-12  | 1e-15  | 1e-17  |\n");
  aim_printf(&uc->pvs,
             "+-+-+---+--------+--------+--------+--------+--------+\n");
  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      int rc;
      int eye_ht_1e06;
      int eye_ht_1e10;
      int eye_ht_1e12;
      int eye_ht_1e15;
      int eye_ht_1e17;

      rc = port_mgr_av_sd_vbtc_get(dev_id,
                                   ring,
                                   sd,
                                   &eye_ht_1e06,
                                   &eye_ht_1e10,
                                   &eye_ht_1e12,
                                   &eye_ht_1e15,
                                   &eye_ht_1e17);
      if (rc < 0) continue;

      aim_printf(&uc->pvs,
                 "|%d:%d:%3d| %3d mV | %3d mV | %3d mV | %3d mV | %3d mV |\n",
                 dev_id,
                 ring,
                 sd,
                 eye_ht_1e06,
                 eye_ht_1e10,
                 eye_ht_1e12,
                 eye_ht_1e15,
                 eye_ht_1e17);
    }
  }
}

void sd_dump_all_dfe(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);
  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      Avago_serdes_tx_eq_t tx_eq;
      Avago_serdes_dfe_state_t dfe_state;
      Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
      int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

      avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);

      memset(&dfe_state, 0, sizeof(dfe_state));
      avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
      aim_printf(
          &uc->pvs,
          "%d:%d:%3d : TxEQ=%2d:%2d:%2d : DC= %3d : LF= %2d : HF= %2d : BW= "
          "%2d : Gain= %2d : Gain2= %2d : Taps: %5.1f : %3d : %3d : %3d : %3d "
          ": %3d : %3d : %3d : %3d\n",
          dev_id,
          ring,
          sd,
          tx_eq.pre,
          tx_eq.atten,
          tx_eq.post,
          dfe_state.dc,
          dfe_state.lf,
          dfe_state.hf,
          dfe_state.bw,
          dfe_state.dfeGAIN,
          dfe_state.dfeGAIN2,
          dfe_state.dfeTAP1,
          dfe_state.dfeTAP[0],
          dfe_state.dfeTAP[1],
          dfe_state.dfeTAP[2],
          dfe_state.dfeTAP[3],
          dfe_state.dfeTAP[4],
          dfe_state.dfeTAP[5],
          dfe_state.dfeTAP[6],
          dfe_state.dfeTAP[7]);
    }
  }
}

extern int dev_port_for_fp[];

void sd_perf_banner(ucli_context_t *uc) {
  aim_printf(
      &uc->pvs,
      "+--+-+--+-+-+-+---+--+--+--+--------+--------+--------+--------+--------"
      "+----+----+----+----+----+----+\n");
  aim_printf(
      &uc->pvs,
      "|P | |  | | |r|   |  | a| p|        |        |        |        |        "
      "|    |    |    |    |    |    |\n");
  aim_printf(
      &uc->pvs,
      "|o | | M| |d|i|   | p| t| o|        |        |        |        |        "
      "|    |    |    |    |    |    |\n");
  aim_printf(
      &uc->pvs,
      "|r |C| a|C|e|n| s | r| t| s|        |        |        |        |        "
      "|    |    |    |    |    |    |\n");
  aim_printf(
      &uc->pvs,
      "|t |h| c|h|v|g| d | e| n| t| 1e-06  | 1e-10  | 1e-12  | 1e-15  | 1e-17  "
      "| DC | LF | HF | BW |GAIN| GN2|\n");
  aim_printf(
      &uc->pvs,
      "+--+-+--+-+-+-+---+--+--+--+--------+--------+--------+--------+--------"
      "+----+----+----+----+----+----+\n");
}

void sd_set_tx_eq(bf_dev_id_t dev_id,
                  int ring,
                  int sd,
                  int pre,
                  int atten,
                  int post,
                  int slew,
                  void *display_ucli_cookie) {
  int state;
  Avago_serdes_tx_eq_t tx_eq = {0};
  Avago_serdes_tx_eq_limits_t limits;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  /* get min/max for the tx_eq parms of sd */
  avago_serdes_get_tx_eq_limits(aapl, sbus_addr, &limits);

  if ((pre < limits.pre_min) || (atten < limits.atten_min) ||
      (post < limits.post_min) || (pre > limits.pre_max) ||
      (atten > limits.atten_max) || (post > limits.post_max)) {
    aim_printf(&uc->pvs,
               "Limits: pre: %d-%d  atten: %d-%d  post: %d-%d\n",
               limits.pre_min,
               limits.pre_max,
               limits.atten_min,
               limits.atten_max,
               limits.post_min,
               limits.post_max);
    return;
  }
  tx_eq.pre = pre;
  tx_eq.post = post;
  tx_eq.atten = atten;
  tx_eq.slew = slew;
  state = avago_serdes_set_tx_eq(aapl, sbus_addr, &tx_eq);
  if (state) {
    aim_printf(&uc->pvs, "Set tx eq fails.\n");
    return;
  }
  return;
}

bf_status_t sd_dfe_set(bf_dev_id_t dev_id,
                       int ring,
                       int sd,
                       uint32_t dfe_ctrl,
                       uint32_t hf_val,
                       uint32_t lf_val,
                       uint32_t dc_val) {
  return port_mgr_serdes_tof_dfe_cfg_set(
      dev_id, ring, sd, dfe_ctrl, hf_val, lf_val, dc_val);
}
void sd_display_port_perf(bf_dev_id_t dev_id,
                          int ring,
                          int sd,
                          void *display_ucli_cookie) {
  bool possible_faulty_data = false;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  Avago_serdes_tx_eq_t tx_eq;
  Avago_serdes_dfe_state_t dfe_state;
  Avago_serdes_eye_config_t *eye_config;
  Avago_serdes_eye_data_t *eye_data;
  int sbus_addr;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) return;

  avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
  aim_printf(&uc->pvs, "%2d|%2d|%2d|", tx_eq.pre, tx_eq.atten, tx_eq.post);

  eye_config = avago_serdes_eye_config_construct(aapl);
  eye_data = avago_serdes_eye_data_construct(aapl);

  eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
  eye_config->ec_no_sbm = TRUE;

  if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
    possible_faulty_data = true;
  }

  if (eye_data->ed_vbtc.top_points == 0 ||
      eye_data->ed_vbtc.bottom_points == 0) {
    possible_faulty_data = true;
  }
  if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
      eye_data->ed_vbtc.top_R_squared < 0.95 ||
      eye_data->ed_vbtc.bottom_slope <= 0.0 ||
      eye_data->ed_vbtc.top_slope >= 0.0) {
    possible_faulty_data = true;
  }
  // get rid of negative values
  if (eye_data->ed_vbtc.vert_eye_1e06 < 0) eye_data->ed_vbtc.vert_eye_1e06 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e10 < 0) eye_data->ed_vbtc.vert_eye_1e10 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e12 < 0) eye_data->ed_vbtc.vert_eye_1e12 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e15 < 0) eye_data->ed_vbtc.vert_eye_1e15 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e17 < 0) eye_data->ed_vbtc.vert_eye_1e17 = 0;
  // also impossibly high values, > 1000
  if (eye_data->ed_vbtc.vert_eye_1e06 > 1000)
    eye_data->ed_vbtc.vert_eye_1e06 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e10 > 1000)
    eye_data->ed_vbtc.vert_eye_1e10 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e12 > 1000)
    eye_data->ed_vbtc.vert_eye_1e12 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e15 > 1000)
    eye_data->ed_vbtc.vert_eye_1e15 = 0;
  if (eye_data->ed_vbtc.vert_eye_1e17 > 1000)
    eye_data->ed_vbtc.vert_eye_1e17 = 0;

  aim_printf(&uc->pvs,
             "%s%3d mV | %3d mV | %3d mV | %3d mV | %3d mV |",
             possible_faulty_data ? "*" : " ",
             eye_data->ed_vbtc.vert_eye_1e06,
             eye_data->ed_vbtc.vert_eye_1e10,
             eye_data->ed_vbtc.vert_eye_1e12,
             eye_data->ed_vbtc.vert_eye_1e15,
             eye_data->ed_vbtc.vert_eye_1e17);

  avago_serdes_eye_data_destruct(aapl, eye_data);
  avago_serdes_eye_config_destruct(aapl, eye_config);

  memset(&dfe_state, 0, sizeof(dfe_state));
  avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
  aim_printf(
      &uc->pvs,
      "%3d | %2d | %2d | %2d | %2d | %2d | %5.1f | %3d | %3d | %3d | %3d "
      "| %3d | %3d | %3d | %3d\n",
      dfe_state.dc,
      dfe_state.lf,
      dfe_state.hf,
      dfe_state.bw,
      dfe_state.dfeGAIN,
      dfe_state.dfeGAIN2,
      dfe_state.dfeTAP1,
      dfe_state.dfeTAP[0],
      dfe_state.dfeTAP[1],
      dfe_state.dfeTAP[2],
      dfe_state.dfeTAP[3],
      dfe_state.dfeTAP[4],
      dfe_state.dfeTAP[5],
      dfe_state.dfeTAP[6],
      dfe_state.dfeTAP[7]);
}

void sd_dump_all_perf(ucli_context_t *uc, bool is_mav) {
  bf_dev_id_t dev_id = 0;
  int ring, sd, fp, ch, mac_block, n_lanes, state, ln;
  bf_dev_port_t dev_port;
  port_mgr_serdes_t *serdes_p;
  int lines = 0;
  int max_fp = is_mav ? 65 : 33;

  for (fp = 0; fp < max_fp; fp++) {
    for (ch = 0; ch < 4; ch++) {
      port_mgr_port_t *port_p;
      bool possible_faulty_data = false;

      dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
      dev_port |= ch;

      // see if its a valid port
      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (port_p == NULL) continue;

      // see if its up
      bf_port_oper_state_get_no_side_effect(dev_id, dev_port, &state);
      if (state == 0) continue;  // down, skip it

      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
      for (ln = 0; ln < n_lanes; ln++) {
        Avago_serdes_tx_eq_t tx_eq;
        Avago_serdes_dfe_state_t dfe_state;
        Avago_serdes_eye_config_t *eye_config;
        Avago_serdes_eye_data_t *eye_data;

        Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
        int sbus_addr;
        serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
        if (serdes_p == NULL) continue;

        if ((lines++ % 32) == 0) {
          sd_perf_banner(uc);
        }

        ring = serdes_p->ring;
        sd = serdes_p->rx_sd;
        aim_printf(&uc->pvs,
                   "|%2d/%d|%2d|%d|%d|%d|%3d|",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd);

        sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
        if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

        avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
        aim_printf(
            &uc->pvs, "%2d|%2d|%2d|", tx_eq.pre, tx_eq.atten, tx_eq.post);

        eye_config = avago_serdes_eye_config_construct(aapl);
        eye_data = avago_serdes_eye_data_construct(aapl);

        eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
        eye_config->ec_no_sbm = TRUE;

        if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
          possible_faulty_data = true;
          // avago_serdes_eye_data_destruct(aapl, eye_data);
          // avago_serdes_eye_config_destruct(aapl, eye_config);
          // aim_printf(&uc->pvs, "<err>\n");
          // continue;
        }

        if (eye_data->ed_vbtc.top_points == 0 ||
            eye_data->ed_vbtc.bottom_points == 0) {
          possible_faulty_data = true;
          // avago_serdes_eye_data_destruct(aapl, eye_data);
          // avago_serdes_eye_config_destruct(aapl, eye_config);
          // aim_printf(&uc->pvs, "<err>\n");
          // continue;
        }
        if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
            eye_data->ed_vbtc.top_R_squared < 0.95 ||
            eye_data->ed_vbtc.bottom_slope <= 0.0 ||
            eye_data->ed_vbtc.top_slope >= 0.0) {
          possible_faulty_data = true;
          // avago_serdes_eye_data_destruct(aapl, eye_data);
          // avago_serdes_eye_config_destruct(aapl, eye_config);
          // aim_printf(&uc->pvs, "<err>\n");
          // continue;
        }
        // get rid of negative values
        if (eye_data->ed_vbtc.vert_eye_1e06 < 0)
          eye_data->ed_vbtc.vert_eye_1e06 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e10 < 0)
          eye_data->ed_vbtc.vert_eye_1e10 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e12 < 0)
          eye_data->ed_vbtc.vert_eye_1e12 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e15 < 0)
          eye_data->ed_vbtc.vert_eye_1e15 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e17 < 0)
          eye_data->ed_vbtc.vert_eye_1e17 = 0;

        // also impossibly high values, > 1000
        if (eye_data->ed_vbtc.vert_eye_1e06 > 1000)
          eye_data->ed_vbtc.vert_eye_1e06 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e10 > 1000)
          eye_data->ed_vbtc.vert_eye_1e10 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e12 > 1000)
          eye_data->ed_vbtc.vert_eye_1e12 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e15 > 1000)
          eye_data->ed_vbtc.vert_eye_1e15 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e17 > 1000)
          eye_data->ed_vbtc.vert_eye_1e17 = 0;

        aim_printf(&uc->pvs,
                   "%s%3d mV | %3d mV | %3d mV | %3d mV | %3d mV |",
                   possible_faulty_data ? "*" : " ",
                   eye_data->ed_vbtc.vert_eye_1e06,
                   eye_data->ed_vbtc.vert_eye_1e10,
                   eye_data->ed_vbtc.vert_eye_1e12,
                   eye_data->ed_vbtc.vert_eye_1e15,
                   eye_data->ed_vbtc.vert_eye_1e17);

        avago_serdes_eye_data_destruct(aapl, eye_data);
        avago_serdes_eye_config_destruct(aapl, eye_config);

        memset(&dfe_state, 0, sizeof(dfe_state));
        avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
        aim_printf(
            &uc->pvs,
            "%3d | %2d | %2d | %2d | %2d | %2d | %5.1f | %3d | %3d | %3d | %3d "
            "| %3d | %3d | %3d | %3d\n",
            dfe_state.dc,
            dfe_state.lf,
            dfe_state.hf,
            dfe_state.bw,
            dfe_state.dfeGAIN,
            dfe_state.dfeGAIN2,
            dfe_state.dfeTAP1,
            dfe_state.dfeTAP[0],
            dfe_state.dfeTAP[1],
            dfe_state.dfeTAP[2],
            dfe_state.dfeTAP[3],
            dfe_state.dfeTAP[4],
            dfe_state.dfeTAP[5],
            dfe_state.dfeTAP[6],
            dfe_state.dfeTAP[7]);
      }
    }
  }
}

#if 0
FILE *diag_perf_fd = NULL;

void perf_prbs_file_open(void) {

  if (diag_perf_fd != NULL) return;

  diag_perf_fd = fopen("serdes-performance.txt", "a");

    fprintf(diag_fd, "CTLE Sweep: ring=%d : sd=%d\n\n", ring, sd);
}
#endif

void sd_dump_all_perf_prbs(ucli_context_t *uc, bool is_mav) {
  bf_dev_id_t dev_id = 0;
  int ring, sd, fp, ch, mac_block, n_lanes, ln;
  bf_dev_port_t dev_port;
  port_mgr_serdes_t *serdes_p;
  int lines = 0;
  int max_fp = is_mav ? 65 : 33;

  for (fp = 0; fp < max_fp; fp++) {
    for (ch = 0; ch < 4; ch++) {
      port_mgr_port_t *port_p;

      dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
      dev_port |= ch;

      // see if its a valid port
      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (port_p == NULL) continue;

      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
      for (ln = 0; ln < n_lanes; ln++) {
        Avago_serdes_tx_eq_t tx_eq;
        Avago_serdes_dfe_state_t dfe_state;
        Avago_serdes_eye_config_t *eye_config;
        Avago_serdes_eye_data_t *eye_data;

        Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
        int sbus_addr;
        serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
        if (serdes_p == NULL) continue;

        if ((lines++ % 32) == 0) {
          sd_perf_banner(uc);
        }

        ring = serdes_p->ring;
        sd = serdes_p->rx_sd;
        aim_printf(&uc->pvs,
                   "|%2d/%d|%2d|%d|%d|%d|%3d|",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd);

        sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
        if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

        avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
        aim_printf(
            &uc->pvs, "%2d|%2d|%2d|", tx_eq.pre, tx_eq.atten, tx_eq.post);

        eye_config = avago_serdes_eye_config_construct(aapl);
        eye_data = avago_serdes_eye_data_construct(aapl);

        eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
        eye_config->ec_no_sbm = TRUE;

        if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);
          aim_printf(&uc->pvs, "<err>\n");
          continue;
        }

        if (eye_data->ed_vbtc.top_points == 0 ||
            eye_data->ed_vbtc.bottom_points == 0) {
          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);
          aim_printf(&uc->pvs, "<err>\n");
          continue;
        }
        if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
            eye_data->ed_vbtc.top_R_squared < 0.95 ||
            eye_data->ed_vbtc.bottom_slope <= 0.0 ||
            eye_data->ed_vbtc.top_slope >= 0.0) {
          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);
          aim_printf(&uc->pvs, "<err>\n");
          continue;
        }
        // get rid of negative values
        if (eye_data->ed_vbtc.vert_eye_1e06 < 0)
          eye_data->ed_vbtc.vert_eye_1e06 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e10 < 0)
          eye_data->ed_vbtc.vert_eye_1e10 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e12 < 0)
          eye_data->ed_vbtc.vert_eye_1e12 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e15 < 0)
          eye_data->ed_vbtc.vert_eye_1e15 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e17 < 0)
          eye_data->ed_vbtc.vert_eye_1e17 = 0;

        aim_printf(&uc->pvs,
                   " %3d mV | %3d mV | %3d mV | %3d mV | %3d mV |",
                   eye_data->ed_vbtc.vert_eye_1e06,
                   eye_data->ed_vbtc.vert_eye_1e10,
                   eye_data->ed_vbtc.vert_eye_1e12,
                   eye_data->ed_vbtc.vert_eye_1e15,
                   eye_data->ed_vbtc.vert_eye_1e17);

        avago_serdes_eye_data_destruct(aapl, eye_data);
        avago_serdes_eye_config_destruct(aapl, eye_config);

        memset(&dfe_state, 0, sizeof(dfe_state));
        avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
        aim_printf(
            &uc->pvs,
            "%3d | %2d | %2d | %2d | %2d | %2d | %5.1f | %3d | %3d | %3d | %3d "
            "| %3d | %3d | %3d | %3d\n",
            dfe_state.dc,
            dfe_state.lf,
            dfe_state.hf,
            dfe_state.bw,
            dfe_state.dfeGAIN,
            dfe_state.dfeGAIN2,
            dfe_state.dfeTAP1,
            dfe_state.dfeTAP[0],
            dfe_state.dfeTAP[1],
            dfe_state.dfeTAP[2],
            dfe_state.dfeTAP[3],
            dfe_state.dfeTAP[4],
            dfe_state.dfeTAP[5],
            dfe_state.dfeTAP[6],
            dfe_state.dfeTAP[7]);
      }
    }
  }
}

typedef struct eye_d_t {
  Avago_serdes_tx_eq_t tx_eq;
  Avago_serdes_dfe_state_t dfe_state;
  Avago_serdes_eye_data_t eye_data;
} eye_d_t;

#define SD_NUM_STATS 10
eye_d_t eye_stats[2][150][SD_NUM_STATS];

void sd_perf_stats_collect(ucli_context_t *uc, bool is_mav) {
  int pass, dev_id = 0, ring, sd, fp, ch, mac_block, n_lanes, ln;
  bf_dev_port_t dev_port;
  port_mgr_serdes_t *serdes_p;
  int max_fp = is_mav ? 65 : 33;

  for (pass = 0; pass < SD_NUM_STATS; pass++) {
    aim_printf(&uc->pvs, "\nPass: %d\n", pass);

    for (ring = 0; ring < g_num_rings; ring++) {
      for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
        if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
          port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        }
      }
    }
    for (ring = 0; ring < g_num_rings; ring++) {
      for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
        if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
          int max_dfe_wait = 10;
          bool dfe_running = false;

          do {
            dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
            if (dfe_running) {
              aim_printf(&uc->pvs, "%d.%d, ", ring, sd);
              bf_sys_usleep(1000000);
            }
          } while (dfe_running && max_dfe_wait--);
        }
      }
    }

    for (fp = 0; fp < max_fp; fp++) {
      for (ch = 0; ch < 4; ch++) {
        port_mgr_port_t *port_p;

        dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
        dev_port |= ch;

        // see if its a valid port
        port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
        if (port_p == NULL) continue;

        port_mgr_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
        bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
        for (ln = 0; ln < n_lanes; ln++) {
          Avago_serdes_tx_eq_t tx_eq;
          Avago_serdes_dfe_state_t dfe_state;
          Avago_serdes_eye_config_t *eye_config;
          Avago_serdes_eye_data_t *eye_data;

          Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
          int sbus_addr;
          serdes_p =
              port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
          if (serdes_p == NULL) continue;

          ring = serdes_p->ring;
          sd = serdes_p->rx_sd;
          sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
          if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

          memset((char *)&eye_stats[ring][sd][pass],
                 0,
                 sizeof(eye_stats[ring][sd][pass]));

          avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
          eye_stats[ring][sd][pass].tx_eq.pre = tx_eq.pre;
          eye_stats[ring][sd][pass].tx_eq.atten = tx_eq.atten;
          eye_stats[ring][sd][pass].tx_eq.post = tx_eq.post;

          eye_config = avago_serdes_eye_config_construct(aapl);
          eye_data = avago_serdes_eye_data_construct(aapl);

          eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
          eye_config->ec_no_sbm = TRUE;

          if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
            avago_serdes_eye_data_destruct(aapl, eye_data);
            avago_serdes_eye_config_destruct(aapl, eye_config);
            continue;
          }

          if (eye_data->ed_vbtc.top_points == 0 ||
              eye_data->ed_vbtc.bottom_points == 0) {
            avago_serdes_eye_data_destruct(aapl, eye_data);
            avago_serdes_eye_config_destruct(aapl, eye_config);
            continue;
          }
          if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
              eye_data->ed_vbtc.top_R_squared < 0.95 ||
              eye_data->ed_vbtc.bottom_slope <= 0.0 ||
              eye_data->ed_vbtc.top_slope >= 0.0) {
            avago_serdes_eye_data_destruct(aapl, eye_data);
            avago_serdes_eye_config_destruct(aapl, eye_config);
            continue;
          }
          // get rid of negative values
          if (eye_data->ed_vbtc.vert_eye_1e06 < 0)
            eye_data->ed_vbtc.vert_eye_1e06 = 0;
          if (eye_data->ed_vbtc.vert_eye_1e10 < 0)
            eye_data->ed_vbtc.vert_eye_1e10 = 0;
          if (eye_data->ed_vbtc.vert_eye_1e12 < 0)
            eye_data->ed_vbtc.vert_eye_1e12 = 0;
          if (eye_data->ed_vbtc.vert_eye_1e15 < 0)
            eye_data->ed_vbtc.vert_eye_1e15 = 0;
          if (eye_data->ed_vbtc.vert_eye_1e17 < 0)
            eye_data->ed_vbtc.vert_eye_1e17 = 0;

          eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e06 =
              eye_data->ed_vbtc.vert_eye_1e06;
          eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e10 =
              eye_data->ed_vbtc.vert_eye_1e10;
          eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e12 =
              eye_data->ed_vbtc.vert_eye_1e12;
          eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e15 =
              eye_data->ed_vbtc.vert_eye_1e15;
          eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e17 =
              eye_data->ed_vbtc.vert_eye_1e17;

          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);

          memset(&dfe_state, 0, sizeof(dfe_state));
          avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
          eye_stats[ring][sd][pass].dfe_state.dc = dfe_state.dc;
          eye_stats[ring][sd][pass].dfe_state.lf = dfe_state.lf;
          eye_stats[ring][sd][pass].dfe_state.hf = dfe_state.hf;
          eye_stats[ring][sd][pass].dfe_state.bw = dfe_state.bw;
          eye_stats[ring][sd][pass].dfe_state.dfeGAIN = dfe_state.dfeGAIN;
          eye_stats[ring][sd][pass].dfe_state.dfeGAIN2 = dfe_state.dfeGAIN2;
          eye_stats[ring][sd][pass].dfe_state.dfeTAP1 = dfe_state.dfeTAP1;
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[0] = dfe_state.dfeTAP[0];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[1] = dfe_state.dfeTAP[1];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[2] = dfe_state.dfeTAP[2];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[3] = dfe_state.dfeTAP[3];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[4] = dfe_state.dfeTAP[4];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[5] = dfe_state.dfeTAP[5];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[6] = dfe_state.dfeTAP[6];
          eye_stats[ring][sd][pass].dfe_state.dfeTAP[7] = dfe_state.dfeTAP[7];
        }
      }
    }
  }

  for (fp = 0; fp < max_fp; fp++) {
    sd_perf_banner(uc);
    for (ch = 0; ch < 4; ch++) {
      for (pass = 0; pass < SD_NUM_STATS; pass++) {
        port_mgr_port_t *port_p;

        dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
        dev_port |= ch;

        // see if its a valid port
        port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
        if (port_p == NULL) continue;

        port_mgr_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
        bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
        for (ln = 0; ln < n_lanes; ln++) {
          serdes_p =
              port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
          if (serdes_p == NULL) continue;

          ring = serdes_p->ring;
          sd = serdes_p->rx_sd;

          aim_printf(&uc->pvs,
                     "|%2d/%d|%2d|%d|%d|%d|%3d|",
                     fp + 1,
                     ch,
                     mac_block,
                     ch + ln,
                     dev_id,
                     ring,
                     sd);
          aim_printf(&uc->pvs,
                     "%2d|%2d|%2d|",
                     eye_stats[ring][sd][pass].tx_eq.pre,
                     eye_stats[ring][sd][pass].tx_eq.atten,
                     eye_stats[ring][sd][pass].tx_eq.post);

          aim_printf(&uc->pvs,
                     " %3d mV | %3d mV | %3d mV | %3d mV | %3d mV |",
                     eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e06,
                     eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e10,
                     eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e12,
                     eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e15,
                     eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e17);

          aim_printf(
              &uc->pvs,
              "%3d | %2d | %2d | %2d | %2d | %2d | %5.1f | %3d | %3d | %3d | "
              "%3d | %3d | %3d | %3d | %3d\n",
              eye_stats[ring][sd][pass].dfe_state.dc,
              eye_stats[ring][sd][pass].dfe_state.lf,
              eye_stats[ring][sd][pass].dfe_state.hf,
              eye_stats[ring][sd][pass].dfe_state.bw,
              eye_stats[ring][sd][pass].dfe_state.dfeGAIN,
              eye_stats[ring][sd][pass].dfe_state.dfeGAIN2,
              eye_stats[ring][sd][pass].dfe_state.dfeTAP1,
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[0],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[1],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[2],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[3],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[4],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[5],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[6],
              eye_stats[ring][sd][pass].dfe_state.dfeTAP[7]);
        }
      }
    }
  }

  for (fp = 0; fp < max_fp; fp++) {
    for (ch = 0; ch < 4; ch++) {
      port_mgr_port_t *port_p;

      dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
      dev_port |= ch;

      // see if its a valid port
      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (port_p == NULL) continue;

      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
      for (ln = 0; ln < n_lanes; ln++) {
        int i, min_e, max_e, avg_e, total_e = 0;
        int total_deviation;
        double std_deviation;

        serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
        if (serdes_p == NULL) continue;

        ring = serdes_p->ring;
        sd = serdes_p->rx_sd;

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e06;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }

        avg_e = total_e / SD_NUM_STATS;

        total_deviation = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e06;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / SD_NUM_STATS);

        aim_printf(&uc->pvs,
                   "1e06: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e10;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / SD_NUM_STATS;

        total_deviation = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e10;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / SD_NUM_STATS);

        aim_printf(&uc->pvs,
                   "1e10: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e12;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / SD_NUM_STATS;

        total_deviation = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e12;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / SD_NUM_STATS);

        aim_printf(&uc->pvs,
                   "1e12: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e15;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / SD_NUM_STATS;

        total_deviation = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e15;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / SD_NUM_STATS);

        aim_printf(&uc->pvs,
                   "1e15: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e17;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / SD_NUM_STATS;

        total_deviation = 0;
        for (pass = 0; pass < SD_NUM_STATS; pass++) {
          int val = eye_stats[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e17;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / SD_NUM_STATS);

        aim_printf(&uc->pvs,
                   "1e17: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");
      }
    }
  }
}

int on_demand_pass = 0;
int sd_num_stats = 0;
eye_d_t eye_stats_on_demand[2][150][SD_NUM_STATS];

void sd_perf_stats_collect_on_demand(ucli_context_t *uc, bool is_mav) {
  int pass, dev_id = 0, ring, sd, fp, ch, mac_block, n_lanes, ln;
  bf_dev_port_t dev_port;
  port_mgr_serdes_t *serdes_p;
  int max_fp = is_mav ? 65 : 33;

  pass = on_demand_pass;
  on_demand_pass = (on_demand_pass + 1) % SD_NUM_STATS;
  if (sd_num_stats < (SD_NUM_STATS - 1)) sd_num_stats++;

  aim_printf(&uc->pvs, "\nPass: %d\n", pass);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        int max_dfe_wait = 10;
        bool dfe_running = false;

        do {
          dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
          if (dfe_running) {
            aim_printf(&uc->pvs, "%d.%d, ", ring, sd);
            bf_sys_usleep(1000000);
          }
        } while (dfe_running && max_dfe_wait--);
      }
    }
  }

  for (fp = 0; fp < max_fp; fp++) {
    for (ch = 0; ch < 4; ch++) {
      port_mgr_port_t *port_p;

      dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
      dev_port |= ch;

      // see if its a valid port
      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (port_p == NULL) continue;

      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
      for (ln = 0; ln < n_lanes; ln++) {
        Avago_serdes_tx_eq_t tx_eq;
        Avago_serdes_dfe_state_t dfe_state;
        Avago_serdes_eye_config_t *eye_config;
        Avago_serdes_eye_data_t *eye_data;

        Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
        int sbus_addr;
        serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
        if (serdes_p == NULL) continue;

        ring = serdes_p->ring;
        sd = serdes_p->rx_sd;
        sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
        if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

        memset((char *)&eye_stats_on_demand[ring][sd][pass],
               0,
               sizeof(eye_stats_on_demand[ring][sd][pass]));

        avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
        eye_stats_on_demand[ring][sd][pass].tx_eq.pre = tx_eq.pre;
        eye_stats_on_demand[ring][sd][pass].tx_eq.atten = tx_eq.atten;
        eye_stats_on_demand[ring][sd][pass].tx_eq.post = tx_eq.post;

        eye_config = avago_serdes_eye_config_construct(aapl);
        eye_data = avago_serdes_eye_data_construct(aapl);

        eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
        eye_config->ec_no_sbm = TRUE;

        if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);
          continue;
        }

        if (eye_data->ed_vbtc.top_points == 0 ||
            eye_data->ed_vbtc.bottom_points == 0) {
          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);
          continue;
        }
        if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
            eye_data->ed_vbtc.top_R_squared < 0.95 ||
            eye_data->ed_vbtc.bottom_slope <= 0.0 ||
            eye_data->ed_vbtc.top_slope >= 0.0) {
          avago_serdes_eye_data_destruct(aapl, eye_data);
          avago_serdes_eye_config_destruct(aapl, eye_config);
          continue;
        }
        // get rid of negative values
        if (eye_data->ed_vbtc.vert_eye_1e06 < 0)
          eye_data->ed_vbtc.vert_eye_1e06 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e10 < 0)
          eye_data->ed_vbtc.vert_eye_1e10 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e12 < 0)
          eye_data->ed_vbtc.vert_eye_1e12 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e15 < 0)
          eye_data->ed_vbtc.vert_eye_1e15 = 0;
        if (eye_data->ed_vbtc.vert_eye_1e17 < 0)
          eye_data->ed_vbtc.vert_eye_1e17 = 0;

        eye_stats_on_demand[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e06 =
            eye_data->ed_vbtc.vert_eye_1e06;
        eye_stats_on_demand[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e10 =
            eye_data->ed_vbtc.vert_eye_1e10;
        eye_stats_on_demand[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e12 =
            eye_data->ed_vbtc.vert_eye_1e12;
        eye_stats_on_demand[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e15 =
            eye_data->ed_vbtc.vert_eye_1e15;
        eye_stats_on_demand[ring][sd][pass].eye_data.ed_vbtc.vert_eye_1e17 =
            eye_data->ed_vbtc.vert_eye_1e17;

        avago_serdes_eye_data_destruct(aapl, eye_data);
        avago_serdes_eye_config_destruct(aapl, eye_config);

        memset(&dfe_state, 0, sizeof(dfe_state));
        avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
        eye_stats_on_demand[ring][sd][pass].dfe_state.dc = dfe_state.dc;
        eye_stats_on_demand[ring][sd][pass].dfe_state.lf = dfe_state.lf;
        eye_stats_on_demand[ring][sd][pass].dfe_state.hf = dfe_state.hf;
        eye_stats_on_demand[ring][sd][pass].dfe_state.bw = dfe_state.bw;
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeGAIN =
            dfe_state.dfeGAIN;
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeGAIN2 =
            dfe_state.dfeGAIN2;
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP1 =
            dfe_state.dfeTAP1;
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[0] =
            dfe_state.dfeTAP[0];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[1] =
            dfe_state.dfeTAP[1];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[2] =
            dfe_state.dfeTAP[2];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[3] =
            dfe_state.dfeTAP[3];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[4] =
            dfe_state.dfeTAP[4];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[5] =
            dfe_state.dfeTAP[5];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[6] =
            dfe_state.dfeTAP[6];
        eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[7] =
            dfe_state.dfeTAP[7];
      }
    }
  }
}

void sd_perf_stats_display_on_demand(ucli_context_t *uc, bool is_mav) {
  int pass, dev_id = 0, ring, sd, fp, ch, mac_block, n_lanes, ln;
  bf_dev_port_t dev_port;
  port_mgr_serdes_t *serdes_p;
  int max_fp = is_mav ? 65 : 33;

  for (fp = 0; fp < max_fp; fp++) {
    sd_perf_banner(uc);
    for (ch = 0; ch < 4; ch++) {
      for (pass = 0; pass < sd_num_stats; pass++) {
        port_mgr_port_t *port_p;

        dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
        dev_port |= ch;

        // see if its a valid port
        port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
        if (port_p == NULL) continue;

        port_mgr_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
        bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
        for (ln = 0; ln < n_lanes; ln++) {
          serdes_p =
              port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
          if (serdes_p == NULL) continue;

          ring = serdes_p->ring;
          sd = serdes_p->rx_sd;

          aim_printf(&uc->pvs,
                     "|%2d/%d|%2d|%d|%d|%d|%3d|",
                     fp + 1,
                     ch,
                     mac_block,
                     ch + ln,
                     dev_id,
                     ring,
                     sd);
          aim_printf(&uc->pvs,
                     "%2d|%2d|%2d|",
                     eye_stats_on_demand[ring][sd][pass].tx_eq.pre,
                     eye_stats_on_demand[ring][sd][pass].tx_eq.atten,
                     eye_stats_on_demand[ring][sd][pass].tx_eq.post);

          aim_printf(&uc->pvs,
                     " %3d mV | %3d mV | %3d mV | %3d mV | %3d mV |",
                     eye_stats_on_demand[ring][sd][pass]
                         .eye_data.ed_vbtc.vert_eye_1e06,
                     eye_stats_on_demand[ring][sd][pass]
                         .eye_data.ed_vbtc.vert_eye_1e10,
                     eye_stats_on_demand[ring][sd][pass]
                         .eye_data.ed_vbtc.vert_eye_1e12,
                     eye_stats_on_demand[ring][sd][pass]
                         .eye_data.ed_vbtc.vert_eye_1e15,
                     eye_stats_on_demand[ring][sd][pass]
                         .eye_data.ed_vbtc.vert_eye_1e17);

          aim_printf(
              &uc->pvs,
              "%3d | %2d | %2d | %2d | %2d | %2d | %5.1f | %3d | %3d | %3d | "
              "%3d | %3d | %3d | %3d | %3d\n",
              eye_stats_on_demand[ring][sd][pass].dfe_state.dc,
              eye_stats_on_demand[ring][sd][pass].dfe_state.lf,
              eye_stats_on_demand[ring][sd][pass].dfe_state.hf,
              eye_stats_on_demand[ring][sd][pass].dfe_state.bw,
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeGAIN,
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeGAIN2,
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP1,
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[0],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[1],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[2],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[3],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[4],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[5],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[6],
              eye_stats_on_demand[ring][sd][pass].dfe_state.dfeTAP[7]);
        }
      }
    }
  }

  for (fp = 0; fp < max_fp; fp++) {
    for (ch = 0; ch < 4; ch++) {
      port_mgr_port_t *port_p;

      dev_port = (bf_dev_port_t)dev_port_for_fp[fp];
      dev_port |= ch;

      // see if its a valid port
      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (port_p == NULL) continue;

      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
      for (ln = 0; ln < n_lanes; ln++) {
        int i, min_e, max_e, avg_e, total_e = 0;
        int total_deviation;
        double std_deviation;

        serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
        if (serdes_p == NULL) continue;

        ring = serdes_p->ring;
        sd = serdes_p->rx_sd;

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e06;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }

        avg_e = total_e / sd_num_stats;

        total_deviation = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e06;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / sd_num_stats);

        aim_printf(&uc->pvs,
                   "1e06: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e10;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / sd_num_stats;

        total_deviation = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e10;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / sd_num_stats);

        aim_printf(&uc->pvs,
                   "1e10: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e12;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / sd_num_stats;

        total_deviation = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e12;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / sd_num_stats);

        aim_printf(&uc->pvs,
                   "1e12: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e15;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / sd_num_stats;

        total_deviation = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e15;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / sd_num_stats);

        aim_printf(&uc->pvs,
                   "1e15: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");

        min_e = 1000;
        max_e = 0;
        total_e = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e17;
          if (val < min_e) min_e = val;
          if (val > max_e) max_e = val;
          total_e += val;
        }
        avg_e = total_e / sd_num_stats;

        total_deviation = 0;
        for (pass = 0; pass < sd_num_stats; pass++) {
          int val = eye_stats_on_demand[ring][sd][pass]
                        .eye_data.ed_vbtc.vert_eye_1e17;
          int diff = abs(val - avg_e);
          bf_dev_id_t dev = diff * diff;
          total_deviation += dev;
        }
        std_deviation = sqrt(total_deviation / sd_num_stats);

        aim_printf(&uc->pvs,
                   "1e17: |%2d/%d|%2d|%d|%d|%d|%3d| %3d mV | %3d mV | %3d mV | "
                   "+/-%5.1f mV |",
                   fp + 1,
                   ch,
                   mac_block,
                   ch + ln,
                   dev_id,
                   ring,
                   sd,
                   min_e,
                   max_e,
                   avg_e,
                   std_deviation);
        min_e /= 10;
        max_e /= 10;
        avg_e /= 10;
        for (i = 0; i < min_e; i++) aim_printf(&uc->pvs, "=");
        for (; i < avg_e; i++) aim_printf(&uc->pvs, "-");
        for (; i < max_e; i++) aim_printf(&uc->pvs, ".");
        aim_printf(&uc->pvs, "\n");
      }
    }
  }
}

void sd_all_rptr_term_float(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 1; ring < g_num_rings; ring++) {
    for (sd = 17; sd < 114; sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_FLOAT);
        aim_printf(&uc->pvs,
                   "Serdes: %d:%d:%d : RPTR lane Rx term set to FLOAT\n",
                   dev_id,
                   ring,
                   sd);
      }
    }
  }
}

void sd_all_rptr_term_avdd(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 1; ring < g_num_rings; ring++) {
    for (sd = 17; sd < 114; sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_rx_term_set(dev_id, ring, sd, BF_SDS_RX_TERM_FLOAT);
        aim_printf(&uc->pvs,
                   "Serdes: %d:%d:%d : RPTR lane Rx term set to AVDD\n",
                   dev_id,
                   ring,
                   sd);
      }
    }
  }
}

void sd_all_delay_cal(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_delay_cal(dev_id, ring, sd);
        aim_printf(&uc->pvs,
                   "Serdes: %d:%d:%d : Delay-CAL started\n",
                   dev_id,
                   ring,
                   sd);
      }
    }
  }
}

void sd_all_pi_cal(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_pi_cal(dev_id, ring, sd);
        aim_printf(
            &uc->pvs, "Serdes: %d:%d:%d : PI-CAL started\n", dev_id, ring, sd);
      }
    }
  }
}

void sd_run_sweep_ctle(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd) {
  int pre, attn, post, dfe_running;
  int max_dfe_wait = 120;
  int cur_val;
  int lf;

  diag_fd = fopen("sweep-output.txt", "a");
  if (diag_fd == NULL) to_file = 0;

  aim_printf(&uc->pvs, "CTLE Sweep: ring=%d : sd=%d\n\n", ring, sd);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd, "CTLE Sweep: ring=%d : sd=%d\n\n", ring, sd);

  for (attn = 0; attn < 1; attn++) {
    for (post = 0; post < 3; post += 2) {
      for (pre = 4; pre < 9; pre += 2) {
        for (lf = 4; lf < 9; lf++) {
          if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

          // set tx-eq
          // port_mgr_av_sd_set_tx_eq(dev_id, ring, sd, pre, attn, post);
          sd_tx_eq_all(dev_id, pre, attn, post);

          // set DC=80
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2250);

          // set LF
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2100 | lf);

          // Launch iCal with fixed DC and fixed LF
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x181);

          max_dfe_wait = 120;
          do {
            dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
            if (dfe_running) {
              bf_sys_usleep(1000000);
            }
          } while (dfe_running && max_dfe_wait--);

          // delay_cal (aka pi_cal)
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2F, 0x101);

          // pCal letting DC adapt
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x2);

          max_dfe_wait = 120;
          do {
            dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
            if (dfe_running) {
              bf_sys_usleep(1000000);
            }
          } while (dfe_running && max_dfe_wait--);

          cur_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2F, 0xF0);
          // start delay_cal from current phase interpolator position
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2F, 0x3001 | cur_val);
          bf_sys_usleep(100000);
          {
            Avago_serdes_dfe_state_t dfe_state;
            Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
            int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

            memset(&dfe_state, 0, sizeof(dfe_state));
            avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
            aim_printf(
                &uc->pvs,
                "%d:%d:%3d : Pre=%d : Atn=%d : Post=%d : DC= %3d : LF= %2d : "
                "HF= %2d : BW= %2d : Gain= %d : Gain2= %d : Taps: %.1f : %3d : "
                "%3d : %3d : %3d : %3d : %3d : %3d : %3d\n",
                dev_id,
                ring,
                sd,
                pre,
                attn,
                post,
                dfe_state.dc,
                dfe_state.lf,
                dfe_state.hf,
                dfe_state.bw,
                dfe_state.dfeGAIN,
                dfe_state.dfeGAIN2,
                dfe_state.dfeTAP1,
                dfe_state.dfeTAP[0],
                dfe_state.dfeTAP[1],
                dfe_state.dfeTAP[2],
                dfe_state.dfeTAP[3],
                dfe_state.dfeTAP[4],
                dfe_state.dfeTAP[5],
                dfe_state.dfeTAP[6],
                dfe_state.dfeTAP[7]);
          }
          extern int port_mgr_av_sd_dump_vbtc(
              bf_dev_id_t dev_id, int ring, int sd);
          port_mgr_av_sd_dump_vbtc(dev_id, ring, sd);
          {
            int e_cnt_1s, e_cnt_10s, e_cnt_100s;

            // clear errors
            aim_printf(&uc->pvs, "Collect errors ...");
            fflush(stdout);
            port_mgr_av_sd_set_rx_cmp_mode(
                dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
            port_mgr_av_sd_get_error_count(dev_id, ring, sd);
            bf_sys_usleep(1 * 1000000);
            e_cnt_1s = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
            aim_printf(&uc->pvs, "\nErrors   1s: %d\n", e_cnt_1s);
            if (e_cnt_1s != -1) {
              bf_sys_usleep(10 * 1000000);
              e_cnt_10s = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
              aim_printf(&uc->pvs, "Errors  10s: %d\n", e_cnt_10s);
              if (e_cnt_10s != -1) {
                bf_sys_usleep(100 * 1000000);
                e_cnt_100s = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
                aim_printf(&uc->pvs, "Errors 100s: %d\n", e_cnt_100s);
              }
            }
          }
          aim_printf(
              &uc->pvs,
              "\n------------------------------------------------------\n\n");
        }
      }
    }
  }
}

void sd_all_bbgain(ucli_context_t *uc,
                   bf_dev_id_t dev_id,
                   uint32_t tx_pll_setting,
                   uint32_t rx_pll_setting) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);
  aim_printf(&uc->pvs,
             "Serdes: %d:-:- : PLL BBGain set : Tx PLL=%x : Rx PLL=%x\n",
             dev_id,
             tx_pll_setting,
             rx_pll_setting);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_pll_bbgain_set_these(
            dev_id, ring, sd, tx_pll_setting, rx_pll_setting);
      }
    }
  }
}

void sd_all_adaptive_pcal(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_adaptive(dev_id, ring, sd);
        aim_printf(&uc->pvs,
                   "Serdes: %d:%d:%d : Adaptive PCAL started\n",
                   dev_id,
                   ring,
                   sd);
      }
    }
  }
}

void sd_all_pcal(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_pcal(dev_id, ring, sd);
        aim_printf(
            &uc->pvs, "Serdes: %d:%d:%d : PCAL started\n", dev_id, ring, sd);
      }
    }
  }
}

void sd_all_dfe_set(bf_dev_id_t dev_id,
                    uint32_t dfe_ctrl,
                    uint32_t hf_val,
                    uint32_t lf_val,
                    uint32_t dc_val) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_serdes_tof_dfe_cfg_set(
            dev_id, ring, sd, dfe_ctrl, hf_val, lf_val, dc_val);
      }
    }
  }
}

void sd_all_ical(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd += 4) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        aim_printf(
            &uc->pvs, "Serdes: %d:%d:%d : ICAL started\n", dev_id, ring, sd);
      }
    }
  }
  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 2; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd += 4) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        aim_printf(
            &uc->pvs, "Serdes: %d:%d:%d : ICAL started\n", dev_id, ring, sd);
      }
    }
  }
  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 3; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd += 4) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        aim_printf(
            &uc->pvs, "Serdes: %d:%d:%d : ICAL started\n", dev_id, ring, sd);
      }
    }
  }
  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 4; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd += 4) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        aim_printf(
            &uc->pvs, "Serdes: %d:%d:%d : ICAL started\n", dev_id, ring, sd);
      }
    }
  }
}

void sd_tx_eq_all(bf_dev_id_t dev_id, int pre, int attn, int post) {
  int ring, sd;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_set_tx_eq(dev_id, ring, sd, pre, attn, post);
      }
    }
  }
}

typedef struct victim_t {
  int ring;
  int sd;
  int pre_cnt;
  int post_cnt;
} victim_t;

#define VICTIM_RADIUS 4
#define XT_GRPS (152 / (2 * VICTIM_RADIUS))

typedef struct xt_grp_t {
  int aggressor_ring;
  int aggressor_sd;
  victim_t victim[2 * VICTIM_RADIUS];
} xt_grp_t;

xt_grp_t xt_test[XT_GRPS] = {{0}};

#define XT_PASSES 7
xt_grp_t xt_pass[XT_PASSES][XT_GRPS] = {{{0}}};

typedef struct xt_t {
  uint32_t pre_cnt[2][156];  // [ring][sd]
  uint32_t post_cnt[2][156];
} xt_t;

#define AGGRESSOR_STRIDE 4

xt_t xt[XT_PASSES][AGGRESSOR_STRIDE] = {{{{{0}}}}};

void print_xt_test_banner(ucli_context_t *uc) {
  aim_printf(&uc->pvs, "+-+---+------------+------------+\n");
  aim_printf(&uc->pvs, "|r|   |            |            |\n");
  aim_printf(&uc->pvs, "|i|   |            |            |\n");
  aim_printf(&uc->pvs, "|n| s | Pre        | Post       |\n");
  aim_printf(&uc->pvs, "|g| d | Errors     | Errors     |\n");
  aim_printf(&uc->pvs, "+-+---+------------+------------+\n");
}

void sd_xtalk_test(ucli_context_t *uc, bf_dev_id_t dev_id) {
  int ring, sd, ring_start, ring_end, pass, agg_ofs;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (pass = 0; pass < XT_PASSES; pass++) {
    for (agg_ofs = 0; agg_ofs < AGGRESSOR_STRIDE; agg_ofs++) {
      for (ring = 0; ring < 2; ring++) {
        if (ring == 0) {
          ring_start = 11;
        } else {
          ring_start = 1;
        }
        ring_end = port_mgr_num_sbus_nodes_get(dev_id, ring);

        for (sd = ring_start; sd < ring_end; sd++) {
          if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
            // set PRBS compare mode
            port_mgr_av_sd_set_rx_cmp_mode(
                dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
            // clear error count
            port_mgr_av_sd_get_error_count(dev_id, ring, sd);
          }
        }
        // let errors accuulate for a few seconds
        sleep(5);

        // collect pre test counts
        for (sd = ring_start; sd < ring_end; sd++) {
          if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
            xt[pass][agg_ofs].pre_cnt[ring][sd] =
                port_mgr_av_sd_get_error_count(dev_id, ring, sd);
          }
        }

        // run DFE on aggressor channels
        aim_printf(&uc->pvs, "Aggressors: ring=%d\n", ring);
        for (sd = ring_start + agg_ofs; sd < ring_end; sd += AGGRESSOR_STRIDE) {
          if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
            port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
            aim_printf(&uc->pvs, "%d, ", sd);
          }
        }
        aim_printf(&uc->pvs, "\n");

        // errors usually show up right after DFE starts
        sleep(3);

        // collect post test counts
        for (sd = ring_start; sd < ring_end; sd++) {
          if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
            bool is_aggressor = false;
            int agg_sd;

            for (agg_sd = ring_start + agg_ofs; agg_sd < ring_end;
                 agg_sd += AGGRESSOR_STRIDE) {
              if (sd == agg_sd) {
                is_aggressor = true;
                break;
              }
            }
            if (is_aggressor) continue;  // skip ones we just ran DFE on

            xt[pass][agg_ofs].post_cnt[ring][sd] =
                port_mgr_av_sd_get_error_count(dev_id, ring, sd);
            if (xt[pass][agg_ofs].pre_cnt[ring][sd] == 0) {
              if (xt[pass][agg_ofs].post_cnt[ring][sd] != 0) {
                aim_printf(&uc->pvs,
                           "%d.%d: %d\n",
                           ring,
                           sd,
                           xt[pass][agg_ofs].post_cnt[ring][sd]);
              }
            }
          }
        }
        // let DFE complete
        sleep(12);

        // reset cmp mode to PRBS on aggessor channels
        for (sd = ring_start + agg_ofs; sd < ring_end; sd += AGGRESSOR_STRIDE) {
          if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
            port_mgr_av_sd_set_rx_cmp_mode(
                dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
          }
        }
        aim_printf(
            &uc->pvs,
            "\n*** End Pass %d (of %d) aggressor_ofs=%d (of %d) : ring=%d\n",
            pass + 1,
            XT_PASSES,
            agg_ofs,
            AGGRESSOR_STRIDE,
            ring);
      }
    }
  }

  aim_printf(
      &uc->pvs,
      "+-----++-----+--------+--------+--------+--------+--------+--------+----"
      "----+\n");
  aim_printf(
      &uc->pvs,
      "|Aggr ||Vict |  Failing nodes error counts                              "
      "    |\n");
  aim_printf(
      &uc->pvs,
      "+-----++-----+--------+--------+--------+--------+--------+--------+----"
      "----+\n");
  aim_printf(
      &uc->pvs,
      "|r|   ||r|   |        |        |        |        |        |        |    "
      "    |\n");
  aim_printf(
      &uc->pvs,
      "|i|   ||i|   |        |        |        |        |        |        |    "
      "    |\n");
  aim_printf(
      &uc->pvs,
      "|n| s ||n| s |        |        |        |        |        |        |    "
      "    |\n");
  aim_printf(
      &uc->pvs,
      "|g| d ||g| d |        |        |        |        |        |        |    "
      "    |\n");
  aim_printf(
      &uc->pvs,
      "+-----++-----+--------+--------+--------+--------+--------+--------+----"
      "----+\n");
  // analyze the passes

  // detailed summary

  for (ring = 0; ring < 2; ring++) {
    int vic_sd, column;

    if (ring == 0) {
      ring_start = 11;
    } else {
      ring_start = 1;
    }
    ring_end = port_mgr_num_sbus_nodes_get(dev_id, ring);
    for (sd = ring_start; sd < ring_end; sd++) {
      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

      // find the column for which this sd is the aggressor
      column = (sd - ring_start) % AGGRESSOR_STRIDE;
      // for each pass, check +/-2 around the aggressor as possible victims
      for (vic_sd = sd - 2; vic_sd <= sd + 2; vic_sd++) {
        bool failed = false;

        // filter out noise
        if (vic_sd < ring_start) continue;
        if (vic_sd == sd) continue;
        if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, vic_sd)) continue;
        // go thru all passes for this vic_sd looking for failures
        for (pass = 0; pass < XT_PASSES; pass++) {
          if (xt[pass][column].pre_cnt[ring][vic_sd] != 0) continue;
          if (xt[pass][column].post_cnt[ring][vic_sd] == 0) continue;
          failed = true;
          break;
        }
        if (!failed) continue;
        aim_printf(&uc->pvs, "|%d|%3d||%d|%3d|", ring, sd, ring, vic_sd);
        for (pass = 0; pass < XT_PASSES; pass++) {
          if (xt[pass][column].pre_cnt[ring][vic_sd] != 0) {
            aim_printf(
                &uc->pvs, "%7d*|", xt[pass][column].post_cnt[ring][vic_sd]);
          } else if (xt[pass][column].post_cnt[ring][vic_sd] == 0) {
            aim_printf(&uc->pvs, "        |");
          } else {
            aim_printf(
                &uc->pvs, "%7d |", xt[pass][column].post_cnt[ring][vic_sd]);
          }
        }
        aim_printf(&uc->pvs, "\n");
      }
    }
  }

  // un-cluttered summary

  for (ring = 0; ring < 2; ring++) {
    int vic_sd, column;

    if (ring == 0) {
      ring_start = 11;
    } else {
      ring_start = 1;
    }
    ring_end = port_mgr_num_sbus_nodes_get(dev_id, ring);
    for (sd = ring_start; sd < ring_end; sd++) {
      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

      // find the column for which this sd is the aggressor
      column = (sd - ring_start) % AGGRESSOR_STRIDE;
      // for each pass, check +/-2 around the aggressor as possible victims
      for (vic_sd = sd - 2; vic_sd <= sd + 2; vic_sd++) {
        bool failed = false;

        // filter out noise
        if (vic_sd < ring_start) continue;
        if (vic_sd == sd) continue;
        if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, vic_sd)) continue;
        // go thru all passes for this vic_sd looking for failures
        for (pass = 0; pass < XT_PASSES; pass++) {
          if (xt[pass][column].pre_cnt[ring][vic_sd] != 0) continue;
          if (xt[pass][column].post_cnt[ring][vic_sd] == 0) continue;
          failed = true;
          break;
        }
        if (!failed) continue;
        aim_printf(&uc->pvs, "|%d|%3d||%d|%3d|", ring, sd, ring, vic_sd);
        for (pass = 0; pass < XT_PASSES; pass++) {
          if (xt[pass][column].pre_cnt[ring][vic_sd] != 0) continue;
          if (xt[pass][column].post_cnt[ring][vic_sd] == 0) continue;
          aim_printf(&uc->pvs, "%8d|", xt[pass][column].post_cnt[ring][vic_sd]);
        }
        aim_printf(&uc->pvs, "\n");
      }
    }
  }
}

void sd_cmp_mode_prbs(bf_dev_id_t dev_id, int ring, int sd) {
  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }

  if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
    port_mgr_av_sd_set_rx_cmp_mode(
        dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
  }
}

void sd_cmp_mode_prbs31_all(bf_dev_id_t dev_id) {
  int ring, sd;

  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < g_num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      sd_cmp_mode_prbs(dev_id, ring, sd);
    }
  }
}

typedef enum sd_type_t {
  SD_OTHER = 0,
  SD_AGGRESSOR,
  SD_VICTIM,
} sd_type_t;

// default to "other" (0)
sd_type_t sd_type[2][256] = {{0}};

void sd_diag_aggressor_add(bf_dev_id_t dev_id, int ring, int sd) {
  if ((uint)ring >= sizeof(sd_type) / sizeof(sd_type[0])) {
    port_mgr_log_warn("Unexpected value: ring = %d", ring);
    return;
  }

  if ((uint)sd >= sizeof(sd_type[0]) / sizeof(sd_type[0][0])) {
    port_mgr_log_warn("Unexpected value: sd = %d", ring);
    return;
  }

  sd_type[ring][sd] = SD_AGGRESSOR;
  (void)dev_id;
}

void sd_diag_aggressor_del(bf_dev_id_t dev_id, int ring, int sd) {
  if ((uint)ring >= sizeof(sd_type) / sizeof(sd_type[0])) {
    port_mgr_log_warn("Unexpected value: ring = %d", ring);
    return;
  }

  if ((uint)sd >= sizeof(sd_type[0]) / sizeof(sd_type[0][0])) {
    port_mgr_log_warn("Unexpected value: sd = %d", ring);
    return;
  }

  sd_type[ring][sd] = SD_OTHER;
  (void)dev_id;
}

void sd_diag_aggressor_on(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_AGGRESSOR) {
          sd_init_prbs31_25g(dev_id, ring, sd, 0);
        }
      }
    }
  }
}

void sd_diag_aggressor_off(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_AGGRESSOR) {
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x01, 0x0);
        }
      }
    }
  }
}

void sd_diag_aggressor_cal(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_AGGRESSOR) {
          port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        }
      }
    }
  }
}

void sd_diag_aggressor_show(ucli_context_t *uc) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  sd_print_prbs_banner(uc);
  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_AGGRESSOR) {
          sd_dump_this_prbs(uc, dev_id, ring, sd);
        }
      }
    }
  }
}

void sd_diag_victim_add(bf_dev_id_t dev_id, int ring, int sd) {
  if ((uint)ring >= sizeof(sd_type) / sizeof(sd_type[0])) {
    port_mgr_log_warn("Unexpected value: ring = %d", ring);
    return;
  }

  if ((uint)sd >= sizeof(sd_type[0]) / sizeof(sd_type[0][0])) {
    port_mgr_log_warn("Unexpected value: sd = %d", ring);
    return;
  }

  sd_type[ring][sd] = SD_VICTIM;
  (void)dev_id;
}

void sd_diag_victim_del(bf_dev_id_t dev_id, int ring, int sd) {
  if ((uint)ring >= sizeof(sd_type) / sizeof(sd_type[0])) {
    port_mgr_log_warn("Unexpected value: ring = %d", ring);
    return;
  }

  if ((uint)sd >= sizeof(sd_type[0]) / sizeof(sd_type[0][0])) {
    port_mgr_log_warn("Unexpected value: sd = %d", ring);
    return;
  }

  sd_type[ring][sd] = SD_OTHER;
  (void)dev_id;
}

void sd_diag_victim_on(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_VICTIM) {
          sd_init_prbs31_25g(dev_id, ring, sd, 0);
        }
      }
    }
  }
}

void sd_diag_victim_off(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_VICTIM) {
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x01, 0x0);
        }
      }
    }
  }
}

void sd_diag_victim_cal(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_VICTIM) {
          port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        }
      }
    }
  }
}

void sd_diag_victim_show(ucli_context_t *uc) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  sd_print_prbs_banner(uc);
  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_VICTIM) {
          sd_dump_this_prbs(uc, dev_id, ring, sd);
        }
      }
    }
  }
}

void sd_diag_other_on(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_OTHER) {
          sd_init_prbs31_25g(dev_id, ring, sd, 0);
        }
      }
    }
  }
}

void sd_diag_other_off(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_OTHER) {
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x01, 0x0);
        }
      }
    }
  }
}

void sd_diag_other_cal(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_OTHER) {
          port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        }
      }
    }
  }
}

void sd_diag_other_show(ucli_context_t *uc) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  sd_print_prbs_banner(uc);
  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        if (sd_type[ring][sd] == SD_OTHER) {
          sd_dump_this_prbs(uc, dev_id, ring, sd);
        }
      }
    }
  }
}

void sd_diag_all_on(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        sd_init_prbs31_25g(dev_id, ring, sd, 0);
      }
    }
  }
}

void sd_diag_off(bf_dev_id_t dev_id, int ring, int sd) {
  if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x01, 0x0);
  }
}

void sd_diag_all_off(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      sd_diag_off(dev_id, ring, sd);
    }
  }
}

void sd_diag_all_cal(void) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
      }
    }
  }
}

void sd_diag_all_show(ucli_context_t *uc) {
  int ring, sd;
  bf_dev_id_t dev_id = 0;
  int num_rings = port_mgr_num_sbus_rings_get(dev_id);

  sd_print_prbs_banner(uc);
  for (ring = 0; ring < num_rings; ring++) {
    for (sd = 1; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
        sd_dump_this_prbs(uc, dev_id, ring, sd);
      }
    }
  }
}

int sd_init_w_output_en_0(bf_dev_id_t dev_id, int ring, int sd) {
  int rc;

  rc = port_mgr_av_sd_init(dev_id,
                           ring,
                           sd,
                           FALSE /*no reset*/,
                           AVAGO_PRBS31_ELB /*init_mode*/,
                           165 /*divider*/,
                           40 /*data_width*/,
                           FALSE /*phase_cal*/,
                           FALSE /*output_en*/);
  return rc;
}

void sd_init_aapl(bf_dev_id_t dev_id, int tcp_mode) {
  Aapl_t *aapl;
  int reset = 1;

  if (dev_id >= g_num_devices) {
    g_num_devices = dev_id + 1;
  }
  g_num_rings = port_mgr_num_sbus_rings_get(dev_id);

  aapl = port_mgr_av_sd_get_aapl(dev_id);
  if (aapl == NULL) {
    port_mgr_av_sd_set_aapl(dev_id);
    aapl = port_mgr_av_sd_get_aapl(dev_id);

    aapl->debug = 0;
    if (tcp_mode) {
      // aapl->communication_method = AVAGO_AACS_SBUS;
      aapl->communication_method = AVAGO_SBUS;
      aapl_connect(aapl, "10.201.201.49" /*<ip_addr>*/, 90);
    } else {
      // aapl->communication_method = AVAGO_USER_SUPPLIED_SBUS_DIRECT;
      aapl->communication_method = AVAGO_SBUS;
    }
    aapl_get_ip_info(aapl, reset);
    // aapl_print_struct( aapl, 1, 0xffff, 0 );
  }
}

void sd_init_aapl_and_load_firmware(ucli_context_t *uc,
                                    bf_dev_id_t dev_id,
                                    int tcp_mode) {
  Aapl_t *aapl;
  int ring, sd, err;
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  sd_init_aapl(dev_id, tcp_mode);

  aapl = port_mgr_av_sd_get_aapl(dev_id);

  /* load spico firmware */
  for (ring = 0; ring < g_num_rings; ring++) {
    port_mgr_av_sd_load_firmware(
        dev_id, ring, sbm, dev_p->sbus_master_fw_ver, dev_p->sbus_master_fw);
    port_mgr_av_sd_load_firmware(
        dev_id, ring, AVAGO_BROADCAST, dev_p->serdes_fw_ver, dev_p->serdes_fw);
  }
  for (ring = 0; ring < g_num_rings; ring++) {
    /* determine which nodes are really serdes */
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

      if (!aapl_check_ip_type(
              aapl, sbus_addr, __func__, __LINE__, FALSE, 1, AVAGO_SERDES))
        continue;

      if ((aapl->process_id[0] == AVAGO_TSMC_28) ||
          (aapl->process_id[0] == AVAGO_TSMC_16)) {
        port_mgr_serdes_set_is_serdes(dev_id, ring, sd);

        err = sd_init_w_output_en_0(dev_id, ring, sd);
        if (err) {
          aim_printf(&uc->pvs,
                     "Serdes: sbus_addr=%d : Error : %d : from serdes_init\n",
                     sbus_addr,
                     err);
        } else {
          aim_printf(&uc->pvs,
                     "Serdes: sbus_addr=%d : Serdes node initialized\n",
                     sbus_addr);
        }
      }
    }
  }
  aapl_print_struct(aapl, 1, 0xffff, 0);
}

void sd_get_debug_stats_this(
    bf_dev_id_t dev_id, int ring, int sd, uint32_t *errors, uint32_t *eye) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t data;
  uint32_t eye_metric;
  if (!errors) return;
  if (!eye) return;
  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) return;

  data = avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);
  eye_metric = avago_serdes_eye_get_simple_metric(aapl, sbus_addr);

  *errors = data;
  *eye = eye_metric;

  return;
}

void sd_dump_this(ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd) {
  Avago_serdes_tx_eq_t tx_eq;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  BOOL tx_rdy, rx_rdy, tx_en, rx_en, tx_output_en, tx_inv, rx_inv;
  Avago_serdes_tx_data_sel_t tx_data_sel;
  uint32_t data;
  int tx_data_width, rx_data_width, dfe_status;
  int rx_cmp_sel, rx_cmp_mode, sig_ok, los, term, sig_ok_thresh, sig_ok_en;
  int sig_live_ok, elec_idle, flock;
  uint32_t eye_metric;

  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) return;

  aim_printf(&uc->pvs, "%d|%d|%3d|", dev_id, ring, sd);

  avago_serdes_get_tx_rx_ready(aapl, sbus_addr, &tx_rdy, &rx_rdy);
  avago_serdes_get_tx_rx_width(aapl, sbus_addr, &tx_data_width, &rx_data_width);

  data = avago_serdes_mem_rd(aapl, sbus_addr, AVAGO_LSB, AVSD_LSB_SERDES_RDY);
  tx_en = data & 1;
  rx_en = (data >> 1) & 1;
  tx_output_en = avago_serdes_get_tx_output_enable(aapl, sbus_addr);
  tx_inv = avago_serdes_get_tx_invert(aapl, sbus_addr);
  aim_printf(&uc->pvs,
             "%s|%s|%s|%s|",
             tx_en ? "1" : "-",
             tx_rdy ? "1" : "-",
             tx_output_en ? "1" : "-",
             tx_inv ? "1" : "-");
  tx_data_sel = avago_serdes_get_tx_data_sel(aapl, sbus_addr);
  aim_printf(
      &uc->pvs, "%8s|%2d|", aapl_data_sel_to_str(tx_data_sel), tx_data_width);

  /* get tx_eq parms of sd */
  avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
  aim_printf(&uc->pvs,
             "%3d|%3d|%3d|%3d|",
             tx_eq.pre,
             tx_eq.atten,
             tx_eq.post,
             tx_eq.slew);

  rx_inv = avago_serdes_get_rx_invert(aapl, sbus_addr);
  rx_cmp_sel = port_mgr_av_sd_get_rx_cmp_sel(dev_id, ring, sd);
  rx_cmp_mode = port_mgr_av_sd_get_rx_cmp_mode(dev_id, ring, sd);
  term = port_mgr_av_sd_rx_term_get(dev_id, ring, sd);
  sig_ok_en = port_mgr_av_sd_signal_ok_en_get(dev_id, ring, sd);
  los = port_mgr_av_sd_los_get(dev_id, ring, sd);
  sig_ok = port_mgr_av_sd_signal_ok_get(dev_id, ring, sd);
  sig_live_ok = port_mgr_av_sd_signal_ok_live_get(dev_id, ring, sd);
  elec_idle = port_mgr_av_sd_elec_idle_get(dev_id, ring, sd);
  flock = port_mgr_av_sd_frequency_lock_get(dev_id, ring, sd);
  sig_ok_thresh = port_mgr_av_sd_signal_ok_thresh_get(dev_id, ring, sd);
  data = avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);
  eye_metric = avago_serdes_eye_get_simple_metric(aapl, sbus_addr);
  dfe_status = avago_serdes_get_dfe_status(aapl, sbus_addr);

  aim_printf(&uc->pvs,
             "%s|%s|%s|%s|%2d|%s|%s|%s|%s|%s|%5s|%11s|%8s|%8x|%2d| %4d | %04x",
             rx_en ? "1" : "-",
             rx_rdy ? "1" : "-",
             rx_inv ? "1" : "-",
             sig_ok_en ? "1" : "-",
             sig_ok_thresh,
             sig_ok ? "1" : "-",
             los ? "1" : "-",
             sig_live_ok ? "1" : "-",
             elec_idle ? "1" : "-",
             flock ? "1" : "-",
             aapl_term_to_str(term),
             aapl_cmp_mode_to_str(rx_cmp_mode),
             aapl_cmp_data_to_str(rx_cmp_sel),
             data,
             rx_data_width,
             eye_metric,
             dfe_status);
  aim_printf(&uc->pvs, "\n");
}

void sd_print_banner(ucli_context_t *uc) {
  if (uc == NULL) return;

  aim_printf(&uc->pvs,
             "-------+-------------------+---------------+---------------------"
             "--------"
             "-------------------------------------+-----+\n");
  aim_printf(&uc->pvs,
             " addr  |   TX              |     Tx Eq     |    RX               "
             "                                                   |\n");
  aim_printf(&uc->pvs,
             "-+-+---+-+-+-+-+--------+--+---+---+---+---+-+-+-+-+--+-+-+-+-+-+"
             "-----+--"
             "---------+--------+--------+--+------+-----+\n");
  aim_printf(&uc->pvs,
             " | |   | | |o| |        |w |   | a |   |   | | | |o|t |s| | | "
             "|f|     |  "
             "         |        |        |w |      |\n");
  aim_printf(&uc->pvs,
             "c|r|n  | | |u| |        |i |   | t | p | s | | | |k|h |i| "
             "|l|e|l|     |  "
             "         |        |        |i |      |\n");
  aim_printf(
      &uc->pvs,
      "h|i|o  | |r|t|i|  data  |d | p | t | o | l | |r|i| |r |g|L|i|l|o|Term.| "
      "Compare   | Compare| Compare|d |      |\n");
  aim_printf(
      &uc->pvs,
      "i|n|d  |e|d|e|n|  sel   |t | r | e | s | e |e|d|n|e|s |o|O|v|e|c|     | "
      "Mode      | Data   | Errors |t |      |\n");
  aim_printf(&uc->pvs,
             "p|g|e  |n|y|n|v|        |h | e | n | t | w |n|y|v|n|h "
             "|k|S|e|c|k|     |  "
             "         |        |        |h | eye  | dfe\n");
  aim_printf(&uc->pvs,
             "-+-+---+-+-+-+-+--------+--+---+---+---+---+-+-+-+-+--+-+-+-+-+-+"
             "-----+--"
             "---------+--------+--------+--+------+-----+\n");
}

void sd_dump_this_sd(ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd) {
  sd_print_banner(uc);
  sd_dump_this(uc, dev_id, ring, sd);
}

void sd_dump_this_sd_range(ucli_context_t *uc,
                           bf_dev_id_t dev_id,
                           int ring,
                           int starting_sd,
                           int num_sd) {
  int end_sd =
      (starting_sd + num_sd) > port_mgr_num_sbus_nodes_get(dev_id, ring)
          ? port_mgr_num_sbus_nodes_get(dev_id, ring)
          : (starting_sd + num_sd);
  int sd, n_dumped = 0;

  for (sd = starting_sd; sd < end_sd; sd++) {
    if ((n_dumped % 32) == 0) {
      sd_print_banner(uc);
    }
    if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

    sd_dump_this(uc, dev_id, ring, sd);
    n_dumped++;
  }
}

void sd_dump(ucli_context_t *uc) {
  bf_dev_id_t dev_id, ring, sd;

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);

  for (ring = 0; ring < port_mgr_num_sbus_rings_get(dev_id); ring++) {
    for (sd = 0; sd < port_mgr_num_sbus_nodes_get(dev_id, ring); sd++) {
      if ((sd % 32) == 0) {
        sd_print_banner(uc);
      }
      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

      sd_dump_this(uc, dev_id, ring, sd);
    }
  }
}

void sd_dfe_data_get(bf_dev_id_t dev_id,
                     int ring,
                     int sd,
                     uint32_t *dc,
                     uint32_t *lf,
                     uint32_t *hf,
                     uint32_t *bw,
                     uint32_t *gain,
                     uint32_t tap[16]) {
  Avago_serdes_dfe_state_t dfe_state;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int i, sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  memset(&dfe_state, 0, sizeof(dfe_state));
  avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);

  *dc = dfe_state.dc;
  *lf = dfe_state.lf;
  *hf = dfe_state.hf;
  *bw = dfe_state.bw;
  *gain = dfe_state.dfeGAIN;
  tap[0] = dfe_state.dfeTAP1;
  for (i = 0; i < 8; i++) {
    tap[i + 1] = dfe_state.dfeTAP[i];
  }
}

void sd_eye_data_get(Aapl_t *aapl,
                     uint32_t sbus_addr,
                     bool *passed,
                     int *width,
                     int *h_1e03,
                     int *h_1e06,
                     int *h_1e10,
                     int *h_1e12,
                     int *h_1e15,
                     int *h_1e17) {
  Avago_serdes_eye_config_t *eye_config;
  Avago_serdes_eye_data_t *eye_data;

  eye_config = avago_serdes_eye_config_construct(aapl);
  eye_data = avago_serdes_eye_data_construct(aapl);

  eye_config->ec_eye_type = AVAGO_EYE_SIZE;
  eye_config->ec_x_resolution = 1024;

  /* get full resolution available */
  if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
    *passed = false;
  } else {
    *passed = true;
  }
  *width = eye_data->ed_hbtc.horz_eye_1e06;
  *h_1e03 = eye_data->ed_vbtc.vert_eye_1e03;
  ;
  *h_1e06 = eye_data->ed_vbtc.vert_eye_1e06;
  *h_1e10 = eye_data->ed_vbtc.vert_eye_1e10;
  *h_1e12 = eye_data->ed_vbtc.vert_eye_1e12;
  *h_1e15 = eye_data->ed_vbtc.vert_eye_1e15;
  *h_1e17 = eye_data->ed_vbtc.vert_eye_1e17;

  avago_serdes_eye_data_destruct(aapl, eye_data);
  avago_serdes_eye_config_destruct(aapl, eye_config);
}

void sd_print_prbs_banner(ucli_context_t *uc) {
  aim_printf(
      &uc->pvs,
      "-------+---------------+---------------+-----+---+-----+-----+-----+----"
      "-+-----+-----+-----+---+---+---+---+---+---+---+---+---+---+---+---+---+"
      "\n");
  aim_printf(
      &uc->pvs,
      " addr  |     Tx Eq     |               |     |   |     |     |     |    "
      " |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   "
      "|\n");
  aim_printf(
      &uc->pvs,
      "-+-+---+---+---+---+---+--------+------+     |   |     |     |     |    "
      " |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   "
      "|\n");
  aim_printf(
      &uc->pvs,
      " | |   |   | a |   |   |        |      |     |   |     |     |     |    "
      " |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   "
      "|\n");
  aim_printf(
      &uc->pvs,
      "c|r|   |   | t | p | s |        |      |     |   |     |     |     |    "
      " |     |     |     |   |   |   |   | g |   |   |   |   |   |   |   |   "
      "|\n");
  aim_printf(
      &uc->pvs,
      "h|i|   | p | t | o | l | Compare|      |     |   |     |     |     |    "
      " |     |     |     |   |   |   |   | a |   |   |   |   |   |   |   |   "
      "|\n");
  aim_printf(
      &uc->pvs,
      "i|n| s | r | e | s | e | Errors |quick |     |eye|     |     |     |    "
      " |     |     |     |   |   |   |   | i |   |   |   |   |   |   |   |   "
      "|\n");
  aim_printf(
      &uc->pvs,
      "p|g| d | e | n | t | w |        | eye  | dfe | ok|width| mean| 1e06| "
      "1e10| 1e12| 1e15| 1e17|dc |lf |hf |bw | n |   |   |   |   |   |   |   | "
      "  |\n");
  aim_printf(
      &uc->pvs,
      "-+-+---+---+---+---+---+--------+------+-----+---+-----+-----+-----+----"
      "-+------+----+-----+---+---+---+---+---+---+---+---+---+---+---+---+---+"
      "\n");

  if (to_file && (diag_fd != NULL)) {
    fprintf(diag_fd,
            "-------+---------------+---------------+-----+---+-----+-----+----"
            "-+-----+-----+-----+-----+---+---+---+---+---+---+---+---+---+---+"
            "---+---+---+\n");
    fprintf(diag_fd,
            " addr  |     Tx Eq     |               |     |   |     |     |    "
            " |     |     |     |     |   |   |   |   |   |   |   |   |   |   "
            "|   |   |   |\n");
    fprintf(diag_fd,
            "-+-+---+---+---+---+---+--------+------+     |   |     |     |    "
            " |     |     |     |     |   |   |   |   |   |   |   |   |   |   "
            "|   |   |   |\n");
    fprintf(diag_fd,
            " | |   |   | a |   |   |        |      |     |   |     |     |    "
            " |     |     |     |     |   |   |   |   |   |   |   |   |   |   "
            "|   |   |   |\n");
    fprintf(diag_fd,
            "c|r|   |   | t | p | s |        |      |     |   |     |     |    "
            " |     |     |     |     |   |   |   |   | g |   |   |   |   |   "
            "|   |   |   |\n");
    fprintf(diag_fd,
            "h|i|   | p | t | o | l | Compare|      |     |   |     |     |    "
            " |     |     |     |     |   |   |   |   | a |   |   |   |   |   "
            "|   |   |   |\n");
    fprintf(diag_fd,
            "i|n| s | r | e | s | e | Errors |quick |     |eye|     |     |    "
            " |     |     |     |     |   |   |   |   | i |   |   |   |   |   "
            "|   |   |   |\n");
    fprintf(diag_fd,
            "p|g| d | e | n | t | w |        | eye  | dfe | ok|width| mean| "
            "1e06| 1e10| 1e12| 1e15| 1e17|dc |lf |hf |bw | n |   |   |   |   | "
            "  |   |   |   |\n");
    fprintf(diag_fd,
            "-+-+---+---+---+---+---+--------+------+-----+---+-----+-----+----"
            "-+-----+------+----+-----+---+---+---+---+---+---+---+---+---+---+"
            "---+---+---+\n");
  }
}

void sd_dump_this_prbs(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd) {
  Avago_serdes_tx_eq_t tx_eq;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t data;
  int dfe_status;
  uint32_t eye_metric;
  bool passed;
  int width;
  int h_1e03;
  int h_1e06;
  int h_1e10;
  int h_1e12;
  int h_1e15;
  int h_1e17;
  uint32_t dc;
  uint32_t lf;
  uint32_t hf;
  uint32_t bw;
  uint32_t gain;
  uint32_t tap[16];
  BOOL tx_en, rx_en;

  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) return;

  aim_printf(&uc->pvs, "%d|%d|%3d|", dev_id, ring, sd);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd, "%d|%d|%3d|", dev_id, ring, sd);

  /* get tx_eq parms of sd */
  avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);

  aim_printf(&uc->pvs,
             "%3d|%3d|%3d|%3d|",
             tx_eq.pre,
             tx_eq.atten,
             tx_eq.post,
             tx_eq.slew);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd,
            "%3d|%3d|%3d|%3d|",
            tx_eq.pre,
            tx_eq.atten,
            tx_eq.post,
            tx_eq.slew);

  port_mgr_av_sd_set_rx_cmp_mode(
      dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
  avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);
  bf_sys_usleep(50000);
  data = avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);

  eye_metric = avago_serdes_eye_get_simple_metric(aapl, sbus_addr);
  dfe_status = avago_serdes_get_dfe_status(aapl, sbus_addr);

  aim_printf(&uc->pvs, "%08x| %4d | %04x|", data, eye_metric, dfe_status);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd, "%08x| %4d | %04x|", data, eye_metric, dfe_status);

  /* trying to get eye data for a slice that has rx_en=0 crashes spico */
  avago_serdes_get_tx_rx_ready(aapl, sbus_addr, &tx_en, &rx_en);
  if (rx_en == 0) {
    passed = 0;
    width = 0;
    h_1e03 = 0;
    h_1e06 = 0;
    h_1e10 = 0;
    h_1e12 = 0;
    h_1e15 = 0;
    h_1e17 = 0;
  } else {
    sd_eye_data_get(aapl,
                    sbus_addr,
                    &passed,
                    &width,
                    &h_1e03,
                    &h_1e06,
                    &h_1e10,
                    &h_1e12,
                    &h_1e15,
                    &h_1e17);
  }
  // aim_printf(&uc->pvs, "%2s | %5.2f | %4.2e |%5.2f | %4.2e |",
  aim_printf(&uc->pvs,
             "%2s |%5d|%5d|%5d|%5d|%5d|%5d|%5d|",
             passed ? "ok" : "--",
             width,
             h_1e03,
             h_1e06,
             h_1e10,
             h_1e12,
             h_1e15,
             h_1e17);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd,
            "%2s |%5d|%5d|%5d|%5d|%5d|%5d|%5d|",
            passed ? "ok" : "--",
            width,
            h_1e03,
            h_1e06,
            h_1e10,
            h_1e12,
            h_1e15,
            h_1e17);

  sd_dfe_data_get(dev_id, ring, sd, &dc, &lf, &hf, &bw, &gain, tap);
  aim_printf(&uc->pvs,
             "%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|\n",
             dc,
             lf,
             hf,
             bw,
             gain,
             tap[0],
             tap[1],
             tap[2],
             tap[3],
             tap[4],
             tap[5],
             tap[6],
             tap[7]);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd,
            "%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|\n",
            dc,
            lf,
            hf,
            bw,
            gain,
            tap[0],
            tap[1],
            tap[2],
            tap[3],
            tap[4],
            tap[5],
            tap[6],
            tap[7]);
}

void sd_run_sweep_mult(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd) {
  int pre, attn, post, dfe_running;
  int max_dfe_wait = 120;
  int ring_start;
  int first_ring, last_ring_not_global;

  first_ring = 0;
  last_ring_not_global = 0;

  diag_fd = fopen("sweep-output.txt", "a");
  if (diag_fd == NULL) to_file = 0;

  aim_printf(&uc->pvs, "Tx EQ Sweep: ring=%d : sd=%d\n\n", ring, sd);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd, "Tx EQ Sweep: ring=%d : sd=%d\n\n", ring, sd);

  for (attn = 0; attn < 1; attn++) {
    for (post = 0; post < 1; post++) {
      for (pre = 0; pre < 14; pre++) {
        for (ring = first_ring; ring <= last_ring_not_global; ring++) {
          if (ring == 0) ring_start = 11;
          if (ring == 1) ring_start = 1;

          sd_print_prbs_banner(uc);

          for (sd = ring_start; sd < port_mgr_num_sbus_nodes_get(dev_id, ring);
               sd++) {
            if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

            port_mgr_av_sd_set_tx_eq(dev_id, ring, sd, pre, attn, post);
          }
        }
        for (ring = first_ring; ring <= last_ring_not_global; ring++) {
          if (ring == 0) ring_start = 11;
          if (ring == 1) ring_start = 1;

          for (sd = ring_start; sd < port_mgr_num_sbus_nodes_get(dev_id, ring);
               sd++) {
            if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

            bf_sys_usleep(50000);
            port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
          }
        }
        for (ring = first_ring; ring <= last_ring_not_global; ring++) {
          if (ring == 0) ring_start = 11;
          if (ring == 1) ring_start = 1;

          for (sd = ring_start; sd < port_mgr_num_sbus_nodes_get(dev_id, ring);
               sd++) {
            if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

            max_dfe_wait = 120;
            do {
              dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
              if (dfe_running) {
                bf_sys_usleep(1000000);
              }
            } while (dfe_running && max_dfe_wait--);

            sd_dump_this_prbs(uc, dev_id, ring, sd);
          }
        }
      }
    }
  }
}

void sd_run_sweep_single(ucli_context_t *uc,
                         bf_dev_id_t dev_id,
                         int ring,
                         int sd) {
  int pre, attn, post, dfe_running;
  int max_dfe_wait = 120;

  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
    aim_printf(&uc->pvs,
               "Warning: %d:%d is not an ethernet serdes slice.\n",
               ring,
               sd);
    return;
  }
  aim_printf(&uc->pvs, "Tx EQ Sweep: ring=%d : sd=%d\n\n", ring, sd);

  sd_print_prbs_banner(uc);

  for (attn = 0; attn < 2; attn++) {
    for (post = 1; post < 7; post++) {
      for (pre = 9; pre < 16; pre++) {
        port_mgr_av_sd_set_tx_eq(dev_id, ring, sd, pre, attn, post);
        port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
        max_dfe_wait = 120;
        do {
          bf_sys_usleep(1000000);
          dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
        } while (dfe_running && max_dfe_wait--);

        sd_dump_this_prbs(uc, dev_id, ring, sd);
      }
    }
  }
}

#if 0
// bf-platforms API
uint32_t bf_pltfm_rptr_conn_eq_set_this(uint32_t mode, //eq=0, ig=1
                                        uint32_t conn_id,
                                        uint8_t eq_bst1,
                                        uint8_t eq_bst2,
                                        uint8_t eq_bw,
                                        uint8_t eq_bypass_bst1,
                                        uint8_t vod,
                                        uint8_t gain);

int ig_bst1_lo, ig_bst1_hi, ig_bst1_strd;
int ig_bst2_lo, ig_bst2_hi, ig_bst2_strd;
int ig_bypass_bst1_lo, ig_bypass_bst1_hi, ig_bypass_bst1_strd;
int ig_bw_lo, ig_bw_hi, ig_bw_strd;
int ig_vod_lo, ig_vod_hi, ig_vod_strd;
int ig_gain_lo, ig_gain_hi, ig_gain_strd;
int eg_bst1_lo, eg_bst1_hi, eg_bst1_strd;
int eg_bst2_lo, eg_bst2_hi, eg_bst2_strd;
int eg_bypass_bst1_lo, eg_bypass_bst1_hi, eg_bypass_bst1_strd;
int eg_bw_lo, eg_bw_hi, eg_bw_strd;
int eg_vod_lo, eg_vod_hi, eg_vod_strd;
int eg_gain_lo, eg_gain_hi, eg_gain_strd;
int pre_lo, pre_hi, pre_strd;
int attn_lo, attn_hi, attn_strd;
int post_lo, post_hi, post_strd;
int ring_lo, ring_hi, ring_strd;
int ring0_sd_lo, ring0_sd_hi, ring0_sd_strd;
int ring1_sd_lo, ring1_sd_hi, ring1_sd_strd;

int sd_read_to_eol(FILE *fp) {
  int n_read;
  char c;
  do {
    n_read = fread(&c, 1, 1, fp);
  } while ((c != '\n') && (n_read == 1) && (!feof(fp)));
  if (feof(fp)) return -1;
  return 0;
}

void sd_load_sweep_parms(void) {
  FILE *cfg_fp;

  cfg_fp = fopen("sweep-cfg.txt","ro");
  if (cfg_fp) {
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ig_bst1_lo, &ig_bst1_hi, &ig_bst1_strd);
    aim_printf(&uc->pvs, "ig_bst1 (lo/hi/stride): %d,%d,%d\n", ig_bst1_lo, ig_bst1_hi, ig_bst1_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ig_bst1 (lo/hi/stride): %d,%d,%d\n", ig_bst1_lo, ig_bst1_hi, ig_bst1_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ig_bst2_lo, &ig_bst2_hi, &ig_bst2_strd);
    aim_printf(&uc->pvs, "ig_bst2 (lo/hi/stride): %d,%d,%d\n", ig_bst2_lo, ig_bst2_hi, ig_bst2_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ig_bst2 (lo/hi/stride): %d,%d,%d\n", ig_bst2_lo, ig_bst2_hi, ig_bst2_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ig_bypass_bst1_lo, &ig_bypass_bst1_hi, &ig_bypass_bst1_strd);
    aim_printf(&uc->pvs, "ig_bst1_bypass (lo/hi/stride): %d,%d,%d\n", ig_bypass_bst1_lo, ig_bypass_bst1_hi, ig_bypass_bst1_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ig_bst1_bypass (lo/hi/stride): %d,%d,%d\n", ig_bypass_bst1_lo, ig_bypass_bst1_hi, ig_bypass_bst1_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ig_bw_lo, &ig_bw_hi, &ig_bw_strd);
    aim_printf(&uc->pvs, "ig_bw (lo/hi/stride): %d,%d,%d\n", ig_bw_lo, ig_bw_hi, ig_bw_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ig_bw (lo/hi/stride): %d,%d,%d\n", ig_bw_lo, ig_bw_hi, ig_bw_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ig_vod_lo, &ig_vod_hi, &ig_vod_strd);
    aim_printf(&uc->pvs, "ig_vod (lo/hi/stride): %d,%d,%d\n", ig_vod_lo, ig_vod_hi, ig_vod_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ig_vod (lo/hi/stride): %d,%d,%d\n", ig_vod_lo, ig_vod_hi, ig_vod_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ig_gain_lo, &ig_gain_hi, &ig_gain_strd);
    aim_printf(&uc->pvs, "ig_gain (lo/hi/stride): %d,%d,%d\n", ig_gain_lo, ig_gain_hi, ig_gain_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ig_gain (lo/hi/stride): %d,%d,%d\n", ig_gain_lo, ig_gain_hi, ig_gain_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &eg_bst1_lo, &eg_bst1_hi, &eg_bst1_strd);
    aim_printf(&uc->pvs, "eg_bst1 (lo/hi/stride): %d,%d,%d\n", eg_bst1_lo, eg_bst1_hi, eg_bst1_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "eg_bst1 (lo/hi/stride): %d,%d,%d\n", eg_bst1_lo, eg_bst1_hi, eg_bst1_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &eg_bst2_lo, &eg_bst2_hi, &eg_bst2_strd);
    aim_printf(&uc->pvs, "eg_bst2 (lo/hi/stride): %d,%d,%d\n", eg_bst2_lo, eg_bst2_hi, eg_bst2_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "eg_bst2 (lo/hi/stride): %d,%d,%d\n", eg_bst2_lo, eg_bst2_hi, eg_bst2_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &eg_bypass_bst1_lo, &eg_bypass_bst1_hi, &eg_bypass_bst1_strd);
    aim_printf(&uc->pvs, "eg_bst1_bypass (lo/hi/stride): %d,%d,%d\n", eg_bypass_bst1_lo, eg_bypass_bst1_hi, eg_bypass_bst1_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "eg_bst1_bypass (lo/hi/stride): %d,%d,%d\n", eg_bypass_bst1_lo, eg_bypass_bst1_hi, eg_bypass_bst1_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &eg_bw_lo, &eg_bw_hi, &eg_bw_strd);
    aim_printf(&uc->pvs, "eg_bw (lo/hi/stride): %d,%d,%d\n", eg_bw_lo, eg_bw_hi, eg_bw_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "eg_bw (lo/hi/stride): %d,%d,%d\n", eg_bw_lo, eg_bw_hi, eg_bw_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &eg_vod_lo, &eg_vod_hi, &eg_vod_strd);
    aim_printf(&uc->pvs, "eg_vod (lo/hi/stride): %d,%d,%d\n", eg_vod_lo, eg_vod_hi, eg_vod_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "eg_vod (lo/hi/stride): %d,%d,%d\n", eg_vod_lo, eg_vod_hi, eg_vod_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &eg_gain_lo, &eg_gain_hi, &eg_gain_strd);
    aim_printf(&uc->pvs, "eg_gain (lo/hi/stride): %d,%d,%d\n", eg_gain_lo, eg_gain_hi, eg_gain_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "eg_gain (lo/hi/stride): %d,%d,%d\n", eg_gain_lo, eg_gain_hi, eg_gain_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &pre_lo, &pre_hi, &pre_strd);
    aim_printf(&uc->pvs, "pre (lo/hi/stride): %d,%d,%d\n", pre_lo, pre_hi, pre_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "pre (lo/hi/stride): %d,%d,%d\n", pre_lo, pre_hi, pre_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &attn_lo, &attn_hi, &attn_strd);
    aim_printf(&uc->pvs, "attn (lo/hi/stride): %d,%d,%d\n", attn_lo, attn_hi, attn_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "attn (lo/hi/stride): %d,%d,%d\n", attn_lo, attn_hi, attn_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &post_lo, &post_hi, &post_strd);
    aim_printf(&uc->pvs, "post (lo/hi/stride): %d,%d,%d\n", post_lo, post_hi, post_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "post (lo/hi/stride): %d,%d,%d\n", post_lo, post_hi, post_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ring_lo, &ring_hi, &ring_strd);
    aim_printf(&uc->pvs, "ring (lo/hi/stride): %d,%d,%d\n", ring_lo, ring_hi, ring_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ring (lo/hi/stride): %d,%d,%d\n", ring_lo, ring_hi, ring_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ring0_sd_lo, &ring0_sd_hi, &ring0_sd_strd);
    aim_printf(&uc->pvs, "ring0 sd (lo/hi/stride): %d,%d,%d\n", ring0_sd_lo, ring0_sd_hi, ring0_sd_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ring0 sd (lo/hi/stride): %d,%d,%d\n", ring0_sd_lo, ring0_sd_hi, ring0_sd_strd);
    }
    sd_read_to_eol(cfg_fp); 
    fscanf(cfg_fp,"%d,%d,%d\n", &ring1_sd_lo, &ring1_sd_hi, &ring1_sd_strd);
    aim_printf(&uc->pvs, "ring1 sd (lo/hi/stride): %d,%d,%d\n", ring1_sd_lo, ring1_sd_hi, ring1_sd_strd);
    if (to_file && (diag_fd != NULL)) {
      fprintf(diag_fd, "ring1 sd (lo/hi/stride): %d,%d,%d\n", ring1_sd_lo, ring1_sd_hi, ring1_sd_strd);
    }
  }
}

void sd_print_rptr_prbs_banner(void) {

  aim_printf(&uc->pvs,  "+-------+-----------------+-----------+---------------+---------------+-----+---+-----+-----+-----+-----+-----+-----+-----+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
  aim_printf(&uc->pvs,  "| addr  |          Egr    |    Igr    |     Tx Eq     |               |     |   |     |     |     |     |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "+-+-+---+--+--+--+--+--+--+--+--+--+--+---+---+---+---+--------+------+     |   |     |     |     |     |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "| | |   |  |  |  |  |  |b |  |  |  |b |   | a |   |   |        |      |     |   |     |     |     |     |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "|c|r|   |  |  |g |b |b |y |g |b |b |y |   | t | p | s |        |      |     |   |     |     |     |     |     |     |     |   |   |   |   | g |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "|h|i|   |v |  |a |s |s |p |a |s |s |p | p | t | o | l | Compare|      |     |   |     |     |     |     |     |     |     |   |   |   |   | a |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "|i|n| s |o |b |i |t |t |a |i |t |t |a | r | e | s | e | Errors |quick |     |eye|     |     |     |     |     |     |     |   |   |   |   | i |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "|p|g| d |d |w |n |1 |2 |s |n |1 |2 |s | e | n | t | w |        | eye  | dfe | ok|width| mean| 1e06| 1e10| 1e12| 1e15| 1e17|dc |lf |hf |bw | n |   |   |   |   |   |   |   |   |\n");
  aim_printf(&uc->pvs,  "+-+-+---+--+--+--+--+--+--+--+--+--+--+---+---+---+---+--------+------+-----+---+-----+-----+-----+-----+------+----+-----+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");

  if (to_file && (diag_fd != NULL)) {
  fprintf(diag_fd, "+-------+-----------------+-----------+---------------+---------------+-----+---+-----+-----+-----+-----+-----+-----+-----+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
  fprintf(diag_fd, "| addr  |          Egr    |    Igr    |     Tx Eq     |               |     |   |     |     |     |     |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "+-+-+---+--+--+--+--+--+--+--+--+--+--+---+---+---+---+--------+------+     |   |     |     |     |     |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "| | |   |  |  |  |  |  |b |  |  |  |b |   | a |   |   |        |      |     |   |     |     |     |     |     |     |     |   |   |   |   |   |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "|c|r|   |  |  |g |b |b |y |g |b |b |y |   | t | p | s |        |      |     |   |     |     |     |     |     |     |     |   |   |   |   | g |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "|h|i|   |v |  |a |s |s |p |a |s |s |p | p | t | o | l | Compare|      |     |   |     |     |     |     |     |     |     |   |   |   |   | a |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "|i|n| s |o |b |i |t |t |a |i |t |t |a | r | e | s | e | Errors |quick |     |eye|     |     |     |     |     |     |     |   |   |   |   | i |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "|p|g| d |d |w |n |1 |2 |s |n |1 |2 |s | e | n | t | w |        | eye  | dfe | ok|width| mean| 1e06| 1e10| 1e12| 1e15| 1e17|dc |lf |hf |bw | n |   |   |   |   |   |   |   |   |\n");
  fprintf(diag_fd, "+-+-+---+--+--+--+--+--+--+--+--+--+--+---+---+---+---+--------+------+-----+---+-----+-----+-----+-----+-----+-----+-----+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
  }
}

typedef struct sweep_score_t {
  // score
  uint32_t score_1e06;
  uint32_t score_1e10;
  uint32_t score_1e12;
  uint32_t score_1e15;
  uint32_t score_1e17;
} sweep_score_t;
  
typedef struct sweep_parms_t {
  int pre;
  int attn;
  int post;
  int vod;
  int eq_bw;
  int eq_bst1;
  int eq_bst2;
  int eq_bypass_bst1;
  int eg_gain;
  int ig_eq_bst1;
  int ig_eq_bst2;
  int ig_eq_bypass_bst1;
  int ig_gain;
} sweep_parms_t;

typedef struct sweep_result_t {
  sweep_score_t sweep_score;
  sweep_parms_t sweep_parms;
} sweep_result_t;

// indexed by dev/ring/sd
sweep_result_t best_result_1e06;
sweep_result_t best_result_1e10;
sweep_result_t best_result_1e12;
sweep_result_t best_result_1e15;
sweep_result_t best_result_1e17;

sweep_score_t pass_score = {0};

void sd_show_scores(bool show_1e06, 
                    bool show_1e10, 
                    bool show_1e12, 
                    bool show_1e15, 
                    bool show_1e17) {
  aim_printf(&uc->pvs, "BER : total  vod bw eq-bst1 eg-bst2 eg-byp eg-gain ig-bst1 ig-bst2 ig-byp ig-gain pre attn post\n");

  if (show_1e06) {
  aim_printf(&uc->pvs, "1e06: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e06.sweep_score.score_1e06,
    best_result_1e06.sweep_parms.vod,
    best_result_1e06.sweep_parms.eq_bw,
    best_result_1e06.sweep_parms.eq_bst1,
    best_result_1e06.sweep_parms.eq_bst2,
    best_result_1e06.sweep_parms.eq_bypass_bst1,
    best_result_1e06.sweep_parms.eg_gain,
    best_result_1e06.sweep_parms.ig_eq_bst1,
    best_result_1e06.sweep_parms.ig_eq_bst2,
    best_result_1e06.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e06.sweep_parms.ig_gain,
    best_result_1e06.sweep_parms.pre,
    best_result_1e06.sweep_parms.attn,
    best_result_1e06.sweep_parms.post);
  }
 
  if (show_1e10) {
  aim_printf(&uc->pvs, "1e10: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e10.sweep_score.score_1e10,
    best_result_1e10.sweep_parms.vod,
    best_result_1e10.sweep_parms.eq_bw,
    best_result_1e10.sweep_parms.eq_bst1,
    best_result_1e10.sweep_parms.eq_bst2,
    best_result_1e10.sweep_parms.eq_bypass_bst1,
    best_result_1e10.sweep_parms.eg_gain,
    best_result_1e10.sweep_parms.ig_eq_bst1,
    best_result_1e10.sweep_parms.ig_eq_bst2,
    best_result_1e10.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e10.sweep_parms.ig_gain,
    best_result_1e10.sweep_parms.pre,
    best_result_1e10.sweep_parms.attn,
    best_result_1e10.sweep_parms.post);
  }

  if (show_1e12) {
  aim_printf(&uc->pvs, "1e12: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e12.sweep_score.score_1e12,
    best_result_1e12.sweep_parms.vod,
    best_result_1e12.sweep_parms.eq_bw,
    best_result_1e12.sweep_parms.eq_bst1,
    best_result_1e12.sweep_parms.eq_bst2,
    best_result_1e12.sweep_parms.eq_bypass_bst1,
    best_result_1e12.sweep_parms.eg_gain,
    best_result_1e12.sweep_parms.ig_eq_bst1,
    best_result_1e12.sweep_parms.ig_eq_bst2,
    best_result_1e12.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e12.sweep_parms.ig_gain,
    best_result_1e12.sweep_parms.pre,
    best_result_1e12.sweep_parms.attn,
    best_result_1e12.sweep_parms.post);
  }

  if (show_1e15) {
  aim_printf(&uc->pvs, "1e15: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e15.sweep_score.score_1e15,
    best_result_1e15.sweep_parms.vod,
    best_result_1e15.sweep_parms.eq_bw,
    best_result_1e15.sweep_parms.eq_bst1,
    best_result_1e15.sweep_parms.eq_bst2,
    best_result_1e15.sweep_parms.eq_bypass_bst1,
    best_result_1e15.sweep_parms.eg_gain,
    best_result_1e15.sweep_parms.ig_eq_bst1,
    best_result_1e15.sweep_parms.ig_eq_bst2,
    best_result_1e15.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e15.sweep_parms.ig_gain,
    best_result_1e15.sweep_parms.pre,
    best_result_1e15.sweep_parms.attn,
    best_result_1e15.sweep_parms.post);
  }

  if (show_1e17) {
  aim_printf(&uc->pvs, "1e17: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e17.sweep_score.score_1e17,
    best_result_1e17.sweep_parms.vod,
    best_result_1e17.sweep_parms.eq_bw,
    best_result_1e17.sweep_parms.eq_bst1,
    best_result_1e17.sweep_parms.eq_bst2,
    best_result_1e17.sweep_parms.eq_bypass_bst1,
    best_result_1e17.sweep_parms.eg_gain,
    best_result_1e17.sweep_parms.ig_eq_bst1,
    best_result_1e17.sweep_parms.ig_eq_bst2,
    best_result_1e17.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e17.sweep_parms.ig_gain,
    best_result_1e17.sweep_parms.pre,
    best_result_1e17.sweep_parms.attn,
    best_result_1e17.sweep_parms.post);
  }

  if (to_file && (diag_fd != NULL)) {
  fprintf(diag_fd, "BER : total  vod bw eq-bst1 eg-bst2 eg-byp eg-gain ig-bst1 ig-bst2 ig-byp ig-gain pre attn post\n");

  if (show_1e06) {
  fprintf(diag_fd, "1e06: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e06.sweep_score.score_1e06,
    best_result_1e06.sweep_parms.vod,
    best_result_1e06.sweep_parms.eq_bw,
    best_result_1e06.sweep_parms.eq_bst1,
    best_result_1e06.sweep_parms.eq_bst2,
    best_result_1e06.sweep_parms.eq_bypass_bst1,
    best_result_1e06.sweep_parms.eg_gain,
    best_result_1e06.sweep_parms.ig_eq_bst1,
    best_result_1e06.sweep_parms.ig_eq_bst2,
    best_result_1e06.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e06.sweep_parms.ig_gain,
    best_result_1e06.sweep_parms.pre,
    best_result_1e06.sweep_parms.attn,
    best_result_1e06.sweep_parms.post);
  }
 
  if (show_1e10) {
  fprintf(diag_fd, "1e10: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e10.sweep_score.score_1e10,
    best_result_1e10.sweep_parms.vod,
    best_result_1e10.sweep_parms.eq_bw,
    best_result_1e10.sweep_parms.eq_bst1,
    best_result_1e10.sweep_parms.eq_bst2,
    best_result_1e10.sweep_parms.eq_bypass_bst1,
    best_result_1e10.sweep_parms.eg_gain,
    best_result_1e10.sweep_parms.ig_eq_bst1,
    best_result_1e10.sweep_parms.ig_eq_bst2,
    best_result_1e10.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e10.sweep_parms.ig_gain,
    best_result_1e10.sweep_parms.pre,
    best_result_1e10.sweep_parms.attn,
    best_result_1e10.sweep_parms.post);
  }

  if (show_1e12) {
  fprintf(diag_fd, "1e12: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e12.sweep_score.score_1e12,
    best_result_1e12.sweep_parms.vod,
    best_result_1e12.sweep_parms.eq_bw,
    best_result_1e12.sweep_parms.eq_bst1,
    best_result_1e12.sweep_parms.eq_bst2,
    best_result_1e12.sweep_parms.eq_bypass_bst1,
    best_result_1e12.sweep_parms.eg_gain,
    best_result_1e12.sweep_parms.ig_eq_bst1,
    best_result_1e12.sweep_parms.ig_eq_bst2,
    best_result_1e12.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e12.sweep_parms.ig_gain,
    best_result_1e12.sweep_parms.pre,
    best_result_1e12.sweep_parms.attn,
    best_result_1e12.sweep_parms.post);
  }

  if (show_1e15) {
  fprintf(diag_fd, "1e15: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e15.sweep_score.score_1e15,
    best_result_1e15.sweep_parms.vod,
    best_result_1e15.sweep_parms.eq_bw,
    best_result_1e15.sweep_parms.eq_bst1,
    best_result_1e15.sweep_parms.eq_bst2,
    best_result_1e15.sweep_parms.eq_bypass_bst1,
    best_result_1e15.sweep_parms.eg_gain,
    best_result_1e15.sweep_parms.ig_eq_bst1,
    best_result_1e15.sweep_parms.ig_eq_bst2,
    best_result_1e15.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e15.sweep_parms.ig_gain,
    best_result_1e15.sweep_parms.pre,
    best_result_1e15.sweep_parms.attn,
    best_result_1e15.sweep_parms.post);
  }

  if (show_1e17) {
  fprintf(diag_fd, "1e17: %6d %2d %2d      %2d      %2d      %1d      %2d      %2d      %2d      %1d %2d %2d  %2d   %2d\n",
    best_result_1e17.sweep_score.score_1e17,
    best_result_1e17.sweep_parms.vod,
    best_result_1e17.sweep_parms.eq_bw,
    best_result_1e17.sweep_parms.eq_bst1,
    best_result_1e17.sweep_parms.eq_bst2,
    best_result_1e17.sweep_parms.eq_bypass_bst1,
    best_result_1e17.sweep_parms.eg_gain,
    best_result_1e17.sweep_parms.ig_eq_bst1,
    best_result_1e17.sweep_parms.ig_eq_bst2,
    best_result_1e17.sweep_parms.ig_eq_bypass_bst1,
    best_result_1e17.sweep_parms.ig_gain,
    best_result_1e17.sweep_parms.pre,
    best_result_1e17.sweep_parms.attn,
    best_result_1e17.sweep_parms.post);
  }
  }
}

void sd_score_pass(int vod, int eq_bw,
                   int eq_bst1, int eq_bst2,
                   int eq_bypass_bst1, int eg_gain,
                   int ig_eq_bst1, int ig_eq_bst2,
                   int ig_eq_bypass_bst1, int ig_gain,
                   int pre, int attn, int post) {
  if (pass_score.score_1e06 > best_result_1e06.sweep_score.score_1e06) {
    best_result_1e06.sweep_score.score_1e06 = pass_score.score_1e06;
    best_result_1e06.sweep_parms.pre = pre;
    best_result_1e06.sweep_parms.attn = attn;
    best_result_1e06.sweep_parms.post = post;
    best_result_1e06.sweep_parms.vod = vod;
    best_result_1e06.sweep_parms.eq_bw = eq_bw;
    best_result_1e06.sweep_parms.eq_bst1 = eq_bst1;
    best_result_1e06.sweep_parms.eq_bst2 = eq_bst2;
    best_result_1e06.sweep_parms.eq_bypass_bst1 = eq_bypass_bst1;
    best_result_1e06.sweep_parms.eg_gain = eg_gain;
    best_result_1e06.sweep_parms.ig_eq_bst1 = ig_eq_bst1;
    best_result_1e06.sweep_parms.ig_eq_bst2 = ig_eq_bst2;
    best_result_1e06.sweep_parms.ig_eq_bypass_bst1 = ig_eq_bypass_bst1;
    best_result_1e06.sweep_parms.ig_gain = ig_gain;
    sd_show_scores(true, false, false, false, false);
  }
  if (pass_score.score_1e10 > best_result_1e10.sweep_score.score_1e10) {
    best_result_1e10.sweep_parms.pre = pre;
    best_result_1e10.sweep_parms.attn = attn;
    best_result_1e10.sweep_parms.post = post;
    best_result_1e10.sweep_score.score_1e10 = pass_score.score_1e10;
    best_result_1e10.sweep_parms.vod = vod;
    best_result_1e10.sweep_parms.eq_bw = eq_bw;
    best_result_1e10.sweep_parms.eq_bst1 = eq_bst1;
    best_result_1e10.sweep_parms.eq_bst2 = eq_bst2;
    best_result_1e10.sweep_parms.eq_bypass_bst1 = eq_bypass_bst1;
    best_result_1e10.sweep_parms.eg_gain = eg_gain;
    best_result_1e10.sweep_parms.ig_eq_bst1 = ig_eq_bst1;
    best_result_1e10.sweep_parms.ig_eq_bst2 = ig_eq_bst2;
    best_result_1e10.sweep_parms.ig_eq_bypass_bst1 = ig_eq_bypass_bst1;
    best_result_1e10.sweep_parms.ig_gain = ig_gain;
    sd_show_scores(false, true, false, false, false);
  }
  if (pass_score.score_1e12 > best_result_1e12.sweep_score.score_1e12) {
    best_result_1e12.sweep_parms.pre = pre;
    best_result_1e12.sweep_parms.attn = attn;
    best_result_1e12.sweep_parms.post = post;
    best_result_1e12.sweep_score.score_1e12 = pass_score.score_1e12;
    best_result_1e12.sweep_parms.vod = vod;
    best_result_1e12.sweep_parms.eq_bw = eq_bw;
    best_result_1e12.sweep_parms.eq_bst1 = eq_bst1;
    best_result_1e12.sweep_parms.eq_bst2 = eq_bst2;
    best_result_1e12.sweep_parms.eq_bypass_bst1 = eq_bypass_bst1;
    best_result_1e12.sweep_parms.eg_gain = eg_gain;
    best_result_1e12.sweep_parms.ig_eq_bst1 = ig_eq_bst1;
    best_result_1e12.sweep_parms.ig_eq_bst2 = ig_eq_bst2;
    best_result_1e12.sweep_parms.ig_eq_bypass_bst1 = ig_eq_bypass_bst1;
    best_result_1e12.sweep_parms.ig_gain = ig_gain;
    sd_show_scores(false, false, true, false, false);
  }
  if (pass_score.score_1e15 > best_result_1e15.sweep_score.score_1e15) {
    best_result_1e15.sweep_parms.pre = pre;
    best_result_1e15.sweep_parms.attn = attn;
    best_result_1e15.sweep_parms.post = post;
    best_result_1e15.sweep_score.score_1e15 = pass_score.score_1e15;
    best_result_1e15.sweep_parms.vod = vod;
    best_result_1e15.sweep_parms.eq_bw = eq_bw;
    best_result_1e15.sweep_parms.eq_bst1 = eq_bst1;
    best_result_1e15.sweep_parms.eq_bst2 = eq_bst2;
    best_result_1e15.sweep_parms.eq_bypass_bst1 = eq_bypass_bst1;
    best_result_1e15.sweep_parms.eg_gain = eg_gain;
    best_result_1e15.sweep_parms.ig_eq_bst1 = ig_eq_bst1;
    best_result_1e15.sweep_parms.ig_eq_bst2 = ig_eq_bst2;
    best_result_1e15.sweep_parms.ig_eq_bypass_bst1 = ig_eq_bypass_bst1;
    best_result_1e15.sweep_parms.ig_gain = ig_gain;
    sd_show_scores(false, false, false, true, false);
  }
  if (pass_score.score_1e17 > best_result_1e17.sweep_score.score_1e17) {
    best_result_1e17.sweep_parms.pre = pre;
    best_result_1e17.sweep_parms.attn = attn;
    best_result_1e17.sweep_parms.post = post;
    best_result_1e17.sweep_score.score_1e17 = pass_score.score_1e17;
    best_result_1e17.sweep_parms.vod = vod;
    best_result_1e17.sweep_parms.eq_bw = eq_bw;
    best_result_1e17.sweep_parms.eq_bst1 = eq_bst1;
    best_result_1e17.sweep_parms.eq_bst2 = eq_bst2;
    best_result_1e17.sweep_parms.eq_bypass_bst1 = eq_bypass_bst1;
    best_result_1e17.sweep_parms.eg_gain = eg_gain;
    best_result_1e17.sweep_parms.ig_eq_bst1 = ig_eq_bst1;
    best_result_1e17.sweep_parms.ig_eq_bst2 = ig_eq_bst2;
    best_result_1e17.sweep_parms.ig_eq_bypass_bst1 = ig_eq_bypass_bst1;
    best_result_1e17.sweep_parms.ig_gain = ig_gain;
    sd_show_scores(false, false, false, false, true);
  }
  memset((char*)&pass_score, 0, sizeof(pass_score)); // reset counters
}

void sd_score_pass_for_sd(int score_1e06,
                          int score_1e10,
                          int score_1e12,
                          int score_1e15,
                          int score_1e17) {

 if ((score_1e06 > 0) && (score_1e06 < 600)) pass_score.score_1e06 += score_1e06;
 if ((score_1e10 > 0) && (score_1e10 < 200)) pass_score.score_1e10 += score_1e10;
 if ((score_1e12 > 0) && (score_1e12 < 150)) pass_score.score_1e12 += score_1e12;
 if ((score_1e15 > 0) && (score_1e15 < 100)) pass_score.score_1e15 += score_1e15;
 if ((score_1e17 > 0) && (score_1e17 < 100)) pass_score.score_1e17 += score_1e17; 
}


void sd_dump_this_rptr_prbs(int port_num, int channel_num,
                            // const parms
                            int vod, int eq_bw,
                            // egress parms
                            int eq_bst1, int eq_bst2,
                            int eq_bypass_bst1, int eg_gain,
                            // ingress parms
                            int ig_eq_bst1, int ig_eq_bst2, 
                            int ig_eq_bypass_bst1, int ig_gain,
                            bf_dev_id_t dev_id, int ring, int sd) {
  Avago_serdes_tx_eq_t tx_eq;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t data;
  int dfe_status;
  uint32_t eye_metric;
  bool passed;
  int width;
  int h_1e03;
  int h_1e06;
  int h_1e10;
  int h_1e12;
  int h_1e15;
  int h_1e17;
  uint32_t dc;
  uint32_t lf;
  uint32_t hf;
  uint32_t bw;
  uint32_t gain;
  uint32_t tap[16];
  BOOL tx_en, rx_en;
  (void)port_num;
  (void)channel_num;

  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) return;

  aim_printf(&uc->pvs, "|%d|%d|%3d|", dev_id, ring, sd);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd, "|%d|%d|%3d|", dev_id, ring, sd);

  aim_printf(&uc->pvs, "%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|",
         // const parms
         vod, eq_bw, 
         // egress parms
         eg_gain, eq_bst1, eq_bst2, 
         eq_bypass_bst1, 
         // ingress parms
         ig_gain, ig_eq_bst1, ig_eq_bst2, ig_eq_bypass_bst1);
  if (to_file && (diag_fd != NULL)) fprintf(diag_fd, "%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|",
         // const parms
         vod, eq_bw,
         // egress parms
         eg_gain, eq_bst1, eq_bst2, 
         eq_bypass_bst1, 
         // ingress parms
         ig_gain, ig_eq_bst1, ig_eq_bst2, ig_eq_bypass_bst1);

  /* get tx_eq parms of sd */
  avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);

  aim_printf(&uc->pvs, "%3d|%3d|%3d|%3d|", tx_eq.pre, tx_eq.atten, tx_eq.post, tx_eq.slew);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd,
            "%3d|%3d|%3d|%3d|",
            tx_eq.pre,
            tx_eq.atten,
            tx_eq.post,
            tx_eq.slew);

  port_mgr_av_sd_set_rx_cmp_mode(dev_id, ring, sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
  data = avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);
  bf_sys_usleep(50000);
  data = avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);

  eye_metric = avago_serdes_eye_get_simple_metric(aapl, sbus_addr);
  dfe_status = avago_serdes_get_dfe_status(aapl, sbus_addr);

  aim_printf(&uc->pvs, "%08x| %4d | %04x|", data, eye_metric, dfe_status);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd, "%08x| %4d | %04x|", data, eye_metric, dfe_status);

  /* trying to get eye data for a slice that has rx_en=0 crashes spico */
  avago_serdes_get_tx_rx_ready(aapl, sbus_addr, &tx_en, &rx_en);
  if (rx_en == 0) {
    passed = 0;
    width = 0;
    h_1e03 = 0;
    h_1e06 = 0;
    h_1e10 = 0;
    h_1e12 = 0;
    h_1e15 = 0;
    h_1e17 = 0;
  } else {
    sd_eye_data_get(aapl,
                    sbus_addr,
                    &passed,
                    &width,
                    &h_1e03,
                    &h_1e06,
                    &h_1e10,
                    &h_1e12,
                    &h_1e15,
                    &h_1e17);
  }
  if (width  < 0) width  = 0;
  if (h_1e03 < 0) h_1e03 = 0;
  if (h_1e06 < 0) h_1e06 = 0;
  if (h_1e10 < 0) h_1e10 = 0;
  if (h_1e12 < 0) h_1e12 = 0;
  if (h_1e15 < 0) h_1e15 = 0;
  if (h_1e17 < 0) h_1e17 = 0;

  sd_score_pass_for_sd(h_1e06,
                       h_1e10,
                       h_1e12,
                       h_1e15,
                       h_1e17);

  // aim_printf(&uc->pvs, "%2s | %5.2f | %4.2e |%5.2f | %4.2e |",
  aim_printf(&uc->pvs, "%2s |%5d|%5d|%5d|%5d|%5d|%5d|%5d|",
         passed ? "ok" : "--",
         width,
         h_1e03,
         h_1e06,
         h_1e10,
         h_1e12,
         h_1e15,
         h_1e17);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd,
            "%2s |%5d|%5d|%5d|%5d|%5d|%5d|%5d|",
            passed ? "ok" : "--",
            width,
            h_1e03,
            h_1e06,
            h_1e10,
            h_1e12,
            h_1e15,
            h_1e17);

  sd_dfe_data_get(dev_id, ring, sd, &dc, &lf, &hf, &bw, &gain, tap);
  aim_printf(&uc->pvs, "%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|\n",
         dc,
         lf,
         hf,
         bw,
         gain,
         tap[0],
         tap[1],
         tap[2],
         tap[3],
         tap[4],
         tap[5],
         tap[6],
         tap[7]);
  if (to_file && (diag_fd != NULL))
    fprintf(diag_fd,
            "%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|%3d|\n",
            dc,
            lf,
            hf,
            bw,
            gain,
            tap[0],
            tap[1],
            tap[2],
            tap[3],
            tap[4],
            tap[5],
            tap[6],
            tap[7]);
}

/*
ig_bst1_lo,ig_bst1_hi,ig_bst1_strd
ig_bst2_lo,ig_bst2_hi,ig_bst2_strd
ig_bypass_bst1_lo,ig_bypass_bst1_hi,ig_bypass_bst1_strd
ig_bw_lo,ig_bw_hi,ig_bw_strd
ig_vod_lo,ig_vod_hi,ig_vod_strd

eg_bst1_lo,eg_bst1_hi,eg_bst1_strd
eg_bst2_lo,eg_bst2_hi,eg_bst2_strd
eg_bypass_bst1_lo,eg_bypass_bst1_hi,eg_bypass_bst1_strd
eg_bw_lo,eg_bw_hi,eg_bw_strd
eg_vod_lo,eg_vod_hi,eg_vod_strd

pre_lo,pre_hi,pre_strd
attn_lo,attn_hi,attn_strd
post_lo,post_hi,post_strd

ring_lo,ring_hi,ring_strd
ring0_sd_lo,ring0_sd_hi,ring0_sd_strd
ring1_sd_lo,ring1_sd_hi,ring1_sd_strd
*/

void rptr_sweep(void) {
  uint8_t port_num;
  uint8_t channel_num=0;
  uint8_t eg_bst1, ig_bst1;
  uint8_t eg_bst2, ig_bst2;
  uint8_t eg_bypass_bst1, ig_bypass_bst1;
  uint8_t eg_bw, ig_bw;
  uint8_t eg_vod, ig_vod;
  uint8_t eg_gain, ig_gain;
  int rc;
  int pre, attn, post, dfe_running, dev_id, ring, sd;
  int max_dfe_wait = 120;

  // constants
  dev_id = 0;
  ring = 1;
  eg_vod = ig_vod = 3;
  eg_bw = ig_bw = 3;
//  eg_gain = ig_gain = 0x2f;

  diag_fd = fopen("sweep-output.txt","a");
  if (diag_fd == NULL) to_file = 0;

  sd_load_sweep_parms();

  // Set egress rptr boost
for (eg_bypass_bst1 = eg_bypass_bst1_lo; 
     eg_bypass_bst1 <= eg_bypass_bst1_hi; 
     eg_bypass_bst1 += eg_bypass_bst1_strd) {
  uint8_t eg_bst1_limit = eg_bypass_bst1 ? eg_bst1_lo:eg_bst1_hi;

 for (eg_gain = eg_gain_lo; 
      eg_gain <= eg_gain_hi; 
      eg_gain += eg_gain_strd) {

  for (eg_bst1 = eg_bst1_lo; 
       eg_bst1 <= eg_bst1_limit; 
       eg_bst1 += eg_bst1_strd) {
    for (eg_bst2 = eg_bst2_lo; 
         eg_bst2 <= eg_bst2_hi; 
         eg_bst2 += eg_bst2_strd) {
      for (port_num = 33; port_num < 65; port_num++) {
        // filter out non-rptr ports
        if ((port_num > 40) && (port_num < 45)) continue;
        if ((port_num > 52) && (port_num < 57)) continue;

        rc = bf_pltfm_rptr_conn_eq_set_this(0, // eg
                               port_num,
                               eg_bst1,
                               eg_bst2,
                               eg_bw,
                               eg_bypass_bst1,
                               eg_vod,
                               eg_gain);
        if (rc != 0) {
          aim_printf(&uc->pvs, "Warning: Err=%d from rptr_eq_set %d %d %d %d %d %d %d %d\n",
                 rc, 0, port_num,
                 eg_bst1, eg_bst2, eg_bw,
                 eg_bypass_bst1, eg_vod, eg_gain);
          return; // terminate
        }
      }
      // Set ingress rptr boost
      for (ig_bypass_bst1 = ig_bypass_bst1_lo; 
           ig_bypass_bst1 <= ig_bypass_bst1_hi; 
           ig_bypass_bst1 += ig_bypass_bst1_strd) {
        uint8_t ig_bst1_limit = ig_bypass_bst1 ? ig_bst1_lo:ig_bst1_hi;

       for (ig_gain = ig_gain_lo;
            ig_gain <= ig_gain_hi;
            ig_gain += ig_gain_strd) {

        for (ig_bst1 = ig_bst1_lo; 
             ig_bst1 <= ig_bst1_limit; 
             ig_bst1 += ig_bst1_strd) {
          for (ig_bst2 = ig_bst2_lo; 
               ig_bst2 <= ig_bst2_hi; 
               ig_bst2 += ig_bst2_strd) {

            for (port_num = 33; port_num < 65; port_num++) {
              // filter out non-rptr ports
              if ((port_num > 40) && (port_num < 45)) continue;
              if ((port_num > 52) && (port_num < 57)) continue;

              rc = bf_pltfm_rptr_conn_eq_set_this(0, // eg
                               port_num,
                               ig_bst1,
                               ig_bst2,
                               ig_bw,
                               ig_bypass_bst1,
                               ig_vod,
                               ig_gain);
              if (rc != 0) {
                aim_printf(&uc->pvs, "Warning: Err=%d from rptr_eq_set %d %d %d %d %d %d %d %d\n",
                       rc, 0, port_num,
                       ig_bst1, ig_bst2, ig_bw,
                       ig_bypass_bst1, ig_vod, ig_gain);
                return; // terminate
              }
            }
            for (pre = pre_lo; 
                 pre <= pre_hi; 
                 pre += pre_strd) {
              for (attn = attn_lo; 
                   attn <= attn_hi; 
                   attn += attn_strd) {
                for (post = post_lo; 
                     post <= post_hi; 
                     post += post_strd) {

                  sd_print_rptr_prbs_banner();

                  for (ring = ring_lo; ring <= ring_hi; ring += ring_strd) {
                    int sd_lo, sd_hi, sd_strd;
                  
                    if (ring == 0) {
                      sd_lo = ring0_sd_lo;
                      sd_hi = ring0_sd_hi;
                      sd_strd = ring0_sd_strd;
                    } else {
                      sd_lo = ring1_sd_lo;
                      sd_hi = ring1_sd_hi;
                      sd_strd = ring1_sd_strd;
                    }
                    // Set TxEQ on all
                    for (sd = sd_lo; sd <= sd_hi; sd += sd_strd) {
                      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

                      port_mgr_av_sd_set_tx_eq(dev_id, ring, sd, pre, attn, post);
                    }
                    // Start DFE after TxEQ set
                    for (sd = sd_lo; sd <= sd_hi; sd += sd_strd) {
                      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

                      port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
                      bf_sys_usleep(1000000);
                    }
                    bf_sys_usleep(1000000);
                    // Wait for DFE to complete
                    for (sd = sd_lo; sd <= sd_hi; sd += sd_strd) {
                      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

                      max_dfe_wait = 120;
                      do {
                        dfe_running = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
                        if (dfe_running) {
                          bf_sys_usleep(1000000);
                        }
                      } while (dfe_running && max_dfe_wait--);
  
                      sd_dump_this_rptr_prbs(port_num, channel_num,
                                             // const parms
                                             eg_vod, eg_bw,
                                             // egress parms
                                             eg_bst1, eg_bst2,
                                             eg_bypass_bst1, eg_gain,
                                             // ingress parms
                                             ig_bst1, ig_bst2, 
                                             ig_bypass_bst1, ig_gain,
                                             dev_id, ring, sd);
                    }
                  }
                  sd_score_pass(eg_vod, eg_bw,
                                // egress parms
                                eg_bst1, eg_bst2,
                                eg_bypass_bst1, eg_gain,
                                // ingress parms
                                ig_bst1, ig_bst2,
                                ig_bypass_bst1, ig_gain,
                                pre, attn, post);
                }
              }
            }
          }
        }
       } //ig gain
      } //ig bst1 byp
    }
  }
 } //eg gain
} //eg bst1 bypass
sd_show_scores(true, true, true, true, true);
}
#else
void rptr_sweep(void) {}
#endif

void sd_dump_pmd(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_pmd_debug_t *pmd_debug = avago_serdes_pmd_debug_construct(aapl);

  avago_serdes_pmd_debug(aapl, sbus_addr, pmd_debug);
  avago_serdes_pmd_debug_print(aapl, pmd_debug);

  avago_serdes_pmd_debug_destruct(aapl, pmd_debug);
}

char *link_training_st_to_str(int st) {
  if (st == 0)
    return "F";
  else if (st == 1)
    return "C";
  else if (st == 2)
    return "R";
  else
    return "-";
}

char *hcd_to_str(int hcd) {
  switch (hcd) {
    case 0:
      return "1000BASE-KX";
    case 1:
      return "10GBASE-KX4";
    case 2:
      return "10GBASE-KR";
    case 3:
      return "40GBASE-KR4";
    case 4:
      return "40GBASE-CR4";
    case 5:
      return "100GBASE-CR10";
    case 6:
      return "40GBASE-KP4";
    case 7:
      return "undefined";
    case 8:
      return "100GBASE-KR4";
    case 9:
    case 10:
      return "undefined";
    case 11:
      return "25GBASE-CR";
    default:
      return "undefined";
  }
}

void sd_dump_an(ucli_context_t *uc) {
  bf_dev_id_t dev_id, ring, sd;
  char *arb_to_str[16] = {
      "AN_ENABLE",
      "TRANSMIT_DISABLE",
      "ABILITY_DETECT",
      "ACKNOWLEDGE_DETECT",
      "COMPLETE_ACKNOWLEDGE",
      "NEXT_PAGE_WAIT",
      "AN_GOOD_CHECK",
      "AN_GOOD",
      "LINK_STATUS_CHECK",
      "PARALLEL_DETECTION_FAULT",
      "KR_CONFIG1",
      "KR_CONFIG2",
      "KR_TRAINING_LAUNCH",
      "KR_TRAINING",
      " ",
      " ",
  };

  for (dev_id = 0; dev_id < 1; dev_id++) {
    for (ring = 0; ring < 1; ring++) {
      for (sd = 0; sd < 16; sd++) {
        if ((sd % 32) == 0) {
          aim_printf(
              &uc->pvs,
              "+------+--------------------------+---------------------+-------"
              "------------------\n");
          aim_printf(
              &uc->pvs,
              "| addr |   AN status              |          LP adv     | cur "
              "state               \n");
          aim_printf(
              &uc->pvs,
              "+-+-+--+-+-+-+-+-+-+--------------+-+--+-+--------------+-------"
              "------------------\n");
          aim_printf(
              &uc->pvs,
              "| | |  |c| |l| | | |              |p|a |t|              |       "
              "                  \n");
          aim_printf(
              &uc->pvs,
              "|c|r|n |m|g|p|n|b| |              |a|r |r|              |       "
              "                  \n");
          aim_printf(
              &uc->pvs,
              "|h|i|o |p|o|a|e|a|f| h            |r|b |a|              |       "
              "                  \n");
          aim_printf(
              &uc->pvs,
              "|i|n|d |l|o|b|x|s|e| c            |F|s |i|              |       "
              "                  \n");
          aim_printf(
              &uc->pvs,
              "|p|g|e |t|d|l|t|e|c| d            |t|t |n|              |       "
              "                  \n");
          aim_printf(
              &uc->pvs,
              "+-+-+--+-+-+-+-+-+-+--------------+-+--+-+--------------+-------"
              "------------------\n");
        }
        if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;
        Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
        int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
        int hcd, fec, base, nxt, lp, cmplt, good, parallel_det_flt, arb_st,
            lt_st;
        int lp_adv_15_0, lp_adv_31_16, lp_adv_47_32;

        hcd = avago_serdes_read_an_status(
            aapl, sbus_addr, AVAGO_SERDES_AN_READ_HCD);
        fec = avago_serdes_read_an_status(
            aapl, sbus_addr, AVAGO_SERDES_AN_READ_FEC_ENABLE);
        base = avago_serdes_read_an_status(
            aapl, sbus_addr, AVAGO_SERDES_AN_BASE_PAGE_RX);
        nxt = avago_serdes_read_an_status(
            aapl, sbus_addr, AVAGO_SERDES_AN_NEXT_PAGE_RX);
        lp = avago_serdes_read_an_status(
            aapl, sbus_addr, AVAGO_SERDES_AN_LP_AN_ABLE);
        cmplt = avago_serdes_read_an_status(
            aapl, sbus_addr, AVAGO_SERDES_AN_COMPLETE);
        good =
            avago_serdes_read_an_status(aapl, sbus_addr, AVAGO_SERDES_AN_GOOD);
        parallel_det_flt =
            (avago_spico_int(aapl, sbus_addr, 0x707, 0x1) >> 11) & 1;
        arb_st = (avago_spico_int(aapl, sbus_addr, 0x807, 1)) & 0xF;
        lt_st = avago_serdes_pmd_status(aapl, sbus_addr);
        lp_adv_15_0 = avago_spico_int(aapl, sbus_addr, 0x629, 0);
        lp_adv_31_16 = avago_spico_int(aapl, sbus_addr, 0x629, 1);
        lp_adv_47_32 = avago_spico_int(aapl, sbus_addr, 0x629, 2);

        aim_printf(&uc->pvs, "|%d|%d|%2d|", dev_id, ring, sd);
        aim_printf(&uc->pvs,
                   "%d|%d|%d|%d|%d|%d|%-12s|%d|%2d|%s|%04x_%04x_%04x|%-25s|\n",
                   cmplt,
                   good,
                   lp,
                   nxt,
                   base,
                   fec,
                   hcd_to_str(hcd),
                   parallel_det_flt,
                   arb_st,
                   link_training_st_to_str(lt_st),
                   lp_adv_47_32,
                   lp_adv_31_16,
                   lp_adv_15_0,
                   arb_to_str[arb_st]);
      }
    }
  }
}

void sd_dump_sensor_data(ucli_context_t *uc,
                         bf_dev_id_t dev_id,
                         int ring,
                         int sd) {
  uint32_t temp_data[9], voltage_data[9], snsr_int;
  int ch;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbm_sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  // verify sd is a sensor
  // get temp data
  for (ch = 0; ch < 8; ch++) {
    snsr_int = (ch << 12) | (0x3 << 8) | (sd);
    temp_data[ch] = avago_spico_int(aapl, sbm_sbus_addr, 0x03, snsr_int);
  }
  for (ch = 0; ch < 8; ch++) {
    snsr_int = (ch << 12) | (0x4 << 8) | (sd);
    // get voltage data
    voltage_data[ch] = avago_spico_int(aapl, sbm_sbus_addr, 0x04, snsr_int);
  }
  aim_printf(&uc->pvs, "Ch   Temperature     Voltage\n");
  for (ch = 0; ch < 8; ch++) {
    aim_printf(&uc->pvs,
               "%d    %d.%03d C        %d.%d mV\n",
               ch,
               temp_data[ch] / 8,
               ((temp_data[ch] % 8) * 125) / 10,
               voltage_data[ch] / 2,
               (voltage_data[ch] & 1) * 5);
  }
}

extern void avago_serdes_state_dump(Aapl_t *aapl, uint32_t addr);

void sd_tech_sppt_dump(bf_dev_id_t dev_id) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int ring, sd;

  for (ring = 0; ring < 1; ring++) {
    for (sd = 0; sd < 256; sd++) {
      int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
      avago_serdes_state_dump(aapl, sbus_addr);
    }
  }
}

void sd_aapl_serdes_dump(bf_dev_id_t dev_id,
                         int ring,
                         int sd,
                         ucli_context_t *uc) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  unsigned int verbose = aapl->verbose;
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  aapl->verbose = 2;  // capture RX Data
  char *buf = avago_serdes_get_state_dump(aapl, sbus_addr, 0, 0);
  aapl->verbose = verbose;
  if (buf && uc) {
    aim_printf(&uc->pvs, "%s \n", buf);
    AAPL_FREE(buf);
  }
}

extern int aapl_serdes_main(int argc, char *argv[], Aapl_t *aapl);

void sd_aapl_diag(int argc, char *argv[], bf_dev_id_t dev_id) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  aapl_diag_main(argc, argv, aapl);
}

void sd_aapl_serdes_main(int argc, char *argv[], bf_dev_id_t dev_id) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  aapl_serdes_main(argc, argv, aapl);
}

int aapl_eye_main(int argc, char *argv[], Aapl_t *aapl);

void sd_aapl_eye_main(int argc, char *argv[], bf_dev_id_t dev_id) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  aapl_eye_main(argc, argv, aapl);
}

void sd_swing(bf_dev_id_t dev_id,
              int ring,
              int sd,
              uint32_t *swing_lo,
              uint32_t *swing_hi,
              uint32_t *lo_los_neq_0,
              uint32_t *lo_sig_ok_eq_0,
              uint32_t *lo_ei_neq_0,
              uint32_t *calibrated_thresh) {
  uint32_t hi = 0, lo = 255;
  uint32_t t, cur_thresh, cur_thresh_en;
  int sig_ok, los;
  int err, inst, sub_inst;
  port_mgr_sbus_ip_type_e unused_ip_type;
  uint32_t lo_los_neq_0_val = 255;
  uint32_t lo_sig_ok_eq_0_val = 255;
  uint32_t lo_ei_neq_0_val = 255;

  if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
    *swing_lo = 0;
    *swing_hi = 0;
    return;
  }

  err = port_mgr_find_mac_info_for(
      dev_id, ring, sd, &unused_ip_type, &inst, &sub_inst);
  if (err || (unused_ip_type != IP_TYPE_ETH_PMA)) {
    *swing_lo = -1;
    *swing_hi = -1;
  }

  cur_thresh_en = port_mgr_av_sd_signal_ok_en_get(dev_id, ring, sd);
  cur_thresh = port_mgr_av_sd_signal_ok_thresh_get(dev_id, ring, sd);
  *calibrated_thresh = (((93 * cur_thresh) - 37) / 10);  // cur_thresh;
  port_mgr_av_sd_los_get(dev_id, ring, sd);

  // en thresh
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x0040 | (1 << 8));
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x20);
  bf_sys_usleep(100);

  *lo_los_neq_0 = 900;
  *lo_sig_ok_eq_0 = 900;
  *lo_ei_neq_0 = 900;

  // check each 10mV step (10-900mv)
  for (t = 1; t < 128; t++) {
    int elec_idle, sig_ok_live, dbn_sigok, flock;

    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x0040 | (t << 8));
    bf_sys_usleep(100);

    los = port_mgr_av_sd_los_get(dev_id, ring, sd);  // reset sticky bit
    sig_ok = port_mgr_av_sd_signal_ok_get(dev_id, ring, sd);
    sig_ok_live = port_mgr_av_sd_signal_ok_live_get(dev_id, ring, sd);
    elec_idle = port_mgr_av_sd_elec_idle_get(dev_id, ring, sd);
    flock = (port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x401c, 0x0) >> 15) & 1;

    dbn_sigok = port_mgr_mac_rxsigok_ctrl_unmapped_get(dev_id, inst, sub_inst);

    // "lo" represents the lowest thresh where sig_ok=0
    // "hi" represents the highest thresh where los=0
    if (sig_ok == 0) {
      if (lo > t) {
        lo = t;
      }
    }
    if (sig_ok == 1) {
      if (hi < t) {
        hi = t;
      }
    }
    if ((los != 0) && (t < lo_los_neq_0_val)) lo_los_neq_0_val = t;
    if ((sig_ok == 0) && (t < lo_sig_ok_eq_0_val)) lo_sig_ok_eq_0_val = t;
    if ((elec_idle != 0) && (t < lo_ei_neq_0_val)) lo_ei_neq_0_val = t;

    port_mgr_log(
        "%d:%d:%3d: t=%3d : thresh=%4d mV : los= %d : sig_ok= %d : live= %d : "
        "elec_idle= %d : flock= %d : MAC rxsigok= %d",
        dev_id,
        ring,
        sd,
        t,
        (((93 * t) - 37) / 10),
        los,
        sig_ok,
        sig_ok_live,
        elec_idle,
        flock,
        dbn_sigok);
  }
  // reset pre-existing thresh/en
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x0040 | (cur_thresh << 8));
  if (cur_thresh_en) {
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x20);
  } else {
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0x0);
  }
  *swing_lo = (((93 * lo) - 37) / 10);
  *swing_hi = (((93 * hi) - 37) / 10);

  // rationalize
  if (*swing_lo > 0x7FFFFFFF) *swing_lo = 0;
  if (*swing_lo > 900) *swing_lo = 900;
  if (*swing_hi > 0x7FFFFFFF) *swing_hi = 0;
  if (*swing_hi > 900) *swing_hi = 900;

  *lo_los_neq_0 = (((93 * lo_los_neq_0_val) - 37) / 10);
  *lo_sig_ok_eq_0 = (((93 * lo_sig_ok_eq_0_val) - 37) / 10);
  *lo_ei_neq_0 = (((93 * lo_ei_neq_0_val) - 37) / 10);
}

/** \brief Display stats for a serdes slice.
 *
 * [ POST_ENABLE ] / [ PRE_ENABLE]
 *
 * \param dev_id             : system-assigned identifier
 *(0..BF_MAX_DEV_COUNT-1)
 * \param port               : physical port # on dev_id (0..LLD_MAX_PORTS-1)
 * \param lane               : logical lane (within port) 0-3, depending upon
 *mode
 * \param display_ucli_cookie: ucli context (typecasted as (void *)) in which to
 *display
 *
 * \return: BF_SUCCESS    : PRBS stats dsiplay successful
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > *BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: invalid lane
 *
 */
bf_status_t sd_diag_prbs_stats_display(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t lane,
                                       void *display_ucli_cookie) {
  port_mgr_serdes_t *serdes_p;
  if (display_ucli_cookie == NULL) return BF_INVALID_ARG;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
      dev_id, dev_port, lane);
  if (serdes_p != NULL) {
    /*FIXME : For the time being, this assumes symmetric serdes and hence we can
     * pass either tx_sd or rx_sd*/
    bf_sys_assert(serdes_p->tx_sd == serdes_p->rx_sd);
    sd_dump_this(uc, dev_id, serdes_p->ring, serdes_p->tx_sd);
    return BF_SUCCESS;
  }
  return BF_INVALID_ARG;
}

/**\display perf
 *
 */
bf_status_t sd_diag_perf_display(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 int fp,
                                 int ch,
                                 uint32_t ln,
                                 void *display_ucli_cookie) {
  port_mgr_serdes_t *serdes_p;
  int ring, sd, mac_block;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  ring = serdes_p->ring;
  sd = serdes_p->rx_sd;
  aim_printf(&uc->pvs,
             "|%2d/%d|%2d|%d|%d|%d|%3d|",
             fp,
             ch,
             mac_block,
             ch + ln,
             dev_id,
             ring,
             sd);
  sd_display_port_perf(dev_id, ring, sd, uc);
  return BF_SUCCESS;
}

/**\plot eye
 *
 */
bf_status_t sd_diag_plot_eye(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t ln,
                             void *display_ucli_cookie) {
  // sd_plot_eye(dev_id, dev_port, uc);
  int ring, sd, state;
  port_mgr_serdes_t *serdes_p;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  ring = serdes_p->ring;
  sd = serdes_p->rx_sd;
  state = port_mgr_av_sd_dump_eye(dev_id, ring, sd, 1, NULL);
  if (state != 0) {
    aim_printf(&uc->pvs, "Eye plot fails.\n");
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/**\set TX eq
 *
 */
bf_status_t sd_diag_set_tx_eq(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              uint32_t ln,
                              int pre,
                              int atten,
                              int post,
                              int slew,
                              void *display_ucli_cookie) {
  int ring, sd;
  port_mgr_serdes_t *serdes_p;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  ring = serdes_p->ring;
  sd = serdes_p->tx_sd;
  sd_set_tx_eq(dev_id, ring, sd, pre, atten, post, slew, uc);
  return BF_SUCCESS;
}

/**\switch to transmit PRBS
 *
 */
bf_status_t sd_diag_chg_to_prbs(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                uint32_t ln,
                                void *display_ucli_cookie) {
  int ring, state;
  port_mgr_serdes_t *serdes_p;
  ucli_context_t *uc = (ucli_context_t *)display_ucli_cookie;
  if (uc == NULL) return BF_INVALID_ARG;
  serdes_p = port_mgr_tof1_map_port_lane_to_serdes(dev_id, dev_port, ln);
  if (serdes_p == NULL) return BF_INVALID_ARG;
  ring = serdes_p->ring;
  // set Tx data sel
  state = port_mgr_av_sd_tx_data_sel_set(
      dev_id, ring, serdes_p->tx_sd, BF_SDS_PAT_PATSEL_PRBS31);
  if (state) {
    aim_printf(&uc->pvs, "Chg to PRBS tx setting fails, retry please.\n");
    return BF_INVALID_ARG;
  }
  // set rx data sel
  state = port_mgr_av_sd_set_rx_cmp_sel(
      dev_id, ring, serdes_p->rx_sd, AVAGO_SERDES_RX_CMP_DATA_PRBS31);
  if (state) {
    aim_printf(&uc->pvs, "Chg to PRBS rx setting fails, retry please.\n");
    return BF_INVALID_ARG;
  }
  // set rx cmp mode
  state = port_mgr_av_sd_set_rx_cmp_mode(
      dev_id, ring, serdes_p->rx_sd, AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
  if (state) {
    aim_printf(&uc->pvs,
               "Chg to PRBS rx cmp mode setting fails, retry please.\n");
    return BF_INVALID_ARG;
  }
  // make sure rigt termination is set
  port_mgr_av_sd_rx_term_set(
      dev_id, ring, serdes_p->rx_sd, BF_SDS_RX_TERM_AVDD);
  return BF_SUCCESS;
}
