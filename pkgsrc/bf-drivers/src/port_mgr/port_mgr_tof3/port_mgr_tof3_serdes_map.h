/*************************************************
 * Placeholders for serdes mapping functions
 *************************************************/

#ifndef port_mgr_tof3_serdes_map_h
#define port_mgr_tof3_serdes_map_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "aw_if.h"

uint32_t map_aw_err_to_bf_err(uint32_t rc);
bf_tf3_sd_t *map_dev_port_to_sd(uint32_t dev_id,
                                uint32_t dev_port,
                                uint32_t ln);
uint32_t map_macro_to_address(uint32_t macro);
uint32_t map_address_to_macro(uint32_t addr,
                              uint32_t *subdev_id,
                              uint32_t *macro);
uint32_t map_dev_port_to_macro(uint32_t dev_id,
                               uint32_t dev_port,
                               uint32_t *subdev_id,
                               uint32_t *macro);

#ifdef __cplusplus
}
#endif /* C++ */

#endif  // port_mgr_tof3_serdes_map_h
