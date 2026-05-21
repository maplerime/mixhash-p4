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
 * @file pipe_mgr_exm_hash.h
 * @date
 *
 *
 * Contains definitions for hash computation for exact match hash tables.
 */

#ifndef _PIPE_MGR_EXM_HASH_H
#define _PIPE_MGR_EXM_HASH_H

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>

/* Local header includes */
//#include "pipe_mgr_exm_tbl_mgr.h"

pipe_status_t pipe_mgr_exm_hash_compute(bf_dev_id_t dev_id,
                                        profile_id_t profile_id,
                                        pipe_mat_tbl_hdl_t mat_tbl_hdl,
                                        pipe_tbl_match_spec_t *pipe_match_spec,
                                        dev_stage_t stage_id,
                                        pipe_exm_hash_t *hash_container,
                                        uint32_t *num_entries);

uint32_t pipe_mgr_exm_extract_per_hashway_hash(pipe_exm_hash_t *hash,
                                               void *hdata,
                                               uint32_t *subword_loc);

pipe_status_t pipe_mgr_exm_proxy_hash_compute(bf_dev_id_t device_id,
                                              profile_id_t profile_id,
                                              pipe_mat_tbl_hdl_t mat_tbl_hdl,
                                              pipe_tbl_match_spec_t *match_spec,
                                              dev_stage_t stage_id,
                                              uint64_t *proxy_hash);

pipe_status_t pipe_mgr_hash_init(void);

#endif
