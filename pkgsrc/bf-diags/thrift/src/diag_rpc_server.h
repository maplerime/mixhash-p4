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

#ifndef _DIAG_RPC_SERVER_H_
#define _DIAG_RPC_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DIAG_RPC_SERVER_PORT 9096

extern int start_diag_rpc_server(void **server_cookie);
extern int stop_diag_rpc_server();

#ifdef __cplusplus
}
#endif

#endif /* _DIAG_RPC_SERVER_H_ */
