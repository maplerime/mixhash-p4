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

#ifndef _TDI_PORT_TBL_ATTRIBUTES_STATE_HPP
#define _TDI_PORT_TBL_ATTRIBUTES_STATE_HPP

#include <mutex>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif
#include "pipe_mgr/pipe_mgr_intf.h"
#include <tdi/common/tdi_defs.h>
#ifdef __cplusplus
}
#endif

#include <tdi/common/tdi_attributes.hpp>
#include <tdi/common/tdi_utils.hpp>

// tofino include
#include <tdi_tofino/tdi_tofino_attributes.hpp>
#include <tdi_tofino/c_frontend/tdi_tofino_attributes.h>

namespace tdi {
namespace tna {
namespace tofino {

int tdiPortStatusChgInternalCb(bf_dev_id_t dev_id,
                               int dev_port,
                               bool port_up,
                               void *cookie);

class StateTableAttributesPort {
 public:
  StateTableAttributesPort(tdi_id_t id) : table_id_(id){};

  void stateTableAttributesPortSet(bool enabled,
                                   PortStatusNotifCb callback_fn,
                                   tdi_port_status_chg_cb callback_c,
                                   const tdi::Table *table,
                                   void *cookie);

  void stateTableAttributesPortReset();
  std::tuple<bool,
             PortStatusNotifCb,
             tdi_port_status_chg_cb,
             const tdi::Table *,
             void *>
  stateTableAttributesPortGet();

 private:
  std::mutex state_lock;
  tdi_id_t table_id_;
  bool enabled_;
  PortStatusNotifCb callback_;
  tdi_port_status_chg_cb callback_c_;
  tdi::Table *table_;
  void *cookie_;
};

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
#endif  // _TDI_PORT_TBL_ATTRIBUTES_STATE_HPP
