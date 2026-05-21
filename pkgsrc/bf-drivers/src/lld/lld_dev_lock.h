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

#ifndef LLD_DEV_LOCK_H_INCLUDED
#define LLD_DEV_LOCK_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t lld_dev_lock(bf_dev_id_t dev_id);
bf_status_t lld_dev_unlock(bf_dev_id_t dev_id);
bf_status_t lld_subdev_lock(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
bf_status_t lld_subdev_unlock(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
bool lld_dev_is_locked(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);

void lld_dev_lock_ind_reg_mutex_init(void);
void lld_dev_lock_ind_reg_lock(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
void lld_dev_lock_ind_reg_unlock(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // LLD_DEV_LOCK_H_INCLUDED
