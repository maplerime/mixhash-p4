/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) SBus-related functions defined in */
/* sbus.cpp. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SBus functions. */

#ifndef AVAGO_SBUS_H_
#define AVAGO_SBUS_H_

EXT uint avago_sbus(     Aapl_t *aapl, uint sbus_addr, unsigned char reg, unsigned char cmd, uint data, int recv_data_back);
EXT uint avago_sbus_rd(  Aapl_t *aapl, uint sbus_addr, unsigned char reg);
EXT uint avago_sbus_wr(  Aapl_t *aapl, uint sbus_addr, unsigned char reg, uint data);
EXT uint avago_sbus_wr_flush(Aapl_t *aapl, uint sbus_addr, unsigned char reg, uint data);
EXT uint avago_sbus_rmw( Aapl_t *aapl, uint sbus_addr, unsigned char reg, uint data, uint mask);
EXT void avago_sbus_reset(Aapl_t *aapl, uint sbus_addr, int hard);
EXT BOOL avago_diag_sbus_rw_test(Aapl_t *aapl, uint sbus_addr, int cycles);
EXT uint avago_sbus_rd_array(Aapl_t *aapl, uint sbus_addr, uint count, const unsigned char *reg, uint *data);

#if AAPL_ALLOW_THREADED_SBUS
EXT void *aapl_sbus_thread_worker(void *aapl_in);
#endif

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS

EXT uint avago_jtag_sbus_fn(Aapl_t *aapl, uint sbus_address, unsigned char data_address, unsigned char command, uint *data);

#if AAPL_ALLOW_OFFLINE_SBUS
EXT uint avago_offline_sbus_fn(Aapl_t *aapl, uint sbus_addr, unsigned char reg_addr, unsigned char command, uint *sbus_data);
EXT int  avago_offline_open_fn(Aapl_t *aapl);
EXT int  avago_offline_close_fn(Aapl_t *aapl);
#endif


#if AAPL_ALLOW_JTAG || AAPL_ALLOW_AACS
EXT void aapl_set_sbus_tap_reset_sel(Aapl_t *aapl, uint chip, uint override);
#endif

#endif /* AAPL_ENABLE_INTERNAL_FUNCTIONS */

#endif /* AVAGO_SBUS_H_ */
