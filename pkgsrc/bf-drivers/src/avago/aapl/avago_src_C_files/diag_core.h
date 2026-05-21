/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) diagnostic/test support. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for diagnostic functions. */

#ifndef AVAGO_DIAG_H_
#define AVAGO_DIAG_H_

#if AAPL_ENABLE_DIAG

EXT void avago_diag_sbus_dump(Aapl_t *aapl, uint sbus_addr, BOOL bin_enable);
EXT int  avago_spico_diag(Aapl_t *aapl, uint addr, int cycles);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
#if AAPL_ENABLE_FILE_IO
EXT char *read_spico_file(Aapl_t *aapl, uint addr, const char *suffix);
#endif
EXT void avago_spico_diag_line(Aapl_t *aapl, uint addr, char *hexfile, char *str, int cycle);
EXT void avago_spico_single_step(Aapl_t *aapl, uint addr, uint state);
#endif

/** @brief Structure specifying what diagnostics to run. */
/**        Note that these functions are not all part of the core functionality. */
typedef struct
{
    BOOL sbus_dump;             /**< Dump SBus */
    BOOL serdes_init_only;      /**< Stop after running SerDes init */
    BOOL state_dump;            /**< Dump SerDes state information */
    BOOL pmd_debug;             /**< Dump SerDes PMD tuning information */
    BOOL dma_dump;              /**< Dump LSB/ESB DMA data */
    BOOL dmem_dump;             /**< Dump SPICO DMEM data */
    BOOL imem_dump;             /**< Dump SPICO IMEM data */
    BOOL binary;                /**< Display binary along with hex */
    BOOL columns;               /**< Display data in multiple columns */

    BOOL destructive;           /**< Perform destructive tests */
    BOOL use_existing_divider;  /**< When performing destructive tests, use current divider */
    int cycles;                 /**< Number of cycles to check SPICO PC */
    int *dividers;              /**< When performing destructive tests, use this built in divider list (populated in the constructor) */

} Avago_diag_config_t;

EXT Avago_diag_config_t *avago_diag_config_construct(Aapl_t *aapl);
EXT void  avago_diag_config_destruct(Aapl_t *aapl, Avago_diag_config_t *config);
EXT int   avago_diag(Aapl_t *aapl, uint addr, Avago_diag_config_t *config);
EXT int   avago_serdes_diag(Aapl_t *aapl, uint addr, Avago_diag_config_t *config);
EXT void  avago_serdes_mem_dump(Aapl_t *aapl, uint sbus_addr, BOOL bin_enable, BOOL columns, BOOL dma_dump, BOOL dmem_dump, BOOL imem_dump);

#endif /* AAPL_ENABLE_DIAG */

EXT void  avago_serdes_state_dump(Aapl_t *aapl, uint addr);
EXT char *avago_serdes_get_state_dump(Aapl_t *aapl, uint addr, uint disable_features, BOOL ignore_errors);

typedef struct
{
    BOOL disable_tx;
    BOOL disable_rx;
    BOOL disable_dfe;
    BOOL disable_clk;
    BOOL disable_info;
    BOOL disable_data; /* 80 bit RX data */
    BOOL keep_open; /* keep socket open */
    Avago_ip_type_t type_filter;
} Avago_state_table_options_t;
EXT void avago_serdes_print_state_table_options(Aapl_t *aapl, Avago_addr_t *addr_struct, Avago_state_table_options_t *options);
EXT void avago_device_info_options(Aapl_t *aapl, Avago_addr_t *addr_struct, Avago_ip_type_t type, Avago_state_table_options_t *options);

#endif /* AVAGO_DIAG_H_ */
