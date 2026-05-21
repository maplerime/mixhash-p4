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

#ifndef _TDI_STATE_HPP
#define _TDI_STATE_HPP

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include <tdi_p4/tdi_table_state.hpp>
#include <tdi_p4/tdi_table_attributes_state.hpp>

#include <tdi_port/tdi_port_table_attributes_state.hpp>

namespace tdi {
namespace tna {
namespace tofino {

namespace state_common {
// This helper template function is for both ObjectMap
// and DeviceState. The ID generates funcs for tdi_id_t and uint16_t
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

class DeviceState {
 public:
  DeviceState(const std::string &prog_name) : program_name(prog_name){};

  void copyFixedObjects(DeviceState & /*src*/) {
#if 0
    this->preState.stateObj = src.preState.getStateObj();
    this->tmPpgState.stateObj = src.tmPpgState.getStateObj();
#endif
  };

  // Helpful nested template to generate unordered maps
  // for this class
  template <typename U>
  class ObjectMap {
   public:
    std::shared_ptr<U> getObjState(tdi_id_t tbl_id);

   private:
    std::map<tdi_id_t, std::shared_ptr<U>> objMap;
  };

  template <typename U>
  class ObjectMapAtomic {
   public:
    std::shared_ptr<U> getObjState(tdi_id_t tbl_id);

   private:
    std::mutex objMap_lock;
    std::map<tdi_id_t, std::shared_ptr<U>> objMap;
  };

  template <typename U>
  class StateObject {
   public:
    std::shared_ptr<U> getStateObj();

   private:
    std::shared_ptr<U> stateObj;
    friend class DeviceState;
  };

  // A map of next handles for table for Get_Next_n function calls
  ObjectMap<StateNextRef> nextRefState;
  // A map of table id to Attributes State
  ObjectMap<StateTableAttributes> attributesState;

  // A map of table id to AttributesPortStateChange state
  ObjectMap<StateTableAttributesPort> attributePortState;

#if 0
  // A map of learn id to Learn State
  ObjectMap<StateLearn> learnState;

  // A map of table id to Operations Register State
  ObjectMapAtomic<
      StateTableOperationsPool<RegisterSyncCb, tdi_register_sync_cb>>
      operationsRegisterState;
  // A map of table id to Operations Counter State
  ObjectMapAtomic<
      StateTableOperationsPool<CounterSyncCb, tdi_counter_sync_cb>>
      operationsCounterState;
  // A map of table id to Operations HitState State
  ObjectMapAtomic<StateTableOperationsPool<HitStateUpdateCb,
                                                   tdi_hit_state_update_cb>>
      operationsHitStateState;

  // PRE state object
  StateObject<PREStateObj> preState;

  // TM PPG State object
  StateObject<TMPpgStateObj> tmPpgState;
#endif

 private:
  std::string program_name;
};

template <typename U>
std::shared_ptr<U> DeviceState::ObjectMap<U>::getObjState(tdi_id_t tbl_id) {
  return state_common::getState(tbl_id, &objMap);
}

template <typename U>
std::shared_ptr<U> DeviceState::ObjectMapAtomic<U>::getObjState(
    tdi_id_t tbl_id) {
  std::lock_guard<std::mutex> lock(objMap_lock);
  return state_common::getState(tbl_id, &objMap);
}

template <typename U>
std::shared_ptr<U> DeviceState::StateObject<U>::getStateObj() {
  return state_common::getStateObject(&stateObj);
}

}  // namespace tofino
}  // namespace tna
}  // namespace tdi

#endif  // _TDI_STATE_HPP
