/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* SerDes HAL types and functions. */

/** Doxygen File Header */
/** @file */
/** @brief AAPL interface to the SerDes HAL interrupt */

#ifndef AAPL_SERDES_HAL_H_
#define AAPL_SERDES_HAL_H_

#include "aapl.h"

/**@brief CTLE parameters */
typedef struct
{
    short hf;                  /**< High-frequency setting */
    short lf;                  /**< Low-frequency setting */
    short dc;                  /**< DC-restore value */
    short bw;                  /**< Band-width setting */
    short gainshape1;          /**< Gainshape1 setting */
    short gainshape2;          /**< Gainshape2 setting */
    short short_channel_en;    /**< Short channel enable setting */

} Avago_serdes_ctle_t;

/**@brief valid limit of CTLE parameters */
typedef struct
{
    short hf_max;                 /**< Maximum high-Frequency value */
    short lf_max;                 /**< Maximum low-Frequency value */
    short dc_max;                 /**< Maximum dC-Restore value */
    short bw_max;                 /**< Maximum band-width value */
    short gainshape1_max;         /**< Maximum gainshape1 value */
    short gainshape2_max;         /**< Maximum gainshape2 value */
    short short_channel_en_max;   /**< Maximum short channel enable value */

} Avago_serdes_ctle_limits_t;

EXT int avago_serdes_ctle_read(Aapl_t *aapl, uint addr, Avago_serdes_ctle_t *ctle);
EXT int avago_serdes_ctle_write(Aapl_t *aapl, uint addr, Avago_serdes_ctle_t *ctle);
EXT int avago_serdes_ctle_get_limits(Aapl_t *aapl, uint addr, Avago_serdes_ctle_limits_t *limits);
EXT int avago_serdes_ctle_apply(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_ctle_t *ctle);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT int avago_serdes_ctle_validate(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_ctle_t *ctle);
#endif

/**@brief RxFFE parameters */
typedef struct
{
    short pre2;              /**< Pre2-cursor setting */
    short pre1;              /**< Pre1-cursor setting */
    short post1;             /**< Post-cursor setting */
    short bflf;              /**< Low-frequency setting */
    short bfhf;              /**< High-frequency setting */
    short datarate;          /**< Datarate setting */
    short short_channel_en;  /**< Short channel enable setting */
} Avago_serdes_rxffe_t;

/**@brief valid limits of RxFFE parameters */
typedef struct
{
    short pre2_min;               /**< Minimum pre2-cursor value */
    short pre2_max;               /**< Maximum pre2-cursor value */
    short pre1_max;               /**< Maximum pre1-cursor value */
    short post1_min;              /**< Minimum post1-cursor value */
    short post1_max;              /**< Maximum post1-cursor value */
    short bflf_max;               /**< Maximum low-frequency value */
    short bfhf_max;               /**< maximum high-frequency value */
    short datarate_max;           /**< Maximum datarate value */
    short short_channel_en_max;   /**< Maximum Short channel enable value */
} Avago_serdes_rxffe_limits_t;

EXT int avago_serdes_rxffe_get_limits(Aapl_t *aapl, uint addr, Avago_serdes_rxffe_limits_t *limits);
EXT int avago_serdes_rxffe_read(Aapl_t *aapl, uint addr, Avago_serdes_rxffe_t *rxffe);
EXT int avago_serdes_rxffe_write(Aapl_t *aapl, uint addr, Avago_serdes_rxffe_t *rxffe);
EXT int avago_serdes_rxffe_apply(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_rxffe_t *rxffe);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT int avago_serdes_rxffe_validate(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_rxffe_t *rxffe);
#endif

/**@brief DFE parameters */
typedef struct
{
    short gaintap;
    short gaintap2;
    short gain2;
    short gain3;
    short gain4;
    short gain5;
    short gain6;
    short gain7;
    short gain8;
    short gain9;
    short gain10;
    short gain11;
    short gain12;
} Avago_serdes_dfe_t;

typedef struct
{
    short gaintap_min;
    short gaintap_max;
    short gaintap2_min;
    short gaintap2_max;
    short gainx_min;
    short gainx_max;
    short gainy_min;
    short gainy_max;
} Avago_serdes_dfe_limits_t;

EXT int avago_serdes_dfe_get_limits(Aapl_t *aapl, uint addr, Avago_serdes_dfe_limits_t *limits);
EXT int avago_serdes_dfe_read(Aapl_t *aapl, uint addr, Avago_serdes_dfe_t *dfe);
EXT int avago_serdes_dfe_write(Aapl_t *aapl, uint addr, Avago_serdes_dfe_t *dfe);
EXT int avago_serdes_dfe_apply(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_dfe_t *dfe);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT int avago_serdes_dfe_validate(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_dfe_t *dfe);
#endif

typedef struct {
    short upper_odd_dly;
    short upper_even_dly;
    short middle_odd_dly;
    short middle_even_dly;
    short lower_odd_dly;
    short lower_even_dly;
    short test_odd_dly;
    short test_even_dly;
    short edge_odd_dly;
    short edge_even_dly;
    short tap_dly;
}Avago_serdes_vernier_delay_t;

EXT int avago_serdes_vernier_delay_write(Aapl_t *aapl, uint addr, Avago_serdes_vernier_delay_t *clock_rx);
EXT int avago_serdes_vernier_delay_apply(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_vernier_delay_t *clock_rx);
EXT int avago_serdes_vernier_delay_read(Aapl_t *aapl, uint addr, Avago_serdes_vernier_delay_t *clock_rx);

typedef struct
{
    short error_threshold;            /* global threshold for error counts. */
    short nrz_ctle_dwell_1ex;         /* set dwell for CTLE tuning in NRZ. */
    short nrz_ctle_lf_dwell_shift;    /* set extra LF evaluation shift amount. */
    short eye_oversample;             /* lev_coarse eye oversample amount */
    short pam4_ctle_dwell_1ex;        /* set dwell for pam4 ctle tuning. (Eye Ratchet) */
    short pam4_dvos_dwell_1ex;        /* set dwell for DVOS tuning. */
    short pam4_dvos_pcal_dwell_1ex;   /* set dwell for DVOS tuning in pCal. */
    short lms_dwell_1ex;              /* set dwell for LMS algorithms */
    short ctle_fixed;                 /* CTLE fixed parameters 0b[short_channel,gainshate2,gainshape1,bw,hf,lf,dc] */
    short rxffe_fixed;                /* RxFFE fixed parameters 0b[short_channel,bfhf,bflf,post1,pre1,pre2] */
    short dfe_fixed;                  /* DFE fixed parameters 0b[dfe12-dfe2,dfegain2,dfegain1] */
    short gaintap_max_min;            /* gaintap max and min [upper byte = max, lower byte = min] */
    short gaintap2_max_min;           /* gaintap max and min [upper byte = max, lower byte = min] */
    short pCal_loops;                 /* number of pCal loops to run */
    short disable_one_shot_pcal;      /* should we run a one-shot pCal after iCal; */
    short lms_pcal_dwell_1ex;         /* set dwell for LMS algorithms in pCal. */
    short dcr_pcal_dwell_1ex;         /* set dwell for DC nns search in pCal. */
    short pCal_delay;                 /* delay between loops in continuous adaptive. */
    short iq_cal_interleave_disable;  /* disable IQ cal interleave in bootstrap. */
    short ctle_lfhf_max_min;          /* place algorithm limits on ctle [lfmax,lfmin,hfmax,hfmin]. default 0x90f0; */
    short ctle_dc_max_min;            /* place algorithm limits on ctle [dcmax,dcmin]. default 0xff00 */
    short vernier_fixed;              /* Vernier fixed parameters 0b[ data_upper, data_middle, data_lower, edge] */
    short prbs_tune_mode;             /* 0= data independent tuning. 1=prbs based tuning */
    short gradient_options;           /* one hot variables.  0b00000000000 */
}Avago_serdes_global_tune_params_t;

EXT int avago_serdes_global_tune_params_write(Aapl_t *aapl, uint addr, Avago_serdes_global_tune_params_t *global_tune_params);
EXT int avago_serdes_global_tune_params_apply(Aapl_t *aapl, uint addr, uint mask, Avago_serdes_global_tune_params_t *global_tune_params);
EXT int avago_serdes_global_tune_params_read(Aapl_t *aapl, uint addr, Avago_serdes_global_tune_params_t *global_tune_params);

#endif /* AAPL_SERDES_HAL_H_ */
