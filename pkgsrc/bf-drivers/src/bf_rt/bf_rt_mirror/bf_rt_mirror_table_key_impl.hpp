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

#ifndef _BF_RT_MIRROR_TABLE_KEY_IMPL_HPP
#define _BF_RT_MIRROR_TABLE_KEY_IMPL_HPP

#include <bf_rt_common/bf_rt_table_key_impl.hpp>

namespace bfrt {

class BfRtMirrorCfgTableKey : public BfRtTableKeyObj {
 public:
  BfRtMirrorCfgTableKey(const BfRtTableObj *tbl_obj)
      : BfRtTableKeyObj(tbl_obj){};
  ~BfRtMirrorCfgTableKey() = default;

  bf_status_t setValue(const bf_rt_id_t &field_id,
                       const uint64_t &value) override final;

  bf_status_t setValue(const bf_rt_id_t &field_id,
                       const uint8_t *value,
                       const size_t &size) override final;

  bf_status_t getValue(const bf_rt_id_t &field_id,
                       uint64_t *value) const override final;

  bf_status_t getValue(const bf_rt_id_t &field_id,
                       const size_t &size,
                       uint8_t *value) const override final;

  bf_status_t reset() override final;

  const bf_mirror_id_t &getId() const { return session_id_; }

  void setId(const bf_mirror_id_t id) { session_id_ = id; }

 private:
  bf_mirror_id_t session_id_ = 0;
};

}  // namespace bfrt
#endif  //_BF_RT_MIRROR_TABLE_KEY_IMPL_HPP
