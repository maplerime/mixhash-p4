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

#ifndef PORT_MGR_TOF1_PORT_H_INCLUDED
#define PORT_MGR_TOF1_PORT_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

// limit MTU on Tofino to avoid a TM issue
#define PORT_MGR_TOF_MAX_FRAME_SZ (10 * 1024)
// Default max-rx-jabber-size, it should be multiple of 16 and lower than 16K
#define PORT_MGR_TOF_DEFLT_MAX_JAB_SZ (0x4000 - 0x10)

bf_status_t port_mgr_tof1_port_add(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   bf_port_attributes_t *port_attrib,
                                   bf_port_cb_direction_t direction);
bf_status_t port_mgr_tof1_port_remove(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bf_port_cb_direction_t direction);
bf_status_t port_mgr_tof1_port_enable(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bool enable);
bf_status_t port_mgr_tof1_port_serdes_upgrade(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t fw_ver,
                                              char *fw_path);

bf_status_t port_mgr_port_disable(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t port_mgr_port_disable_all(bf_dev_id_t dev_id);
bf_status_t port_mgr_port_set_speed(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    bf_port_speed_t speed);

int port_mgr_ch_reqd_by_speed(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              bf_port_speed_t speed);
uint32_t port_mgr_ch_in_use(bf_dev_id_t dev_id, bf_dev_port_t port);
void port_mgr_port_pgm_speed_fec(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 bf_port_speed_t speed,
                                 bf_fec_type_t fec);
bf_status_t port_mgr_port_eth_cpu_port_reset(bf_dev_id_t dev_id,
                                             bf_dev_port_t eth_cpu_port);

void port_mgr_link_up_actions(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
void port_mgr_link_dn_actions(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
int port_mgr_tof1_get_num_lanes(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t port_mgr_tof1_port_delta_compute(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    bf_ha_port_reconcile_info_t *recon_info);
bf_status_t port_mgr_port_serdes_upgrade(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t fw_ver,
                                         char *fw_path);
void port_mgr_init_port_mtx(bf_dev_id_t dev_id);
void port_mgr_deinit_port_mtx(bf_dev_id_t dev_id);
bf_status_t port_mgr_port_tx_ignore_rx_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           bool en);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // PORT_MGR_TOF1_PORT_H_INCLUDED
