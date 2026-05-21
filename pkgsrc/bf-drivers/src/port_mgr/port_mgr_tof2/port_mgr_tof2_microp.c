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
#include <inttypes.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <dvm/dvm_intf.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_log.h>
#include <lld/lld_reg_if.h>
#include <tof2_regs/tof2_reg_drv.h>
#include "eth400g_mac_rspec_access.h"
#include "eth100g_reg_rspec_access.h"

#define HOST_BLK_MICROP_ID 0x21
#define CPU_MICROP_ID 0x0
#define SEC_TO_100_USEC (10000U)
#define DAY_TO_100_USEC (24U * 3600U * SEC_TO_100_USEC)

extern bool microp_init_done;

// turn off once we know it works
static bool microp_read_verify = true;
static uint8_t static_img_buffer[32 * 1024] = {0};  // max RAM sz

static bf_status_t port_mgr_tof2_microp_reset_set(bf_dev_id_t dev_id,
                                                  uint32_t microp_id,
                                                  bool assert_reset);
static bf_status_t port_mgr_tof2_microp_pre_config(bf_dev_id_t dev_id,
                                                   uint32_t microp_id);
static bf_status_t port_mgr_tof2_microp_boot(bf_dev_id_t dev_id,
                                             uint32_t microp_id,
                                             uint8_t *ram_img,
                                             uint32_t ram_img_len);
static uint32_t port_mgr_tof2_microp_ram_base(uint32_t umac);
static bf_status_t port_mgr_tof2_microp_image_load(char *img_path,
                                                   uint8_t **img_buffer,
                                                   uint32_t *img_len);
static bf_status_t port_mgr_tof2_microp_halted_get(bf_dev_id_t dev_id,
                                                   uint32_t microp_id,
                                                   uint32_t *halted);
bf_status_t port_mgr_tof2_microp_pcie_debug_fifo_dump(bf_dev_id_t device_id,
                                                      uint8_t mode);

/********************************************************************
 *
 * Initialize all the tv80 micro-processors on this Tofino2
 *******************************************************************/
void port_mgr_tof2_microp_init(bf_dev_id_t dev_id,
                               bf_device_profile_t *profile) {
  uint32_t port;
  uint8_t *ram_img = NULL;
  uint32_t ram_img_len = 0;
  uint32_t microp_id;
  bf_status_t rc;

  port_mgr_log("uP: Terminate and log PCIE debug info");
  port_mgr_tof2_microp_pcie_debug_fifo_dump(dev_id, 0);

#ifdef DEVICE_IS_EMULATOR
  return;
#endif

  port_mgr_log("uP: RAM image: %s", profile->microp_prof.microp_fw);

  // read binary file into buffer
  rc = port_mgr_tof2_microp_image_load(
      profile->microp_prof.microp_fw, &ram_img, &ram_img_len);
  if (rc != BF_SUCCESS) return;
  if (ram_img_len == 0) return;
  if (ram_img == NULL) return;

  // for now dont init host tv80. leave pcie debug log in ram
  if (0) {
    // load host micro
    port_mgr_tof2_microp_pre_config(dev_id, HOST_BLK_MICROP_ID);
    rc = port_mgr_tof2_microp_boot(
        dev_id, HOST_BLK_MICROP_ID, ram_img, ram_img_len);
    if (rc != BF_SUCCESS) {
      port_mgr_log("uP: RAM image load failed. host uP left in reset");
    }
  }

  // load CPU UMAC micro
  port_mgr_tof2_microp_pre_config(dev_id, CPU_MICROP_ID);
  rc = port_mgr_tof2_microp_boot(dev_id, CPU_MICROP_ID, ram_img, ram_img_len);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: RAM image load failed. Cpu uP left in reset");
  }

  // load UMAC micros
  for (port = 1; port <= 32; port++) {
    port_mgr_tof2_microp_pre_config(dev_id, port);
    rc = port_mgr_tof2_microp_boot(dev_id, port, ram_img, ram_img_len);
    if (rc != BF_SUCCESS) {
      port_mgr_log("uP: RAM image load failed. p%02d uP left in reset", port);
    }
  }

  // check POST status (tv80 halts if POST fails
  // for (microp_id = 0; microp_id <= HOST_BLK_MICROP_ID; microp_id++) {
  for (microp_id = 0; microp_id < HOST_BLK_MICROP_ID; microp_id++) {
    uint32_t halted;

    rc = port_mgr_tof2_microp_halted_get(dev_id, microp_id, &halted);
    if (rc != BF_SUCCESS) {
      port_mgr_log("uP: p%02d POST check failed: %d", microp_id, rc);
      continue;
    }
    if (halted) {
      port_mgr_log("uP: p%02d POST failed: HALTED", microp_id);
    }
  }

  // allow programming thru tv80 now (if configured)
  microp_init_done = true;
}

/********************************************************************
 *
 * Return base address of microprocessor csr-mapped memory
 *******************************************************************/
static uint32_t port_mgr_tof2_microp_ram_base(uint32_t umac) {
  if (umac == HOST_BLK_MICROP_ID) {
    return offsetof(tof2_reg, device_select.misc_tv80_regs);
  } else if (umac == CPU_MICROP_ID) {
    return offsetof(tof2_reg, eth100g_regs.eth100g_tv80);
  } else {
    uint32_t stride =
        offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);

    return offsetof(tof2_reg, eth400g_p1.eth400g_tv80) + (stride * (umac - 1));
  }
}

/********************************************************************
 *
 * Assert or de-assert soft reset to one of the micro-processors
 *******************************************************************/
static bf_status_t port_mgr_tof2_microp_reset_set(bf_dev_id_t dev_id,
                                                  uint32_t microp_id,
                                                  bool assert_reset) {
  uint32_t soft_reset_reg, soft_reset_val;

  switch (microp_id) {
    case HOST_BLK_MICROP_ID: {
      uint32_t misc_soft_reset_ofs =
          offsetof(tof2_reg, device_select.misc_regs.soft_reset);
      lld_read_register(dev_id, misc_soft_reset_ofs, &soft_reset_reg);
      if (assert_reset) {
        soft_reset_reg |= (1 << 21);
      } else {
        // hack, leave host tv80 in reset
        return BF_SUCCESS;
        // soft_reset_reg &= ~(1 << 21);
      }
      lld_write_register(dev_id, misc_soft_reset_ofs, soft_reset_reg);
      break;
    }
    case CPU_MICROP_ID:
      eth100g_reg_rspec_eth_soft_reset_eth_swrst_get(
          dev_id, 0, &soft_reset_reg, &soft_reset_val, true);
      if (assert_reset) {
        soft_reset_val |= (1 << 2);
      } else {
        soft_reset_val &= ~(1 << 2);
      }
      eth100g_reg_rspec_eth_soft_reset_eth_swrst_set(
          dev_id, 0, &soft_reset_reg, soft_reset_val, true);
      break;
    default: {
      uint32_t port = microp_id;
      eth400g_mac_rspec_eth_soft_reset_eth_swrst_get(
          dev_id, port, &soft_reset_reg, &soft_reset_val, true);
      if (assert_reset) {
        soft_reset_val |= (1 << 2);
      } else {
        soft_reset_val &= ~(1 << 2);
      }
      eth400g_mac_rspec_eth_soft_reset_eth_swrst_set(
          dev_id, port, &soft_reset_reg, soft_reset_val, true);
      break;
    }
  }
  return BF_SUCCESS;
}

/********************************************************************
 *
 * Configure the debug log on one of the micro-processors prior to
 * enabling it
 * Current settings are:
 *  circular mode
 *  last 1KB of address space
 *  log everything
 *  no interrupts
 *  hd = tail = 0
 *  wdog disabled
 *  stall on error or mbe, NOT "absolute"
 *******************************************************************/
static bf_status_t port_mgr_tof2_microp_pre_config_debug_log(
    bf_dev_id_t dev_id, uint32_t microp_id) {
  uint32_t cfg_wd, cfg_val;
  uint32_t port = microp_id;

  switch (microp_id) {
    case HOST_BLK_MICROP_ID:
      break;
    case CPU_MICROP_ID:
      // read in current settings
      eth100g_reg_rspec_tv80_debug_ctrl_base_addr_get(
          dev_id, port, &cfg_wd, &cfg_val, true);
      cfg_val = (0x4000 - 0x400);  // 1KB of log area
      eth100g_reg_rspec_tv80_debug_ctrl_base_addr_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 0x3fff;
      eth100g_reg_rspec_tv80_debug_ctrl_limit_addr_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 1;
      eth100g_reg_rspec_tv80_debug_ctrl_log_reg_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      eth100g_reg_rspec_tv80_debug_ctrl_log_inst_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 0;
      eth100g_reg_rspec_tv80_debug_ctrl_fifomode_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      eth100g_reg_rspec_tv80_debug_ctrl_full_int_set(
          dev_id, port, &cfg_wd, cfg_val, true);
      cfg_val = 1;
      eth100g_reg_rspec_tv80_debug_ctrl_enable_set(
          dev_id, port, &cfg_wd, cfg_val, true);

      cfg_val = 0;
      eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr_set(
          dev_id, port, &cfg_wd, cfg_val, true);
      eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr_set(
          dev_id, port, &cfg_wd, cfg_val, true);

      cfg_val = 0;
      eth100g_reg_rspec_tv80_watchdog_ctrl_enable_rmw(
          dev_id, port, &cfg_wd, cfg_val);

      eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_get(
          dev_id, port, &cfg_wd, &cfg_val, true);
      cfg_val = 0;
      eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 1;
      eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr_set(
          dev_id, port, &cfg_wd, cfg_val, true);
      break;
    default:
      // read in current settings
      eth400g_mac_rspec_tv80_debug_ctrl_base_addr_get(
          dev_id, port, &cfg_wd, &cfg_val, true);
      cfg_val = (0x40000 - 0x400);  // 1KB of log area
      eth400g_mac_rspec_tv80_debug_ctrl_base_addr_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 0x3fffc;
      eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 1;
      eth400g_mac_rspec_tv80_debug_ctrl_log_reg_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      eth400g_mac_rspec_tv80_debug_ctrl_log_inst_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 0;
      eth400g_mac_rspec_tv80_debug_ctrl_fifomode_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      eth400g_mac_rspec_tv80_debug_ctrl_full_int_set(
          dev_id, port, &cfg_wd, cfg_val, true);
      cfg_val = 1;
      eth400g_mac_rspec_tv80_debug_ctrl_enable_set(
          dev_id, port, &cfg_wd, cfg_val, true);

      cfg_val = 0;
      eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_set(
          dev_id, port, &cfg_wd, cfg_val, true);
      eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_set(
          dev_id, port, &cfg_wd, cfg_val, true);

      cfg_val = 0;
      eth400g_mac_rspec_tv80_watchdog_ctrl_enable_rmw(
          dev_id, port, &cfg_wd, cfg_val);

      eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_get(
          dev_id, port, &cfg_wd, &cfg_val, true);
      cfg_val = 0;
      eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      cfg_val = 1;
      eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_set(
          dev_id, port, &cfg_wd, cfg_val, false);
      eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_set(
          dev_id, port, &cfg_wd, cfg_val, true);
      break;
  }
  return BF_SUCCESS;
}

/********************************************************************
 *
 * Check the halted status of one of the micro-processors
 *******************************************************************/
static bf_status_t port_mgr_tof2_microp_halted_get(bf_dev_id_t dev_id,
                                                   uint32_t microp_id,
                                                   uint32_t *halted) {
  uint32_t halted_wd;

  switch (microp_id) {
    case HOST_BLK_MICROP_ID:
      *halted = true;  // FIXME
      break;
    case CPU_MICROP_ID:
      eth100g_reg_rspec_tv80_halted_status_tv80_halted_get(
          dev_id, microp_id, &halted_wd, halted, true);
      break;
    default:
      eth400g_mac_rspec_tv80_halted_status_tv80_halted_get(
          dev_id, microp_id, &halted_wd, halted, true);
      break;
  }
  return BF_SUCCESS;
}

/********************************************************************
 *
 * Configure one of the micro-processors prior to enabling it
 *******************************************************************/
static bf_status_t port_mgr_tof2_microp_pre_config(bf_dev_id_t dev_id,
                                                   uint32_t microp_id) {
  // first, assert its reset
  port_mgr_tof2_microp_reset_set(dev_id, microp_id, true);

  // configure the debug log
  port_mgr_tof2_microp_pre_config_debug_log(dev_id, microp_id);

  switch (microp_id) {
    case HOST_BLK_MICROP_ID:
    case CPU_MICROP_ID:
    default:
      break;
  }
  return BF_SUCCESS;
}

/********************************************************************
 *
 * Boot one of the micro-processors after loading the image
 *******************************************************************/
static bf_status_t port_mgr_tof2_microp_boot(bf_dev_id_t dev_id,
                                             uint32_t microp_id,
                                             uint8_t *ram_img,
                                             uint32_t ram_img_len) {
  uint32_t microp_ram_base = port_mgr_tof2_microp_ram_base(microp_id);
  uint32_t microp_addr;
  uint32_t *microp_img_ptr = (uint32_t *)ram_img;
  uint32_t img_wd;

  if (ram_img == NULL) return BF_INVALID_ARG;

  for (microp_addr = 0; microp_addr < ram_img_len; microp_addr += 4) {
    img_wd = *microp_img_ptr;
    lld_write_register(dev_id, (microp_ram_base + microp_addr), img_wd);

    if (microp_read_verify) {
      uint32_t r_wd;
      lld_read_register(dev_id, (microp_ram_base + microp_addr), &r_wd);
      if (r_wd != img_wd) {
        port_mgr_log(
            "uP: Read-verify error: microp offset=%08X : abs addr=%08X : "
            "exp=%08X : got=%08X",
            microp_addr,
            (microp_ram_base + microp_addr),
            img_wd,
            r_wd);
        return BF_HW_COMM_FAIL;
      }
    }
    microp_img_ptr++;
  }

  // de-assert soft reset
  port_mgr_tof2_microp_reset_set(dev_id, microp_id, false);

  port_mgr_log("uP: %s (%d) micro-processor booted",
               ((microp_id == HOST_BLK_MICROP_ID)
                    ? "HOST blk"
                    : (microp_id == CPU_MICROP_ID) ? "CPU" : "p"),
               microp_id);
  return BF_SUCCESS;
}

/********************************************************************
 *
 * Load the micro-processor binary image in to a RAM buffer
 * in prep for downloading the uPs.
 *******************************************************************/
static bf_status_t port_mgr_tof2_microp_image_load(char *img_path,
                                                   uint8_t **img_buffer,
                                                   uint32_t *img_len) {
  FILE *fp;
  char full_path_name[256] = {0};

  *img_buffer = static_img_buffer;
  *img_len = 0;

  snprintf(full_path_name, sizeof(full_path_name) - 1, "%s", img_path);
  // strncat(full_path_name, "/microp_fw.bin", sizeof(full_path_name) - 1);

  fp = fopen(full_path_name, "r");
  if (fp == NULL) {
    port_mgr_log("uP: Error opening: %s", full_path_name);
    return BF_INVALID_ARG;
  }
  *img_len = fread(*img_buffer, 1, sizeof(static_img_buffer), fp);
  fclose(fp);

  if (*img_len <= 0) {
    port_mgr_log("uP: Error reading: %s", full_path_name);
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/********************************************************************
* From the tv80 firmware:
*
                      66     66
                      67     67 ; Control and Status
0110  00 00           68     68 command: DEFW 0 ; [] 0=no-req, 1=mem-rd,
2=mem-wr, 3=io-rd, 4=io-wr
0112  00 00           69     69          DEFW 0 ; [] pad to 32 bits for easier
host access
0114  00 00           70     70 status:  DEFW 0 ; [] 0=idle, 1=busy, 2=done,
100-107, mskable int, 250=NMI
0116  00 00           71     71          DEFW 0 ; [] pad to 32 bits for easier
host access
0118  00 00           72     72 a00_15:  DEFW 0 ; [] Address[31:0]
011a  00 00           73     73 a16_31:  DEFW 0 ; []
011c  00 00           74     74 d00_15:  DEFW 0 ; [] Data[31:0]
011e  00 00           75     75 d16_31:  DEFW 0 ; []
                      76     76
                      77     77 ; verification data
                      78     78 ;
                      79     79 ;
0120  00 00           80     80 v_command: DEFW 0 ; [] Last command code
processed
0122  00 00           81     81 v_a00_15:  DEFW 0 ; [] Last requested
Address[31:0]
0124  00 00           82     82 v_a16_31:  DEFW 0 ; []
0126  00 00           83     83 v_d00_15:  DEFW 0 ; [] Write-only, Last
requested data
0128  00 00           84     84 v_d16_31:  DEFW 0 ; []
                      85     85 ;
                      86     86 ; POST variables
                      87     87 ;
012a  11 00           88     88 post_rd8:   DEFW 17   ;  0x11
012c  00 00           89     89 post_wr8:   DEFW 0    ;  should be 0x11 after
POST
012e  11 11           90     90 post_rd16:  DEFW 4369 ;  0x1111
0130  00 00           91     91 wr16:       DEFW 0    ;  should be 0x1111 after
POST
0132  00 00           92     92 wr16_2      DEFW 0    ;  should be 0x2222 after
POST
0134  00 00           93     93 wr16_3      DEFW 0    ;  should be 0x4444 after
POST
0136  00 00           94     94 wr16_4      DEFW 0    ;  should be 0x8888 after
POST
********************************************************************/
#define FW_CMD_OFS (0x110 / 4)
#define FW_STS_OFS (0x114 / 4)
#define FW_ADR_OFS (0x118 / 4)
#define FW_DTA_OFS (0x11C / 4)
#define FW_CMD_VERIF_OFS (0x120 / 4)
#define FW_ADR_VERIF_OFS (0x124 / 4)

#define FW_POST_WR8_VERIF (0x12C / 4)    /* after POST should be 0x11 */
#define FW_POST_WR16_VERIF_1 (0x130 / 4) /* after POST should be 0x11112222 */
#define FW_POST_WR16_VERIF_2 (0x134 / 4) /* after POST should be 0x44448888 */

/*****************************************************************************
 ****************************************************************************/
bf_status_t port_mgr_tof2_microp_wait_not_busy(bf_dev_id_t dev_id,
                                               uint32_t microp_id,
                                               uint32_t tries_left,
                                               uint32_t *sts_val) {
  bf_status_t rc;
  uint32_t microp_ram_base = port_mgr_tof2_microp_ram_base(microp_id);
  uint32_t sts_addr = microp_ram_base + FW_STS_OFS;

  do {
    rc = lld_read_register(dev_id, sts_addr, sts_val);
  } while ((rc == BF_SUCCESS) && (*sts_val == 1) && (--tries_left));

  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(sts): %d", microp_id, rc);
    return rc;
  }
  if (tries_left == 0) {
    port_mgr_log("uP: Error : p%02d BUSY", microp_id);
    return BF_HW_COMM_FAIL;
  }
  if ((*sts_val != 0) && (*sts_val != 2)) {
    port_mgr_log("uP: Error : p%02d STS error <%d>", microp_id, *sts_val);
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 ****************************************************************************/
bf_status_t port_mgr_tof2_microp_rd(bf_dev_id_t dev_id,
                                    uint32_t offset,
                                    uint32_t *r_data,
                                    uint32_t microp_id) {
  bf_status_t rc;
  uint32_t microp_ram_base = port_mgr_tof2_microp_ram_base(microp_id);
  uint32_t cmd_addr = microp_ram_base + FW_CMD_OFS;
  uint32_t adr_addr = microp_ram_base + FW_ADR_OFS;
  uint32_t dta_addr = microp_ram_base + FW_DTA_OFS;
  uint32_t sts_val, dta_val;
  uint32_t retries = 1000;  // max wait for previous cmd done

  rc = port_mgr_tof2_microp_wait_not_busy(dev_id, microp_id, retries, &sts_val);
  if (rc != BF_SUCCESS) return rc;

  rc = lld_write_register(dev_id, adr_addr, offset);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(adr): %d", microp_id, rc);
    return rc;
  }
  rc = lld_write_register(dev_id, cmd_addr, 3 /*rd*/);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(cmd): %d", microp_id, rc);
    return rc;
  }

  rc = port_mgr_tof2_microp_wait_not_busy(dev_id, microp_id, retries, &sts_val);
  if (rc != BF_SUCCESS) return rc;

  rc = lld_read_register(dev_id, dta_addr, &dta_val);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(data): %d", microp_id, rc);
    return rc;
  }
  *r_data = dta_val;  // return 32b of data
  return rc;
}

/*****************************************************************************
 ****************************************************************************/
bf_status_t port_mgr_tof2_microp_wr(bf_dev_id_t dev_id,
                                    uint32_t offset,
                                    uint32_t w_data,
                                    uint32_t microp_id) {
  bf_status_t rc;
  uint32_t microp_ram_base = port_mgr_tof2_microp_ram_base(microp_id);
  uint32_t cmd_addr = microp_ram_base + FW_CMD_OFS;
  uint32_t adr_addr = microp_ram_base + FW_ADR_OFS;
  uint32_t dta_addr = microp_ram_base + FW_DTA_OFS;
  uint32_t sts_val;
  uint32_t retries = 1000;  // max wait for previous cmd done

  rc = port_mgr_tof2_microp_wait_not_busy(dev_id, microp_id, retries, &sts_val);
  if (rc != BF_SUCCESS) return rc;

  rc = lld_write_register(dev_id, adr_addr, offset);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(adr): %d", microp_id, rc);
    return rc;
  }

  rc = lld_write_register(dev_id, dta_addr, w_data);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(data): %d", microp_id, rc);
    return rc;
  }
  rc = lld_write_register(dev_id, cmd_addr, 4 /*wr*/);
  if (rc != BF_SUCCESS) {
    port_mgr_log("uP: Error : p%02d access error(cmd): %d", microp_id, rc);
    return rc;
  }

  rc = port_mgr_tof2_microp_wait_not_busy(dev_id, microp_id, retries, &sts_val);
  return rc;
}

/*
When PCIe boots up and SW want to TV80, it will have to change the configuration
of TV80 memory (reading out of PCIe debug entries.

each entry is 32bit and log different events:
- [31:28]: event type
- [27:16]: event time in 100us increment
- [15:0]  : event data

event type list:
- 0001: time overflow (when the 12bit counter of 100us overflow) - bit [27:0] =
absolute time in 409.6 ms increment (4096*100us)
- 0010: reset0 change - event_data = dbg_reset[15:0] (is one of the misc_regs)
- 0011: reset1 change - event data = dbg_reset[31:16]
- 0101: LTSSM change (except detect.active) - event data[5:0] LTSSM encoding,
[7:6]: current rate, [11:8]: pipe_txelecidle, [15:12]: pipe_rxstandby
- 0110: Tx detect Rx - [3:0] latest TxDetectRx result(per lane), [7:4] Previous
TxDetectRx result(per lane), [15:8] consecutive identical result
- 0111: Error detected: [15:0] = error_type (see below)
- 1000: RxEQ lane 0 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1001: RxEQ lane 1 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1010: RxEQ lane 2 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1011: RxEQ lane 3 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1100: TxEQ lane 0 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor
- 1101: TxEQ lane 1 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor
- 1110: TxEQ lane 2 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor
- 1111: TxEQ lane 3 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor


Error_type [0]: deskew error
Error_type [1]: 128b/130b framing error (any)
Error_type [2]: received bad TLP
Error_type [3]: Rx Buffer overflow
Error_type [4]: NAK sent
Error_type [5]: Rx bad DLLP
Error_type [6]: Flow Control protocol error
Error_type [7]: Flow control timeout
Error_type [8]: Replay start
Error_type [9]: Replay number error
Error_type [10]: Replay timer error
Error_type [11]: NAK rcvd with correct SeqNum
Error_type [12]: TLP malformed
Error_type [13]: TLP Unexpected completion
Error_type [14]: TLP BAR no match
Error_type [15]: Unsupported TLP
*/

static char *ltssm_state[] = {
    "00h: detect.quiet",
    "01h: detect.active",
    "02h: polling.active",
    "03h: polling.compliance",
    "04h: polling.configuration",
    "05h: config.linkwidthstart",
    "06h: config.linkwidthaccept",
    "07h: config.lanenumwait",
    "08h: config.lanenumaccept",
    "09h: config.complete",
    "0Ah: config.idle",
    "0Bh: recovery.receiverlock",
    "0Ch: recovery.equalization (phase 0)",
    "0Dh: recovery.speed",
    "0Eh: recovery.receiverconfig",
    "0Fh: recovery.idle",
    "10h: L0",
    "11h: L0s",
    "12h: L1.entry",
    "13h: L1.idle",
    "14h: L2.idle/L2.transmitwake",
    "15h: reserved",
    "16h: disable",
    "17h: loopback.entry",
    "18h: loopback.active",
    "19h: loopback.exit",
    "1Ah: hotreset",
    "1Bh: ?",
    "1Ch: ?",
    "1Dh: ?",
    "1Eh: ?",
    "1Fh: ?",
    "20h: ?",
    "21h: recovery.equalization (phase 1)",
    "22h: recovery.equalization (phase 2)",
    "23h: recovery.equalization (phase 3)",
};

static char *rate_str[] = {
    "gen1 (2.5 Gbps): PCIe PHY PIPE 8-bit @ 250 MHz",
    "gen2 (5 Gbps): PCIe PHY PIPE 16-bit @ 250 MHz",
    "gen3 (8 Gbps): PCIe PHY PIPE 32-bit @ 250 MHz",
    "genX (invalid)",
};

static void decode_wd(uint16_t *current_entry_index,
                      uint64_t *current_fifo_time_100usec,
                      uint32_t wd) {
  uint32_t evt_typ, evt_tim, evt_dta;

  evt_typ = (wd >> 28) & 0xF;
  evt_tim = (wd >> 16) & 0xFFF;
  evt_dta = (wd & 0xFFFF);

  if (evt_typ == 1) {
    current_fifo_time_100usec[0] =
        (((((current_fifo_time_100usec[0] / 4096) & 0x0FFFFFFF) <=
           (wd & 0x0FFFFFFF))
              ? 0x0
              : ((uint64_t)4096 * 0x10000000)) +
         (current_fifo_time_100usec[0] & ~(0x0FFFFFFFFFF))) |
        ((uint64_t)4096 *
         (wd & 0x0FFFFFFF));  // take care of the 28-bit counter wraparound at
                              // ~3.5 years
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : time overflow : 12-bit counter of "
                 "100us overflowed. at least 409.6 ms had elapsed since last "
                 "timer overflow counter increment",
                 current_fifo_time_100usec[0] / DAY_TO_100_USEC,
                 current_fifo_time_100usec[0] / SEC_TO_100_USEC,
                 current_fifo_time_100usec[0] % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log(
        "    %07X : [27:0] absolute time in 409.6 ms increment (4096*100us)",
        (wd & 0x0FFFFFFF));
  } else if (evt_typ == 2) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : reset0 change",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log(
        "    %1X : [0]     : Power-On reset pin (after synchronous "
        "deassertion)",
        ((evt_dta >> 0) & 1));
    port_mgr_log("    %1X : [1]     : Core reset pin after debouncing",
                 ((evt_dta >> 1) & 1));
    port_mgr_log("    %1X : [2]     : PCIe reset pin after debouncing",
                 ((evt_dta >> 2) & 1));
    port_mgr_log("    %1X : [3]     : Power-On Done Status",
                 ((evt_dta >> 3) & 1));
    port_mgr_log("    %1X : [4]     : Power-On Done for PCIe only",
                 ((evt_dta >> 4) & 1));
    port_mgr_log("    %1X : [5]     : Fuse1 load", ((evt_dta >> 5) & 1));
    port_mgr_log("    %1X : [6]     : Fuse2 load", ((evt_dta >> 6) & 1));
    port_mgr_log("    %1X : [7]     : Fuse Reset", ((evt_dta >> 7) & 1));
    port_mgr_log("    %1X : [8]     : Fuse 1 Done", ((evt_dta >> 8) & 1));
    port_mgr_log("    %1X : [9]     : Fuse 2 Done", ((evt_dta >> 9) & 1));
    port_mgr_log("    %1X : [10]    : Fuse 1 Timeout", ((evt_dta >> 10) & 1));
    port_mgr_log(
        "    %1X : [13:11] : PCIe FSM State: COLDRST(000b), FIRMW(001b), "
        "PHY_WAIT(010b), CTL_ENA(011b), CTL_RST(100b), APP_RST(101b)",
        ((evt_dta >> 11) & 7));
    port_mgr_log("    %1X : [14]    : PCIe Controller functional reset",
                 ((evt_dta >> 14) & 1));
    port_mgr_log("    %1X : [15]    : PCIe application layer reset",
                 ((evt_dta >> 15) & 1));
  } else if (evt_typ == 3) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : reset1 change",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %1X : [16] : SPI Firmware Enable", ((evt_dta >> 0) & 1));
    port_mgr_log("    %1X : [17] : PCIe PHY initialization done",
                 ((evt_dta >> 1) & 1));
    port_mgr_log("    %1X : [18] : SPI Error captured during firmware loading",
                 ((evt_dta >> 2) & 1));
    port_mgr_log("    %1X : [19] : PCIe gen3 advertised after reset",
                 ((evt_dta >> 3) & 1));
    port_mgr_log("    %1X : [20] : Core PLL reset", ((evt_dta >> 4) & 1));
    port_mgr_log("    %1X : [21] : PPS PLL reset", ((evt_dta >> 5) & 1));
    port_mgr_log("    %1X : [22] : MAC 0 PLL reset", ((evt_dta >> 6) & 1));
    port_mgr_log("    %1X : [23] : MAC 1 PLL reset", ((evt_dta >> 7) & 1));
    port_mgr_log("    %1X : [24] : Core  reset", ((evt_dta >> 8) & 1));
    port_mgr_log("    %1X : [25] : MAC   reset", ((evt_dta >> 9) & 1));
    port_mgr_log("    %1X : [26] : MAC Bus reset", ((evt_dta >> 10) & 1));
    port_mgr_log("    %1X : [27] : Core PLL lock", ((evt_dta >> 11) & 1));
    port_mgr_log("    %1X : [28] : PPS PLL lock", ((evt_dta >> 12) & 1));
    port_mgr_log("    %1X : [29] : MAC 0 PLL lock", ((evt_dta >> 13) & 1));
    port_mgr_log("    %1X : [30] : MAC 1 PLL lock", ((evt_dta >> 14) & 1));
    port_mgr_log(
        "    %1X : [31] : Drive CCLK instead of PCLK to PCIe controller",
        ((evt_dta >> 15) & 1));
  } else if (evt_typ == 5) {
    uint32_t index = evt_dta & 0x3F;
    if (index >= sizeof(ltssm_state) / sizeof(ltssm_state[0])) {
      port_mgr_log_warn("Incorrect evt_dta");
      return;
    }  // end if
    port_mgr_log(
        "[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
        "] FIFO entry %4u : %08X : LTSSM change (except detect.active)",
        (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
        (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
        (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
        current_entry_index[0],
        wd);
    port_mgr_log(
        "    %02X : [ 5: 0] LTSSM encoding <%s>", index, ltssm_state[index]);
    port_mgr_log("    %2X : [ 7: 6] current rate is %s",
                 (evt_dta >> 6) & 3,
                 rate_str[(evt_dta >> 6) & 3]);
    port_mgr_log("    %2X : [11: 8] pipe_txelecidle", (evt_dta >> 8) & 0xF);
    port_mgr_log("    %2X : [15:12] pipe_rxstandby", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 6) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : Tx detect Rx",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %2X : [ 3:0] latest TxDetectRx result(per lane)",
                 evt_dta & 0xF);
    port_mgr_log("    %2X : [ 7:4] Previous TxDetectRx result(per lane)",
                 (evt_dta >> 4) & 0xF);
    port_mgr_log("    %02X : [15:8] consecutive identical result",
                 (evt_dta >> 8) & 0xFF);
  } else if (evt_typ == 7) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : Error detected",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %1X :  [0] : deskew error", ((evt_dta >> 0) & 1));
    port_mgr_log("    %1X :  [1] : 128b/130b framing error (any)",
                 ((evt_dta >> 1) & 1));
    port_mgr_log("    %1X :  [2] : received bad TLP", ((evt_dta >> 2) & 1));
    port_mgr_log("    %1X :  [3] : Rx Buffer overflow", ((evt_dta >> 3) & 1));
    port_mgr_log("    %1X :  [4] : NAK sent", ((evt_dta >> 4) & 1));
    port_mgr_log("    %1X :  [5] : Rx bad DLLP", ((evt_dta >> 5) & 1));
    port_mgr_log("    %1X :  [6] : Flow Control protocol error",
                 ((evt_dta >> 6) & 1));
    port_mgr_log("    %1X :  [7] : Flow control timeout", ((evt_dta >> 7) & 1));
    port_mgr_log("    %1X :  [8] : Replay start", ((evt_dta >> 8) & 1));
    port_mgr_log("    %1X :  [9] : Replay number error", ((evt_dta >> 9) & 1));
    port_mgr_log("    %1X : [10] : Replay timer error", ((evt_dta >> 10) & 1));
    port_mgr_log("    %1X : [11] : NAK rcvd with correct SeqNum",
                 ((evt_dta >> 11) & 1));
    port_mgr_log("    %1X : [12] : TLP malformed", ((evt_dta >> 12) & 1));
    port_mgr_log("    %1X : [13] : TLP Unexpected completion",
                 ((evt_dta >> 13) & 1));
    port_mgr_log("    %1X : [14] : TLP BAR no match", ((evt_dta >> 14) & 1));
    port_mgr_log("    %1X : [15] : Unsupported TLP", ((evt_dta >> 15) & 1));
  } else if (evt_typ == 8) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : RxEQ lane 0",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log(
        "    %02X : [ 7: 0] FOM (Figure of Merit: 0xFF best, 0x00 worst)",
        evt_dta & 0xFF);
    port_mgr_log("    %02X : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("    %2X : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 9) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : RxEQ lane 1",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log(
        "    %02X : [ 7: 0] FOM (Figure of Merit: 0xFF best, 0x00 worst)",
        evt_dta & 0xFF);
    port_mgr_log("    %02X : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("    %2X : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 10) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : RxEQ lane 2",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log(
        "    %02X : [ 7: 0] FOM (Figure of Merit: 0xFF best, 0x00 worst)",
        evt_dta & 0xFF);
    port_mgr_log("    %02X : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("    %2X : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 11) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : RxEQ lane 3",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log(
        "    %02X : [ 7: 0] FOM (Figure of Merit: 0xFF best, 0x00 worst)",
        evt_dta & 0xFF);
    port_mgr_log("    %02X : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("    %2X : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 12) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : TxEQ lane 0",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %02X : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("    %02X : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    port_mgr_log("    %2X : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 13) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : TxEQ lane 1",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %02X : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("    %02X : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    port_mgr_log("    %2X : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 14) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : TxEQ lane 2",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %02X : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("    %02X : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    port_mgr_log("    %2X : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 15) {
    port_mgr_log("[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
                 "] FIFO entry %4u : %08X : TxEQ lane 3",
                 (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
                 (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
                 current_entry_index[0],
                 wd);
    port_mgr_log("    %02X : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("    %02X : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    port_mgr_log("    %2X : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else {
    port_mgr_log_warn(
        "[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64
        "] FIFO entry %4u : %08X : Unknown event type %1X. FIFO entry content "
        "may be corrupted/invalid. time=%03X data=%04X",
        (current_fifo_time_100usec[0] | evt_tim) / DAY_TO_100_USEC,
        (current_fifo_time_100usec[0] | evt_tim) / SEC_TO_100_USEC,
        (current_fifo_time_100usec[0] | evt_tim) % SEC_TO_100_USEC,
        current_entry_index[0],
        wd,
        evt_typ,
        evt_tim,
        evt_dta);
  }  // end if & else if & else
}  // end decode_wd

/** \brief  Go through PCIe debug FIFO to try to find the first timer overflow
 * entry for determining the correct beginning time of the current PCIe debug
 * FIFO content within the specified range between head and tail
 *
 *          Note: None
 *
 * \param[in]  device_id                 : System-assigned identifier
 * (0..BFN_MAX_ASICS-1) \param[in]  head                      : the first entry
 * of the range to search \param[in]  tail                      : the last entry
 * of the range to search \param[out] current_fifo_time_100usec : the
 * calibrated/determined beginnning time of current PCIe debug FIFO content
 * within the specified range
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid range specified with head and tail
 */
bf_status_t port_mgr_tof2_microp_pcie_debug_fifo_calibrate_time(
    bf_dev_id_t device_id,
    uint32_t head,
    uint32_t tail,
    uint64_t *current_fifo_time_100usec) {
  uint32_t reg_data32, reg_addr;
  uint32_t misc_pcie_debug_log_reg_addr =
      offsetof(tof2_reg, device_select.misc_tv80_regs);

  if (device_id >= BF_MAX_DEV_COUNT) {
    port_mgr_log_error("Error: invalid device_id %u", device_id);
    return BF_INVALID_ARG;
  }  // end if

  if (head > tail) {
    port_mgr_log_error(
        "Error: invalid search range with head=%u tail=%u", head, tail);
    return BF_INVALID_ARG;
  }  // end if

  for (reg_addr = misc_pcie_debug_log_reg_addr | (head * 4),
      current_fifo_time_100usec[0] = 0;
       reg_addr <= (misc_pcie_debug_log_reg_addr | (tail * 4));
       reg_addr += 4) {
    lld_read_register(device_id, reg_addr, &reg_data32);
    if ((reg_data32 & 0xF0000000) == 0x10000000) {
      current_fifo_time_100usec[0] =
          (uint64_t)4096 * ((reg_data32 & 0x0FFFFFFF) - 1);
      port_mgr_log(
          "PCIe debug FIFO start time calibrated with entry %u content %08X to "
          "[%" PRIu64 " days|%" PRIu64 ".%05" PRIu64 " seconds]",
          (reg_addr - misc_pcie_debug_log_reg_addr) / 4,
          reg_data32,
          current_fifo_time_100usec[0] / DAY_TO_100_USEC,
          current_fifo_time_100usec[0] / SEC_TO_100_USEC,
          current_fifo_time_100usec[0] % SEC_TO_100_USEC);
      return BF_SUCCESS;
    }  // end if
  }    // end for

  port_mgr_log(
      "Couldn't find any PCIe debug FIFO entry within range [%u, %u] to "
      "calibrated time. set PCIe debug FIFO start time to 0",
      head,
      tail);
  return BF_SUCCESS;
}  // end port_mgr_tof2_microp_pcie_debug_fifo_calibrate_time

/** \brief  Go through PCIe debug FIFO and dump non-zero entries
 *
 *          Note: current implementation dumps into bf_drivers.log with default
 * logging level. To Do: include more explanations in the dump for the events
 * and orders of events to help debugging activities. this probably requires
 * looking at multiple FIFO entries
 *
 * \param[in]  device_id  : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  mode       : mode determines the entries to go through for dump.
 * 0 - goes through all 4096 entries. 1 - goes through entries between current
 * hardware head and tail pointers
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 */
bf_status_t port_mgr_tof2_microp_pcie_debug_fifo_dump(bf_dev_id_t device_id,
                                                      uint8_t mode) {
  uint32_t reg_data32, reg_addr, head, tail;
  uint32_t misc_pcie_debug_ctrl_reg_addr =
      offsetof(tof2_reg, device_select.misc_regs.pcie_debug_ctrl);
  uint32_t misc_pcie_debug_head_ptr_reg_addr =
      offsetof(tof2_reg, device_select.misc_regs.pcie_debug_head_ptr);
  uint32_t misc_pcie_debug_tail_ptr_reg_addr =
      offsetof(tof2_reg, device_select.misc_regs.pcie_debug_tail_ptr);
  uint32_t misc_pcie_debug_log_reg_addr =
      offsetof(tof2_reg, device_select.misc_tv80_regs);

  uint16_t current_entry_index, consecutive_entry_count, total_entry_count;
  uint64_t current_fifo_time_100usec;

  if (device_id >= BF_MAX_DEV_COUNT) {
    port_mgr_log_error("Error: invalid device_id %u", device_id);
    return BF_INVALID_ARG;
  }  // end if

  lld_read_register(device_id, misc_pcie_debug_ctrl_reg_addr, &reg_data32);
  port_mgr_log("PCIe debug FIFO is currently %s in the hardware",
               (reg_data32 & (1 << 24)) ? "enabled" : "disabled");

  if (mode == 0) {
    head = 0x0;
    tail = 0xFFF;
    port_mgr_log("Mode %u dumping all 4096 PCIe debug FIFO entries", mode);
  } else if (mode == 1) {
    lld_read_register(device_id, misc_pcie_debug_head_ptr_reg_addr, &head);
    head &= 0xFFF;
    lld_read_register(device_id, misc_pcie_debug_tail_ptr_reg_addr, &tail);
    tail &= 0xFFF;
    port_mgr_log(
        "Mode %u dumping %u PCIe debug FIFO entries between current FIFO head "
        "0x%03X and tail 0x%03X pointers",
        mode,
        tail - head + 1,
        head,
        tail);
  } else {
    port_mgr_log_error("Error: invalid mode %u", mode);
    return BF_INVALID_ARG;
  }  // end if & else if & else
  for (reg_addr = misc_pcie_debug_log_reg_addr | (head * 4),
      current_entry_index = head,
      total_entry_count = 0,
      consecutive_entry_count = 0;
       reg_addr <= (misc_pcie_debug_log_reg_addr | (tail * 4));
       reg_addr += 4, current_entry_index++) {
    lld_read_register(device_id, reg_addr, &reg_data32);
    if (reg_data32) {
      if (consecutive_entry_count == 0)
        port_mgr_tof2_microp_pcie_debug_fifo_calibrate_time(
            device_id,
            current_entry_index,
            tail,
            &current_fifo_time_100usec);  // try calibrate time on first valid
                                          // entry from the beginning or after
                                          // at least one entry of 0x00000000
      decode_wd(&current_entry_index, &current_fifo_time_100usec, reg_data32);
      consecutive_entry_count++;
      total_entry_count++;
    } else {
      if (consecutive_entry_count) {
        port_mgr_log(
            "PCIe debug FIFO entry %4u is 0x00000000 after %4u non-zero "
            "entries",
            current_entry_index,
            consecutive_entry_count);
        consecutive_entry_count = 0;
      }  // end if
    }    // end if & else
  }      // end for
  port_mgr_log("Logged %u non-zero PCIe debug FIFO entries", total_entry_count);
  return BF_SUCCESS;
}  // end port_mgr_tof2_microp_pcie_debug_fifo_dump

/** \brief  Clear PCIe debug FIFO
 *
 *          Note: hardware will automatically reset head and tail pointers to 0
 * when we stop hardware's PCIe debug FIFO logging as part of the clearing
 * process. the RTL designer of this block said we have to stop hardware's PCIe
 * debug FIFO logging before doing any FIFO clearing
 *
 * \param[in]  device_id  : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  mode       : mode determines the entries to clear. 0 - clears all
 * 4096 entries. 1 - clears entries between current hardware head and tail
 * pointers
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 */
bf_status_t port_mgr_tof2_microp_pcie_debug_fifo_clear(bf_dev_id_t device_id,
                                                       uint8_t mode) {
  uint32_t reg_data32, reg_addr, head, tail;
  uint32_t misc_pcie_debug_ctrl_reg_addr =
      offsetof(tof2_reg, device_select.misc_regs.pcie_debug_ctrl);
  uint32_t misc_pcie_debug_head_ptr_reg_addr =
      offsetof(tof2_reg, device_select.misc_regs.pcie_debug_head_ptr);
  uint32_t misc_pcie_debug_tail_ptr_reg_addr =
      offsetof(tof2_reg, device_select.misc_regs.pcie_debug_tail_ptr);
  uint32_t misc_pcie_debug_log_reg_addr =
      offsetof(tof2_reg, device_select.misc_tv80_regs);

  if (device_id >= BF_MAX_DEV_COUNT) {
    port_mgr_log_error("Error: invalid device_id %u", device_id);
    return BF_INVALID_ARG;
  }  // end if

  if (mode == 0) {
    head = 0x0;
    tail = 0xFFF;
    port_mgr_log("Mode %u clearing all 4096 PCIe debug FIFO entries", mode);
  } else if (mode == 1) {
    lld_read_register(device_id, misc_pcie_debug_head_ptr_reg_addr, &head);
    head &= 0xFFF;
    lld_read_register(device_id, misc_pcie_debug_tail_ptr_reg_addr, &tail);
    tail &= 0xFFF;
    port_mgr_log(
        "Mode %u clearing %u PCIe debug FIFO entries between current FIFO head "
        "%03X and tail %03X pointers",
        mode,
        tail - head + 1,
        head,
        tail);
  } else {
    port_mgr_log_error("Error: invalid mode %u", mode);
    return BF_INVALID_ARG;
  }  // end if & else if & else

  // stop PCIe debug FIFO logging
  lld_read_register(device_id, misc_pcie_debug_ctrl_reg_addr, &reg_data32);
  reg_data32 &= ~(1 << 24);     // log_ena = 0x0
  reg_data32 &= ~(0x3F << 26);  // event_sel = 0x0
  lld_write_register(device_id, misc_pcie_debug_ctrl_reg_addr, reg_data32);
  // clear PCIe debug FIFO
  for (reg_addr = misc_pcie_debug_log_reg_addr | (head * 4);
       reg_addr <= (misc_pcie_debug_log_reg_addr | (tail * 4));
       reg_addr += 4) {
    lld_write_register(device_id, reg_addr, 0x0);
  }                            // resume PCIe debug FIFO logging
  reg_data32 |= (1 << 24);     // log_ena = 0x1
  reg_data32 |= (0x3f << 26);  // event_sel = 0x3F
  lld_write_register(device_id, misc_pcie_debug_ctrl_reg_addr, reg_data32);

  port_mgr_log("Cleared %u PCIe debug FIFO entries", tail - head + 1);
  return BF_SUCCESS;
}  // end port_mgr_tof2_microp_pcie_debug_fifo_clear
