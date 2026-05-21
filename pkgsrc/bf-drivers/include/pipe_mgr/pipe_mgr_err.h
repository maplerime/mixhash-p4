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

/*!
 * @file pipe_mgr_err.h
 * @date
 *
 * Error code definitions for pipeline management API
 *
 */

#ifndef _PIPE_MGR_ERR_H
#define _PIPE_MGR_ERR_H

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "bf_types/bf_types.h"

/**
 * Pipeline management API error codes.
 */
typedef int pipe_status_t;
typedef enum pipe_status {
  /**< Operation successful. */
  PIPE_SUCCESS = BF_SUCCESS,
  PIPE_NOT_READY = BF_NOT_READY,
  /**< No system resources (e.g. malloc failures). */
  PIPE_NO_SYS_RESOURCES = BF_NO_SYS_RESOURCES,
  /**< Incorrect inputs. */
  PIPE_INVALID_ARG = BF_INVALID_ARG,
  PIPE_ALREADY_EXISTS = BF_ALREADY_EXISTS,
  PIPE_COMM_FAIL = BF_HW_COMM_FAIL,
  PIPE_OBJ_NOT_FOUND = BF_OBJECT_NOT_FOUND,
  PIPE_MAX_SESSIONS_EXCEEDED = BF_MAX_SESSIONS_EXCEEDED,
  PIPE_SESSION_NOT_FOUND = BF_SESSION_NOT_FOUND,
  PIPE_NO_SPACE = BF_NO_SPACE,
  /**< Temporarily out of resources, try again later (e.g.  no free DMA buffers,
     FIFOs full, etc.). */
  PIPE_TRY_AGAIN = BF_EAGAIN,
  PIPE_INIT_ERROR = BF_INIT_ERROR,
  /**< API call not supported in transactions. */
  PIPE_TXN_NOT_SUPPORTED = BF_TXN_NOT_SUPPORTED,
  PIPE_TABLE_LOCKED = BF_TABLE_LOCKED,
  PIPE_IO = BF_IO,
  PIPE_UNEXPECTED = BF_UNEXPECTED,
  PIPE_ENTRY_REFERENCES_EXIST = BF_ENTRY_REFERENCES_EXIST,
  PIPE_NOT_SUPPORTED = BF_NOT_SUPPORTED,
  PIPE_LLD_FAILED = BF_HW_UPDATE_FAILED,
  PIPE_NO_LEARN_CLIENTS = BF_NO_LEARN_CLIENTS,
  PIPE_IDLE_UPDATE_IN_PROGRESS = BF_IDLE_UPDATE_IN_PROGRESS,
  PIPE_DEVICE_LOCKED = BF_DEVICE_LOCKED,
  PIPE_INTERNAL_ERROR = BF_INTERNAL_ERROR,
  PIPE_TABLE_NOT_FOUND = BF_TABLE_NOT_FOUND,
  PIPE_NO_HW_ENTRY = BF_NO_HW_ENTRY,
} pipe_status_enum;

/* Routine to get error string corresponding to an error code */

static inline const char *pipe_str_err(pipe_status_t sts) {
  return bf_err_str((bf_status_t)sts);
}

#ifdef __cplusplus
}
#endif /* C++ */

#endif /* _PIPE_MGR_ERR_H */
