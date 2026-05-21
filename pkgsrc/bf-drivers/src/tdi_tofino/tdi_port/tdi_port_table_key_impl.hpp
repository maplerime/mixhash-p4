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

#ifndef _TDI_PORT_TABLE_KEY_IMPL_HPP
#define _TDI_PORT_TABLE_KEY_IMPL_HPP

#include <tdi/common/tdi_table_key.hpp>
#include <tdi/common/tdi_table.hpp>

namespace tdi {

class PortCfgTableKey : public TableKey {
 public:
  PortCfgTableKey(const Table *tbl_obj) : TableKey(tbl_obj), dev_port_(){};

  ~PortCfgTableKey() = default;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const tdi::KeyFieldValue &field_value) override final;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const uint8_t *value,
                        const size_t &size);

  tdi_status_t getValue(const tdi_id_t &field_id,
                        tdi::KeyFieldValue *field_value) const override final;

  tdi_status_t getValue(const tdi_id_t &field_id,
                        const size_t &size,
                        uint8_t *value) const;

  const uint32_t &getId() const { return dev_port_; }

  void setId(const uint32_t id) { dev_port_ = id; }

  tdi_status_t reset() override final;

 private:
  uint32_t dev_port_;
};

class PortStatTableKey : public tdi::TableKey {
 public:
  PortStatTableKey(const tdi::Table *tbl_obj)
      : tdi::TableKey(tbl_obj), dev_port_(){};

  ~PortStatTableKey() = default;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const tdi::KeyFieldValue &field_value) override final;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const uint8_t *value,
                        const size_t &size);

  tdi_status_t getValue(const tdi_id_t &field_id,
                        KeyFieldValue *field_value) const override final;

  tdi_status_t getValue(const tdi_id_t &field_id, uint64_t *value) const;

  tdi_status_t getValue(const tdi_id_t &field_id,
                        const size_t &size,
                        uint8_t *value) const;

  const uint32_t &getId() const { return dev_port_; }

  void setId(const uint32_t id) { dev_port_ = id; }

  tdi_status_t reset() override final;

 private:
  uint32_t dev_port_;
};

class PortStrInfoTableKey : public tdi::TableKey {
 public:
  PortStrInfoTableKey(const tdi::Table *tbl_obj) : tdi::TableKey(tbl_obj){};
  ~PortStrInfoTableKey() = default;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const tdi::KeyFieldValue &field_value) override final;

  // tdi_status_t setValue(const tdi_id_t &field_id, const std::string &value);

  tdi_status_t getValue(const tdi_id_t &field_id,
                        KeyFieldValue *field_value) const override final;

  // tdi_status_t getValue(const tdi_id_t &field_id, std::string *value) const;

  const std::string &getPortStr() const { return port_str_; };

  void setPortStr(const std::string &name_str) { port_str_ = name_str; };

 private:
  std::string port_str_;
};

class PortHdlInfoTableKey : public tdi::TableKey {
 public:
  PortHdlInfoTableKey(const tdi::Table *tbl_obj)
      : tdi::TableKey(tbl_obj), conn_id_(), chnl_id_(){};

  ~PortHdlInfoTableKey() = default;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const tdi::KeyFieldValue &field_value) override final;

  tdi_status_t setValue(const tdi::KeyFieldInfo *key_field,
                        const uint64_t &value);

  tdi_status_t setValue(const tdi::KeyFieldInfo *key_field,
                        const uint8_t *value,
                        const size_t &size);

  tdi_status_t getValue(const tdi_id_t &field_id,
                        KeyFieldValue *field_value) const override final;

  tdi_status_t getValue(const tdi::KeyFieldInfo *key_field,
                        uint64_t *value) const;

  tdi_status_t getValue(const tdi::KeyFieldInfo *key_field,
                        const size_t &size,
                        uint8_t *value) const;

  tdi_status_t getPortHdl(uint32_t *conn_id, uint32_t *chnl_id) const;

  tdi_status_t reset() override final;

  void setPortHdl(const uint32_t conn_id, const uint32_t chnl_id) {
    conn_id_ = conn_id;
    chnl_id_ = chnl_id;
  }

 private:
  uint32_t conn_id_;
  uint32_t chnl_id_;
};

class PortFpIdxInfoTableKey : public tdi::TableKey {
 public:
  PortFpIdxInfoTableKey(const tdi::Table *tbl_obj)
      : tdi::TableKey(tbl_obj), fp_idx_(){};
  ~PortFpIdxInfoTableKey() = default;

  tdi_status_t setValue(const tdi_id_t &field_id,
                        const tdi::KeyFieldValue &field_value) override final;

  tdi_status_t setValue(const tdi::KeyFieldInfo *key_field,
                        const uint64_t &value);

  tdi_status_t setValue(const tdi::KeyFieldInfo *key_field,
                        const uint8_t *value,
                        const size_t &size);

  tdi_status_t getValue(const tdi_id_t &field_id,
                        KeyFieldValue *field_value) const override final;

  tdi_status_t getValue(const tdi::KeyFieldInfo *key_field,
                        uint64_t *value) const;

  tdi_status_t getValue(const tdi::KeyFieldInfo *key_field,
                        const size_t &size,
                        uint8_t *value) const;

  const uint32_t &getId() const { return fp_idx_; }

  void setId(const uint32_t idx) { fp_idx_ = idx; }

  tdi_status_t reset() override final;

 private:
  uint32_t fp_idx_;
};

}  // namespace tdi
#endif  // _BF_RT_PORT_TABLE_KEY_IMPL_HPP
