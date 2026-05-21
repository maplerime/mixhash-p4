#include "bf_switchd.h"
#include "bf_switchd_i2c.h"
/*
 * Register the functions defined in platforms i2c library to read and write
 * tofino
 * registers
 */
void bf_switchd_i2c_fn_reg(bf_pltfm_reg_dir_i2c_rd rd_fn,
                           bf_pltfm_reg_dir_i2c_wr wr_fn) {
  reg_dir_i2c_rd_func = rd_fn;
  reg_dir_i2c_wr_func = wr_fn;
}
