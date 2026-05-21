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

#ifndef _LLD_PYTHON_SHELL_MUTEX_H
#define _LLD_PYTHON_SHELL_MUTEX_H

#include <target-sys/bf_sal/bf_sys_sem.h>

typedef struct py_shell_context_t_ {
  bf_sys_mutex_t python_exclude_mutex;
} py_shell_context_t;

extern py_shell_context_t py_shell_ctx;

static inline bool try_py_shell_lock(void) {
  if (0 == bf_sys_mutex_trylock(&py_shell_ctx.python_exclude_mutex)) {
    return true;
  } else {
    return false;
  }
}
#define INIT_PYTHON_SHL_LOCK() \
  { bf_sys_mutex_init(&py_shell_ctx.python_exclude_mutex); }

#define TRY_PYTHON_SHL_LOCK() try_py_shell_lock();

#define RELEASE_PYTHON_SHL_LOCK() \
  { bf_sys_mutex_unlock(&py_shell_ctx.python_exclude_mutex); }

#endif  //_LLD_PYTHON_SHELL_MUTEX_H
