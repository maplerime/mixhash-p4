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

#ifndef _TDI_PORT_TABLE_IMPL_HPP
#define _TDI_PORT_TABLE_IMPL_HPP

#include <tdi/common/tdi_table.hpp>
#include "tdi_port_table_data_impl.hpp"
#include "tdi_port_table_key_impl.hpp"
#include "tdi_port_mgr_intf.hpp"
#include <tdi_common/tdi_pipe_mgr_intf.hpp>

namespace tdi {

#define CHECK_AND_RETURN_HW_FLAG(flags)                                \
  do {                                                                 \
    bool read_from_hw = false;                                         \
    flags.getValue(static_cast<tdi_flags_e>(TDI_TOFINO_FLAGS_FROM_HW), \
                   &read_from_hw);                                     \
    if (read_from_hw) {                                                \
      LOG_TRACE("%s:%d %s ERROR : Read from hardware not supported",   \
                __func__,                                              \
                __LINE__,                                              \
                tableInfoGet()->nameGet().c_str());                    \
      return TDI_NOT_SUPPORTED;                                        \
    }                                                                  \
  } while (0)

class TdiInfo;
/*
 * PortCfgTable
 * PortStatTable
 * PortHdlInfoTable
 * PortFpIdxInfoTable
 * PortStrInfoTable
 */

class PortCfgTable : public tdi::Table {
 public:
  PortCfgTable(const tdi::TdiInfo *tdi_info, const tdi::TableInfo *table_info)
      : tdi::Table(
            tdi_info,
            SupportedApis({
                {TDI_TABLE_API_TYPE_ADD, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_MODIFY, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_DELETE, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_CLEAR, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET_FIRST,
                 {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET_NEXT_N,
                 {"dev_id", "pipe_id", "pipe_all"}},
            }),
            table_info) {
    mapInit();
    LOG_DBG("Creating PortCfgTable table for %s",
            table_info->nameGet().c_str());
  };

  tdi_status_t entryAdd(const Session & /*session*/,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        const TableData &data) const override;

  tdi_status_t entryMod(const Session & /*session*/,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        const TableData &data) const override;

  tdi_status_t entryDel(const tdi::Session & /*session*/,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        const tdi::TableKey &key) const override;

  tdi_status_t clear(const tdi::Session & /*session*/,
                     const tdi::Target &dev_tgt,
                     const tdi::Flags &flags) const override;

  tdi_status_t entryGet(const Session & /*session*/,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        TableData *data) const override;

  tdi_status_t entryGetFirst(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             TableKey *key,
                             TableData *data) const override;

  tdi_status_t entryGetNextN(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             const TableKey &key,
                             const uint32_t &n,
                             keyDataPairs *key_data_pairs,
                             uint32_t *num_returned) const override;

  tdi_status_t usageGet(const Session & /*session*/,
                        const Target &dev_tgt,
                        const Flags &flags,
                        uint32_t *count) const override;

  tdi_status_t keyAllocate(std::unique_ptr<TableKey> *key_ret) const override;

  tdi_status_t dataAllocate(
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t dataAllocate(
      const std::vector<tdi_id_t> &fields,
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t keyReset(TableKey *key) const override final;

  tdi_status_t dataReset(TableData *data) const override final;

  tdi_status_t dataReset(const std::vector<tdi_id_t> &fields,
                         TableData *data) const override final;

  // Attribute APIs
  tdi_status_t attributeAllocate(
      const tdi_attributes_type_e &type,
      std::unique_ptr<tdi::TableAttributes> *attr) const override;
#if 0
  tdi_status_t attributeReset(
      const TableAttributesType &type,
      std::unique_ptr<BfRtTableAttributes> *attr) const override;
#endif
  tdi_status_t tableAttributesSet(
      const tdi::Session & /*session*/,
      const tdi::Target &dev_tgt,
      const tdi::Flags &flags,
      const tdi::TableAttributes &tableAttributes) const override;

 private:
  tdi_status_t EntryMod_internal(const Session &session,
                                 const Target &dev_tgt,
                                 const uint32_t &dev_port,
                                 const TableData &data) const;
  tdi_status_t EntryGet_internal(const Session &session,
                                 const Target &dev_tgt,
                                 const uint32_t &dev_port,
                                 TableData *data) const;

  void mapInit();
  std::map<std::string, bf_port_speed_t> speedMap;
  std::map<std::string, bf_fec_type_t> fecMap;
  std::map<std::string, bf_media_type_t> mediaTypeMap;
  std::map<std::string, bf_pm_port_dir_e> portDirMap;
  std::map<std::string, bf_loopback_mode_e> loopbackModeMap;
  std::map<std::string, bf_pm_port_autoneg_policy_e> autonegoPolicyMap;
};

class RecircPortCfgTable : public tdi::Table {
 public:
  RecircPortCfgTable(const tdi::TdiInfo *tdi_info,
                     const tdi::TableInfo *table_info)
      : tdi::Table(tdi_info,
                   SupportedApis({
                       {TDI_TABLE_API_TYPE_ADD, {"dev_id"}},
                       {TDI_TABLE_API_TYPE_DELETE, {"dev_id"}},
                       {TDI_TABLE_API_TYPE_GET, {"dev_id"}},
                   }),
                   table_info) {
    mapInit();
    LOG_DBG("Creating RecircPortCfgTable table for %s",
            table_info->nameGet().c_str());
  };

  tdi_status_t entryAdd(const Session & /*session*/,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        const TableData &data) const override;

  tdi_status_t entryDel(const tdi::Session & /*session*/,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        const tdi::TableKey &key) const override;

  tdi_status_t entryGet(const Session & /*session*/,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        TableData *data) const override;

  tdi_status_t keyAllocate(std::unique_ptr<TableKey> *key_ret) const override;

  tdi_status_t dataAllocate(
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t dataAllocate(
      const std::vector<tdi_id_t> &fields,
      std::unique_ptr<TableData> *data_ret) const override;

 private:
  tdi_status_t EntryGet_internal(const Session &session,
                                 const Target &dev_tgt,
                                 const uint32_t &dev_port,
                                 TableData *data) const;

  std::string getStrFromSpeed(const bf_port_speed_t &speed) const {
    for (const auto &kv : speedMap) {
      if (kv.second == speed) {
        return kv.first;
      }
    }
    return "UNKNOWN";
  }

  void mapInit();
  std::map<std::string, bf_port_speed_t> speedMap;
};

class PortStatTable : public tdi::Table {
 public:
  PortStatTable(const tdi::TdiInfo *tdi_info, const tdi::TableInfo *table_info)
      : tdi::Table(
            tdi_info,
            tdi::SupportedApis({
                {TDI_TABLE_API_TYPE_GET, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_MODIFY, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET_FIRST,
                 {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET_NEXT_N,
                 {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_CLEAR, {"dev_id", "pipe_id", "pipe_all"}},
            }),
            table_info){};

  tdi_status_t clear(const Session & /*session*/,
                     const Target &dev_tgt,
                     const Flags &flags) const override;

  tdi_status_t entryMod(const Session &session,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        const TableData &data) const override;

  tdi_status_t entryGet(const Session &session,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        TableData *data) const override;

  tdi_status_t entryGetFirst(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             TableKey *key,
                             TableData *data) const override;

  tdi_status_t entryGetNextN(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             const TableKey &key,
                             const uint32_t &n,
                             keyDataPairs *key_data_pairs,
                             uint32_t *num_returned) const override;
  tdi_status_t keyAllocate(std::unique_ptr<TableKey> *key_ret) const override;

  tdi_status_t dataAllocate(
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t dataAllocate(
      const std::vector<tdi_id_t> &fields,
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t keyReset(TableKey *key) const override final;

  tdi_status_t dataReset(TableData *data) const override final;

  tdi_status_t dataReset(const std::vector<tdi_id_t> &fields,
                         TableData *data) const override final;

  // Attribute APIs
  tdi_status_t attributeAllocate(
      const tdi_attributes_type_e &type,
      std::unique_ptr<tdi::TableAttributes> *attr) const override;
#if 0
  tdi_status_t attributeReset(
      const TableAttributesType &type,
      std::unique_ptr<BfRtTableAttributes> *attr) const override;
#endif
  tdi_status_t tableAttributesSet(
      const Session &session,
      const Target &dev_tgt,
      const Flags &flags,
      const tdi::TableAttributes &tableAttributes) const override;
  tdi_status_t tableAttributesGet(
      const Session &session,
      const Target &dev_tgt,
      const Flags &flags,
      tdi::TableAttributes *tableAttributes) const override;

 private:
  tdi_status_t EntryGet_internal(const Target &dev_tgt,
                                 const Flags &flags,
                                 const uint32_t &dev_port,
                                 PortStatTableData *data) const;
};

class PortStrInfoTable : public tdi::Table {
 public:
  PortStrInfoTable(const tdi::TdiInfo *tdi_info,
                   const tdi::TableInfo *table_info)
      : tdi::Table(tdi_info,
                   tdi::SupportedApis({{TDI_TABLE_API_TYPE_GET,
                                        {"dev_id", "pipe_id", "pipe_all"}},
                                       {TDI_TABLE_API_TYPE_GET_FIRST,
                                        {"dev_id", "pipe_id", "pipe_all"}},
                                       {TDI_TABLE_API_TYPE_GET_NEXT_N,
                                        {"dev_id", "pipe_id", "pipe_all"}}}),
                   table_info){};

  tdi_status_t entryGet(const Session &session,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        TableData *data) const override;
  tdi_status_t entryGetFirst(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             TableKey *key,
                             TableData *data) const override;
  tdi_status_t entryGetNextN(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             const TableKey &key,
                             const uint32_t &n,
                             keyDataPairs *key_data_pairs,
                             uint32_t *num_returned) const override;
  tdi_status_t keyAllocate(std::unique_ptr<TableKey> *key_ret) const override;

  tdi_status_t dataAllocate(
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t keyReset(TableKey *key) const override final;

  tdi_status_t dataReset(TableData *data) const override final;
};

class PortHdlInfoTable : public tdi::Table {
 public:
  PortHdlInfoTable(const tdi::TdiInfo *tdi_info,
                   const tdi::TableInfo *table_info)
      : tdi::Table(tdi_info,
                   tdi::SupportedApis({{TDI_TABLE_API_TYPE_GET,
                                        {"dev_id", "pipe_id", "pipe_all"}},
                                       {TDI_TABLE_API_TYPE_GET_FIRST,
                                        {"dev_id", "pipe_id", "pipe_all"}},
                                       {TDI_TABLE_API_TYPE_GET_NEXT_N,
                                        {"dev_id", "pipe_id", "pipe_all"}}}),
                   table_info){};

  tdi_status_t entryGet(const Session &session,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        TableData *data) const override;
  tdi_status_t entryGetFirst(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             TableKey *key,
                             TableData *data) const override;
  tdi_status_t entryGetNextN(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             const TableKey &key,
                             const uint32_t &n,
                             keyDataPairs *key_data_pairs,
                             uint32_t *num_returned) const override;
  tdi_status_t keyAllocate(std::unique_ptr<TableKey> *key_ret) const override;

  tdi_status_t dataAllocate(
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t keyReset(TableKey *key) const override final;

  tdi_status_t dataReset(TableData *data) const override final;
};

class PortFpIdxInfoTable : public Table {
 public:
  PortFpIdxInfoTable(const tdi::TdiInfo *tdi_info,
                     const tdi::TableInfo *table_info)
      : tdi::Table(tdi_info,
                   tdi::SupportedApis({{TDI_TABLE_API_TYPE_GET,
                                        {"dev_id", "pipe_id", "pipe_all"}},
                                       {TDI_TABLE_API_TYPE_GET_FIRST,
                                        {"dev_id", "pipe_id", "pipe_all"}},
                                       {TDI_TABLE_API_TYPE_GET_NEXT_N,
                                        {"dev_id", "pipe_id", "pipe_all"}}}),
                   table_info){};

  tdi_status_t entryGet(const Session &session,
                        const Target &dev_tgt,
                        const Flags &flags,
                        const TableKey &key,
                        TableData *data) const override;
  tdi_status_t entryGetFirst(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             TableKey *key,
                             TableData *data) const override;
  tdi_status_t entryGetNextN(const Session & /*session*/,
                             const Target &dev_tgt,
                             const Flags &flags,
                             const TableKey &key,
                             const uint32_t &n,
                             keyDataPairs *key_data_pairs,
                             uint32_t *num_returned) const override;
  tdi_status_t keyAllocate(std::unique_ptr<TableKey> *key_ret) const override;

  tdi_status_t dataAllocate(
      std::unique_ptr<TableData> *data_ret) const override;

  tdi_status_t keyReset(TableKey *key) const override final;

  tdi_status_t dataReset(TableData *data) const override final;
};

}  // namespace tdi

#endif  // _TDI_PORT_TABLE_IMPL_HPP
