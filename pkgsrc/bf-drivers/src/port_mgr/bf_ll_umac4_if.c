/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include <bf_types/bf_types.h>
#include "port_mgr_tof2/umac4c8_access.h"

extern bf_status_t bf_map_logical_umac4_to_physical(bf_dev_id_t dev_id,
                                                    uint32_t logical_umac,
                                                    uint32_t *physical_umac);
/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : mode
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
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
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__mode_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__mode_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__mode_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__mode_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__mode_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__mode_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__mode_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : txswrst
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* 1'b1 TX Reset Active 
* 1'b0 TX Normal Operation""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__txswrst_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txswrst_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txswrst_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txswrst_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txswrst_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txswrst_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txswrst_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : rxswrst
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* 1'b1 RX Reset Active 
* 1'b0 RX Normal Operation""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__rxswrst_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxswrst_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__rxswrst_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxswrst_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__rxswrst_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxswrst_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__rxswrst_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : txen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* 1'b1 TX Normal Operation
* 1'b0 TX Channel Disabled""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__txen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : txdrain
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Setting this bit causes transmit path to enter into Drain mode where
* all the data from the TXFIFO is drained out.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__txdrain_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txdrain_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txdrain_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txdrain_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txdrain_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txdrain_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txdrain_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : rxen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* 1'b1 RX Normal Operation
* 1'b0 RX Channel Disabled""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__rxen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__rxen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__rxen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__rxen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : gmiilpbk
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Setting this bit enables the Loopback on the MAC-PCS Interface for
* this Channel. The Transmit data and controls are looped back on to the
* receive MAC module for this Channel. When this loopback is enabled,
* the data received from the PCS for this channel is ignored.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__gmiilpbk_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__gmiilpbk_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__gmiilpbk_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__gmiilpbk_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__gmiilpbk_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__gmiilpbk_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__gmiilpbk_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : txjabber
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*This field determines the Jabber Size for the outgoing (Transmit)
* frames on this Channel. When the length of the current outgoing frame
* on this Channel exceeds the value programmed in this field,  the Frame
* is considered a Jabber Frame and is truncated at that point with EOF-
* ERROR. This will limit the frame transmission run-off in case of
* Application logic error.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__txjabber_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txjabber_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txjabber_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txjabber_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__txjabber_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__txjabber_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__txjabber_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : rxjabber
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*This field determines the Jabber Size for the incoming (Receive)
* frames on this Channel. When the length of the current incoming frame
* on this Channel equals or exceeds the value programmed in this field,
* the Frame is considered a Jabber Frame and is truncated at that point.
* The Frame Status is updated with a Jabber Error and the rest of the
* Jabbered frame that is being received is ignored.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__rxjabber_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxjabber_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__rxjabber_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxjabber_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__rxjabber_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__rxjabber_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__rxjabber_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : disfcs
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Disables the Transmit FCS Insertion""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__disfcs_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__disfcs_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__disfcs_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__disfcs_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__disfcs_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__disfcs_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__disfcs_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : invfcs
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Forces the inserted Transmit FCS to an inverted value to force an
* error in all conditions""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__invfcs_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__invfcs_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__invfcs_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__invfcs_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__invfcs_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__invfcs_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__invfcs_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : ignfcs
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Ignore the FCS on received frames""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__ignfcs_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ignfcs_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__ignfcs_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ignfcs_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__ignfcs_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ignfcs_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__ignfcs_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : stripfcs
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Strips the FCS of received frames before they are forwarded to the
* application""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__stripfcs_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__stripfcs_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__stripfcs_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__stripfcs_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__stripfcs_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__stripfcs_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__stripfcs_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : ifglen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Selects the minimum IFG length in bytes inserted on transmit frames""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__ifglen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ifglen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__ifglen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ifglen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__ifglen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ifglen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__ifglen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chmode0
*  register: chmode
*  field   : ifgpacing
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* IFG controls (reserved)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chmode0__chmode__ifgpacing_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ifgpacing_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__ifgpacing_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ifgpacing_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chmode0__chmode__ifgpacing_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chmode0__chmode__ifgpacing_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chmode0__chmode__ifgpacing_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : disfcsonerr
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When Disable FCS is set,  this will include errored frames""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__disfcsonerr_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__disfcsonerr_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__disfcsonerr_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__disfcsonerr_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__disfcsonerr_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__disfcsonerr_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__disfcsonerr_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txfcen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Enable Pause frame generation from XOFF pins""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txfcen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txfcen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txfcen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txfcen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txfcen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txfcen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txfcen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxfcen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the UMAC Core is enabled for Flow-Control decode operation for this Channel and it will decode all the incoming frames for PAUSE Control Frames as specified in the IEEE 802.3 Specification.
* When reset,  this Channel does not decode the frames for PAUSE Control Frames.""
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfcen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfcen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfcen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfcen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfcen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfcen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfcen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxpfcen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the UMAC Core is enabled for Priority Flow-Control decode operation for this Channel.
* If the UMAC Core receives a valid Priority PAUSE Control Frame,  it will load the timers and provide the XOFF indication to the Application logic (ff_rxch0pfcxoff[7:0]) based on the Time Vector fields in the 8 priorities.
* When reset,  this Channel does not decode the frames for Priority PAUSE Control Frames.""
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxpfcen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxpfcen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxpfcen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxpfcen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxpfcen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxpfcen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxpfcen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxfctotx
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*If decoding of Flow-Control frames is enabled,  setting this bit will
* disable the transmission of user data frames for the time given in the
* PAUSE_TIME field of the PAUSE Control Frame when UMAC Core receives a
* valid PAUSE Control Frame.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfctotx_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfctotx_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfctotx_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfctotx_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfctotx_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfctotx_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfctotx_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxfilterfc
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When this bit is set,  the UMAC will filter out the Pause frames from being sent to the Application Logic. 
* When this bit is reset,  the UMAC does not filter out the Pause frames .
* This bit has no effect on actual processing/decoding on the PAUSEControl frames,  which is controlled using the Enable Receive Flow Control Decode register bit.""
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfilterfc_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfilterfc_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfilterfc_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfilterfc_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfilterfc_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfilterfc_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfilterfc_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxfilterpfc
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When this bit is set,  the UMAC will filter out the Priority Pause frames from being sent to the Application Logic. 
* When this bit is reset,  the UMAC does not filter out the Priority Pause frames 
* This bit has no effect on actual processing/decoding on the Priority PAUSE Control frames,  which is controlled using the Enable Priority Receive Flow Control Decode register bit.""
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfilterpfc_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfilterpfc_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfilterpfc_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfilterpfc_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxfilterpfc_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxfilterpfc_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxfilterpfc_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txpadrunt
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* TX PAD length (set to 0 to disable)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txpadrunt_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txpadrunt_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txpadrunt_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txpadrunt_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txpadrunt_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txpadrunt_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txpadrunt_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txwrthresh
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* TX fifo write threshold. Set to 255-(delay between afull and vld)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txwrthresh_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txwrthresh_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txwrthresh_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txwrthresh_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txwrthresh_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txwrthresh_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txwrthresh_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txrdthresh
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* TX fifo read threshold""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txrdthresh_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txrdthresh_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txrdthresh_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txrdthresh_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txrdthresh_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txrdthresh_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txrdthresh_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txlfault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the MAC continuously transmits local faults. Normal traffic
* is overriden and not paused""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txlfault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txlfault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txlfault_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txlfault_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txlfault_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txlfault_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txlfault_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txrfault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the MAC continuously transmits remote faults. Normal
* traffic is overriden and not paused""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txrfault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txrfault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txrfault_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txrfault_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txrfault_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txrfault_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txrfault_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txidle
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the MAC continuously transmits idles. Normal traffic is
* overriden and not paused""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txidle_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txidle_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txidle_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txidle_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txidle_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txidle_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txidle_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxpadrunt
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Defines the MinFrame Size for padding in the Receive direction""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxpadrunt_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxpadrunt_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxpadrunt_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxpadrunt_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxpadrunt_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxpadrunt_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxpadrunt_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxlfault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the MAC continuously receives L_FAULT ordered set.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxlfault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxlfault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxlfault_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxlfault_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxlfault_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxlfault_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxlfault_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxrfault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the MAC continuously receives R_FAULT ordered set.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxrfault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxrfault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxrfault_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxrfault_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxrfault_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxrfault_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxrfault_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : rxidle
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  the MAC continuously receives IDLE Pattern.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__rxidle_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxidle_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxidle_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxidle_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__rxidle_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__rxidle_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__rxidle_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : statsclr
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Set to clear stats for mac channel""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__statsclr_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__statsclr_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__statsclr_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__statsclr_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__statsclr_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__statsclr_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__statsclr_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txignorerx
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Overrides the FAULT state recieved from the rx Reconciliation Layer""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txignorerx_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txignorerx_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txignorerx_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txignorerx_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txignorerx_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txignorerx_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txignorerx_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : maccfg0
*  register: maccfg
*  field   : txpfcen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Enable PFC frame generation from XOFF pins (Mask per pin)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_maccfg0__maccfg__txpfcen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txpfcen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txpfcen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txpfcen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_maccfg0__maccfg__txpfcen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_maccfg0__maccfg__txpfcen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_maccfg0__maccfg__txpfcen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig120
*  register: chconfig12
*  field   : txvlantag
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* VLAN tag to identify frames for VLAN TX statistic counter""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig120__chconfig12__txvlantag_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig120__chconfig12__txvlantag_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig120__chconfig12__txvlantag_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig120__chconfig12__txvlantag_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig120__chconfig12__txvlantag_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig120__chconfig12__txvlantag_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig120__chconfig12__txvlantag_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig30
*  register: chconfig3
*  field   : ifgppm
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* PPM adjustment of IFG by +/-base*2^-(10+exp)
* [15] = +/-
* [14:10] = exp
* [9:0]=base
* ""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig30__chconfig3__ifgppm_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__ifgppm_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__ifgppm_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__ifgppm_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__ifgppm_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__ifgppm_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__ifgppm_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig30
*  register: chconfig3
*  field   : rxmaxfrmsize
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Max frame length before error is generated""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxmaxfrmsize_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxmaxfrmsize_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxmaxfrmsize_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxmaxfrmsize_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__rxmaxfrmsize_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig30
*  register: chconfig3
*  field   : txpreamble
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* TX frame preamble length (including /S/)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig30__chconfig3__txpreamble_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__txpreamble_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__txpreamble_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__txpreamble_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__txpreamble_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__txpreamble_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__txpreamble_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig30
*  register: chconfig3
*  field   : txdrainonfault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Setting this bit causes transmit path to enter into drain mode when
* the RX is in a fault state""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig30__chconfig3__txdrainonfault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__txdrainonfault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__txdrainonfault_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__txdrainonfault_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__txdrainonfault_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__txdrainonfault_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__txdrainonfault_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig30
*  register: chconfig3
*  field   : rxpreamble
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*When set,  enable 4-byte preamble support,  otherwise only support
* 8-byte preamble.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxpreamble_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxpreamble_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxpreamble_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxpreamble_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxpreamble_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxpreamble_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__rxpreamble_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig30
*  register: chconfig3
*  field   : rxerrmask
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Each bit corresponds to a type of error,  when set,  error is masked.
*[0]: CRC Error Mask
*[1]: Jabber Error Mask
*[2]: PCS Error Mask
*[3]: MaxFrameLenVio Error Mask
*[4]: Length Error Mask""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxerrmask_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxerrmask_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxerrmask_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxerrmask_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig30__chconfig3__rxerrmask_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig30__chconfig3__rxerrmask_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig30__chconfig3__rxerrmask_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig40
*  register: chconfig4
*  field   : macaddr
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Mac address used for SA in TX pause frames and DA in RX pause
* frames""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig40__chconfig4__macaddr_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig40__chconfig4__macaddr_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig40__chconfig4__macaddr_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig40__chconfig4__macaddr_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig40__chconfig4__macaddr_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig40__chconfig4__macaddr_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig40__chconfig4__macaddr_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig40
*  register: chconfig4
*  field   : pauseontime
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pause refresh time to send in pause fame""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig40__chconfig4__pauseontime_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig40__chconfig4__pauseontime_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig40__chconfig4__pauseontime_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig40__chconfig4__pauseontime_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig40__chconfig4__pauseontime_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig40__chconfig4__pauseontime_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig40__chconfig4__pauseontime_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig50
*  register: chconfig5
*  field   : pausedest
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pause DA to send in pause frames""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig50__chconfig5__pausedest_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig50__chconfig5__pausedest_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig50__chconfig5__pausedest_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig50__chconfig5__pausedest_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig50__chconfig5__pausedest_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig50__chconfig5__pausedest_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig50__chconfig5__pausedest_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig50
*  register: chconfig5
*  field   : pauserefresh
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pause refresh time to begin retransmission.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig50__chconfig5__pauserefresh_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig50__chconfig5__pauserefresh_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig50__chconfig5__pauserefresh_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig50__chconfig5__pauserefresh_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig50__chconfig5__pauserefresh_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig50__chconfig5__pauserefresh_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig50__chconfig5__pauserefresh_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig310
*  register: chconfig31
*  field   : macoverride
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Reserved Override controls
* [19:0] Alignment marker cycle length
* [27:20] Alignment marker count
* [31:28] SOF Byte alignment (4'd8 or 4'd4)
* [63:32] Reserved
* (channel speed needs to be programmed first)""
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig310__chconfig31__macoverride_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig310__chconfig31__macoverride_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig310__chconfig31__macoverride_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig310__chconfig31__macoverride_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig310__chconfig31__macoverride_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig310__chconfig31__macoverride_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig310__chconfig31__macoverride_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : txclkpresentall
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* All TX Serdes clocks are seen""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__txclkpresentall_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__txclkpresentall_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : rxclkpresentall
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* All RX Serdes clocks are seen""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__rxclkpresentall_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__rxclkpresentall_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : rxsigokall
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* The Receive Serdes Signal OK for all the serdes lanes active for the
* current channel has been set""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__rxsigokall_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__rxsigokall_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : blocklockall
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* The Block Lock has been achieved on all the active virtual PCS lanes
* for the current channel.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__blocklockall_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__blocklockall_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : amlockall
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* The Alignment Lock has been achieved on all the active virtual PCS
* lanes for the current channel.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__amlockall_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__amlockall_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : aligned
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Deskew has been achieved on all the active virtual PCS lanes for the
* current channel.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__aligned_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__aligned_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : nohiber
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Channel is not in a HiBER state""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__nohiber_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__nohiber_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : nolocalfault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* MAC is not receiving Local Faults""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__nolocalfault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__nolocalfault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : noremotefault
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* MAC is not receiving Remote Faults""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__noremotefault_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__noremotefault_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : linkup
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Link Up achieved on the current channel""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__linkup_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__linkup_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : hiser
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Channel is not in a HiSER State""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__hiser_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__hiser_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : fecdegser
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Local Degraded SER status""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__fecdegser_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__fecdegser_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : chsts0
*  register: chsts
*  field   : rxamsf
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Recovered Degraded SER status {rx_remote_degrade, rx_local_degrade,
* rsvd}""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chsts0__chsts__rxamsf_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chsts0__chsts__rxamsf_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-write
*  block   : chconfig80
*  register: chconfig8
*  field   : vlantag1
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Vlan tag match #1""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag1_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag1_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag1_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag1_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag1_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag1_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__vlantag1_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig80
*  register: chconfig8
*  field   : vlantag2
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Vlan tag match #2""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag2_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag2_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag2_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag2_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag2_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag2_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__vlantag2_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig80
*  register: chconfig8
*  field   : vlantag3
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Vlan tag match #3 (innermost)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag3_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag3_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag3_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag3_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__vlantag3_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__vlantag3_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__vlantag3_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig80
*  register: chconfig8
*  field   : maxvlancnt
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Number of valid VLAN tags to search for.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig80__chconfig8__maxvlancnt_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__maxvlancnt_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__maxvlancnt_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__maxvlancnt_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__maxvlancnt_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__maxvlancnt_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__maxvlancnt_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig80
*  register: chconfig8
*  field   : usclkcnt
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* One MicroSecond Clock Count""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig80__chconfig8__usclkcnt_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__usclkcnt_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__usclkcnt_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__usclkcnt_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig80__chconfig8__usclkcnt_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig80__chconfig8__usclkcnt_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig80__chconfig8__usclkcnt_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : appCfg0
*  register: appCfg0
*  field   : afulltxinv
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Invert AFULL output""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_appCfg0__appCfg0__afulltxinv_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__afulltxinv_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__afulltxinv_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__afulltxinv_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__afulltxinv_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__afulltxinv_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__afulltxinv_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : appCfg0
*  register: appCfg0
*  field   : afullrxinv
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Invert AFULL output""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_appCfg0__appCfg0__afullrxinv_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__afullrxinv_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__afullrxinv_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__afullrxinv_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__afullrxinv_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__afullrxinv_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__afullrxinv_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : appCfg0
*  register: appCfg0
*  field   : bitend
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Bit endianess of Appfifo interface""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_appCfg0__appCfg0__bitend_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__bitend_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__bitend_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__bitend_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__bitend_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__bitend_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__bitend_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : appCfg0
*  register: appCfg0
*  field   : bytend
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Byte endianess of Appfifo interface""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_appCfg0__appCfg0__bytend_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__bytend_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__bytend_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__bytend_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__bytend_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__bytend_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__bytend_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : appCfg0
*  register: appCfg0
*  field   : statscor
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Statistic Counter Clear on Read""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_appCfg0__appCfg0__statscor_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__statscor_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__statscor_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__statscor_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_appCfg0__appCfg0__statscor_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_appCfg0__appCfg0__statscor_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_appCfg0__appCfg0__statscor_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : txpltesten
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pseudo-random Local Fault transmit testpattern""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__txpltesten_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txpltesten_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txpltesten_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txpltesten_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txpltesten_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txpltesten_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txpltesten_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : txp0testen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pseudo-random Data-0 transmit testpattern""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__txp0testen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txp0testen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txp0testen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txp0testen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txp0testen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txp0testen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txp0testen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : txsitesten
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Scrambled Idle transmit testpattern""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__txsitesten_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txsitesten_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txsitesten_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txsitesten_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txsitesten_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txsitesten_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txsitesten_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : txswtesten
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Squarewave transmit testpattern""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__txswtesten_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txswtesten_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txswtesten_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txswtesten_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__txswtesten_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__txswtesten_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__txswtesten_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : rxpltesten
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pseudorandom Localfault Testpattern checker""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__rxpltesten_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxpltesten_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__rxpltesten_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxpltesten_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__rxpltesten_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxpltesten_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__rxpltesten_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : rxp0testen
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Pseudorandom Data-0 Testpattern checker""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__rxp0testen_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxp0testen_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__rxp0testen_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxp0testen_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__rxp0testen_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxp0testen_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__rxp0testen_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test00
*  register: test0
*  field   : rxsitesten
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Scrambled Idle Testpattern checker""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test00__test0__rxsitesten_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxsitesten_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__rxsitesten_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxsitesten_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test00__test0__rxsitesten_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test00__test0__rxsitesten_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test00__test0__rxsitesten_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test1
*  register: test1
*  field   : seeda
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Seed A value for Psuedo-Random Testpattern""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test1__test1__seeda_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test1__test1__seeda_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test1__test1__seeda_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test1__test1__seeda_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test1__test1__seeda_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test1__test1__seeda_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test1__test1__seeda_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : test2
*  register: test2
*  field   : seedb
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Seed B value for Psuedo-Random Testpattern""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_test2__test2__seedb_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test2__test2__seedb_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test2__test2__seedb_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test2__test2__seedb_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_test2__test2__seedb_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_test2__test2__seedb_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_test2__test2__seedb_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : serdeslpbk
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Serdes Tx->RX loopback enable""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__serdeslpbk_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__serdeslpbk_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__serdeslpbk_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__serdeslpbk_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__serdeslpbk_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__serdeslpbk_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__serdeslpbk_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : txremap
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Transmit Serdes lane can be remapped to any other Serdes lane""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txremap_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txremap_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txremap_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txremap_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txremap_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txremap_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__txremap_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : sigokoverride
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Signal OK Override. 
* 00 : normal 
*01 invert 
* 10 force to 0 
* 11 force to 1""
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__sigokoverride_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__sigokoverride_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__sigokoverride_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__sigokoverride_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__sigokoverride_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__sigokoverride_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__sigokoverride_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : rxremap
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Receive Serdes lane can be remapped to any other Serdes lane""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxremap_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxremap_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxremap_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxremap_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxremap_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxremap_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__rxremap_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : txinv
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Invert data going out of this Serdes lane""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txinv_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txinv_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txinv_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txinv_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txinv_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txinv_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__txinv_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : rxinv
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Invert RX Serdes data""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxinv_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxinv_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxinv_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxinv_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxinv_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxinv_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__rxinv_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : paceren
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Controls Serdes Ready Pacer Select
* 1'b0: Normal
* 1'b1: Pacer Enabled""
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__paceren_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__paceren_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__paceren_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__paceren_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__paceren_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__paceren_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__paceren_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : pacerdiv
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Serdes ready pacer divide scaling value determined by: (((Channel_Rate * Overhead_Ratio) / #Serdes_Per_Channel) / (Core_Clock_Frequency * Serdes_Width)) * 2^16 
*Overhead_Ratio = 10/8 (1G and below),  68/64 (Modes with KP FEC),  66/64 (All other modes) 
*Example for 100GR4: (((100*10^9 * 66/64) / 4) / (825*10^6 * 40)) * 2^16 = 51200 
*Note: All serdes pacer divide scaling value registers associated with a channel must be set to the same value with the corresponding pacer enable bits set to 1'b1 when in use""
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__pacerdiv_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__pacerdiv_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__pacerdiv_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__pacerdiv_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__pacerdiv_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__pacerdiv_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__pacerdiv_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : txprbssel
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Select which RX PRBS polynomial is used. Valid cases: 31, 23, 15, 11,
* 9, 7.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txprbssel_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txprbssel_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txprbssel_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txprbssel_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__txprbssel_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__txprbssel_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__txprbssel_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sdcfg0
*  register: sdcfg
*  field   : rxprbssel
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Select which RX PRBS polynomial is used. Valid cases: 31, 23, 15, 11,
* 9, 7.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxprbssel_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxprbssel_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxprbssel_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxprbssel_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdcfg0__sdcfg__rxprbssel_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdcfg0__sdcfg__rxprbssel_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdcfg0__sdcfg__rxprbssel_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sderrcfg0
*  register: sderrcfg
*  field   : txerrperiod
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Error injection period. Period will be equal to (value*1024) bit
* times rounded down to the serdes width. Serdes width is set using TX
* PRBS Width.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sderrcfg0__sderrcfg__txerrperiod_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sderrcfg0__sderrcfg__txerrperiod_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sderrcfg0__sderrcfg__txerrperiod_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sderrcfg0__sderrcfg__txerrperiod_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sderrcfg0__sderrcfg__txerrperiod_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sderrcfg0__sderrcfg__txerrperiod_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sderrcfg0__sderrcfg__txerrperiod_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : sderrcfg0
*  register: sderrcfg
*  field   : txerrburst
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Error injection burst length: 0-127. Must be set to non-zero to
* inject errors.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sderrcfg0__sderrcfg__txerrburst_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sderrcfg0__sderrcfg__txerrburst_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sderrcfg0__sderrcfg__txerrburst_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sderrcfg0__sderrcfg__txerrburst_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sderrcfg0__sderrcfg__txerrburst_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sderrcfg0__sderrcfg__txerrburst_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sderrcfg0__sderrcfg__txerrburst_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-only
*  block   : sdsts0
*  register: sdsts
*  field   : txclkpresent
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* TX clock is present""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdsts0__sdsts__txclkpresent_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__txclkpresent_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : sdsts0
*  register: sdsts
*  field   : txclkrate
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* TX Clock rate (compared to core clock)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdsts0__sdsts__txclkrate_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__txclkrate_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : sdsts0
*  register: sdsts
*  field   : rxclkpresent
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* RX clock is present""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdsts0__sdsts__rxclkpresent_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__rxclkpresent_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : sdsts0
*  register: sdsts
*  field   : rxclkrate
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* RX serdes clock rate (compared to clock rate)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdsts0__sdsts__rxclkrate_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__rxclkrate_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : sdsts0
*  register: sdsts
*  field   : sigok
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Signal OK status""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdsts0__sdsts__sigok_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__sigok_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-write
*  block   : sdsts0
*  register: sdsts
*  field   : rxprbserrcnt
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*RX PRBS error count (ignores all-0 case,  autosync)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_sdsts0__sdsts__rxprbserrcnt_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__rxprbserrcnt_rd(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdsts0__sdsts__rxprbserrcnt_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__rxprbserrcnt_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_sdsts0__sdsts__rxprbserrcnt_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t serdes_lane, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_sdsts0__sdsts__rxprbserrcnt_rd(dev_id, physical_umac, serdes_lane, reg64, &unused_fld64, true);
  umac4_sdsts0__sdsts__rxprbserrcnt_wr(dev_id, physical_umac, serdes_lane, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam0
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam0__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam0__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam0__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam0__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam0__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam0__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam0__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam0
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam0__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam0__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam0__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam0__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam0__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam0__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam0__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam2
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam2__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam2__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam2__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam2__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam2__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam2__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam2__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam2
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam2__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam2__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam2__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam2__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam2__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam2__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam2__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam3
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam3__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam3__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam3__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam3__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam3__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam3__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam3__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam3
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam3__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam3__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam3__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam3__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam3__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam3__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam3__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam4
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam4__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam4__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam4__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam4__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam4__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam4__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam4__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam4
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam4__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam4__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam4__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam4__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam4__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam4__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam4__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam5
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam5__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam5__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam5__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam5__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam5__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam5__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam5__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam5
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam5__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam5__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam5__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam5__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam5__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam5__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam5__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam6
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam6__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam6__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam6__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam6__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam6__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam6__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam6__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam6
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam6__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam6__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam6__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam6__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam6__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam6__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam6__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam7
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam7__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam7__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam7__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam7__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam7__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam7__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam7__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam7
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam7__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam7__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam7__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam7__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam7__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam7__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam7__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam8
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam8__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam8__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam8__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam8__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam8__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam8__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam8__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam8
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam8__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam8__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam8__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam8__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam8__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam8__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam8__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam9
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam9__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam9__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam9__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam9__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam9__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam9__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam9__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam9
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam9__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam9__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam9__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam9__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam9__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam9__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam9__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamA
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamA__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamA__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamA__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamA__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamA__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamA__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamA__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamA
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamA__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamA__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamA__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamA__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamA__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamA__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamA__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamB
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamB__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamB__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamB__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamB__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamB__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamB__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamB__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamB
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamB__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamB__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamB__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamB__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamB__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamB__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamB__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamC
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamC__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamC__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamC__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamC__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamC__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamC__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamC__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamC
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamC__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamC__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamC__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamC__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamC__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamC__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamC__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamD
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamD__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamD__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamD__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamD__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamD__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamD__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamD__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamD
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamD__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamD__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamD__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamD__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamD__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamD__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamD__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamE
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamE__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamE__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamE__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamE__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamE__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamE__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamE__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamE
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamE__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamE__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamE__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamE__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamE__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamE__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamE__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamF
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamF__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamF__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamF__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamF__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamF__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamF__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamF__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progamF
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progamF__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamF__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamF__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamF__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progamF__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progamF__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progamF__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam10
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam10__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam10__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam10__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam10__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam10__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam10__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam10__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam10
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam10__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam10__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam10__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam10__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam10__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam10__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam10__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam11
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam11__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam11__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam11__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam11__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam11__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam11__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam11__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam11
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam11__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam11__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam11__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam11__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam11__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam11__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam11__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam12
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam12__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam12__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam12__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam12__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam12__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam12__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam12__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam12
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam12__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam12__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam12__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam12__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam12__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam12__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam12__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam13
*  register: progam
*  field   : am
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Maker Override Bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam13__progam__am_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam13__progam__am_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam13__progam__am_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam13__progam__am_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam13__progam__am_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam13__progam__am_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam13__progam__am_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam13
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam13__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam13__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam13__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam13__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam13__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam13__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam13__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam14
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam14__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam14__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam14__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam14__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam14__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam14__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam14__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam15
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam15__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam15__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam15__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam15__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam15__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam15__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam15__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam16
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam16__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam16__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam16__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam16__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam16__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam16__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam16__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam17
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam17__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam17__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam17__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam17__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam17__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam17__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam17__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam18
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam18__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam18__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam18__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam18__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam18__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam18__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam18__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam19
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam19__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam19__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam19__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam19__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam19__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam19__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam19__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1A
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1A__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1A__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1A__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1A__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1A__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1A__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1A__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1B
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1B__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1B__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1B__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1B__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1B__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1B__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1B__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1C
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1C__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1C__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1C__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1C__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1C__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1C__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1C__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1D
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1D__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1D__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1D__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1D__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1D__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1D__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1D__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1E
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1E__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1E__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1E__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1E__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1E__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1E__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1E__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : progam1F
*  register: progam
*  field   : bip
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* BIP value override bank""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_progam1F__progam__bip_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1F__progam__bip_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1F__progam__bip_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1F__progam__bip_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_progam1F__progam__bip_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_progam1F__progam__bip_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_progam1F__progam__bip_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig60
*  register: chconfig6
*  field   : corrbyp
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Bypass FEC correction""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig60__chconfig6__corrbyp_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__corrbyp_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__corrbyp_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__corrbyp_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__corrbyp_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__corrbyp_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__corrbyp_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig60
*  register: chconfig6
*  field   : indibyp
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Bypass FEC indication""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig60__chconfig6__indibyp_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__indibyp_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__indibyp_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__indibyp_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__indibyp_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__indibyp_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__indibyp_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig60
*  register: chconfig6
*  field   : hiserthresh
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* HiSER symbol error threshold.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig60__chconfig6__hiserthresh_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__hiserthresh_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__hiserthresh_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__hiserthresh_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__hiserthresh_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__hiserthresh_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__hiserthresh_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig60
*  register: chconfig6
*  field   : degserenable
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Enable Degraded SER generation""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig60__chconfig6__degserenable_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__degserenable_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__degserenable_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__degserenable_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__degserenable_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__degserenable_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__degserenable_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig60
*  register: chconfig6
*  field   : degserinterval
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Degraded SER window interval""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig60__chconfig6__degserinterval_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__degserinterval_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__degserinterval_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__degserinterval_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig60__chconfig6__degserinterval_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig60__chconfig6__degserinterval_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig60__chconfig6__degserinterval_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig70
*  register: chconfig7
*  field   : degseractivatethresh
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Degraded SER active threshold""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig70__chconfig7__degseractivatethresh_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig70__chconfig7__degseractivatethresh_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig70__chconfig7__degseractivatethresh_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig70__chconfig7__degseractivatethresh_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig70__chconfig7__degseractivatethresh_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig70__chconfig7__degseractivatethresh_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig70__chconfig7__degseractivatethresh_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : chconfig70
*  register: chconfig7
*  field   : degserdeactivatethresh
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Degraded SER inactive threshold (hyst)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig70__chconfig7__degserdeactivatethresh_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig70__chconfig7__degserdeactivatethresh_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig70__chconfig7__degserdeactivatethresh_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig70__chconfig7__degserdeactivatethresh_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig70__chconfig7__degserdeactivatethresh_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig70__chconfig7__degserdeactivatethresh_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig70__chconfig7__degserdeactivatethresh_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : pcsrxoverride00
*  register: pcsrxoverride0
*  field   : rxoverride0
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Reserved Override controls
* [63:62] Reserved
* [61:44] AM count per virtual lane
* [43: 0]  Reserved""
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_pcsrxoverride00__pcsrxoverride0__rxoverride0_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : pcsrxoverride01
*  register: pcsrxoverride1
*  field   : rxoverride1
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
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
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_pcsrxoverride01__pcsrxoverride1__rxoverride1_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : pcsrxoverride02
*  register: pcsrxoverride2
*  field   : rxoverride2
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
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
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_pcsrxoverride02__pcsrxoverride2__rxoverride2_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-only
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : amlock
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Lock Status""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcslcfg0__pcslcfg__amlock_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcslcfg0__pcslcfg__amlock_rd(dev_id, physical_umac, virtual_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : blocklock
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Block Lock Status""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcslcfg0__pcslcfg__blocklock_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcslcfg0__pcslcfg__blocklock_rd(dev_id, physical_umac, virtual_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : mapping
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Marker Mapping Recieved""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcslcfg0__pcslcfg__mapping_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcslcfg0__pcslcfg__mapping_rd(dev_id, physical_umac, virtual_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-only
*  block   : pcslcfg0
*  register: pcslcfg
*  field   : amperiod
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Alignment Marker Period""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_pcslcfg0__pcslcfg__amperiod_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t virtual_lane, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_pcslcfg0__pcslcfg__amperiod_rd(dev_id, physical_umac, virtual_lane, reg64, fld64, hw);
  return BF_SUCCESS;
}

/*******************************************************************
*  access  : read-write
*  block   : chconfig130
*  register: chconfig13
*  field   : txtsoffset
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Timestamp Offset (signed)""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_chconfig130__chconfig13__txtsoffset_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig130__chconfig13__txtsoffset_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig130__chconfig13__txtsoffset_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig130__chconfig13__txtsoffset_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_chconfig130__chconfig13__txtsoffset_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_chconfig130__chconfig13__txtsoffset_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_chconfig130__chconfig13__txtsoffset_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : version
*  register: version
*  field   : version
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Product Version""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_version__version__version_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_version__version__version_rd(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_version__version__version_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_version__version__version_wr(dev_id, physical_umac, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_version__version__version_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_version__version__version_rd(dev_id, physical_umac, reg64, &unused_fld64, true);
  umac4_version__version__version_wr(dev_id, physical_umac, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : intcontrol0
*  register: intcontrol
*  field   : intsts
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*Interrupt status,  also serves as interrupt override.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intsts_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intsts_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intsts_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intsts_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intsts_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intsts_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_intcontrol0__intcontrol__intsts_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : intcontrol0
*  register: intcontrol
*  field   : intclr
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Write 1 to clear interrupt status and intraw.""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intclr_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intclr_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intclr_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intclr_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intclr_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intclr_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_intcontrol0__intcontrol__intclr_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-write
*  block   : intcontrol0
*  register: intcontrol
*  field   : intena
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
*1'b1 Interrupt is enabled 
* 1'b0 Interrupt is masked,  intsts value is masked""
*
*******************************************************************/
bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intena_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intena_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intena_wr(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intena_wr(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intena_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t  fld64) {
  uint64_t unused_fld64;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intena_rd(dev_id, physical_umac, channel, reg64, &unused_fld64, true);
  umac4_intcontrol0__intcontrol__intena_wr(dev_id, physical_umac, channel, reg64, fld64, true);
  return BF_SUCCESS;
}


/*******************************************************************
*  access  : read-only
*  block   : intcontrol0
*  register: intcontrol
*  field   : intraw
*----------:
*  dev_id  : logical chip identifier
*  umac    : logical UMAC4 identifier (1-32)
*  reg64   : u64 for resulting register value or contents to extract field from
*  fld64   : u64 for field inserted or extracted
*  hw      : true=read/write hw : false=insert/extract from reg64 (only)
*----------+
*
* Interrupt raw data""
* 
*
*******************************************************************/
bf_status_t bf_ll_umac4_intcontrol0__intcontrol__intraw_rd(bf_dev_id_t dev_id, uint32_t logical_umac, uint32_t channel, uint64_t *reg64, uint64_t *fld64, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  umac4_intcontrol0__intcontrol__intraw_rd(dev_id, physical_umac, channel, reg64, fld64, hw);
  return BF_SUCCESS;
}

