/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserialzier) slices on ASIC Sbus rings;  Diagnostic */
/* routines */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SerDes diagnostic functions. */

#ifndef AVAGO_SERDES_DIAG_H_
#define AVAGO_SERDES_DIAG_H_

EXT void avago_serdes_print_state_table(Aapl_t *aapl, Avago_addr_t *addr_struct);

#if AAPL_ENABLE_DIAG
/**@brief Select when BTC are done for a serdes_dfe_repeat call */
typedef enum
{
    AVAGO_DFE_REPEAT_INIT_ALL,  /**< For every loop */
    AVAGO_DFE_REPEAT_INIT_ONCE, /**< On first loop only */
    AVAGO_DFE_REPEAT_INIT_NEVER /**< Never */
} Avago_dfe_repeat_init_sel_t;

/**@brief Select when BTC are done for a serdes_dfe_repeat call */
typedef enum
{
    AVAGO_DFE_REPEAT_BTC_NONE,  /**< Never */
    AVAGO_DFE_REPEAT_BTC_FIRST, /**< After first DFE tune only */
    AVAGO_DFE_REPEAT_BTC_LAST,  /**< After last loop of DFE tune only */
    AVAGO_DFE_REPEAT_BTC_ALL    /**< After every DFE tune */
} Avago_dfe_repeat_btc_sel_t;

/**@brief Control serdes_dfe_repeat flow. */
typedef struct
{
    uint loops;             /**< Number of dfe tunes to execute */
    BOOL multi_line;        /**< Set to TRUE to print data over multiple lines rather single line */
    uint timeout;           /**< Tuning timeout, sec */
    BOOL use_file;          /**< TRUE to output to file instead of memory log */
    const char *file_name;  /**< File to store output if use_file is TRUE */

    /* Chip Configuration */
    float refclk;           /**< REFCLK frequency */
    float voltage;          /**< Analog Supply Voltage */
    int   temp;             /**< Part Temperature, milli-degrees C */
    char  channel_name[20]; /**< Channel Name */

    /* SerDes Initialization control */
    BOOL disable_other_slices;              /**< Issue a Spico Enable Off for all SerDes before loops start */
    Avago_dfe_repeat_init_sel_t init;       /**< Control when a SerDes Init is done */
    Avago_serdes_line_encoding_t encoding;  /**< Control SerDes encoding */
    BOOL init_tx;                           /**< Control whether to modify tx */
    BOOL init_rx;                           /**< Control whether to modify rx */
    uint width_mode;                        /**< Value for avago_serdes_init */
    BOOL phase_cal;                         /**< Value for avago_serdes_init */
    uint divider;                           /**< Divider value to configure serdes to */
    BOOL signal_ok_en;                      /**< Set to FALSE to disable electrical idle check (default:TRUE) */
    Avago_serdes_rx_term_t term;            /**< Rx termination configuration */
    Avago_serdes_tx_data_sel_t prbs;        /**< PRBS sequence to set Transmitter to */

    /* Tx Eq Control */
    int  pre;   /**< Pre-cursor setting to use, number of Initial Precur loops to issue in pmd mode */
    int  pre2;  /**< Pre-cursor 2 setting to use, number of Initial Precur loops to issue in pmd mode */
    int  pre3;  /**< Pre-cursor 3 setting to use, number of Initial Precur loops to issue in pmd mode */
    uint atten; /**< Attenuator setting to use, ignored in pmd mode */
    int  post;  /**< Post-cursor setting to use, number of Initial Precur loops to issue in pmd mode */

    /* Rx Eq Control */
    BOOL dfe_tap_disable;           /*< Set to true to set a DFE tap disable mask */
    int dfe_tap_disable_mask;       /*< Mask written to dfe_tap_disable */
    BOOL pcie_mode;                 /**< Set to TRUE to configure serdes in PCIe mode (default: FALSE) */
    BOOL pmd_train;                 /**< Set to TRUE to execute PMD link training (default: FALSE) */
    BOOL tune_once;                 /* < set true to only tune once. Validates eye measurement repeatability (default: FALSE) */
    BOOL tune_never;                /* < set true to not tune and only capture eyes. Validates eye measurement repeatability (default: FALSE) */
    Avago_serdes_pmd_config_t *pmd_config;/**< Pointer to PMD CONFIG struct to configure pmd link training opeartion */
    Avago_serdes_dfe_state_t *dfe_state;  /**< Pointer to DFE STATE struct for configure dfe tune mode */

    /* Validation Control */
    BOOL enable_error_check;     /**< Controls doing an error count after tuning completes */
    uint error_loops;            /**< Number of error accumulations to run after tuning */
    uint error_threshold;        /**< Error threshold to stop accumulations on if exceeded */
    bigint btc_dwell;            /**< Sets how long the eye capture should dwell for */
    Avago_dfe_repeat_btc_sel_t btc_mode; /**< Control when Bath-Tub Curve information is gathered */

    /* Temperature Sensor Configuration */
    uint rx_temp_addr;      /**< Sbus Rx addr for temperature sensor */
    BOOL rx_temp_configure; /**< Control whether or to configure the thermal sensor divider */
    uint temp_sensor_cnt;   /**< Number of temp sensors to read */

} Avago_serdes_dfe_repeat_t;


EXT void avago_serdes_dfe_repeat(Aapl_t *aapl, uint sbus_addr, Avago_serdes_dfe_repeat_t *config);
EXT Avago_serdes_dfe_repeat_t *avago_serdes_dfe_repeat_construct(Aapl_t *aapl);
EXT void avago_serdes_dfe_repeat_destruct(Aapl_t *aapl, Avago_serdes_dfe_repeat_t *config);

EXT void avago_serdes_kr_logger  (Aapl_t *aapl, uint sbus_addr, uint int_kr_data,  const char *tablefile);
EXT void avago_serdes_pcie_logger(Aapl_t *aapl, uint sbus_addr, uint int_dfe_data, const char *tablefile);
EXT void avago_serdes_sas_addrs(Aapl_t *aapl,
                                const char *tablefile,
                                int *ref1_km1,
                                int *ref2_km1,
                                int *noeq_km1,
                                int *ref1_k0,
                                int *ref2_k0,
                                int *noeq_k0,
                                int *ref1_kp1,
                                int *ref2_kp1,
                                int *noeq_kp1,
                                int *pmd_km1,
                                int *pmd_k0,
                                int *pmd_kp1);
EXT void avago_serdes_sas_logger (Aapl_t *aapl, uint sbus_addr, uint int_sas_data,  const char *tablefile, int sas_iterations, int log_type, int log_sas_iter, int log_steps);
EXT void avago_serdes_dfe_logger (Aapl_t *aapl, uint sbus_addr, uint int_dfe_data, const char *tablefile);


typedef enum
{
    AVAGO_SERDES_BSB_GND              = 0,
    AVAGO_SERDES_BSB_RX_F10           = 1,
    AVAGO_SERDES_BSB_RX_F20           = 2,
    AVAGO_SERDES_BSB_RX_F40_FIFO_CLK  = 3,
    AVAGO_SERDES_BSB_RX_CLK           = 4,
    AVAGO_SERDES_BSB_RX_FIFO_CLK      = 5,
    AVAGO_SERDES_BSB_RX_F66_CLK       = 6,
    AVAGO_SERDES_BSB_PCS6466_FIFO_CLK = 7,
    AVAGO_SERDES_BSB_TX_F10           = 8,
    AVAGO_SERDES_BSB_TX_F20           = 9,
    AVAGO_SERDES_BSB_TX_F40           = 0xa,
    AVAGO_SERDES_BSB_TX_FIFO_CLK      = 0xb,
    AVAGO_SERDES_BSB_SBUS_CLK         = 0xc,
    AVAGO_SERDES_BSB_SBUS_CLK_TEST    = 0xd,
    AVAGO_SERDES_BSB_TX_TEST_CLK      = 0xf,
    AVAGO_SERDES_BSB_REFCLK           = 0x11,
    AVAGO_SERDES_BSB_REFCLK_TEST      = 0x12,
    AVAGO_SERDES_BSB_TX_DIVX_CLK      = 0x14,
    AVAGO_SERDES_BSB_RX_DIVX_CLK      = 0x15,
    AVAGO_SERDES_BSB_DIVX_CLK_TEST    = 0x16,
    AVAGO_SERDES_BSB_TX_F10_CLK_VAR   = 0x17,
    AVAGO_SERDES_BSB_TX_F20_CLK_VAR   = 0x18,
    AVAGO_SERDES_BSB_TX_CLK_TEST      = 0x19,
    AVAGO_SERDES_BSB_LSSEL            = 0x1b,
    AVAGO_SERDES_BSB_RESET_COMPLETE   = 0x1c,
    AVAGO_SERDES_BSB_RX_PI_CLK        = 0x1e,
    AVAGO_SERDES_BSB_AVDD             = 0x1f
} Avago_serdes_bsb_clk_sel_t;

typedef enum
{
    AVAGO_SERDES_BSB_DISABLE,     /* Disables the BSB and sets passthru mode */
    AVAGO_SERDES_BSB_CLK,         /* send local clock select by AVAGO_SERDES_BSB_CLK_SEL */
    AVAGO_SERDES_BSB_PASSTHRU,    /* sends incoming BSB data to the next SerDes */
    AVAGO_SERDES_BSB_DMA,         /* send out DMA address live */
    AVAGO_SERDES_BSB_SBUS,        /* send out SBus address live */
    AVAGO_SERDES_BSB_CORE         /* send out data from ASIC core */
} Avago_serdes_bsb_mode_t;

EXT void avago_serdes_bsb_setup(Aapl_t *aapl, uint sbus_addr, Avago_serdes_bsb_mode_t bsb_mode, Avago_serdes_bsb_clk_sel_t bsb_clk_sel, int tx_en, int mem_addr, int mem_bit);
EXT void avago_serdes_set_bsb(Aapl_t *aapl, uint origin_addr, uint dest_addr, Avago_serdes_bsb_mode_t bsb_mode, Avago_serdes_bsb_clk_sel_t bsb_clk_sel, int mem_addr, int mem_bit);
EXT int avago_serdes_get_bsb(Aapl_t *aapl, uint sbus_addr, Avago_serdes_bsb_mode_t *bsb_mode, Avago_serdes_bsb_clk_sel_t *bsb_clk_sel, int *mem_addr, int *mem_bit);

typedef struct
{
    uint verbose;
    uint failed;

    uint crc;
    uint spico_running;
    uint ready;

    uint tx_invert_check;
    uint tx_invert;
    uint tx_out;

    uint rx_invert_check;
    uint rx_invert;
    uint loopback_check;
    uint loopback;
    uint elec_idle_check;
    uint signal_ok_check;
    uint errors_check;

} Avago_serdes_health_check_config_t;

EXT Avago_serdes_health_check_config_t *avago_serdes_health_check_config_construct(Aapl_t *aapl);
EXT void avago_serdes_health_check_config_destruct(Aapl_t *aapl, Avago_serdes_health_check_config_t *config);
EXT BOOL avago_serdes_health_check(Aapl_t *aapl, uint addr, Avago_serdes_health_check_config_t *config);

EXT void avago_sbus_speed_test(Aapl_t *aapl, uint sbus_addr, int cycles);
EXT BOOL avago_diag_mdio_rw_test(Aapl_t *aapl, uint prtad, int cycles);
EXT BOOL avago_bist(Aapl_t *aapl, uint reset, uint sbus_addr_in, uint divider, uint testmask, uint upgrade_warnings, int rom_size, const int *rom_ptr);
#if AAPL_ENABLE_FILE_IO
EXT BOOL avago_bist_file(Aapl_t *aapl, uint reset, uint sbus_addr_in, uint divider, uint testmask, uint upgrade_warnings, const char * firmware);
#endif /* AAPL_ENABLE_FILE_IO */

#if AAPL_ENABLE_EYE_MEASUREMENT || AAPL_ENABLE_ESCOPE_MEASUREMENT
EXT unsigned char avago_serdes_set_delay_vernier(Aapl_t *aapl, uint addr, int x);
EXT int avago_serdes_delay_cal(Aapl_t *aapl, uint addr, uint mode, bigint dwell, int *start, int *stop);
#endif

#if AAPL_ENABLE_FLOAT_USAGE
EXT float avago_serdes_get_ber( Aapl_t *aapl, Avago_addr_t *addr_struct, uint dwell, BOOL console, BOOL reset);
#endif

#endif /* AAPL_ENABLE_DIAG */
#endif /* AVAGO_SERDES_DIAG_H_ */
