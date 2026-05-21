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

#ifndef lld_dr_tof2_h
#define lld_dr_tof2_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void lld_dr_tof2_dma_enable_set(bf_dev_id_t dev_id, bool en_dma);
void lld_dr_tof2_pbus_arb_ctrl_set(bf_dev_id_t dev_id,
                                   pbus_arb_ctrl_t *pbus_arb_ctrl);
void lld_dr_tof2_cbus_arb_ctrl_set(bf_dev_id_t dev_id,
                                   uint32_t cbus_arb_ctrl_val);
void lld_dr_tof2_enable_set(bf_dev_id_t dev_id, bf_dma_dr_id_t dr_id, bool en);
bf_status_t lld_dr_tof2_write_time_mode_set(bf_dev_id_t dev_id,
                                            bf_dma_dr_id_t dr_id,
                                            bool en);
bf_status_t lld_dr_tof2_pushed_ptr_mode_set(bf_dev_id_t dev_id,
                                            bf_dma_dr_id_t dr_id,
                                            bool en);
void lld_dr_tof2_ring_timeout_set(bf_dev_id_t dev_id,
                                  bf_dma_dr_id_t dr_id,
                                  uint16_t timeout);
void lld_dr_tof2_data_timeout_set(bf_dev_id_t dev_id,
                                  bf_dma_dr_id_t dr_id,
                                  uint32_t timeout);
bf_status_t lld_dr_tof2_data_timeout_get(bf_dev_id_t dev_id,
                                         bf_dma_dr_id_t dr_id,
                                         uint32_t *timeout);
void lld_dr_tof2_flush_all(bf_dev_id_t dev_id);
uint32_t lld_dr_tof2_wait_for_flush_done(bf_dev_id_t dev_id,
                                         uint32_t max_tries);
void lld_dr_tof2_clear_link_down(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
