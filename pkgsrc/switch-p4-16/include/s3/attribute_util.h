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

#ifndef INCLUDE_S3_ATTRIBUTE_UTIL_H__
#define INCLUDE_S3_ATTRIBUTE_UTIL_H__

#include <functional>
#include <string>

#include "bf_switch/bf_switch_types.h"

inline bool operator!=(const switch_object_id_t &lhs,
                       const switch_object_id_t &rhs) {
  return !(lhs == rhs);
}
inline bool operator==(const switch_object_id_t &lhs, const uint64_t rhs) {
  return lhs.data == rhs;
}
inline bool operator!=(const switch_object_id_t &lhs, const uint64_t rhs) {
  return lhs.data != rhs;
}

inline bool operator<(const switch_object_id_t &lhs,
                      const switch_object_id_t &rhs) {
  return lhs.data < rhs.data;
}

namespace smi {
namespace attr_util {

switch_status_t parse_mac(const std::string &str, switch_mac_addr_t &mac);
switch_status_t parse_ip_address(const std::string &str,
                                 switch_ip_address_t &ip);
switch_status_t parse_ip_prefix(const std::string &str,
                                switch_ip_prefix_t &prefix);
bool is_dir_bcast_addr(uint16_t prefix_len,
                       const switch_ip_address_t &switch_ip_addr);

template <typename T>
switch_status_t v_get(const switch_attribute_value_t &value, T &val);
template <typename T>
void v_set(switch_attribute_value_t &value, const T val);

class object_and_attribute_t {
 public:
  object_and_attribute_t(switch_object_id_t _oid, switch_attr_id_t _attr_id)
      : oid(_oid), attr_id(_attr_id) {}
  switch_object_id_t oid;
  switch_attr_id_t attr_id;

  inline bool operator==(const object_and_attribute_t &other) const {
    return (attr_id == other.attr_id && oid == other.oid);
  }
};

}  // namespace attr_util
}  // namespace smi
#endif  // INCLUDE_S3_ATTRIBUTE_UTIL_H__
