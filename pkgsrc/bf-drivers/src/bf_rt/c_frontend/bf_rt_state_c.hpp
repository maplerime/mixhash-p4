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

#ifndef _BF_RT_STATE_C_HPP
#define _BF_RT_STATE_C_HPP

#include <mutex>
#include <unordered_map>

namespace bfrt {
namespace bfrt_c {

class BfRtCFrontEndSessionState {
 public:
  // To get the singleton instance. Threadsafe
  static BfRtCFrontEndSessionState &getInstance();

  // Get the shared_ptr from the raw pointer
  std::shared_ptr<BfRtSession> getSharedPtr(const BfRtSession *session_raw);

  // Insert shared_ptr in the state
  void insertShared(std::shared_ptr<BfRtSession> session);
  // Delete an entry from the raw ptr
  void removeShared(const BfRtSession *session_raw);
  BfRtCFrontEndSessionState(BfRtCFrontEndSessionState const &) = delete;
  void operator=(BfRtCFrontEndSessionState const &) = delete;

 private:
  BfRtCFrontEndSessionState() {}
  std::mutex state_lock;
  std::map<const BfRtSession *, std::shared_ptr<BfRtSession> > sessionStateMap;
};

}  // namespace bfrt_c
}  // namespace bfrt

#endif  // _BF_RT_STATE_C_HPP
