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
//  mc_mgr_shared_dr_wrapper.c
//

#include <stdio.h>
#include <sched.h>
#include <stdint.h>

#include <dvm/bf_drv_intf.h>
#include <lld/bf_dma_if.h>
#include <lld/lld_dr_descriptors.h>
#include <lld/lld_memory_mapping.h>
#include <lld/lld_dr_if.h>
#include <mc_mgr/mc_mgr_shared_dr.h>

typedef struct dr_user_info_ {
  int dr_user_id;
  dr_completion_callback_fn cb_fn;
  bf_dma_dr_id_t dr;
} dr_user_info_t;

#define BF_MAX_DR_USERS (2)
static dr_user_info_t dr_users[BF_MAX_DEV_COUNT][BF_MAX_DR_USERS];
static bool dr_active_state[BF_MAX_DEV_COUNT] = {false};
typedef enum {
  BF_SHARED_DR_USER_MCMGR = 0,
  BF_SHARED_DR_USER_TM,
} bf_shared_dr_users;

static void bf_shared_dr_dma_complete_cb(bf_dev_id_t dev,
                                         bf_subdev_id_t subdev_id,
                                         bf_dma_dr_id_t dr,
                                         uint64_t data_sz_or_ts,
                                         uint32_t attr,
                                         uint32_t status,
                                         uint32_t type,
                                         uint64_t msg_id,
                                         int s,
                                         int e) {
  int i;

  bf_sys_assert(lld_dr_cmp_que_write_list == dr);
  for (i = 0; i < BF_MAX_DR_USERS; i++) {
    if (dr_users[dev][i].cb_fn != NULL) {
      dr_users[dev][i].cb_fn(
          dev, subdev_id, dr, data_sz_or_ts, attr, status, type, msg_id, s, e);
    }
  }
}

// A primitive implementation of sharing DR by two or more
// components/submodules.
// Primitive because we do not forsee sharing of DRs. As of now there is only
// one case of TM and MC mgr code sharing write list DR.
int mcmgr_tm_register_completion_cb(bf_dev_id_t chip,
                                    bf_dma_dr_id_t dr,
                                    dr_completion_callback_fn fn,
                                    int dr_user_id) {
  dr_completion_callback_fn local_dma_cb;
  int i, rc;

  i = (dr_user_id == BF_MC_DMA_MSG_ID) ? BF_SHARED_DR_USER_MCMGR
                                       : BF_SHARED_DR_USER_TM;
  // setup cb
  dr_users[chip][i].dr_user_id = dr_user_id;
  dr_users[chip][i].cb_fn = fn;
  dr_users[chip][i].dr = dr;

  local_dma_cb = (dr_completion_callback_fn)bf_shared_dr_dma_complete_cb;
  rc = lld_register_completion_callback(chip, 0, dr, local_dma_cb);
  return rc < 0 ? rc : 0;
}

void mcmgr_tm_set_dr_state(bf_dev_id_t dev, bool enable) {
  dr_active_state[dev] = enable;
}

bool mcmgr_tm_get_dr_state(bf_dev_id_t dev) { return dr_active_state[dev]; }
