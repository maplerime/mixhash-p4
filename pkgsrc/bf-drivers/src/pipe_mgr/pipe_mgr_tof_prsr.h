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
 * @file pipe_mgr_tof_prsr.h
 * @date
 *
 * Implementation/Configuration of Ingress parser, parser-merge and Egress
 * parser based on port speed.
 */
#ifndef __PIPE_MGR_TOF_PRSR_H__
#define __PIPE_MGR_TOF_PRSR_H__

#include <pipe_mgr/pipe_mgr_intf.h>

pipe_status_t pipe_mgr_tof_iprsr_port_speed_based_cfg(rmt_dev_info_t *dev_info,
                                                      bf_dev_port_t port_id);

pipe_status_t pipe_mgr_tof_eprsr_port_speed_based_cfg(rmt_dev_info_t *dev_info,
                                                      bf_dev_port_t port_id);

pipe_status_t pipe_mgr_tof_eprsr_complete_port_mode_transition_wa(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);

pipe_status_t pipe_mgr_tof_iprsr_get_pri_thresh(rmt_dev_info_t *dev_info,
                                                rmt_port_info_t *port_info,
                                                uint32_t *val);

pipe_status_t pipe_mgr_tof_iprsr_set_pri_thresh(rmt_dev_info_t *dev_info,
                                                rmt_port_info_t *port_info,
                                                uint32_t val);
#endif
