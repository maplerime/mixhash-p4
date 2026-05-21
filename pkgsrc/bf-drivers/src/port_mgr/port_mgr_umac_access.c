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

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_reg_if.h>
#include <tofino_regs/tofino.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/port_mgr_intf.h>
#include "port_mgr.h"
#include "port_mgr_dev.h"
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof2/port_mgr_tof2_port.h"

extern bool port_mgr_tof2_umac_is_cpu_port(bf_dev_id_t dev_id, uint32_t umac);
extern bool autogen_log_en;

/*****************************************************************************
 * CSR accesses have already computed base and stride
 ****************************************************************************/
void port_mgr_csr_access_rd32(bf_dev_id_t dev_id,
                              uint32_t umac,
                              uint32_t offset,
                              uint32_t *r_data,
                              const char *fn) {
  // these pre-rd logs are mainly useful on the emulator
  if (0 && autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Rd : %08x : -------- : %s",
                 dev_id,
                 umac,
                 offset,
                 fn);
  }

  lld_read_register(dev_id, offset, r_data);

  if (autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Rd : %08x : %08x :",
                 dev_id,
                 umac,
                 offset,
                 *r_data);
  }
}

/*****************************************************************************
 * CSR accesses have already computed base and stride
 ****************************************************************************/
void port_mgr_csr_access_wr32(bf_dev_id_t dev_id,
                              uint32_t umac,
                              uint32_t offset,
                              uint32_t w_data,
                              const char *fn) {
  if (autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Wr : %08x : %08x : %s",
                 dev_id,
                 umac,
                 offset,
                 w_data,
                 fn);
  }
  lld_write_register(dev_id, offset, w_data);
}

/*****************************************************************************
  if ((umac == PORT_MGR_TOF2_CPU_PORT_UMAC3) ||
      (umac == PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3)) {  // cpu port, UMAC3
 ****************************************************************************/
void port_mgr_umac_access_rd32(bf_dev_id_t dev_id,
                               uint32_t umac,
                               uint32_t offset,
                               uint32_t *r_data,
                               const char *fn) {
  uint32_t base;

  if (port_mgr_dev_is_tof1(dev_id)) {
    base = offsetof(Tofino, macs_t[umac]);
  } else {  // tof2
    if (umac == PORT_MGR_TOF2_CPU_PORT_UMAC3) {
      base = offsetof(tof2_reg, eth100g_regs.eth100g_umac3);
    } else if (umac == PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3) {
      base = offsetof(tof2_reg, eth100g_regs_rot.eth100g_umac3);
    } else {
      uint32_t stride =
          offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
      base =
          offsetof(tof2_reg, eth400g_p1.eth400g_umac4) + ((umac - 1) * stride);
    }
  }
  offset += base;

  // these pre-rd logs are mainly useful on the emulator
  if (0 && autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Rd : %08x : -------- : %s",
                 dev_id,
                 umac,
                 offset,
                 fn);
  }
  lld_read_register(dev_id, offset, r_data);

  if (autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Rd : %08x : %08x :",
                 dev_id,
                 umac,
                 offset,
                 *r_data);
  }
}

/*****************************************************************************
 ****************************************************************************/
void port_mgr_umac_access_wr32(bf_dev_id_t dev_id,
                               uint32_t umac,
                               uint32_t offset,
                               uint32_t w_data,
                               const char *fn) {
  uint32_t base;

  if (port_mgr_dev_is_tof1(dev_id)) {
    base = offsetof(Tofino, macs_t[umac]);
  } else {  // tof2
    if (umac == PORT_MGR_TOF2_CPU_PORT_UMAC3) {
      base = offsetof(tof2_reg, eth100g_regs.eth100g_umac3);
    } else if (umac == PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3) {
      base = offsetof(tof2_reg, eth100g_regs_rot.eth100g_umac3);
    } else {
      uint32_t stride =
          offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
      base =
          offsetof(tof2_reg, eth400g_p1.eth400g_umac4) + ((umac - 1) * stride);
    }
  }
  offset += base;

  if (autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Wr : %08x : %08x : %s",
                 dev_id,
                 umac,
                 offset,
                 w_data,
                 fn);
  }

  lld_write_register(dev_id, offset, w_data);
}

/*****************************************************************************
 * port_mgr_umac_address_get
 *
 * Return the chip offset of the passed umac register
 ****************************************************************************/
uint32_t port_mgr_umac_address_get(bf_dev_id_t dev_id,
                                   uint32_t umac,
                                   uint32_t offset) {
  uint32_t base;

  if (port_mgr_tof2_umac_is_cpu_port(dev_id, umac)) {
    base = offsetof(tof2_reg, eth100g_regs.eth100g_umac3);
  } else {
    uint32_t stride =
        offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
    base = offsetof(tof2_reg, eth400g_p1.eth400g_umac4) + ((umac - 1) * stride);
  }
  offset += base;
  return offset;
}

/*****************************************************************************
 * port_mgr_umac_access_rd64
 *
 * Read a 64b Comira umac4 register.
 ****************************************************************************/
void port_mgr_umac_access_rd64(bf_dev_id_t dev_id,
                               uint32_t umac,
                               uint32_t offset,
                               uint64_t *reg64,
                               const char *fn) {
  uint32_t lower, upper;
  uint32_t base;

  if (umac == PORT_MGR_TOF2_CPU_PORT_UMAC3) {
    base = offsetof(tof2_reg, eth100g_regs.eth100g_umac3);
  } else if (umac == PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3) {
    base = offsetof(tof2_reg, eth100g_regs_rot.eth100g_umac3);
  } else {
    uint32_t stride =
        offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
    base = offsetof(tof2_reg, eth400g_p1.eth400g_umac4) + ((umac - 1) * stride);
  }
  offset += base;
  lld_read_register(dev_id, offset, &lower);
  lld_read_register(dev_id, offset + 4, &upper);

  if (autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Rd : %08x : %08x_%08x : %s",
                 dev_id,
                 umac,
                 offset,
                 upper,
                 lower,
                 fn);
  }
  *reg64 = ((uint64_t)upper << 32ull) | ((uint64_t)lower);
}

/*****************************************************************************
 * port_mgr_umac_access_wr64
 *
 * Write a 64b Comira umac4 register.
 ****************************************************************************/
void port_mgr_umac_access_wr64(bf_dev_id_t dev_id,
                               uint32_t umac,
                               uint32_t offset,
                               uint64_t reg64,
                               const char *fn) {
  uint32_t lower, upper;
  uint32_t base;

  if (umac == PORT_MGR_TOF2_CPU_PORT_UMAC3) {
    base = offsetof(tof2_reg, eth100g_regs.eth100g_umac3);
  } else if (umac == PORT_MGR_TOF2_ROT_CPU_PORT_UMAC3) {
    base = offsetof(tof2_reg, eth100g_regs_rot.eth100g_umac3);
  } else {
    uint32_t stride =
        offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);
    base = offsetof(tof2_reg, eth400g_p1.eth400g_umac4) + ((umac - 1) * stride);
  }
  offset += base;

  lower = (uint32_t)(reg64 & 0xffffffffull);
  upper = (uint32_t)(reg64 >> 32ull);

  if (autogen_log_en) {
    port_mgr_log("TRC : %d: p%02d :     : Wr : %08x : %08x_%08x : %s",
                 dev_id,
                 umac,
                 offset,
                 upper,
                 lower,
                 fn);
  }
  lld_write_register(dev_id, offset, lower);
  lld_write_register(dev_id, offset + 4, upper);
}
