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

#ifndef _PIPE_MGR_TOF3_DKM_H
#define _PIPE_MGR_TOF3_DKM_H

#include "pipe_mgr_dkm.h"

uint32_t pipe_mgr_tof3_dkm_match_mask_addr_get(
    bf_dev_pipe_t pipe, int stage, int ram_row, int ram_col, int index);

uint32_t pipe_mgr_tof3_dkm_galios_field_matrix_addr_get(bf_dev_pipe_t pipe,
                                                        int stage,
                                                        int gfm_row,
                                                        int gfm_col);
#endif
