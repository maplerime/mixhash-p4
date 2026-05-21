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

#include <memory>
// local includes
#include "bf_rt_utils.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <bf_types/bf_types.h>
#include <target-sys/bf_sal/bf_sys_mem.h>

void bf_rt_err_str(bf_status_t sts, const char **err_str) {
  *err_str = bf_err_str(sts);
}

#ifdef __cplusplus
}
#endif
