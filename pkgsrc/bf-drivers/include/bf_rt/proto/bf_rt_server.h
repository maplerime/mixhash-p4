/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2021 Intel Corporation
 *  All Rights Reserved.
 *
 *  This software and the related documents are Intel copyrighted materials,
 *  and your use of them is governed by the express license under which they
 *  were provided to you ("License"). Unless the License provides otherwise,
 *  you may not use, modify, copy, publish, distribute, disclose or transmit
 *  this software or the related documents without Intel's prior written
 *  permission.
 *
 *  This software and the related documents are provided as is, with no express
 *  or implied warranties, other than those that are expressly stated in the
 *  License.
 ******************************************************************************/

/** @file bf_rt_server.h
 *
 *  @brief Contains BF-RT gRPC server APIs
 */
#ifndef _BF_RT_SERVER_H
#define _BF_RT_SERVER_H

#include <bf_types/bf_types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @brief Start server and bind to default address (0.0.0.0:50052) or
 * port as input param in format bfrt-grpc-port
 *
 * @param[in] program_name      P4 program name to use
 * @param[in] local_only        Specifies if server should bind to local
 *                              loopback interface only
 * @param[in] port              port number to use default is 50052
 */
bf_status_t bf_rt_grpc_server_run(const char *program_name,
                                  bool local_only,
                                  int port);

/**
 * @brief Start server and bind specified address
 *
 * @param[in] server_address    Server address to bind to represented as string
 *                              in format ip:port
 */
bf_status_t bf_rt_grpc_server_run_with_addr(const char *server_address);

#ifdef __cplusplus
}
#endif

#endif  // _BF_RT_SERVER_H
