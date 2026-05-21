//
//  lld_subdev_reg_if.h
//
//  Copyright (c) 2014 BFN. All rights reserved.
//

#ifndef lld_subdev_reg_if_h
#define lld_subdev_reg_if_h

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

int lld_subdev_wr(bf_dev_id_t dev_id,
                  bf_subdev_id_t subdev_id,
                  uint32_t reg,
                  uint32_t data);
int lld_subdev_rd(bf_dev_id_t dev_id,
                  bf_subdev_id_t subdev_id,
                  uint32_t reg,
                  uint32_t *val);

#ifdef __cplusplus
}
#endif /* C++ */

#endif
