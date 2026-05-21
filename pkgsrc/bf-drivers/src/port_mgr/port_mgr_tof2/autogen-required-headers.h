#ifndef AUTOGEN_H_INCLUDED
#define AUTOGEN_H_INCLUDED

#include <stddef.h>
#include <bf_types/bf_types.h>
#include <lld/lld_reg_if.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/port_mgr_log.h>

extern int autogen_log(const char *fmt, ...);

extern void port_mgr_csr_access_rd32(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t offset,
                                     uint32_t *r_data,
                                     const char *fn);

extern void port_mgr_csr_access_wr32(bf_dev_id_t dev_id,
                                     uint32_t umac,
                                     uint32_t offset,
                                     uint32_t w_data,
                                     const char *fn);

extern void port_mgr_umac_access_rd32(bf_dev_id_t dev_id,
                                      uint32_t umac,
                                      uint32_t offset,
                                      uint32_t *r_data,
                                      const char *fn);

extern void port_mgr_umac_access_wr32(bf_dev_id_t dev_id,
                                      uint32_t umac,
                                      uint32_t offset,
                                      uint32_t w_data,
                                      const char *fn);

extern void port_mgr_umac_access_rd64(bf_dev_id_t dev_id,
                                      uint32_t umac,
                                      uint32_t offset,
                                      uint64_t *reg64,
                                      const char *fn);

extern void port_mgr_umac_access_wr64(bf_dev_id_t dev_id,
                                      uint32_t umac,
                                      uint32_t offset,
                                      uint64_t reg64,
                                      const char *fn);
uint32_t port_mgr_umac_address_get(bf_dev_id_t dev_id,
                                   uint32_t umac,
                                   uint32_t offset);

#define autogen_rd(dev, ofs, ptr) \
  port_mgr_csr_access_rd32(dev, umac, ofs, ptr, __func__)

#define autogen_wr(dev, ofs, val) \
  port_mgr_csr_access_wr32(dev, umac, ofs, val, __func__)

#define umac4_rd64(dev, umac, ofs, val) \
  port_mgr_umac_access_rd64(dev, umac, ofs, val, __func__)

#define umac4_wr64(dev, umac, ofs, val) \
  port_mgr_umac_access_wr64(dev, umac, ofs, val, __func__)

#define umac3_rd32(dev, umac, ofs, val) \
  port_mgr_umac_access_rd32(dev, umac, ofs, val, __func__)

#define umac3_wr32(dev, umac, ofs, val) \
  port_mgr_umac_access_wr32(dev, umac, ofs, val, __func__)

#endif  // AUTOGEN_H_INCLUDED
