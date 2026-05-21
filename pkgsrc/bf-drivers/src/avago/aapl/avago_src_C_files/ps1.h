/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/** Doxygen File Header */
/** @file */
/** @brief PS1 device specific functions. */

#ifndef AVAGO_PS1_H
#define AVAGO_PS1_H

#if AAPL_ENABLE_DIAG && AAPL_ENABLE_FLOAT_USAGE && AAPL_ENABLE_PS1

typedef struct
{
    const char *chip_name;
    int   power_enable[5];
    float voltage[4];
    float current[4];
    int   clock_enable[4];
    float freq_MHz[4];
    int   crystal;
} Avago_ps1_power_t;

EXT BOOL avago_ps1_get_power_settings(Aapl_t *aapl, Avago_ps1_power_t *power);
EXT BOOL avago_ps1_get_power_values(  Aapl_t *aapl, Avago_ps1_power_t *power);
EXT BOOL avago_ps1_set_power_settings(Aapl_t *aapl, Avago_ps1_power_t *power);
EXT BOOL avago_ps1_set_clock_settings(Aapl_t *aapl, Avago_ps1_power_t *power);

EXT BOOL avago_ps1_power_off(         Aapl_t *aapl);
EXT BOOL avago_ps1_set_power_defaults(Aapl_t *aapl);

EXT void  avago_ps1_display_power_status(Aapl_t *aapl);

#endif /* AAPL_ENABLE_DIAG && AAPL_ENABLE_FLOAT_USAGE && AAPL_ENABLE_PS1 */

#endif
