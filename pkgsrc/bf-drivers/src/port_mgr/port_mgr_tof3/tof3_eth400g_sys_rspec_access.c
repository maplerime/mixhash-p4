/* clang-format off */

#include "tof3-autogen-required-headers.h"

#include <inttypes.h>
#include <stddef.h>
#include <bf_types/bf_types.h>
#include <tof3_regs/tof3_reg_drv.h>
#include "port_mgr_tof3.h"


/**********************************************************************
             When set to 1, the RxMAC removes the FCS field from the 
            packet.

**********************************************************************/

void tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_mac[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_mac[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Defines the number of bytes in Tx Preamble sent (8 for 
            Ethernet, 0,1,2,4 for Fabric)

**********************************************************************/

void tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_mac[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_mac[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Defines the Tx playout margin for that channel (increment by 
            step of 4)

**********************************************************************/

void tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_mac[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_mac[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Place the Tx MAC logic in soft reset

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Place the Rx MAC logic in soft reset

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_sreset[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set, check that control packet are 64B exactly

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Defines the (average) number of IFG to insert between packets

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, stomp all FCS inserted on the packet.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, FCS is already part of the packet and MAC 
            swaps with new FCS.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, PTP 1 step zero the checksum instead of 
            updating checksum with egress time adjustment

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, the Tx check the LT field for length type and 
            compare it to packet length.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, the Tx MAC enters flush mode. Pause is 
            disregarded, RS layer goes in Tx Fault Idle state. Tx fifo is 
            flushed. Stats are still updated.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, the IFG DIC mode logic compensates for AM 
            bandwidth. Should be set to 1 for spec compliance.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, the Inter Frame Gap insertion function is 
            operating in DIC mode.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Enables transmission of packets to the MAC Tx fifo. 

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Define the maximum packet size which can be transmitted 
            before reporting an error.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_pkt_len[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_pkt_len[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Define the minimum packet size on transmit. Any packet 
            smaller than that received on EBUF interface will be padded 
            by the MAC. The value defined include FCS.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_pkt_len[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_pkt_len[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Define the Tx fifo almost full threshold to prevent the Tx 
            fifo from overflowing.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_fifo[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_fifo[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             defines the Tx fifo almost empty threshold which applies on a 
            start of packet to forward to MAC/PCS layer.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_fifo[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_tx_fifo[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Disregard the full flag in Rx app fifo. Need to be set low. 
            Potentially use for backdoor extra room of 1-2 entries.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set, check that control packet are 64B exactly

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set to 1, the Rx check the LT field for length type and 
            compare it to packet length.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Enable reception of packet by the MAC

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Define the maximum packet size which can be received before 
            reporting an error.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx_pkt_len[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx_pkt_len[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Define the minimum packet size which can be received before 
            reporting an error.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx_pkt_len[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_sys_mac_rx_pkt_len[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             defines the Rx MAC destination to decodefor identification of 
            unicast Pause frame

**********************************************************************/

void tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_pause_rx_mac_addr[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.static_pause_rx_mac_addr[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Enables the decoding of PFC frames and defines which PFC is 
            enabled for generating Rx PFC status.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_pause_rx_ctrl[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_pause_rx_ctrl[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Enables the decoding of Pause frame and generation of Rx Port 
            Pause status

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_pause_rx_ctrl[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_pause_rx_ctrl[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Report the Tx Control Path fifo current depth 

**********************************************************************/

void tof3_eth400g_sys_rspec_txfifo_depth_cp_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.txfifo_depth[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_txfifo_depth_cp(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_txfifo_depth_cp_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.txfifo_depth[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_txfifo_depth_cp(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_txfifo_depth_cp_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_txfifo_depth_cp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_txfifo_depth_cp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Report the Tx Data Path fifo current depth 

**********************************************************************/

void tof3_eth400g_sys_rspec_txfifo_depth_dp_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.txfifo_depth[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_txfifo_depth_dp(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_txfifo_depth_dp_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.txfifo_depth[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_txfifo_depth_dp(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_txfifo_depth_dp_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_txfifo_depth_dp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_txfifo_depth_dp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             enables the Rx Runt packet filtering

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             enables the Rx PFC packet filtering

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             enables the SOP-EOP rule functionality which terminates 
            packet automatically after long period of idle time.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             enables the Rx truncation functionality

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             When set, checks for destination address against the 
            static_mac_addr

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             .

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Defines the length of fabric header for decoding fabric 
            Pause/PFC frames: 
             - 00b: Disabled (Ethernet mode) 
             - 01b: 12B fabric header 
             - 10b: 16B fabric header 
             - 11b: 20B fabric header 
            

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Defines the number of quanta (16 clocks) for an opened packet 
            without valid dataphase to automatically close the packet. 
            Value 0-7 should be avoided for proper operation.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Controls when a packet gets truncated on Rx path and closed 
            with EOP with Error. The value programmed include the FCS 
            field (if RxFCS is not removed) and should be programmed as 
            the last packet size which is accepted.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_rxfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             enables the Rx truncation functionality

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_txfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_txfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Controls when a packet gets truncated on Tx path and closed 
            with EOP with Error. The value programmed does not include 
            the FCS field (as Tx FCS is removed at that level) and should 
            be programmed as the last packet size which is accepted.

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_txfilt[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_txfilt[ch]);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}

/**********************************************************************
             Write data register bit [31:0]

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_wdata0);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_wdata0);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             Write data register bit [63:32]

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_wdata1);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_wdata1);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             When set to 1, the indirect request is sent to the statistic 
            memory. This field gets automatically cleared by HW when the 
            request completes. 

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             Defines whether the indirect transaction is a write request 
            or a read request 

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             Defines the statistic memory address which need to be 
            accessed. Each statistics counter is 64-bit and the address 
            should represent (96*channel + counter_offset) (96 counters 
            per channel). 

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_ctrl);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             Read data register bit [31:0]

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_rdata0);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_rdata0);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             Read data register bit [63:32]

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_rdata1);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_rdata1);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             Report a dual ECC error happening during the read operation

**********************************************************************/

void tof3_eth400g_sys_rspec_statmem_csr_rerr_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_rerr);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_statmem_csr_rerr_data(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_statmem_csr_rerr_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.statmem_csr_rerr);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_statmem_csr_rerr_data(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_statmem_csr_rerr_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_statmem_csr_rerr_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_rerr_data_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             One bit per channel. Automatically cleared by hardware when 
            clear operation completes. 

**********************************************************************/

void tof3_eth400g_sys_rspec_stat_clear_req_chan_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.stat_clear_req);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_stat_clear_req_chan(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_stat_clear_req_chan_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.stat_clear_req);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_stat_clear_req_chan(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_stat_clear_req_chan_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_stat_clear_req_chan_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_stat_clear_req_chan_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             globally configures Rx-to-Tx loopoback for all channels 

**********************************************************************/

void tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_mac_lpbk);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cfg_mac_lpbk);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

void tof3_eth400g_sys_rspec_cts_fifo_stat_count3_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cts_fifo_stat_count3(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count3_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cts_fifo_stat_count3(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count3_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count3_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count3_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

void tof3_eth400g_sys_rspec_cts_fifo_stat_count2_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cts_fifo_stat_count2(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count2_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cts_fifo_stat_count2(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count2_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count2_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count2_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

void tof3_eth400g_sys_rspec_cts_fifo_stat_count1_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cts_fifo_stat_count1(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count1_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cts_fifo_stat_count1(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count1_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count1_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count1_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

void tof3_eth400g_sys_rspec_cts_fifo_stat_count0_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cts_fifo_stat_count0(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count0_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_stat);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_eth400g_sys_rspec_cts_fifo_stat_count0(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cts_fifo_stat_count0_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count0_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count0_set(dev_id, subdev_id, tmac, data_p, val, true);
}

/**********************************************************************
             When the fifo is empty return 0x0, otherwise return: 
             - [47:0] : Capture timestamp 
             - [56:48]: Timestamp ID (9-bit self incrementing allowing to 
            number of lost ID) 
             - [62:57]: Reserved 
             - [63]   : Valid entry when set to 1 

**********************************************************************/

void tof3_eth400g_sys_rspec_cts_fifo_out_cts_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_out[ch]);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_eth400g_sys_rspec_cts_fifo_out_cts(data_p, val);

  if (hw) {
    uint32_t reg_val;
    reg_val = (uint32_t)(*data_p & 0xffffffffull);
    autogen_tmac_wr(dev_id, subdev_id, ofs, reg_val);
    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);
    autogen_tmac_wr(dev_id, subdev_id, ofs + 4, reg_val);
  }
}

void tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].eth400g_sys.cts_fifo_out[ch]);

  if (hw) {
    uint32_t upper, lower;
    autogen_tmac_rd(dev_id, subdev_id, ofs, &lower);
    autogen_tmac_rd(dev_id, subdev_id, ofs + 4, &upper);
    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));
  }

  *val = getp_tof3_eth400g_sys_rspec_cts_fifo_out_cts(data_p);
  tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void tof3_eth400g_sys_rspec_cts_fifo_out_cts_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;

  tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_out_cts_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
}
