/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserializer) slices on ASIC SBus rings; all functions exported */
/* from this directory. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SerDes PMD/KR functions. */

#ifndef AVAGO_SERDES_PMD_H_
#define AVAGO_SERDES_PMD_H_

/*////////////////////////////////////////////////////////////////////////////// */
/* START: KR functions and structs */

/**@brief Controls which Rx DFE settings are updated. */
typedef enum
{
    AVAGO_PMD_RESTART,        /**< Reset the PMD training */
    AVAGO_PMD_BYPASS,         /**< Set Tx Eq to Initialize and start sending core data */
    AVAGO_PMD_TRAIN           /**< Run full PMD training */
} Avago_serdes_pmd_train_mode_t;

/**@brief Controls which Rx DFE settings are updated. */
typedef enum
{
    AVAGO_PMD_FC16G,         /**< PRBS11 - x^11 + x^9 + 1 */
    AVAGO_PMD_CL72,          /**< PRBS11 - x^11 + x^9 + 1 */
    AVAGO_PMD_CL84,          /**< PRBS11 - x^11 + x^9 + 1 */
    AVAGO_PMD_CL92           /**< PRBS11 - sequence based on lane config */
} Avago_serdes_pmd_clause_t;

/**@brief Link Training Configuration struct. */
typedef struct
{
  Avago_serdes_pmd_train_mode_t train_mode;/**< Select pmd link training mode to execute by serdes_pmd_train */

  uint sbus_addr;        /**< SBus Rx Address of SerDes to run training on */
                         /**< When asymmetric = TRUE this is Rx portion of the duplex link */
  uint asym_tx;          /**< SBus Rx Address of SerDes that is the Tx portion of the duplex link, ignored if asymmetric_mode = FALSE */

  uint initial_pre; /**< Number of pre-cursor DEC commands to issue after PRESET & before DFE tuning (max of 15) */
  uint initial_post;/**< Number of post-cursor DEC commands to issue after PRESET & before DFE tuning (max of 15) */

  BOOL disable_txeq_adj; /**< Disables requesting TxEq adjustments */
  BOOL asymmetric_mode;  /**< When enabled o_core_status & i_core_cntl used to transfer control/status */
                         /**<  channels to another SerDes that is the partner to form the duplex link */
  BOOL invert_requests;  /**< Invert requests made to remote Tx, (DEC -> INC, INC -> DEC) */

  BOOL reset_parameters;    /**< Reset any previously configured parameters, ie. clause,lane,FEC* */

  BOOL disable_timeout;  /**< Disables the timeout timer */

  Avago_serdes_pmd_clause_t clause; /**< CL72,CL84,CL92 */
  uint                        lane; /**< When clause is set to CL92, defines which PMD lane this SerDes is */
  uint                   prbs_seed; /**< When non-zero overrides the default PRBS seed value */

  /* FC specific configs - Ingored unless clause == FC16G */
  BOOL  TT_FECreq;      /**< Set to transmit FEC request in TT training frames. */
  BOOL  TT_FECcap;      /**< Set to transmit FEC capability in TT training frames. */
  BOOL  TT_TF;          /**< Set to transmit Fixed TxEq in TT training frames. */

} Avago_serdes_pmd_config_t;

EXT Avago_serdes_pmd_config_t *avago_serdes_pmd_config_construct(Aapl_t *aapl);
EXT void avago_serdes_pmd_config_destruct( Aapl_t *aapl, Avago_serdes_pmd_config_t *pmd_config);
EXT void avago_serdes_pmd_train(Aapl_t *aapl, Avago_serdes_pmd_config_t *mode_control);
EXT int  avago_serdes_pmd_status(Aapl_t *aapl, uint addr);

/**@brief Tap request & response tracking struct */
typedef struct
{
    uint inc; /**< Number of inc requests */
    uint dec; /**< Number of dec requests */
    uint max; /**< Number of max responses */
    uint min; /**< Number of min responses */
} Avago_serdes_pmd_request_t;

/**@brief Statistics for a given transmitter (local or remote) */
typedef struct
{
    uint  preset;                 /**< Number of preset requests */
    uint  initialize;             /**< Number of initialize requests */
    Avago_serdes_pmd_request_t *tap[3]; /**< Stats for taps (pre,cursor,post) */
} Avago_serdes_pmd_tap_stats_t;

/**@brief Link Training Debug Information */
typedef struct
{
    BOOL reset; /**< Query statistics and then clear them (Note: also done when training is started) */

    uint rx_metric;              /**< RxEq metric */

    Avago_serdes_pmd_tap_stats_t *lcl;    /**< Stats for change requests done by local training code */
    Avago_serdes_pmd_tap_stats_t *remote; /**< Stats for change requests done by remote */

    uint last_remote_request[8]; /**< Last 8 requests sent by remote partner */
    uint last_local_request;     /**< Last request made by local training logic */

} Avago_serdes_pmd_debug_t;

EXT Avago_serdes_pmd_debug_t *avago_serdes_pmd_debug_construct(Aapl_t *aapl);
EXT void avago_serdes_pmd_debug_destruct( Aapl_t *aapl, Avago_serdes_pmd_debug_t *pmd_debug);

/* Run/Halt PMD link training  on a serdes based on tune_mode */
EXT void avago_serdes_pmd_debug(Aapl_t *aapl, uint sbus_addr, Avago_serdes_pmd_debug_t *pmd_debug);
EXT void avago_serdes_pmd_debug_print(Aapl_t *aapl, Avago_serdes_pmd_debug_t *pmd_debug);
EXT void avago_serdes_pmd_debug_print_repeat(Aapl_t *aapl, Avago_serdes_pmd_debug_t * pmd_debug);

EXT const char *aapl_pmd_clause_to_str    (Avago_serdes_pmd_clause_t     val);
EXT const char *aapl_pmd_train_mode_to_str(Avago_serdes_pmd_train_mode_t val);
EXT BOOL aapl_str_to_pmd_clause(    const char *name, Avago_serdes_pmd_clause_t     *out);
EXT BOOL aapl_str_to_pmd_train_mode(const char *name, Avago_serdes_pmd_train_mode_t *out);

#endif /* AVAGO_SERDES_PMD_H_ */
