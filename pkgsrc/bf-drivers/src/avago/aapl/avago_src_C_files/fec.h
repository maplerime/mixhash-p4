/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* Support for AAPL (ASIC and ASSP Programming Layer) generic handling of
 */
/* SerDes (Serializer/Deserializer) slices on ASIC SBus rings.
 */

/** Doxygen File Header
 */
/** @file
 */
/** @brief  Function and type definitions for FEC blocks.
 */

#ifndef AVAGO_FEC_H_
#define AVAGO_FEC_H_

/** @brief FEC mode information.
 */
typedef enum
{
  AVAGO_FEC_RS528,                    /**< 100G-KR4, 100G-CR4, 25G-CR, 32GFC, 128GFC
 */
  AVAGO_FEC_RS544,                    /**< 100G-KP4
 */
  AVAGO_FEC_FIRECODE,                 /**< 10G-KR, 40G-CR4, 16GFC
 */
  AVAGO_FEC_UNCONFIGURED = -1

} Avago_fec_mode_t;

/** @brief FEC configuration information.
 */
typedef struct
{
  Avago_fec_mode_t mode            ;  /**< Selects FEC operating mode (ie. RS528 v. RS544)
 */
  BOOL stats_clear_on_read         ;  /**< Cause stats counters to clear when read
 */
  BOOL error_indication_enable     ;  /**< Enable error indication to PCS
 */
  BOOL error_correct_bypass_enable ;  /**< Enable bypass of correction (detector only)
 */
  BOOL consec_lane_track_enable[4] ;  /**< Control which lanes monitor for consecutive symbol in error
 */
} Avago_fec_config_t;

/** @brief FEC ebuf reset options
 */
typedef enum
{
  AVAGO_FEC_CFG_MODE           ,
  AVAGO_FEC_CFG_CLR_ON_READ    ,
  AVAGO_FEC_CFG_ERR_INDICATION ,
  AVAGO_FEC_CFG_ERR_COR_BYPASS ,
  AVAGO_FEC_CFG_LANE_TRACK
}Avago_fec_config_options_t;

/** @brief FEC statistics information.
 */
typedef struct
{
  BOOL halted                      ; /**< Indicates if stats accumulation is halted
 */
  uint codeword_size               ; /**< Size of FEC codeword
 */
  bigint codewords                 ; /**< Codewords checked count
 */
  uint corrected                   ; /**< Correted CW count
 */
  uint uncorrected                 ; /**< Uncorrected CW count
 */
  /* RS-FEC only
 */
  uint cw_X_sym_count[16]          ; /**< CW with X symbols in error (X is index, 0 unused)
 */
  uint rs_lane_sym_count[4]        ; /**< Per FEC lane symbol error count
 */
  BOOL rs_sym_consec_active[4]     ; /**< Which lanes are checking consecutive errors
 */
  uint rs_X_sym_consec[7]          ; /**< Consecutive symbols in error count
 */

} Avago_fec_stats_t;

/** @brief FEC status information.
 */
typedef struct
{
  BOOL clear_on_read_active         ; /**< Clear on read bit set
 */
  BOOL fec_align_status[4]          ; /**< Alignment status for FEC lanes
 */
  uint fec_lane_map[4]              ; /**< Physical lane indexed Logical lane is received on
 */
  BOOL error_indication_ability     ; /**< Indicates FEC capable of error indication to PCS
 */
  BOOL error_correct_bypass_ability ; /**< Indicates FEC capable of bypassing error correction logic
 */
  BOOL error_indication             ; /**< FEC error indication status to PCS
 */
  BOOL error_correct_bypass         ; /**< FEC error correction bypass status
 */

} Avago_fec_status_t;

/** @brief FEC gearfifo status information.
 */
typedef struct
{
  BOOL tx_overflow  ;
  BOOL tx_underflow ;
  uint tx_wr_ptr    ;
  uint tx_rd_ptr    ;

  BOOL rx_overflow  ;
  BOOL rx_underflow ;
  uint rx_wr_ptr    ;
  uint rx_rd_ptr    ;

} Avago_fec_gearfifo_status_t;

/** @brief FEC gearfifo reset options
 */
typedef enum
{
  TX_GEARFIFO_OVERFLOW_RESET ,
  TX_GEARFIFO_UNDERFLOW_RESET,
  RX_GEARFIFO_OVERFLOW_RESET ,
  RX_GEARFIFO_UNDERFLOW_RESET
} Avago_fec_gearfifo_reset_options_t;

/** @brief FEC ebuf status information.
 */
typedef struct
{
  BOOL tx_sync_overflow  ;
  BOOL tx_sync_underflow ;
  BOOL tx_underflow      ;
  BOOL tx_idle_err       ;
  BOOL tx_idle_detect    ;
  BOOL tx_idle_force_out ;

  BOOL rx_sync_overflow  ;
  BOOL rx_sync_underflow ;
  BOOL rx_underflow      ;
  BOOL rx_idle_err       ;
  BOOL rx_idle_detect    ;
  BOOL rx_idle_force_out ;

} Avago_fec_ebuf_status_t;

/** @brief FEC ebuf control options
 */
typedef enum
{
  TX_EBUF_ENABLE                    ,
  TX_EBUF_DISABLE                   ,
  TX_EBUF_FORCE_IDLE_OUTPUT_ENABLE  ,
  TX_EBUF_FORCE_IDLE_OUTPUT_DISABLE ,
  TX_EBUF_IDLE_DETECT_ENABLE        ,
  TX_EBUF_IDLE_DETECT_DISABLE       ,
  RX_EBUF_ENABLE                    ,
  RX_EBUF_DISABLE                   ,
  RX_EBUF_FORCE_IDLE_OUTPUT_ENABLE  ,
  RX_EBUF_FORCE_IDLE_OUTPUT_DISABLE ,
  RX_EBUF_IDLE_DETECT_ENABLE        ,
  RX_EBUF_IDLE_DETECT_DISABLE

} Avago_fec_ebuf_control_options_t;

/** @brief FEC ebuf reset options
 */
typedef enum
{
  TX_EBUF_OVERFLOW_RESET ,
  TX_EBUF_UNDERFLOW_RESET,
  RX_EBUF_OVERFLOW_RESET ,
  RX_EBUF_UNDERFLOW_RESET
} Avago_fec_ebuf_reset_options_t;

EXT Avago_fec_config_t *avago_fec_config_construct(Aapl_t *aapl);
EXT void  avago_fec_config(Aapl_t *aapl, uint addr, Avago_fec_config_options_t fec_config_options, Avago_fec_config_t *config);
EXT void  avago_fec_config_destruct(Aapl_t *aapl, Avago_fec_config_t *config);

EXT void   avago_fec_get_status(Aapl_t *aapl, uint addr, Avago_fec_status_t *status);
EXT void   avago_fec_get_stats(Aapl_t *aapl, Avago_fec_stats_t *stats, uint addr, uint lane);
EXT void   avago_fec_reset_stats(Aapl_t *aapl, uint addr, BOOL halt_flag);
EXT BOOL   avago_fec_get_stats_halt(Aapl_t *aapl, uint addr);
EXT void   avago_fec_set_stats_halt(Aapl_t *aapl, uint addr, BOOL now, BOOL on_max_cw);

EXT void avago_fec_get_gearfifo(Aapl_t *aapl, uint addr, Avago_fec_gearfifo_status_t *status );
EXT void avago_fec_reset_gearfifo(Aapl_t *aapl, uint addr, Avago_fec_gearfifo_reset_options_t gearfifo_reset_option);

EXT void avago_fec_get_ebuf(Aapl_t *aapl, uint addr, Avago_fec_ebuf_status_t *status );
EXT void avago_fec_control_ebuf(Aapl_t *aapl, uint addr, Avago_fec_ebuf_control_options_t ebuf_control_option);
EXT void avago_fec_reset_ebuf(Aapl_t *aapl, uint addr, Avago_fec_ebuf_reset_options_t ebuf_reset_option);
EXT BOOL avago_fec_get_reset_ebuf(Aapl_t *aapl, uint addr, Avago_fec_ebuf_reset_options_t ebuf_reset_option);

EXT BOOL avago_fec_get_alignment_reset_status(Aapl_t *aapl, uint addr);
EXT void avago_fec_reset_alignment_status(Aapl_t *aapl, uint addr);

EXT uint   avago_fec_get_consec_lane_active(Aapl_t *aapl, uint addr);
EXT bigint avago_fec_get_codeword(Aapl_t *aapl, uint addr, uint lane);
EXT uint   avago_fec_get_corrected(Aapl_t *aapl, uint addr, uint lane);
EXT uint   avago_fec_get_uncorrected(Aapl_t *aapl, uint addr, uint lane);
EXT uint   avago_rsfec_get_lane_sym_count(Aapl_t *aapl, uint addr, uint lane);
EXT uint   avago_rsfec_get_cw_X_sym_count(Aapl_t *aapl, uint addr, uint num);
EXT uint   avago_rsfec_get_consec_X_sym_count(Aapl_t *aapl, uint addr, uint num);
EXT char  *avago_rsfec_stats_to_str(Aapl_t *aapl, Avago_fec_stats_t *stats, uint addr);

/* Functions to configure tesla
 */
#ifdef AAPL_FEC_SIMULATION
EXT void avago_fec_chip_config(Aapl_t *aapl, Avago_fec_mode_t mode);
EXT int  avago_rsfec_frame(Aapl_t *aapl);
EXT void avago_fec_tesla_config(Aapl_t *aapl, Avago_fec_mode_t mode);
#else
EXT void avago_fec_chip_config(Aapl_t *aapl, uint sbus_addr, Avago_fec_mode_t mode);
EXT int  avago_rsfec_frame(Aapl_t *aapl, uint sbus_addr);
EXT void avago_fec_tesla_config(Aapl_t *aapl, uint sbus_addr, Avago_fec_mode_t mode);
#endif
EXT void avago_fec_tesla_upload(Aapl_t *aapl);
EXT void avago_set_tesla_rsfec528_2x50g_div(Aapl_t *aapl, uint sbus_addr, uint cm4_count, int div);
EXT void avago_set_tesla_rsfec544_2x50g_div(Aapl_t *aapl, uint sbus_addr, uint cm4_count, int div);

EXT void avago_fec_chip_upload(Aapl_t *aapl, uint addr, const char *cm4_file, const char *d6_file);

EXT Avago_fec_mode_t avago_rsfec_get_fec_mode( Aapl_t *aapl, uint sbus_addr);

EXT Avago_fec_mode_t aapl_fec_mode_from_str(const char *str, const char *optname);

EXT BOOL aapl_str_to_fec_mode(const char *name, Avago_fec_mode_t *out);

EXT const char *aapl_fec_mode_to_str(Avago_fec_mode_t value);

#endif
