/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/** Doxygen File Header */
/** @file */
/** @brief Declarations for measurement functions. */

#ifndef AVAGO_SERDES_MEAS_H_
#define AVAGO_SERDES_MEAS_H_


#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS

EXT int avago_serdes_get_phase(            Aapl_t *aapl, uint sbus_addr);
EXT int avago_serdes_step_phase(Aapl_t *aapl, uint sbus_addr, int new_phase,
                                int *current_phase, BOOL get_errors);
EXT int avago_serdes_get_phase_interpolator_power_state(Aapl_t *aapl, uint sbus_addr);
EXT int avago_serdes_set_dac(Aapl_t *aapl, uint sbus_addr, uint dac, BOOL get_errors);
EXT int avago_serdes_set_dac2(Aapl_t *aapl, uint sbus_addr, uint dac, BOOL full_precision, BOOL get_errors);

EXT bigint avago_serdes_get_error_timer(Aapl_t *aapl, uint sbus_addr);

EXT BOOL avago_serdes_get_rx_test_chan_rclk(Aapl_t *aapl, uint sbus_addr);
EXT int  avago_serdes_set_rx_test_chan_rclk(Aapl_t *aapl, uint sbus_addr, BOOL select_rclk);

EXT int avago_serdes_find_phase_center(Aapl_t *aapl, uint sbus_addr, char **log_message);


/* EScope functions: */

EXT uint avago_least_common_multiple(uint a, uint b);
EXT int avago_meas_calculate_heartbeat(int pattern_length, int rx_clock_divider);
EXT int avago_meas_setup_heartbeat(Aapl_t *aapl, uint sbus_addr, int pat_len);
EXT int avago_meas_setup_capture_offset(Aapl_t *aapl, uint sbus_addr, int capture_offset);
EXT int avago_meas_capture_and_read(Aapl_t *aapl, uint sbus_addr, int base_compare_register, int bits_to_read, short data[8]);
EXT int avago_meas_setup_data_capture(Aapl_t *aapl, uint addr, uint pattern_length, uint ui, uint window);

#endif /* AAPL_ENABLE_INTERNAL_FUNCTIONS */

EXT char *avago_serdes_pattern_capture(Aapl_t *aapl, uint addr, int pattern_length);
EXT int  avago_serdes_set_error_timer(Aapl_t *aapl, uint sbus_addr, bigint dwell_bits);


EXT BOOL avago_serdes_dfe_los(Aapl_t *aapl, uint sbus_addr);
EXT BOOL avago_serdes_dfe_running(Aapl_t *aapl, uint sbus_addr);
EXT int  avago_serdes_dfe_pause( Aapl_t *aapl, uint sbus_addr, uint *rr_enabled);
EXT int  avago_serdes_dfe_wait(  Aapl_t *aapl, uint sbus_addr);
EXT int  avago_serdes_dfe_wait_timeout(  Aapl_t *aapl, uint sbus_addr, int timeout);
EXT int  avago_serdes_dfe_resume(Aapl_t *aapl, uint sbus_addr, uint rr_enable);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT int  avago_serdes_get_dac_range(Aapl_t *aapl, uint sbus_addr);
EXT uint avago_serdes_get_phase_multiplier(Aapl_t *aapl, uint sbus_addr);
EXT int  avago_serdes_power_down_phase_interpolator(Aapl_t *aapl, uint sbus_addr);
#endif /* AAPL_ENABLE_INTERNAL_FUNCTIONS */

#endif /* AVAGO_SERDES_MEAS_H_ */
