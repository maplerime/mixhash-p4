/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <bf_types/bf_types.h>
#include <tof3_regs/tof3_reg_drv.h>
#include "port_mgr_tof3.h"

#include "tof3_eth400g_sys_rspec_access.h"
extern bf_status_t bf_map_logical_tmac_to_physical(bf_dev_id_t dev_id,
                                                   bf_subdev_id_t *subdev_id,
                                                   uint32_t logical_tmac,
                                                   uint32_t *physical_tmac);

/**********************************************************************
             When set to 1, the RxMAC removes the FCS field from the 
            packet.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the number of bytes in Tx Preamble sent (8 for 
            Ethernet, 0,1,2,4 for Fabric)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Tx playout margin for that channel (increment by 
            step of 4)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Place the Tx MAC logic in soft reset

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Place the Rx MAC logic in soft reset

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, check that control packet are 64B exactly

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the (average) number of IFG to insert between packets

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, stomp all FCS inserted on the packet.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, FCS is already part of the packet and MAC 
            swaps with new FCS.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, PTP 1 step zero the checksum instead of 
            updating checksum with egress time adjustment

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the Tx check the LT field for length type and 
            compare it to packet length.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the Tx MAC enters flush mode. Pause is 
            disregarded, RS layer goes in Tx Fault Idle state. Tx fifo is 
            flushed. Stats are still updated.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the IFG DIC mode logic compensates for AM 
            bandwidth. Should be set to 1 for spec compliance.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the Inter Frame Gap insertion function is 
            operating in DIC mode.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables transmission of packets to the MAC Tx fifo. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the maximum packet size which can be transmitted 
            before reporting an error.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the minimum packet size on transmit. Any packet 
            smaller than that received on EBUF interface will be padded 
            by the MAC. The value defined include FCS.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the Tx fifo almost full threshold to prevent the Tx 
            fifo from overflowing.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             defines the Tx fifo almost empty threshold which applies on a 
            start of packet to forward to MAC/PCS layer.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Disregard the full flag in Rx app fifo. Need to be set low. 
            Potentially use for backdoor extra room of 1-2 entries.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, check that control packet are 64B exactly

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the Rx check the LT field for length type and 
            compare it to packet length.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enable reception of packet by the MAC

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the maximum packet size which can be received before 
            reporting an error.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Define the minimum packet size which can be received before 
            reporting an error.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             defines the Rx MAC destination to decodefor identification of 
            unicast Pause frame

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables the decoding of PFC frames and defines which PFC is 
            enabled for generating Rx PFC status.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables the decoding of Pause frame and generation of Rx Port 
            Pause status

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Tx Control Path fifo current depth 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_cp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_txfifo_depth_cp_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_cp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_txfifo_depth_cp_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_cp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_txfifo_depth_cp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_txfifo_depth_cp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Tx Data Path fifo current depth 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_dp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_txfifo_depth_dp_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_dp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_txfifo_depth_dp_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_dp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_txfifo_depth_dp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_txfifo_depth_dp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             enables the Rx Runt packet filtering

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             enables the Rx PFC packet filtering

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             enables the SOP-EOP rule functionality which terminates 
            packet automatically after long period of idle time.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             enables the Rx truncation functionality

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, checks for destination address against the 
            static_mac_addr

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             .

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the length of fabric header for decoding fabric 
            Pause/PFC frames: 
             - 00b: Disabled (Ethernet mode) 
             - 01b: 12B fabric header 
             - 10b: 16B fabric header 
             - 11b: 20B fabric header 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the number of quanta (16 clocks) for an opened packet 
            without valid dataphase to automatically close the packet. 
            Value 0-7 should be avoided for proper operation.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Controls when a packet gets truncated on Rx path and closed 
            with EOP with Error. The value programmed include the FCS 
            field (if RxFCS is not removed) and should be programmed as 
            the last packet size which is accepted.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             enables the Rx truncation functionality

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Controls when a packet gets truncated on Tx path and closed 
            with EOP with Error. The value programmed does not include 
            the FCS field (as Tx FCS is removed at that level) and should 
            be programmed as the last packet size which is accepted.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Write data register bit [31:0]

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Write data register bit [63:32]

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the indirect request is sent to the statistic 
            memory. This field gets automatically cleared by HW when the 
            request completes. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines whether the indirect transaction is a write request 
            or a read request 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the statistic memory address which need to be 
            accessed. Each statistics counter is 64-bit and the address 
            should represent (96*channel + counter_offset) (96 counters 
            per channel). 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Read data register bit [31:0]

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Read data register bit [63:32]

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report a dual ECC error happening during the read operation

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rerr_data_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rerr_data_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rerr_data_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rerr_data_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rerr_data_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_statmem_csr_rerr_data_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_statmem_csr_rerr_data_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             One bit per channel. Automatically cleared by hardware when 
            clear operation completes. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_stat_clear_req_chan_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_stat_clear_req_chan_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_stat_clear_req_chan_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_stat_clear_req_chan_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_stat_clear_req_chan_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_stat_clear_req_chan_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_stat_clear_req_chan_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             globally configures Rx-to-Tx loopoback for all channels 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count3_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count3_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count3_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count3_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count3_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count3_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count3_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count2_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count2_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count2_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count2_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count2_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count2_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count2_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count1_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count1_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count1_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count1_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count1_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count1_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count1_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             report the number of entry in fifo (max 4)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count0_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count0_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count0_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count0_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count0_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_stat_count0_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_stat_count0_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When the fifo is empty return 0x0, otherwise return: 
             - [47:0] : Capture timestamp 
             - [56:48]: Timestamp ID (9-bit self incrementing allowing to 
            number of lost ID) 
             - [62:57]: Reserved 
             - [63]   : Valid entry when set to 1 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_out_cts_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_out_cts_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_out_cts_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_sys_rspec_cts_fifo_out_cts_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}
