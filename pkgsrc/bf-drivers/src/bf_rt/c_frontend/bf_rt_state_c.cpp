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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <bf_rt/bf_rt_learn.h>

#ifdef __cplusplus
}
#endif

#include <bf_rt/bf_rt_session.hpp>
#include "bf_rt_state_c.hpp"

namespace bfrt {
namespace bfrt_c {

BfRtCFrontEndSessionState &BfRtCFrontEndSessionState::getInstance() {
  static BfRtCFrontEndSessionState instance;
  return instance;
}

std::shared_ptr<BfRtSession> BfRtCFrontEndSessionState::getSharedPtr(
    const BfRtSession *session_raw) {
  if (session_raw == nullptr) {
    return nullptr;
  }
  std::lock_guard<std::mutex> lock(state_lock);
  if (sessionStateMap.find(session_raw) != sessionStateMap.end()) {
    return sessionStateMap.at(session_raw);
  }
  return nullptr;
}

void BfRtCFrontEndSessionState::insertShared(
    std::shared_ptr<BfRtSession> session) {
  std::lock_guard<std::mutex> lock(state_lock);
  if (sessionStateMap.find(session.get()) != sessionStateMap.end()) {
    return;
  }
  sessionStateMap[session.get()] = session;
}

void BfRtCFrontEndSessionState::removeShared(const BfRtSession *session) {
  std::lock_guard<std::mutex> lock(state_lock);
  if (sessionStateMap.find(session) == sessionStateMap.end()) {
    return;
  }
  sessionStateMap.erase(sessionStateMap.find(session));
}

}  // namespace bfrt_c
}  // namespace bfrt
