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

#ifndef _BF_RT_TM_TABLE_IMPL_CFG_HPP
#define _BF_RT_TM_TABLE_IMPL_CFG_HPP

#include "bf_rt_tm_table_impl.hpp"

namespace bfrt {

class BfRtTMCfgTable : public BfRtTMTable {
 public:
  BfRtTMCfgTable(const std::string &program_name,
                 bf_rt_id_t id,
                 std::string name,
                 const size_t &size)
      : BfRtTMTable(program_name,
                    id,
                    name,
                    size,
                    TableType::TM_CFG,
                    std::set<TableApi>{TableApi::DEFAULT_ENTRY_SET,
                                       TableApi::DEFAULT_ENTRY_RESET,
                                       TableApi::DEFAULT_ENTRY_GET,
                                       TableApi::CLEAR}) {}

  ~BfRtTMCfgTable() = default;

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
}  // namespace bfrt
#endif
