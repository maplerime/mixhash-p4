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

#ifndef port_mgr_tof1_ha_h_included
#define port_mgr_tof1_ha_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void port_mgr_tof1_ha_warm_init(bf_dev_id_t dev_id);
bf_status_t port_mgr_tof1_ha_hardware_read(bf_dev_id_t dev_id);
bf_status_t port_mgr_tof1_ha_compute_delta_changes(bf_dev_id_t dev_id);
bf_status_t port_mgr_tof1_ha_port_serdes_upgrade(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t fw_ver,
                                                 char *fw_path);
bf_status_t port_mgr_tof1_ha_enable_input_packets(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
