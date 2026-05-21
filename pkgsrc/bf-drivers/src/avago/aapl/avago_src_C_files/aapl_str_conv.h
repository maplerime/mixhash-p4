/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/** Doxygen File Header */
/** @file */
/** @brief Declarations for string conversion functions. */

#ifndef AVAGO_SERDES_STR_CONV_H_
#define AVAGO_SERDES_STR_CONV_H_

/* Functions to convert types to strings: */
EXT const char *aapl_rx_data_qual_to_str  (Avago_serdes_rx_data_qual_t   val);
EXT const char *aapl_data_qual_to_str     (Avago_serdes_data_qual_t      val);
EXT const char *aapl_pcs_fifo_clk_to_str  (Avago_serdes_pcs_fifo_clk_t   val);

/* Functions to convert strings to types: */
/* Return TRUE if name is a recognized name for the type, FALSE if not. */
EXT BOOL aapl_str_to_data_qual(   const char *name, Avago_serdes_data_qual_t    *out);
EXT BOOL aapl_str_to_bool(        const char *name, BOOL                        *out);
EXT BOOL aapl_str_to_pcs_fifo_clk(const char *name, Avago_serdes_pcs_fifo_clk_t *out);

#if AAPL_ENABLE_AVSP
EXT const char *aapl_avsp_mode_to_str     (Avsp_mode_t                   val);
EXT const char *aapl_supervisor_mode_to_str(Avsp_supervisor_mode_t       val);
EXT BOOL aapl_str_to_avsp_mode(const char *name, Avsp_mode_t *out);
EXT BOOL aapl_str_to_supervisor_mode(const char *name, Avsp_supervisor_mode_t *out);
#endif

EXT const char *aapl_an_hcd_to_str(Avago_an_hcd_t value);
#if AAPL_ENABLE_DIAG
EXT const char *aapl_bsb_mode_to_str      (Avago_serdes_bsb_mode_t       val);
EXT const char *aapl_bsb_clk_to_str       (Avago_serdes_bsb_clk_sel_t    val);
EXT BOOL aapl_str_to_bsb_mode(const char *name, Avago_serdes_bsb_mode_t *out);
EXT BOOL aapl_str_to_bsb_clk(const char *name, Avago_serdes_bsb_clk_sel_t *out);
#endif

#endif /* AVAGO_SERDES_STR_CONV_H_ */


