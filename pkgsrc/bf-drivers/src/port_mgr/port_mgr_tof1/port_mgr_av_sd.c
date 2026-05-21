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

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>  // for PRIx64

// for aim_printf
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

#include <dvm/bf_drv_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/bf_serdes_if.h>
#include <port_mgr/port_mgr_serdes_sbus_map.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof1_map.h"
#include "port_mgr_av_sd.h"
#include "port_mgr_av_sd_an.h"
#include "port_mgr_serdes.h"
#include <tofino_regs/tofino.h>
#include <avago/avago_aapl.h>

int log_spico_ints = 0;

/* Define this if SDS SBUS logging desired */
//#define SDS_SBUS_LOGGING
/* Define this if SDS SPICO_INT logging desired */
//#define SDS_SPICO_INT_LOGGING
#ifdef SDS_SBUS_LOGGING
//#define sds_sbus_log sds_sbus_log_debug
#define sds_sbus_log port_mgr_log
#else
#define sds_sbus_log(...)
#endif

#ifdef SDS_SPICO_INT_LOGGING
#define sds_spico_int_log sds_sbus_log_debug
#else
#define sds_spico_int_log(...)
#endif

#include <target-sys/bf_sal/bf_sys_intf.h>
bf_sys_mutex_t sd_access_mutex;
int sd_access_mutex_initd = 0;

bf_sys_rmutex_t sd_spico_int_mutex;
int sd_spico_int_mutex_initd = 0;

/*************************************************************
 * port_mgr_av_sd_encode_sbus_addr
 *
 * Encodes the dev_id, ring, and node-id into a single
 * uint32_t. This encoded value is what is passed to the avago
 * APIs. It gets decoded by the *_sbus_fn.
 *************************************************************/
uint32_t port_mgr_av_sd_encode_sbus_addr(bf_dev_id_t dev_id,
                                         int ring,
                                         int node) {
  uint32_t sbus_addr;

  sbus_addr = (((dev_id & 0xF) << 12) | ((ring & 0xF) << 8) | (node & 0xFF));

  return sbus_addr;
}

/*************************************************************
 * port_mgr_av_sd_decode_sbus_addr
 *
 * Decodes the dev_id, ring, and node-id from an encoded
 * sbus address.
 *************************************************************/
void port_mgr_av_sd_decode_sbus_addr(uint32_t sbus_addr,
                                     bf_dev_id_t *dev_id,
                                     int *ring,
                                     int *node) {
  *dev_id = ((sbus_addr >> 12) & 0xF);
  *ring = ((sbus_addr >> 8) & 0xF);
  *node = (sbus_addr & 0xFF);
}

/*************************************************************
 * port_mgr_av_sd_get_aapl
 *
 *************************************************************/
Aapl_t *port_mgr_av_sd_get_aapl(bf_dev_id_t dev_id) {
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  if (dev_p != NULL) {
    return (Aapl_t *)dev_p->aapl_hook;
  }
  return NULL;
}

/*************************************************************
 * port_mgr_av_sd_set_aapl
 *
 *************************************************************/
void port_mgr_av_sd_set_aapl(bf_dev_id_t dev_id) {
  // port_mgr_dev_t *dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  if (dev_p == NULL) {
    port_mgr_log("SDS: Map dev %d to dev_p failed", dev_id);
    return;
  }

  dev_p->aapl_hook = aapl_construct();
  if (dev_p->aapl_hook == NULL) {
    port_mgr_log("SDS: Construct an aapl struct for dev %d failed", dev_id);
    return;
  }
  ((Aapl_t *)(dev_p->aapl_hook))->chips = dev_id + 1;

  aapl_bind_set(dev_p->aapl_hook, (void *)(uintptr_t)dev_id);
}

/*************************************************************
 * port_mgr_av_sd_jtag_id_fn
 *
 *************************************************************/
uint32_t port_mgr_av_sd_jtag_id_fn(Aapl_t *aapl, uint32_t chip) {
#ifndef UTEST
  return 0x0995357f;  // Tofino jtag-id
#else
  return 0x0954857f;  // McKinley
#endif
  (void)aapl;
  (void)chip;
}

/*************************************************************
 * port_mgr_av_sd_identify_serdes_node_types
 *
 *************************************************************/
void port_mgr_av_sd_identify_serdes_node_types(bf_dev_id_t dev_id) {
  uint32_t n_rings, n_sd, ring, sd;

  n_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < n_rings; ring++) {
    n_sd = port_mgr_num_sbus_nodes_get(dev_id, ring);
    for (sd = 1; sd < n_sd; sd++) {
      port_mgr_sbus_ip_type_e ip_type;
      int inst, sub_inst;

      port_mgr_find_mac_info_for(dev_id, ring, sd, &ip_type, &inst, &sub_inst);
      if ((ip_type == IP_TYPE_ETH_PMA) || (ip_type == IP_TYPE_PCIE_PMA)) {
#ifdef AVAGO_EVAL_BOARD
        Aapl_t *aapl;

        aapl = port_mgr_av_sd_get_aapl(dev_id);
        uint32_t sbus_addr;
        sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
        if (!aapl_check_ip_type(
                aapl, sbus_addr, __func__, __LINE__, FALSE, 1, AVAGO_SERDES)) {
          sds_sbus_log("SDS: slice addr: %d : not a serdes on eval board <<<",
                       sd);
          continue;
        }
        if ((aapl->process_id[0] != AVAGO_TSMC_28) &&
            (aapl->process_id[0] != AVAGO_TSMC_16)) {
          continue;
        }
#endif
        port_mgr_serdes_set_is_serdes(dev_id, ring, sd);
      }
    }
  }
}

/*************************************************************
 * port_mgr_av_sd_set_initial_state
 *
 *************************************************************/
void port_mgr_av_sd_set_initial_state(bf_dev_id_t dev_id) {
  uint32_t n_rings, n_sd, ring, sd;

  n_rings = port_mgr_num_sbus_rings_get(dev_id);

  for (ring = 0; ring < n_rings; ring++) {
    n_sd = port_mgr_num_sbus_nodes_get(dev_id, ring);
    for (sd = 1; sd < n_sd; sd++) {
      int rc;

      if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) continue;

      // port_mgr_log("SDS: %d:%d:%d: Set initial state", dev_id, ring, sd);

      // set default DFE mode
      port_mgr_serdes_tof_dfe_cfg_default_set(dev_id, ring, sd);

      // set serdes in known (quiet) state (basically 10g
      // with tx_output disabled).
      rc = port_mgr_av_sd_init(dev_id,
                               ring,
                               sd,
                               TRUE, /*reset*/
                               AVAGO_CORE_DATA_ELB,
                               66 /*divider*/,
                               20 /*data_width*/,
                               TRUE /*phase_cal*/,
                               FALSE /*output_en*/);
      if (rc != 0) {
        port_mgr_log("SDS: %d:%d:%d: Error: %d : setting initial state",
                     dev_id,
                     ring,
                     sd,
                     rc);
      }
      // now turn off the PLLs
      rc = port_mgr_av_sd_set_rx_tx_and_tx_output_en(dev_id,
                                                     ring,
                                                     sd,
                                                     FALSE /*rx_en*/,
                                                     FALSE /*tx_en*/,
                                                     FALSE /*tx_output_en*/);
      if (rc != 0) {
        port_mgr_log(
            "SDS: %d:%d:%d: Error: %d : turning it off", dev_id, ring, sd, rc);
      }
    }
  }
}

/*************************************************************
 * port_mgr_av_sd_tx_loop_bandwidth_set
 *
 *************************************************************/
void port_mgr_av_sd_tx_loop_bandwidth_set(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd,
                                          uint32_t tx_pll_setting) {
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x80d2, tx_pll_setting);
}

/*************************************************************
 * port_mgr_av_sd_rx_loop_bandwidth_set
 *
 *************************************************************/
void port_mgr_av_sd_rx_loop_bandwidth_set(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd,
                                          uint32_t rx_pll_setting) {
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x8081, rx_pll_setting);
}

/*************************************************************
 * port_mgr_av_sd_tx_loop_bandwidth_get
 *
 *************************************************************/
void port_mgr_av_sd_tx_loop_bandwidth_get(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd,
                                          uint32_t *tx_pll_setting) {
  *tx_pll_setting = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x40d2, 0);
}

/*************************************************************
 * port_mgr_av_sd_rx_loop_bandwidth_get
 *
 *************************************************************/
void port_mgr_av_sd_rx_loop_bandwidth_get(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd,
                                          uint32_t *rx_pll_setting) {
  *rx_pll_setting = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4081, 0);
}

/*************************************************************
 * port_mgr_av_sd_pll_bbgain_set_these
 *
 *************************************************************/
void port_mgr_av_sd_pll_bbgain_set_these(bf_dev_id_t dev_id,
                                         int ring,
                                         int sd,
                                         uint32_t tx_pll_setting,
                                         uint32_t rx_pll_setting) {
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x80d2, tx_pll_setting);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x8081, rx_pll_setting);
}

/*************************************************************
 * port_mgr_av_sd_pll_bbgain_set
 *
 *************************************************************/
void port_mgr_av_sd_pll_bbgain_set(bf_dev_id_t dev_id,
                                   int ring,
                                   int sd,
                                   bool is_10g) {
  lld_err_t err;
  bf_sku_chip_part_rev_t rev_no;

  /* A0 (rev_no==0) requires custom bbgain settings
   *  B0 (rev_no==1) defaults recommended
   */
  err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
  if ((err == LLD_OK) && rev_no != 0) {
    // FIXME: revisit if we need the change bbgain!
    // port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x80d2, 0x511);
    // port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x8081, 0x404);
    return;  // use default
  }

  if (is_10g) {
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x80d2, 0x511);
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x8081, 0x404);
  } else {
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x80d2, 0x511);
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x8081, 0x404);
  }
}

/*************************************************************
 * port_mgr_av_sd_fw_ver_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_fw_ver_get(bf_dev_id_t dev_id, int ring, int sd) {
  return port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x0, 0x0);
}

/*************************************************************
 * port_mgr_av_sd_fw_build_id_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_fw_build_id_get(bf_dev_id_t dev_id, int ring, int sd) {
  return port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3f, 0x0);
}

/*************************************************************
 * port_mgr_av_sd_init_aapl
 *
 *************************************************************/
void port_mgr_av_sd_init_aapl(bf_dev_id_t dev_id, int tcp_mode) {
  Aapl_t *aapl;

  aapl = port_mgr_av_sd_get_aapl(dev_id);
  if (aapl == NULL) {
    port_mgr_av_sd_set_aapl(dev_id);
    aapl = port_mgr_av_sd_get_aapl(dev_id);
  }

  // try (really hard) to turn off aapl logging
  // aapl->enable_debug_logging = 0;
  // aapl->enable_stream_logging = 0;
  aapl->debug = 0;

#ifdef UTEST
  aapl->communication_method = AVAGO_OFFLINE;
#else
  aapl->communication_method = AVAGO_SBUS;
#endif

  aapl_register_jtag_idcode_fn(aapl, port_mgr_av_sd_jtag_id_fn);

  sds_sbus_log("SDS: Init AAPL <mode=%s>", tcp_mode ? "TCP" : "on-chip");

  if (tcp_mode) {
    aapl->aacs = 1;
    aapl->max_cmds_buffered = 0;

    aapl_connect(aapl, "10.201.201.15" /*<ip_addr>*/, 90);
    aapl->max_cmds_buffered = 0;
    sds_sbus_log("SDS: AACS server connected <<<");
  }

#ifndef AVAGO_EVAL_BOARD
  port_mgr_av_sd_access_fn_set(dev_id, BF_SDS_ACCESS_SBUS);
#endif

  aapl_get_ip_info(aapl, 0 /*reset*/);
  // aapl_print_struct(aapl, 1, 0xffff, 0);

  (void)tcp_mode;
}

/*************************************************************
 * port_mgr_av_sd_sbus_rd
 *
 *************************************************************/
uint32_t port_mgr_av_sd_sbus_rd(bf_dev_id_t dev_id,
                                int ring,
                                int sd,
                                uint32_t reg) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  return avago_sbus_rd(aapl, sbus_addr, reg);
}

/*************************************************************
 * port_mgr_av_sd_sbus_wr
 *
 *************************************************************/
void port_mgr_av_sd_sbus_wr(
    bf_dev_id_t dev_id, int ring, int sd, uint32_t reg, uint32_t data) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  avago_sbus_wr(aapl, sbus_addr, reg, data);
  return;
}

/*************************************************************
 * port_mgr_av_sd_spico_int
 *
 *************************************************************/
int port_mgr_av_sd_spico_int(
    bf_dev_id_t dev_id, int ring, int sd, int interrupt, uint32_t int_data) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  // filter out some common ones
  if ((interrupt != 0x4069) && (interrupt != 0x629)) {
    sds_spico_int_log("SDS: %d:%d:%d: Spico INT : 0x%04x : 0x%08x ..",
                      dev_id,
                      ring,
                      sd,
                      interrupt,
                      int_data);
  }

  rc = avago_spico_int(aapl, sbus_addr, interrupt, int_data);

  if ((interrupt != 0x4069) && (interrupt != 0x629)) {
    sds_spico_int_log("SDS: %d:%d:%d: Spico INT : 0x%04x : 0x%08x : rx=%08x",
                      dev_id,
                      ring,
                      sd,
                      interrupt,
                      int_data,
                      rc);
  }
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_set_ignore_bcast
 *
 *************************************************************/
void port_mgr_av_sd_ignore_bcast_set(bf_dev_id_t dev_id,
                                     int ring,
                                     int sd,
                                     int ignore) {
  uint32_t val;

  val = port_mgr_av_sd_sbus_rd(dev_id, ring, sd, 253);
  val &= ~(1 << 0);
  val |= (ignore ? 1 : 0);
  port_mgr_av_sd_sbus_wr(dev_id, ring, sd, 253, val);
}

/*************************************************************
 * port_mgr_av_sd_rmv_pcie_nodes_from_bcast_list
 *
 *************************************************************/
void port_mgr_av_sd_rmv_pcie_nodes_from_bcast_list(bf_dev_id_t dev_id,
                                                   int ring) {
  int sd, n, n_nodes, pcie_nodes[256];

  // get a list of the PCIe PMA nodes
  // and prevent them from listening to bcast's
  port_mgr_get_nodes_of_type(
      dev_id, ring, IP_TYPE_PCIE_PMA, pcie_nodes, &n_nodes);

  bf_sys_assert((n_nodes < 256));  // way too many pcie nodes, somethings wrong

  for (n = 0; n < n_nodes; n++) {
    sd = pcie_nodes[n];

    port_mgr_log("SDS: %d:%d:%d: Set ignore bcast", dev_id, ring, sd);
    port_mgr_av_sd_ignore_bcast_set(dev_id, ring, sd, 1);
  }
  // also remove mapped out nodes

  for (sd = (ring == 0) ? 11 : 1;
       sd < port_mgr_num_sbus_nodes_get(dev_id, ring);
       sd++) {
    if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
      port_mgr_av_sd_ignore_bcast_set(dev_id, ring, sd, 1);
    }
  }
}

/*************************************************************
 * port_mgr_av_sd_load_firmware
 *
 *************************************************************/
int port_mgr_av_sd_load_firmware(
    bf_dev_id_t dev_id, int ring, int sd, uint32_t fw_ver, char *fw_path) {
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int rc = 0;
  uint32_t fw_ver_loaded, expected_fw_ver = fw_ver;
  lld_err_t err = LLD_OK;
  bf_sku_chip_part_rev_t rev_no = 0;
  bool is_sw_model = false;
  bool fw_load_failed = false;

  bf_drv_device_type_get(dev_id, &is_sw_model);
  if (is_sw_model) {
    return LLD_OK;
  }

  /* quick sanity check to make sure we are loading the correct firmware
   * A0 (rev_no==0) fw_ver = 0x106f
   * B0 (rev_no==1) fw_ver = some other value
   */
  err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
  if ((err == LLD_OK) && rev_no != 0) {
    if (fw_ver == 0x106F) {
      port_mgr_log(
          "%d:%d:%3d: ERROR: Incorrect FW version to load on Tofino B0 part",
          dev_id,
          ring,
          sd);
      port_mgr_log(
          "%d:%d:%3d: ver=%04x : %s", dev_id, ring, sd, fw_ver, fw_path);
      bf_sys_assert(0);
    }
  }

  /* Disabe SBUS master timeout */
  uint32_t data;
  lld_read_register(dev_id, 0x400c4, &data);
  data &= ~0x1;
  lld_write_register(dev_id, 0x400c4, data);

  if (sd == AVAGO_BROADCAST) {
    int this_ring, this_sd;

    port_mgr_av_sd_rmv_pcie_nodes_from_bcast_list(dev_id, ring);

    port_mgr_log("SDS: %d:%d:%d: FW load: broadcast", dev_id, ring, sd);
    port_mgr_log("SDS:         : FW path: %s", fw_path);
    port_mgr_log("SDS:         : FW ver : %04x", fw_ver);
    avago_spico_upload_file(aapl, sbus_addr, TRUE, fw_path);

    // make sure they all got loaded correctly
    this_ring = ring;
    for (this_sd = 1; this_sd < port_mgr_num_sbus_nodes_get(dev_id, this_ring);
         this_sd++) {
      if (port_mgr_serdes_is_eth_serdes(dev_id, this_ring, this_sd)) {
        // get loaded firmware version
        fw_ver_loaded =
            port_mgr_av_sd_spico_int(dev_id, this_ring, this_sd, 0x0, 0x0);

        // make sure its what we expect
        if (fw_ver_loaded != expected_fw_ver) {
          port_mgr_log(
              "SDS: %d:%d:%d: FW load : bcast failed: try ucast: exp=%x : "
              "got=%x",
              dev_id,
              this_ring,
              this_sd,
              expected_fw_ver,
              fw_ver_loaded);

          // try unicast
          rc = port_mgr_av_sd_load_firmware(
              dev_id, this_ring, this_sd, fw_ver, fw_path);
          if (rc != 0) {
            port_mgr_log(
                "SDS: %d:%d:%d: FW load: ucast failed too: exp=%x : got=%x",
                dev_id,
                this_ring,
                this_sd,
                expected_fw_ver,
                fw_ver_loaded);
            rc = -1;
            fw_load_failed = true;
          }
        }
        // port_mgr_log("SDS: %d:%d:%d: FW rev=%x",
        //             dev_id,
        //             this_ring,
        //             this_sd,
        //             fw_ver_loaded);
      }
    }
  } else {  // load serdes firmware to a single slice
    if (!port_mgr_serdes_is_eth_serdes(dev_id, ring, sd)) {
      return 0;
    }
    port_mgr_log("SDS: %d:%d:%d: FW load: ucast", dev_id, ring, sd);
    port_mgr_log("SDS:         : FW path: %s", fw_path);
    port_mgr_log("SDS:         : FW ver : %04x", fw_ver);
    avago_spico_upload_file(aapl, sbus_addr, TRUE, fw_path);
    // make sure it worked
    fw_ver_loaded = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x0, 0x0);
    // make sure its what we expect
    if (fw_ver != expected_fw_ver) {
      port_mgr_log("SDS: %d:%d:%d: FW load : ucast failed: exp=%x : got=%x",
                   dev_id,
                   ring,
                   sd,
                   expected_fw_ver,
                   fw_ver_loaded);
      rc = -1;
    } else {
      rc = 0;
    }
  }
  if (fw_load_failed) {
    rc = -1;
  }
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_init
 *
 *************************************************************/
int port_mgr_av_sd_init(bf_dev_id_t dev_id,
                        int ring,
                        int sd,
                        int reset,
                        int init_mode,
                        int divider,
                        int data_width,
                        int phase_cal,
                        int output_en) {
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  Avago_serdes_init_config_t *cfg_p;
  int err, aerr;

  sds_sbus_log(
      "%d:%d:%d: serdes_init: rst=%d : mode=%d : div=%d : wdth=%d : cal=%d : "
      "en=%d",
      dev_id,
      ring,
      sd,
      reset,
      init_mode,
      divider,
      data_width,
      phase_cal,
      output_en);

  /* clear any prior error code */
  aapl_get_return_code(aapl);

  cfg_p = avago_serdes_init_config_construct(aapl);

  cfg_p->sbus_reset = reset;
  cfg_p->spico_reset = reset;
  cfg_p->init_tx = TRUE;
  cfg_p->init_rx = TRUE;
  cfg_p->init_mode = init_mode;
  cfg_p->tx_divider = divider;
  cfg_p->rx_divider = divider;
  cfg_p->tx_width = data_width;
  cfg_p->rx_width = data_width;
  cfg_p->tx_phase_cal = phase_cal;
  cfg_p->tx_output_en = output_en;
  cfg_p->signal_ok_en = FALSE;
  cfg_p->signal_ok_threshold = -1;

  err = avago_serdes_init(aapl, sbus_addr, cfg_p);

  avago_serdes_init_config_destruct(aapl, cfg_p);
  aerr = aapl_get_return_code(aapl);
  if (err | aerr) {
    sds_sbus_log("Serdes: %d:%d:%d: serdes_init: err=%d : aerr=%d",
                 dev_id,
                 ring,
                 sd,
                 err,
                 aerr);
  }
  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  return (err ? err : aerr);
}

/*************************************************************
 * port_mgr_av_sd_check_tx_pll_state
 *
 *************************************************************/
int port_mgr_av_sd_check_tx_pll_state(bf_dev_id_t dev_id,
                                      int ring,
                                      int sd,
                                      uint32_t expected_divider) {
  Avago_serdes_pll_state_t pll_state;
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  avago_serdes_get_tx_pll_state(aapl, sbus_addr, &pll_state);
  if (pll_state.divider != expected_divider) {
    sds_sbus_log(
        "Serdes: %d:%d:%d: Requested TX divider of %d was not achieved."
        "The TX PLL's divider is currently set to: %d",
        dev_id,
        ring,
        sd,
        expected_divider,
        pll_state.divider);
    return -1;
  }
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_check_rx_pll_state
 *
 *************************************************************/
int port_mgr_av_sd_check_rx_pll_state(bf_dev_id_t dev_id,
                                      int ring,
                                      int sd,
                                      uint32_t expected_divider) {
  Avago_serdes_pll_state_t pll_state;
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  avago_serdes_get_rx_pll_state(aapl, sbus_addr, &pll_state);
  if (pll_state.divider != expected_divider) {
    sds_sbus_log(
        "Serdes: %d:%d:%d: Requested RX divider of %d was not achieved."
        "The RX PLL's divider is currently set to: %d",
        dev_id,
        ring,
        sd,
        expected_divider,
        pll_state.divider);
    return -1;
  }
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_get_tx_output_en
 *
 *************************************************************/
int port_mgr_av_sd_get_tx_output_en(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int en;

  en = avago_serdes_get_tx_output_enable(aapl, sbus_addr);
  return en;
}

/*************************************************************
 * port_mgr_av_sd_set_tx_output_en
 *
 *************************************************************/
int port_mgr_av_sd_set_tx_output_en(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    int en) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int err;

  err = avago_serdes_set_tx_output_enable(aapl, sbus_addr, en);
  return err;
}

/*************************************************************
 * port_mgr_av_sd_set_rx_tx_and_tx_output_en
 *
 *************************************************************/
int port_mgr_av_sd_set_rx_tx_and_tx_output_en(bf_dev_id_t dev_id,
                                              int ring,
                                              int sd,
                                              int rx_en,
                                              int tx_en,
                                              int tx_output_en) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int err;

  err = avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, tx_en, rx_en, tx_output_en);
  return err;
}

/*************************************************************
 * port_mgr_av_sd_elec_idle_get
 *
 *************************************************************/
int port_mgr_av_sd_elec_idle_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int elec_idle;

  elec_idle = avago_serdes_get_electrical_idle(aapl, sbus_addr);
  return elec_idle ? 1 : 0;
}

/*************************************************************
 * port_mgr_av_sd_signal_ok_en_get
 *
 *************************************************************/
int port_mgr_av_sd_signal_ok_en_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int ok_en;

  ok_en = avago_serdes_get_signal_ok_enable(aapl, sbus_addr);
  return ok_en ? 1 : 0;
}

/*************************************************************
 * port_mgr_av_sd_signal_ok_thresh_get
 *
 *************************************************************/
int port_mgr_av_sd_signal_ok_thresh_get(bf_dev_id_t dev_id, int ring, int sd) {
  int thresh;
  lld_err_t err;
  bf_sku_chip_part_rev_t rev_no;

  /* A0 (rev_no==0) use aapl
   *  B0 (rev_no==1) aapl not updated to support B0 yet
   */
  err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
  if ((err == LLD_OK) && rev_no != 0) {
    thresh = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x40c4, 0);
  } else {
    Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
    int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

    thresh = avago_serdes_get_signal_ok_threshold(aapl, sbus_addr);
  }
  thresh = thresh & 0xff;
  return thresh;
}

/*************************************************************
 * port_mgr_av_sd_signal_ok_thresh_set
 *
 *************************************************************/
int port_mgr_av_sd_signal_ok_thresh_set(bf_dev_id_t dev_id,
                                        int ring,
                                        int sd,
                                        int thresh) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int err;

  err = avago_serdes_initialize_signal_ok(aapl, sbus_addr, thresh);
  return err;
}

/*************************************************************
 * port_mgr_av_sd_signal_ok_get
 *
 *************************************************************/
int port_mgr_av_sd_signal_ok_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int sig_ok;
  lld_err_t err;
  bf_sku_chip_part_rev_t rev_no;

  /* A0 (rev_no==0) use aapl
   *  B0 (rev_no==1) aapl not updated to support B0 yet
   */
  err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
  if ((err == LLD_OK) && rev_no != 0) {
    sig_ok = avago_serdes_get_signal_ok(aapl, sbus_addr, 0 /*no reset*/);
  } else {
    /* want current status, not latched */
    sig_ok = avago_serdes_get_signal_ok_live(aapl, sbus_addr);
  }
  return sig_ok ? 1 : 0;
}

/*************************************************************
 * port_mgr_av_sd_signal_ok_live_get
 *
 *************************************************************/
int port_mgr_av_sd_signal_ok_live_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int sig_ok;

  /* want current status, not latched */
  sig_ok = avago_serdes_get_signal_ok_live(aapl, sbus_addr);
  return sig_ok ? 1 : 0;
}

/*************************************************************
 * port_mgr_av_sd_los_get
 *
 *************************************************************/
int port_mgr_av_sd_los_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int sig_ok;

  /* want latched status, not current */
  sig_ok = avago_serdes_get_signal_ok(aapl, sbus_addr, 1 /*reset sticky*/);
  return !sig_ok ? 1 : 0;
}

/*************************************************************
 * port_mgr_av_sd_frequency_lock_get
 *
 *************************************************************/
int port_mgr_av_sd_frequency_lock_get(bf_dev_id_t dev_id, int ring, int sd) {
  int flock;

  flock = (port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x401c, 0x0) >> 15) & 1;
  return flock;
}

/*************************************************************
 * port_mgr_av_sd_calibration_status_get
 *
 *************************************************************/
int port_mgr_av_sd_calibration_status_get(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd) {
  int cal_sts;

  cal_sts = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x12e, 0x0);
  return cal_sts;
}

/*************************************************************
 * port_mgr_av_sd_get_error_count
 *
 *************************************************************/
uint32_t port_mgr_av_sd_get_error_count(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t err_cnt;

  err_cnt = avago_serdes_get_errors(aapl, sbus_addr, AVAGO_LSB_DIRECT, TRUE);
  return err_cnt;
}

/*************************************************************
 * port_mgr_av_sd_start_dfe
 *
 * Note: Following are the "default" values returned by the
 *       constructor. To use the defaults pass -1 or the value
 *       of the default, or use one of the simpler APIs,
 *
 *       port_mgr_av_sd_start_dfe_ical
 *       port_mgr_av_sd_start_dfe_pcal
 *       port_mgr_av_sd_start_dfe_adaptive
 *
 *************************************************************
 * Important:
 *       Modify from defaults only if you really know what you
 *       are doing!
 *************************************************************
 *
 *    fixed_dc = 0;
 *    fixed_lf = 0;
 *    fixed_hf = 0;
 *    dfe_disable = 0;
 *    tune_mode=AVAGO_DFE_ICAL;
 *
 *    dc = 0x38;  56
 *    lf = 0x0C;  12
 *    hf = 0x00;   0
 *    bw = 0x0F;  15
 *
 *    dwell_bits      = 0x00010001;  BER level ~3e-6
 *    error_threshold = 4;
 *
 *    dfeGAIN_min = 0;
 *    dfeGAIN_max = 15;
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe(bf_dev_id_t dev_id,
                              int ring,
                              int sd,
                              int tune_mode,
                              int fixed_dc,
                              int fixed_lf,
                              int fixed_hf,
                              int dfe_disable,
                              int dc,
                              int lf,
                              int hf,
                              int bw,
                              int dwell_bits,
                              int error_threshold,
                              int dfeGAIN_min,
                              int dfeGAIN_max,
                              int dfe_tap_disable[AVAGO_DFE_TAP_COUNT + 1]) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *mode_control;

  mode_control = avago_serdes_dfe_state_construct(aapl);

  mode_control->tune_mode = tune_mode;

  if (fixed_dc != -1) mode_control->fixed_dc = fixed_dc;
  if (fixed_lf != -1) mode_control->fixed_lf = fixed_lf;
  if (fixed_hf != -1) mode_control->fixed_hf = fixed_hf;
  if (dfe_disable != -1) mode_control->dfe_disable = dfe_disable;
  if (dc != -1) mode_control->dc = dc;
  if (lf != -1) mode_control->lf = lf;
  if (hf != -1) mode_control->hf = hf;
  if (bw != -1) mode_control->bw = bw;
  if (dwell_bits != -1) mode_control->dwell_bits = dwell_bits;
  if (error_threshold != -1) mode_control->error_threshold = error_threshold;
  if (dfeGAIN_min != -1) mode_control->dfeGAIN_min = dfeGAIN_min;
  if (dfeGAIN_max != -1) mode_control->dfeGAIN_max = dfeGAIN_max;

  if (dfe_tap_disable && (dfe_tap_disable != (int *)-1)) {
    int tap;

    for (tap = 0; tap < AVAGO_DFE_TAP_COUNT + 1; tap++) {
      if (dfe_tap_disable[tap] && (dfe_tap_disable[tap] != -1)) {
        dfe_tap_disable[tap] = TRUE;
      }
    }
  }
  avago_serdes_dfe_tune(aapl, sbus_addr, mode_control);

  avago_serdes_dfe_state_destruct(aapl, mode_control);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_w_pcal
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_w_pcal(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);
  // launch ICAL
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x01);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal(bf_dev_id_t dev_id,
                                           int ring,
                                           int sd) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);
  // launch ICAL
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x01);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_hf
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_hf(bf_dev_id_t dev_id,
                                                    int ring,
                                                    int sd,
                                                    int hf) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);

  // fix HF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2000 | (hf & 0xF));
  // run ICAL with fixed HF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x201);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_lf
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_lf(bf_dev_id_t dev_id,
                                                    int ring,
                                                    int sd,
                                                    int lf) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);

  // fix LF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2100 | (lf & 0xF));
  // run ICAL with fixed LF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x101);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_hf_and_lf
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_hf_and_lf(
    bf_dev_id_t dev_id, int ring, int sd, int hf, int lf) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);

  // fix HF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2000 | (hf & 0xF));

  // fix LF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2100 | (lf & 0xF));
  // run ICAL with fixed HF and LF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x301);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_dc
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_dc(bf_dev_id_t dev_id,
                                                    int ring,
                                                    int sd,
                                                    int dc) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);

  // fix DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2200 | (dc & 0xFF));

  // run ICAL with fixed DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x81);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal_seeded_dc
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal_seeded_dc(bf_dev_id_t dev_id,
                                                     int ring,
                                                     int sd,
                                                     int dc) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);

  // set DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2200 | (dc & 0xFF));

  // seed DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x200 | (dc & 0xFF));

  // run ICAL with seeded DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x801);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_hf_seeded_dc
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical_no_pcal_fixed_hf_seeded_dc(
    bf_dev_id_t dev_id, int ring, int sd, int hf, int dc) {
  // enforce rate-limit on ICALs
  bf_sys_usleep(5000);

  // force sig_ok_en false
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

  // disable "auto-pcal"
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);

  // set DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2200 | (dc & 0xFF));

  // seed DC
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x200 | (dc & 0xFF));

  // fix HF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2000 | (hf & 0xF));

  // run ICAL with fixed HF
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x201 | 0x800);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_ical
 *
 * Default DFE function. Used by bring-up FSM as well as serdes
 * diagnostics.
 * Uses the DFE mode specified for the given serdes slice.
 * The default is ICAL only. This can be overriden via an API
 * (bf_serdes_dfe_config_set).
 *************************************************************/
void port_mgr_av_sd_start_dfe_ical(bf_dev_id_t dev_id, int ring, int sd) {
  uint32_t dfe_ctrl;
  uint32_t hf_val;
  uint32_t lf_val;
  uint32_t dc_val;
  bf_status_t rc;

  rc = port_mgr_serdes_tof_dfe_cfg_get(
      dev_id, ring, sd, &dfe_ctrl, &hf_val, &lf_val, &dc_val);
  if (rc == BF_SUCCESS) {
    bf_sds_tof_dfe_ctrl_t ctrl = (bf_sds_tof_dfe_ctrl_t)dfe_ctrl;
    uint32_t int_0xA_val = 0;

    // enforce rate-limit on ICALs
    bf_sys_usleep(5000);

    // force sig_ok_en false
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x20, 0);

    if (ctrl == BF_SDS_TOF_DFE_CTRL_DEFAULT) {
      port_mgr_av_sd_start_dfe_ical_no_pcal(dev_id, ring, sd);
      return;
    } else if (ctrl == BF_SDS_TOF_DFE_CTRL_ICAL) {
      port_mgr_av_sd_start_dfe_ical_no_pcal(dev_id, ring, sd);
      return;
    }

    // orthogonal options
    if ((ctrl & BF_SDS_TOF_DFE_CTRL_PCAL) == 0) {
      // disable "auto-pcal"
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b01);
    } else {
      // enable "auto-pcal"
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5b00);
    }

    if (ctrl & BF_SDS_TOF_DFE_CTRL_SEEDED_HF) {
      // set HF
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2000 | (hf_val & 0xF));
      // seed HF
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x0000 | (hf_val & 0xF));
      // run ICAL with seeded HF
      int_0xA_val |= 0x2001;
    }
    if (ctrl & BF_SDS_TOF_DFE_CTRL_SEEDED_LF) {
      // set LF
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2100 | (lf_val & 0xF));
      // seed LF
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x0100 | (lf_val & 0xF));
      // run ICAL with seeded LF
      int_0xA_val |= 0x1001;
    }
    if (ctrl & BF_SDS_TOF_DFE_CTRL_SEEDED_DC) {
      // set DC
      port_mgr_av_sd_spico_int(
          dev_id, ring, sd, 0x26, 0x2200 | (dc_val & 0xFF));
      // seed DC
      port_mgr_av_sd_spico_int(
          dev_id, ring, sd, 0x26, 0x0200 | (dc_val & 0xFF));
      // run ICAL with seeded DC
      int_0xA_val |= 0x801;
    }
    if (ctrl & BF_SDS_TOF_DFE_CTRL_FIXED_HF) {
      // fix HF
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2000 | (hf_val & 0xF));
      // run ICAL with fixed HF
      int_0xA_val |= 0x201;
    }
    if (ctrl & BF_SDS_TOF_DFE_CTRL_FIXED_LF) {
      // fix LF
      port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x2100 | (lf_val & 0xF));
      // run ICAL with fixed LF
      int_0xA_val |= 0x101;
    }
    if (ctrl & BF_SDS_TOF_DFE_CTRL_FIXED_DC) {
      // fix DC
      port_mgr_av_sd_spico_int(
          dev_id, ring, sd, 0x26, 0x2200 | (dc_val & 0xFF));
      // run ICAL with fixed DC
      int_0xA_val |= 0x81;
    }
    // run ICAL with combined options
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, int_0xA_val);
  }
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_pcal
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_pcal(bf_dev_id_t dev_id, int ring, int sd) {
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x2);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_pi_cal
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_pi_cal(bf_dev_id_t dev_id, int ring, int sd) {
  uint32_t pi_current;

  pi_current = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2F, 0xF0);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2F, 0x101 | pi_current);

  // #continuous pCal with no DCR update
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x86);
}

/*************************************************************
 * port_mgr_av_sd_start_dfe_adaptive
 *
 *************************************************************/
void port_mgr_av_sd_start_dfe_adaptive(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *mode_control;

  mode_control = avago_serdes_dfe_state_construct(aapl);
  mode_control->tune_mode = AVAGO_DFE_START_ADAPTIVE;

  avago_serdes_dfe_tune(aapl, sbus_addr, mode_control);
  avago_serdes_dfe_state_destruct(aapl, mode_control);
}

/*************************************************************
 * port_mgr_av_sd_stop_dfe_adaptive
 *
 *************************************************************/
void port_mgr_av_sd_stop_dfe_adaptive(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *mode_control;

  mode_control = avago_serdes_dfe_state_construct(aapl);
  mode_control->tune_mode = AVAGO_DFE_STOP_ADAPTIVE;

  avago_serdes_dfe_tune(aapl, sbus_addr, mode_control);
  avago_serdes_dfe_state_destruct(aapl, mode_control);
}

/*************************************************************
 * port_mgr_av_sd_stop_dfe
 *
 *************************************************************/
void port_mgr_av_sd_stop_dfe(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *mode_control;

  mode_control = avago_serdes_dfe_state_construct(aapl);
  mode_control->dfe_disable = TRUE;

  avago_serdes_dfe_tune(aapl, sbus_addr, mode_control);
  avago_serdes_dfe_state_destruct(aapl, mode_control);
}

/*************************************************************
 * port_mgr_av_sd_check_dfe_running
 *
 *************************************************************/
int port_mgr_av_sd_check_dfe_running(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int running;

  running = avago_serdes_dfe_running(aapl, sbus_addr);
  return running;
}

/*************************************************************
 * port_mgr_av_sd_log_dfe_st
 *
 *************************************************************/
int port_mgr_av_sd_log_dfe_st(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *dfe_st;
  uint32_t eye_metric;

  dfe_st = avago_serdes_dfe_state_construct(aapl);

  avago_serdes_get_dfe_state(aapl, sbus_addr, dfe_st);
  eye_metric = avago_serdes_eye_get_simple_metric(aapl, sbus_addr);
  (void)eye_metric;

  sds_sbus_log("Serdes: %d : %d : %d : state=%x : status=%x :",
               dev_id,
               ring,
               sd,
               dfe_st->state,
               dfe_st->status);
  sds_sbus_log("Serdes: %d : %d : %d : %s %s %s %s %s %s <eye: %d>",
               dev_id,
               ring,
               sd,
               dfe_st->status & 0x80 ? "inpOfsCorrCmplt : " : "",
               dfe_st->status & 0x40 ? "AdptvPcalEn : " : "",
               dfe_st->status & 0x20 ? "RunPcal : " : "",
               dfe_st->status & 0x10 ? "RunIcal : " : "",
               dfe_st->status & 0x02 ? "PcalRng : " : "",
               dfe_st->status & 0x01 ? "IcalRng : " : "",
               eye_metric);
  if ((dfe_st->state != 0xf) || (dfe_st->status != 0x80)) {
    sds_sbus_log("Serdes: %d : %d : %d : DFE failed (or not complete)",
                 dev_id,
                 ring,
                 sd);
  }
  sds_sbus_log("Serdes: %d : %d : %d : Eye : %d", dev_id, ring, sd, eye_metric);
  avago_serdes_dfe_state_destruct(aapl, dfe_st);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dump_eye
 *
 *************************************************************/
int port_mgr_av_sd_dump_eye(
    bf_dev_id_t dev_id, int ring, int sd, int plot, ucli_context_t *uc) {
  Avago_serdes_eye_config_t *eye_config;
  Avago_serdes_eye_data_t *eye_data;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  eye_config = avago_serdes_eye_config_construct(aapl);
  eye_data = avago_serdes_eye_data_construct(aapl);

  eye_config->ec_real_time_plot = TRUE;  // test

  // test
  // eye_config->ec_max_dwell_bits = (bigint)1e10;

  /* get full resolution available */
  if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
    sds_sbus_log("**** Eye capture failed");
    avago_serdes_eye_data_destruct(aapl, eye_data);
    avago_serdes_eye_config_destruct(aapl, eye_config);
    return 0;
  }

  if (eye_data->ed_hbtc.left_points == 0 ||
      eye_data->ed_hbtc.right_points == 0) {
    if (eye_data->ed_hbtc.data_row != ~0U) {
      sds_sbus_log("hbtc data insufficient(1). No eye");
    }
    return -1;
  }

  if (eye_data->ed_hbtc.left_R_squared < 0.95 ||
      eye_data->ed_hbtc.right_R_squared < 0.95 ||
      eye_data->ed_hbtc.left_slope <= 0.0 ||
      eye_data->ed_hbtc.right_slope >= 0.0) {
    sds_sbus_log("hbtc data insufficient(2). Bad eye");
  }

  if (eye_data->ed_vbtc.top_points == 0 ||
      eye_data->ed_vbtc.bottom_points == 0) {
    if (eye_data->ed_vbtc.data_column >= -1) {
      sds_sbus_log("vbtc data insufficient(3). No eye");
    }
    return -2;
  }
  if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
      eye_data->ed_vbtc.top_R_squared < 0.95 ||
      eye_data->ed_vbtc.bottom_slope <= 0.0 ||
      eye_data->ed_vbtc.top_slope >= 0.0) {
    sds_sbus_log("hbtc data insufficient(2). Bad eye");
  }

  /* Print results: */
  if (uc) {
    char *hbuf = avago_serdes_eye_hbtc_format(&eye_data->ed_hbtc);
    char *vbuf = avago_serdes_eye_vbtc_format(&eye_data->ed_vbtc);
    if (hbuf) {
      aim_printf(&uc->pvs, "%s \n", hbuf);
      AAPL_FREE(hbuf);
    }
    if (vbuf) {
      aim_printf(&uc->pvs, "%s \n", vbuf);
      AAPL_FREE(vbuf);
    }
  } else {
    avago_serdes_eye_hbtc_write(stdout, &eye_data->ed_hbtc);
    avago_serdes_eye_vbtc_write(stdout, &eye_data->ed_vbtc);
  }

  if (plot) {
    char *eye_text = avago_serdes_eye_plot_format(eye_data);
    if (eye_text) {
      if (uc) {
        aim_printf(&uc->pvs, "%s\n", eye_text);
      } else {
        printf("%s\n", eye_text);
      }
      AAPL_FREE(eye_text);
    }
  }
  avago_serdes_eye_data_destruct(aapl, eye_data);
  avago_serdes_eye_config_destruct(aapl, eye_config);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dump_vbtc
 *
 *************************************************************/
int port_mgr_av_sd_dump_vbtc(bf_dev_id_t dev_id, int ring, int sd) {
  Avago_serdes_eye_config_t *eye_config;
  Avago_serdes_eye_data_t *eye_data;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  eye_config = avago_serdes_eye_config_construct(aapl);
  eye_data = avago_serdes_eye_data_construct(aapl);

  eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
  eye_config->ec_no_sbm = TRUE;
  //    eye_config->ec_x_resolution = 80;
  // eye_config->ec_real_time_plot = TRUE;  // test

  /* get full resolution available */
  if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
    sds_sbus_log("**** Eye capture failed");
    avago_serdes_eye_data_destruct(aapl, eye_data);
    avago_serdes_eye_config_destruct(aapl, eye_config);
    return -1;
  }

  if (eye_data->ed_vbtc.top_points == 0 ||
      eye_data->ed_vbtc.bottom_points == 0) {
    if (eye_data->ed_vbtc.data_column >= -1) {
      sds_sbus_log("vbtc data insufficient(3). No eye");
    }
    return -2;
  }
  if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
      eye_data->ed_vbtc.top_R_squared < 0.95 ||
      eye_data->ed_vbtc.bottom_slope <= 0.0 ||
      eye_data->ed_vbtc.top_slope >= 0.0) {
    sds_sbus_log("hbtc data insufficient(2). Bad eye");
  }

  /* Print results: */
  avago_serdes_eye_vbtc_write(stdout, &eye_data->ed_vbtc);

  avago_serdes_eye_data_destruct(aapl, eye_data);
  avago_serdes_eye_config_destruct(aapl, eye_config);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_tx_invert_get
 *
 *************************************************************/
int port_mgr_av_sd_tx_invert_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int polarity;

  polarity = avago_serdes_get_tx_invert(aapl, sbus_addr);
  return polarity;
}

/*************************************************************
 * port_mgr_av_sd_tx_invert_set
 *
 *************************************************************/
int port_mgr_av_sd_tx_invert_set(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 int inv) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_set_tx_invert(aapl, sbus_addr, inv ? TRUE : FALSE);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_invert_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_invert_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int polarity;

  polarity = avago_serdes_get_rx_invert(aapl, sbus_addr);

  return polarity;
}

/*************************************************************
 * port_mgr_av_sd_rx_invert_set
 *
 *************************************************************/
int port_mgr_av_sd_rx_invert_set(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 int inv) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_set_rx_invert(aapl, sbus_addr, inv ? TRUE : FALSE);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_tx_error_inject_set
 *
 *************************************************************/
int port_mgr_av_sd_tx_error_inject_set(bf_dev_id_t dev_id,
                                       int ring,
                                       int sd,
                                       int num_bits) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_tx_inject_error(aapl, sbus_addr, num_bits);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_error_inject_set
 *
 *************************************************************/
int port_mgr_av_sd_rx_error_inject_set(bf_dev_id_t dev_id,
                                       int ring,
                                       int sd,
                                       int num_bits) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_rx_inject_error(aapl, sbus_addr, num_bits);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_tx_data_sel_get
 *
 *************************************************************/
int port_mgr_av_sd_tx_data_sel_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int av_patsel, rx_patsel;

  av_patsel = avago_serdes_get_tx_data_sel(aapl, sbus_addr);
  switch (av_patsel) {
    case AVAGO_SERDES_TX_DATA_SEL_PRBS7:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS7;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS9:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS9;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS11:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS11;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS15:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS15;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS23:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS23;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS31:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS31;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_USER:
      rx_patsel = BF_SDS_PAT_PATSEL_FIXED;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_CORE:
      rx_patsel = BF_SDS_PAT_PATSEL_OFF;
      break;
    default:
      return BF_INVALID_ARG;
  }
  return rx_patsel;
}

/*************************************************************
 * port_mgr_av_sd_tx_data_sel_set
 *
 *************************************************************/
int port_mgr_av_sd_tx_data_sel_set(bf_dev_id_t dev_id,
                                   int ring,
                                   int sd,
                                   int tx_patsel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  int av_patsel;

  switch (tx_patsel) {
    case BF_SDS_PAT_PATSEL_PRBS7:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS7;
      break;
    case BF_SDS_PAT_PATSEL_PRBS9:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS9;
      break;
    case BF_SDS_PAT_PATSEL_PRBS11:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS11;
      break;
    case BF_SDS_PAT_PATSEL_PRBS15:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS15;
      break;
    case BF_SDS_PAT_PATSEL_PRBS23:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS23;
      break;
    case BF_SDS_PAT_PATSEL_PRBS31:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS31;
      break;
    case BF_SDS_PAT_PATSEL_FIXED:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_USER;
      break;
    case BF_SDS_PAT_PATSEL_OFF:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_CORE;
      break;
    default:
      return BF_INVALID_ARG;
  }

  rc = avago_serdes_set_tx_data_sel(aapl, sbus_addr, av_patsel);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_get_rx_cmp_sel
 *
 *************************************************************/
int port_mgr_av_sd_get_rx_cmp_sel(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int sel;

  sel = avago_serdes_get_rx_cmp_data(aapl, sbus_addr);
  return sel;
}

/*************************************************************
 * port_mgr_av_sd_set_rx_cmp_sel
 *
 *************************************************************/
int port_mgr_av_sd_set_rx_cmp_sel(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  int sel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_set_rx_cmp_data(aapl, sbus_addr, sel);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_get_rx_cmp_mode
 *
 *************************************************************/
int port_mgr_av_sd_get_rx_cmp_mode(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int mode;

  mode = avago_serdes_get_rx_cmp_mode(aapl, sbus_addr);
  return mode;
}

/*************************************************************
 * port_mgr_av_sd_set_rx_cmp_mode
 *
 *************************************************************/
int port_mgr_av_sd_set_rx_cmp_mode(bf_dev_id_t dev_id,
                                   int ring,
                                   int sd,
                                   int mode) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_set_rx_cmp_mode(aapl, sbus_addr, mode);
  return rc;
}

#if 0
/*************************************************************
* port_mgr_av_sd_get_rx_data_qual
*
*************************************************************/
Avago_serdes_rx_data_qual_t port_mgr_av_sd_get_rx_data_qual(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_rx_data_qual_t qual;

  qual = avago_serdes_get_rx_data_qual(aapl, sbus_addr);
  return qual;
}

/*************************************************************
* port_mgr_av_sd_set_rx_data_qual
*
*************************************************************/
int port_mgr_av_sd_set_rx_data_qual(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    Avago_serdes_rx_data_qual_t qual) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_set_rx_data_qual(aapl, sbus_addr, qual);
  return rc;
}
#endif  // 0

/*************************************************************
 * port_mgr_av_sd_rx_term_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_term_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int term, av_term;

  av_term = avago_serdes_get_rx_term(aapl, sbus_addr);

  switch (av_term) {
    case AVAGO_SERDES_RX_TERM_AGND:
      term = BF_SDS_RX_TERM_GND;
      break;
    case AVAGO_SERDES_RX_TERM_AVDD:
      term = BF_SDS_RX_TERM_AVDD;
      break;
    case AVAGO_SERDES_RX_TERM_FLOAT:
      term = BF_SDS_RX_TERM_FLOAT;
      break;
    default:
      term = -1;
  }
  return term;
}

/*************************************************************
 * port_mgr_av_sd_rx_term_set
 *
 *************************************************************/
int port_mgr_av_sd_rx_term_set(bf_dev_id_t dev_id, int ring, int sd, int term) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc, rx_term;

  switch (term) {
    case BF_SDS_RX_TERM_GND:
      rx_term = AVAGO_SERDES_RX_TERM_AGND;
      break;
    case BF_SDS_RX_TERM_AVDD:
      rx_term = AVAGO_SERDES_RX_TERM_AVDD;
      break;
    case BF_SDS_RX_TERM_FLOAT:
      rx_term = AVAGO_SERDES_RX_TERM_FLOAT;
      break;
    default:
      return BF_INVALID_ARG;
  }

  rc = avago_serdes_set_rx_term(aapl, sbus_addr, rx_term);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_tx_pll_clk_source_get
 *
 *************************************************************/
int port_mgr_av_sd_tx_pll_clk_source_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int av_clk;
  int clk;

  av_clk = avago_serdes_get_tx_pll_clk_src(aapl, sbus_addr);

  switch (av_clk) {
    case AVAGO_SERDES_TX_PLL_REFCLK:
      clk = BF_SDS_TX_PLL_ETH_REFCLK;  // could also be ALT_REFCLK
      break;
    case AVAGO_SERDES_TX_PLL_RX_DIVX:
      clk = BF_SDS_TX_PLL_RXCLK;
      break;
    case AVAGO_SERDES_TX_PLL_OFF:
      clk = BF_SDS_TX_PLL_OFF;
      break;
    case AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK:
      clk = BF_SDS_TX_PLL_PCIECLK;
      break;
    default:
      clk = -1;
      break;
  }
  return clk;
}

/*************************************************************
 * port_mgr_av_sd_tx_pll_clk_source_set
 *
 *************************************************************/
int port_mgr_av_sd_tx_pll_clk_source_set(bf_dev_id_t dev_id,
                                         int ring,
                                         int sd,
                                         int clk) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  Avago_serdes_tx_pll_clk_t av_clk;

  switch (clk) {
    case BF_SDS_TX_PLL_ETH_REFCLK:
    case BF_SDS_TX_PLL_ALT_REFCLK:
      av_clk = AVAGO_SERDES_TX_PLL_REFCLK;
      break;
    case BF_SDS_TX_PLL_RXCLK:
      av_clk = AVAGO_SERDES_TX_PLL_RX_DIVX;
      break;
    case BF_SDS_TX_PLL_OFF:
      av_clk = AVAGO_SERDES_TX_PLL_OFF;
      break;
    case BF_SDS_TX_PLL_PCIECLK:
      av_clk = AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK;
      break;
    default:
      return BF_INVALID_ARG;
  }

  sds_sbus_log("%d:%d:%d: Tx PLL CLK set: %s",
               dev_id,
               ring,
               sd,
               (clk == BF_SDS_TX_PLL_ETH_REFCLK)
                   ? "TX_PLL_ETH_REFCLK"
                   : (clk == BF_SDS_TX_PLL_ALT_REFCLK)
                         ? "TX_PLL_ALT_REFCLK"
                         : (clk == BF_SDS_TX_PLL_RXCLK)
                               ? "TX_PLL_RXCLK"
                               : (clk == BF_SDS_TX_PLL_OFF)
                                     ? "TX_PLL_OFF"
                                     : (clk == BF_SDS_TX_PLL_PCIECLK)
                                           ? "TX_PLL_PCIECLK"
                                           : "invalid");

  rc = avago_serdes_set_tx_pll_clk_src(aapl, sbus_addr, av_clk);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_spico_clk_source_get
 *
 *************************************************************/
int port_mgr_av_sd_spico_clk_source_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int av_clk;
  int clk;

  av_clk = avago_serdes_get_spico_clk_src(aapl, sbus_addr);
  switch (av_clk) {
    case AVAGO_SERDES_SPICO_REFCLK:
      clk = BF_SDS_MGMT_CLK_REFCLK;
      break;
    case AVAGO_SERDES_SPICO_REFCLK_DIV2:
      clk = BF_SDS_MGMT_CLK_REFCLK_DIV2;
      break;
    case AVAGO_SERDES_SPICO_PCIE_CORE_CLK:
      clk = BF_SDS_MGMT_CLK_PCIECLK;
      break;
    case AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2:
      clk = BF_SDS_MGMT_CLK_PCIECLK_DIV2;
      break;
    default:
      clk = -1;
      break;
  }
  return clk;
}

/*************************************************************
 * port_mgr_av_sd_spico_clk_source_set
 *
 *************************************************************/
int port_mgr_av_sd_spico_clk_source_set(bf_dev_id_t dev_id,
                                        int ring,
                                        int sd,
                                        int clk) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  Avago_serdes_spico_clk_t av_clk;

  switch (clk) {
    case BF_SDS_MGMT_CLK_REFCLK:
      av_clk = AVAGO_SERDES_SPICO_REFCLK; /**< Source from external REFCLK */
      break;
    case BF_SDS_MGMT_CLK_REFCLK_DIV2:
      av_clk = AVAGO_SERDES_SPICO_REFCLK_DIV2; /**< Debug Only. REFCLK/2 */
      break;
    case BF_SDS_MGMT_CLK_PCIECLK:
      av_clk =
          AVAGO_SERDES_SPICO_PCIE_CORE_CLK; /**< Source from external PCIE CLK
                                             */
      break;
    case BF_SDS_MGMT_CLK_PCIECLK_DIV2:
      av_clk =
          AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2; /**< Debug Only. PCIE CLK/2 */
      break;
    default:
      return BF_INVALID_ARG;
  }

  rc = avago_serdes_set_spico_clk_src(aapl, sbus_addr, av_clk);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_init_serdes
 *
 *************************************************************/
int port_mgr_av_sd_init_serdes(bf_dev_id_t dev_id,
                               int ring,
                               int sd,
                               bf_sds_line_rate_mode_t line_rate,
                               bool init_rx,
                               bool init_tx,
                               bool tx_drv_en,
                               bool phase_cal) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, false, false, false); /* Disable serdes */

  switch (line_rate) {
    case BF_SDS_LINE_RATE_1p25G:
      avago_spico_int(
          aapl, sbus_addr, 0x05, 0x9008); /* set serdes bit/ref ratio = 1.25G */
      avago_serdes_set_tx_rx_width(aapl, sbus_addr, 10, 10); /* 10bit width */
      break;
    case BF_SDS_LINE_RATE_10G:
      avago_spico_int(
          aapl, sbus_addr, 0x05, 0x8042); /* set serdes bit/ref ratio = 10G */
      avago_serdes_set_tx_rx_width(aapl, sbus_addr, 20, 20); /* 20bit width */
      break;
    case BF_SDS_LINE_RATE_25G:
      avago_spico_int(
          aapl, sbus_addr, 0x05, 0x80A5); /* set serdes bit/ref ratio = 25G */
      avago_serdes_set_tx_rx_width(aapl, sbus_addr, 40, 40); /* 40bit width */
      break;
    default:
    case BF_SDS_LINE_RATE_2p5G:
      return -1;
      break;  // ??
  }
  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, init_tx, init_rx, tx_drv_en); /* Enable Tx and Rx */
  (void)phase_cal;
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_param_set
 *
 *************************************************************/
int port_mgr_av_sd_dfe_param_set(
    bf_dev_id_t dev_id, int ring, int sd, int row, int col, int value) {
  uint32_t data;

  data = ((row & 0xF) << 8) | ((col & 0x7) << 12) |
         (((value < 0) ? 1 : 0) << 15) |
         (((value < 0) ? -value : value) & 0xFF);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_param_get
 *
 *************************************************************/
int port_mgr_av_sd_dfe_param_get(
    bf_dev_id_t dev_id, int ring, int sd, int row, int col, int *value) {
  uint32_t data, int_val, av_val;

  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  int_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);

  av_val = int_val & 0xFF;    // [7:0]
  if (int_val & (1 << 15)) {  // value is negative
    *value = -av_val;
  } else {
    *value = av_val;
  }
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_get_tx_eq_limits
 *
 *************************************************************/
int port_mgr_av_sd_get_tx_eq_limits(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    int *pre_min,
                                    int *atten_min,
                                    int *post_min,
                                    int *pre_max,
                                    int *atten_max,
                                    int *post_max) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_tx_eq_limits_t limits;
  int rc;

  if ((!pre_min) || (!pre_max) || (!atten_min) || (!atten_max) || (!post_min) ||
      (!post_max))
    return -1;

  memset((char *)&limits, 0, sizeof(limits));
  rc = avago_serdes_get_tx_eq_limits(aapl, sbus_addr, &limits);
  *pre_min = limits.pre_min;
  *pre_max = limits.pre_max;
  *atten_min = limits.atten_min;
  *atten_max = limits.atten_max;
  *post_min = limits.post_min;
  *post_max = limits.post_max;
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_get_tx_eq
 *
 *************************************************************/
int port_mgr_av_sd_get_tx_eq(
    bf_dev_id_t dev_id, int ring, int sd, int *pre, int *atten, int *post) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  Avago_serdes_tx_eq_t eq;

  avago_serdes_tx_eq_init(&eq);
#ifdef UTEST
  rc = 0;
  (void)aapl;
  (void)sbus_addr;
#else
  rc = avago_serdes_get_tx_eq(aapl, sbus_addr, &eq);
#endif  // UTEST
  if (rc == 0) {
    *pre = eq.pre;
    *atten = eq.atten;
    *post = eq.post;
  }
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_set_tx_eq
 *
 *************************************************************/
int port_mgr_av_sd_set_tx_eq(
    bf_dev_id_t dev_id, int ring, int sd, int pre, int atten, int post) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc = 0;
  Avago_serdes_tx_eq_t eq;
  Avago_serdes_tx_eq_t c_eq;
  Avago_serdes_tx_eq_limits_t limits;

  memset((char *)&limits, 0, sizeof(limits));
  avago_serdes_get_tx_eq_limits(aapl, sbus_addr, &limits);
  if ((pre < limits.pre_min) || (pre > limits.pre_max)) return -2;
  if ((atten < limits.atten_min) || (atten > limits.atten_max)) return -3;
  if ((post < limits.post_min) || (post > limits.post_max)) return -4;

  avago_serdes_tx_eq_init(&eq);
  eq.pre = pre;
  eq.atten = atten;
  eq.post = post;

  if (!sd_spico_int_mutex_initd) {
    bf_sys_rmutex_init(&sd_spico_int_mutex);
    sd_spico_int_mutex_initd = 1;
  }
  bf_sys_rmutex_lock(&sd_spico_int_mutex);

  avago_serdes_get_tx_eq(aapl, sbus_addr, &c_eq);
  if (rc || c_eq.pre != eq.pre || c_eq.atten != eq.atten ||
      c_eq.post != eq.post) {
    rc = avago_serdes_set_tx_eq(aapl, sbus_addr, &eq);
  }

  bf_sys_rmutex_unlock(&sd_spico_int_mutex);

  return rc;
}

/*************************************************************
 * port_mgr_av_sd_check_crc
 * 0 = failed
 * 1 = OK
 *************************************************************/
int port_mgr_av_sd_check_crc(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_spico_crc(aapl, sbus_addr);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_autoneg_en_set
 *
 *************************************************************/
int port_mgr_av_sd_autoneg_en_set(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  int en) {
  int rc;

  rc = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x7, (en ? 0x1 : 0x0));

  return (rc != 0x7);
}

/*************************************************************
 * port_mgr_av_sd_autoneg_en_set
 *
 *************************************************************/
int port_mgr_av_sd_autoneg_advert_set(bf_dev_id_t dev_id,
                                      int ring,
                                      int sd,
                                      uint64_t base_pg,
                                      int num_next_pg,
                                      uint64_t *next_pg) {
  uint32_t advert_wd0 = (uint32_t)((base_pg >> 0) & 0xfffffull);
  uint32_t advert_wd1 = (uint32_t)((base_pg >> 20) & 0xfffffull);
  uint32_t advert_wd2 = (uint32_t)((base_pg >> 40) & 0xfffffull);

  sds_sbus_log("Serdes: %d : %d : %d : base_pg: %016" PRIx64 " : num_pgs=%d",
               dev_id,
               ring,
               sd,
               base_pg,
               num_next_pg);

  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x029, advert_wd0);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x129, advert_wd1);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x229, advert_wd2);
  return 0;

  (void)num_next_pg;
  (void)next_pg;
}

/*************************************************************
 * port_mgr_av_sd_autoneg_start
 *
 *************************************************************/
int port_mgr_av_sd_autoneg_start(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 uint64_t base_pg,
                                 int num_next_pg,
                                 bool disable_nonce_match,
                                 bool disable_link_inhibit_timer) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_an_config_t *an_cfg_p = port_mgr_av_sd_an_config_construct(aapl);
  int rc;

  if (an_cfg_p == NULL) {
    port_mgr_log("SDS: Construct an auto-neg struct for dev %d failed", dev_id);
    return 0;
  }

  /* first, disable AN */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x7, 0);

  an_cfg_p->an_clk = 0;
  an_cfg_p->disable_link_inhibit_timer = disable_link_inhibit_timer;
  an_cfg_p->ignore_nonce_match = disable_nonce_match;
  an_cfg_p->nonce_pattern_sel = 0;
  an_cfg_p->nonce_user_pattern = 0;
  an_cfg_p->user_cap = (uint32_t)((base_pg >> 21ull));
  an_cfg_p->auto_kr = 0;
  // Warning: Using auto_kr can result in spico crashes
  // an_cfg_p->auto_kr = 1;
  an_cfg_p->pmd_config = 0x12;  // 2;
  an_cfg_p->np_enable = (num_next_pg > 0) ? 1 : 0;
  an_cfg_p->fec_ability = 0;
  an_cfg_p->fec_request = 0;
  if ((base_pg & (1ull << 44))) {
    an_cfg_p->fec_request |= (1 << 1);  // 25g RS FEC
  }
  if ((base_pg & (1ull << 45))) {
    an_cfg_p->fec_request |= (1 << 2);  // 25g FC FEC
  }
  if ((base_pg & (1ull << 46))) {  // 10g FC FEC ability
    an_cfg_p->fec_ability = 1;
  }
  if ((base_pg & (1ull << 47))) {  // 10g FC FEC request
    an_cfg_p->fec_request |= (1 << 0);
  }
  an_cfg_p->np_continuous_load = 0;  // need to save LP NP's so cant autoload

  bf_an_base_page_log(dev_id, ring, sd, base_pg);

  rc = port_mgr_av_sd_an_start(aapl, sbus_addr, an_cfg_p);

  port_mgr_av_sd_an_config_destruct(aapl, an_cfg_p);

  return rc;
}

/*************************************************************
 * port_mgr_av_sd_autoneg_stop
 *
 *************************************************************/
int port_mgr_av_sd_autoneg_stop(bf_dev_id_t dev_id, int ring, int sd) {
  /* disable AN */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x7, 0);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_o_core_status_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_o_core_status_get(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4069, 0);
  if ((o_core_status != 0) && (o_core_status != 0x20)) {
    if (0) {
      port_mgr_log("%d:%d:%3d: AN o_core_status: %08x : %s%s%s%s%s%s",
                   dev_id,
                   ring,
                   sd,
                   o_core_status,
                   (o_core_status >> 1) & 1 ? "NPRdy " : "",
                   (o_core_status >> 2) & 1 ? "BsPgRdy " : "",
                   (o_core_status >> 3) & 1 ? "ANCp " : "",
                   (o_core_status >> 4) & 1 ? "ANGd " : "",
                   (o_core_status >> 8) & 1 ? "FCFec " : "",
                   (o_core_status >> 9) & 1 ? "RSFec " : "");
    }
  }
  return o_core_status;
}

void port_mgr_av_sd_an_status_get(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  bool *lp_base_pg_rdy,
                                  bool *lp_next_pg_rdy,
                                  bool *an_good,
                                  bool *an_complete,
                                  bool *an_failed) {
  uint32_t o_core_status, sticky_st;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  *lp_next_pg_rdy = (o_core_status >> 1) & 1 ? true : false;
  *lp_base_pg_rdy = (o_core_status >> 2) & 1 ? true : false;
  *an_good = (o_core_status >> 4) & 1 ? true : false;
  *an_complete = (o_core_status >> 3) & 1 ? true : false;

  sticky_st = port_mgr_av_sd_spico_int(
      dev_id, ring, sd, 0x807, 2);  // read sticky state
  *an_failed = (sticky_st == 8) ? true : false;
}

/*************************************************************
 * port_mgr_av_sd_lp_np_pg_rdy_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_lp_np_pg_rdy_get(bf_dev_id_t dev_id, int ring, int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  return ((o_core_status >> 1) & 1);
}

/*************************************************************
 * port_mgr_av_sd_lp_base_pg_rdy_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_lp_base_pg_rdy_get(bf_dev_id_t dev_id,
                                           int ring,
                                           int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  return ((o_core_status >> 2) & 1);
}

/*************************************************************
 * port_mgr_av_sd_an_cmplt_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_an_cmplt_get(bf_dev_id_t dev_id, int ring, int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  return ((o_core_status >> 3) & 1);
}

/*************************************************************
 * port_mgr_av_sd_an_good_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_an_good_get(bf_dev_id_t dev_id, int ring, int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  return ((o_core_status >> 4) & 1);
}

/*************************************************************
 * port_mgr_av_sd_fc_fec_resolution_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_fc_fec_resolution_get(bf_dev_id_t dev_id,
                                              int ring,
                                              int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  return ((o_core_status >> 8) & 1);
}

/*************************************************************
 * port_mgr_av_sd_rs_fec_resolution_get
 *
 *************************************************************/
uint32_t port_mgr_av_sd_rs_fec_resolution_get(bf_dev_id_t dev_id,
                                              int ring,
                                              int sd) {
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  return ((o_core_status >> 9) & 1);
}

/*************************************************************
 * port_mgr_av_sd_lp_base_pg_get
 *
 *************************************************************/
uint64_t port_mgr_av_sd_lp_base_pg_get(bf_dev_id_t dev_id, int ring, int sd) {
  uint64_t lp_base_pg;
  uint32_t pg_15_0, pg_31_16, pg_47_32;

  pg_15_0 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x629, 0x0);
  pg_31_16 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x629, 0x1);
  pg_47_32 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x629, 0x2);

  lp_base_pg = ((uint64_t)pg_15_0 << 0ull) | ((uint64_t)pg_31_16 << 16ull) |
               ((uint64_t)pg_47_32 << 32ull);
  bf_an_base_page_log(dev_id, ring, sd, lp_base_pg);
  return lp_base_pg;
}

/*************************************************************
 * port_mgr_av_sd_lp_next_pg_get
 *
 *************************************************************/
uint64_t port_mgr_av_sd_lp_next_pg_get(bf_dev_id_t dev_id, int ring, int sd) {
  uint64_t lp_next_pg;
  uint32_t pg_15_0, pg_31_16, pg_47_32;

  pg_15_0 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x729, 0x0);
  pg_31_16 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x729, 0x1);
  pg_47_32 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x729, 0x2);

  lp_next_pg = ((uint64_t)pg_15_0 << 0ull) | ((uint64_t)pg_31_16 << 16ull) |
               ((uint64_t)pg_47_32 << 32ull);
  return lp_next_pg;
}

/*************************************************************
 * port_mgr_av_sd_next_pg_set
 *
 *************************************************************/
void port_mgr_av_sd_next_pg_set(bf_dev_id_t dev_id,
                                int ring,
                                int sd,
                                uint64_t next_pg) {
  uint32_t pg_15_0, pg_31_16, pg_47_32;

  pg_15_0 = (next_pg & 0xFFFFull);
  pg_31_16 = ((next_pg >> 16ull) & 0xFFFFull);
  pg_47_32 = ((next_pg >> 32ull) & 0xFFFFull);

  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x329, pg_15_0);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x429, pg_31_16);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x529, pg_47_32);

  // tell spico NP is loaded
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x507, 1);
  return;
}

/*************************************************************
 * port_mgr_av_sd_pmd_training_start
 *
 *************************************************************/
uint32_t port_mgr_av_sd_pmd_training_start(bf_dev_id_t dev_id,
                                           int ring,
                                           int sd,
                                           bool hi_spd) {
  int rc;
  lld_err_t err;
  bf_sku_chip_part_rev_t rev_no;

  if (hi_spd) {
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x0104);
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x0F);
  }

  /* A0 (rev_no==0) requires delay-cal.
   *  B0 (rev_no==1) delay-cal doesn't currently work with the B0
   *                 firmware.
   */
  err = lld_sku_get_chip_part_revision_number(dev_id, &rev_no);
  if ((err == LLD_OK) && rev_no == 0) {
    // run delay-cal after ical
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5301);
  } else {
    // run delay-cal after ical
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, 0x5301);
  }

  rc = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4, 0x02);
  return ((rc == 4) ? 0 : -1);
}

/*************************************************************
 * port_mgr_av_sd_pmd_training_stop
 *
 *************************************************************/
uint32_t port_mgr_av_sd_pmd_training_stop(bf_dev_id_t dev_id,
                                          int ring,
                                          int sd) {
  int rc;

  rc = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4, 0x4);

  // Test, turn off PRBS generation
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2, 0x3ff);

  return ((rc == 4) ? 0 : -1);
}

/*************************************************************
 * port_mgr_av_sd_pmd_training_restart
 *
 *************************************************************/
uint32_t port_mgr_av_sd_pmd_training_restart(bf_dev_id_t dev_id,
                                             int ring,
                                             int sd) {
  /* disable AN */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4, 0);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_autoneg_st_get
 *
 *************************************************************/
int port_mgr_av_sd_autoneg_st_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int an_st, sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int cmplt, good;

  cmplt =
      avago_serdes_read_an_status(aapl, sbus_addr, AVAGO_SERDES_AN_COMPLETE);
  good = avago_serdes_read_an_status(aapl, sbus_addr, AVAGO_SERDES_AN_GOOD);

  port_mgr_log("%d:%d:%d: AN rtnd: gd=%d cp=%d", dev_id, ring, sd, good, cmplt);

  if (good == 0) {
    if (1) {
      uint32_t lp_bp0, lp_bp1, lp_bp2;
      uint64_t base_pg;

      // this will log o_core_status
      port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);

      if (1) {
        lp_bp0 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x629, 0x0);
        lp_bp1 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x629, 0x1);
        lp_bp2 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x629, 0x2);
        port_mgr_log("%d:%d:%d: AN LP base-pg: %04x_%04x_%04x",
                     dev_id,
                     ring,
                     sd,
                     lp_bp2,
                     lp_bp1,
                     lp_bp0);
        base_pg = ((uint64_t)lp_bp2 << 32ull) | ((uint64_t)lp_bp1 << 16ull) |
                  ((uint64_t)lp_bp0 << 0ull);
        bf_an_base_page_log(dev_id, ring, sd, base_pg);
        an_st = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x807, 0x1);
        // sds_sbus_log(
        //    "%d:%d:%d: AN real-time st: %02xh", dev_id, ring, sd, an_st);
      }
    }
  } else if (cmplt == 0) {
    an_st = BF_AN_ST_GOOD;
  } else {
    an_st = BF_AN_ST_COMPLETE;
  }
  return an_st;
}

/*************************************************************
 * port_mgr_av_sd_autoneg_hcd_fec_get
 *
 *************************************************************/
int port_mgr_av_sd_autoneg_hcd_fec_get(bf_dev_id_t dev_id,
                                       int ring,
                                       int sd,
                                       bf_port_speed_t *resolved_hcd,
                                       bf_fec_type_t *resolved_fec,
                                       uint32_t *av_hcd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int hcd, fec;
  uint32_t o_core_status;

  o_core_status = port_mgr_av_sd_o_core_status_get(dev_id, ring, sd);
  if ((o_core_status >> 8) & 1) {
    fec = BF_FEC_TYP_FIRECODE;
  } else if ((o_core_status >> 9) & 1) {
    fec = BF_FEC_TYP_REED_SOLOMON;
  } else {
    fec = BF_FEC_TYP_NONE;
  }
  *resolved_fec = fec;

  hcd = avago_serdes_read_an_status(aapl, sbus_addr, AVAGO_SERDES_AN_READ_HCD);
  *av_hcd = hcd;  // return raw hcd value for assert link-status

  switch (hcd) {
    case 0:
      *resolved_hcd = BF_SPEED_1G;
      break;
    case 2:
      *resolved_hcd = BF_SPEED_10G;
      break;
    case 3:
    case 4:
      *resolved_hcd = BF_SPEED_40G;
      break;
    case 8:
    case 9:
      *resolved_hcd = BF_SPEED_100G;
      break;
    case 10:
    case 11:
      *resolved_hcd = BF_SPEED_25G;
      break;
    default:
      *resolved_hcd = 0;  // unsupported
      break;
  }
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_link_training_st_extended_get
 *
 *************************************************************/
void port_mgr_av_sd_link_training_st_extended_get(bf_dev_id_t dev_id,
                                                  int ring,
                                                  int sd,
                                                  int *failed,
                                                  int *in_prg,
                                                  int *rx_trnd,
                                                  int *frm_lk,
                                                  int *rmt_rq,
                                                  int *lcl_rq,
                                                  int *rmt_rcvr_rdy) {
  uint32_t o_core_status_15_0, o_core_status_31_16;

  o_core_status_15_0 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4027, 0);
  o_core_status_31_16 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4069, 0);

  *failed = (o_core_status_15_0 >> 0) & 1;
  *in_prg = (o_core_status_15_0 >> 1) & 1;
  *rx_trnd = (o_core_status_15_0 >> 2) & 1;
  *frm_lk = (o_core_status_15_0 >> 3) & 1;
  *rmt_rq = (o_core_status_15_0 >> 6) & 1;
  *lcl_rq = (o_core_status_15_0 >> 7) & 1;
  *rmt_rcvr_rdy = (o_core_status_31_16 >> 2) & 1;
}

/*************************************************************
 * port_mgr_av_sd_link_training_st_get
 *
 *************************************************************/
int port_mgr_av_sd_link_training_st_get(bf_dev_id_t dev_id,
                                        int ring,
                                        int sd,
                                        bf_lt_state_e *lt_st) {
  uint32_t o_core_status_15_0, o_core_status_31_16;

  o_core_status_15_0 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4027, 0);
  o_core_status_31_16 = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x4069, 0);

  // log if anything interesting
  if (((o_core_status_15_0 >> 0) & 1) ||   // failed
      ((o_core_status_15_0 >> 2) & 1) ||   // local rx trained
      ((o_core_status_31_16 >> 2) & 1)) {  // remote rx trained
    port_mgr_log(
        "%d:%d:%3d: Link training: fail=%d : in_prg=%d : rx_trnd=%d : "
        "frm_lk=%d : rmt-rq=%d : lcl-rq=%d : RemRcvrRdy=%d",
        dev_id,
        ring,
        sd,
        (o_core_status_15_0 >> 0) & 1,
        (o_core_status_15_0 >> 1) & 1,
        (o_core_status_15_0 >> 2) & 1,
        (o_core_status_15_0 >> 3) & 1,
        (o_core_status_15_0 >> 6) & 1,
        (o_core_status_15_0 >> 7) & 1,
        (o_core_status_31_16 >> 2) & 1);
  }
  if ((o_core_status_15_0 & 7) == 4)
    *lt_st = BF_LT_ST_COMPLETE;  // cmplt, no failure, not in-progress
  else if ((o_core_status_15_0 & 1) != 0)
    *lt_st = BF_LT_ST_FAILED;
  else if ((o_core_status_15_0 & 2) != 0)
    *lt_st = BF_LT_ST_RUNNING;
  else
    *lt_st = BF_LT_ST_NONE;

  return 0;
}

/**************************************************
 * port_mgr_av_sd_assert_hcd_link_status
 ***************************************************/
int port_mgr_av_sd_assert_hcd_link_status(
    bf_dev_id_t dev_id, int ring, int tx_sd, int rx_sd, uint32_t hcd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int tx_sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, tx_sd);
  int rc;

  rc = port_mgr_av_sd_an_assert_link_status_after_an_good(
      aapl, tx_sbus_addr, hcd);
  if (rc != 0) {
    port_mgr_log("%d:%d:%d: Assert hcd link-status failed <%d %xh>",
                 dev_id,
                 ring,
                 tx_sd,
                 rc,
                 rc);
  }
  return rc;
  (void)rx_sd;
}

/**************************************************
 * Map back from an aapl ptr to a bf_dev_id_t.
 ***************************************************/
bf_dev_id_t port_mgr_av_sd_map_aapl_to_dev_id(Aapl_t *aapl) {
  void *reverse_map_dev = aapl_bind_get(aapl);

  return (bf_dev_id_t)(uintptr_t)reverse_map_dev;
}

typedef enum {
  BF_SDS_RESET = 0,
  BF_SDS_WRITE = 1,
  BF_SDS_READ = 2
} bf_av_sd_cmd_t;

bool pcie_locked = true;

void port_mgr_av_sd_unlock_pcie(void) { pcie_locked = false; }

void port_mgr_av_sd_lock_pcie(void) { pcie_locked = true; }

int port_mgr_av_sd_parallel_serdes_int_sbus_fn(
    Aapl_t *aapl,            /**< [in] Pointer to AAPL structure. */
    Avago_addr_t *addr_list, /**< [in,out] List of addresses and results. */
    int int_num,             /**< [in] Interrupt code. */
    int int_data)            /**< [in] Interrupt data. */
{
  int loops = 0;
  int rc;
  int return_code = aapl->return_code;
  BOOL identical_results = TRUE;
  Avago_addr_t *addr_struct;

  for (addr_struct = addr_list; addr_struct != 0;
       addr_struct = addr_struct->next) {
    uint addr = avago_struct_to_addr(addr_struct);
    addr_struct->results = avago_spico_int(aapl, addr, int_num, int_data);
    if (addr_struct->results != addr_list->results) identical_results = FALSE;
  }

  rc = aapl->return_code == return_code ? (identical_results ? 1 : 0) : -1;
  aapl_log_printf(
      aapl,
      AVAGO_DEBUG5,
      __func__,
      __LINE__,
      "parallel_int(0x%x, 0x%x) -> 0x%x (%s); int_ret = 0x%x; loops=%d\n",
      int_num,
      int_data,
      rc,
      rc == 1 ? "all same" : rc == 0 ? "some differences" : "error",
      addr_list->results,
      loops);
  return rc;
}

uint32_t port_mgr_av_sd_access_via_sbus_fn(
    Aapl_t *aapl, uint32_t addr, uint8_t reg, uint8_t cmd, uint32_t *data) {
  bf_dev_id_t dev_id = port_mgr_av_sd_map_aapl_to_dev_id(aapl);
  bf_dev_id_t decoded_dev_id;
  int ring, sd, min_ring0_sd;

  port_mgr_av_sd_decode_sbus_addr(addr, &decoded_dev_id, &ring, &sd);

  // hack, make sure everything matches til we're sure
  if (dev_id != decoded_dev_id) {
    port_mgr_log("%d:%d:%d: Warning: sbus_addr(%x) and Aapl_t (%x) dont match?",
                 dev_id,
                 ring,
                 sd,
                 addr,
                 decoded_dev_id);
  }
  // construct PCIe address
  // [25:23] = 001'b
  // [22:19] = 0
  // [18:18] = ring
  // [17:10] = node address
  // [9:2]  = register
  // [1:0]   = 00'b
  min_ring0_sd = pcie_locked ? 10 : 1;
  if ((ring == 0) && (sd <= min_ring0_sd) && ((reg != 253) && (reg != 255))) {
    port_mgr_log("%d:%d:%d : Errant PCIe access : addr=%xh : reg=%xh : cmd=%s",
                 dev_id,
                 ring,
                 sd,
                 addr,
                 reg,
                 (cmd == 0)
                     ? "reset"
                     : (cmd == 1) ? "write" : (cmd == 2) ? "Read" : "??");
    return FALSE;
  } else if ((ring == 0) && (sd <= min_ring0_sd) &&
             ((reg == 253) || (reg == 255))) {
    port_mgr_log("%d:%d:%d : PCIe access : addr=%xh : reg=%xh : cmd=%s",
                 dev_id,
                 ring,
                 sd,
                 addr,
                 reg,
                 (cmd == 0)
                     ? "reset"
                     : (cmd == 1) ? "write" : (cmd == 2) ? "Read" : "??");
  }
  if (!sd_access_mutex_initd) {
    bf_sys_mutex_init(&sd_access_mutex);
    sd_access_mutex_initd = 1;
  }
  bf_sys_mutex_lock(&sd_access_mutex);

  // issue read or write
  switch (cmd) {
    case 0:  // reset, requires indirect access
    {
      uint32_t sbm_ind_wdata =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_wdata);
      uint32_t sbm_ind_ctrl =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_ctrl);
      uint32_t sbm_ind_rslt =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_rslt);
      uint32_t sts, tmo = 1000;

      // HACK, dont reset sbus ctrlr or spico
      if ((addr == 0xfe) || (addr == 0xfd)) {
        port_mgr_log("SDS: SKip reset of %x (%s) as it seems to hang sim",
                     addr,
                     addr == 0xfe ? "sbm ctrlr" : "sbm spico");
        bf_sys_mutex_unlock(&sd_access_mutex);
        return TRUE;
      }
      lld_write_register(dev_id, sbm_ind_wdata, 0);
      lld_write_register(dev_id,
                         sbm_ind_ctrl,
                         (0x20 << 16) | (sd << 8) | (ring << 24) | (1 << 25));

      while (--tmo) {
        lld_read_register(dev_id, sbm_ind_rslt, &sts);
        if (sts & 0x10) continue;  // done
        if (sts & 0x8) {
          sds_sbus_log(
              "SDS: ERROR: sbus timeout on reset: addr=%xh : rslt code=%d",
              addr,
              sts & 7);
          bf_sys_mutex_unlock(&sd_access_mutex);
          return FALSE;
        } else
          break;
      }
      break;
    }
    case 1:  // write
    {
      uint32_t sbm_ind_wdata =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_wdata);
      uint32_t sbm_ind_ctrl =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_ctrl);
      uint32_t sbm_ind_rslt =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_rslt);
      uint32_t sts, tmo = 1000;

      lld_write_register(dev_id, sbm_ind_wdata, *data);
      lld_write_register(
          dev_id,
          sbm_ind_ctrl,
          reg | (0x21 << 16) | (sd << 8) | (ring << 24) | (1 << 25));
      while (--tmo) {
        lld_read_register(dev_id, sbm_ind_rslt, &sts);
        if (sts & 0x10) continue;
        if (sts & 0x8) {
          sds_sbus_log(
              "SDS: ERROR: sbus timeout on write: addr=%xh : rslt code=%d",
              addr,
              sts & 7);
          bf_sys_mutex_unlock(&sd_access_mutex);
          return FALSE;
        } else
          break;
      }
      break;
    }
    case 2:  // read
    {
      uint32_t sbm_ind_ctrl =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_ctrl);
      uint32_t sbm_ind_rslt =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_rslt);
      uint32_t sbm_ind_rdata =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_rdata);
      uint32_t sts, tmo = 1000;

      lld_write_register(
          dev_id,
          sbm_ind_ctrl,
          reg | (0x22 << 16) | (sd << 8) | (ring << 24) | (1 << 25));
      while (--tmo) {
        lld_read_register(dev_id, sbm_ind_rslt, &sts);
        if (sts & 0x10) continue;
        if (sts & 0x8) {
          sds_sbus_log(
              "SDS: ERROR: sbus timeout on read: addr=%xh : rslt code=%d",
              addr,
              sts & 7);
          bf_sys_mutex_unlock(&sd_access_mutex);
          return FALSE;
        } else
          break;
      }
      lld_read_register(dev_id, sbm_ind_rdata, data);
      break;
    }
    default:
      bf_sys_assert(0);
  }
  bf_sys_mutex_unlock(&sd_access_mutex);

  sds_sbus_log(
      "SDS: sbus: %s : addr=%02x : reg= %02x, data=%08x",
      cmd == 0 ? "reset" : cmd == 1 ? "write" : cmd == 2 ? "read" : "?",
      addr,
      reg,
      *data);

  return TRUE;
}

uint32_t port_mgr_av_sd_access_via_sbus_direct_fn(
    Aapl_t *aapl, uint32_t addr, uint8_t reg, uint8_t cmd, uint32_t *data) {
  uint32_t pcie_addr;
  bf_dev_id_t dev_id = port_mgr_av_sd_map_aapl_to_dev_id(aapl);
  bf_dev_id_t decoded_dev_id;
  int ring, sd;

  port_mgr_av_sd_decode_sbus_addr(addr, &decoded_dev_id, &ring, &sd);

  // hack, make sure everything matches til we're sure
  if (dev_id != decoded_dev_id) {
    sds_sbus_log("%d:%d:%d: Warning: sbus_addr(%x) and Aapl_t (%x) dont match?",
                 dev_id,
                 ring,
                 sd,
                 addr,
                 decoded_dev_id);
  }
  // construct PCIe address
  // [25:23] = 001'b
  // [22:19] = 0
  // [18:18] = ring
  // [17:10] = node address
  // [9:2]  = register
  // [1:0]   = 00'b
  pcie_addr = (1 << 23) |            // address decode
              ((ring & 1) << 18) |   // ring
              ((sd & 0xFF) << 10) |  // node
              ((reg & 0x0FF) << 2);  // register address

  if ((ring == 0) && (sd < 10) && (reg != 253)) {
    sds_sbus_log("%d:%d:%d : Errant PCIe access : addr=%xh : reg=%xh : cmd=%s",
                 dev_id,
                 ring,
                 sd,
                 addr,
                 reg,
                 (cmd == 0)
                     ? "reset"
                     : (cmd == 1) ? "write" : (cmd == 2) ? "Read" : "??");
    return FALSE;
  }
  if (!sd_access_mutex_initd) {
    bf_sys_mutex_init(&sd_access_mutex);
    sd_access_mutex_initd = 1;
  }

  // issue read or write
  switch (cmd) {
    case 0:  // reset, requires indirect access
    {
      uint32_t sbm_ind_wdata =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_wdata);
      uint32_t sbm_ind_ctrl =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_ctrl);
      uint32_t sbm_ind_rslt =
          offsetof(Tofino, device_select.misc_regs.sbm_ind_rslt);
      uint32_t sts, tmo = 1000;

      bf_sys_mutex_lock(&sd_access_mutex);

      // HACK, dont reset sbus ctrlr or spico
      if ((addr == 0xfe) || (addr == 0xfd)) {
        sds_sbus_log("SDS: SKip reset of %x (%s) as it seems to hang sim",
                     addr,
                     addr == 0xfe ? "sbm ctrlr" : "sbm spico");
        bf_sys_mutex_unlock(&sd_access_mutex);
        return TRUE;
      }
      lld_write_register(dev_id, sbm_ind_wdata, 0);
      lld_write_register(dev_id,
                         sbm_ind_ctrl,
                         (0x20 << 16) | (sd << 8) | (ring << 24) | (1 << 25));

      while (--tmo) {
        lld_read_register(dev_id, sbm_ind_rslt, &sts);
        if (sts & 0x10) continue;  // done
        if (sts & 0x8) {
          sds_sbus_log(
              "SDS: ERROR: sbm timeout on reset: addr=%xh : rslt code=%d",
              addr,
              sts & 7);
          bf_sys_mutex_unlock(&sd_access_mutex);
          return FALSE;
        } else
          break;
      }
      bf_sys_mutex_unlock(&sd_access_mutex);
      break;
    }
    case 1:  // write
    {
      lld_write_register(dev_id, pcie_addr, *data);
      break;
    }
    case 2:  // read
    {
      lld_read_register(dev_id, pcie_addr, data);
      break;
    }
    default:
      bf_sys_assert(0);
  }

  sds_sbus_log(
      "SDS: sbus: %s : addr=%02x : reg= %02x, data=%08x",
      cmd == 0 ? "reset" : cmd == 1 ? "write" : cmd == 2 ? "read" : "?",
      addr,
      reg,
      *data);

  return TRUE;
}

/************************************************************************
 * port_mgr_av_sd_spico_int_via_sbus_fn
 *
 * Note: int_tmo may require tuning on real HW
 ************************************************************************/
uint32_t port_mgr_av_sd_spico_int_via_sbus_fn(Aapl_t *aapl,
                                              uint32_t addr,
                                              int int_code,
                                              int int_data) {
  uint32_t int_cmd, int_sts, int_tmo = 10000;
  uint32_t success;

  if (!sd_spico_int_mutex_initd) {
    bf_sys_rmutex_init(&sd_spico_int_mutex);
    sd_spico_int_mutex_initd = 1;
  }
  bf_sys_rmutex_lock(&sd_spico_int_mutex);

  if (log_spico_ints) {
    port_mgr_log("SDS: sbus: INT : addr=%02x : int_code=%04x : int_data=%04x",
                 addr,
                 int_code,
                 int_data);
  }
  int_cmd = ((uint32_t)int_code << 16) | (uint32_t)int_data;
  port_mgr_av_sd_access_via_sbus_fn(aapl, addr, 0x3, BF_SDS_WRITE, &int_cmd);

  while (--int_tmo > 0) {
    int_sts = 0xffffffff;
    success = port_mgr_av_sd_access_via_sbus_fn(
        aapl, addr, 0x4, BF_SDS_READ, &int_sts);
    /* check status and return if in progress and disabled are low */
    if (success && ((int_sts & 0x30000) == 0)) {
      if (log_spico_ints) {
        port_mgr_log(
            "SDS: sbus: INT : addr=%02x : int_code=%04x : int_data=%04x : "
            "retval=%08x",
            addr,
            int_code,
            int_data,
            int_sts);
      }
      bf_sys_rmutex_unlock(&sd_spico_int_mutex);
      return int_sts;
    }
  }
  bf_sys_rmutex_unlock(&sd_spico_int_mutex);
  port_mgr_log_error(
      "SDS: ERROR: sbus: INT : addr=%02x : int_code=%04x : int_data=%04x : "
      "retval=%08x ***TIMEOUT***",
      addr,
      int_code,
      int_data,
      int_sts);
  {
    uint32_t reg_3_val, reg_4_val;

    port_mgr_av_sd_access_via_sbus_fn(aapl, addr, 0x3, BF_SDS_READ, &reg_3_val);
    port_mgr_av_sd_access_via_sbus_fn(aapl, addr, 0x4, BF_SDS_READ, &reg_4_val);
    port_mgr_log_error(
        "SDS: ERROR: sbus: INT : addr=%02x : reg_3==%04x : reg_4=%04x : ",
        addr,
        reg_3_val,
        reg_4_val);
    uint32_t sbus_addr = addr;
    int pc = avago_sbus_rd(aapl, sbus_addr, 0x25);
    int mem_bist = avago_sbus_rd(aapl, sbus_addr, 0x9);
    int stepping = avago_sbus_rd(aapl, sbus_addr, 0x20);
    int enable = avago_sbus_rd(aapl, sbus_addr, 0x7);
    int error = avago_sbus_rd(aapl, sbus_addr, 0x2a);
    port_mgr_log_error(
        "SDS: ERROR: addr=%02x : pc=%x : mem_bist=%x : step=%x : ena=%x : "
        "error=%x",
        sbus_addr,
        pc,
        mem_bist,
        stepping,
        enable,
        error);
  }
  avago_spico_reset(aapl, addr);
  return 0;
}

/************************************************************************
 * port_mgr_av_sd_access_via_core_fn
 ************************************************************************/
uint32_t port_mgr_av_sd_access_via_core_fn(
    Aapl_t *aapl, uint32_t addr, uint8_t reg, uint8_t cmd, uint32_t *data) {
  // core interface only supports spico interrupts
  return port_mgr_av_sd_access_via_sbus_fn(aapl, addr, reg, cmd, data);
}

/************************************************************************
 * port_mgr_av_sd_spico_int_via_core_fn
 ************************************************************************/
uint32_t port_mgr_av_sd_spico_int_via_core_fn(Aapl_t *aapl,
                                              uint32_t addr,
                                              int int_code,
                                              int int_data) {
  int ring, sd;
  port_mgr_sbus_ip_type_e ip_type;
  int mac_block, ch;
  uint32_t ctrl_reg, sts_reg, status, int_tmo = 1000;
  bf_dev_id_t dev_id = port_mgr_av_sd_map_aapl_to_dev_id(aapl);
  bf_dev_id_t decoded_dev_id;

  sds_sbus_log("SDS: sbus: INT : addr=%02x : int_code=%04x : int_data=%04x",
               addr,
               int_code,
               int_data);

  port_mgr_av_sd_decode_sbus_addr(addr, &decoded_dev_id, &ring, &sd);

  // hack, make sure everything matches til we're sure
  if (dev_id != decoded_dev_id) {
    port_mgr_log_warn(
        "%d:%d:%d: Warning: sbus_addr(%x) and Aapl_t (%x) dont match?",
        dev_id,
        ring,
        sd,
        addr,
        decoded_dev_id);
  }

  // ring = ((addr >> 8) & 1);
  // sd = (addr & 0xFF);
  if (sd == 0xfe) return 0xFEFEFEFE;  // no core i/f on sbus controller

  port_mgr_find_mac_info_for(dev_id, ring, sd, &ip_type, &mac_block, &ch);
  if (ip_type != IP_TYPE_ETH_PMA) return 0xEEEEEEEE;  // no core i/f

  // mac block,ch is the MAC that has sd as its unswizzled serdes slice
  // ethsds_int_ctrl[31:16] int_code
  // ethsds_int_ctrl[15:0]  int_data
  // ethsds_int_stat[16:16] busy
  // ethsds_int_stat[15:0]  data_out
  ctrl_reg =
      offsetof(Tofino, macs_t[mac_block].macs.eth_regs.ethsds_int_ctrl[ch]);
  sts_reg =
      offsetof(Tofino, macs_t[mac_block].macs.eth_regs.ethsds_int_stat[ch]);

  lld_write_register(dev_id, ctrl_reg, (int_code << 16) | int_data);
  while (--int_tmo > 0) {
    lld_read_register(dev_id, sts_reg, &status);
    /* check status and return if in progress and disabled are low */
    if ((status & 0x10000) == 0) {
      sds_sbus_log(
          "SDS: core: INT : addr=%02x : int_code=%04x : int_data=%04x : "
          "retval=%08x",
          addr,
          int_code,
          int_data,
          status);
      return status & 0xffff;
    }
  }

  return 0;
}

/****************************************************************************
 * port_mgr_av_sd_access_fn_set
 *
 * Set serdes access method.
 * The core interface does not provide a means of accessing registers so
 * the "sbus" function has to be the direct interface.
 * For issuing spico interrupts there are two choices,
 *  BF_SDS_ACCESS_SBUS, direct interface
 *  BF_SDS_ACCESS_CORE, core interface
 *
 * Since the core interface is directly connected to the slice this interface
 * allows you to issue multiple spico interrupts simulatneousy. It is also
 * faster since the request does not need to traverse the sbus ring.
 *
 * There is one idiosyncracy with the core interface:
 *
 * There are 4 core interfaces per quad, one for each channel. However, the
 * "channel" whose core interface must be used is not necessarily the same
 * as the "channel" of the quad implementing the port on which you are
 * operating. This is because the core interface is connected directly from
 * the eth_regs to a given serdes slice, but the serdes used by the
 * corresponding port (MAC channel) may be arbitrarily remapped within a
 * quad.
 *
 * For example, the following is from the sbus map for tofino,
 *
 *    { 11  IP_TYPE_ETH_PMA, 0, 0, "Eth PMA"},
 *    { 12  IP_TYPE_ETH_PMA, 0, 1, "Eth PMA"},
 *    { 13  IP_TYPE_ETH_PMA, 0, 2, "Eth PMA"},
 *    { 14  IP_TYPE_ETH_PMA, 0, 3, "Eth PMA"},
 *
 * It shows serdes slices at addresses 11-14 are physically connected to
 * MAC block 0, ch0-3 respectively.
 * However, due to board layout, it may be that MAC channel 0 actually
 * uses serdes 12 for Rx and 14 for Tx. So, if you need to execute a spico
 * interrupt for the port on MAC channel 0 you will need to to use the
 * core interface on either ch1 (if an Rx-related int) or ch3 (if a Tx-
 * related int).
 ****************************************************************************/
int port_mgr_av_sd_access_fn_set(bf_dev_id_t dev_id,
                                 bf_serdes_access_method_e method) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  if (method == BF_SDS_ACCESS_SBUS) {
    aapl_register_sbus_fn(aapl, port_mgr_av_sd_access_via_sbus_fn, NULL, NULL);
    aapl_register_spico_int_fn(aapl, &port_mgr_av_sd_spico_int_via_sbus_fn);
    aapl_register_parallel_serdes_int_fn(
        aapl, &port_mgr_av_sd_parallel_serdes_int_sbus_fn);
    return 0;
  } else if (method == BF_SDS_ACCESS_CORE) {
    // aapl_register_sbus_fn(aapl, port_mgr_av_sd_access_via_core_fn, NULL,
    // NULL);
    aapl_register_sbus_fn(aapl, port_mgr_av_sd_access_via_sbus_fn, NULL, NULL);
    aapl_register_spico_int_fn(aapl, &port_mgr_av_sd_spico_int_via_core_fn);
    return 0;
  }
  return -1;
}

int port_mgr_av_sd_pll_lock_get(bf_dev_id_t dev_id,
                                int ring,
                                int tx_sd,
                                int rx_sd,
                                bool *tx_sd_ready,
                                bool *rx_sd_ready) {
  int tx_ready = 0, rx_ready = 0;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int tx_sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, tx_sd);
  int rx_sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, rx_sd);

  avago_serdes_get_tx_rx_ready(aapl, tx_sbus_addr, &tx_ready, &rx_ready);
  *tx_sd_ready = tx_ready;
  if (tx_sd == rx_sd) {
    *rx_sd_ready = rx_ready;
  } else {
    avago_serdes_get_tx_rx_ready(aapl, rx_sbus_addr, &tx_ready, &rx_ready);
    *rx_sd_ready = rx_ready;
  }
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_check_tx_pll_state
 *
 *************************************************************/
int port_mgr_av_sd_tx_pll_state_get(
    bf_dev_id_t dev_id, int ring, int sd, bool *locked, int *div, int *freq) {
  Avago_serdes_pll_state_t pll_state;
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  avago_serdes_get_tx_pll_state(aapl, sbus_addr, &pll_state);

  *locked = pll_state.ready;
  *div = pll_state.divider;
  *freq = (int)(pll_state.est_rate >> 20ul);  // bps to Mbps

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_check_rx_pll_state
 *
 *************************************************************/
int port_mgr_av_sd_rx_pll_state_get(
    bf_dev_id_t dev_id, int ring, int sd, bool *locked, int *div, int *freq) {
  Avago_serdes_pll_state_t pll_state;
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);

  avago_serdes_get_rx_pll_state(aapl, sbus_addr, &pll_state);

  *locked = pll_state.ready;
  *div = pll_state.divider;
  *freq = (int)(pll_state.est_rate >> 20ul);  // bps to Mbps

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_far_loopback_set
 *
 *************************************************************/
int port_mgr_av_sd_far_loopback_set(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    bool en) {
  int rc;

  rc = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x8, en ? 0x210 : 0x200);
  return ((rc == 0x8) ? 0 : 1);
}

/*************************************************************
 * port_mgr_av_sd_near_loopback_set
 *
 *************************************************************/
int port_mgr_av_sd_near_loopback_set(bf_dev_id_t dev_id,
                                     int ring,
                                     int sd,
                                     bool en) {
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int rc;

  rc = avago_serdes_set_rx_input_loopback(aapl, sbus_addr, en ? TRUE : FALSE);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_near_loopback_get
 *
 *************************************************************/
int port_mgr_av_sd_near_loopback_get(bf_dev_id_t dev_id,
                                     int ring,
                                     int sd,
                                     bool *en) {
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  BOOL rc;

  rc = avago_serdes_get_rx_input_loopback(aapl, sbus_addr);
  *en = (rc == TRUE) ? true : false;
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_tx_drv_en_set
 *
 *************************************************************/
int port_mgr_av_sd_tx_drv_en_set(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 bool tx_drv_en) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;

  rc = avago_serdes_set_tx_output_enable(
      aapl, sbus_addr, tx_drv_en ? TRUE : FALSE);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_tx_drv_en_get
 *
 *************************************************************/
int port_mgr_av_sd_tx_drv_en_get(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 bool *tx_drv_en) {
  BOOL en;

  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  en = avago_serdes_get_tx_output_enable(aapl, sbus_addr);
  *tx_drv_en = (en == TRUE) ? true : false;
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_running_get
 *
 *************************************************************/
int port_mgr_av_sd_dfe_running_get(bf_dev_id_t dev_id,
                                   int ring,
                                   int sd,
                                   bool *running) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  BOOL av_running;

  av_running = avago_serdes_dfe_running(aapl, sbus_addr);
  *running = ((av_running == 0) ? false : true);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_ctle_set
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_ctle_set(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  int ctle_dc,
                                  int ctle_lf,
                                  int ctle_hf,
                                  int ctle_bw) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *dfe_state = avago_serdes_dfe_state_construct(aapl);

  if (dfe_state == NULL) return -1;

  dfe_state->dc = ctle_dc;
  dfe_state->lf = ctle_lf;
  dfe_state->hf = ctle_hf;
  dfe_state->bw = ctle_bw;

  avago_serdes_dfe_state_ext(
      aapl, sbus_addr, dfe_state, AVAGO_DFE_MODE_CTLE, TRUE);

  avago_serdes_dfe_state_destruct(aapl, dfe_state);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_ctle_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_ctle_get(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  int *ctle_dc,
                                  int *ctle_lf,
                                  int *ctle_hf,
                                  int *ctle_bw) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *dfe_state = avago_serdes_dfe_state_construct(aapl);

  if (dfe_state == NULL) return -1;

  avago_serdes_dfe_state_ext(
      aapl, sbus_addr, dfe_state, AVAGO_DFE_MODE_CTLE, FALSE);

  *ctle_dc = dfe_state->dc;
  *ctle_lf = dfe_state->lf;
  *ctle_hf = dfe_state->hf;
  *ctle_bw = dfe_state->bw;

  avago_serdes_dfe_state_destruct(aapl, dfe_state);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_tap_set
 *
 *************************************************************/
int port_mgr_av_sd_dfe_tap_set(
    bf_dev_id_t dev_id, int ring, int sd, int dfe_tap_num, int dfe_tap_val) {
  int row, col, sign_bit = 0;
  ;
  uint32_t data, tap_val;

  row = dfe_tap_num + 1;
  col = 3;
  tap_val = ((dfe_tap_val >= 0) ? dfe_tap_val : -dfe_tap_val) & 0xff;
  sign_bit = (((dfe_tap_val >= 0) ? 0 : 1) << 15);

  data = ((row & 0xF) << 8) | ((col & 0x7) << 12) | tap_val | sign_bit;

  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_tap_get
 *
 *************************************************************/
int port_mgr_av_sd_dfe_tap_get(
    bf_dev_id_t dev_id, int ring, int sd, int dfe_tap_num, int *dfe_tap_val) {
  int row, col;
  uint32_t data, av_val, tap_val;

  row = dfe_tap_num + 1;
  col = 3;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  av_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);
  tap_val = av_val & 0xFF;  // [7:0]
  *dfe_tap_val = (av_val & (1 << 15)) ? -tap_val : tap_val;

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_gain_set
 *
 *************************************************************/
int port_mgr_av_sd_dfe_gain_set(bf_dev_id_t dev_id,
                                int ring,
                                int sd,
                                uint32_t dfe_gain) {
  int row, col;
  uint32_t data;

  row = 0;
  col = 3;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12) | (dfe_gain & 0xFF);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_dfe_gain_get
 *
 *************************************************************/
int port_mgr_av_sd_dfe_gain_get(bf_dev_id_t dev_id,
                                int ring,
                                int sd,
                                uint32_t *dfe_gain) {
  int row, col;
  uint32_t data, av_val;

  row = 0;
  col = 3;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  av_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);
  *dfe_gain = (uint32_t)(av_val & 0xFF);  // [7:0]

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_status_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_status_get(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    int *dfe_status) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  /* DFE status bit meanings (as of 0x1048): */
  /* dfe_status[0] iCal in_prog */
  /* dfe_status[1] pCal in_prog */
  /* dfe_status[2] vos  in_prog */
  /* dfe_status[3] No REFCLK tuning flag */
  /* dfe_status[4] run_ical */
  /* dfe_status[5] run_pcal */
  /* dfe_status[6] adaptive pcal */
  /* dfe_status[7] VOS done */
  /* dfe_status[9] EI detected (loss of signal) */

  *dfe_status = avago_serdes_get_dfe_status(aapl, sbus_addr);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_vos_done_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_vos_done_get(bf_dev_id_t dev_id,
                                      int ring,
                                      int sd,
                                      bool *vos_done) {
  int dfe_status;

  port_mgr_av_sd_rx_eq_status_get(dev_id, ring, sd, &dfe_status);
  *vos_done = ((dfe_status >> 7) & 1) ? true : false;

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_cal_param_set
 *
 *      seed_DC         = ctle_dc_hint
 *      min_gainDFE     = dfe_gain_range[3:0]
 *      max_gainDFE     = dfe_gain_range[7:4]
 *      pCal_loop_count = pcal_loop_cnt
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_cal_param_set(bf_dev_id_t dev_id,
                                       int ring,
                                       int sd,
                                       int ctle_dc_hint,
                                       int dfe_gain_range,
                                       int pcal_loop_cnt) {
  int row, col;
  uint32_t data;

  // set seed_DC
  row = 2;
  col = 0;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12) | (ctle_dc_hint & 0xFF);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);

  // set min_gainDFE
  row = 5;
  col = 0;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12) | (dfe_gain_range & 0xF);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);

  // set max_gainDFE
  row = 6;
  col = 0;
  data =
      ((row & 0xF) << 8) | ((col & 0x7) << 12) | ((dfe_gain_range >> 4) & 0xF);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);

  // set max_gainDFE
  row = 1;
  col = 5;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12) | (pcal_loop_cnt & 0xFF);
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x26, data);

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_cal_param_get
 *
 *      seed_DC         = ctle_dc_hint
 *      min_gainDFE     = dfe_gain_range[3:0]
 *      max_gainDFE     = dfe_gain_range[7:4]
 *      pCal_loop_count = pcal_loop_cnt
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_cal_param_get(bf_dev_id_t dev_id,
                                       int ring,
                                       int sd,
                                       int *ctle_dc_hint,
                                       int *dfe_gain_range,
                                       int *pcal_loop_cnt) {
  int row, col;
  uint32_t data;
  uint32_t av_val, min_gain, max_gain;

  // get seed_DC
  row = 2;
  col = 0;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  av_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);
  *ctle_dc_hint = av_val & 0xFF;

  // get min_gainDFE
  row = 5;
  col = 0;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  av_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);
  min_gain = av_val & 0xF;

  // get max_gainDFE
  row = 6;
  col = 0;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  av_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);
  max_gain = av_val & 0xF;
  *dfe_gain_range = (max_gain << 4) | min_gain;

  // get max_gainDFE
  row = 1;
  col = 5;
  data = ((row & 0xF) << 8) | ((col & 0x7) << 12);
  av_val = port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x126, data);
  *pcal_loop_cnt = av_val & 0xFF;

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_cal_adv_run
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_cal_adv_run(bf_dev_id_t dev_id,
                                     int ring,
                                     int sd,
                                     bf_sds_rx_cal_mode_t cal_cmd,
                                     int ctle_cal_cfg,
                                     int dfe_fixed) {
#if 0
  BF_SDS_RX_ICAL_PCAL = 0,       /**< Run iCal follow by pCal_once */
  BF_SDS_RX_ICAL_NO_PCAL = 1,    /**< (Debug) Run iCal without pCal */
  BF_SDS_RX_PCAL_ONCE = 2,       /**< Run pCal once */
  BF_SDS_RX_PCAL_CONT_START = 3, /**< Start pCal continuous mode */
  BF_SDS_RX_PCAL_CONT_STOP = 4,  /**< Stop  pCal */
  BF_SDS_RX_PCAL_RR_DISABLE = 5, /**< Disable Round Robin participation */
  BF_SDS_RX_PCAL_RR_ENABLE = 6,  /**< Enable  Round robin participation */
  BF_SDS_RX_CAL_SLICER_ONLY = 7, /**< Run slicer cal only */

    AVAGO_DFE_ICAL,          /**< Initial calibration, coarse+fine tuning. (Default) */
    AVAGO_DFE_PCAL,          /**< Periodic calibration, fine tuning, no LF,HF adjustments */
    AVAGO_DFE_ICAL_ONLY,     /**< Initial calibration, coarse tuning without PCAL. */
    AVAGO_DFE_START_ADAPTIVE,/**< Launch continous pCAL */
    AVAGO_DFE_STOP_ADAPTIVE, /**< Stop continous pCAL */
    AVAGO_DFE_ENABLE_RR,     /**< Enable SerDes to participate in Round-Robin pCal */
    AVAGO_DFE_DISABLE_RR     /**< Disable SerDes from participation in Round-Robin pCal */

  //  6   Bypass DFE          Yes         dfe_disable     dfe_fixed
  //  7   Fixed DC            Yes         fixed_dc        ctle_cal_cfg[0]
  //  8   Fixed LF            Yes         fixed_lf        ctle_cal_cfg[1]
  //  9   Fixed HF            Yes         fixed_hf        ctle_cal_cfg[2]
  //  10  Fixed BW            Yes         fixed_bw        ctle_cal_cfg[3]
  //  11  Seeded DC           Yes         seeded_dc       ctle_cal_cfg[4]
  //  12  Seeded LF           No
  //  13  Seeded HF           No
  //  14  Reserved            No
  //  15  Run VOS only        Yes         N/A (*2)        cal_cmd=CAL_SLICER

#endif
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_dfe_state_t *dfe_st;
  int av_cal_cmd;

  if (cal_cmd == BF_SDS_RX_CAL_SLICER_ONLY) {
    //  2. For cal_cmd=CAL_SLICER_ONLY, write to Int 0x0A=0x80 directly.
    //      avago_serdes_dfe_tune() does not have this implemented
    port_mgr_av_sd_spico_int(dev_id, ring, sd, 0xA, 0x80);
    return 0;
  }

  dfe_st = avago_serdes_dfe_state_construct(aapl);
  if (dfe_st == NULL) return -1;

  dfe_st->fixed_dc = ((ctle_cal_cfg >> 0) & 1) ? TRUE : FALSE;
  dfe_st->fixed_lf = ((ctle_cal_cfg >> 1) & 1) ? TRUE : FALSE;
  dfe_st->fixed_hf = ((ctle_cal_cfg >> 2) & 1) ? TRUE : FALSE;
  dfe_st->fixed_bw = ((ctle_cal_cfg >> 3) & 1) ? TRUE : FALSE;
  dfe_st->seeded_dc = ((ctle_cal_cfg >> 4) & 1) ? TRUE : FALSE;

  dfe_st->dfe_disable = dfe_fixed;

  switch (cal_cmd) {
    case BF_SDS_RX_ICAL_PCAL:
      av_cal_cmd = AVAGO_DFE_ICAL;
      break;
    case BF_SDS_RX_ICAL_NO_PCAL:
      av_cal_cmd = AVAGO_DFE_ICAL_ONLY;
      break;
    case BF_SDS_RX_PCAL_ONCE:
      av_cal_cmd = AVAGO_DFE_PCAL;
      break;
    case BF_SDS_RX_PCAL_CONT_START:
      av_cal_cmd = AVAGO_DFE_START_ADAPTIVE;
      break;
    case BF_SDS_RX_PCAL_CONT_STOP:
      av_cal_cmd = AVAGO_DFE_STOP_ADAPTIVE;
      break;
    case BF_SDS_RX_PCAL_RR_ENABLE:
      av_cal_cmd = AVAGO_DFE_ENABLE_RR;
      break;
    case BF_SDS_RX_PCAL_RR_DISABLE:
      av_cal_cmd = AVAGO_DFE_DISABLE_RR;
      break;
    default:
      return -1;
  }

  dfe_st->tune_mode = av_cal_cmd;

  avago_serdes_dfe_tune(aapl, sbus_addr, dfe_st);

  avago_serdes_dfe_state_destruct(aapl, dfe_st);

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_rx_eq_cal_eye_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_eq_cal_eye_get(bf_dev_id_t dev_id,
                                     int ring,
                                     int sd,
                                     int *cal_eye) {
  //  Call avago_serdes_eye_get() with configp->ec_eye_type ==
  //  AVAGO_EYE_HEIGHT_DVOS. This retrieves testLEV results through Int 0x26
  int rc;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_eye_config_t *configp = NULL;
  Avago_serdes_eye_data_t *datap = NULL;

  if (!aapl) return -1;

  configp = avago_serdes_eye_config_construct(aapl);
  if (!configp) return -2;

  datap = avago_serdes_eye_data_construct(aapl);
  if (!datap) {
    avago_serdes_eye_config_destruct(aapl, configp);
    return -3;
  }

  configp->ec_eye_type = AVAGO_EYE_HEIGHT_DVOS;

  rc = avago_serdes_eye_get(aapl, sbus_addr, configp, datap);

  if (rc == 0) {
    *cal_eye = datap->ed_height_mV;
  }
  avago_serdes_eye_config_destruct(aapl, configp);
  avago_serdes_eye_data_destruct(aapl, datap);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_tx_fixed_pat_set
 *
 *************************************************************/
int port_mgr_av_sd_tx_fixed_pat_set(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    int fixed_pat[4]) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  long int av_fixed_pat[4];

  av_fixed_pat[0] = fixed_pat[0];
  av_fixed_pat[1] = fixed_pat[1];
  av_fixed_pat[2] = fixed_pat[2];
  av_fixed_pat[3] = fixed_pat[3];

  rc = avago_serdes_set_tx_user_data(aapl, sbus_addr, av_fixed_pat);

  return rc;
}

/*************************************************************
 * port_mgr_av_sd_tx_fixed_pat_get
 *
 *************************************************************/
int port_mgr_av_sd_tx_fixed_pat_get(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    int fixed_pat[4]) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  long int av_fixed_pat[4];

  rc = avago_serdes_get_tx_user_data(aapl, sbus_addr, (long int *)av_fixed_pat);

  fixed_pat[0] = (int)av_fixed_pat[0];
  fixed_pat[1] = (int)av_fixed_pat[1];
  fixed_pat[2] = (int)av_fixed_pat[2];
  fixed_pat[3] = (int)av_fixed_pat[3];

  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_patsel_set
 *
 *************************************************************/
int port_mgr_av_sd_rx_patsel_set(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 int rx_patsel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  int av_patsel;

  switch (rx_patsel) {
    case BF_SDS_PAT_PATSEL_PRBS7:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS7;
      break;
    case BF_SDS_PAT_PATSEL_PRBS9:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS9;
      break;
    case BF_SDS_PAT_PATSEL_PRBS11:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS11;
      break;
    case BF_SDS_PAT_PATSEL_PRBS15:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS15;
      break;
    case BF_SDS_PAT_PATSEL_PRBS23:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS23;
      break;
    case BF_SDS_PAT_PATSEL_PRBS31:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_PRBS31;
      break;
    case BF_SDS_PAT_PATSEL_FIXED:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_USER;
      break;
    case BF_SDS_PAT_PATSEL_OFF:
      av_patsel = AVAGO_SERDES_TX_DATA_SEL_CORE;
      break;

    default:
      return BF_INVALID_ARG;
  }

  rc = avago_serdes_set_rx_cmp_data(aapl, sbus_addr, av_patsel);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_patsel_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_patsel_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int av_patsel, rx_patsel;

  av_patsel = avago_serdes_get_rx_cmp_data(aapl, sbus_addr);

  switch (av_patsel) {
    case AVAGO_SERDES_TX_DATA_SEL_PRBS7:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS7;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS9:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS9;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS11:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS11;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS15:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS15;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS23:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS23;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_PRBS31:
      rx_patsel = BF_SDS_PAT_PATSEL_PRBS31;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_USER:
      rx_patsel = BF_SDS_PAT_PATSEL_FIXED;
      break;
    case AVAGO_SERDES_TX_DATA_SEL_CORE:
      rx_patsel = BF_SDS_PAT_PATSEL_OFF;
      break;
    default:
      return BF_INVALID_ARG;
  }
  return rx_patsel;
}

/*************************************************************
 * port_mgr_av_sd_rx_cmp_mode_set
 *
 *************************************************************/
int port_mgr_av_sd_rx_cmp_mode_set(bf_dev_id_t dev_id,
                                   int ring,
                                   int sd,
                                   int rx_patsel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  int mode;

  //      When PRBS is enabled,       mode =
  //      AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN
  //      When main data is selected, mode = AVAGO_SERDES_RX_CMP_MODE_OFF
  switch (rx_patsel) {
    case BF_SDS_PAT_PATSEL_PRBS7:
    case BF_SDS_PAT_PATSEL_PRBS9:
    case BF_SDS_PAT_PATSEL_PRBS11:
    case BF_SDS_PAT_PATSEL_PRBS15:
    case BF_SDS_PAT_PATSEL_PRBS23:
    case BF_SDS_PAT_PATSEL_PRBS31:
    case BF_SDS_PAT_PATSEL_FIXED:
      mode = AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN;
      break;
    default:
      mode = AVAGO_SERDES_RX_CMP_MODE_OFF;
      break;
  }

  rc = avago_serdes_set_rx_cmp_mode(aapl, sbus_addr, mode);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_cmp_mode_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_cmp_mode_get(bf_dev_id_t dev_id, int ring, int sd) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int av_mode;

#if 0
    AVAGO_SERDES_RX_CMP_DATA_PRBS7    = 0, /**< PRBS7 (x^7+x^6+1) generator. */
    AVAGO_SERDES_RX_CMP_DATA_PRBS9    = 1, /**< PRBS9 (x^9+x^7+1). */
    AVAGO_SERDES_RX_CMP_DATA_PRBS11   = 2, /**< PRBS11 (x^11+x^9+1). */
    AVAGO_SERDES_RX_CMP_DATA_PRBS15   = 3, /**< PRBS15 (x^15+x^14+1). */
    AVAGO_SERDES_RX_CMP_DATA_PRBS23   = 4, /**< PRBS23 (x^23+x^18+1). */
    AVAGO_SERDES_RX_CMP_DATA_PRBS31   = 5, /**< PRBS31 (x^31+x^28+1). */
    AVAGO_SERDES_RX_CMP_DATA_SELF_SEED= 7, /**< Auto-seed to received 40 bit repeating pattern. */
                                           /**< NOTE: This is USER mode in firmware. */
    AVAGO_SERDES_RX_CMP_DATA_OFF      = 8  /**< Disable cmp data generator */

//Avago_serdes_rx_cmp_data_t avago_serdes_get_rx_cmp_data(Aapl_t *aapl, uint sbus_addr);
#endif
  av_mode = avago_serdes_get_rx_cmp_mode(aapl, sbus_addr);
  return av_mode;
}

/*************************************************************
 * port_mgr_av_sd_rx_data_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_data_get(bf_dev_id_t dev_id,
                               int ring,
                               int sd,
                               int rx_data[4]) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int rc;
  long int av_rx_data[4];

  rc = avago_serdes_get_rx_data(aapl, sbus_addr, (long *)av_rx_data);

  rx_data[0] = (int)av_rx_data[0];
  rx_data[1] = (int)av_rx_data[1];
  rx_data[2] = (int)av_rx_data[2];
  rx_data[3] = (int)av_rx_data[3];

  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_eye_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_eye_get(bf_dev_id_t dev_id,
                              int ring,
                              int sd,
                              bf_sds_rx_eye_meas_mode_t mode,
                              bf_sds_rx_eye_meas_ber_t ber,
                              int *meas_eye) {
  int rc;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_eye_config_t *configp = NULL;
  Avago_serdes_eye_data_t *datap = NULL;

  if (!meas_eye) return -1;
  if (!aapl) return -2;

  configp = avago_serdes_eye_config_construct(aapl);
  if (!configp) return -3;

  datap = avago_serdes_eye_data_construct(aapl);
  if (!datap) {
    avago_serdes_eye_config_destruct(aapl, configp);
    return -4;
  }

  if (mode == BF_SDS_RX_EYE_MEAS_HEIGHT) {
    configp->ec_eye_type = AVAGO_EYE_HEIGHT;
  } else if (mode == BF_SDS_RX_EYE_MEAS_WIDTH) {
    configp->ec_eye_type = AVAGO_EYE_WIDTH;
  } else {
    rc = -5;
    goto exit;
  }
  if (ber == BF_SDS_RX_EYE_BER_1E6) {
    configp->ec_max_dwell_bits = 1000000;
  } else if (ber == BF_SDS_RX_EYE_BER_1E9) {
    configp->ec_max_dwell_bits = 1000000000;
  } else {
    rc = -6;
    goto exit;
  }

  configp->ec_min_dwell_bits = 1000;
  configp->ec_error_threshold = 2;

  rc = avago_serdes_eye_get(aapl, sbus_addr, configp, datap);

  if (rc == 0) {
    if (mode == BF_SDS_RX_EYE_MEAS_HEIGHT) {
      *meas_eye = datap->ed_height_mV;
    } else {
      *meas_eye = datap->ed_width_mUI;
    }
  }
exit:
  avago_serdes_eye_config_destruct(aapl, configp);
  avago_serdes_eye_data_destruct(aapl, datap);
  return rc;
}

/*************************************************************
 * port_mgr_av_sd_rx_full_eye_get
 *
 *************************************************************/
int port_mgr_av_sd_rx_full_eye_get(bf_dev_id_t dev_id,
                                   int ring,
                                   int sd,
                                   bf_sds_rx_eye_meas_ber_t ber,
                                   char *meas_eye,
                                   int eye_data_max_len) {
  int rc;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  Avago_serdes_eye_data_t *datap = NULL;
  Avago_serdes_eye_config_t *configp = NULL;

  if (!meas_eye) return -1;
  if (!aapl) return -2;

  configp = avago_serdes_eye_config_construct(aapl);
  if (!configp) return -3;

  datap = avago_serdes_eye_data_construct(aapl);
  if (!datap) {
    avago_serdes_eye_config_destruct(aapl, configp);
    return -4;
  }

  configp->ec_eye_type = AVAGO_EYE_FULL;

  if (ber == BF_SDS_RX_EYE_BER_1E6) {
    configp->ec_max_dwell_bits = 1000000;
  } else if (ber == BF_SDS_RX_EYE_BER_1E9) {
    configp->ec_max_dwell_bits = 1000000000;
  } else {
    rc = -5;
    goto exit;
  }

  rc = avago_serdes_eye_get(aapl, sbus_addr, configp, datap);

  if (rc == 0) {
    char *eye_text = avago_serdes_eye_plot_format(datap);
    snprintf(meas_eye, eye_data_max_len, "%s\n", eye_text);
    AAPL_FREE(eye_text);
  }
exit:
  avago_serdes_eye_config_destruct(aapl, configp);
  avago_serdes_eye_data_destruct(aapl, datap);
  return rc;
}

// some proto's conditionally compiled it seems
extern int avago_serdes_step_phase(Aapl_t *aapl,
                                   uint sbus_addr,
                                   int new_phase,
                                   int *current_phase,
                                   BOOL get_errors);
extern int avago_serdes_set_dac(Aapl_t *aapl,
                                uint sbus_addr,
                                uint dac,
                                BOOL get_errors);
extern uint avago_serdes_get_phase_multiplier(Aapl_t *aapl, uint sbus_addr);
extern int avago_serdes_get_phase(Aapl_t *aapl, uint sbus_addr);

/*************************************************************
 * port_mgr_av_sd_offset_set
 *
 *************************************************************/
int port_mgr_av_sd_offset_set(
    bf_dev_id_t dev_id, int ring, int sd, int x, int y) {
  int rc, cur_x;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  // get current pos
  cur_x = avago_serdes_get_phase(aapl, sbus_addr);

  // set x/y pos
  // note: assume (for now) "center" == 0
  rc = avago_serdes_step_phase(aapl, sbus_addr, x, &cur_x, FALSE);
  if (rc != 0) return -1;

  rc = avago_serdes_set_dac(aapl, sbus_addr, y, FALSE);
  if (rc != 0) return -1;

  return 0;
}

/*************************************************************
* port_mgr_av_sd_offset_step_set

 * \param[in]  pos_x     : Horizontal Position ( -32.. 31 phase setting)
 * \param[in]  pos_y     : Vertical Position   (-450..450 mV)

  //      Call avago_serdes_set_rx_cmp_mode() to set cmp_mode to
  //          AVAGO_SERDES_RX_CMP_MODE_XOR
  //
  //      Call avago_serdes_get_phase_multiplier() to determine
  //      how many phase steps are in an UI.  If divider=2, number of
  //      steps in one UI doubles.  At full rate, 1 UI is 64 steps
  //
  //      Call avago_serdes_step_phase() to adjust pos_x.
  //      Call avago_serdes_set_dac() to adjust pos_y.
  //          See aapl->eye.c->serdes_get_dvos_height() on how to
  //          convert mV to dac setting
  //              - Full dac range is 256 and it corresponds to 900mV (AVDD)
  //              - Average the 4 slicers (even/odd, 0 and 180 degree)
  //                  + some small offset.
*
*************************************************************/

extern int avago_serdes_get_dac_range(
    Aapl_t *aapl, /**< [in] Pointer to Aapl_t structure. */
    uint addr);

int port_mgr_av_sd_offset_step_set(
    bf_dev_id_t dev_id, int ring, int sd, int x, int y) {
  int rc, cur_x, set_x, mult, dac_range, dac_setting;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  if ((x < -32) || (x > 31)) return -1;
  if ((y < -450) || (y > 450)) return -1;

  rc = avago_serdes_set_rx_cmp_mode(
      aapl, sbus_addr, AVAGO_SERDES_RX_CMP_MODE_XOR);
  if (rc != 0) return -1;

  // get current pos
  cur_x = avago_serdes_get_phase(aapl, sbus_addr);

  // get multiplier, 1 if 25g, 2 or more if lower speed
  mult = avago_serdes_get_phase_multiplier(aapl, sbus_addr);

  // scale callers phase setting
  set_x = (x * mult);

  // set x/y pos
  rc = avago_serdes_step_phase(aapl, sbus_addr, set_x, &cur_x, FALSE);
  if (rc != 0) return -1;

  dac_range = avago_serdes_get_dac_range(aapl, sbus_addr);

  // scale requested dac setting
  dac_setting = ((y + 450) * dac_range) / 900;

  rc = avago_serdes_set_dac(aapl, sbus_addr, dac_setting, FALSE);
  if (rc != 0) return -1;

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_temp_read_start
 *
 *************************************************************/
void port_mgr_av_sd_temp_read_start(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    uint32_t channel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  avago_sensor_start_temperature(aapl, sbus_addr, channel, PMRO_FREQ);
}

/*************************************************************
 * port_mgr_av_sd_temp_read_get
 *
 *************************************************************/
int port_mgr_av_sd_temp_read_get(bf_dev_id_t dev_id,
                                 int ring,
                                 int sd,
                                 uint32_t channel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int temp;

  temp = avago_sensor_wait_temperature(aapl, sbus_addr, channel);
  if (temp == -1000000) return -1;

  return temp;  // in degrees mC
}

/*************************************************************
 * port_mgr_av_sd_voltage_read_start
 *
 *************************************************************/
void port_mgr_av_sd_voltage_read_start(bf_dev_id_t dev_id,
                                       int ring,
                                       int sd,
                                       uint32_t channel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  avago_sensor_start_voltage(aapl, sbus_addr, channel, PMRO_FREQ);
}

/*************************************************************
 * port_mgr_av_sd_voltage_read_get
 *
 *************************************************************/
int port_mgr_av_sd_voltage_read_get(bf_dev_id_t dev_id,
                                    int ring,
                                    int sd,
                                    uint32_t channel) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  int voltage;

  voltage = avago_sensor_wait_voltage(aapl, sbus_addr, channel);
  return voltage;  // -1 or voltage mV
}

/*************************************************************
 * port_mgr_av_sd_reset
 *
 *************************************************************/
void port_mgr_av_sd_reset(
    bf_dev_id_t dev_id, int ring, int sd, bool node_reset, bool microp_reset) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  if (!sd_spico_int_mutex_initd) {
    bf_sys_rmutex_init(&sd_spico_int_mutex);
    sd_spico_int_mutex_initd = 1;
  }
  bf_sys_rmutex_lock(&sd_spico_int_mutex);

  if (node_reset) {
    avago_sbus_reset(aapl, sbus_addr, 0); /* soft SBus reset */
  }
  if (microp_reset) {
    avago_spico_reset(aapl, sbus_addr);
  }

  bf_sys_rmutex_unlock(&sd_spico_int_mutex);
}

/*************************************************************
 * port_mgr_av_sd_encode_bitrate_and_width
 *
 *************************************************************/
void port_mgr_av_sd_encode_bitrate_and_width(int speed,
                                             uint32_t *bit_rate_code,
                                             uint32_t *data_width) {
  switch (speed) {
    case 1:
      *bit_rate_code = 0x0008;
      *data_width = 10;
      break;
    case 10:
      *bit_rate_code = 0x0042;
      *data_width = 20;
      break;
    case 25:
      *bit_rate_code = 0x00A5;
      *data_width = 40;
      break;
    case 125:  // special-case for 125Mhz AN
      *bit_rate_code = 0x0008;
      *data_width = 20;
      break;
    case 3125:  // special-case for 3.125Ghz AN
      *bit_rate_code = 0x0014;
      *data_width = 20;
      break;
    default:
      // in asymmetric mode, one side may get programmed
      // when the other has not been set to any port speed,
      // so "speed" (above) could be 0. In that case just
      // return the values for 10g. It will get corrected
      // when the other side is programmed.
      *bit_rate_code = 0x0042;
      *data_width = 20;
      break;
  }
}

/*************************************************************
 * port_mgr_av_sd_pgm_symmetric
 *
 *************************************************************/
void port_mgr_av_sd_pgm_symmetric(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  int speed,
                                  bool tx_output_en,
                                  float pll_ovrclk) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t bit_rate_code, data_width;

  port_mgr_av_sd_encode_bitrate_and_width(speed, &bit_rate_code, &data_width);
  bit_rate_code = (uint32_t)((bit_rate_code * (100 + pll_ovrclk)) / 100);
  if (bit_rate_code > 180) bit_rate_code = 180;
  bit_rate_code |= 0x8000;  // apply to both rx and tx

  // Test, always set "not slave"
  bit_rate_code |= 0x1000;

  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, FALSE, FALSE, FALSE); /* Disable serdes */
  avago_spico_int(aapl,
                  sbus_addr,
                  0x05,
                  bit_rate_code); /* set serdes bit/ref ratio = 10G */
  avago_serdes_set_tx_rx_width(
      aapl, sbus_addr, data_width, data_width); /* 20bit width */
  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, TRUE, TRUE, (tx_output_en ? TRUE : FALSE));

  // test PLL bbGain setting based on speed
  port_mgr_av_sd_pll_bbgain_set(dev_id, ring, sd, (speed == 10));
}

/*************************************************************
 * port_mgr_av_sd_pgm_asymmetric_tx
 *
 *************************************************************/
void port_mgr_av_sd_pgm_asymmetric_tx(bf_dev_id_t dev_id,
                                      int ring,
                                      int sd,
                                      int speed,
                                      int rx_speed,
                                      int rx_en,
                                      bool tx_output_en,
                                      float pll_ovrclk) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t bit_rate_code, data_width;
  uint32_t rx_bit_rate_code, rx_data_width;

  port_mgr_av_sd_encode_bitrate_and_width(speed, &bit_rate_code, &data_width);
  bit_rate_code = (uint32_t)((bit_rate_code * (100 + pll_ovrclk)) / 100);
  if (bit_rate_code > 180) bit_rate_code = 180;
  port_mgr_av_sd_encode_bitrate_and_width(
      rx_speed, &rx_bit_rate_code, &rx_data_width);

  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, FALSE, rx_en, FALSE); /* Disable serdes */
  avago_spico_int(
      aapl, sbus_addr, 0x05, bit_rate_code); /* set tx bit/ref ratio */
  avago_serdes_set_tx_rx_width(
      aapl, sbus_addr, data_width, rx_data_width); /* set data width */
  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, TRUE, rx_en, (tx_output_en ? TRUE : FALSE));
}

/*************************************************************
 * port_mgr_av_sd_pgm_asymmetric_rx
 *
 *************************************************************/
void port_mgr_av_sd_pgm_asymmetric_rx(bf_dev_id_t dev_id,
                                      int ring,
                                      int sd,
                                      int speed,
                                      int tx_speed,
                                      int tx_en,
                                      float pll_ovrclk) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t bit_rate_code, data_width;
  uint32_t tx_bit_rate_code, tx_data_width;

  port_mgr_av_sd_encode_bitrate_and_width(speed, &bit_rate_code, &data_width);
  bit_rate_code = (uint32_t)((bit_rate_code * (100 + pll_ovrclk)) / 100);
  if (bit_rate_code > 180) bit_rate_code = 180;
  bit_rate_code &= 0xFF;
  port_mgr_av_sd_encode_bitrate_and_width(
      tx_speed, &tx_bit_rate_code, &tx_data_width);

  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, tx_en, FALSE, tx_en); /* Disable serdes rx */
  avago_spico_int(
      aapl, sbus_addr, 0x06, bit_rate_code); /* set rx bit/ref ratio */
  avago_serdes_set_tx_rx_width(
      aapl, sbus_addr, tx_data_width, data_width); /* set data width */
  avago_serdes_set_tx_rx_enable(
      aapl, sbus_addr, tx_en, TRUE, tx_en); /* enable serdes */
}

/*************************************************************
 * port_mgr_av_sd_tx_rx_en_get
 *
 * Huge hack here. These bits are really tx_rdy/rx_rdy i think
 *************************************************************/
void port_mgr_av_sd_tx_rx_en_get(
    bf_dev_id_t dev_id, int ring, int sd, bool *tx_en, bool *rx_en) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  uint32_t mem_val;

  /* [0] TX_RDY, [1] RX_RDY */
  mem_val = avago_serdes_mem_rd(aapl, sbus_addr, AVAGO_LSB, 0x26);
  *tx_en = (mem_val & 1) ? true : false;
  *rx_en = (mem_val & 2) ? true : false;
}

/*************************************************************
 * port_mgr_av_sd_tx_rx_en_set
 *
 *************************************************************/
void port_mgr_av_sd_tx_rx_en_set(
    bf_dev_id_t dev_id, int ring, int sd, bool tx_en, bool rx_en) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);
  BOOL rx, tx, out_en = avago_serdes_get_tx_output_enable(aapl, sbus_addr);

  rx = rx_en ? TRUE : FALSE;
  tx = tx_en ? TRUE : FALSE;
  avago_serdes_set_tx_rx_enable(aapl, sbus_addr, tx, rx, out_en);
}

/*************************************************************
 * port_mgr_av_sd_clause_92_training_set
 *
 *************************************************************/
void port_mgr_av_sd_clause_92_training_set(bf_dev_id_t dev_id,
                                           int ring,
                                           int sd,
                                           uint32_t pcs_lane) {
  uint32_t seed;

  switch (pcs_lane) {
    case 3: {
      seed = 0x7b6 /*b111_1011_0110*/;
      break;
    }
    case 2: {
      seed = 0x72d /*b111_0010_1101*/;
      break;
    }
    case 1: {
      seed = 0x645 /*b110_0100_0101*/;
      break;
    }
    case 0:
    default: {
      seed = 0x57e /*b101_0111_1110*/;
      break;
    }
  }
  /* Configure PRBS pattern */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3d, 0x3000 | pcs_lane);
  /* Configure PRBS seed */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3d, 0x4000 | seed);
  /* Configure Repeating PRBS pattern */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3d, 0x2000 | 0x01);
}

/*************************************************************
 * port_mgr_av_sd_clause_72_training_set
 *
 *************************************************************/
void port_mgr_av_sd_clause_72_training_set(bf_dev_id_t dev_id,
                                           int ring,
                                           int sd,
                                           uint32_t pcs_lane) {
  uint32_t seed;

  switch (pcs_lane) {
    case 3: {
      seed = 0x7b6 /*b111_1011_0110*/;
      break;
    }
    case 2: {
      seed = 0x72d /*b111_0010_1101*/;
      break;
    }
    case 1: {
      seed = 0x645 /*b110_0100_0101*/;
      break;
    }
    case 0:
    default: {
      seed = 0x57e /*b101_0111_1110*/;
      break;
    }
  }
  /* Configure PRBS pattern */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3d, 0x3000 | 0x4);
  /* Configure PRBS seed */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3d, 0x4000 | seed);
  /* Configure Random, reseeded pattern per frame */
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x3d, 0x2000 | 0x03);
}

/*************************************************************
 * port_mgr_av_sd_eye_metric_get
 *
 *************************************************************/
int port_mgr_av_sd_eye_metric_get(bf_dev_id_t dev_id,
                                  int ring,
                                  int sd,
                                  uint32_t *eye_metric) {
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  *eye_metric = avago_serdes_eye_get_simple_metric(aapl, sbus_addr);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_delay_cal
 *
 *************************************************************/
int port_mgr_av_sd_delay_cal(bf_dev_id_t dev_id, int ring, int sd) {
  port_mgr_av_sd_spico_int(dev_id, ring, sd, 0x2f, 0x101);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_vbtc_get
 *
 *************************************************************/
int port_mgr_av_sd_vbtc_get(bf_dev_id_t dev_id,
                            int ring,
                            int sd,
                            int *eye_ht_1e06,
                            int *eye_ht_1e10,
                            int *eye_ht_1e12,
                            int *eye_ht_1e15,
                            int *eye_ht_1e17) {
  Avago_serdes_eye_config_t *eye_config;
  Avago_serdes_eye_data_t *eye_data;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  eye_config = avago_serdes_eye_config_construct(aapl);
  eye_data = avago_serdes_eye_data_construct(aapl);

  eye_config->ec_eye_type = AVAGO_EYE_HEIGHT;
  eye_config->ec_no_sbm = TRUE;

  if (avago_serdes_eye_get(aapl, sbus_addr, eye_config, eye_data) < 0) {
    avago_serdes_eye_data_destruct(aapl, eye_data);
    avago_serdes_eye_config_destruct(aapl, eye_config);
    return -1;
  }

  if (eye_data->ed_vbtc.top_points == 0 ||
      eye_data->ed_vbtc.bottom_points == 0) {
    avago_serdes_eye_data_destruct(aapl, eye_data);
    avago_serdes_eye_config_destruct(aapl, eye_config);
    return -2;
  }
  if (eye_data->ed_vbtc.bottom_R_squared < 0.95 ||
      eye_data->ed_vbtc.top_R_squared < 0.95 ||
      eye_data->ed_vbtc.bottom_slope <= 0.0 ||
      eye_data->ed_vbtc.top_slope >= 0.0) {
    avago_serdes_eye_data_destruct(aapl, eye_data);
    avago_serdes_eye_config_destruct(aapl, eye_config);
    return -3;
  }
  *eye_ht_1e06 = eye_data->ed_vbtc.vert_eye_1e06;
  *eye_ht_1e10 = eye_data->ed_vbtc.vert_eye_1e10;
  *eye_ht_1e12 = eye_data->ed_vbtc.vert_eye_1e12;
  *eye_ht_1e15 = eye_data->ed_vbtc.vert_eye_1e15;
  *eye_ht_1e17 = eye_data->ed_vbtc.vert_eye_1e17;

  // normalize
  if (*eye_ht_1e06 < 0) *eye_ht_1e06 = 0;
  if (*eye_ht_1e10 < 0) *eye_ht_1e10 = 0;
  if (*eye_ht_1e12 < 0) *eye_ht_1e12 = 0;
  if (*eye_ht_1e15 < 0) *eye_ht_1e15 = 0;
  if (*eye_ht_1e17 < 0) *eye_ht_1e17 = 0;

  avago_serdes_eye_data_destruct(aapl, eye_data);
  avago_serdes_eye_config_destruct(aapl, eye_config);
  return 0;
}

static int port_mgr_av_sd_map_eye_prep(bf_dev_id_t dev_id, int ring, int sd) {
  int rc, dac_range;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  rc = avago_serdes_set_rx_cmp_mode(
      aapl, sbus_addr, AVAGO_SERDES_RX_CMP_MODE_XOR);
  if (rc != 0) return -1;

  dac_range = avago_serdes_get_dac_range(aapl, sbus_addr);
  return dac_range;
}

static int port_mgr_av_sd_map_eye_y_set(
    bf_dev_id_t dev_id, int ring, int sd, int y, int dac_range) {
  int rc, dac_setting;
  Aapl_t *aapl = port_mgr_av_sd_get_aapl(dev_id);
  int sbus_addr = port_mgr_av_sd_encode_sbus_addr(dev_id, ring, sd);

  // scale requested dac setting
  dac_setting = ((y + 450) * dac_range) / 900;

  rc = avago_serdes_set_dac(aapl, sbus_addr, dac_setting, FALSE);
  if (rc != 0) return -1;

  return 0;
}

/*************************************************************
 * port_mgr_av_sd_map_eye
 *
 *************************************************************/
int port_mgr_av_sd_map_eye(bf_dev_id_t dev_id, int ring, int sd) {
  int offset, dac_range;
  uint32_t cnt = 1;

  port_mgr_log("Map eye: start");
  dac_range = port_mgr_av_sd_map_eye_prep(dev_id, ring, sd);
  // Start at +/- 450mV (900mV swing)
  offset = 450;
  while ((cnt > 0) && (offset > 0)) {
    port_mgr_av_sd_map_eye_y_set(dev_id, ring, sd, offset, dac_range);
    port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    bf_sys_usleep(4);
    cnt = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    printf("Coarse: offset=%d : err=%d\n", offset, cnt);
    port_mgr_log("Coarse: offset=%d : err=%d", offset, cnt);

    if (cnt > 0) {
      offset -= 100;
    } else {
      offset += 100;  // go back to last failing offset
    }
  }
  cnt = 1;
  offset -= 10;
  while ((cnt > 0) && (offset > 0)) {
    port_mgr_av_sd_map_eye_y_set(dev_id, ring, sd, offset, dac_range);
    port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    bf_sys_usleep(40);
    cnt = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    printf("Fine  : offset=%d : err=%d\n", offset, cnt);
    port_mgr_log("Fine  : offset=%d : err=%d", offset, cnt);

    if (cnt > 0) {
      offset -= 10;
    } else {
      offset += 10;
    }
  }
  cnt = 1;
  offset -= 1;
  while ((cnt > 0) && (offset > 0)) {
    port_mgr_av_sd_map_eye_y_set(dev_id, ring, sd, offset, dac_range);
    port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    bf_sys_usleep(400);
    cnt = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    printf("Fine  : offset=%d : err=%d\n", offset, cnt);
    port_mgr_log("Fine  : offset=%d : err=%d", offset, cnt);

    if (cnt > 0) {
      offset -= 1;
    } else {
      // offset += 1;
    }
  }
  port_mgr_log("Map eye: end");
  if (offset < 0) {
    offset = 0;
  }
  printf("Upper eye edge: +/- %d mV (ht=%dmV)\n", offset, 2 * offset);
  port_mgr_log("Upper eye edge: +/- %d mV (ht=%dmV)", offset, 2 * offset);
  return 0;
}

/*************************************************************
 * port_mgr_av_sd_map_eye_quick
 *
 *************************************************************/
int port_mgr_av_sd_map_eye_quick(bf_dev_id_t dev_id, int ring, int sd) {
  int offset, dac_range;
  uint32_t cnt = 1;

  dac_range = port_mgr_av_sd_map_eye_prep(dev_id, ring, sd);
  // Start at +/- 450mV (900mV swing)
  offset = 450;
  while ((cnt > 0) && (offset > 0)) {
    port_mgr_av_sd_map_eye_y_set(dev_id, ring, sd, offset, dac_range);
    port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    bf_sys_usleep(4);
    cnt = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    // port_mgr_log("Coarse: offset=%d : err=%d", offset, cnt);

    if (cnt > 0) {
      offset -= 100;
    } else {
      offset += 100;  // go back to last failing offset
    }
  }
  cnt = 1;
  offset -= 10;
  while ((cnt > 0) && (offset > 0)) {
    port_mgr_av_sd_map_eye_y_set(dev_id, ring, sd, offset, dac_range);
    port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    bf_sys_usleep(40);
    cnt = port_mgr_av_sd_get_error_count(dev_id, ring, sd);
    // port_mgr_log("Fine  : offset=%d : err=%d", offset, cnt);

    if (cnt > 0) {
      offset -= 10;
    } else {
      offset += 10;
    }
  }
  if (offset < 0) {
    offset = 0;
  }
  port_mgr_log("Upper eye edge: +/- %d mV (ht=%dmV)", offset, 2 * offset);
  return (2 * offset);
}
