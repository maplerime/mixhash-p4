/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* i2c.h - Defines i2c related functions and constants */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for i2c bus functions. */

#ifndef AVAGO_I2C_H_
#define AVAGO_I2C_H_

#if AAPL_ALLOW_I2C || AAPL_ALLOW_SYSTEM_I2C

EXT int avago_i2c_write(Aapl_t *aapl, uint dev_addr, uint length, unsigned char *buffer);
EXT int avago_i2c_read(Aapl_t *aapl, uint dev_addr, uint length, unsigned char *buffer);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS

EXT uint avago_i2c_sbus_fn(Aapl_t *aapl, uint sbus_addr, unsigned char reg_addr, unsigned char command, uint *sbus_data);

#if AAPL_ALLOW_AACS
EXT int avago_aacs_i2c_write_fn(Aapl_t *aapl, uint dev_addr, uint length, unsigned char *buffer);
EXT int avago_aacs_i2c_read_fn(Aapl_t *aapl, uint dev_addr, uint length, unsigned char *buffer);
#endif

#if AAPL_ALLOW_SYSTEM_I2C
EXT int avago_system_i2c_open_fn(Aapl_t *aapl);
EXT int avago_system_i2c_close_fn(Aapl_t *aapl);
EXT int avago_system_i2c_write_fn(Aapl_t *aapl, uint dev_addr, uint length, unsigned char *buffer);
EXT int avago_system_i2c_read_fn(Aapl_t *aapl, uint dev_addr, uint length, unsigned char *buffer);
#endif

#endif /* AAPL_ENABLE_INTERNAL_FUNCTIONS */

#endif /* AAPL_ALLOW_I2C || AAPL_ALLOW_SYSTEM_I2C */

#endif /* AVAGO_I2C_H_ */
