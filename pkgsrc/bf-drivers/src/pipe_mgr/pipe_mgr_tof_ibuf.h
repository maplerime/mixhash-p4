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

#ifndef __PIPE_MGR_TOF_IBUF_H__
#define __PIPE_MGR_TOF_IBUF_H__

#include <pipe_mgr/pipe_mgr_intf.h>

#define BF_TOFINO_IBUF_SIZE (24 * 1024)  // 24KB

pipe_status_t pipe_mgr_ibuf_tof_set_logical_port(pipe_sess_hdl_t sess_hdl,
                                                 rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ibuf_tof_set_version_bits(pipe_sess_hdl_t sess_hdl,
                                                 rmt_dev_info_t *dev_info,
                                                 uint8_t version);
pipe_status_t pipe_mgr_ibuf_tof_disable_all_chan(pipe_sess_hdl_t sess_hdl,
                                                 rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ibuf_tof_port_set_drop_threshold(
    rmt_dev_info_t *dev_info,
    bf_dev_port_t port_id,
    uint32_t drop_hi_thrd,
    uint32_t drop_low_thrd);
pipe_status_t pipe_mgr_ibuf_tof_port_set_afull_threshold(
    rmt_dev_info_t *dev_info,
    bf_dev_port_t port_id,
    uint32_t afull_hi_thrd,
    uint32_t afull_low_thrd);

pipe_status_t pipe_mgr_ibuf_tof_set_port_speed_based_cfg(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_ibuf_tof_enable_channel(rmt_dev_info_t *dev_info,
                                               bf_dev_port_t port_id);
pipe_status_t pipe_mgr_ibuf_tof_enable_channel_all(pipe_sess_hdl_t sess_hdl,
                                                   rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ibuf_tof_enable_congestion_notif_to_parser(
    rmt_dev_info_t *dev_info, rmt_port_info_t *port_info);
pipe_status_t pipe_mgr_ibuf_tof_parb_enable_flow_control_to_mac(
    rmt_dev_info_t *dev_info,
    bf_dev_port_t port_id,
    uint16_t low_wm_bytes,
    uint16_t hi_wm_bytes);
pipe_status_t pipe_mgr_ibuf_tof_disable_congestion_notif_to_parser(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_ibuf_tof_parb_disable_flow_control_to_mac(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
pipe_status_t pipe_mgr_ibuf_tof_disable_chnl(rmt_dev_info_t *dev_info,
                                             bf_dev_port_t port_id);
pipe_status_t pipe_mgr_ibuf_tof_set_1588_timestamp_offset(
    pipe_sess_hdl_t sess_hdl, rmt_dev_info_t *dev_info);

pipe_status_t ibuf_set_chnl_ctrl(rmt_dev_info_t *dev_info,
                                 bf_dev_port_t port_id,
                                 bool chnl_enable,
                                 pipe_sess_hdl_t shdl,
                                 bool use_dma);
#endif
