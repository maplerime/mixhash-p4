/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) aacs_server exported functions */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for aacs_server function. */

#ifndef AVAGO_AACS_SERVER_H_
#define AVAGO_AACS_SERVER_H_

/* Create a new AACS server to handle incoming connection at TCP port tcp_port: */
EXT int avago_aacs_server(Aapl_t *aapl, int tcp_port);
EXT int avago_aacs_server_options(Aapl_t *aapl, int tcp_port, const char * aacs_server_host, uint aacs_server_host_port, BOOL close_connection);

EXT char *avago_aacs_process_cmd(Aapl_t *aapl, const char *cmd, int *chip_nump, int *ring_nump);

#if AAPL_ALLOW_THREAD_SUPPORT && AAPL_ENABLE_HS2
typedef struct
{
    Aapl_t *aapl_upstream;
    Aapl_t *aapl_downstream;
} Aapl_server_cntl_t;

EXT void *aapl_telnet_thread(void *aapl_server_cntl);
#endif /* AAPL_ALLOW_THREAD_SUPPORT && AAPL_ENABLE_HS2 */

#endif
