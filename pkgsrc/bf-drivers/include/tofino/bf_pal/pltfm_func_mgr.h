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

#ifndef _TOFINO_BF_PAL_PLATFORM_FUNC_MGR_H
#define _TOFINO_BF_PAL_PLATFORM_FUNC_MGR_H

#include <bf_types/bf_types.h>

typedef bf_status_t (*bf_pal_pltfm_type_get_fn)(bf_dev_id_t dev_id,
                                                bool *is_sw_model);

typedef struct pltfm_func_s {
  bf_pal_pltfm_type_get_fn pltfm_type_get; /* Device type (model or asic) */
} pltfm_func_s;

/**
 * @brief Get pointer to the function to get the device type (model or asic)
 * @param pltfm_type_get_fn Pointer to the function to get the device type
 * @return Status of the API call
 */
bf_status_t bf_pal_pltfm_type_get_fn_get(
    bf_pal_pltfm_type_get_fn *pltfm_type_get_fn);

/**
 * @brief Register all appropriate platform related functions with the
 * corresponding function pointers encapsulated in a structure
 * @param all_func Pointer to the structure containing all the function pointers
 * @return Status of the API call
 */
bf_status_t bf_pal_pltfm_fn_reg(pltfm_func_s *all_func);

/**
 * @brief Get all the platform function pointers in a structure populated with
 * the registered function pointers
 * @param all_func Pointer to the structure containing all the function pointers
 * @return Status of the API call
 */
bf_status_t bf_pal_pltfm_fn_get(pltfm_func_s *all_func);

#endif
