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
 * @file pipe_mgr_tof3_interrupt.h
 * @date
 *
 * Contains definitions of pipe-mgr interrupt handling
 *
 */
#ifndef _PIPE_MGR_TOF3_INTERRUPT_H
#define _PIPE_MGR_TOF3_INTERRUPT_H

/* Module header includes */
#include "pipe_mgr_int.h"
pipe_status_t pipe_mgr_tof3_tcam_read(bf_dev_id_t dev);
/*register interrupt notification*/
pipe_status_t pipe_mgr_tof3_register_mau_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_mirror_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_tm_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_parser_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_deparser_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_parde_misc_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_pgr_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_lfltr_interrupt_notifs(
    rmt_dev_info_t *dev_info);
pipe_status_t pipe_mgr_tof3_register_sbc_interrupt_notifs(bf_dev_id_t dev);

/*interrupt mode set helper*/
pipe_status_t pipe_mgr_tof3_interrupt_en_set_helper(rmt_dev_info_t *dev_info,
                                                    bool enable,
                                                    bool push_now);

#endif
