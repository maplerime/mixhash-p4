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

addr_dict = {
  'CMNMFSM_SCRATCH_REG3_ADDR': '0x0000011C',
  'CMNMFSM_SCRATCH_REG4_ADDR': '0x00000120',
  'RXMFSM_SCRATCH_REG1_ADDR' : '0x02000790',
  'RXMFSM_SCRATCH_REG10_ADDR': '0x02000794',
  'RXMFSM_SCRATCH_REG11_ADDR': '0x02000798',
  'RXMFSM_SCRATCH_REG12_ADDR': '0x0200079C',
  'RXMFSM_SCRATCH_REG2_ADDR' : '0x020007A0',
  'RXMFSM_SCRATCH_REG3_ADDR' : '0x020007A4',
  'RXMFSM_SCRATCH_REG4_ADDR' : '0x020007A8',
  'RXMFSM_SCRATCH_REG5_ADDR' : '0x020007AC',
  'RXMFSM_SCRATCH_REG6_ADDR' : '0x020007B0',
  'RXMFSM_SCRATCH_REG7_ADDR' : '0x020007B4',
  'RXMFSM_SCRATCH_REG8_ADDR' : '0x020007B8',
  'RXMFSM_SCRATCH_REG9_ADDR' : '0x020007BC',
  'RX_CTLE_ADAPT_ERROR_CALC_REG3_ADDR' : '0x0200018C',
  'RX_CTLE_ADAPT_ERROR_CALC_REG4_ADDR' : '0x02000190',
}

if __name__ == "__main__":

  if len(sys.argv) < 2:
    print("python ./make_guardians_vfields_h.py <path-to-csv-file>\n")
    exit()

  csv_file = str(sys.argv[1])

  with open('vflds.h',"w") as f_out_h:
   with open(csv_file) as f_in:
        first_ln = True
        for line in f_in:
          if first_ln:
            first_ln = False
            continue
          ln = line.rstrip()
          parts = ln.split(",")
          name = parts[0]
          name = name.upper()
          name = name.replace("\"","")
          addr = parts[1]
          addr = addr.replace("\"","")
          addr = addr.upper()
          key = addr + "_ADDR"
          if key in addr_dict.keys():
            print("    " + addr_dict[key] )
          else:
            print "key=" + key
            print str(addr_dict)
            for k in addr_dict.keys():
              print str(k) + " : " + addr_dict[k] + "\n"
            exit() 
          offset = parts[2]
          width = parts[3]
          address = addr.upper()
          address = address.replace("\"","")
          #f_out_h.write("#define " + name + "_ADDR " + address  + "\n")
          f_out_h.write("#define " + name + "_ADDR " + addr_dict[key]  + "\n")
          f_out_h.write("#define " + name + "_OFFSET " + offset + "\n")
          f_out_h.write("#define " + name + "_BITWIDTH " + width + "\n")
          mask = pow(2,(int(width) + int(offset))) - 1
          f_out_h.write("#define " + name + "_MASK " + hex((mask >> int(offset)) << int(offset)) + "\n"  )
          f_out_h.write("\n")

