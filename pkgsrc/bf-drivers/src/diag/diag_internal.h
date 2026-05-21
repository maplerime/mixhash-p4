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

#ifndef DIAG_INTERNAL_H_INCLUDED
#define DIAG_INTERNAL_H_INCLUDED

#define DEVDIAG_NUM_DEVICES BF_MAX_DEV_COUNT
#define DEVDIAG_NUM_SUBDEVICES BF_MAX_SUBDEV_COUNT
#define diag_u64_to_void_ptr(u64) ((void *)((uintptr_t)u64))
#define diag_ptr_to_u64(ptr) ((uintptr_t)ptr)

#endif  // DIAG_INTERNAL_H_INCLUDED
