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

#ifndef _BF_RT_PKTGEN_TABLE_KEY_IMPL_HPP
#define _BF_RT_PKTGEN_TABLE_KEY_IMPL_HPP

#include <bf_rt_common/bf_rt_table_key_impl.hpp>

namespace bfrt {
class BfRtPktgenPortTableKey : public BfRtTableKeyObj {
 public:
  BfRtPktgenPortTableKey(const BfRtTableObj *tbl_obj)
      : BfRtTableKeyObj(tbl_obj){};
  ~BfRtPktgenPortTableKey() = default;

  bf_status_t setValue(const bf_rt_id_t &field_id, const uint64_t &value);

  bf_status_t setValue(const bf_rt_id_t &field_id,
                       const uint8_t *value,
                       const size_t &size);

  bf_status_t getValue(const bf_rt_id_t &field_id, uint64_t *value) const;

  bf_status_t getValue(const bf_rt_id_t &field_id,
                       const size_t &size,
                       uint8_t *value) const;

  const uint32_t &getId() const { return dev_port_; }

  void setId(const uint32_t id) { dev_port_ = id; }

 private:
  uint32_t dev_port_ = 0;
};

class BfRtPktgenAppTableKey : public BfRtTableKeyObj {
 public:
  BfRtPktgenAppTableKey(const BfRtTableObj *tbl_obj)
      : BfRtTableKeyObj(tbl_obj){};
  ~BfRtPktgenAppTableKey() = default;

  bf_status_t setValue(const bf_rt_id_t &field_id, const uint64_t &value);

  bf_status_t setValue(const bf_rt_id_t &field_id,
                       const uint8_t *value,
                       const size_t &size);

  bf_status_t getValue(const bf_rt_id_t &field_id, uint64_t *value) const;

  bf_status_t getValue(const bf_rt_id_t &field_id,
                       const size_t &size,
                       uint8_t *value) const;

  const uint32_t &getId() const { return app_id_; }

  void setId(const uint32_t id) { app_id_ = id; }

 private:
  uint32_t app_id_ = 0xffff;
};

class BfRtPktgenPortMaskTableKey : public BfRtTableKeyObj {
 public:
  BfRtPktgenPortMaskTableKey(const BfRtTableObj *tbl_obj)
      : BfRtTableKeyObj(tbl_obj){};
  ~BfRtPktgenPortMaskTableKey() = default;

  bf_status_t setValue(const bf_rt_id_t &field_id, const uint64_t &value);

  bf_status_t setValue(const bf_rt_id_t &field_id,
                       const uint8_t *value,
                       const size_t &size);

  bf_status_t getValue(const bf_rt_id_t &field_id, uint64_t *value) const;

  bf_status_t getValue(const bf_rt_id_t &field_id,
                       const size_t &size,
                       uint8_t *value) const;

  const uint32_t &getId() const { return port_mask_sel_; }

  void setId(const uint32_t id) { port_mask_sel_ = id; }

 private:
  uint32_t port_mask_sel_ = 2;
};

class BfRtPktgenPktBufferTableKey : public BfRtTableKeyObj {
 public:
  BfRtPktgenPktBufferTableKey(const BfRtTableObj *tbl_obj)
      : BfRtTableKeyObj(tbl_obj){};
  ~BfRtPktgenPktBufferTableKey() = default;

  bf_status_t setValue(const bf_rt_id_t &field_id, const uint64_t &value);

  bf_status_t setValue(const bf_rt_id_t &field_id,
                       const uint8_t *value,
                       const size_t &size);

  bf_status_t getValue(const bf_rt_id_t &field_id, uint64_t *value) const;

  bf_status_t getValue(const bf_rt_id_t &field_id,
                       const size_t &size,
                       uint8_t *value) const;

  const uint32_t &getId() const { return byte_buf_offset_; }
  const uint32_t &getBuffSize() const { return byte_buf_size_; }
  void setId(const uint32_t id) { byte_buf_offset_ = id; }
  void setBuffSize(const uint32_t size) { byte_buf_size_ = size; }

 private:
  uint32_t byte_buf_offset_ = 0xffff;
  uint32_t byte_buf_size_ = 0xffff;
};

}  // namespace bfrt
#endif  //_BF_RT_PKTGEN_TABLE_KEY_IMPL_HPP
