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

#ifndef LLD_DEV_H_INCLUDED
#define LLD_DEV_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

struct bf_dma_info_s;

bf_status_t lld_master_dev_add(bf_dev_id_t dev_id,
                               bf_dev_family_t dev_family,
                               struct bf_dma_info_s *dma_info,
                               bf_dev_init_mode_t warm_init_mode);

bf_status_t lld_dev_add(bf_dev_id_t dev_id,
                        bf_dev_family_t dev_family,
                        bf_device_profile_t *profile,
                        struct bf_dma_info_s *dma_info,
                        bf_dev_init_mode_t warm_init_mode);
bf_status_t lld_dev_remove(bf_dev_id_t dev_id);
bf_status_t lld_reset_core(bf_dev_id_t dev_id);
bool lld_dev_is_tofino(bf_dev_id_t dev_id);
bool lld_dev_is_tof2(bf_dev_id_t dev_id);
bool lld_dev_is_tof3(bf_dev_id_t dev_id);

bool lld_dev_ready(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
bf_dev_type_t lld_dev_type_get(bf_dev_id_t dev_id);
bf_dev_family_t lld_dev_family_get(bf_dev_id_t dev_id);

bf_status_t lld_dev_lock(bf_dev_id_t dev_id);
bf_status_t lld_dev_unlock(bf_dev_id_t dev_id);
bf_status_t lld_subdev_lock(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
bf_status_t lld_subdev_unlock(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
bool lld_dev_is_locked(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
bf_status_t lld_warm_init_quick(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // LLD_DEV_H_INCLUDED
