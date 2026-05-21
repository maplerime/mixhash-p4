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
 * @file pipe_mgr_tof_deprsr.h
 * @date
 *
 * Configuration of Tofino Deparser based on port speed.
 */

#ifndef __PIPE_MGR_TOF_DEPRSR_H__
#define __PIPE_MGR_TOF_DEPRSR_H__
/* Maintains a software shadow of the following three registers:
 *  pipes[pipe].deparser.out_ingr.regs.ctm_ch_rate.ctm_ch_rate_0_3
 *  pipes[pipe].deparser.out_ingr.regs.ctm_ch_rate.ctm_ch_rate_1_3
 *  pipes[pipe].deparser.out_ingr.regs.ctm_ch_rate.ctm_ch_rate_2_3
 * Note that these are arrays with the physical pipe as an index.
 * Note that these values are only modified when a port is added. */
struct pipe_mgr_tof_deprsr_ctx {
  uint32_t *ch_rate_0_3;
  uint32_t *ch_rate_1_3;
  uint32_t *ch_rate_2_3;
};

pipe_status_t pipe_mgr_tof_deprsr_cfg_init(pipe_sess_hdl_t sess_hdl,
                                           rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof_deprsr_cfg_deinit(bf_dev_id_t dev_id);
pipe_status_t pipe_mgr_tof_deprsr_set_port_speed_based_cfg(
    rmt_dev_info_t *dev_info, bf_dev_port_t port_id);
#endif
