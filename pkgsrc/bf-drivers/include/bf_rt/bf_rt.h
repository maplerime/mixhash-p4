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

/** @file bf_rt.h
 *
 *  @brief One-stop C header file for applications to include for
 *  using BFRT C-frontend
 */
#ifndef _BF_RT_H
#define _BF_RT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bf_rt/bf_rt_common.h>
#include <bf_rt/bf_rt_info.h>
#include <bf_rt/bf_rt_init.h>
#include <bf_rt/bf_rt_learn.h>
#include <bf_rt/bf_rt_session.h>
#include <bf_rt/bf_rt_table.h>
#include <bf_rt/bf_rt_table_attributes.h>
#include <bf_rt/bf_rt_table_data.h>
#include <bf_rt/bf_rt_table_key.h>
#include <bf_rt/bf_rt_table_operations.h>

#ifdef __cplusplus
}
#endif

#endif  //_BF_RT_H
