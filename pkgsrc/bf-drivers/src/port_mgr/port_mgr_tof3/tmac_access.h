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

#ifndef TMAC_ACCESS_H
#define TMAC_ACCESS_H

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void port_mgr_csr_tmac_access_rd32(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t tmac,
                                   uint32_t offset,
                                   uint32_t *r_data,
                                   const char *fn);

void port_mgr_csr_tmac_access_wr32(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t tmac,
                                   uint32_t offset,
                                   uint32_t w_data,
                                   const char *fn);

void port_mgr_tmac_access_rd32(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t offset,
                               uint32_t *r_data,
                               const char *fn);

void port_mgr_tmac_access_wr32(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t offset,
                               uint32_t w_data,
                               const char *fn);

void port_mgr_tmac_access_rd64(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t offset,
                               uint64_t *reg64,
                               const char *fn);

void port_mgr_tmac_access_wr64(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint32_t tmac,
                               uint32_t offset,
                               uint64_t reg64,
                               const char *fn);
uint32_t port_mgr_tmac_address_get(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t tmac,
                                   uint32_t offset);

#ifdef __cplusplus
}
#endif /* C++ */

#endif /* !TMAC_ACCESS_H */
