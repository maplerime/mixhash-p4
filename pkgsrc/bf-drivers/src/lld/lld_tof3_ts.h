
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
 *  @file lld_tof3_ts.h
 *  @date
 *
 */

#ifndef __LLD_TOF3_TS_H_
#define __LLD_TOF3_TS_H_

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t lld_tof3_ts_global_ts_state_set(bf_dev_id_t dev_id, bool enable);
bf_status_t lld_tof3_ts_global_ts_state_get(bf_dev_id_t dev_id, bool *enable);
bf_status_t lld_tof3_ts_global_ts_value_set(bf_dev_id_t dev_id,
                                            uint64_t global_ts_ns);
bf_status_t lld_tof3_ts_global_ts_value_get(bf_dev_id_t dev_id,
                                            uint64_t *global_ts_ns);
bf_status_t lld_tof3_ts_global_ts_inc_value_set(bf_dev_id_t dev_id,
                                                uint32_t global_inc_ns);
bf_status_t lld_tof3_ts_global_ts_inc_value_get(bf_dev_id_t dev_id,
                                                uint32_t *global_inc_ns);

bf_status_t lld_tof3_ts_global_ts_increment_one_time_set(
    bf_dev_id_t dev_id, uint64_t global_ts_inc_time_ns);
bf_status_t lld_tof3_ts_global_ts_offset_set(bf_dev_id_t dev_id,
                                             uint64_t global_ts_offset_ns);
bf_status_t lld_tof3_ts_global_ts_offset_get(bf_dev_id_t dev_id,
                                             uint64_t *global_ts_offset_ns);
bf_status_t lld_tof3_ts_global_baresync_ts_get(bf_dev_id_t dev_id,
                                               uint64_t *global_ts_ns,
                                               uint64_t *baresync_ts_ns);
// bf_status_t lld_tof3_ts_global_ts_periodic_distribution_timer_set(bf_dev_id_t
// dev_id,
//                                                            uint32_t
//                                                            timer_ns);
bf_status_t lld_tof3_ts_baresync_state_set(bf_dev_id_t dev_id,
                                           uint32_t reset_count_threshold,
                                           uint32_t debounce_count,
                                           bool enable);
bf_status_t lld_tof3_ts_baresync_state_get(bf_dev_id_t dev_id,
                                           uint32_t *reset_count_threshold,
                                           uint32_t *debounce_count,
                                           bool *enable);
bf_status_t lld_tof3_ts_baresync_reset_value_set(bf_dev_id_t dev_id,
                                                 uint64_t baresync_time_ns);
bf_status_t lld_tof3_ts_baresync_reset_value_get(bf_dev_id_t dev_id,
                                                 uint64_t *baresync_time_ns);
bf_status_t lld_tof3_ts_baresync_increment_set(
    bf_dev_id_t dev_id,
    uint32_t baresync_inc_time_ns,
    uint32_t baresync_inc_time_fract_ns,
    uint32_t baresync_inc_time_fract_den);
bf_status_t lld_tof3_ts_baresync_increment_get(
    bf_dev_id_t dev_id,
    uint32_t *baresync_inc_time_ns,
    uint32_t *baresync_inc_time_fract_ns,
    uint32_t *baresync_inc_time_fract_den);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
