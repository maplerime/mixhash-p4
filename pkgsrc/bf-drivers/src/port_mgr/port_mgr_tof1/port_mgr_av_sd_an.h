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

#ifndef port_mgr_av_sd_an_h_included
#define port_mgr_av_sd_an_h_included

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include <avago/aapl.h>

Avago_serdes_an_config_t *port_mgr_av_sd_an_config_construct(Aapl_t *aapl);
void port_mgr_av_sd_an_config_destruct(Aapl_t *aapl,
                                       Avago_serdes_an_config_t *config);

uint32_t port_mgr_av_sd_an_start(Aapl_t *aapl,
                                 uint32_t addr,
                                 Avago_serdes_an_config_t *config);
uint32_t port_mgr_av_sd_an_read_an_status(Aapl_t *aapl,
                                          uint32_t addr,
                                          Avago_serdes_an_status_t status);
uint32_t port_mgr_av_sd_an_next_page_transmit(Aapl_t *aapl,
                                              uint32_t addr,
                                              const char *data_buf);
uint32_t port_mgr_av_sd_an_assert_link_status(Aapl_t *aapl,
                                              uint32_t addr,
                                              Avago_serdes_an_config_t *config);
uint32_t port_mgr_av_sd_an_assert_link_status_after_an_good(Aapl_t *aapl,
                                                            uint32_t addr,
                                                            uint32_t hcd);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // port_mgr_av_sd_an_h_included
