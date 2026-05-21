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

#include "lld.h"
#include "lld_map.h"
#include <lld/lld_sku.h>

/*************************************************
 * lld_map_dev_id_to_dev_p
 *
 * Map chip # to the corresponding lld_dev_t
 * structure in the LLD context. Returns NULL
 * if "chip" exceeds SDK configured max.
 *************************************************/
lld_dev_t *lld_map_dev_id_to_dev_p(bf_dev_id_t dev_id) {
  if (dev_id < 0) return NULL;
  if (dev_id >= BF_MAX_DEV_COUNT) return NULL;
  if (!lld_ctx->asic[dev_id][0].assigned) return NULL;

  return (&lld_ctx->asic[dev_id][0]);
}

/*************************************************
 * lld_map_subdev_id_to_dev_p
 *
 * Map chip # and subdevice # to the corresponding lld_dev_t
 * structure in the LLD context. Returns NULL
 * if "chip" exceeds SDK configured max.
 *************************************************/
lld_dev_t *lld_map_subdev_id_to_dev_p(bf_dev_id_t dev_id,
                                      bf_subdev_id_t subdev_id) {
  if (dev_id < 0) return NULL;
  if (dev_id >= BF_MAX_DEV_COUNT || subdev_id >= BF_MAX_SUBDEV_COUNT)
    return NULL;
  if (!lld_ctx->asic[dev_id][subdev_id].assigned) return NULL;

  return (&lld_ctx->asic[dev_id][subdev_id]);
}

/*************************************************
 * lld_map_dev_id_to_dev_p_allow_unassigned
 *
 * Map dev_id # to the corresponding lld_dev_t
 * structure in the LLD context. Returns NULL
 * if "chip" exceeds SDK configured max.
 *************************************************/
lld_dev_t *lld_map_dev_id_to_dev_p_allow_unassigned(bf_dev_id_t dev_id) {
  if (dev_id < 0) return NULL;
  if (dev_id >= BF_MAX_DEV_COUNT) return NULL;

  return (&lld_ctx->asic[dev_id][0]);
}

/*************************************************
 * lld_map_subdev_id_to_dev_p_allow_unassigned
 *
 * Map dev_id # and subdevice # to the corresponding lld_dev_t
 * structure in the LLD context. Returns NULL
 * if "chip" exceeds SDK configured max.
 *************************************************/
lld_dev_t *lld_map_subdev_id_to_dev_p_allow_unassigned(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id) {
  if (dev_id < 0 || subdev_id < 0) return NULL;
  if (dev_id >= BF_MAX_DEV_COUNT || subdev_id >= BF_MAX_SUBDEV_COUNT)
    return NULL;

  return (&lld_ctx->asic[dev_id][subdev_id]);
}

static lld_dr_view_t *map_dr_to_view_helper(lld_dev_t *dev_p,
                                            bf_dma_dr_id_t dr) {
  bf_dma_dr_id_t max_dr = 0;

  if (dev_p == NULL) return NULL;
  switch (dev_p->dev_family) {
    case BF_DEV_FAMILY_TOFINO:
      max_dr = BF_DMA_MAX_TOF_DR;
      break;
    case BF_DEV_FAMILY_TOFINO2:
      max_dr = BF_DMA_MAX_TOF2_DR;
      break;
    case BF_DEV_FAMILY_TOFINO3:
      max_dr = BF_DMA_MAX_TOF3_DR;
      break;



    case BF_DEV_FAMILY_UNKNOWN:
      break;
  }
  if (dr > max_dr) return NULL;
  return &dev_p->dr_view[dr];
}
/*************************************************
 * lld_map_dev_id_and_dr_to_view
 *
 * Map dev_id # and DR to a DR view struct
 *************************************************/
lld_dr_view_t *lld_map_dev_id_and_dr_to_view(bf_dev_id_t dev_id,
                                             bf_dma_dr_id_t dr) {
  lld_dev_t *dev_p = lld_map_dev_id_to_dev_p(dev_id);
  return map_dr_to_view_helper(dev_p, dr);
}

/*************************************************
 * lld_map_subdev_id_and_dr_to_view
 *
 * Map dev_id # and DR to a DR view struct
 *************************************************/
lld_dr_view_t *lld_map_subdev_id_and_dr_to_view(bf_dev_id_t dev_id,
                                                bf_subdev_id_t subdev_id,
                                                bf_dma_dr_id_t dr) {
  lld_dev_t *dev_p = lld_map_subdev_id_to_dev_p(dev_id, subdev_id);
  return map_dr_to_view_helper(dev_p, dr);
}

/*************************************************
 * lld_map_dev_id_and_dr_to_view_allow_unassigned
 *
 * Map dev_id # and DR to a DR view struct
 *************************************************/
lld_dr_view_t *lld_map_dev_id_and_dr_to_view_allow_unassigned(
    bf_dev_id_t dev_id, bf_dma_dr_id_t dr) {
  lld_dev_t *dev_p = lld_map_dev_id_to_dev_p_allow_unassigned(dev_id);
  return map_dr_to_view_helper(dev_p, dr);
}

/*************************************************
 * lld_map_subdev_id_and_dr_to_view_allow_unassigned
 *
 * Map dev_id # and DR to a DR view struct
 *************************************************/
lld_dr_view_t *lld_map_subdev_id_and_dr_to_view_allow_unassigned(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, bf_dma_dr_id_t dr) {
  lld_dev_t *dev_p =
      lld_map_subdev_id_to_dev_p_allow_unassigned(dev_id, subdev_id);
  return map_dr_to_view_helper(dev_p, dr);
}
