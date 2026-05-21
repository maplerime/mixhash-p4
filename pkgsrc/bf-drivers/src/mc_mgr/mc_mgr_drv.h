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

#ifndef __MC_MGR_DRV_H__
#define __MC_MGR_DRV_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <dvm/bf_drv_intf.h>
#include <mc_mgr/mc_mgr_config.h>

#define MC_MGR_DRV_SUBDEV_ID_ALL 0xff
#define MC_PVT_MASK_ALL 0x3
typedef struct mc_mgr_drv_buf_t mc_mgr_drv_buf_t;
struct mc_mgr_drv_buf_t {
  mc_mgr_drv_buf_t *next;
  mc_mgr_drv_buf_t *prev;
  bf_map_t mgids_updated;
  bf_map_t freed_rdm_addrs;
  uint8_t *addr;
  bf_sys_dma_pool_handle_t pool;
  bf_phys_addr_t phys_addr;
  uint64_t msgId;
  uint32_t size;
  uint32_t used;
  uint16_t count;
  bf_dev_id_t dev;
  uint8_t buf_pushed;
  uint8_t wr_list_size;
  bf_subdev_id_t subdev_id; /* Set to specific subdevice */
};

typedef struct mc_mgr_drv_buf_pool_t mc_mgr_drv_buf_pool_t;
struct mc_mgr_drv_buf_pool_t {
  bf_sys_dma_pool_handle_t pool;
  unsigned int buf_sz;    /* Size of each DMA buffer. */
  unsigned int buf_cnt;   /* Number of DMA buffers available. */
  int in_use;             /* Count of buffers outstanding. */
  mc_mutex_t mtx;         /* Protects the in_use count. */
  mc_mgr_drv_buf_t *used; /* Allocated with size equal to buf_cnt. */
};

typedef struct mc_mgr_drv_wr_list_t mc_mgr_drv_wr_list_t;
struct mc_mgr_drv_wr_list_t {
  mc_mgr_drv_buf_t *bufList;  // List of buffers
  int count;                  // Number of buffers
};

bf_status_t mc_mgr_drv_init();
bf_status_t mc_mgr_drv_init_dev(bf_dev_id_t dev, bf_dma_info_t *dma_info);
void mc_mgr_drv_remove_dev(bf_dev_id_t dev);
void mc_mgr_drv_warm_init_quick(bf_dev_id_t dev);
void mc_mgr_drv_cmplt_operations(int sid, int dev_id);
int mc_mgr_drv_wrl_append(int dev,
                          bf_subdev_id_t subdev_id,
                          int sid,
                          int width,
                          uint64_t addr,
                          uint64_t hi,
                          uint64_t lo,
                          const char *where,
                          const int line);
int mc_mgr_drv_wrl_append_reg(int dev,
                              int sid,
                              uint32_t addr,
                              uint32_t data,
                              const char *where,
                              const int line);
bf_status_t mc_mgr_drv_wrl_send(int sid, bool is_last);
void mc_mgr_drv_wrl_abort(int sid);
int mc_mgr_drv_start_rdm_change(bf_dev_id_t dev, bf_dev_pipe_t pipe);
int mc_mgr_drv_read_rdm_change(bf_dev_id_t dev, bf_dev_pipe_t pipe);

bf_status_t mc_mgr_write_register(bf_dev_id_t dev_id,
                                  uint32_t reg_addr,
                                  uint32_t reg_data);

bf_status_t mc_mgr_drv_service_dr(bf_dev_id_t dev_id);
#endif
