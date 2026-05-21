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

#include <stddef.h>

#include <dvm/bf_drv_intf.h>
#include "traffic_mgr/common/tm_error.h"
#include "traffic_mgr/common/tm_ctx.h"
#include "traffic_mgr/hw_intf/tm_tofino_hw_intf.h"

#include <tofino_regs/tofino.h>
#include <lld/bf_dma_if.h>
#include <lld/lld_dr_if.h>

bf_status_t bf_tm_tofinolite_start_init_seq_during_fast_recfg(bf_dev_id_t dev) {
  // This function is supposed to invoke init sequence for tofinolite
  dev &= dev;
  return (BF_SUCCESS);
}

/*
 *  This file implements default parameter values for TM.
 *  Defaults are set at the device init time (device-add)
 */
bf_status_t bf_tm_tofinolite_set_default(bf_dev_id_t dev) {
  dev &= dev;
  return (BF_SUCCESS);
}
