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

#ifndef _BF_RT_TM_TABLE_IMPL_POOL_HPP
#define _BF_RT_TM_TABLE_IMPL_POOL_HPP

#include "bf_rt_tm_table_impl.hpp"

namespace bfrt {

// Pool table
class BfRtTMPoolCfgTable : public BfRtTMTable {
 public:
  BfRtTMPoolCfgTable(const std::string &program_name,
                     bf_rt_id_t id,
                     std::string name,
                     const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_POOL_CFG,
                    std::set<TableApi>{TableApi::MODIFY,
                                       TableApi::GET,
                                       TableApi::GET_FIRST,
                                       TableApi::GET_NEXT_N,
                                       TableApi::USAGE_GET}) {}

  bf_status_t tableEntryMod(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            const BfRtTableData &data) const override final;

  bf_status_t tableEntryGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            BfRtTableData *data) const override final;

  bf_status_t tableUsageGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            uint32_t *count) const override final;

  bf_status_t tableEntryGetFirst(const BfRtSession & /*session*/,
                                 const bf_rt_target_t &dev_tgt,
                                 const uint64_t & /*flags*/,
                                 BfRtTableKey *key,
                                 BfRtTableData *data) const override final;

  bf_status_t tableEntryGetNext_n(const BfRtSession & /*session*/,
                                  const bf_rt_target_t &dev_tgt,
                                  const uint64_t & /*flags*/,
                                  const BfRtTableKey &key,
                                  const uint32_t &n,
                                  keyDataPairs *key_data_pairs,
                                  uint32_t *num_returned) const override final;

  bf_status_t keyAllocate(
      std::unique_ptr<BfRtTableKey> *key_ret) const override final;

  bf_status_t keyReset(BfRtTableKey *key) const override final;

 private:
  bf_status_t tableEntryGet_internal(const bf_rt_target_t &dev_tgt,
                                     const std::string &pool,
                                     BfRtTableData *data) const;

  bf_status_t specialPoolSizeSet_internal(bf_dev_id_t dev,
                                          const std::string &pool,
                                          uint32_t cells) const;

  bf_status_t specialPoolSizeGet_internal(bf_dev_id_t dev,
                                          const std::string &pool,
                                          uint32_t *cells) const;
};

// Skid pool table
class BfRtTMPoolSkidTable : public BfRtTMTable {
 public:
  BfRtTMPoolSkidTable(const std::string &program_name,
                      bf_rt_id_t id,
                      std::string name,
                      const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_POOL_SKID,
                    std::set<TableApi>{TableApi::DEFAULT_ENTRY_SET,
                                       TableApi::DEFAULT_ENTRY_RESET,
                                       TableApi::DEFAULT_ENTRY_GET,
                                       TableApi::CLEAR}) {}

  bf_status_t tableDefaultEntrySet(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const uint64_t &flags,
      const BfRtTableData &data) const override final;

  bf_status_t tableDefaultEntryReset(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const uint64_t &flags) const override final;

  bf_status_t tableDefaultEntryGet(const BfRtSession &session,
                                   const bf_rt_target_t &dev_tgt,
                                   const uint64_t &flags,
                                   BfRtTableData *data) const override final;

  bf_status_t tableClear(const BfRtSession &session,
                         const bf_rt_target_t &dev_tgt,
                         const uint64_t &flags) const override final;
};

// App pool table
class BfRtTMPoolAppTable : public BfRtTMTable {
 public:
  BfRtTMPoolAppTable(const std::string &program_name,
                     bf_rt_id_t id,
                     std::string name,
                     const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_POOL_APP,
                    std::set<TableApi>{TableApi::MODIFY,
                                       TableApi::GET,
                                       TableApi::GET_FIRST,
                                       TableApi::GET_NEXT_N,
                                       TableApi::USAGE_GET}) {}

  bool actionIdApplicable() const override final { return true; }

  bf_status_t tableEntryMod(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            const BfRtTableData &data) const override final;

  bf_status_t tableEntryGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            BfRtTableData *data) const override final;

  bf_status_t tableEntryGetFirst(const BfRtSession & /*session*/,
                                 const bf_rt_target_t &dev_tgt,
                                 const uint64_t & /*flags*/,
                                 BfRtTableKey *key,
                                 BfRtTableData *data) const override final;

  bf_status_t tableEntryGetNext_n(const BfRtSession & /*session*/,
                                  const bf_rt_target_t &dev_tgt,
                                  const uint64_t & /*flags*/,
                                  const BfRtTableKey &key,
                                  const uint32_t &n,
                                  keyDataPairs *key_data_pairs,
                                  uint32_t *num_returned) const override final;

  bf_status_t tableUsageGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            uint32_t *count) const override final;

  bf_status_t keyAllocate(
      std::unique_ptr<BfRtTableKey> *key_ret) const override final;

  bf_status_t keyReset(BfRtTableKey *key) const override final;

 private:
  bf_status_t tableEntryGet_internal(const bf_rt_target_t &dev_tgt,
                                     const std::string &pool,
                                     BfRtTableData *data) const;
};

// Pool color table
class BfRtTMPoolColorTable : public BfRtTMTable {
 public:
  BfRtTMPoolColorTable(const std::string &program_name,
                       bf_rt_id_t id,
                       std::string name,
                       const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_POOL_COLOR,
                    std::set<TableApi>{TableApi::MODIFY,
                                       TableApi::GET,
                                       TableApi::GET_FIRST,
                                       TableApi::GET_NEXT_N,
                                       TableApi::USAGE_GET,
                                       TableApi::CLEAR}) {}

  ~BfRtTMPoolColorTable() = default;

  bf_status_t tableEntryMod(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            const BfRtTableData &data) const override final;

  bf_status_t tableEntryGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            BfRtTableData *data) const override final;

  bf_status_t tableUsageGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            uint32_t *count) const override final;

  bf_status_t tableEntryGetFirst(const BfRtSession & /*session*/,
                                 const bf_rt_target_t &dev_tgt,
                                 const uint64_t & /*flags*/,
                                 BfRtTableKey *key,
                                 BfRtTableData *data) const override final;

  bf_status_t tableEntryGetNext_n(const BfRtSession & /*session*/,
                                  const bf_rt_target_t &dev_tgt,
                                  const uint64_t & /*flags*/,
                                  const BfRtTableKey &key,
                                  const uint32_t &n,
                                  keyDataPairs *key_data_pairs,
                                  uint32_t *num_returned) const override final;

  bf_status_t tableClear(const BfRtSession & /*session*/,
                         const bf_rt_target_t &dev_tgt,
                         const uint64_t & /*flags*/) const override final;

  bf_status_t keyReset(BfRtTableKey *key) const override final;

  bf_status_t keyAllocate(
      std::unique_ptr<BfRtTableKey> *key_ret) const override final;

 private:
  bf_status_t tableEntryMod_internal(
      const bf_rt_target_t &dev_tgt,
      const std::string &color,
      const uint32_t &color_drop_resume_limit) const;

  bf_status_t tableEntryGet_internal(const bf_rt_target_t &dev_tgt,
                                     const std::string &color,
                                     BfRtTableData *data) const;
};

// Pool color table
class BfRtTMPoolAppPfcTable : public BfRtTMTable {
 public:
  BfRtTMPoolAppPfcTable(const std::string &program_name,
                        bf_rt_id_t id,
                        std::string name,
                        const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_POOL_COLOR,
                    std::set<TableApi>{TableApi::MODIFY,
                                       TableApi::GET,
                                       TableApi::GET_FIRST,
                                       TableApi::GET_NEXT_N,
                                       TableApi::USAGE_GET,
                                       TableApi::CLEAR}) {}

  ~BfRtTMPoolAppPfcTable() = default;

  bf_status_t tableEntryMod(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            const BfRtTableData &data) const override final;

  bf_status_t tableEntryGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            const BfRtTableKey &key,
                            BfRtTableData *data) const override final;

  bf_status_t tableUsageGet(const BfRtSession & /*session*/,
                            const bf_rt_target_t &dev_tgt,
                            const uint64_t & /*flags*/,
                            uint32_t *count) const override final;

  bf_status_t tableEntryGetFirst(const BfRtSession & /*session*/,
                                 const bf_rt_target_t &dev_tgt,
                                 const uint64_t & /*flags*/,
                                 BfRtTableKey *key,
                                 BfRtTableData *data) const override final;

  bf_status_t tableEntryGetNext_n(const BfRtSession & /*session*/,
                                  const bf_rt_target_t &dev_tgt,
                                  const uint64_t & /*flags*/,
                                  const BfRtTableKey &key,
                                  const uint32_t &n,
                                  keyDataPairs *key_data_pairs,
                                  uint32_t *num_returned) const override final;

  bf_status_t tableClear(const BfRtSession & /*session*/,
                         const bf_rt_target_t &dev_tgt,
                         const uint64_t & /*flags*/) const override final;

  bf_status_t keyReset(BfRtTableKey *key) const override final;

  bf_status_t keyAllocate(
      std::unique_ptr<BfRtTableKey> *key_ret) const override final;

 private:
  bf_status_t tableEntryMod_internal(const bf_rt_target_t &dev_tgt,
                                     const std::string &pool,
                                     const uint8_t &cos,
                                     const uint32_t &pfc_limit_cells) const;

  bf_status_t tableEntryGet_internal(const bf_rt_target_t &dev_tgt,
                                     const std::string &pool,
                                     const uint8_t &cos,
                                     BfRtTableData *data) const;
};
}  // namespace bfrt
#endif
