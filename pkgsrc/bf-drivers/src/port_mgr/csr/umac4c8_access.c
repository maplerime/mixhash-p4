/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include "umac4c8_fld_access.h"

#include <bf_types/bf_types.h>
#include "autogen-required-headers.h"

/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : mode
*  access  : read-write
*----------+
*
*Channel Mode/Speed - Controls the speed,   phy type,   and fec options of a channel. (Not all modes are valid in all channels)
*  6'd00 : Disabled (Channel is shut down and held in reset)
*  6'd01 - 14: Reserved
*  6'd15 : 10GBASE-R
*  6'd16 : 10GBASE-R + FCFEC
*  6'd17 - 20: Reserved
*  6'd21 : 25GBASE-R1
*  6'd22 : 25GBASE-R1 + FCFEC
*  6'd23 : 25GBASE-R1 + RSFEC
*  6'd24 : 25GBASE-R1 + RSFEC r1.5
*  6'd25 : 25GBASE-R1 + RSFEC r1.6
*  6'd26 : 40GBASE-R4
*  6'd27 : 40GBASE-R4 + FCFEC
*  6'd28 - 36: Reserved
*  6'd37 : 50GBASE-R2
*  6'd38 : 50GBASE-R2 + FCFEC
*  6'd39 : 50GBASE-R2 + RSFEC
*  6'd40 - 42: Reserved
*  6'd43 : 50GBASE-R1 + RSFEC (KP)
*  6'd44 - 45: Reserved
*  6'd46 : 100GBASE-R4
*  6'd47 : 100GBASE-R4 + RSFEC
*  6'd48 - 49: Reserved
*  6'd50 : 100GBASE-R2 + RSFEC (KP)
*  6'd51 : 100GBASE-R1 + RSFEC (KP)
*  6'd52 : 200GBASE-R8 + RSFEC (KP)
*  6'd53 : 200GBASE-R4 + RSFEC (KP)
*  6'd54 : 200GBASE-R2 + RSFEC (KP)
*  6'd55 : Reserved
*  6'd56 : 400GBASE-R8 + RSFEC (KP)
*  6'd57 : 400GBASE-R4 + RSFEC (KP)
*  6'd58 - 6'd63 : Reserved""
*******************************************************************/
void umac4_chmode0__chmode__mode_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__mode(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__mode_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__mode(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__mode_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__mode_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__mode_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : txswrst
*  access  : read-write
*----------+
*
* 1'b1 TX Reset Active 
* 1'b0 TX Normal Operation""
*******************************************************************/
void umac4_chmode0__chmode__txswrst_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__txswrst(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__txswrst_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__txswrst(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__txswrst_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__txswrst_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txswrst_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : rxswrst
*  access  : read-write
*----------+
*
* 1'b1 RX Reset Active 
* 1'b0 RX Normal Operation""
*******************************************************************/
void umac4_chmode0__chmode__rxswrst_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__rxswrst(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__rxswrst_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__rxswrst(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__rxswrst_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__rxswrst_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : txen
*  access  : read-write
*----------+
*
* 1'b1 TX Normal Operation
* 1'b0 TX Channel Disabled""
*******************************************************************/
void umac4_chmode0__chmode__txen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__txen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__txen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__txen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__txen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__txen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : txdrain
*  access  : read-write
*----------+
*
* Setting this bit causes transmit path to enter into Drain mode where
* all the data from the TXFIFO is drained out.""
* 
*******************************************************************/
void umac4_chmode0__chmode__txdrain_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__txdrain(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__txdrain_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__txdrain(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__txdrain_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__txdrain_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txdrain_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : rxen
*  access  : read-write
*----------+
*
* 1'b1 RX Normal Operation
* 1'b0 RX Channel Disabled""
*******************************************************************/
void umac4_chmode0__chmode__rxen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__rxen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__rxen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__rxen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__rxen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__rxen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__rxen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : gmiilpbk
*  access  : read-write
*----------+
*
*Setting this bit enables the Loopback on the MAC-PCS Interface for
* this Channel. The Transmit data and controls are looped back on to the
* receive MAC module for this Channel. When this loopback is enabled,
* the data received from the PCS for this channel is ignored.""
* 
*******************************************************************/
void umac4_chmode0__chmode__gmiilpbk_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__gmiilpbk(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__gmiilpbk_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__gmiilpbk(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__gmiilpbk_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__gmiilpbk_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__gmiilpbk_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : txjabber
*  access  : read-write
*----------+
*
*This field determines the Jabber Size for the outgoing (Transmit)
* frames on this Channel. When the length of the current outgoing frame
* on this Channel exceeds the value programmed in this field,  the Frame
* is considered a Jabber Frame and is truncated at that point with EOF-
* ERROR. This will limit the frame transmission run-off in case of
* Application logic error.""
* 
*******************************************************************/
void umac4_chmode0__chmode__txjabber_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__txjabber(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__txjabber_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__txjabber(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__txjabber_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__txjabber_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txjabber_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : rxjabber
*  access  : read-write
*----------+
*
*This field determines the Jabber Size for the incoming (Receive)
* frames on this Channel. When the length of the current incoming frame
* on this Channel equals or exceeds the value programmed in this field,
* the Frame is considered a Jabber Frame and is truncated at that point.
* The Frame Status is updated with a Jabber Error and the rest of the
* Jabbered frame that is being received is ignored.""
* 
*******************************************************************/
void umac4_chmode0__chmode__rxjabber_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__rxjabber(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__rxjabber_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__rxjabber(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__rxjabber_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__rxjabber_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__rxjabber_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : disfcs
*  access  : read-write
*----------+
*
* Disables the Transmit FCS Insertion""
* 
*******************************************************************/
void umac4_chmode0__chmode__disfcs_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__disfcs(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__disfcs_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__disfcs(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__disfcs_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__disfcs_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__disfcs_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : invfcs
*  access  : read-write
*----------+
*
* Forces the inserted Transmit FCS to an inverted value to force an
* error in all conditions""
* 
*******************************************************************/
void umac4_chmode0__chmode__invfcs_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__invfcs(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__invfcs_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__invfcs(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__invfcs_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__invfcs_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__invfcs_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : ignfcs
*  access  : read-write
*----------+
*
* Ignore the FCS on received frames""
* 
*******************************************************************/
void umac4_chmode0__chmode__ignfcs_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__ignfcs(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__ignfcs_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__ignfcs(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__ignfcs_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__ignfcs_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__ignfcs_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : stripfcs
*  access  : read-write
*----------+
*
* Strips the FCS of received frames before they are forwarded to the
* application""
* 
*******************************************************************/
void umac4_chmode0__chmode__stripfcs_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__stripfcs(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__stripfcs_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__stripfcs(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__stripfcs_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__stripfcs_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__stripfcs_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : ifglen
*  access  : read-write
*----------+
*
* Selects the minimum IFG length in bytes inserted on transmit frames""
* 
*******************************************************************/
void umac4_chmode0__chmode__ifglen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__ifglen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__ifglen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__ifglen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__ifglen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__ifglen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__ifglen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chmode0
*  register: chmode
*  field   : ifgpacing
*  access  : read-write
*----------+
*
* IFG controls (reserved)""
* 
*******************************************************************/
void umac4_chmode0__chmode__ifgpacing_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chmode0__chmode__ifgpacing(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chmode0__chmode__ifgpacing_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chmode0__chmode__ifgpacing(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x0 + (channel * 0x200), *reg64);
  }
}

void umac4_chmode0__chmode__ifgpacing_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chmode0__chmode__ifgpacing_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__ifgpacing_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : disfcsonerr
*  access  : read-write
*----------+
*
*When Disable FCS is set,  this will include errored frames""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__disfcsonerr_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__disfcsonerr(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__disfcsonerr_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__disfcsonerr(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__disfcsonerr_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__disfcsonerr_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__disfcsonerr_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txfcen
*  access  : read-write
*----------+
*
* Enable Pause frame generation from XOFF pins""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txfcen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txfcen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txfcen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txfcen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txfcen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txfcen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txfcen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxfcen
*  access  : read-write
*----------+
*
*When set,  the UMAC Core is enabled for Flow-Control decode operation for this Channel and it will decode all the incoming frames for PAUSE Control Frames as specified in the IEEE 802.3 Specification.
* When reset,  this Channel does not decode the frames for PAUSE Control Frames.""
*******************************************************************/
void umac4_maccfg0__maccfg__rxfcen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxfcen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxfcen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxfcen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxfcen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxfcen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfcen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxpfcen
*  access  : read-write
*----------+
*
*When set,  the UMAC Core is enabled for Priority Flow-Control decode operation for this Channel.
* If the UMAC Core receives a valid Priority PAUSE Control Frame,  it will load the timers and provide the XOFF indication to the Application logic (ff_rxch0pfcxoff[7:0]) based on the Time Vector fields in the 8 priorities.
* When reset,  this Channel does not decode the frames for Priority PAUSE Control Frames.""
*******************************************************************/
void umac4_maccfg0__maccfg__rxpfcen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxpfcen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxpfcen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxpfcen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxpfcen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxpfcen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxpfcen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxfctotx
*  access  : read-write
*----------+
*
*If decoding of Flow-Control frames is enabled,  setting this bit will
* disable the transmission of user data frames for the time given in the
* PAUSE_TIME field of the PAUSE Control Frame when UMAC Core receives a
* valid PAUSE Control Frame.""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__rxfctotx_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxfctotx(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxfctotx_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxfctotx(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxfctotx_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxfctotx_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfctotx_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxfilterfc
*  access  : read-write
*----------+
*
*When this bit is set,  the UMAC will filter out the Pause frames from being sent to the Application Logic. 
* When this bit is reset,  the UMAC does not filter out the Pause frames .
* This bit has no effect on actual processing/decoding on the PAUSEControl frames,  which is controlled using the Enable Receive Flow Control Decode register bit.""
*******************************************************************/
void umac4_maccfg0__maccfg__rxfilterfc_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxfilterfc(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxfilterfc_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxfilterfc(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxfilterfc_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxfilterfc_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfilterfc_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxfilterpfc
*  access  : read-write
*----------+
*
*When this bit is set,  the UMAC will filter out the Priority Pause frames from being sent to the Application Logic. 
* When this bit is reset,  the UMAC does not filter out the Priority Pause frames 
* This bit has no effect on actual processing/decoding on the Priority PAUSE Control frames,  which is controlled using the Enable Priority Receive Flow Control Decode register bit.""
*******************************************************************/
void umac4_maccfg0__maccfg__rxfilterpfc_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxfilterpfc(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxfilterpfc_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxfilterpfc(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxfilterpfc_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxfilterpfc_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfilterpfc_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txpadrunt
*  access  : read-write
*----------+
*
* TX PAD length (set to 0 to disable)""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txpadrunt_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txpadrunt(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txpadrunt_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txpadrunt(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txpadrunt_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txpadrunt_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txpadrunt_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txwrthresh
*  access  : read-write
*----------+
*
* TX fifo write threshold. Set to 255-(delay between afull and vld)""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txwrthresh_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txwrthresh(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txwrthresh_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txwrthresh(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txwrthresh_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txwrthresh_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txwrthresh_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txrdthresh
*  access  : read-write
*----------+
*
* TX fifo read threshold""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txrdthresh_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txrdthresh(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txrdthresh_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txrdthresh(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txrdthresh_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txrdthresh_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txrdthresh_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txlfault
*  access  : read-write
*----------+
*
*When set,  the MAC continuously transmits local faults. Normal traffic
* is overriden and not paused""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txlfault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txlfault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txlfault_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txlfault(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txlfault_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txlfault_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txlfault_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txrfault
*  access  : read-write
*----------+
*
*When set,  the MAC continuously transmits remote faults. Normal
* traffic is overriden and not paused""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txrfault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txrfault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txrfault_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txrfault(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txrfault_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txrfault_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txrfault_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txidle
*  access  : read-write
*----------+
*
*When set,  the MAC continuously transmits idles. Normal traffic is
* overriden and not paused""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txidle_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txidle(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txidle_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txidle(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txidle_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txidle_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txidle_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxpadrunt
*  access  : read-write
*----------+
*
* Defines the MinFrame Size for padding in the Receive direction""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__rxpadrunt_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxpadrunt(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxpadrunt_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxpadrunt(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxpadrunt_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxpadrunt_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxpadrunt_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxlfault
*  access  : read-write
*----------+
*
*When set,  the MAC continuously receives L_FAULT ordered set.""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__rxlfault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxlfault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxlfault_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxlfault(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxlfault_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxlfault_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxlfault_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxrfault
*  access  : read-write
*----------+
*
*When set,  the MAC continuously receives R_FAULT ordered set.""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__rxrfault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxrfault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxrfault_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxrfault(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxrfault_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxrfault_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxrfault_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : rxidle
*  access  : read-write
*----------+
*
*When set,  the MAC continuously receives IDLE Pattern.""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__rxidle_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__rxidle(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__rxidle_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__rxidle(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__rxidle_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__rxidle_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxidle_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : statsclr
*  access  : read-write
*----------+
*
* Set to clear stats for mac channel""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__statsclr_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__statsclr(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__statsclr_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__statsclr(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__statsclr_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__statsclr_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__statsclr_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txignorerx
*  access  : read-write
*----------+
*
* Overrides the FAULT state recieved from the rx Reconciliation Layer""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txignorerx_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txignorerx(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txignorerx_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txignorerx(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txignorerx_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txignorerx_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txignorerx_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : maccfg0
*  register: maccfg
*  field   : txpfcen
*  access  : read-write
*----------+
*
* Enable PFC frame generation from XOFF pins (Mask per pin)""
* 
*******************************************************************/
void umac4_maccfg0__maccfg__txpfcen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_maccfg0__maccfg__txpfcen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_maccfg0__maccfg__txpfcen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_maccfg0__maccfg__txpfcen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x8 + (channel * 0x200), *reg64);
  }
}

void umac4_maccfg0__maccfg__txpfcen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_maccfg0__maccfg__txpfcen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txpfcen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig120
*  register: chconfig12
*  field   : txvlantag
*  access  : read-write
*----------+
*
* VLAN tag to identify frames for VLAN TX statistic counter""
* 
*******************************************************************/
void umac4_chconfig120__chconfig12__txvlantag_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig120__chconfig12__txvlantag(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig120__chconfig12__txvlantag_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig120__chconfig12__txvlantag(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig120__chconfig12__txvlantag_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig120__chconfig12__txvlantag_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig120__chconfig12__txvlantag_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig30
*  register: chconfig3
*  field   : ifgppm
*  access  : read-write
*----------+
*
* PPM adjustment of IFG by +/-base*2^-(10+exp)
* [15] = +/-
* [14:10] = exp
* [9:0]=base
* ""
*******************************************************************/
void umac4_chconfig30__chconfig3__ifgppm_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x18 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig30__chconfig3__ifgppm(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig30__chconfig3__ifgppm_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig30__chconfig3__ifgppm(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x18 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig30__chconfig3__ifgppm_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__ifgppm_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__ifgppm_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig30
*  register: chconfig3
*  field   : rxmaxfrmsize
*  access  : read-write
*----------+
*
* Max frame length before error is generated""
* 
*******************************************************************/
void umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x18 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig30__chconfig3__rxmaxfrmsize(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig30__chconfig3__rxmaxfrmsize_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig30__chconfig3__rxmaxfrmsize(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x18 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig30__chconfig3__rxmaxfrmsize_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__rxmaxfrmsize_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig30
*  register: chconfig3
*  field   : txpreamble
*  access  : read-write
*----------+
*
* TX frame preamble length (including /S/)""
* 
*******************************************************************/
void umac4_chconfig30__chconfig3__txpreamble_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x18 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig30__chconfig3__txpreamble(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig30__chconfig3__txpreamble_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig30__chconfig3__txpreamble(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x18 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig30__chconfig3__txpreamble_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__txpreamble_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__txpreamble_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig30
*  register: chconfig3
*  field   : txdrainonfault
*  access  : read-write
*----------+
*
* Setting this bit causes transmit path to enter into drain mode when
* the RX is in a fault state""
* 
*******************************************************************/
void umac4_chconfig30__chconfig3__txdrainonfault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x18 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig30__chconfig3__txdrainonfault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig30__chconfig3__txdrainonfault_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig30__chconfig3__txdrainonfault(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x18 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig30__chconfig3__txdrainonfault_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__txdrainonfault_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__txdrainonfault_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig30
*  register: chconfig3
*  field   : rxpreamble
*  access  : read-write
*----------+
*
*When set,  enable 4-byte preamble support,  otherwise only support
* 8-byte preamble.""
* 
*******************************************************************/
void umac4_chconfig30__chconfig3__rxpreamble_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x18 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig30__chconfig3__rxpreamble(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig30__chconfig3__rxpreamble_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig30__chconfig3__rxpreamble(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x18 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig30__chconfig3__rxpreamble_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__rxpreamble_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__rxpreamble_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig30
*  register: chconfig3
*  field   : rxerrmask
*  access  : read-write
*----------+
*
*Each bit corresponds to a type of error,  when set,  error is masked.
*[0]: CRC Error Mask
*[1]: Jabber Error Mask
*[2]: PCS Error Mask
*[3]: MaxFrameLenVio Error Mask
*[4]: Length Error Mask""
*******************************************************************/
void umac4_chconfig30__chconfig3__rxerrmask_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x18 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig30__chconfig3__rxerrmask(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig30__chconfig3__rxerrmask_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig30__chconfig3__rxerrmask(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x18 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig30__chconfig3__rxerrmask_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig30__chconfig3__rxerrmask_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__rxerrmask_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig40
*  register: chconfig4
*  field   : macaddr
*  access  : read-write
*----------+
*
* Mac address used for SA in TX pause frames and DA in RX pause
* frames""
* 
*******************************************************************/
void umac4_chconfig40__chconfig4__macaddr_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x20 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig40__chconfig4__macaddr(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig40__chconfig4__macaddr_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig40__chconfig4__macaddr(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x20 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig40__chconfig4__macaddr_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig40__chconfig4__macaddr_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig40__chconfig4__macaddr_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig40
*  register: chconfig4
*  field   : pauseontime
*  access  : read-write
*----------+
*
* Pause refresh time to send in pause fame""
* 
*******************************************************************/
void umac4_chconfig40__chconfig4__pauseontime_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x20 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig40__chconfig4__pauseontime(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig40__chconfig4__pauseontime_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig40__chconfig4__pauseontime(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x20 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig40__chconfig4__pauseontime_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig40__chconfig4__pauseontime_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig40__chconfig4__pauseontime_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig50
*  register: chconfig5
*  field   : pausedest
*  access  : read-write
*----------+
*
* Pause DA to send in pause frames""
* 
*******************************************************************/
void umac4_chconfig50__chconfig5__pausedest_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x28 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig50__chconfig5__pausedest(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig50__chconfig5__pausedest_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig50__chconfig5__pausedest(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x28 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig50__chconfig5__pausedest_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig50__chconfig5__pausedest_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig50__chconfig5__pausedest_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig50
*  register: chconfig5
*  field   : pauserefresh
*  access  : read-write
*----------+
*
* Pause refresh time to begin retransmission.""
* 
*******************************************************************/
void umac4_chconfig50__chconfig5__pauserefresh_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x28 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig50__chconfig5__pauserefresh(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig50__chconfig5__pauserefresh_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig50__chconfig5__pauserefresh(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x28 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig50__chconfig5__pauserefresh_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig50__chconfig5__pauserefresh_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig50__chconfig5__pauserefresh_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig310
*  register: chconfig31
*  field   : macoverride
*  access  : read-write
*----------+
*
* Reserved Override controls
* [19:0] Alignment marker cycle length
* [27:20] Alignment marker count
* [31:28] SOF Byte alignment (4'd8 or 4'd4)
* [63:32] Reserved
* (channel speed needs to be programmed first)""
*******************************************************************/
void umac4_chconfig310__chconfig31__macoverride_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xf8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig310__chconfig31__macoverride(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig310__chconfig31__macoverride_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig310__chconfig31__macoverride(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xf8 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig310__chconfig31__macoverride_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig310__chconfig31__macoverride_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig310__chconfig31__macoverride_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : txclkpresentall
*  access  : read-only
*----------+
*
* All TX Serdes clocks are seen""
* 
*******************************************************************/
void umac4_chsts0__chsts__txclkpresentall_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__txclkpresentall(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : rxclkpresentall
*  access  : read-only
*----------+
*
* All RX Serdes clocks are seen""
* 
*******************************************************************/
void umac4_chsts0__chsts__rxclkpresentall_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__rxclkpresentall(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : rxsigokall
*  access  : read-only
*----------+
*
* The Receive Serdes Signal OK for all the serdes lanes active for the
* current channel has been set""
* 
*******************************************************************/
void umac4_chsts0__chsts__rxsigokall_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__rxsigokall(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : blocklockall
*  access  : read-only
*----------+
*
* The Block Lock has been achieved on all the active virtual PCS lanes
* for the current channel.""
* 
*******************************************************************/
void umac4_chsts0__chsts__blocklockall_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__blocklockall(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : amlockall
*  access  : read-only
*----------+
*
* The Alignment Lock has been achieved on all the active virtual PCS
* lanes for the current channel.""
* 
*******************************************************************/
void umac4_chsts0__chsts__amlockall_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__amlockall(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : aligned
*  access  : read-only
*----------+
*
* Deskew has been achieved on all the active virtual PCS lanes for the
* current channel.""
* 
*******************************************************************/
void umac4_chsts0__chsts__aligned_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__aligned(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : nohiber
*  access  : read-only
*----------+
*
* Channel is not in a HiBER state""
* 
*******************************************************************/
void umac4_chsts0__chsts__nohiber_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__nohiber(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : nolocalfault
*  access  : read-only
*----------+
*
* MAC is not receiving Local Faults""
* 
*******************************************************************/
void umac4_chsts0__chsts__nolocalfault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__nolocalfault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : noremotefault
*  access  : read-only
*----------+
*
* MAC is not receiving Remote Faults""
* 
*******************************************************************/
void umac4_chsts0__chsts__noremotefault_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__noremotefault(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : linkup
*  access  : read-only
*----------+
*
* Link Up achieved on the current channel""
* 
*******************************************************************/
void umac4_chsts0__chsts__linkup_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__linkup(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : hiser
*  access  : read-only
*----------+
*
* Channel is not in a HiSER State""
* 
*******************************************************************/
void umac4_chsts0__chsts__hiser_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__hiser(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : fecdegser
*  access  : read-only
*----------+
*
* Local Degraded SER status""
* 
*******************************************************************/
void umac4_chsts0__chsts__fecdegser_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__fecdegser(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chsts0
*  register: chsts
*  field   : rxamsf
*  access  : read-only
*----------+
*
*Recovered Degraded SER status {rx_remote_degrade, rx_local_degrade,
* rsvd}""
* 
*******************************************************************/
void umac4_chsts0__chsts__rxamsf_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x10 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chsts0__chsts__rxamsf(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chconfig80
*  register: chconfig8
*  field   : vlantag1
*  access  : read-write
*----------+
*
* Vlan tag match #1""
* 
*******************************************************************/
void umac4_chconfig80__chconfig8__vlantag1_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x40 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig80__chconfig8__vlantag1(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig80__chconfig8__vlantag1_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig80__chconfig8__vlantag1(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x40 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig80__chconfig8__vlantag1_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig80__chconfig8__vlantag1_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__vlantag1_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig80
*  register: chconfig8
*  field   : vlantag2
*  access  : read-write
*----------+
*
* Vlan tag match #2""
* 
*******************************************************************/
void umac4_chconfig80__chconfig8__vlantag2_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x40 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig80__chconfig8__vlantag2(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig80__chconfig8__vlantag2_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig80__chconfig8__vlantag2(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x40 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig80__chconfig8__vlantag2_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig80__chconfig8__vlantag2_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__vlantag2_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig80
*  register: chconfig8
*  field   : vlantag3
*  access  : read-write
*----------+
*
* Vlan tag match #3 (innermost)""
* 
*******************************************************************/
void umac4_chconfig80__chconfig8__vlantag3_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x40 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig80__chconfig8__vlantag3(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig80__chconfig8__vlantag3_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig80__chconfig8__vlantag3(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x40 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig80__chconfig8__vlantag3_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig80__chconfig8__vlantag3_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__vlantag3_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig80
*  register: chconfig8
*  field   : maxvlancnt
*  access  : read-write
*----------+
*
* Number of valid VLAN tags to search for.""
* 
*******************************************************************/
void umac4_chconfig80__chconfig8__maxvlancnt_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x40 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig80__chconfig8__maxvlancnt(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig80__chconfig8__maxvlancnt_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig80__chconfig8__maxvlancnt(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x40 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig80__chconfig8__maxvlancnt_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig80__chconfig8__maxvlancnt_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__maxvlancnt_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig80
*  register: chconfig8
*  field   : usclkcnt
*  access  : read-write
*----------+
*
* One MicroSecond Clock Count""
* 
*******************************************************************/
void umac4_chconfig80__chconfig8__usclkcnt_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x40 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig80__chconfig8__usclkcnt(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig80__chconfig8__usclkcnt_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig80__chconfig8__usclkcnt(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x40 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig80__chconfig8__usclkcnt_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig80__chconfig8__usclkcnt_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__usclkcnt_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : appCfg0
*  register: appCfg0
*  field   : afulltxinv
*  access  : read-write
*----------+
*
* Invert AFULL output""
* 
*******************************************************************/
void umac4_appCfg0__appCfg0__afulltxinv_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6000 + 0, reg64);
  }
  *fld64 = get_fld_appCfg0__appCfg0__afulltxinv(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_appCfg0__appCfg0__afulltxinv_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_appCfg0__appCfg0__afulltxinv(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6000 + 0, *reg64);
  }
}

void umac4_appCfg0__appCfg0__afulltxinv_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_appCfg0__appCfg0__afulltxinv_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__afulltxinv_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : appCfg0
*  register: appCfg0
*  field   : afullrxinv
*  access  : read-write
*----------+
*
* Invert AFULL output""
* 
*******************************************************************/
void umac4_appCfg0__appCfg0__afullrxinv_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6000 + 0, reg64);
  }
  *fld64 = get_fld_appCfg0__appCfg0__afullrxinv(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_appCfg0__appCfg0__afullrxinv_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_appCfg0__appCfg0__afullrxinv(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6000 + 0, *reg64);
  }
}

void umac4_appCfg0__appCfg0__afullrxinv_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_appCfg0__appCfg0__afullrxinv_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__afullrxinv_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : appCfg0
*  register: appCfg0
*  field   : bitend
*  access  : read-write
*----------+
*
* Bit endianess of Appfifo interface""
* 
*******************************************************************/
void umac4_appCfg0__appCfg0__bitend_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6000 + 0, reg64);
  }
  *fld64 = get_fld_appCfg0__appCfg0__bitend(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_appCfg0__appCfg0__bitend_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_appCfg0__appCfg0__bitend(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6000 + 0, *reg64);
  }
}

void umac4_appCfg0__appCfg0__bitend_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_appCfg0__appCfg0__bitend_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__bitend_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : appCfg0
*  register: appCfg0
*  field   : bytend
*  access  : read-write
*----------+
*
* Byte endianess of Appfifo interface""
* 
*******************************************************************/
void umac4_appCfg0__appCfg0__bytend_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6000 + 0, reg64);
  }
  *fld64 = get_fld_appCfg0__appCfg0__bytend(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_appCfg0__appCfg0__bytend_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_appCfg0__appCfg0__bytend(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6000 + 0, *reg64);
  }
}

void umac4_appCfg0__appCfg0__bytend_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_appCfg0__appCfg0__bytend_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__bytend_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : appCfg0
*  register: appCfg0
*  field   : statscor
*  access  : read-write
*----------+
*
* Statistic Counter Clear on Read""
* 
*******************************************************************/
void umac4_appCfg0__appCfg0__statscor_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6000 + 0, reg64);
  }
  *fld64 = get_fld_appCfg0__appCfg0__statscor(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_appCfg0__appCfg0__statscor_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_appCfg0__appCfg0__statscor(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6000 + 0, *reg64);
  }
}

void umac4_appCfg0__appCfg0__statscor_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_appCfg0__appCfg0__statscor_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__statscor_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : txpltesten
*  access  : read-write
*----------+
*
* Pseudo-random Local Fault transmit testpattern""
* 
*******************************************************************/
void umac4_test00__test0__txpltesten_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__txpltesten(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__txpltesten_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__txpltesten(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__txpltesten_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__txpltesten_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txpltesten_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : txp0testen
*  access  : read-write
*----------+
*
* Pseudo-random Data-0 transmit testpattern""
* 
*******************************************************************/
void umac4_test00__test0__txp0testen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__txp0testen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__txp0testen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__txp0testen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__txp0testen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__txp0testen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txp0testen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : txsitesten
*  access  : read-write
*----------+
*
* Scrambled Idle transmit testpattern""
* 
*******************************************************************/
void umac4_test00__test0__txsitesten_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__txsitesten(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__txsitesten_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__txsitesten(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__txsitesten_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__txsitesten_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txsitesten_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : txswtesten
*  access  : read-write
*----------+
*
* Squarewave transmit testpattern""
* 
*******************************************************************/
void umac4_test00__test0__txswtesten_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__txswtesten(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__txswtesten_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__txswtesten(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__txswtesten_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__txswtesten_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txswtesten_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : rxpltesten
*  access  : read-write
*----------+
*
* Pseudorandom Localfault Testpattern checker""
* 
*******************************************************************/
void umac4_test00__test0__rxpltesten_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__rxpltesten(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__rxpltesten_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__rxpltesten(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__rxpltesten_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__rxpltesten_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__rxpltesten_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : rxp0testen
*  access  : read-write
*----------+
*
* Pseudorandom Data-0 Testpattern checker""
* 
*******************************************************************/
void umac4_test00__test0__rxp0testen_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__rxp0testen(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__rxp0testen_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__rxp0testen(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__rxp0testen_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__rxp0testen_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__rxp0testen_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test00
*  register: test0
*  field   : rxsitesten
*  access  : read-write
*----------+
*
* Scrambled Idle Testpattern checker""
* 
*******************************************************************/
void umac4_test00__test0__rxsitesten_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x48 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test00__test0__rxsitesten(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test00__test0__rxsitesten_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test00__test0__rxsitesten(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x48 + (channel * 0x200), *reg64);
  }
}

void umac4_test00__test0__rxsitesten_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test00__test0__rxsitesten_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__rxsitesten_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test1
*  register: test1
*  field   : seeda
*  access  : read-write
*----------+
*
* Seed A value for Psuedo-Random Testpattern""
* 
*******************************************************************/
void umac4_test1__test1__seeda_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x50 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test1__test1__seeda(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test1__test1__seeda_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test1__test1__seeda(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x50 + (channel * 0x200), *reg64);
  }
}

void umac4_test1__test1__seeda_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test1__test1__seeda_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test1__test1__seeda_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : test2
*  register: test2
*  field   : seedb
*  access  : read-write
*----------+
*
* Seed B value for Psuedo-Random Testpattern""
* 
*******************************************************************/
void umac4_test2__test2__seedb_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x58 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_test2__test2__seedb(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_test2__test2__seedb_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_test2__test2__seedb(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x58 + (channel * 0x200), *reg64);
  }
}

void umac4_test2__test2__seedb_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_test2__test2__seedb_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_test2__test2__seedb_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : serdeslpbk
*  access  : read-write
*----------+
*
* Serdes Tx->RX loopback enable""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__serdeslpbk_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__serdeslpbk(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__serdeslpbk_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__serdeslpbk(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__serdeslpbk_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__serdeslpbk_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__serdeslpbk_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : txremap
*  access  : read-write
*----------+
*
* Transmit Serdes lane can be remapped to any other Serdes lane""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__txremap_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__txremap(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__txremap_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__txremap(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__txremap_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__txremap_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__txremap_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : sigokoverride
*  access  : read-write
*----------+
*
* Signal OK Override. 
* 00 : normal 
*01 invert 
* 10 force to 0 
* 11 force to 1""
*******************************************************************/
void umac4_sdcfg0__sdcfg__sigokoverride_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__sigokoverride(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__sigokoverride_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__sigokoverride(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__sigokoverride_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__sigokoverride_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__sigokoverride_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : rxremap
*  access  : read-write
*----------+
*
* Receive Serdes lane can be remapped to any other Serdes lane""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__rxremap_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__rxremap(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__rxremap_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__rxremap(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__rxremap_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__rxremap_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__rxremap_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : txinv
*  access  : read-write
*----------+
*
* Invert data going out of this Serdes lane""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__txinv_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__txinv(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__txinv_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__txinv(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__txinv_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__txinv_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__txinv_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : rxinv
*  access  : read-write
*----------+
*
* Invert RX Serdes data""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__rxinv_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__rxinv(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__rxinv_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__rxinv(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__rxinv_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__rxinv_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__rxinv_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : paceren
*  access  : read-write
*----------+
*
* Controls Serdes Ready Pacer Select
* 1'b0: Normal
* 1'b1: Pacer Enabled""
*******************************************************************/
void umac4_sdcfg0__sdcfg__paceren_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__paceren(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__paceren_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__paceren(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__paceren_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__paceren_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__paceren_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : pacerdiv
*  access  : read-write
*----------+
*
*Serdes ready pacer divide scaling value determined by: (((Channel_Rate * Overhead_Ratio) / #Serdes_Per_Channel) / (Core_Clock_Frequency * Serdes_Width)) * 2^16 
*Overhead_Ratio = 10/8 (1G and below),  68/64 (Modes with KP FEC),  66/64 (All other modes) 
*Example for 100GR4: (((100*10^9 * 66/64) / 4) / (825*10^6 * 40)) * 2^16 = 51200 
*Note: All serdes pacer divide scaling value registers associated with a channel must be set to the same value with the corresponding pacer enable bits set to 1'b1 when in use""
*******************************************************************/
void umac4_sdcfg0__sdcfg__pacerdiv_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__pacerdiv(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__pacerdiv_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__pacerdiv(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__pacerdiv_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__pacerdiv_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__pacerdiv_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : txprbssel
*  access  : read-write
*----------+
*
*Select which RX PRBS polynomial is used. Valid cases: 31, 23, 15, 11,
* 9, 7.""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__txprbssel_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__txprbssel(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__txprbssel_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__txprbssel(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__txprbssel_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__txprbssel_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__txprbssel_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdcfg0
*  register: sdcfg
*  field   : rxprbssel
*  access  : read-write
*----------+
*
*Select which RX PRBS polynomial is used. Valid cases: 31, 23, 15, 11,
* 9, 7.""
* 
*******************************************************************/
void umac4_sdcfg0__sdcfg__rxprbssel_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdcfg0__sdcfg__rxprbssel(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdcfg0__sdcfg__rxprbssel_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdcfg0__sdcfg__rxprbssel(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1000 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdcfg0__sdcfg__rxprbssel_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdcfg0__sdcfg__rxprbssel_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__rxprbssel_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sderrcfg0
*  register: sderrcfg
*  field   : txerrperiod
*  access  : read-write
*----------+
*
* Error injection period. Period will be equal to (value*1024) bit
* times rounded down to the serdes width. Serdes width is set using TX
* PRBS Width.""
* 
*******************************************************************/
void umac4_sderrcfg0__sderrcfg__txerrperiod_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1008 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sderrcfg0__sderrcfg__txerrperiod(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sderrcfg0__sderrcfg__txerrperiod_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sderrcfg0__sderrcfg__txerrperiod(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1008 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sderrcfg0__sderrcfg__txerrperiod_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sderrcfg0__sderrcfg__txerrperiod_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sderrcfg0__sderrcfg__txerrperiod_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sderrcfg0
*  register: sderrcfg
*  field   : txerrburst
*  access  : read-write
*----------+
*
* Error injection burst length: 0-127. Must be set to non-zero to
* inject errors.""
* 
*******************************************************************/
void umac4_sderrcfg0__sderrcfg__txerrburst_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1008 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sderrcfg0__sderrcfg__txerrburst(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sderrcfg0__sderrcfg__txerrburst_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sderrcfg0__sderrcfg__txerrburst(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1008 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sderrcfg0__sderrcfg__txerrburst_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sderrcfg0__sderrcfg__txerrburst_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sderrcfg0__sderrcfg__txerrburst_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : sdsts0
*  register: sdsts
*  field   : txclkpresent
*  access  : read-only
*----------+
*
* TX clock is present""
* 
*******************************************************************/
void umac4_sdsts0__sdsts__txclkpresent_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdsts0__sdsts__txclkpresent(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : sdsts0
*  register: sdsts
*  field   : txclkrate
*  access  : read-only
*----------+
*
* TX Clock rate (compared to core clock)""
* 
*******************************************************************/
void umac4_sdsts0__sdsts__txclkrate_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdsts0__sdsts__txclkrate(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : sdsts0
*  register: sdsts
*  field   : rxclkpresent
*  access  : read-only
*----------+
*
* RX clock is present""
* 
*******************************************************************/
void umac4_sdsts0__sdsts__rxclkpresent_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdsts0__sdsts__rxclkpresent(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : sdsts0
*  register: sdsts
*  field   : rxclkrate
*  access  : read-only
*----------+
*
* RX serdes clock rate (compared to clock rate)""
* 
*******************************************************************/
void umac4_sdsts0__sdsts__rxclkrate_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdsts0__sdsts__rxclkrate(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : sdsts0
*  register: sdsts
*  field   : sigok
*  access  : read-only
*----------+
*
* Signal OK status""
* 
*******************************************************************/
void umac4_sdsts0__sdsts__sigok_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdsts0__sdsts__sigok(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : sdsts0
*  register: sdsts
*  field   : rxprbserrcnt
*  access  : read-write
*----------+
*
*RX PRBS error count (ignores all-0 case,  autosync)""
* 
*******************************************************************/
void umac4_sdsts0__sdsts__rxprbserrcnt_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), reg64);
  }
  *fld64 = get_fld_sdsts0__sdsts__rxprbserrcnt(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(*fld64), __func__);
}

void umac4_sdsts0__sdsts__rxprbserrcnt_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, serdes_lane, (uint32_t)(fld64), __func__);
  set_fld_sdsts0__sdsts__rxprbserrcnt(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x1010 + (serdes_lane * 0x80), *reg64);
  }
}

void umac4_sdsts0__sdsts__rxprbserrcnt_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_sdsts0__sdsts__rxprbserrcnt_rd(dev_id, umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdsts0__sdsts__rxprbserrcnt_wr(dev_id, umac, serdes_lane, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam0
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam0__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6028 + 0, reg64);
  }
  *fld64 = get_fld_progam0__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam0__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam0__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6028 + 0, *reg64);
  }
}

void umac4_progam0__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam0__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam0__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam0
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam0__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6028 + 0, reg64);
  }
  *fld64 = get_fld_progam0__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam0__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam0__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6028 + 0, *reg64);
  }
}

void umac4_progam0__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam0__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam0__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam1__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6030 + 0, reg64);
  }
  *fld64 = get_fld_progam1__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6030 + 0, *reg64);
  }
}

void umac4_progam1__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6030 + 0, reg64);
  }
  *fld64 = get_fld_progam1__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6030 + 0, *reg64);
  }
}

void umac4_progam1__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam2
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam2__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6038 + 0, reg64);
  }
  *fld64 = get_fld_progam2__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam2__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam2__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6038 + 0, *reg64);
  }
}

void umac4_progam2__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam2__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam2__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam2
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam2__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6038 + 0, reg64);
  }
  *fld64 = get_fld_progam2__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam2__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam2__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6038 + 0, *reg64);
  }
}

void umac4_progam2__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam2__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam2__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam3
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam3__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6040 + 0, reg64);
  }
  *fld64 = get_fld_progam3__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam3__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam3__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6040 + 0, *reg64);
  }
}

void umac4_progam3__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam3__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam3__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam3
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam3__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6040 + 0, reg64);
  }
  *fld64 = get_fld_progam3__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam3__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam3__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6040 + 0, *reg64);
  }
}

void umac4_progam3__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam3__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam3__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam4
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam4__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6048 + 0, reg64);
  }
  *fld64 = get_fld_progam4__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam4__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam4__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6048 + 0, *reg64);
  }
}

void umac4_progam4__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam4__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam4__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam4
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam4__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6048 + 0, reg64);
  }
  *fld64 = get_fld_progam4__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam4__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam4__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6048 + 0, *reg64);
  }
}

void umac4_progam4__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam4__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam4__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam5
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam5__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6050 + 0, reg64);
  }
  *fld64 = get_fld_progam5__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam5__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam5__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6050 + 0, *reg64);
  }
}

void umac4_progam5__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam5__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam5__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam5
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam5__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6050 + 0, reg64);
  }
  *fld64 = get_fld_progam5__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam5__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam5__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6050 + 0, *reg64);
  }
}

void umac4_progam5__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam5__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam5__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam6
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam6__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6058 + 0, reg64);
  }
  *fld64 = get_fld_progam6__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam6__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam6__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6058 + 0, *reg64);
  }
}

void umac4_progam6__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam6__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam6__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam6
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam6__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6058 + 0, reg64);
  }
  *fld64 = get_fld_progam6__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam6__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam6__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6058 + 0, *reg64);
  }
}

void umac4_progam6__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam6__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam6__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam7
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam7__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6060 + 0, reg64);
  }
  *fld64 = get_fld_progam7__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam7__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam7__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6060 + 0, *reg64);
  }
}

void umac4_progam7__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam7__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam7__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam7
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam7__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6060 + 0, reg64);
  }
  *fld64 = get_fld_progam7__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam7__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam7__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6060 + 0, *reg64);
  }
}

void umac4_progam7__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam7__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam7__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam8
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam8__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6068 + 0, reg64);
  }
  *fld64 = get_fld_progam8__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam8__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam8__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6068 + 0, *reg64);
  }
}

void umac4_progam8__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam8__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam8__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam8
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam8__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6068 + 0, reg64);
  }
  *fld64 = get_fld_progam8__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam8__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam8__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6068 + 0, *reg64);
  }
}

void umac4_progam8__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam8__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam8__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam9
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam9__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6070 + 0, reg64);
  }
  *fld64 = get_fld_progam9__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam9__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam9__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6070 + 0, *reg64);
  }
}

void umac4_progam9__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam9__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam9__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam9
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam9__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6070 + 0, reg64);
  }
  *fld64 = get_fld_progam9__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam9__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam9__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6070 + 0, *reg64);
  }
}

void umac4_progam9__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam9__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam9__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamA
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progamA__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6078 + 0, reg64);
  }
  *fld64 = get_fld_progamA__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamA__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamA__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6078 + 0, *reg64);
  }
}

void umac4_progamA__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamA__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamA__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamA
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progamA__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6078 + 0, reg64);
  }
  *fld64 = get_fld_progamA__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamA__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamA__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6078 + 0, *reg64);
  }
}

void umac4_progamA__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamA__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamA__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamB
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progamB__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6080 + 0, reg64);
  }
  *fld64 = get_fld_progamB__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamB__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamB__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6080 + 0, *reg64);
  }
}

void umac4_progamB__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamB__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamB__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamB
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progamB__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6080 + 0, reg64);
  }
  *fld64 = get_fld_progamB__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamB__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamB__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6080 + 0, *reg64);
  }
}

void umac4_progamB__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamB__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamB__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamC
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progamC__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6088 + 0, reg64);
  }
  *fld64 = get_fld_progamC__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamC__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamC__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6088 + 0, *reg64);
  }
}

void umac4_progamC__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamC__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamC__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamC
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progamC__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6088 + 0, reg64);
  }
  *fld64 = get_fld_progamC__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamC__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamC__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6088 + 0, *reg64);
  }
}

void umac4_progamC__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamC__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamC__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamD
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progamD__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6090 + 0, reg64);
  }
  *fld64 = get_fld_progamD__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamD__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamD__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6090 + 0, *reg64);
  }
}

void umac4_progamD__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamD__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamD__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamD
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progamD__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6090 + 0, reg64);
  }
  *fld64 = get_fld_progamD__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamD__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamD__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6090 + 0, *reg64);
  }
}

void umac4_progamD__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamD__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamD__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamE
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progamE__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6098 + 0, reg64);
  }
  *fld64 = get_fld_progamE__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamE__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamE__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6098 + 0, *reg64);
  }
}

void umac4_progamE__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamE__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamE__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamE
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progamE__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6098 + 0, reg64);
  }
  *fld64 = get_fld_progamE__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamE__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamE__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6098 + 0, *reg64);
  }
}

void umac4_progamE__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamE__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamE__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamF
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progamF__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60a0 + 0, reg64);
  }
  *fld64 = get_fld_progamF__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamF__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamF__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60a0 + 0, *reg64);
  }
}

void umac4_progamF__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamF__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamF__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progamF
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progamF__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60a0 + 0, reg64);
  }
  *fld64 = get_fld_progamF__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progamF__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progamF__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60a0 + 0, *reg64);
  }
}

void umac4_progamF__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progamF__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progamF__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam10
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam10__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60a8 + 0, reg64);
  }
  *fld64 = get_fld_progam10__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam10__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam10__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60a8 + 0, *reg64);
  }
}

void umac4_progam10__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam10__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam10__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam10
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam10__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60a8 + 0, reg64);
  }
  *fld64 = get_fld_progam10__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam10__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam10__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60a8 + 0, *reg64);
  }
}

void umac4_progam10__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam10__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam10__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam11
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam11__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60b0 + 0, reg64);
  }
  *fld64 = get_fld_progam11__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam11__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam11__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60b0 + 0, *reg64);
  }
}

void umac4_progam11__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam11__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam11__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam11
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam11__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60b0 + 0, reg64);
  }
  *fld64 = get_fld_progam11__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam11__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam11__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60b0 + 0, *reg64);
  }
}

void umac4_progam11__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam11__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam11__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam12
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam12__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60b8 + 0, reg64);
  }
  *fld64 = get_fld_progam12__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam12__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam12__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60b8 + 0, *reg64);
  }
}

void umac4_progam12__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam12__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam12__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam12
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam12__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60b8 + 0, reg64);
  }
  *fld64 = get_fld_progam12__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam12__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam12__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60b8 + 0, *reg64);
  }
}

void umac4_progam12__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam12__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam12__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam13
*  register: progam
*  field   : am
*  access  : read-write
*----------+
*
* Alignment Maker Override Bank""
* 
*******************************************************************/
void umac4_progam13__progam__am_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60c0 + 0, reg64);
  }
  *fld64 = get_fld_progam13__progam__am(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam13__progam__am_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam13__progam__am(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60c0 + 0, *reg64);
  }
}

void umac4_progam13__progam__am_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam13__progam__am_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam13__progam__am_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam13
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam13__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60c0 + 0, reg64);
  }
  *fld64 = get_fld_progam13__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam13__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam13__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60c0 + 0, *reg64);
  }
}

void umac4_progam13__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam13__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam13__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam14
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam14__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60c8 + 0, reg64);
  }
  *fld64 = get_fld_progam14__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam14__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam14__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60c8 + 0, *reg64);
  }
}

void umac4_progam14__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam14__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam14__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam15
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam15__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60d0 + 0, reg64);
  }
  *fld64 = get_fld_progam15__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam15__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam15__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60d0 + 0, *reg64);
  }
}

void umac4_progam15__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam15__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam15__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam16
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam16__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60d8 + 0, reg64);
  }
  *fld64 = get_fld_progam16__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam16__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam16__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60d8 + 0, *reg64);
  }
}

void umac4_progam16__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam16__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam16__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam17
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam17__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60e0 + 0, reg64);
  }
  *fld64 = get_fld_progam17__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam17__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam17__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60e0 + 0, *reg64);
  }
}

void umac4_progam17__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam17__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam17__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam18
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam18__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60e8 + 0, reg64);
  }
  *fld64 = get_fld_progam18__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam18__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam18__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60e8 + 0, *reg64);
  }
}

void umac4_progam18__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam18__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam18__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam19
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam19__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60f0 + 0, reg64);
  }
  *fld64 = get_fld_progam19__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam19__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam19__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60f0 + 0, *reg64);
  }
}

void umac4_progam19__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam19__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam19__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1A
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1A__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x60f8 + 0, reg64);
  }
  *fld64 = get_fld_progam1A__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1A__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1A__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x60f8 + 0, *reg64);
  }
}

void umac4_progam1A__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1A__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1A__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1B
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1B__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6100 + 0, reg64);
  }
  *fld64 = get_fld_progam1B__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1B__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1B__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6100 + 0, *reg64);
  }
}

void umac4_progam1B__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1B__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1B__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1C
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1C__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6108 + 0, reg64);
  }
  *fld64 = get_fld_progam1C__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1C__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1C__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6108 + 0, *reg64);
  }
}

void umac4_progam1C__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1C__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1C__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1D
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1D__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6110 + 0, reg64);
  }
  *fld64 = get_fld_progam1D__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1D__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1D__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6110 + 0, *reg64);
  }
}

void umac4_progam1D__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1D__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1D__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1E
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1E__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6118 + 0, reg64);
  }
  *fld64 = get_fld_progam1E__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1E__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1E__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6118 + 0, *reg64);
  }
}

void umac4_progam1E__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1E__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1E__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : progam1F
*  register: progam
*  field   : bip
*  access  : read-write
*----------+
*
* BIP value override bank""
* 
*******************************************************************/
void umac4_progam1F__progam__bip_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x6120 + 0, reg64);
  }
  *fld64 = get_fld_progam1F__progam__bip(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_progam1F__progam__bip_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_progam1F__progam__bip(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x6120 + 0, *reg64);
  }
}

void umac4_progam1F__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_progam1F__progam__bip_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_progam1F__progam__bip_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig60
*  register: chconfig6
*  field   : corrbyp
*  access  : read-write
*----------+
*
* Bypass FEC correction""
* 
*******************************************************************/
void umac4_chconfig60__chconfig6__corrbyp_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x30 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig60__chconfig6__corrbyp(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig60__chconfig6__corrbyp_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig60__chconfig6__corrbyp(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x30 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig60__chconfig6__corrbyp_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig60__chconfig6__corrbyp_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__corrbyp_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig60
*  register: chconfig6
*  field   : indibyp
*  access  : read-write
*----------+
*
* Bypass FEC indication""
* 
*******************************************************************/
void umac4_chconfig60__chconfig6__indibyp_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x30 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig60__chconfig6__indibyp(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig60__chconfig6__indibyp_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig60__chconfig6__indibyp(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x30 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig60__chconfig6__indibyp_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig60__chconfig6__indibyp_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__indibyp_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig60
*  register: chconfig6
*  field   : hiserthresh
*  access  : read-write
*----------+
*
* HiSER symbol error threshold.""
* 
*******************************************************************/
void umac4_chconfig60__chconfig6__hiserthresh_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x30 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig60__chconfig6__hiserthresh(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig60__chconfig6__hiserthresh_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig60__chconfig6__hiserthresh(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x30 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig60__chconfig6__hiserthresh_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig60__chconfig6__hiserthresh_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__hiserthresh_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig60
*  register: chconfig6
*  field   : degserenable
*  access  : read-write
*----------+
*
* Enable Degraded SER generation""
* 
*******************************************************************/
void umac4_chconfig60__chconfig6__degserenable_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x30 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig60__chconfig6__degserenable(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig60__chconfig6__degserenable_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig60__chconfig6__degserenable(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x30 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig60__chconfig6__degserenable_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig60__chconfig6__degserenable_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__degserenable_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig60
*  register: chconfig6
*  field   : degserinterval
*  access  : read-write
*----------+
*
* Degraded SER window interval""
* 
*******************************************************************/
void umac4_chconfig60__chconfig6__degserinterval_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x30 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig60__chconfig6__degserinterval(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig60__chconfig6__degserinterval_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig60__chconfig6__degserinterval(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x30 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig60__chconfig6__degserinterval_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig60__chconfig6__degserinterval_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__degserinterval_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig70
*  register: chconfig7
*  field   : degseractivatethresh
*  access  : read-write
*----------+
*
* Degraded SER active threshold""
* 
*******************************************************************/
void umac4_chconfig70__chconfig7__degseractivatethresh_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x38 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig70__chconfig7__degseractivatethresh(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig70__chconfig7__degseractivatethresh_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig70__chconfig7__degseractivatethresh(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x38 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig70__chconfig7__degseractivatethresh_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig70__chconfig7__degseractivatethresh_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig70__chconfig7__degseractivatethresh_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : chconfig70
*  register: chconfig7
*  field   : degserdeactivatethresh
*  access  : read-write
*----------+
*
* Degraded SER inactive threshold (hyst)""
* 
*******************************************************************/
void umac4_chconfig70__chconfig7__degserdeactivatethresh_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x38 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig70__chconfig7__degserdeactivatethresh(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig70__chconfig7__degserdeactivatethresh_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig70__chconfig7__degserdeactivatethresh(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x38 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig70__chconfig7__degserdeactivatethresh_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig70__chconfig7__degserdeactivatethresh_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig70__chconfig7__degserdeactivatethresh_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : pcsrxoverride00
*  register: pcsrxoverride0
*  field   : rxoverride0
*  access  : read-write
*----------+
*
* Reserved Override controls
* [63:62] Reserved
* [61:44] AM count per virtual lane
* [43: 0]  Reserved""
*******************************************************************/
void umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xe0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_pcsrxoverride00__pcsrxoverride0__rxoverride0(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_pcsrxoverride00__pcsrxoverride0__rxoverride0(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xe0 + (channel * 0x200), *reg64);
  }
}

void umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : pcsrxoverride01
*  register: pcsrxoverride1
*  field   : rxoverride1
*  access  : read-write
*----------+
*
* Reserved Override controls
* [63:59] Reserved
* [58]      Deskew ECC Enable
* [57:21] Reserved
* [20:15] AM1 Select
* [14: 9]  AM0 Select
* [ 8: 7]   Reserved
* [ 6 ]      Dynamic AM lock
* [ 5: 0]   Reserved""
*******************************************************************/
void umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xe8 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_pcsrxoverride01__pcsrxoverride1__rxoverride1(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_pcsrxoverride01__pcsrxoverride1__rxoverride1(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xe8 + (channel * 0x200), *reg64);
  }
}

void umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : pcsrxoverride02
*  register: pcsrxoverride2
*  field   : rxoverride2
*  access  : read-write
*----------+
*
* Reserved Override controls
* [63]      Reserved
* [62]      Pre-code enable
* [61]      Gray code enable
* [60:55] Reserved
* [54]      PCS descrambler enable
* [53:12] Reserved
* [11]      RSFEC header descrambler enable
* [10]      FEC ECC enable
* [ 9: 8]   Reserved
* [ 7 ]      FEC Desrambler enable
* [ 6: 0]   Reserved""
*******************************************************************/
void umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xf0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_pcsrxoverride02__pcsrxoverride2__rxoverride2(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_pcsrxoverride02__pcsrxoverride2__rxoverride2(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xf0 + (channel * 0x200), *reg64);
  }
}

void umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : amlock
*  access  : read-only
*----------+
*
* Alignment Lock Status""
* 
*******************************************************************/
void umac4_pcslcfg0__pcslcfg__amlock_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1800 + (virtual_lane * 0x10), reg64);
  }
  *fld64 = get_fld_pcslcfg0__pcslcfg__amlock(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, virtual_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : blocklock
*  access  : read-only
*----------+
*
* Block Lock Status""
* 
*******************************************************************/
void umac4_pcslcfg0__pcslcfg__blocklock_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1800 + (virtual_lane * 0x10), reg64);
  }
  *fld64 = get_fld_pcslcfg0__pcslcfg__blocklock(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, virtual_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : mapping
*  access  : read-only
*----------+
*
* Alignment Marker Mapping Recieved""
* 
*******************************************************************/
void umac4_pcslcfg0__pcslcfg__mapping_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1800 + (virtual_lane * 0x10), reg64);
  }
  *fld64 = get_fld_pcslcfg0__pcslcfg__mapping(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, virtual_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : amperiod
*  access  : read-only
*----------+
*
* Alignment Marker Period""
* 
*******************************************************************/
void umac4_pcslcfg0__pcslcfg__amperiod_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x1800 + (virtual_lane * 0x10), reg64);
  }
  *fld64 = get_fld_pcslcfg0__pcslcfg__amperiod(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, virtual_lane, (uint32_t)(*fld64), __func__);
}

/*******************************************************************
*  block   : chconfig130
*  register: chconfig13
*  field   : txtsoffset
*  access  : read-write
*----------+
*
* Timestamp Offset (signed)""
* 
*******************************************************************/
void umac4_chconfig130__chconfig13__txtsoffset_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x68 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_chconfig130__chconfig13__txtsoffset(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_chconfig130__chconfig13__txtsoffset_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_chconfig130__chconfig13__txtsoffset(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x68 + (channel * 0x200), *reg64);
  }
}

void umac4_chconfig130__chconfig13__txtsoffset_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_chconfig130__chconfig13__txtsoffset_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig130__chconfig13__txtsoffset_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : version
*  register: version
*  field   : version
*  access  : read-write
*----------+
*
* Product Version""
* 
*******************************************************************/
void umac4_version__version__version_rd(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0x7ff8 + 0, reg64);
  }
  *fld64 = get_fld_version__version__version(reg64);
  autogen_log("TRC : %d: p%02d : --- : Rd : -------- : %08x : %s", dev_id, umac, (uint32_t)(*fld64), __func__);
}

void umac4_version__version__version_wr(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : --- : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)(fld64), __func__);
  set_fld_version__version__version(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0x7ff8 + 0, *reg64);
  }
}

void umac4_version__version__version_rmw(bf_dev_id_t dev_id, uint32_t umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_version__version__version_rd(dev_id, umac, reg64, &unused_fld64, true);
  umac4_version__version__version_wr(dev_id, umac, reg64, fld64, true);
}


/*******************************************************************
*  block   : intcontrol0
*  register: intcontrol
*  field   : intsts
*  access  : read-write
*----------+
*
*Interrupt status,  also serves as interrupt override.""
* 
*******************************************************************/
void umac4_intcontrol0__intcontrol__intsts_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xb0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_intcontrol0__intcontrol__intsts(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_intcontrol0__intcontrol__intsts_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_intcontrol0__intcontrol__intsts(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xb0 + (channel * 0x200), *reg64);
  }
}

void umac4_intcontrol0__intcontrol__intsts_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_intcontrol0__intcontrol__intsts_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_intcontrol0__intcontrol__intsts_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : intcontrol0
*  register: intcontrol
*  field   : intclr
*  access  : read-write
*----------+
*
* Write 1 to clear interrupt status and intraw.""
* 
*******************************************************************/
void umac4_intcontrol0__intcontrol__intclr_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xb0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_intcontrol0__intcontrol__intclr(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_intcontrol0__intcontrol__intclr_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_intcontrol0__intcontrol__intclr(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xb0 + (channel * 0x200), *reg64);
  }
}

void umac4_intcontrol0__intcontrol__intclr_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_intcontrol0__intcontrol__intclr_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_intcontrol0__intcontrol__intclr_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : intcontrol0
*  register: intcontrol
*  field   : intena
*  access  : read-write
*----------+
*
*1'b1 Interrupt is enabled 
* 1'b0 Interrupt is masked,  intsts value is masked""
*******************************************************************/
void umac4_intcontrol0__intcontrol__intena_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xb0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_intcontrol0__intcontrol__intena(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

void umac4_intcontrol0__intcontrol__intena_wr(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(fld64), __func__);
  set_fld_intcontrol0__intcontrol__intena(reg64, fld64);
  if (hw) {
    umac4_wr64(dev_id, umac, 0xb0 + (channel * 0x200), *reg64);
  }
}

void umac4_intcontrol0__intcontrol__intena_rmw(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;

  umac4_intcontrol0__intcontrol__intena_rd(dev_id, umac, channel, reg64, &unused_fld64, true);
  umac4_intcontrol0__intcontrol__intena_wr(dev_id, umac, channel, reg64, fld64, true);
}


/*******************************************************************
*  block   : intcontrol0
*  register: intcontrol
*  field   : intraw
*  access  : read-only
*----------+
*
* Interrupt raw data""
* 
*******************************************************************/
void umac4_intcontrol0__intcontrol__intraw_rd(bf_dev_id_t dev_id, uint32_t umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  if (hw) {
    umac4_rd64(dev_id, umac, 0xb0 + (channel * 0x200), reg64);
  }
  *fld64 = get_fld_intcontrol0__intcontrol__intraw(reg64);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, channel, (uint32_t)(*fld64), __func__);
}

