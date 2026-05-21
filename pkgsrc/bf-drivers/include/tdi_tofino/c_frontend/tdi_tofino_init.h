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

/** @file tdi_tofino_init.h
 *
 *  @brief C frontend for tofino specific init
 */
#ifndef _TDI_TOFINO_INIT_H_
#define _TDI_TOFINO_INIT_H_

// tdi includes
#include <tdi/common/tdi_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

tdi_status_t tdi_module_init(void *target_options);

#ifdef __cplusplus
}
#endif

#endif  // _TDI_TOFINO_INIT_H_
