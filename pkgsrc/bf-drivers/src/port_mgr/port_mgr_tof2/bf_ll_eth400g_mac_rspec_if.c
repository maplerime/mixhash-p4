/* clang-format off */
#include <stdbool.h>
#include <stdint.h>
#include <bf_types/bf_types.h>
#include "eth400g_mac_rspec_access.h"
extern bf_status_t bf_map_logical_umac4_to_physical(bf_dev_id_t dev_id,
                                                    uint32_t logical_umac,
                                                    uint32_t *physical_umac);
/**********************************************************************
 Each bit triggers a different software reset or control: "
                   " -[0]   : Reset Comira MAC/PCS "
                   " -[1]   : Reset App logic "
                   " -[2]   : Reset microcontroller"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_soft_reset_eth_swrst_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_soft_reset_eth_swrst_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_soft_reset_eth_swrst_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_soft_reset_eth_swrst_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_soft_reset_eth_swrst_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_soft_reset_eth_swrst_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_soft_reset_eth_swrst_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enables the selected clock obervation selected of the MAC to be sent on the daisy chain up to the clock observation pad "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enables the selected clock obervation selected of the MAC to be sent on the daisy chain up to the clock observation pad "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_clkobs_ctrl_ena_clkobs1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines a timeslot associated for the port "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_setup_ring_id_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_setup_ring_id_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_setup_ring_id_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_setup_ring_id_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_setup_ring_id_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_setup_ring_id_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_setup_ring_id_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Select GPIO ring 0 or 1"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_setup_ring_sel_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_setup_ring_sel_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_setup_ring_sel_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_setup_ring_sel_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_setup_ring_sel_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_setup_ring_sel_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_setup_ring_sel_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 In I2c Mode, the first byte sent over I2C is bit [7:0], followed by [15:8],... "
                   "In MDIO Mode, bit [4:0] defines portaddr[4:0] and bit [12:8] defines devaddr[4:0]. "
                   "In StateOut Mode,this register can be used to write an extra 4B. "
                   "The number of bytes depends on Ethernet Ring Control settings. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_addr_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_addr_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_addr_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_addr_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_addr_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_addr_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_addr_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Data written during a direct write access. The first byte sent over I2C is bit [7:0]."
                   " The number of byte written depends on ethernet ring control register settings. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_wdata_wdata_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_wdata_wdata_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_wdata_wdata_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_wdata_wdata_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_wdata_wdata_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_wdata_wdata_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_wdata_wdata_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the 4 byte data read on I2C/MDIO interface during a register read. "
                   "first byte read is on bit [7:0],..."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_rdata_rdata_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_rdata_rdata_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_rdata_rdata_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_rdata_rdata_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_rdata_rdata_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_rdata_rdata_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_rdata_rdata_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
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
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_req_type_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_req_type_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_req_type_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_req_type_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_req_type_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_req_type_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_req_type_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 In I2C mode, defines which command:"
                   " - 00b : write address and write data command "
                   " - 01b : write address and read data "
                   " - 10b : write device address only"
                   " - 11b : read data only "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_cmd_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_cmd_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_cmd_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_cmd_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_cmd_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_cmd_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_cmd_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 In I2C mode, defines the number of address bytes (0-4) to transfer after the device address byte. "
                   "In MDIO mode, contains the MDIO transaction code (on bit [1:0]) and transaction type (on bit [2]) "
                   "In StateOut mode, this field is reserved"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_addrnum_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_addrnum_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_addrnum_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_addrnum_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_addrnum_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_addrnum_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_addrnum_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 In I2C mode, bit [6:0] defines the device address of the I2C device to target. "
                   "On StateOut mode, bit [7:0] defines the start address of the StateOut transfer. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_devaddr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_devaddr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_devaddr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_devaddr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_devaddr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_devaddr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_devaddr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 For I2C mode, maximum number of byte is 4"
                   "For MDIO mode, "
                   "In StateOut mode, maximum 8B can be transferred "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_datanum_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_datanum_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_datanum_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_datanum_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_datanum_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_datanum_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_datanum_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Transaction "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_lock_req_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_lock_req_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_lock_req_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_lock_req_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_lock_req_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_lock_req_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_lock_req_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines which GPIO group/pair is targeted by the transaction. Value value are 0-5."
                   " It is assumed that the GPIO pair targeted is programmed in expected mode for I2C and MDIO settings. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_gpio_id_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_gpio_id_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_gpio_id_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_gpio_id_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_gpio_id_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_gpio_id_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_gpio_id_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the status of the on-going transaction: "
                   " - 000b: No update "
                   " - 001b: Request sent to GPIO block. "
                   " - 010b: Request granted by GPIO block. "
                   " - 011b: Response received from GPIO block with no error. "
                   " - 101b: Response received from GPIO block with error."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_status_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_status_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_status_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_status_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_status_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_status_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_status_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Register get set when lock error status is received. Lock error bit need to be cleared by hardware."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_lock_err_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_lock_err_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_lock_err_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_lock_err_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_ring_ctrl_lock_err_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_ring_ctrl_lock_err_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_ring_ctrl_lock_err_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the software-controlled port_alive entry (1 bit per channel), going to the Look-Up Table"
                   " to create the final port_alive signal which will be sent to PGR block."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_soft_port_alive_val_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_soft_port_alive_val_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_soft_port_alive_val_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_soft_port_alive_val_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_soft_port_alive_val_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_soft_port_alive_val_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_soft_port_alive_val_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Offset which is added to timestamp during egress timestamping and"
                   "which need to account for full MAC/PCS layer latency. "
                   "Note that increment/drift is received from MBus Station as"
                   "the logic works on CoreClk. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_offset_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Adjust the Egress timestamping offset by the current Tx app fifo count multiply by"
                   " the adjustment factor (6 lsb represent nanosecond and 10 lsb represent fractional nanosecond)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_onestep_ets_offset_ctrl_eth_onestep_ets_txff_cnt_adj_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Offset which is added to timestamp received from MBus Station"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_rmw( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_offset_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Increment/Drift on MAC clock to automatically increment timestamp every"
                   "MAC clock cycle (4 lsb represent ns and 12 lsb represent ps)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_mac_ts_offset_ctrl_eth_mac_ts_incr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the local fault status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_lfault_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_lfault_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_lfault_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_lfault_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_lfault_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_lfault_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status0_macsts_lfault_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the remote fault status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_rfault_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_rfault_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_rfault_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_rfault_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_rfault_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_rfault_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status0_macsts_rfault_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the ORed local/remote fault status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_ofault_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_ofault_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_ofault_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_ofault_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_ofault_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_ofault_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status0_macsts_ofault_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the link up status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_linkup_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_linkup_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_linkup_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_linkup_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status0_macsts_linkup_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status0_macsts_linkup_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status0_macsts_linkup_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Signal OK status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_sigok_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_sigok_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_sigok_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_sigok_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_sigok_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_sigok_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status1_macsts_sigok_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Tx Idle status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_txidle_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_txidle_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_txidle_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_txidle_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_txidle_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_txidle_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status1_macsts_txidle_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Rx Idle status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_rxidle_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_rxidle_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_rxidle_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_rxidle_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_rxidle_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_rxidle_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status1_macsts_rxidle_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Tx good status of the MAC (per channel)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_txgood_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_txgood_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_txgood_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_txgood_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_status1_macsts_txgood_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_status1_macsts_txgood_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_status1_macsts_txgood_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Each 3-bit correspond to a channel slot. By default, each 50G channel has one time slot and must be sequenced as follow 0,4,2,6,1,5,3,7 (0xEE9CA0). When any channel is configured in 100G, 200G or 400G mode, it consumes the slot of the next channels (up to creating teh required number of timeslot. Those timeslot are fixed and should not be modified for the channels which are not reconfigured. A 100G channel requires 2 timeslots every 4 clock cycles. A 200G channel requires 4 timeslot every 2 clock cycles. A 400G channel requires 8 timeslot every clock cycle. Common configuration are as follow: "
                   " - 1x400G : 0x000000 "
                   " - 2x200G : 0x820820 "
                   " - 4x100G : 0xCA0CA0 "
                   " - 8x50G : 0xEE9CA0 "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_seq_chnl_seq_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_seq_chnl_seq_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_seq_chnl_seq_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_seq_chnl_seq_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_seq_chnl_seq_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_seq_chnl_seq_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_seq_chnl_seq_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Can be set dynamically before transmitting data to use the egress buffer sequencing"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_seq_sel_buf_seq_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_seq_sel_buf_seq_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_seq_sel_buf_seq_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_seq_sel_buf_seq_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_seq_sel_buf_seq_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_seq_sel_buf_seq_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_seq_sel_buf_seq_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enable Tx fifo for traffic"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_chnl_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_chnl_ena_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_chnl_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_chnl_ena_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_chnl_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_chnl_ena_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_chnl_ena_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, put the Tx fifo in flush mode, discarding any new packet coming from Egress Buffer"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_tx_flush_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_tx_flush_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_tx_flush_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_tx_flush_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_tx_flush_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_tx_flush_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_tx_flush_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the channel configuration as follow: "
                   " - 00b: 400G Mode (channel 0 only) "
                   " - 01b: 200G Mode (channel 0/4 only) "
                   " - 10b: 100G Mode (channel 0/2/4/6 only) "
                   " - 11b: 50G Mode (any channel) "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_chnl_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_chnl_mode_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_chnl_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_chnl_mode_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_chnl_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_chnl_mode_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_chnl_mode_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 This register need to be set when the MAC workis configured in Pause XOFF mode (not PFC mode). In that mode, the MAC reports RX_XOFF "
                   "through PFC_XOFF[0] signal and that is translated through asserting all 8 RX_PFCXOFF signals to the Core."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_rx_xoff_mode_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, replace the Rx PFC XOFF received and extracted by the MAC by a fix value provided by the register space"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_ovr_rx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, loopback the Tx data (from Egress Buffer) back to Rx (to Ingress Parser Buffer)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_txrx_lpbk_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_txrx_lpbk_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_txrx_lpbk_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_txrx_lpbk_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the value of Rx PFC XOFF when provided by the register space (one bit per PFC)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_val_rx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the amount of credit to send to Egress Buffer when channel get enabled (one credit is equivalent to 16B data payload)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_cred_ini_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_cred_ini_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_cred_ini_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_cred_ini_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_cred_ini_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_cred_ini_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_cred_ini_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the number of 16B data payload to be accumulated on a start of packet before forwarding the packet to the Tx MAC."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_min_thr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_min_thr_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_min_thr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_min_thr_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ctrl_min_thr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ctrl_min_thr_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ctrl_min_thr_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Rx PFC XOFF reported by the MAC layer"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_status_pfc_rxxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_pfc_rxxoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_status_pfc_rxxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_pfc_rxxoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_status_pfc_rxxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_pfc_rxxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_status_pfc_rxxoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Tx fifo current count in 16B increment"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_status_txff_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_txff_count_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_status_txff_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_txff_count_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_status_txff_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_txff_count_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_status_txff_count_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the Tx application fifo current count in 64B increment"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_status_txappfifo_count_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_txappfifo_count_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_status_txappfifo_count_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_txappfifo_count_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_status_txappfifo_count_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_status_txappfifo_count_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_status_txappfifo_count_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, globally disable the internal CRC regeneration from the MAC layer"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_crcgen_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_crcgen_dis_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_crcgen_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_crcgen_dis_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_crcgen_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_crcgen_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_crcgen_dis_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, disable the internal CRC regeneration from the MAC layer only for frames received with error"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_crcerr_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_crcerr_dis_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_crcerr_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_crcerr_dis_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_crcerr_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_crcerr_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_crcerr_dis_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, assert all PFC XOFF bits when Tx XOFF is received from the core of the chip for that channel"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_en_tx_xoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, replace the Tx PFC XOFF received from Core side and sent to the MAC by a fix value provided by the register space"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_ovr_tx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 This register need to be set when the MAC is configured in Pause Frame mode. In that mode, any of the 8 Tx PFC XOFF (if enable)"
                   " or Tx XOFF received from the core will be translated as a Pause Frame."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_tx_xoff_mode_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 In Tx Pause frame mode, this register defines if XOFF is sent to MAC layer if any of the Tx PFC XOFF bit "
                   " or Tx XOFF received from the core will be translated as a Pause Frame."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_en_tx_pfc_xoff_mode_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Defines the value of Tx PFC XOFF when provided by the register space (one bit per PFC)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_val_tx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enable each Tx PFC XOFF (one bit per PFC) to be sent to MAC layer"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxff_ctrl_en_tx_pfcxoff_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, globally disable the internal CRC checker from the MAC layer"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txcrc_trunc_ctrl_crcchk_dis_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, disable the internal CRC checker and do not remove the 4B CRC before sending to the Tx MAC layer (MAC should be dynamically controlled to not generate Tx CRC."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txcrc_trunc_ctrl_crcerr_dis_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, disable the internal CRC regeneration from the MAC layer only for frames received with error"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txcrc_trunc_ctrl_crcrmv_dis_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enable packet truncation for that channel"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_ena_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report the size in byte for truncating packet. "
                   "Truncated packet will have valid CRC32 independantly of the CRC received from Deparser."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_txcrc_trunc_ctrl_trunc_size_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Capture the Rx status of last error packet received"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxpkt_err_sts_last_rxsts_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 report the number of entry in fifo (max 4)"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_stat_count7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_stat_count7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_stat_count7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When the fifo is empty return 0x0, otherwise return: "
                   " - [47:0] : Capture timestamp "
                   " - [56:48]: Timestamp ID (9-bit self incrementing allowing to number of lost ID) "
                   " - [62:57]: Reserved "
                   " - [63]  : Valid entry when set to 1 "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_out_cts_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_out_cts_set(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_out_cts_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_out_cts_get(dev_id, umac, ch, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_fifo_out_cts_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t ch, uint64_t *data_p, uint64_t val) {
  uint64_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_fifo_out_cts_get( dev_id, umac, ch, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_fifo_out_cts_set( dev_id, umac, ch, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Allow to flexibly map internal status to generate port_alive status for TM and packet generator : "
                   " port_alive = port_alive_lut[{ofault,ext_rxsigok,rxsigok,txgood,rxsync}] "
                   " Thus if only rxsync contributes to port_alive, programming is 16'hAAAA."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_port_alive_lut_port_alive_lut_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_port_alive_lut_port_alive_lut_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_port_alive_lut_port_alive_lut_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_port_alive_lut_port_alive_lut_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_port_alive_lut_port_alive_lut_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_port_alive_lut_port_alive_lut_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_port_alive_lut_port_alive_lut_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report which interrupt register is asserted: "
                   " - bit 0 : Channel interrupt "
                   " - bit 1 : Memory interrupt "
                   " - bit 2 : Two-step timestamp interrupt "
                   " - bit 3 : MAC interrupt "
                   " - bit 4 : PCS memory interrupt "
                   " - bit 5 : MDIOCI interrupt "
                   " - bit 6 : Microcontroller interrupt. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_global_intr_stat_intr_lo_stat_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_global_intr_stat_intr_lo_stat_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_global_intr_stat_intr_lo_stat_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_global_intr_stat_intr_lo_stat_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_global_intr_stat_intr_lo_stat_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_global_intr_stat_intr_lo_stat_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_global_intr_stat_intr_lo_stat_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Report which interrupt register is asserted: "
                   " - bit 0 : Channel interrupt "
                   " - bit 1 : Memory interrupt "
                   " - bit 2 : Two-step timestamp interrupt "
                   " - bit 3 : MAC interrupt "
                   " - bit 4 : PCS memory interrupt "
                   " - bit 5 : MDIOCI interrupt "
                   " - bit 6 : Microcontroller interrupt. "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_global_intr_stat_intr_hi_stat_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_global_intr_stat_intr_hi_stat_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_global_intr_stat_intr_hi_stat_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_global_intr_stat_intr_hi_stat_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_global_intr_stat_intr_hi_stat_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_global_intr_stat_intr_hi_stat_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_global_intr_stat_intr_hi_stat_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject a crc error for each channel"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_crcerr_inj_inj_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_crcerr_inj_inj_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_crcerr_inj_inj_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_crcerr_inj_inj_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_crcerr_inj_inj_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_crcerr_inj_inj_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_crcerr_inj_inj_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the base address of debug fifo structure"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_base_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_base_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_base_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_base_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_base_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_base_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_base_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the limit address of debug fifo structure"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_limit_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
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
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_log_reg_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_log_reg_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_log_reg_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_log_reg_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_log_reg_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_log_reg_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_log_reg_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
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
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_log_inst_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_log_inst_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_log_inst_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_log_inst_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_log_inst_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_log_inst_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_log_inst_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When set, the debug buffer works in fifo mode which stalls automatically when fifo is full. "
                   " When cleared, the fifo works in circular buffer mode, overriding the oldest instruction executed."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_fifomode_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_fifomode_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_fifomode_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_fifomode_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_fifomode_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_fifomode_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_fifomode_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When set, assert an interrupt when debug fifo get full. It is only applicable if debug works in fifo mode and not in circular buffer"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_full_int_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_full_int_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_full_int_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_full_int_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_full_int_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_full_int_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_full_int_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 when set, enable the debug fifo structure"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_enable_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_enable_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_enable_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_enable_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_ctrl_enable_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_ctrl_enable_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_ctrl_enable_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the current write address of debug fifo structure"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_tail_ptr_tail_ptr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the current read address of the debug fifo"
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_debug_head_ptr_head_ptr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the maximum of the timeout counter."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_watchdog_ctrl_timeout_value_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Enables the watchdog timeout mechanism to trigger NMI."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_ctrl_enable_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_ctrl_enable_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_ctrl_enable_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_ctrl_enable_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_ctrl_enable_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_ctrl_enable_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_watchdog_ctrl_enable_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the maximum of the timeout counter."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_count_cur_time_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_count_cur_time_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_count_cur_time_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_count_cur_time_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_watchdog_count_cur_time_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_watchdog_count_cur_time_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_watchdog_count_cur_time_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When set, the microcontroller code execution is halted through the logic "
                   "does not responding to TV80 trying to read the next instruction from its firmware located in SRAM."
                   " SW will have to clear this bit to enable TV80 to resume its operation."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_stall_on_error_stall_absolute_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When set, the TV80 will be stalled when a memory multi-bit error is detected on its own SRAM containing data/firmware. SW will have to clear this bit to enable TV80 to resume its operation."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_stall_on_error_stall_on_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 When set, the TV80 will be stalled when a read error transaction generated by TV80."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_stall_on_error_stall_on_rerr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Status register reporting that the TV80 microcontroller has been halted."
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_halted_status_tv80_halted_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_halted_status_tv80_halted_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_halted_status_tv80_halted_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_halted_status_tv80_halted_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_halted_status_tv80_halted_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_halted_status_tv80_halted_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_halted_status_tv80_halted_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Define the static address bits of the address sent to Serdes IO Tile register interface from the CPU (MBus):"
                   "-bit [3:0] : corresponds to address bits [15:12] of Serdes IO Tile register interface (defines pages of 4K register) "
                   "-bit [7:4] : corresponds to address bits [22:19] of Serdes IO Tile register interface (Serdes ID bit [6:3] should be 0) "
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_eth_mdioci_addr_cpu_addrreg_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_txfifo_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_stat_rxeop_timo7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_txfifo_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en0_rxeop_timo7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_txfifo_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_en1_rxeop_timo7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx MAC report an errored packet for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx CRC detect a CRC error for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txcrc_err7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx Fifo overflows signalling a credit loop issue for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_txfifo_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 0.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 1.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 2.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 3.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 4.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 5.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 6.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx Packet is never closed by the MAC and timeout occurs for channel 7.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_chnl_intr_inj_rxeop_timo7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_nonempty7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_stat_cts_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_stat_cts_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_stat_cts_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_nonempty7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en0_cts_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en0_cts_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en0_cts_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_nonempty7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_en1_cts_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_en1_cts_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_en1_cts_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo is non-empty for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_nonempty7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 0
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 1
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 2
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 3
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 4
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 5
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 6
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when the two-step timestamping fifo overflows for channel 7
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_cts_intr_inj_cts_ovf7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_cts_intr_inj_cts_ovf7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_cts_intr_inj_cts_ovf7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_txfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_txfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_txappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_txappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_rxappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_rxappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_statsmem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_statsmem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_stat_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_txfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_txfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_txappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_txappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_rxappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_rxappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_statsmem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_statsmem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en0_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_txfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_txfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_txappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_txappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_rxappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_rxappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_statsmem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_statsmem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_en1_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_txfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_txfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_txappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Tx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_txappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_rxappfifo_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Rx app fifo gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_rxappfifo_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_statsmem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Stats memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_statsmem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when Microcontroller memory gets correctable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_tv80mem_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Asserted when microcontroller memory gets uncorrectable error.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mem_intr_inj_tv80mem_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_intr7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_intr7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_intr7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_rerr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_rerr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_rerr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_debug_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_debug_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_stat_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_stat_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_stat_debug_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_intr7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_intr7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_intr7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_rerr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_rerr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_rerr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_debug_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_debug_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en0_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en0_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en0_debug_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_intr7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_intr7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_intr7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_rerr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_rerr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_rerr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_debug_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_debug_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_en1_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_en1_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_en1_debug_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 0 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 1 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 2 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 3 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 4 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 5 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 6 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 interrupt 7 for PCIe. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_intr7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_intr7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_intr7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Register transaction initiated by TV80 ended up in error. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_rerr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_rerr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_rerr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_rerr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_rerr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_rerr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_rerr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Generates TV80 debug fifo full interrupt. If enabled interrupt is sent to PCIe and must be cleared by main CPU
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_debug_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_debug_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_debug_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_debug_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_uctrl_intr_inj_debug_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_uctrl_intr_inj_debug_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_uctrl_intr_inj_debug_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Disable Error Checking
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram0_disable_check_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram0_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram1_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram2_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txfifo_sram3_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Disable Error Checking
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_disable_check_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram0_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram1_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram2_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram3_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_sbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Inject Multiple Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txff_ecc_txappfifo_sram4_inject_mbe_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx Fifo Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txfifo_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_sbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_sbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txfifo_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx Fifo Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txfifo_sbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_sbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_sbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_sbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_sbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_sbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txfifo_sbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx fifo Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txfifo_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx fifo Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txfifo_mbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_mbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_mbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_mbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txfifo_mbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txfifo_mbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txfifo_mbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx App Fifo Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_sbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_sbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txappfifo_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx App Fifo Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_sbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_sbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_sbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_sbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_sbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_sbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txappfifo_sbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx App fifo Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txappfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Tx App fifo Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_mbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_mbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_mbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_mbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_txappfifo_mbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_txappfifo_mbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_txappfifo_mbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Rx App Fifo Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxappfifo_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Rx App Fifo Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxappfifo_sbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Rx App fifo Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxappfifo_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Rx App fifo Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_rxappfifo_mbe_err_log_instid_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Statistic Memory Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_statsmem_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_statsmem_sbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_statsmem_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_statsmem_sbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_statsmem_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_statsmem_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_statsmem_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Statistic Memory Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_statsmem_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_statsmem_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_statsmem_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_statsmem_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_statsmem_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_statsmem_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_statsmem_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Microcontroller Memory Single Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80mem_sbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80mem_sbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80mem_sbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80mem_sbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80mem_sbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80mem_sbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80mem_sbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Log Register for Microcontroller Memory Dual Bit Error
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80mem_mbe_err_log_addr_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80mem_mbe_err_log_addr_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80mem_mbe_err_log_addr_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80mem_mbe_err_log_addr_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80mem_mbe_err_log_addr_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80mem_mbe_err_log_addr_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80mem_mbe_err_log_addr_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mac_en0_mac_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mac_en0_mac_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mac_en0_mac_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mac_en0_mac_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mac_en0_mac_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mac_en0_mac_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mac_en0_mac_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Interrupt Enable Register
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_mac_en1_mac_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mac_en1_mac_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mac_en1_mac_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mac_en1_mac_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_mac_en1_mac_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_mac_en1_mac_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_mac_en1_mac_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_intr_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_intr_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_intr_7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_nmi_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_nmi_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_stat_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_stat_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_stat_nmi_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_intr_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_intr_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_intr_7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_nmi_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_nmi_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en0_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en0_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en0_nmi_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_intr_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_intr_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_intr_7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_nmi_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_nmi_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_en1_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_en1_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_en1_nmi_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 0 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_0_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_0_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_0_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_0_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_0_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_0_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_0_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 1 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_1_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_1_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_1_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_1_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_1_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_1_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_1_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 2 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_2_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_2_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_2_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_2_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_2_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_2_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_2_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 3 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_3_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_3_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_3_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_3_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_3_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_3_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_3_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 4 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_4_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_4_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_4_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_4_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_4_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_4_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_4_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 5 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_5_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_5_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_5_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_5_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_5_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_5_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_5_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 6 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_6_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_6_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_6_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_6_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_6_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_6_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_6_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Maskable Interrupt 7 for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_7_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_7_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_7_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_7_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_intr_7_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_intr_7_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_intr_7_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

/**********************************************************************
 Assert Non-Maskable Interrupt for TV80 microcontroller through interrupt injection. Interrupt should be cleared by TV80.
***********************************************************************/
bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_nmi_set( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_nmi_set( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_nmi_get( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t *val, bool hw) {
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_nmi_get( dev_id, umac, data_p, val, hw);
  return BF_SUCCESS;
}

bf_status_t bf_ll_eth400g_mac_rspec_tv80_intr_inj_nmi_rmw( bf_dev_id_t dev_id, uint32_t umac, uint32_t *data_p, uint32_t val) {
  uint32_t unused_fld = 0;
  uint32_t physical_umac;
  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);

  if (rc != BF_SUCCESS) return rc;

  eth400g_mac_rspec_tv80_intr_inj_nmi_get( dev_id, umac, data_p, &unused_fld, true);
  eth400g_mac_rspec_tv80_intr_inj_nmi_set( dev_id, umac, data_p, val, true);
  return BF_SUCCESS;
}

