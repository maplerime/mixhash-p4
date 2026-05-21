/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) support for talking with SBus */
/* Master. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SBus Master SPICO processor functions. */

#ifndef AVAGO_SBUS_MASTER_H_
#define AVAGO_SBUS_MASTER_H_

/* These operate on the SBus Controller: */

EXT int avago_sbm_get_sbus_clock_divider(Aapl_t *aapl, uint sbus_addr);
EXT int avago_sbm_set_sbus_clock_divider(Aapl_t *aapl, uint sbus_addr, uint divider);

EXT int avago_sbm_get_refclk(Aapl_t *aapl, uint addr);

/* These operate on the SBus Master: */
EXT int   avago_sbm_get_temperature(Aapl_t *aapl, uint sensor_addr, uint sensor);

EXT uint avago_sbm_read_mem(     Aapl_t *aapl, uint sbus_addr, uint addr);
EXT uint avago_sbm_read_next_mem(Aapl_t *aapl, uint sbus_addr);
EXT int  avago_sbm_write_mem(    Aapl_t *aapl, uint sbus_addr, uint addr, uint data);

EXT uint avago_sbm_get_firmware_rev(     Aapl_t *aapl, uint sbus_addr);
EXT uint avago_sbm_get_firmware_build_id(Aapl_t *aapl, uint sbus_addr);

EXT int avago_sbm_get_dfe_rr_mode(Aapl_t *aapl, uint addr);
EXT int avago_sbm_set_dfe_rr_mode(Aapl_t *aapl, uint addr, int count);

EXT BOOL avago_sbm_imem_read(Aapl_t *aapl, uint addr, uint mem_addr, uint *data, uint data_len);
EXT void avago_sbm_imem_dump(Aapl_t *aapl, uint addr);
EXT void avago_sbm_dmem_dump(Aapl_t *aapl, uint addr);
EXT void avago_sbm_rom_dump(Aapl_t *aapl, uint addr);


#endif /* AVAGO_SBUS_MASTER_H_ */
