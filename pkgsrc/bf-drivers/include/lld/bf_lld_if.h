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

#ifndef BF_LLD_IF_H_INCLUDED
#define BF_LLD_IF_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file bf_lld_if.h
 * \brief Details Device-level APIs.
 *
 */

/**
 * @addtogroup lld-api
 * @{
 * This is a description of some APIs.
 */

typedef bf_status_t (*bf_reg_wr_fn)(bf_dev_id_t dev_id,
                                    bf_subdev_id_t subdev_id,
                                    uint32_t addr,
                                    uint32_t data);
typedef bf_status_t (*bf_reg_rd_fn)(bf_dev_id_t dev_id,
                                    bf_subdev_id_t subdev_id,
                                    uint32_t addr,
                                    uint32_t *data);

bf_status_t bf_lld_bind_wr_fn(bf_reg_wr_fn fn);
bf_status_t bf_lld_bind_rd_fn(bf_reg_rd_fn fn);
bf_status_t bf_lld_init(bool is_master, bf_reg_wr_fn wr_fn, bf_reg_rd_fn rd_fn);

// legacy APIs
bf_status_t bf_bind_wr_fn(bf_reg_wr_fn fn);
bf_status_t bf_bind_rd_fn(bf_reg_rd_fn fn);
char *bf_dbg_get_full_reg_path_name(uint32_t offset);
bool bf_lld_dev_is_tof1(bf_dev_id_t dev_id);
bool bf_lld_dev_is_tof2(bf_dev_id_t dev_id);
bool bf_lld_dev_is_tof3(bf_dev_id_t dev_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // BF_LLD_IF_H_INCLUDED
