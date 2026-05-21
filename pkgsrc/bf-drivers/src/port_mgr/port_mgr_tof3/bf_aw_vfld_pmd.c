
#include <stdint.h>
#include <bf_types/bf_types.h>
#include "aw_if.h"

/***********************************************************************
 *           bf_aw_pmd_vfld_lsref_bypass_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_lsref_bypass_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_lsref_bypass_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_lsref_bypass_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_lsref_bypass_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_lsref_bypass_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_lsref_bypass_get(&mss, val);

  bf_aw_trace("pmd_vfld_lsref_bypass_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cmn_sris_enable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cmn_sris_enable_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_vfld_cmn_sris_enable_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cmn_sris_enable_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cmn_sris_enable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cmn_sris_enable_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cmn_sris_enable_get(&mss, val);

  bf_aw_trace("pmd_vfld_cmn_sris_enable_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_fast_sram_clk_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_fast_sram_clk_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_fast_sram_clk_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_fast_sram_clk_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_fast_sram_clk_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_fast_sram_clk_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_fast_sram_clk_get(&mss, val);

  bf_aw_trace("pmd_vfld_fast_sram_clk_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_tx_spare_0_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_tx_spare_0_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_vfld_tx_spare_0_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_tx_spare_0_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_tx_spare_0_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_tx_spare_0_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_tx_spare_0_get(&mss, val);

  bf_aw_trace("pmd_vfld_tx_spare_0_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_tx_spare_1_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_tx_spare_1_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_vfld_tx_spare_1_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_tx_spare_1_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_tx_spare_1_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_tx_spare_1_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_tx_spare_1_get(&mss, val);

  bf_aw_trace("pmd_vfld_tx_spare_1_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq0_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq0_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nyq0_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq0_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq0_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq0_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq0_get(&mss, val);

  bf_aw_trace("pmd_vfld_nyq0_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq1_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq1_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nyq1_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq1_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq1_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq1_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq1_get(&mss, val);

  bf_aw_trace("pmd_vfld_nyq1_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq2_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq2_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nyq2_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq2_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq2_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq2_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq2_get(&mss, val);

  bf_aw_trace("pmd_vfld_nyq2_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq3_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq3_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nyq3_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq3_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq3_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq3_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq3_get(&mss, val);

  bf_aw_trace("pmd_vfld_nyq3_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq4_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq4_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nyq4_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq4_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq4_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq4_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq4_get(&mss, val);

  bf_aw_trace("pmd_vfld_nyq4_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq5_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq5_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nyq5_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq5_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nyq5_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nyq5_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nyq5_get(&mss, val);

  bf_aw_trace("pmd_vfld_nyq5_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cm1c1_takeover_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cm1c1_takeover_ratio_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_cm1c1_takeover_ratio_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cm1c1_takeover_ratio_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cm1c1_takeover_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cm1c1_takeover_ratio_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cm1c1_takeover_ratio_get(&mss, val);

  bf_aw_trace("pmd_vfld_cm1c1_takeover_ratio_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c0_takeover_code_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c0_takeover_code_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_c0_takeover_code_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c0_takeover_code_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c0_takeover_code_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c0_takeover_code_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c0_takeover_code_get(&mss, val);

  bf_aw_trace("pmd_vfld_c0_takeover_code_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_counter_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_counter_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_eqbk_counter_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_counter_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_counter_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_counter_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_counter_get(&mss, val);

  bf_aw_trace("pmd_vfld_eqbk_counter_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_temp1_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_temp1_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_temp1_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_temp1_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_temp1_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_temp1_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_temp1_get(&mss, val);

  bf_aw_trace("pmd_vfld_temp1_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_temp2_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_temp2_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_temp2_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_temp2_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_temp2_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_temp2_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_temp2_get(&mss, val);

  bf_aw_trace("pmd_vfld_temp2_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_sigdet_offset_cal_valid_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_sigdet_offset_cal_valid_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       uint32_t ln,
                                                       uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_sigdet_offset_cal_valid_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_sigdet_offset_cal_valid_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_sigdet_offset_cal_valid_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_sigdet_offset_cal_valid_get(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       uint32_t ln,
                                                       uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_sigdet_offset_cal_valid_get(&mss, val);

  bf_aw_trace("pmd_vfld_sigdet_offset_cal_valid_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_sigdet_offset_cal_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_sigdet_offset_cal_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_sigdet_offset_cal_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_sigdet_offset_cal_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_sigdet_offset_cal_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_sigdet_offset_cal_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_sigdet_offset_cal_get(&mss, val);

  bf_aw_trace("pmd_vfld_sigdet_offset_cal_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_fg_done_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_fg_done_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_fg_done_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_fg_done_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_fg_done_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_fg_done_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_fg_done_get(&mss, val);

  bf_aw_trace("pmd_vfld_fg_done_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_first_iter_done_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_first_iter_done_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_eqbk_first_iter_done_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_first_iter_done_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_first_iter_done_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_first_iter_done_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_first_iter_done_get(&mss, val);

  bf_aw_trace("pmd_vfld_eqbk_first_iter_done_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_post1_npre1_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_post1_npre1_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_post1_npre1_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_post1_npre1_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_post1_npre1_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_post1_npre1_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_post1_npre1_get(&mss, val);

  bf_aw_trace("pmd_vfld_post1_npre1_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_linkeval_state_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_linkeval_state_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_linkeval_state_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_linkeval_state_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_linkeval_state_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_linkeval_state_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_linkeval_state_get(&mss, val);

  bf_aw_trace("pmd_vfld_linkeval_state_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c0_dec_counter_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c0_dec_counter_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_c0_dec_counter_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c0_dec_counter_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c0_dec_counter_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c0_dec_counter_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c0_dec_counter_get(&mss, val);

  bf_aw_trace("pmd_vfld_c0_dec_counter_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cm1_inc_counter_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cm1_inc_counter_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_cm1_inc_counter_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cm1_inc_counter_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cm1_inc_counter_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cm1_inc_counter_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cm1_inc_counter_get(&mss, val);

  bf_aw_trace("pmd_vfld_cm1_inc_counter_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c1_inc_counter_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c1_inc_counter_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_c1_inc_counter_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c1_inc_counter_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c1_inc_counter_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c1_inc_counter_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c1_inc_counter_get(&mss, val);

  bf_aw_trace("pmd_vfld_c1_inc_counter_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c0_iter_remain_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c0_iter_remain_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_c0_iter_remain_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c0_iter_remain_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_c0_iter_remain_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_c0_iter_remain_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_c0_iter_remain_get(&mss, val);

  bf_aw_trace("pmd_vfld_c0_iter_remain_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_channel_type_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_channel_type_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_channel_type_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_channel_type_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_channel_type_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_channel_type_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_channel_type_get(&mss, val);

  bf_aw_trace("pmd_vfld_channel_type_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_autoeq_disable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_autoeq_disable_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_autoeq_disable_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_autoeq_disable_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_autoeq_disable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_autoeq_disable_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_autoeq_disable_get(&mss, val);

  bf_aw_trace("pmd_vfld_autoeq_disable_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqstore_valid_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqstore_valid_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_eqstore_valid_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqstore_valid_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqstore_valid_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqstore_valid_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqstore_valid_get(&mss, val);

  bf_aw_trace("pmd_vfld_eqstore_valid_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_vga_cap_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_vga_cap_adapt_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_disable_vga_cap_adapt_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_vga_cap_adapt_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_vga_cap_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_vga_cap_adapt_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_vga_cap_adapt_get(&mss, val);

  bf_aw_trace("pmd_vfld_disable_vga_cap_adapt_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_ctle_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_ctle_adapt_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_disable_ctle_adapt_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_ctle_adapt_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_ctle_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_ctle_adapt_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_ctle_adapt_get(&mss, val);

  bf_aw_trace("pmd_vfld_disable_ctle_adapt_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_c0_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_c0_adapt_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_disable_c0_adapt_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_c0_adapt_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_c0_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_c0_adapt_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_c0_adapt_get(&mss, val);

  bf_aw_trace("pmd_vfld_disable_c0_adapt_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_cm1c1_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_cm1c1_adapt_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_disable_cm1c1_adapt_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_cm1c1_adapt_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_disable_cm1c1_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_disable_cm1c1_adapt_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_disable_cm1c1_adapt_get(&mss, val);

  bf_aw_trace("pmd_vfld_disable_cm1c1_adapt_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_vga_cap_takeover_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_vga_cap_takeover_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_vga_cap_takeover_set", dev_id, dev_port, ln,
              1, " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_vga_cap_takeover_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_vga_cap_takeover_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_vga_cap_takeover_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_vga_cap_takeover_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_vga_cap_takeover_get", dev_id, dev_port, ln,
              1, " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_ctle_takeover_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_ctle_takeover_set(bf_dev_id_t dev_id,
                                                        bf_dev_port_t dev_port,
                                                        uint32_t ln,
                                                        uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_ctle_takeover_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_ctle_takeover_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_ctle_takeover_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_ctle_takeover_get(bf_dev_id_t dev_id,
                                                        bf_dev_port_t dev_port,
                                                        uint32_t ln,
                                                        uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_ctle_takeover_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_ctle_takeover_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_c0_takeover_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_c0_takeover_set(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_c0_takeover_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_c0_takeover_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_c0_takeover_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_c0_takeover_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_c0_takeover_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_c0_takeover_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_cm1c1_dz_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_cm1c1_dz_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_cm1c1_dz_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_cm1c1_dz_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_cm1c1_dz_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_cm1c1_dz_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_cm1c1_dz_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_cm1c1_dz_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_cdr_offset_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_cdr_offset_set(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_cdr_offset_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_cdr_offset_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_cdr_offset_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_cdr_offset_get(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln,
                                                     uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_cdr_offset_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_cdr_offset_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_skip_wait_lt_done_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_skip_wait_lt_done_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_skip_wait_lt_done_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_skip_wait_lt_done_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_skip_wait_lt_done_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_skip_wait_lt_done_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_skip_wait_lt_done_get(&mss, val);

  bf_aw_trace("pmd_vfld_skip_wait_lt_done_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_target_cma_bypass_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_target_cma_bypass_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_target_cma_bypass_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_target_cma_bypass_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_target_cma_bypass_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_target_cma_bypass_get(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_target_cma_bypass_get(&mss, val);

  bf_aw_trace("pmd_vfld_target_cma_bypass_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_zero_small_taps_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_zero_small_taps_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_zero_small_taps_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_zero_small_taps_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_zero_small_taps_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_zero_small_taps_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_zero_small_taps_get(&mss, val);

  bf_aw_trace("pmd_vfld_zero_small_taps_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nes_mode_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nes_mode_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nes_mode_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nes_mode_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nes_mode_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nes_mode_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nes_mode_get(&mss, val);

  bf_aw_trace("pmd_vfld_nes_mode_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_pick_c162_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_pick_c162_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_pick_c162_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_pick_c162_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_pick_c162_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_pick_c162_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_pick_c162_get(&mss, val);

  bf_aw_trace("pmd_vfld_pick_c162_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_vga_cap_takeover_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_vga_cap_takeover_ratio_set(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_vga_cap_takeover_ratio_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_vga_cap_takeover_ratio_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_vga_cap_takeover_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_vga_cap_takeover_ratio_get(bf_dev_id_t dev_id,
                                                      bf_dev_port_t dev_port,
                                                      uint32_t ln,
                                                      uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_vga_cap_takeover_ratio_get(&mss, val);

  bf_aw_trace("pmd_vfld_vga_cap_takeover_ratio_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_ctle_takeover_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_ctle_takeover_ratio_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_ctle_takeover_ratio_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_ctle_takeover_ratio_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_ctle_takeover_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_ctle_takeover_ratio_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_ctle_takeover_ratio_get(&mss, val);

  bf_aw_trace("pmd_vfld_ctle_takeover_ratio_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_dz_pre1_fw_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_dz_pre1_fw_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_dz_pre1_fw_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_dz_pre1_fw_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_dz_pre1_fw_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_dz_pre1_fw_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_dz_pre1_fw_get(&mss, val);

  bf_aw_trace("pmd_vfld_dz_pre1_fw_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_dz_post1_fw_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_dz_post1_fw_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_dz_post1_fw_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_dz_post1_fw_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_dz_post1_fw_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_dz_post1_fw_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_dz_post1_fw_get(&mss, val);

  bf_aw_trace("pmd_vfld_dz_post1_fw_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cdr_offset_cfg_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cdr_offset_cfg_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_cdr_offset_cfg_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cdr_offset_cfg_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_cdr_offset_cfg_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_cdr_offset_cfg_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_cdr_offset_cfg_get(&mss, val);

  bf_aw_trace("pmd_vfld_cdr_offset_cfg_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_rxeq_prbs_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_rxeq_prbs_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_rxeq_prbs_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_rxeq_prbs_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_rxeq_prbs_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_rxeq_prbs_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_rxeq_prbs_get(&mss, val);

  bf_aw_trace("pmd_vfld_rxeq_prbs_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_skip_delay_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_skip_delay_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_eqbk_skip_delay_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_skip_delay_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_skip_delay_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_skip_delay_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_skip_delay_get(&mss, val);

  bf_aw_trace("pmd_vfld_eqbk_skip_delay_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_hold_req_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_hold_req_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_eqbk_hold_req_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_hold_req_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_hold_req_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_hold_req_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_hold_req_get(&mss, val);

  bf_aw_trace("pmd_vfld_eqbk_hold_req_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_hold_ack_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_hold_ack_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_eqbk_hold_ack_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_hold_ack_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_eqbk_hold_ack_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_eqbk_hold_ack_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_eqbk_hold_ack_get(&mss, val);

  bf_aw_trace("pmd_vfld_eqbk_hold_ack_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_ffe_tap_disable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_ffe_tap_disable_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_ffe_tap_disable_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_ffe_tap_disable_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_ffe_tap_disable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_ffe_tap_disable_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_ffe_tap_disable_get(&mss, val);

  bf_aw_trace("pmd_vfld_ffe_tap_disable_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_ffe_tap_disable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_ffe_tap_disable_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_ffe_tap_disable_set", dev_id, dev_port, ln,
              1, " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_ffe_tap_disable_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_ffe_tap_disable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_ffe_tap_disable_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_ffe_tap_disable_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_ffe_tap_disable_get", dev_id, dev_port, ln,
              1, " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_rx_sris_enable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_rx_sris_enable_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_rx_sris_enable_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_rx_sris_enable_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_rx_sris_enable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_rx_sris_enable_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_rx_sris_enable_get(&mss, val);

  bf_aw_trace("pmd_vfld_rx_sris_enable_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_enable_roaming_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_enable_roaming_adapt_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_enable_roaming_adapt_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_enable_roaming_adapt_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_enable_roaming_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_enable_roaming_adapt_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_enable_roaming_adapt_get(&mss, val);

  bf_aw_trace("pmd_vfld_enable_roaming_adapt_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_roaming_windows_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_roaming_windows_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_custom_roaming_windows_set", dev_id, dev_port, ln,
              1, " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_roaming_windows_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_custom_roaming_windows_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_custom_roaming_windows_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_custom_roaming_windows_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_custom_roaming_windows_get", dev_id, dev_port, ln,
              1, " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_first_fom_done_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_first_fom_done_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_first_fom_done_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_first_fom_done_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_first_fom_done_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_first_fom_done_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_first_fom_done_get(&mss, val);

  bf_aw_trace("pmd_vfld_first_fom_done_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_roaming_windows_cfg_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_roaming_windows_cfg_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_roaming_windows_cfg_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_roaming_windows_cfg_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_roaming_windows_cfg_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_roaming_windows_cfg_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_roaming_windows_cfg_get(&mss, val);

  bf_aw_trace("pmd_vfld_roaming_windows_cfg_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_reduced_taps_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_reduced_taps_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_reduced_taps_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_reduced_taps_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_reduced_taps_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_reduced_taps_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_reduced_taps_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_reduced_taps_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_reduced_taps_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_reduced_taps_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_reduced_taps_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_reduced_taps_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_reduced_taps_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_reduced_taps_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_reduced_taps_get(&mss, val);

  bf_aw_trace("pmd_vfld_reduced_taps_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_auto_lookup_dfe_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_auto_lookup_dfe_ratio_set(bf_dev_id_t dev_id,
                                                         bf_dev_port_t dev_port,
                                                         uint32_t ln,
                                                         uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_use_auto_lookup_dfe_ratio_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_auto_lookup_dfe_ratio_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_use_auto_lookup_dfe_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_use_auto_lookup_dfe_ratio_get(bf_dev_id_t dev_id,
                                                         bf_dev_port_t dev_port,
                                                         uint32_t ln,
                                                         uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_use_auto_lookup_dfe_ratio_get(&mss, val);

  bf_aw_trace("pmd_vfld_use_auto_lookup_dfe_ratio_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_enable_dfe_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_enable_dfe_ratio_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_enable_dfe_ratio_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_enable_dfe_ratio_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_enable_dfe_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_enable_dfe_ratio_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_enable_dfe_ratio_get(&mss, val);

  bf_aw_trace("pmd_vfld_enable_dfe_ratio_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nes_dfe_bypass_value_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nes_dfe_bypass_value_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_nes_dfe_bypass_value_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nes_dfe_bypass_value_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_nes_dfe_bypass_value_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_nes_dfe_bypass_value_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_nes_dfe_bypass_value_get(&mss, val);

  bf_aw_trace("pmd_vfld_nes_dfe_bypass_value_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_target_cma_bypass_value_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_target_cma_bypass_value_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       uint32_t ln,
                                                       uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_target_cma_bypass_value_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_target_cma_bypass_value_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_target_cma_bypass_value_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_target_cma_bypass_value_get(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       uint32_t ln,
                                                       uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_target_cma_bypass_value_get(&mss, val);

  bf_aw_trace("pmd_vfld_target_cma_bypass_value_get", dev_id, dev_port, ln, 1,
              " val", *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_custom_dfe_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_custom_dfe_ratio_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_vfld_custom_dfe_ratio_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_custom_dfe_ratio_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_vfld_custom_dfe_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_vfld_custom_dfe_ratio_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t *val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_vfld_custom_dfe_ratio_get(&mss, val);

  bf_aw_trace("pmd_vfld_custom_dfe_ratio_get", dev_id, dev_port, ln, 1, " val",
              *(uint32_t *)(intptr_t)((intptr_t)(val)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}
