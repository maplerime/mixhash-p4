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

#ifndef port_mgr_tof2_map_h
#define port_mgr_tof2_map_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "port_mgr_tof2_physical_dev.h"

bool port_mgr_tof2_dev_port_is_cpu_port(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port);
bf_status_t port_mgr_tof2_map_dev_port_to_all(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t *pipe_id,
                                              uint32_t *port_id,
                                              uint32_t *umac,
                                              uint32_t *ch,
                                              bool *is_cpu_port);
uint32_t port_mgr_tof2_map_dev_port_to_port_index(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port);
uint32_t port_mgr_tof2_map_dev_port_lane_to_sd_base(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln);
port_mgr_tof2_serdes_t *port_mgr_tof2_map_dev_port_lane_to_serdes(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln);
port_mgr_umac3_t *port_mgr_tof2_map_dev_port_lane_to_umac3(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port);
port_mgr_umac4_t *port_mgr_tof2_map_dev_port_lane_to_umac4(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
