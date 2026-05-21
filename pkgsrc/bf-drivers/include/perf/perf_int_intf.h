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
 * @file perf_int_intf.h
 * @date
 *
 * Performance interrupts handling definitions.
 */

#ifndef _PERF_INT_INTF_H
#define _PERF_INT_INTF_H

extern struct test_description interrupts_test;

enum interrupts_enum_res { RES_INT_BUS };
enum interrupts_int_res { RES_INTERRUPTS, RES_ITERATIONS };
enum interrupts_double_res {
  RES_LAT_AVG,
  RES_LAT_SD,
  RES_LAT_MIN,
  RES_LAT_MAX
};

/**
 * @brief Run performance test that will measure latency of the interrupts
 * processing
 *
 * @param dev_id device id
 * @param bus_type Bus type
 * @param it number of iterations
 * @return interrupts_result
 */
struct test_results interrupts(bf_dev_id_t dev_id,
                               perf_bus_t_enum bus_type,
                               int it);

#endif
