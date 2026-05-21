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

#include <bf_rt_common/bf_rt_table_operations_impl.hpp>
#include <bf_rt_common/bf_rt_utils.hpp>

bf_status_t bf_rt_operations_register_sync_set(
    bf_rt_table_operations_hdl *tbl_ops,
    const bf_rt_session_hdl *session,
    const bf_rt_target_t *dev_tgt,
    const bf_rt_register_sync_cb callback,
    const void *cookie) {
  auto table_operations =
      reinterpret_cast<bfrt::BfRtTableOperationsImpl *>(tbl_ops);
  return table_operations->registerSyncSetCFrontend(
      *reinterpret_cast<const bfrt::BfRtSession *>(session),
      *dev_tgt,
      callback,
      cookie);
}

bf_status_t bf_rt_operations_counter_sync_set(
    bf_rt_table_operations_hdl *tbl_ops,
    const bf_rt_session_hdl *session,
    const bf_rt_target_t *dev_tgt,
    const bf_rt_counter_sync_cb callback,
    const void *cookie) {
  auto table_operations =
      reinterpret_cast<bfrt::BfRtTableOperationsImpl *>(tbl_ops);
  return table_operations->counterSyncSetCFrontend(
      *reinterpret_cast<const bfrt::BfRtSession *>(session),
      *dev_tgt,
      callback,
      cookie);
}

bf_status_t bf_rt_operations_hit_state_update_set(
    bf_rt_table_operations_hdl *tbl_ops,
    const bf_rt_session_hdl *session,
    const bf_rt_target_t *dev_tgt,
    const bf_rt_hit_state_update_cb callback,
    const void *cookie) {
  auto table_operations =
      reinterpret_cast<bfrt::BfRtTableOperationsImpl *>(tbl_ops);
  return table_operations->hitStateUpdateSetCFrontend(
      *reinterpret_cast<const bfrt::BfRtSession *>(session),
      *dev_tgt,
      callback,
      cookie);
}
