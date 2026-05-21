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

#ifndef LLD_TOF3_EOS_H_INCLUDED
#define LLD_TOF3_EOS_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void lld_tof3_non_tm_eos_audit(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t phys_pipe);
void lld_tof3_eos_mau_audit(bf_dev_id_t dev_id,
                            bf_subdev_id_t subdev_id,
                            uint32_t phys_pipe,
                            uint32_t stage);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // LLD_TOF3_EOS_H_INCLUDED
