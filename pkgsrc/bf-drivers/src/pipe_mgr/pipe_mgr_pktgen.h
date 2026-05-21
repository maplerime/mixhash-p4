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

#ifndef _PIPE_MGR_PKTGEN_H
#define _PIPE_MGR_PKTGEN_H

#include <pipe_mgr/pktgen_intf.h>
#include "pipe_mgr_int.h"

/* all chips */
typedef struct pipe_mgr_pg_dev_ctx {
  union {
    struct pipe_mgr_tof_pg_dev_ctx *tof_ctx;
    struct pipe_mgr_tof2_pg_dev_ctx *tof2_ctx;
    struct pipe_mgr_tof3_pg_dev_ctx *tof3_ctx;
  } u;
} pipe_mgr_pg_dev_ctx;

/* Functions */
bf_status_t pipe_mgr_pktgen_port_add(rmt_dev_info_t *dev_info,
                                     bf_dev_port_t port_id,
                                     bf_port_speeds_t speed);
bf_status_t pipe_mgr_pktgen_port_rem(rmt_dev_info_t *dev_info,
                                     bf_dev_port_t port_id);
bf_status_t pipe_mgr_pktgen_add_dev(bf_session_hdl_t shdl, bf_dev_id_t dev);
bf_status_t pipe_mgr_pktgen_rmv_dev(bf_dev_id_t dev);
bf_status_t pipe_mgr_pktgen_warm_init_quick(bf_session_hdl_t shdl,
                                            bf_dev_id_t dev);
void pipe_mgr_pktgen_txn_commit(bf_dev_id_t dev);
void pipe_mgr_pktgen_txn_abort(bf_dev_id_t dev);

uint32_t pipe_mgr_pktgen_get_app_count(bf_dev_id_t dev);
bf_status_t pipe_mgr_pktgen_buffer_write_from_shadow(bf_session_hdl_t shdl,
                                                     bf_dev_target_t dev_tgt);
bf_status_t pipe_mgr_pktgen_create_dma(bf_session_hdl_t shdl,
                                       rmt_dev_info_t *dev_info);
#endif
