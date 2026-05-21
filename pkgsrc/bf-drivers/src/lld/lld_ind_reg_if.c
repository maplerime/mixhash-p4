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
 * @file lld_ind_reg_if.c
 * \brief Details Indirect Register access APIs
 *
 */

/**
 * @addtogroup lld-reg-api
 * @{
 * This is a description of some APIs.
 */

#ifdef __KERNEL__
#define bf_sys_assert()
#endif

#include "dvm/bf_drv_intf.h"
#include "lld/bf_dev_if.h"
#include "lld/lld_err.h"
#include "lld/lld_ind_reg_if_tof.h"
#include "lld/lld_ind_reg_if_tof2.h"
#include "lld/lld_ind_reg_if_tof3.h"

#include "lld.h"
#include "lld_map.h"
#include "lld_dev.h"
#include "lld_dev_lock.h"

/** \brief lld_subdev_ind_write
 *         Write a 42b address using the indirect address registers
 *
 * \param dev_id  : dev_id #
 * \param subdev_id : subdev_id #
 * \param ind_addr: 42b register/memory offset
 * \param data_hi : upper 64-bits to write
 * \param data_lo : lower 64-bits to write
 *
 * \return LLD_OK (0)
 * \return LLD_ERR_BAD_PARM : bad dev_id
 *
 */
lld_err_t lld_subdev_ind_write(bf_dev_id_t dev_id,
                               bf_subdev_id_t subdev_id,
                               uint64_t ind_addr,
                               uint64_t data_hi,
                               uint64_t data_lo) {
  lld_dev_t *dev_p = lld_map_subdev_id_to_dev_p(dev_id, subdev_id);

  if (dev_p == NULL) return LLD_ERR_BAD_PARM;

  /* Since an indirect access requires multiple steps, must lock to make sure
   * accesses from different threads don't get inter-mixed.  */
  lld_dev_lock_ind_reg_lock(dev_id, subdev_id);
  switch (dev_p->dev_family) {
    case BF_DEV_FAMILY_TOFINO:
      lld_ind_write_tof(dev_id, ind_addr, data_hi, data_lo);
      break;
    case BF_DEV_FAMILY_TOFINO2:
      lld_ind_write_tof2(dev_id, ind_addr, data_hi, data_lo);
      break;
    case BF_DEV_FAMILY_TOFINO3:
      lld_ind_write_tof3(dev_id, subdev_id, ind_addr, data_hi, data_lo);
      break;



    default:
      break;
  }
  lld_dev_lock_ind_reg_unlock(dev_id, subdev_id);
  return LLD_OK;
}

/** \brief lld_subdev_ind_read
 *         Read a 42b address using the indirect address registers
 *
 * \param dev_id  : dev_id #
 * \param subdev_id : subdev_id #
 * \param ind_addr: 42b register/memory offset
 * \param data_hi : (returned) upper 64-bits
 * \param data_lo : (returned) lower 64-bits
 *
 * \return LLD_OK (0)
 * \return LLD_ERR_BAD_PARM : bad dev_id
 *
 */
lld_err_t lld_subdev_ind_read(bf_dev_id_t dev_id,
                              bf_subdev_id_t subdev_id,
                              uint64_t ind_addr,
                              uint64_t *data_hi,
                              uint64_t *data_lo) {
  lld_dev_t *dev_p = lld_map_subdev_id_to_dev_p(dev_id, subdev_id);

  if (dev_p == NULL) return LLD_ERR_BAD_PARM;

  /* Since an indirect access requires multiple steps, must lock to make sure
   * accesses from different threads don't get inter-mixed.  */
  lld_dev_lock_ind_reg_lock(dev_id, subdev_id);
  switch (dev_p->dev_family) {
    case BF_DEV_FAMILY_TOFINO:
      lld_ind_read_tof(dev_id, ind_addr, data_hi, data_lo);
      break;
    case BF_DEV_FAMILY_TOFINO2:
      lld_ind_read_tof2(dev_id, ind_addr, data_hi, data_lo);
      break;
    case BF_DEV_FAMILY_TOFINO3:
      lld_ind_read_tof3(dev_id, subdev_id, ind_addr, data_hi, data_lo);
      break;



    default:
      break;
  }
  lld_dev_lock_ind_reg_unlock(dev_id, subdev_id);
  return LLD_OK;
}

/** \brief lld_ind_write
 *         Write a 42b address using the indirect address registers
 *
 * \param dev_id  : dev_id #
 * \param ind_addr: 42b register/memory offset
 * \param data_hi : upper 64-bits to write
 * \param data_lo : lower 64-bits to write
 *
 * \return LLD_OK (0)
 * \return LLD_ERR_BAD_PARM : bad dev_id
 *
 */
lld_err_t lld_ind_write(bf_dev_id_t dev_id,
                        uint64_t ind_addr,
                        uint64_t data_hi,
                        uint64_t data_lo) {
  return lld_subdev_ind_write(dev_id, 0, ind_addr, data_hi, data_lo);
}

/** \brief lld_ind_read
 *         Read a 42b address using the indirect address registers
 *
 * \param dev_id  : dev_id #
 * \param ind_addr: 42b register/memory offset
 * \param data_hi : (returned) upper 64-bits
 * \param data_lo : (returned) lower 64-bits
 *
 * \return LLD_OK (0)
 * \return LLD_ERR_BAD_PARM : bad dev_id
 *
 */
lld_err_t lld_ind_read(bf_dev_id_t dev_id,
                       uint64_t ind_addr,
                       uint64_t *data_hi,
                       uint64_t *data_lo) {
  return lld_subdev_ind_read(dev_id, 0, ind_addr, data_hi, data_lo);
}
