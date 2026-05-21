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

#ifndef port_mgr_tof1_dev_h_included
#define port_mgr_tof1_dev_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t port_mgr_tof1_dev_add(bf_dev_id_t dev_id,
                                  bf_dev_family_t dev_family,
                                  bf_device_profile_t *profile,
                                  struct bf_dma_info_s *dma_info,
                                  bf_dev_init_mode_t warm_init_mode);
bf_status_t port_mgr_tof1_dev_remove(bf_dev_id_t dev_id);
bf_status_t port_mgr_tof1_dev_register_port_cb(bf_dev_id_t dev_id,
                                               port_mgr_port_callback_t fn,
                                               void *userdata);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
