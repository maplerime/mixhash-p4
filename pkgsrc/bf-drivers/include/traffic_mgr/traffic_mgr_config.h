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
 * @brief traffic_mgr Configuration Header
 *
 * @addtogroup traffic_mgr-config
 * @{
 *
 */
#ifndef __TRAFFIC_MGR_CONFIG_H__
#define __TRAFFIC_MGR_CONFIG_H__

#ifdef GLOBAL_INCLUDE_CUSTOM_CONFIG
#include <global_custom_config.h>
#endif
#ifdef TRAFFIC_MGR_INCLUDE_CUSTOM_CONFIG
#include <traffic_mgr_custom_config.h>
#endif

/**
 * TRAFFIC_MGR_CONFIG_PORTING_STDLIB
 *
 * Default all porting macros to use the C standard libraries. */

#ifndef TRAFFIC_MGR_CONFIG_PORTING_STDLIB
#define TRAFFIC_MGR_CONFIG_PORTING_STDLIB 1
#endif

/**
 * TRAFFIC_MGR_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
 *
 * Include standard library headers for stdlib porting macros. */

#ifndef TRAFFIC_MGR_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
#define TRAFFIC_MGR_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS \
  TRAFFIC_MGR_CONFIG_PORTING_STDLIB
#endif

/**
 * TRAFFIC_MGR_CONFIG_INCLUDE_UCLI
 *
 * Include generic uCli support. */

#ifndef TRAFFIC_MGR_CONFIG_INCLUDE_UCLI
#define TRAFFIC_MGR_CONFIG_INCLUDE_UCLI 0
#endif

#include "traffic_mgr_porting.h"

#endif /* __TRAFFIC_MGR_CONFIG_H__ */
/* @} */
