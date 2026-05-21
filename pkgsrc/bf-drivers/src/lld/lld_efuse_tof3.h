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

#ifndef LLD_EFUSE_TOF3_H_INCLUDED
#define LLD_EFUSE_TOF3_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

int lld_efuse_tof3_load(bf_dev_id_t dev_id,
                        bf_subdev_id_t subdev_id,
                        bf_dev_init_mode_t warm_init_mode);
void lld_efuse_tof3_wafer_str_get(bf_dev_id_t dev_id,
                                  bf_subdev_id_t subdev_id,
                                  int s_len,
                                  char *s);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // LLD_EFUSE_TOF3_H_INCLUDED
