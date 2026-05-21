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

#ifndef _BF_RT_PORT_TBL_ATTRIBUTES_STATE_HPP
#define _BF_RT_PORT_TBL_ATTRIBUTES_STATE_HPP

#include <mutex>
#include <unordered_map>

#include <bf_rt/bf_rt_table_attributes.hpp>
#include <bf_rt_common/bf_rt_table_impl.hpp>

namespace bfrt {

class BfRtTableObj;

int bfRtPortStatusChgInternalCb(bf_dev_id_t dev_id,
                                int dev_port,
                                bool port_up,
                                void *cookie);

class BfRtStateTableAttributesPort {
 public:
  BfRtStateTableAttributesPort(bf_rt_id_t id) : table_id_(id){};

  void stateTableAttributesPortSet(bool enabled,
                                   BfRtPortStatusNotifCb callback_fn,
                                   bf_rt_port_status_chg_cb callback_c,
                                   const BfRtTableObj *table,
                                   void *cookie);

  void stateTableAttributesPortReset();
  std::tuple<bool,
             BfRtPortStatusNotifCb,
             bf_rt_port_status_chg_cb,
             const BfRtTableObj *,
             void *>
  stateTableAttributesPortGet();

 private:
  std::mutex state_lock;
  bf_rt_id_t table_id_ = 0;
  bool enabled_ = false;
  BfRtPortStatusNotifCb callback_ = nullptr;
  bf_rt_port_status_chg_cb callback_c_ = nullptr;
  BfRtTableObj *table_ = nullptr;
  void *cookie_ = nullptr;
};

}  // namespace bfrt
#endif  // _BF_RT_PORT_TBL_ATTRIBUTES_STATE_HPP
