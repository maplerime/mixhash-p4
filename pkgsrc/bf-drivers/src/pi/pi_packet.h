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

#ifndef _PI_PACKET_H__
#define _PI_PACKET_H__

#include <PI/pi_base.h>

pi_status_t packet_register_with_pkt_mgr(pi_dev_id_t dev_id);

pi_status_t packet_send_to_pkt_mgr(pi_dev_id_t dev_id,
                                   const char *out_packet,
                                   size_t packet_size);

#endif  // _PI_PACKET_H__
