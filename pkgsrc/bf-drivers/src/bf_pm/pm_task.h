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

#ifndef __PM_TASK_H__
#define __PM_TASK_H__
/*-------------------- pm_task.h -----------------------------*/
#include <stdint.h>

#define TASK_DONE 0xffffffff
typedef uint32_t (*tasklet_fn)(void *context);
typedef enum { LO_PRI = 0, HI_PRI, MAX_PRI } tasklet_pri_t;

typedef enum { STATE_RUNNING = 0, STATE_REMOVE, STATE_DEFAULT } tasklet_state_t;

void pm_fsm_queues_init();
void pm_tasklet_scheduler(void);
void pm_tasklet_new(tasklet_fn fn, void *context, tasklet_pri_t priority);
void pm_tasklet_rmv(void *context);
bool pm_is_current_tasklet_valid(void *context);

// debug facility
void pm_tasklet_free_run_set(bool st);
bool pm_tasklet_free_run_get(void);
void pm_tasklet_single_step_set(void);

#endif /* __PM_TASK_H__ */
