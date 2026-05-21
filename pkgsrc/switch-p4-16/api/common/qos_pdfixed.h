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

#ifndef __COMMON_QOS_PDFIXED_H__
#define __COMMON_QOS_PDFIXED_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <tofino/pdfixed/pd_tm.h>
#ifdef __cplusplus
}
#endif

#include "bf_switch/bf_switch.h"

/** maximum ingress buffer pools */
#define SWITCH_BUFFER_POOL_INGRESS_MAX 4

/** maximum egress buffer pools */
#define SWITCH_BUFFER_POOL_EGRESS_MAX 4

/** maximum buffer profiles */
#define SWITCH_BUFFER_PROFILE_MAX 64

#define SWITCH_BUFFER_PFC_ICOS_MAX 8

#define SWITCH_BUFFER_DYNAMIC_THRESHOLD_FACTOR 32

#define SWITCH_DEFAULT_PPG_INDEX 0xFF

#define SWITCH_MAX_PPGS_PER_PIPE 0x80

/** ASIC's default PPG starts from 128 */
#define SWITCH_ASIC_FIRST_DEFAULT_PPG_HANDLE 128

/** Default buffer pool size */
#define SWITCH_BUFFER_POOL_DEFAULT 80000

#endif  // __COMMON_QOS_PDFIXED_H__
