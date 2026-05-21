/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) sensor functions */

/** Doxygen File Header */
/** @file */
/** @brief Function definitions for temperature and voltage sensor. */

#ifndef AAPL_SENSOR_H_
#define AAPL_SENSOR_H_

/* Sensor functions: */
EXT void avago_sensor_start_temperature(Aapl_t *aapl, uint addr, int sensor, uint frequency);
EXT int  avago_sensor_wait_temperature(Aapl_t *aapl, uint addr, int sensor);
EXT int  avago_sensor_get_temperature(Aapl_t *aapl, uint addr, int sensor, uint frequency);
EXT void avago_sensor_start_voltage(Aapl_t *aapl, uint addr, int sensor, uint frequency);
EXT int  avago_sensor_wait_voltage(Aapl_t *aapl, uint addr, int sensor);
EXT int  avago_sensor_get_voltage(Aapl_t *aapl, uint addr, int sensor, uint frequency);

#endif  /* AAPL_SENSOR_H_ */
