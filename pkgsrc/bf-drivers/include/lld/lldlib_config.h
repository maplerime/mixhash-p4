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
 * @brief lld Configuration Header
 *
 * @addtogroup lld-config
 * @{
 *
 */
#ifndef __LLDLIB_CONFIG_H__
#define __LLDLIB_CONFIG_H__

#ifdef GLOBAL_INCLUDE_CUSTOM_CONFIG
#include <global_custom_config.h>
#endif
#ifdef LLDLIB_INCLUDE_CUSTOM_CONFIG
#include <lld_custom_config.h>
#endif

/**
 * LLDLIB_CONFIG_PORTING_STDLIB
 *
 * Default all porting macros to use the C standard libraries. */

#ifndef LLDLIB_CONFIG_PORTING_STDLIB
#define LLDLIB_CONFIG_PORTING_STDLIB 1
#endif

/**
 * LLDLIB_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
 *
 * Include standard library headers for stdlib porting macros. */

#ifndef LLDLIB_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
#define LLDLIB_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS \
  LLDLIB_CONFIG_PORTING_STDLIB
#endif

/**
 * LLDLIB_CONFIG_INCLUDE_UCLI
 *
 * Include generic uCli support. */

#ifndef LLDLIB_CONFIG_INCLUDE_UCLI
#define LLDLIB_CONFIG_INCLUDE_UCLI 0
#endif

#endif /* __LLDLIB_CONFIG_H__ */
/* @} */
