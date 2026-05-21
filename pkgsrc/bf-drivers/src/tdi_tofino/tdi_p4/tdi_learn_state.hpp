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

#ifndef _TDI_LEARN_STATE_HPP
#define _TDI_LEARN_STATE_HPP

#ifdef __cplusplus
extern "C" {
#endif
#include "pipe_mgr/pipe_mgr_intf.h"
#include <tdi/tdi_common.h>
#ifdef __cplusplus
}
#endif

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <tuple>

#include <tdi/tdi_learn.h>
#include <tdi/tdi_learn.hpp>
#include "tdi_learn_impl.hpp"

namespace tdi {

class TdiStateLearn {
 public:
  TdiStateLearn(tdi_id_t id) : learn_id_(id){};

  void stateLearnSet(const TdiLearnObj *learn_obj,
                     const std::weak_ptr<TdiSession> session,
                     tdiCbFunction callback_cpp,
                     tdi_cb_function callback_c,
                     const void *cookie);
  void stateLearnReset();
  std::tuple<const TdiLearnObj *,
             const std::weak_ptr<TdiSession>,
             tdiCbFunction,
             tdi_cb_function,
             const void *>
  stateLearnGet();

 private:
  std::mutex state_lock;
  tdi_id_t learn_id_;
  const TdiLearnObj *learn_obj_;
  std::weak_ptr<TdiSession> session_obj_;
  tdiCbFunction callback_cpp_;
  tdi_cb_function callback_c_;
  void *cookie_;
};

}  // namespace tdi

#endif  // _TDI_LEARN_STATE_HPP
