/*******************************************************************************
 Barefoot Networks Switch ASIC Linux Packet driver
 Copyright(c) 2015 - 2019 Barefoot Networks, Inc.
 
 This program is free software; you can redistribute it and/or modify it
 under the terms and conditions of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

 The full GNU General Public License is included in this distribution in
 the file called "COPYING".

 Contact Information:
 info@barefootnetworks.com
 Barefoot Networks, 4750 Patrick Henry Drive, Santa Clara CA 95054

*******************************************************************************/
#include <linux/types.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/version.h>

#include <linux/netdevice.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <dvm/bf_dma_types.h>

#include "bf_kpkt_priv.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
static void bf_kpkt_timer_fn(struct timer_list *timer) {
  struct bf_kpkt_adapter *adapter = from_timer(adapter, timer, timer);;
  if (!adapter) {
    printk(KERN_ERR "Error: bad parameter in bf_kpkt_timer_fn\n");
    return;
  }
  /* schedule napi even if interrupts are enabled */
  if (adapter->napi_enable) {
    napi_schedule(&adapter->napi);
  }
}
#else
static void bf_kpkt_timer_fn(unsigned long data) {
  struct bf_kpkt_adapter *adapter = (struct bf_kpkt_adapter *)data;
  if (!adapter) {
    printk(KERN_ERR "Error: bad parameter in bf_kpkt_timer_fn\n");
    return;
  }
  /* schedule napi even if interrupts are enabled */
  if (adapter->napi_enable) {
    napi_schedule(&adapter->napi);
  }
}
#endif

void bf_kpkt_timer_init(struct bf_kpkt_adapter *adapter) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
  timer_setup(&adapter->timer, bf_kpkt_timer_fn, 0);
#else
  struct timer_list *timer = &adapter->timer;
  init_timer(timer);
  timer->function = bf_kpkt_timer_fn;
  timer->data = (unsigned long)adapter;
#endif
}

void bf_kpkt_timer_add(struct bf_kpkt_adapter *adapter, u32 ms) {
  /* setup a timer to run later */
  adapter->timer.expires = jiffies +  msecs_to_jiffies(ms);
  mod_timer(&adapter->timer, adapter->timer.expires);
}

void bf_kpkt_timer_del(struct bf_kpkt_adapter *adapter) {
  del_timer_sync(&adapter->timer);
}
