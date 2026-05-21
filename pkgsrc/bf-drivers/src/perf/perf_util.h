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
 * @file perf_util.h
 * @date
 *
 * Performance utils handling definitions.
 */

#ifndef _PERF_UTIL_H
#define _PERF_UTIL_H

/**
 * @brief Count average of elements in an array
 *
 * @param array array of elements to be counted
 * @param elements number of elements in the array
 * @return double average value of the elements in the array
 */
double average(double array[], int elements);

/**
 * @brief Count average value and standard deviation of the elements in an array
 *
 * @param array array of values to be counted
 * @param elements number of elements in the array
 * @param avg pointer for result average value
 * @param sd pointer for result standard deviation value
 */
void basic_stats(double array[], int elements, double *avg, double *sd);

/**
 * @brief Calculates the difference between two timestamps
 *
 * @param start Start time
 * @param stop End time
 * @return time in nanoseconds
 */
uint64_t time_delta_ns(struct timespec start, struct timespec stop);

/**
 * @brief Counts rates based on the timestamps and number of operations
 *
 * @param start Start timestamp
 * @param stop End timestamp
 * @param operations number of operations performed
 * @param op_per_s rate in operations per second
 * @param ns_per_op nanoseconds per operations
 * @return bool
 */
bool ts_to_ops(struct timespec start,
               struct timespec stop,
               int operations,
               double *op_per_s,
               double *ns_per_op);

/**
 * @brief Counts rates based on the timestamps and number of bytes
 *
 * @param start Start timestamp
 * @param stop End timestamp
 * @param bytes number of bytes
 * @param mb_per_s rate in megabytes per second
 * @param us_per_mb microseconds per megabyte
 * @return bool
 */
bool ts_to_mb(struct timespec start,
              struct timespec stop,
              int bytes,
              double *mb_per_s,
              double *us_per_mb);

#endif
