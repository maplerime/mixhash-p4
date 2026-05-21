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

#ifndef port_mgr_serdes_h_included
#define port_mgr_serdes_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t port_mgr_serdes_init(bf_dev_id_t dev_id);
void port_mgr_serdes_init_warm_boot(bf_dev_id_t dev_id);
int port_mgr_serdes_get_num_rings(bf_dev_id_t dev_id);
int port_mgr_serdes_get_num_serdes(bf_dev_id_t dev_id);
void port_mgr_serdes_set_is_serdes(bf_dev_id_t dev_id, int ring, int sd);
int port_mgr_serdes_is_serdes(bf_dev_id_t dev_id, int ring, int sd);
int port_mgr_serdes_is_eth_serdes(bf_dev_id_t dev_id, int ring, int sd);

bf_status_t port_mgr_serdes_log_dfe(bf_dev_id_t dev_id,
                                    bf_dev_port_t port,
                                    int lane);
bf_status_t port_mgr_serdes_hw_cfg_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port);
bf_status_t port_mgr_serdes_delta_compute(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    bf_ha_port_reconcile_info_t *recon_info);
bf_status_t port_mgr_serdes_delta_settings_apply(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port);
bf_status_t port_mgr_serdes_firmware_upgrade(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t fw_ver,
                                             char *fw_path);
bf_status_t port_mgr_serdes_tof_dfe_cfg_get(bf_dev_id_t dev_id,
                                            uint32_t ring,
                                            uint32_t sd,
                                            uint32_t *dfe_ctrl,
                                            uint32_t *hf_val,
                                            uint32_t *lf_val,
                                            uint32_t *dc_val);
bf_status_t port_mgr_serdes_tof_dfe_cfg_set(bf_dev_id_t dev_id,
                                            uint32_t ring,
                                            uint32_t sd,
                                            uint32_t dfe_ctrl,
                                            uint32_t hf_val,
                                            uint32_t lf_val,
                                            uint32_t dc_val);
bf_status_t port_mgr_serdes_tof_dfe_cfg_default_set(bf_dev_id_t dev_id,
                                                    uint32_t ring,
                                                    uint32_t sd);
bf_status_t port_mgr_tof1_serdes_lane_map_set(
    bf_dev_id_t dev_id,
    bf_mac_block_id_t mac_block,
    bf_mac_block_lane_map_t *lane_map);
bf_status_t port_mgr_tof1_serdes_lane_map_get(
    bf_dev_id_t dev_id,
    bf_mac_block_id_t mac_block,
    bf_mac_block_lane_map_t *lane_map);
bf_status_t bf_serdes_clkobs_clksel_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        bf_clkobs_pad_t pad,
                                        bf_sds_clkobs_clksel_t clk_src);
bf_status_t bf_serdes_clkobs_div_set(bf_dev_id_t dev_id,
                                     bf_clkobs_pad_t pad,
                                     int divider,
                                     bool daisy_sel);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // port_mgr_serdes_h_included
