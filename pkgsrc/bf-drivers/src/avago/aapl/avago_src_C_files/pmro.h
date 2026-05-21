/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/** Doxygen File Header */
/** @file */
/** @brief PMRO device specific definitions. */

#ifndef AVAGO_PMRO_H
#define AVAGO_PMRO_H

EXT uint avago_pmro_get_metric(Aapl_t *aapl, uint addr);

#if AAPL_ENABLE_DIAG && AAPL_ENABLE_FLOAT_USAGE

typedef struct
{
    int total_delay;           /**< Total delay when all cells active */
    int delay[16];             /**< Total delay when cell X is removed */
} Avago_pmro_loop_t;

typedef struct
{
    double clock_freq;         /**< Frequency of clock driving PMRO in MHz */
    uint   repetitions;        /**< Loops run */
    BOOL   disable_loop[4];    /**< Disables gathering of data for VT loop(s) */
    BOOL   disable_fine_grain; /**< Disables gathering of individual cell delays */
    Avago_pmro_loop_t *loop[4]; /**< Struct of delay for each cell in VT loops */
} Avago_pmro_t;

EXT int  avago_pmro_get_results       (Aapl_t *aapl, uint sbus_addr, Avago_pmro_t *config);
EXT void avago_pmro_print_results     (Aapl_t *aapl, uint sbus_addr, Avago_pmro_t *config);
EXT Avago_pmro_t *avago_pmro_construct(Aapl_t *aapl);
EXT void avago_pmro_destruct          (Aapl_t *aapl, Avago_pmro_t *config);

#endif /* AAPL_ENABLE_DIAG && AAPL_ENABLE_FLOAT_USAGE */
#endif /* AVAGO_PMRO_H */
