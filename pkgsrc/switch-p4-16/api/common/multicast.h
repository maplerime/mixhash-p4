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

#ifndef __COMMON_MULTICAST_H__
#define __COMMON_MULTICAST_H__

#include <bf_types/bf_types.h>
#include <mc_mgr/mc_mgr_types.h>

/*
 * TODO:
 *  The following looks like it was copied from drivers.
 *  Can we expose and use instead of redefining?
 */

#define SWITCH_MGID_TABLE_SIZE 64 * 1024
#define SWITCH_MGID_ECMP_TABLE_SIZE 64 * 1024

#define SWITCH_MCAST_GROUP_HASH_KEY_SIZE sizeof(switch_mcast_group_key_t)

typedef uint32_t mc_mgrp_hdl_t;

#define SWITCH_API_MAX_PORTS BF_PORT_COUNT

#define SWITCH_RID_INVALID 0
#define SWITCH_API_MAX_LAG BF_LAG_COUNT
#define SWITCH_PORT_ARRAY_SIZE BF_MC_PORT_ARRAY_SIZE
typedef uint8_t switch_mc_port_map_t[SWITCH_PORT_ARRAY_SIZE];
#define SWITCH_LAG_ARRAY_SIZE BF_MC_LAG_ARRAY_SIZE
typedef uint8_t switch_mc_lag_map_t[SWITCH_LAG_ARRAY_SIZE];

#define SWITCH_DEV_PORT_TO_PIPE(_dp) (((_dp) >> 7) & 0x3)
#define SWITCH_DEV_PORT_TO_LOCAL_PORT(_dp) ((_dp)&0x7F)

#define SWITCH_DEV_PORT_TO_BIT_IDX(_dp) \
  (72 * SWITCH_DEV_PORT_TO_PIPE(_dp) + SWITCH_DEV_PORT_TO_LOCAL_PORT(_dp))

#define SWITCH_MC_PORT_MAP_CLEAR(pm, port)          \
  do {                                              \
    int _port_p = SWITCH_DEV_PORT_TO_BIT_IDX(port); \
    switch_mc_port_map_t *_port_pm = &(pm);         \
    if (_port_p >= SWITCH_API_MAX_PORTS) break;     \
    size_t _port_i = (_port_p) / 8;                 \
    unsigned int _port_j = (_port_p) % 8;           \
    (*_port_pm)[_port_i] &= ~(1 << _port_j);        \
  } while (0);

#define SWITCH_MC_PORT_MAP_SET(pm, port)            \
  do {                                              \
    int _port_p = SWITCH_DEV_PORT_TO_BIT_IDX(port); \
    switch_mc_port_map_t *_port_pm = &(pm);         \
    if (_port_p >= SWITCH_API_MAX_PORTS) break;     \
    size_t _port_i = (_port_p) / 8;                 \
    unsigned int _port_j = (_port_p) % 8;           \
    (*_port_pm)[_port_i] |= (1 << _port_j);         \
  } while (0);

#define SWITCH_MC_LAG_MAP_CLEAR(pm, lag)     \
  do {                                       \
    int _lag_p = (lag);                      \
    switch_mc_lag_map_t *_lag_pm = &(pm);    \
    if (_lag_p >= SWITCH_API_MAX_LAG) break; \
    size_t _lag_i = (_lag_p) / 8;            \
    unsigned int _lag_j = (_lag_p) % 8;      \
    (*_lag_pm)[_lag_i] &= ~(1 << _lag_j);    \
  } while (0);

#define SWITCH_MC_LAG_MAP_SET(pm, lag)       \
  do {                                       \
    int _lag_p = (lag);                      \
    switch_mc_lag_map_t *_lag_pm = &(pm);    \
    if (_lag_p >= SWITCH_API_MAX_LAG) break; \
    size_t _lag_i = (_lag_p) / 8;            \
    unsigned int _lag_j = (_lag_p) % 8;      \
    (*_lag_pm)[_lag_i] |= (1 << _lag_j);     \
  } while (0);

#endif  // __COMMON_MULTICAST_H__
