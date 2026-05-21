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

#ifndef _PIPE_MGR_PKTGEN_COMM_H
#define _PIPE_MGR_PKTGEN_COMM_H

#define PIPE_MGR_PKT_BUFFER_MEM_ROWS 1024
#define PIPE_MGR_PKT_BUFFER_WIDTH 16
#define PIPE_MGR_PKT_BUFFER_SIZE \
  (PIPE_MGR_PKT_BUFFER_MEM_ROWS * PIPE_MGR_PKT_BUFFER_WIDTH)
#define PIPE_MGR_PKTGEN_SRC_PRT_MAX 127
struct pkt_buffer_shadow_t {
  uint8_t data[PIPE_MGR_PKT_BUFFER_SIZE];
  uint8_t txn_data[PIPE_MGR_PKT_BUFFER_SIZE];
  bool txn_data_valid;
};

int pg_log_pipe_mask(bf_dev_target_t dev_tgt);
bf_status_t pg_write_one_pipe_reg(pipe_sess_hdl_t sid,
                                  bf_dev_id_t dev,
                                  uint8_t pm,
                                  uint32_t addr,
                                  uint32_t data);

#endif
