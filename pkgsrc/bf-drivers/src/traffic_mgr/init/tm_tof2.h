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

// This file contains code relevant to Tof2 version of RMT ASIC.
// When a new version of RMT ASIC need to be supported, make a copy
// of this file and change relevant hardware values.

#ifndef __TM_TOF2_H__
#define __TM_TOF2_H__

#include "traffic_mgr/common/tm_ctx.h"

/* TOF2 Hardware Resources */
#define BF_TM_TOF2_BUFFER_CELLS (384 * 1024)
#define BF_TM_TOF2_CELL_SIZE (176)
#define BF_TM_TOF2_PG_PER_PIPE (9)
#define BF_TM_TOF2_PORTS_PER_PG (8)
#define BF_TM_TOF2_PORTS_PER_PIPE                       \
  ((BF_TM_TOF2_PG_PER_PIPE * BF_TM_TOF2_PORTS_PER_PG) + \
   1)  // 72 + 1 mirror (ingress, but not egress) port
#define BF_TM_TOF2_MAU_PIPES (4)
#define BF_TM_TOF2_PPG_PER_PIPE (128)
#define BF_TM_TOF2_DEFAULT_PPG_PER_PIPE (BF_TM_TOF2_PORTS_PER_PIPE)
#define BF_TM_TOF2_TOTAL_PPG_PER_PIPE \
  (BF_TM_TOF2_PPG_PER_PIPE + BF_TM_TOF2_DEFAULT_PPG_PER_PIPE)

#define BF_TM_TOF2_PFC_LEVELS BF_TM_MAX_PFC_LEVELS

#define BF_TM_TOF2_APP_POOLS (4)
#define BF_TM_TOF2_QUEUES_PER_PG (128)
#define BF_TM_TOF2_HW_QUEUES_PER_PG (128)
#define BF_TM_TOF2_MIN_QUEUES_PER_PORT (16)
#define BF_TM_TOF2_TOTAL_QUEUES_PER_PIPE \
  (BF_TM_TOF2_QUEUES_PER_PG * BF_TM_TOF2_PG_PER_PIPE)  // 1152
#define BF_TM_TOF2_SCH_L1_PER_PG (32)
#define BF_TM_TOF2_TOTAL_SCH_L1_PER_PIPE \
  (BF_TM_TOF2_SCH_L1_PER_PG * BF_TM_TOF2_PG_PER_PIPE)  // 288

#define BF_TM_TOF2_RESUME_PROFILES (32)

#define BF_TM_TOF2_PERCENTAGE_LIMIT(limit) ((limit) >> 24)
#define BF_TM_TOF2_CELL_LIMIT(limit) ((limit) & (0xffffff))

#define BF_TM_TOF2_WAC_RESET_HYSTERESIS (0x10)  // in 8 cells unit
#define BF_TM_TOF2_QAC_RESET_HYSTERESIS (0)     // in 8 cells unit
#define BF_TM_TOF2_WAC_POR_HYSTERESIS (4)       // in 8 cells unit
#define BF_TM_TOF2_QAC_POR_HYSTERESIS (4)       // in 8 cells unit
#define BF_TM_TOF2_WAC_DEFAULT_HYSTERESIS (BF_TM_TOF2_WAC_POR_HYSTERESIS * 8)
#define BF_TM_TOF2_QAC_DEFAULT_HYSTERESIS (BF_TM_TOF2_QAC_POR_HYSTERESIS * 8)
#define BF_TM_TOF2_HYSTERESIS_PROFILES (32)

#define BF_TM_TOF2_Q_PROF_CNT (288)

#define BF_TM_TOF2_UC_CT_MAX_CELLS (127)

#endif
