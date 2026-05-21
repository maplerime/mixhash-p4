/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2022 Intel Corporation
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

/*
 *    This file contains all data strcutures and parameters
 *    related to TM device
 */

#ifndef __TM_DEV_H__
#define __TM_DEV_H__

#include <stdint.h>

typedef bf_tm_status_t (*bf_tm_dev_wr_fptr)(bf_dev_id_t);
typedef bf_tm_status_t (*bf_tm_dev_uint8_wr_fptr)(bf_dev_id_t, uint8_t);
typedef bf_tm_status_t (*bf_tm_dev_uint8_rd_fptr)(bf_dev_id_t, uint8_t *);

typedef struct _bf_tm_dev_hw_funcs {
  bf_tm_dev_uint8_wr_fptr timestamp_shift_wr_fptr;
  bf_tm_dev_uint8_rd_fptr timestamp_shift_rd_fptr;
  bf_tm_dev_wr_fptr ddr_train_wr_fptr;
} bf_tm_dev_hw_funcs_tbl;

bf_status_t bf_tm_set_timestamp_shift(bf_dev_id_t dev, uint8_t shift);
bf_status_t bf_tm_get_timestamp_shift(bf_dev_id_t dev, uint8_t *shift);
bf_status_t bf_tm_set_ddr_train(bf_dev_id_t dev);

#endif
