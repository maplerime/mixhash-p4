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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// for aim_printf
#include <target-utils/uCli/ucli.h>
#include <target-utils/uCli/ucli_argparse.h>
#include <target-utils/uCli/ucli_handler_macros.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <port_mgr/port_mgr_intf.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <lld/lld_reg_if.h>
#include <port_mgr/port_mgr_ha.h>
#include <port_mgr/port_mgr.h>
#include <port_mgr/port_mgr_log.h>
#include <port_mgr/port_mgr_dev.h>
#include "port_mgr_mac.h"
#include <port_mgr/port_mgr_map.h>
#include <port_mgr/port_mgr_physical_dev.h>
#include <port_mgr/port_mgr_port.h>
#include "port_mgr_tof1_map.h"
#include <port_mgr/bf_port_if.h>
#include <tofino_regs/tofino.h>
#include "comira_reg_def_autogen.h"
#include "comira_reg_access_autogen.h"

// fwd ref
char *comira_reg_to_str(uint32_t offset);

// Debug mode.
// Enabling this causes alignment markers to be inserted every 127
// 66b blocks to speed link-up time. It will NOT work if connected
// to something else that does not support this mode (such as epgm)
//
extern uint64_t use_short_timers_map;
#define USE_SHORT_TIMER ((use_short_timers_map >> mac_block) & 1)
//#define USE_SHORT_TIMER false

/* Compute the register address offset between adjacent MACs from tofino.h */
#define MAC_STRIDE (offsetof(Tofino, macs_t[1]) - offsetof(Tofino, macs_t[0]))

/*****************************************************************************
 * Comira UMAC addressing
 *
 * "mac_block" specifies one of the (up to) 65 umac instancesa in tofino
 *
 * "offset" specifies the offset of the target register in the umac.
 *
 * Offsets are defined (auto-generated) in comira_reg_def_autogen.h
 * Each 16b Comira register occupies 32b of address space in the tofino
 * address map (hence "(offset*4):).
 ****************************************************************************/

/*****************************************************************************
 * mac_read
 *
 * Read a 16b Comira umac register.
 ****************************************************************************/
uint32_t mac_read(bf_dev_id_t dev_id, int mac_block, uint32_t offset) {
  uint32_t val;
  uint32_t mac_addr = offsetof(Tofino, macs_t) + (mac_block * MAC_STRIDE);
  uint32_t cmra_reg =
      mac_addr + offsetof(Mac_addrmap, comira_regs) + (offset * 4);

  lld_read_register(dev_id, cmra_reg, &val);

  if (0 || port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC:  RD : %d:%2d:- %08x == %08x : %s",
                 dev_id,
                 mac_block,
                 cmra_reg,
                 val,
                 comira_reg_to_str(offset));
  }
  return val;
}

/*****************************************************************************
 * mac_write
 *
 * Write a 16b Comira umac register.
 ****************************************************************************/
void mac_write(bf_dev_id_t dev_id,
               int mac_block,
               uint32_t offset,
               uint32_t val) {
  uint32_t mac_addr = offsetof(Tofino, macs_t) + (mac_block * MAC_STRIDE);
  uint32_t cmra_reg =
      mac_addr + offsetof(Mac_addrmap, comira_regs) + (offset * 4);

  if (0 || port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC:  WR : %d:%2d:- %08x  = %08x : %s",
                 dev_id,
                 mac_block,
                 cmra_reg,
                 val,
                 comira_reg_to_str(offset));
  }

  lld_write_register(dev_id, cmra_reg, val);
  return;
}

/*****************************************************************************
 * port_mgr_default_mac_interrupt_cb
 *
 * Called from port_mgr_mac_interrupt_poll when a umac interrupt status register
 * with asserted interrupts is identified. Parameters include the interrupt
 * register offset (from comira_reg_def_autogen.h) and the asserted
 * interrupt bits.
 *
 * First clears all asserted interrupt bits, then checks for actionable
 * events. The umac interrupt registers are defined in a standard format
 * with the interrupt status register defined at offset "x" and the
 * corresponding "interrupt clear" register defined at offset "x+1". Bit
 * positions are identical in each register.
 *
 * The default umac interrupt handler looks only for link-status change
 * interrupts (local-fault, msk=0x10 and remote-fault, msk=0x20) on each
 * umac channel.
 *
 * On a change in either fault state the master link-status determination
 * function (bf_port_oper_state_get) is called to determine the
 * current operational state of the port.
 *
 ****************************************************************************/
void port_mgr_default_mac_interrupt_cb(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t reg,
                                       uint32_t set_int_bits,
                                       void *userdata) {
  bf_port_link_fault_st_t link_fault;
  port_mgr_port_t *port_p = NULL;
  int mac_block;
  int ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  // regs are defined as "status"=n, "clear"=n+1
  mac_write(dev_id, mac_block, reg + 1, set_int_bits);

  if (((ch == 0) && (reg == interrupts0__intstat3)) ||
      ((ch == 1) && (reg == interrupts0__intstat4)) ||
      ((ch == 2) && (reg == interrupts0__intstat5)) ||
      ((ch == 3) && (reg == interrupts0__intstat6))) {
    if (set_int_bits & 0x30) {  // change in LF or RF

      port_mgr_log("MAC: INT: %d:%2d:%d : dev_port=%3d : %s%s",
                   dev_id,
                   mac_block,
                   ch,
                   dev_port,
                   (set_int_bits & 0x10) ? "LF " : "",
                   (set_int_bits & 0x20) ? "RF" : "");

      /* lld_sku.c handles mapping of mac-block and channel to the
       * corresponding dev_port, based on the specific tofino model
       * identified by the efuse bits */
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

      port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
      if (!port_p) return;

      port_p->lstate.remote_fault |= (set_int_bits & 0x20) ? 1 : 0;
      port_p->lstate.local_fault |= (set_int_bits & 0x10) ? 1 : 0;

      // Update alt_link_fault_st.
      if (port_p->lstate.local_fault) {
        link_fault = BF_PORT_LINK_FAULT_LOC_FAULT;
      } else {
        link_fault = BF_PORT_LINK_FAULT_REM_FAULT;
      }
#if 0
      port_mgr_log(
          "MAC: INT: %d:%2d:%d : dev_port=%3d : alt_link_fault %d->%d",
          dev_id,
          mac_block,
          ch,
          dev_port,
          port_p->fsm_ext.alt_link_fault_st,
          link_fault);
#endif
      port_p->fsm_ext.alt_link_fault_st = link_fault;

      /* It is not needed to check the port state again: if link fault
       * interrupt has hit, there is/was a link fault condition: trigger
       * "link-dn-actions" if link is going from UP to DOWN.
       */
      bf_port_oper_state_set_pending_callbacks(dev_id, dev_port, false);
      port_mgr_ldev_t *dev_p = port_mgr_dev_logical_dev_get(dev_id);
      if (dev_p != NULL) {
        dev_p->port_intr_bhalf_valid = true;
      }
    }
  }
  (void)userdata;
}

/*****************************************************************************
 * port_mgr_mac_register_default_handler_for_mac_ints
 *
 * Register the default interrupt handler for all umac interrupts (to make
 * sure any asserted interrupts get cleared properly). Registration consists
 * of two parts. First bind a callback function (lld_default_mac_interrupt_cb)
 * to the port that corresponds to each mac-block/channel. Second, indicate
 * which interrupts are of interest (i.e. which bits in each register should
 * generate a callback...in this case, all bits).
 *
 ****************************************************************************/
void port_mgr_mac_register_default_handler_for_mac_ints(bf_dev_id_t dev_id) {
  int mac_block, ch, rc, int_reg;
  uint32_t comira_int_regs[] = {interrupts0__intstat0,
                                interrupts0__intstat1,
                                interrupts0__intstat2,
                                interrupts0__intstat3,
                                interrupts0__intstat4,
                                interrupts0__intstat5,
                                interrupts0__intstat6,
                                interrupts0__intstat7,
                                interrupts0__intstat8,
                                interrupts0__intstat9,
                                interrupts0__intstat10,
                                interrupts0__intstat11,
                                interrupts0__intstat12,
                                interrupts0__intstat13,
                                interrupts0__intstat14,
                                interrupts0__intstat15};

  for (mac_block = 0; mac_block < lld_get_max_mac_blocks(dev_id); mac_block++) {
    bf_dev_port_t dev_port;
    lld_err_t err = lld_sku_map_mac_ch_to_dev_port_id(
        dev_id, mac_block, 0 /*ch*/, &dev_port);
    if (err != LLD_OK) continue;  // invalid mac-block for sku

    for (ch = 0; ch < 4; ch++) {
      rc = port_mgr_mac_bind_interrupt_callback(
          dev_id, mac_block, ch, port_mgr_default_mac_interrupt_cb, NULL);
      if (rc != 0) {
        port_mgr_log(
            "ERR: Int cb registration failed: rc=%d : dev_id=%d : mac_block=%d "
            ": ch=%d",
            rc,
            dev_id,
            mac_block,
            ch);
        // mac-block invalid for this sku, just skip it
        continue;
      }
      for (int_reg = 0; int_reg < (int)(sizeof(comira_int_regs) /
                                        sizeof(comira_int_regs[0]));
           int_reg++) {
        port_mgr_mac_interrupt_registration(
            dev_id, mac_block, ch, comira_int_regs[int_reg], 0xffff);
      }
    }
  }

  port_mgr_port_bind_int_bh_wakeup_callback(
      dev_id, port_mgr_port_default_int_bh_wakeup_cb);
}

void port_mgr_mac_int_fault_get(bf_dev_id_t dev_id,
                                int mac_block,
                                int ch,
                                int *local_fault,
                                int *remote_fault) {
  int int_stat_reg_per_ch[] = {interrupts0__intstat3,
                               interrupts0__intstat4,
                               interrupts0__intstat5,
                               interrupts0__intstat6};

  int int_clr_reg_per_ch[] = {interrupts0__intclr3,
                              interrupts0__intclr4,
                              interrupts0__intclr5,
                              interrupts0__intclr6};
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  uint32_t data;

  if (!local_fault || !remote_fault) {
    return;
  }

  *local_fault = 0;
  *remote_fault = 0;

  data = mac_read(dev_id, mac_block, int_stat_reg_per_ch[ch]);

  if (data & 0x10) {
    *local_fault = 1;
    mac_write(dev_id, mac_block, int_clr_reg_per_ch[ch], 0x10);
  }

  if (data & 0x20) {
    *remote_fault = 1;
    mac_write(dev_id, mac_block, int_clr_reg_per_ch[ch], 0x20);
  }

  if (data & 0x30) {
    port_mgr_log("%d : MAC%d: CH%d: link_interrups: 0x%4.4x RF: %0d, LF: %0d",
                 dev_id,
                 mac_block,
                 ch,
                 data,
                 *remote_fault,
                 *local_fault);
  }

  /* Save link interrupts values */
  lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p) {
    port_p->lstate.remote_fault |= *remote_fault;
    port_p->lstate.local_fault |= *local_fault;
    port_p->lstate.lnk_int_fcnt++;
  }
}

/*****************************************************************************
 * port_mgr_mac_int_fault_cache_get
 *
 * Return the cached values of link state interrupt.
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_int_fault_cache_get(bf_dev_id_t dev_id,
                                             int mac_block,
                                             int ch,
                                             int *local_fault,
                                             int *remote_fault) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  int remote_fault_tmp;
  int local_fault_tmp;
  bf_status_t rc;

  rc = lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);
  if (rc) {
    return rc;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  // read link fault interrupt register. This function will update the
  // cached values.
  port_mgr_mac_int_fault_get(
      dev_id, mac_block, ch, &local_fault_tmp, &remote_fault_tmp);

  // Return remote_fault & local_fault cached values
  if (remote_fault) {
    *remote_fault = port_p->lstate.remote_fault;
  }

  if (local_fault) {
    *local_fault = port_p->lstate.local_fault;
  }
  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_mac_int_fault_clr
 *
 * Clear the cached values of link interrupts and, if clear_all is non-zero,
 * also clear the interrupt register.
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_int_fault_clr(bf_dev_id_t dev_id,
                                       int mac_block,
                                       int ch,
                                       int clear_all) {
  int int_clr_reg_per_ch[] = {interrupts0__intclr3,
                              interrupts0__intclr4,
                              interrupts0__intclr5,
                              interrupts0__intclr6};
  port_mgr_dev_t *dev_p = NULL;
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  bf_status_t rc;

  rc = lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);
  if (rc) {
    return rc;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p) {
    return BF_INVALID_ARG;
  }

  dev_p = port_mgr_map_dev_id_to_dev_p(dev_id);
  if (dev_p == NULL) return BF_INVALID_ARG;

  port_p->lstate.remote_fault = 0;
  port_p->lstate.local_fault = 0;

  if (clear_all) {
    mac_write(dev_id, mac_block, int_clr_reg_per_ch[ch], 0x30);
  }

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_mac_live_link_state
 *
 * Return the current livelnkstat value for the specified mac-block/channel
 *
 ****************************************************************************/
int port_mgr_mac_live_link_state(bf_dev_id_t dev_id,
                                 int mac_block,
                                 int channel) {
  uint32_t data;
  int st = 0;

  data = mac_read(dev_id, mac_block, glbl__livelnkstat0);

  if (channel == 0) {
    st = get_fld_glbl__livelnkstat0__chlinkup0(&data);
  } else if (channel == 1) {
    st = get_fld_glbl__livelnkstat0__chlinkup1(&data);
  } else if (channel == 2) {
    st = get_fld_glbl__livelnkstat0__chlinkup2(&data);
  } else if (channel == 3) {
    st = get_fld_glbl__livelnkstat0__chlinkup3(&data);
  } else
    bf_sys_assert(0);
  return st;
}

/*****************************************************************************
 * port_mgr_mac_link_state
 *
 * Return the current livelnkstat value for the specified mac-block/channel
 *
 ****************************************************************************/
int port_mgr_mac_link_state(bf_dev_id_t dev_id,
                            int mac_block,
                            int channel,
                            bool oper_state) {
  uint32_t data;
  int local_fault = 0, remote_fault = 0;
  int st = 0;
  uint32_t mac_ctrl_reg =
      mac_read(dev_id, mac_block, mcmac0__ctrl + channel * 0x100);
  if (mac_ctrl_reg & 0x8) {  // MAC near lpbk is set
    return 0;
  }

  data = mac_read(dev_id, mac_block, glbl__livelnkstat0);

  if (channel == 0) {
    st = get_fld_glbl__livelnkstat0__chlinkup0(&data);
  } else if (channel == 1) {
    st = get_fld_glbl__livelnkstat0__chlinkup1(&data);
  } else if (channel == 2) {
    st = get_fld_glbl__livelnkstat0__chlinkup2(&data);
  } else if (channel == 3) {
    st = get_fld_glbl__livelnkstat0__chlinkup3(&data);
  } else
    bf_sys_assert(0);

  // Down to up. Clear any latched RF/LF and ignore LF/RF
  if (!oper_state && st) {
    // Once to clear stale one
    port_mgr_mac_int_fault_get(
        dev_id, mac_block, channel, &local_fault, &remote_fault);
    local_fault = remote_fault = 0;
    port_mgr_mac_int_fault_get(
        dev_id, mac_block, channel, &local_fault, &remote_fault);
    port_mgr_log("%d : MAC%d: CH%d: oper: %0d live-st: %0d LF: %0d : RF:%0d ",
                 dev_id,
                 mac_block,
                 channel,
                 oper_state,
                 st,
                 local_fault,
                 remote_fault);
  } else if (oper_state) {  // ignore live st
    // Previously link was declared up.
    // Get latched state RF/LF
    port_mgr_mac_int_fault_get(
        dev_id, mac_block, channel, &local_fault, &remote_fault);
    if (st && (local_fault || remote_fault)) {
      port_mgr_log(
          "%d : MAC%d: CH%d: oper: %0d live-st: %0d LF: %0d : RF:%0d pull link "
          "down",
          dev_id,
          mac_block,
          channel,
          oper_state,
          st,
          local_fault,
          remote_fault);
      st = 0;  // pull link-down
    } else if (!st) {
      // Up to down log
      port_mgr_log(
          "%d : MAC%d: CH%d: oper: %0d live-st: %0d LF: %0d : RF:%0d link down",
          dev_id,
          mac_block,
          channel,
          oper_state,
          st,
          local_fault,
          remote_fault);
    }
  }

  return st;
}

/*****************************************************************************
 * port_mgr_mac_link_state_v2
 *
 * Return the current livelnkstat value for the specified dev_id/dev_port
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_link_state_v2(bf_dev_id_t dev_id,
                                       int mac_block,
                                       int ch,
                                       int *state,
                                       int *local_fault,
                                       int *remote_fault) {
  uint32_t data;
  int st = 0;

  if (!state || !local_fault || !remote_fault) {
    return BF_INVALID_ARG;
  }

  *state = 0;
  *local_fault = 0;
  *remote_fault = 0;

  data = mac_read(dev_id, mac_block, glbl__livelnkstat0);

  if (ch == 0) {
    st = get_fld_glbl__livelnkstat0__chlinkup0(&data);
  } else if (ch == 1) {
    st = get_fld_glbl__livelnkstat0__chlinkup1(&data);
  } else if (ch == 2) {
    st = get_fld_glbl__livelnkstat0__chlinkup2(&data);
  } else if (ch == 3) {
    st = get_fld_glbl__livelnkstat0__chlinkup3(&data);
  } else
    return BF_INVALID_ARG;

  // get cached LF/RF cached values
  port_mgr_mac_int_fault_cache_get(
      dev_id, mac_block, ch, local_fault, remote_fault);

  *state = st;

  return BF_SUCCESS;
}

/*****************************************************************************
 * port_mgr_mac_basic_setup
 *
 * Called from the port_mgr_mac_set_speed_* functions (below) to configure all
 * settings that must be set with the umac in reset. These values are
 * saved in the port_mgr_port_t structure for later use (here) when the port
 * is being programmed.
 ****************************************************************************/
void port_mgr_mac_basic_setup(bf_dev_id_t dev_id, int mac_block, int ch) {
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  uint32_t data;
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err != LLD_OK) {
    port_mgr_log(
        "Error: "
        "port_mgr_mac_basic_setup: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        err,
        dev_id,
        mac_block,
        ch);
    return;
  }
  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) {
    bf_sys_assert(0);
    return;
  }

  // MAC Tx Configuration
  data = 0;

  // set default IFG (12)
  set_fld_mcmac0__txconfig__ifglength(&data, port_p->sw.ifg);

  // if link-pause enabled, enable pause frame generation
  set_fld_mcmac0__txconfig__fcfrmgen(&data, port_p->sw.link_pause_tx ? 1 : 0);

  // if PFC enabled, enable PFC pause frame generation
  set_fld_mcmac0__txconfig__pfcfrmgen(&data, port_p->sw.pfc_pause_tx ? 1 : 0);

  // enable auto-drain-on-fault
  set_fld_mcmac0__txconfig__enautodrnonflt(&data, 1);

  // FIXME once comira provides updated xml file
  // Also, only 10G and over configs are supported here currently
  data &= ~(7 << 11);
  data |= (port_p->sw.preamble_length == 8)
              ? (0 << 11)
              : (port_p->sw.preamble_length == 4) ? (4 << 11) : (0 << 11);

  mac_write(dev_id, mac_block, mcmac0__txconfig + ch * 0x100, data);

  // MAC Rx Configuration
  data = 0;
  set_fld_mcmac0__rxconfig__promiscuous(&data,
                                        port_p->sw.promiscuous_mode ? 1 : 0);

  // PAUSE/PFC frames should be filtered out always
  set_fld_mcmac0__rxconfig__filterpf(&data, 1);

  if (port_p->sw.link_pause_rx || port_p->sw.pfc_pause_rx) {
    set_fld_mcmac0__rxconfig__enrxfcdec(&data, 1);
  } else {
    set_fld_mcmac0__rxconfig__enrxfcdec(&data, 0);
  }
  // FIXME once comira provides updated xml file
  // Also, only 10G and over configs are supported here currently
  data |= (port_p->sw.preamble_length == 8)
              ? (0 << 7)
              : (port_p->sw.preamble_length == 4) ? (1 << 7) : (0 << 7);

  // dont overwrite stripfcs bit if in mac-far lpbk
  if (port_p->sw.lpbk_mode == BF_LPBK_MAC_FAR) {
    set_fld_mcmac0__rxconfig__stripfcs(&data, 1);
  }

  mac_write(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100, data);

  // MAC Max Frame Sz
  data = port_p->sw.rx_mtu;
  mac_write(dev_id, mac_block, mcmac0__maxfrmsize + ch * 0x100, data);

  data = port_p->sw.rx_max_jab_sz;
  mac_write(dev_id, mac_block, mcmac0__maxrxjabsize + ch * 0x100, data);

  data = port_p->sw.tx_mtu;
  mac_write(dev_id, mac_block, mcmac0__maxtxjabsize + ch * 0x100, data);

  // if PFC TX is enabled, set PFC vector for all priorities (per Comira)
  mac_write(dev_id,
            mac_block,
            mcmac0__txpfcvec + ch * 0x100,
            port_p->sw.pfc_pause_tx ? 0xff : 0);

  // set mac address
  set_fld_mcmac0__macaddrlo__macaddr(
      &data, (port_p->sw.mac_addr[1] << 8) | port_p->sw.mac_addr[0]);
  mac_write(dev_id, mac_block, mcmac0__macaddrlo + ch * 0x100, data);
  set_fld_mcmac0__macaddrmid__macaddr(
      &data, (port_p->sw.mac_addr[3] << 8) | port_p->sw.mac_addr[2]);
  mac_write(dev_id, mac_block, mcmac0__macaddrmid + ch * 0x100, data);
  set_fld_mcmac0__macaddrhi__macaddr(
      &data, (port_p->sw.mac_addr[5] << 8) | port_p->sw.mac_addr[4]);
  mac_write(dev_id, mac_block, mcmac0__macaddrhi + ch * 0x100, data);

  port_mgr_mac_set_fc_src_mac(dev_id, dev_port, port_p->sw.fc_src_mac_addr);
  port_mgr_mac_set_fc_dst_mac(dev_id, dev_port, port_p->sw.fc_dst_mac_addr);
  port_mgr_mac_set_xoff_pause_time(
      dev_id, dev_port, port_p->sw.xoff_pause_time);
  port_mgr_mac_set_xon_pause_time(dev_id, dev_port, port_p->sw.xon_pause_time);

  if (port_p->sw.fec == BF_FEC_TYP_FIRECODE) {
    port_mgr_mac_set_fc_fec_control(
        dev_id, dev_port, port_p->sw.fc_corr_en, port_p->sw.fc_ind_en);
  }
}

/*****************************************************************************
 * port_mgr_mac_hw_cfg_get
 *
 * Called during warm init to populate the port_p->hw structure based on
 * values read from hardware.
 ****************************************************************************/
void port_mgr_mac_hw_cfg_get(bf_dev_id_t dev_id, int mac_block, int ch) {
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  uint32_t data, speed, en, reset, mode_reg = 0, fec, fec_reg = 0;
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err != LLD_OK) {
    port_mgr_log(
        "Error: "
        "port_mgr_mac_hw_cfg_get: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        err,
        dev_id,
        mac_block,
        ch);
    return;
  }
  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);

  if (port_p == NULL) {
    port_mgr_log("Error: No port for dev_port=%x\n", dev_port);
    bf_sys_assert(0);
    return;
  }

  // FIXME : Have to read the ipg, premable[], loopback_enabled and the dfe type

  // Set some channel-specific reg addresses
  switch (ch) {
    case 0:
      mode_reg = glbl__ch0mode;
      fec_reg = hsmcpcs0__mode;
      break;
    case 1:
      mode_reg = glbl__ch1mode;
      fec_reg = hsmcpcs1__mode;
      break;
    case 2:
      mode_reg = glbl__ch2mode;
      fec_reg = hsmcpcs2__mode;
      break;
    case 3:
      mode_reg = glbl__ch3mode;
      fec_reg = hsmcpcs3__mode;
      break;
    default:
      bf_sys_assert(0);
  }
  // get speed, reset, and channel enable state
  data = mac_read(dev_id, mac_block, mode_reg);
  speed = get_fld_glbl__ch0mode__speed(&data);
  reset = get_fld_glbl__ch0mode__swreset(&data);
  en = get_fld_glbl__ch0mode__chena(&data);
  (void)en;
  // if en == 0 the channel is disabled meaning it has not been configured
  // as a port. In that case there's no need to collect the hw config from
  // the MAC as it will be completely reprogrammed when a port is added
  // on that channel.
  // if (en == 0) return;

  /* We cannot base our decision of if the port has been added or not on
     the
     value of en, since it ts kept disabled until the port bring up FSM is
     started. Hence a better basis to decide if a port has been added or not
     can be the speed for the speed since cannot be NONE */
  port_p->hw.assigned = true;

  // set port_p->hw.speed
  switch (speed) {
    case CMRA_CH_SPD_1G:
      port_p->hw.speed = BF_SPEED_1G;
      break;
    case CMRA_CH_SPD_10G:
      port_p->hw.speed = BF_SPEED_10G;
      break;
    case CMRA_CH_SPD_25G:
      port_p->hw.speed = BF_SPEED_25G;
      break;
    case CMRA_CH_SPD_40G:
      port_p->hw.speed = BF_SPEED_40G;
      break;
    case CMRA_CH_SPD_50G:
      port_p->hw.speed = BF_SPEED_50G;
      break;
    case CMRA_CH_SPD_100G:
      port_p->hw.speed = BF_SPEED_100G;
      break;
    default:
      port_p->hw.assigned = false;
      return;
  }

  // read fec mode
  data = mac_read(dev_id, mac_block, fec_reg);
  fec = get_fld_hsmcpcs0__mode__fec(&data);
  port_p->hw.fec =
      (fec == 0)
          ? BF_FEC_TYP_NONE
          : (fec == 1) ? BF_FEC_TYP_FIRECODE
                       : (fec == 2) ? BF_FEC_TYP_REED_SOLOMON : BF_FEC_TYP_NONE;

  // if reset=1 port has not been enabled yet
  /* we are basing our decision of wether the port is enabled or not
     on this bit. An important point to note is that this bit is set to 0
     (which indicates that the port is enabled) during the port bring up FSM
     in the config serdes stage. As a consequence, if the port is enabled but
     if the HA sequence is ran soon enough (before the FSM could set this bit
     to 0) then when we read the hardware we are going to think that the port
     was never enabled */
  if (reset) {
    port_p->hw.enabled = false;
  } else {
    port_p->hw.enabled = true;
  }

  // read the link state of the port
  port_p->hw.oper_state =
      port_mgr_mac_link_state(dev_id, mac_block, ch, 0) == 1 ? true : false;
  /* We have no way to repopulate the sw.oper_state since that is not
     explicitly replayed by the user during cfg replay. Hence just set
     it equal to the value read from hardware */
  port_p->sw.oper_state = port_p->hw.oper_state;

  // MAC Tx Configuration
  data = mac_read(dev_id, mac_block, mcmac0__txconfig + ch * 0x100);

  // set default IFG (12)
  port_p->hw.ifg = get_fld_mcmac0__txconfig__ifglength(&data);

  // if link-pause enabled, enable pause frame generation
  port_p->hw.link_pause_tx = get_fld_mcmac0__txconfig__fcfrmgen(&data);

  // if PFC enabled, enable PFC pause frame generation
  port_p->hw.pfc_pause_tx = get_fld_mcmac0__txconfig__pfcfrmgen(&data);

  // FIXME once comira provides updated xml file
  // Also, only 10G and over configs are supported here currently
  port_p->hw.preamble_length = (((data >> 11) & 7) == 4) ? 4 : 8;

  // data &= ~(7 << 11);
  // data |= (port_p->sw.preamble_length == 8)
  //            ? (0 << 11)
  //            : (port_p->sw.preamble_length == 4) ? (4 << 11) : (0 << 11);
  // mac_write(dev_id, mac_block, mcmac0__txconfig + ch * 0x100, data);

  // MAC Rx Configuration
  data = mac_read(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100);
  port_p->hw.promiscuous_mode = get_fld_mcmac0__rxconfig__promiscuous(&data);

  // FIXME: figure out how to distinguish PAUSE from PFC
  port_p->hw.link_pause_rx = get_fld_mcmac0__rxconfig__enrxfcdec(&data);
  port_p->hw.pfc_pause_rx = port_p->hw.link_pause_rx;

  // if (port_p->sw.link_pause_tx || port_p->sw.link_pause_rx ||
  // port_p->sw.pfc_pause_tx ||
  //    port_p->sw.pfc_pause_rx) {
  //  set_fld_mcmac0__rxconfig__enrxfcdec(&data, 1);
  //  set_fld_mcmac0__rxconfig__filterpf(&data, 1);
  //} else {
  //  set_fld_mcmac0__rxconfig__enrxfcdec(&data, 0);
  //  set_fld_mcmac0__rxconfig__filterpf(&data, 0);
  //}

  // hw.preamble length set above
  //
  // FIXME once comira provides updated xml file
  // Also, only 10G and over configs are supported here currently
  // data |= (port_p->sw.preamble_length == 8)
  //            ? (0 << 7)
  //            : (port_p->sw.preamble_length == 4) ? (1 << 7) : (0 << 7);

  // mac_write(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100, data);

  // MAC Max Frame Sz and Max Jabber size.
  // data = port_p->sw.rx_mtu;
  port_p->hw.rx_mtu =
      mac_read(dev_id, mac_block, mcmac0__maxfrmsize + ch * 0x100);

  port_p->hw.rx_max_jab_sz =
      mac_read(dev_id, mac_block, mcmac0__maxrxjabsize + ch * 0x100);

  port_p->hw.tx_mtu =
      mac_read(dev_id, mac_block, mcmac0__maxtxjabsize + ch * 0x100);

  port_p->hw.pfc_pause_tx =
      mac_read(dev_id, mac_block, mcmac0__txpfcvec + ch * 0x100);

  // set mac address
  // set_fld_mcmac0__macaddrlo__macaddr(
  //    &data, (port_p->sw.mac_addr[1] << 8) | port_p->sw.mac_addr[0]);
  // mac_write(dev_id, mac_block, mcmac0__macaddrlo + ch * 0x100, data);
  // set_fld_mcmac0__macaddrmid__macaddr(
  //    &data, (port_p->sw.mac_addr[3] << 8) | port_p->sw.mac_addr[2]);
  // mac_write(dev_id, mac_block, mcmac0__macaddrmid + ch * 0x100, data);
  // set_fld_mcmac0__macaddrhi__macaddr(
  //    &data, (port_p->sw.mac_addr[5] << 8) | port_p->sw.mac_addr[4]);
  // mac_write(dev_id, mac_block, mcmac0__macaddrhi + ch * 0x100, data);
  data = mac_read(dev_id, mac_block, mcmac0__macaddrlo + ch * 0x100);
  port_p->hw.mac_addr[0] = data & 0xff;
  port_p->hw.mac_addr[1] = (data >> 8) & 0xff;

  data = mac_read(dev_id, mac_block, mcmac0__macaddrmid + ch * 0x100);
  port_p->hw.mac_addr[2] = data & 0xff;
  port_p->hw.mac_addr[3] = (data >> 8) & 0xff;

  data = mac_read(dev_id, mac_block, mcmac0__macaddrhi + ch * 0x100);
  port_p->hw.mac_addr[4] = data & 0xff;
  port_p->hw.mac_addr[5] = (data >> 8) & 0xff;

  port_mgr_mac_get_fc_src_mac(dev_id, dev_port, port_p->hw.fc_src_mac_addr);
  port_mgr_mac_get_fc_dst_mac(dev_id, dev_port, port_p->hw.fc_dst_mac_addr);

  port_mgr_mac_get_xoff_pause_time(
      dev_id, dev_port, &port_p->hw.xoff_pause_time);
  port_mgr_mac_get_xon_pause_time(dev_id, dev_port, &port_p->hw.xon_pause_time);

  if (port_p->sw.fec == BF_FEC_TYP_FIRECODE) {
    port_mgr_mac_get_fc_fec_control(
        dev_id, dev_port, &port_p->hw.fc_corr_en, &port_p->hw.fc_ind_en);
  } else {
    /* Since we program these values in the hw only when the fec type is
       Firecode, read these only in the case when the fec type is Firecode.
       For other fec types, just set the shadow memory values to the
       default initialized values */
    port_p->hw.fc_corr_en = true;
    port_p->hw.fc_ind_en = true;
  }

  port_mgr_mac_get_txff_ctrl(dev_id,
                             mac_block,
                             &port_p->hw.txff_ctrl_pad_disable,
                             &port_p->hw.txff_ctrl_fcs_insert_disable,
                             &port_p->hw.txff_ctrl_crc_removal_disable,
                             &port_p->hw.txff_ctrl_crc_check_disable);

  port_mgr_mac_get_txff_trunc_ctrl(dev_id,
                                   mac_block,
                                   ch,
                                   &port_p->hw.txff_trunc_ctrl_size,
                                   &port_p->hw.txff_trunc_ctrl_en);
  port_mgr_mac_get_loopback_mode(dev_id,
                                 mac_block,
                                 ch,
                                 &port_p->hw.loopback_enabled,
                                 &port_p->hw.lpbk_mode);
}

/*****************************************************************************
 * umac interrupt register configuration
 *
 * Each umac interrupt register maps to one of the below lines. This data
 * structure is used to initialize the registers when the a port is being
 * initialized.
 ****************************************************************************/
typedef struct cmra_int_reg_setup_t {
  int ch_mask[4];
  int init_val;
} cmra_int_reg_setup_t;

cmra_int_reg_setup_t cmra_int_reg_setup[] = {
    /* first 16*2 registers are,
          inten,
          intdis
     */
    {{0x1111, 0x2222, 0x4444, 0x8888},
     0x0fff},  // setintenable0,  0x0000, disable Tx Idle
    {{0x1111, 0x2222, 0x4444, 0x8888}, 0xf000},  // clrintenable0,  0x0001
    {{0x0300, 0x0300, 0x0300, 0x0300}, 0x0300},  // setintenable1,  0x0002
    {{0x0300, 0x0300, 0x0300, 0x0300}, 0x0000},  // clrintenable1,  0x0003
    {{0x000F, 0x00F0, 0x0F00, 0xF000}, 0xffff},  // setintenable2,  0x0004
    {{0x000F, 0x00F0, 0x0F00, 0xF000}, 0x0000},  // clrintenable2,  0x0005
    {{0x007F, 0x0000, 0x0000, 0x0000}, 0x007f},  // setintenable3,  0x0006
    {{0x007F, 0x0000, 0x0000, 0x0000}, 0x0000},  // clrintenable3,  0x0007
    {{0x0000, 0x007F, 0x0000, 0x0000}, 0x007f},  // setintenable4,  0x0008
    {{0x0000, 0x007F, 0x0000, 0x0000}, 0x0000},  // clrintenable4,  0x0009
    {{0x0000, 0x0000, 0x007F, 0x0000}, 0x007f},  // setintenable5,  0x000a
    {{0x0000, 0x0000, 0x007F, 0x0000}, 0x0000},  // clrintenable5,  0x000b
    {{0x0000, 0x0000, 0x0000, 0x007F}, 0x007f},  // setintenable6,  0x000c
    {{0x0000, 0x0000, 0x0000, 0x007F}, 0x0000},  // clrintenable6,  0x000d
    {{0x7FFF, 0x0000, 0x0000, 0x0000}, 0x7fff},  // setintenable7,  0x000e
    {{0x7FFF, 0x0000, 0x0000, 0x0000}, 0x0000},  // clrintenable7,  0x000f
    {{0x0000, 0x7FFF, 0x0000, 0x0000}, 0x7fff},  // setintenable8,  0x0010
    {{0x0000, 0x7FFF, 0x0000, 0x0000}, 0x0000},  // clrintenable8,  0x0011
    {{0x0000, 0x0000, 0x7FFF, 0x0000}, 0x7fff},  // setintenable9,  0x0012
    {{0x0000, 0x0000, 0x7FFF, 0x0000}, 0x0000},  // clrintenable9,  0x0013
    {{0x0000, 0x0000, 0x0000, 0x7FFF}, 0x7fff},  // setintenable10, 0x0014
    {{0x0000, 0x0000, 0x0000, 0x7FFF}, 0x0000},  // clrintenable10, 0x0015
    {{0x0FFF, 0x0000, 0x0000, 0x0000}, 0x0fff},  // setintenable11, 0x0016
    {{0x0FFF, 0x0080, 0x0080, 0x0080},
     0x0080},  // clrintenable11, 0x0017, disable RSFEC #0 BER over threshold
    {{0x0000, 0x0FFF, 0x0000, 0x0000}, 0x0fff},  // setintenable12, 0x0018
    {{0x0000, 0x0FFF, 0x0000, 0x0000},
     0x0080},  // clrintenable12, 0x0019, disable RSFEC #1 BER over threshold
    {{0x0000, 0x0000, 0x0FFF, 0x0000}, 0x0fff},  // setintenable13, 0x001a
    {{0x0000, 0x0000, 0x0FFF, 0x0000},
     0x0080},  // clrintenable13, 0x001b, disable RSFEC #2 BER over threshold
    {{0x0000, 0x0000, 0x0000, 0x0FFF}, 0x0fff},  // setintenable14, 0x001c
    {{0x0000, 0x0000, 0x0000, 0x0FFF},
     0x0080},  // clrintenable14, 0x001d, disable RSFEC #3 BER over threshold
    {{0x0080, 0x0100, 0x0200, 0x0400}, 0x0780},  // setintenable15, 0x001e
    {{0x0080, 0x0100, 0x0200, 0x0400}, 0x0000},  // clrintenable15, 0x001f

    /* second 16*2 registers are,
          intstat,
          intclr
     */
    {{0x1111, 0x2222, 0x4444, 0x8888}, 0x0000},  // 0x0020
    {{0x1111, 0x2222, 0x4444, 0x8888}, 0xffff},  // 0x0021
    {{0x0300, 0x0300, 0x0300, 0x0300}, 0x0000},  // 0x0022
    {{0x0300, 0x0300, 0x0300, 0x0300}, 0x0300},  // 0x0023
    {{0x000F, 0x00F0, 0x0F00, 0xF000}, 0x0000},  // 0x0024
    {{0x000F, 0x00F0, 0x0F00, 0xF000}, 0xffff},  // 0x0025
    {{0x007F, 0x0000, 0x0000, 0x0000}, 0x0000},  // 0x0026
    {{0x007F, 0x0000, 0x0000, 0x0000}, 0x007f},  // 0x0027
    {{0x0000, 0x007F, 0x0000, 0x0000}, 0x0000},  // 0x0028
    {{0x0000, 0x007F, 0x0000, 0x0000}, 0x007f},  // 0x0029
    {{0x0000, 0x0000, 0x007F, 0x0000}, 0x0000},  // 0x002a
    {{0x0000, 0x0000, 0x007F, 0x0000}, 0x007f},  // 0x002b
    {{0x0000, 0x0000, 0x0000, 0x007F}, 0x0000},  // 0x002c
    {{0x0000, 0x0000, 0x0000, 0x007F}, 0x007f},  // 0x002d
    {{0x7FFF, 0x0000, 0x0000, 0x0000}, 0x0000},  // 0x002e
    {{0x7FFF, 0x0000, 0x0000, 0x0000}, 0x7fff},  // 0x002f
    {{0x0000, 0x7FFF, 0x0000, 0x0000}, 0x0000},  // 0x0030
    {{0x0000, 0x7FFF, 0x0000, 0x0000}, 0x7fff},  // 0x0031
    {{0x0000, 0x0000, 0x7FFF, 0x0000}, 0x0000},  // 0x0032
    {{0x0000, 0x0000, 0x7FFF, 0x0000}, 0x7fff},  // 0x0033
    {{0x0000, 0x0000, 0x0000, 0x7FFF}, 0x0000},  // 0x0034
    {{0x0000, 0x0000, 0x0000, 0x7FFF}, 0x7fff},  // 0x0035
    {{0x0FFF, 0x0000, 0x0000, 0x0000}, 0x0000},  // 0x0036
    {{0x0FFF, 0x0000, 0x0000, 0x0000}, 0x0fff},  // 0x0037
    {{0x0000, 0x0FFF, 0x0000, 0x0000}, 0x0000},  // 0x0038
    {{0x0000, 0x0FFF, 0x0000, 0x0000}, 0x0fff},  // 0x0039
    {{0x0000, 0x0000, 0x0FFF, 0x0000}, 0x0000},  // 0x003a
    {{0x0000, 0x0000, 0x0FFF, 0x0000}, 0x0fff},  // 0x003b
    {{0x0000, 0x0000, 0x0000, 0x0FFF}, 0x0000},  // 0x003c
    {{0x0000, 0x0000, 0x0000, 0x0FFF}, 0x0fff},  // 0x003d
    {{0x0080, 0x0100, 0x0200, 0x0400}, 0x0000},  // 0x003e
    {{0x0080, 0x0100, 0x0200, 0x0400}, 0x0780},  // 0x003f
};

/*****************************************************************************
 * port_mgr_mac_basic_interrupt_setup
 *
 * Enable/disable umac interrupts based on the table above.
 * The interrupt block of the umac starts at offset 0 of the umac address
 * space, so the offset matches the ordinal position of the register in the
 * table above.
 * By iterating thru the table top to bottom we are first enabling or
 * disabling interrupt bits (as specified above), then clearing any existing
 * interrupt bits.
 *
 * Note: intstat registers contain a reset value of "0x0000" above and 0
 *       values are skipped, since they would have no effect.
 ****************************************************************************/
void port_mgr_mac_basic_interrupt_setup(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port) {
  int i, mac_blk, ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_blk, 1)) {
    port_mgr_log("MAC: %d:%2d:%d :  Interrupt init", dev_id, mac_blk, ch);
  }
  for (i = 0;
       i < (int)(sizeof(cmra_int_reg_setup) / sizeof(cmra_int_reg_setup[0]));
       i++) {
    // writing 0 has no effect so we can skip those
    if (cmra_int_reg_setup[i].init_val & cmra_int_reg_setup[i].ch_mask[ch]) {
      mac_write(
          dev_id,
          mac_blk,
          i,
          cmra_int_reg_setup[i].init_val & cmra_int_reg_setup[i].ch_mask[ch]);
    }
  }
}

/*****************************************************************************
 * port_mgr_mac_basic_interrupt_teardown
 *
 * Disable and clear all interrupts for the given port.
 *
 * By iterating thru the table top to bottom and incrementing by 2 we are
 * first disabling each interrupt and then clearing any leftover interrupts
 ****************************************************************************/
void port_mgr_mac_basic_interrupt_teardown(bf_dev_id_t dev_id,
                                           bf_dev_port_t port) {
  int i, mac_blk, ch;

  port_mgr_map_dev_port_to_all(dev_id, port, NULL, NULL, &mac_blk, &ch, NULL);

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_blk, 1)) {
    port_mgr_log("MAC: %d:%2d:%d :  Interrupt de-init", dev_id, mac_blk, ch);
  }
  // first disable, then clear, all ints from this channel
  // "setinten"=n, "clrinten"=n+1, so start at 1 below.
  // for (i = 1; i < 0x40; i+=2) {
  for (i = 0;
       i < (int)(sizeof(cmra_int_reg_setup) / sizeof(cmra_int_reg_setup[0]));
       i += 2) {
    mac_write(dev_id, mac_blk, i + 1, cmra_int_reg_setup[i].ch_mask[ch]);
  }
}

/*****************************************************************************
 * port_mgr_mac_disable_mac_ints
 *
 * Disable and clear all MAC interrupts.
 *
 * This is done when switching to interrupt-based link-status change mode.
 * Since all Comira interrupts get mapped thru a single bit in the eth_regs
 * for a quad (up to 4 ports) we disable all interrupts then re-enable ONLY
 * LF and RF bits.
 ****************************************************************************/
void port_mgr_mac_disable_mac_ints(bf_dev_id_t dev_id) {
  uint32_t comira_int_clr_regs[] = {interrupts0__intclr0,
                                    interrupts0__intclr1,
                                    interrupts0__intclr2,
                                    interrupts0__intclr3,
                                    interrupts0__intclr4,
                                    interrupts0__intclr5,
                                    interrupts0__intclr6,
                                    interrupts0__intclr7,
                                    interrupts0__intclr8,
                                    interrupts0__intclr9,
                                    interrupts0__intclr10,
                                    interrupts0__intclr11,
                                    interrupts0__intclr12,
                                    interrupts0__intclr13,
                                    interrupts0__intclr14,
                                    interrupts0__intclr15};
  uint32_t comira_int_dis_regs[] = {interrupts0__clrintenable0,
                                    interrupts0__clrintenable1,
                                    interrupts0__clrintenable2,
                                    interrupts0__clrintenable3,
                                    interrupts0__clrintenable4,
                                    interrupts0__clrintenable5,
                                    interrupts0__clrintenable6,
                                    interrupts0__clrintenable7,
                                    interrupts0__clrintenable8,
                                    interrupts0__clrintenable9,
                                    interrupts0__clrintenable10,
                                    interrupts0__clrintenable11,
                                    interrupts0__clrintenable12,
                                    interrupts0__clrintenable13,
                                    interrupts0__clrintenable14,
                                    interrupts0__clrintenable15};
  uint32_t msk = 0xffffffff;
  uint32_t ofs;
  int mac_block;

  for (mac_block = 0; mac_block < lld_get_max_mac_blocks(dev_id); mac_block++) {
    for (ofs = 0;
         ofs < sizeof(comira_int_clr_regs) / sizeof(comira_int_clr_regs[0]);
         ofs++) {
      mac_write(dev_id, mac_block, comira_int_dis_regs[ofs], msk);
      mac_write(dev_id, mac_block, comira_int_clr_regs[ofs], msk);
    }
  }
}

// indexed by dev_id, mac_block, channel, int-reg, bit, <total=0, shown=1>
uint32_t mac_interrupt_count[BF_MAX_DEV_COUNT][65][4][16][16][2] = {
    {{{{{0}}}}}};

/********************************************************************
 * port_mgr_mac_interrupt_dump
 *
 ********************************************************************/
void port_mgr_mac_interrupt_dump(mac_int_dump_cb fn, bf_dev_id_t input_dev) {
  bf_dev_id_t dev_id;
  int mac_block, ch, bit, int_reg;

  for (dev_id = 0; dev_id < BF_MAX_DEV_COUNT; dev_id++) {
    if ((input_dev != -1) && (input_dev != dev_id)) {
      continue;
    }
    for (mac_block = 0; mac_block < lld_get_max_mac_blocks(dev_id);
         mac_block++) {
      for (ch = 0; ch < 4; ch++) {
        for (int_reg = 0; int_reg < 16; int_reg++) {
          for (bit = 0; bit < 16; bit++) {
            if (mac_interrupt_count[dev_id][mac_block][ch][int_reg][bit][0] !=
                0) {
              uint32_t mac_addr =
                  offsetof(Tofino, macs_t) + (mac_block * MAC_STRIDE);
              uint32_t cmra_reg = mac_addr +
                                  offsetof(Mac_addrmap, comira_regs) +
                                  (((2 * int_reg) + 0x20) * 4);

              fn(dev_id,
                 mac_block,
                 ch,
                 cmra_reg,
                 bit,
                 mac_interrupt_count[dev_id][mac_block][ch][int_reg][bit]
                                    [0],  // total
                 mac_interrupt_count[dev_id][mac_block][ch][int_reg][bit]
                                    [1]);  // shown

              // copy to "shown"
              mac_interrupt_count[dev_id][mac_block][ch][int_reg][bit][1] =
                  mac_interrupt_count[dev_id][mac_block][ch][int_reg][bit][0];
            }
          }
        }
      }
    }
  }
}

/********************************************************************
 * port_mgr_mac_interrupt_poll
 *
 * Called from process_mbus_ints in lld_interrupt.c
 *
 * Poll Comira UMAC registers for interrupts. Issue user callbacks
 * if any registers/bits he registered for are discovered.
 ********************************************************************/
void port_mgr_mac_interrupt_poll(bf_dev_id_t dev_id, int mac_block, int ch) {
  int i;
  uint32_t data;
  bf_dev_port_t dev_port;
  port_mgr_port_t *port_p;
  port_mgr_mac_block_t *mac_block_p =
      port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err != LLD_OK) {
    port_mgr_log(
        "Error: "
        "port_mgr_mac_interrupt_poll: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        err,
        dev_id,
        mac_block,
        ch);
    return;
  }

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) return;  // assoc port is disabled or not added
  if (mac_block_p == NULL) return;

  // int status registers start at 0x20
  for (i = interrupts0__intstat0;
       i < (int)(sizeof(cmra_int_reg_setup) / sizeof(cmra_int_reg_setup[0]));
       i += 2) {
    uint32_t ch_ints;

    // status regs are RO and have a "0" for init_val
    // and make sure there are some relevant bits
    if ((cmra_int_reg_setup[i].init_val == 0) &&
        (cmra_int_reg_setup[i].ch_mask[ch] != 0)) {
      data = mac_read(dev_id, mac_block, i);
      ch_ints = data & cmra_int_reg_setup[i].ch_mask[ch];
      if (ch_ints) {  // some bit(s) set
        int bit;
        uint32_t uninteresting_bits;

        // count first
        for (bit = 0; bit < 16; bit++) {
          if (ch_ints & (1 << bit)) {
            mac_interrupt_count[dev_id][mac_block][ch][((i - 0x20) / 2)][bit]
                               [0]++;
          }
        }
        // see if user requested notification of any of these
        if ((port_p->mac_int_cb != NULL) &&
            (mac_block_p->comira_int_regs[i] & data)) {
          lld_err_t lld_err = lld_sku_map_mac_ch_to_dev_port_id(
              dev_id, mac_block, ch, &dev_port);
          if (lld_err == LLD_OK) {
            // issue user interrupt callback
            if (port_mgr_log_worthy(
                    LOG_TYP_PORT, dev_id, mac_block * 4 + ch, 1)) {
              port_mgr_log("MAC: INT: %d:%2d:%d Int reg=%04x : bits=%04x",
                           dev_id,
                           mac_block,
                           ch,
                           i,
                           (mac_block_p->comira_int_regs[i] & data));
            }
            port_p->mac_int_cb(dev_id,
                               dev_port,
                               i,
                               (mac_block_p->comira_int_regs[i] & data),
                               port_p->mac_int_userdata);
          }
        }
        uninteresting_bits = mac_block_p->comira_int_regs[i] ^ data;
        if (uninteresting_bits) {
          // regs are defined as "status"=n, "clear"=n+1
          mac_write(dev_id, mac_block, i + 1, uninteresting_bits);
#if 0
          port_mgr_log(
              "MAC: INT: %d:%2d:%d Clear un-interesting bits too: reg=%x : "
              "bits=%x",
              dev_id,
              mac_block,
              ch,
              i,
              uninteresting_bits);
#endif
        }
      }
    }
  }
}

/********************************************************************
 * port_mgr_mac_bind_interrupt_callback
 *
 * Bind an interrupt callback to a Tofino port. The passed callback
 * will be issued for any interrupt the caller is interested
 * in (via port_mgr_mac_interrupt_registration). The callback will be
 * issued from within port_mgr_mac_interrupt_poll().
 *
 * The opaque "userdata" will be returned in the callback.
 *
 * The callback may be "un-bound" using this API with NULL for the
 * "fn" parameter.
 ********************************************************************/
int port_mgr_mac_bind_interrupt_callback(bf_dev_id_t dev_id,
                                         int mac_block,
                                         int ch,
                                         bf_port_int_callback_t fn,
                                         void *userdata) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err != LLD_OK) {
    port_mgr_log(
        "port_mgr_mac_bind_interrupt_callback: mac-block=%d : ch=%d : invalid "
        "for sku",
        mac_block,
        ch);
    return -2;
  }
  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);

  if (port_p != NULL) {
    port_p->mac_int_cb = fn;
    port_p->mac_int_userdata = userdata;
    return 0;
  }
  bf_sys_assert(0);  // hear about it

  return -1;  // bad port or comira int register offset
}

/********************************************************************
 * port_mgr_mac_interrupt_registration
 *
 * Register for notification (via callback) of the assertion of any
 * of the itnerrupt bits defined by "reg" and "mask".
 *
 * Each bf_dev_port_t contains an array of structures mirroring the
 * umac interrupt status registers. For each register a mask of bits
 * that are of interest to the user is maintained. When the
 * corresponding register contains any asserted interrupt bits that
 * are also contained in the mask then the callback bound to that
 * port is executed.
 *
 * NOTE:
 *
 * The user is responsible for clearing all asserted interrupt bits
 * within "mask" after registering for an interrupt.
 *
 ********************************************************************/
int port_mgr_mac_interrupt_registration(
    bf_dev_id_t dev_id, int mac_block, int ch, uint32_t reg, uint32_t mask) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  port_mgr_mac_block_t *mac_block_p =
      port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err != LLD_OK) {
    port_mgr_log(
        "Error: "
        "port_mgr_mac_interrupt_registration: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        err,
        dev_id,
        mac_block,
        ch);
    return -2;
  }
  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);

  if ((port_p != NULL) && (mac_block_p != NULL)) {
    if (reg < sizeof(mac_block_p->comira_int_regs) /
                  sizeof(mac_block_p->comira_int_regs[0])) {
      mac_block_p->comira_int_regs[reg] |= mask;
      return 0;
    } else {
      bf_sys_assert(0);  // hear about it
    }
  }
  bf_sys_assert(0);  // hear about it

  return -1;  // bad port or comira int register offset
}

/********************************************************************
 * port_mgr_mac_interrupt_unregistration
 *
 * Clear "bits of interest" in the interrupt mask for the given
 * umac interrupt register (see description above).
 ********************************************************************/
int port_mgr_mac_interrupt_unregistration(
    bf_dev_id_t dev_id, int mac_block, int ch, uint32_t reg, uint32_t mask) {
  port_mgr_port_t *port_p;
  bf_dev_port_t dev_port;
  port_mgr_mac_block_t *mac_block_p =
      port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  if (mac_block_p == NULL) {
    port_mgr_log("Error: invalid device id %d", dev_id);
    bf_sys_assert(0);  // hear about it
    return -2;
  }
  lld_err_t err =
      lld_sku_map_mac_ch_to_dev_port_id(dev_id, mac_block, ch, &dev_port);

  if (err != LLD_OK) {
    port_mgr_log(
        "Error: "
        "port_mgr_mac_interrupt_unregistration: err: %x : from "
        "lld_sku_map_mac_ch_to_dev_port_id: %d:%d:%d\n",
        err,
        dev_id,
        mac_block,
        ch);
    return -2;
  }
  port_p = port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);

  if (port_p != NULL) {
    if (reg < sizeof(mac_block_p->comira_int_regs) /
                  sizeof(mac_block_p->comira_int_regs[0])) {
      mac_block_p->comira_int_regs[reg] &= ~mask;
      return 0;
    } else {
      bf_sys_assert(0);  // hear about it
    }
  }
  bf_sys_assert(0);  // hear about it

  return -1;  // bad port or comira int register offset
}

/********************************************************************
 * port_mgr_mac_reset_txff_ctrl_chnl_enable
 *
 ********************************************************************/
bf_status_t port_mgr_mac_reset_txff_ctrl_chnl_enable(bf_dev_id_t dev_id,
                                                     int mac_block) {
  uint32_t txff_ctrl, addr;
  port_mgr_mac_block_t *mac_block_p =
      port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  if (!mac_block_p) return BF_INVALID_ARG;

  /* Add a trace log */
  port_mgr_log_trace("%s: MAC: txff_ctrl: %d:%2d:- : %04x",
                     __func__,
                     dev_id,
                     mac_block,
                     mac_block_p->txff_ctrl);

  /* First get the cached value for txff_ctrl */
  txff_ctrl = mac_block_p->txff_ctrl;
  /* Disable channel enable bit for all channel in the MAC & write to HW */
  setp_eth_regs_txff_ctrl_chnl_ena(&txff_ctrl, 0);
  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.txff_ctrl);
  lld_write_register(dev_id, addr, txff_ctrl);

  /*
   * Now write the cached value so that added ports' MAC channel will get
   * enabled to reset credit to EBUF (through PGR)
   */
  lld_write_register(dev_id, addr, mac_block_p->txff_ctrl);

  return BF_SUCCESS;
}

/********************************************************************
 * port_mgr_mac_pgm_rxsigok_ctrl
 *
 ********************************************************************/
void port_mgr_mac_pgm_rxsigok_ctrl(bf_dev_id_t dev_id, int mac_block) {
  uint32_t data, addr;
  int ch;

  // just do all channels each time since they are always initd the same
  for (ch = 0; ch < 4; ch++) {
    addr = offsetof(Tofino,
                    macs_t[mac_block].macs.eth_regs.ethsds_rxsigok_ctrl[ch]);
    lld_read_register(dev_id, addr, &data);
    setp_eth_regs_ethsds_rxsigok_ctrl_sigok_debounce_count(&data, 0x400);
    lld_write_register(dev_id, addr, data);
  }
}

/********************************************************************
 * port_mgr_mac_rxsigok_ctrl_get
 *
 ********************************************************************/
int port_mgr_mac_rxsigok_ctrl_get(bf_dev_id_t dev_id, int mac_block, int ch) {
  uint32_t data, addr;
  int connected_ln = 0;

  // just do all channels each time since they are always initd the same
  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.eth_status);
  lld_read_register(dev_id, addr, &data);
  /* gives bit pos in 4b field */
  // get_fld_serdesmux__laneremaprx0__remap0(&data);
  data = mac_read(dev_id, mac_block, serdesmux__laneremaprx0);
  switch (ch) {
    case 0:
      connected_ln = get_fld_serdesmux__laneremaprx0__remap0(&data);
      break;
    case 1:
      connected_ln = get_fld_serdesmux__laneremaprx0__remap1(&data);
      break;
    case 2:
      connected_ln = get_fld_serdesmux__laneremaprx0__remap2(&data);
      break;
    case 3:
      connected_ln = get_fld_serdesmux__laneremaprx0__remap3(&data);
      break;
    default:
      break;
  }
  return ((data >> connected_ln) & 1);
}

/********************************************************************
 * port_mgr_mac_rxsigok_ctrl_unmapped_get
 *
 ********************************************************************/
int port_mgr_mac_rxsigok_ctrl_unmapped_get(bf_dev_id_t dev_id,
                                           int mac_block,
                                           int ch) {
  uint32_t data, addr;

  // just do all channels each time since they are always initd the same
  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.eth_status);
  lld_read_register(dev_id, addr, &data);
  return ((data >> ch) & 1);
}

/********************************************************************
 * port_mgr_mac_pgm_txff_ctrl
 *
 ********************************************************************/
void port_mgr_mac_pgm_txff_ctrl(bf_dev_id_t dev_id, int mac_block) {
  uint32_t addr, chnl_seq, slot2ch_map = 0, data;
  port_mgr_mac_block_t *mac_block_p =
      port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);

  if (mac_block_p == NULL) {
    port_mgr_log("MAC: invalid device id %d", dev_id);
    return;
  }

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 1)) {
    port_mgr_log(
        "MAC: TXF: %d:%2d:- : %04x", dev_id, mac_block, mac_block_p->txff_ctrl);
  }
  /* txff_ctrl value is set in lld_dvm_if.c on add/chg port config */
  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.txff_ctrl);
  lld_write_register(dev_id, addr, mac_block_p->txff_ctrl);

  /* the chnl_seq from txff_ctrl maps directly to the slot2chnl mapping */
  chnl_seq = getp_eth_regs_txff_ctrl_chnl_seq(&mac_block_p->txff_ctrl);

  set_fld_fifoctrl0__chmap0__slot0chmap(&slot2ch_map,
                                        (chnl_seq >> (2 * 0)) & 0x3);
  set_fld_fifoctrl0__chmap0__slot1chmap(&slot2ch_map,
                                        (chnl_seq >> (2 * 1)) & 0x3);
  set_fld_fifoctrl0__chmap0__slot2chmap(&slot2ch_map,
                                        (chnl_seq >> (2 * 2)) & 0x3);
  set_fld_fifoctrl0__chmap0__slot3chmap(&slot2ch_map,
                                        (chnl_seq >> (2 * 3)) & 0x3);
  mac_write(dev_id, mac_block, fifoctrl0__chmap0, slot2ch_map);
  mac_write(dev_id, mac_block, fifoctrl0__chmap1, slot2ch_map);

  data = mac_read(dev_id, mac_block, fifoctrl0__ctrl1);
  set_fld_fifoctrl0__ctrl1__s2chmapena(&data, 1);
  mac_write(dev_id, mac_block, fifoctrl0__ctrl1, data);
}

/********************************************************************
 * port_mgr_mac_get_txff_ctrl
 *
 ********************************************************************/
void port_mgr_mac_get_txff_ctrl(bf_dev_id_t dev_id,
                                int mac_block,
                                bool *txff_ctrl_pad_disable,
                                bool *txff_ctrl_fcs_insert_disable,
                                bool *txff_ctrl_crc_removal_disable,
                                bool *txff_ctrl_crc_check_disable) {
  uint32_t addr, data;

  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.txff_ctrl);
  lld_read_register(dev_id, addr, &data);

  *txff_ctrl_pad_disable =
      getp_eth_regs_txff_ctrl_txdispad(&data) ? true : false;
  *txff_ctrl_fcs_insert_disable =
      getp_eth_regs_txff_ctrl_txdisfcs(&data) ? true : false;
  *txff_ctrl_crc_removal_disable =
      getp_eth_regs_txff_ctrl_crcrmv_dis(&data) ? true : false;
  *txff_ctrl_crc_check_disable =
      getp_eth_regs_txff_ctrl_crcchk_dis(&data) ? true : false;
}

/********************************************************************
 * port_mgr_mac_get_loopback_mode
 *
 ********************************************************************/
void port_mgr_mac_get_loopback_mode(bf_dev_id_t dev_id,
                                    int mac_block,
                                    int ch,
                                    bool *loopback_enable,
                                    bf_loopback_mode_e *mode) {
  uint32_t mcmac_ctrl_reg[4] = {
      mcmac0__ctrl, mcmac1__ctrl, mcmac2__ctrl, mcmac3__ctrl};
  uint32_t near_mac_lpbk;
  uint32_t data;

  if (!loopback_enable || !mode) {
    return;
  }

  *loopback_enable = false;
  *mode = BF_LPBK_NONE;

  /* Note: only MAC NEAR loopback supported by the time being */
  data = mac_read(dev_id, mac_block, mcmac_ctrl_reg[ch]);
  near_mac_lpbk = get_fld_mcmac0__ctrl__maclpbk(&data);
  if (near_mac_lpbk) {
    *loopback_enable = true;
    *mode = BF_LPBK_MAC_NEAR;
  }
}

/********************************************************************
 * port_mgr_mac_pgm_force_pfc_flush
 *
 ********************************************************************/
void port_mgr_mac_pgm_force_pfc_flush(bf_dev_id_t dev_id,
                                      int mac_block,
                                      int ch) {
  uint32_t addr;
  port_mgr_mac_block_t *mac_block_p =
      port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  if (mac_block_p == NULL) {
    port_mgr_log("Error: No mac for mac_block=%x\n", mac_block);
    bf_sys_assert(0);
    return;
  }
  uint32_t ch_in_use = mac_block_p->ch_in_use & (1 << ch);

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 1)) {
    port_mgr_log("MAC: FSH: %d:%2d:%d : %s",
                 dev_id,
                 mac_block,
                 ch,
                 ch_in_use ? "clr" : "set");
  }
  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.force_pfc_flush[ch]);
  /* if the channel is in use then this is being called by port_mgr_port_add
   * and we need to clear any previously set flush bits.
   * If the channel is not in use then this is being called by
   * port_mgr_port_remove
   * and we need to set all the flush bits */
  lld_write_register(dev_id, addr, ch_in_use ? 0 : 0x3ff);
}

/********************************************************************
 * port_mgr_mac_pgm_txff_trunc_ctrl
 *
 ********************************************************************/
void port_mgr_mac_pgm_txff_trunc_ctrl(
    bf_dev_id_t dev_id, int mac_block, int ch, int trunc_sz, int trunc_en) {
  uint32_t addr, data;

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 1)) {
    port_mgr_log("MAC:    : %d:%2d:%d : trunc_ctrl : sz=%d : en=%d",
                 dev_id,
                 mac_block,
                 ch,
                 trunc_sz,
                 trunc_en);
  }
  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.trunc_ctrl[ch]);

  lld_read_register(dev_id, addr, &data);

  setp_eth_regs_trunc_ctrl_trunc_ena(&data, trunc_en);
  setp_eth_regs_trunc_ctrl_trunc_size(&data, trunc_sz);

  lld_write_register(dev_id, addr, data);
}

/********************************************************************
 * port_mgr_mac_get_txff_trunc_ctrl
 *
 ********************************************************************/
void port_mgr_mac_get_txff_trunc_ctrl(bf_dev_id_t dev_id,
                                      int mac_block,
                                      int ch,
                                      uint32_t *trunc_sz,
                                      bool *trunc_en) {
  uint32_t addr, data;

  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.trunc_ctrl[ch]);

  lld_read_register(dev_id, addr, &data);

  *trunc_en = getp_eth_regs_trunc_ctrl_trunc_ena(&data) ? true : false;
  *trunc_sz = getp_eth_regs_trunc_ctrl_trunc_size(&data);
}

/********************************************************************
 * port_mgr_mac_enable_ch
 *
 ********************************************************************/
void port_mgr_mac_enable_ch(bf_dev_id_t dev_id, int mac_block, int ch) {
  uint32_t data;
  uint32_t ch_mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};

  if (1 || port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: ENA: %d:%2d:%d : Enable channel", dev_id, mac_block, ch);
  }

  data = mac_read(dev_id, mac_block, ch_mode_reg[ch]);

  {
    uint32_t ena_st = get_fld_glbl__ch0mode__chena(&data);
    if (ena_st == 0) {
      port_mgr_log(
          "MAC: ENA: %d:%2d:%d : ch0mode.ena = 0", dev_id, mac_block, ch);
      uint32_t swr_st = get_fld_glbl__ch0mode__swreset(&data);
      if (swr_st == 0) {
        set_fld_glbl__ch0mode__swreset(&data, 1);
        mac_write(dev_id, mac_block, ch_mode_reg[ch], data);
        bf_sys_usleep(100);
      }
      set_fld_glbl__ch0mode__chena(&data, 1);
      mac_write(dev_id, mac_block, ch_mode_reg[ch], data);
      bf_sys_usleep(100);
    }
  }

  // IMPORTANT:
  // The swreset and chena bits CANNOT be modified at the same
  // time. Differences in internal propagation delays can cause
  // unpredictable behavior due to the reset arriving before or after
  // the enable and mode settings have been made.
  // So, we will always leave chena asserted (1) and will disable
  // the channel with swreset.
  //
  set_fld_glbl__ch0mode__swreset(&data, 0);
  set_fld_glbl__ch0mode__chena(&data, 1);
  mac_write(dev_id, mac_block, ch_mode_reg[ch], data);
}

/********************************************************************
 * port_mgr_mac_disable_ch
 *
 ********************************************************************/
void port_mgr_mac_disable_ch(bf_dev_id_t dev_id, int mac_block, int ch) {
  uint32_t data;
  uint32_t ch_mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};

  if (1 || port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log(
        "MAC: DIS: %d:%2d:%d : Disable channel", dev_id, mac_block, ch);
  }

  data = mac_read(dev_id, mac_block, ch_mode_reg[ch]);

  {
    uint32_t ena_st = get_fld_glbl__ch0mode__chena(&data);
    if (ena_st == 0) {
      port_mgr_log("MAC: DIS: %d:%2d:%d : ERROR! : ch0mode.ena = 0!",
                   dev_id,
                   mac_block,
                   ch);
    }
  }
  // IMPORTANT:
  // The swreset and chena bits CANNOT be modified at the same
  // time. Differences in internal propagation delays can cause
  // unpredictable behavior due to the reset arriving before or after
  // the enable and mode settings have been made.
  // So, we will always leave chena asserted (1) and will disable
  // the channel with swreset.
  //
  set_fld_glbl__ch0mode__swreset(&data, 1);
  set_fld_glbl__ch0mode__chena(&data, 1);
  mac_write(dev_id, mac_block, ch_mode_reg[ch], data);
}

/********************************************************************
 * port_mgr_mac_delete_ch
 *
 * Must set speed to "0" or a prior 100G channel will mess up all
 * other channels in the quad.
 ********************************************************************/
void port_mgr_mac_delete_ch(bf_dev_id_t dev_id, int mac_block, int ch) {
  uint32_t data;
  uint32_t ch_mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log(
        "MAC: DIS: %d:%2d:%d : Disable channel", dev_id, mac_block, ch);
  }

  data = mac_read(dev_id, mac_block, ch_mode_reg[ch]);

  set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_NONE);
  mac_write(dev_id, mac_block, ch_mode_reg[ch], data);

  // hack until we get new_xml
  // 4011 is MLG cfg, disable mlg
  if ((ch == 0) || (ch == 2)) {
    mac_write(dev_id, mac_block, hsmcpcs0__mode + 1, 0);
  }
}

/********************************************************************
 * port_mgr_mac_rs_fec_reset_set
 *
 ********************************************************************/
void port_mgr_mac_rs_fec_reset_set(bf_dev_id_t dev_id,
                                   int mac_block,
                                   int ch,
                                   bool assert_reset) {
  uint32_t data;
  uint32_t fecrs_ch_reg[4] = {
      fecrs0__dbgctrl, fecrs1__dbgctrl, fecrs2__dbgctrl, fecrs3__dbgctrl};

  data = mac_read(dev_id, mac_block, fecrs_ch_reg[ch]);

  set_fld_fecrs0__dbgctrl__softreset(&data, assert_reset ? 1 : 0);
  mac_write(dev_id, mac_block, fecrs_ch_reg[ch], data);
}

/********************************************************************
 * port_mgr_mac_rs_fec_scrambler_en_set
 *
 ********************************************************************/
void port_mgr_mac_rs_fec_scrambler_en_set(bf_dev_id_t dev_id,
                                          int mac_block,
                                          int ch,
                                          bool en) {
  uint32_t data;
  uint32_t fecrs_ch_reg[4] = {
      fecrs0__dbgctrl, fecrs1__dbgctrl, fecrs2__dbgctrl, fecrs3__dbgctrl};

  data = mac_read(dev_id, mac_block, fecrs_ch_reg[ch]);

  data = (data & ~(1 << 8)) | ((en ? 1 : 0) << 8);
  mac_write(dev_id, mac_block, fecrs_ch_reg[ch], data);
}

/********************************************************************
 * port_mgr_mac_rs_fec_25g_am_fix
 *
 ********************************************************************/
void port_mgr_mac_rs_fec_25g_am_fix(bf_dev_id_t dev_id,
                                    int mac_block,
                                    int ch,
                                    bool en) {
  uint32_t data;
  uint32_t amctrl_reg = hsmcpcs0__mode - 0x10 + 0x50;
  uint32_t am0_low_reg = hsmcpcs0__mode - 0x10 + 0x5C;

  /* AM0 alignment markers format is as described in IEEE 802.3 Table 82-2
   * (0xC1, 0x68, 0x21, BIP3, 0x3E, 0x97, 0xDE, BIP7),
   *
   * and AM1, 2 and 3 are as described in IEEE 802.3 Table 82-3
   * (AM1 = 0xF0, 0xC4, 0xE6, BIP3, 0x0F, 0x3B, 0x19, BIP7),
   * (AM2 = 0xC5, 0x65, 0x9B, BIP3, 0x3A, 0x9A, 0x64, BIP7) and
   * (AM3 = 0xA2, 0x79, 0x3D, BIP3, 0x5D, 0x86, 0xC2, BIP7)
   */

  // AM0
  mac_write(dev_id, mac_block, am0_low_reg + 0, 0x68c1);
  mac_write(dev_id, mac_block, am0_low_reg + 1, 0x3321);

  // AM1
  mac_write(dev_id, mac_block, am0_low_reg + 2, 0xc4f0);
  mac_write(dev_id, mac_block, am0_low_reg + 3, 0x33e6);

  // AM2
  mac_write(dev_id, mac_block, am0_low_reg + 4, 0x65c5);
  mac_write(dev_id, mac_block, am0_low_reg + 5, 0x339b);

  // AM3
  mac_write(dev_id, mac_block, am0_low_reg + 6, 0x79a2);
  mac_write(dev_id, mac_block, am0_low_reg + 7, 0x333d);

  data = mac_read(dev_id, mac_block, amctrl_reg + (ch * 0x100));
  data &= ~(1 << 0);
  if (en) {
    data |= (1 << 0);
  }
  mac_write(dev_id, mac_block, amctrl_reg + (ch * 0x100), data);
}

/* port_mgr_mac_set_speed_100g
 *
 */
void port_mgr_mac_set_speed_100g(bf_dev_id_t dev_id,
                                 int mac_block,
                                 int ch,
                                 int pma,
                                 int fec,
                                 int loopback_en) {
  uint32_t data;

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: PGM: %d:%2d:%d : Pgm: spd=100g : fec=%d : lpbk=%d",
                 dev_id,
                 mac_block,
                 ch,
                 fec,
                 loopback_en);
  }

  // sanity checks for 100g mode
  // (keeps signatures of set_speed_xxxg consistent)
  bf_sys_assert(ch == 0);
  bf_sys_assert(pma == CMRA_PMA_BASE_R4);
  bf_sys_assert((fec == FEC_TYP_NONE) || (fec == FEC_TYP_REED_SOLOMON));

  /* # Activate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f10, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch0mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch0mode, data);

  /* # Activate Ch SW reset for CH1 [Global.ch1mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f11, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch1mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch1mode, data);

  /* # Activate Ch SW reset for CH2 [Global.ch2mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f12, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch2mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch2mode, data);

  /* # Activate Ch SW reset for CH3 [Global.ch3mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f13, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch3mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch3mode, data);

  // hack until we get new_xml
  // 4011 is MLG cfg, disable mlg
  mac_write(dev_id, mac_block, hsmcpcs0__mode + 1, 0);

  /* # Configure Ch speed/enable for CH0[Global.ch0mode]
   * # Ch Speed = 1001 [100G]
   * # Ch SW reset = 1
   * # Ch enable = 1
   * apb_write(addr=0x1f10, data=0x0027)
   */
  data = 0;
  set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_100G);
  set_fld_glbl__ch0mode__swreset(&data, 1);
  set_fld_glbl__ch0mode__chena(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch0mode, data);

  /* # Configure MAC CH0 Ctrl[MAC0.ctrl]
   * # Transmit Enable = 1
   * # Receive  Enable = 1
   * apb_write(addr=0x2100, data=0x0003)
   */
  data = 0;
  set_fld_mcmac0__ctrl__txenable(&data, 1);
  set_fld_mcmac0__ctrl__rxenable(&data, 1);
  mac_write(dev_id, mac_block, mcmac0__ctrl, data);

  // Configure according to port_mgr_port_t settings
  port_mgr_mac_basic_setup(dev_id, mac_block, ch);

  /* # Configure PCS CH0 Mode[PCS0.mode]
   * # PMA = 0101 [BASE-R4]
   * # fec = 00 [None]
   *         01 [FCFEC]
   *         10 [RSFEC]
   * apb_write(addr=0x4010, data=0x0014)
   */
  data = 0;
  set_fld_hsmcpcs0__mode__pma(&data, pma);
  set_fld_hsmcpcs0__mode__fec(&data, fec);
  mac_write(dev_id, mac_block, hsmcpcs0__mode, data);

  /* # Configure PCS CH0 settings[PCS0.dbgctrl1]
   * # dbguseshorttimer = 1 [Use non-standard AM frequency to
   * # speed up link bringup / not intended for normal use]
   * apb_write(addr=0x40FF, data=0x1e05)
   */
  data = 0;
  set_fld_hsmcpcs0__dbgctrl1__ena(&data, 1);

  if (USE_SHORT_TIMER) {
    set_fld_hsmcpcs0__dbgctrl1__dbguseshorttimer(&data, 1);
  }

  set_fld_hsmcpcs0__dbgctrl1__dbgfullthreshold(&data, 8);
  set_fld_hsmcpcs0__dbgctrl1__decodetrapsel(&data, 7);
  mac_write(dev_id, mac_block, hsmcpcs0__dbgctrl1, data);

  // Configure RSFEC CH0 settings[RSFEC0.dbgctrl]
  if (fec == 0) {  // disable both RSFEC and FCFEC
    data = 0;
    set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
    mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);

    data = 0;
    set_fld_fecfc0__cfg__enareq(&data, 0);
    mac_write(dev_id, mac_block, fecfc0__cfg, data);
  } else if (fec == 1) {  // fcfec
    // disable any rsfec
    data = 0;
    set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
    mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);

    /* # Configure FCFEC CH0 settings[FCFEC0.cfg]
     * # fc enable request = 1
     * apb_write(addr=0x48ab, data=0x0007)
     */
    data = 0;
    set_fld_fecfc0__cfg__enareq(&data, 1);
    set_fld_fecfc0__cfg__enapcserr(&data, 1);
    set_fld_fecfc0__cfg__enaerrcorr(&data, 1);
    mac_write(dev_id, mac_block, fecfc0__cfg, data);
  } else if (fec == 2) {  // rsfec
    // disable any fcfec
    data = 0;
    set_fld_fecfc0__cfg__enareq(&data, 0);
    mac_write(dev_id, mac_block, fecfc0__cfg, data);

    data = 0;
    set_fld_fecrs0__dbgctrl__disablefec(&data, 0);

    if (USE_SHORT_TIMER) {
      set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
    }

    // hack til we get new xml file
    data |= (1 << 8);  // enable_pcs_scrambler
    mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);
  }

  /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
   * # Port Map0 = 000
   * # PortMap 1 = 100
   * # PortMap 2 = 100
   * # PortMap 3 = 100
   * # PortMap0 enable = 1
   * # PortMap1 enable = 0
   * # PortMap2 enable = 0
   * # PortMap3 enable = 0
   * apb_write(addr=0x3f05, data=0x1920)
   */
  data = 0;
  set_fld_fifoctrl0__appfifoportmap0__portmap0(&data, 0);
  set_fld_fifoctrl0__appfifoportmap0__portmap1(&data, 4);
  set_fld_fifoctrl0__appfifoportmap0__portmap2(&data, 4);
  set_fld_fifoctrl0__appfifoportmap0__portmap3(&data, 4);

  set_fld_fifoctrl0__appfifoportmap0__portmap0ena(&data, 1);
  set_fld_fifoctrl0__appfifoportmap0__portmap1ena(&data, 0);
  set_fld_fifoctrl0__appfifoportmap0__portmap2ena(&data, 0);
  set_fld_fifoctrl0__appfifoportmap0__portmap3ena(&data, 0);
  mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

  /* # If SerDes TX->RX internal loopback is desired, activate loopback
   * [SerDes_mux.serdeslpbk].
   * # SerDes CH0 loopback enable = 1
   * apb_write(addr=0x5f01, data=0x0001)
   */
  if (loopback_en) {
    data = 0;
    set_fld_serdesmux__serdeslpbk__lpbken0(&data, 1);
    mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
  }

  /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 0
   * apb_write(addr=0x1f10, data=0x0025)
   */
  data = 0;
  set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_100G);
  set_fld_glbl__ch0mode__swreset(&data, 1);  // leave ch disabled (swreset=1)
  set_fld_glbl__ch0mode__chena(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch0mode, data);
}

/* port_mgr_mac_set_speed_50g
 *
 */
void port_mgr_mac_set_speed_50g(bf_dev_id_t dev_id,
                                int mac_block,
                                int ch,
                                int pma,
                                int fec,
                                int loopback_en) {
  uint32_t data;

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: PGM: %d:%2d:%d : Pgm: spd=50g : fec=%d : lpbk=%d",
                 dev_id,
                 mac_block,
                 ch,
                 fec,
                 loopback_en);
  }

  // sanity checks for 50g mode
  // (keeps signatures of set_speed_xxxg consistent)
  bf_sys_assert((ch == 0) || (ch == 2));
  bf_sys_assert(pma == CMRA_PMA_BASE_R2);

  if (ch == 0) {
    /* # Activate Ch SW reset for CH0 [Global.ch0mode]
     * # Ch SW reset = 1
     * apb_write(addr=0x1f10, data=0x0002)
     */
    data = 0;
    set_fld_glbl__ch0mode__swreset(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch0mode, data);

    /* # Activate Ch SW reset for CH1 [Global.ch1mode]
     * # Ch SW reset = 1
     * apb_write(addr=0x1f11, data=0x0002)
     */
    data = 0;
    set_fld_glbl__ch1mode__swreset(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch1mode, data);

    // hack until we get new_xml
    // 4011 is MLG cfg, disable mlg
    mac_write(dev_id, mac_block, hsmcpcs0__mode + 1, 0);

    /* # Configure Ch speed/enable for CH0[Global.ch0mode]
     * # Ch Speed = 1000 [50G]
     * # Ch SW reset = 1
     * # Ch enable = 1
     * apb_write(addr=0x1f10, data=0x0027)
     */
    data = 0;
    set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_50G);
    set_fld_glbl__ch0mode__swreset(&data, 1);
    set_fld_glbl__ch0mode__chena(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch0mode, data);

    /* # Configure MAC CH0 Ctrl[MAC0.ctrl]
     * # Transmit Enable = 1
     * # Receive  Enable = 1
     * apb_write(addr=0x2100, data=0x0003)
     */
    data = 0;
    set_fld_mcmac0__ctrl__txenable(&data, 1);
    set_fld_mcmac0__ctrl__rxenable(&data, 1);
    mac_write(dev_id, mac_block, mcmac0__ctrl, data);

    /* # Configure MAC CH0 RX[MAC0.rxconfig]
     * # Strip FCS = 1
     * # Promiscuous Mode = 1
     * apb_write(addr=0x2102, data=0x0012)
     */
    data = 0;
    // not set by rtl
    // set_fld_mcmac0__rxconfig__stripfcs( &data, 1);
    // set_fld_mcmac0__rxconfig__promiscuous( &data, 1);
    // mac_write( dev_id,  mac_block, mcmac0__rxconfig, data );
    port_mgr_mac_basic_setup(dev_id, mac_block, ch);

    /* # Configure PCS CH0 Mode[PCS0.mode]
     * # PMA = 0100 [BASE-R2]
     * # fec = 00 [None]
     *         01 [FCFEC]
     *         10 [RSFEC]
     * apb_write(addr=0x4010, data=0x0014)
     */
    data = 0;
    set_fld_hsmcpcs0__mode__pma(&data, pma);
    set_fld_hsmcpcs0__mode__fec(&data, fec);
    mac_write(dev_id, mac_block, hsmcpcs0__mode, data);

    /* # Configure PCS CH0 settings[PCS0.dbgctrl1]
     * # dbguseshorttimer = 1 [Use non-standard AM frequency to
     * # speed up link bringup / not intended for normal use]
     * apb_write(addr=0x40FF, data=0x1e05)
     */
    data = 0;
    set_fld_hsmcpcs0__dbgctrl1__ena(&data, 1);

    if (USE_SHORT_TIMER) {
      set_fld_hsmcpcs0__dbgctrl1__dbguseshorttimer(&data, 1);
    }

    set_fld_hsmcpcs0__dbgctrl1__dbgfullthreshold(&data, 8);
    set_fld_hsmcpcs0__dbgctrl1__decodetrapsel(&data, 7);
    mac_write(dev_id, mac_block, hsmcpcs0__dbgctrl1, data);

    // Configure RSFEC CH0 settings[RSFEC0.dbgctrl]
    if (fec == 0) {  // disable both RSFEC and FCFEC
      data = 0;
      set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
      mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);

      data = 0;
      set_fld_fecfc0__cfg__enareq(&data, 0);
      mac_write(dev_id, mac_block, fecfc0__cfg, data);
    } else if (fec == 1) {  // fcfec
      // disable any rsfec
      data = 0;
      set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
      mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);

      /* # Configure FCFEC CH0 settings[FCFEC0.cfg]
       * # fc enable request = 1
       * apb_write(addr=0x48ab, data=0x0007)
       */
      data = 0;
      set_fld_fecfc0__cfg__enareq(&data, 1);
      set_fld_fecfc0__cfg__enapcserr(&data, 1);
      set_fld_fecfc0__cfg__enaerrcorr(&data, 1);
      mac_write(dev_id, mac_block, fecfc0__cfg, data);
    } else if (fec == 2) {  // rsfec
      // disable any fcfec
      data = 0;
      set_fld_fecfc0__cfg__enareq(&data, 0);
      mac_write(dev_id, mac_block, fecfc0__cfg, data);

      data = 0;
      set_fld_fecrs0__dbgctrl__disablefec(&data, 0);

      if (USE_SHORT_TIMER) {
        set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
      }

      // hack til we get new xml file
      data |= (1 << 8);  // enable_pcs_scrambler
      mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);
    }

    /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
     * # Port Map0 = 000
     * # PortMap 1 = 100
     * # PortMap 2 = 100
     * # PortMap 3 = 100
     * # PortMap0 enable = 1
     * # PortMap1 enable = 0
     * # PortMap2 enable = 0
     * # PortMap3 enable = 0
     * apb_write(addr=0x3f05, data=0x1920)
     */
    data = mac_read(dev_id, mac_block, fifoctrl0__appfifoportmap0);
    set_fld_fifoctrl0__appfifoportmap0__portmap0(&data, 0);
    set_fld_fifoctrl0__appfifoportmap0__portmap1(&data, 4);
    // set_fld_fifoctrl0__appfifoportmap0__portmap2( &data, 4 );
    // set_fld_fifoctrl0__appfifoportmap0__portmap3( &data, 4 );

    set_fld_fifoctrl0__appfifoportmap0__portmap0ena(&data, 1);
    set_fld_fifoctrl0__appfifoportmap0__portmap1ena(&data, 0);
    // set_fld_fifoctrl0__appfifoportmap0__portmap2ena( &data, 0 );
    // set_fld_fifoctrl0__appfifoportmap0__portmap3ena( &data, 0 );
    mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

    /* # If SerDes TX->RX internal loopback is desired, activate loopback
     * [SerDes_mux.serdeslpbk].
     * # SerDes CH0 loopback enable = 1
     * apb_write(addr=0x5f01, data=0x0001)
     */
    if (loopback_en) {
      data = 0;
      set_fld_serdesmux__serdeslpbk__lpbken0(&data, 1);
      mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
    }

    /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
     * # Ch SW reset = 0
     * apb_write(addr=0x1f10, data=0x0025)
     */
    data = 0;
    set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_50G);
    set_fld_glbl__ch0mode__swreset(&data, 1);  // lv chn disabled
    set_fld_glbl__ch0mode__chena(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch0mode, data);
  } else {  // ch 2
            /* # Activate Ch SW reset for CH2 [Global.ch2mode]
             * # Ch SW reset = 1
             * apb_write(addr=0x1f12, data=0x0002)
             */
    data = 0;
    set_fld_glbl__ch2mode__swreset(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch2mode, data);

    /* # Activate Ch SW reset for CH3 [Global.ch3mode]
     * # Ch SW reset = 1
     * apb_write(addr=0x1f13, data=0x0002)
     */
    data = 0;
    set_fld_glbl__ch3mode__swreset(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch3mode, data);

    // hack until we get new_xml
    // 4011 is MLG cfg, disable mlg
    data = mac_read(dev_id, mac_block, hsmcpcs0__mode + 1);  // 4011=MLG cfg
    if (data != 0) {
      mac_write(dev_id, mac_block, hsmcpcs0__mode + 1, 0);
    }
    /* # Configure Ch speed/enable for CH2[Global.ch2mode]
     * # Ch Speed = 1001 [100G]
     * # Ch SW reset = 1
     * # Ch enable = 1
     * apb_write(addr=0x1f10, data=0x0027)
     */
    data = 0;
    set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_50G);
    set_fld_glbl__ch2mode__swreset(&data, 1);
    set_fld_glbl__ch2mode__chena(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch2mode, data);

    /* # Configure MAC CH2 Ctrl[MAC2.ctrl]
     * # Transmit Enable = 1
     * # Receive  Enable = 1
     * apb_write(addr=0x2100, data=0x0003)
     */
    data = 0;
    set_fld_mcmac2__ctrl__txenable(&data, 1);
    set_fld_mcmac2__ctrl__rxenable(&data, 1);
    mac_write(dev_id, mac_block, mcmac2__ctrl, data);

    /* # Configure MAC CH2 RX[MAC2.rxconfig]
     * # Strip FCS = 1
     * # Promiscuous Mode = 1
     * apb_write(addr=0x2102, data=0x0012)
     */
    data = 0;
    // not set by rtl
    // set_fld_mcmac2__rxconfig__stripfcs( &data, 1);
    // set_fld_mcmac2__rxconfig__promiscuous(&data, 1);
    // mac_write(dev_id, mac_block, mcmac2__rxconfig, data);
    port_mgr_mac_basic_setup(dev_id, mac_block, ch);

    /* # Configure PCS CH2 Mode[PCS2.mode]
     * # PMA = 0100 [BASE-R2]
     * # fec = 00 [None]
     *         01 [FCFEC]
     *         10 [RSFEC]
     * apb_write(addr=0x4010, data=0x0014)
     */
    data = 0;
    set_fld_hsmcpcs2__mode__pma(&data, pma);
    set_fld_hsmcpcs2__mode__fec(&data, fec);
    mac_write(dev_id, mac_block, hsmcpcs2__mode, data);

    /* # Configure PCS CH2 settings[PCS2.dbgctrl1]
     * # dbguseshorttimer = 1 [Use non-standard AM frequency to
     * # speed up link bringup / not intended for normal use]
     * apb_write(addr=0x40FF, data=0x1e05)
     */
    data = 0;
    set_fld_hsmcpcs0__dbgctrl1__ena(&data, 1);

    if (USE_SHORT_TIMER) {
      set_fld_hsmcpcs0__dbgctrl1__dbguseshorttimer(&data, 1);
    }

    set_fld_hsmcpcs0__dbgctrl1__dbgfullthreshold(&data, 8);
    set_fld_hsmcpcs0__dbgctrl1__decodetrapsel(&data, 7);
    mac_write(dev_id, mac_block, hsmcpcs2__dbgctrl1, data);

    // Configure RSFEC CH2 settings[RSFEC2.dbgctrl]
    if (fec == 0) {  // disable both RSFEC and FCFEC
      data = 0;
      set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
      mac_write(dev_id, mac_block, fecfc2__cfg, data);

      data = 0;
      set_fld_fecfc2__cfg__enareq(&data, 0);
      mac_write(dev_id, mac_block, fecfc2__cfg, data);
    } else if (fec == 1) {  // fcfec
      // disable any rsfec
      data = 0;
      set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
      mac_write(dev_id, mac_block, fecfc2__cfg, data);

      /* # Configure FCFEC CH2 settings[FCFEC2.cfg]
       * # fc enable request = 1
       * apb_write(addr=0x48ab, data=0x0007)
       */
      data = 0;
      set_fld_fecfc2__cfg__enareq(&data, 1);
      set_fld_fecfc2__cfg__enapcserr(&data, 1);
      set_fld_fecfc2__cfg__enaerrcorr(&data, 1);
      mac_write(dev_id, mac_block, fecfc2__cfg, data);
    } else if (fec == 2) {  // rsfec
      // disable any fcfec
      data = 0;
      set_fld_fecfc2__cfg__enareq(&data, 0);
      mac_write(dev_id, mac_block, fecfc2__cfg, data);

      data = 0;
      set_fld_fecrs2__dbgctrl__disablefec(&data, 0);

      if (USE_SHORT_TIMER) {
        set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
      }

      // hack til we get new xml file
      data |= (1 << 8);  // enable_pcs_scrambler
      mac_write(dev_id, mac_block, fecrs2__dbgctrl, data);
    }

    /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
     * # Port Map0 = 100
     * # PortMap 1 = 100
     * # PortMap 2 = 010
     * # PortMap 3 = 100
     * # PortMap0 enable = 0
     * # PortMap1 enable = 0
     * # PortMap2 enable = 1
     * # PortMap3 enable = 0
     * apb_write(addr=0x3f05, data=0x1920)
     */
    data = mac_read(dev_id, mac_block, fifoctrl0__appfifoportmap0);
    // set_fld_fifoctrl0__appfifoportmap0__portmap0( &data, 0 );
    // set_fld_fifoctrl0__appfifoportmap0__portmap1( &data, 4 );
    set_fld_fifoctrl0__appfifoportmap0__portmap2(&data, 2);
    set_fld_fifoctrl0__appfifoportmap0__portmap3(&data, 4);

    // set_fld_fifoctrl0__appfifoportmap0__portmap0ena( &data, 1 );
    // set_fld_fifoctrl0__appfifoportmap0__portmap1ena( &data, 0 );
    set_fld_fifoctrl0__appfifoportmap0__portmap2ena(&data, 1);
    set_fld_fifoctrl0__appfifoportmap0__portmap3ena(&data, 0);
    mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

    /* # If SerDes TX->RX internal loopback is desired, activate loopback
     * [SerDes_mux.serdeslpbk].
     * # SerDes CH0 loopback enable = 1
     * apb_write(addr=0x5f01, data=0x0001)
     */
    if (loopback_en) {
      data = 0;
      set_fld_serdesmux__serdeslpbk__lpbken2(&data, 1);
      mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
    }

    /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
     * # Ch SW reset = 0
     * apb_write(addr=0x1f10, data=0x0025)
     */
    data = 0;
    set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_50G);
    set_fld_glbl__ch2mode__swreset(&data, 1);  // lv chn disabled
    set_fld_glbl__ch2mode__chena(&data, 1);
    mac_write(dev_id, mac_block, glbl__ch2mode, data);
  }
}

/* port_mgr_mac_set_speed_40g
 *
 */
void port_mgr_mac_set_speed_40g(bf_dev_id_t dev_id,
                                int mac_block,
                                int ch,
                                int pma,
                                int fec,
                                int loopback_en) {
  uint32_t data;

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: PGM: %d:%2d:%d : Pgm: spd=40g : fec=%d : lpbk=%d",
                 dev_id,
                 mac_block,
                 ch,
                 fec,
                 loopback_en);
  }

  // sanity checks for 100g mode
  // (keeps signatures of set_speed_xxxg consistent)
  bf_sys_assert(ch == 0);
  bf_sys_assert(pma == CMRA_PMA_BASE_R4);
  bf_sys_assert((fec == FEC_TYP_NONE) || (fec == FEC_TYP_FIRECODE));

  /* # Activate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f10, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch0mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch0mode, data);

  /* # Activate Ch SW reset for CH1 [Global.ch1mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f11, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch1mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch1mode, data);

  /* # Activate Ch SW reset for CH2 [Global.ch2mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f12, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch2mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch2mode, data);

  /* # Activate Ch SW reset for CH3 [Global.ch3mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f13, data=0x0002)
   */
  data = 0;
  set_fld_glbl__ch3mode__swreset(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch3mode, data);

  // hack until we get new_xml
  // 4011 is MLG cfg, disable mlg
  mac_write(dev_id, mac_block, hsmcpcs0__mode + 1, 0);

  /* # Configure Ch speed/enable for CH0[Global.ch0mode]
   * # Ch Speed = 0111 [40G]
   * # Ch SW reset = 1
   * # Ch enable = 1
   * apb_write(addr=0x1f10, data=0x0027)
   */
  data = 0;
  set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_40G);
  set_fld_glbl__ch0mode__swreset(&data, 1);
  set_fld_glbl__ch0mode__chena(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch0mode, data);

  /* # Configure MAC CH0 Ctrl[MAC0.ctrl]
   * # Transmit Enable = 1
   * # Receive  Enable = 1
   * apb_write(addr=0x2100, data=0x0003)
   */
  data = 0;
  set_fld_mcmac0__ctrl__txenable(&data, 1);
  set_fld_mcmac0__ctrl__rxenable(&data, 1);
  mac_write(dev_id, mac_block, mcmac0__ctrl, data);

  /* # Configure MAC CH0 RX[MAC0.rxconfig]
   * # Strip FCS = 1
   * # Promiscuous Mode = 1
   * apb_write(addr=0x2102, data=0x0012)
   */
  data = 0;
  // not set by rtl
  // set_fld_mcmac0__rxconfig__stripfcs( &data, 1);
  // set_fld_mcmac0__rxconfig__promiscuous( &data, 1);
  // mac_write( dev_id,  mac_block, mcmac0__rxconfig, data );
  port_mgr_mac_basic_setup(dev_id, mac_block, ch);

  /* # Configure PCS CH0 Mode[PCS0.mode]
   * # PMA = 0101 [BASE-R4]
   * # fec = 00 [None]
   *         01 [FCFEC]
   *         10 [RSFEC]
   * apb_write(addr=0x4010, data=0x0014)
   */
  data = 0;
  set_fld_hsmcpcs0__mode__pma(&data, pma);
  set_fld_hsmcpcs0__mode__fec(&data, fec);
  mac_write(dev_id, mac_block, hsmcpcs0__mode, data);

  /* # Configure PCS CH0 settings[PCS0.dbgctrl1]
   * # dbguseshorttimer = 1 [Use non-standard AM frequency to
   * # speed up link bringup / not intended for normal use]
   * apb_write(addr=0x40FF, data=0x1e05)
   */
  data = 0;
  set_fld_hsmcpcs0__dbgctrl1__ena(&data, 1);

  if (USE_SHORT_TIMER) {
    set_fld_hsmcpcs0__dbgctrl1__dbguseshorttimer(&data, 1);
  }

  set_fld_hsmcpcs0__dbgctrl1__dbgfullthreshold(&data, 8);
  set_fld_hsmcpcs0__dbgctrl1__decodetrapsel(&data, 7);
  mac_write(dev_id, mac_block, hsmcpcs0__dbgctrl1, data);

  // Configure RSFEC CH0 settings[RSFEC0.dbgctrl]
  if (fec == 0) {  // disable both RSFEC and FCFEC
    data = 0;
    set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
    mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);

    data = 0;
    set_fld_fecfc0__cfg__enareq(&data, 0);
    mac_write(dev_id, mac_block, fecfc0__cfg, data);
  } else if (fec == 1) {  // fcfec
    // disable any rsfec
    data = 0;
    set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
    mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);

    /* # Configure FCFEC CH0 settings[FCFEC0.cfg]
     * # fc enable request = 1
     * apb_write(addr=0x48ab, data=0x0007)
     */
    data = 0;
    set_fld_fecfc0__cfg__enareq(&data, 1);
    set_fld_fecfc0__cfg__enapcserr(&data, 1);
    set_fld_fecfc0__cfg__enaerrcorr(&data, 1);
    mac_write(dev_id, mac_block, fecfc0__cfg, data);
  } else if (fec == 2) {  // rsfec
    // disable any fcfec
    data = 0;
    set_fld_fecfc0__cfg__enareq(&data, 0);
    mac_write(dev_id, mac_block, fecfc0__cfg, data);

    data = 0;
    set_fld_fecrs0__dbgctrl__disablefec(&data, 0);

    if (USE_SHORT_TIMER) {
      set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
    }
    // hack til we get new xml file
    data |= (1 << 8);  // enable_pcs_scrambler
    mac_write(dev_id, mac_block, fecrs0__dbgctrl, data);
  }

  /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
   * # Port Map0 = 000
   * # PortMap 1 = 100
   * # PortMap 2 = 100
   * # PortMap 3 = 100
   * # PortMap0 enable = 1
   * # PortMap1 enable = 0
   * # PortMap2 enable = 0
   * # PortMap3 enable = 0
   * apb_write(addr=0x3f05, data=0x1920)
   */
  data = 0;
  set_fld_fifoctrl0__appfifoportmap0__portmap0(&data, 0);
  set_fld_fifoctrl0__appfifoportmap0__portmap1(&data, 4);
  set_fld_fifoctrl0__appfifoportmap0__portmap2(&data, 4);
  set_fld_fifoctrl0__appfifoportmap0__portmap3(&data, 4);

  set_fld_fifoctrl0__appfifoportmap0__portmap0ena(&data, 1);
  set_fld_fifoctrl0__appfifoportmap0__portmap1ena(&data, 0);
  set_fld_fifoctrl0__appfifoportmap0__portmap2ena(&data, 0);
  set_fld_fifoctrl0__appfifoportmap0__portmap3ena(&data, 0);
  mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

  /* # If SerDes TX->RX internal loopback is desired, activate loopback
   * [SerDes_mux.serdeslpbk].
   * # SerDes CH0 loopback enable = 1
   * apb_write(addr=0x5f01, data=0x0001)
   */
  if (loopback_en) {
    data = 0;
    set_fld_serdesmux__serdeslpbk__lpbken0(&data, 1);
    mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
  }

  /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 0
   * apb_write(addr=0x1f10, data=0x0025)
   */
  data = 0;
  set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_40G);
  set_fld_glbl__ch0mode__swreset(&data, 1);
  set_fld_glbl__ch0mode__chena(&data, 1);
  mac_write(dev_id, mac_block, glbl__ch0mode, data);
}

/* port_mgr_mac_set_speed_25g
 *
 */
void port_mgr_mac_set_speed_25g(bf_dev_id_t dev_id,
                                int mac_block,
                                int ch,
                                int pma,
                                int fec,
                                int loopback_en) {
  uint32_t data;
  uint32_t mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};
  uint32_t mcmac_ctrl_reg[4] = {
      mcmac0__ctrl, mcmac1__ctrl, mcmac2__ctrl, mcmac3__ctrl};
#if 0
    uint32_t mcmac_rxconfig_reg[4] = { mcmac0__rxconfig,
                                       mcmac1__rxconfig, 
                                       mcmac2__rxconfig, 
                                       mcmac3__rxconfig };
#endif
  uint32_t hsmcpcs_mode_reg[4] = {
      hsmcpcs0__mode, hsmcpcs1__mode, hsmcpcs2__mode, hsmcpcs3__mode};
  uint32_t fecrs_cfg_reg[4] = {
      fecrs0__dbgctrl, fecrs1__dbgctrl, fecrs2__dbgctrl, fecrs3__dbgctrl};
  uint32_t fecfc_cfg_reg[4] = {
      fecfc0__cfg, fecfc1__cfg, fecfc2__cfg, fecfc3__cfg};

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: PGM: %d:%2d:%d : Pgm: spd=25g : fec=%d : lpbk=%d",
                 dev_id,
                 mac_block,
                 ch,
                 fec,
                 loopback_en);
  }

  // sanity checks for 25g mode
  // (keeps signatures of set_speed_xxxg consistent)
  bf_sys_assert(pma == CMRA_PMA_BASE_R1);

  /* # Activate Ch SW reset for "ch" [Global.ch0-3mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f10-3, data=0x0002)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__swreset(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__swreset(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__swreset(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__swreset(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);

  // hack until we get new_xml
  // 4011 is MLG cfg, disable mlg
  mac_write(dev_id, mac_block, hsmcpcs_mode_reg[ch] + 1, 0);

  /* # Configure Ch speed/enable for CH0[Global.ch0mode]
   * # Ch Speed = 0110 [25G]
   * # Ch SW reset = 1
   * # Ch enable = 1
   * apb_write(addr=0x1f10, data=0x0027)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch0mode__swreset(&data, 1);
      set_fld_glbl__ch0mode__chena(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch1mode__swreset(&data, 1);
      set_fld_glbl__ch1mode__chena(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch2mode__swreset(&data, 1);
      set_fld_glbl__ch2mode__chena(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch3mode__swreset(&data, 1);
      set_fld_glbl__ch3mode__chena(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);

  /* # Configure MAC CH0 Ctrl[MAC0.ctrl]
   * # Transmit Enable = 1
   * # Receive  Enable = 1
   * apb_write(addr=0x2100, data=0x0003)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_mcmac0__ctrl__txenable(&data, 1);
      set_fld_mcmac0__ctrl__rxenable(&data, 1);
      break;
    case 1:
      set_fld_mcmac1__ctrl__txenable(&data, 1);
      set_fld_mcmac1__ctrl__rxenable(&data, 1);
      break;
    case 2:
      set_fld_mcmac2__ctrl__txenable(&data, 1);
      set_fld_mcmac2__ctrl__rxenable(&data, 1);
      break;
    case 3:
      set_fld_mcmac3__ctrl__txenable(&data, 1);
      set_fld_mcmac3__ctrl__rxenable(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mcmac_ctrl_reg[ch], data);

#if 0
    /* # Configure MAC CH0 RX[MAC0.rxconfig]
     * # Strip FCS = 1
     * # Promiscuous Mode = 1
     * apb_write(addr=0x2102, data=0x0012)
     */
    data = 0;
    switch(ch) {
        case 0:
            //set_fld_mcmac0__rxconfig__stripfcs( &data, 1); // not set by rtl
            set_fld_mcmac0__rxconfig__promiscuous( &data, 1);
            break;
        case 1:
            //set_fld_mcmac1__rxconfig__stripfcs( &data, 1); // not set by rtl
            set_fld_mcmac1__rxconfig__promiscuous( &data, 1);
            break;
        case 2:
            //set_fld_mcmac2__rxconfig__stripfcs( &data, 1); // not set by rtl
            set_fld_mcmac2__rxconfig__promiscuous( &data, 1);
            break;
        case 3:
            //set_fld_mcmac3__rxconfig__stripfcs( &data, 1); // not set by rtl
            set_fld_mcmac3__rxconfig__promiscuous( &data, 1);
            break;
        default: bf_sys_assert(0);
    }
    mac_write( dev_id,  mac_block, mcmac_rxconfig_reg[ch], data );
#endif  // 0
  port_mgr_mac_basic_setup(dev_id, mac_block, ch);

  /* # Configure PCS CH0 Mode[PCS0.mode]
   * # PMA = 0011 [BASE-R1]
   * # fec = 00 [None]
   *         01 [FCFEC]
   *         10 [RSFEC]
   * apb_write(addr=0x4010, data=0x0014)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_hsmcpcs0__mode__pma(&data, pma);
      set_fld_hsmcpcs0__mode__fec(&data, fec);
      break;
    case 1:
      set_fld_hsmcpcs1__mode__pma(&data, pma);
      set_fld_hsmcpcs1__mode__fec(&data, fec);
      break;
    case 2:
      set_fld_hsmcpcs2__mode__pma(&data, pma);
      set_fld_hsmcpcs2__mode__fec(&data, fec);
      break;
    case 3:
      set_fld_hsmcpcs3__mode__pma(&data, pma);
      set_fld_hsmcpcs3__mode__fec(&data, fec);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, hsmcpcs_mode_reg[ch], data);

  // Configure RSFEC CH0 settings[RSFEC0.dbgctrl]
  if (fec == 0) {  // disable both RSFEC and FCFEC
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 1);
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);

    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 0);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 0);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 0);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 0);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);
  } else if (fec == 1) {  // fcfec
    // disable any rsfec
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 1);
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);

    /* # Configure FCFEC CH0 settings[FCFEC0.cfg]
     * # fc enable request = 1
     * apb_write(addr=0x48ab, data=0x0007)
     */
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 1);
        set_fld_fecfc0__cfg__enapcserr(&data, 1);
        set_fld_fecfc0__cfg__enaerrcorr(&data, 1);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 1);
        set_fld_fecfc1__cfg__enapcserr(&data, 1);
        set_fld_fecfc1__cfg__enaerrcorr(&data, 1);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 1);
        set_fld_fecfc2__cfg__enapcserr(&data, 1);
        set_fld_fecfc2__cfg__enaerrcorr(&data, 1);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 1);
        set_fld_fecfc3__cfg__enapcserr(&data, 1);
        set_fld_fecfc3__cfg__enaerrcorr(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);
  } else if (fec == 2) {  // rsfec
    // disable any fcfec
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 0);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 0);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 0);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 0);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);

    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs0__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs1__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs2__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      default:
        bf_sys_assert(0);
    }
    // hack til we get new xml file
    data |= (1 << 8);  // enable_pcs_scrambler
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);
  }

  /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
   * # PortMap 0 = 000
   * # PortMap 1 = 001
   * # PortMap 2 = 010
   * # PortMap 3 = 011
   * # PortMap0 enable = 1
   * # PortMap1 enable = 1
   * # PortMap2 enable = 1
   * # PortMap3 enable = 1
   * apb_write(addr=0x3f05, data=0x1920)
   */
  data = mac_read(dev_id, mac_block, fifoctrl0__appfifoportmap0);
  if (ch == 0) set_fld_fifoctrl0__appfifoportmap0__portmap0(&data, 0);
  if (ch == 1) set_fld_fifoctrl0__appfifoportmap0__portmap1(&data, 1);
  if (ch == 2) set_fld_fifoctrl0__appfifoportmap0__portmap2(&data, 2);
  if (ch == 3) set_fld_fifoctrl0__appfifoportmap0__portmap3(&data, 3);

  if (ch == 0) set_fld_fifoctrl0__appfifoportmap0__portmap0ena(&data, 1);
  if (ch == 1) set_fld_fifoctrl0__appfifoportmap0__portmap1ena(&data, 1);
  if (ch == 2) set_fld_fifoctrl0__appfifoportmap0__portmap2ena(&data, 1);
  if (ch == 3) set_fld_fifoctrl0__appfifoportmap0__portmap3ena(&data, 1);
  mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

  /* # If SerDes TX->RX internal loopback is desired, activate loopback
   * [SerDes_mux.serdeslpbk].
   * # SerDes CH0 loopback enable = 1
   * apb_write(addr=0x5f01, data=0x0001)
   */
  if (loopback_en) {
    data = mac_read(dev_id, mac_block, serdesmux__serdeslpbk);
    switch (ch) {
      case 0:
        set_fld_serdesmux__serdeslpbk__lpbken0(&data, 1);
        break;
      case 1:
        set_fld_serdesmux__serdeslpbk__lpbken1(&data, 1);
        break;
      case 2:
        set_fld_serdesmux__serdeslpbk__lpbken2(&data, 1);
        break;
      case 3:
        set_fld_serdesmux__serdeslpbk__lpbken3(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
  }

  /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 0
   * apb_write(addr=0x1f10, data=0x0025)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch0mode__swreset(&data, 1);
      set_fld_glbl__ch0mode__chena(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch1mode__swreset(&data, 1);
      set_fld_glbl__ch1mode__chena(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch2mode__swreset(&data, 1);
      set_fld_glbl__ch2mode__chena(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__speed(&data, CMRA_CH_SPD_25G);
      set_fld_glbl__ch3mode__swreset(&data, 1);
      set_fld_glbl__ch3mode__chena(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);
}

/* port_mgr_mac_set_speed_10g
 *
 */
void port_mgr_mac_set_speed_10g(bf_dev_id_t dev_id,
                                int mac_block,
                                int ch,
                                int pma,
                                int fec,
                                int loopback_en) {
  uint32_t data;
  uint32_t mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};
  uint32_t mcmac_ctrl_reg[4] = {
      mcmac0__ctrl, mcmac1__ctrl, mcmac2__ctrl, mcmac3__ctrl};
  uint32_t mcmac_rxconfig_reg[4] = {
      mcmac0__rxconfig, mcmac1__rxconfig, mcmac2__rxconfig, mcmac3__rxconfig};
  uint32_t hsmcpcs_mode_reg[4] = {
      hsmcpcs0__mode, hsmcpcs1__mode, hsmcpcs2__mode, hsmcpcs3__mode};
  uint32_t fecrs_cfg_reg[4] = {
      fecrs0__dbgctrl, fecrs1__dbgctrl, fecrs2__dbgctrl, fecrs3__dbgctrl};
  uint32_t fecfc_cfg_reg[4] = {
      fecfc0__cfg, fecfc1__cfg, fecfc2__cfg, fecfc3__cfg};

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: PGM: %d:%2d:%d : Pgm: spd=10g : fec=%d : lpbk=%d",
                 dev_id,
                 mac_block,
                 ch,
                 fec,
                 loopback_en);
  }

  // sanity checks for 25g mode
  // (keeps signatures of set_speed_xxxg consistent)
  bf_sys_assert(pma == CMRA_PMA_BASE_R1);

  /* # Activate Ch SW reset for "ch" [Global.ch0-3mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f10-3, data=0x0002)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__swreset(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__swreset(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__swreset(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__swreset(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);

  // hack until we get new_xml
  // 4011 is MLG cfg, disable mlg
  mac_write(dev_id, mac_block, hsmcpcs_mode_reg[ch] + 1, 0);

  /* # Configure Ch speed/enable for CH0[Global.ch0mode]
   * # Ch Speed = 0100 [10G]
   * # Ch SW reset = 1
   * # Ch enable = 1
   * apb_write(addr=0x1f10, data=0x0027)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch0mode__swreset(&data, 1);
      set_fld_glbl__ch0mode__chena(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch1mode__swreset(&data, 1);
      set_fld_glbl__ch1mode__chena(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch2mode__swreset(&data, 1);
      set_fld_glbl__ch2mode__chena(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch3mode__swreset(&data, 1);
      set_fld_glbl__ch3mode__chena(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);

  /* # Configure MAC CH0 Ctrl[MAC0.ctrl]
   * # Transmit Enable = 1
   * # Receive  Enable = 1
   * apb_write(addr=0x2100, data=0x0003)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_mcmac0__ctrl__txenable(&data, 1);
      set_fld_mcmac0__ctrl__rxenable(&data, 1);
      break;
    case 1:
      set_fld_mcmac1__ctrl__txenable(&data, 1);
      set_fld_mcmac1__ctrl__rxenable(&data, 1);
      break;
    case 2:
      set_fld_mcmac2__ctrl__txenable(&data, 1);
      set_fld_mcmac2__ctrl__rxenable(&data, 1);
      break;
    case 3:
      set_fld_mcmac3__ctrl__txenable(&data, 1);
      set_fld_mcmac3__ctrl__rxenable(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mcmac_ctrl_reg[ch], data);

  /* # Configure MAC CH0 RX[MAC0.rxconfig]
   * # Strip FCS = 1
   * # Promiscuous Mode = 1
   * apb_write(addr=0x2102, data=0x0012)
   */
  data = 0;
  switch (ch) {
    case 0:
      // set_fld_mcmac0__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac0__rxconfig__promiscuous(&data, 1);
      break;
    case 1:
      // set_fld_mcmac1__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac1__rxconfig__promiscuous(&data, 1);
      break;
    case 2:
      // set_fld_mcmac2__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac2__rxconfig__promiscuous(&data, 1);
      break;
    case 3:
      // set_fld_mcmac3__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac3__rxconfig__promiscuous(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mcmac_rxconfig_reg[ch], data);

  port_mgr_mac_basic_setup(dev_id, mac_block, ch);

  /* # Configure PCS CH0 Mode[PCS0.mode]
   * # PMA = 0011 [BASE-R1]
   * # fec = 00 [None]
   *         01 [FCFEC]
   *         10 [RSFEC]
   * apb_write(addr=0x4010, data=0x0014)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_hsmcpcs0__mode__pma(&data, pma);
      set_fld_hsmcpcs0__mode__fec(&data, fec);
      break;
    case 1:
      set_fld_hsmcpcs1__mode__pma(&data, pma);
      set_fld_hsmcpcs1__mode__fec(&data, fec);
      break;
    case 2:
      set_fld_hsmcpcs2__mode__pma(&data, pma);
      set_fld_hsmcpcs2__mode__fec(&data, fec);
      break;
    case 3:
      set_fld_hsmcpcs3__mode__pma(&data, pma);
      set_fld_hsmcpcs3__mode__fec(&data, fec);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, hsmcpcs_mode_reg[ch], data);

  // Configure RSFEC CH0 settings[RSFEC0.dbgctrl]
  if (fec == 0) {  // disable both RSFEC and FCFEC
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 1);
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);

    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 0);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 0);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 0);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 0);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);
  } else if (fec == 1) {  // fcfec
    // disable any rsfec
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 1);
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);

    /* # Configure FCFEC CH0 settings[FCFEC0.cfg]
     * # fc enable request = 1
     * apb_write(addr=0x48ab, data=0x0007)
     */
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 1);
        set_fld_fecfc0__cfg__enapcserr(&data, 1);
        set_fld_fecfc0__cfg__enaerrcorr(&data, 1);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 1);
        set_fld_fecfc1__cfg__enapcserr(&data, 1);
        set_fld_fecfc1__cfg__enaerrcorr(&data, 1);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 1);
        set_fld_fecfc2__cfg__enapcserr(&data, 1);
        set_fld_fecfc2__cfg__enaerrcorr(&data, 1);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 1);
        set_fld_fecfc3__cfg__enapcserr(&data, 1);
        set_fld_fecfc3__cfg__enaerrcorr(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);
  } else if (fec == 2) {  // rsfec
    // disable any fcfec
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 0);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 0);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 0);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 0);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);

    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs0__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs1__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs2__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      default:
        bf_sys_assert(0);
    }
    // hack til we get new xml file
    data |= (1 << 8);  // enable_pcs_scrambler
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);
  }

  /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
   * # PortMap 0 = 000
   * # PortMap 1 = 001
   * # PortMap 2 = 010
   * # PortMap 3 = 011
   * # PortMap0 enable = 1
   * # PortMap1 enable = 1
   * # PortMap2 enable = 1
   * # PortMap3 enable = 1
   * apb_write(addr=0x3f05, data=0x1920)
   */
  data = mac_read(dev_id, mac_block, fifoctrl0__appfifoportmap0);
  if (ch == 0) set_fld_fifoctrl0__appfifoportmap0__portmap0(&data, 0);
  if (ch == 1) set_fld_fifoctrl0__appfifoportmap0__portmap1(&data, 1);
  if (ch == 2) set_fld_fifoctrl0__appfifoportmap0__portmap2(&data, 2);
  if (ch == 3) set_fld_fifoctrl0__appfifoportmap0__portmap3(&data, 3);

  if (ch == 0) set_fld_fifoctrl0__appfifoportmap0__portmap0ena(&data, 1);
  if (ch == 1) set_fld_fifoctrl0__appfifoportmap0__portmap1ena(&data, 1);
  if (ch == 2) set_fld_fifoctrl0__appfifoportmap0__portmap2ena(&data, 1);
  if (ch == 3) set_fld_fifoctrl0__appfifoportmap0__portmap3ena(&data, 1);
  mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

  /* # If SerDes TX->RX internal loopback is desired, activate loopback
   * [SerDes_mux.serdeslpbk].
   * # SerDes CH0 loopback enable = 1
   * apb_write(addr=0x5f01, data=0x0001)
   */
  if (loopback_en) {
    data = mac_read(dev_id, mac_block, serdesmux__serdeslpbk);
    switch (ch) {
      case 0:
        set_fld_serdesmux__serdeslpbk__lpbken0(&data, 1);
        break;
      case 1:
        set_fld_serdesmux__serdeslpbk__lpbken1(&data, 1);
        break;
      case 2:
        set_fld_serdesmux__serdeslpbk__lpbken2(&data, 1);
        break;
      case 3:
        set_fld_serdesmux__serdeslpbk__lpbken3(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
  }

  /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 0
   * apb_write(addr=0x1f10, data=0x0025)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch0mode__swreset(&data, 1);
      set_fld_glbl__ch0mode__chena(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch1mode__swreset(&data, 1);
      set_fld_glbl__ch1mode__chena(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch2mode__swreset(&data, 1);
      set_fld_glbl__ch2mode__chena(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__speed(&data, CMRA_CH_SPD_10G);
      set_fld_glbl__ch3mode__swreset(&data, 1);
      set_fld_glbl__ch3mode__chena(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);
}

/* port_mgr_mac_set_speed_1g
 *
 */
void port_mgr_mac_set_speed_1g(bf_dev_id_t dev_id,
                               int mac_block,
                               int ch,
                               int pma,
                               int fec,
                               int loopback_en) {
  uint32_t data;
  uint32_t mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};
  uint32_t mcmac_ctrl_reg[4] = {
      mcmac0__ctrl, mcmac1__ctrl, mcmac2__ctrl, mcmac3__ctrl};
  uint32_t mcmac_rxconfig_reg[4] = {
      mcmac0__rxconfig, mcmac1__rxconfig, mcmac2__rxconfig, mcmac3__rxconfig};
  uint32_t hsmcpcs_mode_reg[4] = {
      hsmcpcs0__mode, hsmcpcs1__mode, hsmcpcs2__mode, hsmcpcs3__mode};
  uint32_t fecrs_cfg_reg[4] = {
      fecrs0__dbgctrl, fecrs1__dbgctrl, fecrs2__dbgctrl, fecrs3__dbgctrl};
  uint32_t fecfc_cfg_reg[4] = {
      fecfc0__cfg, fecfc1__cfg, fecfc2__cfg, fecfc3__cfg};

  if (port_mgr_log_worthy(LOG_TYP_MAC, dev_id, mac_block, 255)) {
    port_mgr_log("MAC: PGM: %d:%2d:%d : Pgm: spd=1g : fec=%d : lpbk=%d",
                 dev_id,
                 mac_block,
                 ch,
                 fec,
                 loopback_en);
  }

  // sanity checks for 25g mode
  // (keeps signatures of set_speed_xxxg consistent)
  bf_sys_assert(pma == CMRA_PMA_BASE_R1);

  /* # Activate Ch SW reset for "ch" [Global.ch0-3mode]
   * # Ch SW reset = 1
   * apb_write(addr=0x1f10-3, data=0x0002)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__swreset(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__swreset(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__swreset(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__swreset(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);

  /* # Configure Ch speed/enable for CH0[Global.ch0mode]
   * # Ch Speed = 0100 [10G]
   * # Ch SW reset = 1
   * # Ch enable = 1
   * apb_write(addr=0x1f10, data=0x0027)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch0mode__swreset(&data, 1);
      set_fld_glbl__ch0mode__chena(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch1mode__swreset(&data, 1);
      set_fld_glbl__ch1mode__chena(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch2mode__swreset(&data, 1);
      set_fld_glbl__ch2mode__chena(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch3mode__swreset(&data, 1);
      set_fld_glbl__ch3mode__chena(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);

  /* # Configure MAC CH0 Ctrl[MAC0.ctrl]
   * # Transmit Enable = 1
   * # Receive  Enable = 1
   * apb_write(addr=0x2100, data=0x0003)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_mcmac0__ctrl__txenable(&data, 1);
      set_fld_mcmac0__ctrl__rxenable(&data, 1);
      break;
    case 1:
      set_fld_mcmac1__ctrl__txenable(&data, 1);
      set_fld_mcmac1__ctrl__rxenable(&data, 1);
      break;
    case 2:
      set_fld_mcmac2__ctrl__txenable(&data, 1);
      set_fld_mcmac2__ctrl__rxenable(&data, 1);
      break;
    case 3:
      set_fld_mcmac3__ctrl__txenable(&data, 1);
      set_fld_mcmac3__ctrl__rxenable(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mcmac_ctrl_reg[ch], data);

  /* # Configure MAC CH0 RX[MAC0.rxconfig]
   * # Strip FCS = 1
   * # Promiscuous Mode = 1
   * apb_write(addr=0x2102, data=0x0012)
   */
  data = 0;
  switch (ch) {
    case 0:
      // set_fld_mcmac0__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac0__rxconfig__promiscuous(&data, 1);
      break;
    case 1:
      // set_fld_mcmac1__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac1__rxconfig__promiscuous(&data, 1);
      break;
    case 2:
      // set_fld_mcmac2__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac2__rxconfig__promiscuous(&data, 1);
      break;
    case 3:
      // set_fld_mcmac3__rxconfig__stripfcs( &data, 1); // not set by rtl
      set_fld_mcmac3__rxconfig__promiscuous(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mcmac_rxconfig_reg[ch], data);

  /* # Configure PCS CH0 Mode[PCS0.mode]
   * # PMA = 0011 [BASE-R1]
   * # fec = 00 [None]
   *         01 [FCFEC]
   *         10 [RSFEC]
   * apb_write(addr=0x4010, data=0x0014)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_hsmcpcs0__mode__pma(&data, pma);
      set_fld_hsmcpcs0__mode__fec(&data, fec);
      break;
    case 1:
      set_fld_hsmcpcs1__mode__pma(&data, pma);
      set_fld_hsmcpcs1__mode__fec(&data, fec);
      break;
    case 2:
      set_fld_hsmcpcs2__mode__pma(&data, pma);
      set_fld_hsmcpcs2__mode__fec(&data, fec);
      break;
    case 3:
      set_fld_hsmcpcs3__mode__pma(&data, pma);
      set_fld_hsmcpcs3__mode__fec(&data, fec);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, hsmcpcs_mode_reg[ch], data);

  // Configure RSFEC CH0 settings[RSFEC0.dbgctrl]
  if (fec == 0) {  // disable both RSFEC and FCFEC
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 1);
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);

    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 0);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 0);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 0);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 0);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);
  } else if (fec == 1) {  // fcfec
    // disable any rsfec
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 1);
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 1);
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 1);
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);

    /* # Configure FCFEC CH0 settings[FCFEC0.cfg]
     * # fc enable request = 1
     * apb_write(addr=0x48ab, data=0x0007)
     */
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 1);
        set_fld_fecfc0__cfg__enapcserr(&data, 1);
        set_fld_fecfc0__cfg__enaerrcorr(&data, 1);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 1);
        set_fld_fecfc1__cfg__enapcserr(&data, 1);
        set_fld_fecfc1__cfg__enaerrcorr(&data, 1);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 1);
        set_fld_fecfc2__cfg__enapcserr(&data, 1);
        set_fld_fecfc2__cfg__enaerrcorr(&data, 1);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 1);
        set_fld_fecfc3__cfg__enapcserr(&data, 1);
        set_fld_fecfc3__cfg__enaerrcorr(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);
  } else if (fec == 2) {  // rsfec
    // disable any fcfec
    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecfc0__cfg__enareq(&data, 0);
        break;
      case 1:
        set_fld_fecfc1__cfg__enareq(&data, 0);
        break;
      case 2:
        set_fld_fecfc2__cfg__enareq(&data, 0);
        break;
      case 3:
        set_fld_fecfc3__cfg__enareq(&data, 0);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, fecfc_cfg_reg[ch], data);

    data = 0;
    switch (ch) {
      case 0:
        set_fld_fecrs0__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs0__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 1:
        set_fld_fecrs1__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs1__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 2:
        set_fld_fecrs2__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs2__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      case 3:
        set_fld_fecrs3__dbgctrl__disablefec(&data, 0);

        if (USE_SHORT_TIMER) {
          set_fld_fecrs3__dbgctrl__debuguseshortamp(&data, 1);
        }
        break;
      default:
        bf_sys_assert(0);
    }
    // hack til we get new xml file
    data |= (1 << 8);  // enable_pcs_scrambler
    mac_write(dev_id, mac_block, fecrs_cfg_reg[ch], data);
  }

  /* # Configure APPFIFO Port map[APPFifo.appfifoportmap0]
   * # PortMap 0 = 000
   * # PortMap 1 = 001
   * # PortMap 2 = 010
   * # PortMap 3 = 011
   * # PortMap0 enable = 1
   * # PortMap1 enable = 1
   * # PortMap2 enable = 1
   * # PortMap3 enable = 1
   * apb_write(addr=0x3f05, data=0x1920)
   */
  data = mac_read(dev_id, mac_block, fifoctrl0__appfifoportmap0);
  if (ch == 0) set_fld_fifoctrl0__appfifoportmap0__portmap0(&data, 0);
  if (ch == 1) set_fld_fifoctrl0__appfifoportmap0__portmap1(&data, 1);
  if (ch == 2) set_fld_fifoctrl0__appfifoportmap0__portmap2(&data, 2);
  if (ch == 3) set_fld_fifoctrl0__appfifoportmap0__portmap3(&data, 3);

  if (ch == 0) set_fld_fifoctrl0__appfifoportmap0__portmap0ena(&data, 1);
  if (ch == 1) set_fld_fifoctrl0__appfifoportmap0__portmap1ena(&data, 1);
  if (ch == 2) set_fld_fifoctrl0__appfifoportmap0__portmap2ena(&data, 1);
  if (ch == 3) set_fld_fifoctrl0__appfifoportmap0__portmap3ena(&data, 1);
  mac_write(dev_id, mac_block, fifoctrl0__appfifoportmap0, data);

  /* # If SerDes TX->RX internal loopback is desired, activate loopback
   * [SerDes_mux.serdeslpbk].
   * # SerDes CH0 loopback enable = 1
   * apb_write(addr=0x5f01, data=0x0001)
   */
  if (loopback_en) {
    data = mac_read(dev_id, mac_block, serdesmux__serdeslpbk);
    switch (ch) {
      case 0:
        set_fld_serdesmux__serdeslpbk__lpbken0(&data, 1);
        break;
      case 1:
        set_fld_serdesmux__serdeslpbk__lpbken1(&data, 1);
        break;
      case 2:
        set_fld_serdesmux__serdeslpbk__lpbken2(&data, 1);
        break;
      case 3:
        set_fld_serdesmux__serdeslpbk__lpbken3(&data, 1);
        break;
      default:
        bf_sys_assert(0);
    }
    mac_write(dev_id, mac_block, serdesmux__serdeslpbk, data);
  }

  /* # Deactivate Ch SW reset for CH0 [Global.ch0mode]
   * # Ch SW reset = 0
   * apb_write(addr=0x1f10, data=0x0025)
   */
  data = 0;
  switch (ch) {
    case 0:
      set_fld_glbl__ch0mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch0mode__swreset(&data, 1);
      set_fld_glbl__ch0mode__chena(&data, 1);
      break;
    case 1:
      set_fld_glbl__ch1mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch1mode__swreset(&data, 1);
      set_fld_glbl__ch1mode__chena(&data, 1);
      break;
    case 2:
      set_fld_glbl__ch2mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch2mode__swreset(&data, 1);
      set_fld_glbl__ch2mode__chena(&data, 1);
      break;
    case 3:
      set_fld_glbl__ch3mode__speed(&data, CMRA_CH_SPD_1G);
      set_fld_glbl__ch3mode__swreset(&data, 1);
      set_fld_glbl__ch3mode__chena(&data, 1);
      break;
    default:
      bf_sys_assert(0);
  }
  mac_write(dev_id, mac_block, mode_reg[ch], data);
}

/****************************************************************************
 * port_mgr_mac_get_pcs_status
 *
 ****************************************************************************/
int port_mgr_mac_get_pcs_status(bf_dev_id_t dev_id,
                                int mac_block,
                                int mac,
                                bool *pcs_status,
                                uint32_t *block_lock_per_pcs_lane,
                                uint32_t *alignment_marker_lock_per_pcs_lane,
                                bool *hi_ber,
                                bool *block_lock_all,
                                bool *alignment_marker_lock_all) {
  uint32_t data, tmp;
  uint32_t stride = (hsmcpcs1__status1 - hsmcpcs0__status1);
  uint32_t blk_lk, am_lk, am_lk_all;

  data = mac_read(dev_id, mac_block, hsmcpcs0__sts1 + (mac * stride));

  // PCS status1 fields
  *pcs_status = get_fld_hsmcpcs0__sts1__pcsstatus(&data);
  *hi_ber = get_fld_hsmcpcs0__sts1__hiber(&data);
  *block_lock_all = get_fld_hsmcpcs0__sts1__blocklockall(&data);

  // Multi-lane BASE-R PCS Alignment status1
  data = mac_read(dev_id, mac_block, hsmcpcs0__algnstat1 + (mac * stride));
  blk_lk = get_fld_hsmcpcs0__algnstat1__blocklock(&data);
  am_lk_all = get_fld_hsmcpcs0__algnstat1__alignstatus(&data);
  *alignment_marker_lock_all = am_lk_all;

  // Multi-lane BASE-R PCS Alignment status1
  data = mac_read(dev_id, mac_block, hsmcpcs0__algnstat2 + (mac * stride));
  tmp = get_fld_hsmcpcs0__algnstat2__blocklock(&data);
  blk_lk = blk_lk | (tmp << 8);
  *block_lock_per_pcs_lane = blk_lk;

  data = mac_read(dev_id, mac_block, hsmcpcs0__algnstat3 + (mac * stride));
  am_lk = get_fld_hsmcpcs0__algnstat3__amlock(&data);

  data = mac_read(dev_id, mac_block, hsmcpcs0__algnstat4 + (mac * stride));
  tmp = get_fld_hsmcpcs0__algnstat4__amlock(&data);
  am_lk = am_lk | (tmp << 8);
  *alignment_marker_lock_per_pcs_lane = am_lk;

  return 0;
}

int port_mgr_mac_get_pcs_status_v2(bf_dev_id_t dev_id,
                                   int mac_block,
                                   int mac,
                                   bool *pcs_status,
                                   bool *hi_ber,
                                   bool *block_lock_all) {
  uint32_t data;
  uint32_t stride = (hsmcpcs1__status1 - hsmcpcs0__status1);

  data = mac_read(dev_id, mac_block, hsmcpcs0__sts1 + (mac * stride));

  // PCS status1 fields
  if (pcs_status) {
    *pcs_status = get_fld_hsmcpcs0__sts1__pcsstatus(&data);
  }
  if (hi_ber) {
    *hi_ber = get_fld_hsmcpcs0__sts1__hiber(&data);
  }
  if (block_lock_all) {
    *block_lock_all = get_fld_hsmcpcs0__sts1__blocklockall(&data);
  }

  return 0;
}

/****************************************************************************
 * port_mgr_mac_get_1588_timestamp_tx
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_get_1588_timestamp_tx(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint64_t *ts,
                                               bool *ts_valid,
                                               int *ts_id) {
  int mac_blk = 0;
  int ch = 0;
  uint32_t data;

  if (!ts || !ts_valid || !ts_id) return BF_INVALID_ARG;

  if (port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL) != PORT_MGR_OK) {
    return BF_INVALID_ARG;
  }

  data = mac_read(dev_id, mac_blk, mcmac0__txtsinfo + (ch * 0x100));
  *ts_id = get_fld_mcmac0__txtsinfo__tsid(&data);
  *ts_valid = get_fld_mcmac0__txtsinfo__tsvld(&data);
  if (*ts_valid) {
    uint64_t v0, v1, v2, v3;

    data = mac_read(dev_id, mac_blk, mcmac0__tsv0 + (ch * 0x100));
    v0 = get_fld_mcmac0__tsv0__ts(&data);
    data = mac_read(dev_id, mac_blk, mcmac0__tsv1 + (ch * 0x100));
    v1 = get_fld_mcmac0__tsv1__ts(&data);
    data = mac_read(dev_id, mac_blk, mcmac0__tsv2 + (ch * 0x100));
    v2 = get_fld_mcmac0__tsv2__ts(&data);
    data = mac_read(dev_id, mac_blk, mcmac0__tsv3 + (ch * 0x100));
    v3 = get_fld_mcmac0__tsv3__ts(&data);
    *ts = ((v3 << 48) | (v2 << 32) | (v1 << 16) | (v0 << 0));
  }

  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_set_1588_timestamp_delta_tx
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_set_1588_timestamp_delta_tx(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint16_t delta) {
  int mac_blk = 0;
  int ch = 0;
  uint32_t data = 0;

  if (port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL) != PORT_MGR_OK) {
    return BF_INVALID_ARG;
  }

  set_fld_mcmac0__txtsdelta__txtsdelta(&data, delta);
  mac_write(dev_id, mac_blk, mcmac0__txtsdelta + (ch * 0x100), data);

  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_get_1588_timestamp_delta_tx
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_get_1588_timestamp_delta_tx(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint16_t *delta) {
  int mac_blk = 0;
  int ch = 0;
  uint32_t data = 0;

  if (!delta) return BF_INVALID_ARG;

  if (port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL) != PORT_MGR_OK) {
    return BF_INVALID_ARG;
  }

  data = mac_read(dev_id, mac_blk, mcmac0__txtsdelta + (ch * 0x100));
  *delta = get_fld_mcmac0__txtsdelta__txtsdelta(&data);

  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_set_1588_timestamp_delta_rx
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_set_1588_timestamp_delta_rx(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint16_t delta) {
  int mac_blk = 0;
  int ch = 0;
  uint32_t data = 0;

  if (port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL) != PORT_MGR_OK) {
    return BF_INVALID_ARG;
  }

  set_fld_mcmac0__rxtsdelta__rxtsdelta(&data, delta);
  mac_write(dev_id, mac_blk, mcmac0__rxtsdelta + (ch * 0x100), data);

  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_get_1588_timestamp_delta_rx
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_get_1588_timestamp_delta_rx(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint16_t *delta) {
  int mac_blk = 0;
  int ch = 0;
  uint32_t data = 0;

  if (!delta) return BF_INVALID_ARG;

  if (port_mgr_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL) != PORT_MGR_OK) {
    return BF_INVALID_ARG;
  }

  data = mac_read(dev_id, mac_blk, mcmac0__rxtsdelta + (ch * 0x100));
  *delta = get_fld_mcmac0__rxtsdelta__rxtsdelta(&data);

  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_set_drain
 *
 ****************************************************************************/
void port_mgr_mac_set_drain(bf_dev_id_t dev_id,
                            bf_dev_port_t dev_port,
                            bool en) {
  int mac_blk, ch;
  uint32_t data;
  uint32_t mcmac_ctrl_reg[4] = {
      mcmac0__ctrl, mcmac1__ctrl, mcmac2__ctrl, mcmac3__ctrl};

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  data = mac_read(dev_id, mac_blk, mcmac_ctrl_reg[ch]);

  set_fld_mcmac0__ctrl__txdrn(&data, (en ? 1 : 0));
  mac_write(dev_id, mac_blk, mcmac_ctrl_reg[ch], data);
  return;
}

/****************************************************************************
 * port_mgr_mac_set_promiscuous_mode
 *
 ****************************************************************************/
void port_mgr_mac_set_promiscuous_mode(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int en) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);
  data = mac_read(dev_id, mac_blk, mcmac0__rxconfig + ch * 0x100);

  set_fld_mcmac0__rxconfig__promiscuous(&data, en ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__rxconfig + ch * 0x100, data);
  return;
}

/****************************************************************************
 * port_mgr_mac_set_lane_map_to_zero
 *
 ****************************************************************************/
void port_mgr_mac_set_lane_map_to_zero(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int num_lanes) {
  uint32_t rx_lane, tx_lane, rx_data, tx_data;
  int ln;
  int mac_block, ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  rx_data = mac_read(dev_id, mac_block, serdesmux__laneremaprx0);
  tx_data = mac_read(dev_id, mac_block, serdesmux__laneremaptx0);

  // Remap to logical-lane zero.
  for (ln = 0; ln < num_lanes; ln++) {
    rx_lane = 0;
    tx_lane = 0;
    switch (ch + ln) {
      case 0:
        set_fld_serdesmux__laneremaprx0__remap0(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap0(&tx_data, tx_lane);
        break;
      case 1:
        set_fld_serdesmux__laneremaprx0__remap1(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap1(&tx_data, tx_lane);
        break;
      case 2:
        set_fld_serdesmux__laneremaprx0__remap2(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap2(&tx_data, tx_lane);
        break;
      case 3:
        set_fld_serdesmux__laneremaprx0__remap3(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap3(&tx_data, tx_lane);
        break;
      default:
        bf_sys_assert(0);
    }
  }
  mac_write(dev_id, mac_block, serdesmux__laneremaprx0, rx_data);
  mac_write(dev_id, mac_block, serdesmux__laneremaptx0, tx_data);
}

/****************************************************************************
 * port_mgr_mac_restore_lane_remap
 *
 ****************************************************************************/
void port_mgr_mac_restore_lane_remap(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     int num_lanes) {
  uint32_t rx_lane, tx_lane, rx_data, tx_data;
  int ln;
  int mac_block, ch;
  port_mgr_tof1_pdev_t *dev_p = port_mgr_dev_physical_dev_get(dev_id);

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  rx_data = mac_read(dev_id, mac_block, serdesmux__laneremaprx0);
  tx_data = mac_read(dev_id, mac_block, serdesmux__laneremaptx0);

  for (ln = 0; ln < num_lanes; ln++) {
    rx_lane = dev_p->mac_block[mac_block].rx_lane_map[ch + ln];
    tx_lane = dev_p->mac_block[mac_block].tx_lane_map[ch + ln];

    switch (ch + ln) {
      case 0:
        set_fld_serdesmux__laneremaprx0__remap0(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap0(&tx_data, tx_lane);
        break;
      case 1:
        set_fld_serdesmux__laneremaprx0__remap1(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap1(&tx_data, tx_lane);
        break;
      case 2:
        set_fld_serdesmux__laneremaprx0__remap2(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap2(&tx_data, tx_lane);
        break;
      case 3:
        set_fld_serdesmux__laneremaprx0__remap3(&rx_data, rx_lane);
        set_fld_serdesmux__laneremaptx0__remap3(&tx_data, tx_lane);
        break;
      default:
        bf_sys_assert(0);
    }
  }
  mac_write(dev_id, mac_block, serdesmux__laneremaprx0, rx_data);
  mac_write(dev_id, mac_block, serdesmux__laneremaptx0, tx_data);
}

/****************************************************************************
 * port_mgr_mac_set_loopback_mode
 *
 *    BF_LPBK_NONE = 0,
 *    BF_LPBK_MAC_NEAR,
 *    BF_LPBK_MAC_FAR,
 *    BF_LPBK_PCS_NEAR,
 *    BF_LPBK_SERDES_NEAR,
 *    BF_LPBK_SERDES_FAR,
 *
 ****************************************************************************/
void port_mgr_mac_set_loopback_mode(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    bf_loopback_mode_e mode) {
  int mac_blk, ch, en;
  uint32_t data, spd, ch_mask, num_lanes, lp_thrsh;
  port_mgr_port_t *port_p;

  /* Don't touch the harware during cfg replay */
  if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_NONE) return;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) return;  // assoc port is disabled or not added

  if (mode == BF_LPBK_NONE) {
    en = 0;  // disable all loopbacks
  } else {
    en = 1;
  }

  // need to determine the affected channels
  data = port_mgr_mac_get_glb_mode(dev_id, dev_port);
  switch (ch) {
    case 0:
      spd = get_fld_glbl__ch0mode__speed(&data);
      break;
    case 1:
      spd = get_fld_glbl__ch1mode__speed(&data);
      break;
    case 2:
      spd = get_fld_glbl__ch2mode__speed(&data);
      break;
    case 3:
      spd = get_fld_glbl__ch3mode__speed(&data);
      break;
    default:
      return;
  }
  switch (spd) {
    case 9:  // 100
      ch_mask = 0xF;
      num_lanes = 4;
      if (mode == BF_LPBK_NONE) {
        lp_thrsh = 4;
      } else {
        lp_thrsh = 0x16;
      }
      break;
    case 7:  // 40
      ch_mask = 0xF;
      num_lanes = 4;
      if (mode == BF_LPBK_NONE) {
        lp_thrsh = 4;
      } else {
        lp_thrsh = 0xd;
      }
      break;
    case 8:  // 50
      ch_mask = (0x3 << ch);
      num_lanes = 2;
      if (mode == BF_LPBK_NONE) {
        lp_thrsh = 4;
      } else {
        lp_thrsh = 0xd;  // 16;
      }
      break;
    default:
      ch_mask = (0x1 << ch);
      num_lanes = 1;
      if (mode == BF_LPBK_NONE || port_p->sw.fec == BF_FEC_TYP_NONE) {
        lp_thrsh = 4;
      } else {
        lp_thrsh = 7;
      }
      break;
  }
  if ((mode == BF_LPBK_NONE) || (mode == BF_LPBK_MAC_NEAR)) {
    uint32_t mcmac_ctrl_reg[4] = {
        mcmac0__ctrl, mcmac1__ctrl, mcmac2__ctrl, mcmac3__ctrl};
    uint32_t sig_ovrd_val = (mode == BF_LPBK_NONE) ? 0 : 3;
    bf_status_t sts;
    bool is_sw_model = false;

    sts = bf_drv_device_type_get(dev_id, &is_sw_model);
    // model can't support serdes accesses
    if (sts || !is_sw_model) {
      port_mgr_mac_sigovrd_set(dev_id, dev_port, num_lanes, sig_ovrd_val);
      if (mode == BF_LPBK_MAC_NEAR) {
        port_mgr_mac_set_lane_map_to_zero(dev_id, dev_port, num_lanes);
      } else {
        port_mgr_mac_restore_lane_remap(dev_id, dev_port, num_lanes);
      }
    }

    // now, set the loopback mode
    data = mac_read(dev_id, mac_blk, mcmac_ctrl_reg[ch]);
    set_fld_mcmac0__ctrl__maclpbk(&data, en);
    mac_write(dev_id, mac_blk, mcmac_ctrl_reg[ch], data);
  }
  if ((mode == BF_LPBK_NONE) || (mode == BF_LPBK_MAC_FAR)) {
    // dis fcs
    uint32_t mcmac_rxconfig_reg[4] = {
        mcmac0__rxconfig, mcmac1__rxconfig, mcmac2__rxconfig, mcmac3__rxconfig};

    data = mac_read(dev_id, mac_blk, mcmac_rxconfig_reg[ch]);
    set_fld_mcmac0__rxconfig__stripfcs(&data, ((mode == BF_LPBK_NONE) ? 0 : 1));
    mac_write(dev_id, mac_blk, mcmac_rxconfig_reg[ch], data);

    // app fifo lpthrsh
    data = mac_read(dev_id, mac_blk, fifoctrl0__appfifolpthrsh);
    set_fld_fifoctrl0__appfifolpthrsh__appfifolpthrsh(&data, lp_thrsh);
    mac_write(dev_id, mac_blk, fifoctrl0__appfifolpthrsh, data);

    // enchnl
    data = mac_read(dev_id, mac_blk, fifoctrl0__appfifolpbk);
    data &= (~ch_mask);
    if (ch_mask & 0x1) {
      set_fld_fifoctrl0__appfifolpbk__ench0(&data, en);
      if (num_lanes > 1) {
        // num_lanes == 2 or 4
        set_fld_fifoctrl0__appfifolpbk__ench1(&data, 0);
      }
      if (num_lanes == 4) {
        set_fld_fifoctrl0__appfifolpbk__ench2(&data, 0);
        set_fld_fifoctrl0__appfifolpbk__ench3(&data, 0);
      }
    }
    if ((ch_mask & 0x2) && (num_lanes == 1)) {
      set_fld_fifoctrl0__appfifolpbk__ench1(&data, en);
    }
    if ((ch_mask & 0x4) && (num_lanes != 4)) {
      // num_lanes == 1 or 2
      set_fld_fifoctrl0__appfifolpbk__ench2(&data, en);
      if (num_lanes == 2) {
        set_fld_fifoctrl0__appfifolpbk__ench3(&data, 0);
      }
    }
    if ((ch_mask & 0x8) && (num_lanes == 1)) {
      set_fld_fifoctrl0__appfifolpbk__ench3(&data, en);
    }
    mac_write(dev_id, mac_blk, fifoctrl0__appfifolpbk, data);
  }
  if ((mode == BF_LPBK_NONE) || (mode == BF_LPBK_PCS_NEAR)) {
    data = mac_read(dev_id, mac_blk, serdesmux__serdeslpbk);
    if (ch_mask & 0x1) {
      set_fld_serdesmux__serdeslpbk__lpbken0(&data, en);
    }
    if (ch_mask & 0x2) {
      set_fld_serdesmux__serdeslpbk__lpbken1(&data, en);
    }
    if (ch_mask & 0x4) {
      set_fld_serdesmux__serdeslpbk__lpbken2(&data, en);
    }
    if (ch_mask & 0x8) {
      set_fld_serdesmux__serdeslpbk__lpbken3(&data, en);
    }
    mac_write(dev_id, mac_blk, serdesmux__serdeslpbk, data);
  }
  return;
}

/****************************************************************************
 * port_mgr_mac_force_pcs_near_loopback
 *
 *
 ****************************************************************************/
void port_mgr_mac_force_pcs_near_loopback(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          bool enable) {
  uint32_t data, spd, ch_mask;
  port_mgr_port_t *port_p;
  int mac_blk, ch, en;

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (port_p == NULL) return;

  if (port_p->forced_pcs_loopback == enable) {
    // nothing to do, required operation was already done
    return;
  } else {
    port_p->forced_pcs_loopback = enable;
  }

  /* Don't touch the harware during cfg replay */
  if (port_mgr_dev_ha_stage_get(dev_id) != PORT_MGR_HA_NONE) return;

  en = enable ? 1 : 0;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  // need to determine the affected channels
  data = port_mgr_mac_get_glb_mode(dev_id, dev_port);
  switch (ch) {
    case 0:
      spd = get_fld_glbl__ch0mode__speed(&data);
      break;
    case 1:
      spd = get_fld_glbl__ch1mode__speed(&data);
      break;
    case 2:
      spd = get_fld_glbl__ch2mode__speed(&data);
      break;
    case 3:
      spd = get_fld_glbl__ch3mode__speed(&data);
      break;
    default:
      return;
  }
  switch (spd) {
    case 9:  // 100
    case 7:  // 40
      ch_mask = 0xF;
      break;
    case 8:  // 50
      ch_mask = (0x3 << ch);
      break;
    default:
      ch_mask = (0x1 << ch);
  }

  data = mac_read(dev_id, mac_blk, serdesmux__serdeslpbk);
  if (ch_mask & 0x1) {
    set_fld_serdesmux__serdeslpbk__lpbken0(&data, en);
  }
  if (ch_mask & 0x2) {
    set_fld_serdesmux__serdeslpbk__lpbken1(&data, en);
  }
  if (ch_mask & 0x4) {
    set_fld_serdesmux__serdeslpbk__lpbken2(&data, en);
  }
  if (ch_mask & 0x8) {
    set_fld_serdesmux__serdeslpbk__lpbken3(&data, en);
  }
  mac_write(dev_id, mac_blk, serdesmux__serdeslpbk, data);
  return;
}

/****************************************************************************
 * port_mgr_mac_set_flow_control
 *
 ****************************************************************************/
void port_mgr_mac_set_flow_control(bf_dev_id_t dev_id, bf_dev_port_t dev_port) {
  int mac_block, ch;
  uint32_t data;
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) {
    return;  // this case should never be hit as caller already checks this
  }

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  // Read-modify-write for MAC Tx Configuration
  data = mac_read(dev_id, mac_block, mcmac0__txconfig + ch * 0x100);

  // if link-pause enabled, enable pause frame generation
  set_fld_mcmac0__txconfig__fcfrmgen(&data, port_p->sw.link_pause_tx ? 1 : 0);

  // if PFC enabled, enable PFC pause frame generation
  set_fld_mcmac0__txconfig__pfcfrmgen(&data, port_p->sw.pfc_pause_tx ? 1 : 0);

  mac_write(dev_id, mac_block, mcmac0__txconfig + ch * 0x100, data);

  // Read-modify-write for MAC Rx Configuration
  data = mac_read(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100);

  // PAUSE/PFC frames should be filtered out always
  set_fld_mcmac0__rxconfig__filterpf(&data, 1);

  if (port_p->sw.link_pause_rx || port_p->sw.pfc_pause_rx) {
    set_fld_mcmac0__rxconfig__enrxfcdec(&data, 1);
  } else {
    set_fld_mcmac0__rxconfig__enrxfcdec(&data, 0);
  }

  mac_write(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100, data);

  // if PFC TX is enabled, set PFC vector for all priorities (per Comira)
  mac_write(dev_id,
            mac_block,
            mcmac0__txpfcvec + ch * 0x100,
            port_p->sw.pfc_pause_tx ? 0xff : 0);
}

/****************************************************************************
 * port_mgr_mac_force_disable_rx_flow_control
 *
 *     This function is used internally during fast reconfig (before core reset)
 *
 ****************************************************************************/
void port_mgr_mac_force_disable_rx_flow_control(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port) {
  int mac_block, ch;
  uint32_t data;
  port_mgr_port_t *port_p =
      port_mgr_map_dev_port_to_port_allow_unassigned(dev_id, dev_port);

  if (port_p == NULL) {
    return;  // this case should never be hit as caller already checks this
  }

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  // Read-modify-write for MAC Rx Configuration
  data = mac_read(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100);

  // Disable RX flow control (takes care of both link pause and PFC)
  set_fld_mcmac0__rxconfig__enrxfcdec(&data, 0);

  mac_write(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_enable_rx_flow_control
 *
 *     This function is used internally during fast reconfig (after core reset)
 *
 ****************************************************************************/
void port_mgr_mac_enable_rx_flow_control(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port) {
  int mac_block, ch;
  uint32_t data;
  port_mgr_port_t *port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);

  if (port_p == NULL) {
    return;  // this case should never be hit as caller already checks this
  }

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  // Read-modify-write for MAC Rx Configuration
  data = mac_read(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100);

  // Enable RX flow control (takes care of both link pause and PFC)
  set_fld_mcmac0__rxconfig__enrxfcdec(&data, 1);
  set_fld_mcmac0__rxconfig__filterpf(&data, 1);

  mac_write(dev_id, mac_block, mcmac0__rxconfig + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_set_src_mac
 *
 ****************************************************************************/
void port_mgr_mac_set_src_mac(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              uint8_t *mac_addr) {
  int mac_blk, ch;
  uint32_t data, fld_val;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  fld_val = (mac_addr[1] << 8) | mac_addr[0];
  set_fld_mcmac0__macaddrlo__macaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__macaddrlo + ch * 0x100, data);

  data = 0;
  fld_val = (mac_addr[3] << 8) | mac_addr[2];
  set_fld_mcmac0__macaddrmid__macaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__macaddrmid + ch * 0x100, data);

  data = 0;
  fld_val = (mac_addr[5] << 8) | mac_addr[4];
  set_fld_mcmac0__macaddrhi__macaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__macaddrhi + ch * 0x100, data);
  return;
}

/****************************************************************************
 * port_mgr_mac_set_fc_src_mac
 *
 ****************************************************************************/
void port_mgr_mac_set_fc_src_mac(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint8_t *mac_addr) {
  int mac_blk, ch;
  uint32_t data, fld_val;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  fld_val = (mac_addr[1] << 8) | mac_addr[0];
  set_fld_mcmac0__fcsaddrlo__fcsaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__fcsaddrlo + ch * 0x100, data);

  data = 0;
  fld_val = (mac_addr[3] << 8) | mac_addr[2];
  set_fld_mcmac0__fcsaddrmid__fcsaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__fcsaddrmid + ch * 0x100, data);

  data = 0;
  fld_val = (mac_addr[5] << 8) | mac_addr[4];
  set_fld_mcmac0__fcsaddrhi__fcsaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__fcsaddrhi + ch * 0x100, data);
  return;
}

/****************************************************************************
 * port_mgr_mac_get_fc_src_mac
 *
 ****************************************************************************/
void port_mgr_mac_get_fc_src_mac(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint8_t *mac_addr) {
  int mac_blk, ch;
  uint32_t data, fld_val;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__fcsaddrlo + ch * 0x100);
  fld_val = get_fld_mcmac0__fcsaddrlo__fcsaddr(&data);
  mac_addr[0] = fld_val & 0xff;
  mac_addr[1] = (fld_val >> 8) & 0xff;

  data = mac_read(dev_id, mac_blk, mcmac0__fcsaddrmid + ch * 0x100);
  fld_val = get_fld_mcmac0__fcsaddrmid__fcsaddr(&data);
  mac_addr[2] = fld_val & 0xff;
  mac_addr[3] = (fld_val >> 8) & 0xff;

  data = mac_read(dev_id, mac_blk, mcmac0__fcsaddrhi + ch * 0x100);
  fld_val = get_fld_mcmac0__fcsaddrhi__fcsaddr(&data);
  mac_addr[4] = fld_val & 0xff;
  mac_addr[5] = (fld_val >> 8) & 0xff;

  return;
}

/****************************************************************************
 * port_mgr_mac_set_fc_dst_mac
 *
 ****************************************************************************/
void port_mgr_mac_set_fc_dst_mac(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint8_t *mac_addr) {
  int mac_blk, ch;
  uint32_t data, fld_val;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  fld_val = (mac_addr[1] << 8) | mac_addr[0];
  set_fld_mcmac0__fcdaddrlo__fcdaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__fcdaddrlo + ch * 0x100, data);

  data = 0;
  fld_val = (mac_addr[3] << 8) | mac_addr[2];
  set_fld_mcmac0__fcdaddrmid__fcdaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__fcdaddrmid + ch * 0x100, data);

  data = 0;
  fld_val = (mac_addr[5] << 8) | mac_addr[4];
  set_fld_mcmac0__fcdaddrhi__fcdaddr(&data, fld_val);
  mac_write(dev_id, mac_blk, mcmac0__fcdaddrhi + ch * 0x100, data);
  return;
}

/****************************************************************************
 * port_mgr_mac_get_fc_dst_mac
 *
 ****************************************************************************/
void port_mgr_mac_get_fc_dst_mac(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint8_t *mac_addr) {
  int mac_blk, ch;
  uint32_t data, fld_val;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__fcdaddrlo + ch * 0x100);
  fld_val = get_fld_mcmac0__fcdaddrlo__fcdaddr(&data);
  mac_addr[0] = fld_val & 0xff;
  mac_addr[1] = (fld_val >> 8) & 0xff;

  data = mac_read(dev_id, mac_blk, mcmac0__fcdaddrmid + ch * 0x100);
  fld_val = get_fld_mcmac0__fcdaddrmid__fcdaddr(&data);
  mac_addr[2] = fld_val & 0xff;
  mac_addr[3] = (fld_val >> 8) & 0xff;

  data = mac_read(dev_id, mac_blk, mcmac0__fcdaddrhi + ch * 0x100);
  fld_val = get_fld_mcmac0__fcdaddrhi__fcdaddr(&data);
  mac_addr[4] = fld_val & 0xff;
  mac_addr[5] = (fld_val >> 8) & 0xff;

  return;
}

/****************************************************************************
 * port_mgr_mac_set_fc_pause_time
 *
 ****************************************************************************/
void port_mgr_mac_set_fc_pause_time(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    uint32_t pause_time) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  set_fld_mcmac0__fcpausetime__fcpausetime(&data, pause_time);
  mac_write(dev_id, mac_blk, mcmac0__fcpausetime + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_set_xoff_pause_time
 *
 ****************************************************************************/
void port_mgr_mac_set_xoff_pause_time(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t pause_time) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  set_fld_mcmac0__xoffpausetime__xoffpausetime(&data, pause_time);
  mac_write(dev_id, mac_blk, mcmac0__xoffpausetime + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_get_xoff_pause_time
 *
 ****************************************************************************/
void port_mgr_mac_get_xoff_pause_time(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t *pause_time) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__xoffpausetime + ch * 0x100);
  *pause_time = get_fld_mcmac0__xoffpausetime__xoffpausetime(&data);
}

/****************************************************************************
 * port_mgr_mac_set_xon_pause_time
 *
 ****************************************************************************/
void port_mgr_mac_set_xon_pause_time(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     uint32_t pause_time) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  set_fld_mcmac0__xonpausetime__xonpausetime(&data, pause_time);
  mac_write(dev_id, mac_blk, mcmac0__xonpausetime + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_get_xon_pause_time
 *
 ****************************************************************************/
void port_mgr_mac_get_xon_pause_time(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     uint32_t *pause_time) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__xonpausetime + ch * 0x100);
  *pause_time = get_fld_mcmac0__xonpausetime__xonpausetime(&data);
}

/****************************************************************************
 * port_mgr_mac_set_rs_fec_control
 *
 ****************************************************************************/
void port_mgr_mac_set_rs_fec_control(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     bool byp_corr_en,
                                     bool byp_ind_en) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, fecrs0__ctrl + ch * 0x100);
  set_fld_fecrs0__ctrl__bypcorrena(&data, byp_corr_en ? 1 : 0);
  set_fld_fecrs0__ctrl__bypindiena(&data, byp_ind_en ? 1 : 0);
  mac_write(dev_id, mac_blk, fecrs0__ctrl + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_set_fc_fec_control
 *
 ****************************************************************************/
void port_mgr_mac_set_fc_fec_control(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     bool corr_en,
                                     bool ind_en) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, fecfc0__cfg + ch * 0x100);
  set_fld_fecfc0__cfg__enaerrcorr(&data, corr_en ? 1 : 0);
  set_fld_fecfc0__cfg__enapcserr(&data, ind_en ? 1 : 0);
  mac_write(dev_id, mac_blk, fecfc0__cfg + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_get_fc_fec_control
 *
 ****************************************************************************/
void port_mgr_mac_get_fc_fec_control(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     bool *corr_en,
                                     bool *ind_en) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, fecfc0__cfg + ch * 0x100);
  *corr_en = get_fld_fecfc0__cfg__enaerrcorr(&data);
  *ind_en = get_fld_fecfc0__cfg__enapcserr(&data);
}

/****************************************************************************
 * port_mgr_mac_get_rs_fec_status_and_counters
 *
 ****************************************************************************/
void port_mgr_mac_get_rs_fec_status_and_counters(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 bool *hi_ser,
                                                 bool *fec_align_status,
                                                 uint32_t *fec_corr_cnt,
                                                 uint32_t *fec_uncorr_cnt,
                                                 uint32_t *fec_ser_lane_0,
                                                 uint32_t *fec_ser_lane_1,
                                                 uint32_t *fec_ser_lane_2,
                                                 uint32_t *fec_ser_lane_3) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  // get requested FEC status/counter values
  data = mac_read(dev_id, mac_blk, fecrs0__sts + ch * 0x100);
  if (hi_ser) {
    *hi_ser = get_fld_fecrs0__sts__hiser(&data);
  }
  if (fec_align_status) {
    *fec_align_status = get_fld_fecrs0__sts__alignstatus(&data);
  }
  if (fec_corr_cnt) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, fecrs0__corrcnthi + ch * 0x100);
    cnt_hi = get_fld_fecrs0__corrcnthi__corrcnt(&data);
    data = mac_read(dev_id, mac_blk, fecrs0__corrcntlo + ch * 0x100);
    cnt_lo = get_fld_fecrs0__corrcntlo__corrcnt(&data);
    *fec_corr_cnt = (cnt_hi << 16) | cnt_lo;
  }
  if (fec_uncorr_cnt) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, fecrs0__uncorrcnthi + ch * 0x100);
    cnt_hi = get_fld_fecrs0__uncorrcnthi__uncorrcnt(&data);
    data = mac_read(dev_id, mac_blk, fecrs0__uncorrcntlo + ch * 0x100);
    cnt_lo = get_fld_fecrs0__uncorrcntlo__uncorrcnt(&data);
    *fec_uncorr_cnt = (cnt_hi << 16) | cnt_lo;
  }
  if (fec_ser_lane_0) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, fecrs0__serlane0hi + ch * 0x100);
    cnt_hi = get_fld_fecrs0__serlane0hi__serlane0(&data);
    data = mac_read(dev_id, mac_blk, fecrs0__serlane0lo + ch * 0x100);
    cnt_lo = get_fld_fecrs0__serlane0lo__serlane0(&data);
    *fec_ser_lane_0 = (cnt_hi << 16) | cnt_lo;
  }
  if (fec_ser_lane_1) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, fecrs0__serlane1hi + ch * 0x100);
    cnt_hi = get_fld_fecrs0__serlane1hi__serlane1(&data);
    data = mac_read(dev_id, mac_blk, fecrs0__serlane1lo + ch * 0x100);
    cnt_lo = get_fld_fecrs0__serlane1lo__serlane1(&data);
    *fec_ser_lane_1 = (cnt_hi << 16) | cnt_lo;
  }
  if (fec_ser_lane_2) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, fecrs0__serlane2hi + ch * 0x100);
    cnt_hi = get_fld_fecrs0__serlane2hi__serlane2(&data);
    data = mac_read(dev_id, mac_blk, fecrs0__serlane2lo + ch * 0x100);
    cnt_lo = get_fld_fecrs0__serlane2lo__serlane2(&data);
    *fec_ser_lane_2 = (cnt_hi << 16) | cnt_lo;
  }
  if (fec_ser_lane_3) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, fecrs0__serlane3hi + ch * 0x100);
    cnt_hi = get_fld_fecrs0__serlane3hi__serlane3(&data);
    data = mac_read(dev_id, mac_blk, fecrs0__serlane3lo + ch * 0x100);
    cnt_lo = get_fld_fecrs0__serlane3lo__serlane3(&data);
    *fec_ser_lane_3 = (cnt_hi << 16) | cnt_lo;
  }
}

/****************************************************************************
 * port_mgr_mac_get_fc_fec_status_and_counters_per_vl
 *
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_get_fc_fec_status_and_counters_per_vl(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    uint32_t vl,
    bool *block_lock_status,
    uint32_t *fec_corr_blk_cnt,
    uint32_t *fec_uncorr_blk_cnt) {
  int mac_blk, ch;
  uint32_t data, speed;
  uint32_t mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};
  uint32_t corr_blks_lo_reg, uncorr_blks_lo_reg;
  uint32_t corr_blks_hi_reg, uncorr_blks_hi_reg;
  uint32_t blk_lk_reg;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mode_reg[ch]);
  speed = get_fld_glbl__ch0mode__speed(&data);
  switch (speed) {
    case CMRA_CH_SPD_10G:
    case CMRA_CH_SPD_25G:
      if (vl > 0) return BF_INVALID_ARG;
      switch (ch) {
        case 0:
          corr_blks_lo_reg = fecfc0__corrblockscounterlo;
          corr_blks_hi_reg = fecfc0__corrblockscounterhi;
          uncorr_blks_lo_reg = fecfc0__uncorrblockscounterlo;
          uncorr_blks_hi_reg = fecfc0__uncorrblockscounterhi;
          blk_lk_reg = fecfc0__sts;
          break;
        case 1:
          corr_blks_lo_reg = fecfc2__corrblockscounterlo;
          corr_blks_hi_reg = fecfc2__corrblockscounterhi;
          uncorr_blks_lo_reg = fecfc2__uncorrblockscounterlo;
          uncorr_blks_hi_reg = fecfc2__uncorrblockscounterhi;
          blk_lk_reg = fecfc2__sts;
          break;
        case 2:
          corr_blks_lo_reg = fecfc0__corrblockscounterlovl1;
          corr_blks_hi_reg = fecfc0__corrblockscounterhivl1;
          uncorr_blks_lo_reg = fecfc0__uncorrblockscounterlovl1;
          uncorr_blks_hi_reg = fecfc0__uncorrblockscounterhivl1;
          blk_lk_reg = fecfc0__stsvl1;
          break;
        case 3:
          corr_blks_lo_reg = fecfc2__corrblockscounterlovl1;
          corr_blks_hi_reg = fecfc2__corrblockscounterhivl1;
          uncorr_blks_lo_reg = fecfc2__uncorrblockscounterlovl1;
          uncorr_blks_hi_reg = fecfc2__uncorrblockscounterhivl1;
          blk_lk_reg = fecfc2__stsvl1;
          break;
        default:
          return BF_INVALID_ARG;
      }
      break;
    case CMRA_CH_SPD_40G:  // 4 VLs
    case CMRA_CH_SPD_50G:  // 4 VLs
      switch (ch) {
        case 0:
          switch (vl) {
            case 0:
              corr_blks_lo_reg = fecfc0__corrblockscounterlo;
              corr_blks_hi_reg = fecfc0__corrblockscounterhi;
              uncorr_blks_lo_reg = fecfc0__uncorrblockscounterlo;
              uncorr_blks_hi_reg = fecfc0__uncorrblockscounterhi;
              blk_lk_reg = fecfc0__sts;
              break;
            case 1:
              corr_blks_lo_reg = fecfc1__corrblockscounterlo;
              corr_blks_hi_reg = fecfc1__corrblockscounterhi;
              uncorr_blks_lo_reg = fecfc1__uncorrblockscounterlo;
              uncorr_blks_hi_reg = fecfc1__uncorrblockscounterhi;
              blk_lk_reg = fecfc1__sts;
              break;
            case 2:
              corr_blks_lo_reg = fecfc2__corrblockscounterlo;
              corr_blks_hi_reg = fecfc2__corrblockscounterhi;
              uncorr_blks_lo_reg = fecfc2__uncorrblockscounterlo;
              uncorr_blks_hi_reg = fecfc2__uncorrblockscounterhi;
              blk_lk_reg = fecfc2__sts;
              break;
            case 3:
              corr_blks_lo_reg = fecfc3__corrblockscounterlo;
              corr_blks_hi_reg = fecfc3__corrblockscounterhi;
              uncorr_blks_lo_reg = fecfc3__uncorrblockscounterlo;
              uncorr_blks_hi_reg = fecfc3__uncorrblockscounterhi;
              blk_lk_reg = fecfc3__sts;
              break;
            default:
              return BF_INVALID_ARG;
          }
          break;
        case 2:
          switch (vl) {
            case 0:
              corr_blks_lo_reg = fecfc0__corrblockscounterlovl1;
              corr_blks_hi_reg = fecfc0__corrblockscounterhivl1;
              uncorr_blks_lo_reg = fecfc0__uncorrblockscounterlovl1;
              uncorr_blks_hi_reg = fecfc0__uncorrblockscounterhivl1;
              blk_lk_reg = fecfc0__stsvl1;
              break;
            case 1:
              corr_blks_lo_reg = fecfc1__corrblockscounterlovl1;
              corr_blks_hi_reg = fecfc1__corrblockscounterhivl1;
              uncorr_blks_lo_reg = fecfc1__uncorrblockscounterlovl1;
              uncorr_blks_hi_reg = fecfc1__uncorrblockscounterhivl1;
              blk_lk_reg = fecfc1__stsvl1;
              break;
            case 2:
              corr_blks_lo_reg = fecfc2__corrblockscounterlovl1;
              corr_blks_hi_reg = fecfc2__corrblockscounterhivl1;
              uncorr_blks_lo_reg = fecfc2__uncorrblockscounterlovl1;
              uncorr_blks_hi_reg = fecfc2__uncorrblockscounterhivl1;
              blk_lk_reg = fecfc2__stsvl1;
              break;
            case 3:
              corr_blks_lo_reg = fecfc3__corrblockscounterlovl1;
              corr_blks_hi_reg = fecfc3__corrblockscounterhivl1;
              uncorr_blks_lo_reg = fecfc3__uncorrblockscounterlovl1;
              uncorr_blks_hi_reg = fecfc3__uncorrblockscounterhivl1;
              blk_lk_reg = fecfc3__stsvl1;
              break;
            default:
              return BF_INVALID_ARG;
          }
          break;
        default:
          return BF_INVALID_ARG;
      }
      break;
    case CMRA_CH_SPD_1G:  // FC FEC not supported
    case CMRA_CH_SPD_100G:
    default:
      return BF_INVALID_ARG;
  }
  // get requested FEC status/counter values
  data = mac_read(dev_id, mac_blk, blk_lk_reg);
  if (block_lock_status) {
    *block_lock_status = get_fld_fecfc0__sts__fcblocklock(&data);
  }
  if (fec_corr_blk_cnt) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, corr_blks_lo_reg);
    cnt_lo = get_fld_fecfc0__corrblockscounterlo__corrblockscounter(&data);
    data = mac_read(dev_id, mac_blk, corr_blks_hi_reg);
    cnt_hi = get_fld_fecfc0__corrblockscounterhi__corrblockscounter(&data);
    *fec_corr_blk_cnt = (cnt_hi << 16) | cnt_lo;
  }
  if (fec_uncorr_blk_cnt) {
    uint32_t cnt_lo, cnt_hi;

    data = mac_read(dev_id, mac_blk, uncorr_blks_lo_reg);
    cnt_lo = get_fld_fecfc0__uncorrblockscounterlo__uncorrblockscounter(&data);
    data = mac_read(dev_id, mac_blk, uncorr_blks_hi_reg);
    cnt_hi = get_fld_fecfc0__uncorrblockscounterhi__uncorrblockscounter(&data);
    *fec_uncorr_blk_cnt = (cnt_hi << 16) | cnt_lo;
  }
  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_set_tx_enable
 *
 ****************************************************************************/
void port_mgr_mac_set_tx_enable(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                bool tx_en) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__ctrl + ch * 0x100);
  set_fld_mcmac0__ctrl__txenable(&data, tx_en ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__ctrl + ch * 0x100, data);
  port_mgr_log("MAC: %d:%2d:%d :  set tx enable = %s",
               dev_id,
               mac_blk,
               ch,
               tx_en ? "true" : "false");
}

/****************************************************************************
 * port_mgr_mac_set_rx_enable
 *
 ****************************************************************************/
void port_mgr_mac_set_rx_enable(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                bool rx_en) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__ctrl + ch * 0x100);
  set_fld_mcmac0__ctrl__rxenable(&data, rx_en ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__ctrl + ch * 0x100, data);
  port_mgr_log("MAC: %d:%2d:%d :  set rx enable = %s",
               dev_id,
               mac_blk,
               ch,
               rx_en ? "true" : "false");
}

/****************************************************************************
 * port_mgr_mac_set_rtestmode
 *
 ****************************************************************************/
void port_mgr_mac_set_rtestmode(bf_dev_id_t dev_id,
                                bf_dev_port_t dev_port,
                                bool rtestmode) {
  int mac_blk, ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  // set rtestmode. This forces PCS to indicate FAULT to
  // the MACxa to ensure glb.livelinkstate is set properly.
  mac_write(dev_id,
            mac_blk,
            hsmcpcs0__testpatctrl + ch * 0x100,
            rtestmode ? 4 /*rtestmode*/ : 0);
}

/****************************************************************************
 * port_mgr_mac_set_force_local_fault
 *
 ****************************************************************************/
void port_mgr_mac_set_force_local_fault(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        bool force_val) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100);
  set_fld_mcmac0__txdebug__txlfault(&data, force_val ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_set_force_remote_fault
 *
 ****************************************************************************/
void port_mgr_mac_set_force_remote_fault(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         bool force_val) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100);
  set_fld_mcmac0__txdebug__txrfault(&data, force_val ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_set_force_idle
 *
 ****************************************************************************/
void port_mgr_mac_set_force_idle(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 bool force_val) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100);
  set_fld_mcmac0__txdebug__txidle(&data, force_val ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_set_tx_auto_drain_on_disable
 *
 ****************************************************************************/
void port_mgr_mac_set_tx_auto_drain_on_disable(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               bool force_val) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100);
  set_fld_mcmac0__txdebug__txautodraintxdisable(&data, force_val ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__txdebug + ch * 0x100, data);
}

/****************************************************************************
 * port_mgr_mac_get_lf_rf_interrupts
 *
 ****************************************************************************/
void port_mgr_mac_get_lf_rf_interrupts(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       bool *latched_lf,
                                       bool *latched_rf) {
  int mac_block;
  int ch;
  uint32_t reg;
  uint32_t data;
  uint32_t sta_reg[4] = {interrupts0__intstat3,
                         interrupts0__intstat4,
                         interrupts0__intstat5,
                         interrupts0__intstat6};
  uint32_t clr_reg[4] = {interrupts0__intclr3,
                         interrupts0__intclr4,
                         interrupts0__intclr5,
                         interrupts0__intclr6};

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  reg = sta_reg[ch];
  data = mac_read(dev_id, mac_block, reg);

  // return latched int status
  if (latched_lf) *latched_lf = ((data >> 4) & 1) ? true : false;
  if (latched_rf) *latched_rf = ((data >> 5) & 1) ? true : false;

  // now clear
  reg = clr_reg[ch];
  mac_write(dev_id, mac_block, reg, 0x30);
  return;
}

/****************************************************************************
 * port_mgr_mac_get_rs_fec_status_and_counters
 *
 ****************************************************************************/
void port_mgr_mac_get_pcs_counters(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t *ber_cnt,
                                   uint32_t *errored_blk_cnt,
                                   uint32_t *sync_loss,
                                   uint32_t *block_lock_loss,
                                   uint32_t *hi_ber_cnt,
                                   uint32_t *valid_error_cnt,
                                   uint32_t *unknown_error_cnt,
                                   uint32_t *invalid_error_cnt,
                                   uint32_t *bip_errors_per_pcs_lane) {
  int mac_blk, ch;
  uint32_t data, sts2;
  port_mgr_port_t *port_p = NULL;
  port_mgr_pcs_ctrs_t *pcs_p = NULL;

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p) return;
  pcs_p = &port_p->pcs_ctrs;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  // get requested FEC status/counter values

  // note: latches the bercounthi and errorblockshi regs
  sts2 = mac_read(dev_id, mac_blk, hsmcpcs0__sts2 + ch * 0x100);
  if (ber_cnt) {
    uint32_t ber_lo, ber_hi;

    ber_lo = get_fld_hsmcpcs0__sts2__bercount(&sts2);

    data = mac_read(dev_id, mac_blk, hsmcpcs0__bercounthi + ch * 0x100);
    ber_hi = get_fld_hsmcpcs0__bercounthi__bercount(&data);
    // jira drv-708. Fix shitf count for high order bits
    *ber_cnt = (ber_hi << 6) | ber_lo;
    if (*ber_cnt) {
      // add it to pcs cummulative counters
      pcs_p->bercount = pcs_p->bercount + *ber_cnt;
      if (pcs_p->bercount >= (1u << 31)) pcs_p->bercount = (1u << 31) - 1;
    }
  }
  if (errored_blk_cnt) {
    uint32_t err_blk_lo, err_blk_hi;

    err_blk_lo = get_fld_hsmcpcs0__sts2__erroredblocks(&sts2);

    data = mac_read(dev_id, mac_blk, hsmcpcs0__erroredblockshi + ch * 0x100);
    err_blk_hi = get_fld_hsmcpcs0__erroredblockshi__erroredblocks(&data);
    *errored_blk_cnt = (err_blk_hi << 8) | err_blk_lo;
    if (*errored_blk_cnt) {
      // add it to pcs cummulative counters
      pcs_p->block_err = pcs_p->block_err + *errored_blk_cnt;
      if (pcs_p->block_err >= (1u << 31)) pcs_p->block_err = (1u << 31) - 1;
    }
  }

  data = mac_read(dev_id, mac_blk, hsmcpcs0__counters0 + ch * 0x100);
  if (sync_loss) {
    *sync_loss = get_fld_hsmcpcs0__counters0__syncloss(&data);
  }
  if (block_lock_loss) {
    *block_lock_loss = get_fld_hsmcpcs0__counters0__blocklockloss(&data);
  }

  data = mac_read(dev_id, mac_blk, hsmcpcs0__counters1 + ch * 0x100);
  if (hi_ber_cnt) {
    *hi_ber_cnt = get_fld_hsmcpcs0__counters1__highber(&data);
  }
  if (valid_error_cnt) {
    *valid_error_cnt = get_fld_hsmcpcs0__counters1__vlderr(&data);
  }

  data = mac_read(dev_id, mac_blk, hsmcpcs0__counters2 + ch * 0x100);
  if (unknown_error_cnt) {
    *unknown_error_cnt = get_fld_hsmcpcs0__counters2__unkerr(&data);
  }
  if (invalid_error_cnt) {
    *invalid_error_cnt = get_fld_hsmcpcs0__counters2__invlderr(&data);
  }
  if (bip_errors_per_pcs_lane) {
    int ln;

    for (ln = 0; ln < 20; ln++) {
      data =
          mac_read(dev_id, mac_blk, hsmcpcs0__biperrorsln0 + ln + ch * 0x100);
      bip_errors_per_pcs_lane[ln] =
          get_fld_hsmcpcs0__biperrorsln0__biperrorsln0(&data);
    }
  }
}

/****************************************************************************
 * port_mgr_mac_get_pcs_cumulative_counters
 *
 ****************************************************************************/
void port_mgr_mac_get_pcs_cumulative_counters(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t *ber_cnt,
                                              uint32_t *errored_blk_cnt) {
  port_mgr_port_t *port_p;

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p) return;

  /* If FEC enabled, just return 0 */
  if (port_p->sw.fec != BF_FEC_TYP_NONE) {
    if (ber_cnt) *ber_cnt = 0;
    if (errored_blk_cnt) *errored_blk_cnt = 0;
    return;
  }

  if (port_p->sw.oper_state && !ber_cnt && !errored_blk_cnt) {
    port_mgr_mac_get_pcs_counters(dev_id,
                                  dev_port,
                                  ber_cnt,
                                  errored_blk_cnt,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);
    return;
  }

  if (ber_cnt) *ber_cnt = port_p->pcs_ctrs.bercount;
  if (errored_blk_cnt) *errored_blk_cnt = port_p->pcs_ctrs.block_err;
}

/****************************************************************************
 * port_mgr_mac_clr_pcs_cumulative_counters
 *
 ****************************************************************************/
void port_mgr_mac_clr_pcs_cumulative_counters(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port) {
  uint32_t dummy_errored_blk_cnt;
  port_mgr_port_t *port_p;
  uint32_t dummy_ber_cnt;

  port_p = port_mgr_map_dev_port_to_port(dev_id, dev_port);
  if (!port_p) return;

  /* Read (to clear) physical counters only if port is UP */
  if (port_p->sw.oper_state) {
    port_mgr_mac_get_pcs_counters(dev_id,
                                  dev_port,
                                  &dummy_ber_cnt,
                                  &dummy_errored_blk_cnt,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);
  }

  port_p->pcs_ctrs.bercount = 0;
  port_p->pcs_ctrs.block_err = 0;
}

int port_mgr_mac_wait_for_stats_not_bz(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       int tmout) {
  int mac_blk, ch;
  uint32_t data, bz;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, stats0__rdctrl);
  bz = get_fld_stats0__rdctrl__rsc(&data);
  while (bz && (tmout-- >= 0)) {
    data = mac_read(dev_id, mac_blk, stats0__rdctrl);
    bz = get_fld_stats0__rdctrl__rsc(&data);
  }
  return bz;
}

/****************************************************************************
 * port_mgr_mac_read_counter
 *
 ****************************************************************************/
int port_mgr_mac_read_counter(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              int ctr,
                              uint64_t *ctr_value) {
  int mac_blk, ch;
  uint32_t data, bz;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  bz = port_mgr_mac_wait_for_stats_not_bz(dev_id, dev_port, 100);
  if (bz) return -1;  // ctr not read

  data = mac_read(dev_id, mac_blk, stats0__rdctrl);
  bz = get_fld_stats0__rdctrl__rsc(&data);
  if (!bz) {
    uint64_t b16[4] = {0};
    uint64_t val;

    data = 0;
    set_fld_stats0__rdctrl__cntrnum(&data, ctr);
    set_fld_stats0__rdctrl__channum(&data, ch);
    set_fld_stats0__rdctrl__rsc(&data, 1);
    mac_write(dev_id, mac_blk, stats0__rdctrl, data);

    bz = port_mgr_mac_wait_for_stats_not_bz(dev_id, dev_port, 100);
    if (bz) return -1;  // ctr not read

    b16[0] = mac_read(dev_id, mac_blk, stats0__rdata0);
    b16[1] = mac_read(dev_id, mac_blk, stats0__rdata1);
    b16[2] = mac_read(dev_id, mac_blk, stats0__rdata2);
    b16[3] = mac_read(dev_id, mac_blk, stats0__rdata3);
    val = (b16[3] << 48ull) | (b16[2] << 32ull) | (b16[1] << 16ull) |
          (b16[0] << 0ull);
    *ctr_value = val;
    return 0;
  }
  return -1;  // busy, try again later
}

/****************************************************************************
 * port_mgr_mac_get_glb_mode
 *
 ****************************************************************************/
uint32_t port_mgr_mac_get_glb_mode(bf_dev_id_t dev_id, bf_dev_port_t dev_port)

{
  int mac_blk, ch;
  uint32_t data;
  uint32_t mode_reg[4] = {
      glbl__ch0mode, glbl__ch1mode, glbl__ch2mode, glbl__ch3mode};

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mode_reg[ch]);
  return (data);
}

/****************************************************************************
 * port_mgr_mac_get_slot2ch_map
 *
 ****************************************************************************/
uint32_t port_mgr_mac_get_slot2ch_map(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port)

{
  int mac_blk, ch;
  uint32_t data;
  uint32_t data2;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, fifoctrl0__chmap0);
  data2 = mac_read(dev_id, mac_blk, fifoctrl0__chmap1);

  return ((data | (data2 << 16)));
}

/****************************************************************************
 * port_mgr_mac_set_lane_remap
 *
 ****************************************************************************/
void port_mgr_mac_set_lane_remap(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 int rx_ln0,
                                 int rx_ln1,
                                 int rx_ln2,
                                 int rx_ln3,
                                 int tx_ln0,
                                 int tx_ln1,
                                 int tx_ln2,
                                 int tx_ln3) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = 0;
  set_fld_serdesmux__laneremaptx0__remap0(&data, tx_ln0);
  set_fld_serdesmux__laneremaptx0__remap1(&data, tx_ln1);
  set_fld_serdesmux__laneremaptx0__remap2(&data, tx_ln2);
  set_fld_serdesmux__laneremaptx0__remap3(&data, tx_ln3);
  mac_write(dev_id, mac_blk, serdesmux__laneremaptx0, data);

  data = 0;
  set_fld_serdesmux__laneremaprx0__remap0(&data, rx_ln0);
  set_fld_serdesmux__laneremaprx0__remap1(&data, rx_ln1);
  set_fld_serdesmux__laneremaprx0__remap2(&data, rx_ln2);
  set_fld_serdesmux__laneremaprx0__remap3(&data, rx_ln3);
  mac_write(dev_id, mac_blk, serdesmux__laneremaprx0, data);
}

/****************************************************************************
 * port_mgr_mac_get_lane_remap
 *
 ****************************************************************************/
void port_mgr_mac_get_lane_remap(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 int *rx_ln0,
                                 int *rx_ln1,
                                 int *rx_ln2,
                                 int *rx_ln3,
                                 int *tx_ln0,
                                 int *tx_ln1,
                                 int *tx_ln2,
                                 int *tx_ln3) {
  int mac_blk, ch;
  uint32_t data;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, serdesmux__laneremaptx0);
  *tx_ln0 = get_fld_serdesmux__laneremaptx0__remap0(&data);
  *tx_ln1 = get_fld_serdesmux__laneremaptx0__remap1(&data);
  *tx_ln2 = get_fld_serdesmux__laneremaptx0__remap2(&data);
  *tx_ln3 = get_fld_serdesmux__laneremaptx0__remap3(&data);

  data = mac_read(dev_id, mac_blk, serdesmux__laneremaprx0);
  *rx_ln0 = get_fld_serdesmux__laneremaprx0__remap0(&data);
  *rx_ln1 = get_fld_serdesmux__laneremaprx0__remap1(&data);
  *rx_ln2 = get_fld_serdesmux__laneremaprx0__remap2(&data);
  *rx_ln3 = get_fld_serdesmux__laneremaprx0__remap3(&data);
}

/********************************************************************
 * port_mgr_mac_kr_backchannel_mux_set
 *
 ********************************************************************/
int port_mgr_mac_kr_backchannel_mux_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int tx_ln,
                                        int rx_ln) {
  uint32_t addr, data;
  int mac_block, ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.ethsds_core_cntl[ch]);
  lld_read_register(dev_id, addr, &data);

  // mux is bits [11:8]
  // tx [11:10]
  // rx [9:8]
  data &= ~(0xF << 8);
  data |= ((tx_ln & 3) << 8);
  data |= ((rx_ln & 3) << 10);
  lld_write_register(dev_id, addr, data);

  return 0;
}

#define COMIRA_REG_STR(name, ofs) {#name, ofs},

typedef struct comira_reg_str_t {
  char *name;
  uint32_t ofs;
} comira_reg_str_t;

comira_reg_str_t comira_reg_strs[] = {
#include "comira_reg_strs.h"
};

char *comira_reg_to_str(uint32_t offset) {
  uint32_t i;

  for (i = 0; i < sizeof(comira_reg_strs) / sizeof(comira_reg_strs[0]); i++) {
    if (comira_reg_strs[i].ofs == offset) {
      return comira_reg_strs[i].name;
    }
  }
  return "<un-named>";
}

bf_status_t port_mgr_mac_min_thr_set(bf_dev_id_t dev_id,
                                     bf_mac_block_id_t mac_block,
                                     uint32_t min_thr_value_25g,
                                     uint32_t min_thr_value_50g,
                                     uint32_t min_thr_value_100g) {
  uint32_t addr, data;
  ;
  port_mgr_mac_block_t *mac_block_p;

  mac_block_p = port_mgr_tof1_map_idx_to_mac_block(dev_id, mac_block);
  if (mac_block_p == NULL) return BF_INVALID_ARG;

  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.min_thr);
  data = 0;

  setp_eth_regs_min_thr_thr_25g(&data, min_thr_value_25g);
  setp_eth_regs_min_thr_thr_50g(&data, min_thr_value_50g);
  setp_eth_regs_min_thr_thr_100g(&data, min_thr_value_100g);

  lld_write_register(dev_id, addr, data);

  return BF_SUCCESS;
}

/* Time delay offset to be added for various MAC blocks.
 * This delay value remains same for all Tofino skew parts.
 */
static uint8_t mac_tofino_tstamp_offset[65] = {
    2,  3,  5,  6,  8,  9,  11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 32, 33, 35, 36, 38, 39, 41, 42, 46, 47,
    49, 50, 52, 53, 55, 56, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
    69, 70, 71, 72, 73, 76, 77, 79, 80, 82, 83, 85, 86, 44};

bf_status_t port_mgr_mac_block_init(bf_dev_id_t dev_id) {
  int mac_block = 0;
  uint32_t addr0, addr1;
  uint64_t clock_speed, bps_clock_speed;
  float clock_speed_in_ns;
  bf_status_t status;
  uint32_t ts_offset_ctrl;

  status = bf_drv_get_clock_speed(dev_id, &bps_clock_speed, &clock_speed);
  if (status != BF_SUCCESS) {
    port_mgr_log_error("%s: getting clock speed failed, status = %s (%d)",
                       __func__,
                       bf_err_str(status),
                       status);
    return status;
  }

  clock_speed_in_ns =
      1.0 / clock_speed * 1000000000ul;  //( 1sec / clock_speed_Hz * ns_in_sec)
  port_mgr_log_trace(
      "%s: core clock speed in ns = %f", __func__, clock_speed_in_ns);

  for (mac_block = 0; mac_block < lld_get_max_mac_blocks(dev_id); mac_block++) {
    port_mgr_mac_min_thr_set(dev_id, (bf_mac_block_id_t)mac_block, 4, 4, 4);
    addr0 = offsetof(
        Tofino,
        macs_t[mac_block]
            .macs.eth_regs.eth_mac_ts_offset_ctrl.eth_mac_ts_offset_ctrl_0_2);
    addr1 = offsetof(
        Tofino,
        macs_t[mac_block]
            .macs.eth_regs.eth_mac_ts_offset_ctrl.eth_mac_ts_offset_ctrl_1_2);
    lld_write_register(dev_id, addr1, (0x11c7 << 16));  // write ts_incr

    /*
     * The mac_tofino_tstamp_offset is number of core clocks. This needs to
     * be multiplied with core clock speed in 'ns' and rounded down to nearest
     * integer for HW programming.
     */
    ts_offset_ctrl =
        (uint32_t)(mac_tofino_tstamp_offset[mac_block] * clock_speed_in_ns);
    lld_write_register(dev_id, addr0, ts_offset_ctrl);
  }

  return BF_SUCCESS;
}

/****************************************************************************
 * port_mgr_mac_cfg_txfifo_ctrl
 *
 ****************************************************************************/
void port_mgr_mac_cfg_txfifo_ctrl(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  uint32_t val) {
  int mac_blk, ch;
  uint32_t ctrl_reg[4] = {fifoctrl0__txfifoctrl0,
                          fifoctrl0__txfifoctrl1,
                          fifoctrl0__txfifoctrl2,
                          fifoctrl0__txfifoctrl3};

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  mac_write(dev_id, mac_blk, ctrl_reg[ch], val);
  return;
}

/****************************************************************************
 * port_mgr_mac_cfg_txfifo_ctrl
 *
 ****************************************************************************/
void port_mgr_mac_cfg_fifo_ctrl1(bf_dev_id_t dev_id,
                                 bf_dev_port_t dev_port,
                                 uint32_t aful_1ch_mode,
                                 uint32_t aful_2ch_mode,
                                 uint32_t aful_4ch_mode) {
  int mac_blk, ch;
  uint32_t data, ctrl1 = fifoctrl0__ctrl1;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, ctrl1);
  set_fld_fifoctrl0__ctrl1__txfullthres4ch(&data, aful_4ch_mode);
  set_fld_fifoctrl0__ctrl1__txfullthres2ch(&data, aful_2ch_mode);
  set_fld_fifoctrl0__ctrl1__txfullthres1ch(&data, aful_1ch_mode);

  mac_write(dev_id, mac_blk, ctrl1, data);
  return;
}

/****************************************************************************
 * port_mgr_mac_stats_clear
 *
 ****************************************************************************/
void port_mgr_mac_stats_clear(bf_dev_id_t dev_id, bf_dev_port_t dev_port) {
  int mac_blk, ch;
  uint32_t val;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  val = mac_read(dev_id, mac_blk, stats0__swreset);

  switch (ch) {
    case 0:
      set_fld_stats0__swreset__swreset0(&val, 1);
      break;
    case 1:
      set_fld_stats0__swreset__swreset1(&val, 1);
      break;
    case 2:
      set_fld_stats0__swreset__swreset2(&val, 1);
      break;
    case 3:
      set_fld_stats0__swreset__swreset3(&val, 1);
      break;
  }
  // assert swreset for the channel
  mac_write(dev_id, mac_blk, stats0__swreset, val);

  switch (ch) {
    case 0:
      set_fld_stats0__swreset__swreset0(&val, 0);
      break;
    case 1:
      set_fld_stats0__swreset__swreset1(&val, 0);
      break;
    case 2:
      set_fld_stats0__swreset__swreset2(&val, 0);
      break;
    case 3:
      set_fld_stats0__swreset__swreset3(&val, 0);
      break;
  }
  // de-assert swreset for the channel
  mac_write(dev_id, mac_blk, stats0__swreset, val);
  return;
}

void port_mgr_mac_local_fault_int_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bool en) {
  int mac_blk, ch;
  int int_en_reg_per_ch[] = {interrupts0__setintenable3,
                             interrupts0__setintenable4,
                             interrupts0__setintenable5,
                             interrupts0__setintenable6};
  int int_dis_reg_per_ch[] = {interrupts0__clrintenable3,
                              interrupts0__clrintenable4,
                              interrupts0__clrintenable5,
                              interrupts0__clrintenable6};
  int int_clr_reg_per_ch[] = {interrupts0__intclr3,
                              interrupts0__intclr4,
                              interrupts0__intclr5,
                              interrupts0__intclr6};

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  if (en) {
    // int enable
    mac_write(dev_id, mac_blk, int_en_reg_per_ch[ch], 0x10);
  } else {
    // int disable
    mac_write(dev_id, mac_blk, int_dis_reg_per_ch[ch], 0x10);
    // and clear it
    mac_write(dev_id, mac_blk, int_clr_reg_per_ch[ch], 0x10);
  }

#if 0
  port_mgr_log("MAC: INT: %d:%2d:%d : dev_port=%x : LF %s",
                   dev_id,
                   mac_blk,
                   ch,
                   dev_port,
                   en ? "Enbl":"Dsbl");
#endif  // 0
}

void port_mgr_mac_remote_fault_int_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       bool en) {
  int mac_blk, ch;
  int int_en_reg_per_ch[] = {interrupts0__setintenable3,
                             interrupts0__setintenable4,
                             interrupts0__setintenable5,
                             interrupts0__setintenable6};
  int int_dis_reg_per_ch[] = {interrupts0__clrintenable3,
                              interrupts0__clrintenable4,
                              interrupts0__clrintenable5,
                              interrupts0__clrintenable6};
  int int_clr_reg_per_ch[] = {interrupts0__intclr3,
                              interrupts0__intclr4,
                              interrupts0__intclr5,
                              interrupts0__intclr6};

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  if (en) {
    // int enable
    mac_write(dev_id, mac_blk, int_en_reg_per_ch[ch], 0x20);
  } else {
    // int disable
    mac_write(dev_id, mac_blk, int_dis_reg_per_ch[ch], 0x20);
    // and clear it
    mac_write(dev_id, mac_blk, int_clr_reg_per_ch[ch], 0x20);
  }

#if 0
  port_mgr_log("MAC: INT: %d:%2d:%d : dev_port=%x : RF %s",
                   dev_id,
                   mac_blk,
                   ch,
                   dev_port,
                   en ? "Enbl":"Dsbl");
#endif  // 0
}

/********************************************************************
 * port_mgr_mac_int_en_get
 *
 ********************************************************************/
void port_mgr_mac_int_en_get(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             bool *en) {
  uint32_t addr, data;
  int mac_block, ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.int_en);
  lld_read_register(dev_id, addr, &data);
  *en = ((data >> 20) & 1) ? true : false;
}

/********************************************************************
 * port_mgr_mac_int_en_set
 *
 ********************************************************************/
void port_mgr_mac_int_en_set(bf_dev_id_t dev_id,
                             bf_dev_port_t dev_port,
                             bool en) {
  uint32_t addr, data;
  int mac_block, ch;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  addr = offsetof(Tofino, macs_t[mac_block].macs.eth_regs.int_en);
  lld_read_register(dev_id, addr, &data);
  data = (data & ~(1 << 20)) | (en ? (1 << 20) : 0);
  lld_write_register(dev_id, addr, data);
  return;
}

/********************************************************************
 * port_mgr_mac_sigovrd_set
 *
 ********************************************************************/
void port_mgr_mac_sigovrd_set(bf_dev_id_t dev_id,
                              bf_dev_port_t dev_port,
                              int num_lanes,
                              bf_sigovrd_fld_t sig_ovrd_val) {
  uint32_t r_data, w_data = 0;
  int mac_block = 0, ch = 0, ln = 0, lane = 0;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_block, &ch, NULL);

  r_data = mac_read(dev_id, mac_block, serdesmux__laneremaprx0);
  w_data = mac_read(dev_id, mac_block, serdesmux__serdessigokovrd);

  for (ln = 0; ln < num_lanes; ln++) {
    switch (ch + ln) {
      case 0:
        lane = get_fld_serdesmux__laneremaprx0__remap0(&r_data);
        break;
      case 1:
        lane = get_fld_serdesmux__laneremaprx0__remap1(&r_data);
        break;
      case 2:
        lane = get_fld_serdesmux__laneremaprx0__remap2(&r_data);
        break;
      case 3:
        lane = get_fld_serdesmux__laneremaprx0__remap3(&r_data);
        break;
      default:
        bf_sys_assert(0);
    }

    switch (lane) {
      case 0:
        set_fld_serdesmux__serdessigokovrd__chsigstatovrd0(&w_data,
                                                           sig_ovrd_val);
        break;
      case 1:
        set_fld_serdesmux__serdessigokovrd__chsigstatovrd1(&w_data,
                                                           sig_ovrd_val);
        break;
      case 2:
        set_fld_serdesmux__serdessigokovrd__chsigstatovrd2(&w_data,
                                                           sig_ovrd_val);
        break;
      case 3:
        set_fld_serdesmux__serdessigokovrd__chsigstatovrd3(&w_data,
                                                           sig_ovrd_val);
        break;
      default:
        bf_sys_assert(0);
    }
  }
  mac_write(dev_id, mac_block, serdesmux__serdessigokovrd, w_data);
}

/****************************************************************************
 * port_mgr_mac_tx_ignore_rx_set
 *
 ****************************************************************************/
bf_status_t port_mgr_mac_tx_ignore_rx_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          bool en) {
  int mac_blk, ch;
  uint32_t data = 0;

  port_mgr_map_dev_port_to_all(
      dev_id, dev_port, NULL, NULL, &mac_blk, &ch, NULL);

  data = mac_read(dev_id, mac_blk, mcmac0__ctrl + ch * 0x100);
  set_fld_mcmac0__ctrl__faultovrd(&data, en ? 1 : 0);
  mac_write(dev_id, mac_blk, mcmac0__ctrl + ch * 0x100, data);
  return 0;
}
