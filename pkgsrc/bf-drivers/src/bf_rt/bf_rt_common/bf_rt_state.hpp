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

#ifndef _BF_RT_STATE_HPP
#define _BF_RT_STATE_HPP

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include <bf_rt/bf_rt_session.hpp>
#include <bf_rt/bf_rt_info.hpp>
#include <bf_rt/bf_rt_table.hpp>
#include <bf_rt/bf_rt_table_data.hpp>
#include <bf_rt/bf_rt_table_key.hpp>
#include <bf_rt/bf_rt_table_attributes.hpp>
#include <bf_rt/bf_rt_table_operations.hpp>
#include "bf_rt_table_key_impl.hpp"
#include "bf_rt_table_data_impl.hpp"
#include "bf_rt_table_attributes_impl.hpp"
#include "bf_rt_table_operations_impl.hpp"
#include "bf_rt_utils.hpp"
#include <bf_rt_p4/bf_rt_table_state.hpp>
#include <bf_rt_p4/bf_rt_learn_state.hpp>
#include <bf_rt_p4/bf_rt_table_attributes_state.hpp>
#include "bf_rt_table_operations_state.hpp"
#include <bf_rt_port/bf_rt_port_table_attributes_state.hpp>
#include <bf_rt_pre/bf_rt_pre_state.hpp>
#include <bf_rt_tm/bf_rt_tm_state.hpp>
namespace bfrt {

namespace state_common {
// This helper template function is for both BfRtObjectMap
// and BfRtDeviceState. The ID generates funcs for bf_rt_id_t and uint16_t
template <class ID, class T>
inline std::shared_ptr<T> getState(
    ID id, std::map<ID, std::shared_ptr<T>> *state_map) {
  // Call under a mutex lock for synchronization.
  if ((*state_map).find(id) == (*state_map).end()) {
    // State does not exist for this id. Probably this is first use.
    // Allocate it.
    (*state_map)[id] = std::make_shared<T>(id);
  }
  return (*state_map).at(id);
}

template <class T>
inline std::shared_ptr<T> getStateObject(std::shared_ptr<T> *state_obj) {
  // TODO: Add mutex lock for synchronization
  if (*state_obj == nullptr) {
    // State does not exist. Probably this is first use.
    // Allocate it.
    *state_obj = std::make_shared<T>();
  }
  return (*state_obj);
}
}  // namespace state_common

class BfRtDeviceState {
 public:
  BfRtDeviceState(const uint16_t &dev_id, const std::string &prog_name)
      : device_id(dev_id), program_name(prog_name){};

  void copyFixedObjects(BfRtDeviceState &src) {
    this->preState.stateObj = src.preState.getStateObj();
    this->tmPpgState.stateObj = src.tmPpgState.getStateObj();
  };

  // Helpful nested template to generate unordered maps
  // for this class
  template <typename U>
  class BfRtObjectMap {
   public:
    std::shared_ptr<U> getObjState(bf_rt_id_t tbl_id);

   private:
    std::map<bf_rt_id_t, std::shared_ptr<U>> objMap;
  };

  template <typename U>
  class BfRtObjectMapAtomic {
   public:
    std::shared_ptr<U> getObjState(bf_rt_id_t tbl_id);

   private:
    std::mutex objMap_lock;
    std::map<bf_rt_id_t, std::shared_ptr<U>> objMap;
  };

  template <typename U>
  class BfRtStateObject {
   public:
    std::shared_ptr<U> getStateObj();

   private:
    std::shared_ptr<U> stateObj;
    friend class bfrt::BfRtDeviceState;
  };

  // A map of next handles for table for Get_Next_n function calls
  BfRtObjectMap<BfRtStateNextRef> nextRefState;
  // A map of learn id to Learn State
  BfRtObjectMap<BfRtStateLearn> learnState;
  // A map of table id to Attributes State
  BfRtObjectMap<BfRtStateTableAttributes> attributesState;

  // A map of table id to AttributesPortStateChange state
  BfRtObjectMap<BfRtStateTableAttributesPort> attributePortState;

  // A map of table id to Operations Register State
  BfRtObjectMapAtomic<
      BfRtStateTableOperationsPool<BfRtRegisterSyncCb, bf_rt_register_sync_cb>>
      operationsRegisterState;
  // A map of table id to Operations Counter State
  BfRtObjectMapAtomic<
      BfRtStateTableOperationsPool<BfRtCounterSyncCb, bf_rt_counter_sync_cb>>
      operationsCounterState;
  // A map of table id to Operations HitState State
  BfRtObjectMapAtomic<BfRtStateTableOperationsPool<BfRtHitStateUpdateCb,
                                                   bf_rt_hit_state_update_cb>>
      operationsHitStateState;

  // PRE state object
  BfRtStateObject<BfRtPREStateObj> preState;

  // TM PPG State object
  BfRtStateObject<BfRtTMPpgStateObj> tmPpgState;

 private:
  uint16_t device_id;
  std::string program_name;
};

template <typename U>
std::shared_ptr<U> BfRtDeviceState::BfRtObjectMap<U>::getObjState(
    bf_rt_id_t tbl_id) {
  return state_common::getState(tbl_id, &objMap);
}

template <typename U>
std::shared_ptr<U> BfRtDeviceState::BfRtObjectMapAtomic<U>::getObjState(
    bf_rt_id_t tbl_id) {
  std::lock_guard<std::mutex> lock(objMap_lock);
  return state_common::getState(tbl_id, &objMap);
}

template <typename U>
std::shared_ptr<U> BfRtDeviceState::BfRtStateObject<U>::getStateObj() {
  return state_common::getStateObject(&stateObj);
}

}  // namespace bfrt

#endif  // _BF_RT_STATE_HPP
