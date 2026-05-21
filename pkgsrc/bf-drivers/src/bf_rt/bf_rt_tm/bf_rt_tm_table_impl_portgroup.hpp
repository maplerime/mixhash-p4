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

#ifndef _BF_RT_TM_TABLE_IMPL_PORTGROUP_HPP
#define _BF_RT_TM_TABLE_IMPL_PORTGROUP_HPP

#include "bf_rt_tm_table_impl.hpp"

namespace bfrt {

class BfRtTMPortGroupCfgTable : public BfRtTMTable {
 public:
  BfRtTMPortGroupCfgTable(const std::string &program_name,
                          bf_rt_id_t id,
                          std::string name,
                          const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_PORT_GROUP_CFG,
                    std::set<TableApi>{TableApi::DEFAULT_ENTRY_GET,
                                       TableApi::GET,
                                       TableApi::GET_FIRST,
                                       TableApi::USAGE_GET}) {}

  ~BfRtTMPortGroupCfgTable() = default;

  bf_status_t tableDefaultEntryGet(const BfRtSession &session,
                                   const bf_rt_target_t &dev_tgt,
                                   const uint64_t &flags,
                                   BfRtTableData *data) const override final;

  bf_status_t tableEntryGet(const BfRtSession &session,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t &flagsi,
                            const BfRtTableKey &key,
                            BfRtTableData *data) const override final;

  bf_status_t tableUsageGet(const BfRtSession &session,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t &flagsi,
                            uint32_t *count) const override final;

  bf_status_t keyReset(BfRtTableKey *key) const override final;

  bf_status_t keyAllocate(
      std::unique_ptr<BfRtTableKey> *key_ret) const override final;

  bf_status_t tableEntryGetFirst(const BfRtSession &session,
                                 const bf_rt_target_t &dev_tgt,
                                 const uint64_t &flags,
                                 BfRtTableKey *key,
                                 BfRtTableData *data) const override final;
};

class BfRtTMPortGroupTable : public BfRtTMTable {
 public:
  BfRtTMPortGroupTable(const std::string &program_name,
                       bf_rt_id_t id,
                       std::string name,
                       const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_PORT_GROUP,
                    std::set<TableApi>{TableApi::MODIFY,
                                       TableApi::DEFAULT_ENTRY_GET,
                                       TableApi::GET,
                                       TableApi::GET_FIRST,
                                       TableApi::GET_NEXT_N,
                                       TableApi::CLEAR,
                                       TableApi::USAGE_GET}) {}

  ~BfRtTMPortGroupTable() = default;

  bool actionIdApplicable() const override final { return true; }

  bf_status_t tableClear(const BfRtSession &session,
                         const bf_rt_target_t &dev_tgt,
                         const uint64_t &flags) const override final;

  bf_status_t tableUsageGet(const BfRtSession &session,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t &flags,
                            uint32_t *count) const override final;

  bf_status_t tableSizeGet(const BfRtSession &session,
                           const bf_rt_target_t &dev_tgt,
                           const uint64_t &flags,
                           size_t *size) const override final;

  bf_status_t keyReset(BfRtTableKey *key) const override final;

  bf_status_t keyAllocate(
      std::unique_ptr<BfRtTableKey> *key_ret) const override final;

  bf_status_t tableDefaultEntryGet(const BfRtSession &session,
                                   const bf_rt_target_t &dev_tgt,
                                   const uint64_t &flags,
                                   BfRtTableData *data) const override final;

  bf_status_t tableEntryGet(const BfRtSession &session,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t &flags,
                            const BfRtTableKey &key,
                            BfRtTableData *data) const override final;

  bf_status_t tableEntryGetFirst(const BfRtSession &session,
                                 const bf_rt_target_t &dev_tgt,
                                 const uint64_t &flags,
                                 BfRtTableKey *key,
                                 BfRtTableData *data) const override final;
  bf_status_t tableEntryGetNext_n(const BfRtSession &session,
                                  const bf_rt_target_t &dev_tgt,
                                  const uint64_t &flags,
                                  const BfRtTableKey &key,
                                  const uint32_t &n,
                                  keyDataPairs *key_data_pairs,
                                  uint32_t *num_returned) const override final;

  bf_status_t tableEntryMod(const BfRtSession &session,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t &flags,
                            const BfRtTableKey &key,
                            const BfRtTableData &data) const override final;

 protected:
  bf_status_t tableSetEntry(const bf_rt_target_t &dev_tgt,
                            const BfRtTMTableData &data,
                            bf_tm_pg_t pg_id) const;
};

}  // namespace bfrt
#endif
