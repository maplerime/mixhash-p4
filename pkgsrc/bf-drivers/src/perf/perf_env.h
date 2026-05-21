/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2022 Intel Corporation
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

/*!
 * @file perf_env.h
 * @date
 *
 * Performance environment handling common definitions.
 */

#ifndef _PERF_ENV_H
#define _PERF_ENV_H

/**
 * @brief Get environment parameters.
 *
 * @param dev_id device id
 * @return env_description
 */
struct env_description environment(bf_dev_id_t dev_id);

#endif
