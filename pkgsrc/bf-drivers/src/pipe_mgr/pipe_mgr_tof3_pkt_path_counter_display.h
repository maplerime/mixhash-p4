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

#ifndef __TOF3_PKT_PATH_DISPLAY_H__
#define __TOF3_PKT_PATH_DISPLAY_H__

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <target-sys/bf_sal/bf_sys_intf.h>

#include "pipe_mgr_int.h"

#if DVM_CONFIG_INCLUDE_UCLI == 1

#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>
void pipe_mgr_tof3_pkt_path_display_iprsr_counter(ucli_context_t *uc,
                                                  int hex,
                                                  bool non_zero,
                                                  int devid,
                                                  int pipe,
                                                  int pg_port,
                                                  int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_eprsr_counter(ucli_context_t *uc,
                                                  int hex,
                                                  bool non_zero,
                                                  int devid,
                                                  int pipe,
                                                  int pg_port,
                                                  int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_ipb_counter(ucli_context_t *uc,
                                                int hex,
                                                bool non_zero,
                                                int devid,
                                                int pipe,
                                                int pg_port,
                                                int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_epb_counter(ucli_context_t *uc,
                                                int hex,
                                                bool non_zero,
                                                int devid,
                                                int pipe,
                                                int pg_port,
                                                int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_ebuf_counter(ucli_context_t *uc,
                                                 int hex,
                                                 bool non_zero,
                                                 int devid,
                                                 int pipe,
                                                 int pg_port,
                                                 int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_s2p_counter(ucli_context_t *uc,
                                                int hex,
                                                bool non_zero,
                                                int devid,
                                                int pipe,
                                                int pg_port,
                                                int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_p2s_counter(ucli_context_t *uc,
                                                int hex,
                                                bool non_zero,
                                                int devid,
                                                int pipe,
                                                int pg_port,
                                                int pg_port_end);
void pipe_mgr_tof3_pkt_path_display_idprsr_counter(
    ucli_context_t *uc, int hex, bool non_zero, int devid, int pipe);
void pipe_mgr_tof3_pkt_path_display_edprsr_counter(
    ucli_context_t *uc, int hex, bool non_zero, int devid, int pipe);
void pipe_mgr_tof3_pkt_path_display_pmarb_counter(
    ucli_context_t *uc, int hex, bool non_zero, int devid, int pipe);
void pipe_mgr_tof3_pkt_path_display_pipe_counter(ucli_context_t *uc,
                                                 int hex,
                                                 int devid,
                                                 int pipe);
void pipe_mgr_tof3_nonzero_chip_counter_cli(ucli_context_t *uc, int devid);
void pipe_mgr_tof3_per_chip_counter_cli(ucli_context_t *uc, int devid);
void pipe_mgr_tof3_nonzero_pipe_counter_cli(ucli_context_t *uc,
                                            int devid,
                                            int pipe);
void pipe_mgr_tof3_pipe_counter_cli(ucli_context_t *uc, int devid, int pipe);
void pipe_mgr_tof3_nonzero_pipe_and_port_counter_cli(
    ucli_context_t *uc, int devid, int pipe, int port, int port_end);
void pipe_mgr_tof3_pipe_and_port_counter_cli(
    ucli_context_t *uc, int devid, int pipe, int port, int port_end);
#endif
void pipe_mgr_tof3_pkt_path_clear_iprsr_counter(int devid,
                                                int pipe,
                                                int pg_port,
                                                int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_eprsr_counter(int devid,
                                                int pipe,
                                                int pg_port,
                                                int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_ipb_counter(int devid,
                                              int pipe,
                                              int pg_port,
                                              int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_epb_counter(int devid,
                                              int pipe,
                                              int pg_port,
                                              int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_ebuf_counter(int devid,
                                               int pipe,
                                               int pg_port,
                                               int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_s2p_counter(int devid,
                                              int pipe,
                                              int pg_port,
                                              int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_p2s_counter(int devid,
                                              int pipe,
                                              int pg_port,
                                              int pg_port_end);
void pipe_mgr_tof3_pkt_path_clear_idprsr_counter(rmt_dev_info_t *dev_info,
                                                 int pipe);
void pipe_mgr_tof3_pkt_path_clear_edprsr_counter(rmt_dev_info_t *dev_info,
                                                 int pipe);
void pipe_mgr_tof3_pkt_path_clear_pmarb_counter(int devid, int pipe);
void pipe_mgr_tof3_pkt_path_clear_pipe_counter(int devid, int pipe);
void pipe_mgr_tof3_pkt_path_clear_all_counter(int devid, int pipe);

// read counter
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_iprsr_read_counter(
    int devid, int p, int pg_port, int port_numb, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_idprsr_read_counter(
    rmt_dev_info_t *dev_info, int p, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_epb_ebuf_read_counter(
    int devid, int p, int pg_port, int port_numb, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_eprsr_read_counter(
    int devid, int p, int pg_port, int port_numb, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_edprsr_read_counter(
    rmt_dev_info_t *dev_info, int p, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_pmarb_read_counter(int devid,
                                                                   int p,
                                                                   int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_s2p_read_counter(
    int devid, int p, int pg_port, int port_numb, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_p2s_read_counter(
    int devid, int p, int pg_port, int port_numb, int *count);
bf_packetpath_counter_t *pipe_mgr_tof3_pkt_path_ipb_read_counter(
    int devid, int p, int pg_port, int port_numb, int *count);
#endif
