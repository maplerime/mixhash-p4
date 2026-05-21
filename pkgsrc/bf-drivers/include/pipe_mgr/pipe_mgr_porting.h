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
 * @brief pipe_mgr Porting Macros.
 *
 * @addtogroup pipe_mgr-porting
 * @{
 *
 */
#ifndef __PIPE_MGR_PORTING_H__
#define __PIPE_MGR_PORTING_H__

#include <pipe_mgr/pipe_mgr_config.h>

/* <auto.start.portingmacro(ALL).define> */
#if PIPE_MGR_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS == 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#include <assert.h>
#endif
#include <target-sys/bf_sal/bf_sys_intf.h>

#define PIPE_MGR_MALLOC bf_sys_malloc
#define PIPE_MGR_REALLOC bf_sys_realloc
#define PIPE_MGR_CALLOC bf_sys_calloc
#define PIPE_MGR_FREE bf_sys_free

#define PIPE_MGR_ASSERT bf_sys_assert
#define PIPE_MGR_DBGCHK bf_sys_dbgchk

#ifndef PIPE_MGR_MEMSET
#if defined(GLOBAL_MEMSET)
#define PIPE_MGR_MEMSET GLOBAL_MEMSET
#elif PIPE_MGR_CONFIG_PORTING_STDLIB == 1
#define PIPE_MGR_MEMSET memset
#else
#error The macro PIPE_MGR_MEMSET is required but cannot be defined.
#endif
#endif

#ifndef PIPE_MGR_MEMCPY
#if defined(GLOBAL_MEMCPY)
#define PIPE_MGR_MEMCPY GLOBAL_MEMCPY
#elif PIPE_MGR_CONFIG_PORTING_STDLIB == 1
#define PIPE_MGR_MEMCPY memcpy
#else
#error The macro PIPE_MGR_MEMCPY is required but cannot be defined.
#endif
#endif

#ifndef PIPE_MGR_MEMCMP
#if defined(GLOBAL_MEMCMP)
#define PIPE_MGR_MEMCMP GLOBAL_MEMCMP
#elif PIPE_MGR_CONFIG_PORTING_STDLIB == 1
#define PIPE_MGR_MEMCMP memcmp
#else
#error The macro PIPE_MGR_MEMCMP is required but cannot be defined.
#endif
#endif

/* <auto.end.portingmacro(ALL).define> */

#define PIPE_MGR_NUM_DEVICES BF_MAX_DEV_COUNT
#define PIPE_MGR_NUM_SUBDEVICES BF_MAX_SUBDEV_COUNT
#define PIPE_MGR_MAX_SESSIONS 20

/* Keeping the below defines decoupled from Tofino. These just define the
 * max values possible. Used for array declarations etc. For the
 * exact number of pipes in a device, the dev_info structure needs to be
 * looked up
 */
#define PIPE_MGR_MAX_PIPES BF_PIPE_COUNT

#endif /* __PIPE_MGR_PORTING_H__ */
/* @} */
