from __future__ import print_function

import argparse
import os
import re
import sys
import time
try:
    import pexpect
except ImportError:
    sys.stderr.write("You do not have pexpect installed\n")

from pprint import pprint
from time import sleep

csr_file = ""
rspec_nm = ""
offset_path = ""
rotated_offset_path = ""
fn_key = ""
expect_tmo=2

class Pexp():
    """
    class Pexp
    """

    child = None

    def __init__(self):
        self.child = pexpect.spawn("cat " + csr_file)
        #self.child.setecho(True)
        self.child.logfile = sys.stdout

    def send_only(self, cmd):
        self.child.send(cmd)

    def send(self, cmd):
        self.child.send(cmd)
        self.child.send("\n")

    def expect(self, rsp):
        self.child.expect(rsp)

    def send_expect_with_timeout(self, cmd, rsp, time_out):
        self.child.send(cmd)
        self.child.send("\n")
        self.child.expect(cmd)
        self.child.expect(rsp,timeout=time_out)
        self.child.before

    def send_expect(self, cmd, rsp):
        self.child.send(cmd)
        self.child.send("\n")
        self.child.expect(cmd)
        self.child.expect(rsp,timeout=expect_tmo)
        self.child.before

    def expect_multiple(self, exp_list):
        rc = self.child.expect(exp_list,timeout=2)
        self.child.before
        return rc

    def expect2(self, exp1, exp2):
        rc = self.child.expect([exp1,exp2],timeout=2)
        self.child.before
        return rc

    def expect2_w_tmo(self, exp1, exp2, tmo):
        rc = self.child.expect([exp1,exp2],timeout=tmo)
        self.child.before
        return rc

    def expect_w_tmo(self, exp, tout):
        rc = self.child.expect([exp,pexpect.TIMEOUT],timeout=tout)
        self.child.before
        return rc

    def str_extract(self, _before, _after):
        rc = self.child.expect([_before,pexpect.TIMEOUT],timeout=expect_tmo)
        if rc == 1:
            return "TIMEOUT"
        else:
            #self.child.expect(_after)
            self.child.expect([_after,pexpect.TIMEOUT],timeout=expect_tmo)
            return self.child.before


def log_to_out_file(out_file, cmd):
    child.send(cmd)
    child.expect(cmd)
    done = False
    while not done:
        line = child.str_extract("","\n")
        if line == "bf-sde> ":
            done = True
        fprint(out_file, line)
    fprint(out_file, "\n")




def new_wide_register_fld_with_these(new_code, map_nm, reg_nm, fld_title, fld_description, fld_nm):

    structured_reg_nm = reg_nm

    # special case for some channel-ized registers
    channelized = False
    if reg_nm == "eth_onestep_ets_offset_ctrl":
        channelized = True
    elif reg_nm == "cts_fifo_out":
        channelized = True

    if channelized:
        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]) + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d : ch%d : Wr : -------- : %016\" PRIu64 \" : %s\", dev_id, umac, ch, val, __func__);\n")
        new_code.write("  setp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p, val);\n")
        new_code.write("\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    uint32_t reg_val;\n")
        new_code.write("    reg_val = (uint32_t)(*data_p & 0xffffffffull);\n")
        new_code.write("    autogen_wr(dev_id, ofs, reg_val);\n")
        new_code.write("    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);\n")
        new_code.write("    autogen_wr(dev_id, ofs + 4, reg_val);\n")
        new_code.write("  }\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]) + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    uint32_t upper, lower;\n")
        new_code.write("    autogen_rd(dev_id, ofs, &lower);\n")
        new_code.write("    autogen_rd(dev_id, ofs + 4, &upper);\n")
        new_code.write("    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));\n")
        new_code.write("  }\n")
        new_code.write("  *val = getp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p);\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d : ch%d : Rd : -------- : %016\" PRIu64 \" : %s\", dev_id, umac, ch, *val, __func__);\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val) {\n")
        new_code.write("  uint64_t unused_fld = 0;\n\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "ch, data_p, &unused_fld, true);\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "ch, data_p, val, true);\n")
        new_code.write("}\n\n")

    else:
        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint64_t *data_p, uint64_t val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint64_t *data_p, uint64_t val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ") + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d :     : Wr : -------- : %016\" PRIu64 \" : %s\", dev_id, umac, (uint32_t)val, __func__);\n")
        new_code.write("  setp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p, val);\n")
        new_code.write("\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    uint32_t reg_val;\n")
        new_code.write("    reg_val = (uint32_t)(*data_p & 0xffffffffull);\n")
        new_code.write("    autogen_wr(dev_id, ofs, reg_val);\n")
        new_code.write("    reg_val = (uint32_t)((*data_p >> 32ull) & 0xffffffffull);\n")
        new_code.write("    autogen_wr(dev_id, ofs + 4, reg_val);\n")
        new_code.write("  }\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint64_t *data_p, uint64_t *val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint64_t *data_p, uint64_t *val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ") + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    uint32_t upper, lower;\n")
        new_code.write("    autogen_rd(dev_id, ofs, &lower);\n")
        new_code.write("    autogen_rd(dev_id, ofs + 4, &upper);\n")
        new_code.write("    *data_p = (uint64_t)(((uint64_t)upper << 32ull) | ((uint64_t)lower));\n")
        new_code.write("  }\n")
        new_code.write("  *val = getp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p);\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d :     : Rd : -------- : %016\" PRIu64 \" : %s\", dev_id, umac, *val, __func__);\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint64_t *data_p, uint64_t val);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint64_t *data_p, uint64_t val) {\n")
        new_code.write("  uint64_t unused_fld = 0;\n\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "data_p, &unused_fld, true);\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "data_p, val, true);\n")
        new_code.write("}\n\n")

    #######################
    new_bf_code.write("/**********************************************************************\n")
    new_bf_code.write(" " + fld_description.replace('\r','').replace('\\n','').replace('  ',' ')  + "\n")
    new_bf_code.write("***********************************************************************/\n")

    if channelized:
        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_set"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "(" + fn_key_as_args + "ch, data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_get"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "(" + fn_key_as_args + "ch, data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint64_t *data_p, uint64_t val) {\n")
        new_bf_code.write("  uint64_t unused_fld = 0;\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "ch, data_p, &unused_fld, true);\n")
        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "ch, data_p, val, true);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

    else:
        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_set"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint64_t *data_p, uint64_t val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint64_t *data_p, uint64_t val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "( " + fn_key_as_args + "data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_get"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint64_t *data_p, uint64_t *val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint64_t *data_p, uint64_t *val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "( " + fn_key_as_args + "data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint64_t *data_p, uint64_t val);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint64_t *data_p, uint64_t val) {\n")
        new_bf_code.write("  uint64_t unused_fld = 0;\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "data_p, &unused_fld, true);\n")
        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "data_p, val, true);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")



def new_fld_with_these(new_code, map_nm, structured_reg_nm, fld_title, fld_description, fld_nm):
    new_code.write("/**********************************************************************\n")
    new_code.write(" " + fld_description.replace('\r','').replace('\\n','').replace('  ',' ')  + "\n")
    new_code.write("***********************************************************************/\n")

    reg_nm = structured_reg_nm.replace(".","_")

    # special case for some channel-ized registers
    channelized = False
    if reg_nm == "eth_onestep_ets_offset_ctrl":
        channelized = True
    elif reg_nm == "txff_ctrl":
        channelized = True
    elif reg_nm == "txff_status":
        channelized = True
    elif reg_nm == "txcrc_trunc_ctrl":
        channelized = True
    elif reg_nm == "rxff_ctrl":
        channelized = True
    elif reg_nm == "rxpkt_err_sts":
        channelized = True
    elif reg_nm == "cts_fifo_out":
        channelized = True

    # handle wide registers in a separate fn
    if reg_nm == "eth_onestep_ets_offset_ctrl":
        new_wide_register_fld_with_these(new_code, map_nm, reg_nm, fld_title, fld_description, fld_nm)
        return
    elif reg_nm == "eth_mac_ts_offset_ctrl":
        new_wide_register_fld_with_these(new_code, map_nm, reg_nm, fld_title, fld_description, fld_nm)
        return
    elif reg_nm == "cts_fifo_out":
        new_wide_register_fld_with_these(new_code, map_nm, reg_nm, fld_title, fld_description, fld_nm)
        return


    if channelized:
        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0 ;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]) + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d : ch%d : Wr : -------- : %08x : %s\", dev_id, umac, ch, val, __func__);\n")
        new_code.write("  setp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p, val);\n")
        new_code.write("\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    autogen_wr(dev_id, ofs, *data_p);\n")
        new_code.write("  }\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + "[ch]);\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + "[ch]) + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    autogen_rd(dev_id, ofs, data_p);\n")
        new_code.write("  }\n")
        new_code.write("  *val = getp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p);\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d : ch%d : Rd : -------- : %08x : %s\", dev_id, umac, ch, *val, __func__);\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val) {\n")
        new_code.write("  uint32_t unused_fld = 0;\n\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "ch, data_p, &unused_fld, true);\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "ch, data_p, val, true);\n")
        new_code.write("}\n\n")

    else:
        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint32_t *data_p, uint32_t val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key + "uint32_t *data_p, uint32_t val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ") + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d :     : Wr : -------- : %08x : %s\", dev_id, umac, (uint32_t)val, __func__);\n")
        new_code.write("  setp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p, val);\n")
        new_code.write("\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    autogen_wr(dev_id, ofs, *data_p);\n")
        new_code.write("  }\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint32_t *data_p, uint32_t *val, bool hw);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key + "uint32_t *data_p, uint32_t *val, bool hw) {\n")
        if map_nm == "eth100g_reg_rspec":
            new_code.write("  uint32_t ofs = 0;\n\n")
            new_code.write("  if (umac == 0) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else if (umac == 39) {\n")
            new_code.write("    ofs = offsetof(tof2_reg, " + rotated_offset_path + "." + structured_reg_nm + ");\n")
            new_code.write("  } else {\n")
            new_code.write("    bf_sys_assert(0);\n")
            new_code.write("  }\n")
        else:
            new_code.write("  uint32_t stride = offsetof(tof2_reg, eth400g_p2) - offsetof(tof2_reg, eth400g_p1);\n")
            new_code.write("  uint32_t ofs = offsetof(tof2_reg, " + offset_path + "." + structured_reg_nm + ") + ((umac - 1) * stride);\n\n")
            new_code.write("  bf_sys_assert((umac >= 1) && (umac <= 32));\n")
        new_code.write("  if (hw) {\n")
        new_code.write("    autogen_rd(dev_id, ofs, data_p);\n")
        new_code.write("  }\n")
        new_code.write("  *val = getp_tof2_" + map_nm + "_" + reg_nm + "_" + fld_nm + "(data_p);\n")
        new_code.write("  autogen_log(\"TRC : %d: p%02d :     : Rd : -------- : %08x : %s\", dev_id, umac, *val, __func__);\n")
        new_code.write("}\n\n")

        new_hdr.write( "void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint32_t *data_p, uint32_t val);\n")
        new_code.write("void " + map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw( " + fn_key + "uint32_t *data_p, uint32_t val) {\n")
        new_code.write("  uint32_t unused_fld = 0;\n\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "data_p, &unused_fld, true);\n")
        new_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "data_p, val, true);\n")
        new_code.write("}\n\n")

    #######################
    new_bf_code.write("/**********************************************************************\n")
    new_bf_code.write(" " + fld_description.replace('\r','').replace('\\n','').replace('  ',' ')  + "\n")
    new_bf_code.write("***********************************************************************/\n")

    if channelized:
        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_set"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "(" + fn_key_as_args + "ch, data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_get"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "(" + fn_key_as_args + "ch, data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t ch, uint32_t *data_p, uint32_t val) {\n")
        new_bf_code.write("  uint32_t unused_fld = 0;\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "ch, data_p, &unused_fld, true);\n")
        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "ch, data_p, val, true);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

    else:
        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_set"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t *data_p, uint32_t val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t *data_p, uint32_t val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "( " + fn_key_as_args + "data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_get"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t *data_p, uint32_t *val, bool hw);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t *data_p, uint32_t *val, bool hw) {\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + api_nm + "( " + fn_key_as_args + "data_p, val, hw);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")

        api_nm = map_nm + "_" + reg_nm + "_" + fld_nm + "_rmw"
        new_bf_hdr.write( "bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t *data_p, uint32_t val);\n")
        new_bf_code.write("bf_status_t bf_ll_" + api_nm + "( " + fn_key + "uint32_t *data_p, uint32_t val) {\n")
        new_bf_code.write("  uint32_t unused_fld = 0;\n")
        new_bf_code.write( "  uint32_t physical_umac;\n")
        if map_nm == "eth100g_reg_rspec":
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac3_to_physical(dev_id, umac, &physical_umac);\n\n")
        else:
            new_bf_code.write( "  bf_status_t rc = bf_map_logical_umac4_to_physical(dev_id, umac, &physical_umac);\n\n")
        new_bf_code.write( "  if (rc != BF_SUCCESS) return rc;\n\n")

        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_get( " + fn_key_as_args + "data_p, &unused_fld, true);\n")
        new_bf_code.write("  " + map_nm + "_" + reg_nm + "_" + fld_nm + "_set( " + fn_key_as_args + "data_p, val, true);\n")
        new_bf_code.write("  return BF_SUCCESS;\n")
        new_bf_code.write("}\n\n")



def new_fld(new_code, map_nm, reg_nm):
    child.expect("property title = \"")
    fld_title = child.str_extract("",";")

    child.expect("property description = \"")
    fld_description = child.str_extract("",";")

    child.expect("} ")
    child.expect("\[")
    child.expect("\] ")
    fld_nm = child.str_extract("",";")
    new_fld_with_these(new_code, map_nm, reg_nm, fld_title, fld_description, fld_nm)

def new_flds(new_code, map_nm, reg_nm):
    end_of_flds = False
    while not end_of_flds:
        fld_str = child.expect2("field {", "};")
        if fld_str == 0:
            new_fld(new_code, map_nm, reg_nm)
        else:
            end_of_flds = True




def new_register_flds(new_code, map_nm, reg_nm):
    child.expect("property title = \"")
    title = child.str_extract("",";")

    child.expect("property description = \"")
    description = child.str_extract("",";")
    #new_code.write("//\r\n// " + description.replace("\"","") + "\r\n//\r\n\r\n")

    new_flds(new_code, map_nm, reg_nm)


def new_ecc_control(new_code, map_nm):
    reg_str = child.str_extract(" ", " \(")
    reg_nm = reg_str.strip(" ")

    child.expect("names")
    nm_list = []
    eol = False
    while not eol:
      child.expect("\"")
      fld_nm = child.str_extract("","\"")
      nm_list.append(fld_nm)
      eol = child.expect2(",","],")

    child.expect("types")
    type_list = []
    eol = False
    while not eol:
      child.expect("\"")
      type_ = child.str_extract("","\"")
      type_list.append(type_)
      eol = child.expect2(",","],")

    for i in range(len(nm_list)):
      fld = nm_list[i]
      type_ = type_list[i]
      if type_ != "gen_only":
        new_fld_with_these(new_code, map_nm, reg_nm, "", "Disable Error Checking", fld + "_disable_check")
      if type_ != "chk_only":
        new_fld_with_these(new_code, map_nm, reg_nm, "", "Inject Single Bit Error", fld + "_inject_sbe")
        new_fld_with_these(new_code, map_nm, reg_nm, "", "Inject Multiple Bit Error", fld + "_inject_mbe")


def new_int_log_r(new_code, map_nm):
    reg_nm_str = child.str_extract(" ", " \(")
    reg_nm = reg_nm_str.strip(" ")

    child.expect("title")
    child.expect("\"")
    title = child.str_extract("","\"")

    child.expect("field_names")
    fld_list = []
    eol = False
    while not eol:
      child.expect("\"")
      fld_nm = child.str_extract("","\"")
      fld_list.append(fld_nm)
      eol = child.expect2(",","],")
      new_fld_with_these(new_code, map_nm, reg_nm, title, title, fld_nm)


def new_int_en(new_code, map_nm):
    reg_nm_str = child.str_extract(" ", " \(")
    reg_nm = reg_nm_str.strip(" ")

    child.expect("interrupts")
    fld_list = []
    eol = False
    while not eol:
      child.expect("\"")
      fld_nm = child.str_extract("","\"")
      fld_list.append(fld_nm)
      eol = child.expect2(",","],")
      new_fld_with_these(new_code, map_nm, reg_nm, "", "Interrupt Enable Register", fld_nm)


def new_int_grp(new_code, map_nm):
    grp_nm = child.str_extract(" ", " \(")
    reg_nm = grp_nm.strip(" ")

    child.expect("interrupts")
    fld_list = []
    eol = False
    while not eol:
      child.expect("\"")
      fld_nm = child.str_extract("","\"")
      fld_list.append(fld_nm)
      eol = child.expect2(",","],")

    child.expect("titles")
    title_list = []
    eol = False
    while not eol:
      child.expect("\"")
      title = child.str_extract("","\"")
      title_list.append(title)
      eol = child.expect2(",","],")

    child.expect("descriptions")
    desc_list = []
    eol = False
    while not eol:
      child.expect("\"")
      desc = child.str_extract("","\"")
      desc_list.append(desc)
      eol = child.expect2(",","],")

    for i in range(len(fld_list)):
      fld = fld_list[i]
      title = title_list[i]
      desc = desc_list[i]
      new_fld_with_these(new_code, map_nm, reg_nm + ".stat", title, desc, fld)
    for i in range(len(fld_list)):
      fld = fld_list[i]
      title = title_list[i]
      desc = desc_list[i]
      new_fld_with_these(new_code, map_nm, reg_nm + ".en0", title, desc, fld)
    for i in range(len(fld_list)):
      fld = fld_list[i]
      title = title_list[i]
      desc = desc_list[i]
      new_fld_with_these(new_code, map_nm, reg_nm + ".en1", title, desc, fld)
    for i in range(len(fld_list)):
      fld = fld_list[i]
      title = title_list[i]
      desc = desc_list[i]
      new_fld_with_these(new_code, map_nm, reg_nm + ".inj", title, desc, fld)

def new_register(new_code, map_nm):
    reg_nm = child.str_extract(""," ")
    child.expect("{")
    new_register_flds(new_code, map_nm, reg_nm)




def new_address_map(new_code):
    map_nm = rspec_nm
    end_of_regs = False
    while not end_of_regs:
        #got_str = child.expect2("register ", "} ")
        got_str = child.expect_multiple(["register ", "interrupt_register_g ", "ecc_control_r ", "interrupt_enable_r", "interrupt_log_r","} "])
        if got_str == 0:
            new_register(new_code, map_nm)
        elif got_str == 1:
            new_int_grp(new_code, map_nm)
        elif got_str == 2:
            new_ecc_control(new_code, map_nm)
        elif got_str == 3:
            new_int_en(new_code, map_nm)
        elif got_str == 4:
            new_int_log_r(new_code, map_nm)
        else:
            map_nm = child.str_extract("",";")
            end_of_regs = True

#
# syntax:
#    python ./csr-gen.py <csr_file_name> <rspec name> <offset_path> <function key string>
#
#    where <function key string> is of the form:
#       "bf_dev_id_t dev_id, uint32_t key1, uint32_t key2, ..."
#
#    where <offset_path> is of the form:
#        tof2_regs.<block>.<sub-block>. ...
#
#    e.g.
#    python ./csr-gen.py eth400g_pcs_rspec.csr eth400g_pcs_rspec "tof2_reg.eth400g_p1.eth400g_pcs"
#
if __name__ == "__main__":
    # global board

    if len(sys.argv) < 3:
        print_usage()
        exit()

    csr_file = str(sys.argv[1])
    rspec_nm = str(sys.argv[2])
    offset_path = str(sys.argv[3])
    rotated_offset_path = str(sys.argv[4])
    fn_key = str("bf_dev_id_t dev_id, uint32_t umac, ")
    fn_key_as_args = str("dev_id, umac, ")

    generated_code_file_nm = rspec_nm + "_access.c"
    generated_header_file_nm = rspec_nm + "_access.h"
    generated_bf_code_file_nm = "bf_ll_" + rspec_nm + "_if.c"
    generated_bf_header_file_nm = "bf_ll_" + rspec_nm + "_if.h"

    with open(generated_code_file_nm, "w") as new_code:
        with open(generated_header_file_nm, "w") as new_hdr:
            with open(generated_bf_code_file_nm, "w") as new_bf_code:
                with open(generated_bf_header_file_nm, "w") as new_bf_hdr:
                    new_hdr.write("/* clang-format off */\n\n")
                    new_code.write("/* clang-format off */\n\n")
                    new_code.write("#include \"autogen-required-headers.h\"\n\n")
                    new_code.write("#include <inttypes.h>\n" )

                    new_bf_hdr.write( "/* clang-format off */\n" )
                    new_bf_code.write( "/* clang-format off */\n" )
                    new_bf_code.write( "#include <stdbool.h>\n" )
                    new_bf_code.write( "#include <stdint.h>\n" )
                    new_bf_code.write( "#include <bf_types/bf_types.h>\n" )
                    new_bf_code.write( "#include \"" + generated_header_file_nm + "\"\n" )
                    if rspec_nm == "eth100g_reg_rspec":
                        new_bf_code.write( "extern bf_status_t bf_map_logical_umac3_to_physical(bf_dev_id_t dev_id,\n")
                        new_bf_code.write( "                                                    uint32_t logical_umac,\n")
                        new_bf_code.write( "                                                    uint32_t *physical_umac);\n")
                    else:
                        new_bf_code.write( "extern bf_status_t bf_map_logical_umac4_to_physical(bf_dev_id_t dev_id,\n")
                        new_bf_code.write( "                                                    uint32_t logical_umac,\n")
                        new_bf_code.write( "                                                    uint32_t *physical_umac);\n")

                    child = Pexp()
                    child.expect("addressmap {")
                    new_address_map(new_code)


