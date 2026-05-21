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

#include "bf_rt_tm_state.hpp"
#include <bf_rt_common/bf_rt_utils.hpp>

namespace bfrt {

//--------------- BFRT TM PPG state object

bf_status_t BfRtTMPpgStateObj::atomicTMPpgGet(const bf_dev_pipe_t &pipe_id,
                                              const bf_tm_ppg_id_t &ppg_id,
                                              bf_tm_ppg_hdl *ppg_hdl) const {
  std::lock_guard<std::mutex> lock(this->state_lock);
  return this->stateTMPpgGet(pipe_id, ppg_id, ppg_hdl);
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgGet(const bf_dev_pipe_t &pipe_id,
                                             const bf_tm_ppg_id_t &ppg_id,
                                             bf_tm_ppg_hdl *ppg_hdl) const {
  // The state_lock must be already taken.
  if (this->pipe_ppg_map.find(pipe_id) != this->pipe_ppg_map.end()) {
    auto pipe_map = this->pipe_ppg_map.at(pipe_id);
    if (pipe_map.find(ppg_id) != pipe_map.end()) {
      if (ppg_hdl) {
        *ppg_hdl = pipe_map.at(ppg_id);
      }
      return BF_SUCCESS;
    }
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgHdlGet(const bf_dev_pipe_t &pipe_id,
                                                const bf_tm_ppg_hdl &ppg_hdl,
                                                bf_tm_ppg_id_t *ppg_id) const {
  // The state_lock must be already taken.
  if (this->pipe_hdl_map.find(pipe_id) != this->pipe_hdl_map.end()) {
    auto &pipe_map = this->pipe_hdl_map.at(pipe_id);
    if (pipe_map.find(ppg_hdl) != pipe_map.end()) {
      if (ppg_id) {
        *ppg_id = this->pipe_hdl_map.at(pipe_id).at(ppg_hdl);
      }
      return BF_SUCCESS;
    }
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::stateTMCntPpgGet(
    const bf_dev_pipe_t &pipe_id,
    const bf_tm_ppg_id_t &ppg_cnt_id,
    bf_tm_ppg_id_t *ppg_id) const {
  // The state_lock must be already taken.
  if (this->pipe_cnt_to_ppg.find(pipe_id) != this->pipe_cnt_to_ppg.end()) {
    auto &pipe_map = this->pipe_cnt_to_ppg.at(pipe_id);
    if (pipe_map.find(ppg_cnt_id) != pipe_map.end()) {
      if (ppg_id) {
        *ppg_id = pipe_map.at(ppg_cnt_id);
      }
      return BF_SUCCESS;
    }
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::atomicTMPpgAdd(const bf_dev_pipe_t &pipe_id,
                                              const bf_tm_ppg_id_t &ppg_id,
                                              const bf_tm_ppg_hdl &ppg_hdl) {
  std::lock_guard<std::mutex> lock(this->state_lock);
  return this->stateTMPpgAdd(pipe_id, ppg_id, ppg_hdl);
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgAdd(const bf_dev_pipe_t &pipe_id,
                                             const bf_tm_ppg_id_t &ppg_id,
                                             const bf_tm_ppg_hdl &ppg_hdl) {
  // The state_lock must be already taken.
  if (this->pipe_ppg_map.size() != this->pipe_hdl_map.size()) {
    return BF_UNEXPECTED;
  }

  // Check if the state already exists.
  auto status1 = stateTMPpgGet(pipe_id, ppg_id, nullptr);
  auto status2 = stateTMPpgHdlGet(pipe_id, ppg_hdl, nullptr);
  if (BF_SUCCESS == status1 || BF_SUCCESS == status2) {
    return BF_ALREADY_EXISTS;
  }

  if (BF_OBJECT_NOT_FOUND == status1 && BF_OBJECT_NOT_FOUND == status2) {
    this->pipe_ppg_map[pipe_id][ppg_id] = ppg_hdl;
    this->pipe_hdl_map[pipe_id][ppg_hdl] = ppg_id;

    LOG_DBG(
        "%s:%d add PPG state: pipe_id=%d, ppg_id=%d, ppg_hdl=0x%x, "
        "size=[%zu:%zu,%zu:%zu]",
        __func__,
        __LINE__,
        pipe_id,
        ppg_id,
        ppg_hdl,
        this->pipe_ppg_map.size(),
        this->pipe_ppg_map.at(pipe_id).size(),
        this->pipe_hdl_map.size(),
        this->pipe_hdl_map.at(pipe_id).size());

    return BF_SUCCESS;
  }
  return BF_UNEXPECTED;
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgCntAdd(
    const bf_dev_pipe_t &pipe_id,
    const bf_tm_ppg_id_t &ppg_cnt_id,
    const bf_tm_ppg_id_t &ppg_id) {
  // The state_lock must be already taken.
  auto status = stateTMCntPpgGet(pipe_id, ppg_cnt_id, nullptr);
  if (BF_SUCCESS == status) {
    return BF_ALREADY_EXISTS;
  } else if (BF_OBJECT_NOT_FOUND == status) {
    this->pipe_cnt_to_ppg[pipe_id][ppg_cnt_id] = ppg_id;
  }

  LOG_DBG(
      "%s:%d add PPG counter state: pipe_id=%d, ppg_counter_id=%d, ppg_id=%d, "
      "size=[%zu:%zu]",
      __func__,
      __LINE__,
      pipe_id,
      ppg_cnt_id,
      ppg_id,
      this->pipe_cnt_to_ppg.size(),
      this->pipe_cnt_to_ppg.at(pipe_id).size());

  return BF_SUCCESS;
}

bf_status_t BfRtTMPpgStateObj::atomicTMPpgDel(const bf_dev_pipe_t &pipe_id,
                                              const bf_tm_ppg_id_t &ppg_id) {
  std::lock_guard<std::mutex> lock(this->state_lock);
  return this->stateTMPpgDel(pipe_id, ppg_id);
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgDel(const bf_dev_pipe_t &pipe_id,
                                             const bf_tm_ppg_id_t &ppg_id) {
  // The state_lock must be already taken.
  if (this->pipe_ppg_map.size() != this->pipe_hdl_map.size()) {
    return BF_UNEXPECTED;
  }

  bf_tm_ppg_hdl ppg_hdl = 0;
  if (BF_SUCCESS == stateTMPpgGet(pipe_id, ppg_id, &ppg_hdl)) {
    auto &pipe_map_ppg = this->pipe_ppg_map.at(pipe_id);
    auto &pipe_map_hdl = this->pipe_hdl_map.at(pipe_id);

    LOG_DBG(
        "%s:%d del PPG state: pipe_id=%d, "
        "ppg_id=%d, ppg_hdl=0x%x, size=[%zu:%zu,%zu]",
        __func__,
        __LINE__,
        pipe_id,
        ppg_id,
        ppg_hdl,
        this->pipe_ppg_map.size(),
        pipe_map_ppg.size(),
        pipe_map_hdl.size());

    pipe_map_ppg.erase(ppg_id);
    pipe_map_hdl.erase(ppg_hdl);

    return BF_SUCCESS;
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgCntDel(
    const bf_dev_pipe_t &pipe_id, const bf_tm_ppg_id_t &ppg_cnt_id) {
  // The state_lock must be already taken.
  if (BF_SUCCESS == stateTMCntPpgGet(pipe_id, ppg_cnt_id, nullptr)) {
    auto &pipe_map_cnt = this->pipe_cnt_to_ppg.at(pipe_id);

    LOG_DBG(
        "%s:%d del PPG counter state: pipe_id=%d, "
        "ppg_cnt_id=0x%x, size=[%zu:%zu]",
        __func__,
        __LINE__,
        pipe_id,
        ppg_cnt_id,
        this->pipe_cnt_to_ppg.size(),
        pipe_map_cnt.size());

    pipe_map_cnt.erase(ppg_cnt_id);

    return BF_SUCCESS;
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::atomicSizeGet(const bf_dev_pipe_t &pipe_id,
                                             std::size_t &size) const {
  std::lock_guard<std::mutex> lock(this->state_lock);
  return this->stateSizeGet(pipe_id, size);
}

bf_status_t BfRtTMPpgStateObj::stateSizeGet(const bf_dev_pipe_t &pipe_id,
                                            std::size_t &size) const {
  // The state_lock must be already taken.
  if (this->pipe_ppg_map.find(pipe_id) != this->pipe_ppg_map.end()) {
    size = this->pipe_ppg_map.at(pipe_id).size();
    return BF_SUCCESS;
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::atomicTMPpgGetFirst(
    const bf_dev_pipe_t &pipe_id,
    bf_tm_ppg_id_t &ppg_id,
    bf_tm_ppg_hdl &ppg_hdl) const {
  std::lock_guard<std::mutex> lock(this->state_lock);
  return this->stateTMPpgGetFirst(pipe_id, ppg_id, ppg_hdl);
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgGetFirst(
    const bf_dev_pipe_t &pipe_id,
    bf_tm_ppg_id_t &ppg_id,
    bf_tm_ppg_hdl &ppg_hdl) const {
  // The state_lock must be already taken.
  if (this->pipe_ppg_map.find(pipe_id) != this->pipe_ppg_map.end()) {
    auto itm_first = this->pipe_ppg_map.at(pipe_id).begin();
    if (itm_first != this->pipe_ppg_map.at(pipe_id).end()) {
      ppg_id = itm_first->first;
      ppg_hdl = itm_first->second;
      return BF_SUCCESS;
    }
  }
  return BF_OBJECT_NOT_FOUND;
}

bf_status_t BfRtTMPpgStateObj::atomicTMPpgGetNext(
    const bf_dev_pipe_t &pipe_id,
    const bf_tm_ppg_id_t &ppg_id,
    bf_tm_ppg_id_t &ppg_id_next,
    bf_tm_ppg_hdl &ppg_hdl_next) const {
  std::lock_guard<std::mutex> lock(this->state_lock);
  return this->stateTMPpgGetNext(pipe_id, ppg_id, ppg_id_next, ppg_hdl_next);
}

bf_status_t BfRtTMPpgStateObj::stateTMPpgGetNext(
    const bf_dev_pipe_t &pipe_id,
    const bf_tm_ppg_id_t &ppg_id,
    bf_tm_ppg_id_t &ppg_id_next,
    bf_tm_ppg_hdl &ppg_hdl_next) const {
  // The state_lock must be already taken.
  if (this->pipe_ppg_map.find(pipe_id) != this->pipe_ppg_map.end()) {
    auto itm_next = this->pipe_ppg_map.at(pipe_id).upper_bound(ppg_id);
    if (itm_next != this->pipe_ppg_map.at(pipe_id).end()) {
      ppg_id_next = itm_next->first;
      ppg_hdl_next = itm_next->second;
      return BF_SUCCESS;
    }
  }
  return BF_OBJECT_NOT_FOUND;
}

}  // namespace bfrt
