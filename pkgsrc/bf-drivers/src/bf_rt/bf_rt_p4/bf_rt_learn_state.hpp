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

#ifndef _BF_RT_LEARN_STATE_HPP
#define _BF_RT_LEARN_STATE_HPP

#ifdef __cplusplus
extern "C" {
#endif
#include "pipe_mgr/pipe_mgr_intf.h"
#include <bf_rt/bf_rt_common.h>
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

#include <bf_rt/bf_rt_learn.h>
#include <bf_rt/bf_rt_learn.hpp>
#include "bf_rt_learn_impl.hpp"

namespace bfrt {

class BfRtStateLearn {
 public:
  BfRtStateLearn(bf_rt_id_t id) : learn_id_(id){};

  void stateLearnSet(const BfRtLearnObj *learn_obj,
                     const std::weak_ptr<BfRtSession> session,
                     bfRtCbFunction callback_cpp,
                     bf_rt_cb_function callback_c,
                     const void *cookie);
  void stateLearnReset();
  std::tuple<const BfRtLearnObj *,
             const std::weak_ptr<BfRtSession>,
             bfRtCbFunction,
             bf_rt_cb_function,
             const void *>
  stateLearnGet();

 private:
  std::mutex state_lock;
  bf_rt_id_t learn_id_ = 0;
  const BfRtLearnObj *learn_obj_ = nullptr;
  std::weak_ptr<BfRtSession> session_obj_;
  bfRtCbFunction callback_cpp_ = nullptr;
  bf_rt_cb_function callback_c_ = nullptr;
  void *cookie_ = nullptr;
};

}  // namespace bfrt

#endif  // _BF_RT_LEARN_STATE_HPP
