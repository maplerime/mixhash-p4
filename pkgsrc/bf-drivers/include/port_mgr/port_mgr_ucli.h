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

#ifndef PORT_MGR_UCLI_H_INCLUDED
#define PORT_MGR_UCLI_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t port_diag_prbs_stats_display(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         void *display_ucli_cookie);
bf_status_t port_diag_perf_display(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   int fp,
                                   int ch,
                                   void *display_ucli_cookie);
bf_status_t port_diag_plot_eye(bf_dev_id_t dev_id,
                               bf_dev_port_t dev_port,
                               void *display_ucli_cookie);
bf_status_t port_diag_dfe_set(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              uint32_t lane,
                              bool set_all_lane,
                              uint32_t dfe_ctrl,
                              uint32_t hf_val,
                              uint32_t lf_val,
                              uint32_t dc_val,
                              void *display_ucli_cookie);
bf_status_t port_diag_set_tx_eq(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                uint32_t lane,
                                bool set_all_lane,
                                int pre,
                                int atten,
                                int post,
                                int slew,
                                void *display_ucli_cookie);
bf_status_t port_diag_rx_inv_set(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint32_t lane,
                                 bool set_all_lane,
                                 int polarity,
                                 void *display_ucli_cookie);
bf_status_t port_diag_tx_inv_set(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint32_t lane,
                                 bool set_all_lane,
                                 int polarity,
                                 void *display_ucli_cookie);
bf_status_t port_diag_dfe_ical_set(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t lane,
                                   bool set_all_lane,
                                   void *display_ucli_cookie);
bf_status_t port_diag_dfe_pcal_set(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t lane,
                                   bool set_all_lane,
                                   void *display_ucli_cookie);
bf_status_t port_diag_chg_to_prbs(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  void *display_ucli_cookie);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // PORT_MGR_UCLI_H
