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

#include "bf_switch_cli_clish.h"
#include "bf_switch_cli.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace smi::switchcli;

CLISH_PLUGIN_SYM(bf_switch_cli_replay_file) {
  return (bf_switch_cli_replay_file_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_add_object) {
  return (bf_switch_cli_add_object_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_del_object) {
  return (bf_switch_cli_del_object_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_del_object_handle) {
  return (bf_switch_cli_del_object_handle_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_set_object) {
  return (bf_switch_cli_set_object_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_object) {
  return (bf_switch_cli_show_object_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_object_handle) {
  return (bf_switch_cli_show_object_handle_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_object_all) {
  return (bf_switch_cli_show_object_all_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_counter_object) {
  return (bf_switch_cli_show_counter_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_clear_counter_object) {
  return (bf_switch_cli_clear_counter_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_add_var) {
  return (bf_switch_cli_add_var_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_get_var) {
  return (bf_switch_cli_get_var_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_set_var) {
  return (bf_switch_cli_set_var_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_describe_object) {
  return (bf_switch_cli_describe_object_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_table_info) {
  return (bf_switch_cli_show_table_info_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_packet_log_level_set) {
  return (bf_switch_cli_packet_log_level_set_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_bfd_log_level_set) {
  return (bf_switch_cli_bfd_log_level_set_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_pkt_path_counter_print) {
  return (bf_switch_cli_pkt_path_counter_print_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_cpu_rx_counter_show) {
  return (bf_switch_cli_show_cpu_rx_counter_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_cpu_rx_counter_clear) {
  return (bf_switch_cli_clear_cpu_rx_counter_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_features) {
  return (bf_switch_cli_show_features_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_log_level_show) {
  return (bf_switch_cli_log_level_show_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_debug_log_level_set) {
  return (bf_switch_cli_debug_log_level_set_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_debug_operation_set) {
  return (bf_switch_cli_debug_operation_set_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_version) {
  return (bf_switch_cli_show_version_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_show_tech_support) {
  return (bf_switch_cli_show_tech_support_internal(clish_context));
}

CLISH_PLUGIN_SYM(bf_switch_cli_state_save) {
  return (bf_switch_cli_state_save_internal(clish_context));
}

CLISH_PLUGIN_INIT(bf_switchapi) {
  clish_plugin_add_sym(
      plugin, bf_switch_cli_replay_file, "bf_switch_cli_replay_file");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_add_object, "bf_switch_cli_add_object");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_del_object, "bf_switch_cli_del_object");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_del_object_handle,
                       "bf_switch_cli_del_object_handle");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_set_object, "bf_switch_cli_set_object");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_show_object, "bf_switch_cli_show_object");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_show_object_handle,
                       "bf_switch_cli_show_object_handle");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_show_object_all, "bf_switch_cli_show_object_all");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_show_counter_object,
                       "bf_switch_cli_show_counter_object");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_clear_counter_object,
                       "bf_switch_cli_clear_counter_object");

  clish_plugin_add_sym(plugin, bf_switch_cli_add_var, "bf_switch_cli_add_var");

  clish_plugin_add_sym(plugin, bf_switch_cli_get_var, "bf_switch_cli_get_var");

  clish_plugin_add_sym(plugin, bf_switch_cli_set_var, "bf_switch_cli_set_var");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_describe_object, "bf_switch_cli_describe_object");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_show_table_info, "bf_switch_cli_show_table_info");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_packet_log_level_set,
                       "bf_switch_cli_packet_log_level_set");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_bfd_log_level_set,
                       "bf_switch_cli_bfd_log_level_set");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_pkt_path_counter_print,
                       "bf_switch_cli_pkt_path_counter_print");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_cpu_rx_counter_show,
                       "bf_switch_cli_cpu_rx_counter_show");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_cpu_rx_counter_clear,
                       "bf_switch_cli_cpu_rx_counter_clear");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_show_features, "bf_switch_cli_show_features");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_log_level_show, "bf_switch_cli_log_level_show");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_debug_log_level_set,
                       "bf_switch_cli_debug_log_level_set");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_debug_operation_set,
                       "bf_switch_cli_debug_operation_set");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_show_version, "bf_switch_cli_show_version");

  clish_plugin_add_sym(plugin,
                       bf_switch_cli_show_tech_support,
                       "bf_switch_cli_show_tech_support");

  clish_plugin_add_sym(
      plugin, bf_switch_cli_state_save, "bf_switch_cli_state_save");

  return 0;
}

#ifdef __cplusplus
}
#endif
