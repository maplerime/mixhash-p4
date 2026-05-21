; ============================================================================
; TITLE boot_code 
; ******  IMPORTANT ******
; This boot code supports running an application from a fixed
; location, LOC_APP_MAIN, and max size, LEN_APP_MAIN, defined in this file.
; Application program can be loaded (even) after the tv80 is booted
; at this location and tv80 can be triggered to execute it
; External application program, if not memory location independent,
; must be written to be resident from this address and within this range.
; 
; instructions for compiling:
;
; download z80pack-1.9.tgz from https://www.autometer.de/unix4fun/z80pack/ftp/
; z80asm assembler is located at z80pack-1.9/z80asm after untar
;
; z80asm -l tv80_boot.asm
;
; produces
;
; tv80_boot.bin and tv80_boot.lis
; ============================================================================
; /* clang-format off */
    ORG   0
    JP    Reset_hdlr
    ORG   102         ; 66H
    JP    NMI_hdlr

; Command codes
NO_REQ EQU 0
MEM_RD EQU 1
MEM_WR EQU 2
IO_RD  EQU 3
IO_WR  EQU 4
HLT_REQ EQU 5
JMP_REQ EQU 6

; Status values
IDLE   EQU 0
BUSY   EQU 1
DONE   EQU 2
HALTED  EQU 3
APP_RUNNING EQU 4
DEAD   EQU 250

; IO port definitions
IO_A7_0   EQU 0
IO_A15_8  EQU 1
IO_A23_16 EQU 2
IO_A31_24 EQU 3
IO_D7_0   EQU 4
IO_D15_8  EQU 5
IO_D23_16 EQU 6
IO_D31_24 EQU 7
IO_STS    EQU 8

; Maskable Interrupt Status definitions
INT_0_RCVD EQU 100
INT_1_RCVD EQU 101
INT_2_RCVD EQU 102
INT_3_RCVD EQU 103
INT_4_RCVD EQU 104
INT_5_RCVD EQU 105
INT_6_RCVD EQU 106
INT_7_RCVD EQU 107

; application program start location
LOC_APP_MAIN EQU 1024
;  last 1K bytes for debug log
LOC_DBG_LOG EQU  15360
; 14K bytes between LOC_APP_MAIN and DBG_LOG_LOC
LEN_APP_MAIN EQU (LOC_DBG_LOG - LOC_APP_MAIN)
; Stack goes from 254 -> 128
;
    ORG   128         ; 80
STK_BTM: DEFW 0
    ORG   254
STK_TOP: DEFW 0
;   ORG   256
;
; Maskable interrupt vector table
MI_TBL:
    DEFW     MI_0
    DEFW     MI_1
    DEFW     MI_2
    DEFW     MI_3
    DEFW     MI_4
    DEFW     MI_5
    DEFW     MI_6
    DEFW     MI_7

; Control and Status
command: DEFW 0 ; [] 0=no-req, 1=mem-rd, 2=mem-wr, 3=io-rd, 4=io-wr
         DEFW 0 ; [] pad to 32 bits for easier host access
status:  DEFW 0 ; [] 0=idle, 1=busy, 2=done, 100-107, mskable int, 250=NMI
         DEFW 0 ; [] pad to 32 bits for easier host access
a00_15:  DEFW 0 ; [] Address[31:0]
a16_31:  DEFW 0 ; []
d00_15:  DEFW 0 ; [] Data[31:0]
d16_31:  DEFW 0 ; []

; verification data
v_command: DEFW 0 ; [] Last command code processed
v_a00_15:  DEFW 0 ; [] Last requested Address[31:0]
v_a16_31:  DEFW 0 ; []
v_d00_15:  DEFW 0 ; [] Write-only, Last requested data
v_d16_31:  DEFW 0 ; []

;
; POST variables
;
post_rd8:   DEFW 17   ;  0x11
post_wr8:   DEFW 0    ;  should be 0x11 after POST
post_rd16:  DEFW 4369 ;  0x1111
wr16:       DEFW 0    ;  should be 0x1111 after POST
wr16_2      DEFW 0    ;  should be 0x2222 after POST
wr16_3      DEFW 0    ;  should be 0x4444 after POST
wr16_4      DEFW 0    ;  should be 0x8888 after POST

; Command handlers
;
MemRd:
;
; save address for verification
;
    LD    HL,(a00_15)
    LD    (v_a00_15), HL
    LD    HL,(a16_31)
    LD    (v_a16_31), HL
; source addr
    LD    HL,(a00_15)
; dest addr
    LD    DE, d00_15
    LD    BC, 4
    LDIR     
    JP    Cmd_done 

MemWr:
;
; save address for verification
;
    LD    HL,(a00_15)
    LD    (v_a00_15), HL
    LD    HL,(a16_31)
    LD    (v_a16_31), HL
;
; save data for verification
;
    LD    HL,(d00_15)
    LD    (v_d00_15), HL
    LD    HL,(d16_31)
    LD    (v_d16_31), HL
; source addr
    LD    HL, d00_15
; dest addr
    LD    DE,(a00_15)
    LD    BC, 4
    LDIR     
    JP    Cmd_done

IoRd:
;
; save address for verification
;
    LD    HL,(a00_15)
    LD    (v_a00_15), HL
    LD    HL,(a16_31)
    LD    (v_a16_31), HL
; source addr
    LD    HL, a00_15
; dest addr
    LD    DE, d00_15
    LD    A, (hl)
    OUT   (IO_A7_0), A
    INC   HL
    LD    A, (hl)
    OUT   (IO_A15_8), A
    INC   HL
    LD    A, (hl)
    OUT   (IO_A23_16), A
    INC   HL
    LD    A, (hl)
    OUT   (IO_A31_24), A
    ;
    IN    A, (IO_D7_0)
    LD    (de), A
    INC   DE
    IN    A, (IO_D15_8)
    LD    (de), A
    INC   DE
    IN    A, (IO_D23_16)
    LD    (de), A
    INC   DE
    IN    A, (IO_D31_24)
    LD    (de), A
    INC   DE
    JP    Cmd_done

IoWr:
;
; save address for verification
;
    LD    HL,(a00_15)
    LD    (v_a00_15), HL
    LD    HL,(a16_31)
    LD    (v_a16_31), HL
;
; save data for verification
;
    LD    HL,(d00_15)
    LD    (v_d00_15), HL
    LD    HL,(d16_31)
    LD    (v_d16_31), HL
; source addr
    LD    HL, a00_15
; dest addr
    LD    DE, d00_15
    LD    A, (hl)
    OUT   (IO_A7_0), A
    INC   HL
    LD    A, (hl)
    OUT   (IO_A15_8), A
    INC   HL
    LD    A, (hl)
    OUT   (IO_A23_16), A
    INC   HL
    LD    A, (hl)
    OUT   (IO_A31_24), A
    ;
    LD    A, (de)
    OUT   (IO_D7_0), A
    INC   DE
    LD    A, (de)
    OUT   (IO_D15_8), A
    INC   DE
    LD    A, (de)
    OUT   (IO_D23_16), A
    INC   DE
    LD    A, (de)
    OUT   (IO_D31_24), A
    JP    Cmd_done

HltCmd:
    ; save the status and command before halting
    DI
    LD    A, NO_REQ
    LD    (command), A
    LD    A, HALTED 
    LD    (status), A
    HALT

JmpAppMain:
   
    ; save the status and command before halting
    LD    A, JMP_REQ
    LD    (command), A
    LD    A, APP_RUNNING 
    LD    (status), A
    CALL  LOC_APP_MAIN
 
    JP    Cmd_done ; jump to boot level command loop if the program returns

VectTbl:
    DEFW     MemRd
    DEFW     MemWr
    DEFW     IoRd
    DEFW     IoWr
    DEFW     HltCmd 
    DEFW     JmpAppMain 

Reset_hdlr:
;
; POST (Power-On Self-Test)
;
; first, 8b read/write
;
    LD    HL,post_rd8
    LD    A, (HL)
    LD    HL,post_wr8 
    LD    (HL), A
;
; Next, 16b read/write
;
    LD    HL,(post_rd16)
    LD    (wr16), HL
;
; Finally, multiple 16b writes
;
    SLA   H
    SLA   L
    LD    (wr16_2), HL
    SLA   H
    SLA   L
    LD    (wr16_3), HL
    SLA   H
    SLA   L
    LD    (wr16_4), HL
;
; Check POST results
;
    LD    HL, post_wr8
    LD    A, (HL)
    CP    17
    JP    Z, ok_8b
    HALT                 ; 8bit read/write failed
ok_8b:
    LD    HL, wr16
    LD    A, (HL)
    CP    17
    JP    Z, ok_16b_lo
    HALT                 ; 16bit (low) read/write failed
ok_16b_lo:
    INC   HL
    LD    A, (HL)
    CP    17
    JP    Z, ok_16b_hi
    HALT                 ; 16bit (high) read/write failed
    
ok_16b_hi:
    LD    HL, wr16_2
    LD    A, (HL)
    CP    34
    JP    Z, ok216blo
    HALT                 ; 16bit (low) read/write failed
ok216blo:
    INC   HL
    LD    A, (HL)
    CP    34
    JP    Z, ok_2_16b_hi
    HALT                 ; 16bit (high) read/write failed
ok_2_16b_hi:
    LD    HL, wr16_3
    LD    A, (HL)
    CP    68
    JP    Z, ok_3_16b_lo
    HALT                 ; 16bit (low) read/write failed
ok_3_16b_lo:
    INC   HL
    LD    A, (HL)
    CP    68
    JP    Z, ok316bhi
    HALT                 ; 16bit (high) read/write failed
ok316bhi:
    LD    HL, wr16_4
    LD    A, (HL)
    CP    136
    JP    Z, ok_4_16b_lo
    HALT                 ; 16bit (low) read/write failed
ok_4_16b_lo:
    INC   HL
    LD    A, (HL)
    CP    136
    JP    Z, ok416bhi
    HALT                 ; 16bit (high) read/write failed
ok416bhi:
;
; POST complete
;
; Set up Mode 2 interrupt vector table
    IM    2
    LD    A,MI_TBL/256
    LD    I, A
; Set up stack for itnerrupts
    LD    SP, STK_TOP
    LD    A, NO_REQ
    LD    (command), A
    LD    A, IDLE
    LD    (status), A
;
; Enable interrupts
    EI
    JP    Main
Cmd_done:
    LD    A, NO_REQ
    LD    (command), A
    LD    A, DONE
    LD    (status), A  
Main:
    LD    A,(command)
    CP    NO_REQ
    JP    Z, Main    ; no req=0
;
; Save command for verification
;
    LD    (v_command), A
;
; Make 0-based index for jump table
; 
    DEC   A
;
; Indicate busy
;
    LD    C, BUSY
    LD    HL, status
    LD    (hl), C   
;
; Call function based on code in A
; A = RPC code
fn_call:
    LD    H, 0
    LD    L, A
    ADD   HL, HL
    LD    DE, VectTbl
    ADD   HL, DE
    LD    A, (HL)
    INC   HL
    LD    H, (HL)
    LD    L, A
    JP    (HL)
; Set Interrupt code i status register for verification
NMI_hdlr:
    LD   A, DEAD
    LD   (status), A
    JP   Main
; Set Interrupt code in status register for verification
Set_Int_Status:
    LD   (status), A
    POP  AF
    RETI
; Interrupt handlers. Just set a code in A and jump to Set_Int_status
; which places the code in the stauts register
MI_0:
    PUSH AF
    LD   A, INT_0_RCVD
    JP   Set_Int_Status
MI_1:
    PUSH AF
    LD   A, INT_1_RCVD
    JP   Set_Int_Status
MI_2:
    PUSH AF
    LD   A, INT_2_RCVD
    JP   Set_Int_Status
MI_3:
    PUSH AF
    LD   A, INT_3_RCVD
    JP   Set_Int_Status
MI_4:
    PUSH AF
    LD   A, INT_4_RCVD
    JP   Set_Int_Status
MI_5:
    PUSH AF
    LD   A, INT_5_RCVD
    JP   Set_Int_Status
MI_6:
    PUSH AF
    LD   A, INT_6_RCVD
    JP   Set_Int_Status
MI_7:
    PUSH AF
    LD   A, INT_7_RCVD
    JP   Set_Int_Status
    END
