#ifndef TOF3_AUTOGEN_H_INCLUDED
#define TOF3_AUTOGEN_H_INCLUDED

#include <stddef.h>
#include <bf_types/bf_types.h>
#include <lld/lld_reg_if.h>
#include <tof3_regs/tof3_reg_drv.h>
#include <port_mgr/port_mgr_log.h>
#include "tmac_access.h"

extern int autogen_log(const char *fmt, ...);

void port_mgr_aw_access_rd32(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t offset,
                             uint32_t *r_data,
                             const char *fn);

void port_mgr_aw_access_wr32(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t offset,
                             uint32_t w_data,
                             const char *fn);

#define aw_rd32(dev, dev_port, ofs, val) \
  port_mgr_aw_access_rd32(dev, dev_port, ofs, val, __func__)

#define aw_wr32(dev, dev_port, ofs, val) \
  port_mgr_aw_access_wr32(dev, dev_port, ofs, val, __func__)

#define autogen_tmac_rd(dev, subdev_id, ofs, ptr) \
  port_mgr_csr_tmac_access_rd32(dev_id, subdev_id, tmac, ofs, ptr, __func__)

#define autogen_tmac_wr(dev, subdev_id, ofs, val) \
  port_mgr_csr_tmac_access_wr32(dev_id, subdev_id, tmac, ofs, val, __func__)

#define tmac_rd64(dev_id, subdev_id, tmac, ofs, val) \
  port_mgr_tmac_access_rd64(dev_id, subdev_id, tmac, ofs, val, __func__)

#define tmac_wr64(dev_id, subdev_id, tmac, ofs, val) \
  port_mgr_tmac_access_wr64(dev_id, subdev_id, tmac, ofs, val, __func__)

#define tmac_rd32(dev_id, subdev_id, tmac, ofs, val) \
  port_mgr_tmac_access_rd32(dev_id, subdev_id, tmac, ofs, val, __func__)

#define tmac_wr32(dev_id, subdev_id, tmac, ofs, val) \
  port_mgr_tmac_access_wr32(dev_id, subdev_id, tmac, ofs, val, __func__)

#endif  // TOF3_AUTOGEN_H_INCLUDED
