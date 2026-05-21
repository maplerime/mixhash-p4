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

/*
 * This file contains error codes
 */

#ifndef __TM_ERROR_H__
#define __TM_ERROR_H__

#include <stdint.h>
#include <stdbool.h>
#include <bf_types/bf_types.h>

typedef enum {
  BF_TM_EOK = BF_SUCCESS,
  BF_TM_EINT = BF_INTERNAL_ERROR,
  BF_TM_EINV_ARG = BF_INVALID_ARG,
} bf_tm_error_en;

#define BF_TM_IS_OK(rc) (rc == BF_TM_EOK)
#define BF_TM_IS_NOTOK(rc) (rc != BF_TM_EOK)

#endif
