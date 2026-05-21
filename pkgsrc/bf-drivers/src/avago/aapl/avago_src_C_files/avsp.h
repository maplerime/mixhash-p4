/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) support for AVSP */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for ASSP functions. */

#ifndef AVSP_H_
#define AVSP_H_

#if AAPL_ENABLE_AVSP

#ifndef SWIG

#define AVSP_1104_BROD_A    AVAGO_SERDES_BROADCAST_ADDRESS /* Broadcast address */
#define AVSP_1104_SBM_A     0xfd /* SBUS Master sbus rx address */
#define AVSP_1104_HOST_A    0xca /* Host slice broadcast address */
#define AVSP_1104_MOD4_A    0xcb /* 25G Mod slice broadcast address */
#define AVSP_1104_MOD6_A    0xcc /* 10G Mod slice broadcast address */
#define AVSP_1104_H10G0     0x0b /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G1     0x0a /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G2     0x09 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G3     0x08 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G4     0x07 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G5     0x06 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G6     0x05 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G7     0x04 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G8     0x03 /* Host slice sbus rx to name mapping */
#define AVSP_1104_H10G9     0x02 /* Host slice sbus rx to name mapping */
#define AVSP_1104_HOST_NUM    10 /* Number of host slices, followed by the full list in AVSP_1104_HOST_LIST */
#define AVSP_1104_HOST_LIST AVSP_1104_H10G0, AVSP_1104_H10G1, AVSP_1104_H10G2, AVSP_1104_H10G3, AVSP_1104_H10G4, AVSP_1104_H10G5, AVSP_1104_H10G6, AVSP_1104_H10G7, AVSP_1104_H10G8, AVSP_1104_H10G9
#define AVSP_1104_M25G0     0x11 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_1104_M25G1     0x12 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_1104_M25G2     0x13 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_1104_M25G3     0x14 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_1104_MOD4_NUM     4 /* Number of 25G Mod slices, followed by the full list in AVSP_1104_MOD4_LIST */
#define AVSP_1104_MOD4_LIST AVSP_1104_M25G0, AVSP_1104_M25G1, AVSP_1104_M25G2, AVSP_1104_M25G3
#define AVSP_1104_M10G4     0x0e /* 10G Mod slice sbus rx to name mapping */
#define AVSP_1104_M10G5     0x0f /* 10G Mod slice sbus rx to name mapping */
#define AVSP_1104_M10G6     0x10 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_1104_M10G7     0x15 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_1104_M10G8     0x16 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_1104_M10G9     0x17 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_1104_MOD6_NUM     6 /* Number of 10G Mod slices, followed by the full list in AVSP_1104_MOD6_LIST */
#define AVSP_1104_MOD6_LIST AVSP_1104_M10G4, AVSP_1104_M10G5, AVSP_1104_M10G6, AVSP_1104_M10G7, AVSP_1104_M10G8, AVSP_1104_M10G9
#define AVSP_1104_MOD_NUM     10 /* Total number of Mod slices, followed by the full list in AVSP_1104_MOD_LIST */
#define AVSP_1104_MOD_LIST  AVSP_1104_M25G0, AVSP_1104_M25G1, AVSP_1104_M25G2, AVSP_1104_M25G3, AVSP_1104_M10G4, AVSP_1104_M10G5, AVSP_1104_M10G6, AVSP_1104_M10G7, AVSP_1104_M10G8, AVSP_1104_M10G9

#define AVSP_9104_HOST_A    0xca /* Host slice broadcast address */
#define AVSP_9104_MOD4_A    0xcb /* 25G Mod slice broadcast address */
#define AVSP_9104_MOD6_A    0xcc /* 10G Mod slice broadcast address */
#define AVSP_9104_H10G0     0x1e /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G1     0x1d /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G2     0x1c /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G3     0x1b /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G4     0x20 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G5     0x1a /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G6     0x18 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G7     0x21 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G8     0x19 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G9     0x1f /* Host slice sbus rx to name mapping */
#define AVSP_9104_HOST_NUM    10 /* Number of host slices, followed by the full list in AVSP_9104_HOST_LIST */
#define AVSP_9104_HOST_LIST AVSP_9104_H10G0, AVSP_9104_H10G1, AVSP_9104_H10G2, AVSP_9104_H10G3, AVSP_9104_H10G4, AVSP_9104_H10G5, AVSP_9104_H10G6, AVSP_9104_H10G7, AVSP_9104_H10G8, AVSP_9104_H10G9
#define AVSP_9104_M25G0     0x08 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_M25G1     0x07 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_M25G2     0x06 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_M25G3     0x05 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_MOD4_NUM     4 /* Number of 25G Mod slices, followed by the full list in AVSP_9104_MOD4_LIST */
#define AVSP_9104_MOD4_LIST AVSP_9104_M25G0, AVSP_9104_M25G1, AVSP_9104_M25G2, AVSP_9104_M25G3
#define AVSP_9104_M10G4     0x0a /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G5     0x04 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G6     0x02 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G7     0x0b /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G8     0x03 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G9     0x09 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_MOD6_NUM     6 /* Number of 10G Mod slices, followed by the full list in AVSP_9104_MOD6_LIST */
#define AVSP_9104_MOD6_LIST AVSP_9104_M10G4, AVSP_9104_M10G5, AVSP_9104_M10G6, AVSP_9104_M10G7, AVSP_9104_M10G8, AVSP_9104_M10G9
#define AVSP_9104_MOD_NUM   10 /* Total number of Mod slices, followed by the full list in AVSP_9104_MOD_LIST */
#define AVSP_9104_MOD_LIST  AVSP_9104_M25G0, AVSP_9104_M25G1, AVSP_9104_M25G2, AVSP_9104_M25G3, AVSP_9104_M10G4, AVSP_9104_M10G5, AVSP_9104_M10G6, AVSP_9104_M10G7, AVSP_9104_M10G8, AVSP_9104_M10G9

#define AVSP_1104_SLIP_LIM  10 /* Number of bit-slips to try for alignment (5 is enough, but do two loops to be sure) */

#define AVSP_CHECK_BIT(var,pos) ((var) & (1<<(pos))) /* Quick routine for checking a single bit in a return value */
#define AVSP_ADDR_LOOP(ptr) for( ;*ptr != ~0U; ptr++)

#ifndef AVSP_1104_PMA_TYPE
#define AVSP_1104_PMA_TYPE 0x002A /* Set a valid PMA type value */
#endif

#ifndef AVSP_1104_RPT_TYPE
#define AVSP_1104_RPT_TYPE 0x0028 /* Set a valid REPEATER type value */
#endif

#ifndef AVSP_1104_SPEED_SEL
#define AVSP_1104_SPEED_SEL 0x204C /* Set a valid SPEED selection */
#endif


/* defines for AVSP-4412/8801 retimer products */
#define AVSP_4412_BROD_A AVAGO_SERDES_BROADCAST_ADDRESS /* Broadcast address */
#define AVSP_4412_SBM_A  0xfd /* SBUS Master sbus rx address */
#define AVSP_4412_SD7    0x04 /* slice sbus rx to name mapping */
#define AVSP_4412_SD6    0x05 /* slice sbus rx to name mapping */
#define AVSP_4412_SD5    0x06 /* slice sbus rx to name mapping */
#define AVSP_4412_SD4    0x07 /* slice sbus rx to name mapping */
#define AVSP_4412_SD3    0x08 /* slice sbus rx to name mapping */
#define AVSP_4412_SD2    0x09 /* slice sbus rx to name mapping */
#define AVSP_4412_SD1    0x0a /* slice sbus rx to name mapping */
#define AVSP_4412_SD0    0x0b /* slice sbus rx to name mapping */
#define AVSP_4412_NUM       8 /* Number of slices, followed by the full list in AVSP_4412_LIST */
#define AVSP_4412_LIST   AVSP_4412_SD0, AVSP_4412_SD1, AVSP_4412_SD2, AVSP_4412_SD3, AVSP_4412_SD4, AVSP_4412_SD5, AVSP_4412_SD6, AVSP_4412_SD7

#define AVSP_8801_BROD_A AVAGO_SERDES_BROADCAST_ADDRESS /* Broadcast address */
#define AVSP_8801_SBM_A  0xfd /* SBUS Master sbus rx address */
#define AVSP_8801_SD7    0x04 /* slice sbus rx to name mapping */
#define AVSP_8801_SD6    0x05 /* slice sbus rx to name mapping */
#define AVSP_8801_SD5    0x06 /* slice sbus rx to name mapping */
#define AVSP_8801_SD4    0x07 /* slice sbus rx to name mapping */
#define AVSP_8801_SD3    0x08 /* slice sbus rx to name mapping */
#define AVSP_8801_SD2    0x09 /* slice sbus rx to name mapping */
#define AVSP_8801_SD1    0x0a /* slice sbus rx to name mapping */
#define AVSP_8801_SD0    0x0b /* slice sbus rx to name mapping */
#define AVSP_8801_NUM       8 /* Number of slices, followed by the full list in AVSP_8801_LIST */
#define AVSP_8801_LIST   AVSP_8801_SD0, AVSP_8801_SD1, AVSP_8801_SD2, AVSP_8801_SD3, AVSP_8801_SD4, AVSP_8801_SD5, AVSP_8801_SD6, AVSP_8801_SD7

/* defines for AVSP-7412 product */
#define AVSP_7412_BROD_A     0xE0 /* Broadcast address configured for all slices */
#define AVSP_7412_SBM_A      0xfd /* SBUS Master sbus rx address */
#define AVSP_7412_HOST_A     0xca /* Host slice broadcast address */
#define AVSP_7412_MOD_A      0xcb /* Mod  slice broadcast address */
#define AVSP_7412_MODP_A     0xee /* Mod PAM slice broadcast address */
#define AVSP_7412_SD0        0x04 /* Host slice sbus rx to name mapping */
#define AVSP_7412_SD1        0x05 /* Host slice sbus rx to name mapping */
#define AVSP_7412_SD2        0x06 /* Host slice sbus rx to name mapping */
#define AVSP_7412_RS528      0x0d /* KR encoder sbus rx to name mapping */
#define AVSP_7412_RS544      0x0e /* KR decoder sbus rx to name mapping */
#define AVSP_7412_CNTL       0x0f /* Control logic sbus rx to name mapping */
#define AVSP_7412_MOD_ALIGN  0x0a /* Control logic sbus rx to name mapping */
#define AVSP_7412_HOST_ALIGN 0x03 /* Control logic sbus rx to name mapping */
#define AVSP_7412_HOST_NUM     4  /* Number of host slices */
#define AVSP_7412_MOD4TO2_NUM  2  /* Number of mod slices in 4to2 mode */
#define AVSP_7412_MOD4TO4_NUM  4  /* Number of mod slices in 4to4 mode */
#define AVSP_7412_HOST_LIST AVSP_7412_SD0
#define AVSP_7412_MOD_LIST  AVSP_7412_SD1, AVSP_7412_SD2
#define AVSP_7412_LIST      AVSP_7412_HOST_LIST, AVSP_7412_MOD_LIST


/* defines for AVSP-5410 product */
#define AVSP_5410_BROD_A     AVAGO_SERDES_BROADCAST_ADDRESS /* Broadcast address configured for all slices */
#define AVSP_5410_SBM_A      0xfd /* SBUS Master sbus rx address */
#define AVSP_5410_SD0        0x009 /* Host slice sbus rx to name mapping */
#define AVSP_5410_SD1        0x008 /* Host slice sbus rx to name mapping */
#define AVSP_5410_SD2        0x007 /* Host slice sbus rx to name mapping */
#define AVSP_5410_SD3        0x006 /* Host slice sbus rx to name mapping */
#define AVSP_5410_SD4        0x005 /* Mod PAM slice sbus rx to name mapping */
/*#define AVSP_5410_SD5        0x004 // Mod PAM slice sbus rx to name mapping */
#define AVSP_5410_HOST_LIST AVSP_5410_SD0, AVSP_5410_SD1, AVSP_5410_SD2, AVSP_5410_SD3
#define AVSP_5410_MOD_LIST  AVSP_5410_SD4
#define AVSP_5410_LIST      AVSP_5410_HOST_LIST, AVSP_5410_MOD_LIST

/* defines for AVSP-8812 retimer products */
#define AVSP_8812_BROD_A    AVAGO_SERDES_BROADCAST_ADDRESS /* Broadcast address */
#define AVSP_8812_SBM_A       0xfd    /* SBUS Master sbus rx address */
#define AVSP_8812_SD15        0x04    /* slice sbus rx to name mapping */
#define AVSP_8812_SD14        0x05    /* slice sbus rx to name mapping */
#define AVSP_8812_SD13        0x06    /* slice sbus rx to name mapping */
#define AVSP_8812_SD12        0x07    /* slice sbus rx to name mapping */
#define AVSP_8812_SD11        0x08    /* slice sbus rx to name mapping */
#define AVSP_8812_SD10        0x09    /* slice sbus rx to name mapping */
#define AVSP_8812_SD9         0x0a    /* slice sbus rx to name mapping */
#define AVSP_8812_SD8         0x0b    /* slice sbus rx to name mapping */
#define AVSP_8812_SD7         0x0c    /* slice sbus rx to name mapping */
#define AVSP_8812_SD6         0x0d    /* slice sbus rx to name mapping */
#define AVSP_8812_SD5         0x0e    /* slice sbus rx to name mapping */
#define AVSP_8812_SD4         0x0f    /* slice sbus rx to name mapping */
#define AVSP_8812_SD3         0x10    /* slice sbus rx to name mapping */
#define AVSP_8812_SD2         0x11    /* slice sbus rx to name mapping */
#define AVSP_8812_SD1         0x12    /* slice sbus rx to name mapping */
#define AVSP_8812_SD0         0x13    /* slice sbus rx to name mapping */
#define AVSP_8812_NUM           16    /* Number of slices, followed by the full list in AVSP_8812_LIST */
#define AVSP_8812_HOST_LIST            AVSP_8812_SD0, AVSP_8812_SD1, AVSP_8812_SD2, AVSP_8812_SD3, AVSP_8812_SD4, AVSP_8812_SD5, AVSP_8812_SD6, AVSP_8812_SD7
#define AVSP_8812_MOD_LIST             AVSP_8812_SD8, AVSP_8812_SD9, AVSP_8812_SD10, AVSP_8812_SD11, AVSP_8812_SD12, AVSP_8812_SD13, AVSP_8812_SD14, AVSP_8812_SD15
#define AVSP_8812_MOD4_LIST            AVSP_8812_SD8, AVSP_8812_SD10, AVSP_8812_SD12, AVSP_8812_SD14
#define AVSP_8812_HOST4_LIST           AVSP_8812_SD0, AVSP_8812_SD2, AVSP_8812_SD4, AVSP_8812_SD6
#define AVSP_8812_MODX_LIST            AVSP_8812_SD9, AVSP_8812_SD11, AVSP_8812_SD13, AVSP_8812_SD15
#define AVSP_8812_HALF0_LIST     AVSP_8812_SD0, AVSP_8812_SD1, AVSP_8812_SD2, AVSP_8812_SD3, AVSP_8812_SD8, AVSP_8812_SD9, AVSP_8812_SD10, AVSP_8812_SD11
#define AVSP_8812_HALF1_LIST     AVSP_8812_SD4, AVSP_8812_SD5, AVSP_8812_SD6, AVSP_8812_SD7, AVSP_8812_SD12, AVSP_8812_SD13, AVSP_8812_SD14, AVSP_8812_SD15
#define AVSP_8812_GB01_HOST_MOD_LIST   AVSP_8812_SD0, AVSP_8812_SD1, AVSP_8812_SD2, AVSP_8812_SD3, AVSP_8812_SD8, AVSP_8812_SD10
#define AVSP_8812_GB23_HOST_MOD_LIST   AVSP_8812_SD4, AVSP_8812_SD5, AVSP_8812_SD6, AVSP_8812_SD7, AVSP_8812_SD12, AVSP_8812_SD14
#define AVSP_8812_GB01_MOD_HOST_LIST   AVSP_8812_SD0, AVSP_8812_SD2, AVSP_8812_SD8, AVSP_8812_SD9, AVSP_8812_SD10, AVSP_8812_SD11
#define AVSP_8812_GB23_MOD_HOST_LIST   AVSP_8812_SD4, AVSP_8812_SD6, AVSP_8812_SD12, AVSP_8812_SD13, AVSP_8812_SD14, AVSP_8812_SD15
#define AVSP_8812_LIST                 AVSP_8812_HOST_LIST, AVSP_8812_MOD_LIST
#define AVSP_8812_GB0_GB1_ADDRS_LIST   0x15,0x16 
#define AVSP_8812_GB2_GB3_ADDRS_LIST   0x17,0x18 
#define AVSP_8812_FEC_ADDRS_LIST   0x19,0x1a
#define AVSP_8812_GB0_ADDR   0x15
#define AVSP_8812_GB1_ADDR   0x16
#define AVSP_8812_GB2_ADDR   0x17
#define AVSP_8812_GB3_ADDR   0x18
#define AVSP_8812_FEC0_ADDR  0x19
#define AVSP_8812_FEC1_ADDR  0x1a
#define AVSP_8812_AN_LIST 0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b

/* defines for AVSP-9104 product */
#define AVSP_9104_BROD_A AVAGO_SERDES_BROADCAST_ADDRESS /* Broadcast address */
#define AVSP_9104_SBM_A    0xfd /* SBUS Master sbus rx address */
#define AVSP_9104_HOST_A   0xca /* Host slice broadcast address */
#define AVSP_9104_MOD4_A   0xcb /* 25G Mod slice broadcast address */
#define AVSP_9104_MOD6_A   0xcc /* 10G Mod slice broadcast address */
#define AVSP_9104_H10G0    0x1e /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G1    0x1d /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G2    0x1c /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G3    0x1b /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G4    0x20 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G5    0x1a /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G6    0x18 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G7    0x21 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G8    0x19 /* Host slice sbus rx to name mapping */
#define AVSP_9104_H10G9    0x1f /* Host slice sbus rx to name mapping */
#define AVSP_9104_HOST_NUM 10 /* Number of host slices, followed by the full list in AVSP_9104_HOST_LIST */
#define AVSP_9104_HOST_LIST AVSP_9104_H10G0, AVSP_9104_H10G1, AVSP_9104_H10G2, AVSP_9104_H10G3, AVSP_9104_H10G4, AVSP_9104_H10G5, AVSP_9104_H10G6, AVSP_9104_H10G7, AVSP_9104_H10G8, AVSP_9104_H10G9
#define AVSP_9104_M25G0  0x08 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_M25G1  0x07 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_M25G2  0x06 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_M25G3  0x05 /* 25G Mod slice sbus rx to name mapping */
#define AVSP_9104_MOD4_NUM 4 /* Number of 25G Mod slices, followed by the full list in AVSP_9104_MOD4_LIST */
#define AVSP_9104_MOD4_LIST AVSP_9104_M25G0, AVSP_9104_M25G1, AVSP_9104_M25G2, AVSP_9104_M25G3
#define AVSP_9104_M10G4  0x0a /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G5  0x04 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G6  0x02 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G7  0x0b /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G8  0x03 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_M10G9  0x09 /* 10G Mod slice sbus rx to name mapping */
#define AVSP_9104_MOD6_NUM 6 /* Number of 10G Mod slices, followed by the full list in AVSP_9104_MOD6_LIST */
#define AVSP_9104_MOD6_LIST AVSP_9104_M10G4, AVSP_9104_M10G5, AVSP_9104_M10G6, AVSP_9104_M10G7, AVSP_9104_M10G8, AVSP_9104_M10G9
#define AVSP_9104_MOD_NUM 10 /* Total number of Mod slices, followed by the full list in AVSP_9104_MOD_LIST */
#define AVSP_9104_MOD_LIST AVSP_9104_M25G0, AVSP_9104_M25G1, AVSP_9104_M25G2, AVSP_9104_M25G3, AVSP_9104_M10G4, AVSP_9104_M10G5, AVSP_9104_M10G6, AVSP_9104_M10G7, AVSP_9104_M10G8, AVSP_9104_M10G9
#define AVSP_9104_MOD_AN_LIST  0x0f,0x10,0x11,0x12,0x0d,0x13,0x15,0x0c,0x14,0x0e
#define AVSP_9104_HOST_AN_LIST 0x25,0x26,0x27,0x28,0x23,0x29,0x2b,0x22,0x2a,0x24

#endif /* SWIG */

#define AVSP_MAX_SLICE_COUNT 20       /* Most number of slices for stating. */

/** @brief The AVSP operating mode. */
typedef enum
{
    AVSP_PMA                 = 0, /**< Select the AVSP-1104 10:4  operating mode. */
    AVSP_GEARBOX_10_4        = 0, /**< Select the AVSP-1104 10:4  operating mode. */
    AVSP_RPT                 = 1, /**< Select the AVSP-1104 10:10 operating mode. */
    AVSP_REPEATER_DUPLEX     = 1, /**< Select the AVSP-1104 10:10 and AVSP-4412 operating mode. */
    AVSP_GEARBOX_10_4_RS_FEC = 2,
    AVSP_GEARBOX_10_4_MLG    = 3,
    AVSP_REPEATER_SIMPLEX    = 4, /**< Select the AVSP-8801 mode. */
    AVSP_GEARBOX_4_1         = 5,
    AVSP_ADHOC               = 6,
    AVSP_GEARBOX_2_1         = 7, /**<Select AVSP-8812 Gearbox 2:1 mode */
    AVSP_GEARBOX_2_1_MOD_HOST= 8, /**<Select AVSP-8812 Gearbox 1:2 mode */
    AVSP_RS_FEC_4x4          = 9, /**<Select AVSP-8812 FEC mode */
    AVSP_RS_FEC_528          = 10,/**<Select AVSP-7412 FEC mode */
    AVSP_RS_FEC_544          = 11,/**<Select AVSP-7412 FEC mode */
    AVSP_RS_FEC              = 12 /**<Select AVSP-5410 FEC mode */
} Avsp_mode_t;

#if AAPL_ENABLE_AVSP_AUTO_NEG
/** @brief  Options for auto-negotiation (AN). */
typedef struct
{
    uint host_mask;     /**< Mask that defines which slices on the Host side will AN */
    uint mod_mask;      /**< Mask that defines which slices on the Mod side will AN */
    uint an_speed;      /**< Defines what speed AN will occur at: */
                        /**< - 0x00 = 3.125GB/s */
                        /**< - 0x01 = 1.56 GB/s */
                        /**< - 0x02 = 1.25 GB/s */
    uint an_side;       /**< Boolean that determines whether AN will start on host (false) or mod (true) side */
    uint rx_clk;        /**< Use the ref_clk, instead of the RX recovered clock */
    uint n_ports;       /**< Number of ports plugged into ASSP */
    uint cap_in;        /**< Whether AN will use: */
                        /**< - 0 : Default capability list */
                        /**< - 1 : User input capability list */
                        /**< - 3 : MDIO capability list */
    uint user_cap;            /**< The user's capability list. (Use 0x000 if default or MDIO) */
    uint kr_training_disable; /**<  Whether or not KR training should be disabled after AN  */
    uint enable_symmetric;    /**<  Whether the KR training should be symmetric   */
} Avsp_an_config_t;

EXT Avsp_an_config_t *avsp_an_config_construct(Aapl_t *aapl);
EXT void avsp_an_config_destruct(Aapl_t *aapl,  Avsp_an_config_t *config);
EXT uint avsp_an_start(Aapl_t *aapl, uint chip, Avsp_an_config_t *config);
#endif /* AAPL_ENABLE_AVSP_AUTO_NEG */

#if AAPL_ENABLE_AVSP_KR_TRAINING
/** @brief  Options for KR Training. */
typedef struct
{
    uint                        channels;            /**< A bit pattern that defines which channels to */
                                                     /**< initiate training on. */
                                                     /**< Ex: 0x001 - Initiate training on channel 1 */
                                                     /**< Ex: 0x003 - Initiate training on channels 1 and 2 */
                                                     /**< Which serdes these channels affect is described in */
                                                     /**< the KR training section of the documentation. */
                                                     /**< Then their inverse is specified. */
                                                     /**< For instance, on a 4412, the channels are paired: */
                                                     /**< Channel 1 : Serdes 0 and Serdes 4 */
                                                     /**< Channel 2 : Serdes 1 and Serdes 5 */
                                                     /**< Channel 3 : Serdes 2 and Serdes 6 */
                                                     /**< Channel 4 : Serdes 3 and Serdes 7 */
                                                     /**< Channel 5 : Serdes 4 and Serdes 0 */
                                                     /**< Channel 6 : Serdes 5 and Serdes 1 */
                                                     /**< Channel 7 : Serdes 6 and Serdes 2 */
                                                     /**< Channel 8 : Serdes 7 and Serdes 3 */
    BOOL                        report_kr_failure;   /**< Report a KR failure. */
    BOOL                        report_frame_lock;   /**< Report a frame lock failure */
    BOOL                        report_signal_detect;/**< Report a signal detect failure */
    BOOL                        enable_symmetric;    /**< Attempt symmetric training */
    BOOL                        side_select;          /**< Chooses host verses mod for chips with more than ten channels in total */

} Avago_kr_config_t;

EXT Avago_kr_config_t *avago_kr_config_construct(Aapl_t *aapl);
EXT void avago_kr_config_destruct(Aapl_t *aapl,  Avago_kr_config_t *config);
EXT int  avsp_kr_enable(Aapl_t *aapl, uint chip, Avago_kr_config_t *config);
EXT int  avsp_kr_status(Aapl_t *aapl, uint chip, Avago_kr_config_t *config);
EXT void avsp_9104_set_kr_backchannel_mux(Aapl_t *aapl, uint chip, uint slice_0_rx, uint slice_1_rx, uint slice_2_rx, uint slice_3_rx);
#endif /* AAPL_ENABLE_AVSP_KR_TRAINING */


EXT int avsp_supervisor_enable(Aapl_t *aapl, uint chip, BOOL enable);
EXT int avsp_supervisor_status(Aapl_t *aapl, uint chip, BOOL *enabled);
EXT int avsp_supervisor_clear_interr_io(Aapl_t *aapl, uint chip);
EXT int avsp_supervisor_get_interr_io(Aapl_t *aapl, uint chip, BOOL *active);

/** @brief */
typedef enum
{
    AVSP_SUPERVISOR_MODE_TUNE_IF_SIGNAL,        /**< Start tuning when signal is detected without waiting for frequency lock. */
    AVSP_SUPERVISOR_MODE_TUNE_IF_LOCKED_SIGNAL, /**< Start tuning only after signal is detected and frequency is locked. */
    AVSP_SUPERVISOR_MODE_NO_TUNE                /**< */
} Avsp_supervisor_mode_t;

/** @brief Structure for retrieving supervisor status, and for controlling */
/**        supervisor operation. */
typedef struct
{
    short assp_control_0;               /**< Control flags.  Change only if you know what you are doing. */
    short assp_control_1;
    short assp_control_2;
    short assp_control_3;
    int  serdes_count;                  /**< Informational */
    BOOL enabled;                       /**< Supervisor run status. */
    BOOL interr_io;                     /**< interr_io pin status. */
    int status[AVSP_MAX_SLICE_COUNT];   /**< Per slice status bits. */
} Avsp_supervisor_config_t;

EXT BOOL avsp_supervisor_config_init(Aapl_t *aapl, Avsp_supervisor_mode_t mode,
        BOOL squelch_tx, BOOL reuse_ical, BOOL adaptive, Avsp_supervisor_config_t *config);
EXT Avsp_supervisor_config_t *avsp_supervisor_config_construct(Aapl_t *aapl,
        Avsp_supervisor_mode_t mode, BOOL squelch_tx, BOOL reuse_ical, BOOL adaptive);
EXT void avsp_supervisor_config_destruct(Aapl_t *aapl, Avsp_supervisor_config_t *config);
EXT int avsp_supervisor_set_config(Aapl_t *aapl, uint chip, Avsp_supervisor_config_t *config);

EXT int avsp_supervisor_get_config(Aapl_t *aapl, uint chip, Avsp_supervisor_config_t *config);
EXT int avsp_supervisor_set_slices(Aapl_t *aapl, uint chip, int count, uint *selections);
EXT int avsp_supervisor_set_signal_ok(Aapl_t *aapl, uint chip, uint select, int threshold);


/* Deprecated: */
typedef enum
{
    AVSP_CUST,
    AVSP_BERT,
    AVSP_SOLO
} Avsp_setup_t;
/* Deprecated: */
typedef enum {AVSP_LB_NEAR, AVSP_LB_FAR, AVSP_LB_OFF, AVSP_LB_STATE} Avsp_lb_mode_t;
/* Deprecated: */
typedef enum
{
    AVSP_LB_HOST,
    AVSP_LB_MOD
} Avsp_lb_side_t;


typedef enum { AVSP_ALL, AVSP_HOST, AVSP_MOD, AVSP_HOST_MOD4, AVSP_MOD_HOST4, AVSP_MOD4, AVSP_MODX, AVSP_GB01_HOST_MOD, AVSP_GB23_HOST_MOD, AVSP_GB01_MOD_HOST, AVSP_GB23_MOD_HOST, AVSP_HALF0, AVSP_HALF1, AVSP_MOD6 = AVSP_MODX } Avsp_addr_list_t;

EXT BOOL aapl_get_addr_list(Aapl_t *aapl, uint prtad, const char *avsp_name, Avsp_addr_list_t list, uint *len, uint **addrs);
EXT BOOL avsp_get_name_list(Aapl_t *aapl, uint prtad, const char *avsp_name, uint *len, const char ***names);

EXT int avago_firmware_upload(Aapl_t *aapl, uint addr,
                              int serdes_rom_size, const int *serdes_rom,
                              int sbm_rom_size,    const int *sbm_rom,
                              int sdi_rom_size,    const int *sdi_rom);

/* General AVSP functions: */
EXT int avsp_upload_firmware(Aapl_t *aapl, uint prtad, BOOL refclk_halve,
                             int serdes_rom_size, const int *serdes_rom,
                             int sbm_rom_size, const int *sbm_rom,
                             int swap_rom_size, const int *swap_rom);

#if AAPL_ENABLE_FILE_IO
EXT int avago_firmware_upload_file(Aapl_t *aapl, uint addr,
                              const char *serdes_rom_file,
                              const char *sbm_rom_file,
                              const char *sdi_rom_file);
EXT int avsp_upload_firmware_file(Aapl_t *aapl, uint prtad, BOOL refclk_halve,
                                  const char *serdes_filename, const char *sbm_filename);
#endif

EXT BOOL avsp_get_self_healing(Aapl_t *aapl, uint prtad);
EXT void avsp_set_self_healing(Aapl_t *aapl, uint prtad, BOOL self_heal_enable);

EXT int avsp_get_refclk_divisor(Aapl_t *aapl, uint prtad);

EXT int avsp_enable_signal_ok(Aapl_t *aapl, uint prtad, Avsp_addr_list_t list, int threshold);

EXT int avsp_batch_tune(Aapl_t *aapl, uint prtad, Avsp_addr_list_t list,
                        Avago_serdes_dfe_state_t *dfe, int start_delay, int loop_delay, int timeout);


/* Gearbox specific functions: */
EXT Avsp_mode_t avsp_1104_get_mode(Aapl_t *aapl, uint prtad);
EXT Avsp_mode_t avsp_9104_get_mode(Aapl_t *aapl, uint prtad);

EXT BOOL avsp_control_logic_reset(Aapl_t *aapl, uint prtad);
EXT BOOL avsp_control_logic_reset_host_to_mod(Aapl_t *aapl, uint prtad, uint slice_mask);
EXT BOOL avsp_control_logic_reset_mod_to_host(Aapl_t *aapl, uint prtad, uint slice_mask);

EXT BOOL avsp_control_logic_reset_direct(Aapl_t *aapl, uint prtad, Avsp_mode_t mode);
EXT BOOL avsp_control_logic_reset_host_to_mod_direct(Aapl_t *aapl, uint prtad, Avsp_mode_t mode, uint slice_mask);
EXT BOOL avsp_control_logic_reset_mod_to_host_direct(Aapl_t *aapl, uint prtad, Avsp_mode_t mode, uint slice_mask);


EXT BOOL avsp_9104_fec_control_logic_reset_host_to_mod_direct(Aapl_t *aapl, uint prtad);
EXT BOOL avsp_9104_fec_control_logic_reset_mod_to_host_direct(Aapl_t *aapl, uint prtad);
EXT BOOL avsp_9104_fec_control_logic_reset_direct(Aapl_t *aapl, uint prtad);

typedef enum
{
    AVSP_FEC_ERROR_INDICATION_DISABLE_ABILITY,   /**< Returns 1. */
    AVSP_FEC_ERROR_CORRECTION_BYPASS_ABILITY,    /**< Returns 1. */
    AVSP_FEC_ERROR_INDICATION_DISABLE,           /**< Returns TRUE if disabled. */
    AVSP_FEC_ERROR_CORRECTION_BYPASS_ENABLE,     /**< Returns TRUE if enabled. */
    AVSP_FEC_BLOCK_LOCK,        /**< Bit map [19..0] 8812 only */
    AVSP_FEC_LANE_ALIGNMENT,    /**< Bit map [3..0] */
    AVSP_FEC_LANE_MAPPING,      /**< Returns mapping for all 4 lanes in one hex value. */
                                /**< Each mapping is 4 bits, ordered 0x3210. */

    AVSP_PCS_BLOCK_LOCK,        /**< Bit map [19..0] */
    AVSP_PCS_LANE_ALIGNMENT,    /**< Bit map [19..0] */
    AVSP_PCS_LANE_MAPPING,      /**< Select lane using lane field: range [0-19]. */

    AVSP_FEC_CORRECTED,         /**< Returns corrected FEC errors. */
    AVSP_FEC_UNCORRECTED,       /**< Returns uncorrected FEC errors. */
    AVSP_FEC_SYMBOL_ERRORS,     /**< Return error count for one lane: range [0-3]. */
    AVSP_BIP_ERRORS,            /**< Return errors for lane [0..19] and mod(0)/host(1) selection */
    AVSP_ALL_ERRORS             /**< Returns sum of all above errors. */

} Avsp_fec_register_t;

EXT int avsp_9104_get_status(Aapl_t *aapl, uint avsp_prtad, Avsp_fec_register_t type, uint lane, BOOL host);
EXT int avsp_9104_set_control(Aapl_t *aapl, uint avsp_prtad, Avsp_fec_register_t type, int value);


#if AAPL_ENABLE_AVSP_7412
typedef enum {
    AVSP_7412_4TO2_RS528,       
    AVSP_7412_4TO2_RS544,
    AVSP_7412_4TO4_RS528,       
    AVSP_7412_4TO2,             
    AVSP_7412_4TO4
} Avsp_state_7412_t;
EXT void avsp_7412_init(Aapl_t *aapl, uint prtad, Avsp_state_7412_t state);

#endif  /* AAPL_ENABLE_AVSP_7412 */

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
EXT void avsp_9104_rs_fec_state(Aapl_t *aapl, uint portad);
EXT void avsp_9104_mlg_state(Aapl_t *aapl, uint prtad);
#endif


/* Debugging functions: */
EXT uint avsp_1104_slip(Aapl_t *aapl, uint prtad, uint host_sbus_addr, uint mod_sbus_addr);
EXT uint avsp_1104_loopback(Aapl_t *aapl, uint div_ratio, uint prtad, Avsp_mode_t pma_v_rpt, Avsp_lb_mode_t lb_mode, Avsp_lb_side_t lb_side);
EXT uint avsp_9104_slip(Aapl_t *aapl, uint prtad, uint host_sbus_addr, uint mod_sbus_addr);
EXT uint avsp_9104_loopback(Aapl_t *aapl, uint div_ratio, uint prtad, Avsp_mode_t pma_v_rpt, Avsp_lb_mode_t lb_mode, Avsp_lb_side_t lb_side);


/* Structures and declarations needed to read/write 1104 state info: */

/** @brief  Describes the RX state for a SerDes device. */
typedef struct
{
    uint                        sbus_addr;
    uint                        mem_addr;
    int                         width;
    int                         divider;
    BOOL                        enable;
    BOOL                        polarity_invert;
    BOOL                        input_loopback;
    Avago_serdes_pcs_fifo_clk_t pcs_fifo_clk_divider;
    Avago_serdes_rx_term_t      term;
    Avago_serdes_rx_cmp_data_t  cmp_data;
    Avago_serdes_rx_cmp_mode_t  cmp_mode;
    Avago_serdes_line_encoding_t encoding;
    BOOL                        signal_ok_enable;
    int                         signal_ok_threshold;
    int                         phase_slip;

    /* These values are directive rather than state: */
    BOOL                        run_ical;
    BOOL                        run_pcal;
    BOOL                        run_adaptive;
    BOOL                        run_vsr_tune;
    BOOL                        bypass_ctle;

    Avago_serdes_dfe_state_t    dfe;

    /* These values are status, not state: */
    Avago_serdes_rx_data_qual_t data_qual;
    BOOL                        signal_ok;
    uint                        errors;

    /* This value is for bookkeeping: */
    int                         flags;

} Avago_serdes_rx_state_t;

/** @brief  Describes the TX state for a SerDes device. */
typedef struct
{
    uint                        sbus_addr;
    int                         sd_num;
    int                         width;  /* 10, 20, 40 */
    int                         divider;
    BOOL                        enable;
    BOOL                        polarity_invert;
    BOOL                        output_enable;
    Avago_serdes_tx_pll_clk_t   pll_clk_source;
    Avago_serdes_tx_data_sel_t  data_sel;
    Avago_serdes_tx_eq_t        eq;
    Avago_serdes_line_encoding_t encoding;
    int                         source_rx;
/*  int                         thermal_mask; */

    /* These values are status, not state: */
    uint                        fw_revision;
    uint                        fw_build_id;

} Avago_serdes_tx_state_t;


/** @brief Describes a unidirectional SerDes link. */
typedef struct
{
    Avago_serdes_rx_state_t    *rx;
    Avago_serdes_tx_state_t    *tx;

} Avago_simplex_link_t;

/** @brief Describes a bidirectional SerDes link. */
/** */
/** @details 8812 SerDes Pairing: (host_rx,mod_tx) & (mod_rx,host_tx) */
/** @details 4412 SerDes Pairing: (host_rx,mod_tx) & (mod_rx,host_tx) */
/** @details 1104 SerDes Pairing: (host_rx,host_tx) & (mod_rx,mod_tx) */
typedef struct
{
    Avago_serdes_rx_state_t    *host_rx;
    Avago_serdes_tx_state_t    *host_tx;
    Avago_serdes_rx_state_t    *mod_rx;
    Avago_serdes_tx_state_t    *mod_tx;

} Avago_duplex_link_t;

/** @brief Describes an AVSP-8801 device state. */
typedef struct
{
    Avago_simplex_link_t    link[8];
    uint                    link_cnt;

} Avsp_8801_state_t;

/** @brief Describes an AVSP-8812 device state. */
typedef struct
{
    Avago_duplex_link_t     link[8];
    uint                    link_cnt;
    Avsp_mode_t             half_mode[2];
} Avsp_8812_state_t;

/** @brief Describes an AVSP-5410 device state. */
typedef struct
{
    Avago_duplex_link_t     link[4];
    uint                    link_cnt;
} Avsp_5410_state_t;
/** @brief Describes an AVSP-4412 device state. */
typedef struct
{
    Avago_duplex_link_t     link[4];
    uint                    link_cnt;

} Avsp_4412_state_t;

/** @brief Describes an AVSP-1104 device state. */
typedef struct
{
    Avago_duplex_link_t     link[10];
    uint                    link_cnt;

} Avsp_1104_state_t;

/** @brief Describes an AVSP-9104 device state. */
typedef struct
{
    Avago_duplex_link_t     link[10];
    uint                    link_cnt;

} Avsp_9104_state_t;

/** @brief Describes an AVSP-7412 device state. */
typedef struct
{
    Avago_duplex_link_t     link[4];
    uint                    link_cnt;

} Avsp_7412_state_t;
/** @brief Describes a SerDes state. */
typedef struct
{
    Avago_serdes_rx_state_t rx;
    Avago_serdes_tx_state_t tx;
} Avsp_serdes_state_t;

/** @brief Describes an AVSP device state. */
typedef struct
{
    int device;    /* 1104, 4412, 8801, 9104, ... */
    uint prtad;
    Avsp_mode_t mode;

    uint serdes_count;   /* Actual SerDes on chip */
    Avsp_serdes_state_t serdes[AVSP_MAX_SLICE_COUNT];

    uint sbm_fw_revision;
    uint sbm_fw_build_id;

    /* xdmem table magic numbers: */
    short magic[4];

    /* 1104 specific values: */
    short prod_var_cnt;
    short device_var1; /*v2_host_clk_sel; */
    short device_var2; /*v2_mod_clk_sel; */
    short device_var3; /*v2_speed_sel; */

    /* General AVSP configuration values: */
    bigint assp_int_cnt;
    short assp_var_cnt;
    short assp_delay_cnt;
    short assp_user_var_0;
    short assp_user_var_1;
    short assp_control[4];
    short assp_delay[2];

    /* Device specific view of part: */
    union
    {
#if AAPL_ENABLE_AVSP_1104
        Avsp_1104_state_t a1104;
#endif
#if AAPL_ENABLE_AVSP_4412
        Avsp_4412_state_t a4412;
#endif
#if AAPL_ENABLE_AVSP_8801
        Avsp_8801_state_t a8801;
#endif
#if AAPL_ENABLE_AVSP_8812
        Avsp_8812_state_t a8812;
#endif
#if AAPL_ENABLE_AVSP_5410
        Avsp_5410_state_t a5410;
#endif
#if AAPL_ENABLE_AVSP_9104
        Avsp_9104_state_t a9104;
#endif
#if AAPL_ENABLE_AVSP_7412
        Avsp_7412_state_t a7412;
#endif
    } u;
} Avsp_state_t;

/* Create/initialize/destruct state object: */
EXT BOOL          avsp_state_init(Aapl_t *aapl, uint prtad, const char *avsp_name, Avsp_mode_t mode, Avsp_state_t *state);
EXT Avsp_state_t *avsp_state_construct(Aapl_t *aapl, uint prtad, const char *avsp_name, Avsp_mode_t mode);
EXT void          avsp_state_destruct( Aapl_t *aapl, Avsp_state_t *state);
EXT Avsp_state_t *avsp_state_get(Aapl_t *aapl, uint prtad);

/* Functions to edit state before applying: */

EXT BOOL avsp_state_set_crosspoint(Aapl_t *aapl, Avsp_state_t *state, char *xp);
EXT int  avsp_state_set_divider(Aapl_t *aapl, Avsp_state_t *state, int divider);
EXT int  avsp_8812_configure_half(Aapl_t *aapl, Avsp_state_t *state, Avsp_mode_t mode, int divider, uint half_sel);
EXT int  avsp_state_get_ical(Avsp_state_t *state);
EXT int  avsp_state_get_pcal(Avsp_state_t *state);
EXT void avsp_state_set_ical(Avsp_state_t *state, int newValue);
EXT void avsp_state_set_pcal(Avsp_state_t *state, int newValue);
EXT void avsp_state_set_ical_host(Avsp_state_t *state, int newValue);
EXT void avsp_state_set_pcal_host(Avsp_state_t *state, int newValue);
EXT void avsp_state_set_ical_mod(Avsp_state_t *state, int newValue);
EXT void avsp_state_set_pcal_mod(Avsp_state_t *state, int newValue);

#if AAPL_ENABLE_AVSP_8812   /* Crosspoint functions only supported on 8812 */
EXT int avsp_8812_crosspoint_get(    Aapl_t *aapl, uint prtad, uint to_sd);
EXT int avsp_8812_crosspoint_connect(Aapl_t *aapl, uint prtad, uint from_sd, uint to_sd);
EXT int avsp_8812_crosspoint_get_all(    Aapl_t *aapl, uint prtad, char crosspoint[16]);
EXT int avsp_8812_crosspoint_connect_all(Aapl_t *aapl, uint prtad, char crosspoint[16]);
EXT BOOL avsp_8812_configure_crosspoint(Aapl_t *aapl, Avsp_state_t *state);
EXT int avsp_8812_get_enabled(Aapl_t *aapl, uint prtad, uint sbus);
EXT int avsp_8812_get_gearbox_mode(Aapl_t *aapl, uint prtad, uint sbus);
EXT BOOL avsp_8812_control_logic_reset_direct(Aapl_t *aapl, uint prtad, Avsp_mode_t mode, uint half_select);
EXT int avsp_8812_set_mode(Aapl_t *aapl, Avsp_state_t *state, uint half);
EXT Avsp_mode_t avsp_8812_get_mode(Aapl_t *aapl, uint prtad, uint half);
EXT int avsp_8812_bring_up(Aapl_t *aapl, uint prtad, uint half, const char * direction, const char * sel_tuning);
EXT int avsp_8812_channel_swap(Aapl_t *aapl, uint prtad, uint gb);
EXT int avsp_8812_channel_align(Aapl_t *aapl, uint prtad);
EXT int avsp_8812_get_status(Aapl_t *aapl, uint avsp_prtad, uint which_fec, Avsp_fec_register_t type, uint lane, BOOL host);
EXT void avsp_8812_fec_error_reset(Aapl_t *aapl, uint avsp_prtad, uint which_fec);
EXT int avsp_8812_set_control(Aapl_t *aapl, uint avsp_prtad, uint which_fec, Avsp_fec_register_t type, int value);
#endif /* AAPL_ENABLE_AVSP_8812 */

#if AAPL_ENABLE_AVSP_7412
EXT int avsp_7412_set_mode(Aapl_t *aapl, Avsp_state_t *state);
EXT Avsp_mode_t avsp_7412_get_mode(Aapl_t *aapl, uint prtad);
#endif /* AAPL_ENABLE_AVSP_7412 */

#if AAPL_ENABLE_AVSP_5410
EXT Avsp_mode_t avsp_5410_get_mode(Aapl_t *aapl, uint prtad);
EXT int avsp_5410_set_mode(Aapl_t *aapl, uint prtad, Avsp_mode_t mode);
EXT BOOL avsp_5410_control_logic_reset_direct(Aapl_t *aapl, uint prtad, const char * direction);
EXT int avsp_5410_bring_up(Aapl_t *aapl, uint prtad, const char *slices);
EXT int avsp_5410_loopback(Aapl_t *aapl, const char *sel_tuning, uint prtad, Avsp_lb_mode_t lb_mode, Avsp_lb_side_t lb_side);
#endif /* AAPL_ENABLE_AVSP_5410 */

/* Functions to read state from various sources: */
EXT void avsp_state_dump_abi(Aapl_t *aapl, const char *abi_name);
EXT int avsp_state_read_from_device(Aapl_t *aapl, Avsp_state_t *state);
EXT int avsp_state_read_from_memory(Aapl_t *aapl, Avsp_state_t *state);
EXT int avsp_state_read_from_sdi_file(Aapl_t *aapl, const char *pathname, Avsp_state_t *state);
EXT int avsp_state_read_from_text_file(Aapl_t *aapl, const char *pathname, Avsp_state_t *state);

/* Functions to write state to various targets: */
typedef struct
{
    const char *d6_serdes_fw_path; /**< [in] Pathname of serdes fw file to use. */
    const char *m4_serdes_fw_path; /**< [in] Pathname of serdes fw file to use. */
    const char *p1_serdes_fw_path; /**< [in] Pathname of serdes fw file to use. */
    const char *sbm_fw_path;    /**< [in] Pathname of sbm fw file to use. */
    Avsp_state_t *state;        /**< [in] State structure to save. */
    int i2c_freq_control;       /**< [in] I2C frequency control [0..15] */
    BOOL auto_state;            /**< [in] Set TRUE to auto-state on load. */
    int address_mask;           /**< [in] Select device i2c addresses. */
                                /**<  Bit 0 = i2c offset 0, etc. */
} Avsp_state_abi_config_t;

EXT int avsp_state_write_to_abi_files(Aapl_t *aapl, int config_count, Avsp_state_abi_config_t *configs, const char *write_abi_name);
EXT int avsp_state_write_to_device(Aapl_t *aapl, Avsp_state_t *state);
EXT int avsp_8812_state_write_to_device(Aapl_t *aapl, Avsp_state_t *state, uint half);
EXT int avsp_8812_configure_lane_speed(Aapl_t *aapl, uint prtad, uint lane, int divider); 
EXT int avsp_state_write_to_memory(Aapl_t *aapl, Avsp_state_t *state);
EXT int avsp_state_write_to_sdi_file(Aapl_t *aapl, const char *serdes_fw, const char *sdi_pathname, Avsp_state_t *state);
EXT int avsp_state_write_to_text_file(Aapl_t *aapl, const char *pathname, Avsp_state_t *state);
EXT int avsp_abi_write_to_device(Aapl_t *aapl, uint prtad, const char *abi_name);
EXT int avsp_abi_write_to_eeprom(Aapl_t *aapl, const char *abi_name);

/* Tell SBus Master firmware to state the device. */
typedef struct
{
    Avsp_mode_t mode;
    int divider;
    /* 8812 specific variables */
    Avsp_mode_t half_mode[2];
    int half0_div;
    int half1_div;
    /* Valid only for 9104 AVSP_GEARBOX_10_4_RS_FEC mode: */
    BOOL disable_fec;
    BOOL enable_fec_bip_error_workaround;
    BOOL disable_mod_during_fec_setup;
    BOOL disable_host_during_fec_setup;
    BOOL run_an_before_fec_setup; /* seq 1 */
    BOOL run_an_after_fec_setup;  /* seq 2, Ignored if run_an_before_fec_setup is set. */
    BOOL run_kr_after_an;         /* If AN disabled, KR still runs (when?) */
    uint extra_opts;              /* For testing/debugging purposes. */

} Avsp_state_options_t;

EXT void avsp_state_options_init(Aapl_t *aapl, Avsp_state_options_t *options);
EXT void avsp_state_device_from_memory(Aapl_t *aapl, uint prtad);
EXT int  avsp_state_device_from_defaults(Aapl_t *aapl, uint prtad, Avsp_mode_t mode, int divider);
EXT int  avsp_state_device_with_options(Aapl_t *aapl, uint prtad, Avsp_state_options_t *options);
EXT int  avsp_state_set(Aapl_t *aapl, Avsp_state_t *state);

/* Display the state in a summary form. */
EXT void avsp_state_print(Aapl_t *aapl, Avsp_state_t *state, BOOL print_dfe);

/* Query chip: */
EXT int avsp_sensor_get_temperature(Aapl_t *aapl, uint prtad, int sensor, uint frequency);
EXT int avsp_sensor_get_voltage(Aapl_t *aapl, uint prtad, int sensor, uint frequency);


# ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS
    EXT int aapl_sign_mag(int value, int mag_bits);
    EXT int aapl_twos_comp(int bits, int mag_bits);
# endif


#endif  /* AAPL_ENABLE_AVSP */

#endif /* AVSP_H_ */

