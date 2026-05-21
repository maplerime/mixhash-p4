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
 * @file pipe_mgr_tcam.h
 * @date
 *
 * TCAM related definitions of pipeline manager
 */

#ifndef _PIPE_MGR_TIND_H_
#define _PIPE_MGR_TIND_H_

#include "pipe_mgr_tcam.h"

pipe_status_t pipe_mgr_tcam_tind_tbl_alloc(tcam_stage_info_t *stage_data,
                                           pipe_mat_tbl_info_t *mat_tbl_info);

bool pipe_mgr_tcam_tind_get_line_no(tcam_stage_info_t *stage_data,
                                    uint32_t physical_line_index,
                                    uint32_t *tind_line_no,
                                    uint32_t *tind_block,
                                    uint32_t *subword_pos);








bool pipe_mgr_tcam_tind_exists(tcam_stage_info_t *stage_data);

#endif
