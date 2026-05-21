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

#include <iostream>

#include <bf_rt/bf_rt_session.hpp>
#include "bf_rt_learn_state.hpp"

namespace bfrt {

void BfRtStateLearn::stateLearnSet(const BfRtLearnObj *learn_obj,
                                   const std::weak_ptr<BfRtSession> session,
                                   bfRtCbFunction callback_cpp,
                                   bf_rt_cb_function callback_c,
                                   const void *cookie) {
  std::lock_guard<std::mutex> lock(state_lock);
  learn_obj_ = learn_obj;
  callback_cpp_ = callback_cpp;
  callback_c_ = callback_c;
  session_obj_ = session;
  cookie_ = const_cast<void *>(cookie);
}

void BfRtStateLearn::stateLearnReset() {
  std::lock_guard<std::mutex> lock(state_lock);
  learn_obj_ = nullptr;
  callback_cpp_ = nullptr;
  callback_c_ = nullptr;
  session_obj_.reset();
  cookie_ = nullptr;
}

std::tuple<const BfRtLearnObj *,
           const std::weak_ptr<BfRtSession>,
           bfRtCbFunction,
           bf_rt_cb_function,
           const void *>
BfRtStateLearn::stateLearnGet() {
  std::lock_guard<std::mutex> lock(state_lock);
  return std::make_tuple(
      learn_obj_, session_obj_, callback_cpp_, callback_c_, cookie_);
}

}  // namespace bfrt
