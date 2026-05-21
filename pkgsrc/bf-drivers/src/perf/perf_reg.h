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
 * @file perf_registers.h
 * @date
 *
 * Performance registers handling common definitions.
 */

#ifndef _PERF_REGISTERS_H
#define _PERF_REGISTERS_H

#define PERF_REG_ITERS 100000

typedef struct reg_entry {
  uint64_t addr;
  char name[10];
} reg_entry_t;

/**
 * @brief Run performance test that will write/read to/from regitser,
 * and calculate the rate.
 *
 * @param dev_id device id
 * @param bus_type bus type
 * @param indirect whether addresses for direct or indirect registers should be
 * generated. If true - indirect. If false - direct.
 * @return register_result
 */
bf_status_t run_reg_test(bf_dev_id_t dev_id,
                         perf_bus_t_enum bus_type,
                         int it,
                         bool indirect,
                         struct register_result *result);
/**
 * @brief Run performance test that will iterate over indirect/direct registers,
 * iterate read/write operations and calculate the rate
 *
 * @param uc ucli context pointer
 * @param dev_id device id
 * @param indirect indirect access
 * @return ucli_status_t
 */
ucli_status_t run_registers(ucli_context_t *uc,
                            bf_dev_id_t dev_id,
                            bool indirect);

#endif
