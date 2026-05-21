
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <bf_types/bf_types.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_log.h>
#include "aw_lane_cfg.h"
#include <port_mgr/bf_tof3_serdes_if.h>
#include <port_mgr/bf_aw_vfld_pmd.h>
#include "port_mgr_tof3_map.h"
#include "port_mgr_tof3_serdes_map.h"
#include "aw_if.h"
#include <port_mgr/bf_aw_pmd.h>

extern int suppress_aw_prints;
extern int64_t timeval_subtract(struct timeval *x, struct timeval *y);
extern int bf_sys_usleep(int us);
extern bf_status_t bf_tof3_serdes_txfir_config_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t cm3,
                                                   uint32_t cm2, uint32_t cm1,
                                                   uint32_t c0, uint32_t c1);
extern bf_status_t bf_tof3_serdes_prbs_rst_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln);
extern bf_status_t bf_tof3_serdes_rx_prbs_err_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *err_cnt);
bf_status_t bf_tof3_serdes_port_speed_to_serdes_speed(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    bf_port_speed_t speed, uint32_t num_lanes, uint32_t *serdes_gb,
    bool *is_pam4);

int aw_trace_en = 0;
int use_printf = 0;

void bf_aw_trace(char *api, uint32_t dev_id, uint32_t dev_port, uint32_t ln,
                 uint32_t argc, ...) {
  va_list ap;

  if (suppress_aw_prints)
    return; // in aw_driver_sim.c
  if (aw_trace_en == 0)
    return;

  if (use_printf) {
    printf("%d, %d, %d : %-34s ", dev_id, dev_port, ln, api);

    va_start(ap, argc);
    for (uint32_t i = 0; i < argc; i++) {
      char *str = va_arg(ap, char *);
      uint32_t val = va_arg(ap, uint32_t);
      printf(": %s = %d", str, val);
    }
    va_end(ap);
    printf("\n");
  } else {
    char log_str[256];

    snprintf(log_str, sizeof(log_str) - 1, "%d, %d, %d : %-34s ", dev_id,
             dev_port, ln, api);

    va_start(ap, argc);
    for (uint32_t i = 0; i < argc; i++) {
      char *str = va_arg(ap, char *);
      uint32_t val = va_arg(ap, uint32_t);
      uint32_t end_of_log_str = strlen(log_str);
      snprintf(&log_str[end_of_log_str], sizeof(log_str) - 1 - end_of_log_str,
               ": %s = %d", str, val);
    }
    va_end(ap);
    port_mgr_log("%s", log_str);
  }
}

/****************************************************************
 * bf_tof3_serdes_addr_set
 *
 * Because the physical tx and rx lanes may be different we need
 * to know, for this access, whether we are referencing a tx
 * or rx function and set the lane_offset appropriately.
 */
bf_tf3_sd_t *bf_tof3_serdes_addr_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, mss_access_t *mss,
                                     bf_tf3_section_t section) {
  bf_tf3_sd_t *tf3_sd;
  uint32_t num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
  uint32_t phy_ln = 0;

  // Due to channel reduction, only 4 channels per MAC are
  // used. On QSFP DD ports, there are 2 physical serdes lanes
  // per channel. Only the even lanes (0,2,4,6) are directly
  // referenceable by "dev_port", the odd lanes use an even numbered
  // dev_port and an odd numbered "ln".
  //
  // The mapping to MAC channel is
  // ((dev_port & 0x7) + ln)/2
  //
  // On OSPF ports, however, there are only 4 physical serdes lanes
  // per MAC, one per MAC channel. The remaining 4 physical serdes
  // lanes are not used or even refernceable
  //
  // The mapping to MAC channel is
  // ((dev_port & 7) + (2*ln))/2
  //
  // The CPU port is another special-case. It has one physical
  // serdes lane per MAC channel.
  //
  // The mapping to MAC channel is
  // ((dev_port & 0x7) + ln)
  //
  // The PCIe PHY is another special-case. It uses four physical
  // serdes lanes and has no associated ethernet MAC.
  //
  // The mapping required is just to an array of 4 tf3_sd structures
  // maintained in port_mgr_tof3_serdes_map.c,
  //
  // ((dev_port & 0x7) + ln)
  //
  // drv-6514 - add special check for PCIe PHY
  if ((dev_port >= 1000) && (dev_port < 1008)) {
    phy_ln = ln; // PCIe PHY has 1 lane per "channel", 0-3
  } else if (num_ln == 1) {
    phy_ln = ln * 2; // OSPF ports have 1 lane per MAC channel, 0-3
  } else {
    phy_ln = ln; // QSFP DD ports have 2 lanes per MAC channel, 0-7
  }

  // map the tf3_sd which contains the logical to physical
  // lane translation for both rx and tx directions as well
  // as the API vector table for the serdes IP type, 4ln
  // of 16ln.
  //
  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, phy_ln);

  // copy dev and subdev to mss
  mss->dev_id = tf3_sd->dev_id;
  mss->subdev_id = tf3_sd->subdev_id;

  mss->dev_port = dev_port;
  mss->ln = ln;

  // set HW read/write fn vector
  mss->io = tf3_sd->io;

  // Set phy_offset, which corresponds to "port"
  //
  mss->phy_offset = tf3_sd->macro_offset;

  mss->rx_lane_offset = tf3_sd->rx_lane_offset;
  mss->tx_lane_offset = tf3_sd->tx_lane_offset;

  if (section == MSS_SECTION_RX) {
    mss->derived_from_name_offset = tf3_sd->rx_lane_offset;
  } else if (section == MSS_SECTION_TX) {
    mss->derived_from_name_offset = tf3_sd->tx_lane_offset;
  } else {
    mss->derived_from_name_offset = 0; // CMN?
  }
  return tf3_sd;
}

/****************************************************************
 * Assumed to be called once for each valid dev_port, to
 * initialize its tf3_sd struct.
 */
bf_status_t bf_tof3_serdes_lane_install(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t tx_ln, uint32_t rx_ln,
                                        uint32_t is_4ln) {
  bf_tf3_sd_t *tf3_sd;
  uint32_t macro;
  uint32_t subdev_id = 0;
  uint32_t rc;

  //
  // map the tf3_sd which will contain the logical to physical
  // lane translation for both rx and tx directions as well
  // as the API vector table for the serdes IPI type, 4ln
  // of 16ln.
  //
  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, ln);
  //
  // map (dev_id, dev_port) to macro (2 400G ports per macro)
  // note: "macro" is a logical macro, 0-32, on the returned
  //       subdev_id.
  rc = map_dev_port_to_macro(dev_id, dev_port, &subdev_id, &macro);
  if (rc != 0) {
    assert(0);
  }
  // copy dev info into tf3_sd for I/O
  tf3_sd->dev_id = dev_id;
  tf3_sd->subdev_id = subdev_id;

  //
  // set phy_offset to base of macro address space
  //
  tf3_sd->macro_offset = map_macro_to_address(macro);
  tf3_sd->logical_lane = ln;
  tf3_sd->physical_rx_lane = rx_ln; // physical lane used for RX
  tf3_sd->physical_tx_lane = tx_ln; // physical lane used for TX

  //
  tf3_sd->cmn_lane_offset = CMN_OFFSET;
  tf3_sd->rx_lane_offset = (rx_ln * LANE_OFFSET);
  tf3_sd->tx_lane_offset = (tx_ln * LANE_OFFSET);

  // printf("Install dev=%d : dev_port=%d : subdev=%d : macro=%d : macro
  // offset=%08x\n",
  //       dev_id, dev_port, subdev_id, macro, tf3_sd->macro_offset);

  if (is_4ln) {
    tf3_sd->io = aw_4ln_io;
    tf3_sd->api = aw_4ln_driver_install();
  } else {
    tf3_sd->io = aw_16ln_io;
    tf3_sd->api = aw_16ln_driver_install();
  }
  return BF_SUCCESS;
}

/****************************************************************
 *
 */
int bf_tof3_serdes_read_status(uint32_t dev_id, uint32_t dev_port, uint32_t ln,
                               uint32_t br) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  int rc;

  // map the tf3_sd which contains the logical to physical
  // lane translation for both rx and tx directions as well
  // as the API vector table for the serdes IPI type, 4ln
  // of 16ln.
  //
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);
  // make sure we dont jump off into space
  if (!tf3_sd)
    return 1;
  if (!tf3_sd->api)
    return 2;

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_status(&mss, br);
  aw_uc_diag_regs_t uc_diag;
  rc = tf3_sd->api->pmd_uc_diag_reg_dump(&mss, &uc_diag);

  // map AW error codes to bf_types_t error codes
  //
  return map_aw_err_to_bf_err(rc);
}

/****************************************************************
 *
 */
int bf_tof3_serdes_read_status2(uint32_t dev_id, uint32_t dev_port, uint32_t ln,
                                uint32_t br) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  int rc;

  // map the tf3_sd which contains the logical to physical
  // lane translation for both rx and tx directions as well
  // as the API vector table for the serdes IPI type, 4ln
  // of 16ln.
  //
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);
  // make sure we dont jump off into space
  if (!tf3_sd)
    return 1;
  if (!tf3_sd->api)
    return 2;

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_status2(&mss, br);

  // map AW error codes to bf_types_t error codes
  //
  return map_aw_err_to_bf_err(rc);
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_csr_def_get(uint32_t dev_id, uint32_t dev_port,
                                       uint32_t ln, uint32_t csr_addr,
                                       char **name, uint32_t *num_flds,
                                       uint32_t *fld_num_base, char **comment) {
  bf_tf3_sd_t *tf3_sd;
  aw_reg_defs_t *regs;
  aw_fld_defs_t *flds;
  char **cmnts;
  aw_reg_defs_t *vregs;
  aw_fld_defs_t *vflds;
  char **vcmnts;
  int rc;
  uint32_t cmnt_idx;

  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, ln);
  if (tf3_sd == NULL)
    return 0xbad1dea1;
  if (tf3_sd->api == NULL)
    return 0xbad1dea2;

  tf3_sd->api->pmd_reg_defs_get(&regs, &flds, (char **)&cmnts, &vregs, &vflds,
                                (char **)&vcmnts);

  // we must normalize the csr address (i.e. remove the lane offset) since
  // the lookup table only has the "lane 0" address defined
  if (csr_addr >= 0x02000000) {
    csr_addr = csr_addr & ~((0x1F) << 24);
    csr_addr = csr_addr | (0x2 << 24);
  }
  // check virtual fields first since the scratch register that defines
  // the virtual field will show up in both tables
  rc = aw_reg_def_get(vregs, csr_addr, name, num_flds, fld_num_base, &cmnt_idx);
  if (rc == 0) { // found
    if (comment) {
      // most of the virtual fields had no comment
      if (cmnt_idx != 0xffffffff) {
        *comment = vcmnts[cmnt_idx];
      } else {
        *comment = "";
      }
    }
    return rc;
  }
  rc = aw_reg_def_get(regs, csr_addr, name, num_flds, fld_num_base, &cmnt_idx);
  if (rc != 0) { // reg not found
    if (name)
      *name = "<unk>";
    if (num_flds)
      *num_flds = 0;
    if (fld_num_base)
      *fld_num_base = 0;
    if (comment)
      *comment = "";
  } else {
    if (comment)
      *comment = cmnts[cmnt_idx];
  }
  return rc;
}

/****************************************************************
 * Find only the virtual registers, ignoring others
 */
bf_status_t bf_tof3_serdes_vreg_def_get(uint32_t dev_id, uint32_t dev_port,
                                        uint32_t ln, uint32_t csr_addr,
                                        char **name, uint32_t *num_flds,
                                        uint32_t *fld_num_base,
                                        char **comment) {
  bf_tf3_sd_t *tf3_sd;
  aw_reg_defs_t *regs;
  aw_fld_defs_t *flds;
  char **cmnts;
  aw_reg_defs_t *vregs;
  aw_fld_defs_t *vflds;
  char **vcmnts;
  int rc;
  uint32_t cmnt_idx;

  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, ln);
  if (tf3_sd == NULL)
    return 0xbad1dea1;
  if (tf3_sd->api == NULL)
    return 0xbad1dea2;

  tf3_sd->api->pmd_reg_defs_get(&regs, &flds, (char **)&cmnts, &vregs, &vflds,
                                (char **)&vcmnts);

  // we must normalize the csr address (i.e. remove the lane offset) since
  // the lookup table only has the "lane 0" address defined
  if (csr_addr >= 0x02000000) {
    csr_addr = csr_addr & ~((0x1F) << 24);
    csr_addr = csr_addr | (0x2 << 24);
  }
  // check virtual fields first since the scratch register that defines
  // the virtual field will show up in both tables
  rc = aw_reg_def_get(vregs, csr_addr, name, num_flds, fld_num_base, &cmnt_idx);
  if (rc == 0) { // found
    if (comment) {
      // most of the virtual fields had no comment
      if (cmnt_idx != 0xffffffff) {
        *comment = vcmnts[cmnt_idx];
      } else {
        *comment = "";
      }
    }
  }
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_csr_def_get_next(uint32_t dev_id, uint32_t dev_port,
                                            uint32_t ln, uint32_t csr_addr,
                                            uint32_t *next_csr_addr,
                                            char **name, uint32_t *num_flds,
                                            uint32_t *fld_num_base,
                                            char **comment) {
  bf_tf3_sd_t *tf3_sd;
  aw_reg_defs_t *regs;
  aw_fld_defs_t *flds;
  char **cmnts;
  aw_reg_defs_t *vregs;
  aw_fld_defs_t *vflds;
  char **vcmnts;
  int rc;
  uint32_t cmnt_idx;

  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, ln);
  if (tf3_sd == NULL)
    return 0xbad1dea1;
  if (tf3_sd->api == NULL)
    return 0xbad1dea2;

  tf3_sd->api->pmd_reg_defs_get(&regs, &flds, (char **)&cmnts, &vregs, &vflds,
                                (char **)&vcmnts);

  rc = aw_reg_def_get_next(regs, csr_addr, next_csr_addr, name, num_flds,
                           fld_num_base, &cmnt_idx);
  if (rc != 0) { // reg not found
    if (name)
      *name = "<unk>";
    if (num_flds)
      *num_flds = 0;
    if (fld_num_base)
      *fld_num_base = 0;
    if (comment)
      *comment = "";
  } else {
    if (comment)
      *comment = cmnts[cmnt_idx];
  }
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_csr_fld_def_get(
    uint32_t dev_id, uint32_t dev_port, uint32_t ln, uint32_t csr_addr,
    uint32_t fld_num, char **name, uint32_t *lo_bit, uint32_t *width,
    uint32_t *mask, uint32_t *access, uint32_t *reset_value, char **comment) {
  bf_tf3_sd_t *tf3_sd;
  aw_reg_defs_t *regs;
  aw_fld_defs_t *flds;
  char **cmnts;
  aw_reg_defs_t *vregs;
  aw_fld_defs_t *vflds;
  char **vcmnts;
  int rc;
  uint32_t num_flds, fld_num_base;
  char *reg_name;
  uint32_t cmnt_idx;

  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, ln);
  if (tf3_sd == NULL)
    return 0xbad1dea1;
  if (tf3_sd->api == NULL)
    return 0xbad1dea2;

  tf3_sd->api->pmd_reg_defs_get(&regs, &flds, (char **)&cmnts, &vregs, &vflds,
                                (char **)&vcmnts);

  // check virtual fields first since the scratch register that defines
  // the virtual field will show up in both tables
  rc = aw_reg_def_get(vregs, csr_addr, &reg_name, &num_flds, &fld_num_base,
                      &cmnt_idx);
  if ((rc == 0) && (fld_num < num_flds)) {
    rc = aw_fld_def_get(vflds, fld_num_base + fld_num, name, lo_bit, width,
                        mask, access, reset_value, &cmnt_idx);
    if (rc != 0) { // reg not found
      if (name)
        *name = "<unk>";
      if (lo_bit)
        *lo_bit = 0;
      if (width)
        *width = 0;
      if (mask)
        *mask = 0;
      if (access)
        *access = 0;
      if (reset_value)
        *reset_value = 0;
      if (comment)
        *comment = "";
    } else {
      if (comment) {
        if (cmnt_idx != 0xffffffff) {
          *comment = vcmnts[cmnt_idx];
        } else {
          *comment = "";
        }
      }
    }
    return rc;
  }
  rc = aw_reg_def_get(regs, csr_addr, &reg_name, &num_flds, &fld_num_base,
                      &cmnt_idx);
  if ((rc == 0) && (fld_num < num_flds)) {
    rc = aw_fld_def_get(flds, fld_num_base + fld_num, name, lo_bit, width, mask,
                        access, reset_value, &cmnt_idx);
  }
  if (rc != 0) { // reg not found
    if (name)
      *name = "<unk>";
    if (lo_bit)
      *lo_bit = 0;
    if (width)
      *width = 0;
    if (mask)
      *mask = 0;
    if (access)
      *access = 0;
    if (reset_value)
      *reset_value = 0;
    if (comment)
      *comment = "";
  } else {
    if (comment)
      *comment = cmnts[cmnt_idx];
  }
  return rc;
}

/****************************************************************
 *
 */
char *bf_tof3_serdes_prbs_mode_str(uint32_t mode) {
  switch (mode) {
  case AW_PRBS7:
    return "prbs7";
  case AW_PRBS9:
    return "prbs9";
  case AW_PRBS11:
    return "prbs11";
  case AW_PRBS13:
    return "prbs13";
  case AW_PRBS15:
    return "prbs15";
  case AW_PRBS23:
    return "prbs23";
  case AW_PRBS31:
    return "prbs31";
  case AW_QPRBS13:
    return "qprbs13";
  case AW_JP03A:
    return "jp03a";
  case AW_JP03B:
    return "jp03b";
  case AW_LINEARITY_PATTERN:
    return "lin";
  case AW_USER_DEFINED_PATTERN:
    return "user";
  case AW_FULL_RATE_CLOCK:
    return "clkfull";
  case AW_HALF_RATE_CLOCK:
    return "clkhalf";
  case AW_QUARTER_RATE_CLOCK:
    return "clkqrtr";
  case AW_PATT_32_1S_32_0S:
    return "32b0b1";
  default:
    break;
  }
  return "unk";
}

/*
 * Map BF_ PRBS modes to AW PRBS modes
 */
bf_status_t bf_tof3_bf_to_aw_prbs_pat(bf_port_prbs_mode_t bf_pat,
                                      aw_bist_pattern_t *aw_pat) {
  if (bf_pat == BF_PORT_PRBS_MODE_31) {
    *aw_pat = AW_PRBS31;
  } else if (bf_pat == BF_PORT_PRBS_MODE_23) {
    *aw_pat = AW_PRBS23;
  } else if (bf_pat == BF_PORT_PRBS_MODE_15) {
    *aw_pat = AW_PRBS15;
  } else if (bf_pat == BF_PORT_PRBS_MODE_13) {
    *aw_pat = AW_PRBS13;
  } else if (bf_pat == BF_PORT_PRBS_MODE_11) {
    *aw_pat = AW_PRBS11;
  } else if (bf_pat == BF_PORT_PRBS_MODE_9) {
    *aw_pat = AW_PRBS9;
  } else if (bf_pat == BF_PORT_PRBS_MODE_7) {
    *aw_pat = AW_PRBS7;
  } else {
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

char *bf_tof3_serdes_term_mode_str(uint32_t mode) {
  switch (mode) {
  case AW_ACC_HI_Z:
    return "hi-z";
  case AW_ACC_TERM_VSS_AC:
    return "vss-ac";
  case AW_ACC_TERM_FL_AC:
    return "flt-ac";
    // RES1 = 3, // Reserved
    // RES2 = 4, // Reserved
  case AW_ACC_TERM_VSS_DC:
    return "vss-dc";
  case AW_ACC_TERM_FL_DC:
    return "flt-dc";
    // RES3 = 7, // Reserved
  default:
    break;
  }
  return "unk";
}

bf_status_t bf_tof3_sweep(bf_dev_id_t dev_id, bf_dev_port_t tx_dev_port,
                          bf_dev_port_t rx_dev_port, uint32_t ln,
                          // tx settings
                          uint32_t cm3, uint32_t cm2, uint32_t cm1, uint32_t c0,
                          uint32_t c1,
                          // rx settings
                          uint32_t ctle_adapt_en, uint32_t ctle_adapt_boost,
                          uint32_t vga_cap,
                          // return vals
                          uint32_t *eq_ack, uint32_t *cdr_lock,
                          uint32_t *bist_lock, double *ber) {

  bf_status_t rc;

  // pre-init return vals
  *eq_ack = 0;
  *cdr_lock = 0;
  *bist_lock = 0;
  *ber = 1.0;

  // set new FIR taps onthhe Tx side
  rc = bf_tof3_serdes_txfir_config_set(dev_id, tx_dev_port, ln, cm3, cm2, cm1,
                                       c0, c1);
  if (rc != BF_SUCCESS)
    return rc;

  // allow tx to settle?
  bf_sys_usleep(10);

  // configure EQ settings on the Rx side
  rc = bf_aw_pmd_rx_ctle_adapt_set(dev_id, rx_dev_port, ln, ctle_adapt_en,
                                   ctle_adapt_boost);
  if (rc != BF_SUCCESS)
    return rc;

  // rx_vga_cap_adapt_set
  rc = bf_aw_pmd_rx_vga_cap_set(dev_id, rx_dev_port, ln, vga_cap);
  if (rc != BF_SUCCESS)
    return rc;

  // run EQ
  rc = bf_aw_pmd_eqeval_type_set(dev_id, rx_dev_port, ln, AW_EQ_FULL_DIR);
  if (rc != BF_SUCCESS)
    return rc;
  rc =
      bf_aw_pmd_eqeval_req_set(dev_id, rx_dev_port, ln, 0); // force clear first
  if (rc != BF_SUCCESS)
    return rc;
  rc = bf_aw_pmd_eqeval_req_set(dev_id, rx_dev_port, ln, 1);
  if (rc != BF_SUCCESS)
    return rc;

  uint32_t eqeval_ack = 0;
  for (int t = 0; t < 3500; t++) { // allow 3.5 sec for EQ
    rc = bf_aw_pmd_eqeval_ack_get(dev_id, rx_dev_port, ln, &eqeval_ack);
    if (rc != BF_SUCCESS)
      return rc;
    if (eqeval_ack)
      break;
    bf_sys_usleep(1 * 1000);
  }

  // ignore rc when resetting ack_req
  bf_aw_pmd_eqeval_req_set(dev_id, rx_dev_port, ln, 0);
  if (rc != BF_SUCCESS)
    return rc;
  if (!eqeval_ack)
    return BF_NOT_READY;

  *eq_ack = 1;

  for (int t = 0; t < 2500; t++) { // allow 2.5 sec for cdr lock
    rc = bf_aw_pmd_rx_lock_status_get(dev_id, rx_dev_port, ln, cdr_lock);
    if (rc != BF_SUCCESS)
      return rc;
    if (*cdr_lock)
      break;
    bf_sys_usleep(1 * 1000);
  }

  if (*cdr_lock == 0)
    return BF_NOT_READY;

  bf_aw_pmd_rx_chk_en_set(dev_id, rx_dev_port, ln, 0);
  bf_aw_pmd_rx_chk_en_set(dev_id, rx_dev_port, ln, 1);

  for (int t = 0; t < 1000; t++) { // allow 1 sec for cdr lock
    rc = bf_aw_pmd_rx_chk_lock_state_get(dev_id, rx_dev_port, ln, bist_lock);
    if (rc != BF_SUCCESS)
      return rc;
    if (*bist_lock)
      break;
    bf_sys_usleep(1 * 1000);
  }

  if (*bist_lock == 0)
    return BF_NOT_READY;

  struct timeval start_time, end_time;
  uint64_t period, bit_rate, num_bits, clk_ghz;

  gettimeofday(&start_time, NULL);

  rc = bf_tof3_serdes_prbs_rst_set(dev_id, rx_dev_port, ln);
  if (rc != BF_SUCCESS)
    return rc;

  // accum errors
  bf_sys_usleep(50 * 1000); // allow 50ms for error collection

  uint32_t err_cnt = 0;
  rc = bf_tof3_serdes_rx_prbs_err_get(dev_id, rx_dev_port, ln, &err_cnt);
  if (rc != BF_SUCCESS)
    return rc;

  gettimeofday(&end_time, NULL);

  period = timeval_subtract(&end_time, &start_time);

  bf_port_speed_t speed;

  rc = bf_port_speed_get(dev_id, rx_dev_port, &speed);
  int num_lanes;

  bf_port_num_lanes_get(dev_id, rx_dev_port, &num_lanes);
  uint32_t rate_gb;
  bool is_pam4;

  // get serdes speed and encoding mode from port speed
  rc = bf_tof3_serdes_port_speed_to_serdes_speed(dev_id, rx_dev_port, ln, speed,
                                                 num_lanes, &rate_gb, &is_pam4);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG; // unsupported rate
  }

  switch (rate_gb) {
  case 100: // 106g
    clk_ghz = 106250000000;
    break;
  case 50: // 56g
    clk_ghz = 53125000000;
    break;
  case 25: // 25g
    clk_ghz = 25781250000;
    break;
  case 10: // 10g
    clk_ghz = 10312500000;
    break;
  case 0: // un-configured
    clk_ghz = 1250000000;
    break;
  default:
    clk_ghz = 0;
    break;
  }
  bit_rate = clk_ghz;
  if (bit_rate != 0) {
    num_bits = (bit_rate * period) / 1000000;
    *ber = (double)(err_cnt) / (double)num_bits;
  } else {
    *ber = 1.0;
  }
  return BF_SUCCESS;
}

/***************************************************************
 * Given a dev_port/ln, find the dev_port/ln whose physical RX
 * ln is the same as the formers physical TX ln
 */
bf_status_t bf_tof3_utils_find_rx_dev_port(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           bf_dev_port_t *rx_dev_port,
                                           uint32_t *rx_ln) {
  bf_tf3_sd_t *tf3_sd;
  mss_access_t mss;
  uint32_t phys_tx_ln, phys_rx_ln;
  bf_dev_port_t base_d_p = dev_port & ~0x7;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_TX);
  phys_rx_ln = tf3_sd->physical_rx_lane;

  uint32_t num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
  for (bf_dev_port_t d_p = base_d_p; d_p < base_d_p + 8; d_p += 2) {
    for (uint32_t _ln = 0; _ln < num_ln; _ln++) {
      tf3_sd = bf_tof3_serdes_addr_set(dev_id, d_p, _ln, &mss, MSS_SECTION_RX);
      phys_tx_ln = tf3_sd->physical_tx_lane;
      if (phys_rx_ln == phys_tx_ln) {
        *rx_dev_port = d_p;
        *rx_ln = _ln;
        return 0;
      }
    }
  }
  return BF_INVALID_ARG;
}

/***************************************************************
 * Run LT without AN
 */
// FIELD: LT_WIDTH
// DESCRIPTION:
//   Link training without AN width control. Used only when
//   eth_lt_without_an_ena is asserted and AN disabled. 000 - reserved 001 -
//   reserved 010 - 16-bit 011 - 20-bit 100 - 32-bit 101 - 40-bit 110 - 64-bit
//   111 - 128-bit
//
//
//    { "10.3125g  (NRZ)",  10,         0,      3 * 10G*, 2 * 16b* },
//    { "25.78125g (NRZ)",  25,         0,      4 * 25G*, 4 * 32b* },
//    { "unsupported 5  ",   0,         0,      5,          0          },
//    { "53.125g  (PAM4)",  50,         1,      6 * 53G*, 6 * 64b* },
//    { "106.25g  (PAM4)", 100,         1,      7 * 53G*, 7 *128b* },
//
//
bf_status_t bf_tof3_run_lt(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                           uint32_t ln, uint32_t *lt_status) {
  bf_status_t rc;
  uint32_t width, clause;
  bf_port_speed_t speed;
  uint32_t rate_gb;
  bool is_pam4;
  uint32_t rate;
  bf_dev_port_t rx_dev_port;
  uint32_t rx_ln;
  uint32_t n_lanes;

  rc = bf_port_speed_get(dev_id, dev_port, &speed);
  if (rc != 0)
    return rc;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                         NULL, NULL);
  if (rc != 0)
    return rc;

  rc = bf_tof3_utils_find_rx_dev_port(dev_id, dev_port, ln, &rx_dev_port,
                                      &rx_ln);
  if (rc != 0)
    return rc;

  // get serdes speed and encoding mode from port speed
  bf_port_num_lanes_get(dev_id, dev_port, (int *)&n_lanes);
  rc = bf_tof3_serdes_port_speed_to_serdes_speed(dev_id, dev_port, ln, speed,
                                                 n_lanes, &rate_gb, &is_pam4);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG; // unsupported rate
  }

  // get rate/width from IP-dependent table base on serdes speed and encoding
  // mode
  rc = bf_tof3_serdes_serdes_speed_to_rate_and_width(
      dev_id, dev_port, ln, rate_gb, is_pam4 ? true : false, &rate, &width);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG; // unsupported rate
  }

  //// FIELD: LT_CTRL
  // DESCRIPTION:
  //   Link training without AN technology control. Used only when
  //   eth_lt_without_an_ena is asserted and AN disabled 0 - Reserved 1 - Clause
  //   72 2 - Clause 92 3 - Clause 136 4 - Clause 162
  if (rate_gb < 25) {
    clause = 1;
  } else if (rate_gb < 50) {
    clause = 2;
  } else if (rate_gb < 100) {
    clause = 3;
  } else {
    clause = 4;
  }
  bf_aw_pmd_vfld_rxeq_prbs_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_vfld_fg_done_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_vfld_linkeval_state_set(dev_id, dev_port, ln, 0);

  // disable grey-code en and precode en
  bf_aw_pmd_tx_pam4_precoder_override_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_tx_pam4_precoder_enable_set(dev_id, dev_port, ln, 0, 0);
  bf_aw_pmd_rx_pam4_precoder_override_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_rx_pam4_precoder_enable_set(dev_id, dev_port, ln, 0, 0);

  // disable TX FIR ovrd on tx and rx lanes
  uint32_t val;
  bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x0200100c, &val);
  val &= ~(1 << 22);
  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, 0x0200100c, val);

  if ((clause == 3) || (clause == 4)) { // 50g
    bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x020020e0, &val);
    val &= ~(3 << 4);
    val |= (3 << 4);
    bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, 0x020020e0, val);
  }

  // 0 :  [ 3: 3] : RW : RXMFSM_CTRL_RXMFSM_EQ_CHECK_RXDISABLE
  bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x02000784, &val);
  val &= ~(1 << 3);
  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, 0x02000784, val);

  rc = bf_aw_pmd_anlt_link_training_timeout_enable_set(dev_id, dev_port, ln, 0);

  // set map_en on the physical TX ln
  rc = bf_aw_pmd_ctrl_map_en_set(dev_id, dev_port, ln, 1 /*anlt_ctrl_map_en*/);

  printf("\nSetting map_en on RX side, d_p=%d, ln=%d\n", dev_port, ln);
  port_mgr_log("\nSetting map_en on RX side, d_p=%d, ln=%d\n", dev_port, ln);

  // also set the dev_port using the RX-side of the same physical ln
  rc = bf_aw_pmd_ctrl_map_en_set(dev_id, rx_dev_port, rx_ln,
                                 1 /*anlt_ctrl_map_en*/);
  rc = bf_aw_pmd_anlt_link_training_en_set(dev_id, dev_port, ln, 1);
  rc = bf_aw_pmd_anlt_link_training_config_set(dev_id, dev_port, ln, width,
                                               clause);
  // set PRBS Seed
  rc = bf_aw_pmd_anlt_link_training_prbs_seed_set(dev_id, dev_port, ln, clause,
                                                  0 /*logical_lane*/);

  // clear ena first
  rc = bf_aw_pmd_anlt_link_training_start_set(dev_id, dev_port, ln, 0);
  // enable LT
  rc = bf_aw_pmd_anlt_link_training_start_set(dev_id, dev_port, ln, 1);

  if (clause < 3) {
    bf_sys_usleep(1 * 1000 * 1000);
  } else if (clause == 3) {
    bf_sys_usleep(4 * 1000 * 1000);
  } else if (clause == 4) {
    bf_sys_usleep(12 * 1000 * 1000);
  }
  /* check FSM state */
  bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x020020a0, &val);
  *lt_status = val;
  return 0;
}

/*************************************************************
 * Run LT but dont wait for a status, so this can be run on
 * multiple lanes in parallel
 */
bf_status_t bf_tof3_run_lt2(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                            uint32_t ln, uint32_t width, uint32_t clause) {

  // clear ena first
  bf_aw_pmd_anlt_link_training_start_set(dev_id, dev_port, ln, 0);

  bf_aw_pmd_vfld_rxeq_prbs_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_vfld_fg_done_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_vfld_linkeval_state_set(dev_id, dev_port, ln, 0);

  // disable grey-code en and precode en
  bf_aw_pmd_tx_pam4_precoder_override_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_tx_pam4_precoder_enable_set(dev_id, dev_port, ln, 0, 0);
  bf_aw_pmd_rx_pam4_precoder_override_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_rx_pam4_precoder_enable_set(dev_id, dev_port, ln, 0, 0);

  // test
  // disable TX FIR ovrd on tx and rx lanes
  uint32_t val;
#if 1
  bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x0200100c, &val);
  val &= ~(1 << 22);
  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, 0x0200100c, val);
#endif

  if ((clause == 3) || (clause == 4)) { // 50g
    bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x020020e0, &val);
    val &= ~(3 << 4);
    val |= (3 << 4);
    bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, 0x020020e0, val);
  }

  // 0 :  [ 3: 3] : RW : RXMFSM_CTRL_RXMFSM_EQ_CHECK_RXDISABLE
  bf_tof3_serdes_csr_rd(dev_id, dev_port, ln, 0x02000784, &val);
  val &= ~(1 << 3);
  bf_tof3_serdes_csr_wr(dev_id, dev_port, ln, 0x02000784, val);

  bf_aw_pmd_rx_ctle_adapt_set(dev_id, dev_port, ln, 1, 0);
  bf_tof3_serdes_rx_vga_cap_adapt_set(
      dev_id, dev_port, ln, 1 /*en*/, 0, false /*use_custom_takeover_ratio*/,
      0 /*custom_takeover_ratio*/, 0 /*custom_nyq_mask*/);

  bf_aw_pmd_anlt_link_training_timeout_enable_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_anlt_link_training_en_set(dev_id, dev_port, ln, 1);
  bf_aw_pmd_anlt_link_training_config_set(dev_id, dev_port, ln, width, clause);
  // set PRBS Seed
  bf_aw_pmd_anlt_link_training_prbs_seed_set(dev_id, dev_port, ln, clause,
                                             0 /*logical_lane*/);

  // clear ena first
  bf_aw_pmd_anlt_link_training_start_set(dev_id, dev_port, ln, 0);
  // enable LT
  bf_aw_pmd_anlt_link_training_start_set(dev_id, dev_port, ln, 1);

  return 0;
}

char *bf_tof3_serdes_tech_ability_to_str(uint32_t tech_ability) {
  char *tech_ability_str[] = {
      "1000BASE-KX",          // 1G	1	NRZ	1.25
      "10GBASE-KX410G",       // 4	NRZ	3.125
      "10GBASE-KR	10G", // 1	NRZ	10.3125
      "40GBASE-KR4",          // 40G	4	NRZ	10.3125
      "40GBASE-CR4",          // 40G	4	NRZ	10.3125
      "100GBASE-CR10",        // 100G	10	NRZ	10.3125
      "100GBASE-KP4",         // 100G	4	PAM	13.59375
      "100GBASE-KR4",         // 100G	4	NRZ	25.78125
      "100GBASE-CR4",         // 100G	4	NRZ	25.78125
      "25GBASE-K/CR-S",       // 25G	1	NRZ	25.78125
      "25GBASE-K/CR",         // 25G	1	NRZ	25.78125
      "2.5GBASE-KX",          // 2.5G	1	NRZ	3.125
      "5GBASE-KR",            // 5G      1	NRZ	5.15625
      "50GBASE-K/CR",         // 50G	1	PAM	26.5625
      "100GBASE-K/CR2",       // 100G	2	PAM	26.5625
      "200GBASE-K/CR4",       // 200G	4	PAM	26.5625
      "100GBASE-K/CR1",       // 100G	1	PAM	53.125
      "200GBASE-K/CR2",       // 200G	2	PAM	53.125
      "400GBASE-K/CR4",       // 400G	4	PAM	53.125
      "undef1",
      "undef2",
      "undef3",
      "undef4",
      "Consortium 25GBASE-KR1",    // 25G	1	NRZ	25.78125
      "Consortium 25GBASE-CR1",    // 25G	1	NRZ	25.78125
      "Consortium 50GBASE-KR2",    // 50G	2	NRZ	25.78125
      "Consortium 50GBASE-CR2",    // 50G	2	NRZ	25.78125
      "Consortium 400GBASE-K/CR8", // 400G	8	PAM	53.125
  };
  if (tech_ability < sizeof(tech_ability_str) / sizeof(tech_ability_str[0])) {
    return tech_ability_str[tech_ability];
  } else {
    return "unknown tech ability";
  }
}

/****************************************************************
 * move to _utils.c after merge
 */
char *bf_tof3_serdes_loopback_mode_to_str(uint32_t loopback_mode) {
  if (loopback_mode == BF_LPBK_NONE)
    return "---";
  if (loopback_mode == BF_LPBK_SERDES_NEAR)
    return "nes";
  if (loopback_mode == BF_LPBK_SERDES_FAR)
    return "fes";
  if (loopback_mode == BF_LPBK_SERDES_NEAR_PARALLEL)
    return "nep";
  if (loopback_mode == BF_LPBK_SERDES_FAR_PARALLEL)
    return "fep";
  return "unk";
}

uint32_t bf_tof3_serdes_str_to_loopback_mode(char *str) {
  if (strncmp(str, "none", 4) == 0)
    return BF_LPBK_NONE;
  if (strncmp(str, "nes", 3) == 0)
    return BF_LPBK_SERDES_NEAR;
  if (strncmp(str, "fes", 3) == 0)
    return BF_LPBK_SERDES_FAR;
  if (strncmp(str, "nep", 3) == 0)
    return BF_LPBK_SERDES_NEAR_PARALLEL;
  if (strncmp(str, "fep", 3) == 0)
    return BF_LPBK_SERDES_FAR_PARALLEL;
  return BF_LPBK_NONE;
}
