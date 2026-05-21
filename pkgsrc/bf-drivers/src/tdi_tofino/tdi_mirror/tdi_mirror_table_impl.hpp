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

#ifndef _TDI_MIRROR_TABLE_IMPL_HPP
#define _TDI_MIRROR_TABLE_IMPL_HPP

// tdi includes
#include <tdi/common/tdi_table.hpp>

// local includes
#include "tdi_mirror_table_data_impl.hpp"
#include "tdi_mirror_table_key_impl.hpp"

namespace tdi {
namespace tna {
namespace tofino {

/* Mirror configuration table */
class MirrorCfgTable : public tdi::Table {
 public:
  MirrorCfgTable(const tdi::TdiInfo *tdi_info, const tdi::TableInfo *table_info)
      : tdi::Table(
            tdi_info,
            SupportedApis({
                {TDI_TABLE_API_TYPE_ADD, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_MODIFY, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_DELETE, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET, {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET_FIRST,
                 {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_GET_NEXT_N,
                 {"dev_id", "pipe_id", "pipe_all"}},
                {TDI_TABLE_API_TYPE_USAGE_GET,
                 {"dev_id", "pipe_id", "pipe_all"}},
            }),
            table_info) {
    LOG_DBG("Creating MirrorCfgTable table for %s",
            table_info->nameGet().c_str());
  };
  tdi_status_t entryAdd(const tdi::Session &session,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        const tdi::TableKey &key,
                        const tdi::TableData &data) const override final;

  tdi_status_t entryMod(const tdi::Session &session,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        const tdi::TableKey &key,
                        const tdi::TableData &data) const override final;

  tdi_status_t entryDel(const tdi::Session &session,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        const tdi::TableKey &key) const override final;

  tdi_status_t entryGet(const tdi::Session &session,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        const tdi::TableKey &key,
                        tdi::TableData *data) const override final;

  tdi_status_t entryGetFirst(const tdi::Session &session,
                             const tdi::Target &dev_tgt,
                             const tdi::Flags &flags,
                             tdi::TableKey *key,
                             tdi::TableData *data) const override final;

  tdi_status_t entryGetNextN(const tdi::Session &session,
                             const tdi::Target &dev_tgt,
                             const tdi::Flags &flags,
                             const tdi::TableKey &key,
                             const uint32_t &n,
                             keyDataPairs *key_data_pairs,
                             uint32_t *num_returned) const override final;

  tdi_status_t usageGet(const tdi::Session &session,
                        const tdi::Target &dev_tgt,
                        const tdi::Flags &flags,
                        uint32_t *count) const override final;

  tdi_status_t keyAllocate(
      std::unique_ptr<tdi::TableKey> *key_ret) const override final;

  tdi_status_t dataAllocate(
      std::unique_ptr<tdi::TableData> *data_ret) const override final;

  tdi_status_t dataAllocate(
      const tdi_id_t &action_id,
      std::unique_ptr<tdi::TableData> *data_ret) const override final;

  tdi_status_t dataAllocate(
      const std::vector<tdi_id_t> &fields,
      std::unique_ptr<tdi::TableData> *data_ret) const override final;

  tdi_status_t dataAllocate(
      const std::vector<tdi_id_t> &fields,
      const tdi_id_t &action_id,
      std::unique_ptr<tdi::TableData> *data_ret) const override final;

  tdi_status_t keyReset(tdi::TableKey *key) const override final;

  tdi_status_t dataReset(tdi::TableData *data) const override final;

  tdi_status_t dataReset(const tdi_id_t &action_id,
                         tdi::TableData *data) const override final;

  tdi_status_t dataReset(const std::vector<tdi_id_t> &fields,
                         const tdi_id_t &action_id,
                         tdi::TableData *data) const override final;

  // This API is more than enough to enable action APIs
  // on this table
  bool actionIdApplicable() const override { return true; };

  tdi_status_t entryGetInternal(const tdi::Session &session,
                                const dev_target_t pipe_dev_tgt,
                                const bf_mirror_session_info_t &s_info,
                                const bf_mirror_id_t &session_id,
                                const bool &session_enable,
                                tdi::TableData *data) const;

  tdi_status_t entryModInternal(const tdi::Session &session,
                                const tdi::Target &dev_tgt,
                                const tdi::TableKey &key,
                                const tdi::TableData &data) const;

 private:
  // Static maps to convert between string and enum types for
  // mirror session direction
  static const std::map<std::string, bf_mirror_direction_e>
      mirror_str_to_dir_map;
  static const std::map<bf_mirror_direction_e, std::string>
      mirror_dir_to_str_map;

  // Static maps to convert between string and enum types for
  // packet color
  static const std::map<std::string, bf_tm_color_t>
      mirror_str_to_packet_color_map;
  static const std::map<bf_tm_color_t, std::string>
      mirror_packet_color_to_str_map;
};

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
#endif  //_TDI_MIRROR_TABLE_IMPL_HPP
