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

#ifndef _PIPE_MGR_MIRROR_BUFFER_HA_H_
#define _PIPE_MGR_MIRROR_BUFFER_HA_H_

#include "pipe_mgr/pipe_mgr_mirror_intf.h"
#include "pipe_mgr_mirror_buffer_comm.h"
#include "pipe_mgr_mirror_buffer.h"

pipe_status_t pipe_mgr_mirror_hitless_ha_init(pipe_sess_hdl_t sess_hdl,
                                              rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_mirror_ha_compute_delta_changes(
    pipe_sess_hdl_t shdl, rmt_dev_info_t *dev_info);
#endif  // _PIPE_MGR_MIRROR_BUFFER_HA_H_
