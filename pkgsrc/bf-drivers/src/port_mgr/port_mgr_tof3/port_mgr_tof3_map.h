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


#ifndef port_mgr_tof3_map_h
#define port_mgr_tof3_map_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "port_mgr_tof3_physical_dev.h"

port_mgr_err_t port_mgr_tof3_map_dev_port_to_all(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t *pipe_id,
                                                 uint32_t *port_id,
                                                 uint32_t *tmac,
                                                 uint32_t *ch,
                                                 bool *is_cpu_port);
uint32_t port_mgr_tof3_map_dev_port_to_port_index(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port);
port_mgr_tof3_serdes_t *port_mgr_tof3_map_tmac_ch_to_serdes(bf_dev_id_t dev_id,
                                                            uint32_t tmac,
                                                            uint32_t ch);
port_mgr_tof3_serdes_t *port_mgr_tof3_map_dev_port_lane_to_serdes(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln);
port_mgr_tmac_t *port_mgr_tof3_map_dev_port_lane_to_tmac(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port);

bf_status_t bf_map_logical_tmac_to_physical(bf_dev_id_t dev_id,
                                            bf_subdev_id_t *subdev_id,
                                            uint32_t logical_tmac,
                                            uint32_t *physical_tmac);

bf_status_t bf_map_physical_tmac_to_logical(bf_dev_id_t dev_id,
                                            bf_subdev_id_t subdev_id,
                                            uint32_t physical_tmac,
                                            uint32_t *logical_tmac);

bool port_mgr_tof3_dev_port_is_cpu_port(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port);
#ifdef __cplusplus
}
#endif /* C++ */

#endif
