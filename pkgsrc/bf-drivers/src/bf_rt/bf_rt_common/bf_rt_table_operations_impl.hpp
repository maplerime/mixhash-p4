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

#ifndef _BF_RT_TABLE_OPERATIONS_IMPL_HPP
#define _BF_RT_TABLE_OPERATIONS_IMPL_HPP

#ifdef __cplusplus
extern "C" {
#endif
#include <bf_rt/bf_rt_common.h>
#ifdef __cplusplus
}
#endif

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>

#include <bf_rt/bf_rt_table_operations.hpp>
#include <bf_rt/bf_rt_table_operations.h>
#include "bf_rt_table_impl.hpp"

namespace bfrt {

class BfRtTableOperationsImpl : public BfRtTableOperations {
 public:
  BfRtTableOperationsImpl(const BfRtTableObj *table,
                          const TableOperationsType allowed_type)
      : table_(table),
        allowed_type_(allowed_type),
        session_(nullptr),
        dev_tgt_(),
        register_cpp_(nullptr),
        register_c_(nullptr),
        counter_cpp_(nullptr),
        counter_c_(nullptr),
        hit_state_cpp_(nullptr),
        hit_state_c_(nullptr),
        cookie_(nullptr){};

  static TableOperationsType getType(const std::string &op_name,
                                     const BfRtTable::TableType &object_type);

  // Works on Match Action tables and register tables only
  bf_status_t registerSyncSet(const BfRtSession &session,
                              const bf_rt_target_t &dev_tgt,
                              const BfRtRegisterSyncCb &callback,
                              const void *cookie);

  // Works on Match Action tables and counter tables only
  bf_status_t counterSyncSet(const BfRtSession &session,
                             const bf_rt_target_t &dev_tgt,
                             const BfRtCounterSyncCb &callback,
                             const void *cookie);

  bf_status_t hitStateUpdateSet(const BfRtSession &session,
                                const bf_rt_target_t &dev_tgt,
                                const BfRtHitStateUpdateCb &callback,
                                const void *cookie);

  // Works on Match Action tables and register tables only
  bf_status_t registerSyncSetCFrontend(const BfRtSession &session,
                                       const bf_rt_target_t &dev_tgt,
                                       const bf_rt_register_sync_cb &callback,
                                       const void *cookie);

  // Works on Match Action tables and counter tables only
  bf_status_t counterSyncSetCFrontend(const BfRtSession &session,
                                      const bf_rt_target_t &dev_tgt,
                                      const bf_rt_counter_sync_cb &callback,
                                      const void *cookie);

  bf_status_t hitStateUpdateSetCFrontend(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const bf_rt_hit_state_update_cb &callback,
      const void *cookie);

  // Hidden
  bf_status_t registerSyncInternal(const BfRtSession &session,
                                   const bf_rt_target_t &dev_tgt,
                                   const BfRtRegisterSyncCb &callback,
                                   const bf_rt_register_sync_cb &callback_c,
                                   const void *cookie);

  bf_status_t counterSyncInternal(const BfRtSession &session,
                                  const bf_rt_target_t &dev_tgt,
                                  const BfRtCounterSyncCb &callback,
                                  const bf_rt_counter_sync_cb &callback_c,
                                  const void *cookie);

  const TableOperationsType &getAllowedOp() const { return allowed_type_; };

  bf_status_t hitStateUpdateInternal(
      const BfRtSession &session,
      const bf_rt_target_t &dev_tgt,
      const BfRtHitStateUpdateCb &callback,
      const bf_rt_hit_state_update_cb &callback_c,
      const void *cookie);

  bf_status_t registerSyncExecute() const;
  bf_status_t counterSyncExecute() const;
  bf_status_t hitStateUpdateExecute() const;

  const BfRtTableObj *tableGet() const { return table_; };

 private:
  // backpointer to table
  const BfRtTableObj *table_;
  TableOperationsType allowed_type_;

  const BfRtSession *session_;
  bf_rt_target_t dev_tgt_;

  BfRtRegisterSyncCb register_cpp_;
  bf_rt_register_sync_cb register_c_;
  BfRtCounterSyncCb counter_cpp_;
  bf_rt_counter_sync_cb counter_c_;
  BfRtHitStateUpdateCb hit_state_cpp_;
  bf_rt_hit_state_update_cb hit_state_c_;

  const void *cookie_;
};

}  // namespace bfrt

#endif  // _BF_RT_TABLE_OPERATIONS_IMPL_HPP
