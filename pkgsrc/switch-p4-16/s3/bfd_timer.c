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

#include <pthread.h>
#include <stdlib.h>

#include "target-sys/bf_sal/bf_sys_intf.h"
#include "third_party/libev/ev.h"
#include <bfd_timer.h>

typedef struct {
  ev_async async_w;
  pthread_mutex_t lock;
  int inited;
  struct ev_loop *loop;
} userdata;

static void timeout_cb(EV_P_ ev_timer *w, int revents) {
  /* Call the users callback with data */
  bfd_timer_t *t = (bfd_timer_t *)w->data;
  t->cb_fn(t, t->cb_data);
  (void)EV_A;
  (void)revents;
}

switch_status_t bfd_timer_create(bfd_timer_t *t,
                                 uint32_t start_msecs,
                                 uint32_t period_msecs,
                                 bfd_timeout_cb cb_fn,
                                 void *cb_data) {
  ev_timer *evt;
  double start = (double)start_msecs / 1000;
  double period = (double)period_msecs / 1000;
  (void)start;
  (void)period;

  if ((t == NULL) || (cb_fn == NULL)) {
    return SWITCH_STATUS_INVALID_PARAMETER;
  }

  evt = (ev_timer *)calloc(1, sizeof(ev_timer));
  if (!evt) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }

  evt->data = t;
  t->cb_fn = cb_fn;
  t->cb_data = cb_data;
  t->timer = (void *)evt;
  ev_init(evt, timeout_cb);  //, start, period);
  return SWITCH_STATUS_SUCCESS;
}

switch_status_t bfd_timer_start(bfd_timer_t *t) {
  int status = 0;
  ev_timer *evt = NULL;
  if ((t == NULL) || (t->timer == NULL)) {
    return SWITCH_STATUS_INVALID_PARAMETER;
  }
  evt = (ev_timer *)t->timer;
  userdata *ud = (userdata *)t->userdata;
  if (!ud->inited) {
    return SWITCH_STATUS_FAILURE;
  }
  status = pthread_mutex_lock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  ev_timer_start(ud->loop, evt);
  ev_async_send(ud->loop, &ud->async_w);
  status = pthread_mutex_unlock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  return SWITCH_STATUS_SUCCESS;
}

switch_status_t bfd_timer_update(bfd_timer_t *t,
                                 uint32_t start_msecs,
                                 uint32_t period_msecs) {
  int status = 0;
  double start = (double)start_msecs / 1000;
  double period = (double)period_msecs / 1000;

  ev_timer *evt = NULL;
  if ((t == NULL) || (t->timer == NULL)) {
    return SWITCH_STATUS_INVALID_PARAMETER;
  }
  evt = (ev_timer *)t->timer;
  userdata *ud = (userdata *)t->userdata;
  if (!ud->inited) {
    return SWITCH_STATUS_FAILURE;
  }
  status = pthread_mutex_lock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  ev_timer_stop(ud->loop, evt);
  ev_timer_set(evt, start, period);
  ev_timer_start(ud->loop, evt);
  ev_async_send(ud->loop, &ud->async_w);
  status = pthread_mutex_unlock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  return SWITCH_STATUS_SUCCESS;
}

switch_status_t bfd_timer_stop(bfd_timer_t *t) {
  int status = 0;
  ev_timer *evt = NULL;
  if ((t == NULL) || (t->timer == NULL)) {
    return SWITCH_STATUS_INVALID_PARAMETER;
  }
  evt = (ev_timer *)t->timer;
  userdata *ud = (userdata *)t->userdata;
  if (!ud->inited) {
    return SWITCH_STATUS_FAILURE;
  }
  status = pthread_mutex_lock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  ev_timer_stop(ud->loop, evt);
  ev_async_send(ud->loop, &ud->async_w);
  status = pthread_mutex_unlock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  return SWITCH_STATUS_SUCCESS;
}

switch_status_t bfd_timer_del(bfd_timer_t *t) {
  if ((t == NULL) || (t->timer == NULL)) {
    return SWITCH_STATUS_INVALID_PARAMETER;
  }

  bfd_timer_stop(t);

  free(t->timer);

  t->cb_fn = NULL;
  t->timer = NULL;
  return SWITCH_STATUS_SUCCESS;
}

switch_status_t bfd_timer_sync(void *ctx) {
  int status = 0;
  userdata *ud = (userdata *)ctx;
  if (!ud->inited) {
    return SWITCH_STATUS_FAILURE;
  }
  status = pthread_mutex_lock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  ev_async_send(ud->loop, &ud->async_w);
  status = pthread_mutex_unlock(&ud->lock);
  if (status != 0) {
    return SWITCH_STATUS_INSUFFICIENT_RESOURCES;
  }
  return SWITCH_STATUS_SUCCESS;
}

static void async_cb(EV_P_ ev_async *w, int revents) {
  (void)EV_A;
  (void)revents;
  (void)w;
  /* Do nothing */
}

static void l_release(EV_P) {
  userdata *ud = ev_userdata(EV_A);
  pthread_mutex_unlock(&ud->lock);
}

static void l_acquire(EV_P) {
  userdata *ud = ev_userdata(EV_A);
  pthread_mutex_lock(&ud->lock);
}

/** Never-ending function. */
switch_status_t bfd_timer_init(void **ret_data) {
  userdata *ud = (userdata *)bf_sys_malloc(sizeof(userdata));
  if (!ud) {
    return SWITCH_STATUS_FAILURE;
  }

  ud->loop = ev_loop_new(0);
  if (!ud->loop) return SWITCH_STATUS_INSUFFICIENT_RESOURCES;

  ev_set_userdata(ud->loop, ud);
  *ret_data = ud;

/* dereferencing type-punned pointer will break strict-aliasing rules
 * so, apply temporary GCC diagnostics pragma
 * the possibly breakage is in the third party header file, ev.h
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  ev_async_init(&ud->async_w, async_cb);
#pragma GCC diagnostic pop

  ev_async_start(ud->loop, &ud->async_w);

  ud->inited = 1;

  pthread_mutex_init(&ud->lock, 0);

  ev_set_loop_release_cb(ud->loop, l_release, l_acquire);

  l_acquire(ud->loop);
  ev_loop(ud->loop, 0);
  l_release(ud->loop);
  return SWITCH_STATUS_SUCCESS;
}
