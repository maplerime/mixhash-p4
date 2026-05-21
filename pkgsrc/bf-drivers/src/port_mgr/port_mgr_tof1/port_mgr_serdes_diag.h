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

#ifndef port_mgr_serdes_diag_h
#define port_mgr_serdes_diag_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void sd_diag_aggressor_add(bf_dev_id_t dev_id, int ring, int sd);
void sd_diag_aggressor_del(bf_dev_id_t dev_id, int ring, int sd);
void sd_diag_aggressor_on(void);
void sd_diag_aggressor_off(void);
void sd_diag_aggressor_cal(void);
void sd_diag_aggressor_show(ucli_context_t *uc);

void sd_diag_victim_add(bf_dev_id_t dev_id, int ring, int sd);
void sd_diag_victim_del(bf_dev_id_t dev_id, int ring, int sd);
void sd_diag_victim_on(void);
void sd_diag_victim_off(void);
void sd_diag_victim_cal(void);
void sd_diag_victim_show(ucli_context_t *uc);

void sd_diag_other_on(void);
void sd_diag_other_off(void);
void sd_diag_other_cal(void);
void sd_diag_other_show(ucli_context_t *uc);

void sd_diag_all_on(void);
void sd_diag_all_off(void);
void sd_diag_all_cal(void);
void sd_diag_all_show(ucli_context_t *uc);
void sd_diag_off(bf_dev_id_t dev_id, int ring, int sd);

void sd_dump_all_dfe(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_dump_all_vbtc(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_dump_all_perf(ucli_context_t *uc, bool is_mav);
void sd_dump_all_perf_prbs(ucli_context_t *uc, bool is_mav);

void sd_perf_stats_collect_on_demand(ucli_context_t *uc, bool is_mav);
void sd_perf_stats_display_on_demand(ucli_context_t *uc, bool is_mav);
void sd_perf_stats_collect(ucli_context_t *uc, bool is_mav);

void sd_all_rptr_term_float(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_rptr_term_avdd(ucli_context_t *uc, bf_dev_id_t dev_id);

void sd_all_bbgain(ucli_context_t *uc,
                   bf_dev_id_t dev_id,
                   uint32_t tx_pll,
                   uint32_t rx_pll);
void sd_all_delay_cal(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_pi_cal(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_adaptive_pcal(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_pcal(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_ical(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_init_all_core_data(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_dfe_set(bf_dev_id_t dev_id,
                    uint32_t dfe_ctrl,
                    uint32_t hf_val,
                    uint32_t lf_val,
                    uint32_t dc_val);
void sd_switch_to_prbs(ucli_context_t *uc, bf_dev_id_t dev_id);

void sd_tx_eq_all(bf_dev_id_t dev_id, int pre, int attn, int post);
void sd_init_prbs(
    ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd, int ilb, int gb);
int sd_init_prbs31_25g(bf_dev_id_t dev_id, int ring, int sd, int ilb);
int sd_init_prbs31_10g(bf_dev_id_t dev_id, int ring, int sd, int ilb);
void sd_cmp_mode_prbs31_all(bf_dev_id_t dev_id);
void sd_cmp_mode_prbs(bf_dev_id_t dev_id, int ring, int sd);
void sd_xtalk_test(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_init_all_prbs(ucli_context_t *uc, bf_dev_id_t dev_id, int ilb, int gb);
void sd_escope(int argc, char **argv);

void sd_run_sweep_ctle(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd);
void sd_run_sweep_single(ucli_context_t *uc,
                         bf_dev_id_t dev_id,
                         int ring,
                         int sd);
void sd_run_sweep_mult(ucli_context_t *uc,
                       bf_dev_id_t dev_id,
                       int ring,
                       int sd);

void sd_set_prbs_mode(ucli_context_t *uc, uint32_t order);
void sd_print_banner(ucli_context_t *uc);
void sd_dump_this(ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd);
void sd_get_debug_stats_this(
    bf_dev_id_t dev_id, int ring, int sd, uint32_t *error, uint32_t *eye);

void sd_dump(ucli_context_t *uc);
void sd_dump_this_sd(ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd);
void sd_dump_this_sd_range(
    ucli_context_t *uc, bf_dev_id_t dev_id, int ring, int sd, int num_sd);
void sd_dump_an(ucli_context_t *uc);
void sd_tech_sppt_dump(bf_dev_id_t dev_id);
void sd_init_aapl(bf_dev_id_t dev_id, int tcp_mode);
void sd_init_aapl_and_load_firmware(ucli_context_t *uc,
                                    bf_dev_id_t dev_id,
                                    int tcp_mode);

void sd_dump_pmd(bf_dev_id_t dev_id, int ring, int sd);
void sd_dump_sensor_data(ucli_context_t *uc,
                         bf_dev_id_t dev_id,
                         int ring,
                         int sd);
void sd_swing(bf_dev_id_t dev_id,
              int ring,
              int sd,
              uint32_t *swing_lo,
              uint32_t *swing_hi,
              uint32_t *lo_los_neq_0,
              uint32_t *lo_sig_ok_eq_0,
              uint32_t *lo_ei_neq_0,
              uint32_t *calibrated_thresh);

void sd_all_swing(ucli_context_t *uc, bf_dev_id_t dev_id);
void sd_all_sig_ok_thresh(bf_dev_id_t dev_id, int thresh);
void sd_all_sig_ok_thresh_en(bf_dev_id_t dev_id);

void sd_display_port_perf(bf_dev_id_t dev_id,
                          int ring,
                          int sd,
                          void *display_ucli_cookie);
bf_status_t sd_dfe_set(bf_dev_id_t dev_id,
                       int ring,
                       int sd,
                       uint32_t dfe_ctrl,
                       uint32_t hf_val,
                       uint32_t lf_val,
                       uint32_t dc_val);
void sd_set_tx_eq(bf_dev_id_t dev_id,
                  int ring,
                  int sd,
                  int pre,
                  int atten,
                  int post,
                  int slew,
                  void *uc);
void sd_perf_banner(ucli_context_t *uc);
bf_status_t sd_diag_prbs_stats_display(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t lane,
                                       void *display_ucli_cookie);
bf_status_t sd_diag_perf_display(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 int fp,
                                 int ch,
                                 uint32_t ln,
                                 void *display_ucli_cookie);
bf_status_t sd_diag_plot_eye(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t ln,
                             void *display_ucli_cookie);
bf_status_t sd_diag_set_tx_eq(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              uint32_t ln,
                              int pre,
                              int atten,
                              int post,
                              int slew,
                              void *display_ucli_cookie);
bf_status_t sd_diag_chg_to_prbs(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                uint32_t ln,
                                void *display_ucli_cookie);
void sd_all_int(ucli_context_t *uc,
                bf_dev_id_t dev_id,
                uint32_t int_code,
                uint32_t int_data);

void sd_aapl_serdes_dump(bf_dev_id_t dev_id,
                         int ring,
                         int sd,
                         ucli_context_t *uc);
#ifdef __cplusplus
}
#endif /* C++ */

#endif
