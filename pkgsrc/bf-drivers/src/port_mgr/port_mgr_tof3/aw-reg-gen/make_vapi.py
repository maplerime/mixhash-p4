#!/usr/bin/python

"""
make_guardians_vfields_h.py

guardians (EVB) used a different (.csv) format to define
the virtual fields. This script converts the .csv into 
the same format as the 4ln and 16ln .h files so we can
use the same python script to generate the virtual field
debug definitions.

Input:
"virtual_name","register","bit_offset","bit_width","specs","readonly","api","notes"
"lsref_bypass","cmnmfsm_scratch_reg3",0,32,,"Y","Y",
"fast_sram_clk","cmnmfsm_scratch_reg4",0,32,,"Y","Y",

Output:
// FAST_SRAM_CLK
// Description: Used by power/cmn.py to determine whether to swith master to fast clock
#define FAST_SRAM_CLK_ADDR 0x00000130
#define FAST_SRAM_CLK_OFFSET 0x00000000
#define FAST_SRAM_CLK_BITWIDTH 0x00000020
#define FAST_SRAM_CLK_MASK 0xFFFFFFFF

"""

import sys
import textwrap

global parts

if __name__ == "__main__":

  if len(sys.argv) < 2:
    print("python ./make_vapi.py <virual_fields_defines.h>\n")
    exit()

  h_file = str(sys.argv[1])
  ip_type = str(sys.argv[2])

  with open(h_file,"r") as f_in_h:
   with open("vfld_vec_type.h","w") as f_vv_type_h:
    with open("vfld_vec_name.h","w") as f_vv_name_h:
     with open("vfld_vec_vector.h","w") as f_vv_vec_h:
      with open('vapi.c',"w") as f_out_vapi_c:
       with open('vapi.h',"w") as f_out_vapi_h:
        f_out_vapi_c.write("#include <stdint.h>\n")
        f_out_vapi_c.write("#include <bf_types/bf_types.h>\n")
        f_out_vapi_c.write("#include \"../aw_if.h\"\n")
        f_out_vapi_c.write("#include \"../aw_driver_sim.h\"\n")
        f_out_vapi_c.write("\n")

        first_ln = True
        addr = ""
        offset = ""
        width = ""
        mask = 0
        name = ""
        for line in f_in_h:
          ln = line.rstrip()
          # skip empty lines
          if ln == "":
            continue
          # skip comment lines
          if "//" in ln:
            continue
          if "\*" in ln:
            continue
          parts = ln.split(" ")
          if len(parts) < 3:
            continue
          define = parts[0]
          attr   = parts[1]
          val    = parts[2]
          if "_ADDR" in attr:
            addr = val
          if "_OFFSET" in attr:
            offset = val
          if "_BITWIDTH" in attr:
            width = val
          if "_MASKS" in attr:
            continue               # special-case for a vfld with "mask" in the field name
          if "NYQ_MASK" in attr:
            continue               # special-case for a vfld with "mask" in the field name
          if "_MASK" in attr:
            mask = val
            name = attr.replace("_MASK","")
            vapi = name.lower()

            # these go in ../aw_vectors.h to define elements ofthe vector 
            f_vv_type_h.write("typedef int (*aw_pmd_vfld_" + vapi + "_set)(mss_access_t *mss, uint32_t val);\n")
            f_vv_type_h.write("typedef int (*aw_pmd_vfld_" + vapi + "_get)(mss_access_t *mss, uint32_t *val);\n")
            f_vv_name_h.write("  aw_pmd_vfld_" + vapi + "_set                  pmd_vfld_" + vapi + "_set;\n")
            f_vv_name_h.write("  aw_pmd_vfld_" + vapi + "_get                  pmd_vfld_" + vapi + "_get;\n")

            # these go in the per-IP vector initialization
            f_vv_vec_h.write( "  aw_pmd_" + ip_type + "vfld_" + vapi + "_set,\n")
            f_vv_vec_h.write( "  aw_pmd_" + ip_type + "vfld_" + vapi + "_get,\n")

            f_out_vapi_h.write("int aw_pmd_" + ip_type + "vfld_" + vapi + "_set(mss_access_t *mss, uint32_t val);\n")
            f_out_vapi_h.write("int aw_pmd_" + ip_type + "vfld_" + vapi + "_get(mss_access_t *mss, uint32_t *val);\n")

            #print "nam = " + name + " addr = " + addr + " mask = " + mask + " offset = " + offset

            # get API
            f_out_vapi_c.write("int aw_pmd_" + ip_type + "vfld_" + vapi + "_get(mss_access_t *mss, uint32_t *val) {\n")
            f_out_vapi_c.write("  CHECK(pmd_read_field(mss, " + addr + ", " + mask + ", " + offset + ", val));\n")
            f_out_vapi_c.write("  return 0;\n")
            f_out_vapi_c.write("}\n\n")


            # set API
            f_out_vapi_c.write("int aw_pmd_" + ip_type + "vfld_" + vapi + "_set(mss_access_t *mss, uint32_t val) {\n")
            f_out_vapi_c.write("  CHECK(pmd_write_field(mss, " + addr + ", " + mask + ", " + offset + ", val));\n")
            f_out_vapi_c.write("  return 0;\n")
            f_out_vapi_c.write("}\n\n")

 
