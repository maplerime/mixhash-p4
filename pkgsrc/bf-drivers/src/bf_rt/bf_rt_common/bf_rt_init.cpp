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

// bf_rt includes
#include <bf_rt/bf_rt_info.hpp>
#include <bf_rt/bf_rt_init.hpp>

// local includes
#include "bf_rt_init_impl.hpp"

namespace bfrt {

BfRtDevMgr *BfRtDevMgr::dev_mgr_instance = nullptr;
std::mutex BfRtDevMgr::dev_mgr_instance_mutex;

bf_status_t BfRtInit::bfRtModuleInit(bool pkt_mgr_skip,
                                     bool mc_mgr_skip,
                                     bool port_mgr_skip,
                                     bool traffic_mgr_skip) {
  // Just call the corresponding implementation class function
  return BfRtInitImpl::bfRtModuleInit(
      pkt_mgr_skip, mc_mgr_skip, port_mgr_skip, traffic_mgr_skip);
}

BfRtDevMgr::BfRtDevMgr() {
  dev_mgr_impl_ = std::unique_ptr<BfRtDevMgrImpl>(new BfRtDevMgrImpl());
}

BfRtDevMgr &BfRtDevMgr::getInstance() {
  if (dev_mgr_instance == nullptr) {
    dev_mgr_instance_mutex.lock();
    if (dev_mgr_instance == nullptr) {
      dev_mgr_instance = new BfRtDevMgr();
    }
    dev_mgr_instance_mutex.unlock();
  }
  return *(BfRtDevMgr::dev_mgr_instance);
}

bf_status_t BfRtDevMgr::bfRtInfoP4NamesGet(
    const bf_dev_id_t &dev_id,
    std::vector<std::reference_wrapper<const std::string>> &p4_names) {
  return devMgrImpl()->bfRtInfoP4NamesGet(dev_id, p4_names);
}

bf_status_t BfRtDevMgr::fixedFilePathsGet(
    const bf_dev_id_t &dev_id,
    std::vector<std::reference_wrapper<const std::string>> &fixed_file_vec) {
  return devMgrImpl()->fixedFilePathsGet(dev_id, fixed_file_vec);
}

bf_status_t BfRtDevMgr::bfRtInfoGet(const bf_dev_id_t &dev_id,
                                    const std::string &prog_name,
                                    const BfRtInfo **ret_obj) const {
  // Just call the corresponding implementation class function
  return devMgrImpl()->bfRtInfoGet(dev_id, prog_name, ret_obj);
}

bf_status_t BfRtDevMgr::bfRtDeviceIdListGet(
    std::set<bf_dev_id_t> *device_id_list) const {
  return devMgrImpl()->bfRtDeviceIdListGet(device_id_list);
}

}  // namespace bfrt
