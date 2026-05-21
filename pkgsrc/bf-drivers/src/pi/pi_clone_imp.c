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

#include <PI/pi_base.h>
#include <PI/pi_clone.h>
#include <PI/target/pi_clone_imp.h>

#include "pi_helpers.h"
#include "pi_log.h"

#include <pipe_mgr/pipe_mgr_mirror_intf.h>
#include <pipe_mgr/pipe_mgr_porting.h>

#include <stdbool.h>

pi_status_t _pi_clone_session_set(
    pi_session_handle_t session_handle,
    pi_dev_tgt_t dev_tgt,
    pi_clone_session_id_t clone_session_id,
    const pi_clone_session_config_t *clone_session_config) {
  LOG_TRACE("%s", __func__);
  dev_target_t pipe_mgr_dev_tgt;
  convert_dev_tgt(dev_tgt, &pipe_mgr_dev_tgt);
  bf_mirror_session_info_t session_info;
  PIPE_MGR_MEMSET(&session_info, 0, sizeof(session_info));
  // only mirror type supported by PI
  session_info.mirror_type = BF_MIRROR_TYPE_NORM;
  switch (clone_session_config->direction) {
    case PI_CLONE_DIRECTION_NONE:
      session_info.dir = BF_DIR_NONE;
      break;
    case PI_CLONE_DIRECTION_I2E:
      session_info.dir = BF_DIR_INGRESS;
      break;
    case PI_CLONE_DIRECTION_E2E:
      session_info.dir = BF_DIR_EGRESS;
      break;
    case PI_CLONE_DIRECTION_BOTH:
      session_info.dir = BF_DIR_BOTH;
      break;
    default:
      // checked by PI common code
      bf_sys_assert(0);
  }
  session_info.pipe_mask = ~(uint32_t)0;
  session_info.ucast_egress_port = clone_session_config->eg_port;
  session_info.ucast_egress_port_v = clone_session_config->eg_port_valid;
  session_info.ingress_cos = clone_session_config->cos;
  session_info.mcast_grp_a = clone_session_config->mc_grp_id;
  session_info.mcast_grp_a_v = clone_session_config->mc_grp_id_valid;
  session_info.icos_for_copy_to_cpu = clone_session_config->cos;
  session_info.copy_to_cpu = clone_session_config->copy_to_cpu;
  session_info.max_pkt_len = clone_session_config->max_packet_length
                                 ? clone_session_config->max_packet_length
                                 : 0xffff;
  bf_status_t status = bf_mirror_session_set(session_handle,
                                             pipe_mgr_dev_tgt,
                                             (bf_mirror_id_t)clone_session_id,
                                             &session_info,
                                             true);
  if (status != BF_SUCCESS) return PI_STATUS_TARGET_ERROR + status;
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_clone_session_reset(pi_session_handle_t session_handle,
                                    pi_dev_tgt_t dev_tgt,
                                    pi_clone_session_id_t clone_session_id) {
  LOG_TRACE("%s", __func__);
  dev_target_t pipe_mgr_dev_tgt;
  convert_dev_tgt(dev_tgt, &pipe_mgr_dev_tgt);
  bf_status_t status = bf_mirror_session_reset(
      session_handle, pipe_mgr_dev_tgt, (bf_mirror_id_t)clone_session_id);
  if (status != BF_SUCCESS) return PI_STATUS_TARGET_ERROR + status;
  return PI_STATUS_SUCCESS;
}
