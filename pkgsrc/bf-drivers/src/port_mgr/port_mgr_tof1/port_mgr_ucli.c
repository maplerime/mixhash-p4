/*******************************************************************************
 * BAREFOOT NETWORKS CONFIDENTIAL & PROPRIETARY
 *
 * Copyright (c) 2015-2019 Barefoot Networks, Inc.

 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the property of
 * Barefoot Networks, Inc. and its suppliers, if any. The intellectual and
 * technical concepts contained herein are proprietary to Barefoot Networks,
 * Inc.
 * and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from
 * Barefoot Networks, Inc.
 *
 * No warranty, explicit or implicit is provided, unless granted under a
 * written agreement with Barefoot Networks, Inc.
 *
 * $Id: $
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <target-sys/bf_sal/bf_sys_intf.h>
#include <lld/lldlib_config.h>

#if PORT_MGR_CONFIG_INCLUDE_UCLI == 1

#include "dvm/bf_drv_intf.h"
#include "lld/bf_dma_dr_id.h"
#include "lld/bf_dma_if.h"
#include "lld/lld_err.h"
#include "lld/lld_sku.h"
#include "lld/lld_dr_if.h"
#include "lld/lld_reg_if.h"
#include "lld/lld_efuse.h"
#include "tofino_regs/tofino.h"
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>
#include "port_mgr/port_mgr_intf.h"
#include "port_mgr/port_mgr_ha.h"
#include "port_mgr/port_mgr.h"
#include "port_mgr/port_mgr_dev.h"
#include "port_mgr/port_mgr_map.h"
#include "port_mgr/port_mgr_log.h"
#include "port_mgr/port_mgr_tof2/port_mgr_tof2_umac.h"
#include "port_mgr/port_mgr_tof3/port_mgr_tof3_map.h"
#include "port_mgr/port_mgr_tof3/aw_if.h"
//#include <port_mgr/bf_aw_pmd.h>
#include "port_mgr_mac.h"
#include "port_mgr_serdes_diag.h"
#include "port_mgr/bf_port_if.h"
#include "port_mgr/bf_serdes_if.h"
#include "port_mgr_memory_mapping.h"
#include "port_mgr/port_mgr_serdes_sbus_map.h"
#include "port_mgr_av_sd.h"
#include "bf_pm/bf_pm_intf.h"

#define SHOW_TECH_PORT_UCLI_GENERIC 6
#define SHOW_TECH_PORT_UCLI_DEV_ID 7

// FIXME
// requires port_mgr ucli file
uint32_t port_mgr_mac_get_glb_mode(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
uint32_t port_mgr_mac_get_slot2ch_map(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port);
int port_mgr_mac_read_counter(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              int ctr,
                              uint64_t *ctr_value);

extern void reg_parse_main(void);
extern void consistency_check_pipes(void);
extern bool port_mgr_is_valid_sbus_address(bf_dev_id_t dev_id,
                                           int ring,
                                           int sd);

static ucli_status_t port_mgr_ucli_ucli__op__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "op", 1, "Serdes oper status <dev_id>");

  sd_dump(uc);

  return 0;
}

void dump_an_base_page(ucli_context_t *uc, uint64_t base_pg) {
  aim_printf(&uc->pvs,
             " %04x_%04x_%04x",
             (unsigned int)(base_pg >> 32ull) & 0xffff,
             (unsigned int)(base_pg >> 16ull) & 0xffff,
             (unsigned int)(base_pg >> 0ull) & 0xffff);

  if (base_pg & (0x7FFull << 21ull)) {
    aim_printf(&uc->pvs,
               " : %s%s%s%s%s%s%s%s%s%s%s",
               (base_pg & (1ull << 21ull)) ? "1000_KX " : "",
               (base_pg & (1ull << 22ull)) ? "10G_KX4 " : "",
               (base_pg & (1ull << 23ull)) ? "10G_KR " : "",
               (base_pg & (1ull << 24ull)) ? "40G_KR4 " : "",
               (base_pg & (1ull << 25ull)) ? "40G_CR4 " : "",
               (base_pg & (1ull << 26ull)) ? "100G_CR10 " : "",
               (base_pg & (1ull << 27ull)) ? "40G_KP4 " : "",
               (base_pg & (1ull << 28ull)) ? "100G_KR4 " : "",
               (base_pg & (1ull << 29ull)) ? "100G_CR4 " : "",
               (base_pg & (1ull << 30ull)) ? "25G_KRS_CRS " : "",
               (base_pg & (1ull << 31ull)) ? "25G_KR_CR " : "");
  }

  if (base_pg & (0xFull << 44ull)) {
    aim_printf(&uc->pvs,
               " : %s%s%s%s",
               (base_pg & (1ull << 44ull)) ? "f2 (25G-RS FEC REQUEST) " : "",
               (base_pg & (1ull << 45ull)) ? "f3 (25G-FC FEC REQUEST) " : "",
               (base_pg & (1ull << 46ull)) ? "f0 (10G-FC FEC ABILITY) " : "",
               (base_pg & (1ull << 47ull)) ? "f1 (10G-FC FEC REQUEST) " : "");
  }
  if (base_pg & (3ull << 10ull)) {
    aim_printf(&uc->pvs,
               " : %s%s",
               (base_pg & (1ull << 10ull)) ? "c0 (RX) " : "",
               (base_pg & (1ull << 11ull)) ? "c1 (TX) " : "");
  }
}

char *dump_speed_str(bf_port_speed_t speed) {
  return (speed == BF_SPEED_100G
              ? "100g"
              : speed == BF_SPEED_50G
                    ? " 50g"
                    : speed == BF_SPEED_40G
                          ? " 40g"
                          : speed == BF_SPEED_25G
                                ? " 25g"
                                : speed == BF_SPEED_10G
                                      ? " 10g"
                                      : speed == BF_SPEED_1G ? "  1g" : "????");
}

char *dump_fec_str(bf_fec_type_t fec) {
  return (fec == BF_FEC_TYP_NONE
              ? "--"
              : fec == BF_FEC_TYP_FIRECODE
                    ? "FC"
                    : fec == BF_FEC_TYP_REED_SOLOMON ? "RS" : "??");
}

char *dump_link_training_st_str(int st) {
  if (st == 0)
    return "F";
  else if (st == 1)
    return "C";
  else if (st == 2)
    return "R";
  return "-";
}

char *dump_arb_st_str(int arb_st) {
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
  return arb_to_str[arb_st];
}

int port_mgr_dump_this_an_status(ucli_context_t *uc,
                                 bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port) {
  bf_port_speed_t hcd;
  bf_fec_type_t fec;
  uint32_t av_hcd, arb_st;
  int ln, num_lanes;
  uint64_t lp_base_pg;
  bool lp_base_pg_rdy;
  bool lp_next_pg_rdy;
  bool an_good;
  bool an_complete;
  bool an_failed;
  uint32_t num_pgs;
  uint32_t hw_addr1, hw_addr2;
  uint64_t pgs[4];  // should never be other than 1 (no next-pg) or 3
                    // (consortium) really
  bf_status_t bf_status;

  bf_status = bf_port_num_lanes_get(dev_id, dev_port, &num_lanes);
  if (bf_status != BF_SUCCESS) return 0;

  for (ln = 0; ln < num_lanes; ln++) {
    int failed;
    int in_prg;
    int rx_trnd;
    int frm_lk;
    int rmt_rq;
    int lcl_rq;
    int rmt_rcvr_rdy;

    bf_serdes_link_training_st_extended_get(dev_id,
                                            dev_port,
                                            ln,
                                            &failed,
                                            &in_prg,
                                            &rx_trnd,
                                            &frm_lk,
                                            &rmt_rq,
                                            &lcl_rq,
                                            &rmt_rcvr_rdy);
    bf_serdes_hw_addr_get(dev_id,
                          dev_port,
                          ln,
                          false /*tx_dir*/,
                          &hw_addr1, /*ring*/
                          &hw_addr2 /*sd*/);
    if (ln == 0) {
      bf_status = bf_port_autoneg_advert_get(dev_id, dev_port, &num_pgs, pgs);
      if (bf_status != BF_SUCCESS) return 0;
      if (pgs[0] == 0ull) return 0;

      bf_status =
          bf_serdes_autoneg_lp_base_pg_get(dev_id, dev_port, 0, &lp_base_pg);
      if (bf_status != BF_SUCCESS) return 0;

      bf_status = bf_serdes_autoneg_all_state(dev_id,
                                              dev_port,
                                              0,
                                              &lp_base_pg_rdy,
                                              &lp_next_pg_rdy,
                                              &an_good,
                                              &an_complete,
                                              &an_failed);
      if (bf_status != BF_SUCCESS) return 0;
      bf_status =
          bf_serdes_autoneg_hcd_fec_get(dev_id, dev_port, &hcd, &fec, &av_hcd);
      if (bf_status != BF_SUCCESS) return 0;

      bf_serdes_mgmt_uc_int(dev_id, dev_port, ln, 0, 0x807, 0x1, &arb_st);

      // fix up fec mode for 100G
      if ((hcd == BF_SPEED_100G) && (fec == BF_FEC_TYP_FIRECODE)) {
        fec = BF_FEC_TYP_REED_SOLOMON;
      }
      aim_printf(&uc->pvs,
                 "|%d|%3d|%d|%d|%3d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%5s|%3s|",
                 dev_id,
                 dev_port,
                 ln,
                 hw_addr1,
                 hw_addr2,
                 failed,
                 in_prg,
                 frm_lk,
                 lcl_rq,
                 rmt_rq,
                 rx_trnd,
                 rmt_rcvr_rdy,
                 an_good ? 1 : 0,
                 an_complete ? 1 : 0,
                 dump_speed_str(hcd),
                 dump_fec_str(fec));

      dump_an_base_page(uc, pgs[0]);
      aim_printf(&uc->pvs, ": LP=> ");
      dump_an_base_page(uc, lp_base_pg);
      // arb_st seems worthless
      // aim_printf( &uc->pvs, " : %s", dump_arb_st_str(arb_st & 0xF));
      aim_printf(&uc->pvs, "\n");
    } else {
      aim_printf(&uc->pvs,
                 "| |   |%d|%d|%3d|%d|%d|%d|%d|%d|%d|%d|\n",
                 ln,
                 hw_addr1,
                 hw_addr2,
                 failed,
                 in_prg,
                 frm_lk,
                 lcl_rq,
                 rmt_rq,
                 rx_trnd,
                 rmt_rcvr_rdy);
    }
  }
  return num_lanes;
}

void dump_an_banner(ucli_context_t *uc) {
  aim_printf(&uc->pvs,
             "+-+---+-+-+---+-------------+------------------------------------"
             "---------------------+\n");
  aim_printf(&uc->pvs,
             "| |   | | |   |Link Training| AN                                 "
             "                     |\n");
  aim_printf(&uc->pvs,
             "| |   | | |   "
             "|-+-+-+-+-+-+-+-+-+-----+---+------------------------------------"
             "-------|\n");
  aim_printf(&uc->pvs,
             "| | p |l|r|   |f|a|l|l|r|t|r|g|c|     |   |                      "
             "                     |\n");
  aim_printf(&uc->pvs,
             "|d| o |a|i|   |a|c|o|r|r|r|r|o|p|     |   |                      "
             "                     |\n");
  aim_printf(&uc->pvs,
             "|e| r |n|n| s |i|t|c|e|e|n|d|o|l|     |   |                      "
             "                     |\n");
  aim_printf(&uc->pvs,
             "|v| t |e|g| d |l|v|k|q|q|d|y|d|t| hcd |fec| Local and Remote "
             "Base Page advertisements |\n");
  aim_printf(&uc->pvs,
             "+-+---+-+-+---+-+-+-+-+-+-+-+-+-+-----+---+----------------------"
             "---------------------+\n");
}

static ucli_status_t port_mgr_ucli_ucli__an__(ucli_context_t *uc) {
  bool show_target_dev_mssg = false;
  bf_dev_id_t dev_id;
  uint32_t pipe;
  int ports_per_banner = 32, ports_dumped = 0;
  int min_fp_port, max_fp_port, port;

  UCLI_COMMAND_INFO(uc, "an", -1, "Serdes AN status");

  dump_an_banner(uc);
  for (dev_id = 0; dev_id < BF_MAX_DEV_COUNT; dev_id++) {
    if (!port_mgr_dev_is_ready(dev_id)) continue;
    if (!port_mgr_dev_is_tof1(dev_id)) {
      show_target_dev_mssg = true;
      continue;
    }

    for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
      min_fp_port = lld_get_min_fp_port(dev_id);
      max_fp_port = lld_get_max_fp_port(dev_id);
      if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

      for (port = min_fp_port; port <= max_fp_port; port++) {
        bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

        if (ports_dumped == ports_per_banner) {
          dump_an_banner(uc);
          ports_dumped = 0;
        }
        ports_dumped += port_mgr_dump_this_an_status(uc, dev_id, dev_port);
      }
    }
    aim_printf(&uc->pvs, "Cpu port(s):\n");
    min_fp_port = lld_get_min_fp_port(dev_id);
    max_fp_port = lld_get_max_fp_port(dev_id);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      bf_dev_port_t dev_port = MAKE_DEV_PORT(0, port);

      port_mgr_dump_this_an_status(uc, dev_id, dev_port);
    }
  }
  if (show_target_dev_mssg) {
    aim_printf(&uc->pvs,
               "This command is deprecated and is only supported on Tofino 1 "
               "devices.\n");
    aim_printf(&uc->pvs, "Please use the \"port-an-info\" command instead.\n");
  }
  return 0;
}

static int match(char *s1, char *s2) {
  int len_s1 = strlen(s1);
  int len_s2 = strlen(s2);
  int min_cmn_len = (len_s1 < len_s2) ? len_s1 : len_s2;

  if (strncmp(s1, s2, min_cmn_len) == 0) {
    return 1;
  }
  return 0;
}

extern int port_mgr_dump_sbus_adrr_map(ucli_context_t *uc, bf_dev_id_t dev_id);
extern bf_status_t port_mgr_serdes_init(bf_dev_id_t dev_id);

static ucli_status_t port_mgr_ucli_ucli__prbs_m__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, dev_id, port, gb;
  int start, min_fp_port, max_fp_port;  // =1
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "prbsm",
                    7,
                    "Start or stop PRBS on multiple ports "
                    "<dev> <1-start,0-stop> <PRBS Speed/Gbps> "
                    "<map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  start = strtol(uc->pargs->args[1], NULL, 0);
  gb = strtol(uc->pargs->args[2], NULL, 0);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(dev_id);
    max_fp_port = lld_get_max_fp_port(dev_id);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      int inst;
      int sub_inst;
      int ring;
      int sd;
      int mac_block, ch;

      if ((map[pipe] & (1ull << port)) == 0) continue;
      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);
      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
      inst = mac_block;
      sub_inst = ch;
      rc = port_mgr_find_addr_for(
          dev_id, IP_TYPE_ETH_PMA, inst, sub_inst, &ring, &sd);
      if (rc == 0) {
        if (start) {
          if (gb == 25) {
            sd_init_prbs31_25g(dev_id, ring, sd, 0);
            aim_printf(
                &uc->pvs, "%d:%3d:%d: Set to 25G PRBS31\n", dev_id, ring, sd);
          } else if (gb == 10) {
            sd_init_prbs31_10g(dev_id, ring, sd, 0);
            aim_printf(
                &uc->pvs, "%d:%3d:%d: Set to 10G PRBS31\n", dev_id, ring, sd);
          } else {
            aim_printf(
                &uc->pvs, "Allowed PRBS31 speeds are 10 and 25 (gb=%d)\n", gb);
          }
        } else {
          port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x1, 0x0);
          aim_printf(
              &uc->pvs, "%d:%3d:%d: Tx/Rx/Drv disabled\n", dev_id, ring, sd);
        }
      }
    }
  }
  return 0;
}

extern void sd_aapl_serdes_main(int argc, char **argv, bf_dev_id_t dev_id);
extern void sd_aapl_eye_main(int argc, char *argv[], bf_dev_id_t dev_id);
extern int sd_aapl_diag(int argc, char *argv[], bf_dev_id_t dev_id);

static ucli_status_t port_mgr_ucli_ucli__sd__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int ring, sd;
  int rc;
  char *api;

  UCLI_COMMAND_INFO(uc, "sd", -1, "Serdes APIs");

  if (uc->pargs->count == 0) {
    aim_printf(&uc->pvs, "Available Commands:\n");
    aim_printf(&uc->pvs, "\tsd aapl_diag            dev_id\n");
    aim_printf(&uc->pvs, "\tsd aapl_serdes          dev_id\n");
    aim_printf(&uc->pvs, "\tsd aapl_eye             dev_id\n");
    aim_printf(&uc->pvs, "\tsd fw_dnld              dev_id\n");
    aim_printf(&uc->pvs, "\tsd unlock_pcie\n");
    aim_printf(&uc->pvs, "\tsd lock_pcie\n");

    aim_printf(&uc->pvs, "\tsd sbus_reset           dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd spico_reset          dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd rd                   dev_id ring sd reg\n");
    aim_printf(&uc->pvs, "\tsd wr                   dev_id ring sd reg data\n");
    aim_printf(&uc->pvs, "\tsd spico_int            dev_id ring sd int data\n");
    aim_printf(&uc->pvs, "\tsd all_int              int data\n");
    aim_printf(
        &uc->pvs,
        "\tsd init                 dev_id ring sd reset init_mode divider "
        "data_width phase_cal output_en\n");
    aim_printf(
        &uc->pvs,
        "\tsd op                   Dump operational status of serdes nodes\n");
    aim_printf(
        &uc->pvs,
        "\tsd tech                 Dump serdes info for techincal support\n");
    aim_printf(&uc->pvs, "\tsd chg_to_prbs          dev_id\n");
    aim_printf(&uc->pvs, "\tsd perf                 dev_id\n");
    aim_printf(&uc->pvs, "\tsd prbs_perf            <mav mon>\n");
    aim_printf(&uc->pvs, "\tsd dump_all_dfe         \n");
    aim_printf(&uc->pvs, "\tsd dump_all_vbtc        \n");
    aim_printf(&uc->pvs, "\tsd escope               dev_id\n");
    aim_printf(&uc->pvs, "\tsd xt                   dev_id\n");
    aim_printf(&uc->pvs, "\tsd serdes_init          dev_id\n");
    aim_printf(&uc->pvs, "\tsd dump_all_addr        dev_id\n");
    aim_printf(&uc->pvs, "\tsd all_eye_get          <mav mon>\n");
    aim_printf(&uc->pvs, "\tsd all_eye_show         <mav mon>\n");
    aim_printf(&uc->pvs, "\tsd eye_stats            <mav mon>\n");
    aim_printf(&uc->pvs,
               "\tsd all_bbgain           dev_id Tx_bbgain Rx_bbgain\n");
    aim_printf(&uc->pvs, "\tsd all_delay_cal        \n");
    aim_printf(&uc->pvs, "\tsd all_pi_cal           \n");
    aim_printf(&uc->pvs, "\tsd all_adaptive_pcal    \n");
    aim_printf(&uc->pvs, "\tsd all_pcal             \n");
    aim_printf(&uc->pvs, "\tsd all_ical             \n");
    aim_printf(&uc->pvs, "\tsd all_cmp_mode_prbs    \n");
    aim_printf(&uc->pvs, "\tsd all_tx_eq            pre atten post\n");
    aim_printf(&uc->pvs,
               "\tsd all_dfe_set          dfe_ctrl hf_val lf_val dc_val\n");
    aim_printf(&uc->pvs, "\tsd all_sig_ok_thresh_en <thresh: 1-255>>\n");
    aim_printf(&uc->pvs, "\tsd all_sig_ok_thresh    <thresh: 1-255>>\n");
    aim_printf(&uc->pvs, "\tsd all_swing            \n");
    aim_printf(&uc->pvs, "\tsd all_rptr_term_float  \n");
    aim_printf(&uc->pvs, "\tsd all_rptr_term_avdd   \n");
    aim_printf(&uc->pvs, "\tsd set_prbs_mode        <7,9,11,13,15,19,23,31>\n");
    aim_printf(&uc->pvs, "\tsd prbs_ilb             \n");
    aim_printf(&uc->pvs, "\tsd prbs_elb             \n");
    aim_printf(&uc->pvs, "\tsd prbs_10g_elb         \n");
    aim_printf(&uc->pvs, "\tsd prbs_25g_elb         \n");
    aim_printf(&uc->pvs, "\tsd 1_prbs_elb           dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd sweep_mult           dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd sweep1               dev_id\n");
    aim_printf(&uc->pvs, "\tsd rptr_sweep           dev_id\n");
    aim_printf(&uc->pvs, "\tsd core_data            dev_id\n");
    aim_printf(&uc->pvs,
               "\tsd tx_pll               dev_id ring sd expected_divider\n");
    aim_printf(&uc->pvs,
               "\tsd rx_pll               dev_id ring sd expected_divider\n");
    aim_printf(&uc->pvs, "\tsd get_tx_output_en     dev_id ring sd enable\n");
    aim_printf(&uc->pvs, "\tsd set_tx_output_en     dev_id ring sd enable\n");
    aim_printf(&uc->pvs, "\tsd get_signal_ok_thresh dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_signal_ok_thresh dev_id ring sd thresh\n");
    aim_printf(&uc->pvs, "\tsd get_elec_idle        dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd swing                dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd check_signal_ok      dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd get_error_count      dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd check_los            dev_id ring sd\n");
    aim_printf(&uc->pvs,
               "\tsd set_dfe              dev_id ring sd row col value\n");
    aim_printf(&uc->pvs, "\tsd dfe_ical             dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dfe_pcal             dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dfe_adaptive         dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dfe_stop_adaptive    dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dfe_stop             dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd check_dfe            dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dump_vbtc            dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dump_dfe             dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dump_eye             dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd plot_eye             dev_id ring sd\n");
    aim_printf(&uc->pvs,
               "\tsd set_tx_eq            dev_id ring sd pre atten post\n");
    aim_printf(&uc->pvs, "\tsd get_tx_eq            dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd get_tx_inv           dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_tx_inv           dev_id ring sd polarity\n");
    aim_printf(&uc->pvs, "\tsd get_rx_inv           dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_rx_inv           dev_id ring sd polarity\n");
    aim_printf(&uc->pvs, "\tsd rx_inject_error      dev_id ring sd num_bits\n");
    aim_printf(&uc->pvs, "\tsd tx_inject_error      dev_id ring sd num_bits\n");
#if 0
    aim_printf(&uc->pvs, "\tsd get_rx_data_qual     dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_rx_data_qual     dev_id ring sd qual\n");
#endif  // 0
    aim_printf(&uc->pvs, "\tsd get_tx_data_sel      dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_tx_data_sel      dev_id ring sd sel\n");
    aim_printf(&uc->pvs, "\tsd get_rx_cmp_mode      dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_rx_cmp_mode      dev_id ring sd mode\n");
    aim_printf(&uc->pvs, "\tsd get_rx_cmp_sel       dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_rx_cmp_sel       dev_id ring sd sel\n");
    aim_printf(&uc->pvs, "\tsd get_rx_term          dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_rx_term          dev_id ring sd term\n");
    aim_printf(&uc->pvs, "\tsd get_tx_pll_clk_src   dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_tx_pll_clk_src   dev_id ring sd clk\n");
    aim_printf(&uc->pvs, "\tsd get_spico_clk_src    dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd set_spico_clk_src    dev_id ring sd clk\n");
    aim_printf(&uc->pvs, "\tsd aapl-init            dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd dnld                 dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd map_eye              dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd crc                  dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd pmd                  dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd snsr                 dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd pos                  dev_id ring sd x y\n");
    aim_printf(&uc->pvs,
               "\tsd sym                  dev_id ring sd speed <1/10/25>\n");
    aim_printf(&uc->pvs,
               "\tsd asym_tx              dev_id ring sd speed <1/10/25> rx_en "
               "rx_speed\n");
    aim_printf(&uc->pvs,
               "\tsd asym_rx              dev_id ring sd speed <1/10/25> tx_en "
               "tx_speed\n");
    aim_printf(&uc->pvs,
               "\tsd aggressor <add del on off cal show> dev_id ring sd\n");
    aim_printf(&uc->pvs,
               "\tsd victim    <add del on off cal show> dev_id ring sd\n");
    aim_printf(&uc->pvs, "\tsd sd_other  <on off cal show>\n");
    aim_printf(&uc->pvs, "\tsd sd_all    <on off cal show>\n");
    aim_printf(&uc->pvs, "\tsd serdes_dump          dev_id ring sd\n");

    return 0;
  }

  // handle some cmds that don't require all the args
  api = (char *)uc->pargs->args[0];
  if (match(api, "aapl_diag")) {
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_aapl_diag(uc->pargs->count - 1, (char **)&uc->pargs->args[1], dev_id);
    return 0;
  }
  if (match(api, "aapl_serdes")) {
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_aapl_serdes_main(
        uc->pargs->count - 1, (char **)&uc->pargs->args[1], dev_id);
    return 0;
  }
  if (match(api, "aapl_eye")) {
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_aapl_eye_main(
        uc->pargs->count - 1, (char **)&uc->pargs->args[1], dev_id);
    return 0;
  }
  if (match(api, "fw-dnld")) {
    int i, n;
    if (uc->pargs->count != 3) {
      aim_printf(&uc->pvs, "usage: fw-dnld dev_id n\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    n = strtol(uc->pargs->args[2], NULL, 0);
    if (n < 0) {
      aim_printf(&uc->pvs, "n must be positive\n");
      return 0;
    }
    if (n > 1000) {
      aim_printf(&uc->pvs, "n must be less than 1000\n");
      return 0;
    }
    extern int port_mgr_serdes_bcast_fw_load(bf_dev_id_t dev_id);
    for (i = 0; i < n; i++) {
      aim_printf(&uc->pvs, "dnld %d\n", i);
      port_mgr_serdes_bcast_fw_load(dev_id);
    }
    return 0;
  }
  if (match(api, "all_sig_ok_thresh_en")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: all_sig_ok_thresh_en dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_sig_ok_thresh_en(dev_id);
    return 0;
  }
  if (match(api, "all_sig_ok_thresh")) {
    if (uc->pargs->count != 3) {
      aim_printf(&uc->pvs, "usage: all_sig_ok_thresh dev_id thresh\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    int thresh = strtol(uc->pargs->args[2], NULL, 0);
    sd_all_sig_ok_thresh(dev_id, thresh);
    return 0;
  }
  if (match(api, "all_int")) {
    uint32_t int_code, int_data;
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: all_int dev_id code data\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    int_code = strtoul(uc->pargs->args[2], NULL, 16);
    int_data = strtoul(uc->pargs->args[3], NULL, 16);
    sd_all_int(uc, dev_id, int_code, int_data);
    return 0;
  }
  if (match(api, "unlock_pcie")) {
    port_mgr_av_sd_unlock_pcie();
    return 0;
  }
  if (match(api, "lock_pcie")) {
    port_mgr_av_sd_lock_pcie();
    return 0;
  }
  if (match(api, "escope")) {
    sd_escope(uc->pargs->count, (char **)uc->pargs->args);
    return 0;
  }
  if (match(api, "aggressor")) {
    if (match((char *)uc->pargs->args[1], "add")) {
      if (uc->pargs->count != 5) {
        aim_printf(
            &uc->pvs,
            "Usage: sd aggressor <add del on off cal show> dev_id ring sd\n");
        return 0;
      }
      dev_id = strtol(uc->pargs->args[2], NULL, 0);
      ring = strtol(uc->pargs->args[3], NULL, 0);
      sd = strtol(uc->pargs->args[4], NULL, 0);

      sd_diag_aggressor_add(dev_id, ring, sd);
    } else if (match((char *)uc->pargs->args[1], "del")) {
      if (uc->pargs->count != 5) {
        aim_printf(
            &uc->pvs,
            "Usage: sd aggressor <add del on off cal show> dev_id ring sd\n");
        return 0;
      }
      dev_id = strtol(uc->pargs->args[2], NULL, 0);
      ring = strtol(uc->pargs->args[3], NULL, 0);
      sd = strtol(uc->pargs->args[4], NULL, 0);

      sd_diag_aggressor_del(dev_id, ring, sd);
    } else if (match((char *)uc->pargs->args[1], "on")) {
      sd_diag_aggressor_on();
    } else if (match((char *)uc->pargs->args[1], "off")) {
      sd_diag_aggressor_off();
    } else if (match((char *)uc->pargs->args[1], "cal")) {
      sd_diag_aggressor_cal();
    } else if (match((char *)uc->pargs->args[1], "show")) {
      sd_diag_aggressor_show(uc);
    } else {
      aim_printf(
          &uc->pvs,
          "Usage: sd aggressor <add del on off cal show> dev_id ring sd\n");
      return 0;
    }
    return 0;
  }
  if (match(api, "victim")) {
    if (match((char *)uc->pargs->args[1], "add")) {
      if (uc->pargs->count != 5) {
        aim_printf(
            &uc->pvs,
            "Usage: sd victim <add del on off cal show> dev_id ring sd\n");
        return 0;
      }
      dev_id = strtol(uc->pargs->args[2], NULL, 0);
      ring = strtol(uc->pargs->args[3], NULL, 0);
      sd = strtol(uc->pargs->args[4], NULL, 0);

      sd_diag_victim_add(dev_id, ring, sd);
    } else if (match((char *)uc->pargs->args[1], "del")) {
      if (uc->pargs->count != 5) {
        aim_printf(
            &uc->pvs,
            "Usage: sd victim <add del on off cal show> dev_id ring sd\n");
        return 0;
      }
      dev_id = strtol(uc->pargs->args[2], NULL, 0);
      ring = strtol(uc->pargs->args[3], NULL, 0);
      sd = strtol(uc->pargs->args[4], NULL, 0);

      sd_diag_victim_del(dev_id, ring, sd);
    } else if (match((char *)uc->pargs->args[1], "on")) {
      sd_diag_victim_on();
    } else if (match((char *)uc->pargs->args[1], "off")) {
      sd_diag_victim_off();
    } else if (match((char *)uc->pargs->args[1], "cal")) {
      sd_diag_victim_cal();
    } else if (match((char *)uc->pargs->args[1], "show")) {
      sd_diag_victim_show(uc);
    } else {
      aim_printf(&uc->pvs,
                 "Usage: sd victim <add del on off cal show> dev_id ring sd\n");
      return 0;
    }
    return 0;
  }
  if (match(api, "sd_other")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "Usage: sd sd_other <on off cal show>\n");
      return 0;
    }
    if (match((char *)uc->pargs->args[1], "on")) {
      sd_diag_other_on();
    } else if (match((char *)uc->pargs->args[1], "off")) {
      sd_diag_other_off();
    } else if (match((char *)uc->pargs->args[1], "cal")) {
      sd_diag_other_cal();
    } else if (match((char *)uc->pargs->args[1], "show")) {
      sd_diag_other_show(uc);
    } else {
      aim_printf(&uc->pvs, "Usage: sd sd_other <on off cal show>\n");
      return 0;
    }
    return 0;
  }
  if (match(api, "sd_all")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "Usage: sd sd_all <on off cal show>\n");
      return 0;
    }
    if (match((char *)uc->pargs->args[1], "on")) {
      sd_diag_all_on();
    } else if (match((char *)uc->pargs->args[1], "off")) {
      sd_diag_all_off();
    } else if (match((char *)uc->pargs->args[1], "cal")) {
      sd_diag_all_cal();
    } else if (match((char *)uc->pargs->args[1], "show")) {
      sd_diag_all_show(uc);
    } else {
      aim_printf(&uc->pvs, "Usage: sd sd_all <on off cal show>\n");
      return 0;
    }
    return 0;
  }
  if (match(api, "dump_all_vbtc")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd dump_all_vbtc dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_dump_all_vbtc(uc, dev_id);
    return 0;
  }
  if (match(api, "prbs_perf")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd prbs_perf <mav mon>\n");
      return 0;
    }
    if (uc->pargs->args[1][1] == 'a') {
      sd_dump_all_perf_prbs(uc, true);
    } else {
      sd_dump_all_perf_prbs(uc, false);
    }
    return 0;
  }
  if (match(api, "perf")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd perf <mav mon>\n");
      return 0;
    }
    if (uc->pargs->args[1][1] == 'a') {
      sd_dump_all_perf(uc, true);
    } else {
      sd_dump_all_perf(uc, false);
    }
    return 0;
  }
  if (match(api, "chg_to_prbs")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd chg_to_prbs dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_switch_to_prbs(uc, dev_id);
    return 0;
  }
  if (match(api, "dump_all_dfe")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd dump_all_dfe dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_dump_all_dfe(uc, dev_id);
    return 0;
  }
  if (match(api, "tech")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd tech dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_tech_sppt_dump(dev_id);
    return 0;
  }
  if (match(api, "xt")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd xt dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_xtalk_test(uc, dev_id);
    return 0;
  }
  if (match(api, "aapl-init")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd aapl-init dev_id\n");
      aim_printf(&uc->pvs, "       sd aapl-init 0\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    if (dev_id >= BF_MAX_DEV_COUNT) {
      aim_printf(&uc->pvs, "Invalid dev_id\n");
      return 0;
    }
    port_mgr_av_sd_init_aapl(dev_id, 0);
    return 0;
  }
  if (match(api, "serdes_init")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd serdes_init dev_id\n");
      aim_printf(&uc->pvs, "       sd serdes_init 0\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    port_mgr_serdes_init(dev_id);
    return 0;
  }
  if (match(api, "dump_all_addr")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd dump_all_addr dev_id\n");
      aim_printf(&uc->pvs, "       sd dump_all_addr 0\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    port_mgr_dump_sbus_adrr_map(uc, dev_id);
    return 0;
  }
  if (match(api, "all_eye_get")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_eye_get <mav mon>\n");
      return 0;
    }
    if (uc->pargs->args[1][1] == 'a') {
      sd_perf_stats_collect_on_demand(uc, true);
    } else {
      sd_perf_stats_collect_on_demand(uc, false);
    }
    return 0;
  }
  if (match(api, "all_eye_show")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_eye_show <mav mon>\n");
      return 0;
    }
    if (uc->pargs->args[1][1] == 'a') {
      sd_perf_stats_display_on_demand(uc, true);
    } else {
      sd_perf_stats_display_on_demand(uc, false);
    }
    return 0;
  }
  if (match(api, "eye_stats")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd eye_stats <mav mon>\n");
      return 0;
    }
    if (uc->pargs->args[1][1] == 'a') {
      sd_perf_stats_collect(uc, true);
    } else {
      sd_perf_stats_collect(uc, false);
    }
    return 0;
  }
  if (match(api, "all_bbgain")) {
    uint32_t tx_val, rx_val;
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd all_bbgain <dev> <tx> <rx>\n");
      return 0;
    }

    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    tx_val = strtoul(uc->pargs->args[2], NULL, 16);
    rx_val = strtoul(uc->pargs->args[3], NULL, 16);
    sd_all_bbgain(uc, dev_id, tx_val, rx_val);
    aim_printf(&uc->pvs, "Tx PLL: %x : Rx PLL: %x \n", tx_val, rx_val);
    return 0;
  }
  if (match(api, "all_delay_cal")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_delay_cal dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_delay_cal(uc, dev_id);
    return 0;
  }
  if (match(api, "all_pi_cal")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_pi_cal dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_pi_cal(uc, dev_id);
    return 0;
  }
  if (match(api, "all_adaptive_pcal")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_adaptive_pcal dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_adaptive_pcal(uc, dev_id);
    return 0;
  }
  if (match(api, "all_pcal")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_pcal dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_pcal(uc, dev_id);
    return 0;
  }
  if (match(api, "all_ical")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_ical dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_ical(uc, dev_id);
    return 0;
  }
  if (match(api, "all_cmp_mode_prbs")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_cmp_mode_prbs dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_cmp_mode_prbs31_all(dev_id);
    return 0;
  }
  if (match(api, "all_tx_eq")) {
    int pre, post, atten;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd all_tx_eq dev_id pre atten post\n");
      aim_printf(&uc->pvs, "       sd all_tx_eq 0      0   1     3\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    pre = strtol(uc->pargs->args[2], NULL, 0);
    atten = strtol(uc->pargs->args[3], NULL, 0);
    post = strtol(uc->pargs->args[4], NULL, 0);

    sd_tx_eq_all(dev_id, pre, atten, post);
    return 0;
  }
  if (match(api, "all_dfe_set")) {
    uint32_t dfe_ctrl, hf_val, lf_val, dc_val;

    if (uc->pargs->count != 6) {
      aim_printf(&uc->pvs,
                 "usage: sd all_dfe_set <dev_id> <dfe-ctrl> <hf_val> <lf_val> "
                 "<dc_val>\n");
      aim_printf(&uc->pvs, "dfe_ctrl is OR of:\n");
      aim_printf(&uc->pvs, "  0  = DEFAULT\n");
      aim_printf(&uc->pvs, "  0  = ICAL (note: default)\n");
      aim_printf(&uc->pvs, "  1  = PCAL\n");
      aim_printf(&uc->pvs, "  2  = SEEDED_HF\n");
      aim_printf(&uc->pvs, "  4  = SEEDED_LF\n");
      aim_printf(&uc->pvs, "  8  = SEEDED_DC\n");
      aim_printf(&uc->pvs, "  16 = FIXED_HF\n");
      aim_printf(&uc->pvs, "  32 = FIXED_LF\n");
      aim_printf(&uc->pvs, "  64 = FIXED_DC\n");

      aim_printf(&uc->pvs, "       sd all_dfe_set 0 0 0 0 0(DEFAULT)\n");
      aim_printf(&uc->pvs, "       sd all_dfe_set 0 16 15 0 0(FIXED_HF=15)\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    dfe_ctrl = strtol(uc->pargs->args[2], NULL, 0);
    hf_val = strtol(uc->pargs->args[3], NULL, 0);
    lf_val = strtol(uc->pargs->args[4], NULL, 0);
    dc_val = strtol(uc->pargs->args[5], NULL, 0);
    sd_all_dfe_set(dev_id, dfe_ctrl, hf_val, lf_val, dc_val);
    return 0;
  }
  if (match(api, "all_swing")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_swing dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_swing(uc, dev_id);
    return 0;
  }
  if (match(api, "all_rptr_term_float")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_rptr_term_float dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_rptr_term_float(uc, dev_id);
    return 0;
  }
  if (match(api, "all_rptr_term_avdd")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd all_rptr_term_avdd dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_all_rptr_term_avdd(uc, dev_id);
    return 0;
  }
  if (match(api, "prbs_ilb")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd prbs_ilb dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_init_all_prbs(uc, dev_id, 1 /*ILB*/, 25);
    return 0;
  }
  if (match(api, "prbs_10g_elb")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd prbs_10g_elb dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_init_all_prbs(uc, dev_id, 0 /*ELB*/, 10);
    return 0;
  }
  if (match(api, "prbs_25g_elb")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd prbs_25g_elb dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_init_all_prbs(uc, dev_id, 0 /*ELB*/, 25);
    return 0;
  }
  if (match(api, "prbs_elb")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd prbs_elb dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_init_all_prbs(uc, dev_id, 0 /*ELB*/, 25);
    return 0;
  }
  if (match(api, "core_data")) {
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "usage: sd core_data dev_id\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    sd_init_all_core_data(uc, dev_id);
    return 0;
  }
  if (match(api, "dnld")) {
    uint32_t rtn_data;
    // port_mgr_dev_t *dev_p;
    port_mgr_tof1_pdev_t *dev_p;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dnld dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dnld 0    0    1\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    ring = strtol(uc->pargs->args[2], NULL, 0);
    sd = strtol(uc->pargs->args[3], NULL, 0);

    if (ring >= 2 || ring < 0) {
      aim_printf(&uc->pvs,
                 "invalid ring: ring=%d , valid ring value is 0 or 1 \n",
                 ring);
      return 0;
    }
    if (!port_mgr_is_valid_sbus_address(dev_id, ring, sd)) {
      aim_printf(&uc->pvs,
                 "invalid sbus address: dev=%d ring=%d sd=%d\n",
                 dev_id,
                 ring,
                 sd);
      return 0;
    }

    // dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
    dev_p = port_mgr_dev_physical_dev_get(dev_id);

    if (dev_p == NULL) {
      aim_printf(&uc->pvs, "Invalid dev_id: %d\n", dev_id);
      return 0;
    }
    rtn_data = port_mgr_av_sd_load_firmware(
        dev_id, ring, sd, dev_p->serdes_fw_ver, dev_p->serdes_fw);
    aim_printf(
        &uc->pvs, "%d returned from port_mgr_av_sd_load_firmware\n", rtn_data);
    return 0;
  }
  if (match(api, "map_eye")) {
    uint32_t rtn_data;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd map_eye dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd map_eye 0    0    1\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    ring = strtol(uc->pargs->args[2], NULL, 0);
    sd = strtol(uc->pargs->args[3], NULL, 0);

    rtn_data = port_mgr_av_sd_map_eye(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d returned from port_mgr_av_sd_map_eye\n", rtn_data);
    return 0;
  }
  if (match(api, "crc")) {
    uint32_t rtn_data;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd crc dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd crc 0    0    1\n");
      return 0;
    }
    dev_id = strtol(uc->pargs->args[1], NULL, 0);
    ring = strtol(uc->pargs->args[2], NULL, 0);
    sd = strtol(uc->pargs->args[3], NULL, 0);

    rtn_data = port_mgr_av_sd_check_crc(dev_id, ring, sd);
    aim_printf(
        &uc->pvs, "%d returned from port_mgr_av_sd_check_crc\n", rtn_data);
    return 0;
  }
  if (match(api, "set_prbs_mode")) {
    uint32_t mode;
    if (uc->pargs->count != 2) {
      aim_printf(&uc->pvs, "Valid PRBS mode are: 7,9,11,15,23,31\n");
      return 0;
    }
    mode = strtol(uc->pargs->args[1], NULL, 0);
    sd_set_prbs_mode(uc, mode);
    return 0;
  }
  // all subsequent cmds require 4 or more args

  if (uc->pargs->count < 4) {
    aim_printf(&uc->pvs, "usage: sd <api> dev_id ring sd [...]\n");
    return 0;
  }
  dev_id = strtol(uc->pargs->args[1], NULL, 0);
  ring = strtol(uc->pargs->args[2], NULL, 0);
  sd = strtol(uc->pargs->args[3], NULL, 0);
  if (dev_id < 0 || ring < 0 || sd < 0 ||
      ring >= port_mgr_num_sbus_rings_get(dev_id)) {
    aim_printf(&uc->pvs, "Invalid argument value specified.\n");
    return 0;
  }

  // if (port_mgr_av_sd_get_aapl(dev_id) == NULL) {
  //  sd_init_aapl_and_load_firmware(dev_id, 1);
  //}

  if (match(api, "init")) {
    int reset, init_mode, divider, data_width, phase_cal, output_en;

    if (uc->pargs->count != 10) {
      aim_printf(
          &uc->pvs,
          "usage: sd init dev_id ring sd reset init_mode divider data_width "
          "phase_cal output_en\n");
      aim_printf(
          &uc->pvs,
          "       sd init 0    0    1  1     1         10      20         0    "
          "     1\n");
      aim_printf(
          &uc->pvs,
          "init_mode:\n\tAVAGO_PRBS31_ILB\n\tAVAGO_PRBS31_ELB\n\tAVAGO_CORE_"
          "DATA_ILB\n\tAVAGO_CORE_DATA_ELB\n\tAVAGO_INIT_ONLY\n");
      return 0;
    }
    reset = strtol(uc->pargs->args[4], NULL, 0);
    init_mode = strtol(uc->pargs->args[5], NULL, 0);
    divider = strtol(uc->pargs->args[6], NULL, 0);
    data_width = strtol(uc->pargs->args[7], NULL, 0);
    phase_cal = strtol(uc->pargs->args[8], NULL, 0);
    output_en = strtol(uc->pargs->args[9], NULL, 0);

    rc = port_mgr_av_sd_init(dev_id,
                             ring,
                             sd,
                             reset,
                             init_mode,
                             divider,
                             data_width,
                             phase_cal,
                             output_en);
    if (rc != 0) {
      aim_printf(&uc->pvs, "Error: %d from port_mgr_av_sd_init\n", rc);
    }
  } else if (match(api, "tx_pll")) {
    uint32_t expected_divider;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd tx_pll dev_id ring sd expected_divider\n");
      aim_printf(&uc->pvs, "       sd tx_pll 0    0    1  10\n");
      return 0;
    }
    expected_divider = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_check_tx_pll_state(dev_id, ring, sd, expected_divider);
    if (rc != 0) {
      aim_printf(
          &uc->pvs, "Error: %d from port_mgr_av_sd_check_tx_pll_state\n", rc);
    } else {
      aim_printf(
          &uc->pvs, "Tx PLL matches expected divider <%d>\n", expected_divider);
    }
  } else if (match(api, "1_prbs_elb")) {
    sd_init_prbs31_25g(dev_id, ring, sd, 0);
    return 0;
  } else if (match(api, "1_prbs_ilb")) {
    sd_init_prbs31_25g(dev_id, ring, sd, 1);
    return 0;
  } else if (match(api, "sweep_mult")) {
    sd_run_sweep_mult(uc, dev_id, ring, sd);
    return 0;
  } else if (match(api, "sweep1")) {
    sd_run_sweep_single(uc, dev_id, ring, sd);
    return 0;
  } else if (match(api, "rptr_sweep")) {
    extern void rptr_sweep(void);
    rptr_sweep();
    return 0;
  } else if (match(api, "sweep_ctle")) {
    sd_run_sweep_ctle(uc, dev_id, ring, sd);
    return 0;
  } else if (match(api, "rx_pll")) {
    uint32_t expected_divider;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd rx_pll dev_id ring sd expected_divider\n");
      aim_printf(&uc->pvs, "       sd rx_pll 0    0    1  10\n");
      return 0;
    }
    expected_divider = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_check_rx_pll_state(dev_id, ring, sd, expected_divider);
    if (rc != 0) {
      aim_printf(
          &uc->pvs, "Error: %d from port_mgr_av_sd_check_rx_pll_state\n", rc);
    } else {
      aim_printf(
          &uc->pvs, "Rx PLL matches expected divider <%d>\n", expected_divider);
    }
  } else if (match(api, "get_tx_output_en")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_tx_output_en dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_tx_output_en 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_get_tx_output_en(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d from port_mgr_av_sd_get_tx_output_en\n", rc);
  } else if (match(api, "set_tx_output_en")) {
    int en;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd set_tx_output_en dev_id ring sd enable\n");
      aim_printf(&uc->pvs, "       sd set_tx_output_en 0    0    1  1\n");
      return 0;
    }
    en = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_set_tx_output_en(dev_id, ring, sd, en);
    if (rc != 0) {
      aim_printf(
          &uc->pvs, "Error: %d from port_mgr_av_sd_set_tx_output_en\n", rc);
    }
  } else if (match(api, "rng")) {
    int num_sd;
    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd rng dev_id ring start_sd num_sd\n");
      aim_printf(&uc->pvs, "       sd op 0    0    11 4\n");
      return 0;
    }
    num_sd = strtol(uc->pargs->args[4], NULL, 0);
    sd_dump_this_sd_range(uc, dev_id, ring, sd, num_sd);
  } else if (match(api, "op")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd op dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd op 0    0    1\n");
      return 0;
    }
    sd_dump_this_sd(uc, dev_id, ring, sd);
  } else if (match(api, "get_signal_ok_thresh")) {
    int thresh;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_signal_ok_thresh dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_signal_ok_thresh 0    0    1\n");
      return 0;
    }
    thresh = port_mgr_av_sd_signal_ok_thresh_get(dev_id, ring, sd);
    aim_printf(
        &uc->pvs, "%d from port_mgr_av_sd_signal_ok_thresh_get\n", thresh);
  } else if (match(api, "set_signal_ok_thresh")) {
    int thresh;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd set_signal_ok_thresh dev_id ring sd thresh\n");
      aim_printf(&uc->pvs, "       sd set_signal_ok_thresh 0    0    1  8\n");
      return 0;
    }
    thresh = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_signal_ok_thresh_set(dev_id, ring, sd, thresh);
    if (rc != 0) {
      aim_printf(
          &uc->pvs, "Error: %d from port_mgr_av_sd_signal_ok_thresh_set\n", rc);
    }
  } else if (match(api, "swing")) {
    uint32_t swing_lo, swing_hi;
    uint32_t lo_los_neq_0, lo_sig_ok_eq_0, lo_ei_neq_0, calibrated;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd swing dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd swing 0      0    1\n");
      return 0;
    }
    sd_swing(dev_id,
             ring,
             sd,
             &swing_lo,
             &swing_hi,
             &lo_los_neq_0,
             &lo_sig_ok_eq_0,
             &lo_ei_neq_0,
             &calibrated);
    aim_printf(
        &uc->pvs,
        "swing = %dmV - %dmV : LOS=%dmv : sig_ok=%dmv : EI=%dmv : cal=%d\n",
        swing_lo,
        swing_hi,
        lo_los_neq_0,
        lo_sig_ok_eq_0,
        lo_ei_neq_0,
        calibrated);
  } else if (match(api, "get_elec_idle")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_elec_idle dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_elec_idle 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_elec_idle_get(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d from port_mgr_av_sd_elec_idle_get\n", rc);
  } else if (match(api, "check_signal_ok")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd check_signal_ok dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd check_signal_ok 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_signal_ok_get(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d from port_mgr_av_sd_signal_ok_get\n", rc);
  } else if (match(api, "check_los")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd check_los dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd check_los 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_los_get(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d from port_mgr_av_sd_los_get\n", rc);
  } else if (match(api, "sbus_reset")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd sbus_reset dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd sbus_reset 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_reset(dev_id, ring, sd, true, false);
    aim_printf(&uc->pvs, "Done.\n");
  } else if (match(api, "spico_reset")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd spico_reset dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd spico_reset 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_reset(dev_id, ring, sd, false, true);
    aim_printf(&uc->pvs, "Done.\n");
  } else if (match(api, "get_error_count")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_error_count dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_error_count 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d\n", rc);
  } else if (match(api, "dfe_ical")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dfe_ical dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dfe_ical 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_start_dfe_ical(dev_id, ring, sd);
    aim_printf(&uc->pvs, "ICAL DFE started..\n");
  } else if (match(api, "dfe_pcal")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dfe_pcal dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dfe_pcal 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_start_dfe_pcal(dev_id, ring, sd);
    aim_printf(&uc->pvs, "PCAL DFE started..\n");
  } else if (match(api, "dfe_adaptive")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dfe_adaptive dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dfe_adaptive 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_start_dfe_adaptive(dev_id, ring, sd);
    aim_printf(&uc->pvs, "ADAPTIVE DFE started..\n");
  } else if (match(api, "dfe_stop_adaptive")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dfe_stop_adaptive dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dfe_stop_adaptive 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_stop_dfe_adaptive(dev_id, ring, sd);
    aim_printf(&uc->pvs, "ADAPTIVE DFE stopped..\n");
  } else if (match(api, "dfe_stop")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dfe_stop dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dfe_stop 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_stop_dfe(dev_id, ring, sd);
    aim_printf(&uc->pvs, "ICAL/PCAL DFE stopped..\n");
  } else if (match(api, "check_dfe")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd check_dfe dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd check_dfe 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_check_dfe_running(dev_id, ring, sd);
    aim_printf(&uc->pvs, "%d from port_mgr_av_sd_check_dfe_running\n", rc);
  } else if (match(api, "dump_dfe")) {
    Avago_serdes_dfe_state_t dfe_state;
    Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
    int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dump_dfe dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dump_dfe 0    0    1\n");
      return 0;
    }
    memset(&dfe_state, 0, sizeof(dfe_state));
    avago_serdes_get_dfe_state(aapl, sbus_addr, &dfe_state);
    avago_serdes_print_dfe(aapl, sbus_addr, &dfe_state, FALSE);
    avago_write_dfe_state(stdout, &dfe_state);
    if (0) {
      BOOL single_line = TRUE;
      char *buf = avago_serdes_dfe_state_to_str(
          aapl, sbus_addr, &dfe_state, single_line, TRUE);
      aim_printf(&uc->pvs, "%s\n", buf);
      free(buf);
    }
    return 0;
  } else if (match(api, "dump_eye")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dump_eye dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dump_eye 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_dump_eye(dev_id, ring, sd, 0, uc);
    return 0;
  } else if (match(api, "dump_vbtc")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd dump_vbtc dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd dump_vbtc 0    0    1\n");
      return 0;
    }
    port_mgr_av_sd_dump_vbtc(dev_id, ring, sd);
    return 0;
  } else if (match(api, "plot_eye")) {
    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd plot_eye dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd plot_eye 0    0    1\n");
      return 0;
    }
    rc = port_mgr_av_sd_dump_eye(dev_id, ring, sd, 1, uc);
    if (rc != 0) {
      aim_printf(&uc->pvs, "Eye bad or non-existent\n");
    }
    return 0;
  } else if (match(api, "set_tx_eq")) {
    Avago_serdes_tx_eq_t tx_eq = {0};
    Avago_serdes_tx_eq_limits_t limits = {0};
    Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
    int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
    int pre, post, atten, slew;

    if (uc->pargs->count != 8) {
      aim_printf(&uc->pvs,
                 "usage: sd tx_eq dev_id ring sd pre atten post slew\n");
      aim_printf(&uc->pvs, "       sd tx_eq 0    0    1  0   1     3    0\n");
      return 0;
    }
    pre = strtol(uc->pargs->args[4], NULL, 0);
    atten = strtol(uc->pargs->args[5], NULL, 0);
    post = strtol(uc->pargs->args[6], NULL, 0);
    slew = strtol(uc->pargs->args[7], NULL, 0);

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
      return 0;
    }
    tx_eq.pre = pre;
    tx_eq.post = post;
    tx_eq.atten = atten;
    tx_eq.slew = slew;
    rc = avago_serdes_set_tx_eq(aapl, sbus_addr, &tx_eq);
    if (rc) {
      aim_printf(&uc->pvs, "Error : %d : from avago_serdes_set_tx_eq\n", rc);
      return 0;
    }
  } else if (match(api, "get_tx_eq")) {
    Avago_serdes_tx_eq_t tx_eq = {0};
    Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
    int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

    rc = avago_serdes_get_tx_eq(aapl, sbus_addr, &tx_eq);
    if (rc) {
      aim_printf(&uc->pvs, "Error : %d : from avago_serdes_get_tx_eq\n", rc);
      return 0;
    }
    aim_printf(&uc->pvs,
               "pre=%d  atten=%d  post=%d  slew=%d\n",
               tx_eq.pre,
               tx_eq.atten,
               tx_eq.post,
               tx_eq.slew);
  } else if (match(api, "get_tx_inv")) {
    int polarity;

    polarity = port_mgr_av_sd_tx_invert_get(dev_id, ring, sd);
    aim_printf(
        &uc->pvs, "%d returned from port_mgr_av_sd_tx_invert_get\n", polarity);
  } else if (match(api, "get_rx_inv")) {
    int polarity;

    polarity = port_mgr_av_sd_rx_invert_get(dev_id, ring, sd);
    aim_printf(
        &uc->pvs, "%d returned from port_mgr_av_sd_rx_invert_get\n", polarity);
  } else if (match(api, "set_tx_inv")) {
    int polarity;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_tx_inv dev_id ring sd polarity\n");
      aim_printf(&uc->pvs, "       sd set_tx_inv 0    0    1  0\n");
      return 0;
    }
    polarity = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_tx_invert_set(dev_id, ring, sd, polarity);
    if (rc) {
      aim_printf(
          &uc->pvs, "%d returned from port_mgr_av_sd_tx_invert_get\n", rc);
    }
  } else if (match(api, "set_rx_inv")) {
    int polarity;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_rx_inv dev_id ring sd polarity\n");
      aim_printf(&uc->pvs, "       sd set_rx_inv 0    0    1  0\n");
      return 0;
    }
    polarity = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_rx_invert_set(dev_id, ring, sd, polarity);
    if (rc) {
      aim_printf(
          &uc->pvs, "%d returned from port_mgr_av_sd_rx_invert_set\n", rc);
    }
  } else if (match(api, "tx_inject_error")) {
    int num_bits;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd tx_inject_error dev_id ring sd num_bits\n");
      aim_printf(&uc->pvs, "       sd tx_inject_error 0    0    1  8\n");
      return 0;
    }
    num_bits = strtol(uc->pargs->args[4], NULL, 0);
    rc = port_mgr_av_sd_tx_error_inject_set(dev_id, ring, sd, num_bits);
    if (rc) {
      aim_printf(&uc->pvs,
                 "%d returned from port_mgr_av_sd_tx_error_inject_set\n",
                 rc);
    }
  } else if (match(api, "rx_inject_error")) {
    int num_bits;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd rx_inject_error dev_id ring sd num_bits\n");
      aim_printf(&uc->pvs, "       sd rx_inject_error 0    0    1  8\n");
      return 0;
    }
    num_bits = strtol(uc->pargs->args[4], NULL, 0);
    if ((num_bits < 0) || (num_bits > 1000)) {
      aim_printf(&uc->pvs, "num_bits must be 1-1000\n");
      return 0;
    }
    rc = port_mgr_av_sd_rx_error_inject_set(dev_id, ring, sd, num_bits);
    if (rc) {
      aim_printf(&uc->pvs,
                 "%d returned from port_mgr_av_sd_rx_error_inject_set\n",
                 rc);
    }
#if 0
  } else if (match(api, "get_rx_data_qual")) {
extern Avago_serdes_rx_data_qual_t port_mgr_av_sd_get_rx_data_qual(bf_dev_id_t dev_id, int ring, int sd);
    int qual;
    Avago_serdes_data_qual_t dq;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_rx_data_qual dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_rx_data_qual 0    0    1\n");
      return 0;
    }
    qual = port_mgr_av_sd_get_rx_data_qual(dev_id, ring, sd);
    dq.d6_data_qual = qual;
    aim_printf(&uc->pvs,
               "%d (%s) returned from port_mgr_av_sd_rx_data_qual_get\n",
               qual,
               aapl_data_qual_to_str(dq));

  }
  else if (match(api, "set_rx_data_qual")) {
    int qual;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_rx_data_qual dev_id ring sd qual\n");
      aim_printf(&uc->pvs, "       sd set_rx_data_qual 0    0    1\n");
      return 0;
    }
    qual = strtol(uc->pargs->args[4], NULL, 0);
    Avago_serdes_data_qual_t dq;
    dq.d6_data_qual = qual;
    rc = port_mgr_av_sd_set_rx_data_qual(dev_id, ring, sd, qual);
    aim_printf(&uc->pvs,
               "%d (%s) returned from iport_mgr_av_sd_set_rx_data_qual\n",
               qual,
               aapl_data_qual_to_str(dq));
#endif  // 0
  } else if (match(api, "set_tx_data_sel")) {
    int sel;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_tx_data_sel dev_id ring sd sel\n");
      aim_printf(&uc->pvs, "       sd set_tx_data_sel 0    0    1  8\n");
      return 0;
    }
    sel = strtol(uc->pargs->args[4], NULL, 0);
    if ((sel > BF_SDS_PAT_PATSEL_FIXED) || (sel < BF_SDS_PAT_PATSEL_OFF)) {
      aim_printf(
          &uc->pvs,
          "Valid data select types are:\n"
          "BF_SDS_PAT_PATSEL_OFF    = 0\n"   /**< Select Core Data */
          "BF_SDS_PAT_PATSEL_PRBS7  = 1\n"   /**< Select PRBS-7   Pattern */
          "BF_SDS_PAT_PATSEL_PRBS9  = 2\n"   /**< Select PRBS-9   Pattern */
          "BF_SDS_PAT_PATSEL_PRBS11 = 3\n"   /**< Select PRBS-11  Pattern */
          "BF_SDS_PAT_PATSEL_PRBS15 = 4\n"   /**< Select PRBS-15  Pattern */
          "BF_SDS_PAT_PATSEL_PRBS23 = 5\n"   /**< Select PRBS-23  Pattern */
          "BF_SDS_PAT_PATSEL_PRBS31 = 6\n"   /**< Select PRBS-31  Pattern */
          "BF_SDS_PAT_PATSEL_FIXED  = 7\n"); /**< Select 80b Fixed Pattern */
      return 0;
    }
    rc = port_mgr_av_sd_tx_data_sel_set(dev_id, ring, sd, sel);
    if (rc) {
      aim_printf(
          &uc->pvs, "%d returned from port_mgr_av_sd_tx_data_sel_set\n", rc);
    }
  } else if (match(api, "get_tx_data_sel")) {
    int sel;
    char *pat_sel_to_str[] = {"BF_SDS_PAT_PATSEL_OFF    = 0\n",
                              "BF_SDS_PAT_PATSEL_PRBS7  = 1\n",
                              "BF_SDS_PAT_PATSEL_PRBS9  = 2\n",
                              "BF_SDS_PAT_PATSEL_PRBS11 = 3\n",
                              "BF_SDS_PAT_PATSEL_PRBS15 = 4\n",
                              "BF_SDS_PAT_PATSEL_PRBS23 = 5\n",
                              "BF_SDS_PAT_PATSEL_PRBS31 = 6\n",
                              "BF_SDS_PAT_PATSEL_FIXED  = 7\n"};

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_tx_data_sel dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_tx_data_sel 0    0    1\n");
      return 0;
    }
    sel = port_mgr_av_sd_tx_data_sel_get(dev_id, ring, sd);
    if ((sel > BF_SDS_PAT_PATSEL_FIXED) || (sel < BF_SDS_PAT_PATSEL_OFF)) {
      aim_printf(&uc->pvs, "Error: invalid pattern select: %d\n", sel);
    } else {
      aim_printf(&uc->pvs, "%d: %s", sel, pat_sel_to_str[sel]);
    }
    return 0;
  } else if (match(api, "get_rx_cmp_sel")) {
    int sel;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_rx_cmp_sel dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_rx_cmp_sel 0    0    1\n");
      return 0;
    }
    sel = port_mgr_av_sd_get_rx_cmp_sel(dev_id, ring, sd);
    aim_printf(&uc->pvs,
               "%d (%s) returned from port_mgr_av_sd_get_rx_cmp_sel\n",
               sel,
               aapl_cmp_data_to_str(sel));
  } else if (match(api, "set_rx_cmp_sel")) {
    int sel;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_rx_cmp_sel dev_id ring sd sel\n");
      aim_printf(&uc->pvs, "       sd set_rx_cmp_sel 0    0    1  8\n");
      return 0;
    }
    sel = strtol(uc->pargs->args[4], NULL, 0);
    if ((sel > AVAGO_SERDES_RX_CMP_DATA_OFF) || (sel == 6)) {
      aim_printf(&uc->pvs,
                 "Valid cmp select types are:\n"
                 "AVAGO_SERDES_RX_CMP_DATA_PRBS7    = 0\n"
                 "AVAGO_SERDES_RX_CMP_DATA_PRBS9    = 1\n"
                 "AVAGO_SERDES_RX_CMP_DATA_PRBS11   = 2\n"
                 "AVAGO_SERDES_RX_CMP_DATA_PRBS15   = 3\n"
                 "AVAGO_SERDES_RX_CMP_DATA_PRBS23   = 4\n"
                 "AVAGO_SERDES_RX_CMP_DATA_PRBS31   = 5\n"
                 "AVAGO_SERDES_RX_CMP_DATA_SELF_SEED= 7\n"
                 "AVAGO_SERDES_RX_CMP_DATA_OFF      = 8\n");
      return 0;
    }
    rc = port_mgr_av_sd_set_rx_cmp_sel(dev_id, ring, sd, sel);
    if (rc) {
      aim_printf(
          &uc->pvs, "%d returned from port_mgr_av_sd_set_rx_cmp_sel\n", rc);
    }
  } else if (match(api, "get_rx_cmp_mode")) {
    int mode;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_rx_cmp_mode dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_rx_cmp_mode 0    0    1\n");
      return 0;
    }
    mode = port_mgr_av_sd_get_rx_cmp_mode(dev_id, ring, sd);
    aim_printf(&uc->pvs,
               "%d (%s) returned from port_mgr_av_sd_get_rx_cmp_mode\n",
               mode,
               aapl_cmp_mode_to_str(mode));
  } else if (match(api, "set_rx_cmp_mode")) {
    int mode;
    int cmp_mode[4] = {AVAGO_SERDES_RX_CMP_MODE_OFF,
                       AVAGO_SERDES_RX_CMP_MODE_XOR,
                       AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN,
                       AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN};

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_rx_cmp_mode dev_id ring sd mode\n");
      aim_printf(&uc->pvs, "       sd set_rx_cmp_mode 0    0    1  8\n");
      return 0;
    }
    mode = strtol(uc->pargs->args[4], NULL, 0);
    if (mode > 3) {
      aim_printf(&uc->pvs,
                 "Valid cmp modes are:\n"
                 "AVAGO_SERDES_RX_CMP_MODE_OFF         = 0\n"
                 "AVAGO_SERDES_RX_CMP_MODE_XOR         = 1\n"
                 "AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN = 2\n"
                 "AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN = 3\n");
      return 0;
    }
    rc = port_mgr_av_sd_set_rx_cmp_mode(dev_id, ring, sd, cmp_mode[mode]);
    if (rc) {
      aim_printf(
          &uc->pvs, "%d returned from port_mgr_av_sd_set_rx_cmp_mode\n", rc);
    }
  } else if (match(api, "get_rx_term")) {
    int term;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_rx_term dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_rx_term 0    0    1\n");
      return 0;
    }
    term = port_mgr_av_sd_rx_term_get(dev_id, ring, sd);
    aim_printf(&uc->pvs,
               "%d (%s) returned from port_mgr_av_sd_rx_term_get\n",
               term,
               aapl_term_to_str(term));
  } else if (match(api, "set_rx_term")) {
    int term;
    int term_str[3] = {AVAGO_SERDES_RX_TERM_AVDD,
                       AVAGO_SERDES_RX_TERM_FLOAT,
                       AVAGO_SERDES_RX_TERM_AGND};

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_rx_term dev_id ring sd term\n");
      aim_printf(&uc->pvs, "       sd set_rx_term 0    0    1  2\n");
      return 0;
    }
    term = strtol(uc->pargs->args[4], NULL, 0);
    if (term > 2) {
      aim_printf(&uc->pvs,
                 "Valid terminations are:\n"
                 "AVAGO_SERDES_RX_TERM_AVDD = 0\n"
                 "AVAGO_SERDES_RX_TERM_FLOAT= 1\n"
                 "AVAGO_SERDES_RX_TERM_AGND = 2\n");
      return 0;
    }
    rc = port_mgr_av_sd_rx_term_set(dev_id, ring, sd, term_str[term]);
    if (rc) {
      aim_printf(&uc->pvs, "%d returned from port_mgr_av_sd_rx_term_set\n", rc);
    }
  } else if (match(api, "get_tx_pll_clk_src")) {
    int clk;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_tx_pll_clk_src dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_tx_pll_clk_src 0    0    1\n");
      return 0;
    }
    clk = port_mgr_av_sd_tx_pll_clk_source_get(dev_id, ring, sd);
    aim_printf(&uc->pvs,
               "%d (%s) returned from port_mgr_av_sd_tx_pll_clk_source_get\n",
               clk,
               aapl_pll_clk_to_str(clk));
  } else if (match(api, "set_tx_pll_clk_src")) {
    int clk;
    int clk_str[] = {AVAGO_SERDES_TX_PLL_REFCLK,
                     AVAGO_SERDES_TX_PLL_RX_DIVX,
                     AVAGO_SERDES_TX_PLL_OFF,
                     AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK,
                     AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK_DIV2};

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs,
                 "usage: sd set_tx_pll_clk_src dev_id ring sd term\n");
      aim_printf(&uc->pvs, "       sd set_tx_pll_clk_src 0    0    1  2\n");
      return 0;
    }
    clk = strtol(uc->pargs->args[4], NULL, 0);
    if (clk > 4) {
      aim_printf(&uc->pvs,
                 "Valid Tx PLL clocks are:\n"
                 "AVAGO_SERDES_TX_PLL_REFCLK             = 0\n"
                 "AVAGO_SERDES_TX_PLL_RX_DIVX            = 1\n"
                 "AVAGO_SERDES_TX_PLL_OFF                = 2\n"
                 "AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK      = 3\n"
                 "AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK_DIV2 = 4\n");

      return 0;
    }
    rc = port_mgr_av_sd_tx_pll_clk_source_set(dev_id, ring, sd, clk_str[clk]);
    if (rc) {
      aim_printf(&uc->pvs,
                 "%d returned from port_mgr_av_sd_tx_pll_clk_source_set\n",
                 rc);
    }
  } else if (match(api, "get_spico_clk_src")) {
    int clk;

    if (uc->pargs->count != 4) {
      aim_printf(&uc->pvs, "usage: sd get_spico_clk_src dev_id ring sd\n");
      aim_printf(&uc->pvs, "       sd get_spico_clk_src 0    0    1\n");
      return 0;
    }
    clk = port_mgr_av_sd_spico_clk_source_get(dev_id, ring, sd);
    aim_printf(&uc->pvs,
               "%d (%s) returned from port_mgr_av_sd_spico_clk_source_get\n",
               clk,
               aapl_spico_clk_to_str(clk));
  } else if (match(api, "set_spico_clk_src")) {
    int clk;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd set_spico_clk_src dev_id ring sd term\n");
      aim_printf(&uc->pvs, "       sd set_spico_clk_src 0    0    1  2\n");
      return 0;
    }
    clk = strtol(uc->pargs->args[4], NULL, 0);
    if (clk > 7) {
      aim_printf(&uc->pvs,
                 "Valid Spico clocks are:\n"
                 "AVAGO_SERDES_SPICO_REFCLK                = 0"
                 "AVAGO_SERDES_SPICO_PCIE_CORE_CLK         = 1"
                 "AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED      = 2"
                 "AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED      = 3"
                 "AVAGO_SERDES_SPICO_REFCLK_DIV2           = 4"
                 "AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2    = 5"
                 "AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED_DIV2 = 6"
                 "AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED_DIV2 = 7");
      return 0;
    }
    rc = port_mgr_av_sd_spico_clk_source_set(dev_id, ring, sd, clk);
    if (rc) {
      aim_printf(&uc->pvs,
                 "%d returned from port_mgr_av_sd_spico_clk_source_set\n",
                 rc);
    }
  } else if (match(api, "rd")) {
    uint32_t data, reg;

    if (uc->pargs->count != 5) {
      aim_printf(&uc->pvs, "usage: sd rd dev_id ring sd reg\n");
      aim_printf(&uc->pvs, "       sd rd 0    0    1  3\n");
      return 0;
    }
    reg = strtoul(uc->pargs->args[4], NULL, 16);
    data = port_mgr_av_sd_sbus_rd(dev_id, ring, sd, reg);
    aim_printf(&uc->pvs,
               "%d <%04xh> returned from port_mgr_av_sd_sbus_rd\n",
               data,
               data);
  } else if (match(api, "wr")) {
    uint32_t data, reg;

    if (uc->pargs->count != 6) {
      aim_printf(&uc->pvs, "usage: sd wr dev_id ring sd reg\n");
      aim_printf(&uc->pvs, "       sd wr 0    0    1  3\n");
      return 0;
    }
    reg = strtoul(uc->pargs->args[4], NULL, 16);
    data = strtoul(uc->pargs->args[5], NULL, 16);
    port_mgr_av_sd_sbus_wr(dev_id, ring, sd, reg, data);
  } else if (match(api, "spico_int")) {
    uint32_t data, interrupt, rtn_data;

    if (uc->pargs->count != 6) {
      aim_printf(&uc->pvs,
                 "usage: sd spico_bf_dev_id_t dev_id ring sd int data\n");
      aim_printf(&uc->pvs, "       sd spico_int 0    0    1  3   0\n");
      return 0;
    }
    interrupt = strtoul(uc->pargs->args[4], NULL, 16);
    data = strtoul(uc->pargs->args[5], NULL, 16);
    rtn_data = port_mgr_av_sd_spico_int(dev_id, ring, sd, interrupt, data);
    aim_printf(&uc->pvs, "%d <%04xh>\n", rtn_data, rtn_data);
  } else if (match(api, "sym")) {
    uint32_t speed;
    float pll_ovrclk = 0;

    if (uc->pargs->count != 6) {
      aim_printf(
          &uc->pvs,
          "usage: sd sym dev_id ring sd speed <1/10/25> <pll_overclk %%>\n");
      aim_printf(&uc->pvs, "       sd spico_int 0    0    1 25 3.5\n");
      return 0;
    }
    speed = strtoul(uc->pargs->args[4], NULL, 10);
    pll_ovrclk = strtof(uc->pargs->args[5], NULL);
    // make sure speed is valid
    if ((speed != 1) && (speed != 10) && (speed != 25) && (speed != 125) &&
        (speed != 3125)) {
      aim_printf(
          &uc->pvs, "Speed (%d) must be one of <1/10/25/125/3125>\n", speed);
      return 0;
    }
    port_mgr_av_sd_pgm_symmetric(dev_id, ring, sd, speed, true, pll_ovrclk);
  } else if (match(api, "asym_tx")) {
    uint32_t speed, rx_en, rx_speed;
    float pll_ovrclk = 0;

    if (uc->pargs->count != 8) {
      aim_printf(&uc->pvs,
                 "usage: sd asym_tx dev_id ring sd speed <1/10/25> rx_speed "
                 "rx_en <pll_overclk %%>\n");
      aim_printf(&uc->pvs, "       sd asym_tx 0    0    1 25 10 1 3.5\n");
      return 0;
    }
    speed = strtoul(uc->pargs->args[4], NULL, 10);
    rx_speed = strtoul(uc->pargs->args[5], NULL, 10);
    rx_en = strtoul(uc->pargs->args[6], NULL, 10);
    pll_ovrclk = strtof(uc->pargs->args[7], NULL);
    // make sure speed is valid
    if ((speed != 1) && (speed != 10) && (speed != 25) && (speed != 125) &&
        (speed != 3125)) {
      aim_printf(
          &uc->pvs, "Speed (%d) must be one of <1/10/25/125/3125>\n", speed);
      return 0;
    }
    // make sure rx_speed is valid
    if ((rx_speed != 1) && (rx_speed != 10) && (rx_speed != 25) &&
        (rx_speed != 125) && (rx_speed != 3125)) {
      aim_printf(&uc->pvs,
                 "Rx Speed (%d) must be one of <1/10/25/125/3125>\n",
                 rx_speed);
      return 0;
    }
    port_mgr_av_sd_pgm_asymmetric_tx(
        dev_id, ring, sd, speed, rx_speed, rx_en, true, pll_ovrclk);
  } else if (match(api, "asym_rx")) {
    uint32_t speed, tx_en, tx_speed;
    float pll_ovrclk = 0;

    if (uc->pargs->count != 8) {
      aim_printf(&uc->pvs,
                 "usage: sd asym_rx dev_id ring sd speed <1/10/25> tx_speed "
                 "tx_en <pll_overclk %%>\n");
      aim_printf(&uc->pvs, "       sd asym_rx 0    0    1 25 10 1 3.5\n");
      return 0;
    }
    speed = strtoul(uc->pargs->args[4], NULL, 10);
    tx_speed = strtoul(uc->pargs->args[5], NULL, 10);
    tx_en = strtoul(uc->pargs->args[6], NULL, 10);
    pll_ovrclk = strtof(uc->pargs->args[7], NULL);
    // make sure speed is valid
    if ((speed != 1) && (speed != 10) && (speed != 25) && (speed != 125) &&
        (speed != 3125)) {
      aim_printf(
          &uc->pvs, "Speed (%d) must be one of <1/10/25/125/3125>\n", speed);
      return 0;
    }
    // make sure tx_speed is valid
    if ((tx_speed != 1) && (tx_speed != 10) && (tx_speed != 25) &&
        (tx_speed != 125) && (tx_speed != 3125)) {
      aim_printf(&uc->pvs,
                 "Tx Speed (%d) must be one of <1/10/25/125/3125>\n",
                 tx_speed);
      return 0;
    }
    port_mgr_av_sd_pgm_asymmetric_rx(
        dev_id, ring, sd, speed, tx_speed, tx_en, pll_ovrclk);
  } else if (match(api, "set_dfe")) {
    uint32_t rtn_data;
    int row, col, value;

    if (uc->pargs->count != 7) {
      aim_printf(&uc->pvs,
                 "usage: sd set_dfe   dev_id ring sd row col value\n");
      aim_printf(&uc->pvs, "       sd set_dfe   0    0    1  2   2   65\n");
      return 0;
    }
    row = strtol(uc->pargs->args[4], NULL, 0);
    col = strtol(uc->pargs->args[5], NULL, 0);
    value = strtol(uc->pargs->args[6], NULL, 0);

    if ((row > 15) || (row < 0) || (col > 4) || (col < 0)) {
      aim_printf(&uc->pvs, "    0            1     2   3     4           \n");
      aim_printf(&uc->pvs, "--+------------+-----+----+-----+------------\n");
      aim_printf(&uc->pvs, "0 - thresh_HF    d0e   HF  GAIN  dvos_d0e_lo*\n");
      aim_printf(&uc->pvs, "1 - thresh_LF    d0o   LF    2*  dvos_d0e_hi*\n");
      aim_printf(&uc->pvs, "2 - thresh_AGC   d1e   DC    3*  dvos_d0o_lo*\n");
      aim_printf(&uc->pvs, "3 - err_cnt_lo   d1o   BW    4*  dvos_d0o_hi*\n");
      aim_printf(&uc->pvs, "4 - err_cnt_hi   t1e   LB    5*  dvos_d1e_lo*\n");
      aim_printf(&uc->pvs, "5 - gainDFE_lo   t1o         6*  dvos_d1e_hi*\n");
      aim_printf(&uc->pvs, "6 - gainDFE_hi   t0e         7*  dvos_d1o_lo*\n");
      aim_printf(&uc->pvs, "7 - agc_gain_bnd t0o         8*  dvos_d1o_hi*\n");
      aim_printf(&uc->pvs, "8 - agc_eq_bnd   alpha       9*  tvos_d0e_lo*\n");
      aim_printf(&uc->pvs, "9 - thresh_lev               A** tvos_d0e_hi*\n");
      aim_printf(&uc->pvs, "10- dfe_state                B** tvos_d0o_lo*\n");
      aim_printf(&uc->pvs, "11- dfe_status               C** tvos_d0o_hi*\n");
      aim_printf(&uc->pvs, "12- d6_vos_only              D** tvos_d1e_lo*\n");
      aim_printf(&uc->pvs, "13- ctle_only                    tvos_d1e_hi*\n");
      aim_printf(&uc->pvs, "14- enable_dlev                  tvos_d1o_lo*\n");
      aim_printf(&uc->pvs, "15- run_coarse                   tvos_d1o_hi*\n");
      return 0;
    }
    rtn_data = port_mgr_av_sd_dfe_param_set(dev_id, ring, sd, row, col, value);
    aim_printf(
        &uc->pvs, "%d returned from port_mgr_av_sd_dfe_param_set\n", rtn_data);
  } else if (match(api, "pmd")) {
    sd_dump_pmd(dev_id, ring, sd);
    //  } else if (match(api, "snsr")) {
    //    sd_dump_sensor_data(uc, dev_id, ring, sd);
  } else if (match(api, "pos")) {
    int x, y;

    if (uc->pargs->count != 6) {
      aim_printf(&uc->pvs, "usage: sd pos dev_id ring sd x y\n");
      return 0;
    }
    x = strtol(uc->pargs->args[4], NULL, 0);
    y = strtol(uc->pargs->args[5], NULL, 0);

    if (x < 0) {
      aim_printf(&uc->pvs, "x must be positive\n");
      return 0;
    }

    rc = port_mgr_av_sd_offset_step_set(dev_id, ring, sd, x, y);
    if (rc != 0) {
      aim_printf(
          &uc->pvs, "err: %d : from port_mgr_av_sd_offset_step_set\n", rc);
    }
  } else if (match(api, "serdes_dump")) {
    sd_aapl_serdes_dump(dev_id, ring, sd, uc);
  }

  return 0;
}

static void port_mgr_ucli_tolower(const char *str, char *str_new) {
  int i = 0, i_new = 0;
  int diff = 'A' - 'a';
  if ((str == NULL) || (str_new == NULL)) return;
  while (str[i] != '\0') {
    if ((str[i] == 'G') || (str[i] == 'R') || (str[i] == 'N') ||
        (str[i] == 'B')) {
      str_new[i_new++] = str[i++] - diff;
    } else {
      str_new[i_new++] = str[i++];
    }
  }
  str_new[i_new] = '\0';
  return;
}

static int port_mgr_ucli_get_speed(const char *str,
                                   bf_port_speed_t *ps,
                                   uint32_t *n_lanes) {
  uint32_t max_len = strlen("40G_NON_BREAKABLE");
  char str_new[max_len + 1];
  if (strlen(str) > max_len) return -1;
  port_mgr_ucli_tolower(str, str_new);

  if ((strcmp(str_new, "1g") == 0) || (strcmp(str_new, "1") == 0)) {
    *ps = BF_SPEED_1G;
    *n_lanes = 1;
    return 0;
  } else if ((strcmp(str_new, "10g") == 0) || (strcmp(str_new, "10") == 0)) {
    *ps = BF_SPEED_10G;
    *n_lanes = 1;
    return 0;
  } else if ((strcmp(str_new, "25g") == 0) || (strcmp(str_new, "25") == 0)) {
    *ps = BF_SPEED_25G;
    *n_lanes = 1;
    return 0;
  } else if ((strcmp(str_new, "40g") == 0) || ((strcmp(str_new, "40") == 0))) {
    *ps = BF_SPEED_40G;
    *n_lanes = 4;
    return 0;
  } else if ((strcmp(str_new, "50g") == 0) || (strcmp(str_new, "50") == 0)) {
    *ps = BF_SPEED_50G;
    *n_lanes = 2;
    return 0;
  } else if ((strcmp(str_new, "50g-r2") == 0) ||
             (strcmp(str_new, "50-r2") == 0)) {
    *ps = BF_SPEED_50G;
    *n_lanes = 2;
    return 0;
  } else if ((strcmp(str_new, "50g-r1") == 0) ||
             (strcmp(str_new, "50-r1") == 0)) {
    *ps = BF_SPEED_50G;
    *n_lanes = 1;
    return 0;
  } else if ((strcmp(str_new, "100g") == 0) || (strcmp(str_new, "100") == 0)) {
    *ps = BF_SPEED_100G;
    *n_lanes = 4;
    return 0;
  } else if ((strcmp(str_new, "100g-r4") == 0) ||
             (strcmp(str_new, "100-r4") == 0)) {
    *ps = BF_SPEED_100G;
    *n_lanes = 4;
    return 0;
  } else if ((strcmp(str_new, "100g-r2") == 0) ||
             (strcmp(str_new, "100-r2") == 0)) {
    *ps = BF_SPEED_100G;
    *n_lanes = 2;
    return 0;
  } else if ((strcmp(str_new, "200g") == 0) || (strcmp(str_new, "200") == 0)) {
    *ps = BF_SPEED_200G;
    *n_lanes = 4;
    return 0;
  } else if ((strcmp(str_new, "200g-r4") == 0) ||
             (strcmp(str_new, "200-r4") == 0)) {
    *ps = BF_SPEED_200G;
    *n_lanes = 4;
    return 0;
  } else if ((strcmp(str_new, "200g-r8") == 0) ||
             (strcmp(str_new, "200-r8") == 0)) {
    *ps = BF_SPEED_200G;
    *n_lanes = 8;
    return 0;
  } else if ((strcmp(str_new, "400g") == 0) || (strcmp(str_new, "400") == 0)) {
    *ps = BF_SPEED_400G;
    *n_lanes = 8;
    return 0;
  } else {
    return -1;
  }
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_add__(ucli_context_t *uc) {
  int rc, asic, pipe, port, fec;
  bf_dev_port_t dev_port;
  bf_port_speed_t gb;
  uint32_t n_lanes;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "bf_port_add", 5, "Add a port <asic> <gb> <fec> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  fec = strtol(uc->pargs->args[2], NULL, 0);
  pipe = strtol(uc->pargs->args[3], NULL, 0);
  port = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  if (port_mgr_ucli_get_speed(uc->pargs->args[1], &gb, &n_lanes) != 0) {
    aim_printf(&uc->pvs, "Unknow speed\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_add_with_lane_numb(asic, dev_port, gb, n_lanes, fec);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_add failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_addm__(ucli_context_t *uc) {
  int port, asic, fec, pipe;
  int min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  bf_status_t bf_status;
  bf_port_speed_t gb;
  uint32_t n_lanes;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_addm",
                    7,
                    "Add multiple ports "
                    "<dev> <speed/Gbps> <fec> "
                    "<map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  fec = strtol(uc->pargs->args[2], NULL, 0);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }
  if (port_mgr_ucli_get_speed(uc->pargs->args[1], &gb, &n_lanes) != 0) {
    aim_printf(&uc->pvs, "Unknow speed\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      bf_status = bf_port_add_with_lane_numb(asic, dev_port, gb, n_lanes, fec);

      if (bf_status != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_add failed: rc=%d, dev_port=%x\n",
                   bf_status,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_rmv__(ucli_context_t *uc) {
  int rc, asic, pipe, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc, "bf_port_rmv", 3, "Remove a port <dev> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_remove(asic, dev_port);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_rmv failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_rmvm__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_rmvm",
      5,
      "Remove multiple ports <dev> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  map[3] = strtoull(uc->pargs->args[1], NULL, 16);
  map[2] = strtoull(uc->pargs->args[2], NULL, 16);
  map[1] = strtoull(uc->pargs->args[3], NULL, 16);
  map[0] = strtoull(uc->pargs->args[4], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  min_fp_port = lld_get_min_fp_port(asic);
  max_fp_port = lld_get_max_fp_port(asic);
  if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_remove(asic, dev_port);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_rmv failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_enable__(ucli_context_t *uc) {
  int rc, asic, pipe, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "bf_port_enable", 3, "Enable a port <dev> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_enable(asic, dev_port, 1);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_enable failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_enablem__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_enablem",
      5,
      "Enable multiple ports <dev> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  map[3] = strtoull(uc->pargs->args[1], NULL, 16);
  map[2] = strtoull(uc->pargs->args[2], NULL, 16);
  map[1] = strtoull(uc->pargs->args[3], NULL, 16);
  map[0] = strtoull(uc->pargs->args[4], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_enable(asic, dev_port, 1);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_enable failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_disable__(ucli_context_t *uc) {
  int rc, asic, pipe, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "bf_port_disable", 3, "Disable a port <dev> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }
  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_enable(asic, dev_port, 0);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_disable failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_disablem__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_disablem",
      5,
      "Disable multiple ports <dev> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  map[3] = strtoull(uc->pargs->args[1], NULL, 16);
  map[2] = strtoull(uc->pargs->args[2], NULL, 16);
  map[1] = strtoull(uc->pargs->args[3], NULL, 16);
  map[0] = strtoull(uc->pargs->args[4], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_enable(asic, dev_port, 0);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_disable failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_pause_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, tx_en, rx_en;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_pause_set",
                    5,
                    "Enable/Disable Link PAUSE on a port <dev> <tx_en> <rx_en> "
                    "<pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  tx_en = strtol(uc->pargs->args[1], NULL, 0);
  rx_en = strtol(uc->pargs->args[2], NULL, 0);
  pipe = strtol(uc->pargs->args[3], NULL, 0);
  port = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_flow_control_link_pause_set(
      asic, dev_port, tx_en ? 1 : 0, rx_en ? 1 : 0);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_flow_control_link_pause_set failed: rc=%d, "
               "dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_pause_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, tx_en, rx_en, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_pause_setm",
      7,
      "Enable/Disable Link PAUSE on multiple ports "
      "<dev> <tx_en> <rx_en> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  tx_en = strtol(uc->pargs->args[1], NULL, 0);
  rx_en = strtol(uc->pargs->args[2], NULL, 0);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_flow_control_link_pause_set(
          asic, dev_port, tx_en ? 1 : 0, rx_en ? 1 : 0);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_flow_control_link_pause_set failed: rc=%d, "
                   "dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_pfc_set__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, tx_en_map, rx_en_map;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_pfc_set",
                    5,
                    "Enable/Disable PFC PAUSE on a port "
                    "<dev> <tx_en_map> <rx_en_map> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  tx_en_map = strtoul(uc->pargs->args[1], NULL, 16);
  rx_en_map = strtoul(uc->pargs->args[2], NULL, 16);
  pipe = strtol(uc->pargs->args[3], NULL, 0);
  port = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }
  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_flow_control_pfc_set(
      asic, dev_port, tx_en_map & 0xFF, rx_en_map & 0xFF);

  if (rc != BF_SUCCESS) {
    aim_printf(
        &uc->pvs,
        "Error: bf_port_flow_control_pfc_set failed: rc=%d, dev_port=%x\n",
        rc,
        dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_pfc_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, tx_en_map, rx_en_map, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_pfc_setm",
      7,
      "Enable/Disable PFC PAUSE on multiple ports "
      "<dev> <tx_en_map> <rx_en_map> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  tx_en_map = strtoul(uc->pargs->args[1], NULL, 16);
  rx_en_map = strtoul(uc->pargs->args[2], NULL, 16);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_flow_control_pfc_set(
          asic, dev_port, tx_en_map & 0xFF, rx_en_map & 0xFF);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_flow_control_pfc_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_preamble_len_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, preamble_len;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_preamble_len_set",
                    4,
                    "Set custom preamble length on a port <dev> <preamble_len> "
                    "<pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  preamble_len = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_preamble_length_set(asic, dev_port, preamble_len);

  if (rc != BF_SUCCESS) {
    aim_printf(
        &uc->pvs,
        "Error: bf_port_preamble_length_set failed: rc=%d, dev_port=%x\n",
        rc,
        dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_preamble_len_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, preamble_len, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_preamble_len_setm",
                    6,
                    "Set custom preamble length on multiple ports "
                    "<dev> <preamble_len> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  preamble_len = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_preamble_length_set(asic, dev_port, preamble_len);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_preamble_length_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_ifg_set__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, ifg, ieee;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_ifg_set",
                    5,
                    "Set inter-frame gap on a port "
                    "<dev> <IFG> <1-check whether against IEEE 802.3 allowed "
                    "ranges,0-no check> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  ifg = strtol(uc->pargs->args[1], NULL, 0);
  ieee = strtol(uc->pargs->args[2], NULL, 0);
  pipe = strtol(uc->pargs->args[3], NULL, 0);
  port = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_ifg_set(asic, dev_port, ifg, ieee);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_ifg_set failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_ifg_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, ifg, ieee, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_ifg_setm",
                    7,
                    "Set inter-frame gap on multiple ports "
                    "<dev> <IFG> <1-check whether against IEEE 802.3 allowed "
                    "ranges,0-no check>"
                    "<map[3]> <map[2]> <map[1]> <map[0]> ");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  ifg = strtol(uc->pargs->args[1], NULL, 0);
  ieee = strtol(uc->pargs->args[2], NULL, 0);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_ifg_set(asic, dev_port, ifg, ieee);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_ifg_set failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_mtu_set__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, tx_mtu, rx_mtu;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_mtu_set",
                    5,
                    "Set MTU on a port <dev> <tx_mtu> <rx_mtu> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  tx_mtu = strtol(uc->pargs->args[1], NULL, 0);
  rx_mtu = strtol(uc->pargs->args[2], NULL, 0);
  pipe = strtol(uc->pargs->args[3], NULL, 0);
  port = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_mtu_set(asic, dev_port, tx_mtu, rx_mtu);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_mtu_set failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_mtu_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, tx_mtu, rx_mtu, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_mtu_setm",
      7,
      "Set MTU on multiple ports "
      "<dev> <tx_mtu> <rx_mtu> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  tx_mtu = strtol(uc->pargs->args[1], NULL, 0);
  rx_mtu = strtol(uc->pargs->args[2], NULL, 0);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_mtu_set(asic, dev_port, tx_mtu, rx_mtu);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_mtu_set failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_loopback_mode_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, loopback_mode;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_loopback_mode_set",
                    4,
                    "Set loopback mode "
                    "<dev> <mode: 0=none,1=MAC near,2=MAC far,3=PCS near,4=sds "
                    "near,5=sds far> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  loopback_mode = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_loopback_mode_set(asic, dev_port, loopback_mode);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_loopback_mode_set failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_clkobs_set__(
    ucli_context_t *uc) {
  int pri_pad, clk_src, divider, div_sel, asic, pipe, port;
  bf_dev_port_t dev_port;
  bf_clkobs_pad_t pad;
  bf_sds_clkobs_clksel_t clk_sel;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_clkobs_set",
                    6,
                    "Set clockpad source "
                    "<dev> <clk_pad: 0=pri 1=sec> <clk_src: 0=none "
                    "1=rx_recoverd 2=tx> <clk-divider tof-1: 1:1, 2:2, 3:3, "
                    "4:8 or tof-2: 1:2, 2:4, 3:8, 4:16> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  pri_pad = strtol(uc->pargs->args[1], NULL, 0);
  clk_src = strtol(uc->pargs->args[2], NULL, 0);
  divider = strtol(uc->pargs->args[3], NULL, 0);
  pipe = strtol(uc->pargs->args[4], NULL, 0);
  port = strtol(uc->pargs->args[5], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return UCLI_STATUS_E_PARAM;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  pad = ((pri_pad) ? BF_CLKOBS_PAD_1 : BF_CLKOBS_PAD_0);
  if (clk_src == 0) {
    clk_sel = BF_SDS_NONE_CLK;
  } else if (clk_src == 1) {
    clk_sel = BF_SDS_RX_RECOVEREDCLK;
  } else if (clk_src == 2) {
    clk_sel = BF_SDS_TX_CLK;
  } else {
    /* we could support a few more selections if needed */
    return UCLI_STATUS_E_PARAM;
  }
  if (divider == 1) {
    div_sel = 0;
  } else if (divider == 2) {
    div_sel = 1;
  } else if (divider == 3) {
    div_sel = 2;
  } else if (divider == 4) {
    div_sel = 3;
  } else {
    return UCLI_STATUS_E_PARAM;
  }
  if (bf_port_clkobs_set(asic, dev_port, pad, clk_sel, div_sel) != BF_SUCCESS) {
    return UCLI_STATUS_E_INTERNAL;
  }
  return UCLI_STATUS_OK;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_tx_clk_sel__(
    ucli_context_t *uc) {
  bf_dev_id_t asic;
  int pipe, port, clk_src, num_lane;
  bf_dev_port_t dev_port;
  bf_sds_tx_pll_clksel_t clk;
  bf_dev_pipe_t phy_pipe;
  int mac_block, ch, rc, i;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_tx_clk_sel",
                    4,
                    "Set Tx clk source "
                    "<dev> <clk_src: 0=eth_ref 1=alt_ref> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  clk_src = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return UCLI_STATUS_E_PARAM;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  bf_port_num_lanes_get(asic, dev_port, &num_lane);
  rc = port_mgr_map_dev_port_to_all(
      asic, dev_port, &phy_pipe, NULL, &mac_block, &ch, NULL);
  if (num_lane > 4 || rc != 0) {
    aim_printf(&uc->pvs, "Error getting dev_port parameters\n");
    return UCLI_STATUS_E_PARAM;
  }
  if (clk_src) {
    clk = BF_SDS_TX_PLL_ALT_REFCLK;
  } else {
    clk = BF_SDS_TX_PLL_ETH_REFCLK;
  }
  for (i = 0; i < num_lane; i++) {
    if (bf_serdes_tx_pll_clksel_set(asic, dev_port, i, clk) != BF_SUCCESS) {
      aim_printf(&uc->pvs, "Error: bf_port_tx_clk_set failed\n");
      return UCLI_STATUS_E_INTERNAL;
    }
  }
  return UCLI_STATUS_OK;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_loopback_mode_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, loopback_mode, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_loopback_mode_setm",
                    6,
                    "Set loopback mode on multiple ports "
                    "<dev> <mode: 0=none,1=MAC near,2=MAC far,3=PCS near,4=sds "
                    "near,5=sds far> "
                    "<map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  loopback_mode = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_loopback_mode_set(asic, dev_port, loopback_mode);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_loopback_mode_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_txff_truncation_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, txff_truncation, en;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_txff_truncation_set",
                    5,
                    "Set Tx Fifo truncation settings on a port <dev> <size> "
                    "<en> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  txff_truncation = strtol(uc->pargs->args[1], NULL, 0);
  en = strtol(uc->pargs->args[2], NULL, 0);
  pipe = strtol(uc->pargs->args[3], NULL, 0);
  port = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_txff_truncation_set(asic, dev_port, txff_truncation, en);

  if (rc != BF_SUCCESS) {
    aim_printf(
        &uc->pvs,
        "Error: bf_port_txff_truncation_set failed: rc=%d, dev_port=%x\n",
        rc,
        dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_txff_truncation_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, txff_truncation, en, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_txff_truncation_setm",
                    7,
                    "Set Tx Fifo truncation settings on multiple ports "
                    "<dev> <size> <en> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  txff_truncation = strtol(uc->pargs->args[1], NULL, 0);
  en = strtol(uc->pargs->args[2], NULL, 0);
  map[3] = strtoull(uc->pargs->args[3], NULL, 16);
  map[2] = strtoull(uc->pargs->args[4], NULL, 16);
  map[1] = strtoull(uc->pargs->args[5], NULL, 16);
  map[0] = strtoull(uc->pargs->args[6], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_txff_truncation_set(asic, dev_port, txff_truncation, en);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_txff_truncation_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_txff_mode_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  int crc_chk_dis, crc_rmv_dis, fcs_ins_dis, pad_dis;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_txff_mode_set",
                    7,
                    "Set Tx Fifo mode on a port "
                    "<dev> <crc_chk_dis> <crc_rmv_dis> <fcs_ins_dis> <pad_dis> "
                    "<pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  crc_chk_dis = strtol(uc->pargs->args[1], NULL, 0);
  crc_rmv_dis = strtol(uc->pargs->args[2], NULL, 0);
  fcs_ins_dis = strtol(uc->pargs->args[3], NULL, 0);
  pad_dis = strtol(uc->pargs->args[4], NULL, 0);
  pipe = strtol(uc->pargs->args[5], NULL, 0);
  port = strtol(uc->pargs->args[6], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_txff_mode_set(
      asic, dev_port, crc_chk_dis, crc_rmv_dis, fcs_ins_dis, pad_dis);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_txff_mode_set failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_txff_mode_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, min_fp_port, max_fp_port;
  int crc_chk_dis, crc_rmv_dis, fcs_ins_dis, pad_dis;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_txff_mode_setm",
                    9,
                    "Set Tx Fifo mode on multiple ports "
                    "<dev> <crc_chk_dis> <crc_rmv_dis> <fcs_ins_dis> <pad_dis> "
                    "<map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  crc_chk_dis = strtol(uc->pargs->args[1], NULL, 0);
  crc_rmv_dis = strtol(uc->pargs->args[2], NULL, 0);
  fcs_ins_dis = strtol(uc->pargs->args[3], NULL, 0);
  pad_dis = strtol(uc->pargs->args[4], NULL, 0);
  map[3] = strtoull(uc->pargs->args[5], NULL, 16);
  map[2] = strtoull(uc->pargs->args[6], NULL, 16);
  map[1] = strtoull(uc->pargs->args[7], NULL, 16);
  map[0] = strtoull(uc->pargs->args[8], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  min_fp_port = lld_get_min_fp_port(asic);
  max_fp_port = lld_get_max_fp_port(asic);
  if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_txff_mode_set(
          asic, dev_port, crc_chk_dis, crc_rmv_dis, fcs_ins_dis, pad_dis);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_txff_mode_set failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_xoff_pause_time_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, xoff_pause_time;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_xoff_pause_time_set",
      4,
      "Set XOFF pause time on a port <dev> <xoff_pause_time> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  xoff_pause_time = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_xoff_pause_time_set(asic, dev_port, xoff_pause_time);

  if (rc != BF_SUCCESS) {
    aim_printf(
        &uc->pvs,
        "Error: bf_port_xoff_pause_time_set failed: rc=%d, dev_port=%x\n",
        rc,
        dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_xoff_pause_time_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, xoff_pause_time, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_xoff_pause_time_setm",
      6,
      "Set XOFF pause time on multiple ports "
      "<dev> <xoff_pause_time> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  xoff_pause_time = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  min_fp_port = lld_get_min_fp_port(asic);
  max_fp_port = lld_get_max_fp_port(asic);

  if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_xoff_pause_time_set(asic, dev_port, xoff_pause_time);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_xoff_pause_time_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_xon_pause_time_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, xon_pause_time;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_xon_pause_time_set",
      4,
      "Set XON pause time on a port <dev> <xon_pause_time> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  xon_pause_time = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_xon_pause_time_set(asic, dev_port, xon_pause_time);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_xon_pause_time_set failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_xon_pause_time_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, xon_pause_time, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_xon_pause_time_setm",
      6,
      "Set XON pause time on multiple ports "
      "<dev> <xon_pause_time> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  xon_pause_time = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_xon_pause_time_set(asic, dev_port, xon_pause_time);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_xon_pause_time_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_tx_get__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;
  uint16_t delta = 0;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_1588_timestamp_delta_tx_get",
                    3,
                    "Get 1588 timestamp on <dev> <pipe> <port>");

  asic = atoi(uc->pargs->args[0]);
  pipe = atoi(uc->pargs->args[1]);
  port = atoi(uc->pargs->args[2]);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  if (!DEV_PORT_VALIDATE(dev_port)) {
    aim_printf(
        &uc->pvs,
        "Error: port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_tx_get__ "
        "failed: "
        "dev_port=%d Invalid\n",
        dev_port);
    return 0;
  }

  rc = bf_port_1588_timestamp_delta_tx_get(asic, dev_port, &delta);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_1588_timestamp_delta_tx_get failed: rc=%d, "
               "dev_port=%d\n",
               rc,
               dev_port);
  } else {
    aim_printf(&uc->pvs, "Offset value is : %d  \n", delta);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_rx_get__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;
  uint16_t delta = 0;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_1588_timestamp_delta_rx_get",
                    3,
                    "Get 1588 timestamp on <dev> <pipe> <port>");

  asic = atoi(uc->pargs->args[0]);
  pipe = atoi(uc->pargs->args[1]);
  port = atoi(uc->pargs->args[2]);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  if (!DEV_PORT_VALIDATE(dev_port)) {
    aim_printf(
        &uc->pvs,
        "Error: port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_rx_get__ "
        "failed: "
        "dev_port=%d Invalid\n",
        dev_port);
    return 0;
  }

  rc = bf_port_1588_timestamp_delta_rx_get(asic, dev_port, &delta);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_1588_timestamp_delta_rx_get failed: rc=%d, "
               "dev_port=%d\n",
               rc,
               dev_port);
  } else {
    aim_printf(&uc->pvs, "Offset value is : %d  \n", delta);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_tx_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;
  uint16_t delta = 0;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_1588_timestamp_delta_tx_set",
                    4,
                    "Set 1588 timestamp on <dev> <pipe> <port> <value>");

  asic = atoi(uc->pargs->args[0]);
  pipe = atoi(uc->pargs->args[1]);
  port = atoi(uc->pargs->args[2]);
  delta = atoi(uc->pargs->args[3]);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  if (!DEV_PORT_VALIDATE(dev_port)) {
    aim_printf(
        &uc->pvs,
        "Error: port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_tx_set__ "
        "failed: "
        "dev_port=%d Invalid\n",
        dev_port);
    return 0;
  }

  rc = bf_port_1588_timestamp_delta_tx_set(asic, dev_port, delta);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_1588_timestamp_delta_tx_set failed: rc=%d, "
               "dev_port=%d\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_rx_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;
  uint16_t delta = 0;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_1588_timestamp_delta_rx_set",
                    4,
                    "Set 1588 timestamp on <dev> <pipe> <port> <value>");

  asic = atoi(uc->pargs->args[0]);
  pipe = atoi(uc->pargs->args[1]);
  port = atoi(uc->pargs->args[2]);
  delta = atoi(uc->pargs->args[3]);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  if (!DEV_PORT_VALIDATE(dev_port)) {
    aim_printf(
        &uc->pvs,
        "Error: port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_rx_set__ "
        "failed: "
        "dev_port=%d Invalid\n",
        dev_port);
    return 0;
  }

  rc = bf_port_1588_timestamp_delta_rx_set(asic, dev_port, delta);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_1588_timestamp_delta_tx_set failed: rc=%d, "
               "dev_port=%d\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_1588_timestamp_tx_get__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;
  uint64_t ts = 0;
  bool ts_valid = false;
  int ts_id = 0;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_1588_timestamp_tx_get",
                    3,
                    "Get 1588 timestamp on <dev> <pipe> <port>");

  asic = atoi(uc->pargs->args[0]);
  pipe = atoi(uc->pargs->args[1]);
  port = atoi(uc->pargs->args[2]);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  rc = bf_port_1588_timestamp_tx_get(asic, dev_port, &ts, &ts_valid, &ts_id);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_1588_timestamp_tx_get failed: rc=%d, "
               "dev_port=%d\n",
               rc,
               dev_port);
  } else {
    if (ts_valid == true) {
      aim_printf(&uc->pvs,
                 "Timestamp id : %d Timestamp value is %" PRIu64 " \n",
                 ts_id,
                 ts);
    } else {
      aim_printf(&uc->pvs, "Not a valid timestamp \n");
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_force_lf_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, lf_val;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_force_lf_set",
      4,
      "Set force LF on/off on a port <dev> <lf_val> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  lf_val = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_force_local_fault_set(asic, dev_port, lf_val ? 1 : 0);

  if (rc != BF_SUCCESS) {
    aim_printf(
        &uc->pvs,
        "Error: bf_port_force_local_fault_set failed: rc=%d, dev_port=%x\n",
        rc,
        dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_force_lf_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, lf_val, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_force_lf_setm",
                    6,
                    "Set force LF on/off on multiple ports "
                    "<dev> <lf_val> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  lf_val = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_force_local_fault_set(asic, dev_port, lf_val);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_force_local_fault_set failed: rc=%d, dev_port=%x\n",
            rc,
            dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_force_rf_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, rf_val;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_force_rf_set",
                    4,
                    "Set force RF on/off on a port "
                    "<dev> <rf_val> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  rf_val = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_force_remote_fault_set(asic, dev_port, rf_val ? 1 : 0);

  if (rc != BF_SUCCESS) {
    aim_printf(
        &uc->pvs,
        "Error: bf_port_force_remote_fault_set failed: rc=%d, dev_port=%x\n",
        rc,
        dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_force_rf_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, rf_val, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_force_rf_setm",
                    6,
                    "Set force RF on/off on multiple ports "
                    "<dev> <rf_val> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  rf_val = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_force_remote_fault_set(asic, dev_port, rf_val);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_force_remote_fault_set failed: rc=%d, "
                   "dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_force_idle_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, idle_val;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "bf_port_force_idle_set",
      4,
      "Set force IDLE on/off on a port <dev> <idle_val> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  idle_val = strtol(uc->pargs->args[1], NULL, 0);
  pipe = strtol(uc->pargs->args[2], NULL, 0);
  port = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_force_idle_set(asic, dev_port, idle_val ? 1 : 0);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_force_idle_set failed: rc=%d, dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_force_idle_setm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, idle_val, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_force_idle_setm",
                    6,
                    "Set force IDLE on/off on multiple ports "
                    "<dev> <idle_val> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  idle_val = strtol(uc->pargs->args[1], NULL, 0);
  map[3] = strtoull(uc->pargs->args[2], NULL, 16);
  map[2] = strtoull(uc->pargs->args[3], NULL, 16);
  map[1] = strtoull(uc->pargs->args[4], NULL, 16);
  map[0] = strtoull(uc->pargs->args[5], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  min_fp_port = lld_get_min_fp_port(asic);
  max_fp_port = lld_get_max_fp_port(asic);
  if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_force_idle_set(asic, dev_port, idle_val);

      if (rc != BF_SUCCESS) {
        aim_printf(&uc->pvs,
                   "Error: bf_port_force_idle_set failed: rc=%d, dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

// uint64_t use_short_timers_map = 0xfffffffffffffffcull; // for loopback, can
// use debug mode
uint64_t use_short_timers_map =
    0ull;  // for epgm, default to not using the debug mode

static ucli_status_t port_mgr_ucli_ucli__use_short_timers__(
    ucli_context_t *uc) {
  UCLI_COMMAND_INFO(
      uc,
      "use_short_timers",
      1,
      "Define the map of ports to use short timers <short_timers_map>");

  use_short_timers_map = strtoull(uc->pargs->args[0], NULL, 16);

  aim_printf(&uc->pvs,
             "Ports to use short timers: %016" PRIx64 "\n",
             use_short_timers_map);

  return 0;
}

int rs_fec_banner_output = 0;
int fc_fec_banner_output = 0;
bool fec_errors_present = false;
bool fec_fatal_errors_present = false;

void ucli_fec_hdlr_init(void) {
  rs_fec_banner_output = 0;
  fc_fec_banner_output = 0;
  fec_errors_present = false;
  fec_fatal_errors_present = false;
}

int ucli_fec_hdlr(ucli_context_t *uc,
                  bf_dev_id_t dev_id,
                  bf_dev_port_t dev_port) {
  int rc;
  uint32_t pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t port = DEV_PORT_TO_LOCAL_PORT(dev_port);

  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  // RS FEC
  if ((port_p != NULL) && (port_p->sw.fec == BF_FEC_TYP_REED_SOLOMON) &&
      (port_p->sw.enabled)) {
    bool hi_ser;
    bool fec_align_status;
    uint32_t fec_corr_cnt;
    uint32_t fec_uncorr_cnt;
    uint32_t fec_ser_lane_0;
    uint32_t fec_ser_lane_1;
    uint32_t fec_ser_lane_2;
    uint32_t fec_ser_lane_3;
    uint32_t fec_ser_lane_4;
    uint32_t fec_ser_lane_5;
    uint32_t fec_ser_lane_6;
    uint32_t fec_ser_lane_7;
    int mac_block, ch;

    if ((rs_fec_banner_output % 16) == 0) {
      rs_fec_banner_output = 0;
      aim_printf(&uc->pvs, "RS-FEC:\n");
      aim_printf(
          &uc->pvs,
          "+------+----+-+------+----------+----------+----------+-------"
          "---+----"
          "------+----------+----------+");
      if (port_mgr_dev_is_tof2(dev_id)) {
        aim_printf(&uc->pvs, "----------+----------+----------+----------+");
      }
      aim_printf(&uc->pvs, "\n");
      aim_printf(
          &uc->pvs,
          "|c|p|p |    | |      |          |          |          |       "
          "   |    "
          "      |          |          |\n");
      aim_printf(
          &uc->pvs,
          "|h|i|o | m  | |      |          |          |          |       "
          "   |    "
          "      |          |          |\n");
      aim_printf(
          &uc->pvs,
          "|i|p|r | a  |c|      |          |          |          |       "
          "   |    "
          "      |          |          |\n");
      aim_printf(&uc->pvs,
                 "|p|e|t | c  |h|hi_ser| algnsts  |   corr   |  uncorr  | ser "
                 "ln 0 | "
                 "ser ln 1 | ser ln 2 | ser ln 3 | ");
      if (port_mgr_dev_is_tof2(dev_id)) {
        aim_printf(&uc->pvs, "ser ln 4 | ser ln 5 | ser ln 6 | ser ln 7 |");
      }
      aim_printf(&uc->pvs, "\n");
      aim_printf(
          &uc->pvs,
          "+------+----+-+------+----------+----------+----------+-------"
          "---+----"
          "------+----------+----------+");
    }
    if (port_mgr_dev_is_tof2(dev_id)) {
      aim_printf(&uc->pvs, "----------+----------+----------+----------+");
    }
    aim_printf(&uc->pvs, "\n");
    rs_fec_banner_output++;

    port_mgr_map_dev_port_to_all(
        dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

    rc = bf_port_rs_fec_status_and_counters_get(dev_id,
                                                dev_port,
                                                &hi_ser,
                                                &fec_align_status,
                                                &fec_corr_cnt,
                                                &fec_uncorr_cnt,
                                                &fec_ser_lane_0,
                                                &fec_ser_lane_1,
                                                &fec_ser_lane_2,
                                                &fec_ser_lane_3,
                                                &fec_ser_lane_4,
                                                &fec_ser_lane_5,
                                                &fec_ser_lane_6,
                                                &fec_ser_lane_7);

    if (rc != BF_SUCCESS) {
      aim_printf(&uc->pvs,
                 "Error: bf_port_rs_fec_status_and_counters_get failed: rc=%d, "
                 "dev_port=%x\n",
                 rc,
                 dev_port);
      return 0;
    }
    fec_fatal_errors_present =
        fec_fatal_errors_present ||
        ((hi_ser != 0) || (fec_align_status == 0) || (fec_uncorr_cnt != 0));
    fec_errors_present =
        fec_errors_present || ((hi_ser != 0) || (fec_align_status == 0) ||
                               (fec_corr_cnt != 0) || (fec_uncorr_cnt != 0));

    aim_printf(&uc->pvs,
               "|%d|%d|%2d| %2d |%d| %4d | %8d | %8d | %8d | %8d | %8d | %8d | "
               "%8d | ",
               dev_id,
               pipe,
               port,
               mac_block,
               ch,
               hi_ser,
               fec_align_status,
               fec_corr_cnt,
               fec_uncorr_cnt,
               fec_ser_lane_0,
               fec_ser_lane_1,
               fec_ser_lane_2,
               fec_ser_lane_3);
    if (port_mgr_dev_is_tof2(dev_id)) {
      aim_printf(&uc->pvs,
                 "%8d | %8d | %8d | %8d |",
                 fec_ser_lane_4,
                 fec_ser_lane_5,
                 fec_ser_lane_6,
                 fec_ser_lane_7);
    }
    aim_printf(&uc->pvs, "\n");
  }
  // FC FEC
  if ((port_p != NULL) && (port_p->sw.fec == BF_FEC_TYP_FIRECODE) &&
      (port_p->sw.enabled)) {
    bool block_lock_status;
    uint32_t fec_corr_blk_cnt;
    uint32_t fec_uncorr_blk_cnt;
    int mac_block, ch, vl, num_vl;

    if ((fc_fec_banner_output % 16) == 0) {
      fc_fec_banner_output = 0;
      aim_printf(&uc->pvs, "FC FEC:\n");
      aim_printf(&uc->pvs, "+-+-+--+----+-+-+----+----------+----------+\n");
      aim_printf(&uc->pvs, "|c|p|p |    | | |    |          |          |\n");
      aim_printf(&uc->pvs, "|h|i|o | m  | | |    |          |          |\n");
      aim_printf(&uc->pvs, "|i|p|r | a  |c|v|blk |          |          |\n");
      aim_printf(&uc->pvs, "|p|e|t | c  |h|l|lock|  corr    |  uncorr  |\n");
      aim_printf(&uc->pvs, "+-+-+--+----+-+-+----+----------+----------+\n");
    }
    fc_fec_banner_output++;

    port_mgr_map_dev_port_to_all(
        dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

    if ((port_p->sw.speed == BF_SPEED_10G) ||
        (port_p->sw.speed == BF_SPEED_25G)) {
      num_vl = 1;
    } else if ((port_p->sw.speed == BF_SPEED_40G) ||
               (port_p->sw.speed == BF_SPEED_50G)) {
      num_vl = 4;
    } else {
      return 0;  // invalid FEC mode for other speeds
    }

    for (vl = 0; vl < num_vl; vl++) {
      rc = bf_port_fc_fec_status_and_counters_get(dev_id,
                                                  dev_port,
                                                  vl,
                                                  &block_lock_status,
                                                  &fec_corr_blk_cnt,
                                                  &fec_uncorr_blk_cnt);

      if (rc != BF_SUCCESS) {
        aim_printf(
            &uc->pvs,
            "Error: bf_port_fc_fec_status_and_counters_get failed: rc=%d, "
            "dev_port=%x, vl=%d\n",
            rc,
            dev_port,
            vl);
        return 0;
      }
      fec_fatal_errors_present =
          fec_fatal_errors_present || (fec_uncorr_blk_cnt != 0);
      fec_errors_present = fec_errors_present || ((fec_corr_blk_cnt != 0) ||
                                                  (fec_uncorr_blk_cnt != 0));
      aim_printf(&uc->pvs,
                 "|%d|%d|%2d| %2d |%d|%d|%4d| %8d | %8d |\n",
                 dev_id,
                 pipe,
                 port,
                 mac_block,
                 ch,
                 vl,
                 block_lock_status,
                 fec_corr_blk_cnt,
                 fec_uncorr_blk_cnt);
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__fec__(ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  //  bf_dev_port_t port;
  bf_dev_id_t dev_id = 0;
  port_mgr_dev_t *dev_p = NULL;
  int min_fp_port, max_fp_port, port;
  UCLI_COMMAND_INFO(
      uc, "fec", 1, "Dump FEC status for all ports with FEC enabled <dev_id>");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }
  rs_fec_banner_output = 0;
  fc_fec_banner_output = 0;
  fec_errors_present = false;
  fec_fatal_errors_present = false;

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    bf_dev_port_t dev_port;
    min_fp_port = lld_get_min_fp_port(dev_id);
    max_fp_port = lld_get_max_fp_port(dev_id);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      dev_port = MAKE_DEV_PORT(pipe, port);
      ucli_fec_hdlr(uc, dev_id, dev_port);
    }
    if (pipe == 0) {
      for (port = lld_get_min_cpu_port(dev_id);
           port <= lld_get_max_cpu_port(dev_id);
           port++) {
        dev_port = MAKE_DEV_PORT(0, port);
        ucli_fec_hdlr(uc, dev_id, dev_port);
      }
    }
  }
  if (fec_fatal_errors_present) {
    aim_printf(&uc->pvs, "FEC errors present\n");
  } else {
    aim_printf(&uc->pvs, "FEC clean\n");
  }
  return 0;
}

void port_mgr_dump_port_config(ucli_context_t *uc);

static ucli_status_t port_mgr_ucli_ucli__ports__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "ports", 0, "Dump port config");

  port_mgr_dump_port_config(uc);
  return 0;
}

bf_status_t bf_port_oper_state_get_no_side_effect(bf_dev_id_t dev_id,
                                                  bf_dev_port_t port,
                                                  int *state);
bf_status_t bf_port_pcs_status_get(bf_dev_id_t dev_id,
                                   bf_dev_port_t port,
                                   bool *pcs_status,
                                   uint32_t *block_lock_per_pcs_lane,
                                   uint32_t *alignment_marker_lock_per_pcs_lane,
                                   bool *hi_ber,
                                   bool *block_lock_all,
                                   bool *alignment_marker_lock_all);

int64_t timeval_subtract(struct timeval *x, struct timeval *y) {
  int64_t xx, yy;

  xx = (x->tv_sec * 1000000) + x->tv_usec;
  yy = (y->tv_sec * 1000000) + y->tv_usec;
  return (xx - yy);
}

static ucli_status_t port_mgr_ucli_ucli__but__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "but", 2, "Get bring-up time <dev_id> <dev_port>");
  int64_t t_us;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtoull(uc->pargs->args[0], NULL, 10);
  dev_port = strtoull(uc->pargs->args[1], NULL, 10);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  t_us = timeval_subtract(&port_p->last_up_time, &port_p->last_en_time);
  aim_printf(&uc->pvs,
             "%4" PRIu64 ".%06" PRIu64 "\n",
             (t_us / 1000000),
             (t_us % 1000000));
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__but_us__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc,
                    "but_us",
                    2,
                    "Get bring-up time in microseconds "
                    "<dev_id> <dev_port>");
  int64_t t_us;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtoull(uc->pargs->args[0], NULL, 10);
  dev_port = strtoull(uc->pargs->args[1], NULL, 10);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  t_us = timeval_subtract(&port_p->last_up_time, &port_p->last_en_time);
  aim_printf(&uc->pvs, "%" PRIu64 "\n", t_us);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__lut__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "lut", 2, "Get link-up time <dev_id> <dev_port>");
  int64_t t_us;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtoull(uc->pargs->args[0], NULL, 10);
  dev_port = strtoull(uc->pargs->args[1], NULL, 10);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  t_us = timeval_subtract(&port_p->last_up_time, &port_p->last_sig_detect_time);
  aim_printf(&uc->pvs,
             "%4" PRIu64 ".%06" PRIu64 "\n",
             (t_us / 1000000),
             (t_us % 1000000));
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__lut_us__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc,
                    "lut_us",
                    2,
                    "Get link-up time in microseconds "
                    "<dev_id> <dev_port>");
  int64_t t_us;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtoull(uc->pargs->args[0], NULL, 10);
  dev_port = strtoull(uc->pargs->args[1], NULL, 10);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  t_us = timeval_subtract(&port_p->last_up_time, &port_p->last_sig_detect_time);
  aim_printf(&uc->pvs, "%" PRIu64 "\n", t_us);
  return 0;
}

void port_mgr_dump_timeval(ucli_context_t *uc, struct timeval *tm) {
  char tbuf[256] = {0};
  char ubuf[256] = {0};
  struct tm *loctime;

  if (!tm) return;

  if (!tm->tv_sec) {
    char t[] = "--- --- -- --:--:--";
    char tt[] = "------";
    aim_printf(&uc->pvs, "%s.%s", t, tt);
    return;
  }

  loctime = localtime(&tm->tv_sec);
  if (loctime == NULL) {
    aim_printf(&uc->pvs, "localtime returned null");
    return;
  }

  strftime(tbuf, sizeof(tbuf), "%a %b %d", loctime);
  aim_printf(&uc->pvs, "%s ", tbuf);

  strftime(ubuf, sizeof(ubuf), "%T\n", loctime);
  ubuf[strlen(ubuf) - 1] = 0;  // remove CR
  aim_printf(&uc->pvs, "%s.%06d", ubuf, (int)tm->tv_usec);
}

void port_mgr_dump_uptime(ucli_context_t *uc,
                          bf_dev_id_t dev_id,
                          bf_dev_port_t dev_port) {
  int64_t tt = 0;
  bool is_sw_model = false;
#if defined(DEVICE_IS_EMULATOR)
  bool is_sw_emu = true;
#else
  bool is_sw_emu = false;
#endif

  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p) return;

  port_mgr_dump_timeval(uc, &port_p->last_up_time);
  aim_printf(&uc->pvs, " | ");
  port_mgr_dump_timeval(uc, &port_p->last_dn_time);
  // dump time to come up
  if (port_p->sw.oper_state) {
    bf_drv_device_type_get(dev_id, &is_sw_model);
    if (is_sw_model || is_sw_emu || (port_mgr_dev_is_tof1(dev_id))) {
      tt = timeval_subtract(&port_p->last_up_time, &port_p->last_en_time);
    } else {
      if ((port_p->sw.lpbk_mode == BF_LPBK_NONE) &&
          (port_p->sw.port_dir == BF_PORT_DIR_DUPLEX)) {
        // accounts for link flap from remote
        tt = timeval_subtract(&port_p->last_up_time,
                              &port_p->last_sig_detect_time);
      } else {
        tt = timeval_subtract(&port_p->last_up_time, &port_p->last_en_time);
      }
    }

    if (tt < (int64_t)0) {
      tt = (int64_t)0;
    }
  }
  aim_printf(&uc->pvs, " | ");
  aim_printf(&uc->pvs,
             "%" PRIu64 ".%06" PRIu64 " sec",
             (tt / 1000000),
             (tt % 1000000));
}

int port_mgr_dump_this_pcs_status(ucli_context_t *uc,
                                  bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port) {
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  if ((port_p->sw.assigned) && (port_p->sw.enabled)) {
    bf_status_t bf_status;
    int state;
    bf_dev_pipe_t pipe;
    bf_dev_port_t port;
    bool pcs_status;
    uint32_t block_lock_per_pcs_lane;
    uint32_t alignment_marker_lock_per_pcs_lane;
    bool hi_ber;
    bool block_lock_all, alignment_marker_lock_all;
    int mac_block, ch;

    port_mgr_map_dev_port_to_all(
        dev_id, dev_port, &pipe, &port, &mac_block, &ch, NULL);

    bf_status = bf_port_oper_state_get_no_side_effect(dev_id, dev_port, &state);
    if (bf_status != BF_SUCCESS) {
      aim_printf(&uc->pvs,
                 "Error: bf_port_oper_state_get_no_side_effect failed: rc=%d, "
                 "dev_port=%x\n",
                 bf_status,
                 dev_port);
      return 0;
    }
    bf_status = bf_port_pcs_status_get(dev_id,
                                       dev_port,
                                       &pcs_status,
                                       &block_lock_per_pcs_lane,
                                       &alignment_marker_lock_per_pcs_lane,
                                       &hi_ber,
                                       &block_lock_all,
                                       &alignment_marker_lock_all);
    if (bf_status != BF_SUCCESS) {
      aim_printf(&uc->pvs,
                 "Error: bf_port_pcs_state_get failed: rc=%d, dev_port=%x\n",
                 bf_status,
                 dev_port);
      return 0;
    }
    aim_printf(&uc->pvs,
               "%d|%d| %2d | %2d |%d| %2s |%d|%d|%d| %05x | %05x | ",
               dev_id,
               pipe,
               port,
               mac_block,
               ch,
               state ? "Up" : "Dn",
               pcs_status,
               block_lock_all,
               hi_ber,
               block_lock_per_pcs_lane,
               alignment_marker_lock_per_pcs_lane);
    port_mgr_dump_uptime(uc, dev_id, dev_port);
    aim_printf(&uc->pvs, "\n");
    return 1;
  }
  return 0;
}

void dump_oper_banner(ucli_context_t *uc) {
  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      "-+-+----+----+-+----+-+-+-+-------+-------+----------------------------+"
      "--------------------------+\n");
  aim_printf(
      &uc->pvs,
      "c|p| i  | m  | | O  |p|b|h| block | align |                            "
      "|                          |\n");
  aim_printf(
      &uc->pvs,
      "h|i| b  | a  | | p  |c|l|i| lock  | marker|                            "
      "|                          |\n");
  aim_printf(
      &uc->pvs,
      "i|p| u  | c  | | e  |s|k|b|       | lock  |                            "
      "|                          |\n");
  aim_printf(
      &uc->pvs,
      "p|e| f  | b  |c| r  | |L|e|       |       |                            "
      "|                          |\n");
  aim_printf(
      &uc->pvs,
      " | |    | k  |h|    | |k|r|       |       | last up time               "
      "| last down time           | Bring-up time \n");
  aim_printf(
      &uc->pvs,
      "-+-+----+----+-+----+-+-+-+-------+-------+----------------------------+"
      "--------------------------+\n");
}

static ucli_status_t port_mgr_ucli_ucli__oper__(ucli_context_t *uc) {
  bf_dev_id_t dev_id, pipe;
  int min_fp_port, max_fp_port, port;
  int ports_per_banner = 32, ports_dumped = 0;

  UCLI_COMMAND_INFO(uc, "oper", 0, "Dump oper status of all configured ports");

  dump_oper_banner(uc);
  for (dev_id = 0; dev_id < BF_MAX_DEV_COUNT; dev_id++) {
    if (!port_mgr_dev_is_ready(dev_id)) continue;

    for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
      min_fp_port = lld_get_min_fp_port(dev_id);
      max_fp_port = lld_get_max_fp_port(dev_id);
      if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
      for (port = min_fp_port; port <= max_fp_port; port++) {
        bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

        if (ports_dumped == ports_per_banner) {
          dump_oper_banner(uc);
          ports_dumped = 0;
        }
        ports_dumped += port_mgr_dump_this_pcs_status(uc, dev_id, dev_port);
      }
    }
    aim_printf(&uc->pvs, "Cpu port(s):\n");
    for (port = lld_get_min_cpu_port(dev_id);
         port <= lld_get_max_cpu_port(dev_id);
         port++) {
      bf_dev_port_t dev_port = MAKE_DEV_PORT(0, port);

      port_mgr_dump_this_pcs_status(uc, dev_id, dev_port);
    }
  }
  return 0;
}

extern int port_mgr_dump_this_port_config(ucli_context_t *uc,
                                          bf_dev_id_t dev_id,
                                          bf_dev_pipe_t pipe,
                                          int port);
extern void port_mgr_dump_port_config_banner(ucli_context_t *uc);
void print_pcs_ctrs_banner(ucli_context_t *uc);
int port_mgr_dump_this_pcs_counters(ucli_context_t *uc,
                                    bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port);
void sd_print_prbs_banner(ucli_context_t *uc);
void sd_dump_this_prbs(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd);

static void port_mgr_dump_quad_port_config(ucli_context_t *uc,
                                           bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port) {
  uint32_t log_pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t log_port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  uint32_t ch;

  aim_printf(&uc->pvs, "\n");
  port_mgr_dump_port_config_banner(uc);

  for (ch = 0; ch < 4; ch++) {
    port_mgr_dump_this_port_config(uc, dev_id, log_pipe, log_port + ch);
  }
}

static void port_mgr_dump_quad_oper_status(ucli_context_t *uc,
                                           bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port) {
  uint32_t log_pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t log_port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  uint32_t ch;

  aim_printf(&uc->pvs, "\n");
  dump_oper_banner(uc);

  for (ch = 0; ch < 4; ch++) {
    dev_port = MAKE_DEV_PORT(log_pipe, log_port + ch);
    port_mgr_dump_this_pcs_status(uc, dev_id, dev_port);
  }
}

int port_mgr_dump_tof1_pcs_ctrs(ucli_context_t *uc,
                                bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port) {
  uint32_t log_pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t log_port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  uint32_t ch;
  int output = 0;

  for (ch = 0; ch < 4; ch++) {
    dev_port = MAKE_DEV_PORT(log_pipe, log_port + ch);
    output |= port_mgr_dump_this_pcs_counters(uc, dev_id, dev_port);
  }
  return output;
}

void port_mgr_dump_quad_pcs_ctrs(ucli_context_t *uc,
                                 bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port) {
  uint32_t log_pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t log_port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  uint32_t ch;
  int banner_needed;

  // aim_printf(&uc->pvs, "\n");
  // print_pcs_ctrs_banner(uc);

  for (ch = 0; ch < 4; ch++) {
    dev_port = MAKE_DEV_PORT(log_pipe, log_port + ch);
    banner_needed = port_mgr_dump_this_pcs_counters(uc, dev_id, dev_port);
    if (banner_needed && (ch != 3)) {
      print_pcs_ctrs_banner(uc);
    }
  }
}

static void port_mgr_dump_quad_serdes_status(ucli_context_t *uc,
                                             bf_dev_id_t dev_id,
                                             uint32_t mac_block) {
  uint32_t ch;

  aim_printf(&uc->pvs, "\n");
  sd_print_prbs_banner(uc);

  for (ch = 0; ch < 4; ch++) {
    int ring, sd;
    int rc = port_mgr_find_addr_for(
        dev_id, IP_TYPE_ETH_PMA, mac_block, ch, &ring, &sd);

    if (rc != 0) continue;

    sd_dump_this_prbs(uc, dev_id, ring, sd);
  }
}

static void port_mgr_dump_quad_status(ucli_context_t *uc,
                                      bf_dev_id_t dev_id,
                                      bf_dev_id_t dev_port) {
  int mac_block;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, NULL, NULL);

  // dump quads port config
  port_mgr_dump_quad_port_config(uc, dev_id, dev_port);

  // dump quads port oper status
  port_mgr_dump_quad_oper_status(uc, dev_id, dev_port);

  // dump quads port pcs counters
  port_mgr_dump_quad_pcs_ctrs(uc, dev_id, dev_port);

  // dump quads serdes status
  port_mgr_dump_quad_serdes_status(uc, dev_id, mac_block);
}

static ucli_status_t port_mgr_ucli_ucli__ch_ena__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  bf_dev_pipe_t log_pipe, pipe;
  int port, min_fp_port, max_fp_port;
  UCLI_COMMAND_INFO(
      uc, "ch_ena", 0, "Dump channel enables of all configured ports");

  aim_printf(
      &uc->pvs,
      "-+-+----+----+-+-----------------------------------+--------------------"
      "--+-----------------+\n");
  aim_printf(
      &uc->pvs,
      "c|p| i  | m  | |                ibuf               |      txff ctrl     "
      "  |       MAC       |\n");
  aim_printf(&uc->pvs,
             "h|i| b  | a  | |  ch 0  |  ch 1  |  ch 2  |  ch 3  "
             "|----+---+------+------+------+----------+\n");
  aim_printf(
      &uc->pvs,
      "i|p| u  | c  | +-+------+-+------+-+------+-+------|  s |   |      |    "
      "  |      | slot to  |\n");
  aim_printf(
      &uc->pvs,
      "p|e| f  | b  |c|e|      |e|      |e|      |e|      |  e | e | ch0  | "
      "ch1  |      | channel  |\n");
  aim_printf(
      &uc->pvs,
      " | |    | k  |h|n| mode |n| mode |n| mode |n| mode |  q | n | mode | "
      "mode | mode |   map    |\n");
  aim_printf(
      &uc->pvs,
      "-+-+----+----+-+----+-+-+-+------+-+------+-+------+----+---+------+----"
      "--+------+----------+\n");

  for (dev_id = 0; dev_id < BF_MAX_DEV_COUNT; dev_id++) {
    uint32_t num_active_pipes = 0;

    if (!port_mgr_dev_is_ready(dev_id)) continue;
    if (!port_mgr_dev_is_tof1(dev_id)) continue;

    lld_sku_get_num_active_pipes(dev_id, &num_active_pipes);

    for (log_pipe = 0; log_pipe < num_active_pipes; log_pipe++) {
      min_fp_port = lld_get_min_fp_port(dev_id);
      max_fp_port = lld_get_max_fp_port(dev_id);
      if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
      for (port = min_fp_port; port <= max_fp_port; port++) {
        bf_dev_port_t dev_port = MAKE_DEV_PORT(log_pipe, port);
        int mac_block, ch;
        uint32_t addr, data, ch_en[4], ch_mode[4];
        uint32_t ch_seq, ch_ena, ch0_mode, ch1_mode;
        uint32_t mac_mode, mac_slot2ch_map;
        uint32_t rc;

        rc = port_mgr_map_dev_port_to_all(
            dev_id, dev_port, &pipe, &port, &mac_block, &ch, NULL);
        if (rc != 0) continue;
        if (bf_port_is_valid(dev_id, dev_port) != BF_SUCCESS) continue;

        // read ibuf channel enables and modes (0=100g, 1=50g, 2=25g)
        addr = offsetof(Tofino,
                        pipes[pipe]
                            .pmarb.ibp18_reg.ibp_reg[port / 4]
                            .ing_buf_regs.chan0_group.chnl_ctrl);
        lld_read_register(dev_id, addr, &data);
        ch_en[0] = data & 1;
        ch_mode[0] = (data >> 2) & 3;

        addr = offsetof(Tofino,
                        pipes[pipe]
                            .pmarb.ibp18_reg.ibp_reg[port / 4]
                            .ing_buf_regs.chan1_group.chnl_ctrl);
        lld_read_register(dev_id, addr, &data);
        ch_en[1] = data & 1;
        ch_mode[1] = (data >> 2) & 3;

        addr = offsetof(Tofino,
                        pipes[pipe]
                            .pmarb.ibp18_reg.ibp_reg[port / 4]
                            .ing_buf_regs.chan2_group.chnl_ctrl);
        lld_read_register(dev_id, addr, &data);
        ch_en[2] = data & 1;
        ch_mode[2] = (data >> 2) & 3;

        addr = offsetof(Tofino,
                        pipes[pipe]
                            .pmarb.ibp18_reg.ibp_reg[port / 4]
                            .ing_buf_regs.chan3_group.chnl_ctrl);
        lld_read_register(dev_id, addr, &data);
        ch_en[3] = data & 1;
        ch_mode[3] = (data >> 2) & 3;

        // read ethregs txff_ctrl chnl_ena, chnl_seq, and modes
        addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.txff_ctrl);
        lld_read_register(dev_id, addr, &data);
        ch_seq = data & 0xff;
        ch_ena = (data >> 16) & 0xf;
        ch0_mode = (data >> 24) & 3;
        ch1_mode = (data >> 26) & 3;

        // get mac mode and slot2ch map
        mac_mode = port_mgr_mac_get_glb_mode(dev_id, dev_port);
        mac_slot2ch_map = port_mgr_mac_get_slot2ch_map(dev_id, dev_port);

        aim_printf(
            &uc->pvs,
            "%d|%d| %2d | %2d |%d|%d| %4s |%d| %4s |%d| %4s |%d| %4s | %02x | "
            "%01x | %4s | %4s | %04x | %08x |\n",
            dev_id,
            pipe,
            port,
            mac_block,
            ch,
            ch_en[0],
            ch_mode[0] == 0
                ? "100g"
                : ch_mode[0] == 1 ? "50g" : ch_mode[0] == 2 ? "25g" : "??",
            ch_en[1],
            ch_mode[1] == 0
                ? "100g"
                : ch_mode[1] == 1 ? "50g" : ch_mode[1] == 2 ? "25g" : "??",
            ch_en[2],
            ch_mode[2] == 0
                ? "100g"
                : ch_mode[2] == 1 ? "50g" : ch_mode[2] == 2 ? "25g" : "??",
            ch_en[3],
            ch_mode[3] == 0
                ? "100g"
                : ch_mode[3] == 1 ? "50g" : ch_mode[3] == 2 ? "25g" : "??",
            ch_seq,
            ch_ena,
            ch0_mode == 0
                ? "100g"
                : ch0_mode == 1 ? "50g" : ch0_mode == 2 ? "25g" : "??",
            ch1_mode == 0
                ? "100g"
                : ch1_mode == 1 ? "50g" : ch1_mode == 2 ? "25g" : "??",
            mac_mode,
            mac_slot2ch_map);
      }
      aim_printf(
          &uc->pvs,
          "-+-+--+----+-+--------+--------+--------+--------+--------+--------+"
          "--------+--"
          "----------+\n");
    }
    aim_printf(&uc->pvs, "Cpu port(s):\n");
    for (port = lld_get_min_cpu_port(dev_id);
         port <= lld_get_max_cpu_port(dev_id);
         port++) {
      // bf_dev_port_t dev_port = MAKE_DEV_PORT(0, port);
    }
  }
  return 0;
}

void print_pcs_ctrs_banner(ucli_context_t *uc) {
  aim_printf(
      &uc->pvs,
      "-+-+--+----+-+--------+--------+--------+--------+--------+--------+----"
      "----+--"
      "----------+\n");
  aim_printf(
      &uc->pvs,
      "c|p| p|    | |        |        |        |        |        |        |    "
      "    |  "
      "           \n");
  aim_printf(
      &uc->pvs,
      "h|i| o|    | |        |        |        |        | block  |        |    "
      "    |  "
      "           \n");
  aim_printf(
      &uc->pvs,
      "i|p| r|mac |c|        |        |        | sync   | lock   | valid  | "
      "invalid| "
      "unknown     \n");
  aim_printf(
      &uc->pvs,
      "p|e| t|blk |h|   ber  |err-blks| hi_ber | loss   | loss   | err    | "
      "err    | "
      "err         \n");
  aim_printf(
      &uc->pvs,
      "-+-+--+----+-+--------+--------+--------+--------+--------+--------+----"
      "----+--"
      "----------+\n");
}

int port_mgr_dump_this_pcs_counters(ucli_context_t *uc,
                                    bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port) {
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  if ((port_p->sw.assigned) && (port_p->sw.enabled)) {
    bf_status_t bf_status;
    bf_dev_pipe_t pipe;
    bf_dev_port_t port;
    uint32_t ber_cnt;
    uint32_t errored_blk_cnt;
    uint32_t sync_loss;
    uint32_t block_lock_loss;
    uint32_t hi_ber_cnt;
    uint32_t valid_error_cnt;
    uint32_t unknown_error_cnt;
    uint32_t invalid_error_cnt;
    uint32_t bip_errors_per_pcs_lane[20];
    int mac_block, ch;
    int ln, bip_err_fnd = 0;

    port_mgr_map_dev_port_to_all(
        dev_id, dev_port, &pipe, &port, &mac_block, &ch, NULL);

    bf_status = bf_port_pcs_counters_get(dev_id,
                                         dev_port,
                                         &ber_cnt,
                                         &errored_blk_cnt,
                                         &sync_loss,
                                         &block_lock_loss,
                                         &hi_ber_cnt,
                                         &valid_error_cnt,
                                         &unknown_error_cnt,
                                         &invalid_error_cnt,
                                         bip_errors_per_pcs_lane);
    if (bf_status != BF_SUCCESS) {
      aim_printf(&uc->pvs,
                 "Error: bf_port_pcs_counters_get failed: rc=%d, dev_port=%x\n",
                 bf_status,
                 dev_port);
      return 0;
    }
    for (ln = 0; ln < 20; ln++) {
      if (bip_errors_per_pcs_lane[ln] != 0) {
        bip_err_fnd = 1;
        break;
      }
    }
    if ((bip_err_fnd == 0) && (ber_cnt == 0) && (errored_blk_cnt == 0) &&
        (sync_loss == 0) && (block_lock_loss == 0) && (hi_ber_cnt == 0) &&
        (valid_error_cnt == 0) && (unknown_error_cnt == 0) &&
        (invalid_error_cnt == 0)) {
      return 0;
    }
    aim_printf(
        &uc->pvs,
        "%d|%d|%2d| %2d |%d| %6d | %6d | %6d | %6d | %6d | %6d | %6d | %6d \n",
        dev_id,
        pipe,
        port,
        mac_block,
        ch,
        ber_cnt,
        errored_blk_cnt,
        hi_ber_cnt,
        sync_loss,
        block_lock_loss,
        valid_error_cnt,
        invalid_error_cnt,
        unknown_error_cnt);

    if (bip_err_fnd) {
      aim_printf(
          &uc->pvs,
          "-+-+--+----+-+--------+--------+--------+--------+--------+-------"
          "-+--------+------------+\n");
      aim_printf(&uc->pvs,
                 "bip err : ln0-3   : %6d : %6d : %6d : %6d\n",
                 bip_errors_per_pcs_lane[0],
                 bip_errors_per_pcs_lane[1],
                 bip_errors_per_pcs_lane[2],
                 bip_errors_per_pcs_lane[3]);
      aim_printf(&uc->pvs,
                 "        : ln4-7   : %6d : %6d : %6d : %6d\n",
                 bip_errors_per_pcs_lane[4],
                 bip_errors_per_pcs_lane[5],
                 bip_errors_per_pcs_lane[6],
                 bip_errors_per_pcs_lane[7]);
      aim_printf(&uc->pvs,
                 "        : ln8-11  : %6d : %6d : %6d : %6d\n",
                 bip_errors_per_pcs_lane[8],
                 bip_errors_per_pcs_lane[9],
                 bip_errors_per_pcs_lane[10],
                 bip_errors_per_pcs_lane[11]);
      aim_printf(&uc->pvs,
                 "        : ln12-15 : %6d : %6d : %6d : %6d\n",
                 bip_errors_per_pcs_lane[12],
                 bip_errors_per_pcs_lane[13],
                 bip_errors_per_pcs_lane[14],
                 bip_errors_per_pcs_lane[15]);
      aim_printf(&uc->pvs,
                 "        : ln16-19 : %6d : %6d : %6d : %6d\n",
                 bip_errors_per_pcs_lane[16],
                 bip_errors_per_pcs_lane[17],
                 bip_errors_per_pcs_lane[18],
                 bip_errors_per_pcs_lane[19]);

      return 1;  // banner needs re-display
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__pcs_ctrs__(ucli_context_t *uc) {
  uint32_t dev_id, pipe;
  uint32_t n_pipes;
  int banner_needed = 0;
  int port, min_fp_port, max_fp_port;
  UCLI_COMMAND_INFO(
      uc, "pcs_ctrs", 0, "Dump PCS counters of all configured ports");

  print_pcs_ctrs_banner(uc);

  for (dev_id = 0; dev_id < BF_MAX_DEV_COUNT; dev_id++) {
    if (!port_mgr_dev_is_ready(dev_id)) continue;

    lld_sku_get_num_active_pipes(dev_id, &n_pipes);

    for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
      if (pipe >= n_pipes) break;
      min_fp_port = lld_get_min_fp_port(dev_id);
      max_fp_port = lld_get_max_fp_port(dev_id);
      if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
      for (port = min_fp_port; port <= max_fp_port; port++) {
        bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

        banner_needed = port_mgr_dump_this_pcs_counters(uc, dev_id, dev_port);
        if (banner_needed) {
          print_pcs_ctrs_banner(uc);
        }
      }
      aim_printf(
          &uc->pvs,
          "-+-+--+----+-+--------+--------+--------+--------+--------+--------+"
          "--------+------------+\n");
    }
    aim_printf(&uc->pvs, "Cpu port(s):\n");
    min_fp_port = lld_get_min_fp_port(dev_id);
    max_fp_port = lld_get_max_fp_port(dev_id);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      bf_dev_port_t dev_port = MAKE_DEV_PORT(0, port);

      banner_needed = port_mgr_dump_this_pcs_counters(uc, dev_id, dev_port);
      if (banner_needed) {
        print_pcs_ctrs_banner(uc);
      }
    }
  }
  return 0;
}

extern int port_mgr_mac_link_state(bf_dev_id_t dev_id,
                                   int mac_block,
                                   int channel,
                                   bool oper_state);

static ucli_status_t port_mgr_ucli_ucli__mac_poll__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int mac_block, ch;
  int i, n;
  int up = 0;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "mac_poll", 4, "Poll MAC link state <dev> <mac_block> <ch> <n>");
  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  mac_block = strtol(uc->pargs->args[1], NULL, 0);
  ch = strtol(uc->pargs->args[2], NULL, 0);
  n = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (i = 0; i < n; i++) {
    up = port_mgr_mac_link_state(dev_id, mac_block, ch, 0);
    if (up) {
      aim_printf(&uc->pvs,
                 "%d:%d:%d Link Up! After %d polls\n",
                 dev_id,
                 mac_block,
                 ch,
                 i);
      break;
    }
  }
  if (!up) {
    aim_printf(&uc->pvs,
               "%d:%d:%d Link down after %d polls\n",
               dev_id,
               mac_block,
               ch,
               i);
  }
  return 0;
}

bf_rmon_counter_array_t g_ctrs;

void ucli_mac_stats_callback(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             bf_rmon_counter_array_t *ctrs,
                             uint64_t dma_timestamp_nsec,
                             void *userdata) {
  bf_rmon_counter_t ctr_id;
  int any_non_zero = 0;
  ucli_context_t *uc = (ucli_context_t *)userdata;

  aim_printf(&uc->pvs, "RMON : dev_id=%d : dev_port=%d\n", dev_id, dev_port);

  for (ctr_id = 0; ctr_id < BF_NUM_RMON_COUNTERS; ctr_id++) {
    bf_status_t bf_status;
    char *ctr_str = NULL;

    if (ctrs->format.ctr_array[ctr_id] == 0) continue;

    bf_status = bf_port_rmon_counter_to_str(ctr_id, &ctr_str);
    if (bf_status != BF_SUCCESS) {
      if (ctr_str == NULL) {
        ctr_str = "invalid name";
      }
    }
    aim_printf(&uc->pvs,
               "%2d : %16" PRIu64 " : %s\n",
               ctr_id,
               ctrs->format.ctr_array[ctr_id],
               ctr_str);
    any_non_zero = 1;
  }
  if (any_non_zero == 0) {
    aim_printf(&uc->pvs, "RMON: All counters 0\n");
  }
  (void)userdata;
  (void)dma_timestamp_nsec;
}

// common hdlr that can be called from bf_pm_uclii as well
void ucli_r_rmon_hdlr(ucli_context_t *uc,
                      bf_dev_id_t dev_id,
                      bf_dev_port_t dev_port) {
  uint64_t dma_timestamp_nsec = 0;
  aim_printf(&uc->pvs, "bf_dev_port_t = %x\n", dev_port);

  bf_port_mac_stats_hw_sync_get(dev_id, dev_port, &g_ctrs);

  // use same dumper as dma'd stats
  ucli_mac_stats_callback(dev_id, dev_port, &g_ctrs, dma_timestamp_nsec, uc);
  return;
}

static ucli_status_t port_mgr_ucli_ucli__r_rmon__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int mac_block, ch;
  bf_dev_port_t dev_port;
  lld_err_t err;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "r_rmon",
      3,
      "Dump MAC stats (using register interface) <dev> <mac_block> <ch>");
  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  mac_block = strtol(uc->pargs->args[1], NULL, 0);
  ch = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err) {
    aim_printf(&uc->pvs,
               "r_rmon: error: %x : from lld_sku_map_mac_ch_to_dev_port_id: "
               "%d:%d:%d\n",
               err,
               dev_id,
               mac_block,
               ch);
    return 0;
  }
  ucli_r_rmon_hdlr(uc, dev_id, dev_port);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__rmon__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int mac_block, ch;
  bf_status_t rc;
  bf_dev_port_t dev_port;
  lld_err_t err;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc, "rmon", 3, "Dump MAC stats <dev> <mac_block> <ch>");
  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  mac_block = strtol(uc->pargs->args[1], NULL, 0);
  ch = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  err = lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err) {
    aim_printf(
        &uc->pvs,
        "rmon: error: %x : from lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        err,
        dev_id,
        mac_block,
        ch);
    return 0;
  }

  aim_printf(&uc->pvs, "bf_dev_port_t = %x\n", dev_port);

  rc = bf_port_mac_stats_hw_async_get(
      dev_id, dev_port, ucli_mac_stats_callback, uc);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__emu_setup__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int port;
  bf_status_t bf_sts;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc,
      "emu_setup",
      1,
      "Set up basic emulation environment (0-15 100g ports) <dev_id>");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  aim_printf(&uc->pvs, "Delete any existing ports..\n");
  for (port = 0; port < 64; port++) {
    bf_port_remove(dev_id, port);  // ignore errors
  }
  aim_printf(&uc->pvs, "Add back as 100g ports\n");
  for (port = 0; port < 64; port += 4) {
    bf_sts = bf_port_add(dev_id, port, BF_SPEED_100G, BF_FEC_TYP_NONE);

    if (bf_sts != BF_SUCCESS) {
      aim_printf(&uc->pvs, "Error: %x : adding port: %d\n", bf_sts, port);
    }
  }
  aim_printf(&uc->pvs, "Enable 100g ports\n");
  for (port = 0; port < 64; port += 4) {
    // lld_port_enable(0, port, true);
    bf_port_enable(dev_id, port, true);
  }

  aim_printf(&uc->pvs, "Wait for link-up on 100g ports..\n");
  for (port = 0; port < 64; port += 4) {
    int tries;

    aim_printf(&uc->pvs, "Waiting for port: %d..\n", port);

    for (tries = 0; tries < 1000; tries++) {
      int up;

      bf_sts = bf_port_oper_state_get_no_side_effect(dev_id, port, &up);
      if ((bf_sts == BF_SUCCESS) && up) {
        uint32_t addr;

        aim_printf(&uc->pvs, "Port: %d : Up after %d polls\n", port, tries);
        addr = offsetof(Tofino, macs_t[port / 4].macs.eth_regs.txff_ctrl);
        lld_write_register(dev_id, addr, 0x8010000);
        break;
      }
    }
  }
  return 0;
}

static void dump_port_cfg(ucli_context_t *uc,
                          port_cfg_settings_t *sw,
                          port_cfg_settings_t *hw) {
  aim_printf(&uc->pvs, "Field:                   Software : Hardware\n");

  aim_printf(&uc->pvs,
             "assigned                        %d : %d\n",
             sw->assigned,
             hw->assigned);
  aim_printf(&uc->pvs,
             "enabled                         %d : %d\n",
             sw->enabled,
             hw->enabled);
  aim_printf(&uc->pvs,
             "oper_state                      %d : %d\n",
             sw->oper_state,
             hw->oper_state);
  aim_printf(&uc->pvs,
             "speed                           %d : %d\n",
             sw->speed,
             hw->speed);
  aim_printf(&uc->pvs,
             "n_lanes                         %d : %d\n",
             sw->n_lanes,
             hw->n_lanes);
  aim_printf(
      &uc->pvs, "fec                             %d : %d\n", sw->fec, hw->fec);
  aim_printf(&uc->pvs,
             "fc_corr_en                      %d : %d\n",
             sw->fc_corr_en,
             hw->fc_corr_en);
  aim_printf(&uc->pvs,
             "fc_ind_en                       %d : %d\n",
             sw->fc_ind_en,
             hw->fc_ind_en);
  aim_printf(&uc->pvs,
             "tx_mtu                      %5d : %-5d\n",
             sw->tx_mtu,
             hw->tx_mtu);
  aim_printf(&uc->pvs,
             "rx_mtu                      %5d : %-5d\n",
             sw->rx_mtu,
             hw->rx_mtu);
  aim_printf(
      &uc->pvs, "ifg                            %d : %d\n", sw->ifg, hw->ifg);
  aim_printf(&uc->pvs,
             "preamble_length                %2d : %-2d\n",
             sw->preamble_length,
             hw->preamble_length);
  aim_printf(&uc->pvs,
             "promiscuous_mode                %d : %d\n",
             sw->promiscuous_mode,
             hw->promiscuous_mode);
  aim_printf(&uc->pvs,
             "link_pause_tx                   %d : %d\n",
             sw->link_pause_tx,
             hw->link_pause_tx);
  aim_printf(&uc->pvs,
             "link_pause_rx                   %d : %d\n",
             sw->link_pause_rx,
             hw->link_pause_rx);
  aim_printf(&uc->pvs,
             "pfc_pause_tx                    %d : %d\n",
             sw->pfc_pause_tx,
             hw->pfc_pause_tx);
  aim_printf(&uc->pvs,
             "pfc_pause_rx                    %d : %d\n",
             sw->pfc_pause_rx,
             hw->pfc_pause_rx);
  aim_printf(&uc->pvs,
             "loopback_enabled                %d : %d\n",
             sw->loopback_enabled,
             hw->loopback_enabled);
  aim_printf(&uc->pvs,
             "dfe_type                        %d : %d\n",
             sw->dfe_type,
             hw->dfe_type);
  aim_printf(&uc->pvs,
             "xoff_pause_time             %5d : %-5d\n",
             sw->xoff_pause_time,
             hw->xoff_pause_time);
  aim_printf(&uc->pvs,
             "xon_pause_time              %5d : %-5d\n",
             sw->xon_pause_time,
             hw->xon_pause_time);
  aim_printf(&uc->pvs,
             "txff_trunc_ctrl_size            %d : %d\n",
             sw->txff_trunc_ctrl_size,
             hw->txff_trunc_ctrl_size);
  aim_printf(&uc->pvs,
             "txff_trunc_ctrl_en              %d : %d\n",
             sw->txff_trunc_ctrl_en,
             hw->txff_trunc_ctrl_en);
  aim_printf(&uc->pvs,
             "txff_ctrl_crc_check_disable     %d : %d\n",
             sw->txff_ctrl_crc_check_disable,
             hw->txff_ctrl_crc_check_disable);
  aim_printf(&uc->pvs,
             "txff_ctrl_crc_removal_disable   %d : %d\n",
             sw->txff_ctrl_crc_removal_disable,
             hw->txff_ctrl_crc_removal_disable);
  aim_printf(&uc->pvs,
             "txff_ctrl_fcs_insert_disable    %d : %d\n",
             sw->txff_ctrl_fcs_insert_disable,
             hw->txff_ctrl_fcs_insert_disable);
  aim_printf(&uc->pvs,
             "txff_ctrl_pad_disable           %d : %d\n",
             sw->txff_ctrl_pad_disable,
             hw->txff_ctrl_pad_disable);
  aim_printf(&uc->pvs,
             "mac_addr        %02x:%02x:%02x:%02x:%02x:%02x :  "
             "%02x:%02x:%02x:%02x:%02x:%02x\n",
             sw->mac_addr[0],
             sw->mac_addr[1],
             sw->mac_addr[2],
             sw->mac_addr[3],
             sw->mac_addr[4],
             sw->mac_addr[5],
             hw->mac_addr[0],
             hw->mac_addr[1],
             hw->mac_addr[2],
             hw->mac_addr[3],
             hw->mac_addr[4],
             hw->mac_addr[5]);
  aim_printf(&uc->pvs,
             "fc_dst_mac_addr %02x:%02x:%02x:%02x:%02x:%02x :  "
             "%02x:%02x:%02x:%02x:%02x:%02x\n",
             sw->fc_dst_mac_addr[0],
             sw->fc_dst_mac_addr[1],
             sw->fc_dst_mac_addr[2],
             sw->fc_dst_mac_addr[3],
             sw->fc_dst_mac_addr[4],
             sw->fc_dst_mac_addr[5],
             hw->fc_dst_mac_addr[0],
             hw->fc_dst_mac_addr[1],
             hw->fc_dst_mac_addr[2],
             hw->fc_dst_mac_addr[3],
             hw->fc_dst_mac_addr[4],
             hw->fc_dst_mac_addr[5]);
  aim_printf(&uc->pvs,
             "fc_src_mac_addr %02x:%02x:%02x:%02x:%02x:%02x :  "
             "%02x:%02x:%02x:%02x:%02x:%02x\n",
             sw->fc_src_mac_addr[0],
             sw->fc_src_mac_addr[1],
             sw->fc_src_mac_addr[2],
             sw->fc_src_mac_addr[3],
             sw->fc_src_mac_addr[4],
             sw->fc_src_mac_addr[5],
             hw->fc_src_mac_addr[0],
             hw->fc_src_mac_addr[1],
             hw->fc_src_mac_addr[2],
             hw->fc_src_mac_addr[3],
             hw->fc_src_mac_addr[4],
             hw->fc_src_mac_addr[5]);
}

static ucli_status_t port_mgr_ucli_ucli__warm__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int pipe, port, mac_block, ch;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "warm", 3, "Compare sw/hw port cfg <dev> <pipe> <port>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }
  dev_port = MAKE_DEV_PORT(pipe, port);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) return 0;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);
  // rebuild port_p->hw
  if (port_mgr_dev_is_tof1(dev_id)) {
    port_mgr_mac_hw_cfg_get(dev_id, mac_block, ch);
  } else if (port_mgr_dev_is_tof2(dev_id)) {
    port_mgr_tof2_umac_hw_cfg_get(dev_id, mac_block, ch);
  }

  dump_port_cfg(uc, &port_p->sw, &port_p->hw);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__temp_start__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  bf_status_t bf_status;
  uint32_t channel;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "temp_start", 2, "Start a temperature reading <dev_id> <channel>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  channel = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status = bf_serdes_temperature_read_start(dev_id, 0, channel);
  if (bf_status != BF_SUCCESS) {
    aim_printf(&uc->pvs, "Error: %d : starting temp read\n", bf_status);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__temp_get__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  bf_status_t bf_status;
  uint32_t channel;
  uint32_t temp_mC;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "temp_get", 2, "Complete a temperature reading <dev_id> <channel>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  channel = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status = bf_serdes_temperature_read_get(dev_id, 0, channel, &temp_mC);
  aim_printf(&uc->pvs,
             "Temp: %d.%03d C (rc=%d)\n",
             temp_mC / 1000,
             temp_mC % 1000,
             bf_status);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__volt_start__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  bf_status_t bf_status;
  uint32_t channel;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "volt_start", 2, "Start a voltage reading <dev_id> <channel>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  channel = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status = bf_serdes_voltage_read_start(dev_id, 0, channel);
  if (bf_status != BF_SUCCESS) {
    aim_printf(&uc->pvs, "Error: %d : starting voltage read\n", bf_status);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__volt_get__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  bf_status_t bf_status;
  uint32_t channel;
  uint32_t volt_mV;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "volt_get", 2, "Complete a voltage reading <dev_id> <channel>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  channel = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status = bf_serdes_voltage_read_get(dev_id, 0, channel, &volt_mV);
  aim_printf(&uc->pvs,
             "Volt: %d.%03d V (rc=%d)\n",
             volt_mV / 1000,
             volt_mV % 1000,
             bf_status);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__qsts__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  uint32_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "qsts", 2, "Dump all quad related status <dev_id> <dev_port>");

  dev_id = strtoul(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);
  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }
  port_mgr_dump_quad_status(uc, dev_id, dev_port);
  return 0;
}

int check_quad_pcs_status(ucli_context_t *uc,
                          bf_dev_id_t dev_id,
                          bf_dev_port_t dev_port,
                          bool rst_cnts,
                          bool print_errors) {
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return 0;

  if ((port_p->sw.assigned) && (port_p->sw.enabled)) {
    bf_status_t bf_status;
    uint32_t ber_cnt;
    uint32_t errored_blk_cnt;
    uint32_t sync_loss;
    uint32_t block_lock_loss;
    uint32_t hi_ber_cnt;
    uint32_t valid_error_cnt;
    uint32_t unknown_error_cnt;
    uint32_t invalid_error_cnt;
    uint32_t bip_errors_per_pcs_lane[20];

    bf_status = bf_port_pcs_counters_get(dev_id,
                                         dev_port,
                                         &ber_cnt,
                                         &errored_blk_cnt,
                                         &sync_loss,
                                         &block_lock_loss,
                                         &hi_ber_cnt,
                                         &valid_error_cnt,
                                         &unknown_error_cnt,
                                         &invalid_error_cnt,
                                         bip_errors_per_pcs_lane);
    if (bf_status != BF_SUCCESS) {
      aim_printf(&uc->pvs,
                 "Error: bf_port_pcs_counters_get failed: rc=%d, dev_port=%x\n",
                 bf_status,
                 dev_port);
      return 0;
    }
    if (rst_cnts) return 0;

    if (print_errors) {
      int mac_block, ch;
      bf_dev_pipe_t pipe;
      bf_dev_port_t port;

      port_mgr_map_dev_port_to_all(
          dev_id, dev_port, &pipe, &port, &mac_block, &ch, NULL);

      aim_printf(
          &uc->pvs,
          "%d|%d|%2d| %2d |%d| %6d | %6d | %6d | %6d | %6d | %6d | %6d | %6d "
          "\n",
          dev_id,
          pipe,
          port,
          mac_block,
          ch,
          ber_cnt,
          errored_blk_cnt,
          hi_ber_cnt,
          sync_loss,
          block_lock_loss,
          valid_error_cnt,
          invalid_error_cnt,
          unknown_error_cnt);
    }
    if (ber_cnt || errored_blk_cnt || sync_loss || hi_ber_cnt ||
        valid_error_cnt || unknown_error_cnt || invalid_error_cnt) {
      return 1;
    }
  }
  return 0;
}

bool quad_check(ucli_context_t *uc,
                bf_dev_id_t dev_id,
                bf_dev_port_t dev_port,
                int aggressor_ch) {
  int ch;
  int pcs_errors;
  bool quad_failed = false;

  for (ch = 0; ch < 4; ch++) {
    // clear pcs stats
    check_quad_pcs_status(uc, dev_id, dev_port + ch, true, false);
  }
  // start dfe on aggressor channel
  bf_serdes_start_dfe_ical(dev_id, dev_port + aggressor_ch, 0);

  // wait for pcs stats on aggressor channel to be all 0
  do {
    // small delay
    bf_sys_usleep(1000000);
    pcs_errors = check_quad_pcs_status(
        uc, dev_id, dev_port + aggressor_ch, false, false);
  } while (pcs_errors);

  // check stats on victim channels
  for (ch = 0; ch < 4; ch++) {
    if (ch == 0) {
      print_pcs_ctrs_banner(uc);
    }
    if (ch == aggressor_ch) continue;

    pcs_errors = check_quad_pcs_status(uc, dev_id, dev_port + ch, false, true);
    if (pcs_errors) {
      quad_failed = true;
    }
  }
  return quad_failed;
}

static ucli_status_t port_mgr_ucli_ucli__qchk__(ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  uint32_t dev_port;
  int pass, num_passes, aggressor_ch;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "qchk", 3, "Check a quad for xtalk <dev_id> <dev_port> <passes>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);
  num_passes = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  // make sure parm is ch0
  dev_port = dev_port & ~3;

  for (pass = 0; pass < num_passes; pass++) {
    bool failed;
    aim_printf(&uc->pvs, "Pass: %d of %d\n", pass, num_passes);

    for (aggressor_ch = 0; aggressor_ch < 4; aggressor_ch++) {
      failed = quad_check(uc, dev_id, dev_port, aggressor_ch);
      if (failed) {
        aim_printf(&uc->pvs, "Aggressor Ch%d\n", aggressor_ch);
        return 0;
      }
    }
  }
  aim_printf(&uc->pvs, "Passed\n");
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__lt__(ucli_context_t *uc) {
  uint32_t dev_port;
  bf_lt_state_e lt_st;
  int ln, num_lanes;
  bf_status_t bf_status;
  bf_dev_id_t dev_id;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc, "lt", 2, "Dump link-training status <dev> <dev_port>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);

  if (!port_mgr_dev_is_tof1(dev_id)) return 0;

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_port_num_lanes_get(dev_id, dev_port, &num_lanes);

  for (ln = 0; ln < num_lanes; ln++) {
    bf_status = bf_serdes_link_training_st_get(dev_id, dev_port, ln, &lt_st);
    if (bf_status != BF_SUCCESS) {
      aim_printf(&uc->pvs,
                 "Error: %d : from bf_serdes_link_training_st_get",
                 bf_status);
    } else {
      aim_printf(&uc->pvs,
                 "%d:%03x:%d: %s\n",
                 dev_id,
                 dev_port,
                 ln,
                 (lt_st == BF_LT_ST_COMPLETE)
                     ? "complete"
                     : (lt_st == BF_LT_ST_RUNNING)
                           ? "running"
                           : (lt_st == BF_LT_ST_FAILED)
                                 ? "failed"
                                 : (lt_st == BF_LT_ST_NONE) ? "none" : "?");
    }
  }
  return 0;
}

extern bf_status_t bf_port_mac_stats_clear(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port);

static ucli_status_t port_mgr_ucli_ucli__bf_port_stats_clear__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "bf_port_stats_clear", 3, "Clear stats on <dev> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);

  rc = bf_port_mac_stats_clear(asic, dev_port);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_mac_stats_clear failed: rc=%d, "
               "dev_port=%x\n",
               rc,
               dev_port);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_lf_rf_get__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  bool latched_lf, latched_rf;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_lf_rf_get",
                    3,
                    "Get LF and RF interrupt state on <dev> <pipe> <port>");

  asic = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  rc = bf_port_lf_rf_get(asic, dev_port, &latched_lf, &latched_rf);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_lf_rf_get failed: rc=%d, "
               "dev_port=%x\n",
               rc,
               dev_port);
  } else {
    aim_printf(
        &uc->pvs, "Latched LF=%d : Latched RF=%d\n", latched_lf, latched_rf);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_link_fault_status_get__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port;
  bf_dev_port_t dev_port;
  bf_port_link_fault_st_t fault_status;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_link_fault_status_get",
                    3,
                    "Get link fault status on <dev> <pipe> <port>");

  asic = atoi(uc->pargs->args[0]);
  pipe = atoi(uc->pargs->args[1]);
  port = atoi(uc->pargs->args[2]);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  rc = bf_port_link_fault_status_get(asic, dev_port, &fault_status);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: bf_port_link_fault_status_get failed: rc=%d, "
               "dev_port=%d\n",
               rc,
               dev_port);
  } else {
    aim_printf(&uc->pvs,
               "Link fault status: %s\n",
               fault_status == BF_PORT_LINK_FAULT_OK
                   ? "Link OK"
                   : fault_status == BF_PORT_LINK_FAULT_LOC_FAULT
                         ? "Local Fault"
                         : fault_status == BF_PORT_LINK_FAULT_REM_FAULT
                               ? "Remote Fault"
                               : "ERROR: invalid link fault status");
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_port_stats_clearm__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int rc, asic, port, min_fp_port, max_fp_port;
  uint64_t map[BF_PIPE_COUNT];
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_port_stats_clearm",
                    5,
                    "Clear stats on multiple ports "
                    "<dev> <map[3]> <map[2]> <map[1]> <map[0]>");

  memset(map, 0, sizeof(map));
  asic = strtol(uc->pargs->args[0], NULL, 0);
  map[3] = strtoull(uc->pargs->args[1], NULL, 16);
  map[2] = strtoull(uc->pargs->args[2], NULL, 16);
  map[1] = strtoull(uc->pargs->args[3], NULL, 16);
  map[0] = strtoull(uc->pargs->args[4], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(asic);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  for (pipe = 0; pipe < BF_PIPE_COUNT; pipe++) {
    min_fp_port = lld_get_min_fp_port(asic);
    max_fp_port = lld_get_max_fp_port(asic);
    if ((min_fp_port < 0) || (max_fp_port < 0)) return 0;
    for (port = min_fp_port; port <= max_fp_port; port++) {
      if ((map[pipe] & (1ull << port)) == 0) continue;

      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe, port);

      rc = bf_port_mac_stats_clear(asic, dev_port);

      if (rc != BF_SUCCESS) {
        continue;  // ignore errors (usu ports that are not defined
        aim_printf(&uc->pvs,
                   "Error: bf_port_mac_stats_clear failed: rc=%d, "
                   "dev_port=%x\n",
                   rc,
                   dev_port);
        return 0;
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_serdes_tx_patsel_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int dev_id, port, lane, pat;
  bf_dev_port_t dev_port;
  bf_status_t bf_status;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_serdes_tx_patsel_set",
                    5,
                    "Set Tx pattern on <dev> <pipe> <port> <lane> <pattern>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);
  lane = strtol(uc->pargs->args[3], NULL, 0);
  pat = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  bf_status = bf_serdes_tx_patsel_set(dev_id, dev_port, lane, pat);
  if (bf_status != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: %d : from bf_serdes_tx_patsel_set %d %d %d %d\n",
               bf_status,
               dev_id,
               dev_port,
               lane,
               pat);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_serdes_rx_patsel_set__(
    ucli_context_t *uc) {
  bf_dev_pipe_t pipe;
  int dev_id, port, lane, pat;
  bf_dev_port_t dev_port;
  bf_status_t bf_status;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "bf_serdes_rx_patsel_set",
                    5,
                    "Set Rx pattern on <dev> <pipe> <port> <lane> <pattern>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  pipe = strtol(uc->pargs->args[1], NULL, 0);
  port = strtol(uc->pargs->args[2], NULL, 0);
  lane = strtol(uc->pargs->args[3], NULL, 0);
  pat = strtol(uc->pargs->args[4], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  dev_port = MAKE_DEV_PORT(pipe, port);
  bf_status = bf_serdes_rx_patsel_set(dev_id, dev_port, lane, pat);
  if (bf_status != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: %d : from bf_serdes_rx_patsel_set %d %d %d %d\n",
               bf_status,
               dev_id,
               dev_port,
               lane,
               pat);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__bf_serdes_map_dev_port_to_ring_sd__(
    ucli_context_t *uc) {
  bf_dev_id_t dev_id;
  int lane;
  bf_dev_port_t dev_port;
  bf_status_t bf_status;
  uint32_t hw_addr1, hw_addr2;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "map_dev_port_to_ring_sd",
                    3,
                    "Map dev_port to corresponding ring/sd for serdes commands "
                    "<dev> <dev_port> <lane>");

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);
  lane = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status = bf_serdes_hw_addr_get(dev_id,
                                    dev_port,
                                    lane,
                                    true,
                                    &hw_addr1, /*ring*/
                                    &hw_addr2 /*sd*/);
  if (bf_status != BF_SUCCESS) {
    aim_printf(&uc->pvs,
               "Error: %d : from bf_serdes_hw_addr_get %d %d %d\n",
               bf_status,
               dev_id,
               dev_port,
               lane);
  }
  aim_printf(&uc->pvs, "ring=%d\n", hw_addr1);
  aim_printf(&uc->pvs, "sd=%d\n", hw_addr2);

  return 0;
}

void fp_montara(ucli_context_t *uc) {
  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      " 1/-       3/-         5/-       7/-         9/-       11/-        13/- "
      "     15/-        17/-      19/-        21/-      23/-        25/-      "
      "27/-        29/-      31/-\n");
  aim_printf(
      &uc->pvs,
      " M46       M42         M38       M34         M30       M26         M22  "
      "     M18         M14       M10         M6        M2          M62       "
      "M58         M52       M48\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|   1   | |   1   |   |   1   | |   1   |   |   0   | |   0   |   |   0 "
      "  | |   0   |   |   0   | |   0   |   |   0   | |   0   |   |   1   | | "
      "  1   |   |   1   | |   1   |\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "| | | | | | | | | |   | | | | | | | | | |   |1|1|1|1| |1|1|1|1|   "
      "|1|1|1|1| | | | | |   | | | | | | | | | |   | | | | | | | | | |   "
      "|1|1|1|1| |1|1|1|1|   | | | | | | | | | |\n");
  aim_printf(
      &uc->pvs,
      "|5|6|5|6| |4|4|4|4|   |2|2|2|2| |1| |1|1|   |3|3|3|3| |1|1|1|1|   "
      "|0|0|0|0| |8|8|8|8|   |6|6|7|7| |5|5|5|5|   |3|3|3|3| |2|1|2|2|   "
      "|2|2|2|2| |0|0|0|0|   |8|8|8|8| |6|6|6|6|\n");
  aim_printf(
      &uc->pvs,
      "|9|1|8|0| |2|3|4|5|   |6|5|8|7| |0|9|2|1|   |3|2|5|4| |7|6|9|8|   "
      "|1|0|3|2| |5|4|7|6|   |8|9|0|1| |2|4|3|5|   |6|5|8|7| |0|9|2|1|   "
      "|3|2|5|4| |7|6|9|8|   |2|3|4|5| |7|8|9|6|\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      " 3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 "
      "1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   "
      "3 2 1 0     3 2 1 0   3 2 1 0\n");
  aim_printf(&uc->pvs, "\n");
  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      " 2/-       4/-         6/-       8/-         10/-      12/-        14/- "
      "     16/-        18/-      20/-        22/-      24/-        26/-      "
      "28/-        30/-      32/-\n");
  aim_printf(
      &uc->pvs,
      " M44       M40         M36       M32         M28       M24         M20  "
      "     M16         M12       M8          M4        M0          M60       "
      "M56         M54       M50 \n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|   1   | |   1   |   |   1   | |   1   |   |   0   | |   0   |   |   0 "
      "  | |   0   |   |   0   | |   0   |   |   0   | |   0   |   |   1   | | "
      "  1   |   |   1   | |   1   |\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "| | | | | | | | | |   | | | | | | | | | |   |1|1|1|1| |1|1|1|1|   | | | "
      "| | | | | | |   | | | | | | | | | |   | | | | | | | | | |   |1|1|1|1| | "
      "| |1|1|   | | | | | | | | | |\n");
  aim_printf(
      &uc->pvs,
      "|5|5|5|5| |3|3|3|3|   |1|1|2|1| | | | | |   |2|2|2|2| |0|0|1|1|   "
      "|9|9|9|9| |7|7|7|7|   |6|6|6|6| |4|4|4|4|   |2|2|3|2| |1|1|1|1|   "
      "|1|1|1|1| |9|9|0|0|   |9|9|9|9| |7|7|7|7|\n");
  aim_printf(
      &uc->pvs,
      "|1|0|3|2| |5|4|7|6|   |8|7|0|9| |2|1|4|3|   |5|4|7|6| |9|8|1|0|   "
      "|5|3|4|2| |7|6|9|8|   |1|0|2|3| |5|4|7|6|   |8|7|0|9| |2|1|4|3|   "
      "|5|4|7|6| |9|8|1|0|   |0|1|2|3| |5|4|7|6|\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      " 3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 "
      "1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   "
      "3 2 1 0     3 2 1 0   3 2 1 0\n");
  aim_printf(&uc->pvs, "\n");
}

void fp_mavericks(ucli_context_t *uc) {
  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      " 1/-       3/-         5/-       7/-         9/-       11/-        13/- "
      "     15/-        17/-      19/-        21/-      23/-        25/-      "
      "27/-       29/-      31/-\n");
  aim_printf(
      &uc->pvs,
      " M31       M29         M27       M25         M23       M21         M19  "
      "     M17         M15       M13         M11       M9          M7        "
      "M5         M3        M1\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|   0   | |   0   |   |   0   | |   0   |   |   0   | |   0   |   |   0 "
      "  | |   0   |   |   0   | |   0   |   |   0   | |   0   |   |   0   | | "
      "  0   |   |   0   | |   0   |\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|1|1|1|1| |1|1|1|1|   |1|1|1|1| |1|1|1|1|   |1|1|1|1| | | | | |   | | | "
      "| | | | | | |   | | | | | | | | | |   | | | | | | | | | |   | | | | | | "
      "| | | |   | | | | | | | | | |\n");
  aim_printf(
      &uc->pvs,
      "|3|3|3|3| |2|3|3|2|   |2|2|2|2| |1|1|1|1|   |0|0|0|0| |9|9|9|9|   "
      "|8|9|8|9| |8|8|8|8|   |7|7|7|7| |6|6|6|6|   |5|5|5|5| |4|4|5|5|   "
      "|4|3|4|4| |3|3|3|3|   |2|2|2|2| |1|1|1|1|\n");
  aim_printf(
      &uc->pvs,
      "|7|6|9|8| |8|0|1|9|   |1|0|3|2| |3|2|4|5|   |4|6|5|7| |6|7|9|8|   "
      "|9|1|8|0| |0|1|3|2|   |2|3|4|5| |4|5|6|7|   |6|7|9|8| |8|9|0|1|   "
      "|0|9|1|2| |2|1|4|3|   |4|6|3|5| |7|5|6|8|\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      " 3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 "
      "1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   "
      "3 2 1 0     3 2 1 0   3 2 1 0\n");
  aim_printf(&uc->pvs, "\n");
  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      " 2/-       4/-         6/-       8/-         10/-      12/-        14/- "
      "     16/-        18/-      20/-        22/-      24/-        26/-     "
      "28/-        30/-      32/-\n");
  aim_printf(
      &uc->pvs,
      " M30       M28         M26       M24         M22       M20         M18  "
      "     M16         M14       M12         M10       M8          M6       "
      "M4          M2        M0 \n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|   0   | |   0   |   |   0   | |   0   |   |   0   | |   0   |   |   0 "
      "  | |   0   |   |   0   | |   0   |   |   0   | |   0   |   |   0   | | "
      "  0   |   |   0   | |   0   |\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|1|1|1|1| |1|1|1|1|   |1|1|1|1| |1|1|1|1|   |1|1|1|1| | | | | |   | | | "
      "| | | | | | |   | | | | | | | | | |   | | | | | | | | | |   | | | | | | "
      "| | | |   | | | | | | | | | |\n");
  aim_printf(
      &uc->pvs,
      "|3|3|3|3| |2|2|2|2|   |1|1|1|1| |0|0|1|1|   |0|0|0|0| |9|9|9|9|   "
      "|8|8|8|8| |7|7|7|7|   |6|6|7|7| |6|6|6|6|   |5|5|5|5| |4|4|4|4|   "
      "|3|3|3|3| |2|2|3|2|   |1|2|2|2| |1|1|1|1|\n");
  aim_printf(
      &uc->pvs,
      "|3|2|5|4| |4|5|6|7|   |7|6|9|8| |8|9|1|0|   |1|0|2|3| |3|2|5|4|   "
      "|5|4|7|6| |6|7|8|9|   |8|9|0|1| |0|1|2|3|   |3|2|4|5| |4|6|5|7|   "
      "|5|6|8|7| |8|7|0|9|   |9|0|1|2| |3|1|4|2|\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      " 3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 "
      "1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   "
      "3 2 1 0     3 2 1 0   3 2 1 0\n");
  aim_printf(&uc->pvs, "\n\n");

  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      " 33/-      35/-        37/-      39/-        41/-      43/-        45/- "
      "     47/-        49/-      51/-        53/-      55/-        57/-      "
      "59/-        61/-      63/-\n");
  aim_printf(
      &uc->pvs,
      " M40       M42         M44       M46         M32       M34         M36  "
      "     M38         M56       M58         M60       M62         M48       "
      "M50        M52       M54\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|   1   | |   1   |   |   1   | |   1   |   |   1   | |   1   |   |   1 "
      "  | |   1   |   |   1   | |   1   |   |   1   | |   1   |   |   1   | | "
      "  1   |   |   1   | |   1   |\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "| | | | | | | | | |   | | | | | | | | | |   | | | | | | | | | |   | | | "
      "| | | | | | |   |1|1| | | |1|1|1|1|   |1|1|1|1| |1|1|1|1|   | | | | | | "
      "| | | |   | | | | | | | | | |\n");
  aim_printf(
      &uc->pvs,
      "|3|3|3|3| |4|4|4|4|   |5|5|5|5| |6|6|5|5|   | | | | | | |1|1|1|   "
      "|1|1|2|1| |2|2|2|2|   |0|0|9|9| |0|0|0|0|   |1|1|1|1| |2|2|2|2|   "
      "|6|6|6|6| |7|7|7|7|   |8|8|8|8| |9|9|9|9|\n");
  aim_printf(
      &uc->pvs,
      "|6|7|4|5| |5|3|4|2|   |2|3|0|1| |0|1|8|9|   |4|1|2|3| |9|0|1|2|   "
      "|8|7|0|9| |5|6|7|8|   |1|0|9|8| |9|8|7|6|   |7|6|5|4| |5|4|3|2|   "
      "|6|7|9|8| |5|4|7|6|   |3|2|5|4| |0|1|2|3|\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      " 3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 "
      "1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   "
      "3 2 1 0     3 2 1 0   3 2 1 0\n");
  aim_printf(&uc->pvs, "\n");
  aim_printf(&uc->pvs, "\n");
  aim_printf(
      &uc->pvs,
      " 34/-      36/-        38/-      40/-        42/-      44/-        46/- "
      "     48/-        50/-      52/-        54/-      56/-        58/-      "
      "60/-        62/-      64/-\n");
  aim_printf(
      &uc->pvs,
      " M41       M43         M45       M47         M33       M35         M37  "
      "     M39         M57       M59         M61       M63         M49       "
      "M51         M53       M55 \n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "|   1   | |   1   |   |   1   | |   1   |   |   1   | |   1   |   |   1 "
      "  | |   1   |   |   1   | |   1   |   |   1   | |   1   |   |   1   | | "
      "  1   |   |   1   | |   1   |\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      "| | | | | | | | | |   | | | | | | | | | |   | | | | | | | | | |   | | | "
      "| | | | | | |   |1|1|1|1| |1|1|1|1|   |1|1|1|1| |1|1|1|1|   | | | | | | "
      "| | | |   | | | | | | | | | |\n");
  aim_printf(
      &uc->pvs,
      "|4|4|3|3| |4|4|4|4|   |5|5|5|5| |6|6|6|6|   | | | | | |1|1|1|1|   "
      "|2|2|2|2| |2|3|3|3|   |0|0|0|0| |1|1|1|1|   |2|2|1|1| |2|2|2|2|   "
      "|7|7|7|7| |7|7|8|8|   |8|8|8|8| |9|9|9|9|\n");
  aim_printf(
      &uc->pvs,
      "|0|1|8|9| |9|8|7|6|   |6|7|4|5| |4|5|2|3|   |7|8|5|6| |6|4|3|5|   "
      "|2|1|3|4| |9|0|1|2|   |5|4|2|3| |3|2|1|0|   |1|0|9|8| |9|8|7|6|   "
      "|1|0|3|2| |9|8|1|0|   |7|6|8|9| |4|5|6|7|\n");
  aim_printf(
      &uc->pvs,
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+   +-------+ +-------+   "
      "+-------+ +-------+   +-------+ +-------+\n");
  aim_printf(
      &uc->pvs,
      " 3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 "
      "1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   3 2 1 0     3 2 1 0   "
      "3 2 1 0     3 2 1 0   3 2 1 0\n");
  aim_printf(&uc->pvs, "\n");
}

static ucli_status_t port_mgr_ucli_ucli__fp__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "fp", 1, "Display front-port views");
  if (((uc->pargs->args[0][0] == 'm') || (uc->pargs->args[0][0] == 'M')) &&
      ((uc->pargs->args[0][1] == 'a') || (uc->pargs->args[0][1] == 'A'))) {
    fp_mavericks(uc);
  } else if (((uc->pargs->args[0][0] == 'm') ||
              (uc->pargs->args[0][0] == 'M')) &&
             ((uc->pargs->args[0][1] == 'o') ||
              (uc->pargs->args[0][1] == 'O'))) {
    fp_montara(uc);
  } else {
    aim_printf(&uc->pvs, "Available platforms are mavericks and montara\n");
  }
  return 0;
}

extern bf_status_t bf_device_warm_init_end(bf_dev_id_t dev_id);

static ucli_status_t port_mgr_ucli_ucli__warm_init_end__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "warm_init_end", 1, "Terminate fast-reconfig <dev_id>");
  bf_dev_id_t dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return UCLI_STATUS_E_ARG;
  }
  bf_status_t status = bf_device_warm_init_end(dev_id);
  if (status != BF_SUCCESS) {
    printf("Error %d terminating fast-reconfig\n", status);
  }
  return 0;
}

// hack
// Tof2 ucli commands
ucli_context_t *cur_uc = NULL;
bool only_nz = true;  // only dump non-0 counts by default

extern void umac4_ctrs_rmon_dump(bf_dev_id_t dev_id,
                                 uint32_t umac,
                                 uint32_t ch);
extern void umac4_ctrs_pcs_dump(bf_dev_id_t dev_id, uint32_t umac, uint32_t ch);
extern void umac4_ctrs_rs_fec_dump(bf_dev_id_t dev_id,
                                   uint32_t umac,
                                   uint32_t ch);
extern void umac4_ctrs_pcs_vl_dump(bf_dev_id_t dev_id, uint32_t umac);
extern void umac4_ctrs_rs_fec_ln_dump(bf_dev_id_t dev_id, uint32_t umac);
extern void umac4_ctrs_fc_fec_ln_dump(bf_dev_id_t dev_id, uint32_t umac);

void u64_ctr_display(int n,
                     uint64_t *ctr_array,
                     char *name_array[],
                     char *desc_array[]) {
  int c;
  bool all_0 = true;

  aim_printf(&cur_uc->pvs,
             "-------------------+---------------------------------------------"
             "+----------------------------\n");
  aim_printf(&cur_uc->pvs,
             "  Count            |          Counter Name                       "
             "| Description\n");
  aim_printf(&cur_uc->pvs,
             "-------------------+---------------------------------------------"
             "+----------------------------\n");
  for (c = 0; c < n; c++) {
    if (only_nz && (ctr_array[c] == 0ull)) continue;

    all_0 = false;
    aim_printf(&cur_uc->pvs,
               "%16" PRIu64 " : %43s : %s\n",
               ctr_array[c],
               name_array[c],
               desc_array[c]);
  }
  if (all_0) {
    aim_printf(&cur_uc->pvs, "  all counters 0\n");
  }
}

bf_status_t port_mgr_tof2_map_dev_port_to_all(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t *pipe_id,
                                              uint32_t *port_id,
                                              uint32_t *umac,
                                              uint32_t *ch,
                                              bool *is_cpu_port);

void port_mgr_tof2_dump_ctrs(bf_dev_id_t dev_id,
                             uint32_t umac,
                             uint32_t ch_ln,
                             char *ctr_type) {
  bool all = false;

  if (strcmp("all", ctr_type) == 0) {
    all = true;
  }
  if (all || (strcmp("rmon", ctr_type) == 0)) {
    aim_printf(&cur_uc->pvs, "\n\nRMON Counters:\n");
    umac4_ctrs_rmon_dump(dev_id, umac, ch_ln);
  }
  if (all || (strcmp("pcs", ctr_type) == 0)) {
    aim_printf(&cur_uc->pvs, "\n\nPCS  Counters:\n");
    umac4_ctrs_pcs_dump(dev_id, umac, ch_ln);
  }
  if (all || (strcmp("vl", ctr_type) == 0)) {
    aim_printf(&cur_uc->pvs, "\n\nPCS Virtual Lane Counters:\n");
    umac4_ctrs_pcs_vl_dump(dev_id, umac);
  }
  if (all || (strcmp("fec", ctr_type) == 0)) {
    aim_printf(&cur_uc->pvs, "\n\nRS FEC  Counters:\n");
    umac4_ctrs_rs_fec_dump(dev_id, umac, ch_ln);
  }
  if (all || (strcmp("rs_fec_ln", ctr_type) == 0)) {
    aim_printf(&cur_uc->pvs, "\n\nRS FEC Per Lane Counters:\n");
    umac4_ctrs_rs_fec_ln_dump(dev_id, umac);
  }
  if (all || (strcmp("fc_fec_ln", ctr_type) == 0)) {
    aim_printf(&cur_uc->pvs, "\n\nFC FEC Per Lane Counters:\n");
    umac4_ctrs_fc_fec_ln_dump(dev_id, umac);
  }
}

static ucli_status_t port_mgr_ucli_ucli__ctr_dp__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "ctr_dp", 3, "Counter dump for a dev_port");
  char *ctr_type;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  uint32_t umac;
  uint32_t channel;
  bool is_cpu_port;
  port_mgr_dev_t *dev_p = NULL;

  cur_uc = uc;  // save for callback

  ctr_type = (char *)uc->pargs->args[0];
  dev_id = strtol(uc->pargs->args[1], NULL, 0);
  dev_port = strtol(uc->pargs->args[2], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &umac, &channel, &is_cpu_port);
  if (sts != BF_SUCCESS) return sts;

  port_mgr_tof2_dump_ctrs(dev_id, umac, channel, ctr_type);

  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__ctr__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "ctr", 4, "Counter dump for a port");
  int umac, ch_ln;
  char *ctr_type;
  bf_dev_id_t dev_id;
  port_mgr_dev_t *dev_p = NULL;

  cur_uc = uc;  // save for callback

  ctr_type = (char *)uc->pargs->args[0];
  dev_id = strtol(uc->pargs->args[1], NULL, 0);
  umac = strtol(uc->pargs->args[2], NULL, 0);
  ch_ln = strtol(uc->pargs->args[3], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_mgr_tof2_dump_ctrs(dev_id, umac, ch_ln, ctr_type);

  return 0;
}

extern bf_status_t port_mgr_tof2_umac4_status_get(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t channel,
                                                  uint64_t *reg64,  // raw value
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
                                                  uint64_t *rxamsf);
bf_status_t port_mgr_tof2_umac3_status_get(bf_dev_id_t dev_id,
                                           uint32_t umac3,
                                           uint32_t ch,
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
                                           uint32_t *ch3_tx_idle);

static ucli_status_t port_mgr_ucli_ucli__chsts__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "chsts", 2, "Read chstsX register for dev_port");
  uint64_t txclkpresentall = 0ull;
  uint64_t rxclkpresentall = 0ull;
  uint64_t rxsigokall = 0ull;
  uint64_t blocklockall = 0ull;
  uint64_t amlockall = 0ull;
  uint64_t aligned = 0ull;
  uint64_t nohiber = 0ull;
  uint64_t nolocalfault = 0ull;
  uint64_t noremotefault = 0ull;
  uint64_t linkup = 0ull;
  uint64_t hiser = 0ull;
  uint64_t fecdegser = 0ull;
  uint64_t rxamsf = 0ull;
  uint64_t reg64 = 0ull;
  bf_status_t rc;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  uint32_t umac;
  uint32_t channel;
  bool is_cpu_port;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &umac, &channel, &is_cpu_port);
  if (sts != BF_SUCCESS) return sts;

  if (is_cpu_port) {
    uint32_t reg32;
    uint32_t ch0_link_sts, ch1_link_sts, ch2_link_sts, ch3_link_sts;
    uint32_t ch0_rx_fault, ch1_rx_fault, ch2_rx_fault, ch3_rx_fault;
    uint32_t ch0_sig_ok, ch1_sig_ok, ch2_sig_ok, ch3_sig_ok;
    uint32_t ch0_tx_idle, ch1_tx_idle, ch2_tx_idle, ch3_tx_idle;

    rc = port_mgr_tof2_umac3_status_get(dev_id,
                                        umac,
                                        channel,
                                        &reg32,
                                        &ch0_link_sts,
                                        &ch1_link_sts,
                                        &ch2_link_sts,
                                        &ch3_link_sts,
                                        &ch0_rx_fault,
                                        &ch1_rx_fault,
                                        &ch2_rx_fault,
                                        &ch3_rx_fault,
                                        &ch0_sig_ok,
                                        &ch1_sig_ok,
                                        &ch2_sig_ok,
                                        &ch3_sig_ok,
                                        &ch0_tx_idle,
                                        &ch1_tx_idle,
                                        &ch2_tx_idle,
                                        &ch3_tx_idle);
    if (rc) {
      aim_printf(&uc->pvs, "       %d : Error\n", dev_id);
    }
    aim_printf(&uc->pvs, "       %d : dev_id\n", dev_id);
    aim_printf(&uc->pvs, "    %i4d : dev_port <%xh> \n", dev_port, dev_port);
    aim_printf(&uc->pvs, "%08x : dev_id\n", reg32);
    if (channel == 0) {
      aim_printf(&uc->pvs,
                 "       %d : CH0 %s\n",
                 ch0_link_sts,
                 ch0_link_sts ? "Up" : "Dn");
      aim_printf(&uc->pvs, "       %d : CH0 Rx Fault\n", ch0_rx_fault);
      aim_printf(&uc->pvs, "       %d : CH0 Rx Sig OK\n", ch0_sig_ok);
      aim_printf(&uc->pvs, "       %d : CH0 Tx Idle\n", ch0_tx_idle);
    } else if (channel == 1) {
      aim_printf(&uc->pvs,
                 "       %d : CH1 %s\n",
                 ch1_link_sts,
                 ch1_link_sts ? "Up" : "Dn");
      aim_printf(&uc->pvs, "       %d : CH1 Rx Fault\n", ch1_rx_fault);
      aim_printf(&uc->pvs, "       %d : CH1 Rx Sig OK\n", ch1_sig_ok);
      aim_printf(&uc->pvs, "       %d : CH1 Tx Idle\n", ch1_tx_idle);
    } else if (channel == 2) {
      aim_printf(&uc->pvs,
                 "       %d : CH2 %s\n",
                 ch2_link_sts,
                 ch2_link_sts ? "Up" : "Dn");
      aim_printf(&uc->pvs, "       %d : CH2 Rx Fault\n", ch2_rx_fault);
      aim_printf(&uc->pvs, "       %d : CH2 Rx Sig OK\n", ch2_sig_ok);
      aim_printf(&uc->pvs, "       %d : CH2 Tx Idle\n", ch2_tx_idle);
    } else if (channel == 3) {
      aim_printf(&uc->pvs,
                 "       %d : CH3 %s\n",
                 ch3_link_sts,
                 ch3_link_sts ? "Up" : "Dn");
      aim_printf(&uc->pvs, "       %d : CH3 Rx Fault\n", ch3_rx_fault);
      aim_printf(&uc->pvs, "       %d : CH3 Rx Sig OK\n", ch3_sig_ok);
      aim_printf(&uc->pvs, "       %d : CH3 Tx Idle\n", ch3_tx_idle);
    }
    return 0;
  }

  rc = port_mgr_tof2_umac4_status_get(dev_id,
                                      umac,
                                      channel,
                                      &reg64,
                                      &txclkpresentall,
                                      &rxclkpresentall,
                                      &rxsigokall,
                                      &blocklockall,
                                      &amlockall,
                                      &aligned,
                                      &nohiber,
                                      &nolocalfault,
                                      &noremotefault,
                                      &linkup,
                                      &hiser,
                                      &fecdegser,
                                      &rxamsf);

  if (rc != BF_SUCCESS) {
    aim_printf(&uc->pvs, "Warning: rc=%d : results may be incorrect\n", rc);
  }
  aim_printf(&uc->pvs, "%16" PRIu64 ": dev_id\n", (uint64_t)dev_id);
  aim_printf(&uc->pvs, "%16" PRIu64 ": dev_port\n", (uint64_t)dev_port);
  aim_printf(&uc->pvs, "%16" PRIx64 ": raw value\n", reg64);
  aim_printf(&uc->pvs, "%16" PRIu64 ": txclkpresentall\n", txclkpresentall);
  aim_printf(&uc->pvs, "%16" PRIu64 ": rxclkpresentall\n", rxclkpresentall);
  aim_printf(&uc->pvs, "%16" PRIu64 ": rxsigokall\n", rxsigokall);
  aim_printf(&uc->pvs, "%16" PRIu64 ": blocklockall\n", blocklockall);
  aim_printf(&uc->pvs, "%16" PRIu64 ": amlockall\n", amlockall);
  aim_printf(&uc->pvs, "%16" PRIu64 ": aligned\n", aligned);
  aim_printf(&uc->pvs, "%16" PRIu64 ": nohiber\n", nohiber);
  aim_printf(&uc->pvs, "%16" PRIu64 ": nolocalfault\n", nolocalfault);
  aim_printf(&uc->pvs, "%16" PRIu64 ": noremotefault\n", noremotefault);
  aim_printf(&uc->pvs, "%16" PRIu64 ": linkup\n", linkup);
  aim_printf(&uc->pvs, "%16" PRIu64 ": hiser\n", hiser);
  aim_printf(&uc->pvs, "%16" PRIu64 ": fecdegser\n", fecdegser);
  aim_printf(&uc->pvs, "%16" PRIu64 ": rxamsf\n", rxamsf);
  return 0;
}

extern bf_status_t port_mgr_tof2_umac4_interrupt_get(bf_dev_id_t dev_id,
                                                     uint32_t umac,
                                                     uint32_t channel,
                                                     uint64_t *reg64);

static ucli_status_t port_mgr_ucli_ucli__chint__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc, "chint", 2, "Read channel interrupts  for dev_port");
  uint64_t reg64 = 0ull;
  bf_status_t rc;
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  uint32_t umac;
  uint32_t channel;
  bool is_cpu_port;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &umac, &channel, &is_cpu_port);
  if (sts != BF_SUCCESS) return sts;

  if (is_cpu_port) {
  } else {
    rc = port_mgr_tof2_umac4_interrupt_get(dev_id, umac, channel, &reg64);

    if (rc != BF_SUCCESS) {
      aim_printf(&uc->pvs, "Warning: rc=%d : results may be incorrect\n", rc);
    }
    aim_printf(&uc->pvs, "%16" PRIu64 ": dev_id\n", (uint64_t)dev_id);
    aim_printf(&uc->pvs, "%16" PRIu64 ": dev_port\n", (uint64_t)dev_port);
    aim_printf(&uc->pvs, "%16" PRIx64 ": raw value\n", reg64);
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 0 ?TX FIFO LEVEL      TX Application FIFO Overflow, TX "
               "Packet Underflow, Tx Packing Overflow\n",
               ((reg64 >> 0ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 1 ?TX PROTOCOL ERR ?  TX Protocol Violation on SOF/EOF/VLD "
               "Input to Application FIFO\n",
               ((reg64 >> 1ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64 ": 2  TX JABBER PKT      TX Packet Jabbered\n",
               ((reg64 >> 2ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 3 ?TX SERDES FIFO ?   TX SERDES Buffer Overflow, TX SERDES "
               "Buffer Underflow\n",
               ((reg64 >> 3ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64 ": 4  RX PCS GEARBOX     RX PCS Gearbox Overflow\n",
               ((reg64 >> 4ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64 ": 5 ?RX PCS DESKEW ?    RX PCS Deskew Overflow\n",
               ((reg64 >> 5ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 6 ?RX PCS HIBER       Rx PCS hiber, hiser error triggered\n",
               ((reg64 >> 6ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 7  RX PCS ERROR       RX PCS Sync Header Error, Block Error, "
               "Codeword Error or BIP Error\n",
               ((reg64 >> 7ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 8 ?RX FAULT           RX Fault (Ordered Set) Received (or "
               "Generated by RX PCS)\n",
               ((reg64 >> 8ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 9 ?RX FRAME DROP"
               "   ?  Frame Dropped due to bad SFD/PREAMBLE\n",
               ((reg64 >> 9ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 10 RX FCS ERR Invalid FCS on Received Packet or PCS Error "
               "character found inside of packet\n",
               ((reg64 >> 10ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64 ": 11 RX JABBER PKT ?    Rx Packet Jabbered\n",
               ((reg64 >> 11ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 12 RX RUNT DROP ?     Rx Runt Packet Filtered/Dropped\n",
               ((reg64 >> 12ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64
               ": 13 RX FIFO LEVEL ?    RX Application FIFO Overflow, RX "
               "unpacking Overflow\n",
               ((reg64 >> 13ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64 ": 14 RX LINK LOST ?     Link Lost\n",
               ((reg64 >> 14ul) & 1ul));
    aim_printf(&uc->pvs,
               "%16" PRIx64 ": 15 RX LINK GAIN ? ?   Link Gain\n",
               ((reg64 >> 15ul) & 1ul));
  }
  return 0;
}

bf_status_t port_mgr_tof2_umac4_chmode_get(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch,
                                           uint64_t *reg64);
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
                                                  uint64_t *ifgpacing);
bf_status_t port_mgr_tof2_umac4_maccfg_get(bf_dev_id_t dev_id,
                                           uint32_t umac4,
                                           uint32_t ch,
                                           uint64_t *reg64);
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
                                                  uint64_t *txpfcen);
bf_status_t port_mgr_tof2_umac4_chconfig30_get(bf_dev_id_t dev_id,
                                               uint32_t umac4,
                                               uint32_t ch,
                                               uint64_t *reg64);
bf_status_t port_mgr_tof2_umac4_sdcfg_get(bf_dev_id_t dev_id,
                                          uint32_t umac4,
                                          uint32_t ch,
                                          uint64_t *reg64);
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
                                                 uint64_t *rxprbssel);
bf_status_t port_mgr_tof2_umac4_sderrcfg_get(bf_dev_id_t dev_id,
                                             uint32_t umac4,
                                             uint32_t ch,
                                             uint64_t *reg64);
bf_status_t port_mgr_tof2_umac4_sderrcfg_detail_get(bf_dev_id_t dev_id,
                                                    uint32_t umac4,
                                                    uint32_t ch,
                                                    uint64_t *txerrperiod,
                                                    uint64_t *txerrburst);
bf_status_t port_mgr_tof2_umac4_sdsts_get(bf_dev_id_t dev_id,
                                          uint32_t umac4,
                                          uint32_t ch,
                                          uint64_t *reg64);
bf_status_t port_mgr_tof2_umac4_sdsts_detail_get(bf_dev_id_t dev_id,
                                                 uint32_t umac4,
                                                 uint32_t ch,
                                                 uint64_t *txclkpresent,
                                                 uint64_t *txclkrate,
                                                 uint64_t *rxclkpresent,
                                                 uint64_t *rxclkrate,
                                                 uint64_t *sigok,
                                                 uint64_t *rxprbserrcnt);
bf_status_t port_mgr_tof2_umac4_chconfig30_detail_get(bf_dev_id_t dev_id,
                                                      uint32_t umac4,
                                                      uint32_t ch,
                                                      uint64_t *ifgppm,
                                                      uint64_t *rxmaxfrmsize,
                                                      uint64_t *txpreamble,
                                                      uint64_t *txdrainonfault,
                                                      uint64_t *rxpreamble,
                                                      uint64_t *rxerrmask);
bf_status_t port_mgr_tof2_eth400g_mac_txff_ctrl_get(bf_dev_id_t dev_id,
                                                    uint32_t umac4,
                                                    uint32_t ch,
                                                    uint32_t *reg32);
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
    uint32_t *min_thr);
bf_status_t port_mgr_tof2_eth400g_mac_chnl_seq_get(bf_dev_id_t dev_id,
                                                   uint32_t umac4,
                                                   uint32_t ch,
                                                   uint32_t *reg32);
bf_status_t port_mgr_tof2_eth400g_serdes_mode_get(bf_dev_id_t dev_id,
                                                  uint32_t umac4,
                                                  uint32_t ch,
                                                  uint32_t *tx_sds_mode,
                                                  bool *tx_is_pam4,
                                                  uint32_t *tx_phys_ln,
                                                  uint32_t *rx_sds_mode,
                                                  bool *rx_is_pam4,
                                                  uint32_t *rx_phys_ln);

void dump_umac4_cfg(ucli_context_t *uc,
                    bf_dev_id_t dev_id,
                    bf_dev_port_t dev_port,
                    uint32_t umac4,
                    uint32_t ch) {
  uint64_t reg64;
  uint32_t reg32;
  uint64_t mode, txswrst, rxswrst, txen, txdrain, rxen, gmiilpbk, txjabber,
      rxjabber, disfcs, invfcs, ignfcs, stripfcs, ifglen, ifgpacing;
  uint64_t disfcsonerr;
  uint64_t txfcen;
  uint64_t rxfcen;
  uint64_t rxpfcen;
  uint64_t rxfctotx;
  uint64_t rxfilterfc;
  uint64_t rxfilterpfc;
  uint64_t txpadrunt;
  uint64_t txwrthresh;
  uint64_t txrdthresh;
  uint64_t txlfault;
  uint64_t txrfault;
  uint64_t txidle;
  uint64_t rxpadrunt;
  uint64_t rxlfault;
  uint64_t rxrfault;
  uint64_t rxidle;
  uint64_t statsclr;
  uint64_t txignorerx;
  uint64_t txpfcen;
  uint64_t ifgppm;
  uint64_t rxmaxfrmsize;
  uint64_t txpreamble;
  uint64_t txdrainonfault;
  uint64_t rxpreamble;
  uint64_t rxerrmask;
  uint32_t chnl_ena;
  uint32_t tx_flush;
  uint32_t chnl_mode;
  uint32_t rx_xoff_mode;
  uint32_t ovr_rx_pfcxoff;
  uint32_t txrx_lpbk;
  uint32_t val_rx_pfcxoff;
  uint32_t cred_ini;
  uint32_t min_thr;
  uint32_t force_hi_raw_val;
  uint32_t force_lo_raw_val;
  uint32_t force_hi;
  uint32_t force_lo;
  uint32_t tx_sds_mode;
  bool tx_is_pam4;
  uint32_t tx_phys_ln;
  uint32_t rx_sds_mode;
  bool rx_is_pam4;
  uint32_t rx_phys_ln;

  port_mgr_tof2_umac4_chmode_get(dev_id, umac4, ch, &reg64);
  aim_printf(&uc->pvs, "%16" PRIx64 ": chmode\n", reg64);
  port_mgr_tof2_umac4_chmode_detail_get(dev_id,
                                        umac4,
                                        ch,
                                        &mode,
                                        &txswrst,
                                        &rxswrst,
                                        &txen,
                                        &txdrain,
                                        &rxen,
                                        &gmiilpbk,
                                        &txjabber,
                                        &rxjabber,
                                        &disfcs,
                                        &invfcs,
                                        &ignfcs,
                                        &stripfcs,
                                        &ifglen,
                                        &ifgpacing);
  aim_printf(&uc->pvs, "%16" PRIx64 ": mode\n", mode);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txswrst\n", txswrst);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxswrst\n", rxswrst);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txen\n", txen);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txdrain\n", txdrain);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxen\n", rxen);
  aim_printf(&uc->pvs, "%16" PRIx64 ": gmiilpbk\n", gmiilpbk);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txjabber\n", txjabber);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxjabber\n", rxjabber);
  aim_printf(&uc->pvs, "%16" PRIx64 ": disfcs\n", disfcs);
  aim_printf(&uc->pvs, "%16" PRIx64 ": invfcs\n", invfcs);
  aim_printf(&uc->pvs, "%16" PRIx64 ": ignfcs\n", ignfcs);
  aim_printf(&uc->pvs, "%16" PRIx64 ": stripfcs\n", stripfcs);
  aim_printf(&uc->pvs, "%16" PRIx64 ": ifglen\n", ifglen);
  aim_printf(&uc->pvs, "%16" PRIx64 ": ifgpacing\n", ifgpacing);

  bf_port_forced_sigok_get(dev_id,
                           dev_port,
                           &force_hi_raw_val,
                           &force_lo_raw_val,
                           &force_hi,
                           &force_lo);
  aim_printf(
      &uc->pvs, "        %08x: force_rxsigok_high (raw)\n", force_hi_raw_val);
  aim_printf(
      &uc->pvs, "        %08x: force_rxsigok_high (for dev_port)\n", force_hi);
  aim_printf(
      &uc->pvs, "        %08x: force_rxsigok_low (raw)\n", force_lo_raw_val);
  aim_printf(
      &uc->pvs, "        %08x: force_rxsigok_low (for dev_port)\n", force_lo);

  port_mgr_tof2_eth400g_serdes_mode_get(dev_id,
                                        umac4,
                                        ch,
                                        &tx_sds_mode,
                                        &tx_is_pam4,
                                        &tx_phys_ln,
                                        &rx_sds_mode,
                                        &rx_is_pam4,
                                        &rx_phys_ln);
  aim_printf(&uc->pvs, "        %08x: Tx Serdes Mode\n", tx_sds_mode);
  aim_printf(
      &uc->pvs, "                : Tx %s\n", tx_is_pam4 ? "PAM4" : "NRZ");
  aim_printf(&uc->pvs, "        %08x: Tx Physical lane\n", tx_phys_ln);
  aim_printf(&uc->pvs, "        %08x: Rx Serdes Mode\n", rx_sds_mode);
  aim_printf(
      &uc->pvs, "                : Rx %s\n", tx_is_pam4 ? "PAM4" : "NRZ");
  aim_printf(&uc->pvs, "        %08x: Rx Physical lane\n", rx_phys_ln);

  port_mgr_tof2_umac4_maccfg_get(dev_id, umac4, ch, &reg64);
  aim_printf(&uc->pvs, "%16" PRIx64 ": maccfg\n", reg64);

  port_mgr_tof2_umac4_maccfg_detail_get(dev_id,
                                        umac4,
                                        ch,
                                        &disfcsonerr,
                                        &txfcen,
                                        &rxfcen,
                                        &rxpfcen,
                                        &rxfctotx,
                                        &rxfilterfc,
                                        &rxfilterpfc,
                                        &txpadrunt,
                                        &txwrthresh,
                                        &txrdthresh,
                                        &txlfault,
                                        &txrfault,
                                        &txidle,
                                        &rxpadrunt,
                                        &rxlfault,
                                        &rxrfault,
                                        &rxidle,
                                        &statsclr,
                                        &txignorerx,
                                        &txpfcen);

  aim_printf(&uc->pvs, "%16" PRIx64 ": disfcsonerr\n", disfcsonerr);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txfcen\n", txfcen);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxfcen\n", rxfcen);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxpfcen\n", rxpfcen);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxfctotx\n", rxfctotx);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxfilterfc\n", rxfilterfc);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxfilterpfc\n", rxfilterpfc);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txpadrunt\n", txpadrunt);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txwrthresh\n", txwrthresh);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txrdthresh\n", txrdthresh);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txlfault\n", txlfault);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txrfault\n", txrfault);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txidle\n", txidle);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxpadrunt\n", rxpadrunt);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxlfault\n", rxlfault);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxrfault\n", rxrfault);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxidle\n", rxidle);
  aim_printf(&uc->pvs, "%16" PRIx64 ": statsclr\n", statsclr);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txignorerx\n", txignorerx);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txpfcen\n", txpfcen);

  port_mgr_tof2_umac4_chconfig30_get(dev_id, umac4, ch, &reg64);
  aim_printf(&uc->pvs, "%16" PRIx64 ": chconfig30\n", reg64);

  port_mgr_tof2_umac4_chconfig30_detail_get(dev_id,
                                            umac4,
                                            ch,
                                            &ifgppm,
                                            &rxmaxfrmsize,
                                            &txpreamble,
                                            &txdrainonfault,
                                            &rxpreamble,
                                            &rxerrmask);
  aim_printf(&uc->pvs, "%16" PRIx64 ": ifgppm\n", ifgppm);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxmaxfrmsize\n", rxmaxfrmsize);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txpreamble\n", txpreamble);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txdrainonfault\n", txdrainonfault);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxpreamble\n", rxpreamble);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxerrmask\n", rxerrmask);

  uint64_t serdeslpbk;
  uint64_t txremap;
  uint64_t sigokoverride;
  uint64_t rxremap;
  uint64_t txinv;
  uint64_t rxinv;
  uint64_t paceren;
  uint64_t pacerdiv;
  uint64_t txprbssel;
  uint64_t rxprbssel;
  uint64_t txerrperiod;
  uint64_t txerrburst;

  port_mgr_tof2_umac4_sdcfg_get(dev_id, umac4, ch, &reg64);
  aim_printf(&uc->pvs, "%16" PRIx64 ": sdcfg\n", reg64);

  port_mgr_tof2_umac4_sdcfg_detail_get(dev_id,
                                       umac4,
                                       ch,
                                       &serdeslpbk,
                                       &txremap,
                                       &sigokoverride,
                                       &rxremap,
                                       &txinv,
                                       &rxinv,
                                       &paceren,
                                       &pacerdiv,
                                       &txprbssel,
                                       &rxprbssel);

  aim_printf(&uc->pvs, "%16" PRIx64 ": serdeslpbk\n", serdeslpbk);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txremap\n", txremap);
  aim_printf(&uc->pvs, "%16" PRIx64 ": sigokoverride\n", sigokoverride);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxremap\n", rxremap);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txinv\n", txinv);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxinv\n", rxinv);
  aim_printf(&uc->pvs, "%16" PRIx64 ": paceren\n", paceren);
  aim_printf(&uc->pvs, "%16" PRIx64 ": pacerdiv\n", pacerdiv);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txprbssel\n", txprbssel);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxprbssel\n", rxprbssel);

  port_mgr_tof2_umac4_sderrcfg_get(dev_id, umac4, ch, &reg64);
  aim_printf(&uc->pvs, "%16" PRIx64 ": sderrcfg\n", reg64);

  port_mgr_tof2_umac4_sderrcfg_detail_get(
      dev_id, umac4, ch, &txerrperiod, &txerrburst);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txerrperiod\n", txerrperiod);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txerrburst\n", txerrburst);

  port_mgr_tof2_umac4_sdsts_get(dev_id, umac4, ch, &reg64);
  aim_printf(&uc->pvs, "%16" PRIx64 ": sdsts\n", reg64);

  uint64_t txclkpresent;
  uint64_t txclkrate;
  uint64_t rxclkpresent;
  uint64_t rxclkrate;
  uint64_t sigok;
  uint64_t rxprbserrcnt;
  port_mgr_tof2_umac4_sdsts_detail_get(dev_id,
                                       umac4,
                                       ch,
                                       &txclkpresent,
                                       &txclkrate,
                                       &rxclkpresent,
                                       &rxclkrate,
                                       &sigok,
                                       &rxprbserrcnt);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txclkpresent\n", txclkpresent);
  aim_printf(&uc->pvs, "%16" PRIx64 ": txclkrate\n", txclkrate);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxclkpresent\n", rxclkpresent);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxclkrate\n", rxclkrate);
  aim_printf(&uc->pvs, "%16" PRIx64 ": sigok\n", sigok);
  aim_printf(&uc->pvs, "%16" PRIx64 ": rxprbserrcnt\n", rxprbserrcnt);

  port_mgr_tof2_eth400g_mac_txff_ctrl_get(dev_id, umac4, ch, &reg32);
  aim_printf(&uc->pvs, "        %08x: txff_ctrl\n", reg32);

  port_mgr_tof2_eth400g_mac_txff_ctrl_detail_get(dev_id,
                                                 umac4,
                                                 ch,
                                                 &chnl_ena,
                                                 &tx_flush,
                                                 &chnl_mode,
                                                 &rx_xoff_mode,
                                                 &ovr_rx_pfcxoff,
                                                 &txrx_lpbk,
                                                 &val_rx_pfcxoff,
                                                 &cred_ini,
                                                 &min_thr);
  aim_printf(&uc->pvs, "        %8x: chnl_ena\n", chnl_ena);
  aim_printf(&uc->pvs, "        %8x: tx_flush\n", tx_flush);
  aim_printf(&uc->pvs, "        %8x: chnl_mode\n", chnl_mode);
  aim_printf(&uc->pvs, "        %8x: rx_xoff_mode\n", rx_xoff_mode);
  aim_printf(&uc->pvs, "        %8x: ovr_rx_pfcxoff\n", ovr_rx_pfcxoff);
  aim_printf(&uc->pvs, "        %8x: txrx_lpbk\n", txrx_lpbk);
  aim_printf(&uc->pvs, "        %8x: val_rx_pfcxoff\n", val_rx_pfcxoff);
  aim_printf(&uc->pvs, "        %8x: cred_ini\n", cred_ini);
  aim_printf(&uc->pvs, "        %8x: min_thr\n", min_thr);

  port_mgr_tof2_eth400g_mac_chnl_seq_get(dev_id, umac4, ch, &reg32);
  aim_printf(&uc->pvs, "        %08x: chnl_seq\n", reg32);
}

static ucli_status_t port_mgr_ucli_ucli__chcfg__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(
      uc, "chcfg", 2, "Read channel config registers for dev_port");
  bf_dev_id_t dev_id;
  bf_dev_port_t dev_port;
  uint32_t umac;
  uint32_t channel;
  bool is_cpu_port;
  port_mgr_dev_t *dev_p = NULL;

  dev_id = strtol(uc->pargs->args[0], NULL, 0);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &umac, &channel, &is_cpu_port);
  if (sts != BF_SUCCESS) return sts;

  if (is_cpu_port) {
  } else {
    dump_umac4_cfg(uc, dev_id, dev_port, umac, channel);
  }
  return 0;
}

extern uint32_t port_mgr_tof2_serdes_tile_rd(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ofs);
extern void port_mgr_tof2_serdes_tile_wr(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ofs,
                                         uint32_t reg_val);

static ucli_status_t port_mgr_ucli_ucli__tile_rd__(ucli_context_t *uc) {
  bf_dev_id_t dev_id = 0;
  bf_dev_port_t dev_port;
  uint32_t ofs, val;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "tile-rd", 3, "Serdes Tile Read <dev_id> <dev-port> <reg>");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);
  ofs = strtoull(uc->pargs->args[2], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  val = port_mgr_tof2_serdes_tile_rd(dev_id, dev_port, ofs);

  aim_printf(&uc->pvs,
             "%d: d_p=%d : Tile Rd : %8x : %04x\n",
             dev_id,
             dev_port,
             ofs,
             val);

  return 0;
}

bf_status_t port_mgr_port_config_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port);

void port_mgr_ucli_dump_port_config(ucli_context_t *uc,
                                    uint32_t conn_id,
                                    uint32_t chnl_id,
                                    bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port) {
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) return;

  /* read config into port_p->hw. */
  port_mgr_port_config_get(dev_id, dev_port);

  aim_printf(&uc->pvs,
             "%4d/%d | %d | %3d | %d (%d) | %d (%d) | %02x (%02x) | %02x "
             "(%02x) | %5d (%5d) | %5d (%5d) | %2d (%2d) | %d (%2d) |\n",
             conn_id,
             chnl_id,
             dev_id,
             dev_port,
             port_p->sw.link_pause_tx,
             port_p->hw.link_pause_tx,
             port_p->sw.link_pause_rx,
             port_p->hw.link_pause_rx,
             port_p->sw.pfc_pause_tx,
             port_p->hw.pfc_pause_tx,
             port_p->sw.pfc_pause_rx,
             port_p->hw.pfc_pause_rx,
             port_p->sw.tx_mtu,
             port_p->hw.tx_mtu,
             port_p->sw.rx_mtu,
             port_p->hw.rx_mtu,
             port_p->sw.ifg,
             port_p->hw.ifg,
             port_p->sw.preamble_length,
             port_p->hw.preamble_length);
}

static ucli_status_t port_mgr_ucli_ucli__tile_wr__(ucli_context_t *uc) {
  bf_dev_id_t dev_id = 0;
  bf_dev_port_t dev_port;
  uint32_t ofs, val;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(
      uc, "tile-wr", 4, "Serdes Tile Write <dev_id> <dev-port> <reg> <val>");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtol(uc->pargs->args[1], NULL, 0);
  ofs = strtoull(uc->pargs->args[2], NULL, 16);
  val = strtoull(uc->pargs->args[3], NULL, 16);

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_mgr_tof2_serdes_tile_wr(dev_id, dev_port, ofs, val);

  aim_printf(&uc->pvs,
             "%d: d_p=%d : Tile Wr : %8x : %04x\n",
             dev_id,
             dev_port,
             ofs,
             val);

  return 0;
}

/*
    for lane in lanes:
        reg_group_dump(0x0000 + 0x800 * lane, range(0x000, 0x1FF + 1, 1), 'Per
   Lane Register', filename)
        reg_group_dump(0x0000 + 0x800 * lane, range(0x0C0, 0x0FF + 1, 1),
   'ANA_Reg', filename)
        reg_group_dump(0x0500 + 0x800 * lane, range(0x000, 0x00B + 1, 1),
   'Aneg_lt Registers', filename)
        reg_group_dump(0x01C0 + 0x800 * lane, range(0x000, 0x00D + 1, 1), 'FEC
   Analyzer Registers', filename)
        reg_group_dump(0x0200 + 0x800 * lane, range(0x000, 0x00C + 1, 1),
   'LANE_SLICE', filename)
        reg_group_dump(0x0400 + 0x800 * lane, range(0x000, 0x06C + 1, 1),
   'Training Registers', filename)
        reg_group_dump(0x0300 + 0x800 * lane, range(0x000, 0x0FC + 1, 1), 'IEEE
   AN Registers', filename)
        reg_group_dump(0x0600 + 0x800 * lane, range(0x000, 0x047 + 1, 1), 'AN
   CSTM Registers', filename)
        #reg_group_dump(0x4000, range(0x000, 0x0EF, 1), 'GROUP8 Registers',
   filename)
        reg_group_dump(0x4B00, range(0x03F, 0x0FF, 1), 'TSensor, VSensor
   Registers', filename)
*/
static void reg_group_dump(ucli_context_t *uc,
                           uint32_t base_addr,
                           uint32_t addr_range_lo,
                           uint32_t addr_range_hi,
                           char *addr_name) {
  bf_dev_id_t dev_id = strtol(uc->pargs->args[0], NULL, 0);
  bf_dev_port_t dev_port = strtol(uc->pargs->args[1], NULL, 0);

  port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return;
  }

  aim_printf(&uc->pvs, "\n\n#---------------------------------------------");
  aim_printf(&uc->pvs,
             "\n#%s (R%04X to R%04X)",
             addr_name,
             base_addr + addr_range_lo,
             base_addr + addr_range_hi);
  aim_printf(&uc->pvs, "\n#Addr Value");
  aim_printf(&uc->pvs,
             "\n#--------------------------------------------------------------"
             "------\n");
  aim_printf(
      &uc->pvs,
      " 20b  | 16b  | ln0  | ln1  | ln2  | ln3  | ln4  | ln5  | ln6  | ln7  |");
  aim_printf(&uc->pvs,
             "\n#--------------------------------------------------------------"
             "------\n");

  for (uint32_t r = addr_range_lo; r <= addr_range_hi; r++) {
    uint32_t ofs, val;
    uint32_t base_20b = (base_addr & 0x7FF) | ((base_addr & 0xF800) << 5);

    for (int ln = 0; ln < 8; ln++) {
      if (((base_20b >> 16) & 0xF) == 0) {
        ofs = base_20b + r + (ln << 16);
      } else {
        ofs = base_20b + r;  // no lane for grp8/9/A
      }
      if (ln == 0) {
        aim_printf(&uc->pvs, "%04X | %04X | ", ofs, base_addr + r);
      }
      val = port_mgr_tof2_serdes_tile_rd(dev_id, dev_port, ofs);
      aim_printf(&uc->pvs, "%04X | ", val);
    }
    aim_printf(&uc->pvs, "\n");
  }
}

static ucli_status_t port_mgr_ucli_ucli__tile_reg_dump__(ucli_context_t *uc) {
  UCLI_COMMAND_INFO(uc,
                    "tile-reg-dump",
                    2,
                    "Serdes Tile dump of all registers in a group "
                    "<dev_id> <dev_port>");

  reg_group_dump(uc, 0x0000, 0x000, 0x1FF, "Per Lane Register");
  reg_group_dump(uc, 0x0000, 0x0C0, 0x0FF, "ANA_Reg");
  reg_group_dump(uc, 0x0500, 0x000, 0x00B, "Aneg_lt Registers");
  reg_group_dump(uc, 0x01C0, 0x000, 0x00D, "FEC Analyzer Registers");
  reg_group_dump(uc, 0x0200, 0x000, 0x00C, "LANE_SLICE");
  reg_group_dump(uc, 0x0400, 0x000, 0x06C, "Training Registers");
  reg_group_dump(uc, 0x0300, 0x000, 0x0FC, "IEEE AN Registers");
  reg_group_dump(uc, 0x0600, 0x000, 0x047, "AN CSTM Registers");
  reg_group_dump(uc, 0x4000, 0x000, 0x0F0, "Group8 Top Registers");
  reg_group_dump(uc, 0x4B00, 0x03F, 0x0FF, "TSensor, VSensor Registers");
  return 0;
}

bf_status_t port_mgr_tof2_serdes_temperature_start_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       bool auto_);
bf_status_t port_mgr_tof2_serdes_temperature_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 bool auto_,
                                                 float *temp);
bf_status_t port_mgr_tof2_serdes_fw_temperature_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    float *temp);

static ucli_status_t port_mgr_ucli_ucli__tile_temp__(ucli_context_t *uc) {
  bf_dev_id_t dev_id = 0;
  bf_dev_port_t dev_port;
  uint32_t num_pipes = 0;
  uint32_t logical_pipe;
  int n, n_samples;
  uint32_t rc;
  float temp;
  float(*sample)[16];
  ucli_status_t ret = UCLI_STATUS_OK;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "tile-temp",
                    -1,
                    "Serdes Tile Temperature Read (from HW) "
                    "<n_samples> <dev_id>");

  if (uc->pargs->count == 1 || uc->pargs->count == 2) {
    n_samples = strtol(uc->pargs->args[0], NULL, 0);
    if (n_samples > 16) {
      aim_printf(&uc->pvs, "Limiting number of samples to 16 (max)");
      n_samples = 16;
    }

    if (uc->pargs->count == 2) {
      dev_id = strtol(uc->pargs->args[1], NULL, 0);
    }
  } else {
    n_samples = 1;
  }

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  sample = bf_sys_calloc(num_pipes, 16 * sizeof(float));
  if (sample == NULL) {
    aim_printf(&uc->pvs, "Memory Allocation failure");
    return UCLI_STATUS_OK;
  }

  for (n = 0; n < n_samples; n++) {
    for (logical_pipe = 0; logical_pipe < num_pipes; logical_pipe++) {
      dev_port = MAKE_DEV_PORT(logical_pipe, 8);

      // read temp (auto)
      port_mgr_tof2_serdes_temperature_start_set(dev_id, dev_port, true);
    }

    for (logical_pipe = 0; logical_pipe < num_pipes; logical_pipe++) {
      dev_port = MAKE_DEV_PORT(logical_pipe, 8);
      rc = port_mgr_tof2_serdes_temperature_get(dev_id, dev_port, true, &temp);
      if (rc == BF_SUCCESS) {
        sample[logical_pipe][n] = temp;
      } else {
        aim_printf(
            &uc->pvs, "D_P %d: Temperature (auto) : <timeout>\n", dev_port);
        sample[logical_pipe][n] = 0.0;
      }
    }
  }
  aim_printf(&uc->pvs, "Temperature(auto) :\n");
  bool rot = lld_efuse_get_die_rotated(dev_id);
  for (logical_pipe = 0; logical_pipe < num_pipes; logical_pipe++) {
    bf_dev_pipe_t physical_pipe = 0;
    rc = lld_sku_map_pipe_id_to_phy_pipe_id(
        dev_id, logical_pipe, &physical_pipe);
    if (rc != LLD_OK) {
      ret = UCLI_STATUS_E_ARG;
      goto cleanup;
    }

    aim_printf(&uc->pvs,
               "Logical Pipe : %d : Physical Pipe : %d Tile %d : ",
               logical_pipe,
               physical_pipe,
               rot ? (physical_pipe + 2) & 3 : physical_pipe);
    for (n = 0; n < n_samples; n++) {
      aim_printf(&uc->pvs, "%2.1f C :", sample[logical_pipe][n]);
    }
    aim_printf(&uc->pvs, "\n");
  }

cleanup:
  bf_sys_free(sample);
  return ret;
}

static ucli_status_t port_mgr_ucli_ucli__tile_fw_temp__(ucli_context_t *uc) {
  bf_dev_id_t dev_id = 0;
  bf_dev_port_t dev_port;
  uint32_t num_pipes = 0;
  uint32_t logical_pipe;
  int n, n_samples;
  uint32_t rc;
  float temp;
  float(*sample)[16];
  ucli_status_t ret = UCLI_STATUS_OK;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc,
                    "tile-fw-temp",
                    -1,
                    "Serdes Tile Temperature Read (from FW) "
                    "<n_samples> <dev_id>");

  if (uc->pargs->count == 1 || uc->pargs->count == 2) {
    n_samples = strtol(uc->pargs->args[0], NULL, 0);
    if (n_samples > 16) {
      aim_printf(&uc->pvs, "Limiting number of samples to 16 (max)");
      n_samples = 16;
    }

    if (uc->pargs->count == 2) {
      dev_id = strtol(uc->pargs->args[1], NULL, 0);
    }
  } else {
    n_samples = 1;
  }

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  sample = bf_sys_calloc(num_pipes, 16 * sizeof(float));
  if (sample == NULL) {
    aim_printf(&uc->pvs, "Memory Allocation failure");
    return UCLI_STATUS_OK;
  }

  for (n = 0; n < n_samples; n++) {
    for (logical_pipe = 0; logical_pipe < num_pipes; logical_pipe++) {
      dev_port = MAKE_DEV_PORT(logical_pipe, 8);
      rc = port_mgr_tof2_serdes_fw_temperature_get(dev_id, dev_port, &temp);
      if (rc == BF_SUCCESS) {
        sample[logical_pipe][n] = temp;
      } else {
        aim_printf(
            &uc->pvs, "D_P %d: Temperature (fw) : <timeout>\n", dev_port);
        sample[logical_pipe][n] = 0.0;
      }
    }
  }
  aim_printf(&uc->pvs, "Temperature(fw) :\n");
  bool rot = lld_efuse_get_die_rotated(dev_id);
  for (logical_pipe = 0; logical_pipe < num_pipes; logical_pipe++) {
    bf_dev_pipe_t physical_pipe = 0;
    rc = lld_sku_map_pipe_id_to_phy_pipe_id(
        dev_id, logical_pipe, &physical_pipe);
    if (rc != LLD_OK) {
      ret = UCLI_STATUS_E_ARG;
      goto cleanup;
    }

    aim_printf(&uc->pvs,
               "Logical Pipe : %d : Physical Pipe : %d Tile %d : ",
               logical_pipe,
               physical_pipe,
               rot ? (physical_pipe + 2) & 3 : physical_pipe);
    for (n = 0; n < n_samples; n++) {
      aim_printf(&uc->pvs, "%2.1f C :", sample[logical_pipe][n]);
    }
    aim_printf(&uc->pvs, "\n");
  }

cleanup:
  bf_sys_free(sample);
  return ret;
}

extern bf_status_t bf_tof2_serdes_tile_efuse_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 int bank,
                                                 uint32_t *efuse_val);

#include <port_mgr/bf_tof2_serdes_if.h>
extern void port_mgr_tof2_gpio_tile_init(bf_dev_id_t dev_id, uint32_t clkdiv);
extern bf_status_t bf_tof2_serdes_part_type_get(
    bf_dev_id_t dev_id, bf_serdes_process_corner_t *part_type);

int port_mgr_log_tile_efuse(void *optional_uc, bf_dev_id_t dev_id) {
  bf_dev_port_t dev_port;
  uint32_t num_pipes = 0;
  uint32_t logical_pipe;
  int bank_to_read[4] = {0x00, 0x20, 0x40, 0x60};
  uint32_t bank_data[4];
  bf_serdes_process_corner_t part_type;
  char *part_type_str = "";
  bf_status_t rc;
  uint32_t dro[4] = {0};
  int32_t max_dro;
  uint32_t num_dro_values;
  ucli_context_t *uc = (ucli_context_t *)optional_uc;

  lld_sku_get_num_active_pipes(dev_id, &num_pipes);

  bf_tof2_serdes_tile_dro_get(dev_id, dro, &max_dro, &num_dro_values);

  rc = bf_tof2_serdes_part_type_get(dev_id, &part_type);
  if (rc != BF_SUCCESS) {
    part_type = PROCESS_CORNER_UNDEF;
  }
  if (part_type == PROCESS_CORNER_SS) {
    part_type_str = "SS (slow)";
  } else if (part_type == PROCESS_CORNER_TT) {
    part_type_str = "TT (typical)";
  } else if (part_type == PROCESS_CORNER_FF) {
    part_type_str = "FF (fast)";
  } else {
    part_type_str = "undefined";
  }
  if (uc) {
    aim_printf(&uc->pvs, "Part type: %s\n", part_type_str);
  }
  for (logical_pipe = 0; logical_pipe < num_pipes; logical_pipe++) {
    uint64_t wafer_info;
    char wafer_str[6];
    uint32_t data, wafer_id, x_coord, y_coord, fab_code, pkg_code;

    dev_port = MAKE_DEV_PORT(logical_pipe, 8);
    for (int info = 0; info < 4; info++) {
      bf_status_t sts;

      sts = bf_tof2_serdes_tile_efuse_get(
          dev_id, dev_port, bank_to_read[info], &data);
      if (sts == BF_SUCCESS) {
        bank_data[info] = data;
      } else {
        bank_data[info] = 0;
      }
    }
    if (uc) {
      aim_printf(&uc->pvs,
                 "       Bank :     3    :     2    :     1    :     0\n");
      aim_printf(&uc->pvs,
                 "Tile %d      : %08x : %08x : %08x : %08x\n",
                 logical_pipe,
                 bank_data[3],
                 bank_data[2],
                 bank_data[1],
                 bank_data[0]);
    }
    wafer_info = (((uint64_t)bank_data[1]) << 32ul) | (uint64_t)bank_data[2];
    wafer_str[5] = (char)((wafer_info >> (64 - (1 * 6))) & 0x3f) + 32;
    wafer_str[4] = (char)((wafer_info >> (64 - (2 * 6))) & 0x3f) + 32;
    wafer_str[3] = (char)((wafer_info >> (64 - (3 * 6))) & 0x3f) + 32;
    wafer_str[2] = (char)((wafer_info >> (64 - (4 * 6))) & 0x3f) + 32;
    wafer_str[1] = (char)((wafer_info >> (64 - (5 * 6))) & 0x3f) + 32;
    wafer_str[0] = (char)((wafer_info >> (64 - (6 * 6))) & 0x3f) + 32;

    wafer_str[5] = isprint((int)wafer_str[5]) ? wafer_str[5] : '_';
    wafer_str[4] = isprint((int)wafer_str[4]) ? wafer_str[4] : '_';
    wafer_str[3] = isprint((int)wafer_str[3]) ? wafer_str[3] : '_';
    wafer_str[2] = isprint((int)wafer_str[2]) ? wafer_str[2] : '_';
    wafer_str[1] = isprint((int)wafer_str[1]) ? wafer_str[1] : '_';
    wafer_str[0] = isprint((int)wafer_str[0]) ? wafer_str[0] : '_';

    wafer_id = (uint32_t)((wafer_info >> 23) & 0x1f);
    x_coord = (uint32_t)((wafer_info >> 15) & 0xff);
    y_coord = (uint32_t)((wafer_info >> 7) & 0xff);
    fab_code = (uint32_t)((wafer_info >> 5) & 0x03);  // 1-TSMC, 2-UMC
    pkg_code = (uint32_t)((wafer_info >> 4) & 0x01);  // 0-pkg, 1-csp

    if (uc) {
      aim_printf(&uc->pvs,
                 "Silicon rev : %d\n"
                 "Wafer string: %c%c%c%c%c%c-%d\n"
                 "X Coord.    : %d\n"
                 "Y Coord.    : %d\n"
                 "Fab code    : %s\n"
                 "Pkg code    : %s\n"
                 "DRO         : %d\n",
                 bank_data[0] & 0x0F,
                 isprint((int)wafer_str[5]) ? (int)wafer_str[5] : 20,
                 isprint((int)wafer_str[4]) ? (int)wafer_str[4] : 20,
                 isprint((int)wafer_str[3]) ? (int)wafer_str[3] : 20,
                 isprint((int)wafer_str[2]) ? (int)wafer_str[2] : 20,
                 isprint((int)wafer_str[1]) ? (int)wafer_str[1] : 20,
                 isprint((int)wafer_str[0]) ? (int)wafer_str[0] : 20,
                 wafer_id,
                 x_coord,
                 y_coord,
                 fab_code == 1 ? "TSMC" : fab_code == 2 ? "UMC" : "???",
                 pkg_code == 0 ? "PKG" : "CSP",
                 dro[logical_pipe]);
    } else {
      port_mgr_log(
          "\n       Bank :     3    :     2    :     1    :     0\n"
          "Tile %d      : %08x : %08x : %08x : %08x\n"
          "\nSilicon rev : %d\n"
          "Wafer string: %c%c%c%c%c%c-%d\n"
          "X Coord.    : %d\n"
          "Y Coord.    : %d\n"
          "Fab code    : %s\n"
          "Pkg code    : %s\n"
          "DRO         : %d\n",
          logical_pipe,
          bank_data[3],
          bank_data[2],
          bank_data[1],
          bank_data[0],
          bank_data[0] & 0x0F,
          isprint((int)wafer_str[5]) ? (int)wafer_str[5] : 20,
          isprint((int)wafer_str[4]) ? (int)wafer_str[4] : 20,
          isprint((int)wafer_str[3]) ? (int)wafer_str[3] : 20,
          isprint((int)wafer_str[2]) ? (int)wafer_str[2] : 20,
          isprint((int)wafer_str[1]) ? (int)wafer_str[1] : 20,
          isprint((int)wafer_str[0]) ? (int)wafer_str[0] : 20,
          wafer_id,
          x_coord,
          y_coord,
          fab_code == 1 ? "TSMC" : fab_code == 2 ? "UMC" : "???",
          pkg_code == 0 ? "PKG" : "CSP",
          dro[logical_pipe]);
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__tile_efuse__(ucli_context_t *uc) {
  bf_dev_id_t dev_id = 0;
  port_mgr_dev_t *dev_p = NULL;

  UCLI_COMMAND_INFO(uc, "tile-efuse", 1, "Serdes Tile Efuse Read <dev_id>");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (!dev_p) {
    aim_printf(&uc->pvs, "Invalid dev_id\n");
    return 0;
  }

  port_mgr_log_tile_efuse(uc, dev_id);
  return 0;
}

// TOF3 serdes commands

//#include "../port_mgr_tof3/bf_tof3_serdes_ucli_if.h"
#include "../port_mgr_tof3/aw_mss.h"
#include "../port_mgr_tof3/bf_tof3_serdes_utils.h"
#include <port_mgr/bf_tof3_serdes_if.h>

extern bf_status_t bf_tof3_serdes_mss_reset(uint32_t dev_id, uint32_t dev_port);
extern bf_status_t bf_tof3_serdes_fw_load(uint32_t dev_id,
                                          uint32_t dev_port,
                                          char *fw_path);
extern int bf_tof3_serdes_read_status(uint32_t dev_id,
                                      uint32_t dev_port,
                                      uint32_t ln,
                                      uint32_t br);
bf_status_t bf_tof3_serdes_csr_def_get(uint32_t dev_id,
                                       uint32_t dev_port,
                                       uint32_t ln,
                                       uint32_t csr_addr,
                                       char **name,
                                       uint32_t *num_flds,
                                       uint32_t *fld_num_base,
                                       char **comment);
bf_status_t bf_tof3_serdes_vreg_def_get(uint32_t dev_id,
                                        uint32_t dev_port,
                                        uint32_t ln,
                                        uint32_t csr_addr,
                                        char **name,
                                        uint32_t *num_flds,
                                        uint32_t *fld_num_base,
                                        char **comment);
bf_status_t bf_tof3_serdes_csr_def_get_next(uint32_t dev_id,
                                            uint32_t dev_port,
                                            uint32_t ln,
                                            uint32_t csr_addr,
                                            uint32_t *next_csr_addr,
                                            char **name,
                                            uint32_t *num_flds,
                                            uint32_t *fld_num_base,
                                            char **comment);

bf_status_t bf_tof3_serdes_csr_fld_def_get(uint32_t dev_id,
                                           uint32_t dev_port,
                                           uint32_t ln,
                                           uint32_t csr_addr,
                                           uint32_t fld_num,
                                           char **name,
                                           uint32_t *lo_bit,
                                           uint32_t *width,
                                           uint32_t *mask,
                                           uint32_t *access,
                                           uint32_t *reset_value,
                                           char **comment);

int dev_port_validate(bf_dev_port_t dev_port) {
  if (DEV_PORT_VALIDATE(dev_port)) {
    return true;
  } else if ((dev_port >= 1000) && (dev_port < 1008)) {
    return true;
  } else {
    return false;
  }
}
static ucli_status_t port_mgr_ucli_ucli__mss_dump__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, br;

  UCLI_COMMAND_INFO(
      uc, "mss-dump", 4, "devid, d_p ln branch: TF3 Serdes debug cmd");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  br = strtoul(uc->pargs->args[3], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  bf_tof3_serdes_read_status(dev_id, dev_port, ln, br);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_dump2__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, br;

  UCLI_COMMAND_INFO(
      uc, "mss-dump2", 4, "devid, d_p ln branch: TF3 Serdes debug cmd");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  br = strtoul(uc->pargs->args[3], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  bf_tof3_serdes_read_status2(dev_id, dev_port, ln, br);
  return 0;
}

static int ucli_mss_rd(ucli_context_t *uc,
                       uint32_t dev_id,
                       uint32_t dev_port,
                       uint32_t ln,
                       uint32_t csr,
                       bool verbose,
                       bool side_specified,
                       uint32_t section) {
  uint32_t val;

#if 1

  bool is_dft_reg = (((csr >> 12) & 3) == 3) ? true : false;
  uint32_t tx_val, rx_val;

  if (is_dft_reg) {
    // MSS_SECTION_RX
    bf_tof3_serdes_csr_rd_specific_side(
        dev_id, dev_port, ln, MSS_SECTION_TX, csr, &tx_val);
    bf_tof3_serdes_csr_rd_specific_side(
        dev_id, dev_port, ln, MSS_SECTION_RX, csr, &rx_val);
  }
#endif
  else if (side_specified) {
    bf_tof3_serdes_csr_rd_specific_side(
        dev_id, dev_port, ln, section, csr, &val);
  } else {
    bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, csr, &val);
  }
  bf_status_t rc;
  char *name, *comment;
  uint32_t num_flds, fld_num_base;
  uint32_t csr_plus_ln_offset;

  rc = bf_tof3_serdes_csr_def_get(
      dev_id, dev_port, ln, csr, &name, &num_flds, &fld_num_base, &comment);
  if (rc == 0) {
    if (csr >= 0x02000000) {
      // add in lane offset
      csr_plus_ln_offset = csr + (ln * 0x02000000);
    } else {
      csr_plus_ln_offset = csr;
    }
    if (verbose) {
      if (is_dft_reg) {
        aim_printf(&uc->pvs,
                   "\n%s%08x : %08x : %08x : %s\n",
                   comment,
                   tx_val,
                   rx_val,
                   csr_plus_ln_offset,
                   name);
      } else {
        aim_printf(&uc->pvs,
                   "\n%s%08x : %08x : %s\n",
                   comment,
                   val,
                   csr_plus_ln_offset,
                   name);
      }
    } else {
      if (is_dft_reg) {
        aim_printf(&uc->pvs, "TX         RX\n");
        aim_printf(&uc->pvs,
                   "%08x : %08x : %08x : %s\n",
                   tx_val,
                   rx_val,
                   csr_plus_ln_offset,
                   name);
      } else {
        aim_printf(
            &uc->pvs, "%08x : %08x : %s\n", val, csr_plus_ln_offset, name);
      }
    }
    for (uint32_t f = 0; f < num_flds; f++) {
      uint32_t lo_bit, width, mask, access, reset_value;

      rc = bf_tof3_serdes_csr_fld_def_get(dev_id,
                                          dev_port,
                                          ln,
                                          csr,
                                          f,
                                          &name,
                                          &lo_bit,
                                          &width,
                                          &mask,
                                          &access,
                                          &reset_value,
                                          &comment);
      if (rc == 0) {
        if (verbose) {
          if (is_dft_reg) {
            aim_printf(
                &uc->pvs,
                "\n%s// -------------------------------------\n%8x : %08x :  "
                "[%2d:%2d] : %s : %s <reset val: %08x>\n",
                comment,
                ((tx_val & mask) >> lo_bit),
                ((rx_val & mask) >> lo_bit),
                (lo_bit + width - 1),
                (lo_bit),
                (access == 0) ? "RW" : "RO",
                name,
                reset_value);
          } else {
            aim_printf(&uc->pvs,
                       "\n%s// -------------------------------------\n%8x :  "
                       "[%2d:%2d] : %s : %s <reset val: %08x>\n",
                       comment,
                       ((val & mask) >> lo_bit),
                       (lo_bit + width - 1),
                       (lo_bit),
                       (access == 0) ? "RW" : "RO",
                       name,
                       reset_value);
          }
        } else {
          if (is_dft_reg) {
            aim_printf(&uc->pvs,
                       "%8x : %8x :  [%2d:%2d] : %s : %s <reset val: %08x>\n",
                       ((tx_val & mask) >> lo_bit),
                       ((rx_val & mask) >> lo_bit),
                       (lo_bit + width - 1),
                       (lo_bit),
                       (access == 0) ? "RW" : "RO",
                       name,
                       reset_value);
          } else {
            aim_printf(&uc->pvs,
                       "%8x :  [%2d:%2d] : %s : %s <reset val: %08x>\n",
                       ((val & mask) >> lo_bit),
                       (lo_bit + width - 1),
                       (lo_bit),
                       (access == 0) ? "RW" : "RO",
                       name,
                       reset_value);
          }
        }
      }
    }
  } else {
    if (csr >= 0x02000000) {
      // add in lane offset
      csr_plus_ln_offset = csr + (ln * 0x02000000);
    } else {
      csr_plus_ln_offset = csr;
    }
    if (is_dft_reg) {
      aim_printf(
          &uc->pvs, "%08x : %08x : %08x\n", tx_val, rx_val, csr_plus_ln_offset);
    } else {
      aim_printf(&uc->pvs, "%08x : %08x\n", val, csr_plus_ln_offset);
    }
  }
  return 0;
}

/*
 * #define DIG_SOC_LANE_OVRD_REG1_ADDR 0x02003014
#define DIG_SOC_LANE_OVRD_REG2_ADDR 0x02003018
#define DIG_SOC_LANE_OVRD_REG3_ADDR 0x0200301C
#define DIG_SOC_LANE_OVRD_REG4_ADDR 0x02003020
#define DIG_SOC_LANE_OVRD_REG5_ADDR 0x02003024
#define DIG_SOC_LANE_OVRD_REG6_ADDR 0x02003028
#define DIG_SOC_LANE_OVRD_REG7_ADDR 0x0200302C
#define DIG_SOC_LANE_STAT_REG1_ADDR 0x02003030
#define DIG_SOC_LANE_STAT_REG2_ADDR 0x02003034
#define DIG_SOC_LANE_STAT_REG3_ADDR 0x02003038
*/

static ucli_status_t port_mgr_ucli_ucli__mss_dfx__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;

  UCLI_COMMAND_INFO(
      uc, "mss-dfx", 3, "devid, d_p, ln : Read TF3 Serdes DFX registers");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003014, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003018, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x0200301C, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003020, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003024, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003028, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x0200302C, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003030, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003034, false, false, 0);
  ucli_mss_rd(uc, dev_id, dev_port, ln, 0x02003038, false, false, 0);
  return 0;
}

uint32_t aw_csr_get_by_name(uint32_t dev_id,
                            uint32_t dev_port,
                            uint32_t ln,
                            char *tgt_name);
extern uint32_t map_macro_to_address(uint32_t macro);

static ucli_status_t port_mgr_ucli_ucli__mss_grp_rd__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln = 0, csr;
  uint32_t tmac, ch;
  UCLI_COMMAND_INFO(uc,
                    "mss-grp-rd",
                    3,
                    "devid, d_p, csr : Read csr from each lane of group of 8");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }

  if ((uc->pargs->args[2][0] >= '0') && (uc->pargs->args[2][0] <= '9')) {
    csr = (uint32_t)strtoul(uc->pargs->args[2], NULL, 16);

  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[2]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }
  dev_port &= ~7;  // make base dev_port
  bf_status_t sts = port_mgr_tof2_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &tmac, &ch, NULL);
  if ((sts != BF_SUCCESS) || (tmac == 0)) {
    aim_printf(&uc->pvs, "invalid dev_port : %d\n", dev_port);
    return 0;
  }
  uint32_t subdev_id;
  uint32_t macro;
  uint32_t phys_tx_ln;
  uint32_t phys_rx_ln;
  uint32_t cmn_ofs;
  uint32_t tx_ofs;
  uint32_t rx_ofs;
  uint32_t sram0;
  uint32_t sram1;
  // get base address of corresponding serdes macro
  bf_tof3_serdes_addr_range(dev_id,
                            dev_port,
                            0,
                            &subdev_id,
                            &macro,
                            &phys_tx_ln,
                            &phys_rx_ln,
                            &cmn_ofs,
                            &tx_ofs,
                            &rx_ofs,
                            &sram0,
                            &sram1);
  if (csr < 0x02000000) {
    aim_printf(&uc->pvs, "Invalid cmd for CMN reg: %d\n", csr);
    return 0;
  }
  uint32_t macro_base = map_macro_to_address(macro);
  uint32_t addr = (csr & 0xFFFF) + macro_base;
  for (ln = 0; ln < 8; ln++) {
    uint32_t rdata;
    lld_subdev_read_register(dev_id, subdev_id, addr + (ln * 0x4000), &rdata);
    aim_printf(&uc->pvs, "ln%d : %08x\n", ln, rdata);
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_rd__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;

  UCLI_COMMAND_INFO(
      uc, "mss-rd", 4, "devid, d_p, ln, csr : (verbose) Read TF3 Serdes csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }
  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, false, -1);
}

static ucli_status_t port_mgr_ucli_ucli__mss_rx_rd__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;

  UCLI_COMMAND_INFO(
      uc,
      "mss-rx-rd",
      4,
      "devid, d_p, ln, csr : (verbose) Read TF3 Serdes Rx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }
  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, true, MSS_SECTION_RX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_tx_rd__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;

  UCLI_COMMAND_INFO(
      uc,
      "mss-tx-rd",
      4,
      "devid, d_p, ln, csr : (verbose) Read TF3 Serdes Tx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }
  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, true, MSS_SECTION_TX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_lkup__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;
  char *tgt_name;

  UCLI_COMMAND_INFO(
      uc, "mss-lkup", 4, "devid, d_p, ln, csr : TF3 Serdes csr by name");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  tgt_name = (char *)uc->pargs->args[3];

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  csr = aw_csr_get_by_name(dev_id, dev_port, ln, tgt_name);
  if (csr != 0xffffffff) {
    aim_printf(&uc->pvs, "%08x : %s\n", csr, tgt_name);
    return 0;
  } else {
    aim_printf(&uc->pvs, "<not found>\n");
    return 1;
  }
  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, false, -1);
}

static ucli_status_t port_mgr_ucli_ucli__mss_wr__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, val;

  UCLI_COMMAND_INFO(
      uc,
      "mss-wr",
      5,
      "devid, d_p, ln, csr, val : (verbose) Write TF3 Serdes csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  val = strtoul(uc->pargs->args[4], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, csr, val);

  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, false, -1);
}

static ucli_status_t port_mgr_ucli_ucli__mss_rx_wr__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, val;

  UCLI_COMMAND_INFO(
      uc,
      "mss-rx-wr",
      5,
      "devid, d_p, ln, csr, val : (verbose) Write TF3 Serdes Rx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  val = strtoul(uc->pargs->args[4], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_wr_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_RX, csr, val);

  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, true, MSS_SECTION_RX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_tx_wr__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, val;

  UCLI_COMMAND_INFO(
      uc,
      "mss-tx-wr",
      5,
      "devid, d_p, ln, csr, val : (verbose) Write TF3 Serdes Tx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  val = strtoul(uc->pargs->args[4], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_wr_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_TX, csr, val);

  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, true, true, MSS_SECTION_TX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_r__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;

  UCLI_COMMAND_INFO(
      uc, "mss-r", 4, "devid, d_p, ln, csr : (brief) Read TF3 Serdes csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, false, false, -1);
}

static ucli_status_t port_mgr_ucli_ucli__mss_rx_r__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;

  UCLI_COMMAND_INFO(
      uc,
      "mss-rx-r",
      4,
      "devid, d_p, ln, csr : (brief) Read TF3 Serdes Rx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  return ucli_mss_rd(
      uc, dev_id, dev_port, ln, csr, false, true, MSS_SECTION_RX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_tx_r__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr;

  UCLI_COMMAND_INFO(
      uc,
      "mss-tx-r",
      4,
      "devid, d_p, ln, csr : (brief) Read TF3 Serdes Tx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  return ucli_mss_rd(
      uc, dev_id, dev_port, ln, csr, false, true, MSS_SECTION_TX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_w__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, val;

  UCLI_COMMAND_INFO(uc,
                    "mss-w",
                    5,
                    "devid, d_p, ln, csr, val : (brief) Write TF3 Serdes csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  val = strtoul(uc->pargs->args[4], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, csr, val);

  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, false, false, -1);
}

static ucli_status_t port_mgr_ucli_ucli__mss_rx_w__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, val;

  UCLI_COMMAND_INFO(
      uc,
      "mss-rx-w",
      5,
      "devid, d_p, ln, csr, val : (brief) Write TF3 Serdes Rx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  val = strtoul(uc->pargs->args[4], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_wr_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_RX, csr, val);

  return ucli_mss_rd(
      uc, dev_id, dev_port, ln, csr, false, true, MSS_SECTION_RX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_tx_w__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, val;

  UCLI_COMMAND_INFO(
      uc,
      "mss-tx-w",
      5,
      "devid, d_p, ln, csr, val : (brief) Write TF3 Serdes Tx-side csr");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  val = strtoul(uc->pargs->args[4], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_wr_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_TX, csr, val);

  return ucli_mss_rd(
      uc, dev_id, dev_port, ln, csr, false, true, MSS_SECTION_TX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_w_fld__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, hi_bit, lo_bit, fld_val, old_val, new_val;

  UCLI_COMMAND_INFO(
      uc,
      "mss-w-fld",
      7,
      "devid, d_p, ln, csr, hi_bit lo_bit val : Write TF3 Serdes csr field");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  hi_bit = strtoul(uc->pargs->args[4], NULL, 10);
  lo_bit = strtoul(uc->pargs->args[5], NULL, 10);
  fld_val = strtoul(uc->pargs->args[6], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, csr, &old_val);
  uint32_t mask;
  uint64_t long_mask;
  long_mask = ((((1ul << (hi_bit + 1)) - 1) >> lo_bit) << lo_bit);
  mask = (uint32_t)long_mask;
  new_val = old_val & ~mask;
  new_val = new_val | ((fld_val << lo_bit) & mask);
  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, csr, new_val);

  return ucli_mss_rd(uc, dev_id, dev_port, ln, csr, false, false, -1);
}

static ucli_status_t port_mgr_ucli_ucli__mss_rx_w_fld__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, hi_bit, lo_bit, fld_val, old_val, new_val;

  UCLI_COMMAND_INFO(uc,
                    "mss-rx-w-fld",
                    7,
                    "devid, d_p, ln, csr, hi_bit lo_bit val : Write TF3 Serdes "
                    "Rx-side csr field");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  hi_bit = strtoul(uc->pargs->args[4], NULL, 10);
  lo_bit = strtoul(uc->pargs->args[5], NULL, 10);
  fld_val = strtoul(uc->pargs->args[6], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_rd_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_RX, csr, &old_val);
  uint32_t mask;
  uint64_t long_mask;
  long_mask = ((((1ul << (hi_bit + 1)) - 1) >> lo_bit) << lo_bit);
  mask = (uint32_t)long_mask;
  new_val = old_val & ~mask;
  new_val = new_val | ((fld_val << lo_bit) & mask);
  bf_tof3_serdes_csr_wr_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_RX, csr, new_val);

  return ucli_mss_rd(
      uc, dev_id, dev_port, ln, csr, false, true, MSS_SECTION_RX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_tx_w_fld__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, hi_bit, lo_bit, fld_val, old_val, new_val;

  UCLI_COMMAND_INFO(uc,
                    "mss-tx-w-fld",
                    7,
                    "devid, d_p, ln, csr, hi_bit lo_bit val : Write TF3 Serdes "
                    "Tx-side csr field");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  hi_bit = strtoul(uc->pargs->args[4], NULL, 10);
  lo_bit = strtoul(uc->pargs->args[5], NULL, 10);
  fld_val = strtoul(uc->pargs->args[6], NULL, 16);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  if ((uc->pargs->args[3][0] >= '0') && (uc->pargs->args[3][0] <= '9')) {
    csr = strtoul(uc->pargs->args[3], NULL, 16);
  } else {
    csr = aw_csr_get_by_name(dev_id, dev_port, ln, (char *)uc->pargs->args[3]);
    if (csr == 0xffffffff) {
      aim_printf(&uc->pvs, "csr: %s not found\n", uc->pargs->args[3]);
      return 1;
    }
  }

  bf_tof3_serdes_csr_rd_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_TX, csr, &old_val);
  uint32_t mask;
  uint64_t long_mask;
  long_mask = ((((1ul << (hi_bit + 1)) - 1) >> lo_bit) << lo_bit);
  mask = (uint32_t)long_mask;
  new_val = old_val & ~mask;
  new_val = new_val | ((fld_val << lo_bit) & mask);
  bf_tof3_serdes_csr_wr_specific_side(
      dev_id, dev_port, ln, MSS_SECTION_TX, csr, new_val);

  return ucli_mss_rd(
      uc, dev_id, dev_port, ln, csr, false, true, MSS_SECTION_TX);
}

static ucli_status_t port_mgr_ucli_ucli__mss_reset__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port;

  UCLI_COMMAND_INFO(uc, "mss-reset", 2, "devid, d_p : Reset MSS");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);

  bf_tof3_serdes_mss_reset(dev_id, dev_port);
  aim_printf(&uc->pvs, "Reset complete\n");

  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__fw_load__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port;
  bf_status_t rc;
  char *fw_path = "./install/share/tofino_sds_fw/alphawave/firmware/eth.hex";

  UCLI_COMMAND_INFO(uc, "mss-fw-load", 2, "devid, d_p : Load MSS firmware");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);

  rc = bf_tof3_serdes_fw_load(dev_id, dev_port, fw_path);
  aim_printf(&uc->pvs, "Done (%d)\n", rc);

  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_modified__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, next_csr, val;
  int rc;

  UCLI_COMMAND_INFO(uc,
                    "mss-modified",
                    3,
                    "devid, d_p, ln : Dump all modified TF3 Serdes csr fields");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (ln > 7) {
    aim_printf(&uc->pvs, "ln (%d) must be 0-7\n", ln);
    return 0;
  }

  csr = 0;
  next_csr = 0;  // will be used in loop below
  rc = 0;
  while (rc == 0) {
    uint32_t num_flds, fld_num_base;
    char *reg_name, *fld_name, *comment;

    csr = next_csr;
    rc = bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, csr, &val);
    if (rc != 0) break;

    rc = bf_tof3_serdes_csr_def_get(dev_id,
                                    dev_port,
                                    ln,
                                    csr,
                                    &reg_name,
                                    &num_flds,
                                    &fld_num_base,
                                    &comment);
    if (rc == 0) {
      for (uint32_t f = 0; f < num_flds; f++) {
        uint32_t lo_bit, width, mask, access, reset_value;

        rc = bf_tof3_serdes_csr_fld_def_get(dev_id,
                                            dev_port,
                                            ln,
                                            csr,
                                            f,
                                            &fld_name,
                                            &lo_bit,
                                            &width,
                                            &mask,
                                            &access,
                                            &reset_value,
                                            &comment);
        // see if field is different from its reset_value
        uint32_t fld_val = (val & mask) >> lo_bit;
        if (fld_val != reset_value) {
          aim_printf(&uc->pvs,
                     "%08x : %08x : %08x : %s : %s : %s\n",
                     fld_val,
                     reset_value,
                     csr,
                     (access == 0) ? "RW" : "RO",
                     reg_name,
                     fld_name);
        }
      }

      if (rc == 0) {
        rc = bf_tof3_serdes_csr_def_get_next(dev_id,
                                             dev_port,
                                             ln,
                                             csr,
                                             &next_csr,
                                             &reg_name,
                                             &num_flds,
                                             &fld_num_base,
                                             &comment);
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_regdump__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, next_csr, val;
  int rc;

  UCLI_COMMAND_INFO(
      uc, "mss-regdump", 3, "dev_id, d_p, ln : Dump all TF3 Serdes csr fields");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (ln > 7) {
    aim_printf(&uc->pvs, "ln (%d) must be 0-7\n", ln);
    return 0;
  }

  csr = 0;
  next_csr = 0;  // will be used in loop below
  rc = 0;
  while (rc == 0) {
    uint32_t num_flds, fld_num_base;
    char *reg_name, *fld_name, *comment;

    csr = next_csr;
    rc = bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, csr, &val);
    if (rc != 0) break;

    rc = bf_tof3_serdes_csr_def_get(dev_id,
                                    dev_port,
                                    ln,
                                    csr,
                                    &reg_name,
                                    &num_flds,
                                    &fld_num_base,
                                    &comment);
    if (rc == 0) {
      for (uint32_t f = 0; f < num_flds; f++) {
        uint32_t lo_bit, width, mask, access, reset_value;

        rc = bf_tof3_serdes_csr_fld_def_get(dev_id,
                                            dev_port,
                                            ln,
                                            csr,
                                            f,
                                            &fld_name,
                                            &lo_bit,
                                            &width,
                                            &mask,
                                            &access,
                                            &reset_value,
                                            &comment);
        // see if field is different from its reset_value
        uint32_t fld_val = (val & mask) >> lo_bit;
        aim_printf(&uc->pvs,
                   "%08x : %08x : %s : %s : %s\n",
                   fld_val,
                   csr,
                   (access == 0) ? "RW" : "RO",
                   reg_name,
                   fld_name);
      }

      if (rc == 0) {
        rc = bf_tof3_serdes_csr_def_get_next(dev_id,
                                             dev_port,
                                             ln,
                                             csr,
                                             &next_csr,
                                             &reg_name,
                                             &num_flds,
                                             &fld_num_base,
                                             &comment);
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_vregs__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, csr, next_csr, val;
  int rc;

  UCLI_COMMAND_INFO(
      uc, "mss-vregs", 3, "devid, d_p, ln : Dump all virtual csr fields");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (ln > 7) {
    aim_printf(&uc->pvs, "ln (%d) must be 0-7\n", ln);
    return 0;
  }
  csr = 0;
  next_csr = 0;  // will be used in loop below
  rc = 0;
  while (rc == 0) {
    uint32_t num_flds, fld_num_base;
    char *reg_name, *fld_name, *comment;

    csr = next_csr;

    rc = bf_tof3_serdes_vreg_def_get(dev_id,
                                     dev_port,
                                     ln,
                                     csr,
                                     &reg_name,
                                     &num_flds,
                                     &fld_num_base,
                                     &comment);
    if (rc == 0) {
      rc = bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, csr, &val);
      if (rc != 0) break;

      for (uint32_t f = 0; f < num_flds; f++) {
        uint32_t lo_bit, width, mask, access, reset_value;

        rc = bf_tof3_serdes_csr_fld_def_get(dev_id,
                                            dev_port,
                                            ln,
                                            csr,
                                            f,
                                            &fld_name,
                                            &lo_bit,
                                            &width,
                                            &mask,
                                            &access,
                                            &reset_value,
                                            &comment);
        // see if field is different from its reset_value
        uint32_t fld_val = (val & mask) >> lo_bit;
        aim_printf(&uc->pvs,
                   "%08x : %08x : %08x : %s : %s : %s\n",
                   fld_val,
                   reset_value,
                   csr,
                   (access == 0) ? "RW" : "RO",
                   reg_name,
                   fld_name);
      }

      if (rc == 0) {
      }
    }
    rc = bf_tof3_serdes_csr_def_get_next(dev_id,
                                         dev_port,
                                         ln,
                                         csr,
                                         &next_csr,
                                         &reg_name,
                                         &num_flds,
                                         &fld_num_base,
                                         &comment);
  }
  return 0;
}

uint32_t aw_csr_get_by_name(uint32_t dev_id,
                            uint32_t dev_port,
                            uint32_t ln,
                            char *tgt_name) {
  uint32_t csr, next_csr;
  int rc;

  csr = 0;
  next_csr = 0;  // will be used in loop below
  rc = 0;
  while (rc == 0) {
    uint32_t num_flds, fld_num_base;
    char *reg_name, *comment;

    csr = next_csr;
    rc = bf_tof3_serdes_csr_def_get(dev_id,
                                    dev_port,
                                    ln,
                                    csr,
                                    &reg_name,
                                    &num_flds,
                                    &fld_num_base,
                                    &comment);
    char with_ADDR[128] = {0};
    strncat(with_ADDR, tgt_name, 128 - 1);
    strncat(with_ADDR, "_ADDR", 128 - strlen(with_ADDR) - 1);
    if (rc == 0) {
      if (((strlen(tgt_name) == strlen(reg_name)) &&
           (strcmp(tgt_name, reg_name) == 0)) ||
          ((strlen(with_ADDR) == strlen(reg_name)) &&
           (strcmp(with_ADDR, reg_name) == 0))) {
        return csr;
      }
    }
    rc = bf_tof3_serdes_csr_def_get_next(dev_id,
                                         dev_port,
                                         ln,
                                         csr,
                                         &next_csr,
                                         &reg_name,
                                         &num_flds,
                                         &fld_num_base,
                                         &comment);
  }
  return 0xffffffff;
}

static ucli_status_t port_mgr_ucli_ucli__top_init__(ucli_context_t *uc) {
  uint32_t dev_id = 0, run_post = 0;
  bf_status_t rc;

  UCLI_COMMAND_INFO(
      uc, "top-init", 2, "dev_id run_post (Top level serdes init)");
  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  run_post = strtoul(uc->pargs->args[1], NULL, 10);

  rc = bf_tof3_serdes_top_init(dev_id, run_post);
  aim_printf(&uc->pvs, "Done (%d)\n", rc);

  return 0;
}

extern bf_status_t bf_tof3_serdes_txfir_config_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t cm3,
                                                   uint32_t cm2,
                                                   uint32_t cm1,
                                                   uint32_t c0,
                                                   uint32_t c1);
bf_status_t bf_tof3_serdes_txfir_config_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *cm3,
                                            uint32_t *cm2,
                                            uint32_t *cm1,
                                            uint32_t *c0,
                                            uint32_t *c1);
bf_status_t bf_tof3_serdes_loopback_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        uint32_t en);

static ucli_status_t port_mgr_ucli_ucli__txfir_set__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;
  uint32_t cm3, cm2, cm1, c0, c1;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc, "mss-fir-set", 8, "tof3 Tx FIR tap set");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  cm3 = strtoul(uc->pargs->args[3], NULL, 10);
  cm2 = strtoul(uc->pargs->args[4], NULL, 10);
  cm1 = strtoul(uc->pargs->args[5], NULL, 10);
  c0 = strtoul(uc->pargs->args[6], NULL, 10);
  c1 = strtoul(uc->pargs->args[7], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  rc = bf_tof3_serdes_txfir_config_set(
      dev_id, dev_port, ln, cm3, cm2, cm1, c0, c1);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__txfir_get__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;
  uint32_t cm3, cm2, cm1, c0, c1;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc, "mss-fir-get", 3, "tof3 Tx FIR tap get");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  rc = bf_tof3_serdes_txfir_config_get(
      dev_id, dev_port, ln, &cm3, &cm2, &cm1, &c0, &c1);
  if (rc == 0) {
    aim_printf(&uc->pvs,
               "cm3= %d, cm2= %d, cm1=%d, c0= %d, c1= %d\n",
               cm3,
               cm2,
               cm1,
               c0,
               c1);
  } else {
    aim_printf(&uc->pvs, "error: %d\n", rc);
  }
  return rc;
}

uint32_t bf_tof3_serdes_str_to_loopback_mode(char *str);
char *bf_tof3_serdes_loopback_mode_to_str(uint32_t loopback_mode);

static ucli_status_t port_mgr_ucli_ucli__mss_loopback_set__(
    ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;
  uint32_t mode;
  bf_status_t rc;

  UCLI_COMMAND_INFO(
      uc, "mss-lpbk-set", 4, "Configure loopback mode: 0=none, 1=NES, 2=FEP");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  //  mode = strtoul(uc->pargs->args[3], NULL, 10);
  mode = bf_tof3_serdes_str_to_loopback_mode((char *)uc->pargs->args[3]);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  rc = bf_tof3_serdes_loopback_set(dev_id, dev_port, ln, mode);
  aim_printf(
      &uc->pvs, "loopback %s\n", bf_tof3_serdes_loopback_mode_to_str(mode));

  return rc;
}

extern char *bf_tof3_serdes_loopback_mode_to_str(uint32_t loopback_mode);

static void mss_sts_banner(ucli_context_t *uc) {
  aim_printf(
      &uc->pvs,
      "------+---+---------------------------+-----+-----+-----+-----+-----"
      "--------------+-----------+---------+-----+-----+-----+---+---+--"
      "-+---+---+---+---+----+----+----+---\n");
  aim_printf(
      &uc->pvs,
      "      |   |    phys                   |reset|rate |width|ps   |Tx "
      "FIR             |Tx         |Precod   |gc   |Pol  "
      "|Disbl|Sig|Cdr|Ctle   |Vga        |Tx  |Rx  |BIST|   \n");
  aim_printf(&uc->pvs,
             "mac|ch|d_p|ln|tx|rx|POST|dest|  term. "
             "|tx|rx|tx|rx|tx|rx|tx|rx|cm3|cm2|cm1|c0 |c1 |Pattern "
             "|En|ovr|tx|rx|tx|rx|tx|rx|tx|rx|Det|Lck|Rte|Bst|Crs|Fin|Ofs|PPM "
             "|PPM |Lck |BER\n");
  aim_printf(
      &uc->pvs,
      "---+--+---+--+--+--+----+----+--------+--+--+--+--+--+--+--+--+---+-"
      "--+---+---+---+--------+--+---+--+--+--+--+--+--+--+--+---+---+--"
      "-+---+---+---+---+----+----+----+---\n");
}

static ucli_status_t port_mgr_ucli_ucli__mss_sts__(ucli_context_t *uc) {
  uint32_t dev_id = 0, dev_port, macro = 0, pipe, port;
  bf_status_t rc = 0;
  int ln = 0;
  uint32_t line = 0, pg_sz = 32;
  uint32_t num_pipes;

  UCLI_COMMAND_INFO(uc, "mss-sts", 0, "display lane config and status")

  mss_sts_banner(uc);

  rc = lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  if (rc != 0) return 0;

  // init each lane in each macro
  for (pipe = 0; pipe < num_pipes; pipe++) {
    for (port = 0; port < 72; port++) {
      uint32_t ch;

      dev_port = MAKE_DEV_PORT(pipe, port);
      // verify this a port managed by us (and get ch)
      rc = port_mgr_tof3_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &macro, &ch, NULL);
      if (rc != 0) continue;

      int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
      for (ln = 0; ln < num_ln; ln++) {
        port_mgr_tmac_t *mac_p;
        uint32_t phys_tx_ln, phys_rx_ln;

        mac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
        if (mac_p == NULL) return 0;

        mss_access_t mss;
        bf_tf3_sd_t *tf3_sd;
        lane_cfg_t rtnd_cfg, *cfg = &rtnd_cfg;

        rc = bf_tof3_serdes_status_get(dev_id, dev_port, ln, &rtnd_cfg);
        if (rc != BF_SUCCESS) {
          // return 1;
        }

        tf3_sd =
            bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);
        if (!tf3_sd) {
          return 0;
        }
        // cfg = &tf3_sd->cfg;

        phys_tx_ln = tf3_sd->physical_tx_lane;
        phys_rx_ln = tf3_sd->physical_rx_lane;

        if ((line % pg_sz) == 0) {
          mss_sts_banner(uc);
          line = 0;
        }
        line++;
        aim_printf(&uc->pvs,
                   "%3d|%2d|%3d|%2d|%2d|%2d|%4s|%4s"
                   "|%8s|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%3d|%3d|%3d|%3d|%"
                   "8s|%2d|%3d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%3d|%3d|%3d|"
                   "%3d|%3d|%3d|%4d|%4d|%4s|%3.1e\n",
                   macro,
                   ch,
                   dev_port,
                   ln,
                   phys_tx_ln,
                   phys_rx_ln,
                   cfg->init_status == 0 ? "Pass" : "Fail",
                   bf_tof3_serdes_loopback_mode_to_str(cfg->loopback_mode),
                   bf_tof3_serdes_term_mode_str(cfg->rx_term),
                   cfg->tx_reset,
                   cfg->rx_reset,
                   cfg->tx_rate,
                   cfg->rx_rate,
                   cfg->tx_width,
                   cfg->rx_width,
                   cfg->tx_pstate,
                   cfg->rx_pstate,
                   cfg->cm3,
                   cfg->cm2,
                   cfg->cm1,
                   cfg->c0,
                   cfg->c1,
                   bf_tof3_serdes_prbs_mode_str(cfg->prbs_mode),
                   cfg->prbs_gen_tx_en,
                   cfg->tx_precoder_override,
                   cfg->tx_precoder_en_pc,
                   cfg->rx_precoder_en_pc,
                   cfg->tx_precoder_en_gc,
                   cfg->rx_precoder_en_gc,
                   cfg->tx_polarity,
                   cfg->rx_polarity,
                   cfg->tx_disable,
                   cfg->rx_disable,
                   cfg->sig_det,
                   cfg->cdr_lock,
                   cfg->ctle_rate,
                   cfg->ctle_boost,
                   cfg->vga_coarse,
                   cfg->vga_fine,
                   cfg->vga_offset,
                   cfg->tx_ppm,
                   cfg->rx_ppm,
                   cfg->bist_lock ? "lock" : "----",
                   cfg->ber);
      }
    }
  }

  return rc;
}

void bf_pm_ucli_tof3_serdes_state_banner(ucli_context_t *uc) {
  aim_printf(&uc->pvs,
             "----+------+---+---------------+---+---+---+---+-----"
             "--------------+-----------+---------+-----+-----+---+----+--"
             "-----+-----------------+----+----+----+------+------+\n");
  aim_printf(
      &uc->pvs,
      "Frnt|      |   |    phys       |rst|rte|wid|ps |Tx "
      "FIR             |Tx         |Precod   |gc   |Pol  "
      "|Sig| Cdr|Ctle   |Vga         e    |Tx  |Rx  |BIST|      |      |\n");
  aim_printf(&uc->pvs,
             "port|mac|ch|d_p|ln|tx|rx| term "
             "|t|r|t|r|t|r|t|r|cm3|cm2|cm1|c0 |c1 |Pattern "
             "|En|ovr|tx|rx|tx|rx|tx|rx|Det|Lock|Rte|Bst|Crs|Fin|Ofs|n|cap|PPM "
             "|PPM |Lck |Tx Vco|Rx Vco|\n");
  aim_printf(&uc->pvs,
             "----+---+--+---+--+--+--+------+-+-+-+-+-+-+-+-+---+-"
             "--+---+---+---+--------+--+---+--+--+--+--+--+--+---+----+--"
             "-+---+---+---+---+-+---+----+----+----+------+------+\n");
}

void tof3_ucli_serdes_state_display(uint32_t fp,
                                    uint32_t chnl,
                                    bf_dev_port_t dev_port,
                                    uint32_t ln,
                                    ucli_context_t *uc) {
  uint32_t dev_id = 0, macro = 0;
  bf_status_t rc = 0;
  uint32_t ch;
  uint32_t ln_incr, nserdes_per_mac;
  port_mgr_tmac_t *mac_p;
  uint32_t phys_tx_ln, phys_rx_ln;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  lane_cfg_t rtnd_cfg, *cfg = &rtnd_cfg;
  int32_t tx_ppm;
  int32_t rx_ppm;
  double tx_vco_freq;
  double rx_vco_freq;

  // verify this a port managed by us (and get ch)
  rc = port_mgr_tof3_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &macro, &ch, NULL);
  if (rc != 0) return;

  mac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
  if (mac_p == NULL) return;

  nserdes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);
  ln_incr = (nserdes_per_mac == 8) ? 1 : 2;

  rc = bf_tof3_serdes_status_get(dev_id, dev_port, ln, &rtnd_cfg);

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);
  if (!tf3_sd) {
    return;
  }
  phys_tx_ln = tf3_sd->physical_tx_lane;
  phys_rx_ln = tf3_sd->physical_rx_lane;

  if (cfg->sig_det) {
    rc = bf_tof3_serdes_clk_get(
        dev_id, dev_port, ln, &tx_ppm, &rx_ppm, &tx_vco_freq, &rx_vco_freq);
    if (rc) {
    }
  } else {
    tx_ppm = 0;
    rx_ppm = 0;
    tx_vco_freq = 0;
    rx_vco_freq = 0;
  }
  uint32_t en;
  uint32_t vga_cap;
  bool use_custom_takeover_ratio;
  uint32_t custom_takeover_ratio;
  uint32_t custom_nyq_mask;

  rc = bf_tof3_serdes_rx_vga_cap_adapt_get(dev_id,
                                           dev_port,
                                           ln,
                                           &en,
                                           &vga_cap,
                                           &use_custom_takeover_ratio,
                                           &custom_takeover_ratio,
                                           &custom_nyq_mask);

  aim_printf(&uc->pvs,
             "%2d/%1d|%3d|%2d|%3d|%2d|%2d|%2d"
             "|%6s|%1d|%1d|%1d|%1d|%1d|%1d|%1d|%1d|%3d|%3d|%3d|%3d|%3d|%"
             "8s|%2d|%3d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%3s|%3d|%3d|"
             "%3d|%3d|%3d|%1d|%3d|%4d|%4d|%4s|%6.3f|%6.3f\n",
             fp,
             chnl,
             macro,
             ch,
             dev_port + ((ln * ln_incr) & ~1),
             ln * ln_incr & 1,
             phys_tx_ln,
             phys_rx_ln,
             bf_tof3_serdes_term_mode_str(cfg->rx_term),
             cfg->tx_reset,
             cfg->rx_reset,
             cfg->tx_rate,
             cfg->rx_rate,
             cfg->tx_width,
             cfg->rx_width,
             cfg->tx_pstate,
             cfg->rx_pstate,
             cfg->cm3,
             cfg->cm2,
             cfg->cm1,
             cfg->c0,
             cfg->c1,
             bf_tof3_serdes_prbs_mode_str(cfg->prbs_mode),
             cfg->prbs_gen_tx_en,
             cfg->tx_precoder_override,
             cfg->tx_precoder_en_pc,
             cfg->rx_precoder_en_pc,
             cfg->tx_precoder_en_gc,
             cfg->rx_precoder_en_gc,
             cfg->tx_polarity,
             cfg->rx_polarity,
             cfg->sig_det,
             cfg->cdr_lock ? "lock" : "----",
             cfg->ctle_rate,
             cfg->ctle_boost,
             cfg->vga_coarse,
             cfg->vga_fine,
             cfg->vga_offset,
             en,
             vga_cap,
             tx_ppm,  // cfg->tx_ppm,
             rx_ppm,  // cfg->rx_ppm,
             cfg->bist_lock ? "lock" : "----",
             tx_vco_freq / (double)1000000000ul,
             rx_vco_freq / (double)1000000000ul);
}

static ucli_status_t port_mgr_ucli_ucli__mss_sts_ln__(ucli_context_t *uc) {
  uint32_t dev_id = 0, dev_port, macro = 0;
  bf_status_t rc = 0;
  int ln = 0;
  uint32_t ch;
  port_mgr_tmac_t *mac_p;
  uint32_t phys_tx_ln, phys_rx_ln;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  lane_cfg_t rtnd_cfg, *cfg = &rtnd_cfg;
  int32_t tx_ppm;
  int32_t rx_ppm;
  double tx_vco_freq;
  double rx_vco_freq;

  UCLI_COMMAND_INFO(uc, "mss-sts-ln", 3, "display lane config and status")

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  bf_pm_ucli_tof3_serdes_state_banner(uc);

  // special check fro PCIe PHY
  if ((dev_port >= 1000) && (dev_port < 1008)) {
    macro = 0;
    ch = dev_port - 1000;
  } else {
    // verify this a port managed by us (and get macro and ch)
    rc = port_mgr_tof3_map_dev_port_to_all(
        dev_id, dev_port, NULL, NULL, &macro, &ch, NULL);
    if (rc != 0) return 1;

    mac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
    if (mac_p == NULL) {
      return 0;
    }
  }
  rc = bf_tof3_serdes_status_get(dev_id, dev_port, ln, &rtnd_cfg);
  if (rc != BF_SUCCESS) {
    // return 1;
  }

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);
  if (!tf3_sd) {
    return 0;
  }
  phys_tx_ln = tf3_sd->physical_tx_lane;
  phys_rx_ln = tf3_sd->physical_rx_lane;

  if (cfg->sig_det) {
    rc = bf_tof3_serdes_clk_get(
        dev_id, dev_port, ln, &tx_ppm, &rx_ppm, &tx_vco_freq, &rx_vco_freq);
    if (rc) {
    }
  } else {
    tx_ppm = 0;
    rx_ppm = 0;
    tx_vco_freq = 0;
    rx_vco_freq = 0;
  }
  aim_printf(&uc->pvs,
             "    |%3d|%2d|%3d|%2d|%2d|%2d|"
             "%6s|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%3d|%3d|%3d|%3d|%"
             "8s|%2d|%3d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%4s|%3d|%3d|"
             "%3d|%3d|%3d|%4d|%4d|%4s|%6.3f|%6.3f\n",
             macro,
             ch,
             dev_port,
             ln,
             phys_tx_ln,
             phys_rx_ln,
             bf_tof3_serdes_term_mode_str(cfg->rx_term),
             cfg->tx_reset,
             cfg->rx_reset,
             cfg->tx_rate,
             cfg->rx_rate,
             cfg->tx_width,
             cfg->rx_width,
             cfg->tx_pstate,
             cfg->rx_pstate,
             cfg->cm3,
             cfg->cm2,
             cfg->cm1,
             cfg->c0,
             cfg->c1,
             bf_tof3_serdes_prbs_mode_str(cfg->prbs_mode),
             cfg->prbs_gen_tx_en,
             cfg->tx_precoder_override,
             cfg->tx_precoder_en_pc,
             cfg->rx_precoder_en_pc,
             cfg->tx_precoder_en_gc,
             cfg->rx_precoder_en_gc,
             cfg->tx_polarity,
             cfg->rx_polarity,
             cfg->sig_det,
             cfg->cdr_lock ? "lock" : "----",
             cfg->ctle_rate,
             cfg->ctle_boost,
             cfg->vga_coarse,
             cfg->vga_fine,
             cfg->vga_offset,
             tx_ppm,  // cfg->tx_ppm,
             rx_ppm,  // cfg->rx_ppm,
             cfg->bist_lock ? "lock" : "----",
             tx_vco_freq / (double)1000000000ul,
             rx_vco_freq / (double)1000000000ul);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_sts_grp__(ucli_context_t *uc) {
  uint32_t dev_id = 0, dev_port, macro = 0;
  bf_status_t rc = 0;
  int ln = 0;

  UCLI_COMMAND_INFO(uc, "mss-sts-grp", 2, "display lane config and status")

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);

  dev_port = dev_port & ~7;  // start from base dev_port

  aim_printf(
      &uc->pvs,
      "------+---+---------------------------+-----+-----+-----+-----+-----"
      "--------------+-----------+---------+-----+-----+-----+---+---+--"
      "-+---+---+---+---+----+----+----+---\n");
  aim_printf(
      &uc->pvs,
      "      |   |    phys                   |reset|rate |width|ps   |Tx "
      "FIR             |Tx         |Precod   |gc   |Pol  "
      "|Disbl|Sig|Cdr|Ctle   |Vga        |Tx  |Rx  |BIST|   \n");
  aim_printf(&uc->pvs,
             "mac|ch|d_p|ln|tx|rx|POST|dest|  term. "
             "|tx|rx|tx|rx|tx|rx|tx|rx|cm3|cm2|cm1|c0 |c1 |Pattern "
             "|En|ovr|tx|rx|tx|rx|tx|rx|tx|rx|Det|Lck|Rte|Bst|Crs|Fin|Ofs|PPM "
             "|PPM |Lck |BER\n");
  aim_printf(
      &uc->pvs,
      "---+--+---+--+--+--+----+----+--------+--+--+--+--+--+--+--+--+---+-"
      "--+---+---+---+--------+--+---+--+--+--+--+--+--+--+--+---+---+--"
      "-+---+---+---+---+----+----+----+---\n");

  int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
  for (uint32_t d_p = dev_port; d_p < (dev_port + 8); d_p += 2) {
    for (ln = 0; ln < num_ln; ln++) {
      uint32_t ch;

      // verify this a port managed by us (and get ch)
      rc = port_mgr_tof3_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &macro, &ch, NULL);
      if (rc != 0) return 1;

      port_mgr_tmac_t *mac_p;
      uint32_t phys_tx_ln, phys_rx_ln;

      mac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, d_p + ln);
      if (mac_p == NULL) return 0;

      mss_access_t mss;
      bf_tf3_sd_t *tf3_sd;
      lane_cfg_t rtnd_cfg, *cfg = &rtnd_cfg;

      rc = bf_tof3_serdes_status_get(dev_id, d_p, ln, &rtnd_cfg);
      if (rc != BF_SUCCESS) {
        // return 1;
      }

      tf3_sd = bf_tof3_serdes_addr_set(dev_id, d_p, ln, &mss, MSS_SECTION_RX);
      if (!tf3_sd) {
        return 0;
      }
      phys_tx_ln = tf3_sd->physical_tx_lane;
      phys_rx_ln = tf3_sd->physical_rx_lane;

      // cfg = &tf3_sd->cfg;

      aim_printf(&uc->pvs,
                 "%3d|%2d|%3d|%2d|%2d|%2d|%4s|%4s"
                 "|%8s|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%3d|%3d|%3d|%3d|%"
                 "8s|%2d|%3d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%2d|%3d|%3d|%3d|%3d|"
                 "%3d|%3d|%3d|%4d|%4d|%4s|%3.1e\n",
                 macro,
                 ch,
                 d_p,
                 ln,
                 phys_tx_ln,
                 phys_rx_ln,
                 cfg->init_status == 0 ? "Pass" : "Fail",
                 bf_tof3_serdes_loopback_mode_to_str(cfg->loopback_mode),
                 bf_tof3_serdes_term_mode_str(cfg->rx_term),
                 cfg->tx_reset,
                 cfg->rx_reset,
                 cfg->tx_rate,
                 cfg->rx_rate,
                 cfg->tx_width,
                 cfg->rx_width,
                 cfg->tx_pstate,
                 cfg->rx_pstate,
                 cfg->cm3,
                 cfg->cm2,
                 cfg->cm1,
                 cfg->c0,
                 cfg->c1,
                 bf_tof3_serdes_prbs_mode_str(cfg->prbs_mode),
                 cfg->prbs_gen_tx_en,
                 cfg->tx_precoder_override,
                 cfg->tx_precoder_en_pc,
                 cfg->rx_precoder_en_pc,
                 cfg->tx_precoder_en_gc,
                 cfg->rx_precoder_en_gc,
                 cfg->tx_polarity,
                 cfg->rx_polarity,
                 cfg->tx_disable,
                 cfg->rx_disable,
                 cfg->sig_det,
                 cfg->cdr_lock,
                 cfg->ctle_rate,
                 cfg->ctle_boost,
                 cfg->vga_coarse,
                 cfg->vga_fine,
                 cfg->vga_offset,
                 cfg->tx_ppm,
                 cfg->rx_ppm,
                 cfg->bist_lock ? "lock" : "----",
                 cfg->ber);
    }
  }
  return 0;
}

extern uint32_t bf_tof3_serdes_str_to_loopback_mode(char *str);

static ucli_status_t port_mgr_ucli_ucli__mss_cfg_ln__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;
  uint32_t rate_gb;
  uint32_t is_pam4;
  uint32_t prbs_mode;
  uint32_t loopback_mode;

  UCLI_COMMAND_INFO(
      uc,
      "mss-cfg-ln",
      7,
      "dev_id dev_port ln rate_gb is_pam4 prbs_mode lpbk_mode\n"
      "                 rate_gb = 1, 10, 25, 50, 100\n"
      "                 is_pam4 = 1 (PAM4 graycode_en) 0 (NRZ no graycode)\n"
      "                 prbs_mode = \n"
      "                  AW_PRBS7 = 0\n"
      "                  AW_PRBS9 = 1\n"
      "                  AW_PRBS11 = 2\n"
      "                  AW_PRBS13 = 3\n"
      "                  AW_PRBS15 = 4\n"
      "                  AW_PRBS23 = 5\n"
      "                  AW_PRBS31 = 6\n"
      "                  AW_QPRBS13 = 7\n"
      "                  AW_JP03A = 8\n"
      "                  AW_JP03B = 9\n"
      "                  AW_LINEARITY_PATTERN = 10\n"
      "                  AW_USER_DEFINED_PATTERN = 11\n"
      "                  AW_FULL_RATE_CLOCK = 12\n"
      "                  AW_HALF_RATE_CLOCK = 13\n"
      "                  AW_QUARTER_RATE_CLOCK = 14\n"
      "                  AW_PATT_32_1S_32_0S = 15, // 32 1s and 32 0s "
      "repeating\n"
      "                 lpbk_mode = none nes nep fes fep rmt\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  rate_gb = strtoul(uc->pargs->args[3], NULL, 10);
  is_pam4 = strtoul(uc->pargs->args[4], NULL, 10);
  prbs_mode = strtoul(uc->pargs->args[5], NULL, 10);
  loopback_mode =
      bf_tof3_serdes_str_to_loopback_mode((char *)uc->pargs->args[6]);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  bf_tof3_serdes_config_lane(
      dev_id, dev_port, ln, rate_gb, is_pam4, prbs_mode, loopback_mode);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_cfg_all__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port;
  int ln;
  bf_status_t rc;
  uint32_t rate_gb;
  uint32_t is_pam4;
  uint32_t prbs_mode;
  uint32_t loopback_mode;

  UCLI_COMMAND_INFO(uc,
                    "mss-cfg-all",
                    4,
                    "  rate_gb is_pam4 prbs_mode lpbk_mode\n"
                    "  rate_gb = 1, 10, 25, 50, 100\n"
                    "  is_pam4 = 1 (PAM4 graycode_en) 0 (NRZ no graycode)\n"
                    "  prbs_mode = \n"
                    "   AW_PRBS7 = 0\n"
                    "   AW_PRBS9 = 1\n"
                    "   AW_PRBS11 = 2\n"
                    "   AW_PRBS13 = 3\n"
                    "   AW_PRBS15 = 4\n"
                    "   AW_PRBS23 = 5\n"
                    "   AW_PRBS31 = 6\n"
                    "   AW_QPRBS13 = 7\n"
                    "   AW_JP03A = 8\n"
                    "   AW_JP03B = 9\n"
                    "   AW_LINEARITY_PATTERN = 10\n"
                    "   AW_USER_DEFINED_PATTERN = 11\n"
                    "   AW_FULL_RATE_CLOCK = 12\n"
                    "   AW_HALF_RATE_CLOCK = 13\n"
                    "   AW_QUARTER_RATE_CLOCK = 14\n"
                    "   AW_PATT_32_1S_32_0S = 15, // 32 1s and 32 0s "
                    "repeating\n"
                    "                 lpbk_mode = none nes nep fes fep rmt\n");

  dev_id = 0;
  rate_gb = strtoul(uc->pargs->args[0], NULL, 10);
  is_pam4 = strtoul(uc->pargs->args[1], NULL, 10);
  prbs_mode = strtoul(uc->pargs->args[2], NULL, 10);
  loopback_mode =
      bf_tof3_serdes_str_to_loopback_mode((char *)uc->pargs->args[3]);

  // init each lane in each macro
  for (uint32_t pipe = 0; pipe < 4; pipe++) {
    for (uint32_t port = 0; port < 72; port++) {
      dev_port = MAKE_DEV_PORT(pipe, port);
      // verify this a port managed by us (and get ch)
      rc = port_mgr_tof3_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, NULL, NULL, NULL);
      if (rc != 0) continue;

      int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
      for (ln = 0; ln < num_ln; ln++) {
        rc = bf_tof3_serdes_config_lane(
            dev_id, dev_port, ln, rate_gb, is_pam4, prbs_mode, loopback_mode);
        if (rc != 0) {
          aim_printf(
              &uc->pvs, "Error: bf_tof3_serdes_config_lane: rc = %d\n", rc);
        }
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_cfg_grp__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port;
  int ln;
  bf_status_t rc;
  uint32_t rate_gb;
  uint32_t is_pam4;
  uint32_t prbs_mode;
  uint32_t loopback_mode;

  UCLI_COMMAND_INFO(uc,
                    "mss-cfg-grp",
                    6,
                    "  dev_id dev_port rate_gb is_pam4 prbs_mode lpbk_mode\n"
                    "  rate_gb = 1, 10, 25, 50, 100\n"
                    "  is_pam4 = 1 (PAM4 graycode_en) 0 (NRZ no graycode)\n"
                    "  prbs_mode = \n"
                    "   AW_PRBS7 = 0\n"
                    "   AW_PRBS9 = 1\n"
                    "   AW_PRBS11 = 2\n"
                    "   AW_PRBS13 = 3\n"
                    "   AW_PRBS15 = 4\n"
                    "   AW_PRBS23 = 5\n"
                    "   AW_PRBS31 = 6\n"
                    "   AW_QPRBS13 = 7\n"
                    "   AW_JP03A = 8\n"
                    "   AW_JP03B = 9\n"
                    "   AW_LINEARITY_PATTERN = 10\n"
                    "   AW_USER_DEFINED_PATTERN = 11\n"
                    "   AW_FULL_RATE_CLOCK = 12\n"
                    "   AW_HALF_RATE_CLOCK = 13\n"
                    "   AW_QUARTER_RATE_CLOCK = 14\n"
                    "   AW_PATT_32_1S_32_0S = 15, // 32 1s and 32 0s "
                    "repeating\n"
                    "                 lpbk_mode = none nes nep fes fep rmt\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  rate_gb = strtoul(uc->pargs->args[2], NULL, 10);
  is_pam4 = strtoul(uc->pargs->args[3], NULL, 10);
  prbs_mode = strtoul(uc->pargs->args[4], NULL, 10);
  loopback_mode =
      bf_tof3_serdes_str_to_loopback_mode((char *)uc->pargs->args[5]);

  uint32_t pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  uint32_t num_lanes;

  if (((dev_port >= 1000) && (dev_port < 1008))) {
    num_lanes = 4;
  } else if (dev_port < 8) {
    num_lanes = 4;
  } else {
    num_lanes = 8;
  }
  // init each lane in each macro
  int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
  for (uint32_t i = 0; i < num_lanes; i += num_ln) {
    uint32_t _dev_port;

    _dev_port = MAKE_DEV_PORT(pipe, port + i);
    // verify this a port managed by us (and get ch)
    rc = port_mgr_tof3_map_dev_port_to_all(
        dev_id, _dev_port, NULL, NULL, NULL, NULL, NULL);
    if (rc != 0) continue;

    for (ln = 0; ln < num_ln; ln++) {
      rc = bf_tof3_serdes_config_lane(
          dev_id, _dev_port, ln, rate_gb, is_pam4, prbs_mode, loopback_mode);
      if (rc != 0) {
        aim_printf(
            &uc->pvs, "Error: bf_tof3_serdes_config_lane: rc = %d\n", rc);
      }
    }
  }
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_un_cfg_ln__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc, "mss-un-cfg-ln", 3, "dev_id dev_port ln (power-dn)\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_power_down(dev_id, dev_port, ln);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__mss_squelch_ln__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, en;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc,
                    "mss-squelch-ln",
                    4,
                    "dev_id dev_port ln (enable/disable tx output)\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  en = strtoul(uc->pargs->args[3], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_squelch_set(dev_id, dev_port, ln, en);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__mss_pstate_set__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, tx_pstate, rx_pstate;
  bf_status_t rc;

  UCLI_COMMAND_INFO(
      uc, "mss-pstate", 5, "dev_id dev_port ln tx_pstate rx_pstate\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  tx_pstate = strtoul(uc->pargs->args[3], NULL, 10);
  rx_pstate = strtoul(uc->pargs->args[4], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_pstate_set(dev_id, dev_port, ln, tx_pstate, rx_pstate);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__mss_ctle_set__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, rx_ctle_adapt_en, rx_ctle_adapt_boost;
  bf_status_t rc;

  UCLI_COMMAND_INFO(
      uc, "mss-ctle-set", 5, "dev_id dev_port ln ctle_en ctle_boost\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  rx_ctle_adapt_en = strtoul(uc->pargs->args[3], NULL, 10);
  rx_ctle_adapt_boost = strtoul(uc->pargs->args[4], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_ctle_adapt_set(
      dev_id, dev_port, ln, rx_ctle_adapt_en, rx_ctle_adapt_boost);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__mss_ctle_get__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, rx_ctle_adapt_en, rx_ctle_adapt_boost;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc,
                    "mss-ctle-get",
                    3,
                    "dev_id dev_port ln (returns ctle_en ctle_boost)\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_ctle_adapt_get(
      dev_id, dev_port, ln, &rx_ctle_adapt_en, &rx_ctle_adapt_boost);
  if (rc == 0) {
    aim_printf(&uc->pvs,
               "rx_ctle_adapt_en = %d\nrx_ctle_adapt_boost = %d\n",
               rx_ctle_adapt_en,
               rx_ctle_adapt_boost);
  } else {
    aim_printf(&uc->pvs, "Error %d from: bf_tof3_serdes_ctle_adapt_get\n", rc);
  }
  return rc;
}

extern bf_status_t bf_aw_pmd_iso_rx_rate_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *value);

#if 0
static ucli_status_t port_mgr_ucli_ucli__mss_check_bist__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, timer_threshold, timeout_us;
  bf_status_t rc;
  uint64_t err_count;
  uint32_t err_count_done;
  uint32_t err_count_overflown;
  double ber;

  UCLI_COMMAND_INFO(uc,
                    "mss-check-bist",
                    5,
                    "dev_id dev_port ln timer_threshold timeout_us\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  timer_threshold = strtoul(uc->pargs->args[3], NULL, 16);
  timeout_us = strtoul(uc->pargs->args[4], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_check_bist(dev_id,
                                 dev_port,
                                 ln,
                                 timer_threshold,
                                 timeout_us,
                                 &err_count,
                                 &err_count_done,
                                 &err_count_overflown,
                                 &ber);
  aim_printf(&uc->pvs,
             "done=%d : count=%16" PRIx64 " : ovfl=%d : BER=%3.2e\n",
             err_count_done,
             err_count,
             err_count_overflown,
             ber);
  return rc;
}
#endif

extern bf_status_t bf_tof3_run_lt(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  uint32_t ln,
                                  uint32_t *lt_status);
static ucli_status_t port_mgr_ucli_ucli__mss_lt__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln;
  bf_status_t rc;
  uint32_t lt_status = 0;

  UCLI_COMMAND_INFO(uc, "mss-lt", 3, "dev_id dev_port ln \n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  rc = bf_tof3_run_lt(dev_id, dev_port, ln, &lt_status);
  aim_printf(
      &uc->pvs, "LT status : %08x      (done is [3:0] = 0xd)\n", lt_status);
  return rc;
}

extern bf_status_t bf_tof3_run_lt2(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t ln,
                                   uint32_t width,
                                   uint32_t clause);

static ucli_status_t port_mgr_ucli_ucli__mss_lt2__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, width, clause;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc, "mss-lt2", 5, "dev_id dev_port ln width clause\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  width = strtoul(uc->pargs->args[3], NULL, 10);
  clause = strtoul(uc->pargs->args[4], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if (width >= 8) {
    aim_printf(&uc->pvs, "invalid width: %d\n", ln);
    return 1;
  }
  if (clause > 4) {
    aim_printf(&uc->pvs, "invalid clause: %d\n", ln);
    return 1;
  }
  rc = bf_tof3_run_lt2(dev_id, dev_port, ln, width, clause);
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__mss_equalize_ln__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, eq_type, tout;
  bf_status_t rc;

  UCLI_COMMAND_INFO(
      uc,
      "mss-eq-ln",
      5,
      "dev_id dev_port ln eq_type timeout_us\n"
      "                eq_type =\n"
      "                 AW_EQ_FULL_DIR = 0, // Full EQ, Directional\n"
      "                 AW_EQ_EVAL_DIR = 1, // Eval Only, Directional\n"
      "                 AW_EQ_INIT_EVAL = 2, // Init Eval\n"
      "                 AW_EQ_CLEAR_EVAL = 3, // Clear Eval\n"
      "                 AW_EQ_FULL_FOM = 4, // Full EQ, FOM\n"
      "                 AW_EQ_EVAL_FOM = 5 // Eval Only, FOM\n"
      "                tout = timeout in usec (decimal, typical=10000)\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  eq_type = strtoul(uc->pargs->args[3], NULL, 10);
  tout = strtoul(uc->pargs->args[4], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }

  rc = bf_tof3_serdes_rx_eq(dev_id, dev_port, ln, eq_type, tout);
  return rc;
}

bf_status_t bf_tof3_serdes_addr_range(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t ln,
                                      uint32_t *subdev_id,
                                      uint32_t *macro,
                                      uint32_t *phys_tx_ln,
                                      uint32_t *phys_rx_ln,
                                      uint32_t *cmn_ofs,
                                      uint32_t *tx_ofs,
                                      uint32_t *rx_ofs,
                                      uint32_t *sram0,
                                      uint32_t *sram1);

static ucli_status_t port_mgr_ucli_ucli__mss_addr__(ucli_context_t *uc) {
  uint32_t dev_id, subdev_id, dev_port, ln, macro, cmn_ofs, tx_ofs, rx_ofs;
  uint32_t phys_tx_ln, phys_rx_ln;
  uint32_t sram0;
  uint32_t sram1;
  bf_status_t rc;

  UCLI_COMMAND_INFO(
      uc, "mss-addr", 3, "dev_id dev_port ln eq_type timeout_us\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  rc = bf_tof3_serdes_addr_range(dev_id,
                                 dev_port,
                                 ln,
                                 &subdev_id,
                                 &macro,
                                 &phys_tx_ln,
                                 &phys_rx_ln,
                                 &cmn_ofs,
                                 &tx_ofs,
                                 &rx_ofs,
                                 &sram0,
                                 &sram1);
  aim_printf(&uc->pvs,
             "-------+------+-----+---------+---+---+----------+---------+-----"
             "-----+----------+----------+\n");
  aim_printf(&uc->pvs,
             "dev_id |subdev| d_p |  macro  |Tx |Rx |    Cmn   |   Tx    |    "
             "Rx    |  sram0   |   sram1  |\n");
  aim_printf(&uc->pvs,
             "-------+------+-----+---------+---+---+----------+---------+-----"
             "-----+----------+----------+\n");
  aim_printf(&uc->pvs,
             "   %d   |   %d   | %3d | serdes%2d | %d | %d | %08x | %08x | "
             "%08x | %08x | %08x |\n",
             dev_id,
             subdev_id,
             dev_port,
             macro,
             phys_tx_ln,
             phys_rx_ln,
             cmn_ofs,
             tx_ofs,
             rx_ofs,
             sram0,
             sram1);

  return rc;
}

extern bf_status_t bf_aw_pmd_read_tracebuffer_ffe(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    int num_samples,
    int32_t ffe_data[2][num_samples],
    int branch_id);

extern bf_status_t bf_aw_pmd_read_tracebuffer_adc(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    int num_samples,
    int32_t ffe_data[2][num_samples]);

/*
 * Example function for gathering FFE data and dumping to CSV
 */
int aw_dump_ffe_data(ucli_context_t *uc,
                     bf_dev_id_t dev_id,
                     bf_dev_port_t dev_port,
                     uint32_t ln,
                     int num_samples) {
  bf_status_t rc = BF_SUCCESS;
  aim_printf(&uc->pvs, "running aw_dump_ffe_data...\n");

  int num_branches_to_collect = 32;  // collect all branches 64/2
  int all_ffe_data[num_branches_to_collect][2][num_samples];

  // collect data for all branches
  for (int branch_id = 0; branch_id < num_branches_to_collect; branch_id++) {
    // aw_pmd_read_tracebuffer_ffe(mss, num_samples, all_ffe_data[branch_id],
    // branch_id);
    bf_aw_pmd_read_tracebuffer_ffe(
        dev_id, dev_port, ln, num_samples, all_ffe_data[branch_id], branch_id);
  }

  // print csv header
  int br_num = 0;
  for (int i = 0; i < num_branches_to_collect; i++) {
    for (int j = 0; j < 2; j++) {
      aim_printf(&uc->pvs, "ffe_br%d", br_num);
      br_num++;
      if (br_num != 64) {
        aim_printf(&uc->pvs, ",");
      }
    }
  }
  aim_printf(&uc->pvs, "\n");
  for (int k = 0; k < num_samples; k++) {
    br_num = 0;
    for (int i = 0; i < num_branches_to_collect; i++) {
      for (int j = 0; j < 2; j++) {
        aim_printf(&uc->pvs, "%d", all_ffe_data[i][j][k]);
        br_num++;
        if (br_num != num_branches_to_collect * 2) {
          aim_printf(&uc->pvs, ",");
        }
      }
    }
    aim_printf(&uc->pvs, "\n");
  }
  return rc;
}

static ucli_status_t port_mgr_ucli_ucli__mss_ffe__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, samples;
  bf_status_t rc = BF_SUCCESS;

  UCLI_COMMAND_INFO(uc, "mss-ffe", 4, "dev_id dev_port ln samples\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  samples = strtoul(uc->pargs->args[3], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if (samples % AW_TBUS_NUM_SAMPLES) {
    aim_printf(&uc->pvs,
               "invalid ln: %d. Must be multiple of %d\n",
               ln,
               AW_TBUS_NUM_SAMPLES);
    return 1;
  }
  aw_dump_ffe_data(uc, dev_id, dev_port, ln, samples);

  return rc;
}

/*
 * Example function for gathering ADC data and dumping to CSV
 */
#define max_samples 50000
int adc_data[max_samples][AW_NUM_BRANCHES];
int aw_dump_adc_data(ucli_context_t *uc,
                     bf_dev_id_t dev_id,
                     bf_dev_port_t dev_port,
                     uint32_t ln,
                     int num_samples) {
  (void)uc;

  FILE *fptr = fopen("samples_adc.csv", "w");
  if (fptr == NULL) {
    printf("Error!\n");
    return 1;
  }
  char cwd[1000];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
    fflush(stdout);
  }

  bf_aw_pmd_read_tracebuffer_adc(dev_id, dev_port, ln, num_samples, adc_data);

#if 0
  // the primary use for this function is to use its output as the
  // input to the serdes analyzer tool, which does not recognize the
  // csv header row.
  //
  for (int j=0;j<AW_NUM_BRANCHES;j++) {
    // print samples in CSV format
    fprintf(fptr, "adc_br%d", j);
    if (j!=AW_NUM_BRANCHES-1){
      fprintf(fptr, ",");
    }
  }
  fprintf(fptr, "\n");
#endif
  for (int i = 0; i < num_samples; i++) {
    for (int j = 0; j < AW_NUM_BRANCHES; j++) {
      // print samples in CSV format
      fprintf(fptr, "%d", adc_data[i][j]);
      if (j != AW_NUM_BRANCHES - 1) {
        fprintf(fptr, ",");
      }
    }
    fprintf(fptr, "\n");
  }
  fclose(fptr);
  return 0;
}

static ucli_status_t port_mgr_ucli_ucli__mss_adc__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, samples;
  bf_status_t rc = BF_SUCCESS;

  UCLI_COMMAND_INFO(uc, "mss-adc", 4, "dev_id dev_port ln samples\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  samples = strtoul(uc->pargs->args[3], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if (samples % AW_TBUS_NUM_SAMPLES) {
    aim_printf(
        &uc->pvs, "Samples must be a multiple of %d\n", AW_TBUS_NUM_SAMPLES);
    return 1;
  }
  if ((samples < 10000) || (samples > 50000)) {
    aim_printf(&uc->pvs, "Samples must be between 10000 - 50000\n");
    return 1;
  }
  aw_dump_adc_data(uc, dev_id, dev_port, ln, samples);

  return rc;
}

bf_status_t bf_aw_pmd_rx_dig_pwr_det_threshold_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t *adc_valid_thresh_nt,
    uint32_t *adc_invalid_thresh_nt);
bf_status_t bf_aw_pmd_rx_dig_pwr_det_threshold_set(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t ln,
    uint32_t adc_valid_thresh_nt,
    uint32_t adc_invalid_thresh_nt);
bf_status_t bf_aw_pmd_rx_signal_detect_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           uint32_t *signal_detect);

static ucli_status_t port_mgr_ucli_ucli__mss_sig_detect__(ucli_context_t *uc) {
  uint32_t dev_id, dev_port, ln, invalid, valid;
  bf_status_t rc = BF_SUCCESS;

  UCLI_COMMAND_INFO(uc, "mss-sig-detect", 3, "dev_id dev_port ln\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  bf_aw_pmd_rx_dig_pwr_det_threshold_get(
      dev_id, dev_port, ln, &valid, &invalid);
  aim_printf(
      &uc->pvs, "valid thresh: %04x : invalid thresh: %04x\n", valid, invalid);

  uint32_t signal_detect;
  bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, &signal_detect);
  aim_printf(&uc->pvs, "%ssignal detected\n", signal_detect ? "" : "No ");

  // reduce invalid thresh until signal is detected continuously for 10 seconds
  //
  int solid = false;
  do {
    bf_aw_pmd_rx_dig_pwr_det_threshold_set(
        dev_id, dev_port, ln, valid, invalid);
    bf_sys_usleep(100);
    bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, &signal_detect);
    if (signal_detect) {
      aim_printf(&uc->pvs,
                 "valid thresh: %04x : invalid thresh: %04x\n",
                 valid,
                 invalid);
      aim_printf(&uc->pvs, "%ssignal detected\n", signal_detect ? "" : "No ");
    }
    uint32_t debounce_tmr = 10000000 / 100;
    while (signal_detect) {
      bf_sys_usleep(100);
      bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, &signal_detect);
      if (--debounce_tmr == 0) {
        solid = true;
        break;
      }
    }
    if (!solid) {
      if (invalid < 100) break;
      invalid -= 1;
    }
  } while (!solid);
  bf_aw_pmd_rx_dig_pwr_det_threshold_get(
      dev_id, dev_port, ln, &valid, &invalid);
  aim_printf(&uc->pvs,
             "Final valid thresh: %04x : invalid thresh: %04x\n",
             valid,
             invalid);

  bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, &signal_detect);
  aim_printf(&uc->pvs, "%ssignal detected\n", signal_detect ? "" : "No ");

  return rc;
}

extern bf_status_t bf_tof3_sweep(bf_dev_id_t dev_id,
                                 bf_dev_port_t tx_dev_port,
                                 bf_dev_port_t rx_dev_port,
                                 uint32_t ln,
                                 // tx settings
                                 uint32_t cm3,
                                 uint32_t cm2,
                                 uint32_t cm1,
                                 uint32_t c0,
                                 uint32_t c1,
                                 // rx settings
                                 uint32_t ctle_adapt_en,
                                 uint32_t ctle_adapt_boost,
                                 uint32_t vga_cap,
                                 // return vals
                                 uint32_t *eq_ack,
                                 uint32_t *cdr_lock,
                                 uint32_t *bist_lock,
                                 double *ber);

uint32_t cm3_min = 0, cm3_max = 1;
uint32_t cm2_min = 0, cm2_max = 8;
uint32_t cm1_min = 0, cm1_max = 16;
uint32_t c0_min = 40, c0_max = 61;
uint32_t c1_min = 0, c1_max = 4;
// rx settings
uint32_t ctle_adapt_en_min = 0, ctle_adapt_en_max = 1;
uint32_t ctle_adapt_boost_min = 0, ctle_adapt_boost_max = 1;
uint32_t vga_cap_min = 0, vga_cap_max = 1;

static ucli_status_t port_mgr_ucli_ucli__mss_sweep__(ucli_context_t *uc) {
  uint32_t dev_id, tx_dev_port, rx_dev_port, ln;
  bf_status_t rc;

  UCLI_COMMAND_INFO(uc, "mss-sweep", 4, "dev_id tx_dev_port ln rx_dev_port\n");

  dev_id = strtoul(uc->pargs->args[0], NULL, 10);
  tx_dev_port = strtoul(uc->pargs->args[1], NULL, 10);
  ln = strtoul(uc->pargs->args[2], NULL, 10);
  rx_dev_port = strtoul(uc->pargs->args[3], NULL, 10);

  if (dev_id > 0) {
    aim_printf(&uc->pvs, "valid dev_id range is 0-0\n");
    return 1;
  }
  if (!dev_port_validate(tx_dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", tx_dev_port);
    return 1;
  }
  if (ln > 8) {
    aim_printf(&uc->pvs, "invalid ln: %d\n", ln);
    return 1;
  }
  if (!dev_port_validate(rx_dev_port)) {
    aim_printf(&uc->pvs, "invalid dev_port: %d\n", rx_dev_port);
    return 1;
  }
  uint32_t cm3;
  uint32_t cm2;
  uint32_t cm1;
  uint32_t c0;
  uint32_t c1;
  // rx settings
  uint32_t ctle_adapt_en;
  uint32_t ctle_adapt_boost;
  uint32_t vga_cap;
  // return vals
  uint32_t eq_ack;
  uint32_t cdr_lock;
  uint32_t bist_lock;
  double ber;

  aim_printf(&uc->pvs,
             "csv, rc, dev_id, tx_dev_port, ln, dev_id, rx_dev_port, ln, cm3, "
             "cm2, cm1, c0, c1, ctle_adapt_en, ctle_adapt_boost, vga_cap, "
             "eq_ack, cdr_lock, bist_lock, ber\n");

  for (cm3 = cm3_min; cm3 < cm3_max; cm3 += 1) {
    for (cm2 = cm2_min; cm2 < cm2_max; cm2 += 1) {
      for (cm1 = cm1_min; cm1 < cm1_max; cm1 += 2) {
        for (c0 = c0_min; c0 < c0_max; c0 += 5) {
          for (c1 = c1_min; c1 < c1_max; c1 += 1) {
            for (ctle_adapt_en = ctle_adapt_en_min;
                 ctle_adapt_en < ctle_adapt_en_max;
                 ctle_adapt_en += 1) {
              for (ctle_adapt_boost = ctle_adapt_boost_min;
                   ctle_adapt_boost < ctle_adapt_boost_max;
                   ctle_adapt_boost += 1) {
                for (vga_cap = vga_cap_min; vga_cap < vga_cap_max;
                     vga_cap += 1) {
                  rc = bf_tof3_sweep(dev_id,
                                     tx_dev_port,
                                     rx_dev_port,
                                     ln,
                                     // tx settings
                                     cm3,
                                     cm2,
                                     cm1,
                                     c0,
                                     c1,
                                     // rx settings
                                     ctle_adapt_en,
                                     ctle_adapt_boost,
                                     vga_cap,
                                     // return vals
                                     &eq_ack,
                                     &cdr_lock,
                                     &bist_lock,
                                     &ber);
                  if (rc != 0) continue;

                  aim_printf(&uc->pvs,
                             "%2d : %1d:%3d:%1d - %1d:%3d:%1d : %1d %1d %2d "
                             "%2d %1d : %1d %2d %1d : %d %d %d %3.1e\n",
                             rc,
                             dev_id,
                             tx_dev_port,
                             ln,
                             dev_id,
                             rx_dev_port,
                             ln,
                             // tx settings
                             cm3,
                             cm2,
                             cm1,
                             c0,
                             c1,
                             // rx settings
                             ctle_adapt_en,
                             ctle_adapt_boost,
                             vga_cap,
                             // return vals
                             eq_ack,
                             cdr_lock,
                             bist_lock,
                             ber);

                  aim_printf(&uc->pvs,
                             "csv,%2d,%1d,%3d,%1d,%1d,%3d,%1d,%1d,%1d,%2d,%2d,%"
                             "1d,%1d,%2d,%1d,%d,%d,%d,%3.1e\n",
                             rc,
                             dev_id,
                             tx_dev_port,
                             ln,
                             dev_id,
                             rx_dev_port,
                             ln,
                             // tx settings
                             cm3,
                             cm2,
                             cm1,
                             c0,
                             c1,
                             // rx settings
                             ctle_adapt_en,
                             ctle_adapt_boost,
                             vga_cap,
                             // return vals
                             eq_ack,
                             cdr_lock,
                             bist_lock,
                             ber);
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

/* <auto.ucli.handlers.start> */
/* <auto.ucli.handlers.end> */
static ucli_command_handler_f port_mgr_ucli_ucli_handlers__[] = {
    // bf_ APIs for use on single ports at a time
    port_mgr_ucli_ucli__bf_port_add__,
    port_mgr_ucli_ucli__bf_port_rmv__,
    port_mgr_ucli_ucli__bf_port_enable__,
    port_mgr_ucli_ucli__bf_port_disable__,
    port_mgr_ucli_ucli__bf_port_pause_set__,
    port_mgr_ucli_ucli__bf_port_pfc_set__,
    port_mgr_ucli_ucli__bf_port_preamble_len_set__,
    port_mgr_ucli_ucli__bf_port_ifg_set__,
    port_mgr_ucli_ucli__bf_port_mtu_set__,
    port_mgr_ucli_ucli__bf_port_loopback_mode_set__,
    port_mgr_ucli_ucli__bf_port_txff_truncation_set__,
    port_mgr_ucli_ucli__bf_port_txff_mode_set__,
    port_mgr_ucli_ucli__bf_port_xoff_pause_time_set__,
    port_mgr_ucli_ucli__bf_port_xon_pause_time_set__,
    port_mgr_ucli_ucli__bf_port_force_lf_set__,
    port_mgr_ucli_ucli__bf_port_force_rf_set__,
    port_mgr_ucli_ucli__bf_port_force_idle_set__,
    port_mgr_ucli_ucli__bf_port_stats_clear__,
    port_mgr_ucli_ucli__bf_port_lf_rf_get__,
    port_mgr_ucli_ucli__bf_port_link_fault_status_get__,
    port_mgr_ucli_ucli__bf_port_clkobs_set__,
    port_mgr_ucli_ucli__bf_port_tx_clk_sel__,
    // bf_ APIs for use on multiple ports at a time
    port_mgr_ucli_ucli__bf_port_addm__,
    port_mgr_ucli_ucli__bf_port_rmvm__,
    port_mgr_ucli_ucli__bf_port_enablem__,
    port_mgr_ucli_ucli__bf_port_disablem__,
    port_mgr_ucli_ucli__bf_port_pause_setm__,
    port_mgr_ucli_ucli__bf_port_pfc_setm__,
    port_mgr_ucli_ucli__bf_port_preamble_len_setm__,
    port_mgr_ucli_ucli__bf_port_ifg_setm__,
    port_mgr_ucli_ucli__bf_port_mtu_setm__,
    port_mgr_ucli_ucli__bf_port_loopback_mode_setm__,
    port_mgr_ucli_ucli__bf_port_txff_truncation_setm__,
    port_mgr_ucli_ucli__bf_port_txff_mode_setm__,
    port_mgr_ucli_ucli__bf_port_xoff_pause_time_setm__,
    port_mgr_ucli_ucli__bf_port_xon_pause_time_setm__,
    port_mgr_ucli_ucli__bf_port_force_lf_setm__,
    port_mgr_ucli_ucli__bf_port_force_rf_setm__,
    port_mgr_ucli_ucli__bf_port_force_idle_setm__,
    port_mgr_ucli_ucli__bf_port_stats_clearm__,
    port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_tx_set__,
    port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_tx_get__,
    port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_rx_set__,
    port_mgr_ucli_ucli__bf_port_1588_timestamp_delta_rx_get__,
    port_mgr_ucli_ucli__bf_port_1588_timestamp_tx_get__,
    // show commands (config and status and counters)
    port_mgr_ucli_ucli__ports__,
    port_mgr_ucli_ucli__oper__,
    port_mgr_ucli_ucli__pcs_ctrs__,
    port_mgr_ucli_ucli__qsts__,
    port_mgr_ucli_ucli__fec__,
    port_mgr_ucli_ucli__but_us__,
    port_mgr_ucli_ucli__but__,
    port_mgr_ucli_ucli__lut_us__,
    port_mgr_ucli_ucli__lut__,
    port_mgr_ucli_ucli__lt__,
    port_mgr_ucli_ucli__qchk__,
    port_mgr_ucli_ucli__ch_ena__,
    // port (rmon) statistics
    port_mgr_ucli_ucli__rmon__,
    port_mgr_ucli_ucli__r_rmon__,
    // port_mgr_ucli_ucli__mac_ints__,
    port_mgr_ucli_ucli__mac_poll__,
    port_mgr_ucli_ucli__use_short_timers__,
    port_mgr_ucli_ucli__emu_setup__,
    port_mgr_ucli_ucli__warm__,
    port_mgr_ucli_ucli__temp_start__,
    port_mgr_ucli_ucli__temp_get__,
    port_mgr_ucli_ucli__volt_start__,
    port_mgr_ucli_ucli__volt_get__,
    port_mgr_ucli_ucli__fp__,
    port_mgr_ucli_ucli__op__,
    port_mgr_ucli_ucli__an__,
    port_mgr_ucli_ucli__sd__,
    port_mgr_ucli_ucli__prbs_m__,
    port_mgr_ucli_ucli__bf_serdes_tx_patsel_set__,
    port_mgr_ucli_ucli__bf_serdes_rx_patsel_set__,
    port_mgr_ucli_ucli__bf_serdes_map_dev_port_to_ring_sd__,
    port_mgr_ucli_ucli__warm_init_end__,
    port_mgr_ucli_ucli__ctr__,
    port_mgr_ucli_ucli__ctr_dp__,
    port_mgr_ucli_ucli__chsts__,
    port_mgr_ucli_ucli__chint__,
    port_mgr_ucli_ucli__chcfg__,
    port_mgr_ucli_ucli__tile_rd__,
    port_mgr_ucli_ucli__tile_wr__,
    port_mgr_ucli_ucli__tile_reg_dump__,
    port_mgr_ucli_ucli__tile_temp__,
    port_mgr_ucli_ucli__tile_fw_temp__,
    port_mgr_ucli_ucli__tile_efuse__,
    port_mgr_ucli_ucli__mss_reset__,
    port_mgr_ucli_ucli__fw_load__,
    port_mgr_ucli_ucli__top_init__,
    port_mgr_ucli_ucli__txfir_set__,
    port_mgr_ucli_ucli__txfir_get__,
    port_mgr_ucli_ucli__mss_loopback_set__,
    port_mgr_ucli_ucli__mss_sts__,
    port_mgr_ucli_ucli__mss_sts_grp__,
    port_mgr_ucli_ucli__mss_sts_ln__,
    port_mgr_ucli_ucli__mss_dump2__,
    port_mgr_ucli_ucli__mss_dump__,
    port_mgr_ucli_ucli__mss_wr__,
    port_mgr_ucli_ucli__mss_rx_wr__,
    port_mgr_ucli_ucli__mss_tx_wr__,
    port_mgr_ucli_ucli__mss_rd__,
    port_mgr_ucli_ucli__mss_rx_rd__,
    port_mgr_ucli_ucli__mss_tx_rd__,
    port_mgr_ucli_ucli__mss_grp_rd__,
    port_mgr_ucli_ucli__mss_w__,
    port_mgr_ucli_ucli__mss_rx_w__,
    port_mgr_ucli_ucli__mss_tx_w__,
    port_mgr_ucli_ucli__mss_r__,
    port_mgr_ucli_ucli__mss_rx_r__,
    port_mgr_ucli_ucli__mss_tx_r__,
    port_mgr_ucli_ucli__mss_w_fld__,
    port_mgr_ucli_ucli__mss_rx_w_fld__,
    port_mgr_ucli_ucli__mss_tx_w_fld__,
    port_mgr_ucli_ucli__mss_dfx__,
    port_mgr_ucli_ucli__mss_vregs__,
    port_mgr_ucli_ucli__mss_modified__,
    port_mgr_ucli_ucli__mss_regdump__,
    port_mgr_ucli_ucli__mss_cfg_ln__,
    port_mgr_ucli_ucli__mss_cfg_grp__,
    port_mgr_ucli_ucli__mss_cfg_all__,
    port_mgr_ucli_ucli__mss_un_cfg_ln__,
    port_mgr_ucli_ucli__mss_squelch_ln__,
    port_mgr_ucli_ucli__mss_lkup__,
    port_mgr_ucli_ucli__mss_equalize_ln__,
    port_mgr_ucli_ucli__mss_pstate_set__,
    port_mgr_ucli_ucli__mss_ctle_set__,
    port_mgr_ucli_ucli__mss_ctle_get__,
    //    port_mgr_ucli_ucli__mss_check_bist__,
    port_mgr_ucli_ucli__mss_addr__,
    port_mgr_ucli_ucli__mss_adc__,
    port_mgr_ucli_ucli__mss_ffe__,
    port_mgr_ucli_ucli__mss_sweep__,
    port_mgr_ucli_ucli__mss_lt__,
    port_mgr_ucli_ucli__mss_lt2__,
    port_mgr_ucli_ucli__mss_sig_detect__,
    NULL};

static ucli_module_t port_mgr_ucli_module__ = {
    "port_mgr_ucli",
    NULL,
    port_mgr_ucli_ucli_handlers__,
    NULL,
    NULL,
};

ucli_node_t *port_mgr_ucli_node_create(void) {
  ucli_node_t *n;
  ucli_module_init(&port_mgr_ucli_module__);
  n = ucli_node_create("port_mgr", NULL, &port_mgr_ucli_module__);
  ucli_node_subnode_add(n, ucli_module_log_node_create("port_mgr"));
  return n;
}

static ucli_command_handler_f bf_drv_show_tech_ucli_port_handlers__[] = {
    port_mgr_ucli_ucli__ports__,
    port_mgr_ucli_ucli__oper__,
    port_mgr_ucli_ucli__pcs_ctrs__,
    port_mgr_ucli_ucli__fec__,
    port_mgr_ucli_ucli__ch_ena__,
    port_mgr_ucli_ucli__an__,
    port_mgr_ucli_ucli__op__,
    port_mgr_ucli_ucli__qsts__,
    port_mgr_ucli_ucli__but_us__,
    port_mgr_ucli_ucli__but__,
    port_mgr_ucli_ucli__lut_us__,
    port_mgr_ucli_ucli__lut__,
    port_mgr_ucli_ucli__lt__,
    port_mgr_ucli_ucli__chsts__,
    port_mgr_ucli_ucli__chint__,
    port_mgr_ucli_ucli__chcfg__,
    NULL};

ucli_status_t bf_drv_show_tech_ucli_port__(ucli_context_t *uc) {
  bf_status_t sts;
  uint32_t num_fp_ports = 0, ports_iter = 0, hdl_iter = 0;
  bf_dev_id_t dev_id;
  aim_printf(&uc->pvs, "-------------------- PORT --------------------\n");
  /* Generic commands which display all ports. */
  while (hdl_iter < SHOW_TECH_PORT_UCLI_GENERIC) {
    bf_drv_show_tech_ucli_port_handlers__[hdl_iter++](uc);
  }

  if (!(*uc->pargs->args[0])) {
    dev_id = (bf_dev_id_t)(*uc->pargs->args[0]);
    *uc->pargs->args = "-d0";
  } else {
    dev_id = strtoul(uc->pargs->args[0], NULL, 0);
  }

  while (hdl_iter < SHOW_TECH_PORT_UCLI_DEV_ID) {
    bf_drv_show_tech_ucli_port_handlers__[hdl_iter++](uc);
  }

  sts = bf_pm_num_max_ports_get(dev_id, &num_fp_ports);
  if (sts != BF_SUCCESS) {
    return 0;
  }
  /* Iterate through all ports for the device. */
  while (bf_drv_show_tech_ucli_port_handlers__[hdl_iter]) {
    for (ports_iter = 0; ports_iter < num_fp_ports; ports_iter++) {
      uc->pargs->args[1] = (char *)&ports_iter;
      aim_printf(&uc->pvs, "port ID : %d \n", ports_iter);
      bf_drv_show_tech_ucli_port_handlers__[hdl_iter](uc);
    }
    hdl_iter++;
  }
  return 0;
}

#else
void *port_mgr_ucli_node_create(void) { return NULL; }
#endif
