/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2021 Intel Corporation
 *  All Rights Reserved.
 *
 *  This software and the related documents are Intel copyrighted materials,
 *  and your use of them is governed by the express license under which they
 *  were provided to you ("License"). Unless the License provides otherwise,
 *  you may not use, modify, copy, publish, distribute, disclose or transmit
 *  this software or the related documents without Intel's prior written
 *  permission.
 *
 *  This software and the related documents are provided as is, with no express
 *  or implied warranties, other than those that are expressly stated in the
 *  License.
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#ifndef TILE_SIM

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <tofino_regs/tofino.h>
#include <lld/lld_reg_if.h>
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <lld/lld_efuse.h>

#include <bf_types/bf_types.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/bf_tof2_serdes_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/port_mgr_log.h>
#include "port_mgr_tof2_physical_dev.h"
#include "port_mgr_tof2_serdes.h"
#include "port_mgr_tof2_map.h"
#include "credo_sd_access.h"
#endif

extern bf_status_t port_mgr_tof2_bg_cal_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *rx_bg,
                                            uint32_t *tx_bg);
extern void port_mgr_tof2_bandgap_cal(bf_dev_id_t dev_id);
extern int port_mgr_log_tile_efuse(void *optional_uc, bf_dev_id_t dev_id);

/** \brief  Set Tx and Rx logical to physical mappings (based on board layout)
 *
 *          Note: this function programs only the serdes, not the MAC, and
 *                does so immediately. It is intended for diagnostic purposes.
 *                For normal operation use bf_port_lane_map_set() in
 *                bf_port_if.c
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 */
bf_status_t bf_tof2_serdes_bandgap_init(bf_dev_id_t dev_id) {
  (void)dev_id;

  port_mgr_log("Serdes Bandgap init");
  port_mgr_tof2_bandgap_cal(dev_id);
  return BF_SUCCESS;
}

/** \brief  Set Tx and Rx logical to physical mappings (based on board layout)
 *
 *          Note: this function programs only the serdes, not the MAC, and
 *                does so immediately. It is intended for diagnostic purposes.
 *                For normal operation use bf_port_lane_map_set() in
 *                bf_port_if.c
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : is assumed to be "0"
 * \param[in]  phys_tx_ln : Tx phys -> logical for each logical lane, 0-7
 * \param[in]  phys_rx_ln : Rx phys -> logical for each logical lane, 0-7
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_lane_map_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        uint32_t phys_tx_ln[8],
                                        uint32_t phys_rx_ln[8]) {
  bf_status_t rc;
  (void)ln;

  rc = port_mgr_tof2_serdes_lane_map_set(
      dev_id, dev_port, phys_tx_ln, phys_rx_ln);
  return rc;
}

/** \brief  Get Tx and Rx logical to physical mappings (based on board layout)
 *
 *          Note: this function programs only the serdes, not the MAC, and
 *                does so immediately. It is intended for diagnostic purposes.
 *                For normal operation use bf_port_lane_map_set() in
 *                bf_port_if.c
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : is assumed to be "0"
 * \param[in]  phys_tx_ln : Tx phys -> logical for each logical lane, 0-7
 * \param[in]  phys_rx_ln : Rx phys -> logical for each logical lane, 0-7
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_lane_map_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t phys_tx_ln[8],
                                        uint32_t phys_rx_ln[8]) {
  return port_mgr_tof2_serdes_lane_map_get(
      dev_id, dev_port, phys_tx_ln, phys_rx_ln);
}

/** \brief  Cache Tx Eq settings and optionally apply to hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 * \param[in]  pre2       : 8 bit signed values
 * \param[in]  pre        :
 * \param[in]  main       :
 * \param[in]  post1      :
 * \param[in]  post2      :
 * \param[in]  apply      : true= apply to hw
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_taps_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       int32_t pre2,
                                       int32_t pre1,
                                       int32_t main,
                                       int32_t post1,
                                       int32_t post2,
                                       bool apply) {
  bf_status_t rc;

  // must SUM to less than ??
  // check here

  rc = port_mgr_tof2_serdes_tx_eq_set(
      dev_id, dev_port, ln, pre2, pre1, main, post1, post2, apply);
  return rc;
}

/** \brief  Retrieve cached Tx Eq settings
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 * \param[in]  pre2       : 8 bit signed values
 * \param[in]  pre        :
 * \param[in]  main       :
 * \param[in]  post1      :
 * \param[in]  post2      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_taps_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       int32_t *pre2,
                                       int32_t *pre1,
                                       int32_t *main,
                                       int32_t *post1,
                                       int32_t *post2) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_eq_get(
      dev_id, dev_port, ln, pre2, pre1, main, post1, post2);
  return rc;
}

/** \brief  Retrieve currently applied (hw)  Tx Eq settings
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 * \param[in]  pre2       : 8 bit signed values
 * \param[in]  pre        :
 * \param[in]  main       :
 * \param[in]  post1      :
 * \param[in]  post2      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_taps_hw_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          int32_t *pre2,
                                          int32_t *pre1,
                                          int32_t *main,
                                          int32_t *post1,
                                          int32_t *post2) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_taps_get(
      dev_id, dev_port, ln, pre2, pre1, main, post1, post2);
  return rc;
}

/** \brief Squelch Tx Output by 0'ing the Tx taps but don't update cache
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_squelch_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln) {
  return port_mgr_tof2_serdes_tx_taps_set(dev_id, dev_port, ln, 0, 0, 0, 0, 0);
}

/** \brief  Get Rx Signal information
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] sig_detect : Rx Signal detected (true)
 * \param[out] phy_ready  : CDR Lock (true)
 * \param[out] ppm        : Apparent PPM difference between local and remote
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_sig_info_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           bool *sig_detect,
                                           bool *phy_ready,
                                           int32_t *ppm) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_sig_detect_get(
      dev_id, dev_port, ln, sig_detect, phy_ready);
  if (rc != BF_SUCCESS) return rc;

  rc = port_mgr_tof2_serdes_ppm_get(dev_id, dev_port, ln, ppm);
  if (rc != BF_SUCCESS) return rc;

  if (*ppm & (1 << 10)) {
    *ppm = (int)*ppm - 2048;
  }
  return BF_SUCCESS;
}

/** \brief  Get AN (and HCD speed set) done
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] an_done    : AN complete (true)
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_an_done_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       bool *an_done) {
  bf_status_t rc;
  if (an_done == NULL) return BF_INVALID_ARG;

  rc = port_mgr_tof2_serdes_an_done_get(dev_id, dev_port, ln, an_done);
  return rc;
}

/** \brief  Get Rx Adaptation information
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] adapt_done : # DFE complete (true), may be NULL
 * \param[out] adapt_cnt  : # DFE attempts (total), may be NULL
 * \param[out] readapt_cnt: # DFE attempts since last read, may be NULL
 * \param[out] link_lost  : # times signal lost since last read, may be NULL
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_adapt_counts_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            bool *adapt_done,
                                            uint32_t *adapt_cnt,
                                            uint32_t *readapt_cnt,
                                            uint32_t *link_lost_cnt) {
  bf_status_t rc;

  if (adapt_done) {
    rc = port_mgr_tof2_serdes_adapt_done_get(dev_id, dev_port, ln, adapt_done);
    if (rc != BF_SUCCESS) return rc;
  }
  if (adapt_cnt) {
    rc = port_mgr_tof2_serdes_fw_adapt_cnt_get(dev_id, dev_port, ln, adapt_cnt);
    if (rc != BF_SUCCESS) return rc;
  }
  if (readapt_cnt) {
    rc = port_mgr_tof2_serdes_fw_readapt_cnt_get(
        dev_id, dev_port, ln, readapt_cnt);
    if (rc != BF_SUCCESS) return rc;
  }
  if (link_lost_cnt) {
    rc = port_mgr_tof2_serdes_fw_link_lost_cnt_get(
        dev_id, dev_port, ln, link_lost_cnt);
    if (rc != BF_SUCCESS) return rc;
  }
  return BF_SUCCESS;
}

/** \brief  Get Rx Adaptation done indicationn
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] adapt_done : # DFE complete (true), may be NULL
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_adapt_done_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          bool *adapt_done) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_adapt_done_get(dev_id, dev_port, ln, adapt_done);
  return rc;
}

/** \brief  Cache Tx polarity in serdes struct and optionally apply to hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 * \param[out] apply      : true=apply to hw
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_polarity_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           bool inv,
                                           bool apply) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_pol_inv_set(dev_id, dev_port, ln, inv, apply);
  return rc;
}

/** \brief  Retreive cached Tx polarity from serdes struct
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_polarity_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           bool *inv) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_pol_inv_get(dev_id, dev_port, ln, inv);
  return rc;
}

/** \brief  Retreive programmed Tx polarity from hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_polarity_hw_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              bool *inv) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_polarity_get(dev_id, dev_port, ln, inv);
  return rc;
}

/** \brief  Cache Rx polarity in serdes struct and optionally apply to hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 * \param[out] apply      : true=apply to hw
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_polarity_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           bool inv,
                                           bool apply) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_rx_pol_inv_set(dev_id, dev_port, ln, inv, apply);
  return rc;
}

/** \brief  Retreive cached Rx polarity from serdes struct
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_polarity_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           bool *inv) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_rx_pol_inv_get(dev_id, dev_port, ln, inv);
  return rc;
}

/** \brief  Retreive programmed Rx polarity from  hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] inv        : true=invert polarity
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_polarity_hw_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t ln,
                                              bool *inv) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_rx_polarity_get(dev_id, dev_port, ln, inv);
  return rc;
}

/** \brief  Retreive programmed Rx bandgap from cache
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] rx_bg      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_bandgap_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          uint32_t *rx_bg) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_rx_bandgap_get(dev_id, dev_port, ln, rx_bg);
  return rc;
}

/** \brief  Retreive programmed Rx bandgap from hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] rx_bg      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_bandgap_hw_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *rx_bg) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_rx_bandgap_hw_get(dev_id, dev_port, ln, rx_bg);
  return rc;
}

/** \brief  Retreive programmed Rx bandgap from cache
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] rx_bg      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_rx_bandgap_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          uint32_t rx_bg,
                                          bool apply) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_rx_bandgap_set(dev_id, dev_port, ln, rx_bg, apply);
  return rc;
}

/** \brief  Retreive programmed Tx bandgap from cache
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] tx_bg      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_bandgap_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          uint32_t *tx_bg) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_bandgap_get(dev_id, dev_port, ln, tx_bg);
  return rc;
}

/** \brief  Retreive programmed Tx bandgap from hw
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] tx_bg      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_bandgap_hw_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *tx_bg) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_bandgap_hw_get(dev_id, dev_port, ln, tx_bg);
  return rc;
}

/** \brief  Retreive programmed Tx bandgap from cache
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : Logical lane within port (0..7, mode dependent)
 * \param[out] tx_bg      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_tx_bandgap_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          uint32_t tx_bg,
                                          bool apply) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_tx_bandgap_set(dev_id, dev_port, ln, tx_bg, apply);
  return rc;
}

/** \brief  Apply a cpu reset. This is done at start-up, after
 *          FW dnld
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_cpu_reset_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_cpu_reset(dev_id, dev_port, ln);
  return rc;
}

/** \brief  Apply a reset to an entire group of 8 (or 4) serdes.
 *
 * This function accomplishes the reset in three steps. The steps
 * are used to mask the required delays between resets.
 *
 * step 0: soft reset
 * step 1: logic reset
 * step 2: Training reset
 *
 * All steps require a 100us delay before executing the next step. You can
 * apply the group reset in parallel to all groups by executing each step
 * on all groups then waiting for a single 100us delay.
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         : unused here (kept for API consistency)
 * \param[out] phase      : 0-2
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 */
bf_status_t bf_tof2_serdes_group_reset_set(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           uint32_t phase) {
  bf_status_t rc;

  if (phase > 2) return BF_INVALID_ARG;

  rc = port_mgr_tof2_serdes_group_reset(dev_id, dev_port, ln, phase);
  return rc;
}

/** \brief Run a simple Power-On Self-Test (POST)
 *
 * Verify accessibility of the tile and group comprising this port
 * by reading and writing a tile register that contains a known
 * reset value and is RW.
 *
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  n_lanes    : 8 for groups 0-7, 4 for group 8 (CPU port)
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 * \return: BF_HW_COMM_FAIL: POST failed
 */
bf_status_t bf_tof2_serdes_power_on_self_test(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              uint32_t n_lanes) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_power_on_self_test(dev_id, dev_port, n_lanes);
  return rc;
}

/** \brief Run SRAM BIST on a group of 8 serdes
 *
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 * \return: BF_HW_COMM_FAIL: POST failed
 */
bf_status_t bf_tof2_serdes_sram_bist_grp(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port) {
  return port_mgr_tof2_serdes_sram_bist_grp(dev_id, dev_port);
}

/** \brief Run ROM BIST on a group of 8 serdes
 *
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 * \return: BF_INVALID_ARG: lane > # of serdes lanes in this mode
 * \return: BF_HW_COMM_FAIL: POST failed
 */
bf_status_t bf_tof2_serdes_rom_bist_grp(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port) {
  return port_mgr_tof2_serdes_rom_bist_grp(dev_id, dev_port);
}

/*
 */
static bool bf_valid_pam4_prbs_mode(bf_port_prbs_mode_t pat,
                                    port_mgr_tof2_prbs_mode_t *pam4_pat) {
  if (pat == BF_PORT_PRBS_MODE_31) {
    *pam4_pat = PORT_MGR_PRBS_PAM4_MODE_PRBS31;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_15) {
    *pam4_pat = PORT_MGR_PRBS_PAM4_MODE_PRBS15;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_13) {
    *pam4_pat = PORT_MGR_PRBS_PAM4_MODE_PRBS13;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_9) {
    *pam4_pat = PORT_MGR_PRBS_PAM4_MODE_PRBS9;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_NONE) {
    *pam4_pat = PORT_MGR_PRBS_PAM4_MODE_NONE;
    return true;
  }
  return false;
}

/*
 */
static bool bf_valid_nrz_prbs_mode(bf_port_prbs_mode_t pat,
                                   port_mgr_tof2_prbs_mode_t *nrz_pat) {
  if (pat == BF_PORT_PRBS_MODE_31) {
    *nrz_pat = PORT_MGR_PRBS_NRZ_MODE_PRBS31;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_23) {
    *nrz_pat = PORT_MGR_PRBS_NRZ_MODE_PRBS23;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_15) {
    *nrz_pat = PORT_MGR_PRBS_NRZ_MODE_PRBS15;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_9) {
    *nrz_pat = PORT_MGR_PRBS_NRZ_MODE_PRBS9;
    return true;
  } else if (pat == BF_PORT_PRBS_MODE_NONE) {
    *nrz_pat = PORT_MGR_PRBS_NRZ_MODE_NONE;
    return true;
  }
  return false;
}

/** \brief Init lane prior to FW config cmd
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_init_ln(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t ln) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_init_lane_for_an(dev_id, dev_port, ln);
  return rc;
}

/** \brief Pre-Configure bandgap and polarity
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_pre_config_ln(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln) {
  bf_status_t rc;
  bool inv;
  uint32_t tx_bg;
  uint32_t rx_bg;

  // retreive calibrated bandgap settings
  port_mgr_tof2_bg_cal_get(dev_id, dev_port, ln, &rx_bg, &tx_bg);

  rc = port_mgr_tof2_serdes_tx_bandgap_set(dev_id, dev_port, ln, tx_bg, true);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d: Tx bandgap set failed: rc=%d", dev_id, dev_port, rc);
  }
  rc = port_mgr_tof2_serdes_rx_bandgap_set(dev_id, dev_port, ln, rx_bg, true);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d: Rx bandgap set failed: rc=%d", dev_id, dev_port, rc);
  }
  rc = bf_tof2_serdes_tx_polarity_get(dev_id, dev_port, ln, &inv);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d: Tx polarity get failed: rc=%d", dev_id, dev_port, rc);
  }
  rc =
      bf_tof2_serdes_tx_polarity_set(dev_id, dev_port, ln, inv, true /*apply*/);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d: Tx polarity set failed: rc=%d", dev_id, dev_port, rc);
  }
  rc = bf_tof2_serdes_rx_polarity_get(dev_id, dev_port, ln, &inv);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d: Rx polarity get failed: rc=%d", dev_id, dev_port, rc);
  }
  rc =
      bf_tof2_serdes_rx_polarity_set(dev_id, dev_port, ln, inv, true /*apply*/);
  if (rc != BF_SUCCESS) {
    port_mgr_log("%d:%3d: Rx polarity set failed: rc=%d", dev_id, dev_port, rc);
  }
  return BF_SUCCESS;
}

/** \brief Configure a lane to run autonegotiation
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 * \param[in]  basepage   : 48b IEEE base page advertisement
 * \param[in]  consortium_np_31_16 : variable consortium advertisement bits
 * \param[in]  is_loop    : disable nonce check on loopback plugs
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_config_ln_autoneg(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint64_t basepage,
                                             uint32_t consortium_np_47_16,
                                             bool is_loop) {
  bf_status_t rc;

  // un-configure any existing lane settings
  port_mgr_tof2_serdes_un_config_ln(dev_id, dev_port, ln);

  // pre-config bandgap and polarity settings
  bf_tof2_serdes_pre_config_ln(dev_id, dev_port, ln);

  // program advertisement and start AN
  rc = port_mgr_tof2_serdes_config_ln_an(
      dev_id, dev_port, ln, basepage, consortium_np_47_16, is_loop);
  return rc;
}

/** \brief Configure a serdes lane i either NRZ or PAM4 mode, based
 *         on the current port speed and n_lanes configured.
 *         If successful, also programs cached values of,
 *           Tx EQ params
 *           Tx polarity
 *           Rx polarity
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  ln         :
 * \param[in]  speed      : 400, 200, 100, ...
 * \param[in]  n_lanes    : w/speed, distinguishes 25g vs 50g serdes speed
 * \param[in]  tx_pat     : NONE=mission mode
 * \param[in]  rx_pat     : NONE=mission mode
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_config_ln(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     uint32_t ln,
                                     bf_port_speed_t speed,
                                     uint32_t n_lanes,
                                     bf_port_prbs_mode_t tx_pat,
                                     bf_port_prbs_mode_t rx_pat,
                                     bool leave_squelched) {
  bf_status_t rc = BF_SUCCESS;
  bf_serdes_encoding_mode_t enc_mode;
  int32_t pre2;
  int32_t pre1;
  int32_t main_tap;  // compiler warns about "main" not being a fn
  int32_t post1;
  int32_t post2;

  if (speed != BF_SPEED_NONE) {
    // pre-config bandgap and polarity settings
    bf_tof2_serdes_pre_config_ln(dev_id, dev_port, ln);
  }

  // un-configure any existing lane settings
  port_mgr_tof2_serdes_un_config_ln(dev_id, dev_port, ln);

  // if speed=0, this was just a request to un-configure the lane
  if (speed == BF_SPEED_NONE) {
    return BF_SUCCESS;
  }

  bf_serdes_encoding_mode_get(speed, n_lanes, &enc_mode);

  if (enc_mode == BF_SERDES_ENC_MODE_PAM4) {
    port_mgr_tof2_prbs_mode_t pam4_tx_pat, pam4_rx_pat;
    // check if valid prbs mode for PAM4
    if (bf_valid_pam4_prbs_mode(tx_pat, &pam4_tx_pat) &&
        (bf_valid_pam4_prbs_mode(rx_pat, &pam4_rx_pat))) {
      rc = port_mgr_tof2_serdes_config_ln_pam4(dev_id,
                                               dev_port,
                                               ln,
                                               PORT_MGR_SERDES_PAM4_SPEED_53,
                                               pam4_tx_pat,
                                               pam4_rx_pat);
      if (rc != BF_SUCCESS) {
        port_mgr_log("%d:%3d: bf_tof2_serdes_config_ln failed: rc=%d",
                     dev_id,
                     dev_port,
                     rc);
      }
    } else {
      bf_sys_assert(0);
    }
  } else if (enc_mode == BF_SERDES_ENC_MODE_NRZ) {
    port_mgr_tof2_prbs_mode_t nrz_tx_pat, nrz_rx_pat;

    // check if valid prbs mode for NRZ
    if (bf_valid_nrz_prbs_mode(tx_pat, &nrz_tx_pat) &&
        (bf_valid_nrz_prbs_mode(rx_pat, &nrz_rx_pat))) {
      port_mgr_serdes_nrz_speed_t nrz_speed;

      if (speed == BF_SPEED_1G) {
        nrz_speed = PORT_MGR_SERDES_NRZ_SPEED_1;
      } else if ((speed == BF_SPEED_10G) || (speed == BF_SPEED_40G)) {
        nrz_speed = PORT_MGR_SERDES_NRZ_SPEED_10;
      } else if (speed == BF_SPEED_40G_R2) {
        nrz_speed = PORT_MGR_SERDES_NRZ_SPEED_20;
      } else {
        nrz_speed = PORT_MGR_SERDES_NRZ_SPEED_25;
      }
      rc = port_mgr_tof2_serdes_config_ln_nrz(
          dev_id, dev_port, ln, nrz_speed, nrz_tx_pat, nrz_rx_pat);
    } else {
      bf_sys_assert(0);
    }
  }

  if (leave_squelched) {
    bf_tof2_serdes_tx_squelch_set(dev_id, dev_port, ln);
  } else {
    // get configured values
    rc = bf_tof2_serdes_tx_taps_get(
        dev_id, dev_port, ln, &pre2, &pre1, &main_tap, &post1, &post2);
    if (rc != BF_SUCCESS) {
      port_mgr_log(
          "%d:%3d: Tx Tap settings get failed: rc=%d", dev_id, dev_port, rc);
    }
    // unsquelch tx output
    rc = bf_tof2_serdes_tx_taps_set(dev_id,
                                    dev_port,
                                    ln,
                                    pre2,
                                    pre1,
                                    main_tap,
                                    post1,
                                    post2,
                                    true /*apply to hw*/);
    if (rc != BF_SUCCESS) {
      port_mgr_log(
          "%d:%3d: Tx Tap settings set failed: rc=%d", dev_id, dev_port, rc);
    }
  }
  return rc;
}

/** \brief Init the tile/group implementing the specified dev_port.
 *         Initialization requires,
 *           POST (optional)
 *           FW load (optional, may also be forced reload)
 *           verify requested FW is loaded
 *           un-configure any previously configured lanes
 *
 * \param[in]  dev_id     : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port   :
 * \param[in]  fw_file_name: full path to FW file
 * \param[in]  force_load : force FW reload
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_init_group(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      char *fw_file_name,
                                      bool force_load) {
  bf_status_t rc;
  uint8_t *fw_buffer_p = NULL;
  uint32_t fw_len;
  uint32_t fw_hash_code, running_hash_code;
  uint32_t fw_crc, running_crc;
  bool load_reqd = true;
  uint32_t ln;
  uint32_t n_lanes = (dev_port < 8) ? 4 : 8;  // CPU port has only 4 lanes

  rc = port_mgr_tof2_serdes_fw_load_to_buffer(
      fw_file_name, &fw_buffer_p, &fw_len, &fw_hash_code, &fw_crc);
  if (rc == BF_SUCCESS) {
    if (force_load) {
      load_reqd = true;
    } else {
      port_mgr_tof2_serdes_fw_check_load_reqd(
          dev_id, dev_port, fw_hash_code, fw_crc, &load_reqd);
    }
    if (load_reqd) {
      port_mgr_log("%d:%3d : FW load : %s %s",
                   dev_id,
                   dev_port,
                   force_load ? "(force)" : "",
                   fw_file_name);

      rc = port_mgr_tof2_serdes_fw_load_from_buffer(
          dev_id, dev_port, fw_buffer_p);
      if (rc != BF_SUCCESS) {
        port_mgr_log("%d:%3d : FW load : FAILED : rc=%d", dev_id, dev_port, rc);
      } else {
        port_mgr_tof2_serdes_fw_hash_get(
            dev_id, dev_port, 0, &running_hash_code);
        rc = port_mgr_tof2_serdes_fw_crc_get(dev_id, dev_port, 0, &running_crc);
        if (rc != BF_SUCCESS) {
          port_mgr_log("%d:%3d : FW CRC: FAILED : rc=%d", dev_id, dev_port, rc);
        } else {
          rc = port_mgr_tof2_serdes_fw_hash_get(
              dev_id, dev_port, 0, &running_hash_code);
          if (rc != BF_SUCCESS) {
            port_mgr_log(
                "%d:%3d : FW hash: FAILED : rc=%d", dev_id, dev_port, rc);
          } else {
            if ((running_crc == fw_crc) &&
                (running_hash_code == fw_hash_code)) {
            } else {
              bf_sys_free(fw_buffer_p);
              return BF_INVALID_ARG;
            }
          }
        }
      }
    }
    for (ln = 0; ln < n_lanes; ln++) {
      if (ln == 0) {
        // restart FW
        bf_tof2_serdes_cpu_reset_set(dev_id, dev_port, ln);
      }
      // un-configure any previously configured lanes
      bf_tof2_serdes_config_ln(dev_id,
                               dev_port,
                               ln,
                               BF_SPEED_NONE,
                               1,
                               BF_PORT_PRBS_MODE_NONE,
                               BF_PORT_PRBS_MODE_NONE,
                               true);
    }
    bf_sys_free(fw_buffer_p);
    return BF_SUCCESS;
  }
  if (fw_buffer_p) bf_sys_free(fw_buffer_p);
  return rc;
}

/** \brief Return file and running hash and crc
 *
 *
 * \param[in]  dev_id           : System-assigned identifier
 *(0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port         :
 * \param[in]  fw_file_name     :
 * \param[in]  fw_buffer_p      :
 * \param[out] fw_len           :
 * \param[out] fw_hash_code     :
 * \param[out] fw_crc           :
 * \param[out] running_hash_code:
 * \param[out] running_crc)     :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fw_ver_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      char *fw_file_name,
                                      uint32_t *fw_len,
                                      uint32_t *fw_hash_code,
                                      uint32_t *fw_crc,
                                      uint32_t *running_hash_code,
                                      uint32_t *running_crc,
                                      uint32_t *fw_ver) {
  uint8_t *fw_buffer_p = NULL;
  port_mgr_tof2_serdes_fw_load_to_buffer(
      fw_file_name, &fw_buffer_p, fw_len, fw_hash_code, fw_crc);
  port_mgr_tof2_serdes_fw_hash_get(dev_id, dev_port, 0, running_hash_code);
  port_mgr_tof2_serdes_fw_crc_get(dev_id, dev_port, 0, running_crc);
  port_mgr_tof2_serdes_fw_ver_get(dev_id, dev_port, 0, fw_ver);
  if (fw_buffer_p) bf_sys_free(fw_buffer_p);
  return BF_SUCCESS;
}

/** \brief Power-up/down specific serdes blocks
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[in]  rx_off   :
 * \param[in]  tx_off   :
 * \param[in]  rx_bg_off:
 * \param[in]  tx_bg_off:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_power_dn_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        bool rx_off,
                                        bool tx_off,
                                        bool rx_bg_off,
                                        bool tx_bg_off) {
  return port_mgr_tof2_serdes_power_dn_set(
      dev_id, dev_port, ln, rx_off, tx_off, rx_bg_off, tx_bg_off);
}

/** \brief Reset the PRBS error count
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_prbs_rst_set(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln) {
  return port_mgr_tof2_serdes_prbs_rst_set(dev_id, dev_port, ln);
}

/** \brief Return the PRBS error count
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] err_cnt  : 32b error count (note: wraps)
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_rx_prbs_err_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           uint32_t *err_cnt) {
  return port_mgr_tof2_serdes_rx_prbs_err_get(dev_id, dev_port, ln, err_cnt);
}

/** \brief Return the TX PRBS configuration
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tx_cfg   : tx config
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_tx_prbs_cfg_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           uint32_t *tx_cfg) {
  return port_mgr_tof2_serdes_tx_prbs_cfg_get(dev_id, dev_port, ln, tx_cfg);
}

/** \brief Return the PPM offset
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] ppm      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_ppm_get(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t ln,
                                   int32_t *ppm) {
  return port_mgr_tof2_serdes_ppm_get(dev_id, dev_port, ln, ppm);
}

/** \brief Return eye height(s) in mv
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] eye_1    : NRZ or PAM4
 * \param[out] eye_2    : PAM4 only
 * \param[out] eye_3    : PAM4 only
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_eye_get(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t ln,
                                   float *eye_1,
                                   float *eye_2,
                                   float *eye_3) {
  return port_mgr_tof2_serdes_eye_get(
      dev_id, dev_port, ln, eye_1, eye_2, eye_3);
}

/** \brief Return OF/HF values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] of       :
 * \param[out] hf       :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_of_get(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  uint32_t ln,
                                  uint32_t *of,
                                  uint32_t *hf) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_of_get(dev_id, dev_port, ln, of);
  if (rc == BF_SUCCESS) {
    rc = port_mgr_tof2_serdes_hf_get(dev_id, dev_port, ln, hf);
  }
  return rc;
}

/** \brief Return Delta value
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] delta    :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_delta_get(bf_dev_id_t dev_id,
                                     bf_dev_port_t dev_port,
                                     uint32_t ln,
                                     uint32_t *delta) {
  return port_mgr_tof2_serdes_delta_get(dev_id, dev_port, ln, delta);
}

/** \brief Return Edge1-4
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] edge1    :
 * \param[out] edge2    :
 * \param[out] edge3    :
 * \param[out] edge4    :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_edge_get(bf_dev_id_t dev_id,
                                    bf_dev_port_t dev_port,
                                    uint32_t ln,
                                    uint32_t *edge1,
                                    uint32_t *edge2,
                                    uint32_t *edge3,
                                    uint32_t *edge4) {
  return port_mgr_tof2_serdes_edge_get(
      dev_id, dev_port, ln, edge1, edge2, edge3, edge4);
}

/** \brief Return NRZ DFE taps
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tap1     :
 * \param[out] tap2     :
 * \param[out] tap3     :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_dfe_nrz_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       uint32_t *tap1,
                                       uint32_t *tap2,
                                       uint32_t *tap3) {
  return port_mgr_tof2_serdes_dfe_nrz_get(
      dev_id, dev_port, ln, tap1, tap2, tap3);
}

/** \brief Return PAM4 DFE values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] f0       :
 * \param[out] f1       :
 * \param[out] ratio    :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_dfe_pam4_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        float *f0,
                                        float *f1,
                                        float *ratio) {
  return port_mgr_tof2_serdes_dfe_pam4_get(dev_id, dev_port, ln, f0, f1, ratio);
}

/** \brief Return Skin Effect (SKEF) values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] skef_val :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_skef_val_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        uint32_t *skef_val) {
  return port_mgr_tof2_serdes_skef_val_get(dev_id, dev_port, ln, skef_val);
}

/** \brief Return DAC value
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] dac_val  :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_dac_val_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       uint32_t *dac_val) {
  return port_mgr_tof2_serdes_dac_val_get(dev_id, dev_port, ln, dac_val);
}

/** \brief Return CTLE values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] ctle_sel :
 * \param[out] ctle_map_0:
 * \param[out] ctle_map_1:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_ctle_val_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        uint32_t ctle_sel,
                                        uint32_t *ctle_map_0,
                                        uint32_t *ctle_map_1) {
  return port_mgr_tof2_serdes_ctle_val_get(
      dev_id, dev_port, ln, ctle_sel, ctle_map_0, ctle_map_1);
}

/** \brief Return CTLE override value
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] ctle_sel :
 * \param[out] ctle_map_0:
 * \param[out] ctle_map_1:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_ctle_over_val_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t *ctle_over_val) {
  return port_mgr_tof2_serdes_ctle_over_val_get(
      dev_id, dev_port, ln, ctle_over_val);
}

/** \brief Set AGC gain values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] ctle_gain_1:
 * \param[out] ctle_gain_2:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_agcgain_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       uint32_t ctle_gain_1,
                                       uint32_t ctle_gain_2) {
  return port_mgr_tof2_serdes_agcgain_set(
      dev_id, dev_port, ln, ctle_gain_1, ctle_gain_2);
}

/** \brief Return AGC gain values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] ctle_gain_1:
 * \param[out] ctle_gain_2:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_ctle_gain_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         uint32_t *ctle_gain_1,
                                         uint32_t *ctle_gain_2) {
  return port_mgr_tof2_serdes_ctle_gain_get(
      dev_id, dev_port, ln, ctle_gain_1, ctle_gain_2);
}

/** \brief Get termination mode
 *
 *
 * \param[in]  dev_id    : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port  :
 * \param[in]  ln        :
 * \param[out] ac_coupled:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_term_mode_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         bool *ac_coupled) {
  return port_mgr_tof2_serdes_ac_couple_get(dev_id, dev_port, ln, ac_coupled);
}

/** \brief Set termination mode
 *
 *
 * \param[in]  dev_id    : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port  :
 * \param[in]  ln        :
 * \param[out] ac_coupled:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_term_mode_set(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         bool ac_coupled) {
  return port_mgr_tof2_serdes_ac_couple_set(dev_id, dev_port, ln, ac_coupled);
}

/** \brief Return programmed precoder settings
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tx_en:
 * \param[out] rx_en:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_precode_get(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       bool *tx_en,
                                       bool *rx_en) {
  return port_mgr_tof2_serdes_precode_get(dev_id, dev_port, ln, tx_en, rx_en);
}

/** \brief Program precoder settings
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tx_en:
 * \param[out] rx_en:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_precode_set(bf_dev_id_t dev_id,
                                       bf_dev_port_t dev_port,
                                       uint32_t ln,
                                       bool tx_en,
                                       bool rx_en) {
  return port_mgr_tof2_serdes_precode_set(dev_id, dev_port, ln, tx_en, rx_en);
}

/** \brief Return FW precoder config settings
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] phy_mode_tx_en:
 * \param[out] phy_mode_rx_en:
 * \param[out] anlt_mode_tx_en:
 * \param[out] anlt_mode_rx_en:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fw_precode_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          bool *phy_mode_tx_en,
                                          bool *phy_mode_rx_en,
                                          bool *anlt_mode_tx_en,
                                          bool *anlt_mode_rx_en) {
  return port_mgr_tof2_serdes_fw_precode_get(dev_id,
                                             dev_port,
                                             ln,
                                             phy_mode_tx_en,
                                             phy_mode_rx_en,
                                             anlt_mode_tx_en,
                                             anlt_mode_rx_en);
}

/** \brief Configure FW precoder settings
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] phy_mode_tx_en:
 * \param[out] phy_mode_rx_en:
 * \param[out] anlt_mode_tx_en:
 * \param[out] anlt_mode_rx_en:
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fw_precode_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          uint32_t ln,
                                          bool phy_mode_tx_en,
                                          bool phy_mode_rx_en,
                                          bool anlt_mode_tx_en,
                                          bool anlt_mode_rx_en) {
  return port_mgr_tof2_serdes_fw_precode_set(dev_id,
                                             dev_port,
                                             ln,
                                             phy_mode_tx_en,
                                             phy_mode_rx_en,
                                             anlt_mode_tx_en,
                                             anlt_mode_rx_en);
}

/** \brief Return FFE values
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] k1       :
 * \param[out] k2       :
 * \param[out] k3       :
 * \param[out] k4       :
 * \param[out] s1       :
 * \param[out] s2       :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_ffe_taps_pam4_get(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             int32_t *k1,
                                             int32_t *k2,
                                             int32_t *k3,
                                             int32_t *k4,
                                             int32_t *s1,
                                             int32_t *s2) {
  return port_mgr_tof2_serdes_ffe_taps_pam4_get(
      dev_id, dev_port, ln, k1, k2, k3, k4, s1, s2);
}

/** \brief Issue group reset to all groups and tiles
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
static bf_status_t bf_tof2_serdes_group_reset_all(bf_dev_id_t dev_id) {
  uint32_t num_pipes;
  lld_err_t rc;
  uint32_t mac_id, ch;
  port_mgr_err_t sts;
  bf_dev_port_t cpu_port = lld_get_min_cpu_port(dev_id);

  rc = lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  if (rc != LLD_OK) return BF_INVALID_ARG;

  for (int phase = 0; phase < 3; phase++) {
    for (uint32_t pipe_id = 0; pipe_id < num_pipes; ++pipe_id) {
      for (int port_id = 8; port_id < 72; port_id += 8) {
        bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe_id, port_id);
        // make sure its valid
        sts = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
        if (sts != PORT_MGR_OK) return BF_INIT_ERROR;

        bf_tof2_serdes_group_reset_set(dev_id, dev_port, 0, phase);
      }
    }
    // The CPU port is always on logical pipe 0 port 2.
    bf_tof2_serdes_group_reset_set(dev_id, cpu_port, 0, phase);
    bf_sys_usleep(100000);
  }
  for (uint32_t pipe_id = 0; pipe_id < num_pipes; ++pipe_id) {
    for (int port_id = 8; port_id < 72; port_id += 8) {
      bf_dev_port_t dev_port = MAKE_DEV_PORT(pipe_id, port_id);
      // make sure its valid
      sts = port_mgr_tof2_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
      if (sts != PORT_MGR_OK) return BF_INIT_ERROR;

      port_mgr_tof2_serdes_init_log_to_phy_reg_range(dev_id, dev_port);
    }
  }
  // Once more for the Eth CPU port.
  port_mgr_tof2_serdes_init_log_to_phy_reg_range(dev_id, cpu_port);
  return BF_SUCCESS;
}

/** \brief Init the serdes tile chips (all ports)
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  fw_file_name      : PMA4+NRZ FW
 * \param[in]  fw_grp8_file_name : NRZ-only FW
 * \param[in]  skip_group_reset  : skip resetting group registers to defaults
 *(beware)
 * \param[in]  skip_post         : skip running POST on the ports
 * \param[in]  force_fw_dnld     : force FW dnld
 * \param[in]  skip_power_dn     : skip placing ports in power-down state at
 *completion
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_tile_init(bf_dev_id_t dev_id,
                                     char *fw_file_name,
                                     char *fw_grp8_file_name,
                                     bool skip_group_reset,
                                     bool skip_post,
                                     bool force_fw_dnld,
                                     bool skip_power_dn) {
  bf_dev_port_t dev_port;
  uint32_t port_id;
  uint32_t mac_id, ch;
  bf_status_t rc = BF_SUCCESS;
  port_mgr_err_t sts = PORT_MGR_OK;
  bool post_passed = false, sram_bist_passed = false, rom_bist_passed = false;
  uint32_t num_pipes = 0;
  bf_dev_port_t cpu_port = lld_get_min_cpu_port(dev_id);
  bf_dev_pipe_t cpu_pipe = DEV_PORT_TO_PIPE(cpu_port);

  /* log tile efuse data */
  port_mgr_log_tile_efuse(NULL, dev_id);

  /* We can determine the number of tiles packaged with the ASIC based on the
   * number of enabled pipes:
   *   4 pipes - 128Q or 64Q, 4 tiles
   *   3 pipes - 96T or 80T, 3 tiles (0, 1, and 3)
   *   2 pipes - 64D, 2 tiles (0 and 1)
   */
  lld_sku_get_num_active_pipes(dev_id, &num_pipes);

  if (!skip_post) {
    skip_group_reset = false; /* Group reset must be done after BIST. */
    /* Wipeout any prior config */
    bf_tof2_serdes_group_reset_all(dev_id);

    for (uint32_t log_pipe_id = 0; log_pipe_id < num_pipes; log_pipe_id++) {
      for (port_id = 0; port_id < 72; port_id += 8) {
        uint32_t n_lanes = 8;

        dev_port = MAKE_DEV_PORT(log_pipe_id, port_id);
        /* Only one logical pipe has the CPU port which is represented as
         * port_id == 0 in this loop.  The other logical pipes do not have a MAC
         * to access those serdes lanes. */
        if (port_id == 0) {
          if (log_pipe_id != cpu_pipe) continue;
          dev_port = cpu_port;
          n_lanes = 4;
        }

        rc = bf_tof2_serdes_power_on_self_test(dev_id, dev_port, n_lanes);
        post_passed = (rc == BF_SUCCESS);

        rc = bf_tof2_serdes_sram_bist_grp(dev_id, dev_port);
        sram_bist_passed = (rc == BF_SUCCESS);

        rc = bf_tof2_serdes_rom_bist_grp(dev_id, dev_port);
        rom_bist_passed = (rc == BF_SUCCESS);

        port_mgr_log("%d:%3d: SRAM BIST %s : ROM BIST %s : POST %s",
                     dev_id,
                     dev_port,
                     sram_bist_passed ? "  Ok" : "FAIL",
                     rom_bist_passed ? "  Ok" : "FAIL",
                     post_passed ? "  Ok" : "FAIL");
      }
    }
  }
  if (!skip_group_reset) {
    bf_tof2_serdes_group_reset_all(dev_id);
  }

  // Download the tile firmware.  Again, loop over logical pipes here since
  // unused tiles do not need their firmware loaded.
  for (uint32_t log_pipe_id = 0; log_pipe_id < num_pipes; log_pipe_id++) {
    for (port_id = 0; port_id < 72; port_id += 8) {
      char *fw_file_nm;

      if ((port_id == 0) && (log_pipe_id != cpu_pipe)) continue;
      if ((port_id == 0) && (log_pipe_id == cpu_pipe)) {
        fw_file_nm = fw_grp8_file_name;  // special FW for CPU port (NRZ only)
        dev_port = cpu_port;
      } else {
        fw_file_nm = fw_file_name;
        dev_port = MAKE_DEV_PORT(log_pipe_id, port_id);
      }
      // make sure its valid
      sts = port_mgr_tof2_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
      if (sts != PORT_MGR_OK) {
        port_mgr_log("%d:%3d: %s group init: Invalid dev_port : sts=%d",
                     dev_id,
                     dev_port,
                     __func__,
                     sts);
        // These are all valid dev_ports so it is a fatal error if we cannot
        // find the MAC and channel needed to download firmware.
        return BF_INIT_ERROR;
      }
      rc = bf_tof2_serdes_init_group(
          dev_id, dev_port, fw_file_nm, force_fw_dnld);
      if (rc != BF_SUCCESS) {
        port_mgr_log("%d:%3d: bf_tof2_serdes_init_group failed: rc=%d",
                     dev_id,
                     dev_port,
                     rc);
        return BF_INIT_ERROR;
      }
    }
  }

  // Configure lane mode only on the tiles that will be used.
  for (uint32_t log_pipe_id = 0; log_pipe_id < num_pipes; log_pipe_id++) {
    for (port_id = 0; port_id < 72; port_id += 8) {
      uint32_t ln, n_lanes;

      if ((port_id == 0) && (log_pipe_id != cpu_pipe)) continue;
      if ((port_id == 0) && (log_pipe_id == cpu_pipe)) {
        n_lanes = 4;
        dev_port = cpu_port;
      } else {
        n_lanes = 8;
        dev_port = MAKE_DEV_PORT(log_pipe_id, port_id);
      }
      // make sure its valid
      sts = port_mgr_tof2_map_dev_port_to_all(
          dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
      if (sts != PORT_MGR_OK) {
        port_mgr_log("%d:%3d: %s ln cfg: invalid dev_port : sts=%d",
                     dev_id,
                     dev_port,
                     __func__,
                     sts);
        // These are all valid dev_ports so it is a fatal error if we cannot
        // find the MAC and channel needed to configure the firmware.
        return BF_INIT_ERROR;
      }

      for (ln = 0; ln < n_lanes; ln++) {
        port_mgr_tof2_serdes_logic_reset(dev_id, dev_port, ln);
      }
    }
  }

  if (!skip_power_dn) {
    // For 128Q all tiles will be powered on.
    // For 64Q the tiles for external pipes will be powered on but the two tiles
    // connected to internal pipes will be powered down.
    // For 64D there are only two real tiles that must be kept powered on, the
    // other two tiles are dummy and should not be accessed.
    // For 96T the fourth tile is dummy and should not be accessed.
    // For 80T the fourth tile is dummy and should not be accessed.  However,
    // there are four MAC blocks which are disabled and those serdes lanes
    // should be powered down.
    uint32_t sku = lld_sku_get_sku(dev_id);
    bool is_80T = (sku == BFN_SKU_BFN0080T) || (sku == BFN_SKU_BFN0080TM);
    bool is_64Q = (sku == BFN_SKU_BFN0064Q);

    // First power down everything and then power on the lanes we need.  This
    // gives us a consistent sequence no matter the SKU.  Limit this power-down
    // for loop based on the number of working tiles.  Note the number of tiles
    // is the same as the number of logical pipes.
    for (uint32_t log_pipe_id = 0; log_pipe_id < num_pipes; log_pipe_id++) {
      for (port_id = 0; port_id < 72; port_id += 8) {
        uint32_t ln, n_lanes = 8;

        // The four CPU port lanes can only be accessed on logical pipe zero
        // since that is where there is a MAC block which is required to access
        // them.
        if ((port_id == 0) && (log_pipe_id != cpu_pipe)) continue;
        if ((port_id == 0) && (log_pipe_id == cpu_pipe)) {
          n_lanes = 4;
          dev_port = cpu_port;
        } else {
          n_lanes = 8;
          dev_port = MAKE_DEV_PORT(log_pipe_id, port_id);
        }

        port_mgr_log("%d:%3d: %s Power-dn", dev_id, dev_port, __func__);
        for (ln = 0; ln < n_lanes; ln++) {
          bf_tof2_serdes_power_dn_set(
              dev_id, dev_port, ln, true, true, true, true);
        }
      }
    }

    // power-up the tiles and lanes needed.
    for (uint32_t log_pipe_id = 0; log_pipe_id < num_pipes; log_pipe_id++) {
      if (is_64Q) {
        // Only power up lanes on pipes 0 and 1.
        if (log_pipe_id > 1) break;
      }
      for (port_id = 0; port_id < 72; port_id += 8) {
        uint32_t ln, n_lanes;

        if (is_80T) {
          // 80T will disable port 32 in logical pipes 0, 1, and 2 (all pipes).
          // 80T will disable port 64 in logical pipe 2.
          if (port_id == 32) continue;
          if (log_pipe_id == 2 && port_id == 64) continue;
        }
        // CPU port is only connected to the first tile.
        if ((port_id == 0) && (log_pipe_id != cpu_pipe)) continue;
        if ((port_id == 0) && (log_pipe_id == cpu_pipe)) {
          n_lanes = 4;
          dev_port = cpu_port;
        } else {
          n_lanes = 8;
          dev_port = MAKE_DEV_PORT(log_pipe_id, port_id);
        }

        // make sure its valid
        sts = port_mgr_tof2_map_dev_port_to_all(
            dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
        if (sts != PORT_MGR_OK) {
          port_mgr_log("%d:%3d: %s Power-up Failed : Invalid dev port sts=%d",
                       dev_id,
                       port_id,
                       __func__,
                       sts);
          return BF_INIT_ERROR;
        }

        port_mgr_log("%d:%3d: %s : Power-up", dev_id, dev_port, __func__);
        for (ln = 0; ln < n_lanes; ln++) {
          bf_tof2_serdes_power_dn_set(
              dev_id, dev_port, ln, false, false, false, false);
        }
      }
    }
  }
  return rc;
}

/** \brief Restart Rx adaptation on a lane
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_lane_reset_set(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int ln) {
  return port_mgr_tof2_serdes_lane_reset_set(dev_id, dev_port, ln);
}

/** \brief Return lane speed and encoding mode
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out]  G       : speed in gigabits
 * \param[out]  enc_mode: NRZ or PAM4
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fw_lane_speed_get(
    bf_dev_id_t dev_id,
    bf_dev_port_t dev_port,
    int ln,
    uint32_t *G,
    bf_serdes_encoding_mode_t *enc_mode) {
  return port_mgr_tof2_serdes_fw_lane_speed_get(
      dev_id, dev_port, ln, G, enc_mode);
}

/** \brief Return F13 value
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] f13      : F13
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_f13_val_pam4_get(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t *f13_val) {
  return port_mgr_tof2_serdes_f13_val_pam4_get(dev_id, dev_port, ln, f13_val);
}

/** \brief Return whether FW is loaded or not.
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] loaded   :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fw_loaded_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         bool *loaded) {
  return port_mgr_tof2_serdes_fw_loaded_get(dev_id, dev_port, loaded);
}

/** \brief Return FEC aalyzer tei
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] tei      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fec_analyzer_tei_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *tei) {
  return port_mgr_tof2_serdes_fec_analyzer_tei_get(dev_id, dev_port, ln, tei);
}

/** \brief Return FEC aalyzer teo
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] teo      :
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fec_analyzer_teo_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port,
                                                uint32_t ln,
                                                uint32_t *teo) {
  return port_mgr_tof2_serdes_fec_analyzer_teo_get(dev_id, dev_port, ln, teo);
}

/** \brief Initialize the FEC analyzer
 *
 *
 * \param[in]  dev_id   : System-assigned identifier (0..BFN_MAX_ASICS-1)
 * \param[in]  dev_port :
 * \param[in]  ln       :
 * \param[out] err_type :
 * \param[out] T        : number of correctable symbols
 * \param[out] M        : message symbol bits
 * \param[out] N        : number of symbols per FEC block
 *
 * \return: BF_SUCCESS    :
 * \return: BF_INVALID_ARG: dev_id never added or dev_id > BF_MAX_DEV_COUNT-1
 * \return: BF_INVALID_ARG: invalid or un-added bf_dev_port_t
 */
bf_status_t bf_tof2_serdes_fec_analyzer_init_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln,
                                                 uint32_t err_type,
                                                 uint32_t T,
                                                 uint32_t M,
                                                 uint32_t N) {
  return port_mgr_tof2_serdes_fec_analyzer_init_set(
      dev_id, dev_port, ln, err_type, T, M, N);
}

/** \brief Retrieve AN status fields
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_an_status_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         uint32_t *lp_an_ability,
                                         uint32_t *link_status,
                                         uint32_t *an_ability,
                                         uint32_t *remote_fault,
                                         uint32_t *an_complete,
                                         uint32_t *page_rcvd,
                                         uint32_t *ext_np_status,
                                         uint32_t *parallel_detect_fault) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_an_status_get(dev_id,
                                          dev_port,
                                          ln,
                                          lp_an_ability,
                                          link_status,
                                          an_ability,
                                          remote_fault,
                                          an_complete,
                                          page_rcvd,
                                          ext_np_status,
                                          parallel_detect_fault);
  return rc;
}

/** \brief Retrieve AN lp base page
 *
 * \param dev_id    : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port  : encoded port identifier
 * \param base_page :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_an_lp_base_page_get(bf_dev_id_t dev_id,
                                               bf_dev_port_t dev_port,
                                               uint64_t *lp_base_page) {
  bf_status_t rc;

  if (!lp_base_page) return BF_INVALID_ARG;

  rc = port_mgr_tof2_serdes_an_lp_base_page_get(dev_id, dev_port, lp_base_page);

  return rc;
}

/** \brief Retrieve AN lp pages
 *
 * \param dev_id       : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port     : encoded port identifier
 * \param lp_base_page : Link partner base page
 * \param lp_nxt_page1 : Link partner next page #1
 * \param lp_nxt_page2 : Link partner next page #2
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_an_lp_pages_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint64_t *lp_base_page,
                                           uint64_t *lp_nxt_page1,
                                           uint64_t *lp_nxt_page2) {
  bf_status_t rc;

  if (!lp_base_page || !lp_nxt_page1 || !lp_nxt_page2) return BF_INVALID_ARG;

  rc = port_mgr_tof2_serdes_an_lp_pages_get(
      dev_id, dev_port, lp_base_page, lp_nxt_page1, lp_nxt_page2);

  return rc;
}

/** \brief Retrieve AN HCD (highest common denomiator) speed
 *
 * \param dev_id    : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port  : encoded port identifier
 * \param ln        :
 * \param hcd       :
 * \param base_r_fec:
 * \param rs_fec    :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_an_hcd_get(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      uint32_t ln,
                                      uint32_t *hcd,
                                      bool *base_r_fec,
                                      bool *rs_fec) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_an_hcd_get(
      dev_id, dev_port, ln, hcd, base_r_fec, rs_fec);
  return rc;
}

/** \brief Retrieve Link-training status fields
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_lt_status_get(bf_dev_id_t dev_id,
                                         bf_dev_port_t dev_port,
                                         uint32_t ln,
                                         uint32_t *readout_state,
                                         uint32_t *frame_lock,
                                         uint32_t *rx_trained,
                                         uint32_t *readout_training_state,
                                         uint32_t *training_failure,
                                         uint32_t *tx_training_data_en,
                                         uint32_t *sig_det,
                                         uint32_t *readout_txstate) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_lt_status_get(dev_id,
                                          dev_port,
                                          ln,
                                          readout_state,
                                          frame_lock,
                                          rx_trained,
                                          readout_training_state,
                                          training_failure,
                                          tx_training_data_en,
                                          sig_det,
                                          readout_txstate);
  return rc;
}

/** \brief Execute a Credo FW cmd
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_fw_cmd(bf_dev_id_t dev_id,
                                  bf_dev_port_t dev_port,
                                  int ln,
                                  uint32_t cmd,
                                  uint32_t detail,
                                  uint32_t *result) {
  bf_status_t rc;

  // bf_status_t port_mgr_tof2_serdes_fw_cmd_w_detail(bf_dev_id_t dev_id,
  //                                                 bf_dev_port_t dev_port,
  //                                                 int ln,
  //                                                 uint32_t cmd,
  //                                                 uint32_t *rsp,
  //                                                 uint32_t detail)
  rc = port_mgr_tof2_serdes_fw_cmd_w_detail(
      dev_id, dev_port, ln, cmd, result, detail);
  return rc;
}

/** \brief Execute a Credo FW debug cmd
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_fw_debug_cmd(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        int ln,
                                        uint32_t section,
                                        uint32_t index,
                                        uint32_t *result) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_debug_cmd(
      dev_id, dev_port, ln, section, index, result);
  return rc;
}

/** \brief Retrieve ISI info
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_isi_get(bf_dev_id_t dev_id,
                                   bf_dev_port_t dev_port,
                                   uint32_t ln,
                                   uint32_t isi_vals[16]) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_isi_get(dev_id, dev_port, ln, isi_vals);
  return rc;
}

/** \brief Retrieve ISI info
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_error_inject_set(bf_dev_id_t dev_id,
                                            bf_dev_port_t dev_port,
                                            uint32_t ln,
                                            uint32_t n_errs) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_error_inject_set(dev_id, dev_port, ln, n_errs);
  return rc;
}

/** \brief Retrieve PLL info
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_pll_info_get(bf_dev_id_t dev_id,
                                        bf_dev_port_t dev_port,
                                        uint32_t ln,
                                        pll_info_t *tx_pll_info,
                                        pll_info_t *rx_pll_info) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_pll_info_get(
      dev_id, dev_port, ln, tx_pll_info, rx_pll_info);
  return rc;
}

/** \brief Start a temperature read operation
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_temperature_start_set(bf_dev_id_t dev_id,
                                                 bf_dev_port_t dev_port,
                                                 uint32_t ln) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_temperature_start_set(dev_id, dev_port, true);

  return rc;
  (void)ln;
}

/** \brief Read the status if a previously started a temperature read operation
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 * \param Out     : temperature (in degrees C)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_temperature_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           float *temp) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_temperature_get(dev_id, dev_port, true, temp);

  return rc;
  (void)ln;
}

/** \brief Read the temperature as measured by the FW
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param Out     : temperature (in degrees C)
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_fw_temperature_get(bf_dev_id_t dev_id,
                                              bf_dev_port_t dev_port,
                                              float *temp) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_temperature_get(dev_id, dev_port, temp);

  return rc;
}

/** \brief FW reg write with section
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_fw_reg_section_wr(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t addr,
                                             uint32_t data,
                                             uint32_t section) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_reg_section_wr(
      dev_id, dev_port, ln, addr, data, section);
  return rc;
}

/** \brief FW reg read with section
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_fw_reg_section_rd(bf_dev_id_t dev_id,
                                             bf_dev_port_t dev_port,
                                             uint32_t ln,
                                             uint32_t addr,
                                             uint32_t *data,
                                             uint32_t section) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_fw_reg_section_rd(
      dev_id, dev_port, ln, addr, data, section);
  return rc;
}

/** \brief Get Eye plot from serdes FW
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param ln      :
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_fw_eye_plot_get(bf_dev_id_t dev_id,
                                           bf_dev_port_t dev_port,
                                           uint32_t ln,
                                           uint8_t **plot_data) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_eye_plot_get(dev_id, dev_port, ln, plot_data);
  return rc;
}

/** \brief Read tile Efuse value
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param bank    : "addr_pins" specifying efuse attribute to read
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_tile_efuse_get(bf_dev_id_t dev_id,
                                          bf_dev_port_t dev_port,
                                          int bank,
                                          uint32_t *efuse_val) {
  if (!efuse_val) return BF_INVALID_ARG;

  *efuse_val = port_mgr_tof2_serdes_tile_efuse_get(dev_id, dev_port, bank);
  return BF_SUCCESS;
}

/*
 * FF parts range from700-800.
 * TT parts range from 750-900.
 * SS parts range from 800-1000
 */
static bf_serdes_process_corner_t bf_tof2_serdes_part_corner(uint32_t dro) {
  if (dro < 700) {
    return PROCESS_CORNER_UNDEF;  // probably early part, DRO not ptogrammed
  } else if (dro < 750) {
    return PROCESS_CORNER_FF;  // 700 - 749
  } else if (dro < 816) {
    return PROCESS_CORNER_TT;  // 750 - 815
  } else if (dro < 1024) {
    return PROCESS_CORNER_SS;  // 815 - 1023
  }
  return PROCESS_CORNER_UNDEF;  // not supposed to be possible
}

/** \brief Tile DRO value get
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param bank    : "addr_pins" specifying efuse attribute to read
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_tile_dro_get(bf_dev_id_t dev_id,
                                        uint32_t dro[4],
                                        int32_t *max_dro,
                                        uint32_t *num_dro_values) {
  uint32_t dro_raw[4] = {0};     // one per tile, raw efuse value
  uint32_t dro_0, dro_1, dro_2;  // 3 redundant 10b values
  uint32_t pipe_id, num_pipes = 0;
  int bank = 3 * 0x20;

  lld_sku_get_num_active_pipes(dev_id, &num_pipes);
  *num_dro_values = num_pipes;

  for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
    bf_dev_port_t dev_port;
    uint32_t port_id = 8;
    uint32_t mac_id, ch, rc;

    dev_port = MAKE_DEV_PORT(pipe_id, port_id);
    // make sure its valid
    rc = port_mgr_tof2_map_dev_port_to_all(
        dev_id, dev_port, NULL, NULL, &mac_id, &ch, NULL);
    if (rc != 0) continue;

    dro_raw[pipe_id] =
        port_mgr_tof2_serdes_tile_efuse_get(dev_id, dev_port, bank);
  }
  *max_dro = 0;
  for (pipe_id = 0; pipe_id < num_pipes; pipe_id++) {
    dro_0 = (dro_raw[pipe_id] >> 22) & 0x3FF;
    dro_1 = (dro_raw[pipe_id] >> 12) & 0x3FF;
    dro_2 = (dro_raw[pipe_id] >> 2) & 0x3FF;
    // vote
    if (dro_0 == dro_1) {
      dro[pipe_id] = dro_0;
    } else if (dro_0 == dro_2) {
      dro[pipe_id] = dro_0;
    } else if (dro_1 == dro_2) {
      dro[pipe_id] = dro_1;
    } else {
      dro[pipe_id] = 0xffffffff;  // indeterminate
    }
    // handle case where DRO value is not populated (early wafers)
    if (dro[pipe_id] == 0) {
      dro[pipe_id] = 0xfffffffe;  // un-populated
    }
    if ((int32_t)dro[pipe_id] > *max_dro) {
      *max_dro = (int32_t)dro[pipe_id];
    }
  }
  return BF_SUCCESS;
}

/** \brief Part type (ss/tt/ff)  get
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param bank    : "addr_pins" specifying efuse attribute to read
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_part_type_get(
    bf_dev_id_t dev_id, bf_serdes_process_corner_t *part_type) {
  uint32_t dro[4] = {0};  // one per tile, most-likely DRO value
  int32_t max_dro = 0;
  uint32_t num_dro_values = 0;
  bf_status_t rc;

  if (!part_type) return BF_INVALID_ARG;

  rc = bf_tof2_serdes_tile_dro_get(dev_id, dro, &max_dro, &num_dro_values);
  if (rc != BF_SUCCESS) {
    *part_type = PROCESS_CORNER_UNDEF;
  } else {
    // determine corner based on max DRO value
    *part_type = bf_tof2_serdes_part_corner(max_dro);
  }
  (void)dro;
  (void)num_dro_values;
  return BF_SUCCESS;
}

/** \brief Read known value from tile register
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_serdes_tile_known_value_get(bf_dev_id_t dev_id,
                                                bf_dev_port_t dev_port) {
  bf_status_t rc;

  rc = port_mgr_tof2_serdes_known_value_get(dev_id, dev_port);
  return rc;
}

#ifndef TILE_SIM

/** \brief tof-2 clkobs pad config
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param dev_port: encoded port identifier
 * \param pad: BF_CLKOBS_PAD_0 or BF_CLKOBS_PAD_1
 * \param clk_src: BF_SDS_NONE_CLK or BF_SDS_RX_RECOVEREDCLK or BF_SDS_TX_CLK
 * \param divider: 0,1,2, 3 for div by 2,4,8,16 respectively
 *
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 * \return: BF_HW_UPDATE_FAILED: failure to apply config
 */
bf_status_t bf_tof2_serdes_clkobs_set(bf_dev_id_t dev_id,
                                      bf_dev_port_t dev_port,
                                      bf_clkobs_pad_t pad,
                                      bf_sds_clkobs_clksel_t clk_src,
                                      int divider,
                                      int daisy_sel) {
  return port_mgr_tof2_serdes_clkobs_set(
      dev_id, dev_port, pad, clk_src, divider, daisy_sel);
}

/** \brief tof-2 clkobs drive strength config
 *
 * \param dev_id  : system-assigned identifier (0..BF_MAX_DEV_COUNT-1)
 * \param drive_strength: Clock observation pad drive strength. (0 ~ 15)
 * \return: BF_SUCCESS
 * \return: BF_INVALID_ARG:
 */
bf_status_t bf_tof2_clkobs_drive_strength_set(bf_dev_id_t dev_id,
                                              int drive_strength) {
  return port_mgr_tof2_clkobs_drive_strength_set(dev_id, drive_strength);
}

#endif
