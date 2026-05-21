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

#ifndef _BF_RT_DEV_TABLE_IMPL_HPP
#define _BF_RT_DEV_TABLE_IMPL_HPP

#include <bf_rt_common/bf_rt_state.hpp>
#include <bf_rt_common/bf_rt_table_impl.hpp>

namespace bfrt {

class BfRtDevTable : public BfRtTableObj {
 public:
  BfRtDevTable(const std::string &program_name,
               bf_rt_id_t id,
               std::string name,
               const size_t &size)
      : BfRtTableObj(program_name,
                     id,
                     name,
                     size,
                     TableType::DEV_CFG,
                     std::set<TableApi>{TableApi::DEFAULT_ENTRY_SET,
                                        TableApi::DEFAULT_ENTRY_GET,
                                        TableApi::DEFAULT_ENTRY_RESET,
                                        TableApi::CLEAR}){};

  bf_status_t tableDefaultEntrySet(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const uint64_t &flags,
      const BfRtTableData &data) const override final;

  bf_status_t tableDefaultEntryGet(const BfRtSession &session,
                                   const bf_rt_target_t &dev_tgt,
                                   const uint64_t &flags,
                                   BfRtTableData *data) const override final;

  bf_status_t tableDefaultEntryReset(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const uint64_t &flags) const override final;

  bf_status_t tableClear(const BfRtSession &session,
                         const bf_rt_target_t &dev_tgt,
                         const uint64_t &flags) const override final;

  bf_status_t dataAllocate(
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  bf_status_t dataAllocate(
      const std::vector<bf_rt_id_t> &fields,
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  bf_status_t dataReset(BfRtTableData *data) const override final;

  bf_status_t dataReset(const std::vector<bf_rt_id_t> &fields,
                        BfRtTableData *data) const override final;

 private:
  bf_status_t getPortList(bf_dev_id_t dev_id,
                          bf_rt_id_t data_field,
                          std::vector<uint32_t> &vect) const;
};

class BfRtDevWarmInitTable : public BfRtTableObj {
 public:
  BfRtDevWarmInitTable(const std::string &program_name,
                       bf_rt_id_t id,
                       std::string name,
                       const size_t &size)
      : BfRtTableObj(program_name,
                     id,
                     name,
                     size,
                     TableType::DEV_WARM_INIT,
                     std::set<TableApi>{TableApi::DEFAULT_ENTRY_SET,
                                        TableApi::DEFAULT_ENTRY_GET,
                                        TableApi::DEFAULT_ENTRY_RESET,
                                        TableApi::CLEAR}){};

  bf_status_t tableDefaultEntrySet(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const uint64_t &flags,
      const BfRtTableData &data) const override final;

  bf_status_t tableDefaultEntryGet(const BfRtSession &session,
                                   const bf_rt_target_t &dev_tgt,
                                   const uint64_t &flags,
                                   BfRtTableData *data) const override final;

  bf_status_t tableDefaultEntryReset(
      const BfRtSession & /*session*/,
      const bf_rt_target_t &dev_tgt,
      const uint64_t & /*flags*/) const override final;

  bf_status_t tableClear(const BfRtSession &session,
                         const bf_rt_target_t &dev_tgt,
                         const uint64_t &flags) const override final;

  bf_status_t dataAllocate(
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  bf_status_t dataAllocate(
      const bf_rt_id_t &action_id,
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  bf_status_t dataAllocate(
      const std::vector<bf_rt_id_t> &fields,
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  bf_status_t dataAllocate(
      const std::vector<bf_rt_id_t> &fields,
      const bf_rt_id_t &action_id,
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  bf_status_t dataAllocateContainer(
      const bf_rt_id_t &container_id,
      const bf_rt_id_t &action_id,
      std::unique_ptr<BfRtTableData> *data_ret) const override final;

  // This API is more than enough to enable action APIs
  // on this table
  bool actionIdApplicable() const override { return true; };

  bf_status_t profileToContainer(bf_dev_id_t dev_id, BfRtTableData *data) const;

 private:
  static const std::map<const std::string, bf_dev_init_mode_t>
      str_to_init_mode_map;
  static const std::map<bf_dev_init_mode_t, std::string> init_mode_to_str_map;

  bf_status_t dataAllocate_internal(
      const std::vector<bf_rt_id_t> &fields,
      const bf_rt_id_t &action_id,
      std::unique_ptr<BfRtTableData> *data_ret) const;
};
}  // namespace bfrt
#endif
