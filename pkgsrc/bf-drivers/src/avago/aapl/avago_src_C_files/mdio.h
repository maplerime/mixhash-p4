/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* AAPL (ASIC and ASSP Programming Layer) MDIO-related functions defined in */
/* mdio.cpp. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for MDIO bus functions. */

#ifndef AVAGO_MDIO_H_
#define AVAGO_MDIO_H_

#if AAPL_ALLOW_MDIO || AAPL_ALLOW_GPIO_MDIO

#define AVSP_DEVAD 8 /* for all SBus-over-MDIO operations: */

#define AVAGO_MDIO_REG_STATUS      32777  /* fifo_empty is bit [11]. */
#define AVAGO_MDIO_REG_STAT_MISC1  32787  /* miscelaneous status bits */
#define AVAGO_MDIO_REG_RESET       32784  /* sbus_reset is bit [0]. */
#define AVAGO_MDIO_REG_CONTROL     0      /* PMA/PMD control2 register contains type select */
#define AVAGO_MDIO_REG_CONTROL2    7      /* PMA/PMD control register contains speed select */

EXT uint avago_mdio(     Aapl_t *aapl, Avago_mdio_cmd_t mdio_cmd, uint port_addr, uint dev_addr, uint data);
EXT uint avago_mdio_rd(  Aapl_t *aapl, uint port_addr, uint dev_addr, uint reg_addr);
EXT uint avago_mdio_wr(  Aapl_t *aapl, uint port_addr, uint dev_addr, uint reg_addr, uint data);

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS

#if AAPL_ALLOW_AACS
EXT uint avago_aacs_mdio_fn(Aapl_t *aapl, Avago_mdio_cmd_t mdio_cmd, uint port_addr, uint dev_addr, uint reg_addr, uint data);
#endif
EXT uint avago_mdio_sbus_fn(Aapl_t *aapl, uint sbus_addr, unsigned char reg_addr, unsigned char command, uint *sbus_data);

#endif /* AAPL_ENABLE_INTERNAL_FUNCTIONS */

#endif /* AAPL_ALLOW_MDIO || AAPL_ALLOW_GPIO_MDIO */

#endif /* AVAGO_MDIO_H_ */



