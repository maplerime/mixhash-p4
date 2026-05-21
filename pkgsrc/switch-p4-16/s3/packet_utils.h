/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2022 Intel Corporation
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

#ifndef S3_PACKET_UTILS_H__
#define S3_PACKET_UTILS_H__

#define PACKED __attribute__((__packed__))
#define ETH_ADDR_LEN 6

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** ethernet header */
typedef struct PACKED switch_ethernet_header_s {
  /** destination mac */
  uint8_t dst_mac[ETH_ADDR_LEN];

  /** source mac */
  uint8_t src_mac[ETH_ADDR_LEN];

  /** ethernet type */
  uint16_t ether_type;
} switch_ethernet_header_t;

/** fabric header */
typedef struct PACKED switch_fabric_header_s {
  /** fabric header ethertype */
  uint16_t ether_type;

#if defined(__LITTLE_ENDIAN_BITFIELD)
  /** padding */
  uint8_t pad1 : 1;

  /** packet version */
  uint8_t packet_version : 2;

  /** header version */
  uint8_t header_version : 2;

  /** header type - cpu/unicast/multicast */
  uint8_t packet_type : 3;

  /** packet color */
  uint8_t fabric_color : 3;

  /** qos value */
  uint8_t fabric_qos : 5;
#elif defined(__BIG_ENDIAN_BITFIELD)
  /** header type - cpu/unicast/multicast */
  uint8_t packet_type : 3;

  /** header version */
  uint8_t header_version : 2;

  /** packet version */
  uint8_t packet_version : 2;

  /** padding */
  uint8_t pad1 : 1;

  /** qos value */
  uint8_t fabric_qos : 5;

  /** packet color */
  uint8_t fabric_color : 3;
#else
#error "Please fix <asm/byteorder.h>"
#endif

  /** device id */
  uint8_t dst_device;
} switch_fabric_header_t;

/** cpu header */
typedef struct PACKED switch_cpu_header_s {
#if defined(__LITTLE_ENDIAN_BITFIELD)
  /** egress queue id */
  uint8_t egress_queue : 5;

  /** reserved */
  uint8_t reserved : 1;

  /** capture departure time */
  uint8_t capture_tstamp_on_tx : 1;

  /** tx bypass */
  uint8_t tx_bypass : 1;
#elif defined(__BIG_ENDIAN_BITFIELD)
  /** tx bypass */
  uint8_t tx_bypass : 1;

  /** capture departure time */
  uint8_t capture_tstamp_on_tx : 1;

  /** reserved */
  uint8_t reserved : 1;

  /** egress queue id */
  uint8_t egress_queue : 5;
#else
#error "Please fix <asm/byteorder.h>"
#endif

  /** ingress port */
  uint16_t ingress_port;

  /** ingress/egress port_lag_index */
  uint16_t port_lag_index;

  /** ingress bridge domain */
  uint16_t ingress_bd;

  /**
   * rx - reason code
   * tx - tx bypass flags
   */
  uint16_t reason_code;
} switch_cpu_header_t;

typedef struct PACKED switch_packet_header_s {
  /** fabric header */
  switch_fabric_header_t fabric_header;

  /** cpu header */
  switch_cpu_header_t cpu_header;
} switch_packet_header_t;

/** vlan header */
typedef struct PACKED switch_vlan_header_s {
#if defined(__LITTLE_ENDIAN_BITFIELD)
  /** vlan id */
  uint16_t vid : 12;

  /** format indicator */
  uint16_t dei : 1;

  /** priority */
  uint16_t pcp : 3;
#elif defined(__BIG_ENDIAN_BITFIELD)
  /** priority */
  uint16_t pcp : 3;

  /** format indicator */
  uint16_t dei : 1;

  /** vlan id */
  uint16_t vid : 12;
#else
#error "Please fix <asm/byteorder.h>"
#endif
  /** vlan protocol id */
  uint16_t tpid;
} switch_vlan_header_t;

#ifdef __cplusplus
}
#endif

#endif
