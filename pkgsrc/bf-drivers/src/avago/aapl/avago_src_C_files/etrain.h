/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/** Doxygen File Header */
/** @file */
/** @brief DDR3 device specific definitions. */

#ifndef AVAGO_ETRAIN_H_
#define AVAGO_ETRAIN_H_

#if AAPL_ENABLE_DDR3_ETRAIN

typedef enum
{
  AVAGO_DDR3_PERFORM_DRAM_INIT             = 0, 
  AVAGO_DDR3_RD_GAP1_EN                    = 1, 
  AVAGO_DDR3_RD_GAP                        = 2, 
  AVAGO_DDR3_WR_GAP                        = 3, 
  AVAGO_DDR3_RD_BURST_LOOP                 = 4, 
  AVAGO_DDR3_BANK                          = 5, 
  AVAGO_DDR3_ROW_OFFSET                    = 6, 
  AVAGO_DDR3_COL_OFFSET                    = 7, 
  AVAGO_DDR3_PATTERN0                      = 8, 
  AVAGO_DDR3_PATTERN1                      = 9, 
  AVAGO_DDR3_PATTERN2                      = 10,
  AVAGO_DDR3_PATTERN3                      = 11,
  AVAGO_DDR3_DM_PATTERN0                   = 12,
  AVAGO_DDR3_DM_PATTERN1                   = 13,
  AVAGO_DDR3_DM_PATTERN2                   = 14,
  AVAGO_DDR3_DM_PATTERN3                   = 15,
  AVAGO_DDR3_PHY_FREQ_CONFIG               = 16,
  AVAGO_DDR3_PHY_T_RDLAT_OFFSET            = 17,
  AVAGO_DDR3_MR0_TRN_WRR                   = 18,
  AVAGO_DDR3_MEM_CAS_LAT                   = 19,
  AVAGO_DDR3_MEM_CAS_WRLAT                 = 20,
  AVAGO_DDR3_MAX_TIMEOUT                   = 21,
  AVAGO_DDR3_PBDS_ENABLE                   = 22,
  AVAGO_DDR3_PBDS_DELAY_INCR               = 23,
  AVAGO_DDR3_PBDS_PASS_COUNT               = 24,
  AVAGO_DDR3_PBDS_SEPERATE_READ_WRITE      = 25,
  AVAGO_DDR3_PBDS_DATAMASK_ENABLE          = 26,
  AVAGO_DDR3_DISABLE_UPPER_NIBBLE_BYTES    = 27,
  AVAGO_DDR3_LOOP_WRITE_READ_COMPARE_COUNT = 28,
  AVAGO_DDR3_RANDOMIZE_PATTERN             = 29
} Avago_ddr3_parameter_t;

/* Set a ddr3 training parameter value */
EXT int avago_ddr3_set_parameter(Aapl_t *aapl, uint spico_addr, Avago_ddr3_parameter_t param, uint value);

/* Get a ddr3 training parameter value */
EXT int avago_ddr3_get_parameter(Aapl_t *aapl, uint spico_addr, Avago_ddr3_parameter_t param);


typedef enum
{
  AVAGO_DDR3_800_CL6   = 0,
  AVAGO_DDR3_1600_CL11 = 1,
  AVAGO_DDR3_1866_CL12 = 2,
  AVAGO_DDR3_1866_CL13 = 3,
  AVAGO_DDR3_2133_CL13 = 4,
  AVAGO_DDR3_2133_CL14 = 5
} Avago_ddr3_frequency_config_t;

/* Get a ddr3 training parameter value */
EXT int avago_ddr3_set_frequency_config(Aapl_t *aapl, uint spico_addr, Avago_ddr3_frequency_config_t freq_config);

/* Map a channel number to an sbus address */
EXT int avago_ddr3_channel_to_sbus_addr(Aapl_t *aapl, uint spico_addr, uint channel);

/* Get number of DDR3 channels on ring */
EXT int avago_ddr3_get_channel_count(Aapl_t *aapl, uint spico_addr);

/* Forces an end to training, lowering DFI_PHYUPD_REQ. */
EXT int  avago_ddr3_force_training_complete(Aapl_t *aapl, uint spico_addr, uint channel);

/* Launch Diagnostics for a single DDR3 channel */
EXT int avago_ddr3_channel_diagnostics(Aapl_t *aapl, uint spico_addr, uint channel);

/* Launch Diagnostics for all DDR3 channels */
EXT int avago_ddr3_diagnostics(Aapl_t *aapl, uint spico_addr);

/* Perform DDR3 training on all channels */
EXT int avago_ddr3_train_all_channels(Aapl_t *aapl, uint spico_addr);

/* Perform DDR3 training on a single channel */
EXT int avago_ddr3_train_channel(Aapl_t *aapl, uint spico_addr, uint channel);

/* Return a textual description for a trainign error code */
EXT char * avago_ddr3_training_error_desc(int result);

/* Report the training status of each channel */
EXT int avago_ddr3_query_training_status(Aapl_t *aapl, uint spico_addr, uint channel);

/* DDR3 training values */
typedef struct
{
      uint write_leveling[16];
      uint read_gate_delay[16];
      uint write_pbds_stb[16];
      uint read_pbds_stb[16];
      uint write_pbds[16][9];
      uint read_pbds[16][9];
} Avago_ddr3_training_values_t;


EXT Avago_ddr3_training_values_t *avago_ddr3_training_values_construct(Aapl_t *aapl);
EXT void avago_ddr3_training_values_destruct(Aapl_t *aapl, Avago_ddr3_training_values_t *values);

/* Get the ddr3 training values for a channel */
EXT int avago_ddr3_get_training_values(Aapl_t *aapl, uint spico_addr, uint channel, Avago_ddr3_training_values_t *values);

/* Report the ddr3 training values for a channel */
EXT int avago_ddr3_report_training_values(Aapl_t *aapl, uint spico_addr, uint channel, Avago_ddr3_training_values_t *values);

/* Force the ddr3 training values for a channel */
EXT int avago_ddr3_set_training_values(Aapl_t *aapl, uint spico_addr, uint channel, Avago_ddr3_training_values_t *values);

/* DDR3 training support functions */
EXT int avago_ddr3_load_wrlvl_value(Aapl_t *aapl, uint etrain_addr, uint byte_num, uint value);
EXT int avago_ddr3_load_coarse_rdlvl_value(Aapl_t *aapl, uint etrain_addr, uint byte_num, uint value);
EXT int avago_ddr3_load_fine_rdlvl_value(Aapl_t *aapl, uint etrain_addr, uint byte_num, uint value);
EXT int avago_ddr3_load_stb_pbds_value(Aapl_t *aapl, uint train_addr, uint byte_num, uint read_write, uint value);
EXT int avago_ddr3_load_data_pbds_value(Aapl_t *aapl, uint train_addr, uint byte_num, uint lane, uint read_write, uint value);

/* DDR3 sbus dump */
EXT void avago_ddr3_sbus_dump_channel_devices(Aapl_t *aapl, uint spico_addr, uint channel);
EXT void avago_ddr3_sbus_dump(Aapl_t *aapl, uint spico_addr, uint channel, char *device_name);

/* Spico status */
EXT void avago_ddr3_report_spico_status(Aapl_t *aapl, uint spico_addr);

/* SBus reset for all ddr3 devices */
EXT int avago_ddr3_reset_sbus(Aapl_t *aapl, uint sbus_addr);

#endif /* AAPL_ENABLE_DDR3_ETRAIN */

#endif /* AVAGO_ETRAIN_H_ */
