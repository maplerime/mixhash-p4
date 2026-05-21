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

#include <tdi_common/tdi_tofino_target.hpp>

namespace tdi {
namespace tna {
namespace tofino {

tdi_status_t Target::setValue(const tdi_target_e &target_field,
                              const uint64_t &value) {
  if (target_field == static_cast<tdi_target_e>(TDI_TOFINO_TARGET_PARSER_ID)) {
    this->parser_id_ = value;
  } else {
    return tdi::tna::Target::setValue(target_field, value);
  }
  return TDI_SUCCESS;
}

tdi_status_t Target::getValue(const tdi_target_e &target_field,
                              uint64_t *value) const {
  if (target_field == static_cast<tdi_target_e>(TDI_TOFINO_TARGET_PARSER_ID)) {
    *value = this->parser_id_;
  } else {
    return tdi::tna::Target::getValue(target_field, value);
  }
  return TDI_SUCCESS;
}

void Target::getTargetVals(bf_dev_target_t *dev_tgt,
                           bf_dev_direction_t *direction,
                           uint8_t *parser_id) const {
  if (dev_tgt) {
    dev_tgt->device_id = this->dev_id_;
    dev_tgt->dev_pipe_id = this->pipe_id_;
  }
  if (direction) {
    *direction = static_cast<bf_dev_direction_t>(this->direction_);
  }
  if (parser_id) {
    *parser_id = this->parser_id_;
  }
}

}  // namespace tofino
}  // namespace tna
}  // namespace tdi
