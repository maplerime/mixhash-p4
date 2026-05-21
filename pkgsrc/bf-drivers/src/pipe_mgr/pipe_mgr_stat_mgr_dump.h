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
 * @file pipe_mgr_adt_mgr_dump.h
 * @date
 *
 * Action data table manager dump definitions to dump debug information.
 */

#ifndef _PIPE_MGR_STAT_MGR_DUMP_H
#define _PIPE_MGR_STAT_MGR_DUMP_H

/* Standard header includes */

/* Module header includes */

/* Local header includes */

#include <pipe_mgr/pipe_mgr_intf.h>

#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

void pipe_mgr_stat_dump_tbl_info(ucli_context_t *uc,
                                 bf_dev_id_t device_id,
                                 pipe_stat_tbl_hdl_t stat_tbl_hdl);

void pipe_mgr_stat_dump_entry_info(ucli_context_t *uc,
                                   bf_dev_id_t device_id,
                                   bf_dev_pipe_t pipe_id,
                                   pipe_stat_tbl_hdl_t stat_tbl_hdl,
                                   pipe_stat_ent_idx_t stat_ent_idx);

#endif  // _PIPE_MGR_STAT_MGR_DUMP_H
