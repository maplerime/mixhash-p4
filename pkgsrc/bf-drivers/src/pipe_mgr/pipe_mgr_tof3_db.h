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

#ifndef _PIPE_MGR_TOF3_DB_H
#define _PIPE_MGR_TOF3_DB_H
#include "pipe_mgr_db.h"
#define PIPE_MGR_TOF3_GFM_PARITY_COL 51
#define PIPE_MGR_TOF3_SINGLE_GFM_ENTRY_SZ 16

pipe_status_t pipe_mgr_tof3_interrupt_db_init(rmt_dev_info_t *dev_info);
void pipe_mgr_tof3_interrupt_db_cleanup(rmt_dev_info_t *dev_info);
void pipe_mgr_tof3_prsr_db_init(bf_dev_id_t dev);
pipe_status_t pipe_mgr_tof3_interrupt_cache_imem_val(rmt_dev_info_t *dev_info,
                                                     uint32_t log_pipe_mask,
                                                     dev_stage_t stage,
                                                     uint32_t base_address,
                                                     uint8_t *data,
                                                     int data_len);

pipe_status_t pipe_mgr_tof3_cache_prsr_reg_val(
    rmt_dev_info_t *dev_info,
    pipe_prsr_instance_hdl_t prsr_instance_hdl,
    profile_id_t prof_id,
    uint32_t address,
    uint32_t data,
    bool *shadowed);

pipe_status_t pipe_mgr_tof3_cache_prsr_val(
    rmt_dev_info_t *dev_info,
    profile_id_t prof_id,
    pipe_prsr_instance_hdl_t prsr_instance_hdl,
    uint64_t address,
    uint8_t *data,
    int data_len,
    bool *shadowed);

pipe_status_t pipe_mgr_tof3_interrupt_cache_mirrtbl_val(
    rmt_dev_info_t *dev_info,
    uint32_t log_pipe_mask,
    uint32_t address,
    uint8_t *data,
    int data_len);
pipe_status_t pipe_mgr_tof3_cache_gfm(rmt_dev_info_t *dev_info,
                                      uint32_t log_pipe_mask,
                                      dev_stage_t stage,
                                      uint32_t address,
                                      uint8_t *data,
                                      int data_len);

pipe_status_t pipe_mgr_tof3_interrupt_set_parser_tcam_shadow(
    rmt_dev_info_t *dev_info,
    bf_dev_pipe_t pipe,
    bool ing0_egr1,
    int prsr_id,
    int tcam_index,
    uint8_t data_len,
    uint8_t *word0,
    uint8_t *word1);
pipe_status_t pipe_mgr_tof3_recalc_write_gfm_parity(pipe_sess_hdl_t sess_hdl,
                                                    rmt_dev_info_t *dev_info,
                                                    pipe_bitmap_t *pipe_bmp,
                                                    dev_stage_t stage,
                                                    bool skip_write);
pipe_status_t pipe_mgr_tof3_mirrtbl_write(pipe_sess_hdl_t shdl,
                                          rmt_dev_info_t *dev_info);
#endif
