/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* Header for defines and declarations only needed by main programs. */
/* */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for main programs. */

#ifndef AAPL_MAIN_PROGRAM_H_
#define AAPL_MAIN_PROGRAM_H_

#if AAPL_ENABLE_MAIN

/* Declared in aapl.c: */
EXT const char *aapl_default_firmware_rev;
EXT const char *aapl_default_sbm_firmware_rev;
EXT const uint  aapl_default_device_addr;
EXT int         aapl_default_divider;

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT const char *aapl_get_default_firmware_build(Aapl_t *aapl, uint addr);
EXT BOOL aapl_find_firmware(Aapl_t *aapl, uint addr, const char *fw_build, char *buf, int buf_len);
EXT int  avago_serdes_upload_firmware(Aapl_t *aapl, Avago_addr_t *addr_list, const char *fw_build, const char *firmware_file_ptr, char *buf, int len);
#endif

/* Global default values: */

#define AAPL_COMMON_MAIN_OPTIONS \
        /* Options always available: */                                                     \
        {"s",               1, 0, 0xf000 | 'S'}, /* <IP_address> */                         \
        {"server",          1, 0, 0xf000 | 'S'}, /* <IP_address> */                         \
        {"p",               1, 0, 0xf000 | 'p'}, /* <port_number> */                        \
        {"port",            1, 0, 0xf000 | 'p'}, /* <port_number> */                        \
        {"comm",            1, 0, 0xf000 | 'M'}, /* Set aapl->communication_method */       \
        {"max-cmds-buff",   1, 0, 0xf000 | 'x'}, /* Set buffering limit for AACS */         \
        {"d",               1, 0, 0xf000 | 'd'}, /* <number> debug level */                 \
        {"debug",           1, 0, 0xf000 | 'd'}, /* <number> debug level */                 \
        {"verbose",         1, 0, 0xf000 | 'V'}, /* increase verbosity */                   \
        {"diag-on-fail",    1, 0, 0xf000 | 'F'}, /* run avago_diag on spico int failure*/   \
        {"jtag",            1, 0, 0xf000 | 'j'}, /* provide jtag id (as int) */             \
        {"capabilities",    1, 0, 0xf102      }, /* provide way to override capabilities */ \
        {"disable-time-stamps",0, 0, 0xf000 | 't'}, /* disable AAPL timestamps */           \
        {"core-interrupt",  0, 0, 0xf103      }, /* use link emulator for interrupts */     \
        {"logging-file",    1, 0, 0xf106      }, /* Redirect debug messages to the specified file */     \
                                                                                            \
        {"timeout",         1, 0, 0xf104},  /* set serdes interrupt timeout */              \
        {"tck-delay",       1, 0, 0xf105},  /* set tck_delay for BB GPIO JTAG mode */       \
                                                                                            \
        {"devices",         1, 0, 0xf000 | 'c'}, /* tell AAPL how many devices there are */ \
        {"sbus-rings",      1, 0, 0xf000 | 'r'}, /* tell AAPL how many SBus rings there are */ \
        {"mdio-base-addr",  1, 0, 0xf100      }, /* mdio base port address */               \
        {"i2c-base-addr",   1, 0, 0xf101      }, /* i2c base address */                     \
                                                                                            \
        {"h",            0, 0, 0xf000 | 'h'}, /* for usage message. */                      \
        {"help",         0, 0, 0xf000 | 'h'}, /* for usage message. */                      \
        {"?",            0, 0, 0xf000 | 'h'}, /* another way to get a usage message. */     \
                                                                                            \
        /* Optional arguments, depending on caller */                                       \
        {"a",            1, 0, 0xf000 | 'a'}, /* SBus address */                            \
        {"addr",         1, 0, 0xf000 | 'a'}  /* SBus address */                            \


/* Global program name.  May not be needed for much longer... */
EXT const char *aapl_myname;      /* for error/usage messages. */

EXT int  aapl_common_main_options(Aapl_t *aapl, int argc, char *argv[], Avago_addr_t *addr);
EXT void aapl_common_main_help(BOOL show_addr_help);

EXT uint   aapl_addr_from_str(const char *str, const char *optname);
EXT BOOL   aapl_addr_list_from_str(Aapl_t *aapl, Avago_addr_t *addr, const char *optarg);
EXT BOOL   aapl_bool_from_str(const char *str, const char *optname);
EXT uint   aapl_num_from_str( const char *str, const char *optname, int base);
EXT bigint aapl_nume_from_str(const char *str, const char *optname);

EXT void aapl_main_print_error(const char *fmt, ...);
EXT void aapl_main_unexpected_arg(const char *arg);
EXT void aapl_main_error(const char * fmt, ...);
EXT void aapl_exit_on_unexpected_arg(const char *arg);

EXT Avago_serdes_rx_cmp_mode_t   aapl_cmp_mode_from_str( const char *str, const char *optname);
EXT Avago_serdes_mem_type_t      aapl_mem_type_from_str( const char *str, const char *optname);
EXT Avago_serdes_data_qual_t     aapl_data_qual_from_str(const char *str, const char *optname);
EXT Avago_serdes_rx_clock_t      aapl_rx_clock_from_str( const char *str, const char *optname);
EXT Avago_serdes_rx_clock_cdc_t  aapl_rx_clock_cdc_from_str(const char *str, const char *optname);
#if AAPL_ENABLE_EYE_MEASUREMENT
EXT Avago_serdes_eye_type_t      eye_sel_from_str(       const char *str, const char *optname);
#endif
#if AAPL_ENABLE_AVSP && !defined SWIG
EXT Avsp_mode_t                  aapl_avsp_mode_from_str(const char *str, const char *optname);
#endif
#if AAPL_ENABLE_DIAG
EXT Avago_serdes_bsb_mode_t      aapl_bsb_mode_from_str( const char *str, const char *optname);
EXT Avago_serdes_bsb_clk_sel_t   aapl_bsb_clk_from_str(  const char *str, const char *optname);
#endif
EXT Aapl_comm_method_t           aapl_comm_method_from_str(const char *str, const char *optname);
EXT Avago_serdes_dfe_tune_mode_t aapl_dfe_tune_mode_from_str(const char *str, const char *optname);

EXT BOOL aapl_parse_range(const char *range, int *start, int *stop);

EXT int aapl_aacs_server_main (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_avsp_1104_main   (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_avsp_retimer_main(int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_avsp_5410_main   (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_avsp_7412_main   (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_avsp_8812_main   (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_avsp_9104_main   (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_bist_main(        int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_bsb_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_build_info_main(  int argc, char *argv[], Aapl_t *aapl);
#if AAPL_ENABLE_CONSOLE && !defined SWIG
EXT int aapl_console_main(     int argc, char *argv[], Aapl_t *aapl);
#endif
EXT int aapl_cmd_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_device_info_main( int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_dfe_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_serdes_an_main(   int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_hal_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_diag_main(        int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_eeprom_main(      int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_escope_main(      int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_examples_main(    int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_eye_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_sbus_master_main (int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_serdes_main(      int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_fec_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_serdes_init_main( int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_state_main(       int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_supervisor_main(  int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_ps1_main(         int argc, char *argv[], Aapl_t *aapl);
EXT int aapl_main(             int argc, char *argv[], Aapl_t *aapl);



struct _cmds
{
    const char *cmd;
    int       (*function)(int argc, char *argv[], Aapl_t *aapl);
    const char *description;

};

EXT struct _cmds commands[];
EXT int commands_length;
EXT int aapl_main_entry(int argc, char *argv[]);

#endif /* AAPL_ENABLE_MAIN */

#endif /* AAPL_MAIN_PROGRAM_H_ */
