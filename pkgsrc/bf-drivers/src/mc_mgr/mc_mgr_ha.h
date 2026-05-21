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

#ifndef __MC_MGR_HA_H__
#define __MC_MGR_HA_H__

void mc_mgr_free_ha_hw_state(struct mc_mgr_dev_hw_state *st);
bf_status_t mc_mgr_hitless_ha_hw_read(bf_dev_id_t dev_id);
bf_status_t mc_mgr_read_hw_state(int sid,
                                 bf_dev_id_t dev,
                                 struct mc_mgr_dev_hw_state *state);
bf_status_t mc_mgr_compute_delta_changes(bf_dev_id_t dev_id,
                                         bool disable_input_pkts);
bf_status_t mc_mgr_push_delta_changes(bf_dev_id_t dev_id);
#endif
