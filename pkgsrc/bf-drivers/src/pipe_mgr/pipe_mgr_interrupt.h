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

/*!
 * @file pipe_mgr_interrupt.h
 * @date
 *
 * Contains definitions of pipe-mgr interrupt handling
 *
 */
#ifndef _PIPE_MGR_INTERRUPT_H
#define _PIPE_MGR_INTERRUPT_H

/* Module header includes */
#include <pipe_mgr/pipe_mgr_intf.h>
#include <target-sys/bf_sal/bf_sys_log.h>
#include <target-sys/bf_sal/bf_sys_timer.h>
#include "pipe_mgr_int.h"

/* ------ FUNCTIONS -------- */
/**
 * The function is used to set the error interrupt mode
 *
 * @param  dev                   ASIC device identifier
 * @param  enable                Enable/Disable interrupt mode
 * @return                       Status of the API call
 */
bf_status_t pipe_mgr_err_interrupt_mode_set(bf_dev_id_t dev, bool enable);

/**
 * The function is used to register interrupt notifications
 *
 * @param  dev                  ASIC device identifier
 * @return                       Status of the API call
 */
pipe_status_t pipe_mgr_register_interrupt_notifs(rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_enable_interrupt_notifs(rmt_dev_info_t *dev_info);

pipe_status_t pipe_mgr_intr_init(rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_intr_start_scrub_timer(bf_dev_id_t dev_id);
pipe_status_t pipe_mgr_intr_cleanup(bf_dev_id_t dev);

/**
 * The function is used to dump all error events
 *
 * @param  uc                    ucli handle
 * @param  dev                   ASIC device identifier
 * @param  n                     Display n most recent events, -1 for all
 * @return                       None
 */
void pipe_mgr_err_evt_log_dump(ucli_context_t *uc, bf_dev_id_t dev_id, int n);
#endif
