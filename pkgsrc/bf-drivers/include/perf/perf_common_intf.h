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
 * @file perf_common_intf.h
 * @date
 *
 * Performance common definitions.
 */

#ifndef _PERF_COMMON_INTF_H
#define _PERF_COMMON_INTF_H

#define PARAMS_N_MAX 10
#define RESULTS_N_MAX 20
#define METRICS_N_MAX 20

#define ENV_PARAM_VALUE_L_MAX 50

extern char *bus_type_name[];

typedef enum bus_type_t {
  PBUS,
  MBUS,
  CBUS,
  HOSTIF,
  PERF_INT_BUS_T_MAX,
} perf_bus_t_enum;

struct param_set {
  char *name;
  char *type;
  char *defaults;
};

struct result_set {
  char *header;
  char *unit;
  char *type;
};

struct test_description {
  char *test_name;
  char *description;
  struct param_set params[PARAMS_N_MAX];
  struct result_set results[RESULTS_N_MAX];
};

struct enum_description {
  char *enum_name;
  char *enum_desc;
};

struct test_results {
  bool status;
  int res_enum[METRICS_N_MAX];
  int res_int[METRICS_N_MAX];
  double res_double[METRICS_N_MAX];
};

struct test_description **list_tests();

int get_params_n_max();

int get_results_n_max();

int get_metrics_n_max();

int get_env_param_n_max();

int get_env_param_value_l_max();

#endif
