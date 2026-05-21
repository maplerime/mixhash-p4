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

#ifndef __MC_MGR_HANDLE_H__
#define __MC_MGR_HANDLE_H__

#include "mc_mgr.h"
#include "mc_mgr_int.h"

/* Session Handles. */
bf_mc_session_hdl_t mc_mgr_encode_sess_hdl(int index);

bool mc_mgr_decode_sess_hdl(bf_mc_session_hdl_t hdl, int *index);

/* MGID Handles. */
bf_mc_mgrp_hdl_t mc_mgr_encode_mgrp_hdl(bf_mc_grp_id_t grp);

bool mc_mgr_decode_mgrp_hdl(bf_mc_mgrp_hdl_t h,
                            bf_mc_grp_id_t *grp,
                            const char *where,
                            const int line);

/* ECMP Handles. */
bf_status_t mc_mgr_encode_ecmp_hdl(bf_dev_id_t dev, bf_mc_ecmp_hdl_t *e);
bf_status_t mc_mgr_decode_ecmp_hdl(bf_mc_ecmp_hdl_t e,
                                   const char *where,
                                   const int line);
bf_status_t mc_mgr_delete_ecmp_hdl(bf_dev_id_t dev, bf_mc_ecmp_hdl_t hdl);

/* Node Handles. */
bf_status_t mc_mgr_encode_l1_node_hdl(bf_dev_id_t dev, bf_mc_node_hdl_t *hdl);

bool mc_mgr_decode_l1_node_hdl(bf_mc_node_hdl_t h,
                               int *id,
                               const char *where,
                               const int line);

bf_status_t mc_mgr_delete_l1_node_hdl(bf_dev_id_t dev, bf_mc_node_hdl_t hdl);
#endif
