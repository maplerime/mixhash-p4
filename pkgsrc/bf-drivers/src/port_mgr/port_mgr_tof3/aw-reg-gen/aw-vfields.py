#!/usr/bin/python

"""
aw-regs.py: Parse Alphawave Virtual field definitions

// NYQ0
// Description: 
#define NYQ0_ADDR 0x020007F8
#define NYQ0_OFFSET 0x00000000
#define NYQ0_BITWIDTH 0x00000010
#define NYQ0_MASK 0x0000FFFF


"""

import sys
import textwrap

global parts


if __name__ == "__main__":

  if len(sys.argv) < 2:
    print("python ./aw-vfields.py <path-to-virtual-fields-defines-file>\n")
    exit()

  csr_file = str(sys.argv[1])

  with open('vcmnts.c',"w") as f_out_cmnts:
   with open('vregs.c',"w") as f_out_regs:
    with open('vflds.c',"w") as f_out_flds:
      f_out_cmnts.write("static char *aw_vcomments[] = {\n")
      f_out_regs.write("static aw_reg_defs_t aw_vregs[] = {\n")
      f_out_flds.write("static aw_fld_defs_t aw_vflds[] = {\n")
      with open(csr_file) as f_in:
        open_reg = False
        open_cmnt = False
        fld_index = -1
        cmnt_index = -1
        cur_comment = ""
        reg_cmnt = False
        fld_cmnt = False
        num_comments = 0
        num_flds = 0
        ignore_all = False #True
        open_reg_addr = "0"
        first_addr = True
        vreg_num = 0
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
          if not "#define" in ln: continue
          elif "//" in ln: continue
          elif "_SIZE " in ln: continue
          elif "_ACCESS " in ln: continue

          parts = ln.split(" ")
          if "_ADDR " in ln:
            if first_addr:
              first_addr = False
            if parts[2] == open_reg_addr:
              pass
            else:
              if open_reg:
                # write out reg struct
                f_out_regs.write("  {\"" + name_part + "\", " + open_reg_addr + ", " + str(num_flds) + ", " + str(fld_index - num_flds + 1) + ", " + str(cmnt_index - num_comments) + "},\n")
              open_reg_addr = parts[2]
              open_reg = True
              open_cmnt = False
              reg_cmnt = False
              fld_cmnt = False
              num_comments = 0
              name_part = "VREG_" + open_reg_addr
              addr_part = open_reg_addr
              num_flds = 0
          elif "_OFFSET " in ln:
            offset_part = parts[2]
          elif "_BITWIDTH " in ln:
            bit_width_part = parts[2]
          #
          elif "_MASK " in ln:
            print( "line   = " + ln)
            print( "parsed = { " + parts[1] + ", " + parts[2] + " }")
            mask_part = parts[2]
            reset_value_part = "0"
            num_flds += 1
            # Note:
            # Need to handle this case
            # #define USE_CUSTOM_NYQ_MASKS_MASK 0x2000000
            #
            if "MASKS_MASK" in parts[1]:
              fld_name = parts[1].replace("MASKS_MASK","MASKS")
            else:
              fld_name = parts[1].replace("_MASK","")
            # note: all virtual fields are RW
            f_out_flds.write(" {" + "\"" + fld_name + "\", " + offset_part + ", " + bit_width_part + ", " + mask_part + ", 0, " + reset_value_part + ", " + str(cmnt_index) + "},\n")
            fld_index += 1
            open_cmnt = False
            reg_cmnt = False
            fld_cmnt = False

      f_out_regs.write("  {NULL, 0, 0, 0, 0},\n};\n")
      f_out_flds.write("  {NULL, 0, 0, 0, 0, 0},\n};\n")
      f_out_cmnts.write(" \"\",\n};\n")
