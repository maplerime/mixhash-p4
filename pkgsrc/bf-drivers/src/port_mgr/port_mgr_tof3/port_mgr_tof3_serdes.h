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


#ifndef port_mgr_tof3_serdes_h
#define port_mgr_tof3_serdes_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

void port_mgr_aw_access_rd32(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t offset,
                             uint32_t *r_data,
                             const char *fn);

void port_mgr_aw_access_wr32(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             uint32_t offset,
                             uint32_t w_data,
                             const char *fn);
bf_status_t port_mgr_tof3_serdes_clkobs_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            bf_clkobs_pad_t pad,
                                            bf_sds_clkobs_clksel_t clk_src,
                                            int divider);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
