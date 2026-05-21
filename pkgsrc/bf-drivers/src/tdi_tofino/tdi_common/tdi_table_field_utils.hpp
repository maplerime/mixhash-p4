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

#ifndef _TDI_TABLE_FIELD_UTILS_HPP_
#define _TDI_TABLE_FIELD_UTILS_HPP_

#include <tdi/common/tdi_utils.hpp>
#include <tdi/common/tdi_table.hpp>

namespace tdi {

namespace utils {

template <class T1, class T2>
static const std::string getStrFromDataMaps(const T1 &str, const T2 &maps) {
  for (const auto &kv : maps) {
    if (kv.second == str) {
      return kv.first;
    }
  }
  return "UNKNOWN";
}

}  // namespace utils
}  // namespace tdi

#endif  // _TDI_TABLE_FIELD_UTILS_HPP_
