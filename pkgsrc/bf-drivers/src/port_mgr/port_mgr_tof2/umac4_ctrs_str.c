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

/*
umac4_rmon.h
umac4_rmon_str.h
umac4_pcs_ctrs.h
umac4_pcs_ctrs_str.h
umac4_rs_fec_ctrs.h
umac4_rs_fec_ctrs_str.h
umac4_pcs_vl_ctrs.h
umac4_pcs_vl_ctrs_str.h
umac4_rs_fec_ln_ctrs.h
umac4_rs_fec_ln_ctrs_str.h
umac4_fc_fec_ln_ctrs.h
umac4_fc_fec_ln_ctrs_str.h

UMAC4_RMON_CTR(FramesXmitOK)
UMAC4_PCS_CTR(HSMC_PCS_BER_Error_Counter)
UMAC4_RS_FEC_CTR(RSFEC Correctable CodeWords)
UMAC4_PCS_VL_CTR(HSMC_PCS_Lane_Chx_BIP_0_Errors_Counter)
UMAC4_RS_FEC_LN_CTR(RSFEC_SERDES_0_Chx_Symbol_Errors)
UMAC4_FC_FEC_LN_CTR(FCFEC_Lane_Chx_lane_0_Correctable_Errors)

*/

/********************************************************************
 * RMON counters (per ch)
 ********************************************************************/
#define UMAC4_RMON_CTR(x) #x,

static char *umac4_rmon_ctr_name_str[] = {
#include "umac4_rmon.h"
};
#undef UMAC4_RMON_CTR

static char *umac4_rmon_ctr_desc_str[] = {
#include "umac4_rmon_str.h"
};

char *umac4_rmon_ctr_to_name_str(umac4_rmon_ctr_e ctr) {
  if (ctr <
      (sizeof(umac4_rmon_ctr_name_str) / sizeof(umac4_rmon_ctr_name_str[0]))) {
    return umac4_rmon_ctr_name_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char *umac4_rmon_ctr_to_desc_str(umac4_rmon_ctr_e ctr) {
  if (ctr <
      (sizeof(umac4_rmon_ctr_desc_str) / sizeof(umac4_rmon_ctr_desc_str[0]))) {
    return umac4_rmon_ctr_desc_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char **umac4_rmon_ctr_name_strs(void) { return umac4_rmon_ctr_name_str; }

char **umac4_rmon_ctr_desc_strs(void) { return umac4_rmon_ctr_desc_str; }

/********************************************************************
 * PCS counters (per ch)
 ********************************************************************/
#define UMAC4_PCS_CTR(x) #x,

static char *umac4_pcs_ctr_name_str[] = {
#include "umac4_pcs_ctrs.h"
};
#undef UMAC4_PCS_CTR

static char *umac4_pcs_ctr_desc_str[] = {
#include "umac4_pcs_ctrs_str.h"
};

char *umac4_pcs_ctr_to_name_str(umac4_pcs_ctr_e ctr) {
  if (ctr <
      (sizeof(umac4_pcs_ctr_name_str) / sizeof(umac4_pcs_ctr_name_str[0]))) {
    return umac4_pcs_ctr_name_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char *umac4_pcs_ctr_to_desc_str(umac4_pcs_ctr_e ctr) {
  if (ctr <
      (sizeof(umac4_pcs_ctr_desc_str) / sizeof(umac4_pcs_ctr_desc_str[0]))) {
    return umac4_pcs_ctr_desc_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char **umac4_pcs_ctr_name_strs(void) { return umac4_pcs_ctr_name_str; }

char **umac4_pcs_ctr_desc_strs(void) { return umac4_pcs_ctr_desc_str; }

/********************************************************************
 * RS FEC counters (per ch)
 ********************************************************************/
#define UMAC4_RS_FEC_CTR(x) #x,

static char *umac4_rs_fec_ctr_name_str[] = {
#include "umac4_rs_fec_ctrs.h"
};
#undef UMAC4_RS_FEC_CTR

static char *umac4_rs_fec_ctr_desc_str[] = {
#include "umac4_rs_fec_ctrs_str.h"
};

char *umac4_rs_fec_ctr_to_name_str(umac4_rs_fec_ctr_e ctr) {
  if (ctr < (sizeof(umac4_rs_fec_ctr_name_str) /
             sizeof(umac4_rs_fec_ctr_name_str[0]))) {
    return umac4_rs_fec_ctr_name_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char *umac4_rs_fec_ctr_to_desc_str(umac4_rs_fec_ctr_e ctr) {
  if (ctr < (sizeof(umac4_rs_fec_ctr_desc_str) /
             sizeof(umac4_rs_fec_ctr_desc_str[0]))) {
    return umac4_rs_fec_ctr_desc_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char **umac4_rs_fec_ctr_name_strs(void) { return umac4_rs_fec_ctr_name_str; }

char **umac4_rs_fec_ctr_desc_strs(void) { return umac4_rs_fec_ctr_desc_str; }

/********************************************************************
 * PCS virtual lane counters (per vl)
 ********************************************************************/
#define UMAC4_PCS_VL_CTR(x) #x,

static char *umac4_pcs_vl_ctr_name_str[] = {
#include "umac4_pcs_vl_ctrs.h"
};
#undef UMAC4_PCS_VL_CTR

static char *umac4_pcs_vl_ctr_desc_str[] = {
#include "umac4_pcs_vl_ctrs_str.h"
};

char *umac4_pcs_vl_ctr_to_name_str(umac4_pcs_vl_ctr_e ctr) {
  if (ctr < (sizeof(umac4_pcs_vl_ctr_name_str) /
             sizeof(umac4_pcs_vl_ctr_name_str[0]))) {
    return umac4_pcs_vl_ctr_name_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char *umac4_pcs_vl_ctr_to_desc_str(umac4_pcs_vl_ctr_e ctr) {
  if (ctr < (sizeof(umac4_pcs_vl_ctr_desc_str) /
             sizeof(umac4_pcs_vl_ctr_desc_str[0]))) {
    return umac4_pcs_vl_ctr_desc_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char **umac4_pcs_vl_ctr_name_strs(void) { return umac4_pcs_vl_ctr_name_str; }

char **umac4_pcs_vl_ctr_desc_strs(void) { return umac4_pcs_vl_ctr_desc_str; }

/********************************************************************
 * RS FEC lane counters (per FEC ln)
 ********************************************************************/
#define UMAC4_RS_FEC_LN_CTR(x) #x,

static char *umac4_rs_fec_ln_ctr_name_str[] = {
#include "umac4_rs_fec_ln_ctrs.h"
};
#undef UMAC4_RS_FEC_LN_CTR

static char *umac4_rs_fec_ln_ctr_desc_str[] = {
#include "umac4_rs_fec_ln_ctrs_str.h"
};

char *umac4_rs_fec_ln_ctr_to_name_str(umac4_rs_fec_ln_ctr_e ctr) {
  if (ctr < (sizeof(umac4_rs_fec_ln_ctr_name_str) /
             sizeof(umac4_rs_fec_ln_ctr_name_str[0]))) {
    return umac4_rs_fec_ln_ctr_name_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char *umac4_rs_fec_ln_ctr_to_desc_str(umac4_rs_fec_ln_ctr_e ctr) {
  if (ctr < (sizeof(umac4_rs_fec_ln_ctr_desc_str) /
             sizeof(umac4_rs_fec_ln_ctr_desc_str[0]))) {
    return umac4_rs_fec_ln_ctr_desc_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char **umac4_rs_fec_ln_ctr_name_strs(void) {
  return umac4_rs_fec_ln_ctr_name_str;
}

char **umac4_rs_fec_ln_ctr_desc_strs(void) {
  return umac4_rs_fec_ln_ctr_desc_str;
}

/********************************************************************
 * FC FEC lane counters (per FEC ln)
 ********************************************************************/
#define UMAC4_FC_FEC_LN_CTR(x) #x,

static char *umac4_fc_fec_ln_ctr_name_str[] = {
#include "umac4_fc_fec_ln_ctrs.h"
};
#undef UMAC4_FC_FEC_LN_CTR

static char *umac4_fc_fec_ln_ctr_desc_str[] = {
#include "umac4_fc_fec_ln_ctrs_str.h"
};

char *umac4_fc_fec_ln_ctr_to_name_str(umac4_fc_fec_ln_ctr_e ctr) {
  if (ctr < (sizeof(umac4_fc_fec_ln_ctr_name_str) /
             sizeof(umac4_fc_fec_ln_ctr_name_str[0]))) {
    return umac4_fc_fec_ln_ctr_name_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char *umac4_fc_fec_ln_ctr_to_desc_str(umac4_fc_fec_ln_ctr_e ctr) {
  if (ctr < (sizeof(umac4_fc_fec_ln_ctr_desc_str) /
             sizeof(umac4_fc_fec_ln_ctr_desc_str[0]))) {
    return umac4_fc_fec_ln_ctr_desc_str[ctr];
  }
  return "*** BAD CTR ID ***";
}

char **umac4_fc_fec_ln_ctr_name_strs(void) {
  return umac4_fc_fec_ln_ctr_name_str;
}

char **umac4_fc_fec_ln_ctr_desc_strs(void) {
  return umac4_fc_fec_ln_ctr_desc_str;
}
