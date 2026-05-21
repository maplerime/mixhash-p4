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

#ifndef lld_fault_h_included
#define lld_fault_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file lld_fault.h
 * \brief Fault-handling types
 *
 */

/**
 * @}
 */

extern void lld_fault_possible_unimplemented_reg(bf_dev_id_t dev_id,
                                                 uint32_t reg);
extern void lld_fault_possible_uncorrectable_ecc_u64(bf_dev_id_t dev_id,
                                                     uint64_t mem64b);
extern void lld_fault_uncorrectable_ecc(bf_dev_id_t dev_id, uint64_t mem64b);
extern void lld_fault_correctable_ecc(bf_dev_id_t dev_id, uint64_t mem64b);
extern void lld_fault_dma_error(bf_dev_id_t dev_id, uint64_t *completion_desc);
extern void lld_fault_sw_error(bf_dev_id_t dev_id, bf_sw_fault_e hint);
extern void lld_debug_bus_init(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // lld_fault_h_included
