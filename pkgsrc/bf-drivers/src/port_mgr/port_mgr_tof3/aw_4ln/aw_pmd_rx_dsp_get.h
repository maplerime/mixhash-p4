/*
** ? 2020 Alphawave IP Inc.
*/

/**
 * @brief Alphawave Alphacore RX DSP Function.
 *
 * @file aw_pmd_rx_dsp_get.h
 *
 */

#ifndef __aw_pmd_rx_dsp_get
#define __aw_pmd_rx_dsp_get

#include "aw_alphacore.h"

/**Reports the adapted receiver digital signal processing (DSP) parameters, used for debugging signal integrity issues involving the lane receiver.
 *
 * Args:
 *    `mss`: Object containing driver methods.
 *    'branch':
 *       0-63 - Report statistics for specified branch value
 *
 * Return:
 *    'dsp_info': Return pointer to DSP parameter struct
 *
 *
 * Constraints:
 *    After EqEval is finished (after octl_rx_linkeval_ack_ln[#] returns high).
 *
 * Typical Application Usage:
 *    When experiencing signal integrity issues such as high BER, run this function to dump all DSP related parameters after running EqEval.
 */
int aw_pmd_4ln_rx_dsp_get(mss_access_t *mss, uint32_t branch, aw_dsp_param_t *dsp_info);

#endif
