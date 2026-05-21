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

#ifndef _bf_switch_cli_api_h_
#define _bf_switch_cli_api_h_

#include <unordered_map>
#include "bf_switch/bf_switch_types.h"
#include "s3/attribute_util.h"

namespace smi {
namespace switchcli {

/* typedef */
typedef uint16_t counter_id_t;

typedef enum bf_switch_cli_api_counter_event_ {
  COUNTER_EVENT_TYPE_INVALID,
  COUNTER_EVENT_TYPE_CLEAR,
  COUNTER_EVENT_TYPE_OBJECT_DELETE,
  COUNTER_EVENT_TYPE_MAX
} bf_switch_cli_api_counter_event_t;

/* struct declaration */

struct switch_cli_api_counter {
  switch_counter_t switch_counter;
  uint64_t delta;
};

struct cli_counter_metadata {
  uint64_t last_count;
  cli_counter_metadata(const switch_counter_t &switch_counter)
      : last_count(switch_counter.count) {}
};

/* function protypes */
switch_status_t bf_switch_cli_api_notify_counter_event(
    bf_switch_cli_api_counter_event_t event_type,
    const switch_object_id_t counter_object_handle);

switch_status_t bf_switch_cli_init();
switch_status_t bf_switch_cli_api_counters_get(
    switch_object_id_t object_handle,
    std::vector<switch_cli_api_counter> &cntrs);

/* class declarations */
class stats_mgr {
 public:
  stats_mgr() {}
  ~stats_mgr() {}
  switch_status_t get_and_update_cntr_changed_since_last_query(
      const switch_object_id_t object_handle,
      const switch_counter_t &curr_counter,
      switch_counter_t &counters_changed);
  switch_status_t clear_counters(const switch_object_id_t object_handle);

  switch_status_t delete_counters(const switch_object_id_t object_handle);

 private:
  struct object_counter_info {
    bool is_dirty;
    std::unordered_map<counter_id_t, cli_counter_metadata> counterId_map;
    object_counter_info() : is_dirty(false) {}
  };

  std::unordered_map<switch_object_id_t, object_counter_info> counter_obj_map;
};

} /* namespace switchcli */
} /* namespace smi */

#endif /* end of _bf_switch_cli_api_h_ */
