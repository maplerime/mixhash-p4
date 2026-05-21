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

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <lld/lld_reg_if.h>
#include <tof2_regs/tof2_reg_drv.h>
#include <port_mgr/port_mgr_log.h>
#include "autogen-required-headers.h"
#include "umac4_ctrs.h"
#include "umac4_ctrs_str.h"

extern void u64_ctr_display(int n,
                            uint64_t *ctr_array,
                            char *name_array[],
                            char *desc_array[]);

/********************************************************************
 * umac4_ctrs_rmon_ctr_get
 ********************************************************************/
uint64_t umac4_ctrs_rmon_ctr_get(bf_dev_id_t dev_id,
                                 uint32_t umac,
                                 uint32_t ch,
                                 umac4_rmon_ctr_e ctr) {
  uint64_t ctr_val;

  umac4_rd64(dev_id,
             umac,
             (ch * 0x400) + UMAC4_CTR_BASE_RMON + (ctr * sizeof(uint64_t)),
             &ctr_val);
  return ctr_val;
}

/********************************************************************
 * umac4_ctrs_rmon_get
 ********************************************************************/
void umac4_ctrs_rmon_get(bf_dev_id_t dev_id,
                         uint32_t umac,
                         uint32_t ch,
                         umac4_rmon_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = (uint64_t *)ctrs;

  for (ctr = 0; ctr < sizeof(*ctrs) / sizeof(uint64_t); ctr++) {
    *ctr_p = umac4_ctrs_rmon_ctr_get(dev_id, umac, ch, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_rmon_dump
 ********************************************************************/
void umac4_ctrs_rmon_dump(bf_dev_id_t dev_id, uint32_t umac, uint32_t ch) {
  umac4_rmon_ctr_t ctrs;
  char **ctr_name_strs = umac4_rmon_ctr_name_strs();
  char **ctr_desc_strs = umac4_rmon_ctr_desc_strs();

  umac4_ctrs_rmon_get(dev_id, umac, ch, &ctrs);

  u64_ctr_display(sizeof(ctrs) / sizeof(uint64_t),
                  (uint64_t *)&ctrs,
                  ctr_name_strs,
                  ctr_desc_strs);
}

/********************************************************************
 * umac4_ctrs_pcs_ctr_get
 ********************************************************************/
uint64_t umac4_ctrs_pcs_ctr_get(bf_dev_id_t dev_id,
                                uint32_t umac,
                                uint32_t ch,
                                umac4_pcs_ctr_e ctr) {
  uint64_t ctr_val;

  umac4_rd64(dev_id,
             umac,
             (ch * 0x400) + UMAC4_CTR_BASE_PCS + (ctr * sizeof(uint64_t)),
             &ctr_val);
  // and clear it
  if (ctr_val > 0ul) {
    umac4_wr64(dev_id,
               umac,
               (ch * 0x400) + UMAC4_CTR_BASE_PCS + (ctr * sizeof(uint64_t)),
               0ul);
  }
  return ctr_val;
}

/********************************************************************
 * umac4_ctrs_pcs_get
 ********************************************************************/
void umac4_ctrs_pcs_get(bf_dev_id_t dev_id,
                        uint32_t umac,
                        uint32_t ch,
                        umac4_pcs_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = (uint64_t *)ctrs;

  for (ctr = 0; ctr < sizeof(*ctrs) / sizeof(uint64_t); ctr++) {
    *ctr_p = umac4_ctrs_pcs_ctr_get(dev_id, umac, ch, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_pcs_dump
 ********************************************************************/
void umac4_ctrs_pcs_dump(bf_dev_id_t dev_id, uint32_t umac, uint32_t ch) {
  umac4_pcs_ctr_t ctrs;
  char **ctr_name_strs = umac4_pcs_ctr_name_strs();
  char **ctr_desc_strs = umac4_pcs_ctr_desc_strs();

  umac4_ctrs_pcs_get(dev_id, umac, ch, &ctrs);

  u64_ctr_display(sizeof(ctrs) / sizeof(uint64_t),
                  (uint64_t *)&ctrs,
                  ctr_name_strs,
                  ctr_desc_strs);
}

/********************************************************************
 * umac4_ctrs_rs_fec_ctr_address_get
 ********************************************************************/
uint32_t umac4_ctrs_rs_fec_ctr_address_get(bf_dev_id_t dev_id,
                                           uint32_t umac,
                                           uint32_t ch,
                                           umac4_rs_fec_ln_ctr_e ctr) {
  return port_mgr_umac_address_get(
      dev_id,
      umac,
      (ch * 0x400) + UMAC4_CTR_BASE_RS_FEC + (ctr * sizeof(uint64_t)));
}

/********************************************************************
 * umac4_ctrs_rs_fec_ctr_get
 ********************************************************************/
uint64_t umac4_ctrs_rs_fec_ctr_get(bf_dev_id_t dev_id,
                                   uint32_t umac,
                                   uint32_t ch,
                                   umac4_rs_fec_ctr_e ctr) {
  uint64_t ctr_val;

  umac4_rd64(dev_id,
             umac,
             (ch * 0x400) + UMAC4_CTR_BASE_RS_FEC + (ctr * sizeof(uint64_t)),
             &ctr_val);
  // and clear it
  if (ctr_val > 0ul) {
    umac4_wr64(dev_id,
               umac,
               (ch * 0x400) + UMAC4_CTR_BASE_RS_FEC + (ctr * sizeof(uint64_t)),
               0ul);
  }
  return ctr_val;
}

/********************************************************************
 * umac4_ctrs_rs_fec_get
 ********************************************************************/
void umac4_ctrs_rs_fec_get(bf_dev_id_t dev_id,
                           uint32_t umac,
                           uint32_t ch,
                           umac4_rs_fec_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = (uint64_t *)ctrs;

  for (ctr = 0; ctr < sizeof(*ctrs) / sizeof(uint64_t); ctr++) {
    *ctr_p = umac4_ctrs_rs_fec_ctr_get(dev_id, umac, ch, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_rs_fec_dump
 ********************************************************************/
void umac4_ctrs_rs_fec_dump(bf_dev_id_t dev_id, uint32_t umac, uint32_t ch) {
  umac4_rs_fec_ctr_t ctrs;
  char **ctr_name_strs = umac4_rs_fec_ctr_name_strs();
  char **ctr_desc_strs = umac4_rs_fec_ctr_desc_strs();

  umac4_ctrs_rs_fec_get(dev_id, umac, ch, &ctrs);

  u64_ctr_display(sizeof(ctrs) / sizeof(uint64_t),
                  (uint64_t *)&ctrs,
                  ctr_name_strs,
                  ctr_desc_strs);
}

// These counters are discontiguous in the address space
uint32_t pcs_vl_ctr_ofs[] = {
    0xC370, 0xC378, 0xC380, 0xC388, 0xC390, 0xC398, 0xC3A0, 0xC3A8, 0xC3B0,
    0xC3B8, 0xC3C0, 0xC3C8, 0xC3D0, 0xC3D8, 0xC3E0, 0xC3E8, 0xC3F0, 0xC3F8,
    0xC770, 0xC778, 0xC780, 0xC788, 0xC790, 0xC798, 0xC7A0, 0xC7A8, 0xC7B0,
    0xC7B8, 0xC7C0, 0xC7C8, 0xC7D0, 0xC7D8, 0xC7E0, 0xC7E8, 0xC7F0, 0xC7F8,
    0xCB70, 0xCB78, 0xCB80, 0xCB88, 0xCB90, 0xCB98, 0xCBA0, 0xCBA8, 0xCBB0,
    0xCBB8, 0xCBC0, 0xCBC8, 0xCBD0, 0xCBD8, 0xCBE0, 0xCBE8, 0xCBF0, 0xCBF8,
    0xCF70, 0xCF78, 0xCF80, 0xCF88, 0xCF90, 0xCF98, 0xCFA0, 0xCFA8, 0xCFB0,
    0xCFB8, 0xCFC0, 0xCFC8, 0xCFD0, 0xCFD8, 0xCFE0, 0xCFE8, 0xCFF0, 0xCFF8,
    0xD370, 0xD378, 0xD380, 0xD388, 0xD390, 0xD398, 0xD3A0, 0xD3A8};

/********************************************************************
 * umac4_ctrs_pcs_vl_ctr_get
 ********************************************************************/
uint64_t umac4_ctrs_pcs_vl_ctr_get(bf_dev_id_t dev_id,
                                   uint32_t umac,
                                   umac4_pcs_vl_ctr_e ctr) {
  uint64_t ctr_val;
  uint32_t ofs = pcs_vl_ctr_ofs[ctr];

  umac4_rd64(dev_id, umac, ofs, &ctr_val);

  // and clear it
  if (ctr_val > 0ul) {
    umac4_wr64(dev_id, umac, ofs, 0ul);
  }
  return ctr_val;
}

/********************************************************************
 * umac4_ctrs_pcs_vl_get
 ********************************************************************/
void umac4_ctrs_pcs_vl_get(bf_dev_id_t dev_id,
                           uint32_t umac,
                           umac4_pcs_vl_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = (uint64_t *)ctrs;

  for (ctr = 0; ctr < sizeof(*ctrs) / sizeof(uint64_t); ctr++) {
    *ctr_p = umac4_ctrs_pcs_vl_ctr_get(dev_id, umac, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_pcs_vl_dump
 ********************************************************************/
void umac4_ctrs_pcs_vl_dump(bf_dev_id_t dev_id, uint32_t umac) {
  umac4_pcs_vl_ctr_t ctrs;
  char **ctr_name_strs = umac4_pcs_vl_ctr_name_strs();
  char **ctr_desc_strs = umac4_pcs_vl_ctr_desc_strs();

  umac4_ctrs_pcs_vl_get(dev_id, umac, &ctrs);

  u64_ctr_display(sizeof(ctrs) / sizeof(uint64_t),
                  (uint64_t *)&ctrs,
                  ctr_name_strs,
                  ctr_desc_strs);
}

/********************************************************************
 * umac4_ctrs_rs_fec_ln_ctr_address_get
 ********************************************************************/
uint32_t umac4_ctrs_rs_fec_ln_ctr_address_get(bf_dev_id_t dev_id,
                                              uint32_t umac,
                                              umac4_rs_fec_ln_ctr_e ctr) {
  return port_mgr_umac_address_get(
      dev_id, umac, UMAC4_CTR_BASE_RS_FEC_LN + (ctr * sizeof(uint64_t)));
}

/********************************************************************
 * umac4_ctrs_rs_fec_ln_ctr_get
 ********************************************************************/
uint64_t umac4_ctrs_rs_fec_ln_ctr_get(bf_dev_id_t dev_id,
                                      uint32_t umac,
                                      umac4_rs_fec_ln_ctr_e ctr) {
  uint64_t ctr_val;

  umac4_rd64(dev_id,
             umac,
             UMAC4_CTR_BASE_RS_FEC_LN + (ctr * sizeof(uint64_t)),
             &ctr_val);
  // and clear it
  if (ctr_val > 0ul) {
    umac4_wr64(
        dev_id, umac, UMAC4_CTR_BASE_RS_FEC_LN + (ctr * sizeof(uint64_t)), 0ul);
  }
  return ctr_val;
}

/********************************************************************
 * umac4_ctrs_rs_fec_ln_get
 ********************************************************************/
void umac4_ctrs_rs_fec_ln_get(bf_dev_id_t dev_id,
                              uint32_t umac,
                              umac4_rs_fec_ln_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = (uint64_t *)ctrs;

  for (ctr = 0; ctr < sizeof(*ctrs) / sizeof(uint64_t); ctr++) {
    *ctr_p = umac4_ctrs_rs_fec_ln_ctr_get(dev_id, umac, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_rs_fec_ln_range_get
 ********************************************************************/
void umac4_ctrs_rs_fec_ln_range_get(bf_dev_id_t dev_id,
                                    uint32_t umac,
                                    uint32_t start_ctr,
                                    uint32_t n_ctr,
                                    umac4_rs_fec_ln_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = ((uint64_t *)ctrs) + start_ctr;

  for (ctr = start_ctr; ctr < (start_ctr + n_ctr); ctr++) {
    *ctr_p = umac4_ctrs_rs_fec_ln_ctr_get(dev_id, umac, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_rs_fec_ln_dump
 ********************************************************************/
void umac4_ctrs_rs_fec_ln_dump(bf_dev_id_t dev_id, uint32_t umac) {
  umac4_rs_fec_ln_ctr_t ctrs;
  char **ctr_name_strs = umac4_rs_fec_ln_ctr_name_strs();
  char **ctr_desc_strs = umac4_rs_fec_ln_ctr_desc_strs();

  umac4_ctrs_rs_fec_ln_get(dev_id, umac, &ctrs);

  u64_ctr_display(sizeof(ctrs) / sizeof(uint64_t),
                  (uint64_t *)&ctrs,
                  ctr_name_strs,
                  ctr_desc_strs);
}

// These counters are discontiguous in the address space
uint32_t fc_fec_ln_ctr_ofs[] = {
    0xD3B0, 0xD3B8, 0xD3C0, 0xD3C8, 0xD3D0, 0xD3D8, 0xD3E0, 0xD3E8,
    0xD3F0, 0xD3F8, 0xD770, 0xD778, 0xD780, 0xD788, 0xD790, 0xD798,
    0xD7A0, 0xD7A8, 0xD7B0, 0xD7B8, 0xD7C0, 0xD7C8, 0xD7D0, 0xD7D8,
    0xD7E0, 0xD7E8, 0xD7F0, 0xD7F8, 0xDB70, 0xDB78, 0xDB80, 0xDB88,
    0xDB90, 0xDB98, 0xDBA0, 0xDBA8, 0xDBB0, 0xDBB8, 0xDBC0, 0xDBC8,
    0xDBD0, 0xDBD8, 0xDBE0, 0xDBE8, 0xDBF0, 0xDBF8, 0xDF70, 0xDF78,
    0xDF80, 0xDF88, 0xDF90, 0xDF98, 0xDFA0, 0xDFA8, 0xDFB0, 0xDFB8,
    0xDFC0, 0xDFC8, 0xDFD0, 0xDFD8, 0xDFE0, 0xDFE8, 0xDFF0, 0xDFF8};

/********************************************************************
 * umac4_ctrs_fc_fec_ln_ctr_get
 ********************************************************************/
uint64_t umac4_ctrs_fc_fec_ln_ctr_get(bf_dev_id_t dev_id,
                                      uint32_t umac,
                                      umac4_fc_fec_ln_ctr_e ctr) {
  uint64_t ctr_val;
  uint32_t ofs = fc_fec_ln_ctr_ofs[ctr];

  umac4_rd64(dev_id, umac, UMAC4_CTR_BASE_FC_FEC_LN + ofs, &ctr_val);
  // and clear it
  if (ctr_val > 0ul) {
    umac4_wr64(dev_id, umac, UMAC4_CTR_BASE_FC_FEC_LN + ofs, 0ul);
  }
  return ctr_val;
}

/********************************************************************
 * umac4_ctrs_fc_fec_ln_get
 ********************************************************************/
void umac4_ctrs_fc_fec_ln_get(bf_dev_id_t dev_id,
                              uint32_t umac,
                              umac4_fc_fec_ln_ctr_t *ctrs) {
  uint32_t ctr;
  uint64_t *ctr_p = (uint64_t *)ctrs;

  for (ctr = 0; ctr < sizeof(*ctrs) / sizeof(uint64_t); ctr++) {
    *ctr_p = umac4_ctrs_fc_fec_ln_ctr_get(dev_id, umac, ctr);
    ctr_p++;
  }
}

/********************************************************************
 * umac4_ctrs_fc_fec_ln_dump
 ********************************************************************/
void umac4_ctrs_fc_fec_ln_dump(bf_dev_id_t dev_id, uint32_t umac) {
  umac4_fc_fec_ln_ctr_t ctrs;
  char **ctr_name_strs = umac4_fc_fec_ln_ctr_name_strs();
  char **ctr_desc_strs = umac4_fc_fec_ln_ctr_desc_strs();

  umac4_ctrs_fc_fec_ln_get(dev_id, umac, &ctrs);

  u64_ctr_display(sizeof(ctrs) / sizeof(uint64_t),
                  (uint64_t *)&ctrs,
                  ctr_name_strs,
                  ctr_desc_strs);
}
