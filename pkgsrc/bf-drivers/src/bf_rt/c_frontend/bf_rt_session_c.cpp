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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <bf_rt/bf_rt_table.h>
#include <bf_rt/bf_rt_session.h>

#ifdef __cplusplus
}
#endif

#include <bf_rt/bf_rt_session.hpp>
#include <bf_rt_common/bf_rt_session_impl.hpp>
#include <bf_rt_common/bf_rt_utils.hpp>
#include "bf_rt_state_c.hpp"

bf_status_t bf_rt_session_create(bf_rt_session_hdl **session) {
  auto sess = bfrt::BfRtSession::sessionCreate();
  if (sess == nullptr) {
    LOG_ERROR("%s:%d Unable to create session", __func__, __LINE__);
    return BF_UNEXPECTED;
  }
  *session = reinterpret_cast<bf_rt_session_hdl *>(sess.get());
  // insert the shared_ptr in the state map
  auto &c_state = bfrt::bfrt_c::BfRtCFrontEndSessionState::getInstance();
  c_state.insertShared(sess);
  return BF_SUCCESS;
}

bf_status_t bf_rt_session_destroy(bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  auto status = sess->sessionDestroy();
  if (status != BF_SUCCESS) {
    LOG_ERROR("%s:%d Failed to destroy session", __func__, __LINE__);
    return status;
  }
  // remove the shared_ptr from the state map. It will get destroyed
  // automatically if not already
  auto &c_state = bfrt::bfrt_c::BfRtCFrontEndSessionState::getInstance();
  c_state.removeShared(sess);
  return BF_SUCCESS;
}

bf_rt_id_t bf_rt_sess_handle_get(const bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<const bfrt::BfRtSession *>(session);
  return sess->sessHandleGet();
}

bf_rt_id_t bf_rt_pre_sess_handle_get(const bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<const bfrt::BfRtSession *>(session);
  return sess->preSessHandleGet();
}

bool bf_rt_session_is_valid(const bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return false;
  }
  auto sess = reinterpret_cast<const bfrt::BfRtSession *>(session);
  return sess->isValid();
}

bf_status_t bf_rt_session_complete_operations(
    const bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return false;
  }
  auto sess = reinterpret_cast<const bfrt::BfRtSession *>(session);
  return sess->sessionCompleteOperations();
}

bf_status_t bf_rt_begin_batch(bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->beginBatch();
}

bf_status_t bf_rt_flush_batch(bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->flushBatch();
}

bf_status_t bf_rt_end_batch(bf_rt_session_hdl *const session,
                            bool hwSynchronous) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->endBatch(hwSynchronous);
}

bf_status_t bf_rt_begin_transaction(bf_rt_session_hdl *const session,
                                    bool isAtomic) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->beginTransaction(isAtomic);
}

bf_status_t bf_rt_verify_transaction(bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->verifyTransaction();
}

bf_status_t bf_rt_commit_transaction(bf_rt_session_hdl *const session,
                                     bool hwSynchronous) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return BF_INVALID_ARG;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->commitTransaction(hwSynchronous);
}

bf_status_t bf_rt_abort_transaction(bf_rt_session_hdl *const session) {
  if (session == nullptr) {
    LOG_ERROR("%s:%d Session Handle passed is null", __func__, __LINE__);
    return false;
  }
  auto sess = reinterpret_cast<bfrt::BfRtSession *>(session);
  return sess->abortTransaction();
}
