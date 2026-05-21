#!/usr/bin/python

"""
aw-xml-to-code.py: Parse Alphawave XML register descriptions  

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
global text

"""
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.accellera.org/XMLSchema/IPXACT/1685-2014 http://www.accellera.org/XMLSchema/IPXACT/1685-2014/index.xsd"

"""

def parse_fieldelt(elt, fld):
    global fld_name
    global bit_ofs
    global bit_width
    global access_mode
    global accessor_entry
    global text
    global block_name

    print( "[" + block_name + "] " + elt.tag )
    #return

    # only process the 0-th block of each type
    if block_name == "RX0":
      generic_block_name = "RX"
    elif block_name == "TX0":
      generic_block_name = "TX"
    elif block_name == "ETH0":
      generic_block_name = "ETH"
    elif block_name == "DFX0":
      generic_block_name = "DFX"
    elif block_name == "CMN":
      generic_block_name = "CMN"
    else:
      return

    if elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}name":
        print "----------fld-name: " + elt.text
        fld_name = elt.text
        fld["fld_name"] = fld_name
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}bitOffset":
        print "----------bitOffset: " + elt.text
        bit_ofs = int(elt.text)
        fld["bit_ofs"] = bit_ofs
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}bitWidth":
        print "----------bitWidth: " + elt.text
        bit_width = int(elt.text)
        fld["bit_width"] = bit_width
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}description":
        if not elt.text:
            elt.text = "<none>"
        print "----------description: " + elt.text
        description = elt.text
        text = description
        fld["desc"] = textwrap.fill(description,40)
        fld["raw_desc"] = elt.text.replace("\n"," ")
        fld["raw_desc"] = fld["raw_desc"].replace("\"","'")

    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}reset":
        print "----------reset: " + elt.text
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}access":
        print "----------access: " + elt.text
        access_mode  = elt.text
        fld["access_mode"] = access_mode 

        stride_parm = ""
        stride_val  = "0"
        stride_arg  = ""

        accessor_h_file.write( "uint32_t get_fld_" + generic_block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val );\n" )

        accessor_c_file.write( "/*******************************************************************\n" )
        accessor_c_file.write( "*  block   : " + generic_block_name + "\n" )
        accessor_c_file.write( "*  register: " + reg_name + "\n" )
        accessor_c_file.write( "*  field   : " + fld_name + "\n" )
        accessor_c_file.write( "*  access  : " + access_mode + "\n" )
        if bit_width > 1:
          accessor_c_file.write( "*  bit(s)  : [" + str(bit_ofs + bit_width - 1) + ":" + str(bit_ofs) + "]\n" )
        else:
          accessor_c_file.write( "*  bit     : [" + str(bit_ofs) + "]\n" )
        accessor_c_file.write( "*----------+\n*\n*" )
        if "<br>" in text:
            wrapped_text = text.replace("<br>","\n*")
        else:
            wrapped_text2 = textwrap.fill(text,70) + "\n"
            wrapped_text = wrapped_text2.replace("\n","\n* ")
        try:
            new_text = wrapped_text.encode(encoding="ascii",errors="replace")
            wrapped_text = new_text
        except:
            pass
        accessor_c_file.write( "*\n* " + wrapped_text + "\n" )
        accessor_c_file.write( "*******************************************************************/\n" )
        accessor_c_file.write( "uint32_t get_fld_" + generic_block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val )\n" )
        accessor_c_file.write( "{\n" )

        accessor_c_file.write( "    return( ((*reg_val) >> " + str(bit_ofs) + ") & width_msk( " + str(bit_width) + " ));\n" )
        accessor_c_file.write( "}\n\n" )

        if access_mode == "read-write":
            accessor_h_file.write( "uint32_t set_fld_" + generic_block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val, uint32_t fld_val );\n" )
            accessor_c_file.write( "uint32_t set_fld_" + generic_block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val, uint32_t fld_val )\n" )
            accessor_c_file.write( "{\n" )
            accessor_c_file.write( "    *reg_val= ( ((*reg_val) & ~fld_msk( " + str(bit_ofs) + ", " + str(bit_width) + " )) |\n" )
            accessor_c_file.write( "                ((fld_val & width_msk( " + str(bit_width) + " )) << " + str(bit_ofs) + ") );\n" )
            accessor_c_file.write( "    return *reg_val;\n" )
            accessor_c_file.write( "}\n\n" )


        aw_h_file.write( "void aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_rd(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t *fld32, bool hw);\n")

        #aw_c_file.write( "// block_base =" + str(block_base) + "\n")
        #aw_c_file.write( "// stride_parm =" + stride_parm + " : " + str(stride_parm) + "\n")
        #aw_c_file.write( "// stride_val =" + stride_val + " : " + str(stride_val) + "\n")
        #aw_c_file.write( "// stride_arg =" + stride_arg + " : " + str(stride_arg) + "\n")

        aw_c_file.write( "/*******************************************************************\n" )
        aw_c_file.write( "*  block   : " + generic_block_name + "\n" )
        aw_c_file.write( "*  register: " + reg_name + "\n" )
        aw_c_file.write( "*  field   : " + fld_name + "\n" )
        aw_c_file.write( "*  access  : " + access_mode + "\n" )
        if bit_width > 1:
          aw_c_file.write( "*  bit(s)  : [" + str(bit_ofs + bit_width - 1) + ":" + str(bit_ofs) + "]\n" )
        else:
          aw_c_file.write( "*  bit     : [" + str(bit_ofs) + "]\n" )
        aw_c_file.write( "*----------+\n*\n*" )
        if "<br>" in text:
            wrapped_text = text.replace("<br>","\n*")
        else:
            wrapped_text2 = textwrap.fill(text,70) + "\n"
            wrapped_text = wrapped_text2.replace("\n","\n* ")
        try:
            new_text = wrapped_text.encode(encoding="ascii",errors="replace")
            wrapped_text = new_text
        except:
            pass
        aw_c_file.write( "*\n* " + wrapped_text + "\n" )
        aw_c_file.write( "*******************************************************************/\n" )
        aw_c_file.write( "void aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_rd(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t *fld32, bool hw) {\n")
        aw_c_file.write( "  if (hw) {\n")
        aw_c_file.write( "    aw_rd32(dev_id, dev_port, " + str(block_base) + " + " + stride_val + ", reg32);\n")
        aw_c_file.write( "  }\n")
        aw_c_file.write( "  *fld32 = get_fld_" + generic_block_name + "__" + reg_name + "__" + fld_name + "(reg32);\n")

        if stride_arg == "":
            aw_c_file.write( "  autogen_log(\"TRC : %d: p%02d : --- : Rd : -------- : %08x : %s\", dev_id, dev_port, (uint32_t)(*fld32), __func__);\n")
        else:
            aw_c_file.write( "  autogen_log(\"TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s\", dev_id, dev_port, " + stride_arg + "(uint32_t)(*fld32), __func__);\n")
        aw_c_file.write( "}\n\n")

        if access_mode == "read-write":
            aw_h_file.write( "void aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_wr(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32, bool hw);\n")
            aw_h_file.write( "void aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32);\n")

            aw_c_file.write( "void aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_wr(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32, bool hw) {\n")
            if stride_arg == "":
                aw_c_file.write( "  autogen_log(\"TRC : %d: p%02d : --- : Wr : -------- : %08x : %s\", dev_id, dev_port, (uint32_t)(fld32), __func__);\n")
            else:
                aw_c_file.write( "  autogen_log(\"TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s\", dev_id, dev_port, " + stride_arg + "(uint32_t)(fld32), __func__);\n")
            aw_c_file.write( "  set_fld_" + generic_block_name + "__" + reg_name + "__" + fld_name + "(reg32, fld32);\n")
            aw_c_file.write( "  if (hw) {\n")
            aw_c_file.write( "    aw_wr32(dev_id, dev_port, " + str(block_base) + " + " + stride_val + ", *reg32);\n")
            aw_c_file.write( "  }\n")
            aw_c_file.write( "}\n\n")

            aw_c_file.write( "void aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32) {\n")
            aw_c_file.write( "  uint32_t unused_fld32;\n\n")
            aw_c_file.write( "  aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_rd(dev_id, dev_port, " + stride_arg + "reg32, &unused_fld32, true);\n")
            aw_c_file.write( "  aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_wr(dev_id, dev_port, " + stride_arg + "reg32, fld32, true);\n")
            aw_c_file.write( "}\n\n\n")

        ######################
        bf_mac_ll_h_file.write( "bf_status_t bf_ll_aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_rd(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t *fld32, bool hw);\n")

        bf_mac_ll_c_file.write( "/*******************************************************************\n" )
        bf_mac_ll_c_file.write( "*  block   : " + generic_block_name + "\n" )
        bf_mac_ll_c_file.write( "*  register: " + reg_name + "\n" )
        bf_mac_ll_c_file.write( "*  field   : " + fld_name + "\n" )
        bf_mac_ll_c_file.write( "*  access  : " + access_mode + "\n" )
        if bit_width > 1:
          bf_mac_ll_c_file.write( "*  bit(s)  : [" + str(bit_ofs + bit_width - 1) + ":" + str(bit_ofs) + "]\n" )
        else:
          bf_mac_ll_c_file.write( "*  bit     : [" + str(bit_ofs) + "]\n" )
        bf_mac_ll_c_file.write( "*----------:\n" )
        bf_mac_ll_c_file.write( "*  dev_id  : logical chip identifier\n" )
        bf_mac_ll_c_file.write( "*  dev_port: bf_dev_port_t\n" )
        bf_mac_ll_c_file.write( "*  reg32   : u32 for resulting register value or contents to extract field from\n" )
        bf_mac_ll_c_file.write( "*  fld32   : u32 for field inserted or extracted\n" )
        bf_mac_ll_c_file.write( "*  hw      : true=read/write hw : false=insert/extract from reg32 (only)\n" )
        bf_mac_ll_c_file.write( "*----------+\n*\n*" )
        if "<br>" in text:
            wrapped_text = text.replace("<br>","\n*")
        else:
            wrapped_text2 = textwrap.fill(text,70) + "\n"
            wrapped_text = wrapped_text2.replace("\n","\n* ")
        try:
            new_text = wrapped_text.encode(encoding="ascii",errors="replace")
            wrapped_text = new_text
        except:
            pass
        bf_mac_ll_c_file.write( "*\n* " + wrapped_text + "\n" )
        bf_mac_ll_c_file.write( "*\n" )
        bf_mac_ll_c_file.write( "*******************************************************************/\n" )
        bf_mac_ll_c_file.write( "bf_status_t bf_ll_aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_rd(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t *fld32, bool hw) {\n")
        bf_mac_ll_c_file.write( "  aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_rd(dev_id, dev_port, " + stride_arg + "reg32, fld32, hw);\n")
        bf_mac_ll_c_file.write( "  return BF_SUCCESS;\n")
        bf_mac_ll_c_file.write( "}\n\n")

        if access_mode == "read-write":
            bf_mac_ll_h_file.write( "bf_status_t bf_ll_aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_wr(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32, bool hw);\n")
            bf_mac_ll_h_file.write( "bf_status_t bf_ll_aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32);\n")

            bf_mac_ll_c_file.write( "bf_status_t bf_ll_aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_wr(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32, bool hw) {\n")
            bf_mac_ll_c_file.write( "  aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_wr(dev_id, dev_port, " + stride_arg + "reg32, fld32, hw);\n")
            bf_mac_ll_c_file.write( "  return BF_SUCCESS;\n")
            bf_mac_ll_c_file.write( "}\n\n")

            bf_mac_ll_c_file.write( "bf_status_t bf_ll_aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name + "_rmw(bf_dev_id_t dev_id, bf_dev_port_t dev_port, " + stride_parm + "uint32_t *reg32, uint32_t  fld32) {\n")
            bf_mac_ll_c_file.write( "  uint32_t unused_fld32;\n")
            bf_mac_ll_c_file.write( "  aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_rd(dev_id, dev_port, " + stride_arg + "reg32, &unused_fld32, true);\n")
            bf_mac_ll_c_file.write( "  aw_"+ generic_block_name + "__" + reg_name + "__" + fld_name +  "_wr(dev_id, dev_port, " + stride_arg + "reg32, fld32, true);\n")
            bf_mac_ll_c_file.write( "  return BF_SUCCESS;\n")
            bf_mac_ll_c_file.write( "}\n\n\n")

    
def parse_field( elt ):
    fld = {}
    for fld_elt in range(0, len(elt)):
        parse_fieldelt( elt[fld_elt], fld )
    return fld

def parse_resetelt(elt):
    if elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}value":
        print "----------value: " + elt.text
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}mask":
        print "----------mask: " + elt.text
    
def parse_registerelt(elt, reg):
    global reg_name
    global reg_ofs
    global block_base

    if elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}name":
        print "--------reg-name: " + elt.text
        reg_name = elt.text
        reg["name"] = reg_name
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}size":
        print "--------size: " + elt.text
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}volatile":
        print "--------volatile: " + elt.text
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}access":
        print "--------access: " + elt.text
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}addressOffset":
        print "--------addressOffset: " + elt.text 
        reg_ofs = elt.text
        reg["offset"] = reg_ofs
        def_file.write( "#define " + block_name + "__" + reg_name + " ((" + block_base + " + " + reg_ofs + "))\n" )
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}reset":
        print "--------reset: "
        for reset in range(0,len(elt)):
            parse_resetelt( elt[reset] )
    elif elt.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}field":
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

    if blk.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}name":
        print "------addr-block-name       : " + blk.text
        block_name = blk.text
        addr_block["block_name"] = blk.text 
    elif blk.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}baseAddress":
        print "------BaseAddress: " + blk.text
        block_base = blk.text
        addr_block["block_base"] = int(block_base,base=16)
    elif blk.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}range":
        print "------Range      : " + blk.text
    elif blk.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}width":
        print "------Width      : " + blk.text
    elif blk.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}register":
        num_regs_in_addrmap = 0

        addr_block["reg_list"].append( parse_register( blk ))

def parse_addr_block( elt ):
        addr_block = {}
        addr_block["reg_list"] = []
        for address_blk in range(0, len(elt)):
            parse_addressblock( elt[address_blk], addr_block )
        return addr_block

def parse_memorymapelts(map, new_map):

    if map.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}name":
        print "------map-name: " + map.text
        new_map["name"] = map.text + "_addrmap"
    elif map.tag == "{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}addressBlock":
        new_map["addr_block_list"].append( parse_addr_block( map ))

def parse_memorymap(map):
    new_map = {}
    new_map["addr_block_list"] = []

    for map_elt in range(0, len(map)):
        parse_memorymapelts( map[map_elt], new_map )
    return new_map

def ParseAlphawaveRegs(filename):
    tree = ET.parse(filename) # ET.ElementTree(file=filename)
    root = tree.getroot()
    #for child in root:
    #  print str(child)

    map_list = []
    #for elem in root.iter():
    #for elem in root.findall('{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}memoryMap' ):

    for elem in tree.iter(tag='{http://www.accellera.org/XMLSchema/IPXACT/1685-2014}memoryMap'):
        print str(elem)
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

    def_file         = open( "aw_def.h", "w" )
    accessor_c_file  = open( "aw_fld_access.c", "w" )
    accessor_h_file  = open( "aw_fld_access.h", "w" )
    aw_c_file     = open( "aw_access.c", "w" )
    aw_h_file     = open( "aw_access.h", "w" )
    bf_mac_ll_c_file = open( "bf_ll_aw_if.c", "w" )
    bf_mac_ll_h_file = open( "bf_ll_aw_if.h", "w" )

    accessor_h_file.write( "/* clang-format off */\n" )
    accessor_c_file.write( "/* clang-format off */\n" )
    accessor_c_file.write( "#include <stdbool.h>\n" )
    accessor_c_file.write( "#include <stdint.h>\n" )
    accessor_c_file.write( "#define width_msk(width) ((uint32_t)(((uint32_t)0xffffffff) >> (32 - (uint32_t)width)))\n")
    accessor_c_file.write( "#define fld_msk( bit, width ) (width_msk(width) << bit)\n\n")

    aw_h_file.write( "/* clang-format off */\n" )
    aw_c_file.write( "/* clang-format off */\n" )
    aw_c_file.write( "#include <stdbool.h>\n" )
    aw_c_file.write( "#include <stdint.h>\n" )
    aw_c_file.write( "#include \"aw_fld_access.h\"\n\n" )
    aw_c_file.write( "#include <bf_types/bf_types.h>\n" )
    aw_c_file.write( "#include \"tof3-autogen-required-headers.h\"\n\n" )

    bf_mac_ll_h_file.write( "/* clang-format off */\n" )
    bf_mac_ll_c_file.write( "/* clang-format off */\n" )
    bf_mac_ll_c_file.write( "#include <stdbool.h>\n" )
    bf_mac_ll_c_file.write( "#include <stdint.h>\n" )
    bf_mac_ll_c_file.write( "#include <bf_types/bf_types.h>\n" )
    bf_mac_ll_c_file.write( "#include \"aw_access.h\"\n\n" )

    register_xml_file_name = str(sys.argv[1])
    map_list = ParseAlphawaveRegs( register_xml_file_name )

    accessor_h_file.close()
    accessor_c_file.close()
    def_file.close()

