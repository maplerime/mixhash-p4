
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <bf_types/bf_types.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <lld/lld_reg_if.h>
#include "aw_lane_cfg.h"
#include <port_mgr/bf_tof3_serdes_if.h>
#include "port_mgr_tof3_serdes_map.h"
#include "aw_io.h"
#include "aw_mss.h"
extern void bf_sys_usleep(uint32_t);

uint32_t aw_print_accesses = 0;
uint32_t last_bad_access = 0;
uint32_t last_bad_data = 0;

void bad_access(uint32_t addr, uint32_t data) {
  last_bad_access = addr;
  last_bad_data = data;
}

/*****************************************************************************
 *
 * Eval board I/O functions
 *
 *****************************************************************************/

extern void io_evb_write_csr(uint32_t dev_id,
                             uint32_t subdev_id,
                             uint32_t addr,
                             uint32_t wdata,
                             uint32_t phy_offset_used);
extern void io_evb_read_csr(uint32_t dev_id,
                            uint32_t subdev_id,
                            uint32_t addr,
                            uint32_t *rdata,
                            uint32_t phy_offset_used);

/******************************************************
 * Arrays to hold simulated register contents for evb
 * IP. This is so the APIs can accumulate bits in a
 * given reg.
 */
uint32_t sim_evb_regs_initd = 0;
uint32_t sim_evb_regs[4096] = {0};
uint32_t sim_evb_sram[16384] = {0};

void init_sim_evb_regs(void) {
  sim_evb_regs[0x14C / 4] = 0x8000;  // DIG_SOC_CMN_OVRD_EXTRA_RESET_B_OFFSET
  sim_evb_regs[0x158 / 4] = 1;       // DIG_SOC_CMN_STAT_ADDR 0x00000158
  sim_evb_regs[0x1CC / 4] = 0x0018;  // RX_DATABIST_TOP_RDREG1_ADDR 0x020001CC
  sim_evb_regs[0x3024 / 4] =
      0x0004;  // DIG_SOC_LANE_OVRD_REG5_EXTRA_RESET_B_OFFSET
  sim_evb_regs[0x3030 / 4] =
      0xa000;  // DIG_SOC_LANE_STAT_REG1_ADDR, for tx/rx state ack
  sim_evb_regs_initd = 1;
}

void aw_evb_write_csr(uint32_t dev_id,
                      uint32_t subdev_id,
                      uint32_t addr,
                      uint32_t wdata,
                      uint32_t phy_offset_used) {
  io_evb_write_csr(dev_id, subdev_id, addr, wdata, phy_offset_used);
}

void aw_evb_read_csr(uint32_t dev_id,
                     uint32_t subdev_id,
                     uint32_t addr,
                     uint32_t *rdata,
                     uint32_t phy_offset_used) {
  io_evb_read_csr(dev_id, subdev_id, addr, rdata, phy_offset_used);
}

/* stub functions, used to simulate access for lanes not represented on thhe EVB
 */

uint32_t post_reg = 0;
void aw_evb_sim_write_csr(uint32_t dev_id,
                          uint32_t subdev_id,
                          uint32_t addr,
                          uint32_t wdata,
                          uint32_t phy_offset_used) {
  if ((addr & 0xffff) == 0x00000114) {
    post_reg = wdata;
  }
  sim_evb_regs[(addr & 0xffff) / 4] = wdata;
}

void aw_evb_sim_read_csr(uint32_t dev_id,
                         uint32_t subdev_id,
                         uint32_t addr,
                         uint32_t *rdata,
                         uint32_t phy_offset_used) {
  if ((addr & 0xffff) == 0x00000114) {
    *rdata = post_reg;
  } else {
    *rdata = sim_evb_regs[(addr & 0xffff) / 4];  // 0; //0xffffffff;
  }
}

/*****************************************************************************
 *
 * Raptors (4ln) I/O functions
 *
 *****************************************************************************/

/*****************************************************************************
 *
 * Raptors (4ln) I/O functions
 *
 *****************************************************************************/

/******************************************************
 * Arrays to hold simulated register contents for 16ln
 * IP. This is so the APIs can accumulate bits in a
 * given reg.
 */
uint32_t sim_4ln_regs_initd = 0;
uint32_t sim_4ln_regs[4096] = {0};
uint32_t sim_4ln_sram[16384] = {0};

void init_sim_4ln_regs(void) {
  sim_4ln_regs[0x14C / 4] = 0x8000;  // DIG_SOC_CMN_OVRD_EXTRA_RESET_B_OFFSET
  sim_4ln_regs[0x150 / 4] = 1;  // DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET
  sim_4ln_regs[0x1C0 / 4] = 0x0018;  // RX_DATABIST_TOP_RDREG1_ADDR 0x020001C0
  sim_4ln_regs[0x3024 / 4] =
      0x0004;  // DIG_SOC_LANE_OVRD_REG5_EXTRA_RESET_B_OFFSET
  sim_4ln_regs[0x3030 / 4] =
      0xa000;  // DIG_SOC_LANE_STAT_REG1_ADDR, for tx/rx state ack
  sim_4ln_regs[0x00D0 / 4] = 0x7; // HSREFBUF_BIAS_ADJ_NT and L2R_HSREF_SELECT_NT
  sim_4ln_regs_initd = 1;
}

void aw_4ln_write_csr(uint32_t dev_id,
                      uint32_t subdev_id,
                      uint32_t addr,
                      uint32_t wdata,
                      uint32_t phy_offset_used) {
  if (!sim_4ln_regs_initd) init_sim_4ln_regs();

  // write to simulated reg
  if ((addr >> 28) == 0x8) {  // sram
    sim_4ln_sram[(addr & 0x3fff) / 4] = wdata;
  } else {
    sim_4ln_regs[(addr & 0xffff) / 4] = wdata;
  }

  // subtract off the phy_offset too recover the AW lane info in bits [28:25]
  uint32_t original_offset = addr - phy_offset_used;
  uint32_t lane = (original_offset >> 25) & 0xF;
  uint32_t is_sram = ((addr >> 28) == 0x8);
  uint32_t is_lane_bcast = ((addr >> 29) == 0x2);
  uint32_t cb_addr;

  if (is_sram) {
    cb_addr =
        phy_offset_used + (original_offset & 0x1fffffff) + 0x28000;  // SRAM0
  } else if (is_lane_bcast) {
    uint32_t macro;
    uint32_t macro_subdev_id;
    uint32_t rc;

    // determine the accessed serdes struct
    rc = map_address_to_macro(phy_offset_used, &macro_subdev_id, &macro);
    if (rc != 0) {
      printf("ERROR: can't identify macro for addr=%08x\n", addr);
    } else if (0 && macro_subdev_id != subdev_id) {
      printf("ERROR: different subdev's identified in access (%d/%d)\n",
             subdev_id,
             macro_subdev_id);
    } else {
      // strip lane broadcast and lane offset its
      cb_addr = phy_offset_used + (original_offset & 0xffff);
      if (aw_print_accesses) {
        printf("BCAST: %08x %08x (macro=%d)\n", cb_addr, wdata, macro);
      }
      // turn on lane bcast in glue logic
      bf_tof3_serdes_lane_bcast_set(dev_id, subdev_id, macro, 1);
      if (bf_tof3_serdes_sppt(dev_id)) {
        lld_subdev_write_register(dev_id, subdev_id, cb_addr, wdata);
      }
      // turn off lane bcast in glue logic
      bf_tof3_serdes_lane_bcast_set(dev_id, subdev_id, macro, 0);
    }
    return;
  } else if (lane == 0) {      // CMN
    cb_addr = addr + 0x20000;  // offset to CMN regs
  } else {
    lane = lane - 1;
    cb_addr = phy_offset_used + (addr & 0xFFFF) + (lane * 0x4000);
  }
  if (aw_print_accesses) {
    printf("wr %08x %08x\n", cb_addr, wdata);
  }
  if (bf_tof3_serdes_sppt(dev_id)) {
    lld_subdev_write_register(dev_id, subdev_id, cb_addr, wdata);
  }
}

void aw_4ln_read_csr(uint32_t dev_id,
                     uint32_t subdev_id,
                     uint32_t addr,
                     uint32_t *rdata,
                     uint32_t phy_offset_used) {
  if (!sim_4ln_regs_initd) init_sim_4ln_regs();

  // subtract off the phy_offset too recover the AW lane info in bits [28:25]
  uint32_t original_offset = addr - phy_offset_used;
  uint32_t lane = (original_offset >> 25) & 0xF;
  uint32_t is_sram = ((addr >> 28) == 0x8);
  uint32_t cb_addr;

  if (is_sram) {
    cb_addr =
        phy_offset_used + (original_offset & 0x1fffffff) + 0x28000;  // SRAM0
  } else if (lane == 0) {                                            // CMN
    cb_addr = addr + 0x20000;  // offset to CMN regs
  } else {
    lane = lane - 1;
    cb_addr = phy_offset_used + (addr & 0xFFFF) + (lane * 0x4000);
    //printf("addr=%08x : phy_offset_used=%08x : lane=%d : cb_addr=%08x\n",
    //       addr, phy_offset_used, lane, cb_addr);
  }

  if (bf_tof3_serdes_sppt(dev_id)) {
    lld_subdev_read_register(dev_id, subdev_id, cb_addr, rdata);
    if ((*rdata == 0x0bad0bad) || (*rdata == 0x0ecc0ecc)) {
      bad_access(cb_addr, *rdata);
    }
  } else {
    // pick up simulated reg
    *rdata = sim_4ln_regs[(addr & 0xffff) / 4];
  }
  if (aw_print_accesses) {
    // note: for ATE vector, ignore read value
    printf("rd %08x 00000000 %08x\n", cb_addr, *rdata);
  }
}

/*****************************************************************************
 *
 * Warriors (16ln) I/O functions
 *
 *****************************************************************************/

/******************************************************
 * Arrays to hold simulated register contents for 16ln
 * IP. This is so the APIs can accumulate bits in a
 * given reg.
 */
uint32_t sim_16ln_regs_initd = 0;
uint32_t sim_16ln_regs[4096] = {0};
uint32_t sim_16ln_sram[16384] = {0};

void init_sim_16ln_regs(void) {
  sim_16ln_regs[0x14C / 4] = 0x8000;  // DIG_SOC_CMN_OVRD_EXTRA_RESET_B_OFFSET
  sim_16ln_regs[0x150 / 4] = 1;  // DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET
  sim_16ln_regs[0x1E4 / 4] =
      0x0018;  // RX_DATABIST_TOP_RDREG1_ADDR, cnt_done & bist lock
  sim_16ln_regs[0x3024 / 4] =
      0x0004;  // DIG_SOC_LANE_OVRD_REG5_EXTRA_RESET_B_OFFSET
  sim_16ln_regs[0x3030 / 4] =
      0xa000;  // DIG_SOC_LANE_STAT_REG1_ADDR, for tx/rx power-dn ack
  sim_16ln_regs[0x00D0 / 4] = 0x7; // HSREFBUF_BIAS_ADJ_NT and L2R_HSREF_SELECT_NT
  sim_16ln_regs[0x04D0 / 4] = 0x0000445b; // RX_SIGNAL_DETECT_REG3_ADDR
  sim_16ln_regs[0x1048/4] =
      (0xB4 << 8) | (1<<6); // TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, TX_DATAPATH_REG1_PAM4_MODE_A_OFFSET
  sim_16ln_regs_initd = 1;
}

void aw_16ln_write_csr(uint32_t dev_id,
                       uint32_t subdev_id,
                       uint32_t addr,
                       uint32_t wdata,
                       uint32_t phy_offset_used) {
  if (!sim_16ln_regs_initd) init_sim_16ln_regs();

  // write to simulated reg
  if ((addr >> 28) == 0xa) {  // sram
    sim_16ln_sram[(addr & 0x3fff) / 4] = wdata;
  } else {
    sim_16ln_regs[(addr & 0xffff) / 4] = wdata;
  }

  // subtract off the phy_offset to recover the AW lane info in bits [28:25]
  uint32_t original_offset = addr - phy_offset_used;
  uint32_t lane = (original_offset >> 25) & 0xF;
  uint32_t is_sram = ((addr >> 28) == 0xa);
  uint32_t is_lane_bcast = ((addr >> 29) == 0x2);
  uint32_t cb_addr;

  //if ((!is_sram && !is_lane_bcast) &&
  //    ((addr >> 28) != 0)) {
  //  printf("Warning: addr %08x treated as single reg write\n", addr);
  //} 

  if (is_sram) {
    cb_addr =
        phy_offset_used + (original_offset & 0x1fffffff) + 0x28000;  // SRAM0
  } else if (is_lane_bcast) {
    uint32_t macro;
    uint32_t macro_subdev_id;
    uint32_t rc;

    // determine the accessed serdes struct
    rc = map_address_to_macro(phy_offset_used, &macro_subdev_id, &macro);
    if (rc != 0) {
      printf("ERROR: can't identify macro for addr=%08x\n", addr);
    } else if (0 && macro_subdev_id != subdev_id) {
      printf("ERROR: different subdev's identified in access (%d/%d)\n",
             subdev_id,
             macro_subdev_id);
    } else {
      // strip lane broadcast and lane offset bits
      cb_addr = phy_offset_used + (original_offset & 0xffff);
      if (aw_print_accesses) {
        printf("wr %08x %08x (macro=%d)\n", cb_addr, wdata, macro);
      }
      // turn on lane bcast in glue logic
      bf_tof3_serdes_lane_bcast_set(dev_id, subdev_id, macro, 1);
      if (bf_tof3_serdes_sppt(dev_id)) {
        lld_subdev_write_register(dev_id, subdev_id, cb_addr, wdata);
      }
      // turn off lane bcast in glue logic
      bf_tof3_serdes_lane_bcast_set(dev_id, subdev_id, macro, 0);
    }
    return;
  } else if (lane == 0) {      // CMN
    cb_addr = addr + 0x20000;  // offset to CMN regs
  } else {
    lane = lane - 1;
    cb_addr = phy_offset_used + (addr & 0xFFFF) + (lane * 0x4000);
  }
  if (aw_print_accesses) {
    printf("wr %08x %08x\n", cb_addr, wdata);
  }
  if (bf_tof3_serdes_sppt(dev_id)) {
    lld_subdev_write_register(dev_id, subdev_id, cb_addr, wdata);
  }
}

void aw_16ln_read_csr(uint32_t dev_id,
                      uint32_t subdev_id,
                      uint32_t addr,
                      uint32_t *rdata,
                      uint32_t phy_offset_used) {
  if (!sim_16ln_regs_initd) init_sim_16ln_regs();

  // subtract off the phy_offset too recover the AW lane info in bits [28:25]
  uint32_t original_offset = addr - phy_offset_used;
  uint32_t lane = (original_offset >> 25) & 0xF;
  uint32_t is_sram = ((addr >> 28) == 0xa);
  uint32_t cb_addr;

  if (is_sram) {
    cb_addr =
        phy_offset_used + (original_offset & 0x1fffffff) + 0x28000;  // SRAM0
  } else if (lane == 0) {                                            // CMN
    cb_addr = addr + 0x20000;  // offset to CMN regs
  } else {
    lane = lane - 1;
    cb_addr = phy_offset_used + (addr & 0xFFFF) + (lane * 0x4000);
  }

  if (bf_tof3_serdes_sppt(dev_id)) {
    lld_subdev_read_register(dev_id, subdev_id, cb_addr, rdata);
    if ((*rdata == 0x0bad0bad) || (*rdata == 0x0ecc0ecc)) {
      bad_access(cb_addr, *rdata);
    }
  } else {
    // pick up simulated reg
    *rdata = sim_16ln_regs[(addr & 0xffff) / 4];
  }
  if (aw_print_accesses) {
    // note: for ATE vector, ignore read value
    printf("rd %08x 00000000 %08x\n", cb_addr, *rdata);
  }
}

/*****************************************************************************
 *
 * Vectors for all I/O functions
 *
 *****************************************************************************/

aw_hw_io_t aw_evb_sim_io = {aw_evb_sim_write_csr, aw_evb_sim_read_csr};
aw_hw_io_t aw_evb_io = {aw_evb_write_csr, aw_evb_read_csr};
aw_hw_io_t aw_4ln_io = {aw_4ln_write_csr, aw_4ln_read_csr};
aw_hw_io_t aw_16ln_io = {aw_16ln_write_csr, aw_16ln_read_csr};
