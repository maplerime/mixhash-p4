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
#include <tof3_regs/tof3_reg_drv.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_dev.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof3_port.h"
#include "tmac_access.h"

extern bool tf3_autogen_log_en;

/*****************************************************************************
 * CSR accesses have already computed base and stride
 ****************************************************************************/
void port_mgr_csr_tmac_access_rd32(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t tmac,
                                   uint32_t offset,
                                   uint32_t *r_data,
                                   const char *fn) {
  // these pre-rd logs are mainly useful on the emulator
  if (0 && tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Rd : %08x : -------- : %s",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 fn);
  }

  lld_subdev_read_register(dev_id, subdev_id, offset, r_data);

  if (tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Rd : %08x : %08x :",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 *r_data);
  }
}

/*****************************************************************************
 * CSR accesses have already computed base and stride
 ****************************************************************************/
void port_mgr_csr_tmac_access_wr32(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t tmac,
                                   uint32_t offset,
                                   uint32_t w_data,
                                   const char *fn) {
  if (tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Wr : %08x : %08x : %s",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 w_data,
                 fn);
  }
  lld_subdev_write_register(dev_id, subdev_id, offset, w_data);
}

/*****************************************************************************
 ****************************************************************************/
void port_mgr_tmac_access_rd32(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t ofs,
                               uint32_t *r_data,
                               const char *fn) {
  uint32_t base = offsetof(tof3_reg, eth400g[tmac]);
  uint32_t offset = base + ofs;

  // these pre-rd logs are mainly useful on the emulator
  if (0 && tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Rd : %08x : -------- : %s",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 fn);
  }
  lld_subdev_read_register(dev_id, subdev_id, offset, r_data);

  if (tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Rd : %08x : %08x :",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 *r_data);
  }
}

/*****************************************************************************
 ****************************************************************************/
void port_mgr_tmac_access_wr32(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t ofs,
                               uint32_t w_data,
                               const char *fn) {
  uint32_t base = offsetof(tof3_reg, eth400g[tmac]);
  uint32_t offset = base + ofs;

  if (tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Wr : %08x : %08x : %s",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 w_data,
                 fn);
  }

  lld_subdev_write_register(dev_id, subdev_id, offset, w_data);
}

/*****************************************************************************
 * port_mgr_tmac_address_get
 *
 * Return the chip offset of the passed tmac register
 ****************************************************************************/
uint32_t port_mgr_tmac_address_get(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t tmac,
                                   uint32_t ofs) {
  uint32_t base = offsetof(tof3_reg, eth400g[tmac]);
  uint32_t offset = base + ofs;
  if (tf3_autogen_log_en) {
    port_mgr_log(
        "TRC : %d: %d-%02d :     : Rd : %08x", dev_id, subdev_id, tmac, offset);
  }

  return offset;
}

/*****************************************************************************
 * port_mgr_tmac_access_rd64
 *
 * Read a 64b Comira tmac4 register.
 ****************************************************************************/
void port_mgr_tmac_access_rd64(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t ofs,
                               uint64_t *reg64,
                               const char *fn) {
  uint32_t lower, upper;
  uint32_t base = offsetof(tof3_reg, eth400g[tmac]);
  uint32_t offset = base + ofs;

  lld_subdev_read_register(dev_id, subdev_id, offset, &lower);
  lld_subdev_read_register(dev_id, subdev_id, offset + 4, &upper);

  if (tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Rd : %08x : %08x_%08x : %s",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 upper,
                 lower,
                 fn);
  }
  *reg64 = ((uint64_t)upper << 32ull) | ((uint64_t)lower);
}

/*****************************************************************************
 * port_mgr_tmac_access_wr64
 *
 * Write a 64b Comira tmac4 register.
 ****************************************************************************/
void port_mgr_tmac_access_wr64(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t ofs,
                               uint64_t reg64,
                               const char *fn) {
  uint32_t lower, upper;
  uint32_t base = offsetof(tof3_reg, eth400g[tmac]);
  uint32_t offset = base + ofs;

  lower = (uint32_t)(reg64 & 0xffffffffull);
  upper = (uint32_t)(reg64 >> 32ull);

  if (tf3_autogen_log_en) {
    port_mgr_log("TRC : %d: %d-%02d :     : Wr : %08x : %08x_%08x : %s",
                 dev_id,
                 subdev_id,
                 tmac,
                 offset,
                 upper,
                 lower,
                 fn);
  }
  lld_subdev_write_register(dev_id, subdev_id, offset, lower);
  lld_subdev_write_register(dev_id, subdev_id, offset + 4, upper);
}
