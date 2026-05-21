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

#include <stdbool.h>
#include <bf_types/bf_types.h>
#include <dvm/bf_dma_types.h>
#include <lld/bf_dma_if.h>
#include "mc_mgr_handle.h"

#define MC_SESS_HDL_HIGH 1
#define MC_MGRP_HDL_HIGH 2
#define MC_ECMP_HDL_HIGH 3
#define MC_L1_HDL_HIGH 4
#define MC_L2_HDL_HIGH 5

bf_mc_session_hdl_t mc_mgr_encode_sess_hdl(int index) {
  return (MC_SESS_HDL_HIGH << 28) | (index << 0);
}

bool mc_mgr_decode_sess_hdl(bf_mc_session_hdl_t hdl, int *index) {
  if (MC_SESS_HDL_HIGH == (hdl >> 28)) {
    if (index) *index = hdl & 0xFF;
    return true;
  } else {
    return false;
  }
}

bf_mc_mgrp_hdl_t mc_mgr_encode_mgrp_hdl(bf_mc_grp_id_t grp) {
  return (MC_MGRP_HDL_HIGH << 28) | (grp << 0);
}
bool mc_mgr_decode_mgrp_hdl(bf_mc_mgrp_hdl_t h,
                            bf_mc_grp_id_t *grp,
                            const char *where,
                            const int line) {
  if (MC_MGRP_HDL_HIGH == (h >> 28)) {
    if (grp) *grp = (h >> 0) & 0xFFFFFF;
    return true;
  } else {
    LOG_ERROR("Invalid mgrp handle %#x from %s:%d", h, where, line);
    return false;
  }
}

bf_status_t mc_mgr_encode_ecmp_hdl(bf_dev_id_t dev, bf_mc_ecmp_hdl_t *e) {
  int id = bf_id_allocator_allocate(mc_mgr_ctx_ecmp_sw_id_gen(dev));
  if (id == -1 || id > 0x00FFFFFF) {
    return BF_NO_SYS_RESOURCES;
  }
  *e = ((MC_ECMP_HDL_HIGH & 0xF) << 28) | id;
  return BF_SUCCESS;
}
bf_status_t mc_mgr_decode_ecmp_hdl(bf_mc_ecmp_hdl_t e,
                                   const char *where,
                                   const int line) {
  if (MC_ECMP_HDL_HIGH == (e >> 28)) {
    return BF_SUCCESS;
  } else {
    LOG_ERROR("Invalid ECMP handle %#x from %s:%d", e, where, line);
    return BF_INVALID_ARG;
  }
}
bf_status_t mc_mgr_delete_ecmp_hdl(bf_dev_id_t dev, bf_mc_ecmp_hdl_t hdl) {
  if (MC_ECMP_HDL_HIGH != (hdl >> 28)) {
    return BF_INVALID_ARG;
  }
  int id = hdl & 0xFFFF;
  bf_id_allocator_release(mc_mgr_ctx_ecmp_sw_id_gen(dev), id);
  return BF_SUCCESS;
}

bf_status_t mc_mgr_encode_l1_node_hdl(bf_dev_id_t dev, bf_mc_node_hdl_t *hdl) {
  int id = bf_id_allocator_allocate(mc_mgr_ctx_l1_id_gen(dev));
  if (id == -1 || id > 0x00FFFFFF) {
    return BF_NO_SYS_RESOURCES;
  }
  *hdl = ((MC_L1_HDL_HIGH & 0xF) << 28) | id;
  return BF_SUCCESS;
}
bool mc_mgr_decode_l1_node_hdl(bf_mc_node_hdl_t h,
                               int *id,
                               const char *where,
                               const int line) {
  if (MC_L1_HDL_HIGH == (h >> 28)) {
    if (id) *id = (h >> 0) & 0x00FFFFFF;
    return true;
  } else {
    LOG_ERROR("Invalid L1 node handle %#x from %s:%d", h, where, line);
    return false;
  }
}
bf_status_t mc_mgr_delete_l1_node_hdl(bf_dev_id_t dev, bf_mc_node_hdl_t hdl) {
  int id;
  if (!mc_mgr_decode_l1_node_hdl(hdl, &id, __func__, __LINE__)) {
    return BF_INVALID_ARG;
  }
  bf_id_allocator_release(mc_mgr_ctx_l1_id_gen(dev), id);
  return BF_SUCCESS;
}
