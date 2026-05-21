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

#ifndef LLD_INTERRUPT_INCLUDED
#define LLD_INTERRUPT_INCLUDED

#include <lld/lld_int_cb.h>

typedef bf_int_cb lld_int_cb;
typedef bf_status_t (*lld_blk_int_traverse_cb)(bf_dev_id_t dev_id,
                                               bf_subdev_id_t subdev_id,
                                               void *blk_lvl_int);

#define LLD_MAX_INT_NBR (511 /* For both Tofino and Tof2, 0-511*/)
#define LLD_TOF_TOF2_TOF3_SHADOW_REG_NUMB \
  (16) /* For both Tofino and Tof2, 0-15 */

#endif  // LLD_INTERRUPT_INCUDED
