/* clang-format off */

#include "autogen-required-headers.h"

#include <inttypes.h>
/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Reset Comira MAC/PCS "
                   " -[1]   : Reset App logic "
                   " -[2]   : Reset microcontroller "
                   " -[3]   : Reset Serdes IO Tile "
***********************************************************************/
void eth100g_reg_rspec_eth_soft_reset_eth_swrst_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_soft_reset);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_soft_reset);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_soft_reset_eth_swrst(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_soft_reset_eth_swrst_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_soft_reset);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_soft_reset);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_soft_reset_eth_swrst(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_soft_reset_eth_swrst_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_soft_reset_eth_swrst_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_soft_reset_eth_swrst_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_rx0_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_rx0_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_rx0_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_rx0_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_rx0_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_rx0_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_rx0_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_tx0_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_tx0_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_tx0_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_tx0_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_tx0_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_tx0_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_tx0_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_rx1_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_rx1_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_rx1_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_rx1_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_rx1_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_rx1_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_rx1_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_tx1_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_tx1_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_tx1_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_tx1_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_tx1_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_tx1_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_tx1_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_rx2_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_rx2_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_rx2_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_rx2_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_rx2_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_rx2_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_rx2_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_tx2_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_tx2_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_tx2_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_tx2_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_tx2_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_tx2_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_tx2_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_rx3_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_rx3_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_rx3_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_rx3_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_rx3_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_rx3_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_rx3_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Need to be set when Serdes operation at 25.78125 Gbps, enables 32-40b conversion. "
                   " -[1]   : Need to be set when Serdes operation at 10.3125 Gbps, enables 16-20b conversion. "
                   " -[2]   : Need to be set when Serdes operation at 1.25 Gbps, enables 8-10b conversion."
***********************************************************************/
void eth100g_reg_rspec_serdes_config_tx3_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_serdes_config_tx3_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_serdes_config_tx3_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.serdes_config);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.serdes_config);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_serdes_config_tx3_mode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_serdes_config_tx3_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_serdes_config_tx3_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_serdes_config_tx3_mode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Enables the selected clock obervation selected of the MAC to be sent on the daisy chain up to the clock observation pad "
***********************************************************************/
void eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the clock divider settings: "
                   " - 00b : divide-by-2 "
                   " - 01b : divide-by-4 "
                   " - 10b : divide-by-8 "
                   " - 11b : divide-by-16 "
***********************************************************************/
void eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the clock divider settings: "
                   " - 0000b : rxclk0 "
                   " - 0001b : rxclk1 "
                   " - 0010b : rxclk2 "
                   " - 0011b : rxclk3 "
                   " - 0100b : txclk0 "
                   " - 0101b : txclk1 "
                   " - 0110b : txclk2 "
                   " - 0111b : txclk3 "
                   " - 1000b : mclk "
                   " - 1001b : clk "
                   " - 1010b : mci "
                   " - 1011b : mco "
                   " - 11xxb : reserved  "
***********************************************************************/
void eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Enables the selected clock obervation selected of the MAC to be sent on the daisy chain up to the clock observation pad "
***********************************************************************/
void eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_clkobs_ctrl_ena_clkobs1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the clock divider settings: "
                   " - 00b : divide-by-2 "
                   " - 01b : divide-by-4 "
                   " - 10b : divide-by-8 "
                   " - 11b : divide-by-16 "
***********************************************************************/
void eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_clkobs_ctrl_div_clkobs1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the clock divider settings: "
                   " - 0000b : rxclk0 "
                   " - 0001b : rxclk1 "
                   " - 0010b : rxclk2 "
                   " - 0011b : rxclk3 "
                   " - 0100b : txclk0 "
                   " - 0101b : txclk1 "
                   " - 0110b : txclk2 "
                   " - 0111b : txclk3 "
                   " - 1000b : mclk "
                   " - 1001b : clk "
                   " - 1010b : mci "
                   " - 1011b : mco "
                   " - 11xxb : reserved  "
***********************************************************************/
void eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_clkobs_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_clkobs_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_clkobs_ctrl_sel_clkobs1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines a timeslot associated for the port "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_setup_ring_id_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_setup);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_setup);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_setup_ring_id(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_setup_ring_id_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_setup);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_setup);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_setup_ring_id(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_setup_ring_id_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_setup_ring_id_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_setup_ring_id_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Select GPIO ring 0 or 1"
***********************************************************************/
void eth100g_reg_rspec_eth_ring_setup_ring_sel_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_setup);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_setup);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_setup_ring_sel(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_setup_ring_sel_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_setup);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_setup);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_setup_ring_sel(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_setup_ring_sel_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_setup_ring_sel_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_setup_ring_sel_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 In I2c Mode, the first byte sent over I2C is bit [7:0], followed by [15:8],... "
                   "In MDIO Mode, bit [4:0] defines portaddr[4:0] and bit [12:8] defines devaddr[4:0]. "
                   "In StateOut Mode,this register can be used to write an extra 4B. "
                   "The number of bytes depends on Ethernet Ring Control settings. "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_addr_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_addr);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_addr);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_addr_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_addr_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_addr);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_addr);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_addr_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_addr_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_addr_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_addr_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Data written during a direct write access. The first byte sent over I2C is bit [7:0]."
                   " The number of byte written depends on ethernet ring control register settings. "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_wdata_wdata_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_wdata);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_wdata);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_wdata_wdata(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_wdata_wdata_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_wdata);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_wdata);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_wdata_wdata(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_wdata_wdata_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_wdata_wdata_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_wdata_wdata_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report the 4 byte data read on I2C/MDIO interface during a register read. "
                   "first byte read is on bit [7:0],..."
***********************************************************************/
void eth100g_reg_rspec_eth_ring_rdata_rdata_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_rdata);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_rdata);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_rdata_rdata(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_rdata_rdata_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_rdata);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_rdata);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_rdata_rdata(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_rdata_rdata_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_rdata_rdata_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_rdata_rdata_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the type of transaction to be performed: "
                   " - 00b : Idle "
                   " - 01b : I2C transaction "
                   " - 10b : MDIO transaction "
                   " - 11b : StateOut "
                   "This register get cleared back to 00b when transaction has been issued to the ethernet GPIO block."
                   "Refer to the Status register to see status of the current transaction" 
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_req_type_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_req_type(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_req_type_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_req_type(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_req_type_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_req_type_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_req_type_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 In I2C mode, defines which command:"
                   " - 00b : write address and write data command "
                   " - 01b : write address and read data "
                   " - 10b : write device address only"
                   " - 11b : read data only "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_cmd_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_cmd(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_cmd_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_cmd(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_cmd_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_cmd_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_cmd_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 In I2C mode, defines the number of address bytes (0-4) to transfer after the device address byte. "
                   "In MDIO mode, contains the MDIO transaction code (on bit [1:0]) and transaction type (on bit [2]) "
                   "In StateOut mode, this field is reserved"
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_addrnum_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_addrnum(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_addrnum_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_addrnum(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_addrnum_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_addrnum_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_addrnum_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 In I2C mode, bit [6:0] defines the device address of the I2C device to target. "
                   "On StateOut mode, bit [7:0] defines the start address of the StateOut transfer. "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_devaddr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_devaddr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_devaddr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_devaddr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_devaddr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_devaddr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_devaddr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 For I2C mode, maximum number of byte is 4"
                   "For MDIO mode, "
                   "In StateOut mode, maximum 8B can be transferred "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_datanum_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_datanum(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_datanum_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_datanum(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_datanum_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_datanum_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_datanum_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Transaction "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_lock_req_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_lock_req(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_lock_req_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_lock_req(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_lock_req_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_lock_req_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_lock_req_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines which GPIO group/pair is targeted by the transaction. Value value are 0-5."
                   " It is assumed that the GPIO pair targeted is programmed in expected mode for I2C and MDIO settings. "
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_gpio_id_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_gpio_id(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_gpio_id_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_gpio_id(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_gpio_id_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_gpio_id_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_gpio_id_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the status of the on-going transaction: "
                   " - 000b: No update "
                   " - 001b: Request sent to GPIO block. "
                   " - 010b: Request granted by GPIO block. "
                   " - 011b: Response received from GPIO block with no error. "
                   " - 101b: Response received from GPIO block with error."
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_status_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_status(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_status_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_status(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_status_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_status_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_status_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Register get set when lock error status is received. Lock error bit need to be cleared by hardware."
***********************************************************************/
void eth100g_reg_rspec_eth_ring_ctrl_lock_err_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ring_ctrl_lock_err(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ring_ctrl_lock_err_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ring_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ring_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ring_ctrl_lock_err(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ring_ctrl_lock_err_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ring_ctrl_lock_err_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ring_ctrl_lock_err_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the software-controlled port_alive entry (1 bit per channel), going to the Look-Up Table"
                   " to create the final port_alive signal which will be sent to PGR block."
***********************************************************************/
void eth100g_reg_rspec_soft_port_alive_val_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.soft_port_alive);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.soft_port_alive);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_soft_port_alive_val(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_soft_port_alive_val_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.soft_port_alive);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.soft_port_alive);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_soft_port_alive_val(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_soft_port_alive_val_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_soft_port_alive_val_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_soft_port_alive_val_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Offset which is added to timestamp during egress timestamping and"
                   "which need to account for full MAC/PCS layer latency. "
                   "Note that increment/drift is received from MBus Station as"
                   "the logic works on CoreClk. "
***********************************************************************/
void eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %016" PRIu64 " : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset(data_p, val);

  if (hw) {
    uint32_t reg_val;
    reg_val = (uint32_t)(*data_p & 0xffffffffull);
    autogen_wr(dev_id, ofs, reg_val);
    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);
    autogen_wr(dev_id, ofs + 4, reg_val);
  }
}

void eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    uint32_t upper, lower;
    autogen_rd(dev_id, ofs, &lower);
    autogen_rd(dev_id, ofs + 4, &upper);
    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %016" PRIu64 " : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;

  eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Adjust the Egress timestamping offset by the current Tx app fifo count multiply by"
                   " the adjustment factor (4 lsb represent ns and 12 lsb represent ps)"
***********************************************************************/
void eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %016" PRIu64 " : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj(data_p, val);

  if (hw) {
    uint32_t reg_val;
    reg_val = (uint32_t)(*data_p & 0xffffffffull);
    autogen_wr(dev_id, ofs, reg_val);
    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);
    autogen_wr(dev_id, ofs + 4, reg_val);
  }
}

void eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_onestep_ets_offset_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    uint32_t upper, lower;
    autogen_rd(dev_id, ofs, &lower);
    autogen_rd(dev_id, ofs + 4, &upper);
    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %016" PRIu64 " : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;

  eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Offset which is added to timestamp received from MBus Station"
***********************************************************************/
void eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %016" PRIu64 " : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset(data_p, val);

  if (hw) {
    uint32_t reg_val;
    reg_val = (uint32_t)(*data_p & 0xffffffffull);
    autogen_wr(dev_id, ofs, reg_val);
    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);
    autogen_wr(dev_id, ofs + 4, reg_val);
  }
}

void eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    uint32_t upper, lower;
    autogen_rd(dev_id, ofs, &lower);
    autogen_rd(dev_id, ofs + 4, &upper);
    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %016" PRIu64 " : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_rmw( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;

  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Increment/Drift on MAC clock to automatically increment timestamp every"
                   "MAC clock cycle (4 lsb represent ns and 12 lsb represent ps)"
***********************************************************************/
void eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %016" PRIu64 " : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr(data_p, val);

  if (hw) {
    uint32_t reg_val;
    reg_val = (uint32_t)(*data_p & 0xffffffffull);
    autogen_wr(dev_id, ofs, reg_val);
    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);
    autogen_wr(dev_id, ofs + 4, reg_val);
  }
}

void eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mac_ts_offset_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    uint32_t upper, lower;
    autogen_rd(dev_id, ofs, &lower);
    autogen_rd(dev_id, ofs + 4, &upper);
    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %016" PRIu64 " : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;

  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report the Tx Idle status of the MAC (per channel)"
***********************************************************************/
void eth100g_reg_rspec_eth_status_macsts_txidle_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_status_macsts_txidle(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_status_macsts_txidle_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_status_macsts_txidle(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_status_macsts_txidle_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_status_macsts_txidle_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_status_macsts_txidle_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report the Tx good status of the MAC (per channel)"
***********************************************************************/
void eth100g_reg_rspec_eth_status_macsts_txgood_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_status_macsts_txgood(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_status_macsts_txgood_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_status_macsts_txgood(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_status_macsts_txgood_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_status_macsts_txgood_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_status_macsts_txgood_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report the Rx Sync status of the MAC (per channel)"
***********************************************************************/
void eth100g_reg_rspec_eth_status_macsts_rxsync_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_status_macsts_rxsync(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_status_macsts_rxsync_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_status_macsts_rxsync(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_status_macsts_rxsync_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_status_macsts_rxsync_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_status_macsts_rxsync_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report the Rx Signal OK status of the MAC (per channel)"
***********************************************************************/
void eth100g_reg_rspec_eth_status_macsts_rxsigok_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_status_macsts_rxsigok(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_status_macsts_rxsigok_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_status);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_status_macsts_rxsigok(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_status_macsts_rxsigok_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_status_macsts_rxsigok_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_status_macsts_rxsigok_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Each 2-bit correspond to a channel slot"
***********************************************************************/
void eth100g_reg_rspec_chnl_seq_chnl_seq_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_seq);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_seq);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_seq_chnl_seq(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_seq_chnl_seq_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_seq);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_seq);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_seq_chnl_seq(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_seq_chnl_seq_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_seq_chnl_seq_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_seq_chnl_seq_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Can be set dynamically before transmitting data to use the egress buffer sequencing"
***********************************************************************/
void eth100g_reg_rspec_chnl_seq_sel_buf_seq_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_seq);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_seq);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_seq_sel_buf_seq(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_seq_sel_buf_seq_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_seq);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_seq);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_seq_sel_buf_seq(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_seq_sel_buf_seq_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_seq_sel_buf_seq_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_seq_sel_buf_seq_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Enable Tx fifo for traffic"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_chnl_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_chnl_ena(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_chnl_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_chnl_ena(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_chnl_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_chnl_ena_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_chnl_ena_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, put the Tx fifo in flush mode, discarding any new packet coming from Egress Buffer"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_tx_flush_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_tx_flush(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_tx_flush_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_tx_flush(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_tx_flush_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_tx_flush_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_tx_flush_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Define the channel configuration as follow: "
                   " - 00b: 100G Mode (channel 0 only) "
                   " - 01b: 50G Mode (channel 0/2 only) "
                   " - 10b: 10G Mode (channel 0/1/2/3) "
                   " - 11b: reserved "
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_chnl_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_chnl_mode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_chnl_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_chnl_mode(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_chnl_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_chnl_mode_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_chnl_mode_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, check whether all PFC XOFF are set to also report port XOFF to the core of the chip"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_en_rx_xoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_en_rx_xoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_en_rx_xoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_en_rx_xoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_en_rx_xoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_en_rx_xoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_en_rx_xoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, replace the Rx PFC XOFF received and extracted by the MAC by a fix value provided by the register space"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_ovr_rx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, loopback the Tx data (from Egress Buffer) back to Rx (to Ingress Parser Buffer)"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_txrx_lpbk_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_txrx_lpbk(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_txrx_lpbk_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_txrx_lpbk(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_txrx_lpbk_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_txrx_lpbk_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_txrx_lpbk_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Defines the value of Rx PFC XOFF when provided by the register space (one bit per PFC)"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_val_rx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Defines the amount of credit to send to Egress Buffer when channel get enabled (one credit is equivalent to 16B data payload)"
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_cred_ini_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_cred_ini(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_cred_ini_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_cred_ini(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_cred_ini_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_cred_ini_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_cred_ini_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Defines the number of 16B data payload to be accumulated on a start of packet before forwarding the packet to the Tx MAC."
***********************************************************************/
void eth100g_reg_rspec_txff_ctrl_min_thr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_ctrl_min_thr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_ctrl_min_thr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_ctrl_min_thr(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_ctrl_min_thr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_ctrl_min_thr_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_ctrl_min_thr_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 This field can be programmed to non-zero value when "
                   "the MAC IP is programmed in single channel mode but that "
                   "the mode in not 100G. Programming this register to 3 will "
                   "insert a wait-states every 4 clock cycles when sending to "
                   "the MAC IP."
***********************************************************************/
void eth100g_reg_rspec_mac_ctrl_ins_ws_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mac_ctrl_ins_ws(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mac_ctrl_ins_ws_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mac_ctrl_ins_ws(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mac_ctrl_ins_ws_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mac_ctrl_ins_ws_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mac_ctrl_ins_ws_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, disable FCS insertion by MAC layer" 
***********************************************************************/
void eth100g_reg_rspec_mac_ctrl_txdisfcs_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mac_ctrl_txdisfcs(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mac_ctrl_txdisfcs_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mac_ctrl_txdisfcs(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mac_ctrl_txdisfcs_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mac_ctrl_txdisfcs_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mac_ctrl_txdisfcs_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, disable PAD insertion by MAC layer" 
***********************************************************************/
void eth100g_reg_rspec_mac_ctrl_txdispad_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mac_ctrl_txdispad(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mac_ctrl_txdispad_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mac_ctrl_txdispad(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mac_ctrl_txdispad_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mac_ctrl_txdispad_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mac_ctrl_txdispad_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the fix preamble to sent with the packet"
***********************************************************************/
void eth100g_reg_rspec_txff_pream0_preamble_lsb_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_pream0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_pream0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_pream0_preamble_lsb(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_pream0_preamble_lsb_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_pream0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_pream0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_pream0_preamble_lsb(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_txff_pream0_preamble_lsb_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_pream0_preamble_lsb_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_pream0_preamble_lsb_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the fix preamble to sent with the packet"
***********************************************************************/
void eth100g_reg_rspec_txff_pream1_preamble_msb_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_pream1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_pream1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_pream1_preamble_msb(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_pream1_preamble_msb_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_pream1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_pream1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_pream1_preamble_msb(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_txff_pream1_preamble_msb_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_pream1_preamble_msb_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_pream1_preamble_msb_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the IPG after each packet when enabled inside the MAC"
***********************************************************************/
void eth100g_reg_rspec_txff_pream1_tx_ipg_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_pream1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_pream1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_pream1_tx_ipg(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_pream1_tx_ipg_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_pream1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_pream1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_pream1_tx_ipg(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_txff_pream1_tx_ipg_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_pream1_tx_ipg_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_pream1_tx_ipg_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report the Rx PFC XOFF reported by the MAC layer"
***********************************************************************/
void eth100g_reg_rspec_txff_status_pfc_rxxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_status[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_status[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_status_pfc_rxxoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_status_pfc_rxxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_status[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_status[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_status_pfc_rxxoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_status_pfc_rxxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_status_pfc_rxxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_status_pfc_rxxoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Report the Tx fifo current count in 16B increment"
***********************************************************************/
void eth100g_reg_rspec_txff_status_txff_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_status[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_status[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_status_txff_count(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_status_txff_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_status[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_status[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_status_txff_count(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_status_txff_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_status_txff_count_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_status_txff_count_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Report the Tx application fifo current count in 64B increment"
***********************************************************************/
void eth100g_reg_rspec_txff_status_txappfifo_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_status[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_status[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txff_status_txappfifo_count(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txff_status_txappfifo_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txff_status[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txff_status[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txff_status_txappfifo_count(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txff_status_txappfifo_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txff_status_txappfifo_count_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txff_status_txappfifo_count_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, globally disable the internal CRC regeneration from the MAC layer"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_crcgen_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_crcgen_dis(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_crcgen_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_crcgen_dis(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_crcgen_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_crcgen_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_crcgen_dis_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, disable the internal CRC regeneration from the MAC layer only for frames received with error"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_crcerr_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_crcerr_dis(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_crcerr_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_crcerr_dis(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_crcerr_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_crcerr_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_crcerr_dis_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, assert all PFC XOFF bits when Tx XOFF is received from the core of the chip for that channel"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_en_tx_xoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_en_tx_xoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_en_tx_xoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_en_tx_xoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_en_tx_xoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_en_tx_xoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_en_tx_xoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, replace the Tx PFC XOFF received from IBP and sent to the MAC the fix value provided by the register space"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_ovr_tx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, replace the Tx XOFF received from IBP and sent to the MAC the fix value provided by the register space"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_ovr_tx_xoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, replace the Tx XOFF received from IBP and sent to the MAC the fix value provided by the register space"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_val_tx_xoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_val_tx_xoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_val_tx_xoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_val_tx_xoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_val_tx_xoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_val_tx_xoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_val_tx_xoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Defines the value of Tx PFC XOFF when provided by the register space (one bit per PFC)"
***********************************************************************/
void eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxff_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxff_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxff_ctrl_val_tx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, globally disable the internal CRC checker from the MAC layer"
***********************************************************************/
void eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txcrc_trunc_ctrl_crcchk_dis_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, disable the internal CRC checker and do not remove the 4B CRC before sending to the Tx MAC layer (MAC should be dynamically controlled to not generate Tx CRC."
***********************************************************************/
void eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txcrc_trunc_ctrl_crcerr_dis_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 when set, disable the internal CRC regeneration from the MAC layer only for frames received with error"
***********************************************************************/
void eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txcrc_trunc_ctrl_crcrmv_dis_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Enable packet truncation for that channel"
***********************************************************************/
void eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_ena_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Report the size in byte for truncating packet. "
                   "Truncated packet will have valid CRC32 independantly of the CRC received from Deparser."
***********************************************************************/
void eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txcrc_trunc_ctrl[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_txcrc_trunc_ctrl_trunc_size_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Capture the Rx status of last error packet received"
***********************************************************************/
void eth100g_reg_rspec_rxpkt_err_sts_last_rxsts_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0 ;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxpkt_err_sts[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxpkt_err_sts[ch]);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s", dev_id, umac, ch, val, __func__);
  setp_tof2_eth100g_reg_rspec_rxpkt_err_sts_last_rxsts(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_rxpkt_err_sts_last_rxsts_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.rxpkt_err_sts[ch]);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.rxpkt_err_sts[ch]);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_rxpkt_err_sts_last_rxsts(data_p);
  autogen_log("TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s", dev_id, umac, ch, *val, __func__);
}

void eth100g_reg_rspec_rxpkt_err_sts_last_rxsts_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_rxpkt_err_sts_last_rxsts_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth100g_reg_rspec_rxpkt_err_sts_last_rxsts_set( dev_id, umac, ch, data_p, val, true);
}

/**********************************************************************
 Allow to flexibly map internal status to generate port_alive status for TM and packet generator : "
                   " port_alive = port_alive_lut[{ofault,ext_rxsigok,rxsigok,txgood,rxsync}] "
                   " Thus if only rxsync contributes to port_alive, programming is 16'hAAAA."
***********************************************************************/
void eth100g_reg_rspec_port_alive_lut_port_alive_lut_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.port_alive_lut);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.port_alive_lut);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_port_alive_lut_port_alive_lut(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_port_alive_lut_port_alive_lut_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.port_alive_lut);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.port_alive_lut);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_port_alive_lut_port_alive_lut(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_port_alive_lut_port_alive_lut_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_port_alive_lut_port_alive_lut_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_port_alive_lut_port_alive_lut_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report which interrupt register is asserted: "
                   " - bit 0 : Channel interrupt "
                   " - bit 1 : Memory interrupt "
                   " - bit 2 : MAC interrupt "
                   " - bit 3 : MDIOCI interrupt "
                   " - bit 4 : Microcontroller interrupt. "
***********************************************************************/
void eth100g_reg_rspec_global_intr_stat_int_lo_stat_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.global_intr_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.global_intr_stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_global_intr_stat_int_lo_stat(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_global_intr_stat_int_lo_stat_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.global_intr_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.global_intr_stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_global_intr_stat_int_lo_stat(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_global_intr_stat_int_lo_stat_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_global_intr_stat_int_lo_stat_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_global_intr_stat_int_lo_stat_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report which interrupt register is asserted: "
                   " - bit 0 : Channel interrupt "
                   " - bit 1 : Memory interrupt "
                   " - bit 2 : MAC interrupt "
                   " - bit 3 : MDIOCI interrupt "
                   " - bit 4 : Microcontroller interrupt. "
***********************************************************************/
void eth100g_reg_rspec_global_intr_stat_int_hi_stat_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.global_intr_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.global_intr_stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_global_intr_stat_int_hi_stat(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_global_intr_stat_int_hi_stat_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.global_intr_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.global_intr_stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_global_intr_stat_int_hi_stat(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_global_intr_stat_int_hi_stat_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_global_intr_stat_int_hi_stat_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_global_intr_stat_int_hi_stat_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject a crc error for each channel"
***********************************************************************/
void eth100g_reg_rspec_crcerr_inj_inj_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.crcerr_inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.crcerr_inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_crcerr_inj_inj(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_crcerr_inj_inj_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.crcerr_inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.crcerr_inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_crcerr_inj_inj(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_crcerr_inj_inj_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_crcerr_inj_inj_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_crcerr_inj_inj_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the base address of debug fifo structure"
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_base_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_base_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_base_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_base_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_base_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_base_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_base_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the limit address of debug fifo structure"
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_limit_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_limit_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_limit_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_limit_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_limit_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_limit_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_limit_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, the debug buffer will capture all register transactions generated by TV80. "
                   "Each instruction will be 32-bit quanta and will logged as follow: : "
                   " -first address bit[31:30] : Instruction code (00b for register access) "
                   " -first address bit[29]  : Error during transaction acknowledge phase "
                   " -first address bit[28]  : Read (1b) / Write (0b) "
                   " -first address bit[27:0] : Word address of the transaction "
                   " -second address bit[31:0] : Write data or read data "
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_log_reg_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_log_reg(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_log_reg_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_log_reg(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_log_reg_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_log_reg_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_log_reg_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, the debug buffer will capture all instruction and data read by the microcontroller. "
                   "Each instruction will be 32-bit quanta and will logged as follow: "
                   " - bit[31:30] : Instruction Code (11b) "
                   " - bit[29]  : M1 cycle "
                   " - bit[28]  : Read (1b) / Write (0b) "
                   " - bit[23:16] : Data "
                   " - bit[13:0] : Address "
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_log_inst_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_log_inst(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_log_inst_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_log_inst(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_log_inst_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_log_inst_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_log_inst_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, the debug buffer works in fifo mode which stalls automatically when fifo is full. "
                   " When cleared, the fifo works in circular buffer mode, overriding the oldest instruction executed."
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_fifomode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_fifomode(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_fifomode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_fifomode(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_fifomode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_fifomode_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_fifomode_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, assert an interrupt when debug fifo get full. It is only applicable if debug works in fifo mode and not in circular buffer"
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_full_int_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_full_int(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_full_int_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_full_int(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_full_int_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_full_int_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_full_int_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 when set, enable the debug fifo structure"
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_ctrl_enable_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_enable(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_ctrl_enable_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_ctrl_enable(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_ctrl_enable_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_ctrl_enable_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_ctrl_enable_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the current write address of debug fifo structure"
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_tail_ptr);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_tail_ptr);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_tail_ptr);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_tail_ptr);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_tail_ptr_tail_ptr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the current read address of the debug fifo"
***********************************************************************/
void eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_head_ptr);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_head_ptr);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_debug_head_ptr);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_debug_head_ptr);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_debug_head_ptr_head_ptr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the maximum of the timeout counter."
***********************************************************************/
void eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_watchdog_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_watchdog_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_watchdog_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_watchdog_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_watchdog_ctrl_timeout_value_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Enables the watchdog timeout mechanism to trigger NMI."
***********************************************************************/
void eth100g_reg_rspec_tv80_watchdog_ctrl_enable_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_watchdog_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_watchdog_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_watchdog_ctrl_enable(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_watchdog_ctrl_enable_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_watchdog_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_watchdog_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_watchdog_ctrl_enable(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_watchdog_ctrl_enable_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_watchdog_ctrl_enable_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_watchdog_ctrl_enable_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the maximum of the timeout counter."
***********************************************************************/
void eth100g_reg_rspec_tv80_watchdog_count_cur_time_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_watchdog_count);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_watchdog_count);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_watchdog_count_cur_time(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_watchdog_count_cur_time_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_watchdog_count);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_watchdog_count);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_watchdog_count_cur_time(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_watchdog_count_cur_time_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_watchdog_count_cur_time_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_watchdog_count_cur_time_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, the microcontroller code execution is halted through the logic "
                   "does not responding to TV80 trying to read the next instruction from its firmware located in SRAM."
                   " SW will have to clear this bit to enable TV80 to resume its operation."
***********************************************************************/
void eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_stall_on_error);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_stall_on_error);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_stall_on_error_stall_absolute(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_stall_on_error);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_stall_on_error);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_stall_on_error_stall_absolute(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_stall_on_error_stall_absolute_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, the TV80 will be stalled when a memory multi-bit error is detected on its own SRAM containing data/firmware. SW will have to clear this bit to enable TV80 to resume its operation."
***********************************************************************/
void eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_stall_on_error);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_stall_on_error);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_stall_on_error);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_stall_on_error);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_stall_on_error_stall_on_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set, the TV80 will be stalled when a read error transaction generated by TV80."
***********************************************************************/
void eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_stall_on_error);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_stall_on_error);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_stall_on_error);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_stall_on_error);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_stall_on_error_stall_on_rerr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Status register reporting that the TV80 microcontroller has been halted."
***********************************************************************/
void eth100g_reg_rspec_tv80_halted_status_tv80_halted_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_halted_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_halted_status);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_halted_status_tv80_halted(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_halted_status_tv80_halted_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_halted_status);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_halted_status);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_halted_status_tv80_halted(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_halted_status_tv80_halted_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_halted_status_tv80_halted_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_halted_status_tv80_halted_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the number of consecutive clock cycle that the Rx_idle_det need to"
                   "be asserted high or low to report rxSigOK to the MAC layer. Set to zero to prevent any debouncing "
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_ctrl_sigok_debounce_count_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Ignore Signal OK from Serdes and forces RxSigOK high (per lane) "
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Ignore Signal OK from Serdes and forces RxSigOK low (per lane)"
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Enables access to MDIOCI interface"
***********************************************************************/
void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_en_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generate a reset command to MDIOCI interface"
***********************************************************************/
void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_reset_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the clock speed of the MDIOCI interface clock"
                   " 0001b : Core clock divider by 2 "
                   " 0011b : Core clock divider by 4 "
                   " 0111b : Core clock divider by 8 "
                   " 1111b : Core clock divider by 16 "
                   "The MDC clock output is generated when the 4-bit counter reaches it max value."
***********************************************************************/
void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mdioci_ctrl_mdioci_clkdiv_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the address of the interrupt to be polled"
***********************************************************************/
void eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_poll_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_poll_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_poll_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_poll_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mdioci_poll_ctrl_poll_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Define the address of the interrupt to be polled"
***********************************************************************/
void eth100g_reg_rspec_eth_mdioci_poll_time_poll_time_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_poll_time);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_poll_time);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mdioci_poll_time_poll_time(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_mdioci_poll_time_poll_time_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_poll_time);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_poll_time);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mdioci_poll_time_poll_time(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mdioci_poll_time_poll_time_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_mdioci_poll_time_poll_time_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mdioci_poll_time_poll_time_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 enables polling of RxSignalOK"
***********************************************************************/
void eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_poll_time);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_poll_time);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_mdioci_poll_time);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_mdioci_poll_time);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_mdioci_poll_time_poll_ena_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_rxsigok_bitsel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_rxsigok_bitsel);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Report which interrupt register is asserted"
***********************************************************************/
void eth100g_reg_rspec_mdioci_intr_stat_intr_stat_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mdioci_intr_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mdioci_intr_stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mdioci_intr_stat_intr_stat(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mdioci_intr_stat_intr_stat_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mdioci_intr_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mdioci_intr_stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mdioci_intr_stat_intr_stat(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mdioci_intr_stat_intr_stat_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mdioci_intr_stat_intr_stat_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mdioci_intr_stat_intr_stat_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Select Rx clock source for PPM comparator."
***********************************************************************/
void eth100g_reg_rspec_eth_ppm_sel_sel_rxclk_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_sel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_sel);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ppm_sel_sel_rxclk(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ppm_sel_sel_rxclk_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_sel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_sel);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ppm_sel_sel_rxclk(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ppm_sel_sel_rxclk_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ppm_sel_sel_rxclk_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ppm_sel_sel_rxclk_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Select Tx clock source for PPM comparator."
***********************************************************************/
void eth100g_reg_rspec_eth_ppm_sel_sel_txclk_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_sel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_sel);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ppm_sel_sel_txclk(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ppm_sel_sel_txclk_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_sel);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_sel);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ppm_sel_sel_txclk(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ppm_sel_sel_txclk_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ppm_sel_sel_txclk_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ppm_sel_sel_txclk_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Defines the number of clock cycles for the PPM comparator to run before reporting valid PPM differences. "
***********************************************************************/
void eth100g_reg_rspec_eth_ppm_ctrl_max_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ppm_ctrl_max_count(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ppm_ctrl_max_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ppm_ctrl_max_count(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ppm_ctrl_max_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ppm_ctrl_max_count_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ppm_ctrl_max_count_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When set to 1, the PPM comparator logic is enabled. It is required to have valid clock source before enabling the PPM comparator. The PPM comparator runs continuously as long as enabled."
***********************************************************************/
void eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_ctrl);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_ctrl);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_ctrl);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ppm_ctrl_ppm_ena_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Represent a signed number of PPM seen between Tx and Rx clock. The logic add 1 clock uncertainty and thus to get an accuracy of 1 PPM, "
                   " it is recommended to run the PPM comparator for at least 10 M cycles. New value get reported automatically after each iteration as long as enabled. "
***********************************************************************/
void eth100g_reg_rspec_eth_ppm_stat_ppm_seen_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ppm_stat_ppm_seen(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ppm_stat_ppm_seen_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ppm_stat_ppm_seen(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ppm_stat_ppm_seen_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ppm_stat_ppm_seen_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ppm_stat_ppm_seen_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 When a result is available after been enabled, report 1 when the PPM comparator has a valid value to report."
***********************************************************************/
void eth100g_reg_rspec_eth_ppm_stat_ppm_val_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_eth_ppm_stat_ppm_val(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_eth_ppm_stat_ppm_val_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.eth_ppm_stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.eth_ppm_stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_eth_ppm_stat_ppm_val(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_eth_ppm_stat_ppm_val_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_eth_ppm_stat_ppm_val_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_eth_ppm_stat_ppm_val_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_stat_rxeop_timo3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en0_rxeop_timo3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_en1_rxeop_timo3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txcrc_err0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txcrc_err1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txcrc_err2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txcrc_err3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txcrc_err3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.chnl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.chnl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_chnl_intr_inj_rxeop_timo3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_stat_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_stat_txfifo_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_stat_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_stat_txfifo_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_stat_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_stat_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_stat_txfifo_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_stat_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_stat_txfifo_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_stat_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_stat_txfifo_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_stat_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_stat_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_stat_txfifo_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_stat_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_stat_statsmem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_stat_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_stat_statsmem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_stat_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_stat_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_stat_statsmem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_stat_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_stat_statsmem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_stat_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_stat_statsmem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_stat_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_stat_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_stat_statsmem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_stat_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_stat_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en0_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en0_txfifo_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en0_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en0_txfifo_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en0_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en0_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en0_txfifo_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en0_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en0_txfifo_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en0_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en0_txfifo_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en0_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en0_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en0_txfifo_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en0_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en0_statsmem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en0_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en0_statsmem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en0_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en0_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en0_statsmem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en0_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en0_statsmem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en0_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en0_statsmem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en0_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en0_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en0_statsmem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en0_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en0_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en1_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en1_txfifo_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en1_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en1_txfifo_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en1_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en1_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en1_txfifo_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en1_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en1_txfifo_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en1_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en1_txfifo_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en1_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en1_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en1_txfifo_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en1_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en1_statsmem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en1_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en1_statsmem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en1_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en1_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en1_statsmem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en1_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en1_statsmem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en1_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en1_statsmem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en1_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en1_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en1_statsmem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en1_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_en1_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_inj_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_inj_txfifo_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_inj_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_inj_txfifo_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_inj_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_inj_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_inj_txfifo_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_inj_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_inj_txfifo_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_inj_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_inj_txfifo_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_inj_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_inj_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_inj_txfifo_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_inj_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_inj_statsmem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_inj_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_inj_statsmem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_inj_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_inj_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_inj_statsmem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_inj_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_inj_statsmem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_inj_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_inj_statsmem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_inj_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_inj_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_inj_statsmem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_inj_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
void eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_intr_inj_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_int7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_int7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_int7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_int7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_int7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_int7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_rerr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_rerr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_rerr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_stat_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_stat_debug(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_stat_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_stat_debug(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_stat_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_stat_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_stat_debug_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_int7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_int7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_int7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_int7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_int7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_int7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_rerr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_rerr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_rerr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en0_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en0_debug(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en0_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en0_debug(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en0_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en0_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en0_debug_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_int7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_int7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_int7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_int7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_int7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_int7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_rerr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_rerr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_rerr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_en1_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_en1_debug(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_en1_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_en1_debug(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_en1_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_en1_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_en1_debug_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_int7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_int7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_int7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_int7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_int7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_int7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_rerr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_rerr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_rerr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
void eth100g_reg_rspec_uctrl_intr_inj_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_uctrl_intr_inj_debug(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_uctrl_intr_inj_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.uctrl_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.uctrl_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_uctrl_intr_inj_debug(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_uctrl_intr_inj_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_uctrl_intr_inj_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_uctrl_intr_inj_debug_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Disable Error Checking
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_txfifo_sram_disable_check_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_txfifo_sram_inject_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Disable Error Checking
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_statsmem_disable_check_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_statsmem_disable_check(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_statsmem_disable_check_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_statsmem_disable_check(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_statsmem_disable_check_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_statsmem_disable_check_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_statsmem_disable_check_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_statsmem_inject_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_statsmem_inject_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Disable Error Checking
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_tv80mem_disable_check_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_tv80mem_disable_check(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_tv80mem_disable_check_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_tv80mem_disable_check(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_tv80mem_disable_check_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_tv80mem_disable_check_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_tv80mem_disable_check_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_tv80mem_inject_sbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
void eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mem_ecc);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mem_ecc);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mem_ecc_tv80mem_inject_mbe_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Log Register for Tx fifo Dual Bit Error
***********************************************************************/
void eth100g_reg_rspec_txfifo_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txfifo_mbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txfifo_mbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_txfifo_mbe_err_log_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_txfifo_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.txfifo_mbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.txfifo_mbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_txfifo_mbe_err_log_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_txfifo_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_txfifo_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_txfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Log Register for Statistic Memory Single Bit Error
***********************************************************************/
void eth100g_reg_rspec_statsmem_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.statsmem_sbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.statsmem_sbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_statsmem_sbe_err_log_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_statsmem_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.statsmem_sbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.statsmem_sbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_statsmem_sbe_err_log_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_statsmem_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_statsmem_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_statsmem_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Log Register for Statistic Memory Dual Bit Error
***********************************************************************/
void eth100g_reg_rspec_statsmem_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.statsmem_mbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.statsmem_mbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_statsmem_mbe_err_log_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_statsmem_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.statsmem_mbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.statsmem_mbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_statsmem_mbe_err_log_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_statsmem_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_statsmem_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_statsmem_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Log Register for Microcontroller Memory Single Bit Error
***********************************************************************/
void eth100g_reg_rspec_tv80mem_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80mem_sbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80mem_sbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80mem_sbe_err_log_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80mem_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80mem_sbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80mem_sbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80mem_sbe_err_log_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80mem_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80mem_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80mem_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Log Register for Microcontroller Memory Dual Bit Error
***********************************************************************/
void eth100g_reg_rspec_tv80mem_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80mem_mbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80mem_mbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80mem_mbe_err_log_addr(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80mem_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80mem_mbe_err_log);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80mem_mbe_err_log);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80mem_mbe_err_log_addr(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80mem_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80mem_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80mem_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
void eth100g_reg_rspec_mac_en0_mac_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mac_en0_mac(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mac_en0_mac_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mac_en0_mac(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mac_en0_mac_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mac_en0_mac_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mac_en0_mac_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
void eth100g_reg_rspec_mac_en1_mac_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mac_en1_mac(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mac_en1_mac_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mac_en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mac_en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mac_en1_mac(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mac_en1_mac_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mac_en1_mac_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mac_en1_mac_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_int_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_int_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_int_7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_int_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_int_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_int_7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_stat_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_stat_nmi(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_stat_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.stat);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.stat);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_stat_nmi(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_stat_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_stat_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_stat_nmi_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_int_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_int_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_int_7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_int_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_int_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_int_7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en0_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en0_nmi(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en0_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en0_nmi(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en0_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en0_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en0_nmi_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_int_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_int_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_int_7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_int_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_int_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_int_7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_en1_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_en1_nmi(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_en1_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_en1_nmi(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_en1_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_en1_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_en1_nmi_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_1(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_1(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_1_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_2(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_2(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_2_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_3(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_3(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_3_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_4(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_4(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_4_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_5(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_5(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_5_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_6(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_6(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_6_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_int_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_7(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_int_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_int_7(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_int_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_int_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_int_7_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
void eth100g_reg_rspec_tv80_intr_inj_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_tv80_intr_inj_nmi(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_tv80_intr_inj_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.tv80_intr.inj);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.tv80_intr.inj);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_tv80_intr_inj_nmi(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_tv80_intr_inj_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_tv80_intr_inj_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_tv80_intr_inj_nmi_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
void eth100g_reg_rspec_mdioci_en0_int0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mdioci_en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mdioci_en0);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mdioci_en0_int0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mdioci_en0_int0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mdioci_en0);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mdioci_en0);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mdioci_en0_int0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mdioci_en0_int0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mdioci_en0_int0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mdioci_en0_int0_set( dev_id, umac, data_p, val, true);
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
void eth100g_reg_rspec_mdioci_en1_int0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mdioci_en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mdioci_en1);
  } else {
    bf_sys_assert(0);
  }
  autogen_log("TRC : %d: p%02d :     : Wr : -------- : %08x : %s", dev_id, umac, (uint32_t)val, __func__);
  setp_tof2_eth100g_reg_rspec_mdioci_en1_int0(data_p, val);

  if (hw) {
    autogen_wr(dev_id, ofs, *data_p);
  }
}

void eth100g_reg_rspec_mdioci_en1_int0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = 0;

  if (umac == 0) {
    ofs = offsetof(tof2_reg, eth100g_regs.eth100g_reg.mdioci_en1);
  } else if (umac == 39) {
    ofs = offsetof(tof2_reg, eth100g_regs_rot.eth100g_reg.mdioci_en1);
  } else {
    bf_sys_assert(0);
  }
  if (hw) {
    autogen_rd(dev_id, ofs, data_p);
  }
  *val = getp_tof2_eth100g_reg_rspec_mdioci_en1_int0(data_p);
  autogen_log("TRC : %d: p%02d :     : Rd : -------- : %08x : %s", dev_id, umac, *val, __func__);
}

void eth100g_reg_rspec_mdioci_en1_int0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  eth100g_reg_rspec_mdioci_en1_int0_get( dev_id, umac, data_p, &unused_fld, true);
  eth100g_reg_rspec_mdioci_en1_int0_set( dev_id, umac, data_p, val, true);
}

