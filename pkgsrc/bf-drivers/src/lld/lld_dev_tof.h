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

#ifndef LLD_DEV_TOF_H_INCLUDED
#define LLD_DEV_TOF_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t lld_dev_tof_set_core_clk(bf_dev_id_t dev_id,
                                     bool force_clk,
                                     uint32_t force_clk_val);
bf_status_t lld_dev_tof_un_reset(bf_dev_id_t dev_id);
bf_status_t lld_dev_tof_reset_core(bf_dev_id_t dev_id);
bf_status_t lld_dev_tof_tcu_seq(bf_dev_id_t dev_id);
bf_status_t lld_dev_tof_change_core_clk(bf_dev_id_t dev_id, uint32_t clk_val);
bf_status_t lld_dev_tof_get_core_clk(bf_dev_id_t dev_id, uint32_t *clk_val);
bf_status_t lld_dev_tof_tlp_poison_set(bf_dev_id_t dev_id, bool en);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // LLD_DEV_TOF_H_INCLUDED
