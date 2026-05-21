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

#ifndef __PIPE_MGR_PARDE_H__
#define __PIPE_MGR_PARDE_H__

#include "pipe_mgr_tof_ibuf.h"
#include "pipe_mgr_tof_ebuf.h"
#include "pipe_mgr_tof_parb.h"
#include "pipe_mgr_tof_prsr.h"
#include "pipe_mgr_tof_deprsr.h"

struct pipe_mgr_tof_ebuf_pipe_reg_ctx;
struct pipe_mgr_tof2_ebuf_pipe_reg_ctx;
struct pipe_mgr_tof3_ebuf_pipe_reg_ctx;

struct pipe_mgr_tof_ebuf_ctx {
  /* Allocated based on number of pipes. */
  struct pipe_mgr_tof_ebuf_pipe_reg_ctx *reg;
};
struct pipe_mgr_tof2_ebuf_ctx {
  /* Allocated based on number of pipes. */
  struct pipe_mgr_tof2_ebuf_pipe_reg_ctx *reg;
};
struct pipe_mgr_tof3_ebuf_ctx {
  /* Allocated based on number of pipes. */
  struct pipe_mgr_tof3_ebuf_pipe_reg_ctx *reg;
};
union pipe_mgr_ebuf_ctx {
  struct pipe_mgr_tof_ebuf_ctx tof;
  struct pipe_mgr_tof2_ebuf_ctx tof2;
  struct pipe_mgr_tof3_ebuf_ctx tof3;
};
union pipe_mgr_deprsr_ctx {
  struct pipe_mgr_tof_deprsr_ctx tof;
};

pipe_status_t pipe_mgr_parde_device_add(pipe_sess_hdl_t shdl,
                                        rmt_dev_info_t *dev_info);

void pipe_mgr_parde_device_rmv(rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_parde_port_add(pipe_sess_hdl_t shdl,
                                      rmt_dev_info_t *dev_info,
                                      bf_port_cb_direction_t direction,
                                      bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_port_rmv(pipe_sess_hdl_t shdl,
                                      rmt_dev_info_t *dev_info,
                                      bf_port_cb_direction_t direction,
                                      bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_complete_port_mode_transition_wa(
    pipe_sess_hdl_t shdl, rmt_dev_info_t *dev_info, bf_dev_port_t port_id);

pipe_status_t pipe_mgr_parde_traffic_disable(pipe_sess_hdl_t shdl,
                                             rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_parde_traffic_enable(pipe_sess_hdl_t shdl,
                                            rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_parde_wait_for_traffic_flush(pipe_sess_hdl_t shdl,
                                                    rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_parde_set_port_cut_through(pipe_sess_hdl_t shdl,
                                                  rmt_dev_info_t *dev_info,
                                                  bf_dev_port_t port_id,
                                                  bool cut_through_enabled);

pipe_status_t pipe_mgr_parde_port_set_drop_threshold(pipe_sess_hdl_t sess_hdl,
                                                     bf_dev_id_t dev_id,
                                                     bf_dev_port_t port_id,
                                                     uint32_t drop_hi_thrd,
                                                     uint32_t drop_low_thrd);

pipe_status_t pipe_mgr_parde_port_set_afull_threshold(pipe_sess_hdl_t sess_hdl,
                                                      bf_dev_id_t dev_id,
                                                      bf_dev_port_t port_id,
                                                      uint32_t afull_hi_thrd,
                                                      uint32_t afull_low_thrd);

pipe_status_t pipe_mgr_parser_config_create_dma(pipe_sess_hdl_t sess_hdl,
                                                rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_parde_port_ebuf_counter_get(pipe_sess_hdl_t shdl,
                                                   rmt_dev_info_t *dev_info,
                                                   bf_dev_port_t port_id,
                                                   uint64_t *value);

pipe_status_t pipe_mgr_parde_port_ebuf_bypass_counter_get(
    pipe_sess_hdl_t shdl,
    rmt_dev_info_t *dev_info,
    bf_dev_port_t port_id,
    uint64_t *value);

pipe_status_t pipe_mgr_parde_port_ebuf_100g_credits_get(
    pipe_sess_hdl_t shdl,
    rmt_dev_info_t *dev_info,
    bf_dev_port_t port_id,
    uint64_t *value);

pipe_status_t pipe_mgr_parde_iprsr_pri_threshold_set(rmt_dev_info_t *dev_info,
                                                     bf_dev_port_t port_id,
                                                     uint32_t threshold);

pipe_status_t pipe_mgr_parde_iprsr_pri_threshold_get(rmt_dev_info_t *dev_info,
                                                     bf_dev_port_t port_id,
                                                     uint32_t *threshold);
#endif
