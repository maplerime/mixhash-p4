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

#include <string.h>
#include <dvm/bf_drv_intf.h>
#include "tm_ctx.h"
#include <target-sys/bf_sal/bf_sys_intf.h>
#include <lld/bf_dev_if.h>

#ifndef __TM_INIT_H__
#define __TM_INIT_H__

void bf_tm_destruct_tm_ctx(bf_tm_dev_ctx_t *tm_ctx);
bf_tm_status_t bf_tm_construct_modules(bf_tm_dev_ctx_t *tm_ctx);
bf_tm_status_t bf_tm_create_tm_ctx(bf_tm_asic_en asic_type,
                                   bf_dev_id_t devid,
                                   bf_tm_ctx_type_en ctx_type,
                                   bf_tm_dev_ctx_t **tm_ctx);
bf_tm_status_t bf_tm_init_new_device(bf_dev_id_t dev,
                                     bf_tm_asic_en asic_type,
                                     bf_dma_info_t *dma_info);
bf_status_t tm_add_device(bf_dev_id_t dev,
                          bf_dev_family_t dev_family,
                          bf_device_profile_t *profile,
                          bf_dma_info_t *dma_info,
                          bf_dev_init_mode_t warm_init_mode);
bf_status_t tm_remove_device(bf_dev_id_t dev);
bf_status_t tm_cleanup_device(bf_tm_dev_ctx_t *ctx);
bf_status_t tm_add_port(bf_dev_id_t dev,
                        bf_dev_port_t port,
                        bf_port_attributes_t *port_attrib,
                        bf_port_cb_direction_t direction);
bf_status_t tm_remove_port(bf_dev_id_t dev,
                           bf_dev_port_t port,
                           bf_port_cb_direction_t direction);
bf_status_t tm_update_port_status(bf_dev_id_t dev,
                                  bf_dev_port_t port,
                                  bool state);
bf_status_t tm_update_port_admin_state(bf_dev_id_t dev,
                                       bf_dev_port_t port,
                                       bool enable);
bf_status_t tm_disable_all_port_tx(bf_dev_id_t dev);

bf_status_t tm_lock_device(bf_dev_id_t dev_id);
bf_status_t tm_unlock_device(bf_dev_id_t dev_id);
bool tm_is_device_locked(bf_dev_id_t dev_id);
bf_status_t tm_chip_init_sequence_during_fast_reconfig(bf_dev_id_t dev_id);
bf_status_t tm_config_complete(bf_dev_id_t dev_id);
bf_status_t tm_push_delta_cfg_hitless_restart(bf_dev_id_t dev_id);
bf_tm_status_t bf_tm_restore_device_cfg(bf_dev_id_t dev);
bf_status_t tm_warm_init_quick(bf_dev_id_t dev);

/* This function is exported for UT purpose. External API for the same purpose
 * is bf_tm_restore_device_cfg()
 */
bf_tm_status_t bf_tm_restore_dev_cfg(bf_dev_id_t dev);

/* UT function */
bf_tm_status_t bf_tm_ut_restore_device_cfg(ucli_context_t *uc, bf_dev_id_t dev);

#endif
