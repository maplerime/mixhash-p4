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

#ifndef _TOFINO_PDFIXED_PD_PKT_H
#define _TOFINO_PDFIXED_PD_PKT_H

/**
 * @file pd_pkt.h
 *
 * @brief PD Fixed APIs for packet manager.
 *
 */

#include <tofino/pdfixed/pd_common.h>

/**
 * @brief Initialize the packet manager service
 *
 * @return Satus of the API call
 *
 */
p4_pd_status_t pkt_pd_init(void);

/**
 * @brief Cleanup the packet manager service
 *
 * @return None
 *
 */
void pkt_pd_cleanup(void);

/**
 * @brief Create a packet manager session
 *
 * @param[out] sess_hdl Pointer to return the session handle
 *
 * @return Status of the API call
 *
 */
p4_pd_status_t pkt_pd_client_init(p4_pd_sess_hdl_t *sess_hdl);

/**
 * @brief Destroy the packet manager session
 *
 * @param[in] sess_hdl Session handle
 *
 * @return Status of the API call
 *
 */
p4_pd_status_t pkt_pd_client_cleanup(p4_pd_sess_hdl_t sess_hdl);

/**
 * @brief Transmit a packet
 *
 * @param[in] shdl Session handle
 * @param[in] buf Pointer to the packet buffer that holds the packet
 * @param[in] size Size of the packet
 * @param[in] tx_ring TX DR ring to be used to transmit the packet
 *
 * @return Status of the API call
 *
 */
p4_pd_status_t pkt_pd_pkt_tx(p4_pd_sess_hdl_t shdl,
                             const uint8_t *buf,
                             uint32_t size,
                             uint32_t tx_ring);

/**
 * @brief Transmit a packet but skip packet padding in case packet
 * size is too small (< 64B)
 *
 * @param[in] shdl Session handle
 * @param[in] buf Pointer to the packet buffer that holds the packet
 * @param[in] size Size of the packet
 * @param[in] tx_ring TX DR ring to be used to transmit the packet
 *
 * @return Status of the API call
 *
 */
p4_pd_status_t pkt_pd_pkt_tx_skip_padding(p4_pd_sess_hdl_t shdl,
                                          const uint8_t *buf,
                                          uint32_t size,
                                          uint32_t tx_ring);

/**
 * @brief Verify if completion callback is received for all transmitted packets
 *
 * @param[in] shdl Session handle
 *
 * @return Status of the API call
 *
 */
p4_pd_status_t pkt_pd_verify_tx(p4_pd_sess_hdl_t shdl);

/**
 * @brief Receive a packet
 *
 * @param[in] shdl Session handle
 * @param[out] buf Pointer to the packet buffer to receive the packet
 * @param[out] size Size of the packet
 * @param[out] rx_ring RX DR ring used to receive the packet
 *
 * @return Status of the API call
 *
 */
void pkt_pd_get_rx(p4_pd_sess_hdl_t shdl,
                   char *buf,
                   uint32_t *size,
                   uint32_t *rx_ring);

#endif
