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

#ifndef LLD_INTERRUPT_IF_INCLUDED
#define LLD_INTERRUPT_IF_INCLUDED
#include <lld/lld_interrupt.h>

void lld_int_gbl_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, bool en);

bf_status_t lld_int_claim(bf_dev_id_t dev_id,
                          bf_subdev_id_t subdev_id,
                          bf_int_nbr_t int_nbr);
void lld_int_svc(bf_dev_id_t dev_id,
                 bf_subdev_id_t subdev_id,
                 uint32_t sh_int_val,
                 uint16_t sh_int_reg);
bf_status_t lld_int_msk(bf_dev_id_t dev_id,
                        bf_subdev_id_t subdev_id,
                        bf_int_nbr_t int_nbr);
bf_status_t lld_int_ena(bf_dev_id_t dev_id,
                        bf_subdev_id_t subdev_id,
                        bf_int_nbr_t int_nbr);
int lld_int_register_cb(bf_dev_id_t dev_id,
                        bf_subdev_id_t subdev_id,
                        uint32_t offset,
                        lld_int_cb cb_fn,
                        void *userdata);
lld_int_cb lld_get_int_cb(bf_dev_id_t dev_id,
                          bf_subdev_id_t subdev_id,
                          uint32_t offset,
                          void **userdata);
int lld_register_mac_int_poll_cb(lld_mac_int_poll_cb fn);
int lld_register_mac_int_dump_cb(lld_mac_int_dump_cb fn);
int lld_register_mac_int_bh_wakeup_cb(lld_mac_int_bh_wakeup_cb fn);
bf_status_t lld_int_poll(bf_dev_id_t dev_id,
                         bf_subdev_id_t subdev_id,
                         bool all_ints);
uint32_t lld_int_get_glb_status(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
uint32_t lld_int_get_shadow_int_status(bf_dev_id_t dev_id,
                                       bf_subdev_id_t subdev_id,
                                       uint16_t sh_int_reg);
uint32_t lld_int_get_shadow_msk_status(bf_dev_id_t dev_id,
                                       bf_subdev_id_t subdev_id,
                                       uint16_t sh_msk_reg);
void lld_int_set_shadow_msk_status(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint16_t sh_msk_reg,
                                   uint32_t value);
uint32_t lld_map_int_nbr_to_sh_int_reg(bf_int_nbr_t int_nbr);
uint32_t lld_map_int_nbr_to_sh_int_bit(bf_int_nbr_t int_nbr);
bf_status_t lld_int_disable_all(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);

void lld_int_host_leaf_enable_set(bf_dev_id_t dev_id,
                                  bf_subdev_id_t subdev_id,
                                  bool en);
void lld_int_mbus_leaf_enable_set(bf_dev_id_t dev_id,
                                  bf_subdev_id_t subdev_id,
                                  bool en);
bf_status_t lld_int_msix_map_set(bf_dev_id_t dev_id,
                                 bf_subdev_id_t subdev_id,
                                 bf_int_nbr_t int_nbr,
                                 int msix_num);

#endif  // LLD_INTERRUPT_IF_INCUDED
