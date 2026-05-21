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
 * @file perf_mem_intf.h
 * @date
 *
 * Performance memory handling definitions.
 */

#ifndef _PERF_MEM_INTF_H
#define _PERF_MEM_INTF_H

extern struct test_description sram_dma_test;
extern struct test_description tcam_dma_test;

enum mem_int_res { RES_PIPES, RES_MAUS, RES_ROWS, RES_COLS };

enum mem_double_res { RES_WRITE_MB, RES_WRITE_US, RES_READ_MB, RES_READ_US };

/**
 * @brief Run performance test that will write/read to/from SRAM memory,
 * and calculate the rate.
 *
 * @param dev_id device id
 * @param num_pipes Number of PIPEs
 * @param num_maus Number of MAUs
 * @param num_rows Number of ROWs
 * @param num_cols Number of COLUMNs
 * @return test_results
 */
struct test_results sram_dma(bf_dev_id_t dev_id,
                             int num_pipes,
                             int num_maus,
                             int num_rows,
                             int num_cols);

/**
 * @brief Run performance test that will write/read to/from TCAM memory,
 * and calculate the rate.
 *
 * @param dev_id device id
 * @param num_pipes Number of PIPEs
 * @param num_maus Number of MAUs
 * @param num_rows Number of ROWs
 * @param num_cols Number of COLUMNs
 * @return test_results
 */
struct test_results tcam_dma(bf_dev_id_t dev_id,
                             int num_pipes,
                             int num_maus,
                             int num_rows,
                             int num_cols);

#endif
