/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <bf_types/bf_types.h>
#include <tof3_regs/tof3_reg_drv.h>
#include "port_mgr_tof3.h"

#include "tof3_eth400g_mac_rspec_access.h"
extern bf_status_t bf_map_logical_tmac_to_physical(bf_dev_id_t dev_id,
                                                   bf_subdev_id_t *subdev_id,
                                                   uint32_t logical_tmac,
                                                   uint32_t *physical_tmac);

/**********************************************************************
             Per-lane, when set, configure the Rx in internal loopback. 
            This signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pma_rx_lpbk_en_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Per-lane, when set, continuously write zero data in PMA Tx 
            fifo. It can be used to disable an interface when Rx PMA 
            loopback is enabled for instance. This signal exists in the 
            physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_disable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_disable_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_disable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_disable_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_disable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_disable_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pma_tx_disable_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Per-lane PMA Rx ready which enables data to be received. A 
            low to high transition reset the PMA Rx fifo. This signal 
            exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pma_rx_rdy_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Per-lane PMA Rx enable which signals that data is valid. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_en_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_en_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_rx_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_rx_en_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pma_rx_en_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When writing 1 to any channel bit, a Pause frame is send by 
            the MAC for that channel. This register is self-clear.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pause_tx_strb_chan_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Use the CCC coming from register

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_overload_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Select between 2 parameters. Set to 0

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_ccc_ctrl_ccc_sel_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the MAC clock cycle compensation for PTP as 16-bit 
            nanosecond [31:16] and 16-bit fractional nanosecond [15:0]

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_ccc_scale_fns_val_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index7_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index6_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index5_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index4_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index3_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index2_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index1_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Tx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_txserdesmux_tx_map_index0_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index7_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index6_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index5_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index4_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index3_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index2_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index1_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the logical to physical mapping of Rx lanes. This 
            signal exists in the physical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rxserdesmux_rx_map_index0_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the PCS (CL82) Alignment Marker period expressed in 
            number of CW. IEEE and Consortium RSFEC spec requires 0x5000 
            whereas FCFEC/NoFEC Consortium spec requires 0x4000. Better 
            use the c82_l4_spacing register which is per channel to 
            support both 50G consortium and 50G IEEE.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_value_c82_l4_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, uses the AM period specified in register instead of 
            hardcoded value. On silicon, recommend to clear this bit to 
            allow multiple version of 50G to coexist.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_rx_am_period_en_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, uses the AM period specified in register instead of 
            hardcoded value. On silicon, recommend to clear this bit to 
            allow multiple version of 50G to coexist.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl0_tx_am_period_en_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Alignment Marker period expressed in number of CW 
            (spec is 1024) for 25G CL91 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c91_25g_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the PCS (CL82) Alignment Marker period expressed for 
            100G CL82/91 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl1_value_c82_l20_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Alignment Marker period expressed in number of CW 
            (spec is 4096) for 100G CL91 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_100g_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Alignment Marker period expressed in number of CW 
            (spec is 4096) for 50G CL91 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl2_value_c91_50g_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Alignment Marker period expressed in number of 2 
            CW (spec is 8192) for 400G CL119 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_400g_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Alignment Marker period expressed in number of 2 
            CW (spec is 4096) for 200G CL119 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl3_value_c119_200g_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Alignment Marker period expressed in number of 2 
            CW (spec is 4096) for 100G CL119 mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_am_period_ctrl4_value_c119_100g_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_amp_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_block_err_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_hi_ber_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_skew_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_align_map_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_align_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_test_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_err_block_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_pcs_ber_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_bip_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_lane_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_bit_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_symb_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_cor_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, read to the counter will clear it.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_counter_cor_fec_unc_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, isolate the Tx MAC from Tx PCS allowing to 
            soft reset the Tx MAC after uncorrectable error detected on 
            any Tx fifo without putting the link down (PCS will send 
            Idle)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_isolate_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_isolate_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_isolate_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_isolate_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_isolate_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_isolate_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_tx_isolate_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the number of bytes in Rx Preamble sent. Must be set 
            to 8 for IEEE spec compliance, other supported values are 0, 
            1, 2, 4 for fabric mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_rx_preamble_bcp1_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set to 1, the Tx PTP block is bypassed and no PTP 
            function can be used. can be hardwired.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_tx_ptp_bypass_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, enable error block reported by PCS to be part of 
            packet and being calculated in stat block. Based on IEEE 
            spec, it should be set to 1, but stable RTL used to truncated 
            packet on first error block seen.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_rx_dec_conform_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables to check for MFD packet delimiter. Check fro 0x55 in 
            middle of premable. Should be cleared.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_rx_mfd_expect_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables to check for SFD packet delimiter

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_rx_sfd_expect_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Insert the SFD On every packet transmitted.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_mac_tx_sfd_insert_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             defines the MAC source address used to generate pause/pfc 
            frames

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pause_tx_mac_addr_addr_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             define the time sent in the Tx pause frame or PFC frame for 
            all PFC asserted

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pause_tx_time_prt_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the beat time when a pause frame is automatically 
            regenerated.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pause_tx_time_beat_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines which PFC is enabled and PFC mode generation.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_pfc_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables the generated of port pause (XON/XOFF)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_port_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Control whether Tx Pause frame are automatically regenerated 
            when beat time value is reached

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pause_tx_ctrl_beat_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Set to 1 to enable Rx link interruption computation

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rs_ctrl_static_rs_rx_link_interruption_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Debug register to issue Tx idle

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_idle_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Debug register to issue Tx fault remote

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_remote_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Debug register to issue Tx fault local

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_local_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             enables to send corresponding fault code

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_rs_ctrl_tx_fault_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Set for PTP testing when doing loopback for reference point 
            the loopback point and not the line.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_cor_lpbk_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Use the cfg_ptp_ui_scal_fns register when set, potentially 
            can be used when serdes is overclocked.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_ui_overload_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             TBD

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_tx_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Program to Zero to not overload. PTP at start of FEC frame 
            for CL91 and 119. refer to table

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_mode_ctrl_rx_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the Unit Interval for PTP as 16-bit nanosecond 
            [31:16] and 16-bit fractional nanosecond [15:0]. Need to be 
            based on protocol speed.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_ui_scale_fns_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the PTP Tx delay adjustment for the Serdes Tx latency 
            and extra fix offset as 16-bit nanosecond [31:16] and 16-bit 
            fractional nanosecond [15:0]. 
            Software need to program this register based on port speed, 
            redundancy activation or mbus_id==0 (ETHCPU).

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_tx_dly_adj_fns_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the PTP Rx delay adjustment for the Serdes Rx latency 
            and extra fix offset as 16-bit nanosecond [31:16] and 16-bit 
            fractional nanosecond [15:0].
            Software need to program this register based on port speed, 
            redundancy activation or mbus_id==0 (ETHCPU).

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_ptp_rx_dly_adj_fns_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enables 200G/400G to compute degraded symbol error rate.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_cl119_rx_fec_degraded_ser_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enable Rx block lock alignment when Clause 91 is used

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_cl91_rx_block_lock_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Rx restart lock process on a bad  alignment marker period.  
            Need to be set for CL91 and 119 (ignore for other). Can be 
            turn off for debug.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_amp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Rx restart lock process on a bad FEC block detected. Need to 
            be set for CL91 and 119 (ignore for other). Can be turn off 
            for debug

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_restart_lock_on_bad_fec_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Configure PCS in Hi-SER mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Disable reporting of Rx Hi-SER

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Rx Hi-SER Force Enable

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiser_force_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Disable reporting of Rx Hi-BER

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_disable_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Force reporting of Rx Hi-BER

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_hiber_force_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, configure the channel in PCS loopback mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_lpbk_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, select Clause 49 test pattern type expected

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_type_sel_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, select Clause 49 test pattern data expected

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_data_sel_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, expect Clause 49 test pattern

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c49_test_pattern_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, expect Clause 82 test pattern

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_c82_test_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, bypass Rx descrambler

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_rx_scrm_bypass_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Select Clause 49 test pattern type 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_data_sel_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, send Clause 49 test pattern

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c49_test_pattern_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, send Clause 82 test pattern

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_c82_test_mode_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, bypass Tx Scrambler function

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs_base_tx_scrm_bypass_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Configure the port type based on Clause 49/82/91/119 selected 
            through correpsonding fields: 
             CL119 port mode : 400G-KP8 (0x0011), 400G-KP4 (0x0012), 
            200G-KP4 (0x2011), 200G-KP2 (0x2012), 100G-KP1 (0x3012) 
                - [15:12] : Speed (0x0 for 400G, 0x2 for 200G, 0x3 for 
            100G) 
                - [9]     : Error indication bypass (0b for Spec 
            compliance) 
                - [8]     : Error correction bypass (0b for Spec 
            compliance) 
                - [5:4]   : FEC type (0x0 for KR, 0x1 for KP, 0x2 for KL) 
                - [1:0]   : Bit Mux (0x0 for none, 0x1 for 2:1, 0x2 for 
            4:1)
             CL91 port mode : 100G-KR4 (0x0000), 100G-KP2 (0x0011), 
            100G-KP1 (0x0012), 50G-KR2 (0x2000), 50G-KP1 (0x2011), 
            25G-KR1 (0x3000) 
                - [15:12] : Speed (0x0 for 100G, 0x2 for 50G, 0x3 for 
            25G) 
                - [9]     : Error indication bypass (0b for Spec 
            compliance) 
                - [8]     : Error correction bypass (0b for Spec 
            compliance) 
                - [5:4]   : FEC type (0x0 for KR, 0x1 for KP, 0x2 for KL) 
                - [1:0]   : Bit Mux (0x0 for none, 0x1 for 2:1, 0x2 for 
            4:1)
             CL82 port mode : 100G-R4 (0x0000), 50G-R2 (0x2000), 40G-R4 
            (0x3000), 40G-FC (0x3010), 50G-FC (0x2010) 
                - [15:12] : Speed (0x0 for 100G, 0x2 for 50G, 0x3 for 
            40G) 
                - [8]     : bypass scrambler (0b for Spec compliance) 
                - [4]     : FEC mode (0x0 for no-FEC, 0x1 for FC-FEC) 
             CL49 port mode : 25G-R1 (0x1000), 10G-R1 (0x0000), 25G-FC 
            (0x1010), 10G-FC (0x0010) 
                - [15:12] : Speed (0x0 for 10G, 0x1 for 25G) 
                - [8]     : bypass scrambler (0b for Spec compliance) 
                - [4]     : FEC mode (0x0 for no-FEC, 0x1 for FC-FEC) 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_port_type_16b_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Must be set for CL119 IEEE compliance

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c119_100g_final_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 1G mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_port_c36_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set to 0 for IEEE compliance.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_100g_final_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set to 0 for IEEE compliance.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_50g_pad_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set to 0 for IEEE compliance

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c91_25g_pad_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When AM period override is not enable, this bit enables to 
            distinguish between CL82 PCS AM period of 0x5000 (when set) 
            and 0x4000 when cleared. 50G mode using RSFEC need to set 
            this bit to 1, whereas 40G/50G with FCFEC or noFEC need to 
            set this bit to 0.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_am_mode_c82_l4_spacing_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 200G/400G mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_port_c119_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 25G/50G/100G RS-FEC mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_port_c91_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 40G/50G/100G no RS-FEC mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_port_c82_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 10G/25G no RS-FEC mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_tx_port_c49_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Configure the port type based on Clause 49/82/91/119 selected 
            through correpsonding fields: 
             CL119 port mode : 400G-KP8 (0x0011), 400G-KP4 (0x0012), 
            200G-KP4 (0x2011), 200G-KP2 (0x2012), 100G-KP1 (0x3012) 
                - [15:12] : Speed (0x0 for 400G, 0x2 for 200G, 0x3 for 
            100G) 
                - [9]     : Error indication bypass (0b for Spec 
            compliance) 
                - [8]     : Error correction bypass (0b for Spec 
            compliance) 
                - [5:4]   : FEC type (0x0 for KR, 0x1 for KP, 0x2 for KL) 
                - [1:0]   : Bit Mux (0x0 for none, 0x1 for 2:1, 0x2 for 
            4:1)
             CL91 port mode : 100G-KR4 (0x0000), 100G-KP2 (0x0011), 
            100G-KP1 (0x0012), 50G-KR2 (0x2000), 50G-KP1 (0x2011), 
            25G-KR1 (0x3000) 
                - [15:12] : Speed (0x0 for 100G, 0x2 for 50G, 0x3 for 
            25G) 
                - [9]     : Error indication bypass (0b for Spec 
            compliance) 
                - [8]     : Error correction bypass (0b for Spec 
            compliance) 
                - [5:4]   : FEC type (0x0 for KR, 0x1 for KP, 0x2 for KL) 
                - [1:0]   : Bit Mux (0x0 for none, 0x1 for 2:1, 0x2 for 
            4:1)
             CL82 port mode : 100G-R4 (0x0000), 50G-R2 (0x2000), 40G-R4 
            (0x3000), 40G-FC (0x3010), 50G-FC (0x2010) 
                - [15:12] : Speed (0x0 for 100G, 0x2 for 50G, 0x3 for 
            40G) 
                - [8]     : bypass scrambler (0b for Spec compliance) 
                - [4]     : FEC mode (0x0 for no-FEC, 0x1 for FC-FEC) 
             CL49 port mode : 25G-R1 (0x1000), 10G-R1 (0x0000), 25G-FC 
            (0x1010), 10G-FC (0x0010) 
                - [15:12] : Speed (0x0 for 10G, 0x1 for 25G) 
                - [8]     : bypass scrambler (0b for Spec compliance) 
                - [4]     : FEC mode (0x0 for no-FEC, 0x1 for FC-FEC) 
            

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_port_type_16b_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
              Must be set for IEEE compliance CL119-100G using new 
            alignment marker.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c119_100g_final_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 1G mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_port_c36_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When AM period override is not enable, this bit enables to 
            distinguish between CL82 PCS AM period of 0x5000 (when set) 
            and 0x4000 (when cleared). 50G mode using RSFEC need to set 
            this bit to 1, whereas 40G/50G with FCFEC or noFEC need to 
            set this bit to 0.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_am_mode_c82_l4_spacing_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for CL119 100G/200G/400G mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_port_c119_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for CL91 25G/50G/100G RS-FEC mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_port_c91_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 40G/50G/100G no RS-FEC mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_port_c82_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Need to be set for 10G/25G no RS-FEC mode

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_static_pcs_rx_port_c49_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the symbol error rate activate threshold to start 
            reporting degraded status over the interval defined

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_activate_threshold_value_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the symbol error rate deactivate threshold to stop 
            reporting degraded status over the interval defined

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_deactivate_threshold_value_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the interval of time to accumulate FEC CW and compute 
            the FEC degraded status.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs119_rx_fec_degraded_ser_interval_value_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines the PMA Tx fifo almost full threshold. This signal 
            exists in the logical domain.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_level_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, control Tx fifo almost full through next field. 
            This signal exists in the logical domain. Can be HW to 1.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pma_tx_fifo_paf_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Enable to report to link partner the degraded local status 
            (which becomes a degraded remote status for the link 
            partner). Need to be set for normal operation.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs119_tx_fec_degraded_ser_local_en_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             TBD

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_off_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             TBD

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cfg_pcs119_tx_am_sf_force_on_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Signal OK per lane. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rxsigok_raw_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the maximum skew seen by the channel (only applicable 
            for multi-lane mode) in increment of 40 UI 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_max_skew_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_max_skew_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_max_skew_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_max_skew_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_max_skew_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_max_skew_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_max_skew_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Signal OK associated to the channel. In case of 
            multi-lane mode, it is a wire-and function of all signal OK. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rxsigok_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rxsigok_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rxsigok_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rxsigok_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rxsigok_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rxsigok_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rxsigok_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the MAC Tx is Ready to transmit packet 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_tx_rdy_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_tx_rdy_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_tx_rdy_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_tx_rdy_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_tx_rdy_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_tx_rdy_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_tx_rdy_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the MAC Rx is ready to receive packet 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rx_rdy_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rx_rdy_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rx_rdy_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rx_rdy_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rx_rdy_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rx_rdy_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rx_rdy_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that Tx is transmitting fault 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rs_tx_fault_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Link Interruption status of RS sub-layer 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rs_rx_link_interruption_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Fault Remote status of RS sub-layer 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_remote_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Fault Local Status of RS sub-layer 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_rs_rx_fault_local_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that local port has detected a degraded condition of 
            the serial. Only applicable for Clause 119 ports 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_degraded_local_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_degraded_local_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_degraded_local_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_degraded_local_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_degraded_local_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_degraded_local_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_degraded_local_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that local port has received a degraded condition from 
            remote link partner.  Only applicable for Clause 119 ports 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_degraded_remote_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_degraded_remote_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_degraded_remote_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_degraded_remote_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_degraded_remote_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_degraded_remote_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_degraded_remote_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that the PCS BaseR status that link is up.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_pcs_base_rx_status_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that the channel is currently reporting High Block 
            Error Rate (only available when FEC does not bypass error 
            indication)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_hi_ber_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_hi_ber_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_hi_ber_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_hi_ber_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_hi_ber_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_hi_ber_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_hi_ber_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that the channel is currently reporting High Symbol 
            Error Rate (only available when FEC bypasses error 
            indication)

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_hi_ser_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_hi_ser_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_hi_ser_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_hi_ser_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_hi_ser_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_hi_ser_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_hi_ser_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that all lanes are AM locked

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_am_lock_all_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_am_lock_all_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_am_lock_all_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_am_lock_all_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_am_lock_all_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_am_lock_all_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_am_lock_all_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that all lane are AM mapping is unique and correct.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_am_map_ok_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_am_map_ok_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_am_map_ok_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_am_map_ok_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_am_map_ok_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_am_map_ok_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_am_map_ok_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that PCS report Alignment Status.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_align_status_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_align_status_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_align_status_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_align_status_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_align_status_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_align_status_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_align_status_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report that all lanes are reporting Block Lock. Only 
            applicable for Clause 119 ports (channel 0 or 4) 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_block_lock_all_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_block_lock_all_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_block_lock_all_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_block_lock_all_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rx_status_block_lock_all_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rx_status_block_lock_all_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rx_status_block_lock_all_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Control Path fifo current depth 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rxfifo_depth_cp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rxfifo_depth_cp_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rxfifo_depth_cp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rxfifo_depth_cp_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rxfifo_depth_cp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rxfifo_depth_cp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rxfifo_depth_cp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the Rx Data Path fifo current depth 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rxfifo_depth_dp_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rxfifo_depth_dp_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rxfifo_depth_dp_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rxfifo_depth_dp_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_rxfifo_depth_dp_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_rxfifo_depth_dp_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_rxfifo_depth_dp_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the current block lock of corresponding VL of the 
            channel: 
             -10G CL49: only bit [0] is valid 
             -25G CL49: only bit [0] is valid 
             -25G CL91: only bit [0] is valid 
             -40G CL82: only bit [3:0] are valid, bit [7:4] for FCFEC 
            block lock if enabled 
             -50G CL82: only bit [3:0] are valid, bit [7:4] for FCFEC 
            block lock if enabled
             -50G CL91: only bit [1:0] are valid 
             -100G CL82: bit [19:0] are valid 
             -100G CL91: bit [3:0] are valid 
             -200G CL119: only bit [7:0] are valid 
             -400G CL119: only bit [15:0] are valid 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_rx_block_lock_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Report the current AM lock of corresponding VL of the 
            channel: 
             -10G CL49: only bit [0] is valid 
             -25G CL49: only bit [0] is valid 
             -25G CL91: only bit [0] is valid 
             -40G CL82: only bit [3:0] are valid 
             -50G CL82: only bit [3:0] are valid 
             -50G CL91: only bit [1:0] are valid 
             -100G CL82: bit [19:0] are valid 
             -100G CL91: bit [3:0] are valid 
             -200G CL119: only bit [7:0] are valid 
             -400G CL119: only bit [15:0] are valid 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_rx_amp_lock_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Get set on a per VL basis when Block error is detected 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_rx_block_err_latched_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Get set on a per VL basis when AM error is detected 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_rx_amp_err_latched_val_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 4 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_fec_unc_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 4 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_fec_cor_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 8 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_fec_symb_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 8 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_ber_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 8 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_err_block_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 8 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_test_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 5 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_block_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 5 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_amp_err_counter_ctr64_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 32 bits wide.
             Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_align_skew_err_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 32 bits wide.
             Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_align_fec_err_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 32 bits wide.
             Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_align_map_err_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 32 bits wide.
             Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_pcs_hi_ber_counter_ctr32_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 13 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_fec_lane_symb_err_counter_ctr64_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 64 bits wide.
             Has 13 bit increment value. Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_fec_lane_bit_err_counter_ctr64_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Saturating counter 16 bits wide.
             Counter is Clear-on-Read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_bip_err_counter_ctr16_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_bip_err_counter_ctr16_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_bip_err_counter_ctr16_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_bip_err_counter_ctr16_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_bip_err_counter_ctr16_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_bip_err_counter_ctr16_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_bip_err_counter_ctr16_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem7_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem6_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem5_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_0_mem0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem7_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem6_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem5_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_ecc_1_mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem1_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_ecc_mem0_sram0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem7_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem6_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem5_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem4_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem3_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem2_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem1_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_ecc_mem0_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_cor_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_res_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_res_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txothfifo_ecc_ctl_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_mbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_inject_sbe_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_distr_ecc_mem_sram_disable_check_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_cl119_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_instid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_memid_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_distr_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_distr_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txctlfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txctlfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txresfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txresfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txcorfifo_sbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 
**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_txcorfifo_mbe_err_log_addr_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Index 0 defines the first 32-symbols to corrupt, index 15 the 
            symbols 480 to 511 to corrupt. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_symb_val_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When a non zero value is programmed, the error injection 
            process starts on next FEC symbol or next AM symbol. If AM 
            FEC block type is selected, it will only corrupts AM FEC 
            block allowing to potentially AM lock FSM. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_repeat_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set the corruption is only applied to the AM FEC block. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_am_type_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             This field defines the channel on which the error injection 
            occurs. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_chan_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             This field defines the XOR function applied for each symbol 
            to be corrupted. 

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_tx_krpfec_err_inj_ctrl_xor_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the FEC error counter distribution are counting per 
            FEC block instead of per Serdes lane.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_sel_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the FEC error counter distribution are clear on 
            read.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_cor_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             When set, the FEC error counter distribution are enabled for 
            counting.

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_ctrl_en_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines which FEC lane goes to the fourth branch of the 
            adder. Value 0x10 means that this branch is not used

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add3_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines which FEC lane goes to the third branch of the adder. 
            Value 0x10 means that this branch is not used

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add2_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines which FEC lane goes to the second branch of the 
            adder. Value 0x10 means that this branch is not used

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add1_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             Defines which FEC lane goes to the first branch of the adder. 
            Value 0x10 means that this branch is not used

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_set(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_get(dev_id, subdev_id, tmac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_get(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_lut_fec_lane_add0_set(dev_id, subdev_id, tmac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
             For each physical lane, implements 16 counters counting the 
            number of symbol corrected in a block. A total of 256 32-bit 
            counters is implemented (implemented in RAM). Those counters 
            are saturating and can be configured to be clear on read, 
            frozen,...

**********************************************************************/

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_set(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_set(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_get(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_get(dev_id, subdev_id, tmac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_rmw(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_get(dev_id, subdev_id, tmac, data_p, &unused_fld, true);
  tof3_eth400g_mac_rspec_krpfec_err_distr_cnt_val_set(dev_id, subdev_id, tmac, data_p, val, true);
  return BF_SUCCESS;
}
