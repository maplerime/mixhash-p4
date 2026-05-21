#ifndef AW_TYPES_INCLUDED
#define AW_TYPES_INCLUDED

#include <stdint.h>

//#include "aw_driver_sim.h"
//#include "aw_alphacore_csr_defines.h"
//#include "aw_alphacore_ip_defines.h"

#define CHECK(x) do { int err = (x); if (err) return err; } while (0)
#define NULL_CHECK(x) do { if(x == NULL) return AW_ERR_CODE_INVALID_ARG_VALUE; } while (0)

// note: this is defined in "ip_defines", but is the same for both 16ln and 4ln
#define AW_FFE_NUM_TAPS 18

/**
 * Forward Declare Access Struct.
 * 
 * This must be defined in the driver file to access the CSR space of the
 * AlphaCORE100 IP
 * 
 */ 
// typedef struct mss_access_s mss_access_t;

#ifndef __aw_err_code_enum
#define __aw_err_code_enum
/**
 * typedef aw_err_code_t  - Alphawave MSS Error Codes Enum
 */
typedef enum aw_err_code_e {
    AW_ERR_CODE_NONE = 0, // function success (no error)
    AW_ERR_CODE_POLL_TIMEOUT = 1, // function containing a poll did not successfully complete
    AW_ERR_CODE_INVALID_ARG_VALUE = 2, // invalid function argument value
    AW_ERR_CODE_WRITE_FAILURE = 3, // returned by pmd_write_field if unsuccessful
    AW_ERR_CODE_READ_FAILURE = 4, // returned by pmd_read_field if unsuccessful
    AW_ERR_CODE_BAD_STATE = 5, // if unexpected values occurred, usually within *_get functions
    AW_ERR_CODE_FUNC_FAILURE = 6, // function did not complete successfully
    AW_ERR_CODE_CHECK_FAILURE = 7 // function did not return the expected value
} aw_err_code_t;
#endif

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

typedef struct tx_hbridge_s {
    uint32_t msb; /**< 'msb': adjust swing for Tx output */
    uint32_t lsb; /**< adjust inner eye level */
    uint32_t bias_adj; /**< afe tx driver h-bridge bias adj */
    uint32_t rlm_ovr; /**< disable auto RLM tracking */
    uint8_t bias_adj_en; /**< Bit, indicating that bias adjust was requested */
    uint8_t rlm_ovr_en; /**< Bit, indicating that rlm_ovr was requested */
} tx_hbridge_t;

// AN NEWDEF return helper struct
typedef struct aw_an_newdef_s {
    uint32_t lane_num_u;
    uint32_t lt_sel_u;
    uint32_t pam_u;
    uint32_t timer_sel_u;
} aw_an_newdef_t;

// AN NEWDEF return struct
typedef struct aw_an_spec_s {
    uint32_t an_def_spec_width[28];    // array of 28 elements, containing readout values of eth_anlt_width_reg*[an_def_spec_width_*r]
    uint32_t an_def_spec_rate[28];     // array of 28 elements, containing readout values of eth_anlt_rate_reg*[an_def_spec_rate_*r]

    uint32_t an_def_an_rate;        // containing readout value of eth_an_pma_def_reg1[an_def_an_rate]

    aw_an_newdef_t newdef1;         // containing readout values of eth_an_newdef_reg*[newdef1_*_u]
    aw_an_newdef_t newdef2;         // containing readout values of eth_an_newdef_reg*[newdef2_*_u]
    aw_an_newdef_t newdef3;         // containing readout values of eth_an_newdef_reg*[newdef3_*_u]
} aw_an_spec_t;

typedef struct aw_lt_status_s {
    uint32_t lt_running; // 0 = not running, 1 = running
    uint32_t lt_done; // 0 = not done, 1 = done
    uint32_t lt_failure; // 0 - link training no fail, 1 - link training fail
    uint32_t lt_rx_ready; // 0 - local RX / remote TX not trained, 1 - local RX / remote TX trained
} aw_lt_status_t;

typedef enum aw_refclk_term_mode_e {
    AW_RC_HI_Z = 0, // REF CLK high impedance
    AW_RC_R50_SE = 1, // REF CLK 50 ohms per leg single ended to ground
    AW_RC_R100_DF = 2, // REF CLK 100 ohms differential
    // UNUSED = 3, // here for clarity
} aw_refclk_term_mode_t;

typedef enum aw_acc_term_mode_e {
    AW_ACC_HI_Z = 0,
    AW_ACC_TERM_VSS_AC = 1, // termination to vss, onchip AC coupling
    AW_ACC_TERM_FL_AC = 2, // floating termination, onchip AC coupling
    // RES1 = 3, // Reserved
    // RES2 = 4, // Reserved
    AW_ACC_TERM_VSS_DC = 5, // termination to vss, DC coupled
    AW_ACC_TERM_FL_DC = 6, // termination to vss, DC coupled
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
    AW_CM3 = 0, // pre-cursor 3
    AW_CM2 = 1, // pre-cursor 2
    AW_CM1 = 2, // pre-cursor 1
    AW_C0  = 3, // max elements
    AW_C1  = 4, // post-cursor 1
    AW_TXFIR_MAX_TAPS = 5
} aw_txfir_cfg_taps_t;

/**
 * 'CM3': c(-3) value (pre-cursor 3), up to 4 max, 3b
 * 'CM2': c(-2) value  (pre-cursor 2), up to 7 max, 3b
 * 'CM1': c(-1) value  (pre-cursor 1), up to 24 max, 6b
 * 'C0': c(0) value  (main cursor), up to 60 max, 6b
 * 'C1': c(1) value  (post-cursor 1), up to 24 max, 6b
 * 'MAIN_OR_MAX': 0 - c0 is main cursor, 1 - c0 is max elements
 */
typedef struct aw_txfir_config_s {
    uint32_t CM3;
    uint32_t CM2;
    uint32_t CM1;
    uint32_t C0;
    uint32_t C1;
    uint32_t main_or_max;
} aw_txfir_config_t;

/**
 * 'nes_post1': c(1) value (post-cursor 1), up to 15 max, 4b 
 * 'nes_c0': c(0) value, up to 15 max, 4b 
 */
typedef struct aw_analog_loopback_txfir_config_s {
    uint32_t nes_post1;
    uint32_t nes_c0;
} aw_analog_loopback_txfir_config_t;


/*
 * State Request Struct
 *
 * Same for TX and RX
 */
typedef enum aw_state_rate_e {
  AW_NRZ_1p25_2p5 = 0, /* 1.25/2.5 Gbps NRZ Auto-Neg */
  AW_NRZ_10p3125 = 1, /* 10.3125 Gbps NRZ (10G) */
  AW_NRZ_25p78125 = 2, /* 25.78125 Gbps NRZ (25G) */
  AW_NRZ_26p5625 = 3, /* 26.5625 Gbps NRZ (26G AUI) */
  AW_PAM4_53p125 = 4, /* 53.125 Gbps PAM4 (50G) */
  AW_NRZ_53p125 = 5, /* 53.125 Gbps NRZ (50G) */
  AW_PAM4_106p25 = 6, /* 106.25 Gbps PAM4 (100G) */
  AW_MAX_RATES = 7,
} aw_state_rate_t;

/*
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

/** AW is reserving this struct and it's corresponding function in this
 * release. If there is a usecase for it, it will be updated in a future
 * release. If not, it will be removed.
 */
//typedef struct lane_cfg_s {
//    RESERVED
// } lane_cfg_t;

typedef enum aw_pll_pstatus_e {
    AW_PWR_DOWN = 0, // PLL power down
    AW_PWR_UP = 1, // PLL power up
    AW_ST_CHNG = 2 // PLL State Changing
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
    *    Returns calibrated duty cycle code for d90 clock
     */
    uint32_t d90;
    /**
    *    Returns calibrated IQ phase code between d0 (I) and d90 (Q) clock
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
    AW_PATT_32_1S_32_0S = 15, // 32 1s and 32 0s repeating
    AW_BIST_PATTERN_MAX = 16 /* Always at the end */
} aw_bist_pattern_t;

typedef enum aw_bist_mode_e {
    AW_TIMER = 0, // will run for a number of defined cycles
    AW_DWELL = 1, // wall clock mode, doing a test longer than 80ms, this needs to be used
} aw_bist_mode_t;

typedef enum aw_eq_type_e {
    AW_EQ_FULL_DIR = 0, // Full EQ, Directional
    AW_EQ_EVAL_DIR = 1, // Eval Only, Directional
    AW_EQ_INIT_EVAL = 2, // Init Eval
    AW_EQ_CLEAR_EVAL = 3, // Clear Eval
    AW_EQ_FULL_FOM = 4, // Full EQ, FOM
    AW_EQ_EVAL_FOM = 5 // Eval Only, FOM
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

typedef enum aw_rx_ffe_tap_count_e {
    AW_FFE_11_TAPS = 0,
    AW_FFE_15_TAPS = 1,
    AW_FFE_18_TAPS = 2,
    AW_FFE_ALL_TAPS_ENABLED = 3
} aw_rx_ffe_tap_count_t;

typedef enum aw_rx_roaming_mode_e {
    AW_NO_ROAMING = 0,  // ignores window select
    AW_BANK1_ROAMING = 1, // ignores window select
    AW_BANK2_ROAMING = 2, // ignores window select
    AW_BANK1_BANK2_ROAMING = 3, // ignores window select
    AW_RESET_DEFAULT_ROAMING_MODE = 4, // places mode and window selects into por default values
    AW_MANUAL_WINDOW_SELECT = 5 // places mode and window selects into por default values
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

typedef enum {
    AW_TB_DCOFFSET = 0,
    AW_TB_TBUS     = 1,
    AW_TB_RAWADC   = 2,
} aw_pmd_tracebuffer_mode_t;


#endif // AW_TYPES_INCLUDED

