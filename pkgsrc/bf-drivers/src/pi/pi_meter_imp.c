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

#include <PI/p4info.h>
#include <PI/pi.h>
#include <PI/target/pi_meter_imp.h>

#include "pi_helpers.h"
#include "pi_log.h"
#include "pi_resource_specs.h"
#include "pi_state.h"

#include <pipe_mgr/pipe_mgr_intf.h>

pi_status_t _pi_meter_read(pi_session_handle_t session_handle,
                           pi_dev_tgt_t dev_tgt,
                           pi_p4_id_t meter_id,
                           size_t index,
                           pi_meter_spec_t *meter_spec) {
  (void)session_handle;
  (void)dev_tgt;
  (void)meter_id;
  (void)index;
  (void)meter_spec;
  LOG_DBG("%s", __func__);
  // not exposed by pipe_mgr
  return PI_STATUS_NOT_IMPLEMENTED_BY_TARGET;
}

pi_status_t _pi_meter_set(pi_session_handle_t session_handle,
                          pi_dev_tgt_t dev_tgt,
                          pi_p4_id_t meter_id,
                          size_t index,
                          const pi_meter_spec_t *meter_spec) {
  LOG_DBG("%s", __func__);
  const pi_p4info_t *p4info = pi_get_device_p4info(dev_tgt.dev_id);
  dev_target_t pipe_mgr_dev_tgt;
  convert_dev_tgt(dev_tgt, &pipe_mgr_dev_tgt);
  pipe_meter_spec_t pipe_meter_spec;
  convert_to_pipe_meter_spec(p4info, meter_id, meter_spec, &pipe_meter_spec);
  uint32_t meter_handle = pi_state_res_id_to_handle(dev_tgt.dev_id, meter_id);
  pipe_status_t status = pipe_mgr_meter_ent_set(session_handle,
                                                pipe_mgr_dev_tgt,
                                                meter_handle,
                                                index,
                                                &pipe_meter_spec,
                                                0);
  if (status != PIPE_SUCCESS) return PI_STATUS_TARGET_ERROR + status;
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_meter_read_direct(pi_session_handle_t session_handle,
                                  pi_dev_tgt_t dev_tgt,
                                  pi_p4_id_t meter_id,
                                  pi_entry_handle_t entry_handle,
                                  pi_meter_spec_t *meter_spec) {
  LOG_DBG("%s", __func__);
  const pi_p4info_t *p4info = pi_get_device_p4info(dev_tgt.dev_id);
  dev_target_t pipe_mgr_dev_tgt;
  convert_dev_tgt(dev_tgt, &pipe_mgr_dev_tgt);
  pi_p4_id_t table_id = pi_p4info_meter_get_direct(p4info, meter_id);
  // checked by PI common code
  bf_sys_assert(table_id != PI_INVALID_ID);
  uint32_t table_handle = pi_state_table_id_to_handle(dev_tgt.dev_id, table_id);

  // this pipe_mgr was designed for debugging, it is likely to be improved in
  // the future
  pipe_meter_spec_t pipe_meter_spec[4];
  pipe_status_t status = pipe_mgr_meter_read_entry(session_handle,
                                                   pipe_mgr_dev_tgt,
                                                   table_handle,
                                                   entry_handle,
                                                   pipe_meter_spec);

  if (status != PIPE_SUCCESS) return PI_STATUS_TARGET_ERROR + status;

  convert_from_pipe_meter_spec(
      p4info, meter_id, &pipe_meter_spec[0], meter_spec);

  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_meter_set_direct(pi_session_handle_t session_handle,
                                 pi_dev_tgt_t dev_tgt,
                                 pi_p4_id_t meter_id,
                                 pi_entry_handle_t entry_handle,
                                 const pi_meter_spec_t *meter_spec) {
  LOG_DBG("%s", __func__);
  const pi_p4info_t *p4info = pi_get_device_p4info(dev_tgt.dev_id);
  dev_target_t pipe_mgr_dev_tgt;
  convert_dev_tgt(dev_tgt, &pipe_mgr_dev_tgt);
  pi_p4_id_t table_id = pi_p4info_meter_get_direct(p4info, meter_id);
  // checked by PI common code
  bf_sys_assert(table_id != PI_INVALID_ID);
  uint32_t table_handle = pi_state_table_id_to_handle(dev_tgt.dev_id, table_id);

  pipe_res_spec_t res_spec;
  pipe_meter_spec_t *pipe_meter_spec = &res_spec.data.meter;
  convert_to_pipe_meter_spec(p4info, meter_id, meter_spec, pipe_meter_spec);
  uint32_t meter_handle = pi_state_res_id_to_handle(dev_tgt.dev_id, meter_id);
  res_spec.tbl_hdl = meter_handle;
  res_spec.tag = PIPE_RES_ACTION_TAG_ATTACHED;

  pipe_status_t status =
      pipe_mgr_mat_ent_set_resource(session_handle,
                                    PI_DEV_ID_TO_PIPE(dev_tgt.dev_id),
                                    table_handle,
                                    entry_handle,
                                    &res_spec,
                                    1 /* res count */,
                                    0);
  if (status != PIPE_SUCCESS) return PI_STATUS_TARGET_ERROR + status;
  return PI_STATUS_SUCCESS;
}
