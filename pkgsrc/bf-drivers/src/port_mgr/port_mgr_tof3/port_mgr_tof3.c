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

#include <stdarg.h>

#include <bf_types/bf_types.h>
#include <dvm/bf_drv_intf.h>
#include <dvm/dvm_intf.h>
#include <tof3_regs/tof3_reg_drv.h>
#include <lld/lld_reg_if.h>
#include <port_mgr/port_mgr_intf.h>
#include <port_mgr/bf_port_if.h>
#include <port_mgr/port_mgr_log.h>

/** \brief port_mgr_tof3_init:
 *
 * Initialize port_mgr module for Tofino2
 */
void port_mgr_tof3_init(void) {}

/***********************************************************************
 * autogen_log
 *
 * wrapper for logs in auto-generated code. This is mainly to control
 * the amount of trace logs generated.
 *
 * The typical usage would be to set autogen_log_en = true then run
 * some set of functions and set autogen_log_en = false, to disable
 * further tracing.
 *
 * Leaving tracing enabled will significantly slow down system operation
 * and will cause rapid overflof of the bf_drivers.log files.
 ************************************************************************
 */
#if defined(DEVICE_IS_EMULATOR)
bool tf3_autogen_log_en = true;
#else
bool tf3_autogen_log_en = false;
#endif

int tf3_autogen_log(const char *fmt, ...) {
  if (!tf3_autogen_log_en) {
    return 0;
  } else {
    char log_string[128];

    va_list args;
    va_start(args, fmt);
    vsnprintf(log_string, sizeof(log_string) - 1, fmt, args);
    va_end(args);
    port_mgr_log("%s", log_string);
  }
  return 0;
}
