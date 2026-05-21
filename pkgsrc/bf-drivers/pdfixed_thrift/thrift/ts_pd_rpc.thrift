include "res.thrift"

namespace py ts_pd_rpc
namespace cpp ts_pd_rpc
namespace c_glib ts_pd_rpc

exception InvalidTimestampOperation {
  1:i32 code
}

typedef i32 ts_dev_t
typedef i32 ts_port_t

struct ts_global_baresync_t {
  1:  i64 global_ts
  2:  i64 baresync_ts
}

struct ts_1588_timestamp_t {
  1: i64 ts
  2: bool ts_valid
  3: i32 ts_id
}

service ts {
  void ts_global_ts_state_set(1: ts_dev_t dev_id, 2: bool enable) throws (1: InvalidTimestampOperation ouch);
  bool ts_global_ts_state_get(1: ts_dev_t dev_id) throws (1: InvalidTimestampOperation ouch);
  void ts_global_ts_value_set(1: ts_dev_t dev_id, 2: i64 global_ts) throws (1: InvalidTimestampOperation ouch);
  i64 ts_global_ts_value_get(1: ts_dev_t dev_id) throws (1: InvalidTimestampOperation ouch);
  void ts_global_ts_inc_value_set(1: ts_dev_t dev_id, 2: i32 global_inc_ns) throws (1: InvalidTimestampOperation ouch);
  i64 ts_global_ts_inc_value_get(1: ts_dev_t dev_id) throws (1: InvalidTimestampOperation ouch);
  void ts_global_ts_offset_value_set(1: ts_dev_t dev_id, 2: i64 global_ts) throws (1: InvalidTimestampOperation ouch);
  i64 ts_global_ts_offset_value_get(1: ts_dev_t dev_id) throws (1: InvalidTimestampOperation ouch);
  ts_global_baresync_t ts_global_baresync_ts_get(1: ts_dev_t dev_id) throws (1: InvalidTimestampOperation ouch);

  void ts_1588_timestamp_delta_tx_set(1: ts_dev_t dev_id, 2: ts_port_t port_id, 3: i16 delta) throws (1: InvalidTimestampOperation ouch);
  i16 ts_1588_timestamp_delta_tx_get(1: ts_dev_t dev_id, 2: ts_port_t port_id) throws (1: InvalidTimestampOperation ouch);
  void ts_1588_timestamp_delta_rx_set(1: ts_dev_t dev_id, 2: ts_port_t port_id, 3: i16 delta) throws (1: InvalidTimestampOperation ouch);
  i16 ts_1588_timestamp_delta_rx_get(1: ts_dev_t dev_id, 2: ts_port_t port_id) throws (1: InvalidTimestampOperation ouch);
  ts_1588_timestamp_t ts_1588_timestamp_tx_get(1: ts_dev_t dev_id, 2: ts_port_t port_id) throws (1: InvalidTimestampOperation ouch);
}
