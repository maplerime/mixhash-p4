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

#ifndef _TDI_MATCHERS_HPP_
#define _TDI_MATCHERS_HPP_

#include <gmock/gmock.h>

#include <iostream>

namespace tdi {
namespace tdi_test {

using ::testing::MakeMatcher;
using ::testing::Matcher;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;

class IsSuccessMatcher : public MatcherInterface<tdi_status_t> {
 public:
  // This function compares the expected status with the actual one
  bool MatchAndExplain(tdi_status_t status,
                       MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;
};

class MatchSpecMatcher : public MatcherInterface<pipe_tbl_match_spec_t *> {
 public:
  MatchSpecMatcher(const pipe_tbl_match_spec_t *ms) : correct_ms(ms) {}

  // This function compares the expected match spec with the actual one
  bool MatchAndExplain(pipe_tbl_match_spec_t *ms,
                       MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;

 private:
  const pipe_tbl_match_spec_t *correct_ms{nullptr};
};

template <class T>
class ActionSpecMatcher : public MatcherInterface<T> {
 public:
  ActionSpecMatcher(T as) : correct_as(as) {}

  // This function compares the expected action spec with the actual one
  bool MatchAndExplain(T as, MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;

 private:
  T correct_as{0};
};

// Explicityle instantiate the above templates
template class ActionSpecMatcher<const pipe_action_spec_t *>;
template class ActionSpecMatcher<pipe_action_spec_t *>;

class CounterSpecMatcher : public MatcherInterface<pipe_stat_data_t *> {
 public:
  CounterSpecMatcher(pipe_stat_data_t *cs) : correct_cs(cs){};

  // This function compares the expeced counter spec with the actual one
  bool MatchAndExplain(pipe_stat_data_t *cs,
                       MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;

 private:
  pipe_stat_data_t *correct_cs{nullptr};
};

class ResourceSpecArrayMatcher : public MatcherInterface<pipe_res_spec_t *> {
 public:
  ResourceSpecArrayMatcher(pipe_res_spec_t *rs, const int &resource_count)
      : correct_rs(rs), correct_count(resource_count){};

  // This function compares the expeced resource spec array with the actual one
  bool MatchAndExplain(pipe_res_spec_t *rs,
                       MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;

 private:
  // FIXME should we be saving the ground truth resource count and only verify
  // the resources for that count OR should I verify the entire array of
  // resources? There's a risk of false negatives with verifying the entire
  // array beyond the resource count as the default initializations might
  // vary between the pipe mgr and the unit test ground truth generation logic
  pipe_res_spec_t *correct_rs{nullptr};
  const int correct_count{0};
};

class GroupMbrIdsMatcher : public MatcherInterface<pipe_adt_ent_hdl_t *> {
 public:
  GroupMbrIdsMatcher(const std::vector<pipe_adt_ent_hdl_t> &ent_hdl_vec)
      : correct_ent_hdl_vec(ent_hdl_vec) {}

  bool MatchAndExplain(pipe_adt_ent_hdl_t *ent_hdls,
                       MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;

 private:
  std::vector<pipe_adt_ent_hdl_t> correct_ent_hdl_vec;
};

class GroupMbrStsMatcher : public MatcherInterface<bool *> {
 public:
  GroupMbrStsMatcher(const std::vector<bool> &ent_hdl_sts_vec)
      : correct_ent_hdl_sts_vec(ent_hdl_sts_vec) {}

  bool MatchAndExplain(bool *ent_hdl_sts,
                       MatchResultListener *listener) const override;

  void DescribeTo(std::ostream *os) const override;
  void DescribeNegationTo(std::ostream *os) const override;

 private:
  std::vector<bool> correct_ent_hdl_sts_vec;
};

}  // namespace tdi_test
}  // namespace tdi
#endif  // _TDI_MATCHERS_HPP_
