################################################################################
 #  INTEL CONFIDENTIAL
 #
 #  Copyright (c) 2021 Intel Corporation
 #  All Rights Reserved.
 #
 #  This software and the related documents are Intel copyrighted materials,
 #  and your use of them is governed by the express license under which they
 #  were provided to you ("License"). Unless the License provides otherwise,
 #  you may not use, modify, copy, publish, distribute, disclose or transmit this
 #  software or the related documents without Intel's prior written permission.
 #
 #  This software and the related documents are provided as is, with no express or
 #  implied warranties, other than those that are expressly stated in the License.
 #################################################################################


#!/usr/bin/python

import csv
import sys

import os.path
import hashlib
import copy
import re
import textwrap

from operator import mul
from types import StringTypes

blknmlist = ["tx","rx","rsvd","fec_analyzer","lane_slice","link_trng","an_lt", "an_ieee", "an_rsvd", "group8", "dfx_even", "dfx_odd", "top_pll", "ecc", "rsvd2", "sensor", "rsvd3", "gpio", "fw"]
blk_def_list = ["CREDO_BLK_TX", "CREDO_BLK_RX", "CREDO_BLK_RSVD", "CREDO_BLK_FEC_ANALYZER", "CREDO_BLK_LANE_SLICE", "CREDO_BLK_LINK_TRNG", "CREDO_BLK_AN_LT", "CREDO_BLK_AN_IEEE", "CREDO_BLK_AN_RSVD", "CREDO_BLK_GROUP8", "CREDO_BLK_DFX_EVEN", "CREDO_BLK_DFX_ODD", "CREDO_BLK_TOP_PLL", "CREDO_BLK_ECC", "CREDO_BLK_RSVD2", "CREDO_BLK_SENSOR", "CREDO_BLK_RSVD3", "CREDO_BLK_GPIO", "CREDO_BLK_FW"]
last_offset = -1
last_blk = "no block"
nth_rsvd_fld = 1

def credo_addr_to_blk(addr):
    blk = "unknown"
    if addr < 0x80000:
        if (addr & 0x000700) == 0x00000: # Tx. PLL Tx, NRZ-25, PAM-4-50
            if (addr & 0x007C0) == 0x000C0: # RSVD
                blk = "rsvd"
            else:
                blk = "tx"
        elif (addr & 0x000700) == 0x00100: # Rx, PLL Rx, PAM-4 Rx, NRZ Rx
            if (addr & 0x001F0) == 0x001C0: # FEC analyzer
                blk = "fec_analyzer"
            else:
                blk = "rx"
        elif (addr & 0x000700) == 0x00200: # lane slice
           blk = "lane_slice"
        elif (addr & 0x000700) == 0x00400: # Link Training
           blk = "link_trng"
        elif (addr & 0x000700) == 0x00300: # Autonegotiation: Link Training
           blk = "an_lt"
        elif (addr & 0x000700) == 0x00500: # Autonegotiation: IEEE
           blk = "an_ieee"
        elif (addr & 0x000700) == 0x00600: # Autonegotiation: RSVD
           blk = "an_rsvd"
    elif addr < 0x80100:
        blk = "group8"
    elif addr < 0x801F0: # IF DFX
        if ((addr >> 4) & 1) == 0:
            blk = "dfx_even"
        else:
            blk = "dfx_odd"
    elif addr < 0x80300: # ECC group8 block
        blk = "ecc"
    elif addr < 0x90000: # hole
        blk = "rsvd2"
    elif addr < 0x90200: # Top Level PLL
        blk = "top_pll"
    elif addr < 0x90300: # ECC group4 block
        blk = "ecc"
    elif addr < 0x90400: # Temperature/Volt sensors
        blk = "sensor"
    elif addr < 0xA0000: # hole
        blk = "rsvd3"
    elif addr < 0xA0100: # Firmware
        blk = "fw"
    return blk

        
#
def credo_c_api_get(fld_nm, blk, offset, hi, lo, description):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_get"

    line = ("/*********************************************************************\n")
    c_file.write(line)
    h_file.write(line)
    line = ("* " + api_nm + "\n* \n")
    c_file.write(line)
    h_file.write(line)
    line = description + "\n"
    c_file.write(line)
    h_file.write(line)
    line = ("*********************************************************************/\n")
    c_file.write(line)
    h_file.write(line)
    line = ("void " + api_nm + "(bf_dev_id_t dev_id,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     bf_dev_port_t dev_port,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t ln,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t *reg32,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t *fld_val,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t hw)")
    c_file.write(line + " {\n")
    h_file.write(line + ";\n\n")

    c_file.write("  if (hw) {\n")
    c_file.write("    credo_reg_rd(dev_id, dev_port, ln, " + blk_def_list[blk_index] + ", " + str(hex(offset)) + ", reg32);\n")
    c_file.write("  }\n")
    c_file.write("  *fld_val = CREDO_FLD_RD(*reg32, " + str(hi) + ", " + str(lo) + ");\n")
    c_file.write("  if (bfn_sd_trace) port_mgr_log(\"TRC : %d: %3d : ln%d : Rd : -------- : %08x : %s\", dev_id, dev_port, ln, *fld_val, __func__);\n")
    c_file.write("}\n\n")

    # save above
    credo_api_nm = api_nm

    api_nm = "bf_ll_serdes_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_get"

    line = ("/*********************************************************************\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("* " + api_nm + "\n* \n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = description + "\n"
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("*********************************************************************/\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("bf_status_t " + api_nm + "(bf_dev_id_t dev_id,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     bf_dev_port_t dev_port,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t ln,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t *reg32,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t *fld_val,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t hw)")
    bf_ll_c_file.write(line + " {\n")
    bf_ll_h_file.write(line + ";\n\n")

    bf_ll_c_file.write("  uint32_t mac_block;\n");
    bf_ll_c_file.write("  port_mgr_err_t rc;\n\n")

    bf_ll_c_file.write("  /* validate dev_id/dev_port */\n")
    bf_ll_c_file.write("  rc = port_mgr_tof2_map_dev_port_to_all(dev_id,\n")
    bf_ll_c_file.write("                                        dev_port,\n")
    bf_ll_c_file.write("                                        NULL, NULL,\n")
    bf_ll_c_file.write("                                        &mac_block, NULL, NULL);\n")
    bf_ll_c_file.write("  if (rc != PORT_MGR_OK) return BF_INVALID_ARG;\n\n")

    bf_ll_c_file.write("  " + credo_api_nm + "(dev_id, dev_port, ln, reg32, fld_val, hw);\n")
    bf_ll_c_file.write("  return BF_SUCCESS;\n")
    bf_ll_c_file.write("}\n\n")


#
def credo_c_api_set(fld_nm, blk, offset, hi, lo, description):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_set"
    
    line = ("void " + api_nm + "(bf_dev_id_t dev_id,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     bf_dev_port_t dev_port,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t ln,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t *reg32,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t fld_val,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t hw)")
    c_file.write(line + " {\n")
    h_file.write(line + ";\n\n")

    c_file.write("  if (bfn_sd_trace) port_mgr_log(\"TRC : %d: %3d : ln%d : Wr : -------- : %08x : %s\", dev_id, dev_port, ln, fld_val, __func__);\n")

    c_file.write("  *reg32 = CREDO_FLD_WR(*reg32, fld_val, " + str(hi) + ", " + str(lo) + ");\n")
    c_file.write("  if (hw) {\n")
    c_file.write("    credo_reg_wr(dev_id, dev_port, ln, " + blk_def_list[blk_index] + ", " + str(hex(offset)) + ", *reg32);\n")
    c_file.write("  }\n")
    c_file.write("}\n\n")

    # save above
    credo_api_nm = api_nm

    api_nm = "bf_ll_serdes_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_set"

    line = ("bf_status_t " + api_nm + "(bf_dev_id_t dev_id,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     bf_dev_port_t dev_port,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t ln,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t *reg32,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t fld_val,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t hw)")
    bf_ll_c_file.write(line + " {\n")
    bf_ll_h_file.write(line + ";\n\n")

    bf_ll_c_file.write("  uint32_t mac_block;\n");
    bf_ll_c_file.write("  port_mgr_err_t rc;\n\n")

    bf_ll_c_file.write("  /* validate dev_id/dev_port */\n")
    bf_ll_c_file.write("  rc = port_mgr_tof2_map_dev_port_to_all(dev_id,\n")
    bf_ll_c_file.write("                                        dev_port,\n")
    bf_ll_c_file.write("                                        NULL, NULL,\n")
    bf_ll_c_file.write("                                        &mac_block, NULL, NULL);\n")
    bf_ll_c_file.write("  if (rc != PORT_MGR_OK) return BF_INVALID_ARG;\n\n")

    bf_ll_c_file.write("  " + credo_api_nm + "(dev_id, dev_port, ln, reg32, fld_val, hw);\n")
    bf_ll_c_file.write("  return BF_SUCCESS;\n")
    bf_ll_c_file.write("}\n\n")


#
def credo_c_api_rmw(fld_nm, blk, offset, hi, lo, description):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    get_api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_get"
    set_api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_set"
    api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_rmw"
    line = ("void " + api_nm + "(bf_dev_id_t dev_id,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     bf_dev_port_t dev_port,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t ln,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t *reg32,\n")
    c_file.write(line)
    h_file.write(line)
    line = ("     uint32_t fld_val) {\n")
    c_file.write(line)
    line = ("     uint32_t fld_val);\n\n")
    h_file.write(line)

    line = ("  uint32_t unused_fld_val = 0;\n\n")
    c_file.write(line)

    c_file.write("  " + get_api_nm + "(dev_id, dev_port, ln, reg32, &unused_fld_val, true);\n")
    c_file.write("  " + set_api_nm + "(dev_id, dev_port, ln, reg32, fld_val, true);\n")
    c_file.write("}\n\n")

    # save above
    credo_api_nm = api_nm

    api_nm = "bf_ll_serdes_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_rmw"
    line = ("bf_status_t " + api_nm + "(bf_dev_id_t dev_id,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     bf_dev_port_t dev_port,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t ln,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t *reg32,\n")
    bf_ll_c_file.write(line)
    bf_ll_h_file.write(line)
    line = ("     uint32_t fld_val) {\n")
    bf_ll_c_file.write(line)
    line = ("     uint32_t fld_val);\n\n")
    bf_ll_h_file.write(line)

    bf_ll_c_file.write("  uint32_t mac_block;\n");
    bf_ll_c_file.write("  port_mgr_err_t rc;\n\n")

    bf_ll_c_file.write("  /* validate dev_id/dev_port */\n")
    bf_ll_c_file.write("  rc = port_mgr_tof2_map_dev_port_to_all(dev_id,\n")
    bf_ll_c_file.write("                                         dev_port,\n")
    bf_ll_c_file.write("                                         NULL, NULL,\n")
    bf_ll_c_file.write("                                         &mac_block, NULL, NULL);\n")
    bf_ll_c_file.write("  if (rc != PORT_MGR_OK) return BF_INVALID_ARG;\n\n")

    bf_ll_c_file.write("  " + credo_api_nm + "(dev_id, dev_port, ln, reg32, fld_val);\n")
    bf_ll_c_file.write("  return BF_SUCCESS;\n")
    bf_ll_c_file.write("}\n\n")

#
def credo_py_api_get(fld_nm, blk, offset, hi, lo, description):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_get"

    line = ("#*********************************************************************\n")
    py_file.write(line)
    line = ("# " + api_nm + "\n# \n")
    py_file.write(line)
    line = description + "\n"
    py_file.write(line)
    line = ("#********************************************************************/\n")
    py_file.write(line)

    py_file.write("def " + api_nm + "(dev_id, dev_port, ln):\n")
    py_file.write("  reg_val = credo_reg_rd(dev_id, dev_port, ln, " + blk_def_list[blk_index] + ", " + hex(offset) + ")\n")
    py_file.write("  val = CREDO_FLD_RD(reg_val, " + str(hi) + ", " + str(lo) + ")\n")
    py_file.write("  return val\n\n")


#
def credo_py_api_set(fld_nm, blk, offset, hi, lo, description):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_set"

    line = ("#*********************************************************************\n")
    py_file.write(line)
    line = ("# " + api_nm + "\n# \n")
    py_file.write(line)
    line = description + "\n"
    py_file.write(line)
    line = ("#********************************************************************/\n")
    py_file.write(line)

    py_file.write("def " + api_nm + "(dev_id, dev_port, ln, val):\n")
    py_file.write("  reg_val = credo_reg_rd(dev_id, dev_port, ln, " + blk_def_list[blk_index] + ", " + hex(offset) + ")\n")
    py_file.write("  CREDO_FLD_WR(reg_val, val, " + str(hi) + ", " + str(lo) + ")\n")
    py_file.write("  credo_reg_wr(dev_id, dev_port, ln, " + blk_def_list[blk_index] + ", " + hex(offset) + ", reg_val);\n\n")

    return


#
def credo_py_api_show(fld_nm, blk, offset, hi, lo, description):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    api_nm = "credo_" + blk_nm + "_" + hex(offset) + "_" + fld_nm + "_show"

    line = ("#*********************************************************************\n")
    py_file.write(line)
    line = ("# " + api_nm + "\n# \n")
    py_file.write(line)
    line = description + "\n"
    py_file.write(line)
    line = ("#********************************************************************/\n")
    py_file.write(line)
    py_file.write("def " + api_nm + "(dev_id, dev_port, ln, val):\n")
    py_file.write("  reg_ofs  = credo_map_blk_offset_to_addr(" + blk_def_list[blk_index] + ", " + hex(offset) + ", ln )\n\n")

    line = description.replace("* ","") + "\n"
    py_file.write("  print(" + "\"" + line.replace("\n","\\n") + "\")\n")
    py_file.write("  phys_addr = reg_ofs\n")
    py_file.write("  print(hex(phys_addr) + \" : \" + \"" + blk_nm + "\" + \" : \" + hex(reg_ofs) + \" : \" + hex(val))\n")
#
def credo_py_menu_add(fld_nm, blk, offset, is_set_api):
    blk_nm = blk
    blk_index = blknmlist.index(blk_nm)

    if is_set_api:
        api_nm = "credo_" + blk_nm + "_reg_" + hex(offset) + "_" + fld_nm + "_set"
        py_menu_file.write("    if cmd == \"" + api_nm + "\":\n")
        py_menu_file.write("        " + api_nm + "(dev_id, dev_port, ln, val)\n")
    else:
        api_nm = "credo_" + blk_nm + "_reg_" + hex(offset) + "_" + fld_nm + "_get"
        py_menu_file.write("    if cmd == \"" + api_nm + "\":\n")
        py_menu_file.write("        val = " + api_nm + "(dev_port, ln)\n")
        api_nm = "credo_" + blk_nm + "_reg_" + hex(offset) + "_" + fld_nm + "_show"
        py_menu_file.write("        " + api_nm + "(dev_id, dev_port, ln, val)\n")
    py_menu_file.write("\n")


#
def process_field(base_addr, block, offset, fld_name, hi, lo, access_mode, field_desc):
    global last_blk
    global last_offset
    global nth_rsvd_fld

    if len(field_desc) < 30:
        desc_spacer = ""
    else:
        desc_spacer = "\n                                                                  : "
    if (base_addr & 0xA0000) == 0xA0000:
        print("[Firmware ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    elif (base_addr & 0x90300) == 0x90300:
        print("[Sensor   ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    elif (base_addr & 0x90200) == 0x90200:
        print("[ECC-G4   ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    elif (base_addr & 0x90000) == 0x90000:
        print("[Top PLL  ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    elif (base_addr & 0x80200) == 0x80200:
        print("[ECC-G8   ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    elif (base_addr & 0x80100) == 0x80100:
        print("[DFX      ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    elif (base_addr & 0x80000) == 0x80000:
        print("[Group8   ] " + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + desc_spacer + field_desc )
    else:
        print("[" + '{0: <10}'.format(blknmlist[block] + "] ") + '{0: >4}'.format(str(offset)) + "[" + '{0: >2}'.format(str(hi)) + ":" + '{0: >2}'.format(str(lo)) + "] : " + access_mode + " : " + '{0: <35}'.format(fld_name) + " : " + desc_spacer + field_desc )

    blk_nm = blknmlist[block]
    fld_nm = fld_name

    if blk_nm == last_blk:
        if offset == last_offset:
            if fld_nm == "rsvd":
                fld_nm = "rsvd" + str(nth_rsvd_fld)
                nth_rsvd_fld = nth_rsvd_fld + 1
                last_blk = blk_nm
                last_offset = offset
        else:
            #nth_rsvd_fld = 1
            last_offset = offset
    else:
        #nth_rsvd_fld = 1
        last_blk = blk_nm

    # replace any non-ASCII chars
    field_desc = field_desc.replace("\xe2",".")
    field_desc = field_desc.replace("\x8b",".")
    field_desc = field_desc.replace("\xae",".")

    #
    # Format description for comment blocks
    text_block = "* " + field_desc.replace("\n","\n* ")
    py_text_block = "# " + field_desc.replace("\n","\n# ")
   
    #
    # some names have reserved chars in them. Convert all such to "_"
    fld_nm = fld_nm.replace(":","_")
    fld_nm = fld_nm.replace("/","_")
    fld_nm = fld_nm.replace(" ","_") 
    fld_nm = fld_nm.replace(".","_") 
    fld_nm = fld_nm.replace("]","_") 
    fld_nm = fld_nm.replace("[","_") 
    # dont generate "set" for RO, RF or reserved fields
    #
    credo_c_api_get(fld_nm, blk_nm, offset, hi, lo, text_block)
    if (fld_name != "rsvd") and (access_mode != "RO") and (access_mode != "RF"):
        credo_c_api_set(fld_nm, blk_nm, offset, hi, lo, text_block)
        credo_c_api_rmw(fld_nm, blk_nm, offset, hi, lo, text_block)

    credo_py_api_get(fld_nm, blk_nm, offset, hi, lo, py_text_block)
    if (fld_name != "rsvd") and (access_mode != "RO") and (access_mode != "RF"):
        credo_py_api_set(fld_nm, blk_nm, offset, hi, lo, py_text_block)
    credo_py_api_show(fld_nm, blk_nm, offset, hi, lo, py_text_block)

    if (fld_name != "rsvd") and (access_mode != "RO") and (access_mode != "RF"):
        credo_py_menu_add(fld_nm, blk_nm, offset, True)
    credo_py_menu_add(fld_nm, blk_nm, offset, False)
    


def parse_csv (filename):

    with open(filename, "rb") as csv_file:
        csv_reader = csv.DictReader(csv_file)
        row_num = 0
        for row in csv_reader:
            #hack
            #print(str(row))

            reg_fld = row["Reg Addr (and Bits)"]
            if "----------" in reg_fld: continue
            if ",,,,,,,,," in reg_fld: continue
            reg_nm  = row["Blue-Jay 1.0 Reg Name"]
            fld_nm  = row["Field Name"]
            fld_acc = row["Access"]
            def_val = row["Default (Hex)"]
            def_bin_val = row["Default (Binary)"]
            fld_width = row["# of Bits"]
            fld_desc = row["Description"]

            #print("reg fld: " + str(reg_fld))
            parts = reg_fld.split(".")

            if str(parts) == "['']": continue

            addr_part = parts[0]
            print("0 :::" + str(parts[0]))
            print("1 :::" + str(parts[1]))

            bits  = parts[1].split(":")
            if len(bits) == 0:
                print("No bits?\n")
            elif len(bits) == 1:
                hi_bit = int(bits[0])
                lo_bit = int(bits[0])
            else:
                hi_bit = int(bits[0])
                lo_bit = int(bits[1])

            # determine "blk" name
            tobe_hex_addr = addr_part.replace("g","8",1)
            tobe_hex_addr = tobe_hex_addr.replace("n","0",1)
            tobe_hex_addr = tobe_hex_addr.replace("m","1",1)
            hex_addr = int(tobe_hex_addr, base=16)
            blk = credo_addr_to_blk(hex_addr)

            base_addr_part = addr_part.replace("n","0",1)
            base_addr_part = base_addr_part.replace("m","1",1)
            base_addr_part = base_addr_part.replace("g","8",1)
            base_addr = int(base_addr_part, base=16)

            if blk == "fec_analyzer":
                ofs = (base_addr & 0x3f)
            elif blk == "rsvd":
                ofs = (base_addr & 0x3f)
            else:
                ofs = (base_addr & 0xff)

            if blk == "dfx_even":
                ofs = (base_addr & 0x0f)
            elif blk == "dfx_odd":
                base_addr += 0x10
                ofs = (base_addr & 0x0f)
            if blk != "unknown":
                blk_index = blknmlist.index(blk)
                process_field(base_addr, blk_index, ofs, fld_nm.lower(), hi_bit, lo_bit, fld_acc, fld_desc)

    csv_file.close()   
    return

def copy_fixed_header_portion(c, h, py, bf_ll_c, bf_ll_h):
    c.write("/* clang-format off */\n\n")
    h.write("/* clang-format off */\n\n")
    h.write("#include <stdbool.h>\n")
    h.write("#include <stdint.h>\n")
    h.write("#include <stddef.h>\n\n")

    bf_ll_c.write("/* clang-format off */\n\n")
    bf_ll_h.write("/* clang-format off */\n\n")
    bf_ll_h.write("#include <stdbool.h>\n")
    bf_ll_h.write("#include <stdint.h>\n")
    bf_ll_h.write("#include <stddef.h>\n\n")
    bf_ll_c.write("#include <bf_types/bf_types.h>\n")
    bf_ll_c.write("#include <port_mgr/port_mgr_intf.h>\n")
    bf_ll_c.write("#include \"port_mgr_tof2_map.h\"\n")
    bf_ll_c.write("#include \"credo_sd_access.h\"\n\n")

    c.write("#include <stdbool.h>\n")
    c.write("#include <stdint.h>\n")
    c.write("#include <stddef.h>\n")
    c.write("#include <bf_types/bf_types.h>\n")
    c.write("#include <port_mgr/port_mgr_intf.h>\n")
    c.write("#include <port_mgr/port_mgr_log.h>\n")
    c.write("#include \"port_mgr_tof2_map.h\"\n")
    c.write("#include \"credo_sd_access.h\"\n\n")

    h.write("#define FLD_MSK(hi,lo) ((((uint32_t)1<<(hi+1)) - 1) - (((uint32_t)1<<(lo)) - 1))\n\n")
    h.write("#define CREDO_FLD_WR(reg_val, fld_val, hi, lo) \\\n")
    h.write("  ((reg_val & (~FLD_MSK(hi,lo))) | ((fld_val << lo) & (FLD_MSK(hi,lo))))\n")

    h.write("#define CREDO_FLD_RD(reg_val, hi, lo) \\\n")
    h.write("  ((reg_val << (32 - hi - 1)) >> (32 - hi - 1 + lo))\n\n")
    h.write("typedef enum {\n")
    h.write("  CREDO_BLK_TX,\n")
    h.write("  CREDO_BLK_RX,\n")
    h.write("  CREDO_BLK_RSVD,\n")
    h.write("  CREDO_BLK_FEC_ANALYZER,\n")
    h.write("  CREDO_BLK_LANE_SLICE,\n")
    h.write("  CREDO_BLK_LINK_TRNG,\n")
    h.write("  CREDO_BLK_AN_LT,\n")
    h.write("  CREDO_BLK_AN_IEEE,\n")
    h.write("  CREDO_BLK_AN_RSVD,\n")
    h.write("  CREDO_BLK_GROUP8,\n")
    h.write("  CREDO_BLK_DFX_EVEN,\n")
    h.write("  CREDO_BLK_DFX_ODD,\n")
    h.write("  CREDO_BLK_TOP_PLL,\n")
    h.write("  CREDO_BLK_ECC,\n")
    h.write("  CREDO_BLK_RSVD2,\n")
    h.write("  CREDO_BLK_SENSOR,\n")
    h.write("  CREDO_BLK_RSVD3,\n")
    h.write("  CREDO_BLK_GPIO,\n")
    h.write("  CREDO_BLK_FW,\n")
    h.write("} credo_blk_typ_e;\n\n")

    c.write("/*********************************************************************\n")
    c.write("* credo_map_blk_offset_to_addr\n")
    c.write("**********************************************************************/\n")
    c.write("uint32_t credo_map_blk_offset_to_addr(credo_blk_typ_e blk_index, uint32_t offset, uint32_t ln) {\n")
    c.write("  uint32_t addr, stride, offset_mask = 0x000FF;\n\n")
    c.write("  if (blk_index ==  CREDO_BLK_TX) {\n")
    c.write("    addr   = 0x00000;\n")
    c.write("    stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_RX) {\n")
    c.write("     addr   = 0x00100;\n")
    c.write("     stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_RSVD) {\n")
    c.write("     addr   = 0x000C0;\n")
    c.write("     stride = 0x10000;\n")
    c.write("     offset_mask = 0x0003F; // # special mask for offset\n")
    c.write("  } else if (blk_index == CREDO_BLK_FEC_ANALYZER) {\n")
    c.write("    addr   = 0x001C0;\n")
    c.write("    stride = 0x10000;\n")
    c.write("    offset_mask = 0x000F; // # special mask for offset\n")
    c.write("  } else if (blk_index == CREDO_BLK_LANE_SLICE) {\n")
    c.write("     addr   = 0x00200;\n")
    c.write("     stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_LINK_TRNG) {\n")
    c.write("     addr   = 0x00400;\n")
    c.write("     stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_AN_LT) {\n")
    c.write("     addr   = 0x00300;\n")
    c.write("     stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_AN_IEEE) {\n")
    c.write("     addr   = 0x00500;\n")
    c.write("     stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_AN_RSVD) {\n")
    c.write("     addr   = 0x00600;\n")
    c.write("     stride = 0x10000;\n")
    c.write("  } else if (blk_index == CREDO_BLK_GROUP8) {\n")
    c.write("     addr   = 0x80000;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_DFX_EVEN) {\n")
    c.write("     addr   = 0x80100;\n")
    c.write("     stride = 0x0020;\n")
    c.write("  } else if (blk_index == CREDO_BLK_DFX_ODD) {\n")
    c.write("     addr   = 0x80110;\n")
    c.write("     stride = 0x0020;\n")
    c.write("  } else if (blk_index == CREDO_BLK_TOP_PLL) {\n")
    c.write("     addr   = 0x90000;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_ECC) {\n")
    c.write("     addr   = 0x90200;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_RSVD2) {\n")
    c.write("     addr   = 0x80300;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_SENSOR) {\n")
    c.write("     addr   = 0x90300;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_GPIO) {\n")
    c.write("     addr   = 0x90400;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_RSVD3) {\n")
    c.write("     addr   = 0x90500;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else if (blk_index == CREDO_BLK_FW) {\n")
    c.write("     addr   = 0xA0000;\n")
    c.write("     stride = 0x0;\n")
    c.write("  } else {\n")
    c.write("     addr   = 0xBAD00;\n")
    c.write("     stride = 0x0;\n")
    c.write("  }\n")
    c.write("  return (addr + (ln * stride) + (offset & offset_mask));\n")
    c.write("}\n\n")


    c.write("extern bool bfn_sd_trace;\n")
    c.write("extern uint32_t port_mgr_tof2_serdes_tile_rd(bf_dev_id_t dev_id,\n")
    c.write("                                             bf_dev_port_t dev_port,\n")
    c.write("                                             uint32_t ofs);\n")
    c.write("extern void     port_mgr_tof2_serdes_tile_wr(bf_dev_id_t dev_id,\n")
    c.write("                                             bf_dev_port_t dev_port,\n")
    c.write("                                             uint32_t ofs,\n")
    c.write("                                             uint32_t val);\n")
    c.write("/*********************************************************************\n")
    c.write("* credo_map_dev_port_ln_to_ln_in_grp\n")
    c.write("**********************************************************************/\n")
    c.write("uint32_t credo_map_dev_port_ln_to_ln_in_grp(bf_dev_id_t dev_id,\n")
    c.write("                                            bf_dev_port_t dev_port,\n")
    c.write("                                            uint32_t ln) {\n")
    c.write("  uint32_t mac_id, ch;\n");
    c.write("  port_mgr_err_t rc;\n\n")

    c.write("  /* get mac_id and base channel */\n")
    c.write("  rc = port_mgr_tof2_map_dev_port_to_all(dev_id,\n")
    c.write("                                         dev_port,\n")
    c.write("                                         NULL, NULL,\n")
    c.write("                                         &mac_id, &ch, NULL);\n")
    c.write("  bf_sys_assert(rc == PORT_MGR_OK);\n")
    c.write("  return(ch + ln);\n")
    c.write("}\n\n")

    c.write("/*********************************************************************\n")
    c.write("* credo_reg_wr\n")
    c.write("**********************************************************************/\n")
    c.write("void credo_reg_wr(bf_dev_id_t dev_id,\n")
    c.write("            bf_dev_port_t dev_port,\n")
    c.write("            uint32_t ln,\n")
    c.write("            uint32_t blk_index,\n")
    c.write("            uint32_t offset,\n")
    c.write("            uint32_t val) {\n")
    c.write("  uint32_t ln_in_grp = credo_map_dev_port_ln_to_ln_in_grp(dev_id, dev_port, ln);\n")
    c.write("  uint32_t reg_ofs  = credo_map_blk_offset_to_addr(blk_index, offset, ln_in_grp);\n\n")

    c.write("  port_mgr_tof2_serdes_tile_wr(dev_id, dev_port, reg_ofs, val);\n")
    c.write("}\n\n")

    c.write("/*********************************************************************\n")
    c.write("* credo_reg_rd\n")
    c.write("**********************************************************************/\n")
    c.write("void credo_reg_rd(bf_dev_id_t dev_id,\n")
    c.write("            bf_dev_port_t dev_port,\n")
    c.write("            uint32_t ln,\n")
    c.write("            uint32_t blk_index,\n")
    c.write("            uint32_t offset,\n")
    c.write("            uint32_t *val) {\n")
    c.write("  uint32_t ln_in_grp = credo_map_dev_port_ln_to_ln_in_grp(dev_id, dev_port, ln);\n")
    c.write("  uint32_t reg_ofs  = credo_map_blk_offset_to_addr(blk_index, offset, ln_in_grp);\n\n")

    c.write("  *val = port_mgr_tof2_serdes_tile_rd(dev_id, dev_port, reg_ofs);\n")
    c.write("}\n\n")

    py.write("def CREDO_FLD_WR(_reg_val, _fld_val, _hi, _lo):\n")
    py.write("  reg_val = int(_reg_val)\n")
    py.write("  fld_val = int(_fld_val)\n")
    py.write("  hi = int(_hi)\n")
    py.write("  lo = int(_lo)\n")
    py.write("  return ((reg_val >> lo) << (32 - hi - 1 + lo) >> (32 - hi - 1) | (fld_val << lo))\n\n")

    py.write("def CREDO_FLD_RD(_reg_val, _hi, _lo):\n")
    py.write("  reg_val = int(_reg_val)\n")
    py.write("  hi = int(_hi)\n")
    py.write("  lo = int(_lo)\n")
    py.write("  return ((reg_val << (32 - hi - 1)) >> (32 - hi - 1 + lo))\n\n")

    blk_enum = 0;
    for blk_nm in blk_def_list:
        py.write(blk_nm + " = " + str(blk_enum) + "\n")
        blk_enum += 1

    line = ("\n#********************************************************************/\n")
    py.write(line)
    py.write("# credo_map_blk_offset_to_addr\n")
    line = ("#********************************************************************/\n")
    py.write(line)
    py.write("def credo_map_blk_offset_to_addr(blk_index, offset, ln):\n")
    py.write("    offset_mask = 0x000FF\n")
    py.write("    if blk_index ==  CREDO_BLK_TX:\n")
    py.write("        addr   = 0x00000\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_RX:\n")
    py.write("        addr   = 0x00100\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_RSVD:\n")
    py.write("        addr   = 0x000C0\n")
    py.write("        stride = 0x10000\n")
    py.write("        offset_mask = 0x0003F # special mask for offset\n")
    py.write("    elif blk_index == CREDO_BLK_FEC_ANALYZER:\n")
    py.write("        addr   = 0x001C0\n")
    py.write("        stride = 0x10000\n")
    py.write("        offset_mask = 0x000F # special mask for offset\n")
    py.write("    elif blk_index == CREDO_BLK_LANE_SLICE:\n")
    py.write("        addr   = 0x00200\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_LINK_TRNG:\n")
    py.write("        addr   = 0x00400\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_AN_LT:\n")
    py.write("        addr   = 0x00300\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_AN_IEEE:\n")
    py.write("        addr   = 0x00500\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_AN_RSVD:\n")
    py.write("        addr   = 0x00600\n")
    py.write("        stride = 0x10000\n")
    py.write("    elif blk_index == CREDO_BLK_GROUP8:\n")
    py.write("        addr   = 0x80100\n")
    py.write("        stride = 0x0\n")
    py.write("    elif blk_index == CREDO_BLK_DFX_EVEN:\n")
    py.write("        addr   = 0x80100\n")
    py.write("        stride = 0x0020\n")
    py.write("    elif blk_index == CREDO_BLK_DFX_ODD:\n")
    py.write("        addr   = 0x80110\n")
    py.write("        stride = 0x0020\n")
    py.write("    elif blk_index == CREDO_BLK_TOP_PLL:\n")
    py.write("        addr   = 0x90000\n")
    py.write("        stride = 0x0\n")
    py.write("    elif blk_index == CREDO_BLK_ECC:\n")
    py.write("        addr   = 0x90200\n")
    py.write("        stride = 0x0\n")
    py.write("    elif blk_index == CREDO_BLK_SENSOR:\n")
    py.write("        addr   = 0x90300\n")
    py.write("        stride = 0x0\n")
    py.write("    elif blk_index == CREDO_BLK_GPIO:\n")
    py.write("        addr   = 0x90400\n")
    py.write("        stride = 0x0\n")
    py.write("    elif blk_index == CREDO_BLK_FW:\n")
    py.write("        addr   = 0xA0000\n")
    py.write("        stride = 0x0\n")
    py.write("    else:\n")
    py.write("        addr   = 0xBAD00\n")
    py.write("        stride = 0x0\n")
    py.write("    return (addr + (ln * stride) + (offset & offset_mask))\n\n")

    line = ("#********************************************************************/\n")
    py.write(line)
    py.write("# credo_reg_rd\n")
    line = ("#********************************************************************/\n")
    py.write(line)
    py.write("def credo_reg_rd(dev_id, dev_port, ln, blk_index, offset):\n")
    py.write("  reg_ofs  = credo_map_blk_offset_to_addr(blk_index, offset, ln)\n\n")

    py.write("  reg_val = credo_hw_reg_rd(dev_id, dev_port, reg_ofs)\n")
    py.write("  return reg_val\n\n")

    line = ("#********************************************************************/\n")
    py.write(line)
    py.write("# credo_reg_wr\n")
    line = ("#********************************************************************/\n")
    py.write(line)
    py.write("def credo_reg_wr(dev_id, dev_port, ln, blk_index, offset, reg_val):\n")
    py.write("  reg_ofs  = credo_map_blk_offset_to_addr(blk_index, offset, ln)\n\n")

    py.write("  credo_hw_reg_wr(dev_id, dev_port, reg_ofs, reg_val)\n")
    py.write("  return\n\n")


if __name__ == "__main__":

    with open("credo_sd_access.c", "wo") as c_file:
        with open("credo_sd_access.h", "wo") as h_file:
            with open("credo_sd_access.py", "wo") as py_file:
                with open("credo_sd_menu.py", "wo") as py_menu_file:
                    with open("bf_ll_serdes_if.c", "wo") as bf_ll_c_file:
                        with open("bf_ll_serdes_if.h", "wo") as bf_ll_h_file:
                            copy_fixed_header_portion(c_file, h_file, py_file, bf_ll_c_file, bf_ll_h_file)
                            parse_csv( str(sys.argv[1]))
                        bf_ll_h_file.close()
                    bf_ll_c_file.close()
                py_menu_file.close()
            py_file.close()
        h_file.close()
    c_file.close()


