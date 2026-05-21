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

#include <dvm/bf_drv_intf.h>
#include <lld/bf_dma_if.h>
#include <lld/lld_dr_regs.h>
#include <lld/lld_dr_regs_tof.h>
#include <lld/lld_dr_regs_tof2.h>
#include <lld/lld_dr_regs_tof3.h>

#include "lld.h"
#include "lld_dev.h"
#include "lld_map.h"

/********************************************************
 * lld_dr_id_get
 *
 * Return the Tofino offset of a particular DRU.
 *******************************************************/
uint32_t lld_dr_base_get(bf_dev_id_t dev_id, bf_dma_dr_id_t dr_id) {
  if (lld_dev_is_tofino(dev_id)) {
    return lld_dr_tof_base_get(dr_id);
  } else if (lld_dev_is_tof2(dev_id)) {
    return lld_dr_tof2_base_get(dr_id);
  } else if (lld_dev_is_tof3(dev_id)) {
    return lld_dr_tof3_base_get(dr_id);


  }
  return 0;
}

/********************************************************
 * lld_dr_id_get
 *
 * Return the DR that owns the register address passed
 *******************************************************/
bf_dma_dr_id_t lld_dr_id_get(bf_dev_id_t dev_id, uint64_t address) {
  if (lld_dev_is_tofino(dev_id)) {
    return lld_dr_tof_dr_id_get(address);
  } else if (lld_dev_is_tof2(dev_id)) {
    return lld_dr_tof2_dr_id_get(address);
  } else if (lld_dev_is_tof3(dev_id)) {
    return lld_dr_tof3_dr_id_get(address);


  }
  return BF_DMA_MAX_DR;
}

/********************************************************
 * lld_dr_is_dru_reg
 *
 * Return true if the specified address belongs to a
 * DRU.
 *******************************************************/
int lld_dr_is_dru_reg(bf_dev_id_t dev_id, uint64_t address) {
  bf_dma_dr_id_t dr_id;

  dr_id = lld_dr_id_get(dev_id, address);

  return ((dr_id == BF_DMA_MAX_DR) ? false : true);
}
