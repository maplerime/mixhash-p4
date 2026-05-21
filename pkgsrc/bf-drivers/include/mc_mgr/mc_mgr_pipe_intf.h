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

#ifndef __MC_MGR_PIPE_INTF_H__
#define __MC_MGR_PIPE_INTF_H__

#include <mc_mgr/mc_mgr_intf.h>

bf_status_t mc_mgr_ecc_correct_pvt(bf_dev_id_t dev,
                                   bf_dev_pipe_t pipe,
                                   uint16_t row,
                                   bool batch);

bf_status_t mc_mgr_ecc_correct_mit(bf_dev_id_t dev,
                                   bf_dev_pipe_t pipe,
                                   uint32_t address);
bf_status_t mc_mgr_ecc_correct_lit_bm(bf_dev_id_t dev,
                                      int ver,
                                      uint32_t address);

bf_status_t mc_mgr_ecc_correct_lit_np(bf_dev_id_t dev,
                                      int ver,
                                      uint32_t address);

bf_status_t mc_mgr_ecc_correct_pmt(bf_dev_id_t dev, int ver, uint32_t address);

bf_status_t mc_mgr_ecc_correct_rdm(bf_dev_id_t dev, uint32_t address);

#endif
