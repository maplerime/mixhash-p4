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

#ifndef LLD_DR_REGS_H
#define LLD_DR_REGS_H

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct dru_addr_s {
  bf_dma_dr_id_t dr;
  uint32_t dru_offset;
} dru_addr_t;

uint32_t lld_dr_base_get(bf_dev_id_t dev_id, bf_dma_dr_id_t dr);
bf_dma_dr_id_t lld_dr_id_get(bf_dev_id_t dev_id, uint64_t address);
int lld_dr_is_dru_reg(bf_dev_id_t dev_id, uint64_t address);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
