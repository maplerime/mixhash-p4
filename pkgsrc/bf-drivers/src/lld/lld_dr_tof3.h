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

#ifndef lld_dr_tof3_h
#define lld_dr_tof3_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void lld_dr_tof3_dma_enable_set(bf_dev_id_t dev_id,
                                bf_subdev_id_t subdev_id,
                                bool en_dma);
void lld_dr_tof3_pbus_arb_ctrl_set(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   pbus_arb_ctrl_t *pbus_arb_ctrl_val);
void lld_dr_tof3_cbus_arb_ctrl_set(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   uint32_t cbus_arb_ctrl_val);
void lld_dr_tof3_enable_set(bf_dev_id_t dev_id,
                            bf_subdev_id_t subdev_id,
                            bf_dma_dr_id_t dr_id,
                            bool en);
bf_status_t lld_dr_tof3_write_time_mode_set(bf_dev_id_t dev_id,
                                            bf_subdev_id_t subdev_id,
                                            bf_dma_dr_id_t dr_id,
                                            bool en);
bf_status_t lld_dr_tof3_pushed_ptr_mode_set(bf_dev_id_t dev_id,
                                            bf_subdev_id_t subdev_id,
                                            bf_dma_dr_id_t dr_id,
                                            bool en);
void lld_dr_tof3_ring_timeout_set(bf_dev_id_t dev_id,
                                  bf_subdev_id_t subdev_id,
                                  bf_dma_dr_id_t dr_id,
                                  uint16_t timeout);
void lld_dr_tof3_data_timeout_set(bf_dev_id_t dev_id,
                                  bf_subdev_id_t subdev_id,
                                  bf_dma_dr_id_t dr_id,
                                  uint32_t timeout);
bf_status_t lld_dr_tof3_data_timeout_get(bf_dev_id_t dev_id,
                                         bf_subdev_id_t subdev_id,
                                         bf_dma_dr_id_t dr_id,
                                         uint32_t *timeout);
void lld_dr_tof3_flush_all(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
uint32_t lld_dr_tof3_wait_for_flush_done(bf_dev_id_t dev_id,
                                         bf_subdev_id_t subdev_id,
                                         uint32_t max_tries);
void lld_dr_tof3_clear_link_down(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id);
lld_dr_bus_t lld_dr_tof3_dr_to_host_bus(bf_dma_dr_id_t dr_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
