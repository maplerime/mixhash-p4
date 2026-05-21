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

/*
 * C/C++ header file for calling server start function from C code
 */

#ifndef _BFN_PD_RPC_SERVER_H_
#define _BFN_PD_RPC_SERVER_H_

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#define BFN_PD_RPC_SERVER_PORT 9090

extern int start_bfn_pd_rpc_server(void **server_cookie, bool is_local_only);

#ifdef __cplusplus
}
#endif

#endif /* _BFN_PD_RPC_SERVER_H_ */
