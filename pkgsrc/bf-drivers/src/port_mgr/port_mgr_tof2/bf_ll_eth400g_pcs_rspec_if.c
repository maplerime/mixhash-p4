/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include <bf_types/bf_types.h>
#include "eth400g_pcs_rspec_access.h"
extern bf_status_t bf_map_logical_umac4_to_physical(bf_dev_id_t dev_id,
                                                    uint32_t logical_umac,
                                                    uint32_t *physical_umac);
/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Reset Comira PCS "
                   " -[1]   : Reset Serdes IO Tile "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_soft_reset_ethpcs_swrst_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select observation clock source : "
                   " -[2:0]  : Select Serdes lane number "
                   " -[4:3]  : Serdes Mux: rx_clk (00b), tx_clk (01b), MAC clock (10b), Core clock (11b) "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select divider of observation clock source : "
                   " - 00b : divide-by-2 "
                   " - 01b : divide-by-4 "
                   " - 10b : divide-by-8 "
                   " - 11b : divide-by-16 "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enable divider for observation clock 0"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select obervation clock source "
                   " -[2:0]  : Select Serdes lane number "
                   " -[4:3]  : Serdes Mux: rx_clk (00b), tx_clk (01b), MAC clock (10b), Core clock (11b) "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkobs1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select divider of observation clock source : "
                   " - 00b : divide-by-2 "
                   " - 01b : divide-by-4 "
                   " - 10b : divide-by-8 "
                   " - 11b : divide-by-16 "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkdiv1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enable divider for observation clock 1"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_clkobs_ctrl_sel_clkena1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the number of consecutive clock cycle that the Rx_idle_det need to"
                   "be asserted high or low to report rxSigOK to the MAC layer. Set to zero to prevent any debouncing "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_ctrl_sigok_debounce_count_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Ignore Signal OK from Serdes and forces RxSigOK high (per lane) "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_high_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Ignore Signal OK from Serdes and forces RxSigOK low (per lane)"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_ctrl_force_rxsigok_low_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enables access to MDIOCI interface"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_en_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generate a reset command to MDIOCI interface"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_reset_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the clock speed of the MDIOCI interface clock"
                   " 0001b : Core clock divider by 2 "
                   " 0011b : Core clock divider by 4 "
                   " 0111b : Core clock divider by 8 "
                   " 1111b : Core clock divider by 16 "
                   "The MDC clock output is generated when the 4-bit counter reaches it max value."
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_mdioci_ctrl_mdioci_clkdiv_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the address of the global Rx signal OK register of the tile"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_mdioci_poll_ctrl_poll_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the time interval between consecutive polling of RxSignalOK register of the tile"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_time_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 enables polling of RxSignalOK"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_mdioci_poll_time_poll_ena_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which bit from the polling register is the rxsigok signal for a specific logical lane"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_rxsigok_bitsel_rxsigok_sel7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report which interrupt register is asserted"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mdioci_intr_stat_intr_stat_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 4 bit per lane: "
                   " - bit [2:0]: physical Tx lane mapping "
                   " - bit [3] : PAM4 mode (enable divide-by-2 clock and double internal datapath) "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_txsds_mode_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_txsds_mode_mode_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_txsds_mode_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_txsds_mode_mode_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_txsds_mode_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_txsds_mode_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_txsds_mode_mode_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 4 bit per lane: "
                   " - bit [2:0]: physical Tx lane mapping "
                   " - bit [3] : PAM4 mode (enable divide-by-2 clock and double internal datapath) "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rxsds_mode_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rxsds_mode_mode_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rxsds_mode_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rxsds_mode_mode_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rxsds_mode_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rxsds_mode_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rxsds_mode_mode_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 . When set to 1, defines that Serdes is working in 112G mode. There is 1 bit per group of 2 logical lane: "
                   " - bit [0]: When set, configure logical lane 0/1 in 112G mode"
                   " - bit [1]: When set, configure logical lane 2/3 in 112G mode"
                   " - bit [2]: When set, configure logical lane 4/5 in 112G mode"
                   " - bit [3]: When set, configure logical lane 6/7 in 112G mode"
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_sds_112g_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_sds_112g_mode_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_sds_112g_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_sds_112g_mode_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_sds_112g_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_sds_112g_mode_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_sds_112g_mode_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select Rx clock source for PPM comparator."
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_ppm_sel_sel_rxclk_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select Tx clock source for PPM comparator."
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_ppm_sel_sel_txclk_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the number of clock cycles for the PPM comparator to run before reporting valid PPM differences. "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_ctrl_max_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_ctrl_max_count_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_ctrl_max_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_ctrl_max_count_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_ctrl_max_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_ctrl_max_count_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_ppm_ctrl_max_count_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When set to 1, the PPM comparator logic is enabled. It is required to have valid clock source before enabling the PPM comparator. The PPM comparator runs continuously as long as enabled."
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_ppm_ctrl_ppm_ena_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Represent a signed number of PPM seen between Tx and Rx clock. The logic add 1 clock uncertainty and thus to get an accuracy of 1 PPM, "
                   " it is recommended to run the PPM comparator for at least 10 M cycles. New value get reported automatically after each iteration as long as enabled. "
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_ppm_stat_ppm_seen_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When a result is available after been enabled, report 1 when the PPM comparator has a valid value to report."
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_stat_ppm_val_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_stat_ppm_val_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_stat_ppm_val_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_stat_ppm_val_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_eth_ppm_stat_ppm_val_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_eth_ppm_stat_ppm_val_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_eth_ppm_stat_ppm_val_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_stat_deskew_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_stat_deskew_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_stat_rsfec_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_stat_rsfec_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en0_deskew_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en0_deskew_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en0_rsfec_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en0_rsfec_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en1_deskew_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en1_deskew_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en1_rsfec_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_en1_rsfec_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_inj_deskew_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Deskew memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_inj_deskew_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_inj_rsfec_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when RS-FEC memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mem_intr_inj_rsfec_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Disable Error Checking
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem0_disable_check_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem0_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem0_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem1_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem1_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem2_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem2_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem3_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem3_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem4_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem4_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem5_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem5_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem6_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem6_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem7_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_ecc_mem7_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram0_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram1_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram2_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem2_sram3_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram0_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram1_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram2_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec23_ecc_mem3_sram3_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_deskew_mbe_err_log_laneid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_mbe_err_log_laneid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_mbe_err_log_laneid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_mbe_err_log_laneid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_deskew_mbe_err_log_laneid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_deskew_mbe_err_log_laneid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_deskew_mbe_err_log_laneid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec_sbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_memid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_memid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_memid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_memid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_sbe_err_log_memid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_sbe_err_log_memid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec_sbe_err_log_memid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec_mbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Deskew Memory multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_memid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_memid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_memid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_memid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_rsfec_mbe_err_log_memid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_rsfec_mbe_err_log_memid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_rsfec_mbe_err_log_memid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_en0_intr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_en0_intr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_en0_intr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_en0_intr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_en0_intr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_en0_intr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mdioci_en0_intr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_en1_intr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_en1_intr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_en1_intr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_en1_intr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_pcs_rspec_mdioci_en1_intr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_pcs_rspec_mdioci_en1_intr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_pcs_rspec_mdioci_en1_intr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

