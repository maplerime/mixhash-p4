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

#ifndef __SWITCH_CONFIG_H__
#define __SWITCH_CONFIG_H__
#include "bf_switchd.h"

void switch_p4_pipeline_config_each_program_update(
    p4_devices_t *p4_device,
    bf_device_profile_t *device_profile,
    const char *install_dir,
    bool absolute_paths);
void switch_p4_pipeline_config_each_profile_update(
    p4_programs_t *p4_program,
    bf_p4_program_t *bf_p4_program,
    const char *install_dir,
    bool absolute_paths);
int switch_dev_config_init(const char *install_dir,
                           const char *config_filename,
                           bf_switchd_internal_context_t *self);
int switch_pci_sysfs_str_get(char *name,
                             size_t name_size,
                             bf_dev_family_t chip_family);
bool switch_is_iommu_enabled(void);
#endif /* __SWITCH_CONFIG_H__ */
