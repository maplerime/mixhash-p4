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

#include <errno.h>
#include <math.h>

#include <target-utils/uCli/ucli.h>
#include <bfutils/bf_utils.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_dev.h>
#include <lld/lld_sku.h>

#include "perf_util.h"

/**
 * @brief Count average of elements in an array
 *
 * @param array array of elements to be counted
 * @param elements number of elements in the array
 * @return double average value of the elements in the array
 */
double average(double array[], int elements) {
  if (elements < 1) {
    bf_sys_dbgchk(0);
    return 0;
  }
  double sum = 0;
  for (int i = 0; i < elements; i++) {
    sum = sum + array[i];
  }
  return sum / (double)elements;
}

/**
 * @brief Count average value and standard deviation of the elements in an array
 *
 * @param array array of values to be counted
 * @param elements number of elements in the array
 * @param avg pointer for result average value
 * @param sd pointer for result standard deviation value
 */
void basic_stats(double array[], int elements, double *avg, double *sd) {
  if (elements < 1) {
    bf_sys_dbgchk(0);
    return;
  }
  double mid = 0;
  *avg = average(array, elements);
  for (int i = 0; i < elements; i++) {
    mid += pow(array[i] - *avg, 2);
  }
  *sd = sqrt(mid / elements);
}

/**
 * @brief Calculates the difference between two timestamps
 *
 * @param start Start time
 * @param stop End time
 * @return time in nanoseconds
 */
uint64_t time_delta_ns(struct timespec start, struct timespec stop) {
  struct timespec delta;
  if (stop.tv_nsec < start.tv_nsec) {
    stop.tv_sec -= 1;
    stop.tv_nsec += 1000000000;
  }
  delta.tv_nsec = stop.tv_nsec - start.tv_nsec;
  delta.tv_sec = stop.tv_sec - start.tv_sec;
  while (delta.tv_sec > 0) {
    delta.tv_nsec += 1000000000;
    delta.tv_sec--;
  }
  return delta.tv_nsec;
}

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
               double *ns_per_op) {
  uint64_t nsec = time_delta_ns(start, stop);
  *op_per_s = *ns_per_op = 0;

  if (nsec <= 0 || operations <= 0) {
    return false;
  }

  *op_per_s = (double)1000000000 / (double)nsec * (double)operations;
  *ns_per_op = (double)nsec / (double)operations;

  return true;
}

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
              double *us_per_mb) {
  uint64_t nsec = time_delta_ns(start, stop);
  *us_per_mb = *mb_per_s = 0;

  if (nsec <= 0 || bytes <= 0) {
    return false;
  }

  *us_per_mb =
      (double)1024 * (double)1024 / (double)1000 * nsec / (double)bytes;
  *mb_per_s = (double)1000000 / *us_per_mb;

  return true;
}
