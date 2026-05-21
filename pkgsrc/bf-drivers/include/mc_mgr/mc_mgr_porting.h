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

/**
 *
 * @file
 * @brief mc_mgr Porting Macros.
 *
 * @addtogroup mc_mgr-porting
 * @{
 *
 */
#ifndef __MC_MGR_PORTING_H__
#define __MC_MGR_PORTING_H__

/* <auto.start.portingmacro(ALL).define> */
#if MC_MGR_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS == 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#include <assert.h>
#include <errno.h>
#endif
#include <target-sys/bf_sal/bf_sys_intf.h>

#define MC_MGR_MALLOC bf_sys_malloc
#define MC_MGR_REALLOC bf_sys_realloc
#define MC_MGR_CALLOC bf_sys_calloc
#define MC_MGR_FREE bf_sys_free

#define MC_MGR_ASSERT bf_sys_assert
#define MC_MGR_DBGCHK bf_sys_dbgchk

#ifndef MC_MGR_MEMSET
#if defined(GLOBAL_MEMSET)
#define MC_MGR_MEMSET GLOBAL_MEMSET
#elif MC_MGR_CONFIG_PORTING_STDLIB == 1
#define MC_MGR_MEMSET memset
#else
#error The macro MC_MGR_MEMSET is required but cannot be defined.
#endif
#endif

#ifndef MC_MGR_MEMCPY
#if defined(GLOBAL_MEMCPY)
#define MC_MGR_MEMCPY GLOBAL_MEMCPY
#elif MC_MGR_CONFIG_PORTING_STDLIB == 1
#define MC_MGR_MEMCPY memcpy
#else
#error The macro MC_MGR_MEMCPY is required but cannot be defined.
#endif
#endif

#ifndef MC_MGR_STRNCPY
#if defined(GLOBAL_STRNCPY)
#define MC_MGR_STRNCPY GLOBAL_STRNCPY
#elif MC_MGR_CONFIG_PORTING_STDLIB == 1
#define MC_MGR_STRNCPY strncpy
#else
#error The macro MC_MGR_STRNCPY is required but cannot be defined.
#endif
#endif

#ifndef MC_MGR_VSNPRINTF
#if defined(GLOBAL_VSNPRINTF)
#define MC_MGR_VSNPRINTF GLOBAL_VSNPRINTF
#elif MC_MGR_CONFIG_PORTING_STDLIB == 1
#define MC_MGR_VSNPRINTF vsnprintf
#else
#error The macro MC_MGR_VSNPRINTF is required but cannot be defined.
#endif
#endif

#ifndef MC_MGR_SNPRINTF
#if defined(GLOBAL_SNPRINTF)
#define MC_MGR_SNPRINTF GLOBAL_SNPRINTF
#elif MC_MGR_CONFIG_PORTING_STDLIB == 1
#define MC_MGR_SNPRINTF snprintf
#else
#error The macro MC_MGR_SNPRINTF is required but cannot be defined.
#endif
#endif

#ifndef MC_MGR_STRLEN
#if defined(GLOBAL_STRLEN)
#define MC_MGR_STRLEN GLOBAL_STRLEN
#elif MC_MGR_CONFIG_PORTING_STDLIB == 1
#define MC_MGR_STRLEN strlen
#else
#error The macro MC_MGR_STRLEN is required but cannot be defined.
#endif
#endif

/* <auto.end.portingmacro(ALL).define> */

#define MC_MGR_NUM_DEVICES BF_MAX_DEV_COUNT
#define MC_MGR_NUM_SESSIONS 20
#define MC_MGR_NUM_PIPES BF_PIPE_COUNT

#ifdef MC_MGR_THREADS
#include <target-sys/bf_sal/bf_sys_sem.h>
#include <time.h>
#define mc_mutex_t bf_sys_mutex_t
#define mc_rmutex_t bf_sys_rmutex_t
#define MC_MGR_LOCK(x)                                              \
  do {                                                              \
    int A_A = bf_sys_mutex_lock(x);                                 \
    if (A_A) {                                                      \
      char errStr[32] = {0};                                        \
      strerror_r(A_A, errStr, 32);                                  \
      LOG_ERROR("LOCK: Error \"%s\" (%d) taking \"%s\" from %s:%d", \
                errStr,                                             \
                A_A,                                                \
                #x,                                                 \
                __func__,                                           \
                __LINE__);                                          \
    }                                                               \
  } while (0);

#define MC_MGR_UNLOCK(x)                                                 \
  do {                                                                   \
    int A_A = bf_sys_mutex_unlock(x);                                    \
    if (A_A) {                                                           \
      char errStr[32] = {0};                                             \
      strerror_r(A_A, errStr, 32);                                       \
      LOG_ERROR("UNLOCK: Error \"%s\" (%d) releasing \"%s\" from %s:%d", \
                errStr,                                                  \
                A_A,                                                     \
                #x,                                                      \
                __func__,                                                \
                __LINE__);                                               \
    }                                                                    \
  } while (0);

#define MC_MGR_LOCK_INIT(x)  \
  do {                       \
    bf_sys_mutex_init(&(x)); \
  } while (0);
#define MC_MGR_LOCK_DEL(x)  \
  do {                      \
    bf_sys_mutex_del(&(x)); \
  } while (0);

#define MC_MGR_LOCK_R(x)                                            \
  do {                                                              \
    int A_A = bf_sys_rmutex_lock(x);                                \
    if (A_A) {                                                      \
      char errStr[32] = {0};                                        \
      strerror_r(A_A, errStr, 32);                                  \
      LOG_ERROR("LOCK: Error \"%s\" (%d) taking \"%s\" from %s:%d", \
                errStr,                                             \
                A_A,                                                \
                #x,                                                 \
                __func__,                                           \
                __LINE__);                                          \
    }                                                               \
  } while (0);

/* Try lock only logs a warn upon not taking the lock */
#define MC_MGR_TRYLOCK_R(x, err)                                    \
  do {                                                              \
    int A_A = bf_sys_rmutex_trylock(x);                             \
    err = A_A;                                                      \
    if (A_A && A_A != EBUSY) {                                      \
      char errStr[32] = {0};                                        \
      strerror_r(A_A, errStr, 32);                                  \
      LOG_ERROR("LOCK: Error \"%s\" (%d) taking \"%s\" from %s:%d", \
                errStr,                                             \
                A_A,                                                \
                #x,                                                 \
                __func__,                                           \
                __LINE__);                                          \
    }                                                               \
  } while (0);

#define MC_MGR_UNLOCK_R(x)                                               \
  do {                                                                   \
    int A_A = bf_sys_rmutex_unlock(x);                                   \
    if (A_A) {                                                           \
      char errStr[32] = {0};                                             \
      strerror_r(A_A, errStr, 32);                                       \
      LOG_ERROR("UNLOCK: Error \"%s\" (%d) releasing \"%s\" from %s:%d", \
                errStr,                                                  \
                A_A,                                                     \
                #x,                                                      \
                __func__,                                                \
                __LINE__);                                               \
    }                                                                    \
  } while (0);

#define MC_MGR_LOCK_INIT_R(x) \
  do {                        \
    bf_sys_rmutex_init(&(x)); \
  } while (0);
#else
#define mc_mutex_t int
#define mc_rmutex_t int
#define MC_MGR_LOCK(x)
#define MC_MGR_UNLOCK(x)
#define MC_MGR_LOCK_INIT(x)
#define MC_MGR_LOCK_DEL(x)
#define MC_MGR_LOCK_R(x)
#define MC_MGR_TRYLOCK_R(x)
#define MC_MGR_UNLOCK_R(x)
#define MC_MGR_LOCK_INIT_R(x)
#endif

#endif /* __MC_MGR_PORTING_H__ */
       /* @} */
