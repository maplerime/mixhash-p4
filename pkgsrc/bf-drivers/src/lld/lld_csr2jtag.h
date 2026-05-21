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

#ifndef LLD_CSR2JTAG_H_INCLUDED
#define LLD_CSR2JTAG_H_INCLUDED

bf_status_t lld_csr2jtag_run_pre_efuse(bf_dev_id_t dev_id,
                                       bf_subdev_id_t subdev_id);
bf_status_t lld_csr2jtag_run_post_efuse(bf_dev_id_t dev_id,
                                        bf_subdev_id_t subdev_id);
bf_status_t lld_csr2jtag_run_one_file(bf_dev_id_t dev_id,
                                      bf_subdev_id_t subdev_id,
                                      const char *fname);
bf_status_t lld_csr2jtag_run_suite(bf_dev_id_t dev_id,
                                   bf_subdev_id_t subdev_id,
                                   const char *fname);

#endif  // LLD_CSR2JTAG_H_INCLUDED
