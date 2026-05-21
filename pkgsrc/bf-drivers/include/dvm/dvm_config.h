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
 * @brief dvm Configuration Header
 *
 * @addtogroup dvm-config
 * @{
 *
 */
#ifndef __DVM_CONFIG_H__
#define __DVM_CONFIG_H__

#ifdef GLOBAL_INCLUDE_CUSTOM_CONFIG
#include <global_custom_config.h>
#endif
#ifdef DVM_INCLUDE_CUSTOM_CONFIG
#include <dvm_custom_config.h>
#endif

/**
 * DVM_CONFIG_PORTING_STDLIB
 *
 * Default all porting macros to use the C standard libraries. */

#ifndef DVM_CONFIG_PORTING_STDLIB
#define DVM_CONFIG_PORTING_STDLIB 1
#endif

/**
 * DVM_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
 *
 * Include standard library headers for stdlib porting macros. */

#ifndef DVM_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
#define DVM_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS DVM_CONFIG_PORTING_STDLIB
#endif

/**
 * DVM_CONFIG_INCLUDE_UCLI
 *
 * Include generic uCli support. */

#ifndef DVM_CONFIG_INCLUDE_UCLI
#define DVM_CONFIG_INCLUDE_UCLI 0
#endif

#endif /* __DVM_CONFIG_H__ */
/* @} */
