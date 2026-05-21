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

#ifndef umac_ctrs_str_h
#define umac_ctrs_str_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "autogen-required-headers.h"
#include "umac4_ctrs.h"

char *umac4_rmon_ctr_to_name_str(umac4_rmon_ctr_e ctr);
char *umac4_rmon_ctr_to_desc_str(umac4_rmon_ctr_e ctr);
char **umac4_rmon_ctr_name_strs(void);
char **umac4_rmon_ctr_desc_strs(void);
char *umac4_pcs_ctr_to_name_str(umac4_pcs_ctr_e ctr);
char *umac4_pcs_ctr_to_desc_str(umac4_pcs_ctr_e ctr);
char **umac4_pcs_ctr_name_strs(void);
char **umac4_pcs_ctr_desc_strs(void);
char *umac4_rs_fec_ctr_to_name_str(umac4_pcs_ctr_e ctr);
char *umac4_rs_fec_ctr_to_desc_str(umac4_pcs_ctr_e ctr);
char **umac4_rs_fec_ctr_name_strs(void);
char **umac4_rs_fec_ctr_desc_strs(void);
char *umac4_pcs_vl_ctr_to_name_str(umac4_pcs_ctr_e ctr);
char *umac4_pcs_vl_ctr_to_desc_str(umac4_pcs_ctr_e ctr);
char **umac4_pcs_vl_ctr_name_strs(void);
char **umac4_pcs_vl_ctr_desc_strs(void);
char *umac4_rs_fec_ln_ctr_to_name_str(umac4_pcs_ctr_e ctr);
char *umac4_rs_fec_ln_ctr_to_desc_str(umac4_pcs_ctr_e ctr);
char **umac4_rs_fec_ln_ctr_name_strs(void);
char **umac4_rs_fec_ln_ctr_desc_strs(void);
char *umac4_fc_fec_ln_ctr_to_name_str(umac4_pcs_ctr_e ctr);
char *umac4_fc_fec_ln_ctr_to_desc_str(umac4_pcs_ctr_e ctr);
char **umac4_fc_fec_ln_ctr_name_strs(void);
char **umac4_fc_fec_ln_ctr_desc_strs(void);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
