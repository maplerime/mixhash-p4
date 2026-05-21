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

///
//  mc_mgr_shared_dr.h
//
#ifndef __MC_MGR_SHARED_DR_H__
#define __MC_MGR_SHARED_DR_H__

#include <stdio.h>

#define BF_MC_DMA_MSG_ID \
  (7)  // Any value other than BF_TM_DMA_MSG_ID becasue
       // both TM and MC write list share same DR.
       // DR level syncronization is done in LLD. No need
       // for another level of locks on DR.
#define BF_TM_DMA_MSG_ID (3)
#define BF_DIAG_DMA_MSG_ID (0)

int mcmgr_tm_register_completion_cb(bf_dev_id_t chip,
                                    bf_dma_dr_id_t dr,
                                    dr_completion_callback_fn fn,
                                    int dr_user_id);

void mcmgr_tm_set_dr_state(bf_dev_id_t dev, bool enable);
bool mcmgr_tm_get_dr_state(bf_dev_id_t dev);
#endif
