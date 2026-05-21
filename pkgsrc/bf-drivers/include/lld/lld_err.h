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

#ifndef lld_err_h
#define lld_err_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  LLD_OK = 0,
  LLD_ERR_BAD_PARM = -1,
  LLD_ERR_NOT_READY = -2,
  LLD_ERR_LOCK_FAILED = -3,
  LLD_ERR_DR_FULL = -4,
  LLD_ERR_DR_EMPTY = -5,
  LLD_ERR_INVALID_CFG = -6,
  LLD_ERR_UT = -7,
} lld_err_t;

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // lld_err_h
