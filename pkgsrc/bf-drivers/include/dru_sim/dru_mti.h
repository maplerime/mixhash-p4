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

#ifndef DRU_MTI_INCLUDED
#define DRU_MTI_INCLUDED

#include <dru_sim/dru_sim.h>

typedef enum {
  MTI_TYP_RX_PKT_0 = 0,
  MTI_TYP_RX_PKT_1,
  MTI_TYP_RX_PKT_2,
  MTI_TYP_RX_PKT_3,
  MTI_TYP_RX_PKT_4,
  MTI_TYP_RX_PKT_5,
  MTI_TYP_RX_PKT_6,
  MTI_TYP_RX_PKT_7,
  MTI_TYP_LRT,
  MTI_TYP_IDLE,
  MTI_TYP_LEARN_PIPE0,
  MTI_TYP_LEARN_PIPE1,
  MTI_TYP_LEARN_PIPE2,
  MTI_TYP_LEARN_PIPE3,
  MTI_TYP_DIAG,
  MTI_TYP_NUM
} mti_typ_e;

void dru_mti_tx(dru_dev_id_t asic, mti_typ_e data_type, void *data, int len);
void dru_learn(dru_dev_id_t asic,
               uint8_t *learn_filter_data,
               int len,
               int pipe_nbr);
void dru_rx_pkt(dru_dev_id_t asic, uint8_t *pkt, int len, int cos);
void dru_lrt_update(dru_dev_id_t asic, uint8_t *lrt_stat_data, int len);
void dru_idle_update(dru_dev_id_t asic, uint8_t *idle_timeout_data, int len);
void dru_diag_event(dru_dev_id_t asic, uint8_t *diag_data, int len);

typedef void *(*dru_sim_dma2virt_dbg_callback_fn_mti)(dru_dev_id_t asic,
                                                      bf_dma_addr_t addr);
void dru_mti_register_dma2virt_cb(
    dru_sim_dma2virt_dbg_callback_fn_mti dma2virt_fn);

#endif  // DRU_MTI_INCLUDED
