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

/*******************************************************************************
 *
 *
 *
 *****************************************************************************/
/* Standard header includes */
#include <math.h>
#include <unistd.h>
#include <dlfcn.h>

/* Module header files */
#include <pipe_mgr/pipe_mgr_err.h>
#include <pipe_mgr/pipe_mgr_config.h>
#include <pipe_mgr/pipe_mgr_porting.h>
#include <pipe_mgr/pipe_mgr_intf.h>

/* Local header files */
#include "pipe_mgr_int.h"
#include "pipe_mgr_table_packing.h"

size_t p4_fake_lrn_cfg_type_sz(uint8_t lrn_cfg_type) {
  (void)lrn_cfg_type;
  return 20;
}

uint8_t p4_fake_fld_lst_hdl_to_lq_cfg_type(
    pipe_fld_lst_hdl_t flow_lrn_fld_lst_hdl) {
  (void)flow_lrn_fld_lst_hdl;
  return (flow_lrn_fld_lst_hdl & 0x7);
}

pipe_status_t p4_fake_lrn_decode(uint8_t pipe,
                                 uint8_t learn_cfg_type,
                                 uint8_t lq_data[48],
                                 void *lrn_digest_entry,
                                 uint32_t index) {
  (void)pipe;
  size_t num_data_bytes = p4_fake_lrn_cfg_type_sz(learn_cfg_type);

  PIPE_MGR_MEMCPY((uint8_t *)lrn_digest_entry + index * num_data_bytes,
                  lq_data,
                  num_data_bytes);

  return PIPE_SUCCESS;
}

void pipe_mgr_setup_fake_profile_func_ptrs(profile_id_t profile_id,
                                           rmt_dev_info_t *dev_info) {
  (void)profile_id;
  dev_info->fake_rmt_cfg = true;
  return;
}
