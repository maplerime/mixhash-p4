/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#define width_msk(width) ((uint64_t)(((uint64_t)0xffffffffffffffffull) >> (64ull - (uint64_t)width)))
#define fld_msk( bit, width ) (width_msk(width) << bit)

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
uint64_t get_fld_chmode0__chmode__mode( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 6ull ));
}

uint64_t set_fld_chmode0__chmode__mode( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 6ull )) |
                ((fld_val & width_msk( 6ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__txswrst( uint64_t *reg_val )
{
    return( ((*reg_val) >> 6ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__txswrst( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 6ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 6ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__rxswrst( uint64_t *reg_val )
{
    return( ((*reg_val) >> 7ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__rxswrst( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 7ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 7ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__txen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 8ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__txen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 8ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 8ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__txdrain( uint64_t *reg_val )
{
    return( ((*reg_val) >> 9ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__txdrain( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 9ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 9ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__rxen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 10ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__rxen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 10ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 10ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__gmiilpbk( uint64_t *reg_val )
{
    return( ((*reg_val) >> 11ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__gmiilpbk( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 11ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 11ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__txjabber( uint64_t *reg_val )
{
    return( ((*reg_val) >> 12ull) & width_msk( 16ull ));
}

uint64_t set_fld_chmode0__chmode__txjabber( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 12ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 12ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__rxjabber( uint64_t *reg_val )
{
    return( ((*reg_val) >> 28ull) & width_msk( 16ull ));
}

uint64_t set_fld_chmode0__chmode__rxjabber( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 28ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 28ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__disfcs( uint64_t *reg_val )
{
    return( ((*reg_val) >> 44ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__disfcs( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 44ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 44ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__invfcs( uint64_t *reg_val )
{
    return( ((*reg_val) >> 45ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__invfcs( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 45ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 45ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__ignfcs( uint64_t *reg_val )
{
    return( ((*reg_val) >> 46ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__ignfcs( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 46ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 46ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__stripfcs( uint64_t *reg_val )
{
    return( ((*reg_val) >> 47ull) & width_msk( 1ull ));
}

uint64_t set_fld_chmode0__chmode__stripfcs( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 47ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 47ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__ifglen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 48ull) & width_msk( 8ull ));
}

uint64_t set_fld_chmode0__chmode__ifglen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 48ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 48ull) );
    return *reg_val;
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
uint64_t get_fld_chmode0__chmode__ifgpacing( uint64_t *reg_val )
{
    return( ((*reg_val) >> 56ull) & width_msk( 8ull ));
}

uint64_t set_fld_chmode0__chmode__ifgpacing( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 56ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 56ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__disfcsonerr( uint64_t *reg_val )
{
    return( ((*reg_val) >> 5ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__disfcsonerr( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 5ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 5ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txfcen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 8ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__txfcen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 8ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 8ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxfcen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 10ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxfcen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 10ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 10ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxpfcen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 11ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxpfcen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 11ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 11ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxfctotx( uint64_t *reg_val )
{
    return( ((*reg_val) >> 12ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxfctotx( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 12ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 12ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxfilterfc( uint64_t *reg_val )
{
    return( ((*reg_val) >> 13ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxfilterfc( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 13ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 13ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxfilterpfc( uint64_t *reg_val )
{
    return( ((*reg_val) >> 14ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxfilterpfc( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 14ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 14ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txpadrunt( uint64_t *reg_val )
{
    return( ((*reg_val) >> 15ull) & width_msk( 8ull ));
}

uint64_t set_fld_maccfg0__maccfg__txpadrunt( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 15ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 15ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txwrthresh( uint64_t *reg_val )
{
    return( ((*reg_val) >> 23ull) & width_msk( 8ull ));
}

uint64_t set_fld_maccfg0__maccfg__txwrthresh( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 23ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 23ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txrdthresh( uint64_t *reg_val )
{
    return( ((*reg_val) >> 31ull) & width_msk( 8ull ));
}

uint64_t set_fld_maccfg0__maccfg__txrdthresh( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 31ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 31ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txlfault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 39ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__txlfault( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 39ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 39ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txrfault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 40ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__txrfault( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 40ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 40ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txidle( uint64_t *reg_val )
{
    return( ((*reg_val) >> 41ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__txidle( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 41ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 41ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxpadrunt( uint64_t *reg_val )
{
    return( ((*reg_val) >> 42ull) & width_msk( 8ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxpadrunt( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 42ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 42ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxlfault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 50ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxlfault( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 50ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 50ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxrfault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 51ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxrfault( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 51ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 51ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__rxidle( uint64_t *reg_val )
{
    return( ((*reg_val) >> 52ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__rxidle( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 52ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 52ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__statsclr( uint64_t *reg_val )
{
    return( ((*reg_val) >> 54ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__statsclr( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 54ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 54ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txignorerx( uint64_t *reg_val )
{
    return( ((*reg_val) >> 55ull) & width_msk( 1ull ));
}

uint64_t set_fld_maccfg0__maccfg__txignorerx( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 55ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 55ull) );
    return *reg_val;
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
uint64_t get_fld_maccfg0__maccfg__txpfcen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 56ull) & width_msk( 8ull ));
}

uint64_t set_fld_maccfg0__maccfg__txpfcen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 56ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 56ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig120__chconfig12__txvlantag( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig120__chconfig12__txvlantag( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig30__chconfig3__ifgppm( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig30__chconfig3__ifgppm( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig30__chconfig3__rxmaxfrmsize( uint64_t *reg_val )
{
    return( ((*reg_val) >> 16ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig30__chconfig3__rxmaxfrmsize( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 16ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 16ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig30__chconfig3__txpreamble( uint64_t *reg_val )
{
    return( ((*reg_val) >> 32ull) & width_msk( 5ull ));
}

uint64_t set_fld_chconfig30__chconfig3__txpreamble( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 32ull, 5ull )) |
                ((fld_val & width_msk( 5ull )) << 32ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig30__chconfig3__txdrainonfault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 37ull) & width_msk( 1ull ));
}

uint64_t set_fld_chconfig30__chconfig3__txdrainonfault( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 37ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 37ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig30__chconfig3__rxpreamble( uint64_t *reg_val )
{
    return( ((*reg_val) >> 38ull) & width_msk( 1ull ));
}

uint64_t set_fld_chconfig30__chconfig3__rxpreamble( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 38ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 38ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig30__chconfig3__rxerrmask( uint64_t *reg_val )
{
    return( ((*reg_val) >> 39ull) & width_msk( 5ull ));
}

uint64_t set_fld_chconfig30__chconfig3__rxerrmask( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 39ull, 5ull )) |
                ((fld_val & width_msk( 5ull )) << 39ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig40__chconfig4__macaddr( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 48ull ));
}

uint64_t set_fld_chconfig40__chconfig4__macaddr( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 48ull )) |
                ((fld_val & width_msk( 48ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig40__chconfig4__pauseontime( uint64_t *reg_val )
{
    return( ((*reg_val) >> 48ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig40__chconfig4__pauseontime( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 48ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 48ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig50__chconfig5__pausedest( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 48ull ));
}

uint64_t set_fld_chconfig50__chconfig5__pausedest( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 48ull )) |
                ((fld_val & width_msk( 48ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig50__chconfig5__pauserefresh( uint64_t *reg_val )
{
    return( ((*reg_val) >> 48ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig50__chconfig5__pauserefresh( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 48ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 48ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig310__chconfig31__macoverride( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 64ull ));
}

uint64_t set_fld_chconfig310__chconfig31__macoverride( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 64ull )) |
                ((fld_val & width_msk( 64ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chsts0__chsts__txclkpresentall( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__rxclkpresentall( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__rxsigokall( uint64_t *reg_val )
{
    return( ((*reg_val) >> 2ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__blocklockall( uint64_t *reg_val )
{
    return( ((*reg_val) >> 3ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__amlockall( uint64_t *reg_val )
{
    return( ((*reg_val) >> 4ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__aligned( uint64_t *reg_val )
{
    return( ((*reg_val) >> 5ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__nohiber( uint64_t *reg_val )
{
    return( ((*reg_val) >> 6ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__nolocalfault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 7ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__noremotefault( uint64_t *reg_val )
{
    return( ((*reg_val) >> 8ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__linkup( uint64_t *reg_val )
{
    return( ((*reg_val) >> 9ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__hiser( uint64_t *reg_val )
{
    return( ((*reg_val) >> 10ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__fecdegser( uint64_t *reg_val )
{
    return( ((*reg_val) >> 11ull) & width_msk( 1ull ));
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
uint64_t get_fld_chsts0__chsts__rxamsf( uint64_t *reg_val )
{
    return( ((*reg_val) >> 12ull) & width_msk( 3ull ));
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
uint64_t get_fld_chconfig80__chconfig8__vlantag1( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig80__chconfig8__vlantag1( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig80__chconfig8__vlantag2( uint64_t *reg_val )
{
    return( ((*reg_val) >> 16ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig80__chconfig8__vlantag2( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 16ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 16ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig80__chconfig8__vlantag3( uint64_t *reg_val )
{
    return( ((*reg_val) >> 32ull) & width_msk( 16ull ));
}

uint64_t set_fld_chconfig80__chconfig8__vlantag3( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 32ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 32ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig80__chconfig8__maxvlancnt( uint64_t *reg_val )
{
    return( ((*reg_val) >> 48ull) & width_msk( 2ull ));
}

uint64_t set_fld_chconfig80__chconfig8__maxvlancnt( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 48ull, 2ull )) |
                ((fld_val & width_msk( 2ull )) << 48ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig80__chconfig8__usclkcnt( uint64_t *reg_val )
{
    return( ((*reg_val) >> 50ull) & width_msk( 12ull ));
}

uint64_t set_fld_chconfig80__chconfig8__usclkcnt( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 50ull, 12ull )) |
                ((fld_val & width_msk( 12ull )) << 50ull) );
    return *reg_val;
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
uint64_t get_fld_appCfg0__appCfg0__afulltxinv( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
}

uint64_t set_fld_appCfg0__appCfg0__afulltxinv( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_appCfg0__appCfg0__afullrxinv( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 1ull ));
}

uint64_t set_fld_appCfg0__appCfg0__afullrxinv( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 1ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 1ull) );
    return *reg_val;
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
uint64_t get_fld_appCfg0__appCfg0__bitend( uint64_t *reg_val )
{
    return( ((*reg_val) >> 2ull) & width_msk( 1ull ));
}

uint64_t set_fld_appCfg0__appCfg0__bitend( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 2ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 2ull) );
    return *reg_val;
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
uint64_t get_fld_appCfg0__appCfg0__bytend( uint64_t *reg_val )
{
    return( ((*reg_val) >> 3ull) & width_msk( 1ull ));
}

uint64_t set_fld_appCfg0__appCfg0__bytend( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 3ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 3ull) );
    return *reg_val;
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
uint64_t get_fld_appCfg0__appCfg0__statscor( uint64_t *reg_val )
{
    return( ((*reg_val) >> 11ull) & width_msk( 1ull ));
}

uint64_t set_fld_appCfg0__appCfg0__statscor( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 11ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 11ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__txpltesten( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__txpltesten( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__txp0testen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__txp0testen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 1ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 1ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__txsitesten( uint64_t *reg_val )
{
    return( ((*reg_val) >> 2ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__txsitesten( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 2ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 2ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__txswtesten( uint64_t *reg_val )
{
    return( ((*reg_val) >> 3ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__txswtesten( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 3ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 3ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__rxpltesten( uint64_t *reg_val )
{
    return( ((*reg_val) >> 4ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__rxpltesten( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 4ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 4ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__rxp0testen( uint64_t *reg_val )
{
    return( ((*reg_val) >> 5ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__rxp0testen( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 5ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 5ull) );
    return *reg_val;
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
uint64_t get_fld_test00__test0__rxsitesten( uint64_t *reg_val )
{
    return( ((*reg_val) >> 6ull) & width_msk( 1ull ));
}

uint64_t set_fld_test00__test0__rxsitesten( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 6ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 6ull) );
    return *reg_val;
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
uint64_t get_fld_test1__test1__seeda( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 58ull ));
}

uint64_t set_fld_test1__test1__seeda( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 58ull )) |
                ((fld_val & width_msk( 58ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_test2__test2__seedb( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 58ull ));
}

uint64_t set_fld_test2__test2__seedb( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 58ull )) |
                ((fld_val & width_msk( 58ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__serdeslpbk( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__serdeslpbk( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__txremap( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 4ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__txremap( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 1ull, 4ull )) |
                ((fld_val & width_msk( 4ull )) << 1ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__sigokoverride( uint64_t *reg_val )
{
    return( ((*reg_val) >> 5ull) & width_msk( 2ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__sigokoverride( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 5ull, 2ull )) |
                ((fld_val & width_msk( 2ull )) << 5ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__rxremap( uint64_t *reg_val )
{
    return( ((*reg_val) >> 7ull) & width_msk( 4ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__rxremap( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 7ull, 4ull )) |
                ((fld_val & width_msk( 4ull )) << 7ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__txinv( uint64_t *reg_val )
{
    return( ((*reg_val) >> 13ull) & width_msk( 1ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__txinv( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 13ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 13ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__rxinv( uint64_t *reg_val )
{
    return( ((*reg_val) >> 14ull) & width_msk( 1ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__rxinv( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 14ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 14ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__paceren( uint64_t *reg_val )
{
    return( ((*reg_val) >> 15ull) & width_msk( 1ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__paceren( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 15ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 15ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__pacerdiv( uint64_t *reg_val )
{
    return( ((*reg_val) >> 16ull) & width_msk( 16ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__pacerdiv( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 16ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 16ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__txprbssel( uint64_t *reg_val )
{
    return( ((*reg_val) >> 32ull) & width_msk( 3ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__txprbssel( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 32ull, 3ull )) |
                ((fld_val & width_msk( 3ull )) << 32ull) );
    return *reg_val;
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
uint64_t get_fld_sdcfg0__sdcfg__rxprbssel( uint64_t *reg_val )
{
    return( ((*reg_val) >> 45ull) & width_msk( 3ull ));
}

uint64_t set_fld_sdcfg0__sdcfg__rxprbssel( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 45ull, 3ull )) |
                ((fld_val & width_msk( 3ull )) << 45ull) );
    return *reg_val;
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
uint64_t get_fld_sderrcfg0__sderrcfg__txerrperiod( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 9ull ));
}

uint64_t set_fld_sderrcfg0__sderrcfg__txerrperiod( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 9ull )) |
                ((fld_val & width_msk( 9ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_sderrcfg0__sderrcfg__txerrburst( uint64_t *reg_val )
{
    return( ((*reg_val) >> 9ull) & width_msk( 7ull ));
}

uint64_t set_fld_sderrcfg0__sderrcfg__txerrburst( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 9ull, 7ull )) |
                ((fld_val & width_msk( 7ull )) << 9ull) );
    return *reg_val;
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
uint64_t get_fld_sdsts0__sdsts__txclkpresent( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
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
uint64_t get_fld_sdsts0__sdsts__txclkrate( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 8ull ));
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
uint64_t get_fld_sdsts0__sdsts__rxclkpresent( uint64_t *reg_val )
{
    return( ((*reg_val) >> 9ull) & width_msk( 1ull ));
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
uint64_t get_fld_sdsts0__sdsts__rxclkrate( uint64_t *reg_val )
{
    return( ((*reg_val) >> 10ull) & width_msk( 8ull ));
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
uint64_t get_fld_sdsts0__sdsts__sigok( uint64_t *reg_val )
{
    return( ((*reg_val) >> 18ull) & width_msk( 1ull ));
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
uint64_t get_fld_sdsts0__sdsts__rxprbserrcnt( uint64_t *reg_val )
{
    return( ((*reg_val) >> 19ull) & width_msk( 16ull ));
}

uint64_t set_fld_sdsts0__sdsts__rxprbserrcnt( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 19ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 19ull) );
    return *reg_val;
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
uint64_t get_fld_progam0__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam0__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam0__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam0__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam1__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam1__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam2__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam2__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam2__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam2__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam3__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam3__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam3__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam3__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam4__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam4__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam4__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam4__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam5__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam5__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam5__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam5__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam6__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam6__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam6__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam6__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam7__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam7__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam7__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam7__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam8__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam8__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam8__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam8__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam9__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam9__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam9__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam9__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progamA__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progamA__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progamA__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progamA__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progamB__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progamB__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progamB__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progamB__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progamC__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progamC__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progamC__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progamC__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progamD__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progamD__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progamD__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progamD__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progamE__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progamE__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progamE__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progamE__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progamF__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progamF__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progamF__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progamF__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam10__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam10__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam10__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam10__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam11__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam11__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam11__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam11__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam12__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam12__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam12__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam12__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam13__progam__am( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 24ull ));
}

uint64_t set_fld_progam13__progam__am( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 24ull )) |
                ((fld_val & width_msk( 24ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_progam13__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam13__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam14__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam14__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam15__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam15__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam16__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam16__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam17__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam17__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam18__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam18__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam19__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam19__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1A__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1A__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1B__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1B__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1C__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1C__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1D__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1D__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1E__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1E__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_progam1F__progam__bip( uint64_t *reg_val )
{
    return( ((*reg_val) >> 24ull) & width_msk( 8ull ));
}

uint64_t set_fld_progam1F__progam__bip( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 24ull, 8ull )) |
                ((fld_val & width_msk( 8ull )) << 24ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig60__chconfig6__corrbyp( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
}

uint64_t set_fld_chconfig60__chconfig6__corrbyp( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig60__chconfig6__indibyp( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 1ull ));
}

uint64_t set_fld_chconfig60__chconfig6__indibyp( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 1ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 1ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig60__chconfig6__hiserthresh( uint64_t *reg_val )
{
    return( ((*reg_val) >> 2ull) & width_msk( 13ull ));
}

uint64_t set_fld_chconfig60__chconfig6__hiserthresh( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 2ull, 13ull )) |
                ((fld_val & width_msk( 13ull )) << 2ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig60__chconfig6__degserenable( uint64_t *reg_val )
{
    return( ((*reg_val) >> 15ull) & width_msk( 1ull ));
}

uint64_t set_fld_chconfig60__chconfig6__degserenable( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 15ull, 1ull )) |
                ((fld_val & width_msk( 1ull )) << 15ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig60__chconfig6__degserinterval( uint64_t *reg_val )
{
    return( ((*reg_val) >> 16ull) & width_msk( 32ull ));
}

uint64_t set_fld_chconfig60__chconfig6__degserinterval( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 16ull, 32ull )) |
                ((fld_val & width_msk( 32ull )) << 16ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig70__chconfig7__degseractivatethresh( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 32ull ));
}

uint64_t set_fld_chconfig70__chconfig7__degseractivatethresh( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 32ull )) |
                ((fld_val & width_msk( 32ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_chconfig70__chconfig7__degserdeactivatethresh( uint64_t *reg_val )
{
    return( ((*reg_val) >> 32ull) & width_msk( 32ull ));
}

uint64_t set_fld_chconfig70__chconfig7__degserdeactivatethresh( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 32ull, 32ull )) |
                ((fld_val & width_msk( 32ull )) << 32ull) );
    return *reg_val;
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
uint64_t get_fld_pcsrxoverride00__pcsrxoverride0__rxoverride0( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 64ull ));
}

uint64_t set_fld_pcsrxoverride00__pcsrxoverride0__rxoverride0( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 64ull )) |
                ((fld_val & width_msk( 64ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_pcsrxoverride01__pcsrxoverride1__rxoverride1( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 64ull ));
}

uint64_t set_fld_pcsrxoverride01__pcsrxoverride1__rxoverride1( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 64ull )) |
                ((fld_val & width_msk( 64ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_pcsrxoverride02__pcsrxoverride2__rxoverride2( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 64ull ));
}

uint64_t set_fld_pcsrxoverride02__pcsrxoverride2__rxoverride2( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 64ull )) |
                ((fld_val & width_msk( 64ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_pcslcfg0__pcslcfg__amlock( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 1ull ));
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
uint64_t get_fld_pcslcfg0__pcslcfg__blocklock( uint64_t *reg_val )
{
    return( ((*reg_val) >> 1ull) & width_msk( 1ull ));
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
uint64_t get_fld_pcslcfg0__pcslcfg__mapping( uint64_t *reg_val )
{
    return( ((*reg_val) >> 2ull) & width_msk( 5ull ));
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
uint64_t get_fld_pcslcfg0__pcslcfg__amperiod( uint64_t *reg_val )
{
    return( ((*reg_val) >> 7ull) & width_msk( 18ull ));
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
uint64_t get_fld_chconfig130__chconfig13__txtsoffset( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 32ull ));
}

uint64_t set_fld_chconfig130__chconfig13__txtsoffset( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 32ull )) |
                ((fld_val & width_msk( 32ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_version__version__version( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 64ull ));
}

uint64_t set_fld_version__version__version( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 64ull )) |
                ((fld_val & width_msk( 64ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_intcontrol0__intcontrol__intsts( uint64_t *reg_val )
{
    return( ((*reg_val) >> 0ull) & width_msk( 16ull ));
}

uint64_t set_fld_intcontrol0__intcontrol__intsts( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 0ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 0ull) );
    return *reg_val;
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
uint64_t get_fld_intcontrol0__intcontrol__intclr( uint64_t *reg_val )
{
    return( ((*reg_val) >> 16ull) & width_msk( 16ull ));
}

uint64_t set_fld_intcontrol0__intcontrol__intclr( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 16ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 16ull) );
    return *reg_val;
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
uint64_t get_fld_intcontrol0__intcontrol__intena( uint64_t *reg_val )
{
    return( ((*reg_val) >> 32ull) & width_msk( 16ull ));
}

uint64_t set_fld_intcontrol0__intcontrol__intena( uint64_t *reg_val, uint64_t fld_val )
{
    *reg_val= ( ((*reg_val) & ~fld_msk( 32ull, 16ull )) |
                ((fld_val & width_msk( 16ull )) << 32ull) );
    return *reg_val;
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
uint64_t get_fld_intcontrol0__intcontrol__intraw( uint64_t *reg_val )
{
    return( ((*reg_val) >> 48ull) & width_msk( 16ull ));
}

