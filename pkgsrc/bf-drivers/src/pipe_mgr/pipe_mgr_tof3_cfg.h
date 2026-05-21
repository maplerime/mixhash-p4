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

/*!
 * @file pipe_mgr_tof3_cfg.h
 * @date
 *
 * Definitions for tof3 device configuration database
 */

#ifndef PIPE_MGR_TOF3_CFG_H
#define PIPE_MGR_TOF3_CFG_H

/* Module header files */
#include <pipe_mgr/pipe_mgr_intf.h>
#include "pipe_mgr_int.h"

#define TOF3_NUM_IPB 9
#define TOF3_NUM_EPB TOF3_NUM_IPB
#define TOF3_PARSER_DEPTH 256
#define TOF3_PARSER_INIT_RAM_DEPTH 16
#define TOF3_PARSER_CSUM_DEPTH 32
#define TOF3_NUM_PIPELINES 4
#define TOF3_NUM_PORTS_PER_PIPE 72
#define TOF3_NUM_CHN_PER_IPB 8
#define TOF3_NUM_CHN_PER_EPB TOF3_NUM_CHN_PER_IPB

#define TOF3_SRAM_UNIT_WIDTH 128
#define TOF3_SRAM_UNIT_DEPTH 1024
#define TOF3_SRAM_NUM_RAM_LINE_BITS 10
#define TOF3_MAXIMUM_ACTION_DATA_BYTES 128

#define TOF3_TCAM_UNIT_WIDTH 44
#define TOF3_TCAM_UNIT_DEPTH 512

#define TOF3_MAP_RAM_UNIT_WIDTH 11
#define TOF3_MAP_RAM_UNIT_DEPTH 1024

#define TOF3_PHASE0_UNIT_WIDTH 32
#define TOF3_PHASE0_UNIT_DEPTH 1

/* Definitions relating to Statistics RAM VPN address */
#define TOF3_STATS_RAM_NUM_SUBWORD_BITS 3
#define TOF3_STATS_RAM_NUM_VPN_BITS 7
#define TOF3_STATS_RAM_NUM_ADDR_BITS 19
#define TOF3_STATS_ADDR_PFE_BIT_POSITION 19

/* Definitions relating to Meters RAM VPN address */
#define TOF3_METER_ADDR_PFE_BIT_POSITION 23
#define TOF3_METER_ADDR_METER_TYPE_BIT_POSITION 24
#define TOF3_METER_ADDR_SEL_TYPE_BIT_POSITION 26
#define TOF3_METER_ADDR_NUM_PFE_BITS 1
#define TOF3_METER_ADDR_NUM_METER_TYPE_BITS 3

#define TOF3_MIN_IDLE_TIMEOUT_COUNTER_BIT 21
/* MAX below is whatever above min So acceptable ranges is 2^21 - 2^36*/
#define TOF3_MAX_IDLE_TIMEOUT_COUNTER_WIDTH 15
#define TOF3_IDLE_SUBWORD_VPN_BITS 4

/* Definitions relating to exact match, match address. */
#define TOF3_EXM_SUBWORD_VPN_BITS 9

/* Number of bits used for virtual memory write */
#define TOF3_SEL_SUBWORD_VPN_BITS 5
/* Number of bits used in the overhead data */
#define TOF3_SELECTOR_HUFFMAN_BITS 7

extern rmt_dev_cfg_t tof3_cfg;

#endif /* PIPE_MGR_TOF3_CFG_H */
