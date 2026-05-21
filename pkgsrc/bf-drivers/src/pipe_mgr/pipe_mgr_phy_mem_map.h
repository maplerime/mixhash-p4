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
 * @file pipe_mgr_phy_mem_map.h
 * @date
 *
 * Internal definitions for pipe_mgr's physical shadow copy
 */

#ifndef _PIPE_MGR_PHY_MEM_MAP_H
#define _PIPE_MGR_PHY_MEM_MAP_H

/* Standard includes */
#include "pipe_mgr_int.h"
#include "pipe_mgr_drv.h"

#define PIPE_MGR_SHADOW_MEM_DUMP_STR_LEN 65536

/* --------- Functions ---------- */
pipe_status_t pipe_mgr_phy_mem_map_init(bf_dev_id_t dev);

pipe_status_t pipe_mgr_phy_mem_map_cleanup(bf_dev_id_t dev);

pipe_status_t pipe_mgr_phy_mem_map_write(bf_dev_id_t dev,
                                         pipe_tbl_dir_t gress,
                                         bf_dev_pipe_t pipe_id,
                                         dev_stage_t stage_id,
                                         pipe_mem_type_t mem_type,
                                         mem_id_t mem_id,
                                         uint32_t line_num,
                                         uint8_t *data,
                                         uint8_t *mask);

pipe_status_t pipe_mgr_phy_mem_map_read(bf_dev_id_t dev,
                                        pipe_tbl_dir_t gress,
                                        bf_dev_pipe_t pipe_id,
                                        dev_stage_t stage_id,
                                        pipe_mem_type_t mem_type,
                                        mem_id_t mem_id,
                                        uint32_t line_num,
                                        uint8_t *data,
                                        int data_len);

pipe_status_t pipe_mgr_phy_mem_map_copy(bf_dev_id_t dev,
                                        pipe_tbl_dir_t gress,
                                        bf_dev_pipe_t pipe_id,
                                        dev_stage_t src_stage_id,
                                        dev_stage_t dst_stage_id,
                                        uint32_t lower_src_addr,
                                        uint32_t lower_dst_addr,
                                        bool invalidate_src);

pipe_status_t pipe_mgr_phy_mem_map_symmetric_mode_set(
    bf_dev_id_t dev,
    pipe_tbl_dir_t gress,
    pipe_bitmap_t *log_pipe_bmp,
    dev_stage_t stage_id,
    pipe_mem_type_t mem_type,
    mem_id_t mem_id,
    bool symmetric);

pipe_status_t pipe_mgr_phy_mem_map_get_ref(bf_dev_id_t dev,
                                           pipe_tbl_dir_t gress,
                                           uint8_t mem_type,
                                           bf_dev_pipe_t pipe_id,
                                           dev_stage_t stage_id,
                                           mem_id_t mem_id,
                                           uint32_t line_num,
                                           uint8_t **data_ref,
                                           bool read_only);

pipe_status_t pipe_mgr_phy_mem_map_download_one_block(bf_dev_id_t dev,
                                                      pipe_tbl_dir_t gress,
                                                      bf_dev_pipe_t pipe_id,
                                                      dev_stage_t stage_id,
                                                      pipe_mem_type_t mem_type,
                                                      mem_id_t mem_id);

pipe_status_t pipe_mgr_sram_tcam_ecc_error_correct(bf_dev_id_t dev,
                                                   uint64_t address);

pipe_status_t pipe_mgr_dump_phy_shadow_memory(bf_dev_id_t dev_id,
                                              pipe_tbl_dir_t gress,
                                              bf_dev_pipe_t log_pipe_id,
                                              dev_stage_t stage_id,
                                              uint16_t mem_id,
                                              uint8_t mem_type,
                                              uint16_t line_no,
                                              bool all_lines,
                                              char *str,
                                              int max_len);

pipe_status_t phy_mem_map_load_srams_tcams(rmt_dev_info_t *dev_info,
                                           bool everything);

#endif
