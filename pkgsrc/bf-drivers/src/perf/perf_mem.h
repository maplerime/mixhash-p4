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
 * @file perf_mem.h
 * @date
 *
 * Performance memory handling definitions.
 */

#ifndef _PERF_MEM_H
#define _PERF_MEM_H

/**
 * @brief Run performance test that will write/read to/from
 * SRAM/TCAM memory, calculate the rate
 *
 * @param dev_id Device id
 * @param mem_type Memory type
 * @param pipes Number of PIPEs
 * @param maus Number of MAUs
 * @param rows Number of ROWs
 * @param cols Number of COLUMNs
 * @param result pointer to struct with results
 * @return ucli_status_t
 */
bf_status_t run_mem_test(bf_dev_id_t dev_id,
                         pipe_mem_type_t mem_type,
                         int pipes,
                         int maus,
                         int rows,
                         int cols,
                         struct test_results *result);

/**
 * @brief Run performance test that will write/read to/from SRAM memory,
 * and calculate the rate.
 *
 * @param uc ucli context pointer
 * @param dev_id device id
 * @return ucli_status_t
 */
ucli_status_t run_sram_dma(ucli_context_t *uc, bf_dev_id_t dev_id);

/**
 * @brief Run performance test that will write/read to/from TCAM memory,
 * and calculate the rate.
 *
 * @param uc ucli context pointer
 * @param dev_id device id
 * @return ucli_status_t
 */
ucli_status_t run_tcam_dma(ucli_context_t *uc, bf_dev_id_t dev_id);

#endif
