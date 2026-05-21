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

#ifndef port_mgr_ha_h_included
#define port_mgr_ha_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum port_mgr_ha_stages_t {
  PORT_MGR_HA_NONE = 0,
  PORT_MGR_HA_CFG_REPLAY,
  PORT_MGR_HA_DELTA_COMPUTE,
  PORT_MGR_HA_DELTA_PUSH,
  PORT_MGR_HA_MAX
} port_mgr_ha_stages_t;

bf_status_t port_mgr_ha_hardware_read(bf_dev_id_t dev_id);
bf_status_t port_mgr_ha_compute_delta_changes(bf_dev_id_t dev_id,
                                              bool disable_input_pkts);
bf_status_t port_mgr_ha_push_delta_changes(bf_dev_id_t dev_id);
bf_status_t port_mgr_ha_register_port_corr_action(
    bf_dev_id_t dev_id, bf_ha_port_reconcile_info_per_device_t *recon_info);
bf_status_t port_mgr_ha_port_delta_push_done(bf_dev_id_t dev_id);
bf_status_t port_mgr_ha_disable_traffic(bf_dev_id_t dev_id);
bf_status_t port_mgr_ha_enable_input_packets(bf_dev_id_t dev_id);
bf_status_t port_mgr_ha_port_serdes_upgrade(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t fw_ver,
                                            char *fw_path);
bool bf_ha_stage_is_valid(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
