
#include <stdint.h>
#include <bf_types/bf_types.h>
#include "aw_if.h"

/***********************************************************************
 *           bf_aw_pmd_cmn_clkgen_refdiv_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_clkgen_refdiv_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t div) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_clkgen_refdiv_set", dev_id, dev_port, ln, 1, " div",
              (uint32_t)((intptr_t)div & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_clkgen_refdiv_set(&mss, div);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_clkgen_refdiv_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_clkgen_refdiv_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *div) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_clkgen_refdiv_get(&mss, div);

  bf_aw_trace("pmd_cmn_clkgen_refdiv_get", dev_id, dev_port, ln, 1, "  div",
              *(uint32_t *)(intptr_t)((intptr_t)(div)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_logical_lane_num_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_logical_lane_num_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t logical_lane,
                                                uint32_t an_no_attached) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_logical_lane_num_set ", dev_id, dev_port, ln, 2,
              " logical_lane", (uint32_t)((intptr_t)logical_lane & 0xffffffff),
              " an_no_attached",
              (uint32_t)((intptr_t)an_no_attached & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_logical_lane_num_set(&mss, logical_lane,
                                                  an_no_attached);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_logical_lane_num_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_logical_lane_num_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *logical_lane,
                                                uint32_t *an_no_attached) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_logical_lane_num_get(&mss, logical_lane,
                                                  an_no_attached);

  bf_aw_trace("pmd_anlt_logical_lane_num_get ", dev_id, dev_port, ln, 2,
              "  logical_lane",
              *(uint32_t *)(intptr_t)((intptr_t)(logical_lane)&UINTMAX_MAX),
              "  an_no_attached",
              *(uint32_t *)(intptr_t)((intptr_t)(an_no_attached)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_adv_ability_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_adv_ability_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *adv_ability, uint32_t *fec_ability, uint32_t nonce) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_auto_neg_adv_ability_set ", dev_id, dev_port, ln, 3,
              " adv_ability", (uint32_t)((intptr_t)adv_ability & 0xffffffff),
              " fec_ability", (uint32_t)((intptr_t)fec_ability & 0xffffffff),
              " nonce", (uint32_t)((intptr_t)nonce & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_adv_ability_set(&mss, adv_ability,
                                                      fec_ability, nonce);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_adv_ability_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_adv_ability_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *adv_ability,
                                                    uint32_t *fec_ability) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_adv_ability_get(&mss, adv_ability,
                                                      fec_ability);

  bf_aw_trace("pmd_anlt_auto_neg_adv_ability_get ", dev_id, dev_port, ln, 2,
              "  adv_ability",
              *(uint32_t *)(intptr_t)((intptr_t)(adv_ability)&UINTMAX_MAX),
              " fec_ability",
              *(uint32_t *)(intptr_t)((intptr_t)(fec_ability)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_config_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t status_check_disable,
                                               uint32_t next_page_en,
                                               uint32_t an_no_nonce_check) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_auto_neg_config_set ", dev_id, dev_port, ln, 3,
              " status_check_disable",
              (uint32_t)((intptr_t)status_check_disable & 0xffffffff),
              " next_page_en", (uint32_t)((intptr_t)next_page_en & 0xffffffff),
              " an_no_nonce_check",
              (uint32_t)((intptr_t)an_no_nonce_check & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_config_set(
      &mss, status_check_disable, next_page_en, an_no_nonce_check);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_config_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *status_check_disable,
                                               uint32_t *next_page_en,
                                               uint32_t *an_no_nonce_check) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_config_get(
      &mss, status_check_disable, next_page_en, an_no_nonce_check);

  bf_aw_trace(
      "pmd_anlt_auto_neg_config_get ", dev_id, dev_port, ln, 3,
      "  status_check_disable",
      *(uint32_t *)(intptr_t)((intptr_t)(status_check_disable)&UINTMAX_MAX),
      "  next_page_en",
      *(uint32_t *)(intptr_t)((intptr_t)(next_page_en)&UINTMAX_MAX),
      "  an_no_nonce_check",
      *(uint32_t *)(intptr_t)((intptr_t)(an_no_nonce_check)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_start_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_start_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t start) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_auto_neg_start_set ", dev_id, dev_port, ln, 1, " start",
              (uint32_t)((intptr_t)start & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_start_set(&mss, start);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_start_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_start_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *start) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_start_get(&mss, start);

  bf_aw_trace("pmd_anlt_auto_neg_start_get ", dev_id, dev_port, ln, 1,
              "  start",
              *(uint32_t *)(intptr_t)((intptr_t)(start)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_status_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_status_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *link_good) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_status_get(&mss, link_good);

  bf_aw_trace("pmd_anlt_auto_neg_status_get ", dev_id, dev_port, ln, 1,
              "  link_good",
              *(uint32_t *)(intptr_t)((intptr_t)(link_good)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_result_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_result_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint32_t *an_result) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_result_get(&mss, an_result);

  bf_aw_trace("pmd_anlt_auto_neg_result_get ", dev_id, dev_port, ln, 1,
              "  an_result",
              *(uint32_t *)(intptr_t)((intptr_t)(an_result)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_page_rx_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_anlt_auto_neg_page_rx_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *an_mr_page_rx,
                                    uint64_t *an_rx_link_code_word) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_page_rx_get(&mss, an_mr_page_rx,
                                                  an_rx_link_code_word);

  bf_aw_trace(
      "pmd_anlt_auto_neg_page_rx_get", dev_id, dev_port, ln, 2,
      " an_mr_page_rx",
      *(uint32_t *)(intptr_t)((intptr_t)(an_mr_page_rx)&UINTMAX_MAX),
      " an_rx_link_code_word",
      *(uint32_t *)(intptr_t)((intptr_t)(an_rx_link_code_word)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_next_page_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_next_page_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint64_t an_tx_np) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_auto_neg_next_page_set", dev_id, dev_port, ln, 1,
              " an_tx_np", (uint32_t)((intptr_t)an_tx_np & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_next_page_set(&mss, an_tx_np);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_newdef_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_newdef_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               aw_an_spec_t *newdef_get) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_newdef_get(&mss, newdef_get);

  bf_aw_trace("pmd_anlt_auto_neg_newdef_get", dev_id, dev_port, ln, 1,
              " newdef_get",
              *(uint32_t *)(intptr_t)((intptr_t)(newdef_get)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_rs_fec_int_ena_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_anlt_auto_neg_rs_fec_int_ena_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *an_rs_fec_int_ena) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_rs_fec_int_ena_get(&mss,
                                                         an_rs_fec_int_ena);

  bf_aw_trace(
      "pmd_anlt_auto_neg_rs_fec_int_ena_get ", dev_id, dev_port, ln, 1,
      "   an_rs_fec_int_ena",
      *(uint32_t *)(intptr_t)((intptr_t)(an_rs_fec_int_ena)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_auto_neg_next_page_oui_compare_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_auto_neg_next_page_oui_compare_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t np_expected_oui) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_auto_neg_next_page_oui_compare_set", dev_id, dev_port,
              ln, 1, " np_expected_oui",
              (uint32_t)((intptr_t)np_expected_oui & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_auto_neg_next_page_oui_compare_set(
      &mss, np_expected_oui);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_en_set(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_link_training_en_set ", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_en_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_en_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_en_get(&mss, en);

  bf_aw_trace("pmd_anlt_link_training_en_get ", dev_id, dev_port, ln, 1, "  en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_config_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t width,
                                                    uint32_t clause) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_link_training_config_set ", dev_id, dev_port, ln, 2,
              " width", (uint32_t)((intptr_t)width & 0xffffffff), " clause",
              (uint32_t)((intptr_t)clause & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_config_set(&mss, width, clause);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_config_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    uint32_t *width,
                                                    uint32_t *clause) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_config_get(&mss, width, clause);

  bf_aw_trace("pmd_anlt_link_training_config_get ", dev_id, dev_port, ln, 2,
              "  width", *(uint32_t *)(intptr_t)((intptr_t)(width)&UINTMAX_MAX),
              "  clause",
              *(uint32_t *)(intptr_t)((intptr_t)(clause)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_prbs_seed_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_prbs_seed_set(bf_dev_id_t dev_id,
                                                       bf_dev_port_t dev_port,
                                                       uint32_t ln,
                                                       uint32_t clause,
                                                       uint32_t logical_lane) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_link_training_prbs_seed_set ", dev_id, dev_port, ln, 2,
              " clause", (uint32_t)((intptr_t)clause & 0xffffffff),
              " logical_lane", (uint32_t)((intptr_t)logical_lane & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_prbs_seed_set(&mss, clause,
                                                         logical_lane);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_start_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_start_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t start) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_link_training_start_set ", dev_id, dev_port, ln, 1,
              " start", (uint32_t)((intptr_t)start & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_start_set(&mss, start);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_start_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_start_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint32_t *start) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_start_get(&mss, start);

  bf_aw_trace("pmd_anlt_link_training_start_get ", dev_id, dev_port, ln, 1,
              "  start",
              *(uint32_t *)(intptr_t)((intptr_t)(start)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_status_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_status_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *lt_running, uint32_t *lt_done, uint32_t *lt_training_failure,
    uint32_t *lt_rx_ready) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_status_get(
      &mss, lt_running, lt_done, lt_training_failure, lt_rx_ready);

  bf_aw_trace(
      "pmd_anlt_link_training_status_get ", dev_id, dev_port, ln, 4,
      "  lt_running",
      *(uint32_t *)(intptr_t)((intptr_t)(lt_running)&UINTMAX_MAX), "  lt_done",
      *(uint32_t *)(intptr_t)((intptr_t)(lt_done)&UINTMAX_MAX),
      "  lt_training_failure",
      *(uint32_t *)(intptr_t)((intptr_t)(lt_training_failure)&UINTMAX_MAX),
      "  lt_rx_ready",
      *(uint32_t *)(intptr_t)((intptr_t)(lt_rx_ready)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_anlt_link_training_timeout_enable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_anlt_link_training_timeout_enable_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_anlt_link_training_timeout_enable_set ", dev_id, dev_port,
              ln, 1, " enable", (uint32_t)((intptr_t)enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_anlt_link_training_timeout_enable_set(&mss, enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_ctrl_map_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_ctrl_map_en_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t anlt_ctrl_map_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_ctrl_map_en_set", dev_id, dev_port, ln, 1,
              " anlt_ctrl_map_en",
              (uint32_t)((intptr_t)anlt_ctrl_map_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_ctrl_map_en_set(&mss, anlt_ctrl_map_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_refclk_termination_set
 ***********************************************************************/
bf_status_t
bf_aw_pmd_refclk_termination_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln,
                                 aw_refclk_term_mode_t lsrefbuf_term_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_refclk_termination_set", dev_id, dev_port, ln, 1,
              " lsrefbuf_term_mode",
              (uint32_t)((intptr_t)lsrefbuf_term_mode & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_refclk_termination_set(&mss, lsrefbuf_term_mode);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_refclk_termination_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_refclk_termination_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln,
                                 aw_refclk_term_mode_t *lsrefbuf_term_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_refclk_termination_get(&mss, lsrefbuf_term_mode);

  bf_aw_trace(
      "pmd_refclk_termination_get", dev_id, dev_port, ln, 1,
      " lsrefbuf_term_mode",
      *(uint32_t *)(intptr_t)((intptr_t)(lsrefbuf_term_mode)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_termination_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_termination_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         aw_acc_term_mode_t acc_term_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_termination_set", dev_id, dev_port, ln, 1,
              " acc_term_mode",
              (uint32_t)((intptr_t)acc_term_mode & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_termination_set(&mss, acc_term_mode);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_termination_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_termination_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         aw_acc_term_mode_t *acc_term_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_termination_get(&mss, acc_term_mode);

  bf_aw_trace("pmd_rx_termination_get", dev_id, dev_port, ln, 1,
              " acc_term_mode",
              *(uint32_t *)(intptr_t)((intptr_t)(acc_term_mode)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_force_signal_detect_config_set
 ***********************************************************************/
bf_status_t
bf_aw_pmd_force_signal_detect_config_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         aw_force_sigdet_mode_t sigdet_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_force_signal_detect_config_set", dev_id, dev_port, ln, 1,
              " sigdet_mode", (uint32_t)((intptr_t)sigdet_mode & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_force_signal_detect_config_set(&mss, sigdet_mode);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_force_signal_detect_config_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_force_signal_detect_config_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         aw_force_sigdet_mode_t *sigdet_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_force_signal_detect_config_get(&mss, sigdet_mode);

  bf_aw_trace("pmd_force_signal_detect_config_get", dev_id, dev_port, ln, 1,
              " sigdet_mode",
              *(uint32_t *)(intptr_t)((intptr_t)(sigdet_mode)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_disable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_disable_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t tx_disable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_disable_set", dev_id, dev_port, ln, 1, " tx_disable",
              (uint32_t)((intptr_t)tx_disable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_disable_set(&mss, tx_disable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_disable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_disable_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t *tx_disable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_disable_get(&mss, tx_disable);

  bf_aw_trace("pmd_tx_disable_get", dev_id, dev_port, ln, 1, " tx_disable",
              *(uint32_t *)(intptr_t)((intptr_t)(tx_disable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_txfir_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_txfir_config_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       aw_txfir_config_t txfir_cfg,
                                       uint32_t fir_ovr_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_txfir_config_set", dev_id, dev_port, ln, 7, "cm3",
              txfir_cfg.CM3, "cm2", txfir_cfg.CM2, "cm1", txfir_cfg.CM1, "c0",
              txfir_cfg.C0, "c1", txfir_cfg.C1, "main_or_max",
              txfir_cfg.main_or_max, " fir_ovr_enable",
              (uint32_t)((intptr_t)fir_ovr_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_txfir_config_set(&mss, txfir_cfg, fir_ovr_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_txfir_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_txfir_config_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       aw_txfir_config_t *txfir_cfg) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_txfir_config_get(&mss, txfir_cfg);

  bf_aw_trace("pmd_txfir_config_get", dev_id, dev_port, ln, 6, "cm3",
              txfir_cfg->CM3, "cm2", txfir_cfg->CM2, "cm1", txfir_cfg->CM1,
              "c0", txfir_cfg->C0, "c1", txfir_cfg->C1, "main_or_max",
              txfir_cfg->main_or_max);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_tap_mode_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_tx_tap_mode_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                          uint32_t ln, uint32_t *max_rng_cm3,
                          uint32_t *max_rng_cm2, uint32_t *max_rng_cm1,
                          uint32_t *max_rng_c1, uint32_t *max_rng_c0) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_tap_mode_get(max_rng_cm3, max_rng_cm2, max_rng_cm1,
                                        max_rng_c1, max_rng_c0);

  bf_aw_trace("pmd_tx_tap_mode_get", dev_id, dev_port, ln, 5, "max_rng_cm3",
              *(uint32_t *)(intptr_t)((intptr_t)(max_rng_cm3)&UINTMAX_MAX),
              " max_rng_cm2",
              *(uint32_t *)(intptr_t)((intptr_t)(max_rng_cm2)&UINTMAX_MAX),
              " max_rng_cm1",
              *(uint32_t *)(intptr_t)((intptr_t)(max_rng_cm1)&UINTMAX_MAX),
              " max_rng_c1",
              *(uint32_t *)(intptr_t)((intptr_t)(max_rng_c1)&UINTMAX_MAX),
              " max_rng_c0",
              *(uint32_t *)(intptr_t)((intptr_t)(max_rng_c0)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_pam4_precoder_override_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_pam4_precoder_override_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_pam4_precoder_override_set", dev_id, dev_port, ln, 1,
              " en", (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_pam4_precoder_override_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_pam4_precoder_override_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_pam4_precoder_override_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_pam4_precoder_override_get(&mss, en);

  bf_aw_trace("pmd_tx_pam4_precoder_override_get", dev_id, dev_port, ln, 1,
              " en", *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_pam4_precoder_enable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_pam4_precoder_enable_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln, uint32_t gray_en,
                                                  uint32_t plusd_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_pam4_precoder_enable_set", dev_id, dev_port, ln, 2,
              " gray_en", (uint32_t)((intptr_t)gray_en & 0xffffffff),
              " plusd_en", (uint32_t)((intptr_t)plusd_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_pam4_precoder_enable_set(&mss, gray_en, plusd_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_pam4_precoder_enable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_pam4_precoder_enable_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *gray_en,
                                                  uint32_t *plusd_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_pam4_precoder_enable_get(&mss, gray_en, plusd_en);

  bf_aw_trace(
      "pmd_tx_pam4_precoder_enable_get", dev_id, dev_port, ln, 2, " gray_en",
      *(uint32_t *)(intptr_t)((intptr_t)(gray_en)&UINTMAX_MAX), " plusd_en",
      *(uint32_t *)(intptr_t)((intptr_t)(plusd_en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_pam4_precoder_override_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_pam4_precoder_override_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_pam4_precoder_override_set", dev_id, dev_port, ln, 1,
              " en", (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_pam4_precoder_override_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_pam4_precoder_override_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_pam4_precoder_override_get(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_pam4_precoder_override_get(&mss, en);

  bf_aw_trace("pmd_rx_pam4_precoder_override_get", dev_id, dev_port, ln, 1,
              " en", *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_pam4_precoder_enable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_pam4_precoder_enable_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln, uint32_t gray_en,
                                                  uint32_t plusd_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_pam4_precoder_enable_set", dev_id, dev_port, ln, 2,
              " gray_en", (uint32_t)((intptr_t)gray_en & 0xffffffff),
              " plusd_en", (uint32_t)((intptr_t)plusd_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_pam4_precoder_enable_set(&mss, gray_en, plusd_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_pam4_precoder_enable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_pam4_precoder_enable_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *gray_en,
                                                  uint32_t *plusd_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_pam4_precoder_enable_get(&mss, gray_en, plusd_en);

  bf_aw_trace(
      "pmd_rx_pam4_precoder_enable_get", dev_id, dev_port, ln, 2, " gray_en",
      *(uint32_t *)(intptr_t)((intptr_t)(gray_en)&UINTMAX_MAX), " plusd_en",
      *(uint32_t *)(intptr_t)((intptr_t)(plusd_en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_remote_loopback_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_remote_loopback_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t remote_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_remote_loopback_set", dev_id, dev_port, ln, 1,
              " remote_loopback_enable",
              (uint32_t)((intptr_t)remote_loopback_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_remote_loopback_set(&mss, remote_loopback_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_remote_loopback_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_remote_loopback_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t *remote_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_remote_loopback_get(&mss, remote_loopback_enable);

  bf_aw_trace(
      "pmd_remote_loopback_get", dev_id, dev_port, ln, 1,
      " remote_loopback_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(remote_loopback_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fep_data_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_fep_data_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, uint32_t datapath_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_fep_data_set", dev_id, dev_port, ln, 1, " datapath_en",
              (uint32_t)((intptr_t)datapath_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_fep_data_set(&mss, datapath_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fep_data_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_fep_data_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, uint32_t *datapath_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_fep_data_get(&mss, datapath_en);

  bf_aw_trace("pmd_fep_data_get", dev_id, dev_port, ln, 1, " datapath_en",
              *(uint32_t *)(intptr_t)((intptr_t)(datapath_en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_dcd_iq_cal
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_dcd_iq_cal(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t enable_d) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_dcd_iq_cal(&mss, enable_d);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fep_clock_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_fep_clock_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint8_t clock_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_fep_clock_set", dev_id, dev_port, ln, 1, " clock_en",
              (uint32_t)((intptr_t)clock_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_fep_clock_set(&mss, clock_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_postdiv_loopback_ena_set
 ***********************************************************************/
bf_status_t
bf_aw_pmd_tx_postdiv_loopback_ena_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint8_t postdiv_loopback_ena) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_postdiv_loopback_ena_set", dev_id, dev_port, ln, 1,
              " postdiv_loopback_ena",
              (uint32_t)((intptr_t)postdiv_loopback_ena & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_postdiv_loopback_ena_set(&mss, postdiv_loopback_ena);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_postdiv_loopback_ena_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_tx_postdiv_loopback_ena_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint8_t *postdiv_loopback_ena) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_postdiv_loopback_ena_get(&mss, postdiv_loopback_ena);

  bf_aw_trace(
      "pmd_tx_postdiv_loopback_ena_get", dev_id, dev_port, ln, 1,
      " postdiv_loopback_ena",
      *(uint32_t *)(intptr_t)((intptr_t)(postdiv_loopback_ena)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fep_clock_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_fep_clock_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *clock_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_fep_clock_get(&mss, clock_en);

  bf_aw_trace("pmd_fep_clock_get", dev_id, dev_port, ln, 1, " clock_en",
              *(uint32_t *)(intptr_t)((intptr_t)(clock_en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_analog_loopback_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_analog_loopback_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t analog_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_analog_loopback_set", dev_id, dev_port, ln, 1,
              " analog_loopback_enable",
              (uint32_t)((intptr_t)analog_loopback_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_analog_loopback_set(&mss, analog_loopback_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_analog_loopback_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_analog_loopback_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t *analog_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_analog_loopback_get(&mss, analog_loopback_enable);

  bf_aw_trace(
      "pmd_analog_loopback_get", dev_id, dev_port, ln, 1,
      " analog_loopback_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(analog_loopback_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fes_loopback_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_fes_loopback_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t fes_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_fes_loopback_set", dev_id, dev_port, ln, 1,
              " fes_loopback_enable",
              (uint32_t)((intptr_t)fes_loopback_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_fes_loopback_set(&mss, fes_loopback_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fes_loopback_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_fes_loopback_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *fes_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_fes_loopback_get(&mss, fes_loopback_enable);

  bf_aw_trace(
      "pmd_fes_loopback_get", dev_id, dev_port, ln, 1, " fes_loopback_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(fes_loopback_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_nep_loopback_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_nep_loopback_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t nep_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_nep_loopback_set", dev_id, dev_port, ln, 1,
              " nep_loopback_enable",
              (uint32_t)((intptr_t)nep_loopback_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_nep_loopback_set(&mss, nep_loopback_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_nep_loopback_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_nep_loopback_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *nep_loopback_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_nep_loopback_get(&mss, nep_loopback_enable);

  bf_aw_trace(
      "pmd_nep_loopback_get", dev_id, dev_port, ln, 1, " nep_loopback_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(nep_loopback_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_polarity_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_polarity_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t tx_pol_flip) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_polarity_set", dev_id, dev_port, ln, 1, " tx_pol_flip",
              (uint32_t)((intptr_t)tx_pol_flip & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_polarity_set(&mss, tx_pol_flip);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_polarity_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_polarity_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *tx_pol_flip) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_polarity_get(&mss, tx_pol_flip);

  bf_aw_trace("pmd_tx_polarity_get", dev_id, dev_port, ln, 1, " tx_pol_flip",
              *(uint32_t *)(intptr_t)((intptr_t)(tx_pol_flip)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_polarity_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_polarity_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t rx_pol_flip) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_polarity_set", dev_id, dev_port, ln, 1, " rx_pol_flip",
              (uint32_t)((intptr_t)rx_pol_flip & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_polarity_set(&mss, rx_pol_flip);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_polarity_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_polarity_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *rx_pol_flip) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_polarity_get(&mss, rx_pol_flip);

  bf_aw_trace("pmd_rx_polarity_get", dev_id, dev_port, ln, 1, " rx_pol_flip",
              *(uint32_t *)(intptr_t)((intptr_t)(rx_pol_flip)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_hbridge_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_hbridge_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, tx_hbridge_t *tx_hbridge_st) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_hbridge_set", dev_id, dev_port, ln, 1, " tx_hbridge_st",
              (uint32_t)((intptr_t)tx_hbridge_st & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_hbridge_set(&mss, tx_hbridge_st);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_hbridge_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_hbridge_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t *msb,
                                     uint32_t *lsb) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_hbridge_get(&mss, msb, lsb);

  bf_aw_trace("pmd_tx_hbridge_get", dev_id, dev_port, ln, 2, " msb",
              *(uint32_t *)(intptr_t)((intptr_t)(msb)&UINTMAX_MAX), " lsb",
              *(uint32_t *)(intptr_t)((intptr_t)(lsb)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dfe_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dfe_adapt_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t dfe_adapt_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_dfe_adapt_set", dev_id, dev_port, ln, 1,
              " dfe_adapt_enable",
              (uint32_t)((intptr_t)dfe_adapt_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dfe_adapt_set(&mss, dfe_adapt_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dfe_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dfe_adapt_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *dfe_adapt_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dfe_adapt_get(&mss, dfe_adapt_enable);

  bf_aw_trace(
      "pmd_rx_dfe_adapt_get", dev_id, dev_port, ln, 1, " dfe_adapt_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(dfe_adapt_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_ctle_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_ctle_adapt_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t ctle_adapt_enable,
                                        uint32_t ctle_boost_a) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_ctle_adapt_set", dev_id, dev_port, ln, 2,
              " ctle_adapt_enable",
              (uint32_t)((intptr_t)ctle_adapt_enable & 0xffffffff),
              " ctle_boost_a", (uint32_t)((intptr_t)ctle_boost_a & 0xffffffff));

  // Call thru API vector
  rc =
      tf3_sd->api->pmd_rx_ctle_adapt_set(&mss, ctle_adapt_enable, ctle_boost_a);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_ctle_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_ctle_adapt_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *ctle_adapt_enable,
                                        uint32_t *ctle_boost_a) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc =
      tf3_sd->api->pmd_rx_ctle_adapt_get(&mss, ctle_adapt_enable, ctle_boost_a);

  bf_aw_trace(
      "pmd_rx_ctle_adapt_get", dev_id, dev_port, ln, 2, " ctle_adapt_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(ctle_adapt_enable)&UINTMAX_MAX),
      " ctle_boost_a",
      *(uint32_t *)(intptr_t)((intptr_t)(ctle_boost_a)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_background_adapt_enable_set
 ***********************************************************************/
bf_status_t
bf_aw_pmd_rx_background_adapt_enable_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint8_t rx_background_adapt) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_background_adapt_enable_set", dev_id, dev_port, ln, 1,
              " rx_background_adapt",
              (uint32_t)((intptr_t)rx_background_adapt & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_background_adapt_enable_set(&mss,
                                                       rx_background_adapt);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_background_adapt_enable_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_rx_background_adapt_enable_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t *rx_bkgrnd_adapt_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_background_adapt_enable_get(&mss,
                                                       rx_bkgrnd_adapt_enable);

  bf_aw_trace(
      "pmd_rx_background_adapt_enable_get", dev_id, dev_port, ln, 1,
      " rx_bkgrnd_adapt_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(rx_bkgrnd_adapt_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_autoeq_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_autoeq_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t rx_autoeq_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_autoeq_set", dev_id, dev_port, ln, 1, " rx_autoeq_enable",
              (uint32_t)((intptr_t)rx_autoeq_enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_autoeq_set(&mss, rx_autoeq_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_autoeq_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_autoeq_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *rx_autoeq_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_autoeq_get(&mss, rx_autoeq_enable);

  bf_aw_trace(
      "pmd_rx_autoeq_get", dev_id, dev_port, ln, 1, " rx_autoeq_enable",
      *(uint32_t *)(intptr_t)((intptr_t)(rx_autoeq_enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_ffe_tap_count_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_ffe_tap_count_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t tap_count) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_ffe_tap_count_set", dev_id, dev_port, ln, 1, " tap_count",
              (uint32_t)((intptr_t)tap_count & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_ffe_tap_count_set(&mss, tap_count);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_ffe_tap_count_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_ffe_tap_count_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *tap_count) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_ffe_tap_count_get(&mss, tap_count);

  bf_aw_trace("pmd_rx_ffe_tap_count_get", dev_id, dev_port, ln, 1,
              "  tap_count",
              *(uint32_t *)(intptr_t)((intptr_t)(tap_count)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_roaming_windows_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_roaming_windows_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    aw_rx_roaming_mode_t mode, uint8_t window_select1, uint8_t window_select2) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_roaming_windows_set", dev_id, dev_port, ln, 3, " mode",
              (uint32_t)((intptr_t)mode & 0xffffffff), " window_select1",
              (uint32_t)((intptr_t)window_select1 & 0xffffffff),
              " window_select2",
              (uint32_t)((intptr_t)window_select2 & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_roaming_windows_set(&mss, mode, window_select1,
                                               window_select2);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_roaming_windows_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_roaming_windows_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             aw_rx_roaming_mode_t *mode,
                                             uint8_t *window_select1,
                                             uint8_t *window_select2) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_roaming_windows_get(&mss, mode, window_select1,
                                               window_select2);

  bf_aw_trace("pmd_rx_roaming_windows_get", dev_id, dev_port, ln, 3, " mode",
              *(uint32_t *)(intptr_t)((intptr_t)(mode)&UINTMAX_MAX),
              " window_select1",
              *(uint32_t *)(intptr_t)((intptr_t)(window_select1)&UINTMAX_MAX),
              " window_select2",
              *(uint32_t *)(intptr_t)((intptr_t)(window_select2)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_vga_cap_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_vga_cap_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t vga_cap) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_vga_cap_set", dev_id, dev_port, ln, 1, " vga_cap",
              (uint32_t)((intptr_t)vga_cap & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_vga_cap_set(&mss, vga_cap);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_vga_cap_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_vga_cap_adapt_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           vga_opt_t *opts) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_vga_cap_adapt_set", dev_id, dev_port, ln, 1, " opts",
              (uint32_t)((intptr_t)opts & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_vga_cap_adapt_set(&mss, opts);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_vga_cap_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_vga_cap_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t *vga_cap) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_vga_cap_get(&mss, vga_cap);

  bf_aw_trace("pmd_rx_vga_cap_get", dev_id, dev_port, ln, 1, " vga_cap",
              *(uint32_t *)(intptr_t)((intptr_t)(vga_cap)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_vga_cap_adapt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_vga_cap_adapt_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           vga_opt_t *opts) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_vga_cap_adapt_get(&mss, opts);

  bf_aw_trace("pmd_rx_vga_cap_adapt_get", dev_id, dev_port, ln, 1, " opts",
              *(uint32_t *)(intptr_t)((intptr_t)(opts)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rxeq_prbs_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rxeq_prbs_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t prbs_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rxeq_prbs_set", dev_id, dev_port, ln, 1, " prbs_en",
              (uint32_t)((intptr_t)prbs_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rxeq_prbs_set(&mss, prbs_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rxeq_prbs_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rxeq_prbs_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *prbs_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rxeq_prbs_get(&mss, prbs_en);

  bf_aw_trace("pmd_rxeq_prbs_get", dev_id, dev_port, ln, 1, " prbs_en",
              *(uint32_t *)(intptr_t)((intptr_t)(prbs_en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_c0_adapt_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_c0_adapt_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_c0_adapt_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_c0_adapt_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_cdr_offset_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_cdr_offset_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t use_custom_cdr_offset,
                                        uint32_t cdr_offset, uint32_t cdr_dir) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_cdr_offset_set", dev_id, dev_port, ln, 3,
              " use_custom_cdr_offset",
              (uint32_t)((intptr_t)use_custom_cdr_offset & 0xffffffff),
              " cdr_offset", (uint32_t)((intptr_t)cdr_offset & 0xffffffff),
              " cdr_dir", (uint32_t)((intptr_t)cdr_dir & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_cdr_offset_set(&mss, use_custom_cdr_offset,
                                          cdr_offset, cdr_dir);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_signal_detect_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_signal_detect_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *signal_detect) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_signal_detect_get(&mss, signal_detect);

  bf_aw_trace("pmd_rx_signal_detect_get", dev_id, dev_port, ln, 1,
              " signal_detect",
              *(uint32_t *)(intptr_t)((intptr_t)(signal_detect)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_signal_detect_check
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_signal_detect_check(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t signal_detect_expected) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_signal_detect_check(&mss, signal_detect_expected);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_ppm_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_ppm_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, uint32_t timing_window,
                                 uint32_t timeout_us, double *tx_ppm,
                                 double *vco_freq, double refclk_freq) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_ppm_get(&mss, timing_window, timeout_us, tx_ppm,
                                   vco_freq, refclk_freq);

  bf_aw_trace("pmd_tx_ppm_get", dev_id, dev_port, ln, 5, " timing_window",
              (uint32_t)((intptr_t)timing_window & 0xffffffff), " timeout_us",
              (uint32_t)((intptr_t)timeout_us & 0xffffffff), " tx_ppm",
              (uint32_t)((intptr_t)tx_ppm & 0xffffffff), " vco_freq",
              (uint32_t)((intptr_t)vco_freq & 0xffffffff), " refclk_freq",
              (uint32_t)((intptr_t)refclk_freq & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_ppm_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_ppm_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, uint32_t timing_window,
                                 uint32_t timeout_us, double *rx_ppm,
                                 double *vco_freq, double refclk_freq) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_ppm_get(&mss, timing_window, timeout_us, rx_ppm,
                                   vco_freq, refclk_freq);

  bf_aw_trace("pmd_rx_ppm_get", dev_id, dev_port, ln, 5, " timing_window",
              (uint32_t)((intptr_t)timing_window & 0xffffffff), " timeout_us",
              (uint32_t)((intptr_t)timeout_us & 0xffffffff), " rx_ppm",
              (uint32_t)((intptr_t)rx_ppm & 0xffffffff), " vco_freq",
              (uint32_t)((intptr_t)vco_freq & 0xffffffff), " refclk_freq",
              (uint32_t)((intptr_t)refclk_freq & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_lock_status_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_lock_status_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t *pmd_rx_lock) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_lock_status_get(&mss, pmd_rx_lock);

  bf_aw_trace("pmd_rx_lock_status_get", dev_id, dev_port, ln, 1, " pmd_rx_lock",
              *(uint32_t *)(intptr_t)((intptr_t)(pmd_rx_lock)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dcdiq_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dcdiq_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln,
                                   aw_dcdiq_data_t *rx_dcdiq_data) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dcdiq_get(&mss, rx_dcdiq_data);

  bf_aw_trace("pmd_rx_dcdiq_get", dev_id, dev_port, ln, 1, " rx_dcdiq_data",
              *(uint32_t *)(intptr_t)((intptr_t)(rx_dcdiq_data)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_dcdiq_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_dcdiq_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln,
                                   aw_dcdiq_data_t *tx_dcdiq_data) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_dcdiq_get(&mss, tx_dcdiq_data);

  bf_aw_trace("pmd_tx_dcdiq_get", dev_id, dev_port, ln, 1, " tx_dcdiq_data",
              *(uint32_t *)(intptr_t)((intptr_t)(tx_dcdiq_data)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_afe_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_afe_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, aw_afe_data_t *rx_afe_data) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_afe_get(&mss, rx_afe_data);

  bf_aw_trace("pmd_rx_afe_get", dev_id, dev_port, ln, 1, " rx_afe_data",
              *(uint32_t *)(intptr_t)((intptr_t)(rx_afe_data)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_invert_datapath
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_invert_datapath(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         char modulation_mode[],
                                         uint32_t gray_code_en,
                                         uint32_t invert_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_invert_datapath(&mss, modulation_mode, gray_code_en,
                                           invert_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_invert_datapath
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_invert_datapath(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         char modulation_mode[],
                                         uint32_t gray_code_en,
                                         uint32_t invert_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_invert_datapath(&mss, modulation_mode, gray_code_en,
                                           invert_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_chk_config_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        aw_bist_pattern_t pattern,
                                        aw_bist_mode_t mode, uint64_t udp,
                                        uint32_t lock_thresh,
                                        uint32_t timer_thresh) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_chk_config_set", dev_id, dev_port, ln, 5, " pattern",
              (uint32_t)((intptr_t)pattern & 0xffffffff), " mode",
              (uint32_t)((intptr_t)mode & 0xffffffff), " udp",
              (uint32_t)((intptr_t)udp & 0xffffffff), " lock_thresh",
              (uint32_t)((intptr_t)lock_thresh & 0xffffffff), " timer_thresh",
              (uint32_t)((intptr_t)timer_thresh & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_config_set(&mss, pattern, mode, udp, lock_thresh,
                                          timer_thresh);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_chk_config_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        aw_bist_pattern_t *pattern,
                                        aw_bist_mode_t *mode, uint64_t *udp,
                                        uint32_t *lock_thresh,
                                        uint32_t *timer_thresh) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_config_get(&mss, pattern, mode, udp, lock_thresh,
                                          timer_thresh);

  bf_aw_trace("pmd_rx_chk_config_get", dev_id, dev_port, ln, 5, " pattern",
              *(uint32_t *)(intptr_t)((intptr_t)(pattern)&UINTMAX_MAX), " mode",
              *(uint32_t *)(intptr_t)((intptr_t)(mode)&UINTMAX_MAX), " udp",
              *(uint32_t *)(intptr_t)((intptr_t)(udp)&UINTMAX_MAX),
              " lock_thresh",
              *(uint32_t *)(intptr_t)((intptr_t)(lock_thresh)&UINTMAX_MAX),
              " timer_thresh",
              *(uint32_t *)(intptr_t)((intptr_t)(timer_thresh)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_chk_en_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_chk_en_set", dev_id, dev_port, ln, 1, " enable",
              (uint32_t)((intptr_t)enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_en_set(&mss, enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_chk_en_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_en_get(&mss, enable);

  bf_aw_trace("pmd_rx_chk_en_get", dev_id, dev_port, ln, 1, " enable",
              *(uint32_t *)(intptr_t)((intptr_t)(enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_lock_state_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_chk_lock_state_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *rx_bist_lock) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_lock_state_get(&mss, rx_bist_lock);

  bf_aw_trace("pmd_rx_chk_lock_state_get", dev_id, dev_port, ln, 1,
              " rx_bist_lock",
              *(uint32_t *)(intptr_t)((intptr_t)(rx_bist_lock)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_err_count_state_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_rx_chk_err_count_state_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint64_t *err_count,
                                     uint32_t *err_count_done,
                                     uint32_t *err_count_overflown) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_err_count_state_get(
      &mss, err_count, err_count_done, err_count_overflown);

  bf_aw_trace(
      "pmd_rx_chk_err_count_state_get", dev_id, dev_port, ln, 3, " err_count",
      *(uint32_t *)(intptr_t)((intptr_t)(err_count)&UINTMAX_MAX),
      " err_count_done",
      *(uint32_t *)(intptr_t)((intptr_t)(err_count_done)&UINTMAX_MAX),
      " err_count_overflown",
      *(uint32_t *)(intptr_t)((intptr_t)(err_count_overflown)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_chk_err_count_state_clear
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_chk_err_count_state_clear(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_chk_err_count_state_clear(&mss);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_config_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        aw_bist_pattern_t pattern,
                                        uint64_t udp) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_gen_config_set", dev_id, dev_port, ln, 2, " pattern",
              (uint32_t)((intptr_t)pattern & 0xffffffff), " udp",
              (uint32_t)((intptr_t)udp & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_config_set(&mss, pattern, udp);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_config_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        aw_bist_pattern_t *pattern,
                                        uint64_t *udp) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_config_get(&mss, pattern, udp);

  bf_aw_trace("pmd_tx_gen_config_get", dev_id, dev_port, ln, 2, " pattern",
              *(uint32_t *)(intptr_t)((intptr_t)(pattern)&UINTMAX_MAX), " udp",
              *(uint32_t *)(intptr_t)((intptr_t)(udp)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_en_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_gen_en_set", dev_id, dev_port, ln, 1, " enable",
              (uint32_t)((intptr_t)enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_en_set(&mss, enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_en_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_en_get(&mss, enable);

  bf_aw_trace("pmd_tx_gen_en_get", dev_id, dev_port, ln, 1, " enable",
              *(uint32_t *)(intptr_t)((intptr_t)(enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_err_inject_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_err_inject_config_set(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint64_t err_pattern,
                                                   uint32_t err_rate) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_gen_err_inject_config_set", dev_id, dev_port, ln, 2,
              " err_pattern", (uint32_t)((intptr_t)err_pattern & 0xffffffff),
              " err_rate", (uint32_t)((intptr_t)err_rate & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_err_inject_config_set(&mss, err_pattern,
                                                     err_rate);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_err_inject_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_err_inject_config_get(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   uint64_t *err_pattern,
                                                   uint32_t *err_rate) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_err_inject_config_get(&mss, err_pattern,
                                                     err_rate);

  bf_aw_trace("pmd_tx_gen_err_inject_config_get", dev_id, dev_port, ln, 2,
              " err_pattern",
              *(uint32_t *)(intptr_t)((intptr_t)(err_pattern)&UINTMAX_MAX),
              " err_rate",
              *(uint32_t *)(intptr_t)((intptr_t)(err_rate)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_err_inject_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_err_inject_en_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_gen_err_inject_en_set", dev_id, dev_port, ln, 1,
              " enable", (uint32_t)((intptr_t)enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_err_inject_en_set(&mss, enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gen_err_inject_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gen_err_inject_en_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gen_err_inject_en_get(&mss, enable);

  bf_aw_trace("pmd_tx_gen_err_inject_en_get", dev_id, dev_port, ln, 1,
              " enable",
              *(uint32_t *)(intptr_t)((intptr_t)(enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_sweep_demapper
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_sweep_demapper(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t npam4_nrz,
                                        uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_sweep_demapper(&mss, npam4_nrz, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_gen_tx_swap_msb_lsb_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_gen_tx_swap_msb_lsb_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_gen_tx_swap_msb_lsb_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_gen_tx_swap_msb_lsb_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_gen_tx_swap_msb_lsb_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_gen_tx_swap_msb_lsb_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_gen_tx_swap_msb_lsb_get(&mss, en);

  bf_aw_trace("pmd_gen_tx_swap_msb_lsb_get", dev_id, dev_port, ln, 1, " en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_gen_rx_swap_msb_lsb_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_gen_rx_swap_msb_lsb_set(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_gen_rx_swap_msb_lsb_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_gen_rx_swap_msb_lsb_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_gen_rx_swap_msb_lsb_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_gen_rx_swap_msb_lsb_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_gen_rx_swap_msb_lsb_get(&mss, en);

  bf_aw_trace("pmd_gen_rx_swap_msb_lsb_get", dev_id, dev_port, ln, 1, " en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_uc_ucode_load
 ***********************************************************************/
bf_status_t bf_aw_pmd_uc_ucode_load(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, aw_ucode_t *ucode_arr,
                                    uint32_t size) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_uc_ucode_load(&mss, ucode_arr, size);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_pll_lock_max_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_pll_lock_max_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_pll_lock_max_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_pll_lock_max_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_pll_lock_min_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_pll_lock_min_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_pll_lock_min_set", dev_id, dev_port, ln, 1, " val",
              (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_pll_lock_min_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_pll_lock_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_pll_lock_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, uint32_t *pll_lock,
                                   uint32_t check_en, uint32_t expected_val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_pll_lock_get(&mss, pll_lock, check_en, expected_val);

  bf_aw_trace("pmd_pll_lock_get", dev_id, dev_port, ln, 3, " pll_lock",
              (uint32_t)((intptr_t)pll_lock & 0xffffffff), "check_en",
              (uint32_t)((intptr_t)check_en & 0xffffffff), " expected_val",
              (uint32_t)((intptr_t)expected_val & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_uc_diag_reg_dump
 ***********************************************************************/
bf_status_t bf_aw_pmd_uc_diag_reg_dump(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       aw_uc_diag_regs_t *uc_diag) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_uc_diag_reg_dump(&mss, uc_diag);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_uc_diag_logging_en_set
 ***********************************************************************/
bf_status_t
bf_aw_pmd_uc_diag_logging_en_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, uint32_t uc_log_cmn_en,
                                 uint32_t uc_log_tx_en, uint32_t uc_log_rx_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_uc_diag_logging_en_set", dev_id, dev_port, ln, 3,
              " uc_log_cmn_en",
              (uint32_t)((intptr_t)uc_log_cmn_en & 0xffffffff), " uc_log_tx_en",
              (uint32_t)((intptr_t)uc_log_tx_en & 0xffffffff), " uc_log_rx_en",
              (uint32_t)((intptr_t)uc_log_rx_en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_uc_diag_logging_en_set(&mss, uc_log_cmn_en,
                                               uc_log_tx_en, uc_log_rx_en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_uc_diag_logging_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_uc_diag_logging_en_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *uc_log_cmn_en, uint32_t *uc_log_tx_en, uint32_t *uc_log_rx_en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_uc_diag_logging_en_get(&mss, uc_log_cmn_en,
                                               uc_log_tx_en, uc_log_rx_en);

  bf_aw_trace("pmd_uc_diag_logging_en_get", dev_id, dev_port, ln, 3,
              " uc_log_cmn_en",
              *(uint32_t *)(intptr_t)((intptr_t)(uc_log_cmn_en)&UINTMAX_MAX),
              " uc_log_tx_en",
              *(uint32_t *)(intptr_t)((intptr_t)(uc_log_tx_en)&UINTMAX_MAX),
              " uc_log_rx_en",
              *(uint32_t *)(intptr_t)((intptr_t)(uc_log_rx_en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_ref_ls_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_ref_ls_en_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_iso_ref_ls_en_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_ref_ls_en_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_cmn_pstate_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_cmn_pstate_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_iso_cmn_pstate_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_cmn_pstate_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_cmn_pstate_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_cmn_pstate_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_cmn_pstate_get(&mss, value);

  bf_aw_trace("pmd_iso_cmn_pstate_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_cmn_state_req_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_cmn_state_req_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_iso_cmn_state_req_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_cmn_state_req_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_cmn_state_ack_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_cmn_state_ack_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *cmn_state_ack) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_cmn_state_ack_get(&mss, cmn_state_ack);

  bf_aw_trace("pmd_iso_cmn_state_ack_get", dev_id, dev_port, ln, 1,
              " cmn_state_ack",
              *(uint32_t *)(intptr_t)((intptr_t)(cmn_state_ack)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_reset_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_reset_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_iso_tx_reset_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_reset_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_reset_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_reset_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_reset_get(&mss, value);

  bf_aw_trace("pmd_iso_tx_reset_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_reset_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_reset_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_iso_rx_reset_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_reset_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_reset_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_reset_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_reset_get(&mss, value);

  bf_aw_trace("pmd_iso_rx_reset_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_rate_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_rate_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_iso_tx_rate_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_rate_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_rate_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_rate_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_rate_get(&mss, value);

  bf_aw_trace("pmd_iso_tx_rate_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_rate_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_rate_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_iso_rx_rate_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_rate_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_rate_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_rate_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_rate_get(&mss, value);

  bf_aw_trace("pmd_iso_rx_rate_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_pstate_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_pstate_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_iso_tx_pstate_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_pstate_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_pstate_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_pstate_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_pstate_get(&mss, value);

  bf_aw_trace("pmd_iso_tx_pstate_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_pstate_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_pstate_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_iso_rx_pstate_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_pstate_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_pstate_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_pstate_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_pstate_get(&mss, value);

  bf_aw_trace("pmd_iso_rx_pstate_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_width_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_width_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_iso_tx_width_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_width_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_width_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_width_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_width_get(&mss, value);

  bf_aw_trace("pmd_iso_tx_width_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_width_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_width_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_iso_rx_width_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_width_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_width_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_width_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_width_get(&mss, value);

  bf_aw_trace("pmd_iso_rx_width_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_state_req_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_state_req_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_iso_tx_state_req_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_state_req_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_state_req_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_state_req_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_iso_rx_state_req_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_state_req_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_tx_state_ack_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_tx_state_ack_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *tx_state_ack) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_tx_state_ack_get(&mss, tx_state_ack);

  bf_aw_trace("pmd_iso_tx_state_ack_get", dev_id, dev_port, ln, 1,
              " tx_state_ack",
              *(uint32_t *)(intptr_t)((intptr_t)(tx_state_ack)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_rx_state_ack_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_rx_state_ack_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *rx_state_ack) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_rx_state_ack_get(&mss, rx_state_ack);

  bf_aw_trace("pmd_iso_rx_state_ack_get", dev_id, dev_port, ln, 1,
              " rx_state_ack",
              *(uint32_t *)(intptr_t)((intptr_t)(rx_state_ack)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_cmn_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_cmn_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_isolate_cmn_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_cmn_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_cmn_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_cmn_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_cmn_get(&mss, en);

  bf_aw_trace("pmd_isolate_cmn_get", dev_id, dev_port, ln, 1, " en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_lane_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_lane_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_isolate_lane_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_lane_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_lane_tx_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_lane_tx_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_isolate_lane_tx_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_lane_tx_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_lane_rx_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_lane_rx_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_isolate_lane_rx_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_lane_rx_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_lane_txrx_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_lane_txrx_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_isolate_lane_txrx_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_lane_txrx_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_isolate_lane_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_isolate_lane_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_isolate_lane_get(&mss, en);

  bf_aw_trace("pmd_isolate_lane_get", dev_id, dev_port, ln, 1, " en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_r2l_hsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_r2l_hsref_sel_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_r2l_hsref_sel_set", dev_id, dev_port, ln, 1, " sel",
              (uint32_t)((intptr_t)sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_r2l_hsref_sel_set(&mss, sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_r2l0_lsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_r2l0_lsref_sel_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_r2l0_lsref_sel_set", dev_id, dev_port, ln, 1, " sel",
              (uint32_t)((intptr_t)sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_r2l0_lsref_sel_set(&mss, sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_r2l1_lsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_r2l1_lsref_sel_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_r2l1_lsref_sel_set", dev_id, dev_port, ln, 1, " sel",
              (uint32_t)((intptr_t)sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_r2l1_lsref_sel_set(&mss, sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_l2r_hsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_l2r_hsref_sel_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_l2r_hsref_sel_set", dev_id, dev_port, ln, 1, " sel",
              (uint32_t)((intptr_t)sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_l2r_hsref_sel_set(&mss, sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_l2r0_lsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_l2r0_lsref_sel_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_l2r0_lsref_sel_set", dev_id, dev_port, ln, 1, " sel",
              (uint32_t)((intptr_t)sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_l2r0_lsref_sel_set(&mss, sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_l2r1_lsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_l2r1_lsref_sel_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_l2r1_lsref_sel_set", dev_id, dev_port, ln, 1, " sel",
              (uint32_t)((intptr_t)sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_l2r1_lsref_sel_set(&mss, sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_r2l_hsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_r2l_hsref_sel_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_r2l_hsref_sel_get(&mss, sel);

  bf_aw_trace("pmd_cmn_r2l_hsref_sel_get", dev_id, dev_port, ln, 1, " sel",
              *(uint32_t *)(intptr_t)((intptr_t)(sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_r2l0_lsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_r2l0_lsref_sel_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_r2l0_lsref_sel_get(&mss, sel);

  bf_aw_trace("pmd_cmn_r2l0_lsref_sel_get", dev_id, dev_port, ln, 1, " sel",
              *(uint32_t *)(intptr_t)((intptr_t)(sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_r2l1_lsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_r2l1_lsref_sel_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_r2l1_lsref_sel_get(&mss, sel);

  bf_aw_trace("pmd_cmn_r2l1_lsref_sel_get", dev_id, dev_port, ln, 1, " sel",
              *(uint32_t *)(intptr_t)((intptr_t)(sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_l2r_hsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_l2r_hsref_sel_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_l2r_hsref_sel_get(&mss, sel);

  bf_aw_trace("pmd_cmn_l2r_hsref_sel_get", dev_id, dev_port, ln, 1, " sel",
              *(uint32_t *)(intptr_t)((intptr_t)(sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_l2r0_lsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_l2r0_lsref_sel_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_l2r0_lsref_sel_get(&mss, sel);

  bf_aw_trace("pmd_cmn_l2r0_lsref_sel_get", dev_id, dev_port, ln, 1, " sel",
              *(uint32_t *)(intptr_t)((intptr_t)(sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_l2r1_lsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_l2r1_lsref_sel_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_l2r1_lsref_sel_get(&mss, sel);

  bf_aw_trace("pmd_cmn_l2r1_lsref_sel_get", dev_id, dev_port, ln, 1, " sel",
              *(uint32_t *)(intptr_t)((intptr_t)(sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_lsref_sel_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_lsref_sel_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t ref_sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_lsref_sel_set", dev_id, dev_port, ln, 1, " ref_sel",
              (uint32_t)((intptr_t)ref_sel & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_lsref_sel_set(&mss, ref_sel);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_lsref_sel_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_lsref_sel_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *ref_sel) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_lsref_sel_get(&mss, ref_sel);

  bf_aw_trace("pmd_cmn_lsref_sel_get", dev_id, dev_port, ln, 1, " ref_sel",
              *(uint32_t *)(intptr_t)((intptr_t)(ref_sel)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_pcie_cmn_lsref_25m_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_pcie_cmn_lsref_25m_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t lsref_25m) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_pcie_cmn_lsref_25m_set", dev_id, dev_port, ln, 1,
              " lsref_25m", (uint32_t)((intptr_t)lsref_25m & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_pcie_cmn_lsref_25m_set(&mss, lsref_25m);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_pcie_cmn_lsref_25m_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_pcie_cmn_lsref_25m_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t *lsref_25m) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_pcie_cmn_lsref_25m_get(&mss, lsref_25m);

  bf_aw_trace("pmd_pcie_cmn_lsref_25m_get", dev_id, dev_port, ln, 1,
              " lsref_25m",
              *(uint32_t *)(intptr_t)((intptr_t)(lsref_25m)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_gen_tx_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_gen_tx_en_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_gen_tx_en_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_gen_tx_en_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_gen_tx_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_gen_tx_en_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_gen_tx_en_get(&mss, value);

  bf_aw_trace("pmd_gen_tx_en_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_error_cnt_done_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_error_cnt_done_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *err_count_done) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_error_cnt_done_get(&mss, err_count_done);

  bf_aw_trace("pmd_rx_error_cnt_done_get", dev_id, dev_port, ln, 1,
              " err_count_done",
              *(uint32_t *)(intptr_t)((intptr_t)(err_count_done)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_request_cmn_state_change
 ***********************************************************************/
bf_status_t bf_aw_pmd_iso_request_cmn_state_change(bf_dev_id_t dev_id,
                                                   bf_dev_port_t dev_port,
                                                   uint32_t ln,
                                                   aw_cmn_pstate_t cmn_pstate,
                                                   uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_request_cmn_state_change(&mss, cmn_pstate,
                                                     timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_request_tx_state_change
 ***********************************************************************/
bf_status_t
bf_aw_pmd_iso_request_tx_state_change(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      aw_pstate_t tx_pstate, uint32_t tx_rate,
                                      uint32_t tx_width, uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_request_tx_state_change(&mss, tx_pstate, tx_rate,
                                                    tx_width, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_iso_request_rx_state_change
 ***********************************************************************/
bf_status_t
bf_aw_pmd_iso_request_rx_state_change(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      aw_pstate_t rx_pstate, uint32_t rx_rate,
                                      uint32_t rx_width, uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_iso_request_rx_state_change(&mss, rx_pstate, rx_rate,
                                                    rx_width, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_check_cdr_lock
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_check_cdr_lock(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_check_cdr_lock(&mss, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_check_bist
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_check_bist(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                    uint32_t ln, aw_bist_mode_t bist_mode,
                                    uint32_t timer_threshold, uint32_t rx_width,
                                    uint32_t timeout_us,
                                    int32_t expected_errors) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_check_bist(&mss, bist_mode, timer_threshold,
                                      rx_width, timeout_us, expected_errors);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_clear
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_clear(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_clear(&mss);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_enable_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_enable_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_enable_get(&mss, enable);

  bf_aw_trace("pmd_rx_prefec_enable_get", dev_id, dev_port, ln, 1, " enable",
              *(uint32_t *)(intptr_t)((intptr_t)(enable)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_enable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_enable_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_prefec_enable_set", dev_id, dev_port, ln, 1, " enable",
              (uint32_t)((intptr_t)enable & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_enable_set(&mss, enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_poll
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_poll(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_poll(&mss, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_config_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *corr_num_syms, uint32_t *symbol_size, uint32_t *wall_mode,
    uint32_t *sym_per_cw, uint32_t *skip_syms, uint32_t *timer_num_cw) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_config_get(&mss, corr_num_syms, symbol_size,
                                             wall_mode, sym_per_cw, skip_syms,
                                             timer_num_cw);

  bf_aw_trace(
      "pmd_rx_prefec_config_get", dev_id, dev_port, ln, 6, " corr_num_syms",
      *(uint32_t *)(intptr_t)((intptr_t)(corr_num_syms)&UINTMAX_MAX),
      " symbol_size",
      *(uint32_t *)(intptr_t)((intptr_t)(symbol_size)&UINTMAX_MAX),
      " wall_mode", *(uint32_t *)(intptr_t)((intptr_t)(wall_mode)&UINTMAX_MAX),
      " sym_per_cw",
      *(uint32_t *)(intptr_t)((intptr_t)(sym_per_cw)&UINTMAX_MAX), " skip_syms",
      *(uint32_t *)(intptr_t)((intptr_t)(skip_syms)&UINTMAX_MAX),
      " timer_num_cw",
      *(uint32_t *)(intptr_t)((intptr_t)(timer_num_cw)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_config_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t corr_num_syms, uint32_t symbol_size, uint32_t wall_mode,
    uint32_t sym_per_cw, uint32_t skip_syms, uint32_t timer_num_cw) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_prefec_config_set", dev_id, dev_port, ln, 6,
              " corr_num_syms",
              (uint32_t)((intptr_t)corr_num_syms & 0xffffffff), " symbol_size",
              (uint32_t)((intptr_t)symbol_size & 0xffffffff), " wall_mode",
              (uint32_t)((intptr_t)wall_mode & 0xffffffff), " sym_per_cw",
              (uint32_t)((intptr_t)sym_per_cw & 0xffffffff), " skip_syms",
              (uint32_t)((intptr_t)skip_syms & 0xffffffff), " timer_num_cw",
              (uint32_t)((intptr_t)timer_num_cw & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_config_set(&mss, corr_num_syms, symbol_size,
                                             wall_mode, sym_per_cw, skip_syms,
                                             timer_num_cw);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_prefec_get_results
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_prefec_get_results(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *hist) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_prefec_get_results(&mss, hist);

  bf_aw_trace("pmd_rx_prefec_get_results", dev_id, dev_port, ln, 1, " hist",
              *(uint32_t *)(intptr_t)((intptr_t)(hist)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_eqeval_type_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_eqeval_type_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t eq_type) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_eqeval_type_set", dev_id, dev_port, ln, 1, " eq_type",
              (uint32_t)((intptr_t)eq_type & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_eqeval_type_set(&mss, eq_type);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_eqeval_req_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_eqeval_req_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_eqeval_req_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_eqeval_req_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_eqeval_ack_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_eqeval_ack_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t *eqeval_ack) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_eqeval_ack_get(&mss, eqeval_ack);

  bf_aw_trace("pmd_eqeval_ack_get", dev_id, dev_port, ln, 1, " eqeval_ack",
              *(uint32_t *)(intptr_t)((intptr_t)(eqeval_ack)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_eqeval_incdec_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_eqeval_incdec_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *incdec) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_eqeval_incdec_get(&mss, incdec);

  bf_aw_trace("pmd_eqeval_incdec_get", dev_id, dev_port, ln, 1, " incdec",
              *(uint32_t *)(intptr_t)((intptr_t)(incdec)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_equalize
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_equalize(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                  uint32_t ln, aw_eq_type_t eq_type,
                                  uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_equalize(&mss, eq_type, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_rxdet_req_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_rxdet_req_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_rxdet_req_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_rxdet_req_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_rxdet
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_rxdet(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                               uint32_t ln, uint32_t rxdet_expected,
                               uint32_t timeout_us) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_rxdet(&mss, rxdet_expected, timeout_us);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_beacon_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_beacon_en_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_beacon_en_set", dev_id, dev_port, ln, 1, " value",
              (uint32_t)((intptr_t)value & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_beacon_en_set(&mss, value);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_beacon_en_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_beacon_en_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *value) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_beacon_en_get(&mss, value);

  bf_aw_trace("pmd_tx_beacon_en_get", dev_id, dev_port, ln, 1, " value",
              *(uint32_t *)(intptr_t)((intptr_t)(value)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_pll_fine_code_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_pll_fine_code_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *tx_pll_fine_code,
                                           uint32_t center_code,
                                           int tolerance) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_pll_fine_code_get(&mss, tx_pll_fine_code,
                                             center_code, tolerance);

  bf_aw_trace("pmd_tx_pll_fine_code_get", dev_id, dev_port, ln, 3,
              "  tx_pll_fine_code",
              (uint32_t)((intptr_t)tx_pll_fine_code & 0xffffffff),
              " center_code", (uint32_t)((intptr_t)center_code & 0xffffffff),
              " tolerance", (uint32_t)((intptr_t)tolerance & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_pll_coarse_code_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_tx_pll_coarse_code_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, uint32_t *tx_pll_coarse_code,
                                 uint32_t center_code, int tolerance) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_pll_coarse_code_get(&mss, tx_pll_coarse_code,
                                               center_code, tolerance);

  bf_aw_trace("pmd_tx_pll_coarse_code_get", dev_id, dev_port, ln, 3,
              "  tx_pll_coarse_code",
              (uint32_t)((intptr_t)tx_pll_coarse_code & 0xffffffff),
              " center_code", (uint32_t)((intptr_t)center_code & 0xffffffff),
              " tolerance", (uint32_t)((intptr_t)tolerance & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_pll_fine_code_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_pll_fine_code_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *rx_pll_fine_code,
                                           uint32_t center_code,
                                           int tolerance) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_pll_fine_code_get(&mss, rx_pll_fine_code,
                                             center_code, tolerance);

  bf_aw_trace("pmd_rx_pll_fine_code_get", dev_id, dev_port, ln, 3,
              "  rx_pll_fine_code",
              (uint32_t)((intptr_t)rx_pll_fine_code & 0xffffffff),
              " center_code", (uint32_t)((intptr_t)center_code & 0xffffffff),
              " tolerance", (uint32_t)((intptr_t)tolerance & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_pll_coarse_code_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_rx_pll_coarse_code_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                 uint32_t ln, uint32_t *rx_pll_coarse_code,
                                 uint32_t center_code, int tolerance) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_pll_coarse_code_get(&mss, rx_pll_coarse_code,
                                               center_code, tolerance);

  bf_aw_trace("pmd_rx_pll_coarse_code_get", dev_id, dev_port, ln, 3,
              "  rx_pll_coarse_code",
              (uint32_t)((intptr_t)rx_pll_coarse_code & 0xffffffff),
              " center_code", (uint32_t)((intptr_t)center_code & 0xffffffff),
              " tolerance", (uint32_t)((intptr_t)tolerance & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_pll_fine_code_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_pll_fine_code_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t *cmn_pll_fine_code,
                                            uint32_t center_code,
                                            int tolerance) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_pll_fine_code_get(&mss, cmn_pll_fine_code,
                                              center_code, tolerance);

  bf_aw_trace("pmd_cmn_pll_fine_code_get", dev_id, dev_port, ln, 3,
              "  cmn_pll_fine_code",
              (uint32_t)((intptr_t)cmn_pll_fine_code & 0xffffffff),
              " center_code", (uint32_t)((intptr_t)center_code & 0xffffffff),
              " tolerance", (uint32_t)((intptr_t)tolerance & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_pll_coarse_code_get
 ***********************************************************************/
bf_status_t
bf_aw_pmd_cmn_pll_coarse_code_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                  uint32_t ln, uint32_t *cmn_pll_coarse_code,
                                  uint32_t center_code, int tolerance) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_pll_coarse_code_get(&mss, cmn_pll_coarse_code,
                                                center_code, tolerance);

  bf_aw_trace("pmd_cmn_pll_coarse_code_get", dev_id, dev_port, ln, 3,
              "  cmn_pll_coarse_code",
              (uint32_t)((intptr_t)cmn_pll_coarse_code & 0xffffffff),
              " center_code", (uint32_t)((intptr_t)center_code & 0xffffffff),
              " tolerance", (uint32_t)((intptr_t)tolerance & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rd_data_pipeline_stages_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rd_data_pipeline_stages_set(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t stages) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rd_data_pipeline_stages_set", dev_id, dev_port, ln, 1,
              " stages", (uint32_t)((intptr_t)stages & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rd_data_pipeline_stages_set(&mss, stages);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rd_data_pipeline_stages_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rd_data_pipeline_stages_get(bf_dev_id_t dev_id,
                                                  bf_dev_port_t dev_port,
                                                  uint32_t ln,
                                                  uint32_t *stages) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rd_data_pipeline_stages_get(&mss, stages);

  bf_aw_trace("pmd_rd_data_pipeline_stages_get", dev_id, dev_port, ln, 1,
              "  stages",
              *(uint32_t *)(intptr_t)((intptr_t)(stages)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_measure_pmon
 ***********************************************************************/
bf_status_t bf_aw_pmd_measure_pmon(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, uint32_t pmon_sel,
                                   uint32_t pvt_measure_timing_window,
                                   uint32_t timeout_us,
                                   uint32_t *pvt_measure_result) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_measure_pmon(&mss, pmon_sel, pvt_measure_timing_window,
                                     timeout_us, pvt_measure_result);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_en
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_en(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                               uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_en(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_cmn_capture
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_cmn_capture(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t atest_addr,
                                        uint32_t atest_term,
                                        uint32_t *atest_adc_val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_cmn_capture(&mss, atest_addr, atest_term,
                                          atest_adc_val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_tx_capture
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_tx_capture(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t atest_addr, uint32_t atest_term,
                                       uint32_t *atest_adc_val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_tx_capture(&mss, atest_addr, atest_term,
                                         atest_adc_val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_rx_a_capture
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_rx_a_capture(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t atest_addr,
                                         uint32_t atest_term,
                                         uint32_t *atest_adc_val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_rx_a_capture(&mss, atest_addr, atest_term,
                                           atest_adc_val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_rx_b_capture
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_rx_b_capture(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         uint32_t atest_addr,
                                         uint32_t atest_term,
                                         uint32_t *atest_adc_val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_rx_b_capture(&mss, atest_addr, atest_term,
                                           atest_adc_val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_adc_power
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_adc_power(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_adc_power(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_adc_temp_capture
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_adc_temp_capture(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln, uint32_t iterations,
                                             aw_adc_temp_data_t *mean_data) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_adc_temp_capture(&mss, iterations, mean_data);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_atest_adc_temp_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_atest_adc_temp_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port, uint32_t ln,
                                         aw_adc_temp_calibration_t *calibration,
                                         aw_adc_temp_method_t method,
                                         uint32_t iterations,
                                         aw_adc_temp_data_t *measured_data,
                                         float *result_temp) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_atest_adc_temp_get(
      &mss, calibration, method, iterations, measured_data, result_temp);

  bf_aw_trace("pmd_atest_adc_temp_get", dev_id, dev_port, ln, 5, " calibration",
              (uint32_t)((intptr_t)calibration & 0xffffffff), " method",
              (uint32_t)((intptr_t)method & 0xffffffff), " iterations",
              (uint32_t)((intptr_t)iterations & 0xffffffff), " measured_data",
              *(uint32_t *)(intptr_t)((intptr_t)(measured_data)&UINTMAX_MAX),
              " result_temp",
              *(uint32_t *)(intptr_t)((intptr_t)(result_temp)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_cdr_lock_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_cdr_lock_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port, uint32_t ln,
                                      uint32_t *rx_cdr_lock) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_cdr_lock_get(&mss, rx_cdr_lock);

  bf_aw_trace("pmd_rx_cdr_lock_get", dev_id, dev_port, ln, 1, " rx_cdr_lock",
              *(uint32_t *)(intptr_t)((intptr_t)(rx_cdr_lock)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_lcpll_vco_counter_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_lcpll_vco_counter_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port, uint32_t ln,
                                            uint32_t timing_window,
                                            double *lcpll_ppm, double *vco_freq,
                                            double refclk_freq) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_lcpll_vco_counter_get(&mss, timing_window, lcpll_ppm,
                                              vco_freq, refclk_freq);

  bf_aw_trace("pmd_lcpll_vco_counter_get", dev_id, dev_port, ln, 4,
              " timing_window",
              (uint32_t)((intptr_t)timing_window & 0xffffffff), " lcpll_ppm",
              (uint32_t)((intptr_t)lcpll_ppm & 0xffffffff), " vco_freq",
              (uint32_t)((intptr_t)vco_freq & 0xffffffff), " refclk_freq",
              (uint32_t)((intptr_t)refclk_freq & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_cdr_offset_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_cdr_offset_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port, uint32_t ln,
                                        uint32_t *use_custom_cdr_offset,
                                        uint32_t *cdr_offset,
                                        uint32_t *cdr_dir) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_cdr_offset_get(&mss, use_custom_cdr_offset,
                                          cdr_offset, cdr_dir);

  bf_aw_trace(
      "pmd_rx_cdr_offset_get", dev_id, dev_port, ln, 3,
      " use_custom_cdr_offset",
      *(uint32_t *)(intptr_t)((intptr_t)(use_custom_cdr_offset)&UINTMAX_MAX),
      " cdr_offset",
      *(uint32_t *)(intptr_t)((intptr_t)(cdr_offset)&UINTMAX_MAX), " cdr_dir",
      *(uint32_t *)(intptr_t)((intptr_t)(cdr_dir)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_status
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_status(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                  uint32_t ln, int branch) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_status(&mss, branch);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_pause_background
 ***********************************************************************/
bf_status_t bf_aw_pmd_pause_background(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t pause_enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_pause_background(&mss, pause_enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tracebuffer_config_enable
 ***********************************************************************/
bf_status_t bf_aw_pmd_tracebuffer_config_enable(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln, uint32_t clk_sel,
                                                uint32_t enable) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tracebuffer_config_enable(&mss, clk_sel, enable);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tracebuffer_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tracebuffer_config_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             aw_pmd_tracebuffer_mode_t *tbmode,
                                             uint32_t *samples_per_cycle) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tracebuffer_config_get(&mss, tbmode, samples_per_cycle);

  bf_aw_trace(
      "pmd_tracebuffer_config_get", dev_id, dev_port, ln, 2, " tbmode",
      *(uint32_t *)(intptr_t)((intptr_t)(tbmode)&UINTMAX_MAX),
      " samples_per_cycle",
      *(uint32_t *)(intptr_t)((intptr_t)(samples_per_cycle)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tracebuffer_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tracebuffer_config_set(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             aw_pmd_tracebuffer_mode_t tbmode,
                                             uint32_t samples_per_cycle) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_tracebuffer_config_set", dev_id, dev_port, ln, 2, " tbmode",
              (uint32_t)((intptr_t)tbmode & 0xffffffff), " samples_per_cycle",
              (uint32_t)((intptr_t)samples_per_cycle & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tracebuffer_config_set(&mss, tbmode, samples_per_cycle);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_tracebuffer(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *tb_data, int tb_size) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer(&mss, tb_data, tb_size);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_convert_data_signed
 ***********************************************************************/
bf_status_t bf_aw_pmd_convert_data_signed(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port, uint32_t ln,
                                          uint32_t data) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_convert_data_signed(data);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer_adc
 ***********************************************************************/
bf_status_t
bf_aw_pmd_read_tracebuffer_adc(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                               uint32_t ln, int num_samples,
                               int adc_data[num_samples][AW_NUM_BRANCHES]) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer_adc(&mss, num_samples, adc_data);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tbus_client_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tbus_client_config_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t *block_id,
    uint32_t *signal_id, uint32_t *trigger, uint32_t *continuous_sample) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tbus_client_config_get(&mss, block_id, signal_id,
                                               trigger, continuous_sample);

  bf_aw_trace(
      "pmd_tbus_client_config_get", dev_id, dev_port, ln, 4, " block_id",
      *(uint32_t *)(intptr_t)((intptr_t)(block_id)&UINTMAX_MAX), " signal_id",
      *(uint32_t *)(intptr_t)((intptr_t)(signal_id)&UINTMAX_MAX), " trigger",
      *(uint32_t *)(intptr_t)((intptr_t)(trigger)&UINTMAX_MAX),
      " continuous_sample",
      *(uint32_t *)(intptr_t)((intptr_t)(continuous_sample)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tbus_client_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tbus_client_config_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t block_id,
    uint32_t signal_id, uint32_t trigger, uint32_t continuous_sample) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_tbus_client_config_set", dev_id, dev_port, ln, 4,
              " block_id", (uint32_t)((intptr_t)block_id & 0xffffffff),
              " signal_id", (uint32_t)((intptr_t)signal_id & 0xffffffff),
              " trigger", (uint32_t)((intptr_t)trigger & 0xffffffff),
              " continuous_sample",
              (uint32_t)((intptr_t)continuous_sample & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tbus_client_config_set(&mss, block_id, signal_id,
                                               trigger, continuous_sample);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer_demapper
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_tracebuffer_demapper(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    int32_t demapper_data[2][AW_TBUS_NUM_SAMPLES], uint32_t branch_id,
    uint32_t is_ffe) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer_demapper(&mss, demapper_data,
                                                  branch_id, is_ffe);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer_general
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_tracebuffer_general(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    int32_t tb_data_out[AW_TBUS_NUM_SAMPLES], int tbus_block_id,
    uint32_t signal_id, int fp_lsb, int fp_msb, int fp_si) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer_general(
      &mss, tb_data_out, tbus_block_id, signal_id, fp_lsb, fp_msb, fp_si);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer_ffe
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_tracebuffer_ffe(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           int num_samples, int32_t *ffe_data,
                                           int branch_id) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer_ffe(&mss, num_samples, ffe_data,
                                             branch_id);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer_itr_dlpf_int
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_tracebuffer_itr_dlpf_int(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln,
                                                    int num_samples,
                                                    int32_t *itr_dlpf_int) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer_itr_dlpf_int(&mss, num_samples,
                                                      itr_dlpf_int);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_read_tracebuffer_quantizer_err
 ***********************************************************************/
bf_status_t bf_aw_pmd_read_tracebuffer_quantizer_err(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, int num_samples,
    int32_t *qztr_err_data, int branch_id) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_read_tracebuffer_quantizer_err(
      &mss, num_samples, qztr_err_data, branch_id);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_ssc_config
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_ssc_config(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, double lsref_mhz,
                                     double lcpll_mhz, int ppm_downspread) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_ssc_config(&mss, lsref_mhz, lcpll_mhz,
                                       ppm_downspread);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_ssc_en_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_ssc_en_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  bf_aw_trace("pmd_cmn_ssc_en_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_ssc_en_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_sram_clk_div_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_sram_clk_div_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_sram_clk_div_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_sram_clk_div_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_sram_clk_div_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_sram_clk_div_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_sram_clk_div_get(&mss, en);

  bf_aw_trace("pmd_sram_clk_div_get", dev_id, dev_port, ln, 1, " en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_burst_mode_config_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_burst_mode_config_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t pam_mode,
                                               uint32_t burst_threshold,
                                               uint32_t burst_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_burst_mode_config_set", dev_id, dev_port, ln, 3,
              " pam_mode", (uint32_t)((intptr_t)pam_mode & 0xffffffff),
              " burst_threshold",
              (uint32_t)((intptr_t)burst_threshold & 0xffffffff), " burst_mode",
              (uint32_t)((intptr_t)burst_mode & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_burst_mode_config_set(&mss, pam_mode,
                                                 burst_threshold, burst_mode);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_burst_mode_config_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_burst_mode_config_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln, uint32_t *pam_mode,
                                               uint32_t *burst_threshold,
                                               uint32_t *burst_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_burst_mode_config_get(&mss, pam_mode,
                                                 burst_threshold, burst_mode);

  bf_aw_trace("pmd_rx_burst_mode_config_get", dev_id, dev_port, ln, 3,
              " pam_mode",
              *(uint32_t *)(intptr_t)((intptr_t)(pam_mode)&UINTMAX_MAX),
              " burst_threshold",
              *(uint32_t *)(intptr_t)((intptr_t)(burst_threshold)&UINTMAX_MAX),
              " burst_mode",
              *(uint32_t *)(intptr_t)((intptr_t)(burst_mode)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_burst_err_cnt_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_burst_err_cnt_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port, uint32_t ln,
                                           uint32_t *burst_err_cnt) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_burst_err_cnt_get(&mss, burst_err_cnt);

  bf_aw_trace("pmd_rx_burst_err_cnt_get", dev_id, dev_port, ln, 1,
              " burst_err_cnt",
              *(uint32_t *)(intptr_t)((intptr_t)(burst_err_cnt)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_burst_mode_stats_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_burst_mode_stats_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln, uint32_t exp_data[],
                                              uint32_t rec_data[],
                                              uint32_t xor_data[]) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_burst_mode_stats_get(&mss, exp_data, rec_data,
                                                xor_data);

  bf_aw_trace("pmd_rx_burst_mode_stats_get", dev_id, dev_port, ln, 3,
              " exp_data", (uint32_t)((intptr_t)exp_data & 0xffffffff),
              " rec_data", (uint32_t)((intptr_t)rec_data & 0xffffffff),
              " xor_data", (uint32_t)((intptr_t)xor_data & 0xffffffff));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_gray_code_mapping_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_gray_code_mapping_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint8_t *gray_code_map) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_gray_code_mapping_get(&mss, gray_code_map);

  bf_aw_trace("pmd_rx_gray_code_mapping_get", dev_id, dev_port, ln, 1,
              " gray_code_map",
              *(uint32_t *)(intptr_t)((intptr_t)(gray_code_map)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_gray_code_mapping_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_gray_code_mapping_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint8_t gray_code_map) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_gray_code_mapping_set", dev_id, dev_port, ln, 1,
              " gray_code_map",
              (uint32_t)((intptr_t)gray_code_map & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_gray_code_mapping_set(&mss, gray_code_map);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gray_code_mapping_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gray_code_mapping_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint8_t *gray_code_map) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gray_code_mapping_get(&mss, gray_code_map);

  bf_aw_trace("pmd_tx_gray_code_mapping_get", dev_id, dev_port, ln, 1,
              " gray_code_map",
              *(uint32_t *)(intptr_t)((intptr_t)(gray_code_map)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_gray_code_mapping_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_gray_code_mapping_set(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint32_t ln,
                                               uint8_t gray_code_map) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_gray_code_mapping_set", dev_id, dev_port, ln, 1,
              " gray_code_map",
              (uint32_t)((intptr_t)gray_code_map & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_gray_code_mapping_set(&mss, gray_code_map);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_tx_perf_settings
 ***********************************************************************/
bf_status_t bf_aw_pmd_tx_perf_settings(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint8_t perf_mode) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_TX);

  bf_aw_trace("pmd_tx_perf_settings", dev_id, dev_port, ln, 1, " perf_mode",
              (uint32_t)((intptr_t)perf_mode & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_tx_perf_settings(&mss, perf_mode);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cmn_bias_bandgap_force_startup
 ***********************************************************************/
bf_status_t bf_aw_pmd_cmn_bias_bandgap_force_startup(bf_dev_id_t dev_id,
                                                     bf_dev_port_t dev_port,
                                                     uint32_t ln) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_CMN);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cmn_bias_bandgap_force_startup(&mss);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_sris_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_sris_set(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                               uint32_t ln, uint32_t en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_sris_set", dev_id, dev_port, ln, 1, " en",
              (uint32_t)((intptr_t)en & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_sris_set(&mss, en);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_sris_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_sris_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                               uint32_t ln, uint32_t *en) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_sris_get(&mss, en);

  bf_aw_trace("pmd_sris_get", dev_id, dev_port, ln, 1, " en",
              *(uint32_t *)(intptr_t)((intptr_t)(en)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_active_branches_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_active_branches_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *active_branches) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_active_branches_get(&mss, active_branches);

  bf_aw_trace("pmd_rx_active_branches_get", dev_id, dev_port, ln, 1,
              "  active_branches",
              *(uint32_t *)(intptr_t)((intptr_t)(active_branches)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fw_version_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_fw_version_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                     uint32_t ln, aw_version_t *version_st) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_fw_version_get(&mss, version_st);

  bf_aw_trace("pmd_fw_version_get", dev_id, dev_port, ln, 1, " version_st",
              *(uint32_t *)(intptr_t)((intptr_t)(version_st)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dfe_ratio_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dfe_ratio_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t enable,
                                       uint32_t use_auto_lookup_dfe_ratio,
                                       double custom_dfe_ratio) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace(
      "pmd_rx_dfe_ratio_set", dev_id, dev_port, ln, 3, " enable",
      (uint32_t)((intptr_t)enable & 0xffffffff), " use_auto_lookup_dfe_ratio",
      (uint32_t)((intptr_t)use_auto_lookup_dfe_ratio & 0xffffffff),
      " custom_dfe_ratio", (uint32_t)((intptr_t)custom_dfe_ratio & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dfe_ratio_set(
      &mss, enable, use_auto_lookup_dfe_ratio, custom_dfe_ratio);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dfe_ratio_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dfe_ratio_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port, uint32_t ln,
                                       uint32_t *enable,
                                       uint32_t *use_auto_lookup_dfe_ratio,
                                       double *custom_dfe_ratio) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dfe_ratio_get(
      &mss, enable, use_auto_lookup_dfe_ratio, custom_dfe_ratio);

  bf_aw_trace(
      "pmd_rx_dfe_ratio_get", dev_id, dev_port, ln, 3, "  enable",
      *(uint32_t *)(intptr_t)((intptr_t)(enable)&UINTMAX_MAX),
      "  use_auto_lookup_dfe_ratio",
      *(uint32_t *)(intptr_t)(
          (intptr_t)(use_auto_lookup_dfe_ratio)&UINTMAX_MAX),
      "  custom_dfe_ratio",
      *(uint32_t *)(intptr_t)((intptr_t)(custom_dfe_ratio)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rd_csr
 ***********************************************************************/
bf_status_t bf_aw_pmd_rd_csr(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                             uint32_t ln, uint32_t addr, uint32_t *rdata) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rd_csr(&mss, addr, rdata);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_wr_csr
 ***********************************************************************/
bf_status_t bf_aw_pmd_wr_csr(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                             uint32_t ln, uint32_t addr, uint32_t wdata) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_wr_csr(&mss, addr, wdata);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_mss_reset
 ***********************************************************************/
bf_status_t bf_aw_pmd_mss_reset(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                uint32_t ln) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_mss_reset(&mss);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_fw_load
 ***********************************************************************/
bf_status_t bf_aw_pmd_fw_load(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                              uint32_t ln, char *path) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_fw_load(&mss, path);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_one_time_pgm
 ***********************************************************************/
bf_status_t bf_aw_pmd_one_time_pgm(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_one_time_pgm(&mss);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_cur_rate_width_pstate
 ***********************************************************************/
bf_status_t bf_aw_pmd_cur_rate_width_pstate(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, uint32_t *tx_rate,
    uint32_t *tx_width, uint32_t *tx_pstate, uint32_t *rx_rate,
    uint32_t *rx_width, uint32_t *rx_pstate) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_cur_rate_width_pstate(
      &mss, tx_rate, tx_width, tx_pstate, rx_rate, rx_width, rx_pstate);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rxmfsm_eq_check_rxdisable_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rxmfsm_eq_check_rxdisable_set(bf_dev_id_t dev_id,
                                                    bf_dev_port_t dev_port,
                                                    uint32_t ln, uint32_t val) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rxmfsm_eq_check_rxdisable_set", dev_id, dev_port, ln, 1,
              " val", (uint32_t)((intptr_t)val & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rxmfsm_eq_check_rxdisable_set(&mss, val);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_lt_info_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_lt_info_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                  uint32_t ln, uint32_t *lt_fsm_st,
                                  uint32_t *frame_lock) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_lt_info_get(&mss, lt_fsm_st, frame_lock);

  bf_aw_trace("pmd_lt_info_get", dev_id, dev_port, ln, 2, "  lt_fsm_st",
              *(uint32_t *)(intptr_t)((intptr_t)(lt_fsm_st)&UINTMAX_MAX),
              "  frame_lock",
              *(uint32_t *)(intptr_t)((intptr_t)(frame_lock)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_reg_defs_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_reg_defs_get(bf_dev_id_t dev_id, bf_dev_port_t dev_port,
                                   uint32_t ln, aw_reg_defs_t **regs,
                                   aw_fld_defs_t **flds, char **cmnts,
                                   aw_reg_defs_t **vregs, aw_fld_defs_t **vflds,
                                   char **vcmnts) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_reg_defs_get(regs, flds, cmnts, vregs, vflds, vcmnts);

  bf_aw_trace("pmd_reg_defs_get", dev_id, dev_port, ln, 6, "regs",
              *(uint32_t *)(intptr_t)((intptr_t)(regs)&UINTMAX_MAX), " flds",
              *(uint32_t *)(intptr_t)((intptr_t)(flds)&UINTMAX_MAX), " cmnts",
              *(uint32_t *)(intptr_t)((intptr_t)(cmnts)&UINTMAX_MAX), " vregs",
              *(uint32_t *)(intptr_t)((intptr_t)(vregs)&UINTMAX_MAX), " vflds",
              *(uint32_t *)(intptr_t)((intptr_t)(vflds)&UINTMAX_MAX), " vcmnts",
              *(uint32_t *)(intptr_t)((intptr_t)(vcmnts)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_speed_to_rate_and_width
 ***********************************************************************/
bf_status_t bf_aw_pmd_speed_to_rate_and_width(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t serdes_speed, bool is_pam4, uint32_t *rate, uint32_t *width) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_speed_to_rate_and_width(&mss, serdes_speed, is_pam4,
                                                rate, width);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dig_pwr_det_threshold_get
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dig_pwr_det_threshold_get(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t *adc_valid_thresh_nt, uint32_t *adc_invalid_thresh_nt) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dig_pwr_det_threshold_get(&mss, adc_valid_thresh_nt,
                                                     adc_invalid_thresh_nt);

  bf_aw_trace(
      "pmd_rx_dig_pwr_det_threshold_get", dev_id, dev_port, ln, 2,
      " adc_valid_thresh_nt",
      *(uint32_t *)(intptr_t)((intptr_t)(adc_valid_thresh_nt)&UINTMAX_MAX),
      " adc_invalid_thresh_nt ",
      *(uint32_t *)(intptr_t)((intptr_t)(adc_invalid_thresh_nt)&UINTMAX_MAX));

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}

/***********************************************************************
 *           bf_aw_pmd_rx_dig_pwr_det_threshold_set
 ***********************************************************************/
bf_status_t bf_aw_pmd_rx_dig_pwr_det_threshold_set(
    bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln,
    uint32_t adc_valid_thresh_nt, uint32_t adc_invalid_thresh_nt) {
  mss_access_t mss;
  int rc;
  bf_tf3_sd_t *tf3_sd =
      bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, MSS_SECTION_RX);

  bf_aw_trace("pmd_rx_dig_pwr_det_threshold_set", dev_id, dev_port, ln, 2,
              " adc_valid_thresh_nt",
              (uint32_t)((intptr_t)adc_valid_thresh_nt & 0xffffffff),
              " adc_invalid_thresh_nt ",
              (uint32_t)((intptr_t)adc_invalid_thresh_nt & 0xffffffff));

  // Call thru API vector
  rc = tf3_sd->api->pmd_rx_dig_pwr_det_threshold_set(&mss, adc_valid_thresh_nt,
                                                     adc_invalid_thresh_nt);

  // map AW error codes to bf_types_t error codes
  return map_aw_err_to_bf_err(rc);
}
