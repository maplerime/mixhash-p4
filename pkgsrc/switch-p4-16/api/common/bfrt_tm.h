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

#ifndef __BFRT_TM_H__
#define __BFRT_TM_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <tofino/pdfixed/pd_tm.h>
#ifdef __cplusplus
}
#endif

#include "bf_switch/bf_switch.h"

namespace smi {
switch_status_t bfrt_tm_pool_cfg_size_cells_get(
    const p4_pd_pool_id_t pd_pool_id, uint32_t &max_cell);

switch_status_t bfrt_tm_counter_pool_usage_get(p4_pd_pool_id_t pd_pool_id,
                                               uint32_t &usage_cells,
                                               uint32_t &watermark_cells);

switch_status_t bfrt_tm_counter_pool_watermark_cells_clear(
    p4_pd_pool_id_t pd_pool_id);

switch_status_t bfrt_tm_pool_app_pfc_limit_cells_set(
    p4_pd_pool_id_t pd_pool_id,
    const uint8_t icos,
    const uint32_t pfc_limit_cells);

switch_status_t bfrt_tm_pool_cfg_size_cells_set(
    const p4_pd_pool_id_t pd_pool_id, const uint32_t size_cells);

switch_status_t bfrt_tm_port_sched_shaping_rate_set(
    const uint32_t dev_port,
    std::string rate_unit,
    const uint32_t max_rate,
    const uint32_t max_burst_size);

switch_status_t bfrt_tm_port_flowcontrol_mode_tx_set(const uint32_t dev_port,
                                                     std::string mode_tx);

switch_status_t bfrt_tm_port_flowcontrol_mode_rx_set(const uint32_t dev_port,
                                                     std::string mode_rx);

switch_status_t bfrt_tm_port_flowcontrol_cos_to_icos_set(
    const uint32_t dev_port, uint8_t cos_to_icos[8]);

switch_status_t bfrt_tm_port_sched_cfg_max_rate_enable_set(
    const uint32_t dev_port, const bool max_rate_enable);

switch_status_t bfrt_tm_cfg_cell_size_bytes_get(const uint16_t device_id,
                                                uint32_t &cell_size_bytes);
switch_status_t bfrt_tm_cfg_total_buffer_size_get(const uint16_t device_id,
                                                  uint64_t &total_buffer_size);

}  // namespace smi
#endif  // __BFRT_TM_H__
