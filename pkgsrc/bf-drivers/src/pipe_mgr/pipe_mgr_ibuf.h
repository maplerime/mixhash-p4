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

#ifndef __PIPE_MGR_IBUF_H__
#define __PIPE_MGR_IBUF_H__

#include <pipe_mgr/pipe_mgr_intf.h>

pipe_status_t pipe_mgr_ibuf_set_version_bits(pipe_sess_hdl_t sess_hdl,
                                             bf_dev_id_t dev_id,
                                             uint8_t version);

#endif
