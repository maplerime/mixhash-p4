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

        accessor_entry += 1
        #
        # Note: The Comira "accessor index" is offset by 10000 to
        #       distinguish them from th CSR-defined Tofino reg
        #       accessors. Thye must be looked up using a different
        #       function in lld_reg_parse.c
        # 
        fld["accessor_entry"] = 10000 + accessor_entry

        accessor_h_file.write( "extern uint32_t get_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val );\n" )

        accessor_c_file.write( "/* block   : " + block_name + "\n" )
        accessor_c_file.write( "   register: " + reg_name + "\n" )
        accessor_c_file.write( "   field   : " + fld_name + "\n" )
        accessor_c_file.write( "   access  : " + access_mode + "\n\n" )
        accessor_c_file.write( textwrap.fill(elt.text,70) + "\n*/\n" )
        accessor_c_file.write( "uint32_t get_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val )\n" )
        accessor_c_file.write( "{\n" )

        # accessor_c_file.write( "    printf(\" *reg_val=0x%08x\\n\", *reg_val );\n" )
        # accessor_c_file.write( "    printf(\" bit_ofs =%d\\n\", " + str(bit_ofs) + " );\n" )
        # accessor_c_file.write( "    printf(\" mask    =%8x\\n\", width_msk( " + str(bit_width) + " ) );\n" )

        accessor_c_file.write( "    return( ((*reg_val) >> " + str(bit_ofs) + ") & width_msk( " + str(bit_width) + " ));\n" )
        accessor_c_file.write( "}\n\n" )

        dbg_access_file.write( "        { { .read_f_32 = get_fld_" + block_name + "__" + reg_name + "__" + fld_name + " },\n")

        if access_mode == "read-write":
            accessor_h_file.write( "extern uint32_t set_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val, uint32_t fld_val );\n" )
            accessor_c_file.write( "uint32_t set_fld_" + block_name + "__" + reg_name + "__" + fld_name + "( uint32_t *reg_val, uint32_t fld_val )\n" )
            accessor_c_file.write( "{\n" )
            accessor_c_file.write( "    *reg_val= ( ((*reg_val) & ~fld_msk( " + str(bit_ofs) + ", " + str(bit_width) + " )) |\n" )
            accessor_c_file.write( "                ((fld_val & width_msk( " + str(bit_width) + " )) << " + str(bit_ofs) + ") );\n" )
            accessor_c_file.write( "    return *reg_val;\n" )
            accessor_c_file.write( "}\n\n" )

            dbg_access_file.write( "          { .write_f_32_rtn_32 = set_fld_" + block_name + "__" + reg_name + "__" + fld_name + " } },\n")
        else:
            dbg_access_file.write( "          { .write_f = NULL } },\n" )

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
        # def_file.write( "#define " + block_name + "__" + reg_name + " ((" + block_base + " + " + reg_ofs + "))\n" )
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
        subprocess.call(["touch", "build_comira_reg_info.py.md5"])
        with open('build_comira_reg_info.py.current.md5', "w") as outfile:
            my_cmd = ["md5sum", "build_comira_reg_info.py"]
            subprocess.call(my_cmd, stdout=outfile)
            outfile.close()
        retcode = subprocess.call(["diff", "build_comira_reg_info.py.current.md5", "build_comira_reg_info.py.md5"])
        if retcode == 0:
            subprocess.call(["rm", "build_comira_reg_info.py.current.md5"])
            quit()
    subprocess.call(["mv", "comira_regs.xml.current.md5", "comira_regs.xml.md5"])
    subprocess.call(["mv", "build_comira_reg_info.py.current.md5", "build_comira_reg_info.py.md5"])

    def_file        = open( "comira_reg_def_autogen.h", "w" )
    accessor_c_file = open( "comira_reg_access_autogen.c", "w" )
    accessor_h_file = open( "comira_reg_access_autogen.h", "w" )
    dbg_info_file   = open( "comira_dbg_info_autogen.h", "wb" )
    dbg_access_file = open( "comira_dbg_access_autogen.h", "w" )

    accessor_c_file.write( "#include <stdio.h>\n\n" )
    accessor_c_file.write( "#include <stdint.h>\n\n" )
    accessor_c_file.write( "#define width_msk(width) ((uint32_t)(((uint32_t)0xffffffff) >> (32 - width)))\n")
    accessor_c_file.write( "#define fld_msk( bit, width ) (width_msk(width) << bit)\n\n")

    dbg_access_file.write( "#if 0\n\n")
    dbg_access_file.write( "#include <lld/lld_accessor_types.h>\n\n")
    dbg_access_file.write( "typedef struct comira_fld_handlers_t {\n")
    dbg_access_file.write( "  read_u  read_f;\n")
    dbg_access_file.write( "  write_u write_f;\n")
    dbg_access_file.write( "} comira_fld_handlers_t;\n\n")

    dbg_access_file.write( "void comira_auto_gen_get_fields( int      entry,\n")
    dbg_access_file.write( "                                 char   **name,\n")
    dbg_access_file.write( "                                 int     *msb,\n")
    dbg_access_file.write( "                                 int     *lsb,\n")
    dbg_access_file.write( "                                 read_u  *getp_fn,\n")
    dbg_access_file.write( "                                 write_u *setp_fn )\n")
    dbg_access_file.write( "{\n")
    dbg_access_file.write( "    comira_fld_handlers_t comira_fld_array[] = {\n")

    accessor_entry = 0

    register_xml_file_name = str(sys.argv[1])
    map_list = ParseComiraRegs( register_xml_file_name )

    dbg_access_file.write( "};\n" )

    dbg_access_file.write( "    if (getp_fn) *getp_fn = comira_fld_array[ entry - 10000 ].read_f;\n" )
    dbg_access_file.write( "    if (setp_fn) *setp_fn = comira_fld_array[ entry - 10000 ].write_f;\n" )
    dbg_access_file.write( "    (void)name;\n" )
    dbg_access_file.write( "    (void)msb;\n" )
    dbg_access_file.write( "    (void)lsb;\n" )
    dbg_access_file.write( "}\n" )
    dbg_access_file.write( "#endif //0\n" )

    build_dbg_info( map_list )

    dbg_access_file.close()
    dbg_info_file.close()
    accessor_h_file.close()
    accessor_c_file.close()
    def_file.close()
