/*
 * ? 2020 Alphawave IP Inc.
 */

/**
 * @brief Alphawave AlphaCORE100 C API functions.
 *
 * @file aw_alphacore.h
 *
 * @version 1.0.7
 */


#ifndef AW_ALPHACORE_H
#define AW_ALPHACORE_H

#include <stdint.h>

// include BF Private headers
#include "bf_alphacore.h"

#if 0
#include "aw_driver_sim.h"
#include "aw_alphacore_csr_defines.h"
#include "aw_alphacore_vfield_defines.h"
#include "aw_alphacore_ip_defines.h"
#include "aw_alphacore_atest_defines.h"
#endif //0

#define CHECK(x) do { int err = (x); if (err) return err; } while (0)
#define AW_MAX(a,b) (((a)>(b))?(a):(b))
#define AW_MIN(a,b) (((a)<(b))?(a):(b))
#define PI 3.14159265


#if 0
///**
// * Forward Declare Access Struct.
// *
// * This must be defined in the driver file to access the CSR space of the
// * AlphaCORE100 IP
// *
// */
//typedef struct mss_access_s mss_access_t;

#ifndef __aw_err_code_enum
#define __aw_err_code_enum
/**
 * typedef aw_err_code_t  - Alphawave MSS Error Codes Enum
 */
typedef enum aw_err_code_e {
    AW_ERR_CODE_NONE = 0,              /**< function success (no error) */
    AW_ERR_CODE_POLL_TIMEOUT = 1,      /**< function containing a poll did not successfully complete */
    AW_ERR_CODE_INVALID_ARG_VALUE = 2, /**< invalid function argument value */
    AW_ERR_CODE_WRITE_FAILURE = 3,     /**< returned by pmd_write_field if unsuccessful */
    AW_ERR_CODE_READ_FAILURE = 4,      /**< returned by pmd_read_field if unsuccessful */
    AW_ERR_CODE_BAD_STATE = 5,         /**< if unexpected values occurred, usually within *_get functions */
    AW_ERR_CODE_FUNC_FAILURE = 6,      /**< function did not complete successfully */
    AW_ERR_CODE_CHECK_FAILURE = 7      /**< function did not return the expected value */
} aw_err_code_t;
#endif

#define NULL_CHECK(x) do { if(x == NULL) return AW_ERR_CODE_INVALID_ARG_VALUE; } while (0)

typedef struct vga_opt_s {
    /**
     *        'en': VGA cap adaptation enable
     *              0 - VGA cap adapt Disabled
     *              1 - VGA cap adapt Enabled
     */
    uint32_t en;
    /**
     *        'vga_cap': VGA cap code ranges from 0-3
     */
    uint32_t vga_cap;
    /**
     *        'use_custom_takeover_ratio': 0 - Use the custom takeover ratio, as specified by 'takeover ratio'
     *                                     1 - Use takeover ratio hardcoded in FW
     */
    uint32_t use_custom_takeover_ratio;
    /**
     *        'custom_takeover_ratio': Custom takeover ratio converted to a format that FW accepts
     */
    uint32_t custom_takeover_ratio;
    /**
     *        'custom_nyq_mask': Unsigned 8-bit integer whose bits [7:0] represent whether the nyquist energies for [1p00..p125] will be used
     */
    uint32_t custom_nyq_mask;

} vga_opt_t;

/** AN NEWDEF return helper struct
 */
typedef struct aw_an_newdef_s {
    uint32_t lane_num_u;
    uint32_t lt_sel_u;
    uint32_t pam_u;
    uint32_t timer_sel_u;
} aw_an_newdef_t;

typedef struct tx_hbridge_s {
    uint32_t msb; /**< 'msb': adjust swing for Tx output */
    uint32_t lsb; /**< adjust inner eye level */
    uint32_t bias_adj; /**< afe tx driver h-bridge bias adj */
    uint32_t rlm_ovr; /**< disable auto RLM tracking */
    uint8_t bias_adj_en; /**< Bit, indicating that bias adjust was requested */
    uint8_t rlm_ovr_en; /**< Bit, indicating that rlm_ovr was requested */
} tx_hbridge_t;

/** AN NEWDEF return struct
 */
typedef struct aw_an_spec_s {
    uint32_t an_def_spec_width[28];    /**< array of 28 elements, containing readout values of eth_anlt_width_reg*[an_def_spec_width_*r] */
    uint32_t an_def_spec_rate[28];     /**< array of 28 elements, containing readout values of eth_anlt_rate_reg*[an_def_spec_rate_*r] */

    uint32_t an_def_an_rate;        /**< containing readout value of eth_an_pma_def_reg1[an_def_an_rate] */

    aw_an_newdef_t newdef1;         /**< containing readout values of eth_an_newdef_reg*[newdef1_*_u] */
    aw_an_newdef_t newdef2;         /**< containing readout values of eth_an_newdef_reg*[newdef2_*_u] */
    aw_an_newdef_t newdef3;         /**< containing readout values of eth_an_newdef_reg*[newdef3_*_u] */
} aw_an_spec_t;

typedef struct aw_lt_status_s {
    uint32_t lt_running; /**< 0 = not running, 1 = running */
    uint32_t lt_done; /**< 0 = not done, 1 = done */
    uint32_t lt_failure; /**< 0 - link training no fail, 1 - link training fail */
    uint32_t lt_rx_ready; /**< 0 - local RX / remote TX not trained, 1 - local RX / remote TX trained */
} aw_lt_status_t;

typedef enum aw_refclk_term_mode_e {
    AW_RC_HI_Z = 0, /**< REF CLK high impedance */
    AW_RC_R50_SE = 1, /**< REF CLK 50 ohms per leg single ended to ground */
    AW_RC_R100_DF = 2, /**< REF CLK 100 ohms differential */
    // UNUSED = 3, // here for clarity
} aw_refclk_term_mode_t;

typedef enum aw_acc_term_mode_e {
    AW_ACC_HI_Z = 0,
    AW_ACC_TERM_VSS_AC = 1, /**< termination to vss, onchip AC coupling */
    AW_ACC_TERM_FL_AC = 2, /**< floating termination, onchip AC coupling */
    // RES1 = 3, // Reserved
    // RES2 = 4, // Reserved
    AW_ACC_TERM_VSS_DC = 5, /**< termination to vss, DC coupled */
    AW_ACC_TERM_FL_DC = 6, /**< floating termination to vss, DC coupled */
    // RES3 = 7, // Reserved
} aw_acc_term_mode_t;

typedef enum aw_force_sigdet_mode_e {
    /**
     * Will set force_invalid=1 and force_valid=0 will force signal detect
     * value to 0, regardless of RX data presence
     */
    AW_SIGDET_FORCE0 = 0,

    /**
     * Will setting force_valid=1 and force_invalid=0 will forces signal
     * detect value to 1, regardless of RX data presence
     */
    AW_SIGDET_FORCE1 = 1,

    /**
     * setting force_valid=0 and force_invalid=0 so signal detect value
     * will not be forced. Signal detect is now dependent on RX data presence
     */
    AW_SIGDET_NORM = 2,
} aw_force_sigdet_mode_t;
typedef enum aw_txfir_cfg_taps_e {
    AW_CM3 = 0, /**< pre-cursor 3 */
    AW_CM2 = 1, /**< pre-cursor 2 */
    AW_CM1 = 2, /**< pre-cursor 1 */
    AW_C0  = 3, /**< max elements */
    AW_C1  = 4, /**< post-cursor 1 */
    AW_TXFIR_MAX_TAPS = 5
} aw_txfir_cfg_taps_t;

typedef struct aw_txfir_config_s {
    uint32_t CM3; /**< c(-3) value (pre-cursor 3), up to 4 max, 3b */
    uint32_t CM2; /**< c(-2) value  (pre-cursor 2), up to 7 max, 3b */
    uint32_t CM1; /**< c(-1) value  (pre-cursor 1), up to 24 max, 6b */
    uint32_t C0;  /**< c(0) value  (main cursor), up to 60 max, 6b */
    uint32_t C1;  /**< c(1) value  (post-cursor 1), up to 24 max, 6b */
    uint32_t main_or_max; /**< 0 - c0 is main cursor, 1 - c0 is max elements */
} aw_txfir_config_t;

typedef struct aw_analog_loopback_txfir_config_s {
    uint32_t nes_post1; /**< c(1) value (post-cursor 1), up to 15 max, 4b */
    uint32_t nes_c0;    /**< c(0) value, up to 15 max, 4b */
} aw_analog_loopback_txfir_config_t;


/**
 * State Request Struct
 *
 * Same for TX and RX
 */
typedef enum aw_state_rate_e {
  AW_NRZ_1p25_2p5 = 0, /**< 1.25/2.5 Gbps NRZ Auto-Neg */
  AW_NRZ_10p3125 = 1, /**< 10.3125 Gbps NRZ (10G) */
  AW_NRZ_25p78125 = 2, /**< 25.78125 Gbps NRZ (25G) */
  AW_NRZ_26p5625 = 3, /**< 26.5625 Gbps NRZ (26G AUI) */
  AW_PAM4_53p125 = 4, /**< 53.125 Gbps PAM4 (50G) */
  AW_NRZ_53p125 = 5, /**< 53.125 Gbps NRZ (50G) */
  AW_PAM4_106p25 = 6, /**< 106.25 Gbps PAM4 (100G) */
  AW_MAX_RATES = 7,
} aw_state_rate_t;

/**
 * Link Training Mode Struct
 *
 * Same for TX and RX
 */
typedef enum aw_training_mode_e {
  AW_DISABLED = 0,
  AW_CL72 = 1,
  AW_CL92 = 2,
  AW_CL136 = 3,
  AW_CL162 = 4,
  AW_MAX_CFG = 5,
} aw_training_mode_t;

///** AW is reserving this struct and it's corresponding function in this
// * release. If there is a usecase for it, it will be updated in a future
// * release. If not, it will be removed.
// */
//typedef struct lane_cfg_s {
//    RESERVED
// } lane_cfg_t;

typedef enum aw_pll_pstatus_e {
    AW_PWR_DOWN = 0, /**< PLL power down */
    AW_PWR_UP = 1, /**< PLL power up */
    AW_ST_CHNG = 2 /**< PLL State Changing */
} aw_pll_pstatus_t;


/**
 * Return adapted FFE Tap values in the following type. Index 0
 * corresponds Tap 0, index 1 to tap 1 and so on all the way to tap 20
 */
typedef uint32_t aw_ffe_t[AW_FFE_NUM_TAPS];

typedef struct aw_thresholds_s {
    /**
     * Return adapted threshold eh settings for specified branch.
     */
    uint32_t eh;
    /**
     * Return adapted threshold ez settings for specified branch.
     */
    uint32_t ez;
    /**
     * Return adapted threshold el settings for specified branch.
     */
    uint32_t el;
    /**
     * Return adapted threshold lower target adaptation settings for specified branch.
     */
    uint32_t thres_low;
    /**
     * Return adapted threshold upper target adaptation settings for specified branch.
     */
    uint32_t thres_hi;
} aw_thresholds_t;

/**
 * Slicer Levels Struct
 */
typedef struct aw_slicers_s {
    /**
     *        'slicer_eh3': Return adapted slicer eh3 settings for specified branch.
     */
    uint32_t eh3;
    /**
     *        'slicer_eh1': Return adapted slicer eh1 settings for specified branch.
     */
    uint32_t eh1;
    /**
     *        'slicer_el1': Return adapted slicer el1 settings for specified branch.
     */
    uint32_t el1;
    /**
     *        'slicer_el3': Return adapted slicer el3 settings for specified branch.
     */
    uint32_t el3;
} aw_slicers_t;

/** all uint32_t are signed, need to include custom precisions layout */
typedef struct aw_dsp_param_s {
    /** Return adapted DC offset value for specified branch */
    uint32_t dc_offset;
    /** Return adapted DFE value for specified branch. */
    uint32_t dfe;
    aw_ffe_t ffe;
    aw_ffe_t pre_ffe;
    aw_ffe_t pre_dfe_ffe;
    aw_thresholds_t thresholds;
    aw_slicers_t slicers;
} aw_dsp_param_t;

typedef struct aw_dcdiq_data_s {
    /**
     *   Returns calibrated duty cycle code for d0 clock
     */
    uint32_t d0;
    /**
     *   Returns calibrated duty cycle code for d90 clock
     */
    uint32_t d90;
    /**
     *   Returns calibrated IQ phase code between d0 (I) and d90 (Q) clock
     */
    uint32_t iq;
} aw_dcdiq_data_t;


typedef struct aw_afe_data_s {
    /**
     * Automatically set based on baudrate. 1 - For 106/112Gbps datarates. 0 - All other datarates.
     */
    uint32_t ctle_rate;
    /**
     * Nyquist boost setting ranging from codes 0 to 12.
     */
    uint32_t ctle_boost;
    /**
     * Coarse VGA code, automatically adapted during foreground adapt to ensure signal going into ADC is within 80-90% full scale. Range is from 0-31.
     */
    uint32_t vga_coarse;
    /**
     * Fine VGA code, automatically adapted during background due to voltage and temperature fluctuations and ensures signal going into ADC is within 80-90% full scale. Range is from 0-63.
     */
    uint32_t vga_fine;
    /**
     * VGA offset code, automatically adapted during foreground, correcting for residual offset at the output of the VGA
     */
    uint32_t vga_offset;
} aw_afe_data_t;

/**
 * Master FSM Diagnostics Struct
 *
 * NOTE: this struct may change
 */
typedef struct aw_uc_diag_regs_s {
    uint32_t rxmfsm_rate_cur;
    uint32_t rxmfsm_rate_new;
    uint32_t rxmfsm_width_cur;
    uint32_t rxmfsm_width_new;
    uint32_t rxmfsm_rxdisable;
    uint32_t rxmfsm_req;
    uint32_t rxmfsm_power_cur;
    uint32_t rxmfsm_power_new;
    uint32_t rxmfsm_pam_cur;
    uint32_t rxmfsm_pam_new;
    uint32_t rxmfsm_pam_ctrl_cur;
    uint32_t rxmfsm_pam_ctrl_new;
    uint32_t rxmfsm_instr_num;
    uint32_t rxmfsm_state;
    uint32_t rxiffsm_state;
    uint32_t rx_log0;
    uint32_t rx_log1;
    uint32_t rx_log2;
    uint32_t rx_log3;
    uint32_t rx_log4;
    uint32_t rx_log5;
    uint32_t rx_log6;
    uint32_t rx_log7;
    uint32_t rx_log8;
    uint32_t txmfsm_rate_cur;
    uint32_t txmfsm_rate_new;
    uint32_t txmfsm_width_cur;
    uint32_t txmfsm_width_new;
    uint32_t txmfsm_power_cur;
    uint32_t txmfsm_power_new;
    uint32_t txmfsm_pam_cur;
    uint32_t txmfsm_pam_new;
    uint32_t txmfsm_instr_num;
    uint32_t txmfsm_req;
    uint32_t txmfsm_state;
    uint32_t txiffsm_state;
    uint32_t tx_log0;
    uint32_t tx_log1;
    uint32_t tx_log2;
    uint32_t tx_log3;
    uint32_t tx_log4;
    uint32_t tx_log5;
    uint32_t tx_log6;
    uint32_t tx_log7;
    uint32_t tx_log8;
    uint32_t tx_log9;
    uint32_t cmnmfsm_rate_cur;
    uint32_t cmnmfsm_instr_num;
    uint32_t cmnmfsm_power_cur;
    uint32_t cmnmfsm_power_new;
    uint32_t cmnmfsm_rate_new;
    uint32_t cmnmfsm_req;
    uint32_t cmnmfsm_state;
    uint32_t cmniffsm_state;
    uint32_t cmn_log0;
    uint32_t cmn_log1;
    uint32_t cmn_log2;
    uint32_t cmn_log3;
    uint32_t cmn_log4;
    uint32_t cmn_log5;
    uint32_t cmn_log6;
    uint32_t cmn_log7;
    uint32_t cmn_log8;
    uint32_t cmn_log9;
} aw_uc_diag_regs_t;

typedef struct aw_ucode_s {
    uint32_t address;
    uint32_t value;
} aw_ucode_t;

typedef enum aw_bist_pattern_e {
    AW_PRBS7 = 0,
    AW_PRBS9 = 1,
    AW_PRBS11 = 2,
    AW_PRBS13 = 3,
    AW_PRBS15 = 4,
    AW_PRBS23 = 5,
    AW_PRBS31 = 6,
    AW_QPRBS13 = 7,
    AW_JP03A = 8,
    AW_JP03B = 9,
    AW_LINEARITY_PATTERN = 10,
    AW_USER_DEFINED_PATTERN = 11,
    AW_FULL_RATE_CLOCK = 12,
    AW_HALF_RATE_CLOCK = 13,
    AW_QUARTER_RATE_CLOCK = 14,
    AW_PATT_32_1S_32_0S = 15, /**< 32 1s and 32 0s repeating */
    AW_BIST_PATTERN_MAX = 16 /**< Always at the end */
} aw_bist_pattern_t;

typedef enum aw_bist_mode_e {
    AW_TIMER = 0, /**< will run for a number of defined cycles */
    AW_DWELL = 1, /**< wall clock mode, doing a test longer than 80ms, this needs to be used */
} aw_bist_mode_t;

typedef enum aw_eq_type_e {
    AW_EQ_FULL_DIR = 0, /**< Full EQ, Directional */
    AW_EQ_EVAL_DIR = 1, /**< Eval Only, Directional */
    AW_EQ_INIT_EVAL = 2, /**< Init Eval */
    AW_EQ_CLEAR_EVAL = 3, /**< Clear Eval */
    AW_EQ_FULL_FOM = 4, /**< Full EQ, FOM */
    AW_EQ_EVAL_FOM = 5 /**< Eval Only, FOM */
} aw_eq_type_t;

typedef struct aw_version_e {
    uint32_t version_major; /**< Major version info */
    uint32_t version_minor; /**< Minor version info */
    uint32_t version_patch; /**< Patch version info */
} aw_version_t;

typedef enum aw_pstate_e {
    AW_P0   = 0,
    AW_P0S  = 1,
    AW_P1   = 2,
    AW_P2   = 3,
    AW_PD   = 4,
    AW_L1_0 = 5,
    AW_L1_1 = 6,
    AW_L1_2 = 7
} aw_pstate_t;

typedef enum aw_cmn_pstate_e {
    AW_CMN_PD   = 0,
    AW_CMN_P0   = 1
} aw_cmn_pstate_t;

typedef enum aw_rx_roaming_mode_e {
    AW_NO_ROAMING = 0,  /**< ignores window select */
    AW_BANK1_ROAMING = 1, /**< ignores window select */
    AW_BANK2_ROAMING = 2, /**< ignores window select */
    AW_BANK1_BANK2_ROAMING = 3, /**< ignores window select */
    AW_RESET_DEFAULT_ROAMING_MODE = 4, /**< places mode and window selects into por default values */
    AW_MANUAL_WINDOW_SELECT = 5 /**< places mode and window selects into por default values */
} aw_rx_roaming_mode_t;

/**
 * ATEST ADC Temperature sensor struct
 */
typedef struct aw_adc_temp_s {
    float vc; /**< VC */
    float vb; /**< VB */
    float bg1; /**< Bandgap voltage 1 */
    float bg2; /**< Bandgap voltage 2 */
} aw_adc_temp_data_t;

/**
 * ATEST ADC Temperature sensor calibration struct
 */
typedef struct aw_adc_temp_calibration_s {
    aw_adc_temp_data_t data_1; /**< Measured ADC temperature voltages at known temperature 1 */
    aw_adc_temp_data_t data_2; /**< Measured ADC temperature voltages at known temperature 2 */
    float              temp_1; /**< Known temperature 1 */
    float              temp_2; /**< Known temperature 2 */
} aw_adc_temp_calibration_t;

/**
 * ATEST ADC Temperature calculation method 
 */
typedef enum aw_atest_adc_temp_method_e {
    AW_ATEST_ADC_1_POINT_SUPPLY_SENSITIVE_METHOD,     /**< 1 Point Supply Sensitive Method */
    AW_ATEST_ADC_1_POINT_SUPPLY_INSENSITIVE_METHOD,   /**< 1 Point Supply Inensitive Method */
    AW_ATEST_ADC_2_POINT_SUPPLY_SENSITIVE_METHOD,     /**< 2 Point Supply Sensitive Method */
    AW_ATEST_ADC_2_POINT_SUPPLY_INSENSITIVE_METHOD,   /**< 2 Point Supply Insensitive Method */
    AW_ATEST_ADC_MAX_METHOD
} aw_adc_temp_method_t;




//ANLT APIs
/**
 * Maps width encoded value to actual value
 *
 * Args:
 *    'width_encoded':  encoded width
 *
 *
 * Return:
      'width': decoded width
 *
 * Constraints:
 *     Null
 *
 * Typical Application Usage:
 *    Datarate calculation in aw_pmd_rx_check_bist
 */
static
uint32_t aw_width_decoder (uint32_t width_encoded);
#endif //0

/**
 * Sets CMN clock generator reference divider ratio
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'div': divider ratio
 *     101 to 111 unused
 *     101 - div32
 *     100 - div16
 *     011 - div8
 *     010 - div4
 *     001 - div2
 *     000 - div1

 * Return:
 *
 * Constraints:
 *     Null
 *
 * Typical Application Usage:
 *    Set this register to use faster LSREF.
 */
int aw_pmd_16ln_cmn_clkgen_refdiv_set(mss_access_t *mss, uint32_t div);
/**
 * Gets CMN clock generator reference divider ratio
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'div': divider ratio
 *     101 to 111 unused
 *     101 - div32
 *     100 - div16
 *     011 - div8
 *     010 - div4
 *     001 - div2
 *     000 - div1

 * Return:
 *
 * Constraints:
 *     Null
 *
 * Typical Application Usage:
 *     Null
 */
int aw_pmd_16ln_cmn_clkgen_refdiv_get(mss_access_t *mss, uint32_t * div);
/**
 * Sets the logical lane number of this physical lane
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'logical_lane': logical lane number
 *    'an_no_attached': set this to 1 if this lane is in a x1 link (only lane in the group)
 *
 * Return:
 *
 * Constraints:
 *    If in the case of running AN, must be called before lane resets are released.
 *
 * Typical Application Usage:
 *    Set this when configuring AN and/or LT.
 */
int aw_pmd_16ln_anlt_logical_lane_num_set (mss_access_t *mss, uint32_t logical_lane, uint32_t an_no_attached);

/**
 * Returns the logical lane number of this physical lane
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'logical_lane': logical lane number
 *    'an_no_attached': 1 if this lane is in a x1 link (only lane in the group)
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_logical_lane_num_get (mss_access_t *mss, uint32_t * logical_lane, uint32_t * an_no_attached);

/**
 * Sets the advertised ability during auto-negotiation
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'adv_ability':
 *        0 - 1000BASE-KX
 *        1 - 10GBASE-KX4
 *        2 - 10GBASE-KR
 *        3 - 40GBASE-KR4
 *        4 - 40GBASE-CR4
 *        5 - 100GBASE-CR10
 *        6 - 100GBASE-KP4
 *        7 - 100GBASE-KR4
 *        8 - 100GBASE-CR4
 *        9 - 25GBASE-K/CR-S
 *        10 - 25GBASE-K/CR
 *        11 - 2.5GBASE-KX
 *        12 - 5GBASE-KR
 *        13 - 50GBASE-K/CR
 *        14 - 100GBASE-K/CR2
 *        15 - 200GBASE-K/CR4
 *        16 - 100GBASE-K/CR1
 *        17 - 200GBASE-K/CR2
 *        18 - 400GBASE-K/CR4
 *  'fec_ability': FEC Capability as per IEEE specification
 *      
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Must be called before lane resets are released.
 *
 * Typical Application Usage:
 *    Set this when configuring AN.
 */
int aw_pmd_16ln_anlt_auto_neg_adv_ability_set (mss_access_t *mss, uint32_t *adv_ability, uint32_t *fec_ability, uint32_t nonce);

/**
 * Returns the advertised ability.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'adv_ability': advertised abilities, same format as described in auto_neg_adv_ability_set()
 *    'fec_ability': FEC abilities, same format as described in auto_neg_adv_ability_set()
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_auto_neg_adv_ability_get (mss_access_t *mss, uint32_t * adv_ability, uint32_t *fec_ability);
/**
 * Basic configurations for auto-negotiation
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'status_check_disable':
 *        bit 0 - disabled transition from AN_GOOD_CHECK to TX_DISABLE
 *        bit 1 - disabled transition from AN_GOOD to TX_DISABLE
 *    'next_page_en':
 *        0 - disable next page
 *        1 - enable next page
 *    'an_no_nonce_check': set to 1 for testing between lanes on the same PHY
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Must be called before lane resets are released.
 *
 * Typical Application Usage:
 *    Set this when configuring AN.
 */
int aw_pmd_16ln_anlt_auto_neg_config_set (mss_access_t *mss, uint32_t status_check_disable, uint32_t next_page_en, uint32_t an_no_nonce_check);

/**
 * Returns basic configurations for auto-negotiation
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'status_check_disable':
 *        bit 0 - disabled transition from AN_GOOD_CHECK to TX_DISABLE
 *        bit 1 - disabled transition from AN_GOOD to TX_DISABLE
 *    'next_page_en':
 *        0 - disable next page
 *        1 - enable next page
 *    'an_no_nonce_check': set to 1 for testing between lanes on the same PHY
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_auto_neg_config_get (mss_access_t *mss, uint32_t * status_check_disable, uint32_t * next_page_en, uint32_t * an_no_nonce_check);

/**
 * Starts auto-negotiation
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'start': AN will start once this is set to 1
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Must be called immediately after lane resets are released. Signal_detect must not be forced high when AN starts.
 *
 * Typical Application Usage:
 *    Start AN after AN has been configured.
 */
int aw_pmd_16ln_anlt_auto_neg_start_set (mss_access_t *mss, uint32_t start);

/**
 * Returns value of start auto-negotiation register
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'start':
 *       0 - AN not started
 *       1 - AN started
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_auto_neg_start_get (mss_access_t *mss, uint32_t * start);

/**
 * Returns status of auto-negotiation
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'link_good': will return 1 if AN is finished
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Use this function to check if AN has finished after it has been configured and started
 */
int aw_pmd_16ln_anlt_auto_neg_status_get (mss_access_t *mss, uint32_t * link_good);

/**
 * Returns status of auto-negotiation
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'no_consortium': Select for consortium mode 
 *                      0 - Using a Consortium technology 
 *                      1 - Not using a consortium technology
 *
 * Return:
 *    'an_result': will return the negotiated ability number
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Use this function to check if AN has negotiated to the right ability after it has been configured and started
 */
//bfn
//int aw_pmd_16ln_anlt_auto_neg_result_get (mss_access_t *mss, uint8_t no_consortium, uint32_t * an_result);
int aw_pmd_16ln_anlt_auto_neg_result_get (mss_access_t *mss, uint32_t * an_result);

/**
 * Returns the AN page received
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'an_mr_page_rx':
 *        0 - no new page has been received
 *        1 - a new page has been received
 *    'an_rx_link_code_word': received next page
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Call this API to read back received new page, after an_mr_page_rx indicates a new page has been received
 */
int aw_pmd_16ln_anlt_auto_neg_page_rx_get(mss_access_t *mss, uint32_t *an_mr_page_rx, uint64_t *an_rx_link_code_word);

/**
 * Set the next page during AN
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'an_tx_np': next page to be loaded.
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Call this API to load next page
 */
int aw_pmd_16ln_anlt_auto_neg_next_page_set(mss_access_t *mss, uint64_t an_tx_np);

/**
 * Get the register settings for all the AN definition related fields
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'newdef_get': struct for read outs. Please refer to struct aw_an_spec_t in this file for struct format.
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_auto_neg_newdef_get(mss_access_t *mss, aw_an_spec_t *newdef_get);

/**
 * Returns if RS-FEC is negotiated for ANLT
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'an_rs_fec_int_ena':
 *        0 - RS-FEC disabled
 *        1 - RS-FEC enabled
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Call this API to read back FEC enable/disable status
 */
int aw_pmd_16ln_anlt_auto_neg_rs_fec_int_ena_get (mss_access_t *mss,  uint32_t * an_rs_fec_int_ena);

/**
 * Specifies the OUI/CID for the ethernet consortium
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'np_expected_oui': OUI/CID
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_auto_neg_next_page_oui_compare_set(mss_access_t *mss, uint32_t np_expected_oui);

/**
 * Enables/disables link training
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en':
 *        0 - link training disabled
 *        1 - link training enabled
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    In the case of running AN without LT, disable link training.
 *    In the case of running LT, enable link training.
 */
int aw_pmd_16ln_anlt_link_training_en_set (mss_access_t *mss, uint32_t en);

/**
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'en':
 *        0 - link training disabled
 *        1 - link training enabled
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 *
 */
int aw_pmd_16ln_anlt_link_training_en_get (mss_access_t *mss, uint32_t * en);

/**
 *  LT sequences:
 *         Stage --------- EQ type ---- EQ EVAL/FULL ------------------------------------- Encoding/Modulation
 *      1. INITIALIZE ---- FOM -------- N/A ---------------------------------------------- N/A
 *      2. PRESET -------- FOM -------- lt_train_preset_eqtype --------------------------- lt_reg_training_sweep_mod
 *      3. THE REST ------ DIR -------- lt_train_mid_eqtype / lt_train_final_eqtype------- lt_reg_training_init_mod / lt_reg_training_mod
 *
 *      lt_train_iter_count determines total number of DIR EQ being run
 *      The DIR EQ calls are divided into 2 chunks:
 *        - <register name to find> sets the # of DIR EQs that run with modulation set by lt_reg_training_init_mod
 *        - The remaining # of DIR EQs will run with modulation set by lt_reg_training_mod
 *
 *      lt_train_final_eqtype sets the EQ type (EVAL or FULL) of the single last iteration of DIR EQ. All previous iterations of DIR EQ type is set by lt_train_mid_eqtype.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'width': same width settings as before LT
 *    'clause': clauses in the ethernet spec
 *        0 - reserved
 *        1 - 72 (10G)
 *        2 - 92 (25G)
 *        3 - 136 (50G)
 *        4 - 162 (100G)
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    In the case of running LT without AN, power up the lanes in the link, configure LT, and start LT.
 */
//bfn
//int aw_pmd_16ln_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause, uint32_t mod);
int aw_pmd_16ln_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause);

/**
 * Get the LT width and clause settings.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'width': same width settings as before LT
 *    'clause': clauses in the ethernet spec
 *        0 - reserved
 *        1 - 72 (10G)
 *        2 - 92 (25G)
 *        3 - 136 (50G)
 *        4 - 162 (100G)
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_link_training_config_get (mss_access_t *mss, uint32_t * width, uint32_t * clause);

/**
 * Set PRBS seed according to IEEE ethernet spec.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'width': same width settings as before LT
 *    'clause': clauses in the ethernet spec
 *        0 - reserved
 *        1 - 72 (10G)
 *        2 - 92 (25G)
 *        3 - 136 (50G)
 *        4 - 162 (100G)
 *    'logical_lane': logical lane number
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_link_training_prbs_seed_set (mss_access_t *mss, uint32_t clause, uint32_t logical_lane);

/**
 * Starts link training.
 *
 *  TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'start': set to 1 to start link training
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Configure link training before starting link training
 */
int aw_pmd_16ln_anlt_link_training_start_set (mss_access_t *mss, uint32_t start);

/**
 * Gets the status of whether link training has been started.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'start':
 *        0 - link training has not been started
 *        1 - link training has been started
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_link_training_start_get (mss_access_t *mss, uint32_t * start);

/**
 * Gets link training related status
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'lt_running':
 *        0 - LT is not running
 *        1 - LT is running
 *    'lt_done':
 *        0 - LT is not done
 *        1 - LT is done
 *    'lt_training_failure':
 *        0 - LT has not failed
 *        1 - LT has failed
 *    'lt_rx_ready':
 *        0 - RX has not been trained during LT
 *        1 - RX has been trained during LT
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_link_training_status_get (mss_access_t *mss, uint32_t * lt_running, uint32_t * lt_done, uint32_t * lt_training_failure, uint32_t * lt_rx_ready);

/**
 * Gets link training related status
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'enable': enable or disable timeout check during LT. Only disable LT timeout check for debug purposes.
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_anlt_link_training_timeout_enable_set (mss_access_t *mss, uint32_t enable);

/**
 * Enable ANLT lane remapping
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'anlt_ctrl_map_en':
 *        0 - enable lane remapping
 *        1 - disable lane remapping
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 */
int aw_pmd_16ln_ctrl_map_en_set(mss_access_t *mss, uint32_t anlt_ctrl_map_en);


/**Set the termination mode of the reference clock input pads, depending on if the macro is at the end of the reference clock trace.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'lsrefbuf_term_mode': Enum type for aw_refclk_term_mode_t
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During PD.
 *    Rate State:   Before desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    In the scenario where multiple macros are sharing the same on-board refclk, set the termination to ground if the PHY macro that is physically furthest away from on-board refclk source. The other PHY macros should be set to high impedance.
 */
int aw_pmd_16ln_refclk_termination_set(mss_access_t *mss, aw_refclk_term_mode_t lsrefbuf_term_mode);

/**Get the termination mode of the reference clock input pads, depending on if the macro is at the end of the reference clock trace.
 *
 * CMN Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    AW Error Indicator Enum
 *    'lsrefbuf_term_mode': Enum type for aw_refclk_term_mode_t
 *
 * Constraints:
 *    Power State:  During PD.
 *    Rate State:   Before desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    In the scenario where multiple macros are sharing the same on-board refclk, set the termination to ground if the PHY macro that is physically furthest away from on-board refclk source. The other PHY macros should be set to high impedance.
 */
int aw_pmd_16ln_refclk_termination_get(mss_access_t *mss, aw_refclk_term_mode_t *lsrefbuf_term_mode);


/**Set the RX termination mode, depending on the channels used.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'acc_term_mode': aw_acc_term_mode_t enum setting the AFE On-Chip RX Termination.
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During PD.
 *    Rate State:   Before desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    RX termination setting changes depending on prescence of on-board coupling capacitor.
 */
int aw_pmd_16ln_rx_termination_set(mss_access_t *mss, aw_acc_term_mode_t acc_term_mode);

/**Set the RX termination mode, depending on the channels used.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    AW Error Indicator Enum
 *    'acc_term_mode': Return pointer enum for AFE On-Chip RX Termination value.
 *
 * Constraints:
 *    Power State:  During PD.
 *    Rate State:   Before desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    RX termination setting changes depending on prescence of on-board coupling capacitor.
 */
int aw_pmd_16ln_rx_termination_get(mss_access_t *mss, aw_acc_term_mode_t *acc_term_mode);

/**Force signal detect to user-specified value. If forced valid, CDR will attempt to lock to data, regardless of presence of RX Data. If forced invalid, CDR will lock to the reference clock.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'sigdet_mode': aw_force_sigdet_mode_t enum type
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Force signal detect is used in scenarios where electrically idle RX data is very noisy, typical in optical systems. Excessive noise during RX electrical idle can falsely trigger signal detect to enable the CDR during P0.
 */
int aw_pmd_16ln_force_signal_detect_config_set(mss_access_t *mss, aw_force_sigdet_mode_t sigdet_mode);

/**Return forced signal detect value.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    `lane`: lane number
 *
 * Return:
 *    'sigdet_mode': Return pointer to aw_force_sigdet_mode_t enum type
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to confirm current signal detect state.
 */
int aw_pmd_16ln_force_signal_detect_config_get(mss_access_t *mss, aw_force_sigdet_mode_t *sigdet_mode);


/**Set TX disable when TX electric idle is asserted.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'tx_disable':
 *        0 - enable TX
 *        1 - disable TX
 *
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to disable TX output, when the corresponding override is enabled.
 */
int aw_pmd_16ln_tx_disable_set(mss_access_t *mss, uint32_t tx_disable);

/**Return if TX disabled.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'tx_disable':
 *        0 - TX enabled
 *        1 - TX disabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current TX disable value.
 */
int aw_pmd_16ln_tx_disable_get(mss_access_t *mss, uint32_t *tx_disable);

// NOTE: these are commented out, and not included in the API spec, but AW is
//       reserving in case there is a future usecase
// int aw_pmd_16ln_rx_disable_pin_override_set(mss_access_t *mss, uint32_t override_enable);
// int aw_pmd_16ln_rx_disable_pin_override_get(mss_access_t *mss, uint32_t *override_enable);
// int aw_pmd_16ln_rx_disable_set(mss_access_t *mss, uint32_t rx_disable);
// int aw_pmd_16ln_rx_disable_get(mss_access_t *mss, uint32_t *rx_disable);
// ENDNOTE

/**Set value for each TX FIR tap setting. Sum of all the cursor values should be 60 at max.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'txfir_cfg': Struct containing all the TX FIR configuration settings
 *    'fir_ovr_enable': TX FIR AFE signal override enable
 *
 * Return:
 *    'poll_err': TX FIR acknowledge signal did not assert
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    When using PHY as a TX, call this function to adjust TX FIR tap weights.
 */
int aw_pmd_16ln_txfir_config_set(mss_access_t *mss, aw_txfir_config_t txfir_cfg, uint32_t fir_ovr_enable);

/**Return value for each TX tap setting written by txfir_config_set function.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'txfir_cfg': Return pointer to TX FIR config struct
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current FIR settings.
 */
int aw_pmd_16ln_txfir_config_get(mss_access_t *mss, aw_txfir_config_t *txfir_cfg);

/**Return mode (NRZ or PAM4), max tap range for each TX FIR tap, and the number of tap drivers that are on
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'max_tap_range_cm3': max tap range for pre cursor 3
 *    'max_tap_range_cm2': max tap range for pre cursor 2
 *    'max_tap_range_cm1': max tap range for pre cursor 1
 *    'max_tap_range_c0': max tap range for main cursor
 *    'max_tap_range_c1': max tap range for post cursor 1
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get return current TX FIR settings and its' limits.
 */
int aw_pmd_16ln_tx_tap_mode_get(uint32_t *max_rng_cm3, uint32_t *max_rng_cm2, uint32_t *max_rng_cm1, uint32_t *max_rng_c1, uint32_t *max_rng_c0);

/**Enable/disable PAM4 precoder for the given lane during ANLT.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'en': Set Precoder enabled or disabled
 *        0 - disable precoder
 *        1 - enable precoder
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    For links with high insertion loss, enabling precoder may help to lower BER.
 */
int aw_pmd_16ln_tx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en);

/**Return if the PAM4 precoder is enabled/disabled for the given lane.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'en': Return pointer to if the TX precoder is enabled
 *        0 - PAM4 precoder disabled
 *        1 - PAM4 precoder enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current TX precoder setting used during ANLT.
 */
int aw_pmd_16ln_tx_pam4_precoder_override_get(mss_access_t *mss, uint32_t *en);

/**Enable/disable TX PAM4 precoder for the given lane during ANLT.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'gray_en':
 *        0 - gray encoding disable
 *        1 - gray encoding enable
 *    'plusd_en':
 *        0 - plusd encoding disable
 *        1 - plusd encoding enable
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    For links with high insertion loss, enabling precoder may help to lower BER.
 */
int aw_pmd_16ln_tx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en);

/**Return if the TX PAM4 gray/plusd encoding is enabled/disabled for the given lane.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'gray_en':
 *        0 - gray encoding disabled
 *        1 - gray encoding enabled
 *    'plusd_en':
 *        0 - plusd encoding disabled
 *        1 - plusd encoding enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current Tx precoder setting.
 */
int aw_pmd_16ln_tx_pam4_precoder_enable_get(mss_access_t *mss, uint32_t *gray_en, uint32_t *plusd_en);

/**Enable/disable RX PAM4 gray encoding override for the given lane.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'en':
 *        0 - rx_demapper register will control the rx gray encoding mapping
 *        1 - rx_cntrl_reg2[rx_gray_ena_nt] register will control the rx gray encoding mapping
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    For links with high insertion loss, enabling precoder may help to lower BER.
 */
int aw_pmd_16ln_rx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en);

/**Return if the RX PAM4 gray encoding override is enabled/disabled for the given lane.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'en':
 *        0 - rx_demapper register will control the rx gray encoding mapping
 *        1 - rx_cntrl_reg2[rx_gray_ena_nt] register will control the rx gray encoding mapping
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current Rx precoder setting.
 */
int aw_pmd_16ln_rx_pam4_precoder_override_get(mss_access_t *mss, uint32_t *en);

/**Enable/disable RX PAM4 gray/plusd encoding for the given lane.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'gray_en':
 *        0 - gray encoding disable
 *        1 - gray encoding enable
 *    'plusd_en':
 *        0 - plusd encoding disable
 *        1 - plusd encoding enable
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    For links with high insertion loss, enabling precoder may help to lower BER.
 */
int aw_pmd_16ln_rx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en);

/**Return if the RX PAM4 gray/plusd encoding is enabled/disabled for the given lane.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'gray_en':
 *        0 - gray encoding disabled
 *        1 - gray encoding enabled
 *    'plusd_en':
 *        0 - plusd encoding disabled
 *        1 - plusd encoding enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current Rx precoder setting.
 */
int aw_pmd_16ln_rx_pam4_precoder_enable_get(mss_access_t *mss, uint32_t *gray_en, uint32_t *plusd_en);

/**Return if the remote far-end parallel (FEP) loopback is enabled/disabled.
 *
 * Deprecated, use aw_pmd_fep_clock_set & aw_pmd_fep_dat_set
 * 
 */
int aw_pmd_16ln_remote_loopback_set(mss_access_t *mss, uint32_t remote_loopback_enable);

/**Return if the remote far-end parallel (FEP) loopback is enabled/disabled.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'remote_loopback_enable': Return pointer to remote loopback enable value
 *        0 - remote loopback disabled
 *        1 - remote loopback enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current FEP setting.
 */
int aw_pmd_16ln_remote_loopback_get(mss_access_t *mss, uint32_t *remote_loopback_enable);

/**Enable/disable tx loopback fifo after dcdiq calibration succeds 
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'enable_d': enable flag
 *        0 - loopback fifo disabled
 *        1 - rloopback fifo enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to set current FEP setting.
 */
int aw_pmd_16ln_tx_dcd_iq_cal(mss_access_t *mss, uint32_t enable_d);

/**Enable/disable remote far-end parallel (FEP) loopback (RX -> TX) datapath for the given lane.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'datapath_en': Set the remote loopback datapath enabled or disabled
 *        0 - remote loopback datapath disabled
 *        1 - remote loopback datapath enabled
 *
 * Return:
 *    'err':
 *        0 - Success, physical lane mapping matches logical lane mapping
 *        1 - Error, physical lane mapping does not match logical lane mapping
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to enable far-end parallel loopback for looping back RX data to TX. It is recommended to call one of
 *    the digref APIs after setting the datapath
 */
int aw_pmd_16ln_fep_data_set(mss_access_t *mss, uint32_t datapath_en);


/**Return if the remote far-end parallel (FEP) loopback for datapath is enabled/disabled.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'datapath_en': Return pointer to remote loopback enable value
 *        0 - remote loopback datapath enabled
 *        1 - remote loopback datapath enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current FEP setting.
 */
int aw_pmd_16ln_fep_data_get(mss_access_t *mss, uint32_t *datapath_en);

/**Enable/disable remote far-end parallel (FEP) loopback (RX -> TX) clock path for the given lane then runs dcdiq calibration
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'clock_en': Set the remote loopback clock path enabled or disabled
 *        0 - remote loopback clock path disabled
 *        1 - remote loopback clock path enabled
 *        255 - Enable corresponding registers and skip dcd iq calibration
 * Return:
 *    'err':
 *        0 - Success, physical lane mapping matches logical lane mapping
 *        1 - Error, physical lane mapping does not match logical lane mapping
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to enable far-end parallel loopback for looping back RX data to TX. It is recommended to call one of
 *    the digref APIs after setting the datapath
 */
int aw_pmd_16ln_fep_clock_set(mss_access_t *mss, uint8_t clock_en);

/**
 * Standalone API to update TX postdiv en value
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'postdiv_loopback_ena': Set the tx postdiv loopback enable to 1 or 0
 *
 * Return:
 *     AW Error Indicator Enum
 *
 * Constraints:
 *    To be used run post FEP loopback is enabled
 *
 * Typical Application Usage:
 *    Used to set and latch postdiv settings after FEP loopback is enabled .
 */
int aw_pmd_16ln_tx_postdiv_loopback_ena_set(mss_access_t *mss, uint8_t postdiv_loopback_ena);

/**
 * Standalone API to readback TX postdiv en value
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'postdiv_loopback_ena': Readback of tx postdiv loopback enable to 1 or 0
 *
 * Return:
 *     AW Error Indicator Enum
 *
 * Constraints:
 *    Can be used anytime
 *
 * Typical Application Usage:
 *    Used to readback postdiv setting for FEP loopback .
 */
int aw_pmd_16ln_tx_postdiv_loopback_ena_get(mss_access_t *mss, uint8_t *postdiv_loopback_ena);

/**Return if the remote far-end parallel (FEP) loopback for clock is enabled/disabled.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'clock_en': Return pointer to remote loopback's clock enable value
 *        0 - remote loopback clock enabled
 *        1 - remote loopback clock enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current FEP setting.
 */
int aw_pmd_16ln_fep_clock_get(mss_access_t *mss, uint32_t *clock_en);


/**Enable/disable analog local near-end serial (NES) loopback (TX -> RX) for the given lane.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'analog_loopback_enable':
 *        0 - analog loopback disabled
 *        1 - analog loopback enabled
 *
 * Return:
 *    'err':
 *        0 - Success, physical lane mapping matches logical lane mapping
 *        1 - Error, physical lane mapping does not match logical lane mapping
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to enable on-die TX to RX loopback, typically used during ATE.
 */
int aw_pmd_16ln_analog_loopback_set(mss_access_t *mss, uint32_t analog_loopback_enable);

/**Return if the analog local near-end serial (NES) loopback is enabled/disabled.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'analog_loopback_enable': Return pointer to value of analog loopback value
 *        -1 - error
 *        0 - analog loopback disabled
 *        1 - analog loopback enabled
 *
 *    'err': returns err = 1 if any logical lane number != physical lane number
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current near-end serial loopback setting.
 */
int aw_pmd_16ln_analog_loopback_get(mss_access_t *mss, uint32_t *analog_loopback_enable);


/**Enable/disable far-end serial (FES) loopback (RX -> TX) for the given lane.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'fes_loopback_enable':
 *        0 -  loopback disabled
 *        1 -  loopback enabled
 *
 * Return:
 *    'err':
 *        0 - Success, physical lane mapping matches logical lane mapping
 *        1 - Error, physical lane mapping does not match logical lane mapping
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to enable on-die far-end serial loopback for looping back RX to TX, typically used during ATE.
 */
int aw_pmd_16ln_fes_loopback_set(mss_access_t *mss, uint32_t fes_loopback_enable);


/**Return if the remote far-end parallel (FES) loopback is enabled/disabled.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'fes_loopback_enable': Return pointer to remote loopback enable value
 *        0 - fes_loopback_enable loopback disabled
 *        1 - fes_loopback_enable loopback enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current FES setting.
 */
int aw_pmd_16ln_fes_loopback_get(mss_access_t *mss, uint32_t *fes_loopback_enable);


/**Set TX polarity for a given lane.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'tx_pol_flip': TX Polarity value
 *        0 - normal polarity
 *        1 - inverted polarity
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During PD.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    If the polarity of the TX is physically inverted in package or board, this function can be used to invert.
 */
int aw_pmd_16ln_tx_polarity_set(mss_access_t *mss, uint32_t tx_pol_flip);

/**Return TX polarity for a given lane.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'tx_pol': Return pointer to value of tx polarity flip value
 *        0 - normal TX polarity
 *        1 - inverted TX polarity
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current TX polarity settings.
 */
int aw_pmd_16ln_tx_polarity_get(mss_access_t *mss, uint32_t *tx_pol_flip);

/**Set RX polarity for a given lane.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'rx_pol': Value for RX Polarity Flip
 *        0 - normal polarity
 *        1 - inverted polarity
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During PD.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    If the polarity of the RX is physically inverted in package or board, this function can be used to invert.
 */
int aw_pmd_16ln_rx_polarity_set(mss_access_t *mss, uint32_t rx_pol_flip);

/**Return RX polarity for a given lane.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'rx_pol': Return pointer for value of RX Polarity Flip Value
 *        0 - normal RX polarity
 *        1 - inverted RX polarity
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current RX polarity settings.
 */
int aw_pmd_16ln_rx_polarity_get(mss_access_t *mss, uint32_t *rx_pol_flip);

/**Set TX hbridge, used to adjust TX PAM4 eye linearity. Set msb and lsb to 30 first. Increase lsb to make inner eye smaller. Decrease lsb to make inner eye larger.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'tx_hbridge_st: input structure
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Certain applications such as optical drivers require PAM4 eye linearity to be adjusted.
 */
int aw_pmd_16ln_tx_hbridge_set(mss_access_t *mss, tx_hbridge_t *tx_hbridge_st);

/**Returns TX hbridge settings, used to adjust TX PAM4 eye linearity.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'msb': Return pointer for value of most significant bitswing for TX output
 *    'lsb': Return pointer for value of least significant bit inner eye size
 *    'lsb0': Return pointer for value of least significant bit inner eye size
 *    'lsb1': Return pointer for value of least significant bit inner eye size
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get current hbridge settings.
 */
int aw_pmd_16ln_tx_hbridge_get(mss_access_t *mss, uint32_t *msb, uint32_t *lsb);
// -----------------------------------------------------------------------------
// int aw_pmd_16ln_lane_cfg_set(mss_access_t *mss, lane_cfg_t lane_cfg); // RESERVED
// int aw_pmd_16ln_lane_cfg_get(mss_access_t *mss, lane_cfg_t *lane_cfg); // RESERVED
// -----------------------------------------------------------------------------

/**Enables or disables DFE adaptation. If DFE adapt is disabled, DFE coefficient will default to 0.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'dfe_adapt_enable':
 *        0 - DFE adapt disabled
 *        1 - DFE adapt enabled
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    When the channel has low loss (ie. short), DFE can be disabled for power savings and slight speed up to running equalization.
 */
int aw_pmd_16ln_rx_dfe_adapt_set(mss_access_t *mss, uint32_t dfe_adapt_enable);

/**Returns DFE enable setting.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'en': Return pointer for DFE enable setting
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get the dfe enable setting.
 */
int aw_pmd_16ln_rx_dfe_adapt_get(mss_access_t *mss, uint32_t *dfe_adapt_enable);

/**Enables or disables CTLE adaptation. If ctle adapt is disabled, ctle boost code will default to opts['ctle_boost_a'].
 *   If ctle adapt is enabled, initial ctle boost value will default to 0.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'ctle_adapt_enable':
 *        0 - CTLE adapt disabled
 *        1 - CTLE adapt enabled
 *    'ctle_boost_a': Forced ctle boost code when 'en' is 0. Ranges from 0-12.
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    When the channel used is short and low loss, CTLE adaptation can be disabled.
 */
int aw_pmd_16ln_rx_ctle_adapt_set(mss_access_t *mss, uint32_t ctle_adapt_enable, uint32_t ctle_boost_a);

/**Returns CTLE adapt enable setting.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'ctle_adapt_enable': Return pointer for CTLE adapt enable
 *    'ctle_boost_a': Return pointer for adapted or forced CTLE boost code
 *
 * Constraints:
 *    Can be used anytime.
 */
int aw_pmd_16ln_rx_ctle_adapt_get(mss_access_t *mss, uint32_t *ctle_adapt_enable, uint32_t *ctle_boost_a);

/**Enables or disables background adapt. When enabled, VGA fine, FFE, DFE, DC offset, slicers and thresholds are continuously adapted.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'rx_bkgrnd_adapt_enable':
 *        0 - Background adapt disable
 *        1 - Background adapt enable
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Don't care.
 *    Rate State:   After desired rate change.
 *    EqEval State: Must be run when foreground adapts are complete
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Background is typically enabled to continuously adapt RX coefficients non-destructively due to temperature and/or voltage fluctuations.
 */
int aw_pmd_16ln_rx_background_adapt_enable_set(mss_access_t *mss, uint8_t rx_background_adapt);

/**Returns background enable adapt setting.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'rx_bkgrnd_adapt_enable': Return pointer for background adapt enable
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get the background adapt enable setting.
 */
int aw_pmd_16ln_rx_background_adapt_enable_get(mss_access_t *mss, uint32_t *rx_bkgrnd_adapt_enable);

/**Enables or disables auto equalization. When enabled, receiver will automatically adapt when RX signal is present.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'rx_autoeq_enable':
 *        0 - Auto equalization enable
 *        1 - Auto equalization disable
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    It is recommended to enable this setting if a minimum BER is required prior to link training.
 *    This setting is typically enabled in PCIe applications where BER must be <1e-4 prior to running EqEval.
 */
int aw_pmd_16ln_rx_autoeq_set(mss_access_t *mss, uint32_t rx_autoeq_enable);

/**Returns Auto equalization enable adapt setting.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    `lane`: lane number
 *
 * Return:
 *    'rx_autoeq_enable': Return pointer for Auto Eq enable
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get the autoeq enable setting.
 */
int aw_pmd_16ln_rx_autoeq_get(mss_access_t *mss, uint32_t *rx_autoeq_enable);

/**Set max FFE tap count.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'tap_count': max number of FFE taps to use. If tap_count` is greater than or equal to the number of total FFE taps that the variant has, then all taps will be used.
 *                 Otherwise, for 11/15 reduced taps, there will always be 3 pre-taps, 1 main-tap, and the remaining are post-taps.
 *                 For 18 reduced taps, there will always be 4 pre-taps, 1 main-tap, and the remaining are post-taps
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to set the maximum number of FFE taps to use. In VSR applications, we may reduce the number of FFE taps to save power
 */
int aw_pmd_16ln_rx_ffe_tap_count_set(mss_access_t *mss, uint32_t tap_count);

/**Returns max number of FFE tap count.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'tap_count': Returns pointer for max number of FFE taps in use
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get maximum number of FFE taps used
 */
int aw_pmd_16ln_rx_ffe_tap_count_get(mss_access_t *mss, uint32_t* tap_count);


/**Set roaming configurations.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'mode': Specifies which roaming-capable banks will perform roaming
 *    'window_select1': Specifies the window used by bank2. Please refer to the lookup table for the meaning of this number
 *    'window_select2': Specifies the window used by bank1. Please refer to the lookup table for the meaning of this number
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect upon EqEval.
 *
 * Typical Application Usage:
 *    Used to set roaming configurations. Due to internal implementation, the relevant roaming registers, as specificed by the datasheet,
 *    will not be set immediately. They will be set during EqEval. This API is applicable for projects with more than 4K SRAM and
 *    two roaming banks. May be useful for optical applications
 */
int aw_pmd_16ln_rx_roaming_windows_set(mss_access_t *mss, aw_rx_roaming_mode_t mode, uint8_t window_select1, uint8_t window_select2);


/**Returns roaming configurations as set by aw_pmd_rx_roaming_windows_set
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'mode': Returns which roaming-capable banks are performing roaming. If mode is AW_RESET_DEFAULT_ROAMING_MODE, roaming is
 *            expected to return to its original por settings and the values of window_select1 and window_select2 should be ignored.
 *    'window_select1': Returns bank1's window select value. Please refer to lookup table on how to interpret this number.
 *    'window_select2': Returns bank2's window select value. Please refer to lookup table on how to interpret this number.
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to get roaming configurations as set by aw_pmd_rx_roaming_windows_set. Due to internal implementation, this does
 *    not return the values in the roaming registers, as specified by the datasheet. This API is applicable for projects with
 *    more than 4K SRAM and two roaming banks.
 */
int aw_pmd_16ln_rx_roaming_windows_get(mss_access_t *mss, aw_rx_roaming_mode_t *mode, uint8_t *window_select1, uint8_t *window_select2);

/**
 *
 * Function implementation has been deprecated. Use aw_pmd_rx_vga_cap_adapt_set
 *
 */
int aw_pmd_16ln_rx_vga_cap_set(mss_access_t *mss, uint32_t vga_cap);

/**
 *
 * Function implementation has been deprecated. Use aw_pmd_rx_vga_cap_adapt_get
 *
 */
int aw_pmd_16ln_rx_vga_cap_get(mss_access_t *mss, uint32_t *vga_cap);

/**Sets whether to use PRBS mode or LT mode
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'prbs_en': Set to 1 to enable rxeq_prbs mode. Set to 0 to use LT mode
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *   When not using LT, set prbs_en=1 for better BER.
 */
int aw_pmd_16ln_rxeq_prbs_set(mss_access_t *mss, uint32_t prbs_en);

/**Return whether to use PRBS mode or LT mode
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'prbs_en': specifies whether prbs mode is enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *   When not using LT, set prbs_en=1 for better BER.
 */
int aw_pmd_16ln_rxeq_prbs_get(mss_access_t *mss, uint32_t *prbs_en);

/**Sets CDR offset and direction
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'use_custom_cdr_offset' : Use custom CDR offset enable
 *          0 - Disable using custom CDR offset
 *          1 - Enable using custom CDR offset 
 *    'cdr_offset' : Sets CDR offset value (8bits)
 *    'cdr_dir' : Sets CDR direction
 *          0 - Negative direction
 *          1 - Positive direction
 *
 * Return:
 *
 * Constraints:
 *    Power State:  Before or during P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before EqEval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *   Should be used to improve post-MLSD BER, and to a lesser degree raw BER. This is very channel dependent and needs to be adapted/swept to find optimal results.
 */
int aw_pmd_16ln_rx_cdr_offset_set(mss_access_t *mss, uint32_t use_custom_cdr_offset, uint32_t cdr_offset, uint32_t cdr_dir);


/**Get state of RX signal detect.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'signal_detect':
 *        0 - signal detect low
 *        1 - signal detect high
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Use this function to report if there is data coming into RX and being detected.
 */
int aw_pmd_16ln_rx_signal_detect_get(mss_access_t *mss, uint32_t *signal_detect);

/**Check state of RX signal detect.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'signal_detect_expected' : expectef value for signal_detect
 *
 * Return:
 *    Will return non-zero if check fails
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Use this function to report if there is data coming into RX and being detected.
 */
int aw_pmd_16ln_rx_signal_detect_check(mss_access_t *mss, uint32_t signal_detect_expected);

/**Read TX clk PPM offset for a given lane. Desired PPM accuracy is 1/2**timing_window-1.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'timing_window': Sampling measurement window to be `2**timing_window-1` clock cycles
 *    'timeout_us': polling iteration limit in us
 *
 * Return:
 *    'err':
 *        0 - Polling successful, ppm value is valid
 *        1 - Polling unsuccesful, ppm value is invalid
 *    'tx_ppm': Return TX clock ppm offset value
 *
 * Constraints:
 *    Used when PLL is locked, in P1 or P0 state.
 *
 * Typical Application Usage:
 *    Use this function to report the PPM offset between reference clock and VCO clock.
 */
int aw_pmd_16ln_tx_ppm_get(mss_access_t *mss, uint32_t timing_window, uint32_t timeout_us, double *tx_ppm, double *vco_freq, double refclk_freq);


/**Read RX clk PPM offset for a given lane. Desired PPM accuracy is 1/2**timing_window-1.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'timing_window': Sampling measurement window to be `2**timing_window-1` clock cycles
 *    'timeout_us': polling iteration limit in us
 *
 * Return:
 *    'err':
 *        0 - Polling successful, ppm value is valid
 *        1 - Polling unsuccesful, ppm value is invalid
 *    'rx_ppm': Return RX clock ppm offset value
 *
 * Constraints:
 *    Used when PLL is locked, in P1 or P0 state.
 *
 * Typical Application Usage:
 *    Use this function to report the PPM offset between reference clock and VCO clock.
 */
int aw_pmd_16ln_rx_ppm_get(mss_access_t *mss, uint32_t timing_window, uint32_t timeout_us, double *rx_ppm, double *vco_freq, double refclk_freq);

/**Return if RX CDR is locked onto incoming data.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'pmd_rx_lock': Return pointer for RX CDR Lock value
 *        0 - RX CDR not locked
 *        1 - RX CDR locked
 *
 * Constraints:
 *    Used when PLL is locked, in P1 or P0 state.
 *
 * Typical Application Usage:
 *    Use this function to determine if CDR is locked to incoming data.
 */
int aw_pmd_16ln_rx_lock_status_get(mss_access_t *mss, uint32_t *pmd_rx_lock);


/**Return adapted codes for duty cycle distortion (DCD) and IQ phase on RX.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'rx_dcdiq_data': Return pointer to DCD and IQ data struct
 *
 *
 * Constraints:
 *    Get when RX is in P0 state after desired rate change.
 */
int aw_pmd_16ln_rx_dcdiq_get (mss_access_t *mss, aw_dcdiq_data_t *rx_dcdiq_data);

/**Return adapted codes for duty cycle distortion (DCD) and IQ phase on TX.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'tx_dcdiq_data': Return pointer to DCD and IQ data struct
 *
 * Constraints:
 *    Get when RX is in P0 state after desired rate change.
 */
int aw_pmd_16ln_tx_dcdiq_get (mss_access_t *mss, aw_dcdiq_data_t *tx_dcdiq_data);

/**Return adapted receiver analog front-end (AFE) parameters, used for signal integrity analysis.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'rx_afe_data': Return pointer to AFE data struct
 *
 *
 * Constraints:
 *    After EqEval is finished (after octl_rx_linkeval_ack_ln[#] returns high).
 *
 * Typical Application Usage:
 *    When experiencing signal integrity issues such as high BER, run this function to dump all DSP related parameters.
 */
int aw_pmd_16ln_rx_afe_get (mss_access_t *mss, aw_afe_data_t *rx_afe_data);

// BIST APIs
// -----------------------------------------------------------------------------

/**Configure RX datapath inversion
 *
 * RX Lane Function
 *
 *Will enable RX datapath inversion for inverted PRBS pattern support. NRZ, PAM4, with and without gray-coding are supported.
 *TODO: Add RTL (1p0, 1p1) ifdef guard.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'modulation_mode': Modulation mode
 *       "pam4" - PAM4 modulation
 *       "nrz"  - NRZ modulation 
 *    'gray_code_en': Gray code enable
 *        0 - Disable (no gray code)
 *        1 - Enable (gray code)
 *    'invert_en': Invert RX datapath enable
 *        0 - Disable inversion
 *        1 - Enable inversion
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After eqeval.
 *    Effect:       Takes effect immediately.
 *    Data:         RX is receiving PRBS data.
 *
 * Typical Application Usage:
 *    Enable RX and TX inverted datapath. Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_rx_invert_datapath(mss_access_t *mss, char modulation_mode[], uint32_t gray_code_en, uint32_t invert_en);


/**Configure TX datapath inversion
 *
 * TX Lane Function
 *
 *Will enable TX datapath inversion for inverted PRBS pattern support. NRZ, PAM4, with and without gray-coding are supported.
 *TODO: Add RTL (1p0, 1p1) ifdef guard.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'modulation_mode': Modulation mode
 *       "pam4" - PAM4 modulation
 *       "nrz"  - NRZ modulation 
 *    'gray_code_en': Gray code enable
 *        0 - Disable (no gray code)
 *        1 - Enable (gray code)
 *    'invert_en': Invert RX datapath enable
 *        0 - Disable inversion
 *        1 - Enable inversion
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After eqeval.
 *    Effect:       Takes effect immediately.
 *    Data:         RX is receiving PRBS data.
 *
 * Typical Application Usage:
 *    Enable RX and TX inverted datapath. Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_tx_invert_datapath(mss_access_t *mss, char modulation_mode[], uint32_t gray_code_en, uint32_t invert_en);

/**Configure RX BIST pattern.
 *
 * RX Lane Function
 *
 *Will disable RX BIST and clear BIST error counter first.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'pattern': Set the pattern to one defined by aw_bist_pattern_t type
 *    'UDP': 64bit user defined pattern
 *    'mode': aw_bist_mode_t enum
 *        0 - timer mode
 *        1 - wall clock mode
 *    'lock_thresh': the number of cycles before entering lock state
 *    'timer_thresh': number of cycles for timer mode
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After eqeval.
 *    Effect:       Takes effect immediately.
 *    Data:         RX is receiving PRBS data.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_rx_chk_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, aw_bist_mode_t mode, uint64_t udp, uint32_t lock_thresh, uint32_t timer_thresh);

/**Return RX BIST configuration.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'pattern': Return pointer to aw_bist_pattern_t type
 *    'UDP': Return pointer to 64bit user defined pattern if in UDP mode
 *    'mode': Return enum of the bist mode
 *        0 - timer mode
 *        1 - wall clock mode
 *    'lock_threshold': Return pointer to the number of cycles before entering lock state value
 *    'timer_thres': Return pointer to the number of cycles for timer mode value
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Get current RX BIST configuration settings.
 */
int aw_pmd_16ln_rx_chk_config_get(mss_access_t *mss, aw_bist_pattern_t *pattern, aw_bist_mode_t *mode, uint64_t *udp, uint32_t *lock_thresh, uint32_t *timer_thresh);

/**Enable/disable RX BIST.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'enable':
 *        0 - disable RX BIST
 *        1 - enable RX BIST
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After eqeval.
 *    Effect:       Takes effect immediately.
 *    Data:         RX is receiving PRBS data.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_rx_chk_en_set(mss_access_t *mss, uint32_t enable);

/**Return the RX BIST enable/disable.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'enable': Return pointer to RX BIST enable value
 *        0 - RX BIST disabled
 *        1 - RX BIST enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Get current RX BIST configuration settings.
 */
int aw_pmd_16ln_rx_chk_en_get(mss_access_t *mss, uint32_t *enable);

/**Return the RX BIST lock state.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'rx_bist_lock': Return pointer to check if RX BIST is locked
 *        0 - RX BIST not locked
 *        1 - RX BIST locked
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After eqeval.
 *    Effect:       Takes effect immediately.
 *    Data:         RX is receiving PRBS data.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_rx_chk_lock_state_get(mss_access_t *mss, uint32_t *rx_bist_lock);

/**Read RX BIST error counts, if the error count is done, and if the error counter is overflown.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'err_count': Return pointer to error count
 *    'err_count_done': Return pointer to err count done
 *        0 - error counting not done
 *        1 - error counting done
 *    'err_count_overflown': Return pointer to error count overflow
 *        0 - error counter not overflown
 *        1 - error counter overflown
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After eqeval.
 *    Effect:       Takes effect immediately.
 *    Data:         RX is receiving PRBS data.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_rx_chk_err_count_state_get(mss_access_t *mss, uint64_t *err_count, uint32_t *err_count_done, uint32_t *err_count_overflown);

/**Clear RX BIST error counter.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Clear the error counter after it overflows, or to reset the counting.
 */
int aw_pmd_16ln_rx_chk_err_count_state_clear(mss_access_t *mss);

/**Configure TX BIST pattern.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'pattern': Set the TX BIST Pattern
 *    'user_defined_pattern': Set TX 64bit user defined pattern
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_tx_gen_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, uint64_t user_defined_pattern);

/**Return TX BIST pattern configuration.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'pattern': Return pointer to TX BIST pattern enum
 *    'user_defined_pattern': Return pointer to 64bit user defined pattern value
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Get current TX BIST setting.
 */
int aw_pmd_16ln_tx_gen_config_get(mss_access_t *mss, aw_bist_pattern_t *pattern, uint64_t *user_defined_pattern);

/**Enable/disable TX BIST.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'enable': Enable value for TX BIST
 *        0 - disable TX BIST
 *        1 - enable TX BIST
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_tx_gen_en_set(mss_access_t *mss, uint32_t enable);

/**Return TX BIST enable/disable status.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'enable': Return pointer to TX BIST enable value
 *        0 - TX BIST disabled
 *        1 - TX BIST enabled
 *
 * Constraints:
 *    Can be used anytime.
 */
int aw_pmd_16ln_tx_gen_en_get(mss_access_t *mss, uint32_t *enable);

/**Configure TX error injection pattern and rate.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'err_pattern': 64bit error injection pattern
 *    'err_rate':
 *        0 - inject 1 bit error
 *        not 0 - error injection rate
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *    Data:         When TX is outputting PRBS pattern.
 *
 * Typical Application Usage:
 *    In a very low BER NRZ system (<1e-12), configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0. An error is injected and error count should increment by 1.
 *
 *    Error injection is not typically useful in PAM4 system because  BER is typically >1e-10 and the error counter has already started accumulating errors.
 */
int aw_pmd_16ln_tx_gen_err_inject_config_set(mss_access_t *mss, uint64_t err_pattern, uint32_t err_rate);

/**Return TX error injection pattern and rate.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'err_pattern': Return pointer to 64bit error injection pattern
 *    'err_rate': Return pointer to error rate value
 *        0 - inject 1 bit error
 *        not 0 - error injection rate
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to confirm error injection rate.
 */
int aw_pmd_16ln_tx_gen_err_inject_config_get(mss_access_t *mss, uint64_t *err_pattern, uint32_t *err_rate);

/**Enable/disable TX error injection.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'enable':
 *        0 - disable error injection
 *        1 - enable error injection
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *    Data:         When TX is outputting PRBS pattern.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 *}
 */
int aw_pmd_16ln_tx_gen_err_inject_en_set(mss_access_t *mss, uint32_t enable);

/**Return TX error injection enable/disable.
 *
 * TX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'enable': Return pointer to TX Error inject value
 *        0 - TX error injection disabled
 *        1 - TX error injection enabled
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to confirm error injection rate.
 */
int aw_pmd_16ln_tx_gen_err_inject_en_get(mss_access_t *mss, uint32_t *enable);

/**Sets the RX demapper settings to automatically account for custom gray code mapping (PAM4 only) and polarity inversion (PAM4 or NRZ) using BIST. BIST needs to be configured prior to calling this function.
 *
 * RX Lane Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'npam4_nrz':
 *        0 - PAM4 mode
 *        1 - NRZ mode
 *    'timeout_us': iteration limits to poll for RX BIST lock for each invert/gray encoding setting. Recommended to set to 5.
 *
 * Return:
 *    'locked':
 *        0 - Unable to find appropriate demapper/inversion settings. Please check PRBS setting or equalization setting.
 *        1 - RX BIST is locked and found appropriate demapper settings.
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: After EqEval
 *    Effect:       Takes effect immediately.
 *    Data:         When RX receiving PRBS pattern.
 *
 * Typical Application Usage:
 *    When using the PHY as a local RX and the remote TX PAM4 gray code mapping is unknown, run this function to automatically sweep for the correct gray code mapping to match that of the remote TX.
 */
int aw_pmd_16ln_rx_sweep_demapper(mss_access_t *mss, uint32_t npam4_nrz, uint32_t timeout_us);

/**Enable/disable TX BIST MSB/LSB bit swap
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'enable':
 *        0 - TX BIST MSB/LSB bit swap disabled
 *        1 - TX BIST MSB/LSB bit swap enabled
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Can be used anytime.
 */
int aw_pmd_16ln_gen_tx_swap_msb_lsb_set(mss_access_t *mss, uint32_t enable);

/**Returns TX BIST MSB/LSB bit swap status
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 *
 * Return:
      'enable': Return pointer to TX BIST enable value
 *        0 - TX BIST MSB/LSB bit swap disabled
 *        1 - TX BIST MSB/LSB bit swap enabled
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Can be used anytime.
 */
int aw_pmd_16ln_gen_tx_swap_msb_lsb_get(mss_access_t *mss, uint32_t *enable);

/**Enable/disable RX BIST MSB/LSB bit swap
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'enable':
 *        0 - RX BIST MSB/LSB bit swap disabled
 *        1 - RX BIST MSB/LSB bit swap enabled
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Can be used anytime.
 */
int aw_pmd_16ln_gen_rx_swap_msb_lsb_set(mss_access_t *mss, uint32_t enable);

/**Returns RX BIST MSB/LSB bit swap status
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 *
 * Return:
      'enable': Return pointer to RX BIST enable value
 *        0 - RX BIST MSB/LSB bit swap disabled
 *        1 - RX BIST MSB/LSB bit swap enabled
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    Can be used anytime.
 */
int aw_pmd_16ln_gen_rx_swap_msb_lsb_get(mss_access_t *mss, uint32_t *enable);

// NOTE: reserving this function here incase it proves to be useful.
// int aw_pmd_16ln_rx_demapper_get(mss_access_t *mss, uint32_t rx_demapper);

// uC APIs
// -----------------------------------------------------------------------------
/** Default method for loading firmware and pointers.
 *
 * Will load firmware and pointers via the PMI and PRAM interface.
 *
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'ucode': Pointer to a structure array containing address and value information
 *    'size': Size of the array
 *
 * Return:
 *    AW Error Indicator Enum
 *
 * Constraints:
 *    PMD must be in reset
 *
 * Typical Application Usage:
 *    Used for loading the sequencing configuration for the PMD while in reset.
 */
int aw_pmd_16ln_uc_ucode_load(mss_access_t *mss, aw_ucode_t *ucode, uint32_t ucode_len);

int aw_pmd_16ln_pll_lock_min_set(mss_access_t *mss, uint32_t val);
int aw_pmd_16ln_pll_lock_max_set(mss_access_t *mss, uint32_t val);


/**Return LCPLL lock status.
 *
 * Note: This is a register set by the MFSM when it has brought up the LCPLL, and is a one shot. Does not indicate if PLL has somehow lost lock.
 *
 * CMN Function
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'pll_lock': Return LCPLL lock return value
 *        0 - PLL not locked
 *        1 - PLL locked
 *
 * Constraints:
 *    Used when PLL is locked, in P1 or P0 state.
 *
 * Typical Application Usage:
 *    Use this function to determine if CMN PLL is locked to reference clock.
 */
int aw_pmd_16ln_pll_lock_get(mss_access_t *mss, uint32_t *pll_lock ,uint32_t check_en, uint32_t expected_val);

/**
 * Dump PHY Sequencing Configuration Debug information
 *
 * Return:
 *    'uc_diag': Pointer to a struct containing all the values of all the debug registers for PHY FW and sequencing
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_uc_diag_reg_dump(mss_access_t *mss, aw_uc_diag_regs_t *uc_diag);

/**
 * Enable diagnostic logging for the different lane sequencers
 *
 * Args:
 *    'uc_log_cmn_en': Enable logging for the common sequencer
 *    'uc_log_tx_en':  Enable logging for the TX sequencer
 *    'uc_log_rx_en':  Enable logging for the RX sequencer
 *
 * Return:
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_uc_diag_logging_en_set(mss_access_t *mss, uint32_t uc_log_cmn_en, uint32_t uc_log_tx_en, uint32_t uc_log_rx_en);

/**
 * Get the enable for diagnostic logging for the different lane sequencers
 *
 * CMN Function
 *
 * Args:
 *
 * Return:
 *    'uc_log_cmn_en': Return pointer to the enable logging for the common sequencer
 *    'uc_log_tx_en':  Return pointer to the enable logging for the TX sequencer
 *    'uc_log_rx_en':  Return pointer to the enable logging for the RX sequencer
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_uc_diag_logging_en_get(mss_access_t *mss, uint32_t *uc_log_cmn_en, uint32_t *uc_log_tx_en, uint32_t *uc_log_rx_en);


/**
 * Override of ictl_ref_ls_ena_a
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Override value for  ictl_ref_ls_ena_a
 *
 * Return:
 *
 * Constraints:
 *    Must be run after CMN isolation mode is configured.
 *
 * Typical Application Usage:
 *    Enables refclk configuration with register overrides.
 */

int aw_pmd_16ln_iso_ref_ls_en_set(mss_access_t *mss, uint32_t value);

/**
 * Override CMN power state settings during isolation mode.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set pstate value for CMN lane.
 *              0 - PD state
 *              1 - P0 state
 *
 * Return:
 *
 * Constraints:
 *    Must be run after CMN isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_cmn_pstate_set(mss_access_t *mss, uint32_t value);

/**
 * Return override CMN power state
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Set pstate value for CMN lane.
 *              0 - PD state
 *              1 - P0 state
 *
 * Constraints:
 *    Must be run after CMN isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_cmn_pstate_get(mss_access_t *mss, uint32_t *value);

/**
 * Override CMN request settings to initiate CMN state changes in isolation mode.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set req value for cmn state req
 *
 * Return:
 *
 * Constraints:
 *    Must be run after CMN isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_cmn_state_req_set(mss_access_t *mss, uint32_t value);

/**
 * Read CMN acknowledge value for state request in isolation mode.
 * Typically this function needs to be called repeatedly until ack_cmn returns a value of 1, indicating CMN state change is done.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'cmn_state_ack': Return CMN acknowledge value
 *
 * Constraints:
 *    Must be called after ictl_pclk_state_req_a has been raised.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_cmn_state_ack_get(mss_access_t *mss, uint32_t *cmn_state_ack);

/**
 * Override TX reset settings during isolation mode.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set value for Tx lane reset.
 *
 * Return:
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_reset_set(mss_access_t *mss, uint32_t value);

/**
 * Return TX reset isolation mode setting.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Return value for Tx lane reset.
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_reset_get(mss_access_t *mss, uint32_t *value);

/**
 * Override RX reset settings during isolation mode.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set value for Rx lane reset.
 *
 * Return:
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_reset_set(mss_access_t *mss, uint32_t value);

/**
 * Return RX reset isolation mode setting.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Return value for Rx lane reset.
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_reset_get(mss_access_t *mss, uint32_t *value);

/**
 * Override TX rate settings during isolation mode.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set value for TX lane rate. Refer to databook for mapping.
 *
 * Return:
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_rate_set(mss_access_t *mss, uint32_t value);

/**
 * Return TX rate isolation mode setting.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Return value for TX lane rate. Refer to databook for mapping.
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_rate_get(mss_access_t *mss, uint32_t *value);

/**
 * Override RX rate settings during isolation mode.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set value for RX lane rate. Refer to databook for mapping.
 *
 * Return:
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_rate_set(mss_access_t *mss, uint32_t value);

/**
 * Return RX rate isolation mode setting.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Return value for RX lane rate. Refer to databook for mapping.
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_rate_get(mss_access_t *mss, uint32_t *value);

/**
 * Override TX power state settings during isolation mode.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set pstate value for Tx lane.
 *          000 - P0 (Fully powered up)
 *          001 - P0s
 *          010 - P1
 *          011 - P2
 *          100 - PD (full power down)
 *          101 - PCIE L1.0 Substate
 *          110 - PCIE L1.1 Substate
 *          111 - PCIE L1.2 Substate
 *
 * Return:
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_pstate_set(mss_access_t *mss, uint32_t value);

/**
 *  Return TX power state isolation mode setting.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Return pstate value for Tx lane.
 *          000 - P0 (Fully powered up)
 *          001 - P0s
 *          010 - P1
 *          011 - P2
 *          100 - PD (full power down)
 *          101 - PCIE L1.0 Substate
 *          110 - PCIE L1.1 Substate
 *          111 - PCIE L1.2 Substate
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_pstate_get(mss_access_t *mss, uint32_t *value);

/**
 * Override RX power state settings during isolation mode.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set pstate value for Rx lane.
 *          000 - P0 (Fully powered up)
 *          001 - P0s
 *          010 - P1
 *          011 - P2
 *          100 - PD (full power down)
 *          101 - PCIE L1.0 Substate
 *          110 - PCIE L1.1 Substate
 *          111 - PCIE L1.2 Substate
 *
 * Return:
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_pstate_set(mss_access_t *mss, uint32_t value);

/**
 *  Return RX power state isolation mode setting.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Return pstate value for Rx lane.
 *          000 - P0 (Fully powered up)
 *          001 - P0s
 *          010 - P1
 *          011 - P2
 *          100 - PD (full power down)
 *          101 - PCIE L1.0 Substate
 *          110 - PCIE L1.1 Substate
 *          111 - PCIE L1.2 Substate
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_pstate_get(mss_access_t *mss, uint32_t *value);

/**
 * Override TX width settings during isolation mode.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set value for Tx lane width.
 *          1000 -> 1111 - unused
 *          0111 - 128 width
 *          0110 - 64 width
 *          0101 - 40 width
 *          0100 - 32 width
 *          0011 - 20 width
 *          0010 - 16 width
 *          0000 -> 0001 reserved
 *
 * Return:
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_width_set(mss_access_t *mss, uint32_t value);

/**
 * Return TX width isolation mode setting.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value': Set value for Tx lane width.
 *          1000 -> 1111 - unused
 *          0111 - 128 width
 *          0110 - 64 width
 *          0101 - 40 width
 *          0100 - 32 width
 *          0011 - 20 width
 *          0010 - 16 width
 *          0000 -> 0001 reserved
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_width_get(mss_access_t *mss, uint32_t *value);

/**
 * Override RX width settings during isolation mode.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': Set value for Rx lane width.
 *          1000 -> 1111 - unused
 *          0111 - 128 width
 *          0110 - 64 width
 *          0101 - 40 width
 *          0100 - 32 width
 *          0011 - 20 width
 *          0010 - 16 width
 *          0000 -> 0001 reserved
 *
 * Return:
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_width_set(mss_access_t *mss, uint32_t value);

/**
 * Return RX width isolation mode setting.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * RX Lane Function
 *
 * Return:
 *    'value': Set value for Rx lane width.
 *          1000 -> 1111 - unused
 *          0111 - 128 width
 *          0110 - 64 width
 *          0101 - 40 width
 *          0100 - 32 width
 *          0011 - 20 width
 *          0010 - 16 width
 *          0000 -> 0001 reserved
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_width_get(mss_access_t *mss, uint32_t *value);

/**
 * Override TX request settings to initiate TX state changes in isolation mode.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': set req value for tx state req
 *
 * Return:
 *
 * Constraints:
 *    Must be run after TX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_state_req_set(mss_access_t *mss, uint32_t value);

/**
 * Override RX request settings to initiate TX state changes in isolation mode.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value': set req value for rx state req
 *
 * Return:
 *
 * Constraints:
 *    Must be run after RX isolation mode is configured.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_state_req_set(mss_access_t *mss, uint32_t value);

/**
 * Read TX acknowledge value for state request in isolation mode.
 * Typically this function needs to be called repeatedly until ack_tx returns a value of 1, indicating TX state change is done.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'tx_state_ack': Return TX acknowledge value
 *
 * Constraints:
 *    Must be called after ictl_tx_state_req_ln[#] has been raised.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_tx_state_ack_get(mss_access_t *mss, uint32_t *tx_state_ack);

/**
 * Read RX acknowledge value for state request in isolation mode.
 * Typically this function needs to be called repeatedly until ack_rx returns a value of 1, indicating RX state change is done.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'rx_state_ack': Return RX acknowledge value
 *
 * Constraints:
 *    Must be called after ictl_rx_state_req_ln[#] has been raised.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_iso_rx_state_ack_get(mss_access_t *mss, uint32_t *rx_state_ack);

/**
 * Override CMN pin interface to enable standalone PHY isolation mode.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : 0 - Use CMN pins from pin interface
 *           1 - Override CMN pin interface
 *
 * Return:
 *
 * Constraints:
 *    Must be run after firmware is loaded, and before asserting TX/RX resets irst_*_ln[#].
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_cmn_set(mss_access_t *mss, uint32_t en);

/**
 * Return CMN isolation mode enable status.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'en' : Returns isolation enable mode.
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_cmn_get(mss_access_t *mss, uint32_t *en);

/**
 * Override lane pin interface to enable standalone PHY isolation mode.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : 0 - Use lane pins from pin interface
 *           1 - Override lane pin interface
 *
 * Return:
 *
 * Constraints:
 *    Must be run after firmware is loaded, and before asserting TX/RX resets irst_*_ln[#].
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
*/

/**
 * Override lane pin interface to enable standalone PHY isolation mode.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : 0 - Use lane pins from pin interface
 *           1 - Override lane pin interface
 *
 * Return:
 *
 * Constraints:
 *    Must be run after firmware is loaded, and before asserting TX/RX resets irst_*_ln[#].
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_lane_set(mss_access_t *mss, uint32_t en);


/**
 * override tx lane pin interface to enable standalone phy isolation mode.
 *
 * args:
 *    'mss': object containing driver methods.
 *    'en' : 0 - use lane pins from pin interface
 *           1 - override lane pin interface
 *
 * return:
 *
 * constraints:
 *    must be run after firmware is loaded, and before asserting tx/rx resets irst_*_ln[#].
 *
 * typical application usage:
 *    used to isolate the phy from the soc for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_lane_tx_set(mss_access_t *mss, uint32_t en);


/**
 * override tx lane pin interface to enable standalone phy isolation mode.
 *
 * args:
 *    'mss': object containing driver methods.
 *    'en' : 0 - use lane pins from pin interface
 *           1 - override lane pin interface
 *
 * return:
 *
 * constraints:
 *    must be run after firmware is loaded, and before asserting tx/rx resets irst_*_ln[#].
 *
 * typical application usage:
 *    used to isolate the phy from the soc for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_lane_rx_set(mss_access_t *mss, uint32_t en);


/**
 * override lane pin interface common to tx and rx to enable standalone phy isolation mode.
 *
 * args:
 *    'mss': object containing driver methods.
 *    'en' : 0 - use lane pins from pin interface
 *           1 - override lane pin interface
 *
 * return:
 *
 * constraints:
 *    must be run after firmware is loaded, and before asserting tx/rx resets irst_*_ln[#].
 *
 * typical application usage:
 *    used to isolate the phy from the soc for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_lane_txrx_set(mss_access_t *mss, uint32_t en);


/**
 * Return lane isolation mode enable status.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'en' : Returns isolation enable mode.
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *    Used to isolate the PHY from the SoC for deep debug or electrical characterization.
 */
int aw_pmd_16ln_isolate_lane_get(mss_access_t *mss, uint32_t *en);

/**
 * Sets the selection of the right to left high-speed reference clock buffer.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : 11 - Unused
 *            10 - Transmit local hsref clock on left to right cml.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right hsref buffer.
 *
 * Return:
 *
 * Constraints:
 *    Must be run at beginning of configuration
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level
 *
 */
int aw_pmd_16ln_cmn_r2l_hsref_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Sets the selection of the right to left low-speed reference clock buffer number 0.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer right to left cml clock
 *            00 - Power down right to left lsref buffer.
 *
 * Return:
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_r2l0_lsref_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Sets the selection of the right to left low-speed reference clock buffer number 1.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer right to left cml clock
 *            00 - Power down right to left lsref buffer.
 *
 * Return:
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_r2l1_lsref_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Sets the selection of the left to right high-speed reference clock buffer.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : 11 - Unused
 *            10 - Transmit local hsref clock on left to right cml.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right hsref buffer.
 *
 * Return:
 *
 * Constraints:
 *    Must be run at beginning of configuration
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level
 *
 */
int aw_pmd_16ln_cmn_l2r_hsref_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Sets the selection of the left to right low-speed reference clock buffer number 0.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right lsref buffer.
 *
 * Return:
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_l2r0_lsref_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Sets the selection of the left to right low-speed reference clock buffer number 1.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right lsref buffer.
 *
 * Return:
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_l2r1_lsref_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Returns the selection of the right to left high-speed reference clock buffer.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'sel' : 11 - Unused
 *            10 - Transmit local hsref clock on left to right cml.
 *            01 - Buffer right to left cml clock
 *            00 - Power down right to left hsref buffer.
 *
 * Constraints:
 *    Must be run at beginning of configuration
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level
 *
 */
int aw_pmd_16ln_cmn_r2l_hsref_sel_get(mss_access_t *mss, uint32_t *sel);

/**
 * Return the selection of the right to left low-speed reference clock buffer number 0.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer right to left cml clock
 *            00 - Power down right to left lsref buffer.
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_r2l0_lsref_sel_get(mss_access_t *mss, uint32_t *sel);

/**
 * Return the selection of the right to left low-speed reference clock buffer number 1.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer right to left cml clock
 *            00 - Power down right to left lsref buffer.
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_r2l1_lsref_sel_get(mss_access_t *mss, uint32_t *sel);

/**
 * Returns the selection of the left to right high-speed reference clock buffer.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'sel' : 11 - Unused
 *            10 - Transmit local hsref clock on left to right cml.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right hsref buffer.
 *
 * Constraints:
 *    Must be run at beginning of configuration
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level
 *
 */
int aw_pmd_16ln_cmn_l2r_hsref_sel_get(mss_access_t *mss, uint32_t *sel);

/**
 * Return the selection of the left to right low-speed reference clock buffer number 0.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right lsref buffer.
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_l2r0_lsref_sel_get(mss_access_t *mss, uint32_t *sel);

/**
 * Return the selection of the left to right low-speed reference clock buffer number 1.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'sel' : 11 - Transmit reference clock from local bu_ref_p, bu_ref_n bumps.
 *            10 - Transmit looptiming clock derived from a local rx lane.
 *            01 - Buffer left to right cml clock
 *            00 - Power down left to right lsref buffer.
 *
 * Constraints:
 *    Must be run at beginning of configuration.
 *
 * Typical Application Usage:
 *    Use to route reference clock to SOC level.
 *
 */
int aw_pmd_16ln_cmn_l2r1_lsref_sel_get(mss_access_t *mss, uint32_t *sel);

/**
 * Override the lsref reference clock source select
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'ref_sel' : Selection of which reference clock is used for the local LC synthesizer
 *            111/110 - Unused
 *            110 - Use lsref_soc for the LC synth
 *            101 - Use rx_ref for the LC synth
 *            100 - Use local ref for the LC synth
 *            011 - Use r2l0 reference clock for LC synth
 *            010 - use r2l1 reference clock for the LC synth
 *            001 - use l2r0 reference clock for the LC synth
 *            000 - use l2r1 reference clock for the LC synth
 *
 * Return:
 *
 * Constraints:
 *    Must be run after before asserting TX/RX resets irst_*_ln[#].
 *
 * Typical Application Usage:
 *    Use to select the lsref reference clock source during configuration.
 */
int aw_pmd_16ln_cmn_lsref_sel_set(mss_access_t *mss, uint32_t ref_sel);

/**
 * Return the lsref reference clock source select
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'ref_sel' : Selection of which reference clock is used for the local LC synthesizer
 *            111/110 - Unused
 *            110 - Use lsref_soc for the LC synth
 *            101 - Use rx_ref for the LC synth
 *            100 - Use local ref for the LC synth
 *            011 - Use r2l0 reference clock for LC synth
 *            010 - use r2l1 reference clock for the LC synth
 *            001 - use l2r0 reference clock for the LC synth
 *            000 - use l2r1 reference clock for the LC synth
 *
 * Constraints:
 *    Can be run at anytime.
 *
 * Typical Application Usage:
 *    Use to return the lsref reference clock source select
 */
int aw_pmd_16ln_cmn_lsref_sel_get(mss_access_t *mss, uint32_t *ref_sel);

/**
 * Set the FW to power up with a 25MHz reference clock
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'lsref_25m': 0 - 100MHz reference clock (default)
 *                 1 - 25MHz reference clock
 *
 * Return:
 *
 * Constraints:
 *    Must be programmed before triggering powerup, ideally while the PHY is in
 *     reset.
 *
 * Typical Application Usage:
 *    Should be used only for PCIe applications.
 */
int aw_pmd_16ln_pcie_cmn_lsref_25m_set(mss_access_t *mss, uint32_t lsref_25m);

/**
 * Get the value of 25MHz reference clock FW mode
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'lsref_25m': 0 - 100MHz reference clock (default)
 *                 1 - 25MHz reference clock
 *
 * Constraints:
 *    Must be programmed before triggering powerup, ideally while the PHY is in
 *     reset.
 *
 * Typical Application Usage:
 *    Should be used only for PCIe applications.
 */
int aw_pmd_16ln_pcie_cmn_lsref_25m_get(mss_access_t *mss, uint32_t *lsref_25m);

/**
 * Enable/disable TX BIST.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value' : 0 - disable TX BIST
 *              1 - enable TX BIST
 *
 * Return:
 *
 * Constraints:
 *    Power State:  During P0.
 *    Rate State:   After desired rate change.
 *    EqEval State: Don't care.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Configure TX and RX BIST to PRBS31 pattern, enable external loopback, and enable TX and RX BIST. RX BIST should lock to the pattern generated by TX BIST with error = 0.
 */
int aw_pmd_16ln_gen_tx_en_set(mss_access_t *mss, uint32_t value);

/**
 * Return TX BIST enable/disable status.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value' : 0 - TX BIST disabled
 *              1 - TX BIST enabled
 *
 * Constraints:
 *    Can be run anytime.
 *
 * Typical Application Usage:
 *    Used to return TX BIST enable/disable status.
 */
int aw_pmd_16ln_gen_tx_en_get(mss_access_t *mss, uint32_t *value);

/**
 * Read error count done bit. Indicating rxbist burst capture is complete
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'err_count_done' : 1 indicates rxbist burst capture is complete
 *
 * Constraints:
 *    Must to run while rxbist is enabled
 *
 * Typical Application Usage:
 *    Used for debug.
 *
 */
int aw_pmd_16ln_rx_error_cnt_done_get(mss_access_t *mss, uint32_t *err_count_done);

/**
 * Initiate cmn power state change.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'cmn_pstate' : Desired pstate
 *    'timeout_us' : Timeout limit in us
 *
 * Return:
 *
 * Constraints:
 *    CMN must be in isolation mode.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_iso_request_cmn_state_change(mss_access_t *mss, aw_cmn_pstate_t cmn_pstate, uint32_t timeout_us);

/**
 * Initiate TX state change.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'tx_pstate' : Desired TX pstate; See datasheet for mapping
 *    'tx_rate'   : Desired TX rate; See datasheet for mapping
 *    'tx_width'  : Desired TX width; See datasheet for mapping
 *    'timeout_us' : Timeout limit in us
 *
 * Return:
 *    Return value will non-zero if timeout
 *
 * Constraints:
 *    TX must be in isolation mode.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_iso_request_tx_state_change(mss_access_t *mss, aw_pstate_t tx_pstate, uint32_t tx_rate, uint32_t tx_width, uint32_t timeout_us);

/**
 * Initiate RX state change.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'rx_pstate' : Desired RX pstate; See datasheet for mapping
 *    'rx_rate'   : Desired RX rate; See datasheet for mapping
 *    'rx_width'  : Desired RX width; See datasheet for mapping
 *    'timeout_us' : Timeout limit in us
 *
 * Return:
 *    Return value will non-zero if timeout
 *
 * Constraints:
 *    RX must be in isolation mode.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_iso_request_rx_state_change(mss_access_t *mss, aw_pstate_t rx_pstate, uint32_t rx_rate, uint32_t rx_width, uint32_t timeout_us);

/**
 * Poll for RX CDR Lock.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'timeout_us' : Timeout limit in us
 *
 * Return:
 *    Return value will non-zero if timeout
 *
 * Constraints:
 *    Must be run after RX is powered up to P0/P1.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_rx_check_cdr_lock(mss_access_t *mss, uint32_t timeout_us);

/**
 * Wrapper function for aw_pmd_rx_chk_err_count_state_get with timeout
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'rx_width' : Data width used in rx data bist (See datasheet for encoding)
 *    'timer_threshold' : number of cycles for timer mode
 *    'timeout_us' : Timeout limit in us
 *    'expected_errors' : Expected amount of rxbist errors. If -1, perform read instead of check.
 *
 * Return:
 *    Return value will non-zero if timeout
 *
 * Constraints:
 *    Must be run after RX BIST is enabled.
 *
 * Typical Application Usage:
 *    Meant to be run to read rxbist error count and error count overflow bit.
 */
int aw_pmd_16ln_rx_check_bist(mss_access_t *mss, aw_bist_mode_t bist_mode, uint32_t timer_threshold, uint32_t rx_width, uint32_t timeout_us, int32_t expected_errors);

/**
 * Clears the prefec histograms counters
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *
 * Constraints:
 *    None
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_clear(mss_access_t *mss);

/**
 * Get enable for the prefec histograms counters
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'enable': 1 - On
 *              0 - Off
 *
 * Constraints:
 *    Enable should be asserted after configuring refec using: aw_pmd_rx_prefec_config.
 *    Enable should be asserted after after confirming bist lock using: aw_pmd_rx_chk_lock_state_get
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_enable_get(mss_access_t *mss, uint32_t *enable);

/**
 * Enables the prefec histograms counters
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'enable': 1 - On
 *              0 - Off
 *
 * Return:
 *
 * Constraints:
 *    Enable should be asserted after configuring refec using: aw_pmd_rx_prefec_config.
 *    Enable should be asserted after after confirming bist lock using: aw_pmd_rx_chk_lock_state_get
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_enable_set(mss_access_t *mss, uint32_t enable);

/**
 * Polls the prefec done register in timer mode (see aw_pmd_rx_prefec_config)
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'timeout_us': Poll timeout in us
 *
 * Return:
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_poll(mss_access_t *mss, uint32_t timeout_us);

/**
 * Get the configuration of the prefec block
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'timeout_us': Poll timeout in us
 *    'symbol_size' : Number of bits in a symbol. 8 and 10 are the only valid parameters
 *    'sym_per_cw'  : Number of Symbols in a Codeword
 *    'corr_num_syms' : Number of Correctable Symbols in a Codeword. Sets the maximum histogram bin to accumulate results
 *    'wall_mode' : 0 - Timer Mode, the prefec counter will run until the number of analyzed codewords reaches timer_num_cw
 *                  1 - Wall Mode, the prefec counter will run indefinitely. Careful of overflows
 *    'skip_syms' : Optionally set the number of symbols to skip before analyzing the next Codeword.
 *    'timer_num_cw' : Sets the codeword count threshold for timer mode for
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_config_get(mss_access_t *mss, uint32_t *corr_num_syms, uint32_t *symbol_size, uint32_t *wall_mode, uint32_t *sym_per_cw, uint32_t *skip_syms, uint32_t *timer_num_cw);

/**
 * Configure the prefec block
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'timeout_us': Poll timeout in us
 *    'symbol_size' : Number of bits in a symbol. 8 and 10 are the only valid parameters
 *    'sym_per_cw'  : Number of Symbols in a Codeword
 *    'corr_num_syms' : Number of Correctable Symbols in a Codeword. Sets the maximum histogram bin to accumulate results
 *    'wall_mode' : 0 - Timer Mode, the prefec counter will run until the number of analyzed codewords reaches timer_num_cw
 *                  1 - Wall Mode, the prefec counter will run indefinitely. Careful of overflows
 *    'skip_syms' : Optionally set the number of symbols to skip before analyzing the next Codeword.
 *    'timer_num_cw' : Sets the codeword count threshold for timer mode for
 *
 * Return:
 *    Poll ack result
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_config_set(mss_access_t *mss, uint32_t corr_num_syms, uint32_t symbol_size, uint32_t wall_mode, uint32_t sym_per_cw, uint32_t skip_syms, uint32_t timer_num_cw);

/**
 * Get prefec histogram results in 32 histogram bins
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'hist': 32bit array of 32 histogram bins.
 *
 * Return:
 *
 * Constraints:
 *     In timer mode, it should be read after aw_pmd_rx_prefec_poll returns ack
 *     In wall mode, aw_pmd_rx_prefec_enable should be set to 0 before reading to prevent skewed bin results
 *
 * Typical Application Usage:
 *    For Prefec Analysis
 */
int aw_pmd_16ln_rx_prefec_get_results(mss_access_t *mss, uint32_t* hist);

/**
 * Run eqeval with desired type during isolation mode. Returns incdec coefficients.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'eq_type' : Sets eqeval type.
 *            0 - Full EQ, Directional
 *            1 - Eval Only, Directional
 *            2 - Init Eval
 *            3 - Clear Eval
 *            4 - Full EQ, FOM
 *            5 - Eval Only, FOM
 *            6+ Reserved
 *
 * Return:
 *
 * Constraints:
 *    Must be called when RX is in P0 power state.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_eqeval_type_set(mss_access_t *mss, uint32_t eq_type);

/**
 * Sets request for initiating EqEval in isolation mode.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'req' : Sets eqeval request.
 *
 * Return:
 *
 * Constraints:
 *    Must be called when RX is in P0 power state.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_eqeval_req_set(mss_access_t *mss, uint32_t value);

/**
 * Read acknowledge for EqEval in isolation mode.
 * Typically this function needs to be called repeatedly until ack returns a value of 1, indicating EqEval is done.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 *
 * Return:
 *    'eqeval_ack' : Return acknowledge value
 *
 * Constraints:
 *    Must be called after ictl_rx_linkeval_req_ln[#] has been raised.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_eqeval_ack_get(mss_access_t *mss, uint32_t *eqeval_ack );

/**
 * Returns eqeval incdec coefficient update in isolation mode.
 * Incdec tells the remote transmitter to increase or decrease adaptation values.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 *
 * Return:
 *    'incdec' : [7:6] C+1, [5:4] C0, [3:2] C-1, [1:0] C-2. 00 = hold 01 = increment 10 = decrement
 *
 * Constraints:
 *    Must be called after octl_rx_linkeval_ack_ln[#] has returned value of 1.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_eqeval_incdec_get(mss_access_t *mss, uint32_t * incdec);

/**
 * Requests rx_linkeval and reads back incdec coefficient.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'eq_type' : Sets eqeval type.
 *            0 - Full EQ, Directional
 *            1 - Eval Only, Directional
 *            2 - Init Eval
 *            3 - Clear Eval
 *            4 - Full EQ, FOM
 *            5 - Eval Only, FOM
 *            6+ Reserved
 *    'timeout_us' : Timeout limit in us
 *
 *
 * Return:
 *    Return value will non-zero if timeout
 *
 * Constraints:
 *    Must be called when RX is in P0 power state.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_rx_equalize(mss_access_t *mss, aw_eq_type_t eq_type, uint32_t timeout_us );

/**
 * Sets receiver detect request
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value' : tx_rxdet req
 *
 * Return:
 *
 * Constraints:
 *    Power State:  During P0/P1.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_tx_rxdet_req_set(mss_access_t *mss, uint32_t value);

/**
 * TX detect RX test.
 * 1) Set tx_rxdet req=1
 * 2) Poll for ack
 * 3) Check for expected tx_rxdet_result
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'rxdet_expected' : Expected tx_rxdet_result
 *                       0 - Receiver not detected
 *                       1 - Receiver detected
 *    'timeout_us' : Timeout limit in us
 *
 * Return:
 *    Return value will non-zero if timeout or if check fails.
 *
 * Constraints:
 *    Power State:  During P0/P1.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_tx_rxdet(mss_access_t *mss, uint32_t rxdet_expected, uint32_t timeout_us);

/**
 * Enable TX Beacon signal transmission
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'value' : enable for tx beacon
 *
 * Return:
 *
 * Constraints:
 *    Power State:  During P2.
 *    Rate State:   After desired rate change.
 *    EqEval State: Before eqeval.
 *    Effect:       Takes effect immediately.
 *
 * Typical Application Usage:
 *    Used to verify TX Beacon requirements for PCIE spec. If external loopback is enabled, can be used to test rx signal detect.
 */
int aw_pmd_16ln_tx_beacon_en_set(mss_access_t *mss, uint32_t value);

/**
 * Return TX Beacon enable status.
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'value' : Return value for tx beacon enable status
 *
 * Constraints:
 *    Can be run anytime.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_tx_beacon_en_get(mss_access_t *mss, uint32_t *value);

/**
 * Capture TX PLL osc fine code
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'center_code' : expected osc code
 *    'tolerance'   : Tolerence for center_code check. If -1, read instead of check.
 *
 * Return:
 *    'tx_pll_fine_code' : TX PLL osc fine code
 *
 * Constraints:
 *    TX PLL must be running.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_tx_pll_fine_code_get(mss_access_t *mss, uint32_t * tx_pll_fine_code, uint32_t center_code, int tolerance);

/**
 * Capture TX PLL osc coarse code
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'tx_pll_coarse_code' : TX PLL osc coarse code
 *
 * Constraints:
 *    TX PLL must be running.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_tx_pll_coarse_code_get(mss_access_t *mss, uint32_t * rx_pll_coarse_code, uint32_t center_code, int tolerance);

/**
 * Read RX PLL osc fine code
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'center_code' : expected osc code
 *    'tolerance'   : Tolerence for center_code check. If -1, read instead of check.
 *
 * Return:
 *    'rx_pll_fine_code' : RX PLL osc fine code
 *
 * Constraints:
 *    RX PLL must be running.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_rx_pll_fine_code_get(mss_access_t *mss, uint32_t * rx_pll_fine_code, uint32_t center_code, int tolerance);


/**
 * Capture RX PLL osc coarse code
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'rx_pll_coarse_code' : RX PLL osc coarse code
 *
 * Constraints:
 *    RX PLL must be running.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_rx_pll_coarse_code_get(mss_access_t *mss, uint32_t * rx_pll_coarse_code, uint32_t center_code, int tolerance);

/**
 * Capture LC PLL osc fine code
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'center_code' : expected osc code
 *    'tolerance'   : Tolerence for center_code check. If -1, read instead of check.
 *
 * Return:
 *    'cmn_pll_fine_code' : LC PLL osc fine code
 *
 * Constraints:
 *    LC PLL must be running.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_cmn_pll_fine_code_get(mss_access_t *mss, uint32_t * cmn_pll_fine_code, uint32_t center_code, int tolerance);

/**
 * Capture LC PLL osc coarse code
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *    'cmn_pll_coarse_code' : LC PLL osc coarse code
 *
 * Constraints:
 *    LC PLL must be running.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_cmn_pll_coarse_code_get(mss_access_t *mss, uint32_t * cmn_pll_coarse_code , uint32_t center_code, int tolerance);

/**
 * Returns process monitor results based on frequency measurements of ring oscillators
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'pmon_sel' : select for target pmon ring osc cell . Please see datasheet for mapping
 *    'pvt_measure_timing_window' : timing window for frequency measurement (in power of 2 refclk cycles)
 *    'timeout_us' : Timeout limit in us
 *
 *
 * Return:
 *    'pvt_measure_result' : Return value from pmon measurement
 *
 * Constraints:
 *    CMN must be in P0.
 *
 * Typical Application Usage:
 *    Used for debug.
 */
int aw_pmd_16ln_measure_pmon(mss_access_t *mss, uint32_t pmon_sel, uint32_t pvt_measure_timing_window, uint32_t timeout_us, uint32_t * pvt_measure_result);

//TODO
/**
 * CMN Function
 */
int aw_pmd_16ln_atest_en(mss_access_t *mss, uint32_t en);

//TODO
/**
 * CMN Function
 */
int aw_pmd_16ln_atest_cmn_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val) ;

/**
 * Wrapper function to call the following status APIs:
 * aw_pmd_rx_dsp_get
 * aw_pmd_rx_afe_get
 * aw_pmd_rx_dcdiq_get
 * aw_pmd_tx_dcdiq_get
 * aw_pmd_tx_pll_fine_code_get
 * aw_pmd_tx_pll_coarse_code_get
 * aw_pmd_rx_pll_fine_code_get
 * aw_pmd_rx_pll_coarse_code_get
 * aw_pmd_cmn_pll_fine_code_get
 * aw_pmd_cmn_pll_coarse_code_get
 *
 * TX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'branch' : Select for which branch to read FFE coefficients. If -1, read all branches
 *
 * Return:
 *
 * Constraints:
 *    Can be read at any time.
 *
 * Typical Application Usage:
 *    Meant to be run after loopback test for debug.
 */
int aw_pmd_16ln_atest_tx_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val);

/**
 * RX Lane Function
 */
int aw_pmd_16ln_atest_rx_a_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val);

/**
 * RX Lane Function
 */
int aw_pmd_16ln_atest_rx_b_capture(mss_access_t *mss, uint32_t atest_addr, uint32_t atest_term, uint32_t * atest_adc_val);

/**
 * Wrapper function to call the following status APIs:
 * aw_pmd_rx_dsp_get
 * aw_pmd_rx_afe_get
 * aw_pmd_rx_dcdiq_get
 * aw_pmd_tx_dcdiq_get
 * aw_pmd_tx_pll_fine_code_get
 * aw_pmd_tx_pll_coarse_code_get
 * aw_pmd_rx_pll_fine_code_get
 * aw_pmd_rx_pll_coarse_code_get
 * aw_pmd_cmn_pll_fine_code_get
 * aw_pmd_cmn_pll_coarse_code_get
 * 
 * Args:
 *    'mss': Object containing driver methods.
 *    'branch' : Select for which branch to read FFE coefficients. If -1, read all branches
 *
 * Return: 
 *
 * Constraints:
 *    Can be read at any time.
 *
 * Typical Application Usage:
 *    Meant to be run after loopback test for debug.
 */
int aw_pmd_16ln_read_status(mss_access_t *mss, int branch);
int aw_pmd_16ln_read_status2(mss_access_t *mss, int branch);

/**
 * Requests background equalization to pause
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'pause_enable' : 1 - Pause Background Equalization
 *                     0 - Enable Background Equalization
 *
 *
 * Return:
 *
 * Constraints:
 *    Use only after power up has completed successfully.
 *
 * Typical Application Usage:
 *    Required to enable reading FFE Coefficients, this must be called prior
 *    reading any per branch FFE values.
 *
 */
int aw_pmd_16ln_pause_background(mss_access_t *mss, uint32_t pause_enable);

#if 0
//defined in ../aw_types.h
//
typedef enum {
    AW_TB_DCOFFSET = 0,
    AW_TB_TBUS     = 1,
    AW_TB_RAWADC   = 2,
} aw_pmd_tracebuffer_mode_t;
#endif

/**
 * Configuration function for the tracebuffer, can pull fill up data from 1 of
 * 3 different probes as defined by the aw_pmd_tracebuffer_mode_t enum.
 *
 * RX Lane Function - Used in test mode only
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'tbmode' : select what block will fill up tracebuffer data
 *    'samples_per_cycle' : configure how many samples of data will be collected
 *                          on each cycle
 *
 * Return:
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    Called before triggering the tracebuffer data collection. Used in debug
 *    functions only.
 *
 */
int aw_pmd_16ln_tracebuffer_config_set(mss_access_t *mss, aw_pmd_tracebuffer_mode_t tbmode, uint32_t samples_per_cycle);
int aw_pmd_16ln_tracebuffer_config_get(mss_access_t *mss, aw_pmd_tracebuffer_mode_t *tbmode, uint32_t *samples_per_cycle);

/**
 * Sets the configuration mode of the tracebuffer and optionally enables it.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'clk_sel' : the clock to use for pulling data from the applicable subblock
 *    'enable' : 1 to enable; 0 to disable
 *
 * Return:
 *
 * Constraints:
 *    Must be programmed prior to powerup
 *
 * Typical Application Usage:
 *   Setting whether to use SRIS mode, intended to be used for isolation mode
 *
 */
int aw_pmd_16ln_tracebuffer_config_enable(mss_access_t *mss, uint32_t clk_sel, uint32_t enable);

/**
 * Generalized tracebuffer function to ease collection and value parsing from
 * various subblocks.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'tb_data_out' : array of output values after conversion
 *    'block_id' : the block id corresponding to the digital block to pull data
 *                 from
 *    'signal_id' : the signals of the corresponding block to pull from
 *    'fp_lsb' : the LSB of the fixed point data value
 *    'fp_msb' : the MSB of the fixed point data value
 *    'fp_si' : indicates if the data is signed or not
 *
 * Return:
 *
 * Constraints:
 *    Must be programmed prior to powerup
 *
 * Typical Application Usage:
 *   Setting whether to use SRIS mode, intended to be used for isolation mode
 *
 */
int aw_pmd_16ln_read_tracebuffer_general(mss_access_t *mss, int32_t tb_data_out[AW_TBUS_NUM_SAMPLES], int tbus_block_id, uint32_t signal_id, int fp_lsb, int fp_msb, int fp_si);

/**
 * Helper function to convert data to signed.
 *
 * Block Type: N/A
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'data' : unsigned data to convert to signed
 *
 *
 * Return:
 *    signed version of ADC data
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    Helper function.
 *
 */
int aw_pmd_16ln_convert_data_signed(uint32_t data);

/**
 * Core tracebuffer read function. Should not be used on its own, it is used
 * internally on specific data probe functions like aw_pmd_read_tracebuffer_adc.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'tb_data' : raw tracebuffer data
 *    'tb_size' : size of the data that the tracebuffer will be capturing, this
 *                is a fixed design parameter.
 *
 * Return:
 *    signed version of ADC data
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    Helper function.
 *
 */
int aw_pmd_16ln_read_tracebuffer(mss_access_t *mss, uint32_t *tb_data, int tb_size);

/**
 * Capture ADC Data from the On Chip Logic Analyzer. This data is captured
 * through the tracebuffer.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'num_samples' : the number of samples to collect
 *
 * Return:
 *    'adc_data' : a pointer to the data structure where the ADC data will be
 *                 be captured. This is an array of arrays where the first index
 *                 is the ADC Number (typically 0-63) and the second index is
 *                 the sample number.
 *
 * Constraints:
 *    Use only after power up has completed successfully.
 *
 * Typical Application Usage:
 *    Used for debug and analysis.
 *
 */
int aw_pmd_16ln_read_tracebuffer_adc(mss_access_t *mss, int num_samples, int adc_data[][AW_NUM_BRANCHES]);

/**
 * Get the TBUS configuration settings
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'block_id' : the block id corresponding to the digital block to pull data
 *                 from
 *    'signal_id' : the signals of the corresponding block to pull from
 *    'trigger' : should always be 1
 *    'continuous_sample' : should always be 1
 *
 * Return:
 *
 * Constraints:
 *    Use after DBE is powered up and respective link blocks are powered on.
 *
 * Typical Application Usage:
 *    Used for debug and analysis.
 *
 */
int aw_pmd_16ln_tbus_client_config_get(mss_access_t *mss, uint32_t *block_id, uint32_t *signal_id, uint32_t *trigger, uint32_t *continuous_sample);

/**
 * Configure the TBUS to capture data from various internal digital blocks and
 * store it in the tracebuffer.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'block_id' : the block id corresponding to the digital block to pull data
 *                 from
 *    'signal_id' : the signals of the corresponding block to pull from
 *    'trigger' : should always be 1
 *    'continuous_sample' : should always be 1
 *
 * Return:
 *
 * Constraints:
 *    Use after DBE is powered up and respective link blocks are powered on.
 *
 * Typical Application Usage:
 *    Used for debug and analysis.
 *
 */
int aw_pmd_16ln_tbus_client_config_set(mss_access_t *mss, uint32_t block_id, uint32_t signal_id, uint32_t trigger, uint32_t continuous_sample);

/**
 * Baseline functionality to pull signal information from within the demapper
 * block and store it in the tracebuffer.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'ffe_data' : ffe_data is assumed to be a 2D Array where the first index
 *                 is the branch and the second index is the sample number.
 *                 This is inherently tied to the size of the tracebuffer.
 *                 At very minimum the array must be allocated for:
 *                     uint32_t ffe_data[2][AW_TB_SIZE];
 *    'branch_id' : Branch ID will pull in two branches of FFE data at the same
 *                  time. For example, if branch_id = 0, then FFE data for
 *                  branches 0 and 1 will be collected. If branch_id = 3, then
 *                  FFE data will be collected for branches 6 and 7, etc. After
 *                  branch_id 31 (ie. starting at branch_id 32), the signal data
 *                  becomes quantizer error from the nearest target. It follows
 *                  the same capture layout as described above, but just
 *                  starting at ID 32 and going to ID 63. You won't have to
 *                  program above 32 manually as the function will take care
 *                  of this for you.
 *    'is_ffe' : Used in determining if we want to capture FFE data, or
 *               quantizer error data.
 *
 * Return:
 *
 * Constraints:
 *    Use after DBE is powered up and respective link blocks are powered on.
 *
 * Typical Application Usage:
 *    This method should not be used directly, it will be called internally by
 *    aw_pmd_read_tracebuffer_ffe or aw_pmd_read_tracebuffer_quantizer_err.
 *
 */
int aw_pmd_16ln_read_tracebuffer_demapper(mss_access_t *mss, int demapper_data[2][AW_TBUS_NUM_SAMPLES], uint32_t branch_id, uint32_t is_ffe);

/**
 * Baseline functionality to pull signal information for the FFE and store it
 * in the tracebuffer. It will pull AW_TB_SIZE (54 in a config with 18 tap FFE
 * + 64 ADCS) continuous samples of data.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'num_samples' : the number of samples to collect
 *    'branch_id' : Branch ID will pull in two branches of FFE data at the same
 *                  time. For example, if branch_id = 0, then FFE data for
 *                  branches 0 and 1 will be collected. If branch_id = 3, then
 *                  FFE data will be collected for branches 6 and 7, etc.
 *
 * Return:
 *    'ffe_data' : ffe_data is assumed to be a 2D Array where the first index
 *                 is the branch and the second index is the sample number.
 *                 This is inherently tied to the size of the tracebuffer.
 *                 At very minimum the array must be allocated for:
 *                     uint32_t ffe_data[2][AW_TB_SIZE];
 *
 * Constraints:
 *    Use after DBE is powered up and respective link blocks are powered on.
 *
 * Typical Application Usage:
 *    Used for debug and analysis.
 *
 */
int aw_pmd_16ln_read_tracebuffer_ffe(mss_access_t *mss, int num_samples, int32_t *ffe_data, int branch_id);

/**
 * Baseline functionality to pull signal information for the DLPF ITR and store
 * it in the tracebuffer. It will pull a variable number of samples based on
 * the input argument. It is recommended that the user specify the number of
 * samples in a multiple of the macro AW_TBUS_NUM_SAMPLES as this is the size
 * of the buffer within the design. If provided in a non multiple, samples will
 * be discarded.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'num_samples' : the number of samples to collect
 *
 * Return:
 *    'itr_dlpf_int' : return data array containing the sampled data from the
 *                     buffer
 *
 * Constraints:
 *    Use after DBE is powered up and respective link blocks are powered on.
 *
 * Typical Application Usage:
 *    Used for debug and analysis.
 *
 */
int aw_pmd_16ln_read_tracebuffer_itr_dlpf_int(mss_access_t *mss, int num_samples, int32_t *itr_dlpf_int);

/**
 * Baseline functionality to pull signal information for the quantizer error
 * and store it in the tracebuffer. It pulls the nearest targets quantizer
 * error. It will pull AW_TB_SIZE (54 in a config with 18 tap FFE + 64 ADCS)
 * continuous samples of data.
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'num_samples' : the number of samples to collect
 *    'branch_id' : Branch ID will pull in two branches of data at the same
 *                  time. For example, if branch_id = 0, then the data for
 *                  branches 0 and 1 will be collected. If branch_id = 3, then
 *                  the data will be collected for branches 6 and 7, etc.
 *
 * Return:
 *    'qztr_err_data' : is assumed to be a 2D Array where the first index
 *                      is the branch and the second index is the sample number.
 *                      This is inherently tied to the size of the tracebuffer.
 *                      At very minimum the array must be allocated for:
 *                         uint32_t qztr_err_data[2][AW_TB_SIZE];
 *
 * Constraints:
 *    Use after DBE is powered up and respective link blocks are powered on.
 *
 * Typical Application Usage:
 *    Used for debug and analysis.
 *
 */
int aw_pmd_16ln_read_tracebuffer_quantizer_err(mss_access_t *mss, int num_samples, int32_t *qztr_err_data, int branch_id);

/**
 * Setup Spread Spectrum Clocking (SSC) for the Common LC PLL.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'lsref_mhz' : the low speed reference clock frequency value in MHz
 *    'lcpll_mhz' : the LC PLL frequency value in MHz
 *    'ppm_downspread' : the amount of PPM to downspread/modulate the LC PLL
 *
 * Return:
 *
 * Constraints:
 *    Use when the PHY is in reset after loading the FW and before triggering
 *    power up. Meant for usage with PCIe rates only.
 *
 * Typical Application Usage:
 *    Used after loading the firmware to program the SSC configuration.
 *
 */
int aw_pmd_16ln_cmn_ssc_config(mss_access_t *mss, double lsref_mhz, double lcpll_mhz, int ppm_downspread);

/**
 * Enable SSC for the Common LC PLL.
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : 1 - enabled, 0 - disabled
 *
 * Return:
 *
 * Constraints:
 *    Use when the PHY is in reset after loading the FW and before triggering
 *    power up. Meant for usage with PCIe rates only.
 *
 * Typical Application Usage:
 *    Used after loading the firmware to program the SSC configuration.
 *
 */
int aw_pmd_16ln_cmn_ssc_en_set(mss_access_t *mss, uint32_t en);

/**
 * Enable DIV2 SRAM clk - Set
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : 1 - enabled (DIV2 ~ 450MHz) , 0 - disabled(DIV1 ~ 900MHz)
 *
 * Return:
 *
 * Constraints:
 *    Use when the PHY is in reset after loading the FW and before triggering CMN
 *    power up.
 *
 * Typical Application Usage:
 *    Use when the PHY is in reset after loading the FW and before triggering CMN
 *    power up
 */
int aw_pmd_16ln_sram_clk_div_set(mss_access_t *mss, uint32_t en);


/**
 * Read SRAM clk DIV ratio - Get
 *
 * CMN Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : 1 - enabled (DIV2 ~ 450MHz) , 0 - disabled(DIV1 ~ 900MHz)
 *
 * Return:
 *
 * Constraints:
 *    Use when the PHY is in reset after loading the FW and before triggering CMN
 *    power up.
 *
 * Typical Application Usage:
 *    Use when the PHY is in reset after loading the FW and before triggering CMN
 *    power up.
 */
int aw_pmd_16ln_sram_clk_div_get(mss_access_t *mss, uint32_t *en);

/**
 * Read Enable/Disable NEP loopback
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'nep_loopback_enable' : 1 - enabled , 0 - disabled
 *
 * Return:
 *
 * Constraints:
 *    Use prior to state change
 *
 *
 * Typical Application Usage:
 *    Use prior to state change
 *
 */
int aw_pmd_16ln_nep_loopback_get(mss_access_t *mss, uint32_t *nep_loopback_enable);

/**
 * Write Enable/Disable NEP loopback
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'nep_loopback_enable' : 1 - enabled , 0 - disabled
 *
 * Return:
 *
 * Constraints:
 *    Use prior to state change
 *    .
 *
 * Typical Application Usage:
 *    Use prior to state change
 *
 */
int aw_pmd_16ln_nep_loopback_set(mss_access_t *mss, uint32_t nep_loopback_enable);

/**
 * This configures the burst mode analyzer, which operates per 128-bit data window, in BIST timer mode.
 * Inputs burst_threshold and burst_mode configures the triggering condition.
 * Once triggered.
 * a). The burst error counter (burst_err_found_cnt_nt) increments
 * b). The expected and received 128-bit window is latched to registers expect_data*_nt and rec_data*_nt
        i.  ONLY the last qualified condition will be stored in expected and received registers,
            e.g. as the hw is running, it will keep overwriting as the condition is satisfied
        ii. Wait until the timer expires before looking at the expected and received read-only registers
 *  XORs of the expected and received data show the locations of bit errors.
 *
 *  Note: PAM4 will typically only have 1 bit error in its symbol, so the XOR pattern for PAM4
 *  may look like this: 0 0 0 1 0 1 0 0 <- this would be consecutive 2 symbol errors
 *
 * Example usage:
        1.  Put RX Bist into timer mode
        2.  rx_databist_top_reg,burst_err_symbol_mode_nt = 1            # PAM mode
        3.  rx_databist_top_Reg,burst_mode_nt=0..7                      # Specifies the # of consecutive bits (0 means single error, 7 means 8 consecutive symbol errors),
        4.  rx_databist_top_reg,burst_error_bits_threshold_nt=0..127    # Symbol errors greater than this value in 128-bit window, required to qualify condition.
        5.  Readouts:
            a.  rx_databist_top_rdreg,burst_err_found_cnt               # Indicates how many cycles (each cycle is processing 128bits) in the BIST timer duration satisfied the programmed conditions.
                                                                            E.g. in Timer Mode = 1 cycle, this register can be up to 1. If Timer Mode=10, this register can be up to 10.
            b.  rx_databist_top_Rdreg,expect_data0/1/2/3_nt             # When conditions are satisfied, the expected bit pattern from BIST is latched here
            c.  rx_databist_top_Rdreg,rec_data0/1/2/3_nt                # When conditions are satisfied, the received bit pattern from BIST is latched here
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'pam_mode' : 1 - PAM , 0 - NRZ
 *    'burst_threshold' : number of symbol errors in each 128-bit window (0 means single error, 7 means 8 consecutive symbol errors)
 *    'burst_mode' : number of consecutive symbol errors in each 128-bit window
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *   Debug API for burst errors , use after PHY is powered up and recieving data.
 *
 */
int aw_pmd_16ln_rx_burst_mode_config_set(mss_access_t *mss, uint32_t pam_mode, uint32_t burst_threshold, uint32_t burst_mode);

/**
 * Readback burst mode analyzer configuration
 *
 * RX Lane Function
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'pam_mode' : 1 - PAM , 0 - NRZ
 *    'burst_threshold' : number of symbol errors in each 128-bit window (0 means single error, 7 means 8 consecutive symbol errors)
 *    'burst_mode' : number of consecutive symbol errors in each 128-bit window
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Debug API for burst errors , use after PHY is powered up and recieving data.
 *
 */
int aw_pmd_16ln_rx_burst_mode_config_get(mss_access_t *mss, uint32_t *pam_mode, uint32_t *burst_threshold, uint32_t *burst_mode);
/**
 * Returns number of burst errors found.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'burst_err_cnt' : Total number of burst errors found when burst error measurement enabled
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Debug API for burst errors , use after PHY is powered up and recieving data.
 *
 */
int aw_pmd_16ln_rx_burst_err_cnt_get(mss_access_t *mss , uint32_t *burst_err_cnt);

/**
 * Returns Digref TX/RX block div values from block clock frequencies
 *
 * RX Lane Function
 *
 * Args:
 *    'a': First real frequency value.
 *    'b': Second real frequency value.
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Digref LCM calculation to find block div values.
 *
 */
uint64_t aw_pmd_16ln_digref_block_div_finder(double a, double b);

/**
 * Register to perform a 2bit swap. Can be used to make ones own gray code for TX
 *   Gray Code mapping for received values:
 *     00:[1:0]
 *     01:[3:2]
 *     10:[5:4]
 *     11:[7:6]
 * Args:
 *    'mss': Object containing driver methods.
 *    'gray_code_map' : Gray code mapping for recieved values
 *
 *
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 *
 * Typical Application Usage:
 *    When gray code is enabled to set the gray code mapping for TX
 *
 */
int aw_pmd_16ln_tx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map);

/**
 * Register to perform a 2bit swap. Can be used to make ones own gray code for RX
 *   Gray Code mapping for received values:
 *     00 (EL3):[1:0]
 *     01 (EL1):[3:2]
 *     10 (EH1):[5:4]
 *     11 (EH3):[7:6]
 * Args:
 *    'mss': Object containing driver methods.
 *    'gray_code_map' : Gray code mapping for recieved values
 *
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 *
 * Typical Application Usage:
 *    When gray code is enabled to set the gray code mapping.
 *
 */
int aw_pmd_16ln_rx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map);


/**
 * Register to readback RX gray code configuration
 *   Gray Code mapping for received values:
 *     00 (EL3):[1:0]
 *     01 (EL1):[3:2]
 *     10 (EH1):[5:4]
 *     11 (EH3):[7:6]
 * Args:
 *    'mss': Object containing driver methods.
 *    'gray_code_map' : Gray code mapping for recieved values
 *
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 *
 * Typical Application Usage:
 *   When Gray code is enabled to readback the gray code mapping for RX
 *
 */

int aw_pmd_16ln_rx_gray_code_mapping_get(mss_access_t *mss, uint8_t *gray_code_map);

/**
 * Register to readback gray code configuration for TX
 *   Gray Code mapping for received values:
 *     00:[1:0]
 *     01:[3:2]
 *     10:[5:4]
 *     11:[7:6]
 * Args:
 *    'mss': Object containing driver methods.
 *    'gray_code_map' : Gray code mapping for recieved values
 *
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 *
 * Typical Application Usage:
 *   When Gray code is enabled to readback the gray code mapping for TX
 *
 */
int aw_pmd_16ln_tx_gray_code_mapping_get(mss_access_t *mss, uint8_t *gray_code_map);

/**
 * Register to set number of SRAM RD pipeline stages.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'stages' : Number of pipeline stages
 *
 *
 * Return:
 *
 * Constraints:
 *    Must be programmed prior to any SRAM accesses.
 *
 *
 * Typical Application Usage:
 *   Programming number of pipeline stages to SRAM.
 *
 */
int aw_pmd_16ln_rd_data_pipeline_stages_set(mss_access_t *mss, uint32_t stages);


/**
 * Readback number of SRAM RD pipeline stages.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'stages' : Number of pipeline stages
 *
 *
 * Return:
 *
 * Constraints:
 *   Null
 *
 *
 * Typical Application Usage:
 *   Reading back the number of pipeline stages to SRAM.
 *
 */
int aw_pmd_16ln_rd_data_pipeline_stages_get(mss_access_t *mss, uint32_t* stages);

/**
 * Debug API for burst errors - Compares the expected and received BIST pattern data.
 * XOR output returned in an array.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'exp_data' : Expected bit pattern from BIST is latched here
 *    'recv_data' : Received bit pattern from BIST is latched here
 *    'xor_data' : exp_data ^ recv_data
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Debug API for burst errors , use after PHY is powered up and recieving data.
 *    Refer to burst_mode_config_set API description for more details
 *
 */
int aw_pmd_16ln_rx_burst_mode_stats_get(mss_access_t *mss, uint32_t exp_data[], uint32_t rec_data[], uint32_t xor_data[]);

/**
 * Enables RX C0 adaptation
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'en' : Enable flag, 1 = enable, 0 = disable
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Null
 *
 */
int aw_pmd_16ln_rx_c0_adapt_set(mss_access_t *mss, uint32_t en);

/**
 * Sets TX perf power mode
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'perf_mode' : power mode to be set.
 *
 * Return:
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Sets TX power mode before waking up
 *
 */
int aw_pmd_16ln_tx_perf_settings(mss_access_t *mss, uint8_t perf_mode);

/**
 * Forces the bandgap startup circuitry to power up
 *
 * Args:
 *    'mss': Object containing driver methods.
 *
 * Return:
 *
 * Constraints:
 *
 * Typical Application Usage:
 *    Only use with discretion from design team.
 */
int aw_pmd_16ln_cmn_bias_bandgap_force_startup(mss_access_t *mss);


/**
 * Gets the number of active branches in the RX
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'active_branches' : the number of active branches in the RX
 *
 * Return:
 *
 * Constraints:
 *
 * Typical Application Usage:
 *   Used to aid with data collection
 *
 */
int aw_pmd_16ln_rx_active_branches_get(mss_access_t *mss, uint32_t * active_branches);

/**Readback vga cap adapt settings.
*
* Args:
*    'mss': Object containing driver methods.
*    'opts':
*        'en':
*             0 - vga_cap adapt disabled
*             1 - vga_cap adapt enabled
*        'vga_cap': Forced vga_cap code when 'en' is 0. Ranges from 0-3.
*        'use_custom_takeover_ratio':
*             0 - use the custom takeover ratio, as specified by 'takeover_ratio'
*             1 - use takeover ratio hardcoded in firmware
*        'custom_takeover_ratio': custom takeover ratio converted to format that FW accepts
*        'custom_nyq_mask': an unsigned 8-bit integer whose bits [7:0] represent whether the nyquist energies for [1p00 .. p125] respectively will be used
*
*
* Return:
*    Null
*
* Constraints:
*    Null
*
* Typical Application Usage:
*    Used to readback VGA cap adapt settings used by link training.
*/
int aw_pmd_16ln_rx_vga_cap_adapt_get(mss_access_t *mss, vga_opt_t *opts);

/**Sets values used for vga_cap adaptation. If vga_cap adapt is disabled, vga_cap code will default to opts['vga_cap'].
*    If vga_cap adapt is enabled, initial vga_cap value will default to 0.
*
* Args:
*    'mss': Object containing driver methods.
*    'opts':
*        'en':
*            0 - vga_cap adapt disabled
*            1 - vga_cap adapt enabled
*        'vga_cap': Forced vga_cap code when 'en' is 0. Ranges from 0-10.
*        'use_custom_takeover_ratio':
*            0 - use the custom takeover ratio, as specified by 'custom_takeover_ratio'
*            1 - use takeover ratio hardcoded in firmware
*        'custom_takeover_ratio': custom takeover ratio converted to format that FW accepts
*        'custom_nyq_mask': an unsigned 8-bit integer whose bits [7:0] represent whether the nyquist energies for [1p00 .. p125] respectively will be used
*
*Lookup code |  vga_cap1  |  vga_cap2
*++++++++++++++++++++++++++++++++++++
*          0 |    0       |    0
*          1 |    4       |    0
*          2 |    8       |    0
*          3 |    12      |    0
*          4 |    16      |    0
*          5 |    20      |    0
*          6 |    20      |    4
*          7 |    20      |    8
*          8 |    20      |    12
*          9 |    20      |    16
*          10|    20      |    20
*
* Return:
*    Null
*
* Constraints:
*    Power State: Don't care.
*    Rate State: Don't care.
*    Link Training State: Before EqEval.
*    Effect: Takes effect immediately.
*
* Typical Application Usage:
*     vga_cap adaptation is usually enabled in link training.
*/
int aw_pmd_16ln_rx_vga_cap_adapt_set(mss_access_t *mss, vga_opt_t *opts);

/**Sets values for dfe ratio (post1/main cursor ratio). Bt default, dfe ratio is disabled. If enabled, the value of the 
*   dfe ratio can either be auto-determined (via use_auto_lookup) or user-set (via custom_dfe_ratio)
*
* Args:
*    'mss': Object containing driver methods.
*    'enable': 1 to enable dfe ratio; 0 to disable 
*    'use_auto_lookup_dfe_ratio': 1 to enable auto lookup; 0 to disable auto lookup and use 'custom_dfe_ratio'.
*                                 Takes into effect only if 'enable' is True. Will use vga cap settings to select dfe ratio. 
*    'custom_dfe_ratio': takes into effect only if 'enable' is True and 'use_auto_lookup_dfe_ratio' is False. User
*                        sets the dfe ratio. This number should be between -1 and 1
*
* Return:
*    Null
*
* Constraints:
*    Power State: Don't care.
*    Rate State: Don't care.
*    Link Training State: Before EqEval.
*    Effect: Takes effect immediately.
*
* Typical Application Usage:
*     Setting a dfe ratio might improve performance 
**/
int aw_pmd_16ln_rx_dfe_ratio_set(mss_access_t *mss, uint32_t enable, uint32_t use_auto_lookup_dfe_ratio, double custom_dfe_ratio);

/**
 * Get dfe ratio settings
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'enable' : returns whether dfe ratio is enabled or not
 *    'use_auto_lookup_dfe_ratio' : determines whether lookup table is used to automatically deduce dfe ratio. This value
 *                                  is meaningful only if 'enable' is 1
 *    'custom_dfe_ratio' : defined dfe ratio. This value is only meaningful is 'enable' is 1 and 'use_auto_lookup_dfe_ratio' is 0
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *
 * Typical Application Usage:
 *    Used to get dfe ratio settings.
 *
 **/
int aw_pmd_16ln_rx_dfe_ratio_get(mss_access_t *mss, uint32_t* enable, uint32_t* use_auto_lookup_dfe_ratio, double* custom_dfe_ratio);

/**Gets the fw version info into the structure and prints this info
*
* Args:
*   'version_st': Object conatining the version info
*       'version_major': Major version number
*       'version_minor': Minor version number
*       'version_patch': Patch version number
*
* Return:
*   Null
*
* Typical Application Usage:
*     Use to get current version numbers
*/
int aw_pmd_16ln_fw_version_get(mss_access_t *mss, aw_version_t *version_st);



/**Reads the CDR lock and stores it into the varibale passed by the pointer
*
* Args:
*    'mss': Object containing driver methods.
*    'rx_cdr_lock': RX DATA VALID to be got
*
* Return:
*   Null
*/
int aw_pmd_16ln_rx_cdr_lock_get(mss_access_t *mss, uint32_t *rx_cdr_lock);

/** Read LCPLL clk PPM offset for a given lane. Desired PPM accuracy is 1/2**timing_window-1.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'timing_window': Sampling measurement window to be `2**timing_window-1` clock cycles
 *    'timeout_us': polling iteration limit in us
 *    'refclk_freq' : Reference clock frequency (in Hz)
 *
 * Return:
 *    'err':
 *        0 - Polling successful, ppm value is valid
 *        1 - Polling unsuccesful, ppm value is invalid
 *    'lcpll_ppm': Return LCPLL clock ppm offset value
 *    'vco_freq':  Return LCPLL VCO frequency
 *
 * Constraints:
 *    Used when PLL is locked, in P1 or P0 state.
 *
 * Typical Application Usage:
 *    Use this function to report the PPM offset between reference clock and VCO clock as well as VCO frequency
 */
int aw_pmd_16ln_lcpll_vco_counter_get(mss_access_t *mss, uint32_t timing_window, double *lcpll_ppm, double *vco_freq, double refclk_freq);

#if 0
/** Used to decode bist pattern type
 *
 * Args:
 *      'bist_pattern_encoded': encoded pattern
 *
 * Return:
 *      (char) decoded pattren
 *
 */  
char * aw_bist_pattern_decoder (uint32_t bist_pattern_encoded);
#endif

/**Get CDR offset, direction, and use custom CDR offset enable 
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *
 * Return:
 *    'use_custom_cdr_offset' : Use custom CDR offset enable
 *          0 - Disable using custom CDR offset
 *          1 - Enable using custom CDR offset 
 *    'cdr_offset' : CDR offset value (8bits)
 *    'cdr_dir' : CDR direction
 *          0 - Negative direction
 *          1 - Positive direction
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *   Should be used to read back CDR offset/direction settings 
 */
int aw_pmd_16ln_rx_cdr_offset_get(mss_access_t *mss, uint32_t *use_custom_cdr_offset, uint32_t *cdr_offset, uint32_t *cdr_dir);


/**Powers on/off the Atest ADC
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'en' : 0 - Powers down the Atest ADC
 *           1 - Powers on the Atest ADC
 *
 * Return:
 *   Null
 *
 * Constraints:
 *    Can be used anytime.
 *
 * Typical Application Usage:
 *   Needs to be turned on the to use the Atest ADC
 */
int aw_pmd_16ln_atest_adc_power(mss_access_t *mss, uint32_t en);

/**Captures bandgap voltage values for temperature calculation and calibration
 *
 * Args:
 *    `mss`        : Object containing driver methods.
 *    'iterations' : Number of times voltages values are collected
 *
 * Return:
 *    'mean_data'  : Mean of bandgap voltage values
 *
 * Constraints:
 *    PHY must be initialized.
 *    CMN must be in power state 1.
 *    IR must be compensated at ATEST for VDD, VDDL and VDDH.
 *    ADC must be powered up.
 *
 * Typical Application Usage:
 *   Used to capture calibration data at a known temperature
 */
int aw_pmd_16ln_atest_adc_temp_capture(mss_access_t *mss, uint32_t iterations, aw_adc_temp_data_t *mean_data);

/**Captures bandgap voltage values and calculates a temperature based on the given method and calibration data
 *
 *
 * Args:
 *    `mss`          : Object containing driver methods.
 *    'calibration'  : Previously determined calibration values. data_2 and temp_2 are not required
 *                     if using single point methods
 *    'method'       : Please see aw_adc_temp_method_t
 *    'iterations'   : Number of times voltages values are collected
 *
 * Return:
 *    'measured_data': Raw bandgap voltages used for temperature calculation
 *    'result_temp'  : Calculated temperature value in degrees C
 *
 * Constraints:
 *    PHY must be initialized.
 *    CMN must be in power state 1.
 *    IR must be compensated at ATEST for VDD, VDDL and VDDH.
 *    ADC must be powered up.
 *
 * Typical Application Usage:
 *   Used to determine silicon temperature
 */
int aw_pmd_16ln_atest_adc_temp_get(mss_access_t *mss, aw_adc_temp_calibration_t *calibration,aw_adc_temp_method_t method, uint32_t iterations,aw_adc_temp_data_t *measured_data,float *result_temp);

int aw_pmd_16ln_sris_set(mss_access_t *mss, uint32_t en);
int aw_pmd_16ln_sris_get(mss_access_t *mss, uint32_t *en);

#endif // AW_ALPHACORE_H
