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

#ifndef _BF_RT_TABLE_STATE_HPP
#define _BF_RT_TABLE_STATE_HPP

#include "pipe_mgr/pipe_mgr_intf.h"
#include <bf_rt/bf_rt_common.h>

#include <bf_rt_common/bf_rt_table_data_impl.hpp>

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace bfrt {
// This class stores handles for GetNext_n function calls in case
// of previously returned keys were deleted from device.
class BfRtStateNextRef {
 public:
  BfRtStateNextRef(bf_rt_id_t tbl_id) : table_id(tbl_id){};

  bf_status_t setRef(const bf_rt_id_t &session,
                     const bf_dev_pipe_t &pipe_id,
                     const pipe_mat_ent_hdl_t &mat_ent_hdl);
  bf_status_t getRef(const bf_rt_id_t &session,
                     const bf_dev_pipe_t &pipe_id,
                     pipe_mat_ent_hdl_t *mat_ent_hdl) const;

 private:
  bf_rt_id_t table_id;
  mutable std::mutex state_lock;

  // Store handle for GetNext_n function call per pipe.
  // Key is build using session number in upper 16 bits,
  // and pipe number for lower 16 bits.
  // (session_id << 16 | pipe_id)
  std::unordered_map<uint32_t, pipe_mat_ent_hdl_t> next_ref_;
};

}  // namespace bfrt

#endif  // _BF_RT_TABLE_STATE_HPP
