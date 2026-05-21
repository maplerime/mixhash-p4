//
//  lld_reg_if.h
//
//  Copyright (c) 2014 BFN. All rights reserved.
//

#ifndef lld_reg_if_h
#define lld_reg_if_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include "bf_types/bf_types.h"

int lld_write_register(bf_dev_id_t dev_id, uint32_t reg, uint32_t data);
int lld_subdev_write_register(bf_dev_id_t dev_id,
                              bf_subdev_id_t subdev_id,
                              uint32_t reg,
                              uint32_t data);

int lld_read_register(bf_dev_id_t dev_id, uint32_t reg, uint32_t *val);
int lld_subdev_read_register(bf_dev_id_t dev_id,
                             bf_subdev_id_t subdev_id,
                             uint32_t reg,
                             uint32_t *val);

int lld_ind_write(bf_dev_id_t dev_id,
                  uint64_t ind_addr,
                  uint64_t data_hi,
                  uint64_t data_lo);

uint32_t lld_ind_read(bf_dev_id_t dev_id,
                      uint64_t ind_addr,
                      uint64_t *data_hi,
                      uint64_t *data_lo);
int lld_subdev_ind_write(bf_dev_id_t dev_id,
                         bf_subdev_id_t subdev_id,
                         uint64_t ind_addr,
                         uint64_t data_hi,
                         uint64_t data_lo);

uint32_t lld_subdev_ind_read(bf_dev_id_t dev_id,
                             bf_subdev_id_t subdev_id,
                             uint64_t ind_addr,
                             uint64_t *data_hi,
                             uint64_t *data_lo);
void lld_dev_lock_ind_reg_mutex_init(void);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
