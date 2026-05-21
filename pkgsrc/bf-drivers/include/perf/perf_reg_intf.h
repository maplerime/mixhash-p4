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
 * @file perf_reg_intf.h
 * @date
 *
 * Performance registers handling definitions.
 */

#ifndef _PERF_REG_INTF_H
#define _PERF_REG_INTF_H

extern struct test_description reg_indir_test;
extern struct test_description reg_dir_test;

enum registers_enum_res { RES_REG_BUS };
enum registers_int_res { RES_REG_IT };
enum registers_double_res {
  RES_REG_WRITE_NS,
  RES_REG_WRITE_OP,
  RES_REG_READ_NS,
  RES_REG_READ_OP
};

struct register_result {
  bool status;
  double write_ns;
  double write_op;
  double read_ns;
  double read_op;
};

/**
 * @brief Run performance test that will indirect write/read to/from regitser,
 * and calculate the rate.
 *
 * @param dev_id device id
 * @param bus_type bus type
 * @param it number of iterations
 * @return register_result
 */
struct test_results reg_indir(bf_dev_id_t dev_id,
                              perf_bus_t_enum bus_type,
                              int it);

/**
 * @brief Run performance test that will direct write/read to/from regitser,
 * and calculate the rate.
 *
 * @param dev_id device id
 * @param bus_type bus type
 * @param it number of iterations
 * @return register_result
 */
struct test_results reg_dir(bf_dev_id_t dev_id,
                            perf_bus_t_enum bus_type,
                            int it);

#endif
