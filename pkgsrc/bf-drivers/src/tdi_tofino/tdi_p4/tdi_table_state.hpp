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

#ifndef _TDI_TABLE_STATE_HPP
#define _TDI_TABLE_STATE_HPP

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <tdi/common/tdi_defs.h>

#include <pipe_mgr/pipe_mgr_intf.h>
#include "tdi_p4_table_data_impl.hpp"

namespace tdi {
namespace tna {
namespace tofino {
// This class stores handles for GetNext_n function calls in case
// of previously returned keys were deleted from device.
class StateNextRef {
 public:
  StateNextRef(tdi_id_t tbl_id) : table_id(tbl_id){};

  tdi_status_t setRef(const tdi_id_t &session,
                      const bf_dev_pipe_t &pipe_id,
                      const pipe_mat_ent_hdl_t &mat_ent_hdl);
  tdi_status_t getRef(const tdi_id_t &session,
                      const bf_dev_pipe_t &pipe_id,
                      pipe_mat_ent_hdl_t *mat_ent_hdl) const;

 private:
  tdi_id_t table_id;
  mutable std::mutex state_lock;

  // Store handle for GetNext_n function call per pipe.
  // Key is build using session number in upper 16 bits,
  // and pipe number for lower 16 bits.
  // (session_id << 16 | pipe_id)
  std::unordered_map<uint32_t, pipe_mat_ent_hdl_t> next_ref_;
};

}  // namespace tofino
}  // namespace tna
}  // namespace tdi

#endif  // _TDI_TABLE_STATE_HPP
