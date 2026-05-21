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

#include <tofino/pdfixed/pd_ts.h>
#include <lld/bf_ts_if.h>
#include <port_mgr/bf_port_if.h>

p4_pd_status_t p4_pd_ts_global_ts_state_set(p4_pd_ts_dev_t dev, bool enable) {
  return bf_ts_global_ts_state_set(dev, enable);
}

p4_pd_status_t p4_pd_ts_global_ts_state_get(p4_pd_ts_dev_t dev, bool *enable) {
  return bf_ts_global_ts_state_get(dev, enable);
}

p4_pd_status_t p4_pd_ts_global_ts_value_set(p4_pd_ts_dev_t dev,
                                            uint64_t global_ts) {
  return bf_ts_global_ts_value_set(dev, global_ts);
}

p4_pd_status_t p4_pd_ts_global_ts_value_get(p4_pd_ts_dev_t dev,
                                            uint64_t *global_ts) {
  return bf_ts_global_ts_value_get(dev, global_ts);
}

p4_pd_status_t p4_pd_ts_global_ts_inc_value_set(p4_pd_ts_dev_t dev,
                                                uint32_t global_inc_ns) {
  return bf_ts_global_ts_inc_value_set(dev, global_inc_ns);
}

p4_pd_status_t p4_pd_ts_global_ts_inc_value_get(p4_pd_ts_dev_t dev,
                                                uint32_t *global_inc_ns) {
  return bf_ts_global_ts_inc_value_get(dev, global_inc_ns);
}

p4_pd_status_t p4_pd_ts_global_ts_offset_value_set(p4_pd_ts_dev_t dev,
                                                   uint64_t global_ts) {
  return bf_ts_global_ts_offset_set(dev, global_ts);
}

p4_pd_status_t p4_pd_ts_global_ts_offset_value_get(p4_pd_ts_dev_t dev,
                                                   uint64_t *global_ts) {
  return bf_ts_global_ts_offset_get(dev, global_ts);
}

p4_pd_status_t p4_pd_ts_global_baresync_ts_get(p4_pd_ts_dev_t dev,
                                               uint64_t *global_ts,
                                               uint64_t *baresync_ts) {
  return bf_ts_global_baresync_ts_get(dev, global_ts, baresync_ts);
}

p4_pd_status_t p4_pd_ts_1588_timestamp_delta_tx_set(p4_pd_ts_dev_t dev,
                                                    p4_pd_ts_port_t port,
                                                    uint16_t delta) {
  return bf_port_1588_timestamp_delta_tx_set(dev, port, delta);
}

p4_pd_status_t p4_pd_ts_1588_timestamp_delta_tx_get(p4_pd_ts_dev_t dev,
                                                    p4_pd_ts_port_t port,
                                                    uint16_t *delta) {
  return bf_port_1588_timestamp_delta_tx_get(dev, port, delta);
}

p4_pd_status_t p4_pd_ts_1588_timestamp_delta_rx_set(p4_pd_ts_dev_t dev,
                                                    p4_pd_ts_port_t port,
                                                    uint16_t delta) {
  return bf_port_1588_timestamp_delta_rx_set(dev, port, delta);
}

p4_pd_status_t p4_pd_ts_1588_timestamp_delta_rx_get(p4_pd_ts_dev_t dev,
                                                    p4_pd_ts_port_t port,
                                                    uint16_t *delta) {
  return bf_port_1588_timestamp_delta_rx_get(dev, port, delta);
}

p4_pd_status_t p4_pd_ts_1588_timestamp_tx_get(p4_pd_ts_dev_t dev,
                                              p4_pd_ts_port_t port,
                                              uint64_t *ts,
                                              bool *ts_valid,
                                              int *ts_id) {
  return bf_port_1588_timestamp_tx_get(dev, port, ts, ts_valid, ts_id);
}
