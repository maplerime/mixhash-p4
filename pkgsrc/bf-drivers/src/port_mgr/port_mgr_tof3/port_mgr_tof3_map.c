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

#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <dvm/bf_drv_intf.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_log.h>
#include <port_mgr/port_mgr_map.h>
#include "port_mgr_tof3_physical_dev.h"
#include "port_mgr_tof3_map.h"

/*************************************************
 * port_mgr_tof3_map_dev_port_to_all
 *
 * Map chip # and bf_dev_port_t to the all corresponding
 * non-NULL values.
 *
 * Tof3 dev port mapping is as follows:
 *
 * pipe[9:7] dev_port[6:0]
 * Pipe, 0-3     Port, 0-71 (only even ports are supported)
 ** -------------+-------------+
 *      0       |      0      | PCIe
 *              |      2,4    | Eth CPU (tmac3) OR recirc_port_1 & 2
 *              |      6      | Recirc_port_3 (not supported by port_mgr)
 *              |      8-71   | tmac1-8
 * -------------+-------------+
 *      1       |      0      | Recirc (not supported by port_mgr)
 *              |      2,4    | Recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac9-16
 * -------------+-------------+
 *      2       |      0      | Recirc (not supported by port_mgr)
 *              |      2,4    | Recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac17-24
 * -------------+-------------+
 *      3       |      0      | Recirc (not supported by port_mgr)
 *              |      2,4    | Recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac25-32
 * -------------+-------------+
 *      4       |      0      | nop (not supported by port_mgr)
 *              |      2,4    | recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac34-41
 * -------------+-------------+
 *      5       |      0      | recirc (not supported by port_mgr)
 *              |      2,4    | recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac42-49
 * -------------+-------------+
 *      6       |      0      | recirc (not supported by port_mgr)
 *              |      2,4    | recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac50-57
 * -------------+-------------+
 *      7       |      0      | recirc (not supported by port_mgr)
 *              |      2,4    | recirc (not supported by port_mgr)
 *              |      6      | Recirc (not supported by port_mgr)
 *              |      8-71   | tmac58-65
 * -------------+-------------+
 *************************************************/
port_mgr_err_t port_mgr_tof3_map_dev_port_to_all(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t *pipe_id,
    uint32_t *port_id, uint32_t *tmac, uint32_t *ch, bool *is_cpu_port) {
  uint32_t channel, mac_blk;
  uint32_t physical_pipe_id;
  uint32_t logical_pipe_id = DEV_PORT_TO_PIPE(dev_port);
  uint32_t port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  lld_err_t rc;
  uint32_t num_pipes;

  if (!DEV_PORT_VALIDATE(dev_port))
    return PORT_MGR_ERR_BAD_PARM;

  rc = lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  if (rc != 0)
    return PORT_MGR_ERR_BAD_PARM;
  if (logical_pipe_id >= num_pipes)
    return PORT_MGR_ERR_BAD_PARM;

  rc = lld_sku_map_pipe_id_to_phy_pipe_id(dev_id, logical_pipe_id,
                                          &physical_pipe_id);
  if (rc != LLD_OK) {
    return PORT_MGR_ERR_BAD_PARM;
  }

  rc = lld_sku_map_dev_port_id_to_mac_ch(dev_id, dev_port, &mac_blk, &channel);
  if (rc != LLD_OK) {
    return PORT_MGR_ERR_BAD_PARM;
  }
  if (pipe_id)
    *pipe_id = physical_pipe_id;
  if (port_id)
    *port_id = port;
  if (tmac)
    *tmac = mac_blk;
  if (ch)
    *ch = channel;
  if (is_cpu_port) {
    *is_cpu_port = false;
    if ((mac_blk == 0) && (logical_pipe_id == 0))
      if ((dev_port == 2) || (dev_port == 4)) {
        *is_cpu_port = true;
      }
  }

  return PORT_MGR_OK;
}

bool port_mgr_tof3_dev_port_is_cpu_port(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port) {
  return dev_port >= lld_get_min_cpu_port(dev_id) &&
         dev_port <= lld_get_max_cpu_port(dev_id);
}

/*************************************************
 * port_mgr_tof3_map_dev_port_to_port_index
 *
 * Support for the linear mapping of port structures
 * in the logical device.
 *************************************************/
uint32_t port_mgr_tof3_map_dev_port_to_port_index(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port) {
  uint32_t pipe = DEV_PORT_TO_PIPE(dev_port);
  uint32_t port = DEV_PORT_TO_LOCAL_PORT(dev_port);
  if (!LOCAL_PORT_VALIDATE(port))
    return -1;

  if ((port % 2) != 0)
    return 0xffffffff;

  if ((pipe == 0) && port_mgr_tof3_dev_port_is_cpu_port(dev_id, dev_port)) {
    return (512 + (port - 2));
  } else if (port < 8) {
    return -1;
  } else {
    return ((pipe * 64) + (port - 8));
  }
  (void)dev_id;
}

/*************************************************
 * port_mgr_tof3_map_dev_port_lane_to_serdes
 *
 * Return ptr to serdes struct
 *************************************************/
port_mgr_tof3_serdes_t *
port_mgr_tof3_map_dev_port_lane_to_serdes(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln) {
  uint32_t tmac, ch;
  port_mgr_err_t rc;
  bool is_cpu_port;

  (void)rc;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);

  assert(dev_p != NULL);
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac,
                                         &ch, &is_cpu_port);
  assert(rc == PORT_MGR_OK);

  return &dev_p->tmac[tmac].sd[(2 * ch + ln)];
}

/*************************************************
 * port_mgr_tof3_map_tmac_ch_to_serdes
 *
 * Return ptr to serdes struct
 *************************************************/
port_mgr_tof3_serdes_t *port_mgr_tof3_map_tmac_ch_to_serdes(bf_dev_id_t dev_id,
                                                            uint32_t tmac,
                                                            uint32_t ch) {
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);
  assert(dev_p != NULL);

  return &dev_p->tmac[tmac].sd[ch];
}

/*************************************************
 * port_mgr_tof3_map_dev_port_lane_to_tmac
 *
 * Return ptr to tmac struct
 *************************************************/
port_mgr_tmac_t *
port_mgr_tof3_map_dev_port_lane_to_tmac(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port) {
  uint32_t tmac, ch;
  bool is_cpu_port;
  port_mgr_err_t rc;

  (void)rc;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);

  assert(dev_p != NULL);
  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &tmac,
                                         &ch, &is_cpu_port);
  assert(rc == PORT_MGR_OK);
  return &dev_p->tmac[tmac];
}

bf_status_t bf_map_logical_tmac_to_physical(bf_dev_id_t dev_id,
                                            bf_subdev_id_t *subdev_id,
                                            uint32_t logical_tmac,
                                            uint32_t *physical_tmac) {
  *physical_tmac = logical_tmac % 33;

  if (logical_tmac < 33) {
    *subdev_id = 0;
  } else {
    // skip mac-0 on die-1
    *physical_tmac = (logical_tmac % 33) + 1;
    *subdev_id = 1;
  }

  if (*physical_tmac >= 33)
    return BF_INVALID_ARG;
  (void)dev_id;
  return BF_SUCCESS;
}

bf_status_t bf_map_physical_tmac_to_logical(bf_dev_id_t dev_id,
                                            bf_subdev_id_t subdev_id,
                                            uint32_t physical_tmac,
                                            uint32_t *logical_tmac) {
  if (logical_tmac == NULL)
    return BF_INVALID_ARG;
  if (physical_tmac >= 33)
    return BF_INVALID_ARG;
  *logical_tmac = subdev_id ? physical_tmac + 32 : physical_tmac;
  (void)dev_id;
  return BF_SUCCESS;
}
