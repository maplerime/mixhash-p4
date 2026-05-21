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

#ifndef _TM_UCLI_APIS_H_
#define _TM_UCLI_APIS_H_

#include <bf_types/bf_types.h>
#include "traffic_mgr/common/tm_ctx.h"
#include "traffic_mgr/common/tm_error.h"

/**
 * @brief Get ppg icos mask from hardware.
 * For internal using
 * @param[in] dev         ASIC device identifier.
 * @param[in] ppg         ppg whose icos mask has to be fetched.
 * @param[out] icos_mask  icos mask
 * @return                Status of API call.
 *  BF_SUCCESS on success
 *  Non-Zero on error
 */
bf_status_t bf_tm_ppg_icos_mask_get(bf_dev_id_t dev,
                                    bf_tm_ppg_hdl ppg,
                                    uint8_t *icos_mask);
#endif  // _TM_UCLI_APIS_H_
