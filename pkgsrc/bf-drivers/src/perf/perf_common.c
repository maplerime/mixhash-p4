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

#include <errno.h>
#include <math.h>

#include <target-utils/uCli/ucli.h>
#include <bfutils/bf_utils.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_dev.h>
#include <lld/lld_sku.h>
#include <pipe_mgr/pipe_mgr_drv.h>

#include <perf/perf_common_intf.h>
#include <perf/perf_env_intf.h>
#include <perf/perf_mem_intf.h>
#include <perf/perf_int_intf.h>
#include <perf/perf_reg_intf.h>
#include "perf_util.h"

char *bus_type_name[PERF_INT_BUS_T_MAX] = {"Pbus", "Mbus", "Cbus", "HostIf"};

struct test_description *tests_list[] = {&sram_dma_test,
                                         &tcam_dma_test,
                                         &interrupts_test,
                                         &reg_indir_test,
                                         &reg_dir_test,
                                         NULL};

struct enum_description enum_list[] = {
    {.enum_name = "bus", .enum_desc = "0:PBUS,1:MBUS,2:CBUS,3:HOSTIF"},
    // last element
    {.enum_name = ""}};

struct test_description **list_tests() {
  return tests_list;
}

struct enum_description *describe_enum() {
  return enum_list;
}

int get_params_n_max() { return PARAMS_N_MAX; }

int get_results_n_max() { return RESULTS_N_MAX; }

int get_metrics_n_max() { return METRICS_N_MAX; }

int get_env_param_n_max() { return ENV_PARAM_N_MAX; }

int get_env_param_value_l_max() { return ENV_PARAM_VALUE_L_MAX; }
