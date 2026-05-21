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

/**
 * @file lld_reg_if.c
 * \brief Details Register access APIs.
 *
 */

/**
 * @addtogroup lld-reg-api
 * @{
 * This is a description of some APIs.
 */

#ifndef __KERNEL__
#include <inttypes.h>
#else
#define bf_sys_assert()
#endif

#include <dvm/bf_drv_intf.h>
#include <lld/bf_dev_if.h>
#include "lld.h"
#include "lld_map.h"
#include "lld_dev.h"

/** \brief lld_subdev_wr
 *         Write a 32b device register via the PCIe interface
 *
 * \param dev_id: dev_id #
 * \param subdev_id: sub device # within dev_id
 * \param reg   : Tofino register offset
 * \param data  : 32-bit value to write to reg
 *
 * \return LLD_OK (0)
 * \return LLD_ERR_BAD_PARM : bad chip
 * \return LLD_ERR_BAD_PARM : bad sub device id
 * \return LLD_ERR_BAD_PARM : register not 32-bit aligned
 *
 */
int lld_subdev_wr(bf_dev_id_t dev_id,
                  bf_subdev_id_t subdev_id,
                  uint32_t reg,
                  uint32_t data)

{
  lld_dev_t *dev_p = lld_map_subdev_id_to_dev_p(dev_id, subdev_id);

  if (dev_p == NULL) return LLD_ERR_BAD_PARM;
  if (reg & 0x3) return LLD_ERR_BAD_PARM;

  lld_ctx->wr_fn(dev_id, subdev_id, reg, data);
  return LLD_OK;
}

/** \brief lld_subdev_rd
 *         Read a 32b device register via the PCIe interface
 *
 * \param dev_id: dev_id #
 * \param subdev_id: sub device # within dev_id
 * \param reg   : Tofino register offset
 * \param val   : Pointer where register contents will be written
 *
 * \return LLD_OK (0)
 * \return LLD_ERR_BAD_PARM : bad dev_id
 * \return LLD_ERR_BAD_PARM : bad sub device id
 * \return LLD_ERR_BAD_PARM : register not 32-bit aligned
 * \return LLD_ERR_BAD_PARM : return value ptr NULL
 *
 */
int lld_subdev_rd(bf_dev_id_t dev_id,
                  bf_subdev_id_t subdev_id,
                  uint32_t reg,
                  uint32_t *val) {
  lld_dev_t *dev_p = lld_map_subdev_id_to_dev_p(dev_id, subdev_id);

  if (dev_p == NULL) return LLD_ERR_BAD_PARM;
  if (reg & 0x3) return LLD_ERR_BAD_PARM;
  if (!val) return LLD_ERR_BAD_PARM;

  lld_ctx->rd_fn(dev_id, subdev_id, reg, val);

  return LLD_OK;
}
