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
/** @brief Declarations for eSCOPE functions. */

#ifndef AVAGO_ESCOPE_H_
#define AVAGO_ESCOPE_H_

/* Utility plotting types and functions: */

typedef struct
{
    uint   x_points;
    uint   y_points;
    char  *points;
#define _2DPEL(sed,elem,x,y) ((sed).elem[(((x) * (sed).y_points) + (y))])
#define AVAGO_PLOT_GET(sed,x,y)       _2DPEL(sed, points, x, y)
#define AVAGO_PLOT_SET(sed,x,y,value) _2DPEL(sed, points, x, y) = (value)

} Avago_plot_t;


/* escope waveform types and functions: */

typedef struct
{
    float dac_mean;
    float dac_stddev;
} Avago_wavepoint_t;

typedef struct
{
    uint  x_points;         /**< Number of points in waveform */
    uint  x_resolution;     /**< x points per UI */
    uint  y_step;           /**< sampling resolution */
    uint  y_center;         /**< y range center point value */
    uint  bits_per_ui;      /**< 1==NRZ, 2==PAM4, ... */
    uint  datapath_flags;   /**< 1=invert, 2=gray, 4=precode, 8=swizzle */
    Avago_wavepoint_t points[1];
} Avago_waveform_t;


#if AAPL_ENABLE_ESCOPE_MEASUREMENT

/* escope config and data types: */

typedef struct
{
    uint  sc_x_UI;              /**< Number of UI to gather */
    uint  sc_x_pattern_length;  /**< Data pattern length */
    uint  sc_x_resolution;      /**< Pixels per UI; always a power of 2. */
    uint  sc_y_step_size;       /**< Separation between y coordinates with data. */
    uint  sc_set_phase_center;  /**< Set to align the waveform with the UI. */

    int   sc_read_count;        /**< Number of times to accumulate. */
    int   sc_flags;             /**< Special options flags.  Not documented. */

} Avago_serdes_escope_config_t;

typedef struct
{
    uint  sd_x_UI;              /**< Number of UI gathered == pattern length. */
    uint  sd_x_pattern_length;  /**< Data pattern length. */
    uint  sd_x_points;          /**< Width dimension of packed output arrays. */
    uint  sd_x_step;            /**< Separation between x coordinates with data. */
    uint  sd_x_resolution;      /**< Pixels per UI; always a power of 2. */
                                /**< Available resolution is sd_x_step times this. */
     int  sd_x_min;             /**< Min PI coordinate (corresponds to data column 0). */

    uint  sd_y_points;          /**< Height dimension of packed output arrays. */
    uint  sd_y_step;            /**< Separation between y coordinates with data. */
    uint  sd_y_center;          /**< Center dac value */
    uint  sd_bits_per_ui;       /**< Number of bits per UI: PAM2=1, PAM4=2. */
    int   sd_flags;             /**< Special options flags.  Not documented. */
    uint  sd_datapath_flags;    /**< Datapath signal encoding, used for decoding of waveform. */
                                /**< 1: invert; 2: gray; 4: precode; 8: swizzle */


    int   sd_read_count;        /**< Number of times we have accumulated. */

    Avago_serdes_dfe_state_t sd_dfe_state;   /**< DFE state during escope capture */

    char *sd_onesp;             /**< Number of ones read at each coordinate. */
                                /**< (Two dimensional array) */

    Avago_waveform_t *sd_waveform; /**< Wave form height/stddev info to plot */
    Avago_waveform_t *sd_waveform2; /**< Alternate wave form height/stddev info to plot */
    Avago_waveform_t *sd_waveform3; /**< Alternate wave form height/stddev info to plot */
    Avago_waveform_t *sd_waveform4; /**< Alternate wave form height/stddev info to plot */
    Avago_waveform_t *sd_pulse;     /**< Pulse response wave form. */
    Avago_waveform_t *sd_pulse2;    /**< Pulse response wave form. */
    Avago_waveform_t *sd_pulse3;    /**< Pulse response wave form. */
    Avago_waveform_t *sd_pulse4;    /**< Pulse response wave form. */

#define _2DEL(sed,elem,x,y) ((sed).elem[(((x) * (sed).sd_y_points) + (y))])
#define AVAGO_ESCOPE_ONES_GET3(sed,bit,x,y)       _2DEL(sed, sd_onesp, (bit)*(sed).sd_x_resolution+(x), y)
#define AVAGO_ESCOPE_ONES_SET3(sed,bit,x,y,value) _2DEL(sed, sd_onesp, (bit)*(sed).sd_x_resolution+(x), y) = (value)
#define AVAGO_ESCOPE_ONES_ADD3(sed,bit,x,y,value) _2DEL(sed, sd_onesp, (bit)*(sed).sd_x_resolution+(x), y) += (value)
#define AVAGO_ESCOPE_ONES_GET(sed,x,y)       _2DEL(sed, sd_onesp, x, y)
#define AVAGO_ESCOPE_ONES_SET(sed,x,y,value) _2DEL(sed, sd_onesp, x, y) = (value)
#define AVAGO_ESCOPE_ONES_ADD(sed,x,y,value) _2DEL(sed, sd_onesp, x, y) += (value)

    char *sd_hardware_log;      /**< Holds hardware info buffer. */
    char *sd_phase_center_log;  /**< Holds phase centering info. */
    char *sd_comment;           /**< Holds any user comment string. */

} Avago_serdes_escope_data_t;


/* Public escope functions: */

EXT Avago_serdes_escope_config_t *avago_serdes_escope_config_construct(Aapl_t *aapl);
EXT void avago_serdes_escope_config_destruct(Aapl_t *aapl, Avago_serdes_escope_config_t *configp);

EXT Avago_serdes_escope_data_t *avago_serdes_escope_data_construct(Aapl_t *aapl);
EXT void avago_serdes_escope_data_destruct(Aapl_t *aapl, Avago_serdes_escope_data_t *datap);

EXT int avago_serdes_escope_get_point(const Avago_serdes_escope_data_t *datap, int x, int dac);

EXT int avago_serdes_escope_get(Aapl_t *aapl, uint sbus_addr, const Avago_serdes_escope_config_t *configp, Avago_serdes_escope_data_t *datap);

EXT int avago_serdes_escope_extract_pulse_response(Aapl_t *aapl, Avago_serdes_escope_data_t *datap, int pulse_width, int delay);

EXT BOOL avago_serdes_escope_read_file(Aapl_t *aapl, const char *file_name, Avago_serdes_escope_data_t *datap);
EXT BOOL avago_serdes_escope_write_file(const char *file_name, const Avago_serdes_escope_data_t *datap);

EXT void avago_plot_ascii_waveform(Avago_plot_t *plot, const Avago_waveform_t *wave);
EXT void avago_plot_ascii_eye(Avago_plot_t *plot, const Avago_waveform_t *wave);

/* Public utility functions: */
EXT Avago_plot_t *avago_plot_construct(Aapl_t *aapl, uint x, uint y);
EXT void avago_plot_destruct(Aapl_t *aapl, Avago_plot_t *plot);
EXT int avago_plot_get_point(const Avago_plot_t *plot, int x, int y);

#if AAPL_ENABLE_FILE_IO
EXT void avago_serdes_escope_write_data(FILE *file, const Avago_serdes_escope_data_t *datap);
EXT void avago_serdes_escope_dump_raw_data(FILE *file, const Avago_serdes_escope_data_t *datap);
EXT void avago_plot_print_ascii(FILE *file, const Avago_plot_t *plot);
#endif

#endif /* AAPL_ENABLE_ESCOPE_MEASUREMENT */

#endif /* AVAGO_ESCOPE_H_ */
