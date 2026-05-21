/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <bf_types/bf_types.h>
#include <tof3_regs/tof3_reg_drv.h>
#include "port_mgr_tof3.h"

#include "tof3_eth400g_app_rspec_access.h"
extern bf_status_t bf_map_logical_tmac_to_physical(bf_dev_id_t dev_id,
                                                   bf_subdev_id_t *subdev_id,
                                                   uint32_t logical_tmac,
                                                   uint32_t *physical_tmac);

/**********************************************************************
             Each bit triggers a different software reset or control: 
             -[0]     : Reset MAC/PCS (can be asserted without app logic 
            being asserted) 
             -[1]     : Reset App logic (should only be asserted if 
            MAC/PCS is also reset) 
             -[2]     : Reset microcontroller (should be kept asserted as 
            long as firmware is not loaded)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_soft_reset_eth_swrst_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Can be set dynamically before transmitting data to use the 
            egress buffer sequencing

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_chnl_seq_sel_buf_seq_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Each 2-bit correspond to a channel slot. By default, each 
            100G channel has one time slot and must be sequenced as 
            follow 0,2,1,3. When any channel is configured in  200G or 
            400G mode, it consumes the slot of the next channels (up to 
            creating the required number of timeslot. Those timeslot are 
            fixed and should not be modified for the channels which are 
            not reconfigured.  A 200G channel requires timeslot every 2 
            clock cycles. A 400G channel requires timeslot every clock 
            cycle. Common configuration are as follow: 
             - 1x400G : 0x00 
             - 2x200G : 0x88 
             - 4x100G : 0xD8 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_chnl_seq_chnl_seq_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_chnl_seq_chnl_seq_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_chnl_seq_chnl_seq_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_chnl_seq_chnl_seq_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_chnl_seq_chnl_seq_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_chnl_seq_chnl_seq_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_chnl_seq_chnl_seq_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Allow to flexibly map internal status to generate port_alive 
            status for TM and packet generator : 
             port_alive = 
            port_alive_lut[{ofault,ext_rxsigok,rxsigok,txgood,rxsync}] 
             Thus if only rxsync contributes to port_alive, programming 
            is 16'hAAAA.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_port_alive_lut_port_alive_lut_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Increment/Drift on MAC clock to automatically increment 
            timestamp everyMAC clock cycle (4 lsb represent ns and 12 lsb 
            represent ps)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Offset which is added to timestamp received from MBus Station

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report which interrupt register is asserted: 
             - bit 0 : APP functional interrupt 
             - bit 1 : APP Memory interrupt 
             - bit 2 : Two-step timestamp interrupt  - bit 3 : AN/LT 
            interrupt 
             - bit 4 : PCS memory interrupt 
              - bit 5 : Serdes interrupt 
              - bit 6 : Microcontroller interrupt. 
             - bit 7 : Rx MAC interrupt. 
             - bit 8 : Tx MAC interrupt. 
             - bit 9 : iCRC error interrupt.
             - bit 10 : PTP error interrupt.
             - bit 11 : Link interrupt. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_global_intr_stat_intr_hi_stat_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report which interrupt register is asserted: 
             - bit 0 : APP functional interrupt 
             - bit 1 : APP Memory interrupt 
             - bit 2 : Two-step timestamp interrupt 
             - bit 3 : AN/LT interrupt 
             - bit 4 : PCS memory interrupt 
              - bit 5 : Serdes interrupt 
              - bit 6 : Microcontroller interrupt. 
             - bit 7 : Rx MAC interrupt. 
             - bit 8 : Tx MAC interrupt. 
             - bit 9 : iCRC error interrupt.
             - bit 10 : PTP error interrupt.
             - bit 11 : Link interrupt 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_global_intr_stat_intr_lo_stat_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the number of 64B data payload to be accumulated on a 
            start of packet before forwarding the packet to the Tx MAC.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_min_thr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_min_thr_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_min_thr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_min_thr_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_min_thr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_min_thr_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_min_thr_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the amount of credit to send to Egress Buffer when 
            channel get enabled (one credit is equivalent to 16B data 
            payload)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_cred_ini_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_cred_ini_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_cred_ini_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_cred_ini_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_cred_ini_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_cred_ini_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_cred_ini_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the value of Rx PFC XOFF when provided by the 
            register space (one bit per PFC)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_val_rx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, loopback the Tx data (from Egress Buffer) back to 
            Rx (to Ingress Parser Buffer)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_txrx_lpbk_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, replace the Rx PFC XOFF received and extracted by 
            the MAC by a fix value provided by the register space

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_ovr_rx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             This register need to be set when the MAC workis configured 
            in Pause XOFF mode (not PFC mode). In that mode, the MAC 
            reports RX_XOFF through PFC_XOFF[0] signal and that is 
            translated through asserting all 8 RX_PFCXOFF signals to the 
            Core.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_rx_xoff_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the channel configuration as follow: 
             - 00b: 400G Mode (channel 0 only) 
             - 01b: 200G Mode (channel 0/2 only) 
             - 10b: 100G Mode (any channel) 
             - 11b: 50G  Mode (any channel) 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_chnl_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, put the Tx fifo in flush mode, discarding any new 
            packet coming from Egress Buffer

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_tx_flush_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_tx_flush_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_tx_flush_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_tx_flush_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_tx_flush_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_tx_flush_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_tx_flush_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enable Tx fifo for traffic

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ctrl_chnl_ena_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the number of packet in Tx fifo current (saturated to 
            63)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_txff_eopcnt_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_txff_eopcnt_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_txff_eopcnt_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_txff_eopcnt_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_txff_eopcnt_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_txff_eopcnt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_status_txff_eopcnt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Tx fifo current count in 16B increment

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_txff_count_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_txff_count_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_txff_count_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_txff_count_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_txff_count_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_txff_count_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_status_txff_count_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx PFC XOFF reported by the MAC layer

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_status_pfc_rxxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the size in byte for truncating packet. Truncated 
            packet will have valid CRC32 independantly of the CRC 
            received from Deparser.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_size_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enable packet truncation for that channel

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_trunc_ena_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, disable the internal CRC removal

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcrmv_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, disable the internal CRC checker and do not remove 
            the 4B CRC before sending to the Tx MAC layer (MAC should be 
            dynamically controlled to not generate Tx CRC.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcerr_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, globally disable the internal CRC checker from the 
            MAC layer

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txcrc_trunc_ctrl_crcchk_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enable each Tx PFC XOFF (one bit per PFC) to be sent to MAC 
            layer

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the value of Tx PFC XOFF when provided by the 
            register space (one bit per PFC)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_val_tx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             In Tx Pause frame mode, this register defines if XOFF is sent 
            to MAC layer if any of the Tx PFC XOFF bit  or Tx XOFF 
            received from the core will be translated as a Pause Frame.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             This register need to be set when the MAC is configured in 
            Pause Frame mode. In that mode, any of the 8 Tx PFC XOFF (if 
            enable) or Tx XOFF received from the core will be translated 
            as a Pause Frame.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_tx_xoff_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, replace the Tx PFC XOFF received from Core side and 
            sent to the MAC by a fix value provided by the register space

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_ovr_tx_pfcxoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, assert all PFC XOFF bits when Tx XOFF is received 
            from the core of the chip for that channel

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_en_tx_xoff_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, disable the internal CRC regeneration from the MAC 
            layer only for frames received with error

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_crcerr_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, globally disable the internal CRC regeneration from 
            the MAC layer

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_ctrl_crcgen_dis_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Capture the Rx status of last error packet received

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxpkt_err_sts_last_rxsts_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             In I2c Mode, the first byte sent over I2C is bit [7:0], 
            followed by [15:8],... 
            In MDIO Mode, it contains mdio_addrdata0 register programming 
            for super cycle instruction. 
            In StateOut Mode,this register can be used to write an extra 
            4B. 
            The number of bytes depends on Ethernet Ring Control 
            settings.  

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_addr_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_addr_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_addr_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_addr_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_addr_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_addr_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_addr_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Data written during a direct write access. 
            The first byte sent over I2C is bit [7:0]. The number of byte 
            written depends on ethernet ring control register settings.
             In MDIO mode, it contains mdio_addrdata1 for super cycle 
            instruction or the register to be programmed for single write 
            instruction.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_wdata_wdata_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_wdata_wdata_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_wdata_wdata_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_wdata_wdata_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_wdata_wdata_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_wdata_wdata_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_wdata_wdata_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the 4 byte data read on I2C/MDIO interface during a 
            register read. first byte read is on bit [7:0],...

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_rdata_rdata_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_rdata_rdata_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_rdata_rdata_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_rdata_rdata_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_rdata_rdata_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_rdata_rdata_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_rdata_rdata_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Register get set when lock error status is received. Lock 
            error bit need to be cleared by hardware.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_err_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the status of the on-going transaction: 
             - 000b: No update 
             - 001b: Request sent to GPIO block. 
             - 010b: Request granted by GPIO block. 
             - 011b: Response received from GPIO block with no error. 
             - 101b: Response received from GPIO block with error.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_status_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_status_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_status_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_status_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_status_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_status_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_status_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines which GPIO group/pair is targeted by the transaction. 
            Valid value are 0-7. It is assumed that the GPIO pair 
            targeted is programmed in expected mode for I2C and MDIO 
            settings. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_gpio_id_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Transaction holds the ressource, until unlock transaction or 
            timeout occurs, preventing any other ressource to access it. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_lock_req_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             For I2C mode, maximum number of byte is 4For MDIO mode, bit 
            [1:0] contains mdio_a[1:0] and bit [3:2] contain mdio_al[1:0] 
            of teh super cycle 
            In StateOut mode, maximum 8B can be transferred 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_datanum_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             In I2C mode, bit [6:0] defines the device address of the I2C 
            device to target. 
            On StateOut mode, bit [7:0] defines the start address of the 
            StateOut transfer. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_devaddr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             In I2C mode, defines the number of address bytes (0-4) to 
            transfer after the device address byte. 
            In MDIO mode super cycle instruction, contains the mdio_td 
            (bit[1]) and mdio_we (bit [0]) field programming 
            In MDIO mode single register access, bit [1:0] defines the 
            address of the single read/write to register and bit [2] 
            contains the clause (0b for CL45 and 0x1 for CL22)  
            In StateOut mode, this field is reserved

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_addrnum_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             In I2C mode, defines which command:
             - 00b : write address and write data command 
             - 01b : write address and read data 
             - 10b : write device address only
             - 11b : read data only 
            In MDIO mode, defines which command 
             - 00b : single write to register 
             - 01b : single read to register 
             - 10b : MDIO super cycle of 1 transaction 
             - 11b : MDIO super cycle of 2 transactions 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_cmd_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the type of transaction to be performed: 
             - 00b : Idle 
             - 01b : I2C transaction 
             - 10b : MDIO transaction 
             - 11b : StateOut 
            This register get cleared back to 00b when transaction has 
            been issued to the ethernet GPIO block.Refer to the Status 
            register to see status of the current transaction

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_ctrl_req_type_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Select GPIO ring 0 or 1

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_setup_ring_sel_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines a timeslot associated for the port 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_setup_ring_id_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_setup_ring_id_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_setup_ring_id_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_setup_ring_id_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_eth_ring_setup_ring_id_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_eth_ring_setup_ring_id_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_eth_ring_setup_ring_id_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Override the state in result with the software-controlled 
            port_alive entry (1 bit per channel), going to the Look-Up 
            Table to create the final port_alive signal which will be 
            sent to PGR block.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_soft_port_alive_ovr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_soft_port_alive_ovr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_soft_port_alive_ovr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_soft_port_alive_ovr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_soft_port_alive_ovr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_soft_port_alive_ovr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_soft_port_alive_ovr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the software-controlled port_alive entry (1 bit per 
            channel), going to the Look-Up Table to create the final 
            port_alive signal which will be sent to PGR block.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_soft_port_alive_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_soft_port_alive_val_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_soft_port_alive_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_soft_port_alive_val_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_soft_port_alive_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_soft_port_alive_val_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_soft_port_alive_val_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txff_ecc_txfifo_sram0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_tv80mem0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_statmem_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxff_stat_ecc_rxappfifo_sram0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txfifo_sbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txfifo_mbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_txfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxappfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxappfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_statmem_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_statmem_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80mem_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80mem_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Inject an iCRC or CRC32C error on next packet transmitted 
            where iCRC/CRC32C is requested

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_crcerr_inj_icrc_inj_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Inject a crc error for each channel

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_crcerr_inj_inj_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_crcerr_inj_inj_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_crcerr_inj_inj_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_crcerr_inj_inj_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_crcerr_inj_inj_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_crcerr_inj_inj_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_crcerr_inj_inj_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, enable the debug fifo structure

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_enable_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, assert an interrupt when debug fifo get full. It is 
            only applicable if debug works in fifo mode and not in 
            circular buffer

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_full_int_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the debug buffer works in fifo mode which stalls 
            automatically when fifo is full.  When cleared, the fifo 
            works in circular buffer mode, overriding the oldest 
            instruction executed.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_fifomode_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the debug buffer will capture all instruction and 
            data read by the microcontroller. Each instruction will be 
            32-bit quanta and will logged as follow:  
             - bit[31:30] : Instruction Code (11b) 
             - bit[29]    : M1 cycle  
             - bit[28]    : Read (1b) / Write (0b)  
             - bit[23:16] : Data  
             - bit[13:0]  : Address  
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_inst_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the debug buffer will capture all register 
            transactions generated by TV80. Each instruction will be 
            32-bit quanta and will logged as follow:  : 
             -first  address bit[31:30] : Instruction code (00b for 
            register access) 
             -first  address bit[29]    : Error during transaction 
            acknowledge phase 
             -first  address bit[28]    : Read (1b) / Write (0b) 
             -first  address bit[27:0]  : Word address of the transaction 
             -second address bit[31:0]  : Write data or read data 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_log_reg_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the limit address of debug fifo structure

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_limit_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the base address of debug fifo structure

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_ctrl_base_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the current read address of the debug fifo

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_head_ptr_head_ptr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the current write address of debug fifo structure

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_debug_tail_ptr_tail_ptr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the TV80 will be stalled when a read error 
            transaction generated by TV80.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_rerr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the TV80 will be stalled when a memory multi-bit 
            error is detected on its own SRAM containing data/firmware. 
            SW will have to clear this bit to enable TV80 to resume its 
            operation.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_on_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the  microcontroller code execution is halted 
            through the logic does not responding to TV80 trying to read 
            the next instruction from its firmware located in SRAM. SW 
            will have to clear this bit to enable TV80 to resume its 
            operation.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_stall_on_error_stall_absolute_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Status register reporting that the TV80 microcontroller has 
            been halted.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_halted_status_tv80_halted_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables the watchdog timeout mechanism to trigger NMI.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_enable_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the maximum of the timeout counter.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_watchdog_ctrl_timeout_value_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the maximum of the timeout counter.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_tv80_watchdog_count_cur_time_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             when set, enables to compute the running polarity between MAC 
            and IPB to save power

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_rxpol_ctrl_pol_inv_en_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Each 3-bit {ICRC,ETS,CTS} for each input code {ETS,CTS} : 
             - {ETS,CTS}==2'b00 : {ICRC,ETS,CTS} = val[2:0] 
             - {ETS,CTS}==2'b01 : {ICRC,ETS,CTS} = val[5:3] 
             - {ETS,CTS}==2'b10 : {ICRC,ETS,CTS} = val[8:6] 
             - {ETS,CTS}==2'b11 : {ICRC,ETS,CTS} = val[11:9] 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_lut_decode_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_lut_decode_val_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_lut_decode_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_lut_decode_val_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_lut_decode_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_lut_decode_val_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_lut_decode_val_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the nibble base mask for masking data for nibble 0-31

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile0_mask_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile0_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile0_mask_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile0_mask_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile0_mask_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile0_mask_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile0_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the nibble base mask for masking data for nibble 
            32-63

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile1_mask_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile1_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile1_mask_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile1_mask_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile1_mask_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile1_mask_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile1_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the nibble base mask for masking data for nibble 
            64-95

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile2_mask_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile2_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile2_mask_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile2_mask_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile2_mask_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile2_mask_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile2_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the nibble base mask for masking data for nibble 
            96-127

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile3_mask_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile3_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile3_mask_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile3_mask_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile3_mask_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile3_mask_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile3_mask_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Set the reset value of a new ICRC computation.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile4_reset_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile4_reset_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile4_reset_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile4_reset_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile4_reset_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile4_reset_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile4_reset_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, replace an existing iCRC (total packet size 
            decreased by 4B), When clear, add the iCRC (same packet size 
            as coming from EBUF).

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_replace_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_replace_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_replace_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_replace_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_replace_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_replace_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile5_replace_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, CRC added is CRC32-C, When clear, CRC added is 
            CRC32 (iCRC).

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_type_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_type_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_type_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_type_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_type_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_type_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile5_type_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables the profile to be used for iCRC computation

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_enable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_enable_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_enable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_enable_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_profile5_enable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_profile5_enable_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_profile5_enable_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             counter 32 bits wide.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_pkt_good_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             counter 32 bits wide.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_app_rspec_icrc_pkt_bad_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}
