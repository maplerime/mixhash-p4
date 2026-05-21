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
 * @file perf_int.h
 * @date
 *
 * Performance interrupts handling common definitions.
 */

#ifndef _PERF_INT_H
#define _PERF_INT_H

/* Interrupt tests callback info */
#define PERF_INT_CACHE_MAX 1000
#define PERF_INT_ITERS 300

struct interrupts_result {
  bool status;
  perf_bus_t_enum bus_type;
  int interrupts;
  int iterations;
  double avg_latency_us;
  double sd_latency_us;
  double min_latency_us;
  double max_latency_us;
};

typedef struct perf_int_cache {
  bool valid;
  lld_int_cb cb_fn;
  void *userdata;
  uint32_t status_addr;
  uint32_t status_val;
  uint32_t exp_int_status_val;
  uint32_t enable_addr;
  uint32_t enable_val;
  uint32_t inject_addr;
  struct timespec start;
  struct timespec stop;
  bool callback_received;
} perf_int_cache_t;

/**
 * @brief Return whether interrupts are support for a given bus type
 *
 * @param dev_id device id
 * @param bus_type bus type
 * @return bool
 */
bool is_bus_ints_supported(bf_dev_id_t dev_id, perf_bus_t_enum bus_type);

/**
 * @brief Run performance test that will measure latency of
 * interrupts processing
 *
 * @param uc ucli context pointer
 * @param dev_id device id
 * @param it number of iterations
 * @return bf_status_t
 */
bf_status_t run_int_test(bf_dev_id_t dev_id,
                         perf_bus_t_enum bus_type,
                         int it,
                         struct interrupts_result *result);

/**
 * @brief Run performance test that will measure latency of
 * interrupts processing
 *
 * @param uc ucli context pointer
 * @param dev_id device id
 * @return bf_status_t
 */
bf_status_t run_interrupts(ucli_context_t *uc, bf_dev_id_t dev_id);

#endif
