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

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_sku.h>
#include <lld/lld_efuse.h>
#include <tof3_regs/tof3_reg_drv.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_physical_dev.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof3_map.h"
#include "port_mgr_tof3_tmac.h"
#include "port_mgr_tof3_port.h"
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_port.h>
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_logical_port.h>
//#include "port_mgr_tof3_microp.h"
#include "tmac_access.h"
#include "tof3_eth400g_mac_rspec_access.h"
#include "tof3_eth400g_sys_rspec_access.h"
#include "tof3_eth400g_app_rspec_access.h"
#include "tof3-autogen-required-headers.h"
#include <lld/bf_dev_if.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_interrupt_if.h>

extern port_mgr_port_t *port_mgr_map_dev_port_to_port(bf_dev_id_t dev_id,
                                                      uint32_t port);
// Set to "true" to program thru tv80
static bool use_tv80_for_tmac_access = false;
static bool microp_init_done = false;
bool port_mgr_tof3_tmac_is_cpu_port(bf_dev_id_t dev_id, uint32_t tmac);
bf_status_t port_mgr_tof3_tmac_serdes_lane_map_set(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t mac, uint32_t tmac,
    uint32_t ch, bf_port_speed_t speed, uint32_t n_lanes, bool en_log,
    bool cfg);
bf_status_t port_mgr_tof3_tmac_even_serdes_lane_map_set(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t mac, uint32_t tmac,
    uint32_t ch, bf_port_speed_t speed, uint32_t n_lanes, bool en_log,
    bool cfg);

static uint32_t tmac_initial_ch_seq[4] = {3, 1, 2, 0};

static uint32_t mac_ch_used[BF_MAX_DEV_COUNT][66][4] = {{{0}}};
static bf_sys_mutex_t tmac_stats_mtx[BF_MAX_DEV_COUNT];

static bool show_mux_use = false;
typedef struct _mux_ {
  uint32_t llane[8];
  uint32_t mux_idx[8];
} mux_t;

mux_t tx_mux_used[BF_MAX_DEV_COUNT][66];
mux_t rx_mux_used[BF_MAX_DEV_COUNT][66];

#ifdef DEVICE_IS_EMULATOR
static int skip_rmon_tmac = 0;
#endif

static uint32_t tmac_rmon_ctrs_map[][2] = {
    {bf_mac_stat_FramesReceivedOK, tmac_RxPacketOK},
    {bf_mac_stat_FramesReceivedAll, tmac_RxPacketAll},
    {bf_mac_stat_FramesReceivedwithFCSError, tmac_RxPacketErrFCS},
    {bf_mac_stat_FrameswithanyError, tmac_RxPacketErr},
    {bf_mac_stat_OctetsReceivedinGoodFrames, tmac_RxOctetOK},
    {bf_mac_stat_OctetsReceived, tmac_RxOctetAll},
    {bf_mac_stat_FramesReceivedwithUnicastAddresses, tmac_RxPacketUnicast},
    {bf_mac_stat_FramesReceivedwithMulticastAddresses, tmac_RxPacketMulticast},
    {bf_mac_stat_FramesReceivedwithBroadcastAddresses, tmac_RxPacketBroadcast},
    {bf_mac_stat_FramesReceivedoftypePAUSE, tmac_RxPacketPause},
    {bf_mac_stat_FramesReceivedwithLengthError, tmac_RxPacketErrLength},
    {bf_mac_stat_FramesReceivedUndersized, tmac_RxPacketErrShort},
    {bf_mac_stat_FramesReceivedOversized, tmac_Reserved},
    {bf_mac_stat_FragmentsReceived, tmac_Reserved},
    {bf_mac_stat_JabberReceived, tmac_Reserved},
    {bf_mac_stat_PriorityPauseFrames, tmac_RxPacketPortXOFF},
    {bf_mac_stat_CRCErrorStomped, tmac_Reserved},
    {bf_mac_stat_FrameTooLong, tmac_RxPacketErrLong},
    {bf_mac_stat_RxVLANFramesGood, tmac_RxPacketVLAN},
    {bf_mac_stat_FramesDroppedBufferFull, tmac_Reserved},
    {bf_mac_stat_FramesReceivedLength_lt_64, tmac_RxPacketLessThan64B},
    {bf_mac_stat_FramesReceivedLength_eq_64, tmac_RxPacketEqualTo64B},
    {bf_mac_stat_FramesReceivedLength_65_127, tmac_RxPacket65BTo127B},
    {bf_mac_stat_FramesReceivedLength_128_255, tmac_RxPacket128BTo255B},
    {bf_mac_stat_FramesReceivedLength_256_511, tmac_RxPacket256BTo511B},
    {bf_mac_stat_FramesReceivedLength_512_1023, tmac_RxPacket512BTo1023B},
    {bf_mac_stat_FramesReceivedLength_1024_1518, tmac_RxPacket1024BTo1518B},
    {bf_mac_stat_FramesReceivedLength_1519_2047, tmac_RxPacket1519BTo2047B},
    {bf_mac_stat_FramesReceivedLength_2048_4095, tmac_RxPacket2048BTo4095B},
    {bf_mac_stat_FramesReceivedLength_4096_8191, tmac_RxPacket4096BTo8191B},
    {bf_mac_stat_FramesReceivedLength_8192_9215, tmac_RxPacket8192BTo9215B},
    {bf_mac_stat_FramesReceivedLength_9216, tmac_RxPacketGreaterThan9216B},
    {bf_mac_stat_FramesTransmittedOK, tmac_TxPacketOK},
    {bf_mac_stat_FramesTransmittedAll, tmac_TxPacketAll},
    {bf_mac_stat_FramesTransmittedwithError, tmac_TxPacketErr},
    {bf_mac_stat_OctetsTransmittedwithouterror, tmac_TxOctetOK},
    {bf_mac_stat_OctetsTransmittedTotal, tmac_TxOctetAll},
    {bf_mac_stat_FramesTransmittedUnicast, tmac_TxPacketUnicast},
    {bf_mac_stat_FramesTransmittedMulticast, tmac_TxPacketMulticast},
    {bf_mac_stat_FramesTransmittedBroadcast, tmac_TxPacketBroadcast},
    {bf_mac_stat_FramesTransmittedPause, tmac_TxPacketPause},
    {bf_mac_stat_FramesTransmittedPriPause, tmac_Reserved},
    {bf_mac_stat_FramesTransmittedVLAN, tmac_TxPacketVLAN},
    {bf_mac_stat_FramesTransmittedLength_lt_64, tmac_TxPacketLessThan64B},
    {bf_mac_stat_FramesTransmittedLength_eq_64, tmac_TxPacketEqualTo64B},
    {bf_mac_stat_FramesTransmittedLength_65_127, tmac_TxPacket65BTo127B},
    {bf_mac_stat_FramesTransmittedLength_128_255, tmac_TxPacket128BTo255B},
    {bf_mac_stat_FramesTransmittedLength_256_511, tmac_TxPacket256BTo511B},
    {bf_mac_stat_FramesTransmittedLength_512_1023, tmac_TxPacket512BTo1023B},
    {bf_mac_stat_FramesTransmittedLength_1024_1518, tmac_TxPacket1024BTo1518B},
    {bf_mac_stat_FramesTransmittedLength_1519_2047, tmac_TxPacket1519BTo2047B},
    {bf_mac_stat_FramesTransmittedLength_2048_4095, tmac_TxPacket2048BTo4095B},
    {bf_mac_stat_FramesTransmittedLength_4096_8191, tmac_TxPacket4096BTo8191B},
    {bf_mac_stat_FramesTransmittedLength_8192_9215, tmac_TxPacket8192BTo9215B},
    {bf_mac_stat_FramesTransmittedLength_9216, tmac_TxPacketGreaterThan9216B},
    {bf_mac_stat_Pri0FramesTransmitted, tmac_TxPacketPFC0XOFF},
    {bf_mac_stat_Pri1FramesTransmitted, tmac_TxPacketPFC1XOFF},
    {bf_mac_stat_Pri2FramesTransmitted, tmac_TxPacketPFC2XOFF},
    {bf_mac_stat_Pri3FramesTransmitted, tmac_TxPacketPFC3XOFF},
    {bf_mac_stat_Pri4FramesTransmitted, tmac_TxPacketPFC4XOFF},
    {bf_mac_stat_Pri5FramesTransmitted, tmac_TxPacketPFC5XOFF},
    {bf_mac_stat_Pri6FramesTransmitted, tmac_TxPacketPFC6XOFF},
    {bf_mac_stat_Pri7FramesTransmitted, tmac_TxPacketPFC7XOFF},
    {bf_mac_stat_Pri0FramesReceived, tmac_RxPacketPFC0XOFF},
    {bf_mac_stat_Pri1FramesReceived, tmac_RxPacketPFC1XOFF},
    {bf_mac_stat_Pri2FramesReceived, tmac_RxPacketPFC2XOFF},
    {bf_mac_stat_Pri3FramesReceived, tmac_RxPacketPFC3XOFF},
    {bf_mac_stat_Pri4FramesReceived, tmac_RxPacketPFC4XOFF},
    {bf_mac_stat_Pri5FramesReceived, tmac_RxPacketPFC5XOFF},
    {bf_mac_stat_Pri6FramesReceived, tmac_RxPacketPFC6XOFF},
    {bf_mac_stat_Pri7FramesReceived, tmac_RxPacketPFC7XOFF},
    {bf_mac_stat_TransmitPri0Pause1USCount, tmac_TxTime1USPFC0XOFF},
    {bf_mac_stat_TransmitPri1Pause1USCount, tmac_TxTime1USPFC1XOFF},
    {bf_mac_stat_TransmitPri2Pause1USCount, tmac_TxTime1USPFC2XOFF},
    {bf_mac_stat_TransmitPri3Pause1USCount, tmac_TxTime1USPFC3XOFF},
    {bf_mac_stat_TransmitPri4Pause1USCount, tmac_TxTime1USPFC4XOFF},
    {bf_mac_stat_TransmitPri5Pause1USCount, tmac_TxTime1USPFC5XOFF},
    {bf_mac_stat_TransmitPri6Pause1USCount, tmac_TxTime1USPFC6XOFF},
    {bf_mac_stat_TransmitPri7Pause1USCount, tmac_TxTime1USPFC7XOFF},
    {bf_mac_stat_ReceivePri0Pause1USCount, tmac_RxTime1USPFC0XOFF},
    {bf_mac_stat_ReceivePri1Pause1USCount, tmac_RxTime1USPFC1XOFF},
    {bf_mac_stat_ReceivePri2Pause1USCount, tmac_RxTime1USPFC2XOFF},
    {bf_mac_stat_ReceivePri3Pause1USCount, tmac_RxTime1USPFC3XOFF},
    {bf_mac_stat_ReceivePri4Pause1USCount, tmac_RxTime1USPFC4XOFF},
    {bf_mac_stat_ReceivePri5Pause1USCount, tmac_RxTime1USPFC5XOFF},
    {bf_mac_stat_ReceivePri6Pause1USCount, tmac_RxTime1USPFC6XOFF},
    {bf_mac_stat_ReceivePri7Pause1USCount, tmac_RxTime1USPFC7XOFF},
    {bf_mac_stat_ReceiveStandardPause1USCount, tmac_RxTime1USPortXOFF},
    {bf_mac_stat_FramesTruncated, tmac_RxSFDError},
};

static inline void set_sds_mux(uint32_t *csr, uint32_t value, uint32_t idx) {
  *((uint32_t *)csr) =
      ((value & 0xFul) << (idx * 4)) |
      (*((uint32_t *)csr) & (~(((uint32_t)0xFul) << (idx * 4))));
}

static inline void set_tx_mux_used(bf_dev_id_t dev_id, uint32_t mac,
                                   uint32_t val, uint32_t idx, bool cfg) {
  if (cfg) {
    tx_mux_used[dev_id][mac].llane[val] = 0xF;
    tx_mux_used[dev_id][mac].mux_idx[idx] = 0xF;
  } else {
    tx_mux_used[dev_id][mac].llane[val] = 0x0;
    tx_mux_used[dev_id][mac].mux_idx[idx] = 0x0;
  }
}

uint32_t show_tx_mux_used_val(bf_dev_id_t dev_id, uint32_t mac) {
  uint32_t i;
  if (mac > 65)
    return 0;

  printf("tx-used mac %d --\n", mac);
  for (i = 0; i < 8; i++) {
    printf("idx %d tx-ll-used 0x%0x sds-used 0x%0x\n", i,
           tx_mux_used[dev_id][mac].llane[i],
           tx_mux_used[dev_id][mac].mux_idx[i]);
  }
  return 0;
}

uint32_t get_tx_mux_unused_val(bf_dev_id_t dev_id, uint32_t mac) {
  uint32_t i;
  if (mac > 65)
    return 0xf;

  for (i = 0; i < 8; i++) {
    if (tx_mux_used[dev_id][mac].llane[i] != 0xF) {
      return i;
    }
  }
  return 0xf; // nothing found
}

static inline void set_rx_mux_used(bf_dev_id_t dev_id, uint32_t mac,
                                   uint32_t val, uint32_t idx, bool cfg) {
  if ((mac > 65) || (val > 7) || (idx > 7))
    return;
  if (cfg) {
    rx_mux_used[dev_id][mac].llane[val] = 0xF;
    rx_mux_used[dev_id][mac].mux_idx[idx] = 0xF;
  } else {
    rx_mux_used[dev_id][mac].llane[val] = 0x0;
    rx_mux_used[dev_id][mac].mux_idx[idx] = 0x0;
  }
}

uint32_t show_rx_mux_used_val(bf_dev_id_t dev_id, uint32_t mac) {
  uint32_t i;
  if (mac > 65)
    return 0;

  printf("rx-used mac %d --\n", mac);
  for (i = 0; i < 8; i++) {
    printf("idx %d rx-ll-used 0x%0x sds-used 0x%0x\n", i,
           rx_mux_used[dev_id][mac].llane[i],
           rx_mux_used[dev_id][mac].mux_idx[i]);
  }

  return 0;
}

uint32_t get_rx_mux_unused_val(bf_dev_id_t dev_id, uint32_t mac) {
  uint32_t i;
  if (mac > 65)
    return 0xf;

  for (i = 0; i < 8; i++) {
    if (rx_mux_used[dev_id][mac].llane[i] != 0xF) {
      return i;
    }
  }
  return 0xf; // nothing found
}

static void find_and_rpl_dup(bf_dev_id_t dev_id, uint32_t mac, uint32_t *tx_map,
                             uint32_t *rx_map) {
  uint32_t i, tval, rval;

  if ((!rx_map) || (!tx_map) || (mac > 65))
    return;

  for (i = 0; i < 8; i++) {
    if (tx_mux_used[dev_id][mac].mux_idx[i] != 0xF) {
      tval = get_tx_mux_unused_val(dev_id, mac);
      if (tval < 8) {
        set_sds_mux(tx_map, tval, i);
      }
    }
    if (rx_mux_used[dev_id][mac].mux_idx[i] != 0xF) {
      rval = get_rx_mux_unused_val(dev_id, mac);
      if (rval < 8) {
        set_sds_mux(rx_map, rval, i);
      }
    }
  }
}

static bool tmac_is_running_on_model(bf_dev_id_t dev_id) {
  bool is_sw_model = false;

  bf_drv_device_type_get(dev_id, &is_sw_model);

  return is_sw_model;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_channel_add
 *
 * Update ch_used for the given tmac and channel(s)
 ****************************************************************************/
static void port_mgr_tof3_tmac_channel_add(bf_dev_id_t dev_id, uint32_t mac,
                                           uint32_t ch, uint32_t n_ch) {
  mac_ch_used[dev_id][mac][ch] = n_ch;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_channel_del
 *
 * Update ch_used for the given tmac and channel(s)
 ****************************************************************************/
static void port_mgr_tof3_tmac_channel_del(bf_dev_id_t dev_id, uint32_t mac,
                                           uint32_t ch, uint32_t n_ch) {
  uint32_t ch_idx;

  for (ch_idx = ch; ch_idx < (ch + n_ch); ch_idx++) {
    mac_ch_used[dev_id][mac][ch_idx] = 0;
  }
}

/*****************************************************************************
 * port_mgr_tof3_tmac_channel_get
 *
 * Return the # of tmac channels required by the port defined on "ch"
 ****************************************************************************/
static uint32_t port_mgr_tof3_tmac_channel_get(bf_dev_id_t dev_id, uint32_t mac,
                                               uint32_t ch) {
  return mac_ch_used[dev_id][mac][ch];
}

/*****************************************************************************
 * port_mgr_tof3_tmac_chnl_seq_calc
 *
 * Calculate the channel sequence based on all defined ports on this tmacs
 * 8x channels.
 *
 * We maintain a map of ports to channels,
 *   CH_USED_4 = 3
 *   CH_USED_2 = 1
 *   CH_USED_1 = 2
 *   CH_USED_0 = 0
 *
 * INITIAL_CH_CFG = [3,,1,2,0]
 *
 * Algorithm:
 *  chnl_seq = INITIAL_CH_CFG
 *  foreach ch {
 *    if ch_used[ch] != CH_USED_0
 *      for slot = 0 to 7
 *        if (chnl_seq[slot] >= ch) and (chnl_seq[slot] < (ch + ch_used[ch])
 *          chnl_seq[slot] = ch
 ****************************************************************************/
static uint32_t port_mgr_tof3_tmac_chnl_seq_calc(bf_dev_id_t dev_id,
                                                 uint32_t mac, uint32_t ch) {
  uint32_t c, chnl_seq;
  uint32_t n_ch;
  uint32_t ch_seq[4];

  for (ch = 0; ch < 4; ch++) {
    ch_seq[ch] = tmac_initial_ch_seq[ch];
  }

  for (ch = 0; ch < 4; ch++) {
    n_ch = port_mgr_tof3_tmac_channel_get(dev_id, mac, ch);
    if (n_ch != 0) {
      for (c = 0; c < 4; c++) {
        if ((ch_seq[c] >= ch) && (ch_seq[c] < (ch + n_ch))) {
          ch_seq[c] = ch;
        }
      }
    }
  }
  // now convert ch_seq into the field
  chnl_seq = 0ull;
  for (c = 0; c < 4; c++) {
    uint32_t fld = ch_seq[c];
    chnl_seq = (chnl_seq << 2) | fld;
  }
  port_mgr_log("chnl_used: [%2d,%2d,%2d,%2d]",
               port_mgr_tof3_tmac_channel_get(dev_id, mac, 0),
               port_mgr_tof3_tmac_channel_get(dev_id, mac, 1),
               port_mgr_tof3_tmac_channel_get(dev_id, mac, 2),
               port_mgr_tof3_tmac_channel_get(dev_id, mac, 3));
  port_mgr_log("chnl_seq : [%2d,%2d,%2d,%2d]", ch_seq[0], ch_seq[1], ch_seq[2],
               ch_seq[3]);
  port_mgr_log("chnl_seq : %08x", chnl_seq);

  return chnl_seq;
}

static void get_mac_tx_fifo_threshold(bf_port_speed_t speed, uint32_t n_lanes,
                                      uint32_t *paf, uint32_t *pae,
                                      uint32_t tmac, uint32_t *paf_level) {
  if (!paf || !pae || !paf_level)
    return;

  *pae = 0xd;
  *paf = 0xA; // 50G and below

  // special case
  if ((speed == BF_SPEED_50G) && (n_lanes == 1)) {
    if (tmac == 0) {
      *paf_level = 0x18;
    } else {
      *paf_level = 0x16;
    }
    return;
  }

  *paf_level = 0x12;

  if ((tmac == 0) && (speed == BF_SPEED_10G)) {
    *paf_level = 0x8;
  }

  // *paf = 0xE;  // default
  switch (speed) {
  case BF_SPEED_400G:
    *paf = 0x40;
    break;

  case BF_SPEED_200G:
    *paf = 0x20;
    break;

  case BF_SPEED_100G:
    *paf = 0x10;
    break;

  default:
    break;
  }
}

enum {
  PORT_CL_NONE = 0,
  PORT_CL119 = 1,
  PORT_CL91,
  PORT_CL82,
  PORT_CL49,
  PORT_CL36
};

static uint32_t get_pcs_configuration(bf_port_speed_t speed, bf_fec_types_t fec,
                                      uint32_t n_lanes, uint32_t *port_type,
                                      uint32_t *port_clause,
                                      uint32_t *clause_val,
                                      uint32_t *am_cl82_50g,
                                      uint32_t *am_cl119_100g) {
  uint32_t mode;
  if ((!port_type) || (!port_clause) || (!clause_val) || (!am_cl82_50g) ||
      (!am_cl119_100g)) {
    return -1;
  }

  *port_type = 0;
  *port_clause = PORT_CL_NONE;
  *clause_val = 0;
  *am_cl82_50g = 0;
  *am_cl119_100g = 0;
  mode = 0;

  /*
      "Configure the port type based on Clause 49/82/91/119 selected through
     correpsonding fields: \n"
      " CL119 port mode : 400G-KP8 (0x0011), 400G-KP4 (0x0012), 200-KP8
     (0x2010),
      "                   200G-KP4 (0x2011), 200G-KP2 (0x2012), 100G-KP1
     (0x3012) \n
      "    - [15:12] : Speed (0x0 for 400G, 0x2 for 200G, 0x3 for 100G) \n"
      "    - [9]     : Error indication bypass (0b for Spec compliance) \n"
      "    - [8]     : Error correction bypass (0b for Spec compliance) \n"
      "    - [5:4]   : FEC type (0x0 for KR, 0x1 for KP, 0x2 for KL) \n"
      "    - [1:0]   : Bit Mux (0x0 for none, 0x1 for 2:1, 0x2 for 4:1)\n"
      " CL91 port mode : 100G-KR4 (0x0000), 100G-KP2 (0x0011), 100G-KP1
                         (0x0012), 50G-KR2 (0x2000), 50G-KP1 (0x2011), 25G-KR1
     (0x3000) \n"
      "    - [15:12] : Speed (0x0 for 100G, 0x2 for 50G, 0x3 for "25G) \n"
      "    - [9]     : Error indication bypass (0b for Spec compliance) \n"
      "    - [8]     : Error correction bypass (0b for Spec compliance) \n"
      "    - [5:4]   : FEC type (0x0 for KR, 0x1 for KP, 0x2 for KL) \n"
      "    - [1:0]   : Bit Mux (0x0 for none, 0x1 for 2:1, 0x2 for 4:1)\n"
      " CL82 port mode : 100G-R4 (0x0000), 50G-R2 (0x2000), 40G-R4 (0x3000),
     40G-FC (0x3010), 50G-FC (0x2010) \n"
      "    - [15:12] : Speed (0x0 for 100G, 0x2 for 50G, 0x3 for 40G) \n"
      "    - [8]     : bypass scrambler (0b for Spec compliance) \n"
      "    - [4]     : FEC mode (0x0 for no-FEC, 0x1 for FC-FEC) \n"
      " CL49 port mode : 25G-R1 (0x1000), 10G-R1 (0x0000), 25G-FC (0x1010),
     10G-FC (0x0010) \n"
      "    - [15:12] : Speed (0x0 for 10G, 0x1 for 25G) \n"
      "    - [8]     : bypass scrambler (0b for Spec compliance) \n"
      "    - [4]     : FEC mode (0x0 for no-FEC, 0x1 for FC-FEC) \n"
  */
  switch (speed) {
  case BF_SPEED_400G:
    *port_clause = PORT_CL119;
    *clause_val = 1;
    if (fec == BF_FEC_TYP_RS) {
      if (n_lanes == 8) {
        mode = 0x0011;
      } else if (n_lanes == 4) {
        mode = 0x0012;
      }
    } else if (fec == BF_FEC_TYP_RS_KL) {
      if (n_lanes == 8) {
        mode = 0x0021;
      } else if (n_lanes == 4) {
        mode = 0x0022;
      }
    }
    break;
  case BF_SPEED_200G:
    *port_clause = PORT_CL119;
    *clause_val = 1;
    if (fec == BF_FEC_TYP_RS) {
      if (n_lanes == 8) {
        mode = 0x2010;
      } else if (n_lanes == 4) {
        mode = 0x2011;
      } else if (n_lanes == 2) {
        mode = 0x2012;
      }
    } else if (fec == BF_FEC_TYP_RS_KL) {
      if (n_lanes == 8) {
        mode = 0x2020;
      } else if (n_lanes == 4) {
        mode = 0x2011;
      } else if (n_lanes == 2) {
        mode = 0x2022;
      }
    } else if (fec == BF_FEC_TYP_NONE) {
      if (n_lanes == 8) {
        mode = 0x2010;
      }
    }
    break;

  case BF_SPEED_100G:
    *clause_val = 1;
    if (fec == BF_FEC_TYP_RS) {
      if (n_lanes == 4) {
        mode = 0x0;
        *port_clause = PORT_CL91;
      } else if (n_lanes == 2) {
        mode = 0x11;
        *port_clause = PORT_CL91;
      } else if (n_lanes == 1) {
        mode = 0x0012; // 100G-KP1
        *port_clause = PORT_CL91;
      }
    } else if (fec == BF_FEC_TYP_RS_KL) {
      if (n_lanes == 4) {
        mode = 0x0020;
        *port_clause = PORT_CL91;
      } else if (n_lanes == 2) {
        mode = 0x21;
        *port_clause = PORT_CL91;
      } else if (n_lanes == 1) {
        mode = 0x3022; // 100G-KP1
        *port_clause = PORT_CL119;
      }
    } else if (fec == BF_FEC_TYP_RS_IN) {
      if (n_lanes == 1) {
        mode = 0x3012; // 100G-KP1
        *port_clause = PORT_CL119;
      }
    } else if (fec == BF_FEC_TYP_NONE) {
      if (n_lanes == 4) { // 100G-R4
        mode = 0x0;
        *port_clause = PORT_CL82;
      }
      // 100G-KR4, no-fec - TBD
    }
    break;

  case BF_SPEED_50G:
    *clause_val = 1;
    if ((fec == BF_FEC_TYP_RS) && (n_lanes == 1)) {
      *port_clause = PORT_CL91;
      mode = 0x2011;
    } else if ((fec == BF_FEC_TYP_NONE) && (n_lanes == 2)) {
      *port_clause = PORT_CL82;
      mode = 0x2000;
      *am_cl82_50g = 1; // bit inverted in hardware
    } else if ((fec == BF_FEC_TYP_FC) && (n_lanes == 2)) {
      *port_clause = PORT_CL82;
      mode = 0x2010;
      *clause_val = 1;
      *am_cl82_50g = 1; // bit inverted in hardware
    } else if ((fec == BF_FEC_TYP_RS_KL) && (n_lanes == 1)) {
      *port_clause = PORT_CL91;
      mode = 0x2021;
    } else if ((fec == BF_FEC_TYP_RS_KL) && (n_lanes == 2)) {
      *port_clause = PORT_CL91;
      mode = 0x2020;
    } else if ((fec == BF_FEC_TYP_RS) && (n_lanes == 2)) {
      *port_clause = PORT_CL91;
      mode = 0x2000;
    }
    break;

  case BF_SPEED_40G:
    *port_clause = PORT_CL82;
    *clause_val = 1;
    if ((n_lanes == 4) && (fec == BF_FEC_TYP_NONE)) {
      mode = 0x3000;
    } else if ((n_lanes == 4) && (fec == BF_FEC_TYP_FC)) {
      mode = 0x3010;
    }
    break;

  case BF_SPEED_25G: // 1*25
    *clause_val = 1;
    if (fec == BF_FEC_TYP_NONE) {
      *port_clause = PORT_CL49;
      mode = 0x1000;
    } else if (fec == BF_FEC_TYP_RS) {
      *port_clause = PORT_CL91;
      mode = 0x3000;
    } else if (fec == BF_FEC_TYP_RS_KL) {
      *port_clause = PORT_CL91;
      mode = 0x3020;
    } else if (fec == BF_FEC_TYP_FC) {
      *port_clause = PORT_CL49;
      mode = 0x1010;
    }
    break;

  case BF_SPEED_10G: // 1*10
    *port_clause = PORT_CL49;
    *clause_val = 1;
    mode = 0x0000;
    if (fec == BF_FEC_TYP_FC) {
      mode = 0x0010;
    }
    break;

  case BF_SPEED_1G: // 1*1
    *port_clause = PORT_CL36;
    *clause_val = 1;
    mode = 0;
    // fec and mode not valid
    break;
  default:
    break;
  }
  *port_type = mode;
  return 0;
}

static uint32_t get_txff_ctrl_chnl_mode(bf_port_speed_t speed) {
  uint32_t val = 3; // 50g and below

  if (speed == BF_SPEED_400G) {
    val = 0;
  } else if (speed == BF_SPEED_200G) {
    val = 1;
  } else if (speed == BF_SPEED_100G) {
    val = 2;
  }

  return val;
}

static uint32_t get_txff_ctrl_cred_ini(bf_port_speed_t speed) {
  uint32_t val = 0xF;
  if (speed == BF_SPEED_400G) {
    val = 0x7F;
  } else if (speed == BF_SPEED_200G) {
    val = 0x3F;
  } else if (speed == BF_SPEED_100G) {
    val = 0x1F;
  }
  return val;
}

/*****************************************************************************
 * Clear RMON counter
 *
 * Note: Triggering clear req on unconfigured channel, will cause hardware
 *       NOT to release the chan-req.
 ****************************************************************************/
void port_mgr_tof3_tmac_clear_stats(bf_dev_id_t dev_id, uint32_t mac,
                                    uint32_t ch) {
  uint32_t data32, val;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  bf_sys_mutex_lock(&tmac_stats_mtx[dev_id]);

  data32 = 0;
  val = 1;
  TMAC_BIT_SET(val, ch);
  tof3_eth400g_sys_rspec_stat_clear_req_chan_rmw(dev_id, subdev_id, tmac,
                                                 &data32, val);

  int count = 0;
  do {
    tof3_eth400g_sys_rspec_stat_clear_req_chan_get(dev_id, subdev_id, tmac,
                                                   &data32, &val, true);
    TMAC_BIT_GET(val, ch);
    count++;
  } while (val && (count < 100));

  bf_sys_mutex_unlock(&tmac_stats_mtx[dev_id]);
}

/*
 * Performs the fifo drain sequence before reconfig a port
 *
 */
static bf_status_t port_mgr_tof3_tmac_drain_fifo(bf_dev_id_t dev_id,
                                                 bf_subdev_id_t subdev_id,
                                                 uint32_t tmac, uint32_t mac,
                                                 uint32_t ch) {
  uint32_t data32 = 0, val = 0, cnt;
  uint32_t txfifo_cnt = 0;

  uint32_t mac_reset_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);
  uint32_t txff_ctrl_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_app.txff_ctrl[ch]);
  uint32_t txff_status_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_app.txff_status[ch]);

  if (tmac_is_running_on_model(dev_id))
    return BF_SUCCESS;

  // Other bits are don't care at this stage
  data32 = 0;
  val = 1;
  setp_tof3_eth400g_app_rspec_txff_ctrl_tx_flush(&data32, val);
  tof3_eth400g_app_rspec_txff_ctrl_tx_flush_rmw(dev_id, subdev_id, tmac, ch,
                                                &data32, val);

  data32 = 1;
  cnt = 0;
  autogen_tmac_rd(dev_id, subdev_id, txff_status_ofs, &data32);
  txfifo_cnt = data32;
  do {
    data32 = getp_tof3_eth400g_app_rspec_txff_status_txff_count(&data32);
    if (data32) {
      bf_sys_usleep(100);
      cnt++;
      autogen_tmac_rd(dev_id, subdev_id, txff_status_ofs, &data32);
    }
  } while (data32 && (cnt < 100));

  txfifo_cnt = getp_tof3_eth400g_app_rspec_txff_status_txff_count(&txfifo_cnt);
  // Return error
  if (data32 && cnt >= 100) {
    port_mgr_log_error("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
                       "txfifo flush txfifo_cnt %d",
                       dev_id, subdev_id, tmac, mac, ch, txfifo_cnt);
    // Release the fifo and return error.
    data32 = 0;
    val = 0;
    setp_tof3_eth400g_app_rspec_txff_ctrl_tx_flush(&data32, val);
    tof3_eth400g_app_rspec_txff_ctrl_tx_flush_rmw(dev_id, subdev_id, tmac, ch,
                                                  &data32, val);

    return -1;
  }

  // deassert flush and disable channel
  data32 = 0;
  val = 1;
  setp_tof3_eth400g_app_rspec_txff_ctrl_tx_flush(&data32, val);
  val = 0;
  setp_tof3_eth400g_app_rspec_txff_ctrl_chnl_mode(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, txff_ctrl_ofs, data32);

  // soft reset
  data32 = 0;
  val = 1;
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(&data32, val);
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, mac_reset_ofs, data32);

  // disable rx and tx
  data32 = 0x0;
  val = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);

  port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
               "txfifo flush done txfifo_cnt %d cnt %d",
               dev_id, subdev_id, tmac, mac, ch, txfifo_cnt, cnt);
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_config
 *
 * Basic speed/fec and channel config (on port-add)
 ****************************************************************************/
void port_mgr_tof3_tmac_config(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch,
                               bf_port_speed_t speed, bf_fec_types_t fec,
                               uint32_t n_lanes, uint32_t n_serdes_lanes) {
  uint32_t chnl_seq;
  uint32_t data32 = 0, val = 0;
  bf_serdes_encoding_mode_t enc_mode;
  uint32_t port_type, port_clause, clause_val;
  uint32_t pae, paf, paf_level;
  char *cl_str[] = {"CL_NONE", "CL119", "CL91", "CL82", "CL49", "CL36"};
  port_mgr_port_t *port_p = NULL;
  uint32_t tx_pkt_max_len;
  uint32_t rx_pkt_max_len;
  uint32_t tx_pkt_min_len = 0x40;
  uint32_t rx_pkt_min_len = 0x40;
  bf_dev_port_t dev_port;
  uint32_t am_cl82_50g, am_cl119_100g;

  lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac, ch, &dev_port);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL)
    return;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  uint32_t n_phy_lanes = n_lanes / n_serdes_lanes;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);

  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t mac_reset_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);
  uint32_t pcs_tx_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.static_pcs_tx[ch]);
  uint32_t pcs_rx_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.static_pcs_rx[ch]);
  uint32_t txff_ctrl_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_app.txff_ctrl[ch]);

  bf_serdes_encoding_mode_get(speed, n_lanes, &enc_mode);

  // Mark ch in use
  port_mgr_tof3_tmac_channel_add(dev_id, mac, ch, n_phy_lanes);

  // perform flush seq before adding
  port_mgr_tof3_tmac_drain_fifo(dev_id, subdev_id, tmac, mac, ch);

  // Soft reset MAC. Released after serdes-init
  data32 = 0;
  val = 1;
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(&data32, val);
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, mac_reset_ofs, data32);

#ifndef DEVICE_IS_EMULATOR
  port_mgr_tof3_tmac_serdes_lane_map_set(dev_id, subdev_id, mac, tmac, ch,
                                         speed, n_lanes, show_mux_use, true);
  if (show_mux_use) {
    show_tx_mux_used_val(dev_id, mac);
    show_rx_mux_used_val(dev_id, mac);
  }
#endif

  data32 = 0;
  chnl_seq = port_mgr_tof3_tmac_chnl_seq_calc(dev_id, mac, ch);
  tof3_eth400g_app_rspec_chnl_seq_chnl_seq_rmw(dev_id, subdev_id, tmac, &data32,
                                               chnl_seq);

  // Configure TX Fifo full Threshold
  data32 = 0;
  get_mac_tx_fifo_threshold(speed, n_lanes, &paf, &pae, tmac, &paf_level);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_rmw(dev_id, subdev_id, tmac,
                                                     ch, &data32, paf);

  data32 = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_rmw(dev_id, subdev_id, tmac,
                                                     ch, &data32, pae);

  data32 = 0;
  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_rmw(dev_id, subdev_id, tmac,
                                                       ch, &data32, paf_level);

  data32 = 0;
  val = 0x1;
  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_rmw(dev_id, subdev_id, tmac, ch,
                                                    &data32, val);

  // Configure port_type & port_type(Both Tx & RX):
  port_type = 0;
  port_clause = 0;
  clause_val = 0;
  get_pcs_configuration(speed, fec, n_lanes, &port_type, &port_clause,
                        &clause_val, &am_cl82_50g, &am_cl119_100g);

  data32 = 0;
  val = 0;
  val = clause_val;
  if (port_clause == PORT_CL119) {
    // rx and tx data are same.
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_port_c119(&data32, val);
  } else if (port_clause == PORT_CL91) {
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_port_c91(&data32, val);
  } else if (port_clause == PORT_CL82) {
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_port_c82(&data32, val);
  } else if (port_clause == PORT_CL49) {
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_port_c49(&data32, val);
  } else if (port_clause == PORT_CL36) {
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_port_c36(&data32, val);
  } else {
    // error
    port_mgr_log_error("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
                       "port-type 0x%0x  port-cl : %s port-cl-val : %d",
                       dev_id, subdev_id, tmac, mac, ch, port_type,
                       cl_str[port_clause], clause_val);
    return;
  }

  if (am_cl82_50g) {
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing(&data32,
                                                                     val);
  }

  if (am_cl119_100g) {
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final(&data32,
                                                                      val);
  }

  if (port_clause != PORT_CL36) {
    val = port_type;
    setp_tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b(&data32, val);
  }

  port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
               "port-type 0x%0x  port-cl %s port-cl-val %d pcs-cfg 0x%0x pae "
               "0x%0x paf 0x%0x paf-level 0x%0x",
               dev_id, subdev_id, tmac, mac, ch, port_type, cl_str[port_clause],
               clause_val, data32, pae, paf, paf_level);

  autogen_tmac_wr(dev_id, subdev_id, pcs_tx_ofs, data32);
  autogen_tmac_wr(dev_id, subdev_id, pcs_rx_ofs, data32);

  // Configure Max TX/RX packet length:
  tx_pkt_max_len = port_p->sw.tx_mtu;
  data32 = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_rmw(
      dev_id, subdev_id, tmac, ch, &data32, tx_pkt_min_len);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_rmw(
      dev_id, subdev_id, tmac, ch, &data32, tx_pkt_max_len);

  rx_pkt_max_len = port_p->sw.rx_mtu;
  data32 = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_rmw(
      dev_id, subdev_id, tmac, ch, &data32, rx_pkt_min_len);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_rmw(
      dev_id, subdev_id, tmac, ch, &data32, rx_pkt_max_len);

  // Keep rx and tx disabled until port-enb
  data32 = 0x0;
  val = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);

  // Ch enable, ch-mode and credit
  data32 = 0;
  autogen_tmac_wr(dev_id, subdev_id, txff_ctrl_ofs, data32);

  data32 = 0;
  val = get_txff_ctrl_chnl_mode(speed);
  setp_tof3_eth400g_app_rspec_txff_ctrl_chnl_mode(&data32, val);

  val = get_txff_ctrl_cred_ini(speed);
  setp_tof3_eth400g_app_rspec_txff_ctrl_cred_ini(&data32, val);

  val = 0x4;
  setp_tof3_eth400g_app_rspec_txff_ctrl_min_thr(&data32, val);

  val = 0;
  setp_tof3_eth400g_app_rspec_txff_ctrl_tx_flush(&data32, val);

  val = 0x0;
  setp_tof3_eth400g_app_rspec_txff_ctrl_chnl_ena(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, txff_ctrl_ofs, data32);

  val = 0x1;
  data32 = 0;
  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_rmw(dev_id, subdev_id, tmac, ch,
                                                &data32, val);

  // Config PMA Per channel
  data32 = 0;
  val = 0xFF;
  tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_rmw(dev_id, subdev_id, tmac, &data32,
                                            val);
  tof3_eth400g_mac_rspec_cfg_pma_rx_en_rmw(dev_id, subdev_id, tmac, &data32,
                                           val);

  // enable the FEC error distribution counters (COR, per FEC blk)
  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_rmw(dev_id, subdev_id, tmac,
                                                       &data32, 1);
  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_rmw(dev_id, subdev_id, tmac,
                                                       &data32, 1);
  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_rmw(dev_id, subdev_id, tmac,
                                                      &data32, 1);

  // Set all the PCS/FEC counters to be "clear-on-read"
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_rmw(dev_id, subdev_id, tmac,
                                                     &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_rmw(dev_id, subdev_id,
                                                           tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_rmw(dev_id, subdev_id, tmac,
                                                        &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_rmw(dev_id, subdev_id,
                                                            tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_rmw(dev_id, subdev_id,
                                                           tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_rmw(dev_id, subdev_id,
                                                           tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_rmw(dev_id, subdev_id,
                                                          tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_rmw(dev_id, subdev_id,
                                                           tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_rmw(dev_id, subdev_id,
                                                         tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_rmw(dev_id, subdev_id, tmac,
                                                     &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_rmw(dev_id, subdev_id,
                                                          tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_rmw(dev_id, subdev_id,
                                                         tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_rmw(dev_id, subdev_id,
                                                          tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_rmw(dev_id, subdev_id,
                                                         tmac, &data32, 1);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_rmw(dev_id, subdev_id,
                                                         tmac, &data32, 1);

  data32 = 0;
  val = 0;
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(&data32, val);
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, mac_reset_ofs, data32);

  // clear the stats
  port_mgr_tof3_tmac_clear_stats(dev_id, mac, ch);

  data32 = 0;
  val = 1;
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(&data32, val);
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, mac_reset_ofs, data32);
}

/*****************************************************************************
 * port_mgr_tof3_tmac_de_config
 *
 * Unconfigure a channel
 ****************************************************************************/
void port_mgr_tof3_tmac_de_config(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch,
                                  bf_port_speed_t speed, uint32_t n_lanes) {
  uint32_t n_ch;
  uint32_t tmac = 0, data32;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t pcs_tx_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.static_pcs_tx[ch]);
  uint32_t pcs_rx_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.static_pcs_rx[ch]);

  data32 = 0;
  autogen_tmac_rd(dev_id, subdev_id, pcs_tx_ofs, &data32);
  // rx and tx are same
  data32 &= ~(uint32_t)0x1FFul;
  autogen_tmac_wr(dev_id, subdev_id, pcs_tx_ofs, data32);
  autogen_tmac_wr(dev_id, subdev_id, pcs_rx_ofs, data32);

#ifndef DEVICE_IS_EMULATOR
  port_mgr_tof3_tmac_serdes_lane_map_set(dev_id, subdev_id, mac, tmac, ch,
                                         speed, n_lanes, show_mux_use, false);
  if (show_mux_use) {
    show_tx_mux_used_val(dev_id, mac);
    show_rx_mux_used_val(dev_id, mac);
  }

#endif

  // At present, ibuf and ebuf are doing flush during port-delete.
  // Hence mac-flush has been added here to support them, albeit
  // we redo the flush sequence just before port-add.
  port_mgr_tof3_tmac_drain_fifo(dev_id, subdev_id, tmac, mac, ch);

  n_ch = port_mgr_tof3_tmac_channel_get(dev_id, mac, ch);
  port_mgr_tof3_tmac_channel_del(dev_id, mac, ch, n_ch);
}

/*****************************************************************************
 * port_mgr_tof3_tmac_enable
 *
 * Enable a tmac channel
 ****************************************************************************/
void port_mgr_tof3_tmac_enable(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch) {
  uint32_t data32, val;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  port_mgr_port_t *port_p = NULL;
  uint32_t tx_pkt_max_len;
  uint32_t rx_pkt_max_len;
  uint32_t tx_pkt_min_len = 0x40;
  uint32_t rx_pkt_min_len = 0x40;

  bf_dev_port_t dev_port;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac, ch, &dev_port);
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL)
    return;
  // Toggle for credit
  val = 0;
  data32 = 0;
  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_rmw(dev_id, subdev_id, tmac, ch,
                                                &data32, val);

  val = 1;
  data32 = 0;
  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_rmw(dev_id, subdev_id, tmac, ch,
                                                &data32, val);

  // Enable rx and tx bufs
  port_mgr_log(
      "MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d Enabling rx and tx",
      dev_id, subdev_id, tmac, mac, ch);
  data32 = 0x0;
  val = 1;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);
  tx_pkt_max_len = port_p->sw.tx_mtu;
  data32 = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_rmw(
      dev_id, subdev_id, tmac, ch, &data32, tx_pkt_min_len);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_rmw(
      dev_id, subdev_id, tmac, ch, &data32, tx_pkt_max_len);

  rx_pkt_max_len = port_p->sw.rx_mtu;
  data32 = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_rmw(
      dev_id, subdev_id, tmac, ch, &data32, rx_pkt_min_len);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_rmw(
      dev_id, subdev_id, tmac, ch, &data32, rx_pkt_max_len);
}

/*****************************************************************************
 * port_mgr_tof3_tmac_disable
 *
 * Disable a tmac channel
 ****************************************************************************/
void port_mgr_tof3_tmac_disable(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch) {
  uint32_t data32, val;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  port_mgr_log(
      "MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d Disabling rx and tx",
      dev_id, subdev_id, tmac, mac, ch);

  // Disable rx and tx
  data32 = 0x0;
  val = 0;
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);

  // RX towards MAC can enabled once DFE is done.
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);
}

/*****************************************************************************
 * port_mgr_tof3_tmac_link_state_get
 *
 * Return the operational state of the port defined on tmac/ch
 ****************************************************************************/
void port_mgr_tof3_tmac_link_state_get(bf_dev_id_t dev_id, uint32_t mac,
                                       uint32_t ch, bool *up) {
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);

  if (tmac >= TMAC_TOF3_MAX)
    return;
  if (rc != BF_SUCCESS)
    return;

  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_mac.rx_status[ch]);
  uint32_t rx_status_reg = 0, tx_rdy = 0, rx_rdy = 0, rx_status = 0;
  uint32_t rs_rx_fault_remote = 0, rs_rx_fault_local = 0;
  uint32_t *data_p = &rx_status_reg;

  autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);

  tx_rdy = getp_tof3_eth400g_mac_rspec_rx_status_tx_rdy(data_p);
  rx_rdy = getp_tof3_eth400g_mac_rspec_rx_status_rx_rdy(data_p);
  rx_status = getp_tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status(data_p);
  rs_rx_fault_remote =
      getp_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote(data_p);
  rs_rx_fault_local =
      getp_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local(data_p);

  if (up) {
    if (tx_rdy && rx_rdy && rx_status && (!rs_rx_fault_remote) &&
        (!rs_rx_fault_local)) {
      *up = true;
    } else {
      *up = false;
    }
  }
}

/*****************************************************************************
 * port_mgr_tof3_tmac_sw_reset_set
 *
 ****************************************************************************/
void port_mgr_tof3_tmac_sw_reset_set(bf_dev_id_t dev_id, uint32_t mac,
                                     uint32_t ch, bool assert_reset) {
#if 0 
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return;
#endif

  (void)mac;
  (void)ch;
  (void)dev_id;
  (void)assert_reset;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_is_cpu_port
 *
 * MAC_ID : CPU MAC is MAC_ID=0 for standard die
 *
 ****************************************************************************/
bool port_mgr_tof3_tmac_is_cpu_port(bf_dev_id_t dev_id, uint32_t mac) {
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return false;

  if ((tmac == PORT_MGR_TOF3_CPU_PORT_TMAC) && (subdev_id == 0)) { // cpu port
    return true;
  }
  return false;
  (void)dev_id;
}

bf_status_t port_mgr_tof3_tmac_to_rmon_ctr_copy(uint64_t *src_ctr_array,
                                                uint64_t *dst_ctr_array) {
  uint32_t ctr_id, tmac_ctr_id;
  uint32_t max_ctr_id = (sizeof(tmac_rmon_ctrs_map) / sizeof(tmac_rmon_ctrs_map[0]));

  for (ctr_id = 0; ctr_id < max_ctr_id; ctr_id++) {
    tmac_ctr_id = tmac_rmon_ctrs_map[ctr_id][1];
    dst_ctr_array[ctr_id] = src_ctr_array[tmac_ctr_id];
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_read_counter
 *
 * Sync read of tmac RMON counter
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_read_counter(bf_dev_id_t dev_id, uint32_t mac,
                                            uint32_t ch,
                                            bf_rmon_counter_t ctr_id,
                                            uint64_t *ctr_value) {
  uint32_t data32, val;
  uint32_t addr;
  csr_stats_t rd;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  if (tmac >= TMAC_TOF3_MAX)
    return BF_INVALID_ARG;

  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

  if (!ctr_value)
    return BF_INVALID_ARG;

  if ((uint32_t)ctr_id >= (uint32_t)TMAC_MAX_RMON_COUNTERS)
    return BF_INVALID_ARG;

  bf_sys_mutex_lock(&tmac_stats_mtx[dev_id]);

  data32 = 0;
  addr = tmac_rmon_ctrs_map[ctr_id][1];
  if (addr == tmac_Reserved) {
    *ctr_value = 0;
    return 0;
  }

#ifdef DEVICE_IS_EMULATOR
  if (skip_rmon_tmac) {
    *ctr_value = 0;
    bf_sys_mutex_unlock(&tmac_stats_mtx[dev_id]);
    return BF_SUCCESS;
  }
#endif

  data32 = 0;
  addr = (96 * ch) + addr;
  setp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr(&data32, addr);
  setp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write(&data32, 0);
  setp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req(&data32, 1);
  autogen_tmac_wr(dev_id, subdev_id, ofs, data32);
  val = 1;
  int count = 0;
  do {
    tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(dev_id, subdev_id, tmac,
                                                    &data32, &val, true);
    // hack
    if (data32 == 0x0bad0bad) {
      bf_sys_mutex_unlock(&tmac_stats_mtx[dev_id]);
      *ctr_value = 0;
      return BF_SUCCESS;
    }
    count++;
  } while (val && (count < 50));
  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(
      dev_id, subdev_id, tmac, &data32, &rd.data32[0], true);
  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(
      dev_id, subdev_id, tmac, &data32, &rd.data32[1], true);
  *ctr_value = rd.data64;

  bf_sys_mutex_unlock(&tmac_stats_mtx[dev_id]);
  return BF_SUCCESS;
}

/*****************************************************************************
 ****************************************************************************/
bf_status_t tmac_ll_rd(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id,
                       uint32_t tmac, uint32_t offset, uint32_t *r_data) {
  bf_status_t rc = BF_SUCCESS;

  if (microp_init_done && use_tv80_for_tmac_access) {
  } else {
    rc = lld_subdev_read_register(dev_id, subdev_id, offset, r_data);
    assert(rc == BF_SUCCESS);
  }
  (void)tmac;
  return rc;
}

/*****************************************************************************
 ****************************************************************************/
bf_status_t tmac_ll_wr(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id,
                       uint32_t tmac, uint32_t offset, uint32_t w_data) {
  bf_status_t rc = BF_SUCCESS;

  if (microp_init_done && use_tv80_for_tmac_access) {
#if 0

#endif
  } else {
    rc = lld_subdev_write_register(dev_id, subdev_id, offset, w_data);
    assert(rc == BF_SUCCESS);
  }
  return rc;
  (void)tmac;
}

bf_status_t port_mgr_tof3_tmac_even_serdes_lane_map_set(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t mac, uint32_t tmac,
    uint32_t ch, bf_port_speed_t speed, uint32_t n_lanes, bool en_log,
    bool cfg) {

  uint32_t phys_tx_ln[4], phys_rx_ln[4], tx_lane, rx_lane, tx_data, rx_data;
  bool serdes_even;

  uint32_t i, phy_lns = 0, j;
  bool serdes_112G_mode = false;
  uint32_t tx_map = 0, rx_map = 0;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);
  uint32_t chan = ch;

  uint32_t tx_serdesmux_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.cfg_txserdesmux);
  uint32_t rx_serdesmux_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.cfg_rxserdesmux);

  if (!dev_p)
    return BF_INVALID_ARG;
  if (tmac == 0)
    return BF_SUCCESS;

  if (dev_p->tmac[mac].max_serdes_per_mac != 4)
    return BF_SUCCESS;

  phy_lns = dev_p->tmac[mac].max_serdes_per_mac;
  if (!phy_lns) {
    port_mgr_log_error("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
                       "phy-lane cannot be %d",
                       dev_id, subdev_id, tmac, mac, ch, phy_lns);
    return BF_INVALID_ARG;
  }

  for (i = 0; i < phy_lns; i++) {
    phys_tx_ln[i] = dev_p->tmac[mac].phys_tx_ln[i];
    phys_rx_ln[i] = dev_p->tmac[mac].phys_rx_ln[i];
  }

  if (0 == phys_tx_ln[0] % 2) {
    serdes_even = true;
  } else {
    serdes_even = false;
  }

  if (!serdes_even)
    return BF_SUCCESS;

  autogen_tmac_rd(dev_id, subdev_id, rx_serdesmux_ofs, &rx_map);
  autogen_tmac_rd(dev_id, subdev_id, tx_serdesmux_ofs, &tx_map);
  switch (speed) {
  case BF_SPEED_400G:
    if (n_lanes == 4) {
      serdes_112G_mode = true;
    }
    break;
  case BF_SPEED_200G:
    if (n_lanes == 2) {
      serdes_112G_mode = true;
    }
    break;
  case BF_SPEED_100G:
    if (n_lanes == 1) {
      serdes_112G_mode = true;
    }
    break;
  default:
    break;
  }

  if (serdes_112G_mode) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = i * 2;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_sds_mux(&tx_map, tx_data + 1, tx_lane + 1);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);
      set_tx_mux_used(dev_id, mac, tx_data + 1, tx_lane + 1, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = i * 2;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_sds_mux(&rx_map, rx_data + 1, rx_lane + 1);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
      set_rx_mux_used(dev_id, mac, rx_data + 1, rx_lane + 1, cfg);
    }
  } else if (((speed == BF_SPEED_100G) || (speed == BF_SPEED_50G)) &&
             (n_lanes == 2)) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = (chan * 2) + j;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = (chan * 2) + j;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
    }

  } else if ((speed == BF_SPEED_10G) || (speed == BF_SPEED_25G) ||
             ((speed == BF_SPEED_50G) && (n_lanes == 1))) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = i * 2;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = i * 2;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
    }

  } else if (((speed == BF_SPEED_100G) && (n_lanes == 4)) ||
             (speed == BF_SPEED_40G) ||
             ((speed == BF_SPEED_200G) && (n_lanes == 4))) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = i;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = i;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
    }
  } else {
    port_mgr_log_error("mac : %0d configuring tx/rx maps - invalid speed\n",
                       mac);
    return BF_INVALID_ARG;
  }

  find_and_rpl_dup(dev_id, mac, &tx_map, &rx_map);

  autogen_tmac_wr(dev_id, subdev_id, rx_serdesmux_ofs, rx_map);
  autogen_tmac_wr(dev_id, subdev_id, tx_serdesmux_ofs, tx_map);

  if (en_log) {
    port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d tx-map "
                 "0x%08x rx-map 0x%08x tx-ofs %0x08x rx-ofs %0x08x",
                 dev_id, subdev_id, tmac, mac, ch, tx_map, rx_map,
                 tx_serdesmux_ofs, rx_serdesmux_ofs);
  }

  return BF_SUCCESS;
}

bf_status_t port_mgr_tof3_tmac_serdes_lane_map_set(
    bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t mac, uint32_t tmac,
    uint32_t ch, bf_port_speed_t speed, uint32_t n_lanes, bool en_log,
    bool cfg) {

  uint32_t phys_tx_ln[4] = {0}, phys_rx_ln[4] = {0};
  uint32_t tx_lane = 0, rx_lane = 0, tx_data = 0, rx_data = 0;
  bool serdes_even;

  uint32_t i, phy_lns = 0, j;
  bool serdes_112G_mode = false;
  uint32_t tx_map = 0, rx_map = 0;
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);
  uint32_t chan = ch;

  uint32_t tx_serdesmux_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.cfg_txserdesmux);
  uint32_t rx_serdesmux_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.cfg_rxserdesmux);

  if (!dev_p)
    return BF_INVALID_ARG;
  if (tmac == 0)
    return BF_SUCCESS;

  if (dev_p->tmac[mac].max_serdes_per_mac != 4)
    return BF_SUCCESS;

  phy_lns = dev_p->tmac[mac].max_serdes_per_mac;

  for (i = 0; i < phy_lns; i++) {
    phys_tx_ln[i] = dev_p->tmac[mac].phys_tx_ln[i];
    phys_rx_ln[i] = dev_p->tmac[mac].phys_rx_ln[i];
  }

  if (0 == phys_tx_ln[0] % 2) {
    serdes_even = true;
  } else {
    serdes_even = false;
  }

  if (serdes_even)
    return port_mgr_tof3_tmac_even_serdes_lane_map_set(
        dev_id, subdev_id, mac, tmac, ch, speed, n_lanes, en_log, cfg);

  autogen_tmac_rd(dev_id, subdev_id, rx_serdesmux_ofs, &rx_map);
  autogen_tmac_rd(dev_id, subdev_id, tx_serdesmux_ofs, &tx_map);
  switch (speed) {
  case BF_SPEED_400G:
    if (n_lanes == 4) {
      serdes_112G_mode = true;
    }
    break;
  case BF_SPEED_200G:
    if (n_lanes == 2) {
      serdes_112G_mode = true;
    }
    break;
  case BF_SPEED_100G:
    if (n_lanes == 1) {
      serdes_112G_mode = true;
    }
    break;
  default:
    break;
  }

  if (serdes_112G_mode) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = i * 2;
      set_sds_mux(&tx_map, tx_data + 1, tx_lane);
      set_sds_mux(&tx_map, tx_data, tx_lane - 1);
      set_tx_mux_used(dev_id, mac, tx_data + 1, tx_lane, cfg);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane - 1, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = i * 2;
      set_sds_mux(&rx_map, rx_data - 1, rx_lane);
      set_sds_mux(&rx_map, rx_data, rx_lane + 1);
      set_rx_mux_used(dev_id, mac, rx_data - 1, rx_lane, cfg);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane + 1, cfg);
    }
  } else if (((speed == BF_SPEED_100G) || (speed == BF_SPEED_50G)) &&
             (n_lanes == 2)) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = (chan * 2) + j;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = (chan * 2) + j;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
    }
  } else if ((speed == BF_SPEED_10G) || (speed == BF_SPEED_25G) ||
             ((speed == BF_SPEED_50G) && (n_lanes == 1))) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = i * 2;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = chan * 2;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
    }
  } else if (((speed == BF_SPEED_100G) && (n_lanes == 4)) ||
             (speed == BF_SPEED_40G) ||
             ((speed == BF_SPEED_200G) && (n_lanes == 4))) {
    for (j = 0; j < n_lanes; j++) {
      i = chan + j;
      tx_lane = phys_tx_ln[i];
      tx_data = i;
      set_sds_mux(&tx_map, tx_data, tx_lane);
      set_tx_mux_used(dev_id, mac, tx_data, tx_lane, cfg);

      rx_data = phys_rx_ln[i];
      rx_lane = i;
      set_sds_mux(&rx_map, rx_data, rx_lane);
      set_rx_mux_used(dev_id, mac, rx_data, rx_lane, cfg);
    }
  } else {
    port_mgr_log_error("mac : %0d "
                       " tx/rx maps - invalid speed\n",
                       mac);
    return BF_INVALID_ARG;
  }
  find_and_rpl_dup(dev_id, mac, &tx_map, &rx_map);

  autogen_tmac_wr(dev_id, subdev_id, rx_serdesmux_ofs, rx_map);
  autogen_tmac_wr(dev_id, subdev_id, tx_serdesmux_ofs, tx_map);

  if (en_log) {
    port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d tx-map "
                 "0x%08x rx-map 0x%08x tx-ofs %0x08x rx-ofs %0x08x",
                 dev_id, subdev_id, tmac, mac, ch, tx_map, rx_map,
                 tx_serdesmux_ofs, rx_serdesmux_ofs);
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_lane_map_set
 *
 * Program the lane remapping
 ****************************************************************************/
void port_mgr_tof3_tmac_lane_map_set(bf_dev_id_t dev_id, uint32_t mac,
                                     uint32_t phys_tx_ln[8],
                                     uint32_t phys_rx_ln[8], uint32_t nlanes) {
  port_mgr_tof3_pdev_t *dev_p = port_mgr_dev_physical_dev_tof3_get(dev_id);
  bf_subdev_id_t subdev_id = 0;
  uint32_t tmac = 0;
  uint32_t tx_map = 0, rx_map = 0;
  uint32_t tx_lane, tx_data, rx_lane, rx_data;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t tx_serdesmux_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.cfg_txserdesmux);
  uint32_t rx_serdesmux_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.cfg_rxserdesmux);

  if (!dev_p)
    return;

  if (mac == 0) {
    // pad upper nibble
    tx_map = 0x76540000;
    rx_map = 0x76540000;
  }

  if ((nlanes == 8) || (mac == 0)) {
    for (uint32_t i = 0; i < nlanes; i++) {
      tx_lane = 4 * dev_p->tmac[mac].phys_tx_ln[i];
      tx_data = i;
      tx_map |= (tx_data << tx_lane);

      rx_data = dev_p->tmac[mac].phys_rx_ln[i];
      rx_lane = i * 4;
      rx_map |= (rx_data << rx_lane);
    }
  } else {
#ifndef DEVICE_IS_EMULATOR
    // Map odd and even serdes
    port_mgr_tof3_tmac_serdes_lane_map_set(
        dev_id, subdev_id, mac, tmac, 0, BF_SPEED_400G, 4, show_mux_use, false);
    return;
#endif
  }

#ifdef DEVICE_IS_EMULATOR
  if (nlanes == 8) {
    rx_map = 0x75316420;
    tx_map = 0x73625140;
  } else { // 112g mode
    rx_map = tx_map = 0x76543210;
  }
#endif

  autogen_tmac_wr(dev_id, subdev_id, rx_serdesmux_ofs, rx_map);
  autogen_tmac_wr(dev_id, subdev_id, tx_serdesmux_ofs, tx_map);

  if (show_mux_use) {
    port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d tx-map 0x%08x "
                 "rx-map 0x%08x tx-ofs %0x08x rx-ofs %0x08x",
                 dev_id, subdev_id, tmac, mac, tx_map, rx_map, tx_serdesmux_ofs,
                 rx_serdesmux_ofs);
  }
}

/*****************************************************************************
 * port_mgr_tof3_tmac_tx_reset_set
 *
 * Reset Tx path in the tmac.
 * One place this is required is after autonegotiatio. This is due to the
 * CLK change executed in the serdes tile based on the autoneg HCD. The CLK
 * change can corrupt the tmac-to-serdes FIFO and mus be reset to ensure
 * proper operation.
 ****************************************************************************/
void port_mgr_tof3_tmac_tx_reset_set(bf_dev_id_t dev_id, uint32_t mac,
                                     uint32_t ch) {
  (void)dev_id;
  (void)mac;
  (void)ch;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_rx_reset_set
 *
 * Reset Rx path in the tmac.
 * One place this is required is after autonegotiatio. This is due to the
 * CLK change executed in the serdes tile based on the autoneg HCD. The CLK
 * change can corrupt the tmac-to-serdes FIFO and mus be reset to ensure
 * proper operation.
 ****************************************************************************/
void port_mgr_tof3_tmac_rx_reset_set(bf_dev_id_t dev_id, uint32_t mac,
                                     uint32_t ch) {
  (void)dev_id;
  (void)mac;
  (void)ch;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_sigovrd_set
 *
 * force rxsigok lo or "pass-thru"
 ****************************************************************************/
void port_mgr_tof3_tmac_sigovrd_set(bf_dev_id_t dev_id, uint32_t mac,
                                    uint32_t ch, uint32_t n_lanes,
                                    bf_sigovrd_fld_t ovrd_val) {
  (void)dev_id;
  (void)mac;
  (void)ch;
  (void)n_lanes;
  (void)ovrd_val;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_loopback_pipe_set
 *
 * Configure (or un-configure) pipe tmac loopback mode
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_loopback_pipe_set(bf_dev_id_t dev_id,
                                                 uint32_t mac, uint32_t ch,
                                                 bool enable) {
  bf_subdev_id_t subdev_id = 0;
  uint32_t tmac = 0;
  bf_status_t rc;

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  uint32_t reg32, fld32 = enable ? 1 : 0;
  uint32_t chnl_ena = 0;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_get(dev_id, subdev_id, tmac, ch,
                                                &reg32, &chnl_ena, true);
  if (chnl_ena) {
    // lpbk needs to be set before chnl_ena goes from 0->1, so set chnl_ena=0
    // first
    // So, if chnl_ena is already set, de-assert it and re-assert afterwards
    // Otherwise, port is disabled and chnl_ena will go from 0 -> 1 on enable
    tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_rmw(dev_id, subdev_id, tmac, ch,
                                                  &reg32, 0);
  }
  tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_rmw(dev_id, subdev_id, tmac, ch,
                                                 &reg32, fld32);
  if (chnl_ena) {
    // then renable the chnl
    tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_rmw(dev_id, subdev_id, tmac, ch,
                                                  &reg32, 1);
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_loopback_pcs_near_set
 *
 * Configure (or un-configure) PCS-NEAR loopback mode
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_loopback_pcs_near_set(bf_dev_id_t dev_id,
                                                     uint32_t mac, uint32_t ch,
                                                     bool enable) {
  bf_subdev_id_t subdev_id = 0;
  uint32_t tmac = 0;
  uint32_t reg = 0;
  uint32_t val = 0;
  bf_status_t rc;

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  val = enable ? 1 : 0;
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_set(dev_id, subdev_id, tmac,
                                                     ch, &reg, val, true);

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_loopback_set
 *
 * Configure (or un-configure) one of the several tmac loopback modes
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_loopback_set(bf_dev_id_t dev_id, uint32_t mac,
                                            uint32_t ch,
                                            bf_loopback_mode_e mode) {
  bf_status_t rc;

  if (mode == BF_LPBK_NONE) {
    /* be sure all loopbacks are disabled */

    /* Currently only PIPE and PCS-NEAR loopback have been implemented */
    rc = port_mgr_tof3_tmac_loopback_pipe_set(dev_id, mac, ch, false);
    if (rc != BF_SUCCESS) {
      port_mgr_log_error(
          "Not able to disable pipe loopback, dev %d mac %d ch %d", dev_id, mac,
          ch);
    }
    rc = port_mgr_tof3_tmac_loopback_pcs_near_set(dev_id, mac, ch, false);
    if (rc != BF_SUCCESS) {
      port_mgr_log_error(
          "Not able to disable pcs near loopback, dev %d mac %d ch %d", dev_id,
          mac, ch);
    }
  } else if (mode == BF_LPBK_PIPE) {
    rc = port_mgr_tof3_tmac_loopback_pipe_set(dev_id, mac, ch, true);
  } else if (mode == BF_LPBK_PCS_NEAR) {
    rc = port_mgr_tof3_tmac_loopback_pcs_near_set(dev_id, mac, ch, true);
  } else {
    rc = BF_INVALID_ARG;
  }

  return rc;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_deinit
 *
 ****************************************************************************/
void port_mgr_tof3_tmac_deinit(bf_dev_id_t dev_id) {
  bf_sys_mutex_del(&tmac_stats_mtx[dev_id]);
}

/*****************************************************************************
 * port_mgr_tof3_tmac_init
 *
 * Apply one-tie tmac configurations.
 ****************************************************************************/
void port_mgr_tof3_tmac_init(bf_dev_id_t dev_id, uint32_t clk_div) {
  bf_sys_mutex_init(&tmac_stats_mtx[dev_id]);

  (void)clk_div;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_forced_sigok_get
 *
 * Return the current settings of the RxSigOk force-hi and force-lo
 * registers.
 ****************************************************************************/
void port_mgr_tof3_tmac_forced_sigok_get(bf_dev_id_t dev_id, uint32_t mac,
                                         uint32_t ch, uint32_t n_lanes,
                                         uint32_t *force_hi_raw_val,
                                         uint32_t *force_lo_raw_val,
                                         uint32_t *force_hi,
                                         uint32_t *force_lo) {
  (void)dev_id;
  (void)mac;
  (void)ch;
  (void)n_lanes;
  (void)force_hi_raw_val;
  (void)force_lo_raw_val;
  (void)force_hi;
  (void)force_lo;
}
/*****************************************************************************
 * port_mgr_tof3_tmac_dis_all_set
 *
 * Disable ALL tmac leaf interrupts
 ****************************************************************************/
void port_mgr_tof3_tmac_dis_all_set(bf_dev_id_t dev_id,
                                    bf_subdev_id_t subdev_id) {
  uint32_t rx_status_reg = 0;
  uint32_t *data_p = &rx_status_reg;
  uint32_t tmac = 0;

  for (tmac = 0; tmac <= 32; tmac++) {
    uint32_t ofs_en0 =
        offsetof(tof3_reg, eth400g[tmac].eth400g_mac.link_intr.en0);
    uint32_t ofs_en1 =
        offsetof(tof3_reg, eth400g[tmac].eth400g_mac.link_intr.en1);
    autogen_tmac_wr(dev_id, subdev_id, ofs_en0, *data_p);
    *data_p = 0;
    autogen_tmac_wr(dev_id, subdev_id, ofs_en1, *data_p);
  }

  (void)dev_id;
}
/*****************************************************************************
 * port_mgr_tof3_tmac_int_en_set
 *
 * Enable tmac to generate interrupts
 ****************************************************************************/
void port_mgr_tof3_tmac_int_en_set(bf_dev_id_t dev_id, uint32_t mac, int ch,
                                   bool on) {

  uint32_t rx_status_reg = 0;
  uint32_t *data_p = &rx_status_reg;
  uint32_t tmac = 0;
  int value = (on ? 1 : 0);
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_mac.link_intr.en0);

  autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);

  switch (ch) {
  case 0:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down0(data_p, value);
    break;
  case 1:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down1(data_p, value);
    break;
  case 2:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down2(data_p, value);
    break;
  case 3:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down3(data_p, value);
    break;
  default:
    break;
  }
  autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);

  return;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_local_fault_int_en_set
 *
 * Enable/disable LF interrupt generation
 ****************************************************************************/
void port_mgr_tof3_tmac_local_fault_int_en_set(bf_dev_id_t dev_id, uint32_t mac,
                                               uint32_t ch, bool en) {
  (void)dev_id;
  (void)mac;
  (void)ch;
  (void)en;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_remote_fault_int_en_set
 *
 * Enable/disable RF interrupt generation
 ****************************************************************************/
void port_mgr_tof3_tmac_remote_fault_int_en_set(bf_dev_id_t dev_id,
                                                uint32_t mac, uint32_t ch,
                                                bool en) {
  uint32_t rx_status_reg = 0;
  uint32_t *data_p = &rx_status_reg;
  uint32_t tmac = 0;
  int value = (en ? 1 : 0);
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_mac.link_intr.en0);

  autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);

  switch (ch) {
  case 0:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault0(data_p, value);
    break;
  case 1:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault1(data_p, value);
    break;
  case 2:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault2(data_p, value);
    break;
  case 3:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault3(data_p, value);
    break;
  default:
    break;
  }
  autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);

  return;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_tx_local_fault_set
 *
 * Force Tx /LF
 ****************************************************************************/
void port_mgr_tof3_tmac_tx_local_fault_set(bf_dev_id_t dev_id, uint32_t mac,
                                           uint32_t ch, bool on) {
  (void)dev_id;
  (void)mac;
  (void)ch;
  (void)on;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_tx_remote_fault_set
 *
 * Force Tx /LF
 ****************************************************************************/
void port_mgr_tof3_tmac_tx_remote_fault_set(bf_dev_id_t dev_id, uint32_t mac,
                                            uint32_t ch, bool on) {
  (void)dev_id;
  (void)mac;
  (void)ch;
  (void)on;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_tx_idle_set
 *
 * Force Tx /LF
 ****************************************************************************/
void port_mgr_tof3_tmac_tx_idle_set(bf_dev_id_t dev_id, uint32_t mac,
                                    uint32_t ch, bool on) {
  (void)dev_id;
  (void)mac;
  (void)ch;
  (void)on;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_rx_enable
 *
 * Enable/disable a tmac RX channel
 ****************************************************************************/
void port_mgr_tof3_tmac_rx_enable(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch,
                                  bool rx_en) {
  uint32_t data32, val;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  data32 = 0;
  val = (rx_en == true) ? 1 : 0;
  //  tof3_eth400g_mac_rspec_cfg_pma_rx_en_rmw(dev_id, subdev_id, tmac, &data32,
  //  val);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);

  data32 = 0;
  val = (rx_en == true) ? 1 : 0;
  // tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_rmw(dev_id, subdev_id, tmac, &data32,
  // val);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_rmw(
      dev_id, subdev_id, tmac, ch, &data32, val);

  return;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_tx_enable
 *
 * Enable/disable a tmac TX channel
 ****************************************************************************/
void port_mgr_tof3_tmac_tx_enable(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch,
                                  bool tx_en) {
  uint32_t data32, val;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;
  if (tmac_is_running_on_model(dev_id))
    return;

#if 0
  // soft reset
  data32 = 0;
  val = ((tx_en == false)? 1 : 0);
  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_rmw(
      dev_id, subdev_id, tmac, ch, &data32, val);
#endif
  // disable tx 0 - disabled 1 - enabled
  data32 = 0x0;
  val = ((tx_en == false) ? 0 : 1);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(dev_id, subdev_id, tmac, ch,
                                               &data32, val);
  return;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_tx_drain_set
 *
 * Enable/disable a tmac TX fifo drain per channel
 ****************************************************************************/
void port_mgr_tof3_tmac_tx_drain_set(bf_dev_id_t dev_id, uint32_t mac,
                                     uint32_t ch, bool tx_drain_en) {
  uint32_t data32, val;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;
  if (tmac_is_running_on_model(dev_id))
    return;

  // Flush the fifo
  data32 = 0;
  val = ((tx_drain_en == true) ? 1 : 0);
  tof3_eth400g_app_rspec_txff_ctrl_tx_flush_rmw(dev_id, subdev_id, tmac, ch,
                                                &data32, val);

  return;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_flowcontrol_set
 *
 * Set PFC/link pause configs in tmac
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_flowcontrol_set(bf_dev_id_t dev_id, uint32_t mac,
                                               uint32_t ch) {
  (void)dev_id;
  (void)mac;
  (void)ch;

#if 0
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;
#endif

  return BF_INVALID_ARG;
}

/*****************************************************************************
 * port_mgr_tof3_tmac_handle_interrupts
 *
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_handle_interrupts(bf_dev_id_t dev_id,
                                                 bf_subdev_id_t subdev_id,
                                                 uint32_t ch,
                                                 uint32_t physical_tmac,
                                                 uint32_t intr_status_val) {
  port_mgr_dev_t *dev_p = NULL;
  port_mgr_port_t *port_p = NULL;
  uint32_t logical_tmac = 0;
  port_mgr_ldev_t *ldev_p = port_mgr_dev_logical_dev_get(dev_id);
  if (physical_tmac >= TMAC_TOF3_MAX)
    return BF_INVALID_ARG;
  bf_status_t rc = bf_map_physical_tmac_to_logical(
      dev_id, subdev_id, physical_tmac, &logical_tmac);
  if (rc != BF_SUCCESS)
    return rc;
  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL)
    return BF_INVALID_ARG;
  bf_dev_port_t dev_port;
  lld_sku_map_mac_ch_to_dev_port_id(dev_id, logical_tmac, ch, &dev_port);
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL)
    return BF_INVALID_ARG;
  if (intr_status_val & (0x0100 << ch)) {
    port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
                 "down interrupt set",
                 dev_id, subdev_id, physical_tmac, logical_tmac, ch);
  }

  if (intr_status_val & (0x1000 << ch)) {
    port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
                 "fault interrupt set",
                 dev_id, subdev_id, physical_tmac, logical_tmac, ch);
  }

  if (!port_p->sw.oper_state) {
    port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
                 "Link is down",
                 dev_id, subdev_id, physical_tmac, logical_tmac, ch);
    return BF_SUCCESS;
  }
  // bf_port_oper_state_get_and_issue_callbacks(dev_id, dev_port, &state);
  bf_port_oper_state_set_pending_callbacks(dev_id, dev_port, false);
  ldev_p->port_intr_bhalf_valid = true;
  return BF_SUCCESS;
}
/*****************************************************************************
 * port_mgr_tof3_tmac_enable_interrupts
 *
 ****************************************************************************/
bf_status_t port_mgr_tof3_tmac_enable_interrupts(bf_dev_id_t dev_id,
                                                 uint32_t mac, uint32_t ch,
                                                 bool ena) {
  uint32_t rx_status_reg = 0;
  uint32_t *data_p = &rx_status_reg;
  uint32_t tmac = 0;
  int value = (ena ? 1 : 0);
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;
  if (tmac >= TMAC_TOF3_MAX)
    return BF_INVALID_ARG;

  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_mac.link_intr.en0);

  autogen_tmac_rd(dev_id, subdev_id, ofs, data_p); // use sub-dev-id API - TBD

  switch (ch) {
  case 0:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault0(data_p, value);
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down0(data_p, value);
    break;
  case 1:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault1(data_p, value);
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down1(data_p, value);
    break;
  case 2:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault2(data_p, value);
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down2(data_p, value);
    break;
  case 3:
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_remotefault3(data_p, value);
    setp_tof3_eth400g_mac_rspec_link_intr_stat_link_down3(data_p, value);
    break;
  default:
    break;
  }
  autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);

  return BF_SUCCESS;
}

void port_mgr_tof3_tmac_pcs_status_get(bf_dev_id_t dev_id, uint32_t mac,
                                       uint32_t ch, bf_tof3_pcs_status_t *pcs) {
  uint32_t rx_status_reg = 0;
  uint32_t *data_p = &rx_status_reg;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_mac.rx_status[ch]);
  if (!pcs)
    return;

  autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
  pcs->rx_status_reg = rx_status_reg;

  pcs->tx_rdy = getp_tof3_eth400g_mac_rspec_rx_status_tx_rdy(data_p);
  pcs->rx_rdy = getp_tof3_eth400g_mac_rspec_rx_status_rx_rdy(data_p);
  pcs->rx_status =
      getp_tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status(data_p);

  pcs->rs_rx_fault_remote =
      getp_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote(data_p);
  pcs->rs_rx_fault_local =
      getp_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local(data_p);
  pcs->rs_tx_fault = getp_tof3_eth400g_mac_rspec_rx_status_rs_tx_fault(data_p);
  pcs->rs_rx_link_interruption =
      getp_tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption(data_p);

  pcs->degraded_local =
      getp_tof3_eth400g_mac_rspec_rx_status_degraded_local(data_p);
  pcs->degraded_remote =
      getp_tof3_eth400g_mac_rspec_rx_status_degraded_remote(data_p);

  pcs->am_lock_all = getp_tof3_eth400g_mac_rspec_rx_status_am_lock_all(data_p);
  pcs->am_map_ok = getp_tof3_eth400g_mac_rspec_rx_status_am_map_ok(data_p);
  pcs->block_lock_all =
      getp_tof3_eth400g_mac_rspec_rx_status_block_lock_all(data_p);
  pcs->align_status =
      getp_tof3_eth400g_mac_rspec_rx_status_align_status(data_p);

  pcs->hi_ber = getp_tof3_eth400g_mac_rspec_rx_status_hi_ber(data_p);
  pcs->hi_ser = getp_tof3_eth400g_mac_rspec_rx_status_hi_ser(data_p);

  if (pcs->tx_rdy && pcs->rx_rdy && pcs->rx_status &&
      (!pcs->rs_rx_fault_remote) && (!pcs->rs_rx_fault_local)) {
    pcs->up = true;
  } else {
    pcs->up = false;
  }
}

void port_mgr_tof3_tmac_fec_status_get(bf_dev_id_t dev_id, uint32_t mac,
                                       uint32_t ch, bf_tof3_fec_status_t *fec) {
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);

  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;
  if (!fec)
    return;

  // uint32_t ofs = offsetof(tof3_reg,
  // eth400g[tmac].eth400g_mac.krpfec_err_distr_cnt[ch*16]);
  uint32_t ofs = offsetof(
      tof3_reg, eth400g[tmac].eth400g_mac.krpfec_err_distr_cnt[ch * 32]);

  for (int ctr = 0; ctr < 16; ctr++) {
    lld_subdev_read_register(dev_id, subdev_id, ofs + 4 * ctr, &fec->cnt[ctr]);
  }
}

void port_mgr_tof3_tmac_reset_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch,
                                  uint32_t val) {
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  uint32_t data32 = 0;

  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  uint32_t mac_reset_ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);

  // Release the reset
  data32 = 0;
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(&data32, val);
  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(&data32, val);
  autogen_tmac_wr(dev_id, subdev_id, mac_reset_ofs, data32);
}

void port_mgr_tof3_tmac_fec_lane_symb_err_counter_get(bf_dev_id_t dev_id,
                                                      uint32_t mac, uint32_t ch,
                                                      uint32_t n_ctrs,
                                                      uint64_t symb_err[16],
                                                      int lanes_per_ch) {
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  uint32_t dta_lo = 0, dta_hi = 0;

  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  if (tmac >= TMAC_TOF3_MAX)
    return;

  bf_dev_port_t dev_port;
  lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac, ch, &dev_port);
  int num_lanes_per_mac = lld_get_num_serdes_per_mac(dev_id, dev_port);
  if (num_lanes_per_mac == 4) {
    ch *= 2;
  }
  uint32_t ofs =
      offsetof(tof3_reg, eth400g[tmac].eth400g_mac.fec_lane_symb_err_counter);

  for (uint32_t n = 0; n < n_ctrs; n++) {
    uint32_t idx;
    if (lanes_per_ch == 2) {
      idx = 2 * ((ch * 2) + n);
    } else {
      idx = 2 * ((ch * 2 * 2) + n);
    }
    lld_subdev_read_register(dev_id, subdev_id, ofs + idx * 4, &dta_lo);
    lld_subdev_read_register(dev_id, subdev_id, ofs + idx * 4 + 4, &dta_hi);
    symb_err[n] = (uint64_t)dta_hi << 32ul | (uint64_t)dta_lo;
    // hack
    // printf("mac=%d : ch=%d : nlanes=%d : ctr=%d : idx=%d : cnt=%lu\n",
    //		    mac, ch, lanes_per_ch, n, idx, symb_err[n]);
  }
}

/*****************************************************************************
 * port_mgr_tof2_umac_rs_fec_status_and_counters_get
 *
 ****************************************************************************/
void port_mgr_tof3_tmac_rs_fec_status_and_counters_get(
    bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, int lanes_per_ch,
    bool *hi_ser, bool *fec_align_status, uint32_t *fec_corr_cnt,
    uint32_t *fec_uncorr_cnt, uint32_t *fec_ser_lane_0,
    uint32_t *fec_ser_lane_1, uint32_t *fec_ser_lane_2,
    uint32_t *fec_ser_lane_3, uint32_t *fec_ser_lane_4,
    uint32_t *fec_ser_lane_5, uint32_t *fec_ser_lane_6,
    uint32_t *fec_ser_lane_7) {
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t tmac, val, val32[2], unused_fld;
  uint64_t symb_err[16];

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;

  tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_get(
      dev_id, subdev_id, tmac, ch, val32, &unused_fld, true);
  *fec_uncorr_cnt = val32[0];
  tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_get(
      dev_id, subdev_id, tmac, ch, val32, &unused_fld, true);
  *fec_corr_cnt = val32[0];

  port_mgr_tof3_tmac_fec_lane_symb_err_counter_get(dev_id, mac, ch, 16,
                                                   symb_err, lanes_per_ch);
  tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_get(
      dev_id, subdev_id, tmac, ch, &unused_fld, &val, true);
  tof3_eth400g_mac_rspec_rx_status_hi_ser_get(dev_id, subdev_id, tmac, ch,
                                              &unused_fld, &val, true);
}

bf_status_t port_mgr_tof3_tmac_1588_timestamp_delta_tx_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t *delta) {
  uint32_t val = 0;
  uint32_t data_p = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t tmac = 0;
  uint32_t mac = 0;
  uint32_t ch = 0;

  if (delta == NULL) {
    return BF_INVALID_ARG;
  }

  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &mac,
                                         &ch, NULL);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_get(
      dev_id, subdev_id, tmac, ch, &data_p, &val, true);

  *delta = val;
  return BF_SUCCESS;
}

bf_status_t port_mgr_tof3_tmac_1588_timestamp_delta_rx_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t *delta) {

  uint32_t val = 0;
  uint32_t data_p = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t tmac = 0;
  uint32_t mac = 0;
  uint32_t ch = 0;

  if (delta == NULL) {
    return BF_INVALID_ARG;
  }

  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &mac,
                                         &ch, NULL);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_get(
      dev_id, subdev_id, tmac, ch, &data_p, &val, true);

  *delta = val;
  return BF_SUCCESS;
}

bf_status_t port_mgr_tof3_tmac_1588_timestamp_delta_tx_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t delta) {

  uint32_t val = delta;
  uint32_t data_p = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t tmac = 0;
  uint32_t mac = 0;
  uint32_t ch = 0;

  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &mac,
                                         &ch, NULL);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_set(dev_id, subdev_id, tmac,
                                                        ch, &data_p, val, true);

  return BF_SUCCESS;
}

bf_status_t port_mgr_tof3_tmac_1588_timestamp_delta_rx_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint16_t delta) {

  uint32_t val = delta;
  uint32_t data_p = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t tmac = 0;
  uint32_t mac = 0;
  uint32_t ch = 0;

  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &mac,
                                         &ch, NULL);
  if (rc != BF_SUCCESS)
    return rc;

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_set(dev_id, subdev_id, tmac,
                                                        ch, &data_p, val, true);

  return BF_SUCCESS;
}

bf_status_t port_mgr_tof3_tmac_1588_timestamp_tx_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint64_t *ts,
                                                     bool *ts_valid,
                                                     int *ts_id) {
  uint64_t val = 0;
  uint64_t data_p = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t mac = 0;
  uint32_t ch = 0;
  uint32_t tmac = 0;

  if ((ts_valid == NULL) || (ts_id == NULL) || (ts == NULL)) {
    return BF_INVALID_ARG;
  }

  rc = port_mgr_tof3_map_dev_port_to_all(dev_id, dev_port, NULL, NULL, &mac,
                                         &ch, NULL);
  if (rc != BF_SUCCESS)
    return rc;

  *ts_valid = false;
  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return rc;

  tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(dev_id, subdev_id, tmac, ch,
                                              &data_p, &val, true);
  if (val & 0x8000000000000000ull) {
    *ts = (data_p & 0xFFFFFFFFFFFF);
    *ts_id = ((data_p >> 48) & 0x1FF);
    *ts_valid = true;
  }
  return BF_SUCCESS;
}

/*Usage: Cleans up PFC data path, workaround
 *
 * Steps: Enable Override for RX PFC XOFF
 *                      eth400g_app.txff_ctrl. ovr_rx_pfcxoff = 1
 *        Set  PFC xoff for all priority
 *                      eth400g_app.val_rx_pfcxoff = 0xff
 *        Reset PFC xoff for all priority
 *                      eth400g_app.val_rx_pfcxoff = 0
 *        Disable override for RX PFC XOFF
 *                      eth400g_app.txff_ctrl. ovr_rx_pfcxoff = 0
 *
 */
bf_status_t port_mgr_tof3_tmac_clear_pfc_data_path(bf_dev_id_t dev_id,
                                                   uint32_t mac, uint32_t ch) {
  uint32_t data32 = 0, val = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc;
  uint32_t tmac;
  bf_sku_chip_part_rev_t rev = BF_SKU_CHIP_PART_REV_B0;
  lld_sku_get_chip_part_revision_number(dev_id, &rev);

  rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return BF_INVALID_ARG;

  if (tmac_is_running_on_model(dev_id))
    return BF_SUCCESS;

  if ((rev != BF_SKU_CHIP_PART_REV_A1) && (rev != BF_SKU_CHIP_PART_REV_A0)) {
    return BF_SUCCESS;
  }

  port_mgr_log("MAC: dev_id %0d subdev_id %d tmac %d mac %d ch %d "
               "Clear PFC data path",
               dev_id, subdev_id, tmac, mac, ch);

  // Other bits are don't care at this stage
  data32 = 0;
  val = 1;
  tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_rmw(dev_id, subdev_id, tmac,
                                                      ch, &data32, val);

  data32 = 0;
  val = 0xFF;
  tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_rmw(dev_id, subdev_id, tmac,
                                                      ch, &data32, val);

  data32 = 0;
  val = 0x00;
  tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_rmw(dev_id, subdev_id, tmac,
                                                      ch, &data32, val);

  data32 = 0;
  val = 0;
  tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_rmw(dev_id, subdev_id, tmac,
                                                      ch, &data32, val);

  return BF_SUCCESS;
}
void port_mgr_tof3_tmac_ignore_fault(bf_dev_id_t dev_id, uint32_t mac,
                                     uint32_t ch, bool en) {
  uint32_t data32 = 0, val = 0;
  uint32_t tmac = 0;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc =
      bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS)
    return;
  if (tmac >= TMAC_TOF3_MAX)
    return;
  data32 = 0;

  val = (en == true) ? 0 : 1;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_rmw(dev_id, subdev_id, tmac,
                                                     ch, &data32, val);
  return;
}
