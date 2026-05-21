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

#ifndef __PIPE_MGR_TOF3_MC_INTF_H__
#define __PIPE_MGR_TOF3_MC_INTF_H__

pipe_status_t pipe_mgr_tof3_mc_copy_to_cpu_tvt_addr_get(bf_dev_pipe_t pipe,
                                                        uint32_t *addr);

pipe_status_t pipe_mgr_tof3_mc_tvt_c2c_mask_addr_get(bf_dev_pipe_t pipe,
                                                     uint32_t *addr);

pipe_status_t pipe_mgr_tof3_mc_pv_table0_addr_get(bf_dev_pipe_t pipe,
                                                  int mgid_grp,
                                                  uint32_t *addr);

pipe_status_t pipe_mgr_tof3_mc_yid_tbl_addr_get(uint32_t *addr0,
                                                uint32_t *addr1,
                                                uint32_t *addr2,
                                                uint32_t *addr3);

pipe_status_t pipe_mgr_tof3_mc_tvt_table0_addr_get(bf_dev_pipe_t phy_pipe,
                                                   int mgid_tvt_grp,
                                                   uint32_t *addr);
#endif
