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

#ifndef _BFD_TIMER_H_
#define _BFD_TIMER_H_

#include <stdint.h>

#include "bf_switch/bf_switch_types.h"

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

struct bfd_timer_s;

typedef void (*bfd_timeout_cb)(struct bfd_timer_s *timer, void *data);

typedef struct bfd_timer_s {
  void *timer;    /* OS abstracted context pointer */
  void *userdata; /* per loop userdata */
  bfd_timeout_cb cb_fn;
  void *cb_data;
} bfd_timer_t;

switch_status_t bfd_timer_create(bfd_timer_t *timer,
                                 uint32_t start_msecs,
                                 uint32_t period_msecs,
                                 bfd_timeout_cb cb_fn,
                                 void *cb_data);

switch_status_t bfd_timer_update(bfd_timer_t *t,
                                 uint32_t start_msecs,
                                 uint32_t period_msecs);

switch_status_t bfd_timer_sync(void *userdata);

switch_status_t bfd_timer_start(bfd_timer_t *timer);

switch_status_t bfd_timer_stop(bfd_timer_t *timer);

switch_status_t bfd_timer_del(bfd_timer_t *timer);

switch_status_t bfd_timer_init(void **userdata);

#ifdef __cplusplus
}
#endif /* C++ */

#endif /* _BFD_TIMER_H_ */
