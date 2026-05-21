/*
** ? 2020 Alphawave IP Inc.
*/

#ifndef __aw_pmd_sw_load_lane_cfg
#define __aw_pmd_sw_load_lane_cfg

#include "../aw_driver_sim.h"

int aw_pmd_sw_load_lane_cfg_tx(mss_access_t *mss, char *rate);
int aw_pmd_sw_load_lane_cfg_rx(mss_access_t *mss, char *rate);

#endif // __aw_pmd_sw_load_lane_cfg
