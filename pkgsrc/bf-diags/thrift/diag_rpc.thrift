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

/*
        diag thrift file
*/

namespace py diag_rpc
namespace cpp diag_rpc

typedef i32 diag_status_t
typedef i32 diag_port_t
typedef i32 diag_vlan_t
typedef byte diag_device_t
typedef i32 diag_entry_hdl_t
typedef i32 diag_sess_hdl_t

enum diag_loop_mode_t {
  DIAG_PORT_LPBK_NONE = 0,
  DIAG_PORT_LPBK_MAC,
  DIAG_PORT_LPBK_PHY,
  DIAG_PORT_LPBK_EXT,
  DIAG_PORT_LPBK_PCS,
}

enum diag_test_status_t {
  DIAG_TEST_STATUS_UNKNOWN = 0,
  DIAG_TEST_STATUS_FAIL,
  DIAG_TEST_STATUS_PASS,
  DIAG_TEST_STATUS_IN_PROGRESS,
}

enum diag_data_pattern_t {
  DIAG_DATA_PATTERN_RANDOM = 0,
  DIAG_DATA_PATTERN_FIXED,
}

enum diag_packet_payload_t {
  DIAG_PACKET_PAYLOAD_RANDOM = 0,
  DIAG_PACKET_PAYLOAD_FIXED,
  DIAG_PACKET_PAYLOAD_RANDOM_FLIP,
}

enum diag_packet_full_t {
  DIAG_PACKET_FULL_RANDOM = 0,
  DIAG_PACKET_FULL_FIXED,
}

enum diag_port_group_t {
  DIAG_PORT_GROUP_ALL  = 0x7fff,
  DIAG_PORT_GROUP_ALLI = 0x7ffe,
  DIAG_PORT_GROUP_ALL_MESH = 0x7ffd,
}

struct diag_port_status_t {
  1: required i32 status;
  2: required i32 tx_total;
  3: required i32 rx_total;
  4: required i32 rx_good;
  5: required i32 rx_bad;
}

exception InvalidDiagOperation {
  1:i32 code
}

service diag_rpc {
    diag_sess_hdl_t diag_loopback_test_setup(1:diag_device_t device, 2:list<i32> port_list, 3:i32 num_ports, 4:i32 loop_mode) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_test_start(1:diag_sess_hdl_t sess_hdl, 2:i32 num_pkt, 3:i32 pkt_size) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_test_abort(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_test_status_get(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    diag_port_status_t diag_loopback_test_port_status_get(1:diag_sess_hdl_t sess_hdl, 2:diag_port_t port) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_test_cleanup(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    diag_sess_hdl_t diag_loopback_snake_test_setup(1:diag_device_t device, 2:list<i32> port_list, 3:i32 num_ports, 4:i32 loop_mode) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_snake_test_start(1:diag_sess_hdl_t sess_hdl, 2:i32 num_pkt, 3:i32 pkt_size, 4:bool bidir) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_snake_test_stop(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_snake_test_status_get(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_snake_test_cleanup(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    diag_sess_hdl_t diag_loopback_pair_test_setup(1:diag_device_t device, 2:list<i32> port_list, 3:i32 num_ports, 4:i32 loop_mode) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_pair_test_start(1:diag_sess_hdl_t sess_hdl, 2:i32 num_pkt, 3:i32 pkt_size, 4:bool bidir) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_pair_test_stop(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_pair_test_status_get(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_loopback_pair_test_cleanup(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    bool diag_session_valid(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_loopback_mode_set(1:diag_device_t device, 2:diag_port_t port, 3:i32 loop_mode) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_loopback_mode_get(1:diag_device_t device, 2:diag_port_t port) throws (1:InvalidDiagOperation ouch);
    diag_entry_hdl_t diag_forwarding_rule_add(1:diag_device_t device, 2:diag_port_t ig_port, 3:diag_port_t eg_port, 4:i32 tcp_dstPort_start, 5:i32 tcp_dstPort_end, 6:i32 priority) throws (1:InvalidDiagOperation ouch);
    i32 diag_forwarding_rule_del(1:diag_device_t device, 2:diag_entry_hdl_t hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_mac_aging_set(1:diag_device_t device, 2:i32 ttl) throws (1:InvalidDiagOperation ouch);
    i32 diag_mac_aging_reset(1:diag_device_t device) throws (1:InvalidDiagOperation ouch);
    i32 diag_mac_aging_get(1:diag_device_t device) throws (1:InvalidDiagOperation ouch);
    i32 diag_learning_timeout_set(1:diag_device_t device, 2:i32 timeout) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_default_vlan_set(1:diag_device_t device, 2:diag_port_t port, 3:diag_vlan_t vlan_id) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_default_vlan_reset(1:diag_device_t device, 2:diag_port_t port) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_default_vlan_get(1:diag_device_t device, 2:diag_port_t port) throws (1:InvalidDiagOperation ouch);
    i32 diag_vlan_create(1:diag_device_t device, 2:diag_vlan_t vlan_id) throws (1:InvalidDiagOperation ouch);
    i32 diag_vlan_destroy(1:diag_device_t device, 2:diag_vlan_t vlan_id) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_vlan_add(1:diag_device_t device,  2:diag_port_t port, 3:diag_vlan_t vlan_id) throws (1:InvalidDiagOperation ouch);
    i32 diag_port_vlan_del(1:diag_device_t device,  2:diag_port_t port, 3:diag_vlan_t vlan_id) throws (1:InvalidDiagOperation ouch);
    i32 diag_packet_inject_from_cpu(1:diag_device_t device, 2:list<i32> port_list, 3:i32 num_ports, 4:i32 num_pkt, 5:i32 pkt_size) throws (1:InvalidDiagOperation ouch);
    i32 diag_cpu_port_get(1:diag_device_t device) throws (1:InvalidDiagOperation ouch);
    diag_port_status_t diag_cpu_stats_get(1:diag_device_t device, 2:diag_port_t port) throws (1:InvalidDiagOperation ouch);
    i32 diag_cpu_stats_clear(1:diag_device_t device, 2:diag_port_t port, 3: byte all_ports) throws (1:InvalidDiagOperation ouch);
    diag_sess_hdl_t diag_multicast_loopback_test_setup(1:diag_device_t device, 2:list<i32> port_list, 3:i32 num_ports, 4:i32 loop_mode) throws (1:InvalidDiagOperation ouch);
    i32 diag_multicast_loopback_test_start(1:diag_sess_hdl_t sess_hdl, 2:i32 num_pkt, 3:i32 pkt_size) throws (1:InvalidDiagOperation ouch);
    i32 diag_multicast_loopback_test_stop(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_multicast_loopback_test_status_get(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_multicast_loopback_test_cleanup(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_data_pattern_set(1:diag_sess_hdl_t sess_hdl, 2:diag_data_pattern_t mode, 3: i32 start_pat, 4: i32 start_pat_len, 5: i32 pat_a, 6: i32 pat_b, 7:i32 pattern_len) throws (1:InvalidDiagOperation ouch);
    i32 diag_packet_payload_set(1:diag_sess_hdl_t sess_hdl, 2:diag_packet_payload_t mode, 3: string payload, 4: string payload_file_path) throws (1:InvalidDiagOperation ouch);
    i32 diag_packet_full_set(1:diag_sess_hdl_t sess_hdl, 2:diag_packet_full_t mode, 3: string pkt, 4: string pkt_file_path) throws (1:InvalidDiagOperation ouch);
    i32 diag_sessions_max_set(1:diag_device_t device, 2:i32 max_sessions) throws (1:InvalidDiagOperation ouch);
    i32 diag_min_packet_size_enable(bool enable) throws (1:InvalidDiagOperation ouch);
    diag_sess_hdl_t diag_stream_setup(1:diag_device_t device, 2:i32 num_pkts, 3: i32 pkt_size, 4: diag_port_t dst_port) throws (1:InvalidDiagOperation ouch);
    i32 diag_stream_start(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_stream_adjust(1:diag_sess_hdl_t sess_hdl, 2:i32 num_pkts, 3: i32 pkt_size) throws (1:InvalidDiagOperation ouch);
    i32 diag_stream_stop(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i64 diag_stream_counter_get(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
    i32 diag_stream_cleanup(1:diag_sess_hdl_t sess_hdl) throws (1:InvalidDiagOperation ouch);
}
