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

#ifndef _TDI_MIRROR_TABLE_KEY_IMPL_HPP
#define _TDI_MIRROR_TABLE_KEY_IMPL_HPP

#include <tdi/common/tdi_table.hpp>
#include <tdi/common/tdi_table_key.hpp>
#include "bf_types/bf_types.h"

namespace tdi {
namespace tna {
namespace tofino {

class MirrorCfgTableKey : public tdi::TableKey {
 public:
  MirrorCfgTableKey(const Table *table) : TableKey(table){};
  ~MirrorCfgTableKey() = default;

  virtual tdi_status_t setValue(const tdi_id_t &field_id,
                                const tdi::KeyFieldValue &field_value) override;

  virtual tdi_status_t getValue(const tdi_id_t &field_id,
                                tdi::KeyFieldValue *value) const override;

  tdi_status_t setValue(const tdi::KeyFieldInfo *key_field,
                        const uint64_t &value);

  tdi_status_t setValue(const tdi::KeyFieldInfo *key_field,
                        const uint8_t *value,
                        const size_t &size);

  tdi_status_t getValue(const tdi::KeyFieldInfo *key_field,
                        uint64_t *value) const;

  tdi_status_t getValue(const tdi::KeyFieldInfo *key_field,
                        const size_t &size,
                        uint8_t *value) const;

  tdi_status_t reset() override final;

  const bf_mirror_id_t &getId() const { return session_id_; }

  void setId(const bf_mirror_id_t id) { session_id_ = id; }

 private:
  bf_mirror_id_t session_id_ = 0;
};

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
#endif  //_TDI_MIRROR_TABLE_KEY_IMPL_HPP
