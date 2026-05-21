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

#ifndef __TM_HW_ACCESS_H__
#define __TM_HW_ACCESS_H__

#include "tm_ctx.h"

/* ----- DMA Buffer sizing for Tofino ------ */
// Came up with number 32 by checking logs; At the TM init time 24 buffers of
// 32KB
// were built before pushing to HW.
#define BF_TM_TOFINO_DMA_BUFFER_SIZE (1 << 15) /* 32 Kilobyte */
#define BF_TM_TOFINO_DMA_POOL_SIZE ((32) * BF_TM_TOFINO_DMA_BUFFER_SIZE)

/* ----- DMA Buffer sizing for TofinoLite ------ */
#define BF_TM_TOFINOLITE_DMA_BUFFER_SIZE (1 << 15) /* 32 Kilobyte */
#define BF_TM_TOFINOLITE_DMA_POOL_SIZE ((32) * BF_TM_TOFINO_DMA_BUFFER_SIZE)

/* ----- Add Here : DMA Buffer sizing for New-ASIC ------ */

#define BF_TM_FLUSH_WL(dev) bf_tm_flush_wlist(dev)

void bf_tm_flush_wlist(bf_dev_id_t dev);
void bf_tm_cleanup_wlist(bf_dev_id_t dev);
void bf_tm_complete_ops(bf_dev_id_t dev);
bf_tm_status_t bf_tm_write_register(bf_dev_id_t dev,
                                    uint32_t offset,
                                    uint32_t data);
bf_tm_status_t bf_tm_write_memory(bf_dev_id_t dev,
                                  uint64_t ind_addr,
                                  uint8_t wr_sz,
                                  uint64_t hi,
                                  uint64_t lo);
bf_tm_status_t bf_tm_read_register(bf_dev_id_t dev,
                                   uint32_t offset,
                                   uint32_t *data);
bf_tm_status_t bf_tm_read_memory(bf_dev_id_t dev,
                                 uint64_t ind_addr,
                                 uint64_t *hi,
                                 uint64_t *lo);

bf_tm_status_t bf_tm_subdev_write_register(bf_dev_id_t dev,
                                           bf_subdev_id_t subdev_id,
                                           uint32_t offset,
                                           uint32_t data);
bf_tm_status_t bf_tm_subdev_write_memory(bf_dev_id_t dev,
                                         bf_subdev_id_t subdev_id,
                                         uint64_t ind_addr,
                                         uint8_t wr_sz,
                                         uint64_t hi,
                                         uint64_t lo);
bf_tm_status_t bf_tm_subdev_read_register(bf_dev_id_t dev,
                                          bf_subdev_id_t subdev_id,
                                          uint32_t offset,
                                          uint32_t *data);
bf_tm_status_t bf_tm_subdev_read_memory(bf_dev_id_t dev,
                                        bf_subdev_id_t subdev_id,
                                        uint64_t ind_addr,
                                        uint64_t *hi,
                                        uint64_t *lo);

bf_tm_status_t bf_tm_setup_dma_sizes(bf_dev_id_t dev,
                                     bf_subdev_id_t subdev_id,
                                     uint32_t poolsize,
                                     uint32_t buffersize);

bf_tm_status_t bf_tm_setup_dma(bf_dev_id_t dev,
                               bf_subdev_id_t subdev_id,
                               bf_sys_dma_pool_handle_t hdl);

#endif
