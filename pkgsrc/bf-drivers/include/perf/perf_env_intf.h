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

/*!
 * @file perf_env_intf.h
 * @date
 *
 * Environment handling definitions.
 */

#ifndef _PERF_ENV_INTF_H
#define _PERF_ENV_INTF_H

enum env_enum_param {
  ENV_FAMILY,
  ENV_TYPE,
  ENV_SKU,
  ENV_SYSLIBS,
  ENV_SYSLIBS_INT,
  ENV_UTILS,
  ENV_UTILS_INT,
  ENV_DRIVERS,
  ENV_DRIVERS_INT,
  ENV_PARAM_N_MAX
};

struct env_param {
  char *name;
  char value[ENV_PARAM_VALUE_L_MAX];
};

struct env_description {
  bool status;
  int num_params;
  struct env_param param[ENV_PARAM_N_MAX];
};

/**
 * @brief Get environment parameters.
 *
 * @param dev_id device id
 * @return env_description
 */
struct env_description environment(bf_dev_id_t dev_id);

#endif
