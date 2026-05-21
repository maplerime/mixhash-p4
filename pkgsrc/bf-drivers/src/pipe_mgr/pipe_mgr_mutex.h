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
 * Internal definitions for pipe_mgr locks and related data structures.
 */
#ifndef PIPE_MGR_MUTEX_H
#define PIPE_MGR_MUTEX_H

#ifdef PIPE_MGR_THREADS
#include <target-sys/bf_sal/bf_sys_sem.h>
#include "pipe_mgr_log.h"
#define pipe_mgr_mutex_t bf_sys_mutex_t
#define pipe_mgr_rmutex_t bf_sys_rmutex_t
#define pipe_mgr_rwlock_t bf_sys_rwlock_t
#define pipe_mgr_rw_mutex_lock_t bf_sys_rw_mutex_lock_t
#define pipe_mgr_cvar_t bf_sys_cond_t
#define PIPE_MGR_LOCK(x)                                            \
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

#define PIPE_MGR_UNLOCK(x)                                               \
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

#define PIPE_MGR_LOCK_INIT(x)                                                 \
  do {                                                                        \
    int A_A = bf_sys_mutex_init(&(x));                                        \
    if (A_A) {                                                                \
      char errStr[32] = {0};                                                  \
      strerror_r(A_A, errStr, 32);                                            \
      LOG_ERROR("LOCKINIT: Error \"%s\" (%d) initializing \"%s\" from %s:%d", \
                errStr,                                                       \
                A_A,                                                          \
                #x,                                                           \
                __func__,                                                     \
                __LINE__);                                                    \
    }                                                                         \
  } while (0);

#define PIPE_MGR_LOCK_DESTROY(x)                                             \
  do {                                                                       \
    int A_A = bf_sys_mutex_del(x);                                           \
    if (A_A) {                                                               \
      char errStr[32] = {0};                                                 \
      strerror_r(A_A, errStr, 32);                                           \
      LOG_ERROR("LOCKDESTROY: Error \"%s\" (%d) deleting \"%s\" from %s:%d", \
                errStr,                                                      \
                A_A,                                                         \
                #x,                                                          \
                __func__,                                                    \
                __LINE__);                                                   \
    }                                                                        \
  } while (0);

#define PIPE_MGR_LOCK_R(x)                                          \
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

#define PIPE_MGR_UNLOCK_R(x)                                             \
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

#define PIPE_MGR_LOCK_R_INIT(x)                                               \
  do {                                                                        \
    int A_A = bf_sys_rmutex_init(&(x));                                       \
    if (A_A) {                                                                \
      char errStr[32] = {0};                                                  \
      strerror_r(A_A, errStr, 32);                                            \
      LOG_ERROR("LOCKINIT: Error \"%s\" (%d) initializing \"%s\" from %s:%d", \
                errStr,                                                       \
                A_A,                                                          \
                #x,                                                           \
                __func__,                                                     \
                __LINE__);                                                    \
    }                                                                         \
  } while (0);

#define PIPE_MGR_LOCK_R_DESTROY(x)                                           \
  do {                                                                       \
    int A_A = bf_sys_rmutex_del(x);                                          \
    if (A_A) {                                                               \
      char errStr[32] = {0};                                                 \
      strerror_r(A_A, errStr, 32);                                           \
      LOG_ERROR("LOCKDESTROY: Error \"%s\" (%d) deleting \"%s\" from %s:%d", \
                errStr,                                                      \
                A_A,                                                         \
                #x,                                                          \
                __func__,                                                    \
                __LINE__);                                                   \
    }                                                                        \
  } while (0);

#define PIPE_MGR_RW_TRYRDLOCK(x)                                    \
  do {                                                              \
    int A_A = bf_sys_rwlock_tryrdlock(x);                           \
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

#define PIPE_MGR_RW_RDLOCK(x)                                       \
  do {                                                              \
    int A_A = bf_sys_rwlock_rdlock(x);                              \
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

#define PIPE_MGR_RW_WRLOCK(x)                                       \
  do {                                                              \
    int A_A = bf_sys_rwlock_wrlock(x);                              \
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

#define PIPE_MGR_RW_UNLOCK(x)                                            \
  do {                                                                   \
    int A_A = bf_sys_rwlock_unlock(x);                                   \
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

#define PIPE_MGR_RWLOCK_INIT(x)                                               \
  do {                                                                        \
    int A_A = bf_sys_rwlock_init(&(x), NULL);                                 \
    if (A_A) {                                                                \
      char errStr[32] = {0};                                                  \
      strerror_r(A_A, errStr, 32);                                            \
      LOG_ERROR("LOCKINIT: Error \"%s\" (%d) initializing \"%s\" from %s:%d", \
                errStr,                                                       \
                A_A,                                                          \
                #x,                                                           \
                __func__,                                                     \
                __LINE__);                                                    \
    }                                                                         \
  } while (0);

#define PIPE_MGR_RWLOCK_DESTROY(x)                                             \
  do {                                                                         \
    int A_A = bf_sys_rwlock_del(x);                                            \
    if (A_A) {                                                                 \
      char errStr[32] = {0};                                                   \
      strerror_r(A_A, errStr, 32);                                             \
      LOG_ERROR("RWLOCKDESTROY: Error \"%s\" (%d) deleting \"%s\" from %s:%d", \
                errStr,                                                        \
                A_A,                                                           \
                #x,                                                            \
                __func__,                                                      \
                __LINE__);                                                     \
    }                                                                          \
  } while (0);

#define PIPE_MGR_RW_MUTEX_RDLOCK(x)                                   \
  do {                                                                \
    int A_A = bf_sys_rw_mutex_lock_rdlock(x);                         \
    if (A_A) {                                                        \
      char errStr[32] = {0};                                          \
      strerror_r(A_A, errStr, 32);                                    \
      LOG_ERROR("RDLOCK: Error \"%s\" (%d) taking \"%s\" from %s:%d", \
                errStr,                                               \
                A_A,                                                  \
                #x,                                                   \
                __func__,                                             \
                __LINE__);                                            \
    }                                                                 \
  } while (0);

#define PIPE_MGR_RW_MUTEX_WRLOCK(x)                                   \
  do {                                                                \
    int A_A = bf_sys_rw_mutex_lock_wrlock(x);                         \
    if (A_A) {                                                        \
      char errStr[32] = {0};                                          \
      strerror_r(A_A, errStr, 32);                                    \
      LOG_ERROR("WRLOCK: Error \"%s\" (%d) taking \"%s\" from %s:%d", \
                errStr,                                               \
                A_A,                                                  \
                #x,                                                   \
                __func__,                                             \
                __LINE__);                                            \
    }                                                                 \
  } while (0);

#define PIPE_MGR_RW_MUTEX_RDUNLOCK(x)                                      \
  do {                                                                     \
    int A_A = bf_sys_rw_mutex_lock_rdunlock(x);                            \
    if (A_A) {                                                             \
      char errStr[32] = {0};                                               \
      strerror_r(A_A, errStr, 32);                                         \
      LOG_ERROR("RDUNLOCK: Error \"%s\" (%d) releasing \"%s\" from %s:%d", \
                errStr,                                                    \
                A_A,                                                       \
                #x,                                                        \
                __func__,                                                  \
                __LINE__);                                                 \
    }                                                                      \
  } while (0);

#define PIPE_MGR_RW_MUTEX_WRUNLOCK(x)                                      \
  do {                                                                     \
    int A_A = bf_sys_rw_mutex_lock_wrunlock(x);                            \
    if (A_A) {                                                             \
      char errStr[32] = {0};                                               \
      strerror_r(A_A, errStr, 32);                                         \
      LOG_ERROR("WRUNLOCK: Error \"%s\" (%d) releasing \"%s\" from %s:%d", \
                errStr,                                                    \
                A_A,                                                       \
                #x,                                                        \
                __func__,                                                  \
                __LINE__);                                                 \
    }                                                                      \
  } while (0);

#define PIPE_MGR_RW_MUTEX_LOCK_INIT(x)                                      \
  do {                                                                      \
    int A_A = bf_sys_rw_mutex_lock_init(&(x));                              \
    if (A_A) {                                                              \
      char errStr[32] = {0};                                                \
      strerror_r(A_A, errStr, 32);                                          \
      LOG_ERROR(                                                            \
          "RW_MUTEX_LOCK_INIT: Error \"%s\" (%d) initializing \"%s\" from " \
          "%s:%d",                                                          \
          errStr,                                                           \
          A_A,                                                              \
          #x,                                                               \
          __func__,                                                         \
          __LINE__);                                                        \
    }                                                                       \
  } while (0);

#define PIPE_MGR_RW_MUTEX_LOCK_DESTROY(x)                                  \
  do {                                                                     \
    int A_A = bf_sys_rw_mutex_lock_del(x);                                 \
    if (A_A) {                                                             \
      char errStr[32] = {0};                                               \
      strerror_r(A_A, errStr, 32);                                         \
      LOG_ERROR(                                                           \
          "RW_MUTEX_LOCK_DESTROY: Error \"%s\" (%d) deleting \"%s\" from " \
          "%s:%d",                                                         \
          errStr,                                                          \
          A_A,                                                             \
          #x,                                                              \
          __func__,                                                        \
          __LINE__);                                                       \
    }                                                                      \
  } while (0);

#define PIPE_MGR_COND_SIGNAL(x)                                              \
  do {                                                                       \
    int A_A = bf_sys_cond_wake(x);                                           \
    if (A_A) {                                                               \
      char errStr[32] = {0};                                                 \
      strerror_r(A_A, errStr, 32);                                           \
      LOG_ERROR("CONDSIGNAL: Error \"%s\" (%d) signaling \"%s\" from %s:%d", \
                errStr,                                                      \
                A_A,                                                         \
                #x,                                                          \
                __func__,                                                    \
                __LINE__);                                                   \
    }                                                                        \
  } while (0);

#define PIPE_MGR_COND_BROADCAST_SIGNAL(x)                                 \
  do {                                                                    \
    int A_A = bf_sys_cond_broadcast(x);                                   \
    if (A_A) {                                                            \
      char errStr[32] = {0};                                              \
      strerror_r(A_A, errStr, 32);                                        \
      LOG_ERROR(                                                          \
          "CONDBROADCAST: Error \"%s\" (%d) signaling \"%s\" from %s:%d", \
          errStr,                                                         \
          A_A,                                                            \
          #x,                                                             \
          __func__,                                                       \
          __LINE__);                                                      \
    }                                                                     \
  } while (0);

#define PIPE_MGR_COND_WAIT(x, y)                                         \
  do {                                                                   \
    int A_A = bf_sys_cond_wait(x, y);                                    \
    if (A_A) {                                                           \
      char errStr[32] = {0};                                             \
      strerror_r(A_A, errStr, 32);                                       \
      LOG_ERROR("CONDWAIT: Error \"%s\" (%d) waiting \"%s\" from %s:%d", \
                errStr,                                                  \
                A_A,                                                     \
                #x,                                                      \
                __func__,                                                \
                __LINE__);                                               \
    }                                                                    \
  } while (0);

#define PIPE_MGR_COND_INIT(x)                                             \
  do {                                                                    \
    int A_A = bf_sys_cond_init(&(x));                                     \
    if (A_A) {                                                            \
      char errStr[32] = {0};                                              \
      strerror_r(A_A, errStr, 32);                                        \
      LOG_ERROR("COND: Error \"%s\" (%d) initializing \"%s\" from %s:%d", \
                errStr,                                                   \
                A_A,                                                      \
                #x,                                                       \
                __func__,                                                 \
                __LINE__);                                                \
    }                                                                     \
  } while (0);

#define PIPE_MGR_COND_DESTROY(x)                                      \
  do {                                                                \
    int A_A = bf_sys_cond_del(x);                                     \
    if (A_A) {                                                        \
      char errStr[32] = {0};                                          \
      strerror_r(A_A, errStr, 32);                                    \
      LOG_ERROR("COND: Error \"%s\" (%d) deleting \"%s\" from %s:%d", \
                errStr,                                               \
                A_A,                                                  \
                #x,                                                   \
                __func__,                                             \
                __LINE__);                                            \
    }                                                                 \
  } while (0);

#else
#define pipe_mgr_mutex_t int
#define PIPE_MGR_LOCK(x)
#define PIPE_MGR_UNLOCK(x)
#define PIPE_MGR_LOCK_INIT(x)
#define PIPE_MGR_LOCK_DESTROY(x)
#define pipe_mgr_rmutex_t int
#define PIPE_MGR_LOCK_R(x)
#define PIPE_MGR_UNLOCK_R(x)
#define PIPE_MGR_LOCK_R_INIT(x)
#define PIPE_MGR_LOCK_R_DESTROY(x)
#define pipe_mgr_rwlock_t int
#define pipe_mgr_rw_mutex_lock_t int
#define PIPE_MGR_RW_RDLOCK(x)
#define PIPE_MGR_RW_WRLOCK(x)
#define PIPE_MGR_RW_UNLOCK(x)
#define PIPE_MGR_RWLOCK_INIT(x)
#define pipe_mgr_cvar_t int
#define PIPE_MGR_COND_SIGNAL(x)
#define PIPE_MGR_COND_WAIT(x)
#define PIPE_MGR_COND_INIT(x)
#define PIPE_MGR_COND_DESTROY(x)
#endif

#endif
