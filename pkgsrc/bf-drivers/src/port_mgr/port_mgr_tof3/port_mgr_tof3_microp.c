/*******************************************************************************
 * BAREFOOT NETWORKS CONFIDENTIAL & PROPRIETARY
 *
 * Copyright (c) 2015-2019 Barefoot Networks, Inc.

 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the property of
 * Barefoot Networks, Inc. and its suppliers, if any. The intellectual and
 * technical concepts contained herein are proprietary to Barefoot Networks,
 * Inc.
 * and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from
 * Barefoot Networks, Inc.
 *
 * No warranty, explicit or implicit is provided, unless granted under a
 * written agreement with Barefoot Networks, Inc.
 *
 * $Id: $
 *
 ******************************************************************************/
#include <stdio.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <dvm/dvm_intf.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_log.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_sku.h>
#include <tof3_regs/tof3_reg_drv.h>

#define HOST_BLK_MICROP_ID 0x21
#define CPU_MICROP_ID 0x0

extern bool microp_init_done;

static bf_status_t port_mgr_tof3_microp_halt_pcie_dbg_logging(
    bf_dev_id_t d2ev_id);

/********************************************************************
 *
 * Initialize all the tv80 micro-processors on this Tofino2
 *******************************************************************/
void port_mgr_tof3_microp_init(bf_dev_id_t dev_id,
                               bf_device_profile_t *profile) {
  port_mgr_log("uP: Terminate and log PCIE debug info");
  port_mgr_tof3_microp_halt_pcie_dbg_logging(dev_id);

  // allow programming thru tv80 now (if configured)
  microp_init_done = true;
}

/*
When PCIe boots up and SW want to TV80, it will have to change the configuration
of TV80 memory (reading out of PCIe debug entries.

each entry is 32bit and log different events:
- [31:28]: event type
- [27:16]: event time in 100us increment
- [15:0]  : event data

event type list:
- 0001: time overflow (when the 12bit counter of 100us overflow) - bit [27:0] =
absolute time in 409.6 ms increment (4096*100us)
- 0010: reset0 change - event_data = dbg_reset[15:0] (is one of the misc_regs)
- 0011: reset1 change - event data = dbg_reset[31:16]
- 0101: LTSSM change (except detect.active) - event data[5:0] LTSSM encoding,
[7:6]: current rate, [11:8]: pipe_txelecidle, [15:12]: pipe_rxstandby
- 0110: Tx detect Rx - [3:0] latest TxDetectRx result(per lane), [7:4] Previous
TxDetectRx result(per lane), [15:8] consecutive identical result
- 0111: Error detected: [15:0] = error_type (see below)
- 1000: RxEQ lane 0 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1001: RxEQ lane 1 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1010: RxEQ lane 2 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1011: RxEQ lane 3 event data [7:0] = FOM, [13:8]: direction [14]:
invalid_request [15]: always 1
- 1100: TxEQ lane 0 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor
- 1101: TxEQ lane 1 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor
- 1110: TxEQ lane 2 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor
- 1111: TxEQ lane 3 event data [5:0] = cursor, [15:11]: pre-cursor, [10:6]:
post-cursor


Error_type [0]: deskew error
Error_type [1]: 128b/130b framing error (any)
Error_type [2]: received bad TLP
Error_type [3]: Rx Buffer overflow
Error_type [4]: NAK sent
Error_type [5]: Rx bad DLLP
Error_type [6]: Flow Control protocol error
Error_type [7]: Flow control timeout
Error_type [8]: Replay start
Error_type [9]: Replay number error
Error_type [10]: Replay timer error
Error_type [11]: NAK rcvd with correct SeqNum
Error_type [12]: TLP malformed
Error_type [13]: TLP Unexpected completion
Error_type [14]: TLP BAR no match
Error_type [15]: Unsupported TLP
*/

static char *ltssm_state[] = {
    "00h: detect.quiet",
    "01h: detect.active",
    "02h: polling.active",
    "03h: polling.compliance",
    "04h: polling.configuration",
    "05h: config.linkwidthstart",
    "06h: config.linkwidthaccept",
    "07h: config.lanenumwait",
    "08h: config.lanenumaccept",
    "09h: config.complete",
    "0Ah: config.idle",
    "0Bh: recovery.receiverlock",
    "0Ch: recovery.equalization (phase 0)",
    "0Dh: recovery.speed",
    "0Eh: recovery.receiverconfig",
    "0Fh: recovery.idle",
    "10h: L0",
    "11h: L0s",
    "12h: L1.entry",
    "13h: L1.idle",
    "14h: L2.idle/L2.transmitwake",
    "15h: reserved",
    "16h: disable",
    "17h: loopback.entry",
    "18h: loopback.active",
    "19h: loopback.exit",
    "1Ah: hotreset",
    "1Bh: ?",
    "1Ch: ?",
    "1Dh: ?",
    "1Eh: ?",
    "1Fh: ?",
    "20h: ?",
    "21h: recovery.equalization (phase 1)",
    "22h: recovery.equalization (phase 2)",
    "23h: recovery.equalization (phase 3)",
};

static char *rate_str[] = {
    "gen1",
    "gen2",
    "gen3",
    "genX",
};

// 0111: Error detected: [15:0] = error_type (see below)
//
static void decode_wd_error_detected(uint32_t wd) {
  uint32_t evt_typ, evt_tim, evt_dta;

  evt_typ = (wd & 0xFFFF);
  evt_tim = (wd >> 16) & 0xFFF;
  evt_dta = (wd & 0xFFFF);

  if (evt_typ & (1<<0)) {
    port_mgr_log("%4d: %08x : deskew error : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<1)) {
    port_mgr_log("%4d: %08x : 128b/130b framing error (any) : %04x",
                 evt_tim,
                 wd,
                 evt_dta);
  } else if (evt_typ & (1<<2)) {
    port_mgr_log("%4d: %08x : received bad TLP : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<3)) {
    port_mgr_log("%4d: %08x : Rx Buffer overflow : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<4)) {
    port_mgr_log("%4d: %08x : NAK sent : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<5)) {
    port_mgr_log("%4d: %08x : Rx bad DLLP : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<6)) {
    port_mgr_log(
        "%4d: %08x : Flow Control protocol error : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<7)) {
    port_mgr_log(
        "%4d: %08x : Flow control timeout : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<8)) {
    port_mgr_log("%4d: %08x : Replay start: %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<9)) {
    port_mgr_log(
        "%4d: %08x : Replay number error : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<10)) {
    port_mgr_log("%4d: %08x : Replay timer error : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<11)) {
    port_mgr_log("%4d: %08x : NAK rcvd with correct SeqNum : %04x",
                 evt_tim,
                 wd,
                 evt_dta);
  } else if (evt_typ & (1<<12)) {
    port_mgr_log("%4d: %08x : TLP malformed : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<13)) {
    port_mgr_log(
        "%4d: %08x : TLP Unexpected completion : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<14)) {
    port_mgr_log("%4d: %08x : TLP BAR no match : %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ & (1<<15)) {
    port_mgr_log("%4d: %08x : Unsupported TLP : %04x", evt_tim, wd, evt_dta);
  }
}

static void decode_wd(uint32_t wd) {
  uint32_t evt_typ, evt_tim, evt_dta;

  evt_typ = (wd >> 28) & 0xF;
  evt_tim = (wd >> 16) & 0xFFF;
  evt_dta = (wd & 0xFFFF);

  if (evt_typ == 0) {
    port_mgr_log("%4d: %08x : ??: %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ == 1) {
    port_mgr_log(
        "%4d: %08x : time overflow : %dms", evt_tim, wd, (wd & 0x0FFFFFFF));
    port_mgr_log("                  when the 12bit counter of 100us overflow");
    port_mgr_log("      %08x : [27:0] absolute time in 409.6 ms",
                 (wd & 0x0FFFFFFF));
    port_mgr_log("                         increment (4096*100us)");
  } else if (evt_typ == 2) {
    port_mgr_log("%4d: %08x : reset0 change", evt_tim, wd);
    port_mgr_log(
        "             %1x : [0]     : Power-On reset pin (after synchronous "
        "deassertion)",
        ((evt_dta >> 0) & 1));
    port_mgr_log("             %1x : [1]     : Core reset pin after debouncing",
                 ((evt_dta >> 1) & 1));
    port_mgr_log("             %1x : [2]     : PCIe reset pin after debouncing",
                 ((evt_dta >> 2) & 1));
    port_mgr_log("             %1x : [3]     : Power-On Done Status",
                 ((evt_dta >> 3) & 1));
    port_mgr_log("             %1x : [4]     : Power-On Done for PCIe only",
                 ((evt_dta >> 4) & 1));
    port_mgr_log("             %1x : [5]     : Fuse1 load",
                 ((evt_dta >> 5) & 1));
    port_mgr_log("             %1x : [6]     : Fuse2 load",
                 ((evt_dta >> 6) & 1));
    port_mgr_log("             %1x : [7]     : Fuse Reset",
                 ((evt_dta >> 7) & 1));
    port_mgr_log("             %1x : [8]     : Fuse 1 Done",
                 ((evt_dta >> 8) & 1));
    port_mgr_log("             %1x : [9]     : Fuse 2 Done",
                 ((evt_dta >> 9) & 1));
    port_mgr_log("             %1x : [10]    : Fuse 1 Timeout",
                 ((evt_dta >> 10) & 1));
    port_mgr_log(
        "             %1x : [13:11] : PCIe FSM State: COLDRST(000b), "
        "FIRMW(001b), PHY_WAIT(010b),",
        ((evt_dta >> 11) & 7));
    port_mgr_log(
        "                           : CTL_ENA(011b), CTL_RST(100b), "
        "APP_RST(101b)");
    port_mgr_log(
        "             %1x : [14]    : PCIe Controller functional reset",
        ((evt_dta >> 14) & 1));
    port_mgr_log("             %1x : [15]    : PCIe application layer reset",
                 ((evt_dta >> 15) & 1));
  } else if (evt_typ == 3) {
    port_mgr_log("%4d: %08x : reset1 change", evt_tim, wd);
    port_mgr_log("             %1x : [16] : SPI Firmware Enable",
                 ((evt_dta >> 0) & 1));
    port_mgr_log("             %1x : [17] : PCIe PHY initialization done",
                 ((evt_dta >> 1) & 1));
    port_mgr_log(
        "             %1x : [18] : SPI Error captured during firmware loading",
        ((evt_dta >> 2) & 1));
    port_mgr_log("             %1x : [19] : PCIe gen3 advertised after reset",
                 ((evt_dta >> 3) & 1));
    port_mgr_log("             %1x : [20] : Core PLL reset",
                 ((evt_dta >> 4) & 1));
    port_mgr_log("             %1x : [21] : PPS PLL reset",
                 ((evt_dta >> 5) & 1));
    port_mgr_log("             %1x : [22] : MAC 0 PLL reset",
                 ((evt_dta >> 6) & 1));
    port_mgr_log("             %1x : [23] : MAC 1 PLL reset",
                 ((evt_dta >> 7) & 1));
    port_mgr_log("             %1x : [24] : Core  reset", ((evt_dta >> 8) & 1));
    port_mgr_log("             %1x : [25] : MAC   reset", ((evt_dta >> 9) & 1));
    port_mgr_log("             %1x : [26] : MAC Bus reset",
                 ((evt_dta >> 10) & 1));
    port_mgr_log("             %1x : [27] : Core PLL lock",
                 ((evt_dta >> 11) & 1));
    port_mgr_log("             %1x : [28] : PPS PLL lock",
                 ((evt_dta >> 12) & 1));
    port_mgr_log("             %1x : [29] : MAC 0 PLL lock",
                 ((evt_dta >> 13) & 1));
    port_mgr_log("             %1x : [30] : MAC 1 PLL lock",
                 ((evt_dta >> 14) & 1));
    port_mgr_log(
        "             %1x : [31] : Drive CCLK instead of PCLK to PCIe "
        "controller",
        ((evt_dta >> 15) & 1));

  } else if (evt_typ == 4) {
    port_mgr_log("%4d: %08x : ??: %04x", evt_tim, wd, evt_dta);
  } else if (evt_typ == 5) {
    uint32_t index = evt_dta & 0x3F;
    if (index >= sizeof(ltssm_state) / sizeof(ltssm_state[0])) {
      port_mgr_log_warn("Incorrect evt_dta");
      return;
    }
    port_mgr_log("%4d: %08x : LTSSM change", evt_tim, wd);
    port_mgr_log(
        "      %08x : [ 5: 0] LTSSM encoding <%s>", index, ltssm_state[index]);
    port_mgr_log("      %08x : [ 7: 6] current rate <%s>",
                 (evt_dta >> 6) & 3,
                 rate_str[(evt_dta >> 6) & 3]);
    port_mgr_log("      %08x : [11: 8] pipe_txelecidle", (evt_dta >> 8) & 0xF);
    port_mgr_log("      %08x : [15:12] pipe_rxstandby", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 6) {
    port_mgr_log("%4d: %08x : Tx detect Rx", evt_tim, wd);
    port_mgr_log("      %08x : [ 3:0] latest TxDetectRx result(per lane)",
                 evt_dta & 0xF);
    port_mgr_log("      %08x : [ 7:4] Previous TxDetectRx result(per lane)",
                 (evt_dta >> 4) & 0xF);
    port_mgr_log("      %08x : [15:8] consecutive identical result",
                 (evt_dta >> 8) & 0xFF);
  } else if (evt_typ == 7) {
    decode_wd_error_detected(wd);
  } else if (evt_typ == 8) {
    port_mgr_log("%4d: %08x : RxEQ lane 0", evt_tim, wd);
    port_mgr_log("      %08x : [ 7: 0] FOM", evt_dta & 0xFF);
    port_mgr_log("      %08x : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("      %08x : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 9) {
    port_mgr_log("%4d: %08x : RxEQ lane 1", evt_tim, wd);
    port_mgr_log("      %08x : [ 7: 0] FOM", evt_dta & 0xFF);
    port_mgr_log("      %08x : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("      %08x : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 10) {
    port_mgr_log("%4d: %08x : RxEQ lane 2", evt_tim, wd);
    port_mgr_log("      %08x : [ 7: 0] FOM", evt_dta & 0xFF);
    port_mgr_log("      %08x : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("      %08x : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 11) {
    port_mgr_log("%4d: %08x : RxEQ lane 3", evt_tim, wd);
    port_mgr_log("      %08x : [ 7: 0] FOM", evt_dta & 0xFF);
    port_mgr_log("      %08x : [13: 8] direction", (evt_dta >> 8) & 0x1F);
    port_mgr_log("      %08x : [14:14] invalid_request", (evt_dta >> 14) & 1);
  } else if (evt_typ == 12) {
    port_mgr_log("%4d: %08x : TxEQ lane 0", evt_tim, wd);
    port_mgr_log("      %08x : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("      %08x : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    // port_mgr_log("      %08x : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 13) {
    port_mgr_log("%4d: %08x : TxEQ lane 1", evt_tim, wd);
    port_mgr_log("      %08x : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("      %08x : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    // port_mgr_log("      %08x : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 14) {
    port_mgr_log("%4d: %08x : TxEQ lane 2", evt_tim, wd);
    port_mgr_log("      %08x : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("      %08x : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    // port_mgr_log("      %08x : [15:12] cursor", (evt_dta >> 12) & 0xF);
  } else if (evt_typ == 15) {
    port_mgr_log("%4d: %08x : TxEQ lane 3", evt_tim, wd);
    port_mgr_log("      %08x : [ 5: 0] pre-cursor", evt_dta & 0x3F);
    port_mgr_log("      %08x : [11: 6] post-cursor", (evt_dta >> 6) & 0x3F);
    // port_mgr_log("      %08x : [15:12] cursor", (evt_dta >> 12) & 0xF);
  }
}

static bf_status_t port_mgr_tof3_microp_halt_pcie_dbg_logging(
    bf_dev_id_t dev_id) {
  uint32_t num_subdev;
  bf_subdev_id_t subdev_id;
  uint32_t hd;
  uint32_t misc_pcie_debug_log_ofs =
      offsetof(tof3_reg, device_select.misc_all_regs.misc_tv80_regs);

  if (lld_sku_get_num_subdev(dev_id, &num_subdev, NULL) != LLD_OK) {
    return LLD_ERR_BAD_PARM;
  }
  for (subdev_id = 0; subdev_id < (bf_subdev_id_t) num_subdev; subdev_id++) {
    port_mgr_log("Dev%d : Subdev%d PCIe log:", dev_id, subdev_id);
    for (hd = 0; hd < 16384*2; hd += 4) {
      uint32_t log_entry;
      //lld_read_register(dev_id, misc_pcie_debug_log_ofs + hd, &log_entry);
      lld_subdev_read_register(dev_id, subdev_id, misc_pcie_debug_log_ofs + hd, &log_entry);
      if (log_entry == 0) continue;
      decode_wd(log_entry);
    }
  }
  return BF_SUCCESS;
}
