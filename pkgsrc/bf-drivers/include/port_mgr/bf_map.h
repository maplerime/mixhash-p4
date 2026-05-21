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

#ifndef BF_MAP_H_INCLUDED
#define BF_MAP_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

bf_status_t bf_map_logical_umac3_to_physical(bf_dev_id_t dev_id,
                                             uint32_t logical_umac,
                                             uint32_t *physical_umac);
bf_status_t bf_map_logical_umac4_to_physical(bf_dev_id_t dev_id,
                                             uint32_t logical_umac,
                                             uint32_t *physical_umac);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // BF_PORT_IF_H_INCLUDED
