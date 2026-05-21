#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <sys/time.h>
#include <bf_types/bf_types.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <dvm/bf_drv_intf.h>
#include <tof3_regs/tof3_reg_drv.h>
#include <lld/lld_reg_if.h>

#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_intf.h>
#include "aw_if.h"
#include "aw_driver_sim.h"
#include <port_mgr/bf_aw_pmd.h>
#include <port_mgr/bf_aw_vfld_pmd.h>
#include "port_mgr_tof3_map.h"
#include "port_mgr_tof3_serdes_map.h"
#include "bf_tof3_serdes_utils.h"
#include "../port_mgr_log.h"
#include "port_mgr_tof3_serdes.h"

/***************************************************************************
 * Main TF3 serdes driver module.
 *
 * This file contains most of the important Alphawave Serdes driver fns.
 *
 * It consists of:
 *
 * - Initialization code, executed on device add
 * - FSM code, to control port bring-up/dn
 * - APIs used by bf_pm and the bf-platform code
 * - Debug code used by UCLI commands
 *
 * This driver supports both Warriors (16ln) and Raptors (4ln) serdes IP
 * from Alphawave. It does so by using a function pointer table to
 * reference the AW-supplied low-level functions in aw_16ln/aw_alphacore.c
 * and aw_4ln/aw_alphacore.c. The programming sequence is the same for both
 * IPs, but the register definitions are different.
 *
 * In addition, the PCIe serdes are an instantiation of the 4ln IP. This
 * driver provides for accessing the PCIe PHY using fake "dev_ports"
 * 1000-1007. dev_ports 1000-1003 on die0, dev_ports 1004-1007 on die1.
 * Be very careful about what you access in the PCIe PHY as
 * it is the mechanism for accessing the TF3 chip. If you break the PCIe
 * PHY you will lose access to the TF3 chip (other than I2C of course).
 *
 * These C APIs, supplied by AW perform simple functions, sometimes just
 * programming a single bit field, sometimes multiple bit fields.
 *
 * There are some Intel-specific functions required by each IP, such as
 * issuing external resets or REFCLK chain programming. Most of these
 * are also implemented as API vectors. All Intel-specific code is
 * captured in a file named bf_alphacore.c in each directory.
 *
 * bf_tof3_serdes_if.c
 * bf_tof3_utils.c
 *       |
 *       +--- aw_16ln/aw_alphacore.c, bf_alphacore.c
 *            aw_4ln/aw_alphacore.c, bf_alphacore.c
 *
 * The actual TF3 address reference is made from aw_io.c in the
 * port_mgr/port_mgr_tof3/ directory. So a call to an API will come thru
 * this file, down thru the vector table into either aw_4ln/ or aw_16ln/,
 * then back up to aw_io.c, where, ultimately, lld_subdev_write_register
 * is called to effect the read/write(s).
 *
 * There are several unusual aspects to the AW serdes that need to be
 * kept in mind when debugging or going thru the code.
 *
 * 1) They do not support any logical to physical mapping of serdes lanes.
 * 2) Registers are implicitly associated with either the TX or RX slice
 * 3) Some (DFX) registers contain bits associated with both TX and RX
 *    in the same register.
 * 4) Some (DFX) register fields change which side (TX or RX) they show up
 *    on based on the setting of the map_en field (which we always set to "1")
 * 5) The Alphawave FW is not "firmware" in the usual sense. It is more a
 *    HW state-machine.
 * 6) The FW contains ONLY the code for a pre-defined set of serdes speeds.
 *    These speeds are ONLY identifiable via an associated yaml file that
 *    comes with each FW file. There is no way to "query" the FW for supported
 *    speeds.
 * 7) Each speed in the FW is defined by a "rate code", which determines the
 *    initial instruction of the code for each speed to be executed on a
 *    "rate change request". The rate code is a value, 0-7, and represents
 *    one entry in an 8-entry table. This table is NOT fully populated. An
 *    attempt to perform a rate-change to/from an empty entry in this table
 *    will result in the FW "hanging" and will require a Power-on-reset to
 *    recover.
 * 8) There is no simple way to determine eye margin. There are python scripts
 *    that calculate something like an eye height, but there is no register
 *    or API to retrieve such a value. So "margin" must be inferred from the
 *    FEC tail data.
 *
 * (1) requires that both the TX and RX lanes be identifiable in the data
 *     structure passed to the C APIs, since one API could reference fields
 *     in both the TX and RX slices.
 *
 * (2) requires identification of which block a CSR belongs to (TX, RX, ETH,
 *     or DFX) so the correct serdes slice can be referenced. The side, TX
 *     or RX is defined as follows:
 *
 *     Block    Slice
 *     -------+-------
 *     TX       TX
 *     RX       RX
 *     ETH      TX
 *     DFX      TX or RX based on knowledge of each register/field
 *
 * Typically, the bf_aw_ APIs are constructed to use the side (TX or RX)
 * indicated in the field name. However, (3,4) requires any DFX register
 * access to use a special function to remap references based on a fixed
 * mapping, eventually supplied by AW, in each directory, aw_16ln/ or
 * aw_4ln/.
 *
 * Most CSR references from the C APIs use a function named pmd_write_field().
 * However, all DFX accesses must use bf_pmd_16ln_write_field(), which can
 * determine the proper side from whoch to write/read the requested field.
 */
extern int64_t timeval_subtract(struct timeval *x, struct timeval *y);
extern char *port_mgr_tof3_fw_get(bf_dev_id_t dev_id, bool is_cpu_port);

bf_status_t bf_tof3_serdes_cdr_lock_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *cdr_lock);
bf_status_t bf_tof3_serdes_port_speed_to_serdes_speed(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    bf_port_speed_t speed, uint32_t num_lanes, uint32_t *serdes_gb,
    bool *is_pam4);

uint32_t macro_init_status[2 * 33] = {-1};
uint32_t lane_init_status[2 * 33][16] = {{-1}};

uint32_t num_pipes = 8;
uint32_t num_macros_per_pipe = 8;
uint32_t num_lanes_per_macro = 8;

// run Power-On Self-Test in synchronous (serial) mode
uint32_t run_synchronous_post = 1;
uint32_t get_ppm = 0;
#define PRBS_EN 1
#define BIST_MODE AW_DWELL

/****************************************************************
 * if running on real hw (not model, not emulator) allow
 * serdes accesses.
 */
bool bf_tof3_serdes_sppt(bf_dev_id_t dev_id) {
  bool is_sw_model = false;
  bool is_emulator = false;

  bf_drv_device_type_get(dev_id, &is_sw_model);
#if defined(DEVICE_IS_EMULATOR) // Emulator
  is_emulator = true;
#endif
  if (!is_sw_model && !is_emulator) {
    return true;
  }
  return false;
}

/****************************************************************
 * Reset the TX side of a lane using the external glue logic
 * as opposed to the AW CSRs which must be used in "isolation mode"
 * Since we always set DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A
 * we use the AW CSRs to control the resets. This function is
 * just in case we ever have a need for it.
 */
bf_status_t bf_tof3_serdes_glue_lane_tx_reset_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln, uint32_t en) {
  uint32_t macro, reg, val;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t phys_tx_ln;
  bf_status_t rc;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  // the serdes lane registers are indexed by physical Tx-lane
  // so we need to retrieve the map
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_TX);
  phys_tx_ln = tf3_sd->physical_tx_lane;

  if (macro == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_soft_reset);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_soft_reset);
    reg = reg_base + (stride * (macro - 1));

    lld_subdev_read_register(dev_id, subdev_id, reg, &val);
    if (en) {
      val |= ((1 << phys_tx_ln) << 8);
    } else {
      val &= ~((1 << phys_tx_ln) << 8);
    }
    lld_subdev_write_register(dev_id, subdev_id, reg, val);
  }
  return 0;
}

/****************************************************************
 * Reset the RX side of a lane using the external glue logic
 * as opposed to the AW CSRs which must be used in "isolation mode"
 * Since we always set DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A
 * we use the AW CSRs to control the resets. This function is
 * just in case we ever have a need for it.
 */
bf_status_t bf_tof3_serdes_glue_lane_rx_reset_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln, uint32_t en) {
  uint32_t macro, reg, val;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t phys_rx_ln;
  bf_status_t rc;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  // the serdes lane registers are indexed by physical Tx-lane
  // so we need to retrieve the map
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_TX);
  phys_rx_ln = tf3_sd->physical_rx_lane;

  if (macro == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_soft_reset);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_soft_reset);
    reg = reg_base + (stride * (macro - 1));

    lld_subdev_read_register(dev_id, subdev_id, reg, &val);
    if (en) {
      val |= (1 << phys_rx_ln);
    } else {
      val &= ~(1 << phys_rx_ln);
    }
    lld_subdev_write_register(dev_id, subdev_id, reg, val);
  }
  return 0;
}

/****************************************************************
 * Use the serdes glue logic to control "broadcast mode". This
 * enables broadcast writes to all lanes within a macro. We do
 * not use this mode in normal operations.
 */
bf_status_t bf_tof3_serdes_lane_bcast_set(bf_dev_id_t dev_id,
                                          bf_subdev_id_t subdev_id,
                                          uint32_t macro, uint32_t en) {
  uint32_t reg_ctrl;

  if (macro == 0) {
    reg_ctrl =
        offsetof(tof3_reg, serdes.serdes0.serdes4ln_glue_regs.serdes_reg_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_ctrl_base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_reg_ctrl);
    reg_ctrl = reg_ctrl_base + (stride * (macro - 1));
  }

  if (bf_tof3_serdes_sppt(dev_id)) {
    uint32_t rdata = 0, wdata = 0;

    lld_subdev_read_register(dev_id, subdev_id, reg_ctrl, &rdata);
    wdata = rdata & ~(1 << 4);
    wdata = wdata | ((en ? 1 : 0) << 4);
    lld_subdev_write_register(dev_id, subdev_id, reg_ctrl, wdata);
  }
  return 0;
}

/****************************************************************
 * After AN and link-training have completed the FSM polls the
 * PCS status for a link UP condition. If the port fails to come
 * up within the LT timeout (different for different LT clauses)
 * then AN is restarted.
 * If the PCS does come up before the LT timeout then this fn
 * is called to let the HW ANLT state-machine know to switch
 * the PMD TX from training frames to PCS data.
 *
 * Note: this bit does not "auto-clear" so it must be manually
 * de-asserted when restarting ANLT. This is done by the port
 * FSM.
 */
bf_status_t bf_tof3_serdes_anlt_link_up_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t val) {
  bf_status_t rc;
  uint32_t reg;
  uint32_t macro;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t phys_tx_ln;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  // the serdes lane registers are indexed by physical Tx-lane
  // so we need to retrieve the map
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_TX);
  phys_tx_ln = tf3_sd->physical_tx_lane;

  if (macro == 0) {
    reg = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_lane[phys_tx_ln]
                       .serdes_eth_anlt_ctrl);
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t reg_base = offsetof(
        tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_lane[phys_tx_ln]
                      .serdes_eth_anlt_ctrl);
    reg = reg_base + (stride * (macro - 1));
    // indicate PCS UP to AN block
    lld_subdev_write_register(dev_id, subdev_id, reg, (val ? 1 : 0));
  }
  return 0;
}

/****************************************************************
 * After we receive an "LT done" indication from the HW ANLT
 * state-machine we check for signal-detect and cdr lock on all
 * lanes. If all lanes report both being asserted then the port
 * FSM transtions to waiting for the PCS to come up. Otherwise
 * the FSM remains in the state waiting for both indications.
 */
bf_status_t bf_tof3_serdes_anlt_lane_status_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *signal_detect,
                                                uint32_t *pmd_rx_lock) {
  bf_status_t rc;

  rc = bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, signal_detect);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  rc = bf_aw_pmd_rx_lock_status_get(dev_id, dev_port, ln, pmd_rx_lock);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  return BF_SUCCESS;
}

/****************************************************************
 * This function resets the serdes glue registers to default
 * values. This is required because the serdes_glue block is not
 * presently tied to any reset except the power-on reset, so
 * the registers do NOT get reset on SDE restart.
 * This should be fixed in B0.
 */
bf_status_t bf_tof3_serdes_glue_reinit(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t num_lanes) {
  bf_status_t rc;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t macro;
  uint32_t phys_tx_ln;
  uint32_t base_ch, tmac_unused;

  if (!bf_tof3_serdes_sppt(dev_id))
    return 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL,
                                         &tmac_unused, &base_ch, NULL);
  if (rc != 0)
    return rc;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  // clear the interrupt statuses
  if (macro == 0) {
    uint32_t data;
    uint32_t addr;

    data = 0xffffffff;
    addr =
        offsetof(tof3_reg, serdes.serdes0.serdes4ln_glue_regs.anlt_intr.stat);
    lld_subdev_write_register(dev_id, subdev_id, addr, data);

    addr = offsetof(tof3_reg, serdes.serdes0.serdes4ln_glue_regs.sds_intr.stat);
    lld_subdev_write_register(dev_id, subdev_id, addr, data);
  } else {
    uint32_t data;
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t base =
        offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.anlt_intr.stat);
    uint32_t addr = base + (stride * (macro - 1));

    data = 0xffffffff;
    lld_subdev_write_register(dev_id, subdev_id, addr, data);

    base = offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.sds_intr.stat);
    addr = base + (stride * (macro - 1));
    lld_subdev_write_register(dev_id, subdev_id, addr, data);
  }
  for (uint32_t lane = 0; lane < num_lanes; lane++) {
    // the serdes mux registers are indexed by physical Tx-lane
    // so we need to retrieve the map
    tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln + lane, &mss,
                                     MSS_SECTION_TX);
    phys_tx_ln = tf3_sd->physical_tx_lane;

    if (macro == 0) {
      uint32_t unused_addr = offsetof(
          tof3_reg, serdes.serdes0.serdes4ln_glue_regs.serdes_lane[phys_tx_ln]
                        .serdes_eth_anlt_muxes);
      (void)unused_addr;
      // TBD, seems to be completely different register def from 16ln
    } else {
      uint32_t data;
      uint32_t stride = offsetof(tof3_reg, serdes.serdes2) -
                        offsetof(tof3_reg, serdes.serdes1);
      uint32_t base = offsetof(
          tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_lane[phys_tx_ln]
                        .serdes_tx_control);
      uint32_t addr = base + (stride * (macro - 1));

      data = 0;
      port_mgr_log("%d:%d:%3d:%d : Reset serdes_glue lane[%d] registers",
                   dev_id, subdev_id, dev_port, ln + lane, phys_tx_ln);
      lld_subdev_write_register(dev_id, subdev_id, addr + 0, data);
      lld_subdev_write_register(dev_id, subdev_id, addr + 4, data);
      lld_subdev_write_register(dev_id, subdev_id, addr + 8, data);
      lld_subdev_write_register(dev_id, subdev_id, addr + 12, data);
      lld_subdev_write_register(dev_id, subdev_id, addr + 16, data);
      lld_subdev_write_register(dev_id, subdev_id, addr + 20, data);
    }
  }
  return 0;
}

/****************************************************************
 * debug fn to return all the interesting glue bits
 */
bf_status_t bf_tof3_serdes_glue_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *rx_sts__rx_valid, uint32_t *anlt_ctrl__link_status,
    uint32_t *anlt_mux__group_master, uint32_t *anlt_mux__group_sel,
    uint32_t *anlt_mux__rxsel, uint32_t *anlt_stat__an_done,
    uint32_t *anlt_stat__an_fec_ena, uint32_t *anlt_stat__an_link_good,
    uint32_t *anlt_stat__an_new_page, uint32_t *anlt_stat__an_rsfec_ena,
    uint32_t *anlt_stat__an_tr_disable, uint32_t *anlt_stat__an_link_control) {
  bf_status_t rc;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t macro;
  uint32_t phys_tx_ln;
  uint32_t base_ch, tmac_unused;

  if (!bf_tof3_serdes_sppt(dev_id))
    return 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL,
                                         &tmac_unused, &base_ch, NULL);
  if (rc != 0)
    return rc;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;
  if (macro == 0)
    return BF_INVALID_ARG; // not implemented (yet)

  // the serdes mux registers are indexed by physical Tx-lane
  // so we need to retrieve the map
  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  phys_tx_ln = tf3_sd->physical_tx_lane;

  uint32_t data;
  uint32_t stride =
      offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
  uint32_t base =
      offsetof(tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_lane[phys_tx_ln]
                             .serdes_tx_control);
  uint32_t addr = base + (stride * (macro - 1));

  lld_subdev_read_register(dev_id, subdev_id, addr + 8, &data);
  *rx_sts__rx_valid = (data & 1);

  lld_subdev_read_register(dev_id, subdev_id, addr + 12, &data);
  *anlt_ctrl__link_status = (data & 1);

  lld_subdev_read_register(dev_id, subdev_id, addr + 16, &data);
  *anlt_stat__an_done = (data & 1);
  *anlt_stat__an_fec_ena = (data >> 1) & 1;
  *anlt_stat__an_link_good = (data >> 2) & 1;
  *anlt_stat__an_new_page = (data >> 3) & 1;
  *anlt_stat__an_rsfec_ena = (data >> 4) & 1;
  *anlt_stat__an_tr_disable = (data >> 5) & 1;
  *anlt_stat__an_link_control = (data >> 8) & 0x1f;

  lld_subdev_read_register(dev_id, subdev_id, addr + 20, &data);
  *anlt_mux__group_master = (data & 1);
  *anlt_mux__group_sel = (data >> 4) & 3;
  *anlt_mux__rxsel = (data >> 8) & 7;
  return 0;
}

/****************************************************************
 * The AW IP does not support lane mapping internally. Remapping
 * is required for link-training as the response to a tap change
 * request will come back on the RX side of the TX lane, which
 * may belong to a different port.
 * So the IP relies on an external mux to route the responses to
 * the correct TX lane.
 * This mux is implemented in the glue logic. It consists of
 * 3 fields per TX lane:
 *
 *  1) an indication whether is TX lane is logical lane 0
 *     called "master lane".
 *  2) an identifier that is common to all lanes within a mulit-
 *     lane port, called "group select".
 *  3) the physical RX lane associated with this physical TX,
 *     called "rx_select"
 *
 * The mux is implemented differently for the 4ln IP than the
 * 16ln IP. This is due to late design changes on Intels side
 * not any difference in the IP.
 *
 * There are several important considerations in the mux
 * programming:
 *
 * - The mux settings are associated with the physical TX lane
 *   not a logical lane.
 * - The group_sel MUST be unique for each port. This is beacuse
 *   the FW uses the group_sel to determine which lanes to wait
 *   for acknowledgements from during link-training. If an unused
 *   lane has the same group_sel as a port running ANLT then the
 *   FW will never get an acknowledgement and AN will fail.
 * - There are only 2 bits for group_sel. No value to use for
 *   "not in a group".
 * - On 112g chips, only 4 serdes lanes are actually used. But
 *   the unused lane still MUST have non-conflicting group_sel
 *   and "is master" fields. This is accomlished by setting
 *   "is master" for all unused lanes.
 * - because the mux is in the glue logic, it is not reset
 *   on SDE restart. It also must be manually reset whenever
 *   the port mode is changed (for example, going from 100G-R4
 *   to 10G the lane 0 mux must be programmed correctly and the
 *   muxes associated with what used to be lanes 1-3 must be
 *   re-initialized to non-conflicting values.
 *
 *   The group_sel field is chosen to be the MAC channel number
 *   of the ports lane 0 divided by 2 (since there are 8
 *   channels). Be mindful that 112g ports are indicated in the
 *   CLI by sequential OSFP channel numbers (0-3), so be sure
 *   you set the correct group_sel value.
 */
bf_status_t bf_tof3_serdes_anlt_mux_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t num_lanes) {
  bf_status_t rc;
  uint32_t mux;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t macro;
  uint32_t phys_tx_ln, phys_rx_ln;
  uint32_t base_ch, tmac_unused;

  if (!bf_tof3_serdes_sppt(dev_id))
    return 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL,
                                         &tmac_unused, &base_ch, NULL);
  if (rc != 0)
    return rc;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  for (uint32_t lane = 0; lane < num_lanes; lane++) {
    // the serdes mux registers are indexed by physical Tx-lane
    // so we need to retrieve the map
    tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln + lane, &mss,
                                     MSS_SECTION_TX);
    phys_tx_ln = tf3_sd->physical_tx_lane;
    phys_rx_ln = tf3_sd->physical_rx_lane;

    if (macro == 0) {
      mux = offsetof(tof3_reg,
                     serdes.serdes0.serdes4ln_glue_regs.serdes_lane[phys_tx_ln]
                         .serdes_eth_anlt_muxes);
      // TBD, seems to be completely different register def from 16ln
      uint32_t anlt_muxes_1_0;
      uint32_t anlt_muxes_m2s_9_8;
      uint32_t anlt_muxes_s2m_13_12;
      uint32_t data;

      anlt_muxes_1_0 = phys_tx_ln;
      anlt_muxes_m2s_9_8 = phys_rx_ln;
      anlt_muxes_s2m_13_12 = phys_rx_ln;

      port_mgr_log("%d:%d:%3d:%d : Eth anlt mux%d set: muxes=%d, m2s=%d, "
                   "s2m=%d (base_ch=%d)",
                   dev_id, subdev_id, dev_port, ln + lane, phys_tx_ln,
                   anlt_muxes_1_0, anlt_muxes_m2s_9_8, anlt_muxes_s2m_13_12,
                   base_ch);

      data = (anlt_muxes_1_0 << 0) | (anlt_muxes_m2s_9_8 << 8) |
             (anlt_muxes_s2m_13_12 << 12);
      lld_subdev_write_register(dev_id, subdev_id, mux, data);

    } else {
      uint32_t stride = offsetof(tof3_reg, serdes.serdes2) -
                        offsetof(tof3_reg, serdes.serdes1);
      uint32_t mux_base = offsetof(
          tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_lane[phys_tx_ln]
                        .serdes_eth_anlt_muxes);
      mux = mux_base + (stride * (macro - 1));
      uint32_t d0_group_master_lane; // is AN lane
      uint32_t d5_4_group_sel_lane;  // grp id
      uint32_t d10_8_rxsel_lane;     // phys rx lane
      uint32_t data;

      if (lane == 0) {
        d0_group_master_lane = 1;
      } else {
        d0_group_master_lane = 0;
      }
      d5_4_group_sel_lane = base_ch;
      d10_8_rxsel_lane = phys_rx_ln;
      data = (d0_group_master_lane << 0) | (d5_4_group_sel_lane << 4) |
             (d10_8_rxsel_lane << 8);
      port_mgr_log("%d:%d:%3d:%d : Eth anlt mux%d set: mstr=%d, grp=%d, "
                   "rxln=%d (base_ch=%d)",
                   dev_id, subdev_id, dev_port, ln + lane, phys_tx_ln,
                   d0_group_master_lane, d5_4_group_sel_lane, d10_8_rxsel_lane,
                   base_ch);
      // port_mgr_log("%d:%d:%3d:%d : addr=%08x : data=%08x : macro=%d",
      //             dev_id, subdev_id, dev_port, ln + lane, mux, data, macro);
      lld_subdev_write_register(dev_id, subdev_id, mux, data);
    }
  }
  return 0;
}

/****************************************************************
 * This fn is similar to mux_set but always sets "master" and
 * "group" fields back to defaults.
 *
 * This fn is used to unconfigure the mux when a port is
 * disabled or deleted, to ensure non-conflicting mux settings
 * with any existing port.
 */
bf_status_t bf_tof3_serdes_anlt_mux_reset(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t num_lanes) {
  bf_status_t rc;
  uint32_t mux;
  bf_subdev_id_t subdev_id;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t macro;
  uint32_t phys_tx_ln, phys_rx_ln;
  uint32_t base_ch, tmac_unused;

  if (!bf_tof3_serdes_sppt(dev_id))
    return 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL,
                                         &tmac_unused, &base_ch, NULL);
  if (rc != 0)
    return rc;

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  port_mgr_log("%d:%d:%3d:%d : Eth anlt mux reset:", dev_id, subdev_id,
               dev_port, ln);

  for (uint32_t lane = 0; lane < num_lanes; lane++) {
    // the serdes mux registers are indexed by physical Tx-lane
    // so we need to retrieve the map
    tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln + lane, &mss,
                                     MSS_SECTION_TX);
    phys_tx_ln = tf3_sd->physical_tx_lane;
    phys_rx_ln = tf3_sd->physical_rx_lane;

    if (macro == 0) {
      mux = offsetof(tof3_reg,
                     serdes.serdes0.serdes4ln_glue_regs.serdes_lane[phys_tx_ln]
                         .serdes_eth_anlt_muxes);
      // TBD, seems to be completely different register def from 16ln
      uint32_t anlt_muxes_1_0;
      uint32_t anlt_muxes_m2s_9_8;
      uint32_t anlt_muxes_s2m_13_12;
      uint32_t data;

      anlt_muxes_1_0 = phys_tx_ln;
      anlt_muxes_m2s_9_8 = phys_rx_ln;
      anlt_muxes_s2m_13_12 = phys_rx_ln;

      port_mgr_log("%d:%d:%3d:%d : Eth anlt mux%d reset: muxes=%d, m2s=%d, "
                   "s2m=%d (base_ch=%d)",
                   dev_id, subdev_id, dev_port, ln + lane, phys_tx_ln,
                   anlt_muxes_1_0, anlt_muxes_m2s_9_8, anlt_muxes_s2m_13_12,
                   base_ch);

      data = (anlt_muxes_1_0 << 0) | (anlt_muxes_m2s_9_8 << 8) |
             (anlt_muxes_s2m_13_12 << 12);
      lld_subdev_write_register(dev_id, subdev_id, mux, data);

    } else {
      uint32_t stride = offsetof(tof3_reg, serdes.serdes2) -
                        offsetof(tof3_reg, serdes.serdes1);
      uint32_t mux_base = offsetof(
          tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_lane[phys_tx_ln]
                        .serdes_eth_anlt_muxes);
      mux = mux_base + (stride * (macro - 1));
      uint32_t d0_group_master_lane; // is AN lane
      uint32_t d5_4_group_sel_lane;  // grp id
      uint32_t d10_8_rxsel_lane;     // phys rx lane
      uint32_t data;
      uint32_t ch_to_set_as_grp;

      // figure out if ch is 0,1,2,3 or 0,2,4,6
      // lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac, ch, &dev_port);
      int num_lanes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);
      if (num_lanes_per_mac == 4) {
        ch_to_set_as_grp = (base_ch + (ln + lane));
      } else {
        ch_to_set_as_grp = (base_ch + (ln + lane) / 2);
      }
      // reset to be master
      d0_group_master_lane = 1;
      // reset group to be its own ch
      d5_4_group_sel_lane = ch_to_set_as_grp; // base_ch;
      d10_8_rxsel_lane = phys_rx_ln;
      data = (d0_group_master_lane << 0) | (d5_4_group_sel_lane << 4) |
             (d10_8_rxsel_lane << 8);
      port_mgr_log("%d:%d:%3d:%d : Eth anlt mux%d reset: mstr=%d, grp=%d, "
                   "rxln=%d (base_ch=%d)",
                   dev_id, subdev_id, dev_port, ln + lane, phys_tx_ln,
                   d0_group_master_lane, d5_4_group_sel_lane, d10_8_rxsel_lane,
                   base_ch);
      // port_mgr_log("%d:%d:%3d:%d : addr=%08x : data=%08x : macro=%d",
      //	      dev_id, subdev_id, dev_port, ln + lane, mux, data, macro);
      lld_subdev_write_register(dev_id, subdev_id, mux, data);
    }
  }
  return 0;
}

/****************************************************************
 * Set the serdes mux so that it doesn't conflict with any other
 * (valid) lane on a switch with 4 lanes/ch.
 * This is required for Stinson, since we will not even get a
 * lane mapping for either the even or odd lanes, depending on
 * which are unused. The "master" bit and the rxsel lane must
 * be set so as not to conflict with any of the valid lanes.
 */
bf_status_t bf_tof3_serdes_anlt_mux_invalidate_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t valid_phys_tx_ln) {
  bf_status_t rc;
  uint32_t mux;
  bf_subdev_id_t subdev_id;
  uint32_t macro, phys_tx_ln, phys_rx_ln;
  uint32_t base_ch, tmac_unused;

  if (!bf_tof3_serdes_sppt(dev_id))
    return 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL,
                                         &tmac_unused, &base_ch, NULL);
  if (rc != 0)
    return rc;

  // if the passed "valid_phys_tx_ln" is even, the set the rx ln to an odd
  // number and vice-versa, and set phys_tx_ln to valid_phys_tx_ln + 1
  // Otherwise, set rx ln to an even number and phys_tx_ln to valid_phys_tx_ln -
  // 1
  if (valid_phys_tx_ln & 1) { // odd
    phys_tx_ln = valid_phys_tx_ln - 1;
    phys_rx_ln = phys_tx_ln; // 6;
  } else {
    phys_tx_ln = valid_phys_tx_ln + 1;
    phys_rx_ln = phys_tx_ln; // 7;
  }

  rc = map_dev_port_to_macro(dev_id, dev_port, (uint32_t *)&subdev_id, &macro);
  if (rc != BF_SUCCESS)
    return rc;

  // the serdes mux registers are indexed by physical Tx-lane
  if (macro == 0) {
    mux = offsetof(tof3_reg,
                   serdes.serdes0.serdes4ln_glue_regs.serdes_lane[phys_tx_ln]
                       .serdes_eth_anlt_muxes);
    // TBD, seems to be completely different register def from 16ln
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t mux_base = offsetof(
        tof3_reg, serdes.serdes1.serdes_glue_regs.serdes_lane[phys_tx_ln]
                      .serdes_eth_anlt_muxes);
    mux = mux_base + (stride * (macro - 1));
    uint32_t d0_group_master_lane; // is AN lane
    uint32_t d5_4_group_sel_lane;  // grp id
    uint32_t d10_8_rxsel_lane;     // phys rx lane
    uint32_t data;

    d0_group_master_lane = 1;
    // test
    // d5_4_group_sel_lane = base_ch;
    d5_4_group_sel_lane = 3;
    d10_8_rxsel_lane = phys_rx_ln;
    data = (d0_group_master_lane << 0) | (d5_4_group_sel_lane << 4) |
           (d10_8_rxsel_lane << 8);
    port_mgr_log("%d:%d:%3d:%d : Eth anlt mux%d invalidate set: mstr=%d, "
                 "grp=%d, rxln=%d (base_ch=%d)",
                 dev_id, subdev_id, dev_port, 1, phys_tx_ln,
                 d0_group_master_lane, d5_4_group_sel_lane, d10_8_rxsel_lane,
                 base_ch);
    lld_subdev_write_register(dev_id, subdev_id, mux, data);
  }
  return 0;
}

/****************************************************************
 * Return the addresses associated with the TX and RX lanes
 * of a dev_port/ln.
 * This is just for debugging to show the mapping between d_p
 * and the Intel CSR space.
 */
bf_status_t bf_tof3_serdes_addr_range(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *subdev_id, uint32_t *macro,
                                      uint32_t *phys_tx_ln,
                                      uint32_t *phys_rx_ln, uint32_t *cmn_ofs,
                                      uint32_t *tx_ofs, uint32_t *rx_ofs,
                                      uint32_t *sram0, uint32_t *sram1) {
  bf_tf3_sd_t *tf3_sd;
  uint32_t base, tx_ln, rx_ln;

  map_dev_port_to_macro(dev_id, dev_port, subdev_id, macro);
  tf3_sd = map_dev_port_to_sd(dev_id, dev_port, ln);

  rx_ln = tf3_sd->physical_rx_lane;
  tx_ln = tf3_sd->physical_tx_lane;
  *phys_tx_ln = tx_ln;
  *phys_rx_ln = rx_ln;
  base = tf3_sd->macro_offset;
  *cmn_ofs = base + 0x20000;
  *tx_ofs = base + tx_ln * 0x4000;
  *rx_ofs = base + rx_ln * 0x4000;
  *sram0 = base + 0x28000; // should only be one sram addr
  *sram1 = base + 0x28000;
  return BF_SUCCESS;
}

/****************************************************************
 * Intel API to perform the initial reset and other necessary
 * programming of a serdes macro. Called from cmn_init().
 *
 * It is only called once per macro (so only the first d_p in a
 * macro is passed).
 *
 * Note: The first call to this function executes the required
 *       power-on initialization of all serdes macros. Subsequent
 *       calls only perform operations on a single macro.
 *
 * The reset API will perform the following:
 * - a POST test to ensure the macro is accessible by performing
 *   read and write tests of a scratch register.
 *
 * On the first call to tis function the following will be
 * performed:
 *
 * - POR reset of all macros, one at a time (otherwise too much
 *   power is drawn and the PCIe interface will fail).
 * - lane reset of all lanes, one macro at a time.
 * - REFCLK chain programming
 * - SRAM delay programming
 *
 * Note: There is a slight assymetry in the initialization. The
 *       first macro will be POST tested prior to being reset.
 *       The remaining macros will have been reset by the call
 *       for the first macro.
 */
bf_status_t bf_tof3_serdes_mss_reset(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  int rc;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_CMN);

  bf_aw_trace("mss_reset", dev_id, dev_port, 0, 0);

  // Call thru API vector
  rc = tf3_sd->api->pmd_mss_reset(&mss);
  if (rc != 0) {
    bf_aw_trace("*** Chip not accessible!", dev_id, dev_port, 0, 0);
  }
  // map AW error codes to bf_types_t error codes
  //
  return map_aw_err_to_bf_err(rc);
}

/****************************************************************
 * Intel API to perform the FW Load for a given macro. Called
 * from cmn_init().
 *
 * The FW files to load are specified in src/CMakelist.txt
 * References to these files are passed to port_mgr_tof3_dev_add()
 * in the profile argument from bf_switchd.
 *
 * The FW file itself is just a sequence of register and SRAM
 * writes. The SRAM writes load the FW and pointers. The CSR
 * writes initialize lane registers. These are broadcast writes,
 * handled by aw_io.c
 *
 */
bf_status_t bf_tof3_serdes_fw_load(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   char *fw_path) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  int rc;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, 0, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_fw_load", dev_id, dev_port, 0, 0);

  // Call thru API vector
  rc = tf3_sd->api->pmd_fw_load(&mss, fw_path);
  if (rc != 0) {
    bf_aw_trace("Error: FW load failed", dev_id, dev_port, 0, 0);
  }
  // map AW error codes to bf_types_t error codes
  //
  return map_aw_err_to_bf_err(rc);
}

/****************************************************************
 * bf_tof3_serdes_cmn_state_req_set
 *
 * Power up the CMN lane as part of macro initialization. i
 *
 * Note: This fn uses the asynchronous AW APIs but polls for
 *       completion itself, making it sychronous.
 */
bf_status_t bf_tof3_serdes_cmn_state_req_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t max_polls,
                                             uint32_t ack_delay_us) {
  int rc;
  uint32_t i, ack;

  rc = bf_aw_pmd_iso_cmn_state_req_set(dev_id, dev_port, ln, 1);
  if (rc == BF_SUCCESS) {
    for (i = 0; i < max_polls; i++) {
      bf_sys_usleep(ack_delay_us);
      rc = bf_aw_pmd_iso_cmn_state_ack_get(dev_id, dev_port, ln, &ack);
      if (rc != BF_SUCCESS) {
        break;
      }
      if (ack == 1) {
        break;
      }
    }
  }
  // always clear
  bf_aw_pmd_iso_cmn_state_req_set(dev_id, dev_port, ln, 0);
  bf_sys_usleep(ack_delay_us);

  if (!ack) {
    bf_aw_trace("*** cmn_state_req ack failed", dev_id, dev_port, ln, 0);
    port_mgr_log("%d:%3d:%d : *** cmn_state_req ack failed", dev_id, dev_port,
                 ln);
    rc = BF_HW_COMM_FAIL;
  }

  // rc already mapped
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_tx_state_req_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t val) {
  int rc;

  rc = bf_aw_pmd_iso_tx_state_req_set(dev_id, dev_port, ln, val);
  // rc already mapped
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_tx_state_ack_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *ack) {
  int rc;

  rc = bf_aw_pmd_iso_tx_state_ack_get(dev_id, dev_port, ln, ack);
  // rc already mapped
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_tx_state_req_set_synchronouos(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t max_polls,
    uint32_t ack_delay_us) {
  int rc;
  uint32_t i, ack;

  rc = bf_tof3_serdes_tx_state_req_set(dev_id, dev_port, ln, 1);
  if (rc == BF_SUCCESS) {
    for (i = 0; i < max_polls; i++) {
      bf_sys_usleep(ack_delay_us);
      rc = bf_tof3_serdes_tx_state_ack_get(dev_id, dev_port, ln, &ack);
      if (rc != BF_SUCCESS) {
        break;
      }
      if (ack == 1) {
        break;
      }
    }
  }
  // always clear, even on timeout
  rc = bf_tof3_serdes_tx_state_req_set(dev_id, dev_port, ln, 0);

  if (!ack) {
    bf_aw_trace("*** tx_state_req ack failed", dev_id, dev_port, ln, 0);
    printf("%d:%d:%d : *** tx_state_req ack failed\n", dev_id, dev_port, ln);
    port_mgr_log("%d:%3d:%d : *** tx_state_req ack failed", dev_id, dev_port,
                 ln);
    rc = BF_HW_COMM_FAIL;
  }

  // rc already mapped
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_rx_state_req_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t val) {
  int rc;

  rc = bf_aw_pmd_iso_rx_state_req_set(dev_id, dev_port, ln, val);
  // rc already mapped
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_rx_state_ack_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *ack) {
  int rc;

  rc = bf_aw_pmd_iso_rx_state_ack_get(dev_id, dev_port, ln, ack);
  // rc already mapped
  return rc;
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_rx_state_req_set_synchronous(bf_dev_id_t dev_id,
                                                        bf_dev_port_t dev_port,
                                                        uint32_t ln,
                                                        uint32_t max_polls,
                                                        uint32_t ack_delay_us) {
  int rc;
  uint32_t i, ack;

  rc = bf_tof3_serdes_rx_state_req_set(dev_id, dev_port, ln, 1);
  if (rc == BF_SUCCESS) {
    for (i = 0; i < max_polls; i++) {
      bf_sys_usleep(ack_delay_us);
      rc = bf_tof3_serdes_rx_state_ack_get(dev_id, dev_port, ln, &ack);
      if (rc != BF_SUCCESS) {
        break;
      }
      if (ack == 1) {
        break;
      }
    }
  }
  // always clear
  rc = bf_tof3_serdes_rx_state_req_set(dev_id, dev_port, ln, 0);

  if (!ack) {
    bf_aw_trace("*** rx_state_req ack failed", dev_id, dev_port, ln, 0);
    printf("%d:%d:%d : *** rx_state_req ack failed\n", dev_id, dev_port, ln);
    port_mgr_log("%d:%3d:%d : *** rx_state_req ack failed", dev_id, dev_port,
                 ln);
    rc = BF_HW_COMM_FAIL;
  }

  // rc already mapped
  return rc;
}

/****************************************************************
 * bf_tof3_serdes_term_mode_adv_set
 *
 * This function handles all the tf3 termination modes. Below
 * is a simpler form that retains backward compatibility with
 * tf2, supporting only "AC" or "DC".
 *
 * FIELD: ACC_TERM_MODE_NT
 * DESCRIPTION:
 *   Termination mode
 *   111 - Reserved
 *   110 - Floating Termination, DC coupled
 *   101 - Termination to vss, DC coupled
 *   100 - Reserved
 *   011 - Reserved
 *   010 - Floating Termination, onchip AC coupling
 *   001 - Termination to vss, onchip AC coupling
 *   000 - High Z
 */
bf_status_t bf_tof3_serdes_term_mode_adv_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             aw_acc_term_mode_t term_mode) {
  int rc;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  // Map BF termination mode to AW define
  switch (term_mode) {
  case AW_ACC_HI_Z:
  case AW_ACC_TERM_VSS_AC: // termination to vss, onchip AC coupling
  case AW_ACC_TERM_FL_AC:  // = 2, // floating termination, onchip AC coupling
  case AW_ACC_TERM_VSS_DC: // termination to vss, DC coupled
  case AW_ACC_TERM_FL_DC:  // termination to vss, DC coupled
    break;                 // valid mode
  default:
    // RES1 = 3, // Reserved
    // RES2 = 4, // Reserved
    // RES3 = 7, // Reserved
    return BF_INVALID_ARG;
    break;
  }

  // save in cfg
  tf3_sd->cfg.rx_term = term_mode;

  rc = bf_aw_pmd_rx_termination_set(dev_id, dev_port, ln, term_mode);
  return rc; // already mapped
}

/** \brief Set termination mode (only supports AC/DC, for backward
 *         compatibility.
 *
 * \param[in]  dev_id    : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port  :
 * \param[in]  ln        :
 * \param[in]  ac_coupleda true=AC, false=DC:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_term_mode_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         bool ac_coupled) {
  uint32_t term_mode = ac_coupled ? AW_ACC_TERM_FL_AC : AW_ACC_TERM_FL_DC;

  return bf_tof3_serdes_term_mode_adv_set(dev_id, dev_port, ln, term_mode);
}

/****************************************************************
 * bf_tof3_serdes_term_mode_adv_get
 *
 * FIXME:
 * term_mode 0 = HI_Z
 *           1 = AC
 *           2 = DC
 *
 * FIELD: ACC_TERM_MODE_NT
 * DESCRIPTION:
 *   Termination mode
 *   111 - Reserved
 *   110 - Floating Termination, DC coupled
 *   101 - Termination to vss, DC coupled
 *   100 - Reserved
 *   011 - Reserved
 *   010 - Floating Termination, onchip AC coupling
 *   001 - Termination to vss, onchip AC coupling
 *   000 - High Z
 */
bf_status_t bf_tof3_serdes_term_mode_adv_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             aw_acc_term_mode_t *term_mode) {

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  return bf_aw_pmd_rx_termination_get(dev_id, dev_port, ln, term_mode);
}

/** \brief Get termination mode (only supports AC/DC, for backward
 *         compatibility.
 *
 * \param[in]  dev_id    : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port  :
 * \param[in]  ln        :
 * \param[out] ac_coupleda true=AC, false=DC:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_term_mode_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         bool *ac_coupled) {
  uint32_t adv_term_mode;
  bf_status_t rc;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  rc = bf_tof3_serdes_term_mode_adv_get(dev_id, dev_port, ln, &adv_term_mode);
  if (rc != BF_SUCCESS)
    return rc;

  // Map AW define to iegacy BF termination mode
  switch (adv_term_mode) {
  case AW_ACC_HI_Z:
    *ac_coupled = false;
    break;
  case AW_ACC_TERM_FL_AC: // = 2, // floating termination, onchip AC coupling
  case AW_ACC_TERM_VSS_AC:
    *ac_coupled = true;
    break;
  case AW_ACC_TERM_VSS_DC: // termination to vss, DC coupled
  case AW_ACC_TERM_FL_DC:  // termination to vss, DC coupled
    *ac_coupled = false;
    break;
  default:
    return BF_INVALID_ARG; // no mapping
    break;
  }
  return rc;
}

/** \brief  Cache Tx polarity in serdes struct and optionally apply to hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 * \param[out] apply      : true=apply to hw
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof3_serdes_tx_polarity_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           bool inv, bool apply) {
  bf_status_t rc = BF_SUCCESS;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t polarity;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }
  // Set tx polarity for a given lane.
  // 1 for inverted tx_polarity
  // 0 for normal operation
  polarity = inv ? 1 : 0;

  // save in cfg
  tf3_sd->cfg.tx_polarity = polarity;

  // write to HW, if requested
  if (apply) {
    rc = bf_aw_pmd_tx_polarity_set(dev_id, dev_port, ln, polarity);
  }
  return rc;
}

/** \brief  Retreive HW Tx polarity and cache in serdes cfg struct
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof3_serdes_tx_polarity_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           bool *inv) {
  bf_status_t rc;
  uint32_t polarity;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id)) {
    *inv = false;
    return BF_SUCCESS;
  }

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  // Set tx polarity for a given lane.
  // 1 for inverted tx_polarity
  // 0 for normal operation
  rc = bf_aw_pmd_tx_polarity_get(dev_id, dev_port, ln, &polarity);

  // save in cfg
  tf3_sd->cfg.tx_polarity = polarity;

  // return setting
  *inv = (polarity == 1) ? true : false;
  return rc;
}

/** \brief  Cache Rx polarity in serdes struct and optionally apply to hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 * \param[out] apply      : true=apply to hw
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof3_serdes_rx_polarity_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           bool inv, bool apply) {
  bf_status_t rc = BF_SUCCESS;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;
  uint32_t polarity;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }
  // Set tx polarity for a given lane.
  // 1 for inverted tx_polarity
  // 0 for normal operation
  polarity = inv ? 1 : 0;

  // save in cfg
  tf3_sd->cfg.rx_polarity = polarity;

  // write to HW, if requested
  if (apply) {
    rc = bf_aw_pmd_rx_polarity_set(dev_id, dev_port, ln, polarity);
  }
  return rc;
}

/** \brief  Retreive HW Rx polarity and cache in serdes cfg struct
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof3_serdes_rx_polarity_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           bool *inv) {
  bf_status_t rc;
  uint32_t polarity;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id)) {
    *inv = false;
    return BF_SUCCESS;
  }

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  // Set tx polarity for a given lane.
  // 1 for inverted tx_polarity
  // 0 for normal operation
  rc = bf_aw_pmd_rx_polarity_get(dev_id, dev_port, ln, &polarity);

  // save in cfg
  tf3_sd->cfg.rx_polarity = polarity;

  // return setting
  *inv = (polarity == 1) ? true : false;
  return rc;
}

/****************************************************************
 * Set TX FIR taps and enable override (to force tap use).
 * Update the serdes cfg struct with the new tap values.
 *
 * 'CM3': c(-3) value (pre-cursor 3), up to 4 max, 3b
 * 'CM2': c(-2) value  (pre-cursor 2), up to 7 max, 3b
 * 'CM1': c(-1) value  (pre-cursor 1), up to 24 max, 6b
 * 'C0': c(0) value  (main cursor), up to 60 max, 6b
 * 'C1': c(1) value  (post-cursor 1), up to 24 max, 6b
 *
 */
bf_status_t bf_tof3_serdes_txfir_config_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t cm3, uint32_t cm2,
                                            uint32_t cm1, uint32_t c0,
                                            uint32_t c1) {
  int rc;
  aw_txfir_config_t taps;
  uint32_t max_rng_cm3, max_rng_cm2, max_rng_cm1, max_rng_c1, max_rng_c0;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  rc = bf_aw_pmd_tx_tap_mode_get(dev_id, dev_port, ln, &max_rng_cm3,
                                 &max_rng_cm2, &max_rng_cm1, &max_rng_c1,
                                 &max_rng_c0);
  if ((cm3 > max_rng_cm3) || (cm2 > max_rng_cm2) || (cm1 > max_rng_cm1) ||
      (c1 > max_rng_c1) || (c0 > max_rng_c0)) {
    bf_aw_trace("*** Invalid Tx tap config", dev_id, dev_port, ln, 0);
    return BF_INVALID_ARG;
  }

  // save in cfg
  tf3_sd->cfg.cm3 = cm3;
  tf3_sd->cfg.cm2 = cm2;
  tf3_sd->cfg.cm1 = cm1;
  tf3_sd->cfg.c0 = c0;
  tf3_sd->cfg.c1 = c1;

  taps.CM3 = cm3;
  taps.CM2 = cm2;
  taps.CM1 = cm1;
  taps.C0 = c0;
  taps.C1 = c1;
  taps.main_or_max = 0;

  rc = bf_aw_pmd_txfir_config_set(dev_id, dev_port, ln, taps, 1);

  // rc already mapped
  return rc;
}

/****************************************************************
 *
 * Update TX FIR taps but do NOT modify configured taps. This
 * is used during bf_tof3_serdes_squelch_set() to shut off the
 * TX output but not require the taps to be set again.
 *
 * 'CM3': c(-3) value (pre-cursor 3), up to 4 max, 3b
 * 'CM2': c(-2) value  (pre-cursor 2), up to 7 max, 3b
 * 'CM1': c(-1) value  (pre-cursor 1), up to 24 max, 6b
 * 'C0': c(0) value  (main cursor), up to 60 max, 6b
 * 'C1': c(1) value  (post-cursor 1), up to 24 max, 6b
 *
 */
bf_status_t bf_tof3_serdes_txfir_hw_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t cm3, uint32_t cm2,
                                        uint32_t cm1, uint32_t c0,
                                        uint32_t c1) {
  int rc;
  aw_txfir_config_t taps;
  uint32_t max_rng_cm3, max_rng_cm2, max_rng_cm1, max_rng_c1, max_rng_c0;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  rc = bf_aw_pmd_tx_tap_mode_get(dev_id, dev_port, ln, &max_rng_cm3,
                                 &max_rng_cm2, &max_rng_cm1, &max_rng_c1,
                                 &max_rng_c0);
  if ((cm3 > max_rng_cm3) || (cm2 > max_rng_cm2) || (cm1 > max_rng_cm1) ||
      (c1 > max_rng_c1) || (c0 > max_rng_c0)) {
    bf_aw_trace("*** Invalid Tx tap config", dev_id, dev_port, ln, 0);
    return BF_INVALID_ARG;
  }

  taps.CM3 = cm3;
  taps.CM2 = cm2;
  taps.CM1 = cm1;
  taps.C0 = c0;
  taps.C1 = c1;
  taps.main_or_max = 0;

  rc = bf_aw_pmd_txfir_config_set(dev_id, dev_port, ln, taps, 1);

  // rc already mapped
  return rc;
}

/****************************************************************
 * Set or clear the TX FIR override enable.
 * The override enable determines whether the configured taps
 * are used or not. The override must be DISABLED during link-
 * training so the HW ANLT state-machine can configure the taps
 * determined by the training protocol.
 *
 */
bf_status_t bf_tof3_serdes_txfir_hw_ovrd_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t ovrd_en) {
  int rc;
  aw_txfir_config_t taps = {0};
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  bf_aw_pmd_txfir_config_get(dev_id, dev_port, ln, &taps);
  rc = bf_aw_pmd_txfir_config_set(dev_id, dev_port, ln, taps, ovrd_en ? 1 : 0);

  // rc already mapped
  return rc;
}

/****************************************************************
 * Return the state of the HW TX FIR taps. Do not update the
 * serdes cfg struct.
 *
 * 'CM3': c(-3) value (pre-cursor 3), up to 4 max, 3b
 * 'CM2': c(-2) value  (pre-cursor 2), up to 7 max, 3b
 * 'CM1': c(-1) value  (pre-cursor 1), up to 24 max, 6b
 * 'C0': c(0) value  (main cursor), up to 60 max, 6b
 * 'C1': c(1) value  (post-cursor 1), up to 24 max, 6b
 *
 */
bf_status_t bf_tof3_serdes_txfir_config_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *cm3, uint32_t *cm2,
                                            uint32_t *cm1, uint32_t *c0,
                                            uint32_t *c1) {
  int rc;
  aw_txfir_config_t taps;

  if (!bf_tof3_serdes_sppt(dev_id)) {
    *cm3 = *cm2 = *cm1 = *c0 = *c1 = 0;
    return BF_SUCCESS;
  }

  taps.main_or_max = 0; // this is an input!

  rc = bf_aw_pmd_txfir_config_get(dev_id, dev_port, ln, &taps);

  *cm3 = taps.CM3;
  *cm2 = taps.CM2;
  *cm1 = taps.CM1;
  *c0 = taps.C0;
  *c1 = taps.C1;

  // rc already mapped
  return rc;
}

/****************************************************************
 * Return the state of the HW TX FIR taps. Do not update the
 * serdes cfg struct.
 *
 * 'CM3': c(-3) value (pre-cursor 3), up to 4 max, 3b
 * 'CM2': c(-2) value  (pre-cursor 2), up to 7 max, 3b
 * 'CM1': c(-1) value  (pre-cursor 1), up to 24 max, 6b
 * 'C0': c(0) value  (main cursor), up to 60 max, 6b
 * 'C1': c(1) value  (post-cursor 1), up to 24 max, 6b
 *
 */
bf_status_t bf_tof3_serdes_txfir_range_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *cm3, uint32_t *cm2,
                                           uint32_t *cm1, uint32_t *c0,
                                           uint32_t *c1) {
  int rc;

  if (!bf_tof3_serdes_sppt(dev_id)) {
    *cm3 = *cm2 = *cm1 = *c0 = *c1 = 0;
    return BF_SUCCESS;
  }

  rc = bf_aw_pmd_tx_tap_mode_get(dev_id, dev_port, ln, cm3, cm2, cm1, c1, c0);

  // rc already mapped
  return rc;
}

/****************************************************************
 * Clear any loopback mode that might be configured.
 *
 */
void bf_tof3_serdes_loopback_clear_all(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln) {
  if (!bf_tof3_serdes_sppt(dev_id))
    return;

  bf_aw_pmd_nep_loopback_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_analog_loopback_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_fep_data_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_fes_loopback_set(dev_id, dev_port, ln, 0);
}

/****************************************************************
 * Configure the requested loopback mode and update cfg struct
 *
 */
bf_status_t bf_tof3_serdes_loopback_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t mode) {
  int rc = BF_SUCCESS;
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }
  // first, clear any existing loopback
  bf_tof3_serdes_loopback_clear_all(dev_id, dev_port, ln);

  if (mode == BF_LPBK_SERDES_NEAR) {
    rc = bf_aw_pmd_analog_loopback_set(dev_id, dev_port, ln, 1);
  } else if (mode == BF_LPBK_SERDES_NEAR_PARALLEL) {
    rc = bf_aw_pmd_nep_loopback_set(dev_id, dev_port, ln, 1);
  } else if (mode == BF_LPBK_SERDES_FAR) {
    rc = bf_aw_pmd_fes_loopback_set(dev_id, dev_port, ln, 1);
  } else if (mode == BF_LPBK_SERDES_FAR_PARALLEL) {
    rc = bf_aw_pmd_fep_data_set(dev_id, dev_port, ln, 1);
    rc = bf_aw_pmd_fep_clock_set(dev_id, dev_port, ln, 1);
  }

  // save in cfg
  tf3_sd->cfg.loopback_mode = mode;

  // rc already mapped
  return rc;
}

/** \brief Return programmed precoder settings
 *         Do not update cfg struct
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tx_en:
 * \param[out] rx_en:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_precode_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       bool *tx_en, bool *rx_en) {
  uint32_t tx_precoder_en_pc, rx_precoder_en_pc, unused;

  if (!bf_tof3_serdes_sppt(dev_id)) {
    *tx_en = *rx_en = false;
    return BF_SUCCESS;
  }

  bf_aw_pmd_tx_pam4_precoder_enable_get(dev_id, dev_port, ln, &unused,
                                        &tx_precoder_en_pc);
  bf_aw_pmd_rx_pam4_precoder_enable_get(dev_id, dev_port, ln, &unused,
                                        &rx_precoder_en_pc);

  *tx_en = tx_precoder_en_pc ? true : false;
  *rx_en = rx_precoder_en_pc ? true : false;
  return BF_SUCCESS;
}

/** \brief Program precoder settings, update serdes cfg struct
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tx_en:
 * \param[out] rx_en:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_precode_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       bool tx_en, bool rx_en) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  if (!bf_tof3_serdes_sppt(dev_id))
    return BF_SUCCESS;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);
  if (!tf3_sd) {
    return BF_INVALID_ARG;
  }

  tf3_sd->cfg.tx_precoder_override = 1; // always on
  tf3_sd->cfg.tx_precoder_en_gc = 1;    // precode implies PAM4, so gc
  tf3_sd->cfg.tx_precoder_en_pc = (tx_en ? 1 : 0);
  bf_aw_pmd_tx_pam4_precoder_override_set(dev_id, dev_port, ln,
                                          tf3_sd->cfg.tx_precoder_override);
  bf_aw_pmd_tx_pam4_precoder_enable_set(dev_id, dev_port, ln,
                                        tf3_sd->cfg.tx_precoder_en_gc,
                                        tf3_sd->cfg.tx_precoder_en_pc);

  tf3_sd->cfg.rx_precoder_override = 1; // always on
  tf3_sd->cfg.rx_precoder_en_gc = 1;    // precode implies PAM4, so gc
  tf3_sd->cfg.rx_precoder_en_pc = (rx_en ? 1 : 0);
  bf_aw_pmd_rx_pam4_precoder_override_set(dev_id, dev_port, ln,
                                          tf3_sd->cfg.rx_precoder_en_gc);
  bf_aw_pmd_rx_pam4_precoder_enable_set(dev_id, dev_port, ln,
                                        tf3_sd->cfg.rx_precoder_en_gc,
                                        tf3_sd->cfg.rx_precoder_en_pc);

  return BF_SUCCESS;
}

/** \brief Reset the PRBS error count
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_prbs_rst_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln) {
  bf_status_t rc;

  // disable BIST first
  rc = bf_aw_pmd_rx_chk_en_set(dev_id, dev_port, ln, 0);
  if (rc != BF_SUCCESS)
    return rc;

  // clear error counts
  rc = bf_aw_pmd_rx_chk_err_count_state_clear(dev_id, dev_port, ln);
  if (rc != BF_SUCCESS)
    return rc;

  // enable BIST
  rc = bf_aw_pmd_rx_chk_en_set(dev_id, dev_port, ln, 1);

  return rc;
}

/** \brief Return the PRBS error count
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] err_cnt  : 32b error count (note: wraps)
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_rx_prbs_err_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *err_cnt) {
  int rc;
  uint64_t err_count;
  uint32_t err_count_done;
  uint32_t err_count_overflown;

  rc = bf_aw_pmd_rx_chk_err_count_state_get(
      dev_id, dev_port, ln, &err_count, &err_count_done, &err_count_overflown);
  *err_cnt = (uint32_t)err_count;
  // hack
  return 0;

  if (rc != BF_SUCCESS) {
    return rc;
  } else if (err_count_overflown) {
    *err_cnt = 0xffffffff;
    return BF_INVALID_ARG;
  } else if (err_count > 0xfffffffful) {
    *err_cnt = 0xffffffff;
    return BF_INVALID_ARG;
  } else {
    *err_cnt = (uint32_t)err_count;
  }
  return BF_SUCCESS;
}

/****************************************************************
 * Read an AW CSR from a specific side, TX or RX. This fn only
 * applies to the AW DFX registers. For others, the side is
 * determined by the block being referenced,
 *
 * TX section = TX side
 * RX section = RX side
 * AN section = TX side
 *
 * This is used by the python interface to provide a way of
 * accessing a particular side.
 */
bf_status_t bf_tof3_serdes_csr_rd_specific_side(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t section,
                                                uint32_t csr, uint32_t *val) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, section);
  if (tf3_sd == NULL)
    return 0xbad1dea1;
  if (tf3_sd->api == NULL)
    return 0xbad1dea2;

  return tf3_sd->api->pmd_rd_csr(&mss, csr, val);
}

/****************************************************************
 * Read an AW CSR from the "default" side.
 *
 * The "default" side for the DFX registers is RX (just based on
 * the argument below).
 */
bf_status_t bf_tof3_serdes_csr_rd(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                  uint32_t ln, uint32_t csr, uint32_t *val) {
  return bf_tof3_serdes_csr_rd_specific_side(dev_id, dev_port, ln,
                                             MSS_SECTION_RX, csr, val);
}

/****************************************************************
 * Write an AW CSR to a specific side, TX or RX. This fn only
 * applies to the AW DFX registers. For others, the side is
 * determined by the block being referenced,
 *
 * TX section = TX side
 * RX section = RX side
 * AN section = TX side
 *
 * This is used by the python interface to provide a way of
 * accessing a particular side.
 *
 */
bf_status_t bf_tof3_serdes_csr_wr_specific_side(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t section,
                                                uint32_t csr, uint32_t val) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, section);
  if (tf3_sd == NULL)
    return 0xbad1dea1;
  if (tf3_sd->api == NULL)
    return 0xbad1dea2;

  return tf3_sd->api->pmd_wr_csr(&mss, csr, val);
}

/****************************************************************
 * Write an AW CSR to the "default" side.
 *
 * The "default" side for the DFX registers is RX (just based on
 * the argument below).
 */
bf_status_t bf_tof3_serdes_csr_wr(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                  uint32_t ln, uint32_t csr, uint32_t val) {
  return bf_tof3_serdes_csr_wr_specific_side(dev_id, dev_port, ln,
                                             MSS_SECTION_RX, csr, val);
}

/****************************************************************
 *
 */
bf_status_t bf_tof3_serdes_clk_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, int32_t *tx_ppm,
                                   int32_t *rx_ppm, double *tx_vco_freq,
                                   double *rx_vco_freq) {
  uint32_t timing_window = 15;
  uint32_t ppm_timeout_us = 0x0000FFFF;
  double dbl_tx_ppm, dbl_rx_ppm;
  /* note: the below comes from the yaml file associated with
   * the serdes FW file. It matches the current 16ln FW. It
   * may not be correct for the 4ln FW.
   */
  double refclk_freq = 3320.3125 * 1000000;
  bf_status_t rc;

  suppress_aw_prints_set(1);

  rc = bf_aw_pmd_tx_ppm_get(dev_id, dev_port, ln, timing_window, ppm_timeout_us,
                            &dbl_tx_ppm, tx_vco_freq, refclk_freq);
  if (rc != BF_SUCCESS)
    return rc;
  *tx_ppm = (int32_t)dbl_tx_ppm;

  rc = bf_aw_pmd_rx_ppm_get(dev_id, dev_port, ln, timing_window, ppm_timeout_us,
                            &dbl_rx_ppm, rx_vco_freq, refclk_freq);

  suppress_aw_prints_set(0);

  if (rc != BF_SUCCESS)
    return rc;
  *rx_ppm = (int32_t)dbl_rx_ppm;
  return rc;
}

/****************************************************************
 * Return a quick calculation of BER assuming PRBS is configured
 */
bf_status_t bf_tof3_serdes_ber_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, double *ber) {
  uint32_t cdr_lock, err_cnt;
  uint32_t integration_ms = 10; // 10ms
  bf_port_speed_t speed;
  int n_lanes;
  bool is_pam4;
  uint32_t rate_gb;
  uint64_t bps;

  bf_tof3_serdes_cdr_lock_get(dev_id, dev_port, ln, &cdr_lock);
  if (!cdr_lock) {
    *ber = 1.0;
    return 0;
  }
  bf_tof3_serdes_prbs_rst_set(dev_id, dev_port, ln);
  bf_sys_usleep(integration_ms * 1000);
  bf_tof3_serdes_rx_prbs_err_get(dev_id, dev_port, ln, &err_cnt);

  /* get ports aggregate bit-rate */
  // get serdes speed and encoding mode from port speed
  bf_port_speed_get(dev_id, dev_port, &speed);
  bf_port_num_lanes_get(dev_id, dev_port, &n_lanes);
  bf_tof3_serdes_port_speed_to_serdes_speed(dev_id, dev_port, ln, speed,
                                            n_lanes, &rate_gb, &is_pam4);

  uint64_t serdes_clk_nrz_1g = 1031250;
  uint64_t serdes_clk_nrz_10g = 10312500;
  uint64_t serdes_clk_nrz_25g = 25781250;
  uint64_t serdes_clk_pam4_53g = 53125000;
  uint64_t serdes_clk_pam4_106g = 2 * serdes_clk_pam4_53g;

  if (rate_gb == 1) {
    bps = serdes_clk_nrz_1g;
  } else if (rate_gb == 10) {
    bps = serdes_clk_nrz_10g;
  } else if (rate_gb == 20) {
    bps = serdes_clk_nrz_10g * 2;
  } else if (rate_gb == 25) {
    bps = serdes_clk_nrz_25g;
  } else if (rate_gb == 50) {
    bps = serdes_clk_pam4_53g;
  } else if (rate_gb == 100) {
    bps = serdes_clk_pam4_106g;
  } else {
    *ber = 1.0;
    return 0;
  }
  bps *= 1000; // note clk speed was in Mhz, not Ghz

  bps = (bps * integration_ms / 1000);
  *ber = (double)((double)err_cnt / (double)bps);
  return 0;
}

/****************************************************************
 * Populate the lane_cfg_t with values read from HW. This fn
 * returns most of the configuration settings used to configure
 * a lane.
 *
 * Note: some of the AW APIs print messages. Here we suppress
 *       any messages from the AW code since this function is
 *       called from CLI commands that only want their own
 *       output displayed.
 */
bf_status_t bf_tof3_serdes_status_get(uint32_t dev_id, uint32_t dev_port,
                                      uint32_t ln, lane_cfg_t *rtnd_cfg) {
  lane_cfg_t *cfg = rtnd_cfg;

  suppress_aw_prints_set(1);

  bf_aw_pmd_rx_termination_get(dev_id, dev_port, ln, &cfg->rx_term);
  bf_aw_pmd_iso_tx_reset_get(dev_id, dev_port, ln, &cfg->tx_reset);
  bf_aw_pmd_iso_rx_reset_get(dev_id, dev_port, ln, &cfg->rx_reset);
#if 0
  bf_aw_pmd_iso_tx_rate_get(dev_id, dev_port, ln, &cfg->tx_rate);
  bf_aw_pmd_iso_rx_rate_get(dev_id, dev_port, ln, &cfg->rx_rate);
  bf_aw_pmd_iso_tx_width_get(dev_id, dev_port, ln, &cfg->tx_width);
  bf_aw_pmd_iso_rx_width_get(dev_id, dev_port, ln, &cfg->rx_width);
  bf_aw_pmd_iso_tx_pstate_get(dev_id, dev_port, ln, &cfg->tx_pstate);
  bf_aw_pmd_iso_rx_pstate_get(dev_id, dev_port, ln, &cfg->rx_pstate);
#endif
  /* The above APIs return the value read from an AW CSR. That value
   * does not necessarily represent the actual value being used. The
   * "real" value can be read from the RX/TXMFSM registers. So we use
   * the API below instead of what you might have expected.
   */
  bf_aw_pmd_cur_rate_width_pstate(
      dev_id, dev_port, ln, &cfg->tx_rate, &cfg->tx_width, &cfg->tx_pstate,
      &cfg->rx_rate, &cfg->rx_width, &cfg->rx_pstate);

  bf_tof3_serdes_txfir_config_get(dev_id, dev_port, ln, &cfg->cm3, &cfg->cm2,
                                  &cfg->cm1, &cfg->c0, &cfg->c1);
  bf_aw_pmd_tx_gen_config_get(dev_id, dev_port, ln, &cfg->prbs_mode,
                              &cfg->user_data_pat);
  bf_aw_pmd_gen_tx_en_get(dev_id, dev_port, ln, &cfg->prbs_gen_tx_en);
  bf_aw_pmd_tx_pam4_precoder_override_get(dev_id, dev_port, ln,
                                          &cfg->tx_precoder_override);
  bf_aw_pmd_tx_pam4_precoder_enable_get(
      dev_id, dev_port, ln, &cfg->tx_precoder_en_gc, &cfg->tx_precoder_en_pc);
  bf_aw_pmd_rx_pam4_precoder_override_get(dev_id, dev_port, ln,
                                          &cfg->rx_precoder_override);
  bf_aw_pmd_rx_pam4_precoder_enable_get(
      dev_id, dev_port, ln, &cfg->rx_precoder_en_gc, &cfg->rx_precoder_en_pc);
  bf_aw_pmd_tx_polarity_get(dev_id, dev_port, ln, &cfg->tx_polarity);
  bf_aw_pmd_rx_polarity_get(dev_id, dev_port, ln, &cfg->rx_polarity);
  // check each loopback type, but return the first enablled type
  // checked in order of near then far and distance from the core
  // nep, nes, fep, fes
  uint32_t nep, nes, fep, fes;
  bf_aw_pmd_nep_loopback_get(dev_id, dev_port, ln, &nep);
  bf_aw_pmd_analog_loopback_get(dev_id, dev_port, ln, &nes);
  bf_aw_pmd_fep_data_get(dev_id, dev_port, ln, &fep);
  bf_aw_pmd_fes_loopback_get(dev_id, dev_port, ln, &fes);
  if (nes) {
    cfg->loopback_mode = BF_LPBK_SERDES_NEAR;
  } else if (nep) {
    cfg->loopback_mode = BF_LPBK_SERDES_NEAR_PARALLEL;
  } else if (fep) {
    cfg->loopback_mode = BF_LPBK_SERDES_FAR_PARALLEL;
  } else if (fes) {
    cfg->loopback_mode = BF_LPBK_SERDES_FAR;
  } else {
    cfg->loopback_mode = BF_LPBK_NONE;
  }
  bf_aw_pmd_tx_disable_get(dev_id, dev_port, ln, &cfg->tx_disable);
  bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, &cfg->sig_det);

  // AFE settings
  aw_afe_data_t rx_afe_data;
  bf_aw_pmd_rx_afe_get(dev_id, dev_port, ln, &rx_afe_data);
  cfg->ctle_rate = rx_afe_data.ctle_rate;
  cfg->ctle_boost = rx_afe_data.ctle_boost;
  cfg->vga_coarse = rx_afe_data.vga_coarse;
  cfg->vga_fine = rx_afe_data.vga_fine;
  cfg->vga_offset = rx_afe_data.vga_offset;

  if (cfg->sig_det) {
    uint32_t rc =
        bf_aw_pmd_rx_check_cdr_lock(dev_id, dev_port, ln, 100 /*100*/);
    cfg->cdr_lock = (rc == AW_ERR_CODE_NONE) ? 1 : 0;
  } else {
    uint32_t rc = bf_aw_pmd_rx_check_cdr_lock(dev_id, dev_port, ln, 1);
    cfg->cdr_lock = (rc == AW_ERR_CODE_NONE) ? 1 : 0;
  }
  bf_aw_pmd_rx_chk_lock_state_get(dev_id, dev_port, ln, &cfg->bist_lock);
  bf_tof3_serdes_ber_get(dev_id, dev_port, ln, &cfg->ber);

  if (cfg->sig_det) {
    double tx_vco_freq, rx_vco_freq;

    bf_tof3_serdes_clk_get(dev_id, dev_port, ln, &cfg->tx_ppm, &cfg->rx_ppm,
                           &tx_vco_freq, &rx_vco_freq);
    (void)tx_vco_freq;
    (void)rx_vco_freq;
  } else {
    cfg->tx_ppm = 0;
    cfg->rx_ppm = 0;
  }
  suppress_aw_prints_set(0);
  return 0;
}

/****************************************************************
 * default lane_cfg, set at start-up
 *
 * Most of these parameters will be overridden on port-enb
 */
static lane_cfg_t default_lane_cfg = {
    .rx_term = 2, // AW_ACC_TERM_FL_AC
    .tx_rate = 6,
    .rx_rate = 6, // 6=53g, 7=106g
    .tx_width = 7,
    .rx_width = 7,
    .tx_pstate = 4, // powered dn 0, //powerd up
    .rx_pstate = 4, // powered dn 0,
    .cm3 = 0,
    .cm2 = 2,
    .cm1 = 10,
    .c0 = 40,
    .c1 = 0,
    .prbs_mode = AW_PRBS31,
    .user_data_pat = 0,
    .prbs_gen_tx_en = PRBS_EN,
    .rx_prbs_chk_en = PRBS_EN,
    .tx_precoder_override = 1,
    .tx_precoder_en_gc = 1,
    .tx_precoder_en_pc = 0,
    .rx_precoder_override = 1,
    .rx_precoder_en_gc = 1,
    .rx_precoder_en_pc = 0,
    .tx_polarity = 0,
    .rx_polarity = 0,
    .loopback_mode = BF_LPBK_NONE,
    .rx_ctle_adapt_en = 1,
    .rx_ctle_adapt_boost = 12,
    .rx_vga_cap = 1,
};

/*****************************************************************************
 * Assert reset on both TX and RX sides of a lane in isolation mode
 */
bf_status_t bf_tof3_serdes_lane_reset_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln) {
  bf_aw_pmd_iso_tx_reset_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_iso_rx_reset_set(dev_id, dev_port, ln, 0);
  return 0;
}

/*****************************************************************************
 * De-assert reset on both TX and RX sides of a lane in isolation mode
 */
bf_status_t bf_tof3_serdes_lane_unreset_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln) {
  bf_aw_pmd_iso_tx_reset_set(dev_id, dev_port, ln, 1);
  bf_aw_pmd_iso_rx_reset_set(dev_id, dev_port, ln, 1);
  return 0;
}

/*****************************************************************************
 * Copy the passed config struct to the specified lane's cfg struct. Typically
 * used to copy the default config
 */
bf_status_t bf_tof3_serdes_lane_config_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           lane_cfg_t *lane_cfg) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  tf3_sd->cfg = *lane_cfg;
  return 0;
}

/*****************************************************************************
 * Copy the specified lane's cfg struct to the passed cfg struct. Typically
 * used to modify the contents of a portion of the cfg struct.
 */
bf_status_t bf_tof3_serdes_lane_config_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           lane_cfg_t **lane_cfg) {
  mss_access_t mss;
  bf_tf3_sd_t *tf3_sd;

  tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // bf_aw_trace("default_config_get", dev_id, dev_port, 0, 0);
  *lane_cfg = &tf3_sd->cfg;
  return 0;
}

/*****************************************************************************
 * bf_tof3_serdes_rx_vga_cap_adapt_get
 */
bf_status_t bf_tof3_serdes_rx_vga_cap_adapt_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t *en,
    uint32_t *vga_cap, bool *use_custom_takeover_ratio,
    uint32_t *custom_takeover_ratio, uint32_t *custom_nyq_mask) {
  bf_status_t rc;
  vga_opt_t opts = {0};

  rc = bf_aw_pmd_rx_vga_cap_adapt_get(dev_id, dev_port, ln, &opts);
  // error or not, just copy the returned values
  *en = opts.en;
  *vga_cap = opts.vga_cap;
  *use_custom_takeover_ratio = opts.use_custom_takeover_ratio;
  *custom_takeover_ratio = opts.custom_takeover_ratio;
  *custom_nyq_mask = opts.custom_nyq_mask;
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_rx_vga_cap_adapt_set
 *
 *        'en': VGA cap adaptation enable
 *              0 - VGA cap adapt Disabled
 *              1 - VGA cap adapt Enabled
 *        'vga_cap': VGA cap code ranges from 0-3
 *        'use_custom_takeover_ratio': 0 - Use the custom takeover ratio, as
 * specified by 'takeover ratio' 1 - Use takeover ratio hardcoded in FW
 *        'custom_takeover_ratio': Custom takeover ratio converted to a format
 * that FW accepts 'custom_nyq_mask': Unsigned 8-bit integer whose bits [7:0]
 * represent whether the nyquist energies for [1p00..p125] will be used uint32_t
 * custom_nyq_mask;
 */
bf_status_t bf_tof3_serdes_rx_vga_cap_adapt_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t en,
    uint32_t vga_cap, bool use_custom_takeover_ratio,
    uint32_t custom_takeover_ratio, uint32_t custom_nyq_mask) {
  bf_status_t rc = 0;
  vga_opt_t opts = {0};

  opts.en = en;
  opts.vga_cap = vga_cap;
  opts.use_custom_takeover_ratio = use_custom_takeover_ratio;
  opts.custom_takeover_ratio = custom_takeover_ratio;
  opts.custom_nyq_mask = custom_nyq_mask;

  rc = bf_aw_pmd_rx_vga_cap_adapt_set(dev_id, dev_port, ln, &opts);
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_ctle_adapt_set
 *
 */
bf_status_t bf_tof3_serdes_ctle_adapt_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t rx_ctle_adapt_en,
                                          uint32_t rx_ctle_adapt_boost) {
  bf_status_t rc = 0;

  rc = bf_aw_pmd_rx_ctle_adapt_set(dev_id, dev_port, ln, rx_ctle_adapt_en,
                                   rx_ctle_adapt_boost);
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_ctle_adapt_get
 *
 */
bf_status_t bf_tof3_serdes_ctle_adapt_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t *rx_ctle_adapt_en,
                                          uint32_t *rx_ctle_adapt_boost) {
  bf_status_t rc = 0;

  rc = bf_aw_pmd_rx_ctle_adapt_get(dev_id, dev_port, ln, rx_ctle_adapt_en,
                                   rx_ctle_adapt_boost);
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_squelch_set
 *
 * Note: when un-squelching (en=0) the FIR taps must be re-programmed
 *       afterwards.
 */
bf_status_t bf_tof3_serdes_squelch_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t en) {
  bf_status_t rc = 0;

  if (en) { // if squelch requested
    //
    // turn of an TX output by setting the main tap to "0"
    //
    // Note: this does NOT update the cfg so it can be restored
    // when un-squelched.
    //
    bf_tof3_serdes_txfir_hw_set(dev_id, dev_port, ln, 0, 0, 0, 0, 1);
  } else {
    //
    // "Un-squelch" by restoring the configured TX taps
    //
    lane_cfg_t *cfg;

    // get lane config struct
    rc = bf_tof3_serdes_lane_config_get(dev_id, dev_port, ln, &cfg);
    if (rc) {
      printf("bf_tof3_serdes_config_lane: cant locate lane_cfg\n");
      return BF_INVALID_ARG;
    }
    bf_tof3_serdes_txfir_config_set(dev_id, dev_port, ln, cfg->cm3, cfg->cm2,
                                    cfg->cm1, cfg->c0, cfg->c1);
  }
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_pstate_set
 *
 * Set Tx and Rx power state synchronously.
 *
 * Note: no longer used
 *
 */
bf_status_t bf_tof3_serdes_pstate_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t tx_pstate, uint32_t rx_pstate) {
  bf_status_t rc = 0;
  uint32_t state_req_ack_polls = 10;     // 1000;
  uint32_t state_req_ack_delay = 100000; // 100;

  rc = bf_aw_pmd_iso_tx_pstate_set(dev_id, dev_port, ln, tx_pstate);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_tof3_serdes_tx_state_req_set_synchronouos(
      dev_id, dev_port, ln, state_req_ack_polls, state_req_ack_delay);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_aw_pmd_iso_rx_pstate_set(dev_id, dev_port, ln, rx_pstate);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_tof3_serdes_rx_state_req_set_synchronous(
      dev_id, dev_port, ln, state_req_ack_polls, state_req_ack_delay);
  if (rc != BF_SUCCESS)
    return rc;

  return rc;
}

/*****************************************************************************
 *
 * Return the serdes rate, in gbps, for the specified port.
 *
 * Note: dev_id, dev_port, and ln are not actually required for this fn
 * but are maintained to keep the signature of "bf_tof3_serdes_" fn's
 * similar.
 */
bf_status_t bf_tof3_serdes_port_speed_to_serdes_speed(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    bf_port_speed_t speed, uint32_t num_lanes, uint32_t *serdes_gb,
    bool *is_pam4) {
  if ((!serdes_gb) || (!is_pam4)) {
    return BF_INVALID_ARG;
  }
  // ensure something is there
  *serdes_gb = 0;
  *is_pam4 = false;

  if (speed == BF_SPEED_NONE) {
    return BF_SUCCESS;
  } else if (speed == BF_SPEED_1G) {
    *serdes_gb = 1;
    *is_pam4 = false;
  } else if (speed == BF_SPEED_10G) {
    *serdes_gb = 10;
    *is_pam4 = false;
  } else if (speed == BF_SPEED_25G) {
    *serdes_gb = 25;
    *is_pam4 = false;
  } else if (speed == BF_SPEED_40G) {
    *serdes_gb = 10;
    *is_pam4 = false;
  } else if (speed == BF_SPEED_50G) {
    if (num_lanes == 2) { // 2x25g NRZ
      *serdes_gb = 25;
      *is_pam4 = false;
    } else if (num_lanes == 1) { // 1x50g PAM4
      *serdes_gb = 50;
      *is_pam4 = true;
    } else {
      return BF_INVALID_ARG;
    }
  } else if (speed == BF_SPEED_100G) {
    if (num_lanes == 4) { // 4x25g NRZ
      *serdes_gb = 25;
      *is_pam4 = false;
    } else if (num_lanes == 2) { // 2x50g PAM4
      *serdes_gb = 50;
      *is_pam4 = true;
    } else if (num_lanes == 1) { // 1x100g PAM4
      *serdes_gb = 100;
      *is_pam4 = true;
    } else {
      return BF_INVALID_ARG;
    }
  } else if (speed == BF_SPEED_200G) {
    if (num_lanes == 8) { // 8x25g NRZ
      *serdes_gb = 25;
      *is_pam4 = false;
    } else if (num_lanes == 4) { // 4x50g PAM4
      *serdes_gb = 50;
      *is_pam4 = true;
    } else if (num_lanes == 2) { // 2x100g PAM4
      *serdes_gb = 100;
      *is_pam4 = true;
    } else {
      return BF_INVALID_ARG;
    }
  } else if (speed == BF_SPEED_400G) {
    if (num_lanes == 8) { // 8x50g PAM4
      *serdes_gb = 50;
      *is_pam4 = true;
    } else if (num_lanes == 4) { // 4x100g PAM4
      *serdes_gb = 100;
      *is_pam4 = true;
    } else {
      return BF_INVALID_ARG;
    }
  } else if (speed == BF_SPEED_40G_R2) { // 40G 2x20G NRZ, Non-standard speed
    *serdes_gb = 20;
    *is_pam4 = false;
  } else if (speed == BF_SPEED_50G_CONS) { // 50G 2x25G NRZ, Consortium mode
    *serdes_gb = 25;
    *is_pam4 = false;
  } else {
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 *
 * Return the serdes rate code and width code associated with this port.
 * These codes are specific to the IP, either 4ln or 16ln, so must be
 * retrieved thru the API vector.
 *
 */
bf_status_t bf_tof3_serdes_serdes_speed_to_rate_and_width(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t serdes_speed, bool is_pam4, uint32_t *rate, uint32_t *width) {
  bf_status_t rc = 0;

  rc = bf_aw_pmd_speed_to_rate_and_width(dev_id, dev_port, ln, serdes_speed,
                                         is_pam4, rate, width);
  return rc;
}

/*****************************************************************************
 *
 * Initialize a serdes lane in a step-wise fashion, allowing any delays to
 * be amortized over multiple lanes at a time. This is the function used
 * by the non-ANLT FSMs to initialize a serdes lane.
 *
 */
#define STEP_CFG_RATE 0x00000001
#define STEP_APPLY_TX_RATE 0x00000002
#define STEP_WAIT_TX_RATE_CHG_DONE 0x00000004
#define STEP_TX_RATE_CHG_DONE 0x00000008
#define STEP_APPLY_RX_RATE 0x00000010
#define STEP_WAIT_RX_RATE_CHG_DONE 0x00000020
#define STEP_RX_RATE_CHG_DONE 0x00000040
#define STEP_CFG_MODE 0x00000080
#define STEP_WAIT_SIG_OK 0x00000100
#define STEP_EQ_START 0x00000200
#define STEP_CHECK_EQ_DONE 0x00000400
#define STEP_CHECK_CDR_LOCK 0x00000800
#define STEP_CHECK_BIST_LOCK 0x00001000
#define STEP_ALL_ETH                                                           \
  (STEP_CFG_RATE | STEP_APPLY_TX_RATE | STEP_WAIT_TX_RATE_CHG_DONE |           \
   STEP_TX_RATE_CHG_DONE | STEP_APPLY_RX_RATE | STEP_WAIT_RX_RATE_CHG_DONE |   \
   STEP_RX_RATE_CHG_DONE | STEP_CFG_MODE | STEP_WAIT_SIG_OK | STEP_EQ_START |  \
   STEP_CHECK_EQ_DONE | STEP_CHECK_CDR_LOCK)
#define STEP_ALL_BIST (STEP_ALL_ETH | STEP_CHECK_BIST_LOCK)

bf_status_t bf_tof3_serdes_lane_init_step(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          lane_cfg_t *cfg, uint32_t step) {
  bf_status_t rc = BF_SUCCESS;
  uint32_t rx_bist_lock = 0;
  uint32_t lock_threshold = 1;

  if (step & STEP_CFG_RATE) {

    // isolate lane (does this also un-reset the lane?)
    rc = bf_aw_pmd_isolate_lane_set(dev_id, dev_port, ln, 1);

    // squelch ch during reconfig
    rc = bf_tof3_serdes_squelch_set(dev_id, dev_port, ln, 1);

    // bf_tof3_serdes_lane_reset(dev_id, dev_port, ln);
    bf_tof3_serdes_lane_reset_set(dev_id, dev_port, ln);

    // clear FEP and NES, if necessary
    rc = bf_aw_pmd_analog_loopback_set(dev_id, dev_port, ln, 0);

    rc = bf_aw_pmd_vfld_eqbk_hold_req_set(dev_id, dev_port, ln, 1);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_tof3_serdes_term_mode_adv_set(dev_id, dev_port, ln, cfg->rx_term);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_tx_rate_set(dev_id, dev_port, ln, cfg->tx_rate);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_rx_rate_set(dev_id, dev_port, ln, cfg->rx_rate);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_tx_width_set(dev_id, dev_port, ln, cfg->tx_width);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_rx_width_set(dev_id, dev_port, ln, cfg->rx_width);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_tx_pstate_set(dev_id, dev_port, ln, cfg->tx_pstate);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_rx_pstate_set(dev_id, dev_port, ln, cfg->rx_pstate);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_rx_background_adapt_enable_set(dev_id, dev_port, ln, 1);
    if (rc != BF_SUCCESS)
      return rc;

    // according to AW, this vfield should be left "0" for both ANLT and non-
    // If set, we see non-ANLT link-up failing even when using Tx taps
    // identified by LT.
    //
    // rc = bf_aw_pmd_rxmfsm_eq_check_rxdisable_set(dev_id, dev_port, ln, 1);
    // if (rc != BF_SUCCESS) return rc;

    bf_tof3_serdes_lane_unreset_set(dev_id, dev_port, ln);
  }

  // change Tx state
  if (step & STEP_APPLY_TX_RATE) {
    rc = bf_tof3_serdes_tx_state_req_set(dev_id, dev_port, ln,
                                         0); // force clear first
    rc = bf_tof3_serdes_tx_state_req_set(dev_id, dev_port, ln, 1);
    if (rc != BF_SUCCESS)
      return rc;
  }

  // delay for ack to be set
  if ((step == STEP_ALL_ETH) || (step == STEP_ALL_BIST)) {
    bf_sys_usleep(100 * 1000);
  }

  if (step & STEP_WAIT_TX_RATE_CHG_DONE) {
    uint32_t ack = 0;

    rc = bf_tof3_serdes_tx_state_ack_get(dev_id, dev_port, ln, &ack);
    if (rc != BF_SUCCESS)
      return rc;
    if (!ack) {
      // if just checking this state, indicate not done
      if (step == STEP_WAIT_TX_RATE_CHG_DONE) {
        return BF_NOT_READY;
      }
    }
  }
  if (step & STEP_TX_RATE_CHG_DONE) {
    rc = bf_tof3_serdes_tx_state_req_set(dev_id, dev_port, ln, 0);
  }

  // change Rx state
  if (step & STEP_APPLY_RX_RATE) {
    rc = bf_tof3_serdes_rx_state_req_set(dev_id, dev_port, ln,
                                         0); // force clear first
    rc = bf_tof3_serdes_rx_state_req_set(dev_id, dev_port, ln, 1);
    if (rc != BF_SUCCESS)
      return rc;
  }

  // delay for ack to be set
  if ((step == STEP_ALL_ETH) || (step == STEP_ALL_BIST)) {
    bf_sys_usleep(100 * 1000);
  }

  if (step & STEP_WAIT_RX_RATE_CHG_DONE) {
    uint32_t ack = 0;

    rc = bf_tof3_serdes_rx_state_ack_get(dev_id, dev_port, ln, &ack);
    if (rc != BF_SUCCESS)
      return rc;
    if (!ack) {
      // if just checking this state, indicate not done
      if (step == STEP_WAIT_RX_RATE_CHG_DONE) {
        return BF_NOT_READY;
      }
    }
  }
  if (step & STEP_RX_RATE_CHG_DONE) {
    rc = bf_tof3_serdes_rx_state_req_set(dev_id, dev_port, ln, 0);
  }

  // basic config set
  if (step & STEP_CFG_MODE) {
    rc = bf_aw_pmd_tx_gen_config_set(dev_id, dev_port, ln, cfg->prbs_mode,
                                     cfg->user_data_pat);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_gen_tx_en_set(dev_id, dev_port, ln, cfg->prbs_gen_tx_en);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_tx_pam4_precoder_override_set(dev_id, dev_port, ln,
                                                 cfg->tx_precoder_override);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_tx_pam4_precoder_enable_set(
        dev_id, dev_port, ln, cfg->tx_precoder_en_gc, cfg->tx_precoder_en_pc);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_rx_pam4_precoder_override_set(dev_id, dev_port, ln,
                                                 cfg->rx_precoder_override);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_rx_pam4_precoder_enable_set(
        dev_id, dev_port, ln, cfg->rx_precoder_en_gc, cfg->rx_precoder_en_pc);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_tx_polarity_set(dev_id, dev_port, ln, cfg->tx_polarity);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_rx_polarity_set(dev_id, dev_port, ln, cfg->rx_polarity);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_tof3_serdes_loopback_set(dev_id, dev_port, ln, cfg->loopback_mode);
    if (rc != BF_SUCCESS)
      return rc;

    if (cfg->loopback_mode == BF_LPBK_SERDES_NEAR) { // force sigdetect high
      aw_force_sigdet_mode_t sigdet_mode = AW_SIGDET_FORCE1;
      rc = bf_aw_pmd_force_signal_detect_config_set(dev_id, dev_port, ln,
                                                    sigdet_mode);
      if (rc != BF_SUCCESS)
        return rc;
    } else {
      aw_force_sigdet_mode_t sigdet_mode = AW_SIGDET_NORM;
      rc = bf_aw_pmd_force_signal_detect_config_set(dev_id, dev_port, ln,
                                                    sigdet_mode);
      if (rc != BF_SUCCESS)
        return rc;
    }

    bf_tof3_serdes_txfir_config_set(dev_id, dev_port, ln, cfg->cm3, cfg->cm2,
                                    cfg->cm1, cfg->c0, cfg->c1);

    // this vfld is required for ctle/vga cap to adapt
    bf_aw_pmd_rxeq_prbs_set(dev_id, dev_port, ln, 1);
  }

  // wait sig-detect
  if (step & STEP_WAIT_SIG_OK) {
    uint32_t signal_detect = 0;
    rc = bf_aw_pmd_rx_signal_detect_get(dev_id, dev_port, ln, &signal_detect);
    if (rc != BF_SUCCESS)
      return rc;

    // if just checking this state, indicate not done
    if ((step == STEP_WAIT_SIG_OK) && (!signal_detect)) {
      return BF_NOT_READY;
    }
  }

  // run EQ
  if (step & STEP_EQ_START) {
    rc = bf_aw_pmd_rx_ctle_adapt_set(
        dev_id, dev_port, ln, cfg->rx_ctle_adapt_en, cfg->rx_ctle_adapt_boost);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_tof3_serdes_rx_vga_cap_adapt_set(
        dev_id, dev_port, ln, 1 /*en*/, cfg->rx_vga_cap,
        false /*use_custom_takeover_ratio*/, 0 /*custom_takeover_ratio*/,
        0 /*custom_nyq_mask*/);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_eqeval_type_set(dev_id, dev_port, ln, AW_EQ_FULL_DIR);
    rc = bf_aw_pmd_eqeval_req_set(dev_id, dev_port, ln, 0); // force clear first
    rc = bf_aw_pmd_eqeval_req_set(dev_id, dev_port, ln, 1);
  }

  if ((step == STEP_ALL_ETH) || (step == STEP_ALL_BIST)) {
    bf_sys_usleep(3 * 500 * 1000);
  }

  if (step & STEP_CHECK_EQ_DONE) {
    uint32_t eqeval_ack = 0, incdec = 0;

    rc = bf_aw_pmd_eqeval_ack_get(dev_id, dev_port, ln, &eqeval_ack);

    port_mgr_log("%d %d %d : EQ ack (%d)", dev_id, dev_port, ln, eqeval_ack);

    if (!eqeval_ack) {
      port_mgr_log("%d %d %d : EQ not complete\n", dev_id, dev_port, ln);
      // if just checking this state, indicate not done
      if (step == STEP_CHECK_EQ_DONE) {
        return BF_NOT_READY;
      }
    }
    rc = bf_aw_pmd_eqeval_req_set(dev_id, dev_port, ln, 0);
    rc = bf_aw_pmd_eqeval_incdec_get(dev_id, dev_port, ln, &incdec);

    rc = bf_aw_pmd_vfld_eqbk_hold_req_set(dev_id, dev_port, ln, 0);
    if (rc != BF_SUCCESS)
      return rc;
  }

  if ((step == STEP_ALL_ETH) || (step == STEP_ALL_BIST)) {
    bf_sys_usleep(2 * 1000 * 1000);
  }

  if (step & STEP_CHECK_CDR_LOCK) {
    // check EQ results in vaild signal
    uint32_t pmd_rx_lock = 0;

    rc = bf_aw_pmd_rx_lock_status_get(dev_id, dev_port, ln, &pmd_rx_lock);
    if (rc != BF_SUCCESS)
      return rc;
    if (!pmd_rx_lock) {
      port_mgr_log("%d %d %d : CDR Not Locked\n", dev_id, dev_port, ln);
      // if just checking this state, indicate not done
      if (step == STEP_CHECK_CDR_LOCK) {
        return BF_NOT_READY;
      }
    }
  }

  //
  // Note: This step is ONLY executed by the PRBS FSM
  //
  if (step & STEP_CHECK_BIST_LOCK) {
    rc = bf_aw_pmd_rx_chk_config_set(dev_id, dev_port, ln, cfg->prbs_mode,
                                     BIST_MODE, 0x3333333333333333,
                                     lock_threshold, 0x07ffffff);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_rx_chk_en_set(dev_id, dev_port, ln, cfg->rx_prbs_chk_en);
    if (rc != BF_SUCCESS)
      return rc;

    if (cfg->prbs_gen_tx_en && cfg->rx_prbs_chk_en) {
      rc = bf_aw_pmd_rx_chk_lock_state_get(dev_id, dev_port, ln, &rx_bist_lock);
      if (rc != BF_SUCCESS)
        return rc;

      if (!rx_bist_lock) {
        // if just checking this state, indicate not done
        if (step == STEP_CHECK_BIST_LOCK) {
          return BF_NOT_READY;
        }
      }
    }
  }
  return rc;
}

/*********************************************************************
 *
 * Execute all steps in the port initialization FSM synchronously.
 *
 * This fn is only used by the low level mss_ UCLI
 */
bf_status_t bf_tof3_serdes_lane_init(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, lane_cfg_t *cfg) {
  bf_status_t rc;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                         NULL, NULL);
  if (rc != 0)
    return rc;

  rc = bf_tof3_serdes_lane_init_step(dev_id, dev_port, ln, cfg, STEP_ALL_BIST);
  return rc;
}

/*********************************************************************
 *
 * Power dow a serdes lane to minimize power when not in use.
 *
 * Note:
 * It was noted that after a POR, the serdes are powered down (power
 * state=7), even though the CSRs indicate power-state=0.
 * Further, it was found that attempting to change the power-state
 * while the rate-code is set to "0" (which it defaults to after POR)
 * will cause the FW to hang since there is no rate=0 FW code. So,
 * we actually have to power "up" the serdes to a known rate so we can
 * power it down.
 */
bf_status_t bf_tof3_serdes_power_down(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln) {
  bf_status_t rc = 0;
  uint32_t rate = 0;
  uint32_t width = 0;
  uint32_t rate_gb = 0;
  bool is_pam4 = true;
  uint32_t state_req_ack_polls = 1000;
  uint32_t state_req_ack_delay = 100;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                         NULL, NULL);
  if (rc != 0)
    return rc;

  // squelch ch during reconfig
  rc = bf_tof3_serdes_squelch_set(dev_id, dev_port, ln, 1);

  // clear FEP and NES, if necessary
  // rc = bf_aw_pmd_remote_loopback_set(dev_id, dev_port, ln, 0);
  rc = bf_aw_pmd_analog_loopback_set(dev_id, dev_port, ln, 0);

  // get serdes speed and encoding mode from port speed
  rc = bf_tof3_serdes_port_speed_to_serdes_speed(
      dev_id, dev_port, ln, BF_SPEED_50G, 1, &rate_gb, &is_pam4);
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

  uint32_t cur_rate = 0;
  bf_aw_pmd_iso_tx_rate_get(dev_id, dev_port, ln, &cur_rate);
  if (cur_rate == 0) {
    rc = bf_aw_pmd_iso_tx_rate_set(dev_id, dev_port, ln, rate);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_rx_rate_set(dev_id, dev_port, ln, rate);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_tx_width_set(dev_id, dev_port, ln, width);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_rx_width_set(dev_id, dev_port, ln, width);
    if (rc != BF_SUCCESS)
      return rc;

    // hack to work-around fW issue. They cannot handle a rate-chg in
    // pstate=4, so, power-UP first, to pstate=2, then power-dn w/out
    // changing the rate.
    rc = bf_aw_pmd_iso_tx_pstate_set(dev_id, dev_port, ln, 2);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_aw_pmd_iso_rx_pstate_set(dev_id, dev_port, ln, 2);
    if (rc != BF_SUCCESS)
      return rc;

    rc = bf_tof3_serdes_tx_state_req_set_synchronouos(
        dev_id, dev_port, ln, state_req_ack_polls, state_req_ack_delay);

    rc = bf_tof3_serdes_rx_state_req_set_synchronous(
        dev_id, dev_port, ln, state_req_ack_polls, state_req_ack_delay);
  }

  // now, power-dn
  rc = bf_aw_pmd_iso_tx_pstate_set(dev_id, dev_port, ln, 4);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_aw_pmd_iso_rx_pstate_set(dev_id, dev_port, ln, 4);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_tof3_serdes_tx_state_req_set_synchronouos(
      dev_id, dev_port, ln, state_req_ack_polls, state_req_ack_delay);

  rc = bf_tof3_serdes_rx_state_req_set_synchronous(
      dev_id, dev_port, ln, state_req_ack_polls, state_req_ack_delay);
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_rx_eq
 *
 * Note: used only by the UCLI
 */
bf_status_t bf_tof3_serdes_rx_eq(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, uint32_t eq_type, uint32_t tout) {
  bf_status_t rc = 0;

  rc = bf_aw_pmd_rx_equalize(dev_id, dev_port, ln, eq_type, tout);
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_cdr_lock_get
 *
 */
bf_status_t bf_tof3_serdes_cdr_lock_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *cdr_lock) {
  bf_status_t rc = 0;

  rc = bf_aw_pmd_rx_lock_status_get(dev_id, dev_port, ln, cdr_lock);
  return rc;
}

/*****************************************************************************
 * bf_tof3_serdes_cleanup
 *
 * This fn is used to clean-up a lane and prepare it for an FSM retry. We
 * do NOT power the lane down since it is still enabled.
 *
 * Note: This function has some code setting "isolation" mode. It was
 * initially thought we needed to operate in both isolated mode and non-
 * isolated mode. Later it was determined we could operate in a mode where
 * only certain functions were "isolated", one of them being the lane resets.
 *
 * The AW serdes have a notion of "isolation mode", where all behavior is
 * determined by the CSRs. This was intended for their eval board, where there
 * was no higher level logic. In non-isolated mode, certain functions are driven
 * by "pins" in higher-level logic (our serdes "glue").
 * However, due to some late design changes on Intels side these "pins" were
 * removed from the 16ln IP. This made the 16ln and 4ln IP different. To keep
 * the same behavior as much as possible, we run most common functions in
 * isolation mode so they are controlled directly by the CSRs.
 */
bf_status_t bf_tof3_serdes_cleanup(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln) {
  bf_status_t rc = 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                         NULL, NULL);
  if (rc != 0)
    return rc;

  port_mgr_log("%d:%3d:%d : Serdes cleanup ", dev_id, dev_port, ln);

  // according to AW, this vfield should be left "0" for both ANLT and non-
  // If set, we see non-ANLT link-up failing even when using Tx taps identified
  // by LT.
  //
  // rc = bf_aw_pmd_rxmfsm_eq_check_rxdisable_set(dev_id, dev_port, ln, 1);
  // if (rc != BF_SUCCESS) return rc;

  bf_aw_pmd_anlt_logical_lane_num_set(dev_id, dev_port, ln, 0,
                                      1 /*an_no_attached*/);
  bf_aw_pmd_anlt_auto_neg_start_set(dev_id, dev_port, ln, 0);
  bf_aw_pmd_isolate_lane_set(dev_id, dev_port, ln, 1);

  // clear the iso reset bits before isolating again
  bf_tof3_serdes_lane_unreset_set(dev_id, dev_port, ln);

  // enter iso mode now that the resets are asserted
  bf_aw_pmd_isolate_lane_set(dev_id, dev_port, ln, 1);

  // reset tx/rx
  rc = bf_tof3_serdes_lane_reset_set(dev_id, dev_port, ln);
  if (rc != BF_SUCCESS)
    return rc;

  // test, keep iso bits and pins in same state
  bf_tof3_serdes_glue_lane_tx_reset_set(dev_id, dev_port, ln, 1);
  bf_tof3_serdes_glue_lane_rx_reset_set(dev_id, dev_port, ln, 1);

  return 0;
}

/*****************************************************************************
 * bf_tof3_serdes_disable
 *
 * Fully disable a lane. Squelch any Tx output, reset it, then power-dn
 *
 */
bf_status_t bf_tof3_serdes_disable(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln) {
  bf_status_t rc = 0;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                         NULL, NULL);
  if (rc != 0)
    return rc;

  port_mgr_log("%d:%3d:%d : Serdes disable", dev_id, dev_port, ln);

  // according to AW, this vfield should be left "0" for both ANLT and non-
  // If set, we see non-ANLT link-up failing even when using Tx taps identified
  // by LT.
  //
  // rc = bf_aw_pmd_rxmfsm_eq_check_rxdisable_set(dev_id, dev_port, ln, 1);
  // if (rc != BF_SUCCESS) return rc;

  // clear the iso reset bits before isolating again
  bf_tof3_serdes_lane_unreset_set(dev_id, dev_port, ln);

  // enter iso mode now that the resets are asserted
  bf_aw_pmd_isolate_lane_set(dev_id, dev_port, ln, 1);

  // set power state(4), lowest power
  rc = bf_tof3_serdes_power_down(dev_id, dev_port, ln);
  // if (rc != BF_SUCCESS) return rc;

  // reset tx/rx
  rc = bf_tof3_serdes_lane_reset_set(dev_id, dev_port, ln);
  if (rc != BF_SUCCESS)
    return rc;

  // test, keep iso bits and pins in same state
  bf_tof3_serdes_glue_lane_tx_reset_set(dev_id, dev_port, ln, 1);
  bf_tof3_serdes_glue_lane_rx_reset_set(dev_id, dev_port, ln, 1);

  return 0;
}

/********************************************************************
 *
 * Table of all speeds supported by FW, in either 16ln or 4ln.
 * speeds not supported by a given lane will be skipped.
 */
typedef struct rate_cfg_t {
  char *name;
  uint32_t serdes_speed;
  uint32_t is_pam4;
} rate_cfg_t;

rate_cfg_t rate_cfg_tbl[] = {
    // speed             serdes_speed graycode_en
    {"106.25g  (PAM4)", 100, 1}, {"53.125g  (PAM4)", 50, 1},
    {"25.78125g (NRZ)", 25, 0},  {"10.3125g  (NRZ)", 10, 0},
    {"1.25g     (NRZ)", 1, 0},   {"2.5g      (NRZ)", 2, 0},
    {"5g        (NRZ)", 5, 0},   {"8g        (NRZ)", 8, 0},
    {"16g       (NRZ)", 16, 0},
};

/********************************************************************
 * config_lane
 *
 * This function can be used by the ucli to program serdes modes.
 *
 * It executes the steps of the FSM synchronously, so will run
 * slower than the normal port FSMs
 *
 * It is expected that at folllowing parameters will have been
 * set prior to calling tihs function,
 *   rx termination
 *   tx/rx precoding enables
 *   tx/rx polarities
 */
bf_status_t bf_tof3_serdes_config_lane(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t rate_gb, uint32_t is_pam4,
                                       uint32_t prbs_mode,
                                       uint32_t loopback_mode) {
  bf_status_t rc = 0;
  lane_cfg_t *cfg = NULL;
  uint32_t rate;
  uint32_t width;

  // verify this a port managed by us
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                         NULL, NULL);
  if (rc != 0)
    return rc;
  ;

  // point to tf3_sd->cfg
  rc = bf_tof3_serdes_lane_config_get(dev_id, dev_port, ln, &cfg);
  if (rc) {
    printf("bf_tof3_serdes_config_lane: cant locate lane_cfg\n");
    return BF_INVALID_ARG;
  }
  rc = bf_tof3_serdes_serdes_speed_to_rate_and_width(
      dev_id, dev_port, ln, rate_gb, is_pam4 ? true : false, &rate, &width);
  if (rc != 0) {
    printf("Invalid rate for IP : serdes_speed=%d\n", rate_gb);
    return BF_INVALID_ARG;
  }
  cfg->tx_rate = rate;
  cfg->rx_rate = rate;
  cfg->tx_width = width;
  cfg->rx_width = width;
  cfg->tx_pstate = 0;
  cfg->rx_pstate = 0;

  if (prbs_mode < AW_BIST_PATTERN_MAX) { // PRBS mode
    cfg->prbs_mode = prbs_mode;
    cfg->user_data_pat = 0; // TBD
    cfg->prbs_gen_tx_en = 1;
    cfg->rx_prbs_chk_en = 1;
  } else {
    cfg->prbs_mode =
        AW_PRBS7; // shouldn't matter since enables are 0. AW_BIST_PATTERN_MAX;
    cfg->user_data_pat = 0;
    cfg->prbs_gen_tx_en = 0;
    cfg->rx_prbs_chk_en = 0;
  }
  cfg->loopback_mode = loopback_mode;
  cfg->tx_precoder_en_gc = is_pam4 ? 1 : 0;
  cfg->rx_precoder_en_gc = is_pam4 ? 1 : 0;

  rc = bf_tof3_serdes_lane_init(dev_id, dev_port, ln, cfg);
  return rc;
}

/*******************************************************************
 * Step the serdes config FSM
 *
 * This fn executes a single step in the non-ANLT port
 * initialization FSM. It is a utility fn for each individual
 * FSM state.
 */
bf_status_t bf_tof3_serdes_fsm_step(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t step) {
  bf_status_t rc = BF_SUCCESS;
  lane_cfg_t *cfg = NULL;

  // get lane config struct
  rc = bf_tof3_serdes_lane_config_get(dev_id, dev_port, ln, &cfg);
  if (rc) {
    printf("bf_tof3_serdes_config_lane: cant locate lane_cfg\n");
    return BF_INVALID_ARG;
  }
  rc = bf_tof3_serdes_lane_init_step(dev_id, dev_port, ln, cfg, step);
  if (rc == BF_NOT_READY) {
    return BF_NOT_READY;
  }
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG;
  }
  return BF_SUCCESS;
}

/***************************************************************
 * Initiate autonegotiation. This is the first step in the
 * ANLT FSM.
 *
 * Note: We always skip the nonce check so ANLT works on
 * loopback modules.
 *
 * Note: We disable the FW from automatically restarting the AN
 * phase upon AN or LT failure. This is necessary to maintain
 * synchronization with the AW FW. Since SW must manage the
 * next-page exchange, we must know where in the process the
 * FW is, and there is no information available from the FW to
 * identify this. So we have to keep track of received pages,
 * base-page first then each next-page (if necessary).
 *
 */
bf_status_t bf_tof3_serdes_config_ln_autoneg(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint64_t basepage,
                                             uint32_t consortium_np_47_16,
                                             bool is_loop) {
  bf_status_t rc;
  bf_status_t ret;
  uint32_t adv_ability[28];
  uint32_t fec_ability[5];
  uint32_t nonce = 0;
  uint32_t status_check_disable = 1; // no restart on LT/link failure
  uint32_t next_page_en = 0;         // no NP
  uint32_t an_no_nonce_check = 1;    // skip nonce check
  uint32_t an_no_attached;
  uint32_t n_lanes;
  lane_cfg_t *cfg;

  bf_port_num_lanes_get(dev_id, dev_port, (int *)&n_lanes);
  if (n_lanes == 1) { // single-lane phy
    an_no_attached = 1;
  } else {
    an_no_attached = 0;
  }
  if (consortium_np_47_16 != 0) {
    next_page_en = 1;
  }
  // copy speed adverts from the base_pg
  for (int i = 0; i <= 18; i++) {
    adv_ability[i] = (basepage >> (21ul + i)) & 1;
  }
  // must transcribe consortium bits
  if ((consortium_np_47_16 >> 4) & 1) {
    adv_ability[23] = 1; // 25g-kr1
  }
  if ((consortium_np_47_16 >> 5) & 1) {
    adv_ability[24] = 1; // 25g-cr1
  }
  if ((consortium_np_47_16 >> 8) & 1) {
    adv_ability[25] = 1; // 25g-kr2
  }
  if ((consortium_np_47_16 >> 9) & 1) {
    adv_ability[26] = 1; // 25g-cr2
  }
  if ((consortium_np_47_16 >> 18) & 1) {
    adv_ability[27] = 1; // 400g-cr8
  }
  // copy FEC adverts from the base_pg
  // note: AW def seems to go beyond 48b
  for (int i = 0; i < 5; i++) {
    fec_ability[i] = (basepage >> (44ul + i)) & 1;
  }
  // place all lanes in reset (both TX and RX sides)
  for (uint32_t __ln = 0; __ln < n_lanes; __ln++) {
    bf_tof3_serdes_lane_reset_set(dev_id, dev_port, (ln + __ln));
  }

  rc = bf_tof3_serdes_lane_config_get(dev_id, dev_port, ln, &cfg);
  if (rc) {
    printf("bf_tof3_serdes_config_lane: cant locate lane_cfg\n");
    return BF_INVALID_ARG;
  }

  for (uint32_t __ln = 0; __ln < n_lanes; __ln++) {
    rc = bf_aw_pmd_anlt_logical_lane_num_set(dev_id, dev_port, (ln + __ln),
                                             __ln, an_no_attached);
    //
    // set map_en for all included lanes
    //
    // note: This should ALWAYS be enabled. We set it at SDE start-up
    // and never deassert it. This is required due to the asymmetric lane
    // map used on our boards.
    //
    rc = bf_aw_pmd_ctrl_map_en_set(dev_id, dev_port, (ln + __ln), 1);
  }
  rc = bf_aw_pmd_anlt_auto_neg_adv_ability_set(dev_id, dev_port, ln,
                                               adv_ability, fec_ability, nonce);
  rc = bf_aw_pmd_anlt_auto_neg_config_set(dev_id, dev_port, ln,
                                          status_check_disable, next_page_en,
                                          an_no_nonce_check);

  for (uint32_t __ln = 0; __ln < n_lanes; __ln++) {
    //
    // disable grey-code en and precode en
    //
    // Note: This is required for ANLT FW to function properly
    //
    bf_aw_pmd_tx_pam4_precoder_override_set(dev_id, dev_port, ln + __ln, 0);
    bf_aw_pmd_tx_pam4_precoder_enable_set(dev_id, dev_port, ln + __ln, 0, 0);
    bf_aw_pmd_rx_pam4_precoder_override_set(dev_id, dev_port, ln + __ln, 0);
    bf_aw_pmd_rx_pam4_precoder_enable_set(dev_id, dev_port, ln + __ln, 0, 0);
    bf_aw_pmd_rx_background_adapt_enable_set(dev_id, dev_port, ln + __ln, 1);

    // according to AW, this vfield should be left "0" for both ANLT and non-
    // If set, we see non-ANLT link-up failing even when using Tx taps
    // identified by LT.
    //
    // bf_aw_pmd_rxmfsm_eq_check_rxdisable_set(dev_id, dev_port, ln + __ln, 0);

    //
    // clear TX FIR OVRD_EN
    //
    // Note: THis is required for LT to be able to set the TX taps
    bf_tof3_serdes_txfir_hw_ovrd_set(dev_id, dev_port, ln + __ln, 0);

    bf_aw_pmd_rx_ctle_adapt_set(dev_id, dev_port, ln + __ln, 1, 12);
    bf_tof3_serdes_rx_vga_cap_adapt_set(
        dev_id, dev_port, ln + __ln, 1 /*en*/, cfg->rx_vga_cap,
        false /*use_custom_takeover_ratio*/, 0 /*custom_takeover_ratio*/,
        0 /*custom_nyq_mask*/);

    // per-AW, should not need to force signal detect here
    // aw_force_sigdet_mode_t sigdet_mode = AW_SIGDET_FORCE1;
    // bf_aw_pmd_force_signal_detect_config_set(dev_id, dev_port, ln + __ln,
    // sigdet_mode);

    // new vfield
    bf_aw_pmd_vfld_pick_c162_set(dev_id, dev_port, ln + __ln, 1);

    bf_aw_pmd_rxeq_prbs_set(dev_id, dev_port, ln + __ln, 0);
    bf_aw_pmd_vfld_fg_done_set(dev_id, dev_port, ln + __ln, 0);
    bf_aw_pmd_vfld_linkeval_state_set(dev_id, dev_port, ln + __ln, 0);
    // clear ena first
    bf_aw_pmd_anlt_link_training_start_set(dev_id, dev_port, ln + __ln, 0);

    // per-AW
    // DFE should be enabled for PAM4 modes but disabled for NRZ since it
    // may extend the LT time beyond the IEEE limit
    //
    // we haven't seen the LT time exceeded yet. For now just leave enabled
    //
    bf_aw_pmd_rx_dfe_adapt_set(dev_id, dev_port, ln + __ln,
                               1 /*dfe_adapt_enable*/);
  }
  // We depend on out own FSM timeout (see note in fn comment)
  for (uint32_t __ln = 0; __ln < n_lanes; __ln++) {
    bf_aw_pmd_anlt_link_training_timeout_enable_set(dev_id, dev_port, ln + __ln,
                                                    0);
  }
  // De-assert reset on all lanes at one time
  for (uint32_t __ln = 0; __ln < n_lanes; __ln++) {
    bf_tof3_serdes_lane_unreset_set(dev_id, dev_port, (ln + __ln));
  }

  // Enable ANLT on all lanes but make sure to enable on
  // ln0 last, as that actually kicks-off AN. If it were set
  // before some other lane(s) AN might complete before we had set
  // AN start on one or more of the other lanes.
  //
  // Note: This is REQUIRED to ensure all dependent lanes have
  // had their AN-start bit set before lane 0 actually initiates
  // ANLT.
  //
  // start on lanes 1-n
  for (uint32_t __ln = 1; __ln < n_lanes; __ln++) {
    rc = bf_aw_pmd_anlt_auto_neg_start_set(dev_id, dev_port, (ln + __ln), 1);
    if(rc != BF_SUCCESS)
    {
      ret = BF_INVALID_ARG;
    }
  }

  // lastly, start on ln0
  rc = bf_aw_pmd_anlt_auto_neg_start_set(dev_id, dev_port, (ln + 0), 1);
  if(rc != BF_SUCCESS)
  {
    ret = BF_INVALID_ARG;
  }

  return ret;
}

/***************************************************************
 * Return HCD and LP base-pg
 *
 * Should only be called after verifying an_done
 */
bf_status_t bf_tof3_serdes_an_result_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t *an_mr_page_rx,
                                         uint32_t *an_result,
                                         uint64_t *an_rx_link_code_word) {
  bf_status_t rc;

  bf_aw_pmd_anlt_auto_neg_page_rx_get(dev_id, dev_port, 0, an_mr_page_rx,
                                      an_rx_link_code_word);
  rc = bf_aw_pmd_anlt_auto_neg_result_get(dev_id, dev_port, 0, an_result);
  return rc;
}

/***************************************************************
 *
 * Return an_done indication. This only signals completion of
 * the AN portion of ANLT. "an_done=1" indicates the HCD
 * technology has been determined and the FW has moved to the
 * link-training phase, which it does automatically.
 */
bf_status_t bf_tof3_serdes_an_done_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       bool *an_done) {
  bf_status_t rc = BF_NOT_READY;
  uint32_t link_good = 0;

  *an_done = false;
  bf_aw_pmd_anlt_auto_neg_status_get(dev_id, dev_port, ln, &link_good);
  if (!link_good) {
    return BF_NOT_READY;
  }

  if (link_good) {
    uint32_t an_result = 0;

    *an_done = true;

    rc = bf_aw_pmd_anlt_auto_neg_result_get(dev_id, dev_port, ln, &an_result);
    port_mgr_log("%d:%3d:%d : AN HCD : %08x", dev_id, dev_port, ln, an_result);
  }
  return rc;
}

/***************************************************************
 *
 * Return whether link-training has completed successfully.
 * If LT is still in progress or has failed the this fn will
 * return link_training_done=0. Ths caller (possibly the ANLT
 * FSM) can decide what, if anything, to do about this state.
 */
bf_status_t bf_tof3_serdes_lt_done_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       bool *link_training_done) {
  bf_status_t rc;
  uint32_t lt_running;
  uint32_t lt_done;
  uint32_t lt_training_failure;
  uint32_t lt_rx_ready;

  *link_training_done = false;

  rc = bf_aw_pmd_anlt_link_training_status_get(
      dev_id, dev_port, ln, &lt_running, &lt_done, &lt_training_failure,
      &lt_rx_ready);
  if (rc == BF_SUCCESS) {
    if (!lt_done) {
      rc = BF_NOT_READY;
    } else {
      *link_training_done = true;
    }
  }
  return rc;
}

/***************************************************************
 *
 * Return detailed link-training state information.
 *
 * This is used by the UCLI to display ANLT state info.
 */
bf_status_t bf_tof3_serdes_anlt_link_training_status_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *lt_running, uint32_t *lt_done, uint32_t *lt_training_failure,
    uint32_t *lt_rx_ready) {

  bf_status_t rc;

  rc = bf_aw_pmd_anlt_link_training_status_get(dev_id, dev_port, ln, lt_running,
                                               lt_done, lt_training_failure,
                                               lt_rx_ready);

  return rc;
}

/***************************************************************
 *
 * Return (more) detailed link-training state information.
 *
 * This is used by the UCLI to display ANLT state info.
 * The "lt_fsm_st" refers to the AW FW state-machine, not our
 * ANLT FSM.
 *
 * From AW:
 *
 * The eth_anlt_state_reg2[lt_state_ctrl_fsm] register gives
 * the state of the main LT control FSM, with the following states:
 *
 * State      Encoding  Description
 * INIT	         0      Idle state. LT is not running
 * SEND_TF_PMA_1 1      FSM is in SEND_TF state. FSM is
 *                      requesting RX/TX change to the
 *                      appropriate state (basically enabling
 *                      gray coding/disabling precoding for PAM)
 *                      to start LT, and waiting for ACK signals
 *                      to be high. NOTE: Timeout timer is
 *                      started in this state for NRZ.
 * SEND_TF_PMA_2 2      FSM has completed those steps in
 *                      SEND_TF_PMA_1 and is waiting for
 *                      related ACK signals to be low.
 * RXEQ_PMA_1    3      FSM is in SEND_TF state, and is
 *                      instructing RXEQ to perform the INIT
 *                      procedure, and is waiting for ACK to
 *                      be high.
 * RXEQ_PMA_2    4      FSM has completed those steps in RXEQ_PMA_1
 *                      and is waiting for related ACK signals
 *                      to be low.
 * SEND_TF       5      Matches IEEE; waiting for local frame lock.
 *                      PAM also waits for remote frame lock.
 * TRAIN_LOCAL   6      Matches IEEE; Local training is being performed
 *                      (via local training FSM). FSM waits for local
 *                      training FSM to indicate done, then requests
 *                      final modulation and waits for acknowledge if
 *                      in PAM4. NOTE: Timeout timer is started in this
 *                      state for PAM.
 * TRAIN_REMOTE  7      Matches IEEE; Local training has been completed,
 *                      and FSM is waiting for far-end to indicate it
 *                      has completed its training.
 * LINK_READY    8      Matches IEEE; Training is complete, FSM is still
 *                      sending frames, waiting for ready timer to expire.
 * TIMEOUT       9      Matches IEEE; training has failed. FSM is waiting
 *                      for holdoff timer to expire.
 * FAILURE      10      Matches IEEE; training has failed, and LT FSM
 *                      will remain in this state forever unless
 *                      restarted (manually or via AN).
 * SEND_DATA_PMA_1  11  FSM is in SEND_DATA state, and is requesting RX/TX
 *                      to change to the appropriate state (effectively
 *                      enabling/disabling precoding for PAM), and waiting
 *                      for ACK signals to be high.
 * SEND_DATA_PMA_2 12   FSM has completed those steps in SEND_DATA_PMA_1
 *                      and is waiting for related ACK signals to be low.
 * SEND_DATA       13   LT has successfully completed, and data from the
 *                      PCS has been enabled.
 * SEND_DATA_INIT  14   Only used if training disable by setting
 *                      lt_mr_training_enable to 0. Initializes TX FIR
 *                      coefficients to their initial values, depending
 *                      on clause.
 * QUIET           15   Not used
 */
bf_status_t bf_tof3_serdes_lt_info_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *lt_fsm_st,
                                       uint32_t *frame_lock) {
  bf_status_t rc;

  rc = bf_aw_pmd_lt_info_get(dev_id, dev_port, ln, lt_fsm_st, frame_lock);
  return rc;
}

/** \brief Configure a serdes lane in non-ANLT mode, based
 *         on the current port speed and n_lanes configured.
 *         This function executes the first step of the non-ANLT
 *         FSM and initiates the port bring-up sequence.
 *         The first step isolates and resets the lanes then
 *         configures rate/width/pstate.
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 * \param[in]  speed      : 400, 200, 100, ...
 * \param[in]  n_lanes    : w/speed, distinguishes 25g/50g/a00g serdes speed
 * \param[in]  tx_pat     : NONE=mission mode
 * \param[in]  rx_pat     : NONE=mission mode
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof3_serdes_config_ln(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, bf_port_speed_t speed,
                                     uint32_t num_lanes,
                                     bf_port_prbs_mode_t tx_pat,
                                     bf_port_prbs_mode_t rx_pat) {
  bf_status_t rc = BF_SUCCESS;
  lane_cfg_t *cfg = NULL;
  uint32_t rate = 0;
  uint32_t width = 0;
  uint32_t rate_gb = 0;
  bool is_pam4 = true;

  // clear some AN-specific config since not running AN
  rc = bf_aw_pmd_anlt_logical_lane_num_set(dev_id, dev_port, ln, 0,
                                           1 /*an_no_attached*/);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG; // shouldnt happen
  }
  rc = bf_aw_pmd_anlt_auto_neg_start_set(dev_id, dev_port, ln, 0);
  if (rc != BF_SUCCESS) {
    return BF_INVALID_ARG; // shouldnt happen
  }
  //
  // check for disable, BF_SPEED_NONE
  //
  // Note: This is a "special" speed used to indicate the port is being
  // disabled (or un-configured).
  //
  if (speed == BF_SPEED_NONE) {
    bf_tof3_serdes_disable(dev_id, dev_port, ln);
    return BF_SUCCESS;
  }

  // get lane config struct
  rc = bf_tof3_serdes_lane_config_get(dev_id, dev_port, ln, &cfg);
  if (rc) {
    printf("bf_tof3_serdes_config_lane: cant locate lane_cfg\n");
    return BF_INVALID_ARG;
  }

  // get serdes speed and encoding mode from port speed
  rc = bf_tof3_serdes_port_speed_to_serdes_speed(dev_id, dev_port, ln, speed,
                                                 num_lanes, &rate_gb, &is_pam4);
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

  cfg->tx_rate = rate;
  cfg->rx_rate = rate;
  cfg->tx_width = width;
  cfg->rx_width = width;
  cfg->tx_pstate = 0;
  cfg->rx_pstate = 0;

  if (tx_pat != rx_pat) {
    return BF_INVALID_ARG;
  }
  if (tx_pat == BF_PORT_PRBS_MODE_NONE) { // mission mode
    cfg->prbs_mode = AW_PRBS31;           // doesn't matter in mission mode
    cfg->user_data_pat = 0;
    cfg->prbs_gen_tx_en = 0;
    cfg->rx_prbs_chk_en = 0;
  } else {
    aw_bist_pattern_t aw_pat;

    // convert to AW PRBS pattern
    rc = bf_tof3_bf_to_aw_prbs_pat(tx_pat, &aw_pat);
    if (rc != BF_SUCCESS) {
      return BF_INVALID_ARG; // unsupported PRBS moode
    }
    // set PRBS config
    cfg->prbs_mode = aw_pat;
    cfg->user_data_pat = 0;
    cfg->prbs_gen_tx_en = 1;
    cfg->rx_prbs_chk_en = 1;
  }
  //
  // Use the _step fn to intiate the FSM
  //
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_CFG_RATE);
}

/***************************************************************************
 * FSM step: Apply Tx rate/width/power-state change
 ***************************************************************************/
bf_status_t bf_tof3_serdes_apply_tx_rate(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_APPLY_TX_RATE);
}

/***************************************************************************
 * FSM step: Wait for Tx rate/width/power-state change done
 ***************************************************************************/
bf_status_t bf_tof3_serdes_wait_tx_rate_change_done(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln,
                                 STEP_WAIT_TX_RATE_CHG_DONE);
}

/***************************************************************************
 * FSM step: Tx rate/width/power-state change done, clear request
 ***************************************************************************/
bf_status_t bf_tof3_serdes_tx_rate_change_done(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_TX_RATE_CHG_DONE);
}

/***************************************************************************
 * FSM step: Apply Rx rate/width/power-state change
 ***************************************************************************/
bf_status_t bf_tof3_serdes_apply_rx_rate(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_APPLY_RX_RATE);
}

/***************************************************************************
 * FSM step: Wait for Rx rate/width/power-state change done
 ***************************************************************************/
bf_status_t bf_tof3_serdes_wait_rx_rate_change_done(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln,
                                 STEP_WAIT_RX_RATE_CHG_DONE);
}

/***************************************************************************
 * FSM step: Rx rate/width/power-state change done, clear request
 ***************************************************************************/
bf_status_t bf_tof3_serdes_rx_rate_change_done(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_RX_RATE_CHG_DONE);
}

/***************************************************************************
 * FSM step: Configue lane mode (attributes)
 ***************************************************************************/
bf_status_t bf_tof3_serdes_config_mode(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_CFG_MODE);
}

/***************************************************************************
 * FSM step: Wait for Rx signal detect
 ***************************************************************************/
bf_status_t bf_tof3_serdes_wait_sig_ok(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_WAIT_SIG_OK);
}

/***************************************************************************
 * FSM step: Start Rx Equalization
 ***************************************************************************/
bf_status_t bf_tof3_serdes_eq_start(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_EQ_START);
}

/***************************************************************************
 * FSM step: Check if Rx Equalization is done
 ***************************************************************************/
bf_status_t bf_tof3_serdes_check_eq_done(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_CHECK_EQ_DONE);
}

/***************************************************************************
 * FSM step: check CDR lock
 ***************************************************************************/
bf_status_t bf_tof3_serdes_check_cdr_lock(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_CHECK_CDR_LOCK);
}

/***************************************************************************
 * FSM step: check BIST lock
 *
 * Note: not currently used.
 ***************************************************************************/
bf_status_t bf_tof3_serdes_check_bist_lock(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln) {
  return bf_tof3_serdes_fsm_step(dev_id, dev_port, ln, STEP_CHECK_BIST_LOCK);
}

/***************************************************************************
 * Return the number of serdes lanes on a given MAC channel
 *
 * TF3 supports both QSFP DD and OSFP connectors.
 *
 * QSFP DD connectors support 8 lanes of up to 53g serdes. The lanes are
 * numbered 0-7 and map directly to the 8 lanes of one MAC.
 *
 * OSFP connectors support 8 lanes of up to 106g serdes. The lanes are
 * numbered 0-7, but are spread across 2 different, sequentially numbered
 * MACs. The lower numbered MAC supporting lanes 0-3 and the higher numbered
 * MAC supporting lanes 4-7.
 *
 * On ports supporting only 53g serdes, there are 8 lanes per MAC, or 2 lanes
 * per MAC channel.
 *
 * On ports supporting 106g serdes, there are 4 lanes per MAC, or only 1
 * lane per channel.
 ***************************************************************************/
int bf_tof3_serdes_num_lanes_per_ch(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port) {
  int num_lanes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);

  if ((dev_port >= 1000) && (dev_port <= 1007)) {
    // PCIE PHY "fake" dev_ports
    return 1;
  } else if ((dev_port >= lld_get_min_cpu_port(dev_id)) &&
             (dev_port <= lld_get_max_cpu_port(dev_id))) {
    return 2;
  } else if (num_lanes_per_mac == 8) {
    return 2;
  } else {
    return 1;
  }
}
/***************************************************************************
 *
 *     init cmn:
 *       sw init (using default lane map)
 *       reset
 *       load FW
 *       any other required one-time init
 ***************************************************************************/
bf_status_t bf_tof3_serdes_cmn_init(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    bool is_4ln) {
  bf_status_t rc;
  char *fw_path;
  uint32_t num_lanes;

  fw_path = port_mgr_tof3_fw_get(dev_id, is_4ln);

  rc = bf_tof3_serdes_mss_reset(dev_id, dev_port);
  if (rc != BF_SUCCESS)
    return rc;

  // Init_phy
  rc = bf_aw_pmd_isolate_cmn_set(dev_id, dev_port, 0, 1);
  if (rc != BF_SUCCESS)
    return rc;

  // foreach lane
  if (is_4ln) {
    num_lanes = (num_lanes_per_macro > 4) ? 4 : num_lanes_per_macro;
  } else {
    num_lanes = num_lanes_per_macro;
  }
  for (uint32_t dp = 0; dp < num_lanes; dp += 2) {
    // verify this a port managed by us
    rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port + dp, NULL, NULL,
                                           NULL, NULL, NULL);
    if (rc != 0)
      continue;

    int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
    for (int ln = 0; ln < num_ln; ln++) {
      rc = bf_aw_pmd_isolate_lane_set(dev_id, dev_port + dp, ln, 1);
      if (rc != BF_SUCCESS)
        return rc;
    }
  }

  rc = bf_tof3_serdes_fw_load(dev_id, dev_port, fw_path);
  if (rc != BF_SUCCESS)
    return rc;

  /*
   * Perform all necessary one-time programming of the serdes.
   *
   * Note: This MUST be done after the FW load as some FW versions
   *       overwrite certain registers.
   *
   */
  for (uint32_t dp = 0; dp < num_lanes; dp += 2) {
    // verify this a port managed by us
    rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port + dp, NULL, NULL,
                                           NULL, NULL, NULL);
    if (rc != 0)
      continue;

    int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
    for (int ln = 0; ln < num_ln; ln++) {
      // perform the per-lane one-time programming
      bf_aw_pmd_one_time_pgm(dev_id, dev_port + dp, ln);
    }
  }

  // power up the cmn lane
  rc = bf_aw_pmd_iso_cmn_pstate_set(dev_id, dev_port, 0, 0);
  if (rc != BF_SUCCESS)
    return rc;

  // cmn_state_req_set
  uint32_t max_polls = 500;
  uint32_t ack_delay_us = 1;
  rc = bf_tof3_serdes_cmn_state_req_set(dev_id, dev_port, 0, max_polls,
                                        ack_delay_us);
  if (rc != BF_SUCCESS)
    return rc;

  // verify PLL is ready
  uint32_t pll_lock = 1;
  uint32_t check_en = 1, expected_val = 0;

  rc = bf_aw_pmd_pll_lock_get(dev_id, dev_port, 0, &pll_lock, check_en,
                              expected_val);
  if ((rc != BF_SUCCESS) || (pll_lock != 0)) {
    printf("PLL %s\n", (pll_lock == 0) ? "Locked" : "Not Locked");
  }
  return rc;
}

/* Tof3 chip-level serdes init
 *
 * Init chip:
 *   foreach macro:
 *     init cmn:
 *       sw init (using default lane map)
 *       reset
 *       load FW
 *       any other required one-time init
 *     foreach ln:
 *       init ln:
 *         squelch tx output (if needed)
 *         any necessary one-time inits (calibrations, etc)
 *         place in lowest power state
 *
 *   sometime later, lane map will be updated via platform code
 *   sw init map using real board map
 */
bf_status_t bf_tof3_serdes_top_init(bf_dev_id_t dev_id, uint32_t run_post) {
  bf_dev_port_t dev_port;
  bf_status_t rc = BF_SUCCESS;
  uint32_t pipe;
  uint32_t port;
  uint32_t num_lanes_per_pipe = (num_macros_per_pipe * num_lanes_per_macro);

  memset((char *)&macro_init_status, 0xff, sizeof(macro_init_status));
  memset((char *)&lane_init_status, 0xff, sizeof(lane_init_status));

  rc = lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  if (rc != 0)
    return BF_INVALID_ARG;

  // map PCIe phy first (hardcoded)
  // we use fake dev_port values (1000-1007)
  // for (int pcie_ln = 0; pcie_ln < 4; pcie_ln++) {
  for (int pcie_ln = 0; pcie_ln < 8; pcie_ln++) {
    printf("Set up addressing for PCIe phy ln%d\n", pcie_ln);
    bf_tof3_serdes_lane_config_set(dev_id, 1000, pcie_ln, &default_lane_cfg);
    bf_tof3_serdes_lane_install(dev_id, 1000 + pcie_ln, 0, 0, 0, true);
  }

  // then cpu port
  for (pipe = 0; pipe < 1; pipe++) {
    for (port = 2; port < 2 + 4; port += 2) {
      uint32_t tx_ln, rx_ln, ch;

      dev_port = MAKE_DEV_PORT(pipe, port);

      // verify this a port managed by us
      rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                             &ch, NULL);
      if (rc != 0)
        continue;

      int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
      for (int ln = 0; ln < num_ln; ln++) {

        // set default config
        rc = bf_tof3_serdes_lane_config_set(dev_id, dev_port, ln,
                                            &default_lane_cfg);
        if (rc != BF_SUCCESS) {
          return BF_INVALID_ARG;
        }
        // retrieve mapping from tmac structure
        // uint32_t phys_tx_ln[8];
        // uint32_t phys_rx_ln[8];
        port_mgr_tmac_t *mac_p;

        mac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
        if (mac_p == NULL)
          return BF_INVALID_ARG;
        tx_ln = mac_p->phys_tx_ln[num_ln * ch + ln];
        rx_ln = mac_p->phys_rx_ln[num_ln * ch + ln];

        printf("MAC0 : TX= %d%d%d%d%d%d%d%d : RX= %d%d%d%d%d%d%d%d : ch=%d : "
               "ln=%d\n",
               mac_p->phys_tx_ln[7], mac_p->phys_tx_ln[6], mac_p->phys_tx_ln[5],
               mac_p->phys_tx_ln[4], mac_p->phys_tx_ln[3], mac_p->phys_tx_ln[2],
               mac_p->phys_tx_ln[1], mac_p->phys_tx_ln[0], mac_p->phys_rx_ln[7],
               mac_p->phys_rx_ln[6], mac_p->phys_rx_ln[5], mac_p->phys_rx_ln[4],
               mac_p->phys_rx_ln[3], mac_p->phys_rx_ln[2], mac_p->phys_rx_ln[1],
               mac_p->phys_rx_ln[0], ch, ln);
#if 0
        //map isnt correct yet
        if (port == 2 && ln == 0) {
          tx_ln = 2; 
          rx_ln = 2;
        } else if (port == 2 && ln == 1) {
          tx_ln = 3; 
          rx_ln = 3;
        } else if (port == 4 && ln == 0) {
          tx_ln = 1; 
          rx_ln = 1;
        } else if (port == 4 && ln == 1) {
          tx_ln = 0; 
          rx_ln = 0;
        }
#endif
        // install correct driver for this lane
        rc = bf_tof3_serdes_lane_install(dev_id, dev_port, ln, tx_ln, rx_ln,
                                         true);
        if (rc != BF_SUCCESS) {
          return BF_INVALID_ARG;
        }
        // init the serdes_glue mux
        bf_tof3_serdes_anlt_mux_set(dev_id, dev_port, ln, 1);
      }
    }
  }

  for (pipe = 0; pipe < num_pipes; pipe++) {
    for (port = 8; port < 8 + num_lanes_per_pipe; port++) {
      uint32_t tx_ln, rx_ln, ch;

      dev_port = MAKE_DEV_PORT(pipe, port);

      // verify this a port managed by us
      rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                             &ch, NULL);
      if (rc != 0)
        continue;

      int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
      for (int ln = 0; ln < num_ln; ln++) {
        /* reset the serdes_glue regs BEFORE cmn_init, to ensure any
         * pins are set properly when the AW serdes are released from
         * reset (in cmn_init) */
        if (((dev_port + ln) % 8) == 0) {
          bf_tof3_serdes_glue_reinit(dev_id, dev_port, 0, 8 /*num_lanes*/);
        }

        // set default config
        rc = bf_tof3_serdes_lane_config_set(dev_id, dev_port, ln,
                                            &default_lane_cfg);
        if (rc != BF_SUCCESS) {
          return BF_INVALID_ARG;
        }
        // retrieve mapping from tmac structure
        port_mgr_tmac_t *mac_p;

        mac_p = port_mgr_tof3_map_dev_port_lane_to_tmac(dev_id, dev_port);
        if (mac_p == NULL)
          return BF_INVALID_ARG;

        port_mgr_log("tx: %d|%d|%d|%d|%d|%d|%d|%d|", mac_p->phys_tx_ln[7],
                     mac_p->phys_tx_ln[6], mac_p->phys_tx_ln[5],
                     mac_p->phys_tx_ln[4], mac_p->phys_tx_ln[3],
                     mac_p->phys_tx_ln[2], mac_p->phys_tx_ln[1],
                     mac_p->phys_tx_ln[0]);
        port_mgr_log("rx: %d|%d|%d|%d|%d|%d|%d|%d|", mac_p->phys_rx_ln[7],
                     mac_p->phys_rx_ln[6], mac_p->phys_rx_ln[5],
                     mac_p->phys_rx_ln[4], mac_p->phys_rx_ln[3],
                     mac_p->phys_rx_ln[2], mac_p->phys_rx_ln[1],
                     mac_p->phys_rx_ln[0]);
        port_mgr_log("d_p=%3d : ln=%d : ch=%d : num_ln=%d", dev_port, ln, ch,
                     num_ln);

        tx_ln = mac_p->phys_tx_ln[num_ln * ch + ln];
        rx_ln = mac_p->phys_rx_ln[num_ln * ch + ln];
        // install correct driver for this lane
        rc = bf_tof3_serdes_lane_install(dev_id, dev_port, ln, tx_ln, rx_ln,
                                         false);
        if (rc != BF_SUCCESS) {
          return BF_INVALID_ARG;
        }
        // if num_ln=1 we must set the mux for the unused lane to an invalid
        // value
        if (num_ln == 1) {
          bf_tof3_serdes_anlt_mux_invalidate_set(dev_id, dev_port, tx_ln);
        }
      }
    }
  }

  for (pipe = 0; pipe < num_pipes; pipe++) {
    for (port = 8; port < 8 + num_lanes_per_pipe; port++) {
      uint32_t ch;

      dev_port = MAKE_DEV_PORT(pipe, port);

      // verify this a port managed by us
      rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                             &ch, NULL);
      if (rc != 0)
        continue;

      int num_ln = bf_tof3_serdes_num_lanes_per_ch(dev_id, dev_port);
      for (int ln = 0; ln < num_ln; ln++) {
        // init the serdes_glue mux
        bf_tof3_serdes_anlt_mux_set(dev_id, dev_port, ln, 1);
      }
    }
  }
  // init each macro

  // cpu port first
  for (pipe = 0; pipe < 1; pipe++) {
    for (port = 2; port < 2 + 4; port += 8) {
      dev_port = MAKE_DEV_PORT(pipe, port);
      rc = bf_tof3_serdes_cmn_init(dev_id, dev_port, true);
      if (rc == BF_SUCCESS) {
        uint32_t subdev_id;
        uint32_t macro;

        rc = map_dev_port_to_macro(dev_id, dev_port, &subdev_id, &macro);
        macro_init_status[macro] = 0;
      }
    }
  }

  // for this step, do macros 1-32 first, since this is where the
  // misc resets are applied (on first mss_reset)
  for (pipe = 0; pipe < num_pipes; pipe++) {
    for (port = 8; port < 8 + num_lanes_per_pipe; port += 8) {
      dev_port = MAKE_DEV_PORT(pipe, port);
      // verify this a port managed by us
      rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, NULL,
                                             NULL, NULL);
      if (rc != 0)
        continue;

      rc = bf_tof3_serdes_cmn_init(dev_id, dev_port, false);
      if (rc == BF_SUCCESS) {
        uint32_t subdev_id;
        uint32_t macro;

        rc = map_dev_port_to_macro(dev_id, dev_port, &subdev_id, &macro);
        macro_init_status[macro] = 0;
      }
    }
  }
  return 0;
}

/************************************************************
 *
 * Top level serdes initialization fn, called during device-
 * add.
 *************************************************************/
bf_status_t bf_tof3_serdes_init(bf_dev_id_t dev_id, bool skip_post,
                                bool force_fw_dnld, bool skip_power_dn) {
  bf_status_t rc;
  uint32_t run_post = (skip_post == false) ? 1 : 0;
  (void)force_fw_dnld;
  (void)skip_power_dn;

  rc = bf_tof3_serdes_top_init(dev_id, run_post);
  return rc;
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
bf_status_t bf_tof3_serdes_clkobs_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bf_clkobs_pad_t pad,
                                      bf_sds_clkobs_clksel_t clk_src,
                                      int divider) {
  return port_mgr_tof3_serdes_clkobs_set(dev_id, dev_port, pad, clk_src,
                                         divider);
}
