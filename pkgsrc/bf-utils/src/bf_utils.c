/*******************************************************************************
 * INTEL CONFIDENTIAL
 *
 * Copyright (c) 2021 Intel Corporation
 * All Rights Reserved.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 ******************************************************************************/
#ifndef BF_UTILS_BLD_VER
#define BF_UTILS_BLD_VER "0"
#endif

#ifndef BF_UTILS_GIT_VER
#define BF_UTILS_GIT_VER "000"
#endif

#define BF_UTILS_REL_VER "9.13.3"
#define BF_UTILS_VER BF_UTILS_REL_VER "-" BF_UTILS_BLD_VER

#define BF_UTILS_INTERNAL_VER BF_UTILS_VER "(" BF_UTILS_GIT_VER ")"

const char *bf_utils_get_version(void) { return BF_UTILS_VER; }
const char *bf_utils_get_internal_version(void) {
  return BF_UTILS_INTERNAL_VER;
}
