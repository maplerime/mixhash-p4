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

#ifndef __PIPE_MGR_TOF3_IBUF_H__
#define __PIPE_MGR_TOF3_IBUF_H__

#include <pipe_mgr/pipe_mgr_intf.h>
#include "pipe_mgr_db.h"
pipe_status_t pipe_mgr_parde_tof3_device_add(pipe_sess_hdl_t shdl,
                                             rmt_dev_info_t *dev_info);
void pipe_mgr_parde_tof3_device_rmv(rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_parde_tof3_port_add_ing(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info,
                                               bf_dev_port_t port_id);
pipe_status_t pipe_mgr_parde_tof3_port_add_egr(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info,
                                               bf_dev_port_t port_id);
pipe_status_t pipe_mgr_parde_tof3_port_rmv_egr(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info,
                                               bf_dev_port_t port_id);
pipe_status_t pipe_mgr_parde_tof3_port_rmv_ing(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info,
                                               bf_dev_port_t port_id);
pipe_status_t pipe_mgr_parde_tof3_port_ena_ing_all(pipe_sess_hdl_t shdl,
                                                   rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_parde_tof3_port_dis_ing_all(pipe_sess_hdl_t shdl,
                                                   rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_parde_tof3_port_dis_ing_all_with_dma(
    pipe_sess_hdl_t shdl, rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_ibuf_tof3_set_version_bits(pipe_sess_hdl_t shdl,
                                                  rmt_dev_info_t *dev_info,
                                                  uint8_t version);
pipe_status_t pipe_mgr_ibuf_tof3_config_congestion_notif_to_parser(
    pipe_sess_hdl_t shdl, rmt_dev_info_t *dev_info, rmt_port_info_t *port_info);

pipe_status_t pipe_mgr_parde_tof3_port_ena_one(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info,
                                               uint8_t logical_pipe,
                                               int ipb_num,
                                               bool ing_0_egr_1);
pipe_status_t pipe_mgr_parde_tof3_port_dis_one(pipe_sess_hdl_t shdl,
                                               rmt_dev_info_t *dev_info,
                                               uint8_t logical_pipe,
                                               int ipb_num,
                                               bool ing_0_egr_1);
pipe_status_t pipe_mgr_parser_config_tof3(
    pipe_sess_hdl_t sess_hdl,
    rmt_dev_info_t *dev_info,
    uint8_t gress,
    pipe_bitmap_t pipe_bmp,
    uint64_t prsr_grp_map,
    struct pipe_mgr_tof3_prsr_bin_config *cfg);

pipe_status_t pipe_mgr_tof3_iprsr_get_pri_thresh(rmt_dev_info_t *dev_info,
                                                 rmt_port_info_t *port_info,
                                                 uint32_t *val);

pipe_status_t pipe_mgr_tof3_iprsr_set_pri_thresh(rmt_dev_info_t *dev_info,
                                                 rmt_port_info_t *port_info,
                                                 uint32_t val);
#endif
