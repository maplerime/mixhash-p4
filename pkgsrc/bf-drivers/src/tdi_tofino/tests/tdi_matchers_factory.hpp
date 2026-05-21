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
#ifndef _TDI_MATCHERS_FACTORY_HPP_
#define _TDI_MATCHERS_FACTORY_HPP_

#include "tdi_matchers.hpp"

namespace tdi {
namespace tdi_test {

using ::testing::MakeMatcher;
using ::testing::Matcher;

class TdiMatchersFactory {
 public:
  static inline Matcher<tdi_status_t> makeIsSuccessMatcher() {
    return MakeMatcher(new IsSuccessMatcher());
  }

  static inline Matcher<pipe_tbl_match_spec_t *> makeMatchSpecMatcher(
      const pipe_tbl_match_spec_t *t) {
    return MakeMatcher(new MatchSpecMatcher(t));
  }

  template <class T>
  static inline Matcher<T> makeActionSpecMatcher(T t) {
    return MakeMatcher(new ActionSpecMatcher<T>(t));
  }

  static inline Matcher<pipe_stat_data_t *> makeCounterSpecMatcher(
      pipe_stat_data_t *t) {
    return MakeMatcher(new CounterSpecMatcher(t));
  }

  static inline Matcher<pipe_res_spec_t *> makeResourceSpecArrayMatcher(
      pipe_res_spec_t *t, const int &resource_count) {
    // FIXME
    return MakeMatcher(new ResourceSpecArrayMatcher(t, resource_count));
  }

  static inline Matcher<pipe_adt_ent_hdl_t *> makeGroupMbrIdMatcher(
      const std::vector<pipe_adt_ent_hdl_t> &t) {
    return MakeMatcher(new GroupMbrIdsMatcher(t));
  }

  static inline Matcher<bool *> makeGroupMbrStsMatcher(
      const std::vector<bool> &t) {
    return MakeMatcher(new GroupMbrStsMatcher(t));
  }
};

// Explicitly instantiate the above function template
template Matcher<const pipe_action_spec_t *>
TdiMatchersFactory::makeActionSpecMatcher<const pipe_action_spec_t *>(
    const pipe_action_spec_t *);

template Matcher<pipe_action_spec_t *>
TdiMatchersFactory::makeActionSpecMatcher<pipe_action_spec_t *>(
    pipe_action_spec_t *);

}  // namespace tdi_test
}  // namespace tdi

#endif  // _TDI_MATCHERS_FACTORY_HPP_
