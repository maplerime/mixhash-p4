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

#include <PI/target/pi_imp.h>

#include <pkt_mgr/pkt_mgr_intf.h>
#include <pipe_mgr/pipe_mgr_int.h>

#include "pi_log.h"

#define PACKET_MAX_BUFFER_SIZE 65536

static bf_status_t packet_rx_from_cpu_ring(bf_dev_id_t dev_id,
                                           bf_pkt *pkt,
                                           void *cookie,
                                           bf_pkt_rx_ring_t rx_ring) {
  (void)rx_ring;
  (void)cookie;
  bf_pkt *orig_pkt = NULL;
  static char in_packet[PACKET_MAX_BUFFER_SIZE];
  char *pkt_buf, *bufp;
  int packet_size = 0;
  int pkt_len = 0;

  /* save a copy of the original packet */
  orig_pkt = pkt;

  /* assemble the received packet */
  bufp = &in_packet[0];
  do {
    pkt_buf = (char *)bf_pkt_get_pkt_data(pkt);
    pkt_len = bf_pkt_get_pkt_size(pkt);
    if ((packet_size + pkt_len) > PACKET_MAX_BUFFER_SIZE) {
      break;
    }
    memcpy(bufp, pkt_buf, pkt_len);
    bufp += pkt_len;
    packet_size += pkt_len;
    pkt = bf_pkt_get_nextseg(pkt);
  } while (pkt);

  /* free the packet */
  bf_pkt_free(dev_id, orig_pkt);

  LOG_DBG("Packet-in received");
  pi_packetin_receive(dev_id, in_packet, packet_size);

  return BF_SUCCESS;
}

static bf_status_t packet_tx_complete(bf_dev_id_t dev_id,
                                      bf_pkt_tx_ring_t tx_ring,
                                      uint64_t tx_cookie,
                                      uint32_t status) {
  (void)tx_ring;
  (void)status;
  LOG_DBG("Packet-out transmission complete");
  bf_pkt_free(dev_id, (bf_pkt *)tx_cookie);
  return BF_SUCCESS;
}

pi_status_t packet_register_with_pkt_mgr(pi_dev_id_t dev_id) {
  bf_pkt_tx_ring_t tx_ring;
  bf_pkt_rx_ring_t rx_ring;
  bf_status_t status;

  /* register callback for TX complete */
  for (tx_ring = BF_PKT_TX_RING_0; tx_ring < BF_PKT_TX_RING_MAX; tx_ring++) {
    status = bf_pkt_tx_done_notif_register(dev_id, packet_tx_complete, tx_ring);
    if (status != BF_SUCCESS) return PI_STATUS_TARGET_ERROR;
  }

  /* register callback for RX */
  for (rx_ring = BF_PKT_RX_RING_0; rx_ring < BF_PKT_RX_RING_MAX; rx_ring++) {
    status = bf_pkt_rx_register(dev_id, packet_rx_from_cpu_ring, rx_ring, NULL);
    if (status != BF_SUCCESS) return PI_STATUS_TARGET_ERROR;
  }

  LOG_TRACE("Packet-in / packet-out callbacks registered successfully");

  return PI_STATUS_SUCCESS;
}

pi_status_t packet_send_to_pkt_mgr(pi_dev_id_t dev_id,
                                   const char *out_packet,
                                   size_t packet_size) {
  bf_pkt *pkt = NULL;
  bf_pkt_tx_ring_t tx_ring = BF_PKT_TX_RING_0;  // only use this ring for now

  if (bf_pkt_alloc(dev_id, &pkt, packet_size, BF_DMA_CPU_PKT_TRANSMIT_0) != 0)
    return PI_STATUS_ALLOC_ERROR;

  if (bf_pkt_get_pkt_size(pkt) < packet_size) {
    LOG_ERROR("Packet-out too big, not transmitting");
    bf_pkt_free(dev_id, pkt);
    return PI_STATUS_PACKETOUT_SEND_ERROR;
  }

  /* copy the packet buffer and send it */
  memcpy(bf_pkt_get_pkt_data(pkt), out_packet, packet_size);
  if (bf_pkt_tx(dev_id, pkt, tx_ring, (void *)pkt) != BF_SUCCESS) {
    bf_pkt_free(dev_id, pkt);
    return PI_STATUS_PACKETOUT_SEND_ERROR;
  }

  return PI_STATUS_SUCCESS;
}
