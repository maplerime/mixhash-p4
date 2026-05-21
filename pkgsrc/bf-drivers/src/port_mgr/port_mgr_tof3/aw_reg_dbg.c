
#include <stdio.h>
#include <stdint.h>
#include "aw_reg_dbg.h"

int aw_reg_def_get(aw_reg_defs_t *aw_regs,
                   uint32_t addr,
                   char **name,
                   uint32_t *num_flds,
                   uint32_t *fld_num_base,
                   uint32_t *cmnt_num) {
  aw_reg_defs_t *reg = NULL;
  ;
  int i;

  for (i = 0; aw_regs[i].name != NULL; i++) {
    if (aw_regs[i].addr == addr) {
      reg = &aw_regs[i];
      break;
    }
  }
  if (reg == NULL) return 1;  // error, reg not found

  if (name) *name = reg->name;
  if (num_flds) *num_flds = reg->num_flds;
  if (fld_num_base) *fld_num_base = reg->fld_num_base;
  if (cmnt_num) *cmnt_num = reg->comment_index;
  return 0;
}

int aw_reg_def_get_next(aw_reg_defs_t *aw_regs,
                        uint32_t addr,
                        uint32_t *next_addr,
                        char **name,
                        uint32_t *num_flds,
                        uint32_t *fld_num_base,
                        uint32_t *cmnt_num) {
  aw_reg_defs_t *reg = NULL;
  ;
  int i;

  for (i = 0; aw_regs[i].name != NULL; i++) {
    if (aw_regs[i].addr == addr) {
      if (aw_regs[i + 1].name == NULL) {  // end of regs
        break;
      }
      reg = &aw_regs[i + 1];
      break;
    }
  }
  if (reg == NULL) return 1;  // error, reg not found

  if (next_addr) *next_addr = reg->addr;
  if (name) *name = reg->name;
  if (num_flds) *num_flds = reg->num_flds;
  if (fld_num_base) *fld_num_base = reg->fld_num_base;
  if (cmnt_num) *cmnt_num = reg->comment_index;
  return 0;
}

int aw_fld_def_get(aw_fld_defs_t *flds,
                   uint32_t fld_num,
                   char **name,
                   uint32_t *lo_bit,
                   uint32_t *width,
                   uint32_t *mask,
                   uint32_t *access,
                   uint32_t *reset_value,
                   uint32_t *cmnt_num) {
  aw_fld_defs_t *fld = &flds[fld_num];

  if (name) *name = fld->name;
  if (lo_bit) *lo_bit = fld->lo_bit;
  if (width) *width = fld->width;
  if (mask) *mask = fld->mask;
  if (access) *access = fld->access;
  if (reset_value) *reset_value = fld->reset_value;
  if (cmnt_num) *cmnt_num = fld->comment_index;
  return 0;
}
