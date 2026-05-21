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

#ifndef __PIPE_MGR_PARB_H__
#define __PIPE_MGR_PARB_H__

#include <pipe_mgr/pipe_mgr_intf.h>

pipe_status_t parb_set_port_ingress_chnl_control(bf_dev_id_t dev_id,
                                                 bf_dev_port_t port_id);

pipe_status_t parb_set_port_egress_chnl_control(bf_dev_id_t dev_id,
                                                bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parb_pps_limit_set(pipe_sess_hdl_t shdl,
                                          rmt_dev_info_t *dev_info,
                                          bf_dev_pipe_t log_pipe,
                                          uint64_t pps);
pipe_status_t pipe_mgr_parb_pps_limit_get(rmt_dev_info_t *dev_info,
                                          bf_dev_pipe_t log_pipe,
                                          uint64_t *max_pps);
pipe_status_t pipe_mgr_parb_pps_limit_max_get(rmt_dev_info_t *dev_info,
                                              bf_dev_pipe_t log_pipe,
                                              uint64_t *max_pps);
pipe_status_t pipe_mgr_parb_pps_limit_reset(pipe_sess_hdl_t shdl,
                                            rmt_dev_info_t *dev_info,
                                            bf_dev_pipe_t log_pipe);
#endif
