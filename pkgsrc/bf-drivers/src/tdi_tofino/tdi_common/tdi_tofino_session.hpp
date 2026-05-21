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

#ifndef _TDI_TOFINO_SESSION_HPP
#define _TDI_TOFINO_SESSION_HPP

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <algorithm>

#include <tdi/common/tdi_session.hpp>

namespace tdi {
namespace tna {
namespace tofino {

class Session : public tdi::Session {
 public:
  Session(const std::vector<tdi_mgr_type_e> &mgr_type_list);

  virtual ~Session();

  virtual tdi_status_t create() override;
  virtual tdi_status_t destroy() override;

  virtual tdi_status_t completeOperations() const override;

  virtual tdi_handle_t handleGet(const tdi_mgr_type_e &mgr_type) const override;
  // Batching functions
  virtual tdi_status_t beginBatch() const override;

  virtual tdi_status_t flushBatch() const override;

  virtual tdi_status_t endBatch(bool hwSynchronous) const override;

  // Transaction functions
  virtual tdi_status_t beginTransaction(bool isAtomic) const override;

  virtual tdi_status_t verifyTransaction() const override;

  virtual tdi_status_t commitTransaction(bool hwSynchronous) const override;

  virtual tdi_status_t abortTransaction() const override;

  // Hidden
  const bool &isInBatch() const { return in_batch_; }
  const bool &isInPipeBatch() const { return in_pipe_mgr_batch_; }
  const bool &isInMcBatch() const { return in_mc_mgr_batch_; }
  void setPipeBatch(const bool batch) const { in_pipe_mgr_batch_ = batch; }
  void setMcBatch(const bool batch) const { in_mc_mgr_batch_ = batch; }

 private:
  mutable bool in_pipe_mgr_batch_{false};
  mutable bool in_mc_mgr_batch_{false};
  mutable bool in_batch_{false};
  bool mc_mgr_skip_{false};
  tdi_handle_t session_handle_;      // Pipe mgr session handle
  tdi_handle_t pre_session_handle_;  // MC mgr (PRE) session handle
};

}  // namespace tofino
}  // namespace tna
}  // namespace tdi

#endif  // _TDI_TOFINO_SESSION_HPP
