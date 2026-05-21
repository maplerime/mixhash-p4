#ifndef AW_REG_DBG_INCLUDED
#define AW_REG_DBG_INCLUDED

typedef struct aw_fld_defs_t {
  char *name;
  uint32_t lo_bit;
  uint32_t width;
  uint32_t mask;
  uint32_t access;
  uint32_t reset_value;
  uint32_t comment_index;
} aw_fld_defs_t;

typedef struct aw_reg_defs_t {
  char *name;
  uint32_t addr;
  uint32_t num_flds;
  uint32_t fld_num_base;
  uint32_t comment_index;
} aw_reg_defs_t;

int aw_reg_def_get(aw_reg_defs_t *aw_regs,
                   uint32_t addr,
                   char **name,
                   uint32_t *num_flds,
                   uint32_t *fld_num_base,
                   uint32_t *cmnt_idx);
int aw_reg_def_get_next(aw_reg_defs_t *aw_regs,
                        uint32_t addr,
                        uint32_t *next_addr,
                        char **name,
                        uint32_t *num_flds,
                        uint32_t *fld_num_base,
                        uint32_t *cmnt_num);
int aw_fld_def_get(aw_fld_defs_t *flds,
                   uint32_t fld_num,
                   char **name,
                   uint32_t *lo_bit,
                   uint32_t *width,
                   uint32_t *mask,
                   uint32_t *access,
                   uint32_t *reset_value,
                   uint32_t *cmnt_idx);

#endif  // AW_REG_DBG_INCLUDED
