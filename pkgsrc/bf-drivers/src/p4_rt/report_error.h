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

#ifndef SRC_REPORT_ERROR_H_
#define SRC_REPORT_ERROR_H_

#include "google/rpc/code.pb.h"
#include "google/rpc/status.pb.h"

namespace bf {

namespace p4rt {

template <typename Arg1, typename... Args>
static inline ::google::rpc::Status ERROR_STATUS(::google::rpc::Code code,
                                                 const char *fmt,
                                                 const Arg1 &arg1,
                                                 const Args &... /* args */) {
  ::google::rpc::Status status;
  status.set_code(code);
  // TODO: format and set message
  return status;
}

template <typename Arg>
static inline ::google::rpc::Status ERROR_STATUS(::google::rpc::Code code,
                                                 const Arg &msg) {
  ::google::rpc::Status status;
  status.set_code(code);
  status.set_message(msg);
  return status;
}

static inline ::google::rpc::Status ERROR_STATUS(::google::rpc::Code code) {
  ::google::rpc::Status status;
  status.set_code(code);
  return status;
}

static inline ::google::rpc::Status OK_STATUS() {
  ::google::rpc::Status status;
  status.set_code(::google::rpc::Code::OK);
  return status;
}

static inline ::google::rpc::Status GENERIC_STATUS(::google::rpc::Code code) {
  ::google::rpc::Status status;
  status.set_code(code);
  return status;
}

}  // namespace p4rt

}  // namespace bf

#define RETURN_OK_STATUS() return ::bf::p4rt::OK_STATUS();
#define RETURN_ERROR_STATUS(...) return ::bf::p4rt::ERROR_STATUS(__VA_ARGS__);
#define RETURN_STATUS(code) return ::bf::p4rt::GENERIC_STATUS(code);

#define IS_OK(status) (status.code() == ::google::rpc::Code::OK)
#define IS_ERROR(status) (status.code() != ::google::rpc::Code::OK)

#define RETURN_IF_ERROR(status)            \
  do {                                     \
    auto status_ = status;                 \
    if (IS_ERROR(status_)) return status_; \
  } while (false)

#endif  // SRC_REPORT_ERROR_H_
