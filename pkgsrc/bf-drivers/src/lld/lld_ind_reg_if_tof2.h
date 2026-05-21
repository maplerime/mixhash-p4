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

#ifndef LLD_IND_REG_IF_TOF2_H_INCLUDED
#define LLD_IND_REG_IF_TOF2_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

lld_err_t lld_ind_write_tof2(bf_dev_id_t dev_id,
                             uint64_t ind_addr,
                             uint64_t data_hi,
                             uint64_t data_lo);

lld_err_t lld_ind_read_tof2(bf_dev_id_t dev_id,
                            uint64_t ind_addr,
                            uint64_t *data_hi,
                            uint64_t *data_lo);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // LLD_IND_REG_IF_TOF2_H_INCLUDED
