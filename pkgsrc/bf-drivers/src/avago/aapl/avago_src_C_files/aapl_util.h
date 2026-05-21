/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) declarations for "system level" calls */
/* (not AVAGO_SERDES, or SPICO, etc) defined in system.cpp. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for system-level calls. */

#ifndef AAPL_UTIL_H
#define AAPL_UTIL_H

/* Declarations for IP-specific values used in AAPL */
typedef struct
{
    Avago_ip_type_t    ip_type;     /* input ip_type */
    Avago_process_id_t process_id;  /* input process_id */
    uint               rev_id;      /* input rev_id */
    int                center_freq; /* output serdes center frequency (MHz) */
    const char * const rev_id_name; /* output name */
    const char * const other;       /* placeholder */
} Avago_ip_info_t;

EXT const Avago_ip_info_t *avago_get_ip_info(Aapl_t *aapl, uint addr);

/* Returns AAPL version and build information */
EXT char *aapl_build_info(int detailed);

/* Print information to STDOUT from the Aapl_t struct. */
EXT void avago_device_info_all(Aapl_t *aapl);
EXT void avago_device_info(Aapl_t *aapl, Avago_addr_t *addr_struct, Avago_ip_type_t type);
EXT const char *aapl_get_ip_type_str(Aapl_t *aapl, uint addr);
#if AAPL_ENABLE_FILE_IO
EXT const char *aapl_get_ip_firmware_file_str(Aapl_t *aapl, uint addr);
#endif
EXT void aapl_print_struct(Aapl_t *aapl, int verbose, uint addr, Avago_ip_type_t type); /* deprecated -- do not use */


#endif  /* AAPL_UTIL_H_ */
