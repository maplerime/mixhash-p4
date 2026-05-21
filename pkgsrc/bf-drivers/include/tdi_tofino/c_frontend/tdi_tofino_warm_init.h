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

/** @file tdi_tofino_warm_init.h
 *
 *  @brief C frontend for tofino specific warm-init
 */
#ifndef _TDI_TOFINO_WARM_INIT_H_
#define _TDI_TOFINO_WARM_INIT_H_

// tdi includes
#include <tdi/common/tdi_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pipeline_ {
  char *name;
  char *context_path;
  char *binary_path;
  uint32_t *scope_vec;
  int num_pipes;
} tdi_pipeline_t;

tdi_status_t device_warm_init_begin(tdi_dev_id_t device_id,
                                    char *init_mode,
                                    char *serdes_mode,
                                    bool upgrade_agents,
                                    tdi_dev_config_hdl *prog_config);

tdi_status_t device_warm_init_end(tdi_dev_id_t dev_id);

tdi_status_t dev_config_allocate(int num_programs,
                                 tdi_dev_config_hdl **dev_config_hdl);

tdi_status_t dev_config_deallocate(tdi_dev_config_hdl *dev_config_hdl);

tdi_status_t set_program_name(tdi_dev_config_hdl *dev_config_hdl,
                              int index,
                              char *program_name);
tdi_status_t set_base_path(tdi_dev_config_hdl *dev_config_hdl,
                           int index,
                           char *base_path);
tdi_status_t set_pipeline(tdi_dev_config_hdl *dev_config_hdl,
                          int prog_index,
                          tdi_pipeline_t pipeline);
tdi_status_t set_tdi_info_path(tdi_dev_config_hdl *dev_config_hdl,
                               int prog_index,
                               char *tdi_info_path);

#ifdef __cplusplus
}
#endif

#endif  // _TDI_TOFINO_WARM_INIT_H_
