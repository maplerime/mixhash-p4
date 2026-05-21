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

#include "google/rpc/code.pb.h"
#include "google/rpc/status.pb.h"

#include "p4_rt/device_mgr.h"

#include "report_error.h"

using Code = ::google::rpc::Code;
using device_id_t = ::pi::fe::proto::DeviceMgr::device_id_t;
using p4_id_t = ::pi::fe::proto::DeviceMgr::p4_id_t;
using Status = ::pi::fe::proto::DeviceMgr::Status;
using StreamMessageResponseCb =
    ::pi::fe::proto::DeviceMgr::StreamMessageResponseCb;

namespace p4v1 = ::p4::v1;
namespace p4configv1 = ::p4::config::v1;

namespace bf {

namespace p4rt {

// TODO: implement with BFRT
class DeviceMgrImp {
 public:
  explicit DeviceMgrImp(device_id_t device_id) : device_id(device_id) {}

  ~DeviceMgrImp() {}

  Status pipeline_config_set(
      p4v1::SetForwardingPipelineConfigRequest::Action /*action*/,
      const p4v1::ForwardingPipelineConfig & /*config*/) {
    RETURN_ERROR_STATUS(Code::UNIMPLEMENTED);
  }

  Status pipeline_config_get(
      p4v1::GetForwardingPipelineConfigRequest::ResponseType /*response_type*/,
      p4v1::ForwardingPipelineConfig * /*config*/) {
    RETURN_ERROR_STATUS(Code::UNIMPLEMENTED);
  }

  Status write(const p4v1::WriteRequest & /*request*/) {
    RETURN_ERROR_STATUS(Code::UNIMPLEMENTED);
  }

  Status read(const p4v1::ReadRequest & /*request*/,
              p4v1::ReadResponse * /*response*/) const {
    RETURN_ERROR_STATUS(Code::UNIMPLEMENTED);
  }

  Status read_one(const p4v1::Entity & /*entity*/,
                  p4v1::ReadResponse * /*response*/) const {
    RETURN_ERROR_STATUS(Code::UNIMPLEMENTED);
  }

  Status stream_message_request_handle(
      const p4::v1::StreamMessageRequest & /*request*/) {
    RETURN_ERROR_STATUS(Code::UNIMPLEMENTED);
  }

  void stream_message_response_register_cb(StreamMessageResponseCb /*cb*/,
                                           void * /*cookie*/) {}

  static void init(size_t /*max_devices*/) {}

  static void destroy() {}

 private:
  device_id_t device_id;
};

}  // namespace p4rt

}  // namespace bf

namespace pi {

namespace fe {

namespace proto {

// The implementation is in the ::bf::p4rt namespace.
// This is just a proxy
class DeviceMgrImp : public ::bf::p4rt::DeviceMgrImp {
 public:
  template <typename... Args>
  DeviceMgrImp(Args &&... args)
      : ::bf::p4rt::DeviceMgrImp(std::forward<Args>(args)...) {}
};

DeviceMgr::DeviceMgr(device_id_t device_id) {
  pimp = std::unique_ptr<DeviceMgrImp>(new DeviceMgrImp(device_id));
}

DeviceMgr::~DeviceMgr() {}

// PIMPL forwarding

Status DeviceMgr::pipeline_config_set(
    p4v1::SetForwardingPipelineConfigRequest::Action action,
    const p4v1::ForwardingPipelineConfig &config) {
  return pimp->pipeline_config_set(action, config);
}

Status DeviceMgr::pipeline_config_get(
    p4v1::GetForwardingPipelineConfigRequest::ResponseType response_type,
    p4v1::ForwardingPipelineConfig *config) {
  return pimp->pipeline_config_get(response_type, config);
}

Status DeviceMgr::write(const p4v1::WriteRequest &request) {
  return pimp->write(request);
}

Status DeviceMgr::read(const p4v1::ReadRequest &request,
                       p4v1::ReadResponse *response) const {
  return pimp->read(request, response);
}

Status DeviceMgr::read_one(const p4v1::Entity &entity,
                           p4v1::ReadResponse *response) const {
  return pimp->read_one(entity, response);
}

Status DeviceMgr::stream_message_request_handle(
    const p4::v1::StreamMessageRequest &request) {
  return pimp->stream_message_request_handle(request);
}

void DeviceMgr::stream_message_response_register_cb(StreamMessageResponseCb cb,
                                                    void *cookie) {
  return pimp->stream_message_response_register_cb(std::move(cb), cookie);
}

void DeviceMgr::init(size_t max_devices) { DeviceMgrImp::init(max_devices); }

void DeviceMgr::destroy() { DeviceMgrImp::destroy(); }

}  // namespace proto

}  // namespace fe

}  // namespace pi
