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

#include "tdi_table_state.hpp"
#include <tdi/common/tdi_utils.hpp>

#include <algorithm>

namespace tdi {
namespace tna {
namespace tofino {
// Reference to next object storage functions
tdi_status_t StateNextRef::setRef(const tdi_id_t &session,
                                  const bf_dev_pipe_t &pipe_id,
                                  const pipe_mat_ent_hdl_t &mat_ent_hdl) {
  // Key operations assume max pipe number equal to TDI_DEV_PIPE_ALL
  uint32_t key = session << 16;
  key |= pipe_id;
  std::lock_guard<std::mutex> lock(state_lock);
  this->next_ref_[key] = mat_ent_hdl;
  return TDI_SUCCESS;
}

tdi_status_t StateNextRef::getRef(const tdi_id_t &session,
                                  const bf_dev_pipe_t &pipe_id,
                                  pipe_mat_ent_hdl_t *mat_ent_hdl) const {
  // Key operations assume max pipe number equal to TDI_DEV_PIPE_ALL
  uint32_t key = session << 16;
  key |= pipe_id;
  std::lock_guard<std::mutex> lock(state_lock);
  auto handle = this->next_ref_.find(key);
  if (handle == this->next_ref_.end()) {
    return TDI_OBJECT_NOT_FOUND;
  }
  *mat_ent_hdl = handle->second;
  return TDI_SUCCESS;
}

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
