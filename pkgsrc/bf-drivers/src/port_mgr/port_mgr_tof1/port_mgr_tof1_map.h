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

#ifndef port_mgr_tof1_map_h
#define port_mgr_tof1_map_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

port_mgr_dev_t *port_mgr_map_dev_id_to_dev_p_allow_unassigned(
    bf_dev_id_t dev_id);
port_mgr_dev_t *port_mgr_map_dev_id_to_dev_p(bf_dev_id_t dev_id);
port_mgr_mac_block_t *port_mgr_map_port_to_mac_block(bf_dev_id_t dev_id,
                                                     uint32_t port);
// port_mgr_port_t *port_mgr_map_dev_port_to_port_allow_unassigned(
//    bf_dev_id_t dev_id, uint32_t port);
// port_mgr_port_t *port_mgr_map_dev_port_to_port(bf_dev_id_t dev_id,
//                                               uint32_t port);
// port_mgr_serdes_t *port_mgr_map_port_lane_to_serdes_allow_unassigned(
//    bf_dev_id_t dev_id, uint32_t port, uint32_t lane);
port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_serdes_int(
    bf_dev_id_t dev_id,
    bf_dev_port_t port,
    uint32_t lane,
    bool allow_unassigned);
port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_serdes(bf_dev_id_t dev_id,
                                                         uint32_t port,
                                                         uint32_t lane);
port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_serdes_allow_unassigned(
    bf_dev_id_t dev_id, uint32_t port, uint32_t lane);
port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_hw_serdes_int(
    bf_dev_id_t dev_id,
    bf_dev_port_t port,
    uint32_t lane,
    bool allow_unassigned);
port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_hw_serdes(bf_dev_id_t dev_id,
                                                            uint32_t port,
                                                            uint32_t lane);
port_mgr_serdes_t *port_mgr_tof1_map_port_lane_to_hw_serdes_allow_unassigned(
    bf_dev_id_t dev_id, uint32_t port, uint32_t lane);
// port_mgr_serdes_t *port_mgr_map_port_lane_to_hw_serdes_allow_unassigned(
//    bf_dev_id_t dev_id, uint32_t port, uint32_t lane);
// port_mgr_serdes_t *port_mgr_map_port_lane_to_hw_serdes(bf_dev_id_t dev_id,
//                                                       uint32_t port,
//                                                       uint32_t lane);
port_mgr_mac_block_t *port_mgr_tof1_map_idx_to_mac_block(bf_dev_id_t dev_id,
                                                         uint32_t idx);
port_mgr_mac_block_t *port_mgr_tof1_map_idx_to_mac_block_allow_unassigned(
    bf_dev_id_t dev_id, uint32_t idx);
port_mgr_serdes_t *port_mgr_tof1_map_ring_sd_to_hw_serdes(bf_dev_id_t dev_id,
                                                          uint32_t ring,
                                                          uint32_t sd);
port_mgr_err_t port_mgr_tof1_map_dev_port_to_all(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 bf_dev_pipe_t *pipe_id,
                                                 bf_dev_port_t *port_id,
                                                 int *mac_block,
                                                 int *ch,
                                                 int *is_cpu_port);
bf_status_t port_mgr_map_port_lane_to_gpio_refclk(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t lane,
                                                  uint32_t *reg,
                                                  uint32_t *bit);
uint32_t port_mgr_tof1_map_dev_port_to_port_index(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port);
#ifdef __cplusplus
}
#endif /* C++ */

#endif
