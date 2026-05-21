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

#ifndef __TRAFFIC_MGR_MCAST_INTF_H__
#define __TRAFFIC_MGR_MCAST_INTF_H__

#include <mc_mgr/mc_mgr_types.h>
#include <traffic_mgr/traffic_mgr_types.h>

/**
 * @file traffic_mgr_mcast.h
 * \brief Details multicast specific APIs.
 */

/**
 * @addtogroup tm-mcast
 * @{
 *  Description of APIs for Traffic Manager application
 *  to program multicast traffic FIFO sizes.
 */

/**
 * Set the input FIFO arbitration mode to strict priority or weighted round
 * robin.  Note that if strict priority is enabled on a FIFO, all FIFOs higher
 * will also be enabled for strict priority.  For example, to set FIFO 1 as
 * strict priority, 2 and 3 must also be strict priority.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe_bmap         Pipe bit mask. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO to configure, must be 0, 1, 2 or 3.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[in] use_strict_pri    If @c true, use strict priority.  If @c false,
 *                              use weighted round robin.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_arb_mode_set(bf_dev_id_t dev,
                                       uint8_t pipe_bmap,
                                       int fifo,
                                       bool use_strict_pri);

/**
 * Set the input FIFO arbitration weights used by the weighted round robin
 * arbitration mode.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe_bmap         Pipe bit mask. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO to configure, must be 0, 1, 2 or 3.
 *                              Check ASIC capabilites to find maximum number of
 *                              fifos.
 * @param[in] weight            The weight assigned to FIFO.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_wrr_weight_set(bf_dev_id_t dev,
                                         uint8_t pipe_bmap,
                                         int fifo,
                                         uint8_t weight);

/**
 * Set multicast fifo to iCoS mapping.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe_bmap         Pipe bit mask. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO to configure, must be 0, 1, 2 or 3.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[in] icos_bmap         iCoS bit map.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_icos_mapping_set(bf_dev_id_t dev,
                                           uint8_t pipe_bmap,
                                           int fifo,
                                           uint8_t icos_bmap);

/**
 * Get multicast fifo to iCoS mapping.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe
 * @param[out] icos_bmap        iCoS bit map.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_icos_mapping_get(bf_dev_id_t dev,
                                           bf_dev_pipe_t pipe,
                                           int fifo,
                                           uint8_t *icos_bmap);

/**
 * Set the input FIFO depth.  Sum of all four sizes cannot exceed 8192,
 * additionally, each size must be a multiple of 8.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe_bmap         Pipe bit mask. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO to configure, must be 0, 1, 2 or 3.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[in] size              The size assigned to FIFO.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_depth_set(bf_dev_id_t dev,
                                    uint8_t pipe_bmap,
                                    int fifo,
                                    int size);

/**
 * Get the input FIFO arbitration mode to strict priority or weighted round
 * robin.  Note that if strict priority is enabled on a FIFO, all FIFOs higher
 * must also be enabled for strict priority.  For example, to set FIFO 1 as
 * strict priority, 2 and 3 must also be strict priority.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[out] use_strict_pri   If @c true, arbitration mode is strict priority.
 *                              If @c false, arbitration is weighted round
 *                              robin.
 * @return Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_arb_mode_get(bf_dev_id_t dev,
                                       bf_dev_pipe_t pipe,
                                       int fifo,
                                       bool *use_strict_pri);

/**
 * Get the input FIFO arbitration weights used by the weighted round robin
 * arbitration mode.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[out] weight           The weight assigned to FIFO.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_wrr_weight_get(bf_dev_id_t dev,
                                         bf_dev_pipe_t pipe,
                                         int fifo,
                                         uint8_t *weight);

/**
 * Get the input FIFO depth.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[out] size             The size assigned to FIFO 0.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_depth_get(bf_dev_id_t dev,
                                    bf_dev_pipe_t pipe,
                                    int fifo,
                                    int *size);

/**
 * Get default multicast fifo to iCoS mapping.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe
 * @param[out] icos_bmap        iCoS bit map.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_icos_mapping_get_default(bf_dev_id_t dev,
                                                   bf_dev_pipe_t pipe,
                                                   int fifo,
                                                   uint8_t *icos_bmap);

/**
 * Get the default input FIFO arbitration mode to strict priority or weighted
 * round robin.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[out] use_strict_pri   If @c true, arbitration mode is strict priority.
 *                              If @c false, arbitration is weighted round
 *                              robin.
 * @return Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_arb_mode_get_default(bf_dev_id_t dev,
                                               bf_dev_pipe_t pipe,
                                               int fifo,
                                               bool *use_strict_pri);

/**
 * Get the default input FIFO arbitration weights used by the weighted round
 * robin arbitration mode.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[out] weight           The weight assigned to FIFO.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_wrr_weight_get_default(bf_dev_id_t dev,
                                                 bf_dev_pipe_t pipe,
                                                 int fifo,
                                                 uint8_t *weight);

/**
 * Get the default input FIFO depth.
 *
 * @param[in] dev               The ASIC id.
 * @param[in] pipe              Pipe number. Check ASIC manual to find maximum
 *                              of pipes.
 * @param[in] fifo              The FIFO id.
 *                              Check ASIC manual to find maximum number of
 *                              fifos per pipe.
 * @param[out] size             The size assigned to FIFO 0.
 * @return                      Status of the API call.
 */
bf_status_t bf_tm_mc_fifo_depth_get_default(bf_dev_id_t dev,
                                            bf_dev_pipe_t pipe,
                                            int fifo,
                                            int *size);

/* @} */

#endif
