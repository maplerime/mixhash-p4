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

// TDI includes
#include <tdi/common/tdi_defs.h>
#include <tdi/common/tdi_utils.hpp>
#include <tdi/common/tdi_utils.hpp>
#include <tdi/common/tdi_json_parser/tdi_cjson.hpp>

// TDI Tofino includes
#include <tdi_tofino/tdi_tofino_defs.h>

//
#include "tdi_pipe_mgr_intf.hpp"
#include "../tdi_pre/tdi_mc_mgr_intf.hpp"
#include "tdi_tofino_session.hpp"

namespace tdi {
namespace tna {
namespace tofino {

Session::Session(const std::vector<tdi_mgr_type_e> &mgr_type_list)
    : tdi::Session(mgr_type_list) {}

Session::~Session() {
  // session delete here
  if (is_valid_) {
    this->destroy();
  }
}

tdi_status_t Session::create() {
  tdi_status_t status = TDI_SUCCESS;
  auto *pipeMgr = PipeMgrIntf::getInstance();
  pipe_sess_hdl_t sessHndl = 0;
  auto *mcMgr = McMgrIntf::getInstance();
  bf_mc_session_hdl_t mcSessHndl = 0;

  for (const auto &mgr_type : mgr_type_list_) {
    if (static_cast<tdi_tofino_mgr_type_e>(mgr_type) ==
        TDI_TOFINO_MGR_TYPE_PIPE_MGR) {
      mc_mgr_skip_ = true;
    }
  }

  status = pipeMgr->pipeMgrClientInit(&sessHndl);
  if (status != TDI_SUCCESS) {
    LOG_ERROR("%s:%d Error in creating pipe mgr session, err %s",
              __func__,
              __LINE__,
              pipe_str_err((pipe_status_t)status));
  }
  session_handle_ = sessHndl;

  if (!mc_mgr_skip_) {
    status = mcMgr->mcMgrCreateSession(&mcSessHndl);
    if (status != TDI_SUCCESS) {
      LOG_ERROR("%s:%d Error in creating mc mgr session, err %s",
                __func__,
                __LINE__,
                bf_err_str(status));
      // Since the mc mgr session creation failed, delete pipe mgr
      // session and return error
      tdi_status_t temp_sts = pipeMgr->pipeMgrClientCleanup(sessHndl);
      if (temp_sts != TDI_SUCCESS) {
        LOG_ERROR(
            "%s:%d Error in cleaning up the pipe mgr session "
            "with handle %d, err %s",
            __func__,
            __LINE__,
            sessHndl,
            pipe_str_err((pipe_status_t)temp_sts));
        return status;
      }
      return status;
    }
    pre_session_handle_ = mcSessHndl;
  }

  // Mark session as is_valid
  is_valid_ = true;
  return status;
}

tdi_status_t Session::destroy() {
  tdi_status_t status = TDI_SUCCESS;
  auto *pipeMgr = PipeMgrIntf::getInstance();
  auto *mcMgr = McMgrIntf::getInstance();

  status = pipeMgr->pipeMgrClientCleanup(session_handle_);
  if (status != TDI_SUCCESS) {
    LOG_ERROR(
        "%s:%d Error in cleaning up the pipe mgr session "
        "with handle %d, err %s",
        __func__,
        __LINE__,
        session_handle_,
        pipe_str_err((pipe_status_t)status));
    return status;
  }
  this->in_pipe_mgr_batch_ = false;

  if (!mc_mgr_skip_) {
    status = mcMgr->mcMgrDestroySession(pre_session_handle_);
    if (status != TDI_SUCCESS) {
      LOG_ERROR(
          "%s:%d Error in cleaning up the mc mgr session "
          "with handle %d, err %s",
          __func__,
          __LINE__,
          pre_session_handle_,
          bf_err_str(status));
      return status;
    }
    this->in_mc_mgr_batch_ = false;
  }
  this->in_batch_ = false;
  // Mark session as inis_valid
  is_valid_ = false;

  return TDI_SUCCESS;
}

tdi_status_t Session::completeOperations() const {
  if (is_valid_) {
    tdi_status_t status = TDI_SUCCESS;
    auto *pipeMgr = PipeMgrIntf::getInstance();
    auto *mcMgr = McMgrIntf::getInstance();

    status = pipeMgr->pipeMgrCompleteOperations(session_handle_);

    if (!mc_mgr_skip_) {
      status |= mcMgr->mcMgrCompleteOperations(pre_session_handle_);
    }
    return status;
  }
  return TDI_INVALID_ARG;
}

tdi_handle_t Session::handleGet(const tdi_mgr_type_e &mgr_type) const {
  auto mgr_type_tofino = static_cast<tdi_tofino_mgr_type_e>(mgr_type);
  switch (mgr_type_tofino) {
    case TDI_TOFINO_MGR_TYPE_PIPE_MGR:
      return session_handle_;
    case TDI_TOFINO_MGR_TYPE_MC_MGR:
      return pre_session_handle_;
    default:
      return 0;
  }
  return 0;
}

// Batching functions
tdi_status_t Session::beginBatch() const {
  if (is_valid_ && !this->in_batch_) {
    this->in_batch_ = true;
    if (mc_mgr_skip_) {
      // Will cause start batch to be ignored.
      this->in_mc_mgr_batch_ = true;
    }

    return TDI_SUCCESS;
  }
  LOG_ERROR(
      "%s:%d Session is inis_valid or already in batch.", __func__, __LINE__);
  return TDI_INVALID_ARG;
}

tdi_status_t Session::flushBatch() const {
  if (is_valid_) {
    tdi_status_t status = TDI_SUCCESS;
    auto *pipeMgr = PipeMgrIntf::getInstance();
    auto *mcMgr = McMgrIntf::getInstance();

    if (this->in_pipe_mgr_batch_) {
      status = pipeMgr->pipeMgrFlushBatch(session_handle_);
    }

    if (!mc_mgr_skip_ && this->in_mc_mgr_batch_) {
      status |= mcMgr->mcMgrFlushBatch(pre_session_handle_);
    }

    return status;
  }
  return TDI_INVALID_ARG;
}

tdi_status_t Session::endBatch(bool hwSynchronous) const {
  if (is_valid_) {
    tdi_status_t status = TDI_SUCCESS;
    auto *pipeMgr = PipeMgrIntf::getInstance();
    auto *mcMgr = McMgrIntf::getInstance();

    if (this->in_pipe_mgr_batch_) {
      status = pipeMgr->pipeMgrEndBatch(session_handle_, hwSynchronous);
      this->in_pipe_mgr_batch_ = false;
    }

    if (!mc_mgr_skip_ && this->in_mc_mgr_batch_) {
      status |= mcMgr->mcMgrEndBatch(pre_session_handle_, hwSynchronous);
    }
    this->in_mc_mgr_batch_ = false;

    this->in_batch_ = false;
    return status;
  }
  return TDI_INVALID_ARG;
}

// Transaction functions
tdi_status_t Session::beginTransaction(bool isAtomic) const {
  if (is_valid_) {
    auto *pipeMgr = PipeMgrIntf::getInstance();
    return pipeMgr->pipeMgrBeginTxn(session_handle_, isAtomic);
  }
  return TDI_INVALID_ARG;
}

tdi_status_t Session::verifyTransaction() const {
  if (is_valid_) {
    auto *pipeMgr = PipeMgrIntf::getInstance();
    return pipeMgr->pipeMgrVerifyTxn(session_handle_);
  }
  return TDI_INVALID_ARG;
}

tdi_status_t Session::commitTransaction(bool hwSynchronous) const {
  if (is_valid_) {
    auto *pipeMgr = PipeMgrIntf::getInstance();
    return pipeMgr->pipeMgrCommitTxn(session_handle_, hwSynchronous);
  }
  return TDI_INVALID_ARG;
}

tdi_status_t Session::abortTransaction() const {
  if (is_valid_) {
    auto *pipeMgr = PipeMgrIntf::getInstance();
    return pipeMgr->pipeMgrAbortTxn(session_handle_);
  }
  return TDI_INVALID_ARG;
}

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
