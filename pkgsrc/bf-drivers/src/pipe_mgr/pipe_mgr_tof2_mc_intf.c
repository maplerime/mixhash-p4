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

#include "pipe_mgr_int.h"
#include <tof2_regs/tof2_reg_drv.h>

pipe_status_t pipe_mgr_tof2_mc_mgid_grp_addr_get(int mgid_grp,
                                                 uint32_t *tbl0_addr,
                                                 uint32_t *tbl1_addr) {
  /* The two PV tables are each of size 8k by 20bits.  One entry holds the 5 bit
   * mask for 4 MGIDs. */
  uint32_t tbl0_base_addr = offsetof(
      tof2_reg, pipes[0].pardereg.dprsrreg.dprsrreg.inp.iim.pv_table.tbl0[0]);
  uint32_t tbl1_base_addr = offsetof(
      tof2_reg, pipes[0].pardereg.dprsrreg.dprsrreg.inp.iim.pv_table.tbl1[0]);
  *tbl0_addr = tbl0_base_addr + 4 * mgid_grp;
  *tbl1_addr = tbl1_base_addr + 4 * mgid_grp;

  return PIPE_SUCCESS;
}

pipe_status_t pipe_mgr_tof2_mc_copy_to_cpu_pv_addr_get(bf_dev_pipe_t pipe,
                                                       uint32_t *addr) {
  *addr = offsetof(
      tof2_reg,
      pipes[pipe].pardereg.dprsrreg.dprsrreg.inp.ipp.ingr.copy_to_cpu_pv);
  return PIPE_SUCCESS;
}

pipe_status_t pipe_mgr_tof2_mc_pv_table0_addr_get(bf_dev_pipe_t pipe,
                                                  int mgid_grp,
                                                  uint32_t *addr) {
  *addr = offsetof(
      tof2_reg,
      pipes[pipe].pardereg.dprsrreg.dprsrreg.inp.iim.pv_table.tbl0[mgid_grp]);
  return PIPE_SUCCESS;
}

pipe_status_t pipe_mgr_tof2_mc_yid_tbl_addr_get(uint32_t *addr0,
                                                uint32_t *addr1,
                                                uint32_t *addr2,
                                                uint32_t *addr3) {
  *addr0 = offsetof(
      tof2_reg,
      pipes[0].pardereg.dprsrreg.dprsrreg.ho_i[0].hir.meta.pre_version);
  *addr1 = offsetof(
      tof2_reg,
      pipes[0].pardereg.dprsrreg.dprsrreg.ho_i[1].hir.meta.pre_version);
  *addr2 = offsetof(
      tof2_reg,
      pipes[0].pardereg.dprsrreg.dprsrreg.ho_i[2].hir.meta.pre_version);
  *addr3 = offsetof(
      tof2_reg,
      pipes[0].pardereg.dprsrreg.dprsrreg.ho_i[3].hir.meta.pre_version);
  return PIPE_SUCCESS;
}
