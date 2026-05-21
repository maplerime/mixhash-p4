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

#ifndef _TDI_MIRROR_TABLE_DATA_IMPL_HPP
#define _TDI_MIRROR_TABLE_DATA_IMPL_HPP

#include <tdi/common/tdi_table.hpp>
#include <tdi/common/tdi_table_data.hpp>

#define TDI_MIRROR_INT_HDR_SIZE 16
namespace tdi {
namespace tna {
namespace tofino {

class MirrorCfgTableData : public tdi::TableData {
 public:
  MirrorCfgTableData(const tdi::Table *table,
                     const tdi_id_t &act_id,
                     const std::vector<tdi_id_t> &fields)
      : tdi::TableData(table, act_id, fields) {}

  ~MirrorCfgTableData() = default;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const bool &value) override final;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const uint64_t &value) override final;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const uint8_t *value,
                        const size_t &size) override final;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const std::string &str) override final;

  tdi_status_t getValue(const tdi_id_t &field_id,
                        bool *value) const override final;

  tdi_status_t getValue(const tdi_id_t &field_id,
                        uint64_t *value) const override final;

  tdi_status_t getValue(const tdi_id_t &field_id,
                        const size_t &size,
                        uint8_t *value) const override final;

  tdi_status_t getValue(const tdi_id_t &field_id,
                        std::string *str) const override final;

  tdi_status_t resetDerived() override final;

  const std::unordered_map<tdi_id_t, bool> &getBoolFieldDataMap() const {
    return boolFieldData;
  }
  const std::unordered_map<tdi_id_t, uint64_t> &getU64FieldDataMap() const {
    return u64FieldData;
  }

  const std::unordered_map<tdi_id_t, std::string> &getStrFieldDataMap() const {
    return strFieldData;
  }

  const std::unordered_map<tdi_id_t,
                           std::array<uint8_t, TDI_MIRROR_INT_HDR_SIZE>>
      &getArrayFieldDataMap() const {
    return arrayFieldData;
  }

 private:
  tdi_status_t setValueInternal(const tdi_id_t &field_id,
                                const uint64_t &value,
                                const uint8_t *value_ptr,
                                const size_t &s);
  tdi_status_t getValueInternal(const tdi_id_t &field_id,
                                uint64_t *value,
                                uint8_t *value_ptr,
                                const size_t &s) const;
  std::unordered_map<tdi_id_t, uint64_t> u64FieldData;
  std::unordered_map<tdi_id_t, bool> boolFieldData;
  std::unordered_map<tdi_id_t, std::string> strFieldData;
  std::unordered_map<tdi_id_t, std::array<uint8_t, TDI_MIRROR_INT_HDR_SIZE>>
      arrayFieldData;
};

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
#endif  // _TDI_MIRROR_TABLE_DATA_IMPL_HPP
