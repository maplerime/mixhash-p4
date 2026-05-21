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

#ifndef BF_FSM_HDLRS_H_INCLUDED
#define BF_FSM_HDLRS_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif
// non-AN port fsm handlers
bf_status_t bf_fsm_init_serdes(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_re_init_serdes_rx(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_pll(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_signal_ok(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_dfe_quick(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_dfe_done(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_remote_fault(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_pcs_up(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_ena_mac(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_abort(bf_dev_id_t dev_id, bf_dev_port_t dev_port);

// AN port fsm handlers
bf_status_t bf_fsm_an_init_serdes(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_pll1(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_base_pg(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_next_pg(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_an_good(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_pll2(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_an_cmplt(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_pcs_up(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_pcal_done(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_wait_for_port_dwn_event(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port);
bf_status_t bf_fsm_an_abort(bf_dev_id_t dev_id, bf_dev_port_t dev_port);

bf_status_t bf_fsm_config_serdes(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_for_port_dwn_event(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port);
bf_status_t bf_fsm_re_config_serdes_rx(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port);
// Non serdes fsm handlers
bf_status_t bf_fsm_enable_mac_tx_rx(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_assert_rs_fec(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_deassert_rs_fec(bf_dev_id_t dev_id, bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_lpbk_port_up(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port);
bf_status_t bf_fsm_default_abort(bf_dev_id_t dev_id, bf_dev_port_t dev_port);

// Port fsm handlers based on port-directions
bf_status_t bf_fsm_config_for_tx_mode(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port);
bf_status_t bf_fsm_wait_for_port_up_in_tx_mode(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port);
#ifdef __cplusplus
}
#endif /* C++ */

#endif  // BF_FSM_HDLRS_H_INCLUDED
