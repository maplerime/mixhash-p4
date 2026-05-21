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

#ifndef port_mgr_tof2_microp_h_included
#define port_mgr_tof2_microp_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void port_mgr_tof2_microp_init(bf_dev_id_t dev_id,
                               bf_device_profile_t *profile);
bf_status_t port_mgr_tof2_microp_rd(bf_dev_id_t dev_id,
                                    uint32_t offset,
                                    uint32_t *r_data,
                                    uint32_t microp_id);
bf_status_t port_mgr_tof2_microp_wr(bf_dev_id_t dev_id,
                                    uint32_t offset,
                                    uint32_t w_data,
                                    uint32_t microp_id);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
