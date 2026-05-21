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

/* This file implements TM buffer management APIs exported to
 * client/application.
 * APIs in this file are the northbound interface to TM.
 */

#include <traffic_mgr/traffic_mgr.h>

#include "traffic_mgr/common/tm_ctx.h"

bf_status_t bf_tm_batch_update_start(bf_dev_id_t dev) {
  BF_TM_INVALID_ARG(TM_IS_DEV_INVALID(dev));
  TM_LOCK(dev, g_tm_ctx[dev]->lock);
  g_tm_ctx[dev]->api_batch_mode = true;
  TM_UNLOCK_AND_FLUSH(dev);
  return (BF_SUCCESS);
}

bf_status_t bf_tm_batch_update_end(bf_dev_id_t dev) {
  BF_TM_INVALID_ARG(TM_IS_DEV_INVALID(dev));
  TM_LOCK(dev, g_tm_ctx[dev]->lock);
  g_tm_ctx[dev]->api_batch_mode = false;
  TM_UNLOCK_AND_FLUSH(dev);
  return (BF_SUCCESS);
}
