/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserializer) slices on ASIC SBus rings. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations and types for SerDes DFE functions. */

#ifndef AVAGO_SERDES_DFE_H_
#define AVAGO_SERDES_DFE_H_

/**@brief Rx DFE Settings and Control functions for DFE tuning. */
typedef struct
{
    uint dc;               /**< DC-Restore value [0-255] */
    uint lf;               /**< CTLE Low-Frequency setting [0-15] */
    uint hf;               /**< CTLE High-Frequency setting [0-15] */
    uint bw;               /**< CTLE Band-width setting [0-15] */
    uint gainshape1;       /**< CTLE gainshape1 setting */
    uint gainshape2;       /**< CTLE gainshape2 setting */
    uint short_channel_en; /**< CTLE Short channel setting */

    uint dfeGAIN;    /**< DFE Gain Tap strength setting [0-15] */
    uint dfeGAIN2;   /**< DFE Gain Tap 2 strength setting [0-15] */


    BOOL fixed_dc;    /**< When set serdes_dfe_tune will cause dfe_tuning to execute with fixed DC */
    BOOL fixed_lf;    /**< When set serdes_dfe_tune will cause dfe_tuning to execute with fixed LF */
    BOOL fixed_hf;    /**< When set serdes_dfe_tune will cause dfe_tuning to execute with fixed HF */
    BOOL fixed_bw;    /**< When set serdes_dfe_tune will cause dfe_tuning to execute with fixed BW (New with 0x104D) */
    /* If fixed_XX is set then the current XX setting of the struct will be loaded before */
    /* dfe_tuning is executed. */

    /* ***NOTE: Seeded tuning feature was added to SerDes firmware revision 0x1046 *** */
    BOOL seeded_dc;   /**< When set serdes_dfe_tune will cause dfe_tuning to execute with seeded DC */
    BOOL seeded_lf;   /**< When set serdes_dfe_tune will cause dfe_tuning to execute with seeded LF (Ignored with 0x104D+) */
    BOOL seeded_hf;   /**< When set serdes_dfe_tune will cause dfe_tuning to execute with seeded HF (Ignored with 0x104D+) */
    uint seedDC;      /**< Seed for DC-Restore value 0-255 */
    uint seedLF;      /**< Seed for CTLE Low-Frequency setting 0-15 (Ignored with 0x104D+) */
    uint seedHF;      /**< Seed for CTLE High-Frequency setting 0-15 (Ignored with 0x104D+) */

    uint dfe_disable; /**< When set serdes_dfe_tune will cause dfe_tuning to not tune dfeTaps 2-N */

    BOOL bw_tune_en;  /**< When TRUE serdes_dfe_tune will enable auto-tuning of the BW */

    Avago_serdes_dfe_tune_mode_t tune_mode; /* Select dfe_tuning mode to be execute by serdes_dfe_tune */

    int  vos[10];     /**< Data & Test sample levels 0-255 */
    uint vosMID[10];  /**< DC offset corrected mid points 0-255, *Read-only */

#if AAPL_ENABLE_FLOAT_USAGE
    float dfeTAP1;    /**< DFE Tap 1 setting -127.0-127.0; *Read-only; */
#else
    int   dfeTAP1;    /**< DFE Tap 1 setting [-127..127]; *Read-only; */
#endif

#define AVAGO_DFE_TAP_COUNT (13-1)

    int   dfeTAP[AVAGO_DFE_TAP_COUNT];/**< DFE Tap 2-N setting -15-15 */

    int  dataLEV[8];  /**< Data sampler test levels -127-127, *Read-only */
    int  testLEV[8];  /**< Test sampler test levels -127-127, *Read-only */
    int  eyeHeights[6]; /**< PAM4 eye heights */
    int  vernierDelay[11]; /**< Vernier delay settings *Read-only */

    uint dwell_bits;  /**< Number of bits received per error count up to 2**32-1 */
    uint error_threshold;/**< Threshold value used to establish dataLEV and testLEV */

    uint dfeGAIN_min; /**< Min setting for dfeGAIN */
    uint dfeGAIN_max; /**< Max setting for dfeGAIN */

    BOOL dfe_tap_disable[AVAGO_DFE_TAP_COUNT+1]; /**< Set to TRUE to disable tuning of DFE tap N */

    int rxFFE[7];     /**< Rx FFE tap settings */

    uint state;       /**< State of the DFE tuning flow, *Read-only */
    uint status;      /**< Status of the DFE tuning code; *Read-only */

} Avago_serdes_dfe_state_t;

EXT void  avago_serdes_get_dfe_state(   Aapl_t *aapl, uint sbus_addr, Avago_serdes_dfe_state_t *dfe_state);
EXT void  avago_serdes_print_dfe(       Aapl_t *aapl, uint sbus_addr, Avago_serdes_dfe_state_t *dfe_state, BOOL single_line);
EXT char *avago_serdes_dfe_state_to_str(Aapl_t *aapl, uint sbus_addr, Avago_serdes_dfe_state_t *dfe_state, BOOL single_line, BOOL header);
EXT int avago_serdes_get_dfe_status(Aapl_t* aapl, uint sbus_addr);

EXT int  avago_parallel_serdes_get_dfe_status(      Aapl_t *aapl, Avago_addr_t *addr_list);
EXT int  avago_parallel_serdes_interpret_dfe_status(Aapl_t *aapl, Avago_addr_t *addr_list);
EXT void avago_parallel_serdes_dfe_tune(            Aapl_t *aapl, Avago_addr_t *addr_list, Avago_serdes_dfe_state_t *mode_control);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT int avago_serdes_interpret_dfe_status(Aapl_t *aapl, uint sbus_addr);
#endif




/* START: DFE functions and structs */
typedef struct
{
    BOOL tuning_initiated;                        /**< This flag will be TRUE when this serdes becomes part of parallel tuning */
    int timeout_count;                            /**< Its a user configurable variable which will decide tuning timeout count value. */
                                                  /** This will be updated everytime poll function is called. */
}Avago_active_serdes_info;

typedef struct
{
    uint addr;
    BOOL ei_detect;                               /**< This flag will be TRUE when EI is detected for given serdes */
    BOOL rx_enable;                               /**< This flag will be TRUE when rx is enabled for given serdes */
    Avago_active_serdes_info active;              /**< This structure shows information whether the serdes is a part of parallel tuning */
    BOOL tuning_completed;                        /**< This flag will be TRUE when tuning gets successfully completed for given serdes */
    BOOL tuning_blocked;                          /**< This flag will be TRUE when tuning is blocked either due to EI detect or Rx turned off */
    BOOL tuning_failed;                           /**< This flag will be TRUE when tuning is failed due to tuning timeout */
    uint los_count;                               /**< Count to consider tuning fail when EI is detected for long time. */
                                                  /**< This count will be updated everytime when loss of signal is detected. */
}Avago_serdes_batch_tune_status;

typedef struct
{
    Avago_serdes_batch_tune_status *serdes_status;
    int total_serdes_count;                       /**< Maximum total number of serdes to be tuned */
    int active_tuning_count;                      /**< track serdes count that are currently part of parallel tuning */
    int completed_tuning_count;                   /**< track serdes count on which tuning is completed */
    int blocked_tuning_count;                     /**< track serdes count which are blocked either due to EI detect or Rx turned off */
    int failed_tuning_count;                      /**< track serdes count for which timeout occured */
    int maximum_serdes_tune_limit;                /**< maximum how many serdes can be tuned parellely. */
}Avago_serdes_batch_tune_work_t;

EXT int avago_serdes_dfe_batch_tune_launch(Aapl_t *aapl, Avago_serdes_dfe_state_t *dfe_state,
                                                                       int addr_cnt, uint *addrs, int limit, int timeout_count,
                                                                       Avago_serdes_batch_tune_work_t* tuning_status);

EXT int avago_serdes_dfe_batch_tune_poll(Aapl_t *aapl, Avago_serdes_dfe_state_t *dfe_state, Avago_serdes_batch_tune_work_t* tuning_status);

/**@brief Controls which Rx DFE settings are updated. */
typedef enum
{
    AVAGO_DFE_MODE_CTLE,          /**< Update DC,LF,HF,BW */
    AVAGO_DFE_MODE_DFE,           /**< Update dfe* */
    AVAGO_DFE_MODE_VOS,           /**< Update vos */
    AVAGO_DFE_MODE_FFE,           /**< Update PRE1, PRE2, POST1 */
    AVAGO_DFE_MODE_CTLE_DFE,      /**< Update DC,LF,HF,BW,dfeTap* */
    AVAGO_DFE_MODE_CTLE_DFE_VOS,  /**< Update DC,LF,HF,BW,dfe*,vos */
    AVAGO_DFE_MODE_PARAM,         /**< Update Parameters */
    AVAGO_DFE_MODE_TESTLEV,       /**< Update testLEV, used by eye plotting */
    AVAGO_DFE_MODE_DATALEV,       /**< Update dataLEV, used by eye plotting */
    AVAGO_DFE_MODE_VERNIER,       /**< Update vernier values */
    AVAGO_DFE_MODE_ALL            /**< Update entire struct */
} Avago_serdes_dfe_mode_t;

typedef enum
{
    AVAGO_SERDES_DFE_COMPLETE      = 0x00,    /* DFE Complete */
    AVAGO_SERDES_ICAL_IN_PROGRESS  = 0x01,    /* iCal is in Progress */
    AVAGO_SERDES_PCAL_IN_PROGRESS  = 0x02,    /* pCal is in progress */
    AVAGO_SERDES_VOS_IN_PROGRESS   = 0x04,    /* vos is in progress */
    AVAGO_SERDES_NO_REFCLK_TUNING  = 0x08,    /* No REFCLK Tuning Flag */
    AVAGO_SERDES_ICAL_SCHEDULED    = 0x10,    /* iCal is scheduled to run */
    AVAGO_SERDES_PCAL_SCHEDULED    = 0x20,    /* pCal is scheduled to run */
    AVAGO_SERDES_DFE_IS_RUNNING    = 0x37,    /* Checking for bits 0, 1, 2, 4, 5. */
    AVAGO_SERDES_ADAPTIVE_PCAL     = 0x40,    /* Adaptive pCal */
    AVAGO_SERDES_VOS_DONE          = 0x80,    /* VOS Done */
    AVAGO_SERDES_PR_MODE           = 0x100,   /* PR MODE: 0-PR0/NRZ 1-PR1 */
    AVAGO_SERDES_DFE_LOS           = 0x200    /* says if there was a loss of signal during tuning which caused the tuning not to complete. If this bit is high you can look at bits [4-5] to tell if the loss of signal happened during an iCal or a pCal. */
} Avago_serdes_dfe_status_t;

EXT Avago_serdes_dfe_state_t *avago_serdes_dfe_state_construct(Aapl_t *aapl);
EXT void avago_serdes_dfe_state_destruct(Aapl_t *aapl, Avago_serdes_dfe_state_t *dfe_state);

#if AAPL_ENABLE_FILE_IO
EXT void avago_write_dfe_state(FILE *file, const Avago_serdes_dfe_state_t *dfe);
#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT void avago_update_dfe_field(Avago_serdes_dfe_state_t *dfe, const char *name, int value, const char *str_val);
#endif
#endif

/* Get or Set the current DFE state. */
/* DFE_MODE is used to control how much of the DFE_STATE struct to get or set. */
/* do_write is used to control whether to set (TRUE) or get (FALSE). */
EXT void avago_serdes_dfe_state_ext(Aapl_t *aapl, uint sbus_addr,
                                    Avago_serdes_dfe_state_t *dfe_state,
                                    Avago_serdes_dfe_mode_t mode, BOOL do_write);

EXT void avago_serdes_set_dfe_state(Aapl_t *aapl, uint sbus_addr,
                                    Avago_serdes_dfe_state_t *dfe_state);
EXT void avago_serdes_get_dfe_state_ext(Aapl_t *aapl, uint sbus_addr,
                                        Avago_serdes_dfe_state_t *dfe_state,
                                        Avago_serdes_dfe_mode_t mode);
EXT void avago_serdes_set_dfe_state_ext(Aapl_t *aapl, uint sbus_addr,
                                        Avago_serdes_dfe_state_t *dfe_state,
                                        Avago_serdes_dfe_mode_t mode);

EXT int avago_serdes_set_dfe_tap1(Aapl_t *aapl, uint sbus_addr, int tap1);

/* Print out the current DFE state in a standard format */

/* Run/Halt DFE tuning on a serdes based on tune_mode */
EXT void avago_serdes_dfe_tune(Aapl_t *aapl, uint sbus_addr, Avago_serdes_dfe_state_t *mode_control);

EXT int avago_serdes_dfe_batch_tune(Aapl_t *aapl, Avago_serdes_dfe_state_t *dfe, int addr_cnt, uint *addrs,
                                    int start_delay, int loop_delay, int timeout);

#ifdef SWIG
EXT void avago_serdes_set_dfeTAP(Avago_serdes_dfe_state_t *ptr, int val, int bit) ;
EXT void avago_serdes_set_vosVAL(Avago_serdes_dfe_state_t *ptr, int val, int bit) ;
#endif

/* END: DFE status functions and structs */
/*////////////////////////////////////////////////////////////////////////////// */


#endif /* AVAGO_SERDES_DFE_H_ */




