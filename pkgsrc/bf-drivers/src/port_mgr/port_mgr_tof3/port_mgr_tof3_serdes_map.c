/*************************************************
 * Placeholders for serdes mapping functions
 *************************************************/
#include <stdint.h>
#include <bf_types/bf_types.h>
#include "aw_if.h"
#include <tof3_regs/tof3_reg_drv.h>
#include <port_mgr/port_mgr_intf.h>
#include "port_mgr_tof3_map.h"

// hardcoded PCIe phy structs
// indexed by (dev_port - 1000)
// bf_tf3_sd_t pcie_phy_sd[4] = {0};
bf_tf3_sd_t pcie_phy_sd[8] = {0};

/**********************************************************************
 *
 **********************************************************************/
uint32_t map_aw_err_to_bf_err(uint32_t rc) {
  if (rc != 0) {
    return BF_INVALID_ARG;
  }
  return 0;
}

/**********************************************************************
 *
 **********************************************************************/
bf_tf3_sd_t *map_dev_port_to_sd(uint32_t dev_id, uint32_t dev_port,
                                uint32_t ln) {
  port_mgr_tof3_serdes_t *sd;

  if ((dev_port >= 1000) && ((dev_port + ln) < 1008)) {
    return &pcie_phy_sd[((dev_port + ln) - 1000)];
  }
  sd = port_mgr_tof3_map_dev_port_lane_to_serdes(dev_id, dev_port, ln);
  return &sd->sd_cfg;
}

/**********************************************************************
*
   tof3_Serdes4ln_regs serdes0; **< Offset 0x0 (R/W)
   tof3_Serdes_regs serdes1; **< Offset 0x40000 (R/W)
   tof3_Serdes_regs serdes2; **< Offset 0x80000 (R/W)
   tof3_Serdes_regs serdes3; **< Offset 0xc0000 (R/W)
   tof3_Serdes_regs serdes4; **< Offset 0x100000 (R/W)
   tof3_Serdes_regs serdes5; **< Offset 0x140000 (R/W)
   tof3_Serdes_regs serdes6; **< Offset 0x180000 (R/W)
   tof3_Serdes_regs serdes7; **< Offset 0x1c0000 (R/W)
   tof3_Serdes_regs serdes8; **< Offset 0x200000 (R/W)
   tof3_Serdes_regs serdes9; **< Offset 0x240000 (R/W)
   tof3_Serdes_regs serdes10; **< Offset 0x280000 (R/W)
   tof3_Serdes_regs serdes11; **< Offset 0x2c0000 (R/W)
   tof3_Serdes_regs serdes12; **< Offset 0x300000 (R/W)
   tof3_Serdes_regs serdes13; **< Offset 0x340000 (R/W)
   tof3_Serdes_regs serdes14; **< Offset 0x380000 (R/W)
   tof3_Serdes_regs serdes15; **< Offset 0x3c0000 (R/W)
   tof3_Serdes_regs serdes16; **< Offset 0x400000 (R/W)
   tof3_Serdes_regs serdes17; **< Offset 0x440000 (R/W)
   tof3_Serdes_regs serdes18; **< Offset 0x480000 (R/W)
   tof3_Serdes_regs serdes19; **< Offset 0x4c0000 (R/W)
   tof3_Serdes_regs serdes20; **< Offset 0x500000 (R/W)
   tof3_Serdes_regs serdes21; **< Offset 0x540000 (R/W)
   tof3_Serdes_regs serdes22; **< Offset 0x580000 (R/W)
   tof3_Serdes_regs serdes23; **< Offset 0x5c0000 (R/W)
   tof3_Serdes_regs serdes24; **< Offset 0x600000 (R/W)
   tof3_Serdes_regs serdes25; **< Offset 0x640000 (R/W)
   tof3_Serdes_regs serdes26; **< Offset 0x680000 (R/W)
   tof3_Serdes_regs serdes27; **< Offset 0x6c0000 (R/W)
   tof3_Serdes_regs serdes28; **< Offset 0x700000 (R/W)
   tof3_Serdes_regs serdes29; **< Offset 0x740000 (R/W)
   tof3_Serdes_regs serdes30; **< Offset 0x780000 (R/W)
   tof3_Serdes_regs serdes31; **< Offset 0x7c0000 (R/W)
   tof3_Serdes_regs serdes32; **< Offset 0x800000 (R/W)
**********************************************************************/
uint32_t map_macro_to_address(uint32_t macro) {
  // special for PCIe phy
  if (macro == 999) {
    return (offsetof(tof3_reg, device_select.pciephy));
  } else if (macro == 0) {
    return (offsetof(tof3_reg, serdes.serdes0));
  } else {
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);

    return (offsetof(tof3_reg, serdes.serdes1) + ((macro - 1) * stride));
  }
}

/**********************************************************************
 *
 **********************************************************************/
uint32_t map_address_to_macro(uint32_t addr, uint32_t *subdev_id,
                              uint32_t *macro) {
  // pcie_phy
  if ((addr >= (offsetof(tof3_reg, device_select.pciephy))) &&
      (addr < (offsetof(tof3_reg, device_select.pciephy) + 0x40000))) {
    *subdev_id = 0;
    *macro = 999;
    return 0;
  } else if ((addr >= (offsetof(tof3_reg, serdes.serdes0))) &&
             (addr < (offsetof(tof3_reg, serdes.serdes1)))) {
    *subdev_id = 0;
    *macro = 0;
    return 0;
  } else {
    // compute the "logical" serdes (==MAC) then map to physical and subdev
    bf_status_t rc;
    uint32_t stride =
        offsetof(tof3_reg, serdes.serdes2) - offsetof(tof3_reg, serdes.serdes1);
    uint32_t ofs = addr - offsetof(tof3_reg, serdes.serdes1);
    uint32_t serdes = (ofs / stride) + 1; // integer divide

    rc = bf_map_logical_tmac_to_physical(0, (bf_subdev_id_t *)&subdev_id,
                                         serdes, macro);
    return rc;
  }
}

/**********************************************************************
 * There is a one-to-one mapping of MAC stn id (physical mac) to serdesX
 * oon a given die (identified by subdev_id).
 **********************************************************************/
uint32_t map_dev_port_to_macro(uint32_t dev_id, uint32_t dev_port,
                               uint32_t *subdev_id, uint32_t *macro) {
  uint32_t logical_mac, physical_mac = 0;
  uint32_t rc;
  uint32_t pipe_id;
  uint32_t port_id;
  uint32_t ch;
  bool is_cpu_port;
  bf_subdev_id_t subdev;

  // handle PCIe phy special
  if ((dev_port >= 1000) && (dev_port < 1004)) {
    *subdev_id = 0;
    *macro = 999;
    return 0;
  } else if ((dev_port >= 1004) && (dev_port < 1008)) {
    *subdev_id = 1;
    *macro = 999;
    return 0;
  }

  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, &pipe_id, &port_id,
                                         &logical_mac, &ch, &is_cpu_port);
  if (rc != BF_SUCCESS) {
    return -1;
  }
  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev, logical_mac,
                                       &physical_mac);
  if (rc != BF_SUCCESS) {
    return -2;
  }
  *subdev_id = (uint32_t)subdev;
  *macro = physical_mac;
  return 0;
}
