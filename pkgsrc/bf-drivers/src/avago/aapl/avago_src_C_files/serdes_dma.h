/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* Internal library header file for AAPL (ASIC and ASSP Programming Layer). */

/** Doxygen File Header */
/** @file */
/** @brief SerDes DMA register naming.  */

#ifndef AVAGO_SERDES_DMA_H_
#define AVAGO_SERDES_DMA_H_

/* SerDes AVAGO_LSB DMA adress map */
#define AVSD_LSB_CLOCK_ENABLES          0x00c
#define AVSD_LSB_ERROR_COUNT_CNTL       0x00d
#define AVSD_LSB_ERROR_COUNT_LO         0x00e
#define AVSD_LSB_ERROR_COUNT_HI         0x00f
#define AVSD_LSB_COMPARE                0x017
#define AVSD_LSB_COMPARE_ENABLE         0x01a
#define AVSD_LSB_TX_DATA                0x021
#define AVSD_LSB_RX_DATA                0x024
#define AVSD_LSB_RX_ELEC_IDLE           0x025
#define AVSD_LSB_SERDES_RDY             0x026
#define AVSD_LSB_CORE_STATUS            0x027
#define AVSD_LSB_TX_PRBS_CNTL           0x029
#define AVSD_LSB_RX_PRBS_CNTL           0x02a
#define AVSD_LSB_INVERT_ONE_WD          0x02b
#define AVSD_LSB_PRBS_SEED_LO           0x02c
#define AVSD_LSB_PRBS_SEED_HI           0x02d
#define AVSD_LSB_ESB_DMA_ADDRESS        0x030 /* LSB DMA address used to access AVAGO_ESB DMA registers */
#define AVSD_LSB_ESB_DMA_WRITE_DATA     0x031 /* LSB DMA address used to write AVAGO_ESB DMA data */
#define AVSD_LSB_ESB_DMA_READ_DATA      0x032 /* LSB DMA address used to read AVAGO_ESB DMA data out */

/* SerDes AVAGO_ESB DMA adress map */
#define AVSD_ESB_RX_PLL_GAIN            0x001
#define AVSD_ESB_RX_PLL_DAC             0x002
#define AVSD_ESB_RX_PLL_CAL_CNTL        0x003
#define AVSD_ESB_RX_PLL_CNTL            0x004
#define AVSD_ESB_RX_TERM_CNTL           0x020
#define AVSD_ESB_RX_POLARITY            0x060
#define AVSD_ESB_RX_PLL_DIVIDER_CNTL2   0x070
#define AVSD_ESB_RX_PLL_DIVIDER_CNTL    0x073
#define AVSD_ESB_RX_EI_THRESHOLD        0x080
#define AVSD_ESB_TX_PLL_CK_MUX          0x200
#define AVSD_ESB_TX_PLL_GAIN            0x201
#define AVSD_ESB_TX_PLL_DAC             0x202
#define AVSD_ESB_TX_PLL_CAL_CNTL        0x203
#define AVSD_ESB_TX_PLL_CNTL            0x204
#define AVSD_ESB_TX_POLARITY            0x211
#define AVSD_ESB_TX_OUTPUT_EN           0x213
#define AVSD_ESB_TX_PLL_DIVIDER         0x220
#define AVSD_ESB_TX_SLEW_MISC           0x240
#define AVSD_ESB_PCIE_CK_MUX            0x300
#define AVSD_ESB_WIDTH_MODE             0x301
#define AVSD_ESB_IDCODE                 0x303

/* SerDes AVAGO_ESB HVD6 DMA adress map */
#define AVSD_ESB_HVD6_RX_PLL_GAIN            0x01
#define AVSD_ESB_HVD6_RX_PLL_DAC             0x02
#define AVSD_ESB_HVD6_RX_PLL_CAL_CNTL        0x03
#define AVSD_ESB_HVD6_RX_PLL_CNTL            0x04
#define AVSD_ESB_HVD6_RX_TERM_CNTL           0x20
#define AVSD_ESB_HVD6_RX_POLARITY            
#define AVSD_ESB_HVD6_RX_PLL_DIVIDER_CNTL2   0xca
#define AVSD_ESB_HVD6_RX_PLL_DIVIDER_CNTL    
#define AVSD_ESB_HVD6_RX_EI_THRESHOLD        0x80
#define AVSD_ESB_HVD6_TX_PLL_CK_MUX          0x08
#define AVSD_ESB_HVD6_TX_PLL_GAIN            0x09
#define AVSD_ESB_HVD6_TX_PLL_DAC             0x0a
#define AVSD_ESB_HVD6_TX_PLL_CAL_CNTL        0x0b
#define AVSD_ESB_HVD6_TX_PLL_CNTL            0x0c
#define AVSD_ESB_HVD6_TX_POLARITY            
#define AVSD_ESB_HVD6_TX_OUTPUT_EN           0xde
#define AVSD_ESB_HVD6_TX_PLL_DIVIDER         0xd8
#define AVSD_ESB_HVD6_TX_SLEW_MISC           
#define AVSD_ESB_HVD6_PCIE_CK_MUX            
#define AVSD_ESB_HVD6_WIDTH_MODE             
#define AVSD_ESB_HVD6_IDCODE                 

/* SerDes AVAGO_ESB 16NM DMA adress map */
#define AVSD_ESB16_RX_PLL_GAIN          0x81
#define AVSD_ESB16_RX_PLL_DAC           0x83
#define AVSD_ESB16_RX_PLL_CAL_CNTL      0x84
#define AVSD_ESB16_RX_PLL_CNTL          0x86
#define AVSD_ESB16_RX_TERM_CNTL         0x92
#define AVSD_ESB16_RX_PLL_DIVIDER_CNTL  0xc1
#define AVSD_ESB16_RX_EI_THRESHOLD      0xc4
#define AVSD_ESB16_RX_MISC              0xc7
#define AVSD_ESB16_RX_DIVX_CNTL         0xca
#define AVSD_ESB16_RX_DIVX2_CNTL        0xcb
#define AVSD_ESB16_RX_POLARITY          0xdf  /* TODO: need to clarify RX invert for 16NM */
#define AVSD_ESB16_TX_PLL_CK_MUX        0xd1
#define AVSD_ESB16_TX_PLL_GAIN          0xd2
#define AVSD_ESB16_TX_PLL_DAC           0xd3
#define AVSD_ESB16_TX_PLL_CAL_CNTL      0xd4
#define AVSD_ESB16_TX_PLL_CNTL          0xd5
#define AVSD_ESB16_TX_DIVX_CNTL         0xd8
#define AVSD_ESB16_TX_DIVX2_CNTL        0xf5
#define AVSD_ESB16_TX_POLARITY          0xdf
#define AVSD_ESB16_TX_OUTPUT_EN         0xdf
#define AVSD_ESB16_TX_SLEW_MISC         0xe5
#define AVSD_ESB16_PCIE_CK_MUX          0xf0
#define AVSD_ESB16_WIDTH_MODE           0xf1
#define AVSD_ESB16_IDCODE               0xf2

#endif /* AVAGO_SERDES_DMA_H_ */
