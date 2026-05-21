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

#ifndef LLD_TOF_INTERRUPT_INCLUDED
#define LLD_TOF_INTERRUPT_INCLUDED

#include <lld/lld_interrupt.h>

void lld_tof_int_svc(bf_dev_id_t dev_id,
                     uint32_t sh_int_val,
                     uint16_t sh_int_reg);
void lld_tof_int_gbl_en_set(bf_dev_id_t dev_id, bool en);
void lld_tof_int_msk(bf_dev_id_t dev_id,
                     uint32_t sh_int_reg,
                     uint32_t sh_int_bit);
void lld_tof_int_ena(bf_dev_id_t dev_id,
                     uint32_t sh_int_reg,
                     uint32_t sh_int_bit);

int lld_tof_int_register_cb(bf_dev_id_t dev_id,
                            uint32_t offset,
                            lld_int_cb cb_fn,
                            void *userdata);
// void lld_tof_get_cbus_cbc_status_offset(uint32_t *offset_p, void *userdata);
lld_int_cb lld_tof_get_int_cb(bf_dev_id_t dev_id,
                              uint32_t offset,
                              void **userdata);
bf_status_t lld_tof_int_poll(bf_dev_id_t dev_id, bool all_ints);
uint32_t lld_tof_int_get_glb_status(bf_dev_id_t dev_id);
uint32_t lld_tof_int_get_shadow_int_status(bf_dev_id_t dev_id,
                                           uint16_t sh_int_reg);
uint32_t lld_tof_int_get_shadow_msk_status(bf_dev_id_t dev_id,
                                           uint16_t sh_msk_reg);
void lld_tof_int_set_shadow_msk_status(bf_dev_id_t dev_id,
                                       uint16_t sh_msk_reg,
                                       uint32_t value);
bf_status_t lld_tof_int_traverse_blk_lvl_int(bf_dev_id_t dev_id,
                                             lld_blk_int_traverse_cb cb_fn);
void lld_tof_int_host_leaf_enable_set(bf_dev_id_t dev_id, bool en);
bf_status_t lld_tof_int_disable_all_cb(bf_dev_id_t dev_id,
                                       bf_subdev_id_t subdev_id,
                                       void *blk_lvl_int);
bf_status_t lld_tof_clear_all_ints_cb(bf_dev_id_t dev_id,
                                      bf_subdev_id_t subdev_id,
                                      void *blk_lvl_int_vd);

bf_status_t lld_tof_clear_ints_cb(bf_dev_id_t dev_id,
                                  bf_subdev_id_t subdev_id,
                                  void *blk_lvl_int_vd);
bf_status_t lld_tof_enable_all_pipe_ints_cb(bf_dev_id_t dev_id,
                                            bf_subdev_id_t subdev_id,
                                            void *blk_lvl_int_vd);
bf_status_t lld_tof_inject_all_ints_cb(bf_dev_id_t dev_id,
                                       bf_subdev_id_t subdev_id,
                                       void *blk_lvl_int_vd);
bf_status_t lld_tof_inject_ints_with_offset(bf_dev_id_t dev_id,
                                            uint32_t offset);
void lld_tof_int_mbus_leaf_enable_set(bf_dev_id_t dev_id, bool en);
bf_status_t lld_tof_dump_int_list_cb(bf_dev_id_t dev_id,
                                     bf_subdev_id_t subdev_id,
                                     void *blk_lvl_int_vd);
bf_status_t lld_tof_dump_new_ints_cb(bf_dev_id_t dev_id,
                                     bf_subdev_id_t subdev_id,
                                     void *blk_lvl_int_vd);
bf_status_t lld_tof_clear_new_ints_cb(bf_dev_id_t dev_id,
                                      bf_subdev_id_t subdev_id,
                                      void *blk_lvl_int_vd);
bf_status_t lld_tof_dump_all_ints_cb(bf_dev_id_t dev_id,
                                     bf_subdev_id_t subdev_id,
                                     void *blk_lvl_int_vd);
bf_status_t lld_tof_dump_unfired_ints_cb(bf_dev_id_t dev_id,
                                         bf_subdev_id_t subdev_id,
                                         void *blk_lvl_int_vd);
uint32_t lld_tof_get_status_reg(void *blk_lvl_int);
#endif  // LLD_TOF_INTERRUPT_INCLUDED
