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

#ifndef PORT_MGR_PORT_EVT_H_INCLUDED
#define PORT_MGR_PORT_EVT_H_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  PORT_MGR_PORT_EVT_NONE = 0,
  PORT_MGR_PORT_EVT_UP,
  PORT_MGR_PORT_EVT_DOWN,
  PORT_MGR_PORT_EVT_SPEED_SET,
} port_mgr_port_event_t;

typedef void (*port_mgr_port_callback_t)(bf_dev_id_t chip,
                                         bf_dev_port_t port,
                                         port_mgr_port_event_t reason,
                                         void *userdata);

bf_status_t port_mgr_register_port_cb(bf_dev_id_t dev_id,
                                      port_mgr_port_callback_t fn,
                                      void *userdata);
#ifdef __cplusplus
}
#endif /* C++ */

#endif  // PORT_MGR_PORT_EVT_H_INCLUDED
