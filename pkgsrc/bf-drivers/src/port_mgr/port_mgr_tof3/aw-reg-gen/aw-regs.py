#!/usr/bin/python

"""
aw-regs.py: Parse Alphawave CSR register definitions

// -------------------------------------
// REG: AFE_CMN_ATEST
// DESCRIPTION:
//   AFE register with fields related to configuration of the analog test pmon selection control.
// -------------------------------------
#define AFE_CMN_ATEST_ADDR 0x00000000
#define AFE_CMN_ATEST_SIZE 32

// FIELD: BYPASS_ENA_A
// DESCRIPTION:
//   Enable register override for atest
//   0 - Normal operation
//   1 - Register overrides enabled
#define AFE_CMN_ATEST_BYPASS_ENA_A_OFFSET 0x00000000
#define AFE_CMN_ATEST_BYPASS_ENA_A_BITWIDTH 0x00000001
#define AFE_CMN_ATEST_BYPASS_ENA_A_MASK 0x00000001
#define AFE_CMN_ATEST_BYPASS_ENA_A_ACCESS AW_CSR_READ_WRITE
#define AFE_CMN_ATEST_BYPASS_ENA_A_RESET_VALUE 0x00000000

"""

import sys
import textwrap

global parts


if __name__ == "__main__":

  if len(sys.argv) < 2:
    print("python ./aw-regs.py <path-to-csr-defines-file>\n")
    exit()

  csr_file = str(sys.argv[1])

  with open('cmnts.c',"w") as f_out_cmnts:
   with open('regs.c',"w") as f_out_regs:
    with open('flds.c',"w") as f_out_flds:
      f_out_cmnts.write("static char *aw_comments[] = {\n")
      f_out_regs.write("static aw_reg_defs_t aw_regs[] = {\n")
      f_out_flds.write("static aw_fld_defs_t aw_flds[] = {\n")
      with open(csr_file) as f_in:
        open_reg = False
        open_cmnt = False
        fld_index = -1
        cmnt_index = -1
        cur_comment = ""
        reg_cmnt = False
        fld_cmnt = False
        num_comments = 0
        ignore_all = True
        for line in f_in:
          ln = line.rstrip()
          # skip some lines
          if ignore_all:
            if "// REG" in ln:
              ignore_all = False
            else:
              continue
          if open_cmnt:
            if not "//" in ln:
              open_cmnt = False
              cur_comment.replace("\"", "\\\"")
              f_out_cmnts.write("\"" + str(cur_comment.replace("\"", "\\\"")) + "\\n\",\n")
              cur_comment = ""
              cmnt_index += 1
              num_comments += 1
            else:
              cur_comment = cur_comment + "\\n" + ln
              continue
          elif "// REG" in ln:
              cur_comment = "// -------------------------------------\\n\"" + ln
              open_cmnt = True
              reg_cmnt = True
              continue
          elif "// FIELD" in ln:
              cur_comment = ln
              open_cmnt = True
              fld_cmnt = True
              continue
          if "#endif" in ln:
            if open_reg:
              # write out reg struct
              f_out_regs.write("  {\"" + name_part + "\", " + addr_part + ", " + str(num_flds) + ", " + str(fld_index - num_flds + 1) + ", " + str(cmnt_index - num_comments) + "},\n")
              continue
          if not "#define" in ln: continue
          elif "//" in ln: continue
          elif "_SIZE " in ln: continue

          parts = ln.split(" ")
          if "_ADDR " in ln:
            if open_reg:
              # write out reg struct
              f_out_regs.write("  {\"" + name_part + "\", " + addr_part + ", " + str(num_flds) + ", " + str(fld_index - num_flds + 1) + ", " + str(cmnt_index - num_comments) + "},\n")
            open_reg = True
            open_cmnt = False
            reg_cmnt = False
            fld_cmnt = False
            num_comments = 0
            name_part = parts[1]
            addr_part = parts[2]
            num_flds = 0
          elif "_OFFSET " in ln:
            offset_part = parts[2]
          elif "_BITWIDTH " in ln:
            bit_width_part = parts[2]
          elif "_MASK " in ln:
            mask_part = parts[2]
          elif "_ACCESS " in ln:
            access = parts[2]
            if access == "AW_CSR_READ_WRITE":
              access_part = "0"
            elif access == "AW_CSR_READ_ONLY":
              access_part = "1"
            else:
              print("access = " + access)
              exit(0)
          elif "_RESET_VALUE " in ln:
            reset_value_part = parts[2]
            num_flds += 1
            fld_name = parts[1].replace("_RESET_VALUE","")
            f_out_flds.write(" {" + "\"" + fld_name + "\", " + offset_part + ", " + bit_width_part + ", " + mask_part + ", " + access_part + ", " + reset_value_part + ", " + str(cmnt_index) + "},\n")
            fld_index += 1
            open_cmnt = False
            reg_cmnt = False
            fld_cmnt = False

      f_out_regs.write("  {NULL, 0, 0, 0, 0},\n};\n")
      f_out_flds.write("  {NULL, 0, 0, 0, 0, 0},\n};\n")
      f_out_cmnts.write(" \"\",\n};\n")
