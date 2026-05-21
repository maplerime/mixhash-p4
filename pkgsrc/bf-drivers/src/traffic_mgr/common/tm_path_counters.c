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

#include "tm_ctx.h"
#include "traffic_mgr/hw_intf/tm_tofino_hw_intf.h"
#include "traffic_mgr/hw_intf/tm_tof2_hw_intf.h"
#include "traffic_mgr/hw_intf/tm_tof3_hw_intf.h"
#include <lld/lld_err.h>
#include <lld/lld_sku.h>
#include <pipe_mgr/pktgen_intf.h>
#include <target-sys/bf_sal/bf_sys_intf.h>

static bf_tm_path_cntr_hw_funcs_tbl_t g_tm_path_cntr_hw_fptr_tbl;

// bf_tm_blklvl_cntrs_t* blk_cntrs
bf_tm_status_t bf_tm_blklvl_get_drop_cntrs(bf_dev_id_t dev,
                                           bf_dev_pipe_t pipe,
                                           bf_tm_blklvl_cntrs_t *blk_cntrs) {
  bf_status_t rc = BF_INVALID_ARG;

  if (g_tm_path_cntr_hw_fptr_tbl.blklvl_cntrs_rd_fptr && blk_cntrs) {
    bf_dev_pipe_t phy_pipe;
    lld_err_t err;

    if ((err = lld_sku_map_pipe_id_to_phy_pipe_id(dev, pipe, &phy_pipe)) ==
        LLD_OK) {
      rc = g_tm_path_cntr_hw_fptr_tbl.blklvl_cntrs_rd_fptr(
          dev, phy_pipe, blk_cntrs);
    } else {
      LOG_ERROR(
          "%s:Invalid pipe number for "
          "Device = %d Logical pipe = %d lld error %d",
          __func__,
          dev,
          pipe,
          err);
      rc = BF_INVALID_ARG;
    }
  }
  return (rc);
}

// bf_tm_pre_fifo_cntrs_t
bf_tm_status_t bf_tm_pre_fifo_get_drop_cntrs(
    bf_dev_id_t dev, bf_tm_pre_fifo_cntrs_t *fifo_cntrs) {
  bf_status_t rc = BF_INVALID_ARG;

  if (g_tm_path_cntr_hw_fptr_tbl.pre_fifo_cntrs_rd_fptr && fifo_cntrs) {
    rc = g_tm_path_cntr_hw_fptr_tbl.pre_fifo_cntrs_rd_fptr(dev, fifo_cntrs);
  }
  return (rc);
}

/*
 *      Clear Registers
 */
bf_tm_status_t bf_tm_blklvl_clr_drop_cntrs(bf_dev_id_t dev,
                                           bf_dev_pipe_t pipe,
                                           uint32_t clear_mask) {
  bf_status_t rc = BF_INVALID_ARG;

  if (g_tm_path_cntr_hw_fptr_tbl.blklvl_cntrs_clr_fptr) {
    bf_dev_pipe_t phy_pipe;
    lld_err_t err;

    if ((err = lld_sku_map_pipe_id_to_phy_pipe_id(dev, pipe, &phy_pipe)) ==
        LLD_OK) {
      rc = g_tm_path_cntr_hw_fptr_tbl.blklvl_cntrs_clr_fptr(
          dev, phy_pipe, clear_mask);
    } else {
      LOG_ERROR(
          "%s:Invalid pipe number for "
          "Device = %d Logical pipe = %d lld error %d",
          __func__,
          dev,
          pipe,
          err);

      rc = BF_INVALID_ARG;
    }
  }
  return (rc);
}

// bf_tm_pre_fifo_cntrs_t
bf_tm_status_t bf_tm_pre_fifo_clr_drop_cntrs(bf_dev_id_t dev,
                                             bf_dev_pipe_t pipe,
                                             uint32_t fifo) {
  bf_status_t rc = BF_INVALID_ARG;

  if (g_tm_path_cntr_hw_fptr_tbl.pre_fifo_cntrs_clr_fptr) {
    rc = g_tm_path_cntr_hw_fptr_tbl.pre_fifo_cntrs_clr_fptr(dev, pipe, fifo);
  }
  return (rc);
}

#define BF_TM_PATH_HW_FTBL_GET(blklvl_cntrs_get_fptr, pre_fifo_cntrs_get_fptr) \
  {                                                                            \
    g_tm_path_cntr_hw_fptr_tbl.blklvl_cntrs_rd_fptr = blklvl_cntrs_get_fptr;   \
    g_tm_path_cntr_hw_fptr_tbl.pre_fifo_cntrs_rd_fptr =                        \
        pre_fifo_cntrs_get_fptr;                                               \
  }

#define BF_TM_PATH_HW_FTBL_CLR(blklvl_cntrs_clear_fptr,   \
                               pre_fifo_cntrs_clear_fptr) \
  {                                                       \
    g_tm_path_cntr_hw_fptr_tbl.blklvl_cntrs_clr_fptr =    \
        blklvl_cntrs_clear_fptr;                          \
    g_tm_path_cntr_hw_fptr_tbl.pre_fifo_cntrs_clr_fptr =  \
        pre_fifo_cntrs_clear_fptr;                        \
  }

void bf_tm_path_null_hw_ftbl(bf_tm_dev_ctx_t *tm_ctx) {
  (void)tm_ctx;
  BF_TM_PATH_HW_FTBL_GET(NULL, NULL);
  BF_TM_PATH_HW_FTBL_CLR(NULL, NULL);
}

static void bf_tm_path_set_hw_ftbl_rd_funcs(bf_tm_dev_ctx_t *tm_ctx) {
  if (BF_TM_IS_TOFINO(tm_ctx->asic_type)) {
    BF_TM_PATH_HW_FTBL_GET(bf_tm_tofino_blklvl_get_drop_cntrs,
                           bf_tm_tofino_pre_fifo_get_drop_cntrs)
  }
  if (BF_TM_IS_TOF2(tm_ctx->asic_type)) {
    BF_TM_PATH_HW_FTBL_GET(bf_tm_tof2_blklvl_get_drop_cntrs,
                           bf_tm_tof2_pre_fifo_get_drop_cntrs)
  }
  if (BF_TM_IS_TOF3(tm_ctx->asic_type)) {
    BF_TM_PATH_HW_FTBL_GET(bf_tm_tof3_blklvl_get_drop_cntrs,
                           bf_tm_tof3_pre_fifo_get_drop_cntrs)
  }
  if (BF_TM_IS_TOFINOLITE(tm_ctx->asic_type)) {
    // Future addition
  }
}

static void bf_tm_path_set_hw_ftbl_clr_funcs(bf_tm_dev_ctx_t *tm_ctx) {
  if (BF_TM_IS_TOFINO(tm_ctx->asic_type)) {
    BF_TM_PATH_HW_FTBL_CLR(bf_tm_tofino_blklvl_clr_drop_cntrs,
                           bf_tm_tofino_pre_fifo_clr_drop_cntrs)
  }
  if (BF_TM_IS_TOF2(tm_ctx->asic_type)) {
    BF_TM_PATH_HW_FTBL_CLR(bf_tm_tof2_blklvl_clear_drop_cntrs,
                           bf_tm_tof2_pre_fifo_clear_drop_cntrs)
  }
  if (BF_TM_IS_TOF3(tm_ctx->asic_type)) {
    BF_TM_PATH_HW_FTBL_CLR(bf_tm_tof3_blklvl_clr_drop_cntrs,
                           bf_tm_tof3_pre_fifo_clr_drop_cntrs)
  }
  if (BF_TM_IS_TOFINOLITE(tm_ctx->asic_type)) {
    // Future addition
  }
}

void bf_tm_path_set_hw_ftbl(bf_tm_dev_ctx_t *tm_ctx) {
  bf_tm_path_set_hw_ftbl_rd_funcs(tm_ctx);
  bf_tm_path_set_hw_ftbl_clr_funcs(tm_ctx);
}

void bf_tm_path_set_hw_ftbl_cfg_restore(bf_tm_dev_ctx_t *tm_ctx) {
  bf_tm_path_null_hw_ftbl(tm_ctx);
  bf_tm_path_set_hw_ftbl_rd_funcs(tm_ctx);
  bf_tm_path_set_hw_ftbl_clr_funcs(tm_ctx);
}

bf_tm_status_t bf_tm_init_counters(bf_tm_dev_ctx_t *tm_ctx) {
  int ports_per_pipe, k, i, j;
  bf_dev_pipe_t pipe = 0;
  bf_tm_port_t *port;

  ports_per_pipe = (tm_ctx->tm_cfg.pg_per_pipe * tm_ctx->tm_cfg.ports_per_pg);
  port = tm_ctx->ports;
  uint32_t num_pipes = 0;
  lld_sku_get_num_active_pipes(tm_ctx->devid, &num_pipes);
  for (i = 0; i < (int)num_pipes; i++) {
    if (lld_sku_map_pipe_id_to_phy_pipe_id(tm_ctx->devid, i, &pipe) != LLD_OK) {
      LOG_ERROR(
          "Unable to map logical pipe to physical pipe id. Device = %d "
          "Logical "
          "pipe = %d",
          tm_ctx->devid,
          i);
    }
    port =
        tm_ctx->ports + (i * (ports_per_pipe + tm_ctx->tm_cfg.mirror_port_cnt));
    for (k = 0; k < ports_per_pipe; k++) {
      port++;
      // Lets do some checks here for the future
    }
    // Program mirror port
    for (j = 0; j < tm_ctx->tm_cfg.mirror_port_cnt; j++) {
      // Lets do some checks in the future
    }
  }

  bf_tm_status_t rc = BF_TM_EOK;

  // Init the Cached Counters infra so we can manage below 64 bit counters
  rc = tm_init_cached_counters(tm_ctx);
  bf_tm_path_set_hw_ftbl(tm_ctx);
  return rc;
}
