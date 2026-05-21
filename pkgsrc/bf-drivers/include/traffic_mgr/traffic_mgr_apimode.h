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
 * APIs for client application to program Traffic Manager block to
 * desired QoS behaviour.
 */

#ifndef __TRAFFIC_MGR_APIMODE_H__
#define __TRAFFIC_MGR_APIMODE_H__

#include <traffic_mgr/traffic_mgr_types.h>
#include <bf_types/bf_types.h>

/**
 * @file traffic_mgr_apimode.h
 * \brief This file contains Traffic Manager helper functions
 *        that application/client can make use.
 */

/**
 * @addtogroup tm-api
 * @{
 *  This file contains Traffic Manager helper functions
 *  that application/client can make use.
 */

/**
 * By default traffic manager updates are non batched. All configuration
 * updates are pushed to hardware/asic immediately.  Updates are made
 * efficient by batching writes over DMA to hardware.
 * Batch mode is very useful during system start. Bulk of Traffic Manager
 * configuration during TM initialization can be pushed to hardware in
 * batch mode.
 *
 * Related API : bf_tm_batch_update_end()
 *
 * @param dev      ASIC device identifier.
 * @return         Status of API call.
 */
bf_status_t bf_tm_batch_update_start(bf_dev_id_t dev);

/**
 * All traffic manager configurations that are not pushed to hardware
 * during batch mode processing will be flushed to hardware. Also
 * Traffic Manager update mode is set back to non batch mode.
 * If write bulk mode is desired, application is expected to invoke
 * bf_tm_batch_update_start().
 *
 * Related API : bf_tm_batch_update_start()
 *
 * @param dev      ASIC device identifier.
 * @return         Status of API call.
 */
bf_status_t bf_tm_batch_update_end(bf_dev_id_t dev);

/* @} */

#endif
