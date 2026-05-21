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

#ifndef lld_int_cb_h
#define lld_int_cb_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*lld_mac_int_dump_this_cb)(bf_dev_id_t dev_id,
                                         int mac_block,
                                         int ch,
                                         uint32_t int_reg,
                                         int bit,
                                         uint32_t total,
                                         uint32_t shown);

typedef void (*lld_mac_int_dump_cb)(lld_mac_int_dump_this_cb fn,
                                    bf_dev_id_t dev_id);
typedef void (*lld_mac_int_poll_cb)(bf_dev_id_t dev_id, int mac_block, int ch);
typedef void (*lld_mac_int_bh_wakeup_cb)(bf_dev_id_t dev_id);

#endif  // lld_int_cb_h
