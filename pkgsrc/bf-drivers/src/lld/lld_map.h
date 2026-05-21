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

#ifndef lld_map_h
#define lld_map_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

lld_dev_t *lld_map_dev_id_to_dev_p_allow_unassigned(bf_dev_id_t dev_id);
lld_dev_t *lld_map_dev_id_to_dev_p(bf_dev_id_t dev_id);
lld_dr_view_t *lld_map_dev_id_and_dr_to_view(bf_dev_id_t asic,
                                             bf_dma_dr_id_t dr);
lld_dr_view_t *lld_map_dev_id_and_dr_to_view_allow_unassigned(
    bf_dev_id_t dev_id, bf_dma_dr_id_t dr);

lld_dev_t *lld_map_subdev_id_to_dev_p_allow_unassigned(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
lld_dev_t *lld_map_subdev_id_to_dev_p(bf_dev_id_t dev_id,
                                      bf_subdev_id_t subdev_id);
lld_dr_view_t *lld_map_subdev_id_and_dr_to_view(bf_dev_id_t asic,
                                                bf_subdev_id_t subdev_id,
                                                bf_dma_dr_id_t dr);
lld_dr_view_t *lld_map_subdev_id_and_dr_to_view_allow_unassigned(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, bf_dma_dr_id_t dr);

static inline bf_subdev_id_t lld_map_subdev_id_get(lld_dev_t *dev_p) {
  if (!dev_p) {
    return 0;
  } else {
    return dev_p->subdev_id;
  }
}

#ifdef __cplusplus
}
#endif /* C++ */

#endif
