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

#ifndef _BF_PM_TOF3_UCLI_H
#define _BF_PM_TOF3_UCLI_H

ucli_status_t bf_pm_ucli_ucli__tof3_command_start__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_pcs__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_fec__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_fec_mon__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_int__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_anlt__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_glue__(ucli_context_t *uc);
ucli_status_t bf_pm_ucli_ucli__tof3_port_serdes_debug__(ucli_context_t *uc);
void pm_ucli_tof3_pcs_status(ucli_context_t *uc,
                             bf_dev_id_t dev_id,
                             int aflag,
                             bf_pal_front_port_handle_t *port_hdl);

#endif
