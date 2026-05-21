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

#ifndef PORT_MGR_PORT_H_INCLUDED
#define PORT_MGR_PORT_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

// Default max-frame-size
#define PORT_MGR_TOF_DEFLT_MAX_FRAME_SZ (16 * 1024 - 256)

bf_status_t port_mgr_port_add(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              bf_port_attributes_t *port_attrib,
                              bf_port_cb_direction_t direction);
bf_status_t port_mgr_port_remove(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 bf_port_cb_direction_t direction);
bf_status_t port_mgr_port_enable(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 bool enable);
void port_mgr_link_up_actions(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
void port_mgr_link_dn_actions(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t port_mgr_port_read_counter(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       bf_rmon_counter_t ctr_id,
                                       uint64_t *ctr_value);
bf_status_t port_mgr_port_read_all_counters(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port);
int port_mgr_get_num_lanes(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t port_mgr_port_bind_interrupt_callback(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  bf_port_int_callback_t fn,
                                                  void *userdata);
bf_status_t port_mgr_port_config_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port);
bf_status_t port_mgr_port_bring_up_time_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint64_t *bring_up_time_us);
bf_status_t port_mgr_port_link_up_time_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint64_t *bring_up_time_us);
bf_status_t port_mgr_port_signal_detect_time_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port);
bf_status_t port_mgr_port_an_lt_start_time_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port);
bf_status_t port_mgr_port_an_lt_dur_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port);
bf_status_t port_mgr_port_an_lt_stats_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint64_t *an_lt_dur_us,
                                          uint32_t *an_try_cnt);
bf_status_t port_mgr_port_an_try_inc(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port);
bf_status_t port_mgr_port_an_lt_stats_init(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           bool init_all);
ucli_status_t bf_drv_show_tech_ucli_port__(ucli_context_t *uc);
void port_mgr_port_default_int_bh_wakeup_cb(bf_dev_id_t dev_id);
void port_mgr_handle_port_int_notif(bf_dev_id_t dev_id);
void port_mgr_tofino3_handle_port_int_notif(bf_dev_id_t dev_id);
void port_mgr_port_int_bh_wakeup(bf_dev_id_t dev_id);
uint port_mgr_max_frame_sz_get(bf_dev_id_t dev_id);
#ifdef __cplusplus
}
#endif /* C++ */

#endif  // PORT_MGR_PORT_H_INCLUDED
