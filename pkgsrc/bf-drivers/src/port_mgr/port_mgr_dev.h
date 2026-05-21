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

#ifndef port_mgr_dev_h_included
#define port_mgr_dev_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bool port_mgr_dev_is_family_tof1(bf_dev_family_t dev_family);
bool port_mgr_dev_is_tof1(bf_dev_id_t dev_id);
bool port_mgr_dev_is_tof2(bf_dev_id_t dev_id);
bool port_mgr_dev_is_tof3(bf_dev_id_t dev_id);
bf_status_t port_mgr_dev_add(bf_dev_id_t dev_id,
                             bf_dev_family_t dev_family,
                             bf_device_profile_t *profile,
                             struct bf_dma_info_s *dma_info,
                             bf_dev_init_mode_t warm_init_mode);
bf_status_t port_mgr_dev_remove(bf_dev_id_t dev_id);
int port_mgr_dev_is_ready(bf_dev_id_t dev_id);
void port_mgr_dev_enable_all_dr(bf_dev_id_t dev_id);
void port_mgr_dev_disable_all_dr(bf_dev_id_t dev_id);

bf_status_t port_mgr_dev_lock(bf_dev_id_t dev_id);
bf_status_t port_mgr_dev_unlock(bf_dev_id_t dev_id);
bool port_mgr_dev_is_locked(bf_dev_id_t dev_id);

// Logical device access fns
bool port_mgr_dev_ready_get(bf_dev_id_t dev_id);
void port_mgr_dev_ready_set(bf_dev_id_t dev_id, bool st);
bool port_mgr_dev_assigned_get(bf_dev_id_t dev_id);
void port_mgr_dev_assigned_set(bf_dev_id_t dev_id, bool st);
bf_dev_family_t port_mgr_dev_family_get(bf_dev_id_t dev_id);
void port_mgr_dev_family_set(bf_dev_id_t dev_id, bf_dev_family_t dev_family);
bf_sku_chip_part_rev_t port_mgr_dev_part_rev_get(bf_dev_id_t dev_id);
void port_mgr_dev_part_rev_set(bf_dev_id_t dev_id,
                               bf_sku_chip_part_rev_t part_rev);
struct bf_dma_info_s *port_mgr_dev_dma_info_get(bf_dev_id_t dev_id);
void port_mgr_dev_dma_info_set(bf_dev_id_t dev_id,
                               struct bf_dma_info_s *dma_info);
port_mgr_ha_stages_t port_mgr_dev_ha_stage_get(bf_dev_id_t dev_id);
void port_mgr_dev_ha_stage_set(bf_dev_id_t dev_id,
                               port_mgr_ha_stages_t ha_stage);
bf_dev_init_mode_t port_mgr_dev_init_mode_get(bf_dev_id_t dev_id);
void port_mgr_dev_init_mode_set(bf_dev_id_t dev_id,
                                bf_dev_init_mode_t init_mode);
void port_mgr_dev_port_cb_get(bf_dev_id_t dev_id,
                              port_mgr_port_callback_t *fn,
                              void **userdata);
void port_mgr_dev_port_cb_set(bf_dev_id_t dev_id,
                              port_mgr_port_callback_t fn,
                              void *userdata);
port_mgr_ldev_t *port_mgr_dev_logical_dev_get(bf_dev_id_t dev_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
