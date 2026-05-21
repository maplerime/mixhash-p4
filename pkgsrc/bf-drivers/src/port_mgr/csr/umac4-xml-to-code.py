#!/usr/bin/python

"""
comira.py: Parse Comira XML register descriptions  

"""

import sys
import xml.etree.cElementTree as ET
import textwrap
import subprocess

global def_file
global accessor_c_file
global accessor_h_file
global dbg_info_file
global reg_ofs
global block_base
global block_name
global reg_name
global fld_name
global bit_ofs
global bit_width
global access_mode
global accessor_entry

def parse_fieldelt(elt, fld):
    global fld_name
    global bit_ofs
    global bit_width
    global access_mode
    global accessor_entry

    if elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}name":
        print "----------fld-name: " + elt.text
        fld_name = elt.text
        fld["fld_name"] = fld_name
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}hdl_path":
        print "----------hdl_path: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}bitOffset":
        print "----------bitOffset: " + elt.text
        bit_ofs = int(elt.text)
        fld["bit_ofs"] = bit_ofs
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}bitWidth":
        print "----------bitWidth: " + elt.text
        bit_width = int(elt.text)
        fld["bit_width"] = bit_width
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}access":
        print "----------access: " + elt.text
        access_mode  = elt.text
        fld["access_mode"] = access_mode 
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}modifiedWriteValue":
        print "----------modifiedWriteValue: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}fieldReset":
        print "----------fieldReset: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}resetConfigured":
        print "----------resetConfigured: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}description":
        if not elt.text:
            elt.text = "<none>"
        print "----------description: " + elt.text
        description = elt.text
        fld["desc"] = textwrap.fill(description,40)
        fld["raw_desc"] = elt.text.replace("\n"," ")
        fld["raw_desc"] = fld["raw_desc"].replace("\"","'")

        stride_parm = ""
        stride_val  = ""
        stride_arg  = ""
        reg_addr = (int(block_base, 16))
        if ((reg_addr >= 0) and (reg_addr < 0x200)):
            stride_parm = str("uint32_t channel, ")
            stride_val  = "(channel * 0x200)"
            stride_arg  = "channel, "
        elif ((reg_addr >= 0x1000) and (reg_addr < 0x1080)):
            stride_parm = "uint32_t serdes_lane, "
            stride_val  = "(serdes_lane * 0x80)"
            stride_arg  = "serdes_lane, "
        elif ((reg_addr >= 0x1800) and (reg_addr < 0x1810)):
            stride_parm = "uint32_t virtual_lane, "
            stride_val  = "(virtual_lane * 0x10)"
            stride_arg  = "virtual_lane, "
        elif (reg_addr >= 0x6000):
            stride_parm = ""
            stride_val  = "0"
            stride_arg  = ""
        else:
            return
  
        accessor_h_file.write( "uint64_t get_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint64_t *reg_val );\n" )

        accessor_c_file.write( "/*******************************************************************\n" )
        accessor_c_file.write( "*  block   : " + block_name + "\n" )
        accessor_c_file.write( "*  register: " + reg_name + "\n" )
        accessor_c_file.write( "*  field   : " + fld_name + "\n" )
        accessor_c_file.write( "*  access  : " + access_mode + "\n" )
        accessor_c_file.write( "*----------+\n*\n*" )
        if "<br>" in elt.text:
            wrapped_text = elt.text.replace("<br>","\n*")
        else:
            wrapped_text2 = textwrap.fill(elt.text,70) + "\n"
            wrapped_text = wrapped_text2.replace("\n","\n* ")
        accessor_c_file.write( wrapped_text + "\n" )
        accessor_c_file.write( "*******************************************************************/\n" )
        accessor_c_file.write( "uint64_t get_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint64_t *reg_val )\n" )
        accessor_c_file.write( "{\n" )

        accessor_c_file.write( "    return( ((*reg_val) >> " + str(bit_ofs) + "ull) & width_msk( " + str(bit_width) + "ull ));\n" )
        accessor_c_file.write( "}\n\n" )

        if access_mode == "read-write":
            accessor_h_file.write( "uint64_t set_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint64_t *reg_val, uint64_t fld_val );\n" )
            accessor_c_file.write( "uint64_t set_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint64_t *reg_val, uint64_t fld_val )\n" )
            accessor_c_file.write( "{\n" )
            accessor_c_file.write( "    *reg_val= ( ((*reg_val) & ~fld_msk( " + str(bit_ofs) + "ull, " + str(bit_width) + "ull )) |\n" )
            accessor_c_file.write( "                ((fld_val & width_msk( " + str(bit_width) + "ull )) << " + str(bit_ofs) + "ull) );\n" )
            accessor_c_file.write( "    return *reg_val;\n" )
            accessor_c_file.write( "}\n\n" )


        umac4_h_file.write( "void umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_rd(bf_dev_id_t dev_id, uint32_t umac, " + stride_parm + "uint64_t *reg64, uint64_t *fld64, bool hw);\n")

        #umac4_c_file.write( "// block_base =" + str(block_base) + "\n")
        #umac4_c_file.write( "// stride_parm =" + stride_parm + " : " + str(stride_parm) + "\n")
        #umac4_c_file.write( "// stride_val =" + stride_val + " : " + str(stride_val) + "\n")
        #umac4_c_file.write( "// stride_arg =" + stride_arg + " : " + str(stride_arg) + "\n")

        umac4_c_file.write( "/*******************************************************************\n" )
        umac4_c_file.write( "*  block   : " + block_name + "\n" )
        umac4_c_file.write( "*  register: " + reg_name + "\n" )
        umac4_c_file.write( "*  field   : " + fld_name + "\n" )
        umac4_c_file.write( "*  access  : " + access_mode + "\n" )
        umac4_c_file.write( "*----------+\n*\n*" )
        if "<br>" in elt.text:
            wrapped_text = elt.text.replace("<br>","\n*")
        else:
            wrapped_text2 = textwrap.fill(elt.text,70) + "\n"
            wrapped_text = wrapped_text2.replace("\n","\n* ")
        umac4_c_file.write( wrapped_text + "\n" )
        umac4_c_file.write( "*******************************************************************/\n" )
        umac4_c_file.write( "void umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_rd(bf_dev_id_t dev_id, uint32_t umac, " + stride_parm + "uint64_t *reg64, uint64_t *fld64, bool hw) {\n")
        umac4_c_file.write( "  if (hw) {\n")
        umac4_c_file.write( "    umac4_rd64(dev_id, umac, " + str(block_base) + " + " + stride_val + ", reg64);\n")
        umac4_c_file.write( "  }\n")
        umac4_c_file.write( "  *fld64 = get_fld_" + block_name + "__" + reg_name + "__" + fld_name + "(reg64);\n")

        if stride_arg == "":
            umac4_c_file.write( "  autogen_log(\"TRC : %d: p%02d : --- : Rd : -------- : %08x : %s\", dev_id, umac, (uint32_t)(*fld64), __func__);\n")
        else:
            umac4_c_file.write( "  autogen_log(\"TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s\", dev_id, umac, " + stride_arg + "(uint32_t)(*fld64), __func__);\n")
        umac4_c_file.write( "}\n\n")

        if access_mode == "read-write":
            umac4_h_file.write( "void umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_wr(bf_dev_id_t dev_id, uint32_t umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64, bool hw);\n")
            umac4_h_file.write( "void umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, uint32_t umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64);\n")

            umac4_c_file.write( "void umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_wr(bf_dev_id_t dev_id, uint32_t umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64, bool hw) {\n")
            if stride_arg == "":
                umac4_c_file.write( "  autogen_log(\"TRC : %d: p%02d : --- : Wr : -------- : %08x : %s\", dev_id, umac, (uint32_t)(fld64), __func__);\n")
            else:
                umac4_c_file.write( "  autogen_log(\"TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s\", dev_id, umac, " + stride_arg + "(uint32_t)(fld64), __func__);\n")
            umac4_c_file.write( "  set_fld_" + block_name + "__" + reg_name + "__" + fld_name + "(reg64, fld64);\n")
            umac4_c_file.write( "  if (hw) {\n")
            umac4_c_file.write( "    umac4_wr64(dev_id, umac, " + str(block_base) + " + " + stride_val + ", *reg64);\n")
            umac4_c_file.write( "  }\n")
            umac4_c_file.write( "}\n\n")

            umac4_c_file.write( "void umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, uint32_t umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64) {\n")
            umac4_c_file.write( "  uint64_t unused_fld64;\n\n")
            umac4_c_file.write( "  umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_rd(dev_id, umac, " + stride_arg + "reg64, &unused_fld64, true);\n")
            umac4_c_file.write( "  umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_wr(dev_id, umac, " + stride_arg + "reg64, fld64, true);\n")
            umac4_c_file.write( "}\n\n\n")

        ######################
        bf_mac_ll_h_file.write( "bf_status_t bf_ll_umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_rd(bf_dev_id_t dev_id, uint32_t logical_umac, " + stride_parm + "uint64_t *reg64, uint64_t *fld64, bool hw);\n")

        bf_mac_ll_c_file.write( "/*******************************************************************\n" )
        bf_mac_ll_c_file.write( "*  access  : " + access_mode + "\n" )
        bf_mac_ll_c_file.write( "*  block   : " + block_name + "\n" )
        bf_mac_ll_c_file.write( "*  register: " + reg_name + "\n" )
        bf_mac_ll_c_file.write( "*  field   : " + fld_name + "\n" )
        bf_mac_ll_c_file.write( "*----------:\n" )
        bf_mac_ll_c_file.write( "*  dev_id  : logical chip identifier\n" )
        bf_mac_ll_c_file.write( "*  umac    : logical UMAC4 identifier (1-32)\n" )
        bf_mac_ll_c_file.write( "*  reg64   : u64 for resulting register value or contents to extract field from\n" )
        bf_mac_ll_c_file.write( "*  fld64   : u64 for field inserted or extracted\n" )
        bf_mac_ll_c_file.write( "*  hw      : true=read/write hw : false=insert/extract from reg64 (only)\n" )
        bf_mac_ll_c_file.write( "*----------+\n*\n*" )
        if "<br>" in elt.text:
            wrapped_text = elt.text.replace("<br>","\n*")
        else:
            wrapped_text2 = textwrap.fill(elt.text,70) + "\n"
            wrapped_text = wrapped_text2.replace("\n","\n* ")
        bf_mac_ll_c_file.write( wrapped_text + "\n" )
        bf_mac_ll_c_file.write( "*\n" )
        bf_mac_ll_c_file.write( "*******************************************************************/\n" )
        bf_mac_ll_c_file.write( "bf_status_t bf_ll_umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_rd(bf_dev_id_t dev_id, uint32_t logical_umac, " + stride_parm + "uint64_t *reg64, uint64_t *fld64, bool hw) {\n")
        bf_mac_ll_c_file.write( "  uint32_t physical_umac;\n")
        bf_mac_ll_c_file.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);\n\n")
        bf_mac_ll_c_file.write( "  if (rc != BF_SUCCESS) return rc;\n\n")
        bf_mac_ll_c_file.write( "  umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_rd(dev_id, physical_umac, " + stride_arg + "reg64, fld64, hw);\n")
        bf_mac_ll_c_file.write( "  return BF_SUCCESS;\n")
        bf_mac_ll_c_file.write( "}\n\n")

        if access_mode == "read-write":
            bf_mac_ll_h_file.write( "bf_status_t bf_ll_umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_wr(bf_dev_id_t dev_id, uint32_t logical_umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64, bool hw);\n")
            bf_mac_ll_h_file.write( "bf_status_t bf_ll_umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64);\n")

            bf_mac_ll_c_file.write( "bf_status_t bf_ll_umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_wr(bf_dev_id_t dev_id, uint32_t logical_umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64, bool hw) {\n")
            bf_mac_ll_c_file.write( "  uint32_t physical_umac;\n")
            bf_mac_ll_c_file.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);\n\n")
            bf_mac_ll_c_file.write( "  if (rc != BF_SUCCESS) return rc;\n\n")
            bf_mac_ll_c_file.write( "  umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_wr(dev_id, physical_umac, " + stride_arg + "reg64, fld64, hw);\n")
            bf_mac_ll_c_file.write( "  return BF_SUCCESS;\n")
            bf_mac_ll_c_file.write( "}\n\n")

            bf_mac_ll_c_file.write( "bf_status_t bf_ll_umac4_"+ block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, uint32_t logical_umac, " + stride_parm + "uint64_t *reg64, uint64_t  fld64) {\n")
            bf_mac_ll_c_file.write( "  uint64_t unused_fld64;\n")
            bf_mac_ll_c_file.write( "  uint32_t physical_umac;\n")
            bf_mac_ll_c_file.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, logical_umac, &physical_umac);\n\n")
            bf_mac_ll_c_file.write( "  if (rc != BF_SUCCESS) return rc;\n\n")
            bf_mac_ll_c_file.write( "  umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_rd(dev_id, physical_umac, " + stride_arg + "reg64, &unused_fld64, true);\n")
            bf_mac_ll_c_file.write( "  umac4_"+ block_name + "__" + reg_name + "__" + fld_name +  "_wr(dev_id, physical_umac, " + stride_arg + "reg64, fld64, true);\n")
            bf_mac_ll_c_file.write( "  return BF_SUCCESS;\n")
            bf_mac_ll_c_file.write( "}\n\n\n")

    
def parse_field( elt ):
    fld = {}
    for fld_elt in range(0, len(elt)):
        parse_fieldelt( elt[fld_elt], fld )
    return fld

def parse_resetelt(elt):
    if elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}value":
        print "----------value: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}mask":
        print "----------mask: " + elt.text
    
def parse_registerelt(elt, reg):
    global reg_name
    global reg_ofs
    global block_base

    if elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}name":
        print "--------reg-name: " + elt.text
        reg_name = elt.text
        reg["name"] = reg_name
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}hdl_path":
        print "--------hdl_path: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}addressOffset":
        print "--------addressOffset: " + elt.text 
        reg_ofs = elt.text
        reg["offset"] = reg_ofs
        def_file.write( "#define " + block_name + "__" + reg_name + " ((" + block_base + " + " + reg_ofs + "))\n" )
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}size":
        print "--------size: " + elt.text
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}reset":
        print "--------reset: "
        for reset in range(0,len(elt)):
            parse_resetelt( elt[reset] )
    elif elt.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}field":
        print "--------field: "
        reg["fld_list"].append( parse_field( elt ) )

def parse_register( elt ):
    reg = {}
    reg["fld_list"] = []
    for reg_elt in range(0, len(elt)):
        parse_registerelt(elt[reg_elt], reg)
    return reg

def parse_addressblock(blk, addr_block ):
    global block_base
    global block_name

    if blk.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}name":
        print "------addr-block-name       : " + blk.text
        block_name = blk.text
        addr_block["block_name"] = blk.text 
    elif blk.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}baseAddress":
        print "------BaseAddress: " + blk.text
        block_base = blk.text
        addr_block["block_base"] = int(block_base,base=16)
    elif blk.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}range":
        print "------Range      : " + blk.text
    elif blk.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}width":
        print "------Width      : " + blk.text
    elif blk.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}register":
        num_regs_in_addrmap = 0

        addr_block["reg_list"].append( parse_register( blk ))

def parse_addr_block( elt ):
        addr_block = {}
        addr_block["reg_list"] = []
        for address_blk in range(0, len(elt)):
            parse_addressblock( elt[address_blk], addr_block )
        return addr_block

def parse_memorymapelts(map, new_map):
    if map.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}name":
        print "------map-name: " + map.text
        new_map["name"] = map.text + "_addrmap"
    elif map.tag == "{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}addressBlock":
        new_map["addr_block_list"].append( parse_addr_block( map ))

def parse_memorymap(map):
    new_map = {}
    new_map["addr_block_list"] = []

    for map_elt in range(0, len(map)):
        parse_memorymapelts( map[map_elt], new_map )
    return new_map

def ParseComiraRegs(filename):
    tree = ET.ElementTree(file=filename)
    map_list = []
    for elem in tree.iter(tag='{http://www.spiritconsortium.org/XMLSchema/SPIRIT/1.5}memoryMap'):
        print "Map: " + elem.text
        map_list.append( parse_memorymap( elem ))
    return map_list

###############
# reg_decoder_fld_t pcie_regs_scratch_reg_fld_list[] = {
#     { "scratch", 31, 0, 0,
#       "This register is located in PCIe clock domain and is always accessible through PCIe.It can be used for initial debug" },
# };
# reg_decoder_t pcie_regs_scratch_reg = { 1, pcie_regs_scratch_reg_fld_list, 32 /* bits */ };
#
#
# cmd_arg_item_t tofino_list[] = {
# { "device_select", &dvsl_addrmap, 0x0, NULL },
#...
# };
# cmd_arg_t tofino = { 137, tofino_list };
#
###############

def build_dbg_info( map_list ):
    map = map_list[0]
    for addr_blk_n in range(0,len(map["addr_block_list"])):
        addr_blk = map["addr_block_list"][addr_blk_n]
        n_regs = 0
        for reg_n in range(0,len(addr_blk["reg_list"])):
            n_regs += 1
            reg = addr_blk["reg_list"][reg_n]

            def_file.write( "#define " + addr_blk["block_name"] + "__" + reg["name"] + " ((" + hex(addr_blk["block_base"]) + " + " + reg["offset"] + "))\n" )

            dbg_info_file.write( "reg_decoder_fld_t " + addr_blk["block_name"] + "_" + reg["name"] + "_fld_list[] = {\n")
            n_flds = 0
            for fld in range(0,len(reg["fld_list"])):
                n_flds += 1
                f = reg["fld_list"][fld]
                dbg_info_file.write ("{ " + "\"" + f["fld_name"] + "\"" + ", " 
                                    + str(f["bit_ofs"] + f["bit_width"] - 1) + ", "
                                    + str(f["bit_ofs"]) + ", 0,"
                                    + str(f["accessor_entry"]) + ",\n"
                                    + "\"" + f["raw_desc"] + "\"" + " },\n\n" )


            dbg_info_file.write( "};\n" )
            dbg_info_file.write( "reg_decoder_t " + addr_blk["block_name"] + "_" + reg["name"] + "_flds = { " + str(n_flds) + ", " + addr_blk["block_name"] + "_" + reg["name"] + "_fld_list, 16 };\n\n")
    map = map_list[0]
    for addr_blk_n in range(0,len(map["addr_block_list"])):
        addr_blk = map["addr_block_list"][addr_blk_n]
        
        dbg_info_file.write( "cmd_arg_item_t " + addr_blk["block_name"] + "_regs[] = {\n")
        n_regs = 0
        for reg_n in range(0,len(addr_blk["reg_list"])):
            n_regs += 1
            reg = addr_blk["reg_list"][reg_n]

            dbg_info_file.write ("{ " + "\"" + reg["name"] + "\"" + ", " 
                                    + "NULL, "
                                    + reg["offset"] + "*4, "
                                    + "&" + addr_blk["block_name"] + "_" + reg["name"] + "_flds },\n" )
            
        dbg_info_file.write( "};\n" )
        dbg_info_file.write( "cmd_arg_t " + addr_blk["block_name"] + "_list = { " + str(n_regs) + ", " + addr_blk["block_name"] + "_regs };\n\n")

    map = map_list[0]
        
    dbg_info_file.write( "cmd_arg_item_t " + "lld_comira_regs[] = {\n")
    n_maps = 0
    for addr_blk_n in range(0,len(map["addr_block_list"])):
        n_maps += 1
        addr_blk = map["addr_block_list"][addr_blk_n]

        dbg_info_file.write ("{ " + "\"" + addr_blk["block_name"] + "\"" + ", " 
                                  + "&" + addr_blk["block_name"] + "_list, "
                                  + hex(addr_blk["block_base"]) + "*4, "
                                  + "NULL },\n" )
            
    dbg_info_file.write( "};\n" )
    dbg_info_file.write( "cmd_arg_t " + "comira_list = { " + str(n_maps) + ", " + "lld_comira_regs };\n\n")


if __name__ == "__main__":

    global def_file

    # make sure md5 file exists
    subprocess.call(["touch", "comira_regs.xml.md5"])
    with open('comira_regs.xml.current.md5', "w") as outfile:
        my_cmd = ["md5sum", "comira_regs.xml"]
        subprocess.call(my_cmd, stdout=outfile)
        outfile.close()
    retcode = subprocess.call(["diff", "comira_regs.xml.current.md5", "comira_regs.xml.md5"])
    if retcode == 0:
        subprocess.call(["rm", "comira_regs.xml.current.md5"])
        #
        # Now check for changes to this python script
        #
        # make sure md5 file exists
        subprocess.call(["touch", "umac4-xml-to-code.py.md5"])
        with open('umac4-xml-to-code.py.current.md5', "w") as outfile:
            my_cmd = ["md5sum", "umac4-xml-to-code.py"]
            subprocess.call(my_cmd, stdout=outfile)
            outfile.close()
        retcode = subprocess.call(["diff", "umac4-xml-to-code.py.current.md5", "umac4-xml-to-code.py.md5"])
        if retcode == 0:
            subprocess.call(["rm", "umac4-xml-to-code.py.current.md5"])
            quit()
    subprocess.call(["mv", "comira_regs.xml.current.md5", "comira_regs.xml.md5"])
    subprocess.call(["mv", "umac4-xml-to-code.py.current.md5", "umac4-xml-to-code.py.md5"])

    def_file         = open( "umac4c8_def.h", "w" )
    accessor_c_file  = open( "umac4c8_fld_access.c", "w" )
    accessor_h_file  = open( "umac4c8_fld_access.h", "w" )
    umac4_c_file     = open( "umac4c8_access.c", "w" )
    umac4_h_file     = open( "umac4c8_access.h", "w" )
    bf_mac_ll_c_file = open( "bf_ll_umac4_if.c", "w" )
    bf_mac_ll_h_file = open( "bf_ll_umac4_if.h", "w" )

    accessor_h_file.write( "/* clang-format off */\n" )
    accessor_c_file.write( "/* clang-format off */\n" )
    accessor_c_file.write( "#include <stdbool.h>\n" )
    accessor_c_file.write( "#include <stdint.h>\n" )
    accessor_c_file.write( "#define width_msk(width) ((uint64_t)(((uint64_t)0xffffffffffffffffull) >> (64ull - (uint64_t)width)))\n")
    accessor_c_file.write( "#define fld_msk( bit, width ) (width_msk(width) << bit)\n\n")

    umac4_h_file.write( "/* clang-format off */\n" )
    umac4_c_file.write( "/* clang-format off */\n" )
    umac4_c_file.write( "#include <stdbool.h>\n" )
    umac4_c_file.write( "#include <stdint.h>\n" )
    umac4_c_file.write( "#include \"umac4c8_fld_access.h\"\n\n" )
    umac4_c_file.write( "#include <bf_types/bf_types.h>\n" )
    umac4_c_file.write( "#include \"autogen-required-headers.h\"\n\n" )

    bf_mac_ll_h_file.write( "/* clang-format off */\n" )
    bf_mac_ll_c_file.write( "/* clang-format off */\n" )
    bf_mac_ll_c_file.write( "#include <stdbool.h>\n" )
    bf_mac_ll_c_file.write( "#include <stdint.h>\n" )
    bf_mac_ll_c_file.write( "#include <bf_types/bf_types.h>\n" )
    bf_mac_ll_c_file.write( "#include \"port_mgr_tof2/umac4c8_access.h\"\n\n" )
    bf_mac_ll_c_file.write( "extern bf_status_t bf_map_logical_umac4_to_physical(bf_dev_id_t dev_id,\n")
    bf_mac_ll_c_file.write( "                                                    uint32_t logical_umac,\n")
    bf_mac_ll_c_file.write( "                                                    uint32_t *physical_umac);\n")

    register_xml_file_name = str(sys.argv[1])
    map_list = ParseComiraRegs( register_xml_file_name )

    accessor_h_file.close()
    accessor_c_file.close()
    def_file.close()

