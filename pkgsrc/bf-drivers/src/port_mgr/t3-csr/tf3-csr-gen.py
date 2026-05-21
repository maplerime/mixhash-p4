#! /usr/bin/env python
# -*- coding: utf-8 -*-
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


from __future__ import print_function

import argparse
import os
import re
import sys
import time
import copy
import string
import shutil
import glob

from string import Template
from pprint import pprint as pp

tof3_str = 'tof3_'

def line_check_for_fld_or_reg_name(line):
   if (('} [' in line) and ('property' not in line) and line_has_digits(line)):
      return True

def desc_ends(line):
   if ('";' in line):
      return True

def line_extract_fld_name(line):
    s = line.rstrip('\t\n')
    patt =  r'\[.*?\]'
    fld_name = re.sub(patt,"", s)
    #patt = r'[\s\n}";]'
    patt = r'[\s\\ \n}";]'
    fld_name = re.sub(patt,"", fld_name)
    return fld_name

def line_reg_is_chan(line):
   patt = '[8];'
   patt1 = '[4];'
   if (line.find(patt) != -1):
      return True
   elif (line.find(patt1) != -1):
      return True
   else:
      return False

def line_has_digits(string):
   res = filter(str.isdigit, string)
   return res is not None

def line_has_no_digits(string):
   res = filter(str.isdigit, string)
   return res is None

def line_matches_block_name(line, block_match):
   if (line.find(block_match) != -1):
      return True

# handle special Channelized cases
def is_tf3_fld_channelized(map_nm, fld_name):
   if map_nm == str('eth400g_mac_rspec'):
      if fld_name == 'chan':
         return True
      else:
         return False 

def get_clean_desc(desc):
    desc = desc.replace('\\n', '')
    desc = desc.replace('"', '')
    desc = desc.replace(';', '')
    return desc 


class RegMap():
   def __init__(self, map_name):
      # block_name 
      self.map_nm  = map_name
      self.grp_nm  = ''
      self.reg_list  = []

   def add_reg_entry(self, grp_reg, reg_nm, is_chan, reg_desc):
       new_reg_entry = {'reg_grp': grp_reg, 'reg_nm':reg_nm, 'reg_chan':is_chan, 'reg_desc': reg_desc}
       self.reg_list.append( new_reg_entry )
       if (not grp_reg):
           self.reg_list.reverse()

   def add_field_entry(self, fld_nm, fld_desc):
       new_fld_entry = {'fld_nm': fld_nm, 'fld_desc': fld_desc}
       self.reg_list.append( new_fld_entry )

   def add_grp_entry(self, grp_nm):
       self.grp_nm = grp_nm
       self.reg_list.reverse()

def gen_access_c_func(block_name, grp_nm, reg_nm, reg_chan, fld_name, fld_desc):
   getp_wrapper_func_template = string.Template("""
/**********************************************************************
 $__fld_desc__
**********************************************************************/

void $__setp_wrapper_func_name__$__set_args__
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].$__block_ofs_nm__$__reg_ofs_nm__);

  tf3_autogen_log("TRC : %d:%d  tmac->%02d :     : Wr : -------- : %08x : %s", dev_id, subdev_id, tmac, (uint32_t)val, __func__);

  setp_tof3_$__block_name__$__reg_nm__$__fld_nm__(data_p, val);

  if (hw) {
    autogen_tmac_wr(dev_id, subdev_id, ofs, *data_p);
  }
}

void $__getp_wrapper_func_name__$__get_args__
  uint32_t ofs = offsetof(tof3_reg, eth400g[tmac].$__block_ofs_nm__$__reg_ofs_nm__);

   if (hw) {
      autogen_tmac_rd(dev_id, subdev_id, ofs, data_p);
   }

   *val = getp_tof3_$__block_name__$__reg_nm__$__fld_nm__(data_p);
   tf3_autogen_log("TRC : %d:%d tmac->%02d :     : Rd : -------- : %08x : %s", dev_id, subdev_id, tmac, *val, __func__);
}

void $__rmw_wrapper_func_name__$__rmw_args__
  uint32_t unused_fld = 0;

  $__getp_wrapper_func_name__$__rmw_get_args__
  $__setp_wrapper_func_name__$__rmw_set_args__
}
""")
   #offsetof(tof3_reg, eth400g[tmac].<eth400g_mac>.<reg_nm><[]>);
   blk = str(block_name)
   blk = blk.replace('_rspec', '')
   if grp_nm == '':
      blk_grp_nm = block_name
      reg_ofs = reg_nm
   else:
      blk_grp_nm = block_name + '_'+ grp_nm 
      reg_ofs = grp_nm + '.' + reg_nm
   
   fn_prefix = tof3_str + blk_grp_nm + '_'+ reg_nm + '_'  + fld_name

   if reg_chan:
       set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {'
       get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {'
       rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {'
       rmw_get_args = '(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);'
       rmw_set_args = '(dev_id, subdev_id, tmac, ch, data_p, val, true);'
       reg_ofs = reg_ofs + '[ch]'
   else:
       set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {'
       get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {'
       rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {'
       rmw_get_args = '(dev_id, subdev_id, tmac, data_p, &unused_fld, true);'
       rmw_set_args = '(dev_id, subdev_id, tmac, data_p, val, true);'

   template_vals = {
      '__getp_wrapper_func_name__': fn_prefix + '_get', 
      '__setp_wrapper_func_name__': fn_prefix + '_set', 
      '__rmw_wrapper_func_name__': fn_prefix + '_rmw',
      
      '__set_args__': set_args,
      '__get_args__': get_args,
      '__rmw_args__': rmw_args,
      '__rmw_get_args__': rmw_get_args,
      '__rmw_set_args__': rmw_set_args,

      '__block_name__': block_name + '_',
      '__reg_nm__': reg_nm + '_',
      '__fld_nm__': fld_name,
      '__fld_desc__': fld_desc,
      '__block_ofs_nm__': blk + '.',
      '__reg_ofs_nm__': reg_ofs,
      }

   return getp_wrapper_func_template.safe_substitute(template_vals)



def gen_access_func_hdr(block_name, grp_nm, reg_nm, reg_chan, fld_name, fld_desc):
   getp_wrapper_hdr_template = string.Template("""
void $__getp_wrapper_func_name__$__get_args__
void $__setp_wrapper_func_name__$__set_args__
void $__rmw_wrapper_func_name__$__rmw_args__
    """)

   blk = str(block_name)
   blk = blk.replace('_rspec', '')
   if grp_nm == '':
      blk_grp_nm = block_name
   else:
      blk_grp_nm = block_name + '_'+ grp_nm 
   
   fn_prefix = tof3_str + blk_grp_nm + '_'+ reg_nm + '_'  + fld_name

   if reg_chan:
       set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);'
       get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);'
       rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);'
   else:
       set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);'
       get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);'
       rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);'

   template_vals = {
      '__getp_wrapper_func_name__': fn_prefix + '_get', 
      '__setp_wrapper_func_name__': fn_prefix + '_set', 
      '__rmw_wrapper_func_name__': fn_prefix + '_rmw',
      
      '__set_args__': set_args,
      '__get_args__': get_args,
      '__rmw_args__': rmw_args,

      '__block_name__': block_name + '_',
      '__reg_nm__': reg_nm + '_',
      '__fld_nm__': fld_name,
      '__fld_desc__': fld_desc,
      }

   return getp_wrapper_hdr_template.safe_substitute(template_vals)

def gen_bf_ll_c_func(block_name, grp_nm, reg_nm, reg_chan, fld_name, fld_desc):
   bf_ll_wrapper_func_template = string.Template("""
/**********************************************************************
 $__fld_desc__
**********************************************************************/

bf_status_t $__set_bf_ll_wrapper_func__$__bf_ll_set_args__
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  $__setp_wrapper_func_name__$__ll_setp_args__
  return BF_SUCCESS;
}

bf_status_t $__get_bf_ll_wrapper_func__$__bf_ll_get_args__
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  $__getp_wrapper_func_name__$__ll_getp_args__
  return BF_SUCCESS;
}

bf_status_t $__rmw_bf_ll_wrapper_func__$__bf_ll_rmw_args__
  uint32_t unused_fld = 0;
  uint32_t tmac;
  bf_subdev_id_t subdev_id = 0;
  bf_status_t rc = bf_map_logical_tmac_to_physical(dev_id, &subdev_id, mac, &tmac);
  if (rc != BF_SUCCESS) return rc;

  $__getp_wrapper_func_name__$__rmw_getp_args__
  $__setp_wrapper_func_name__$__rmw_setp_args__
  return BF_SUCCESS;
}
""")

   blk = str(block_name)
   blk = blk.replace('_rspec', '')
   if grp_nm == '':
      blk_grp_nm = block_name
      reg_ofs = reg_nm
   else:
      blk_grp_nm = block_name + '_'+ grp_nm 
      reg_ofs = grp_nm + '.' + reg_nm
   
   fn_prefix = tof3_str + blk_grp_nm + '_'+ reg_nm + '_'  + fld_name

   if reg_chan:
       ll_set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {'
       ll_setp_args = '(dev_id, subdev_id, tmac, ch, data_p, val, hw);'
       ll_get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {'
       ll_getp_args = '(dev_id, subdev_id, tmac, ch, data_p, val, hw);'
       ll_rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val) {'
       rmw_getp_args = '(dev_id, subdev_id, tmac, ch, data_p, &unused_fld, true);'
       rmw_setp_args = '(dev_id, subdev_id, tmac, ch, data_p, val, true);'
       bf_ll_set_args = '(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw) {'
       bf_ll_get_args = '(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw) {'
       bf_ll_rmw_args = '(bf_dev_id_t dev_id, uint32_t mac, uint32_t ch, uint32_t *data_p, uint32_t val) {'
   else:
       ll_set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw) {'
       ll_setp_args = '(dev_id, subdev_id, tmac, data_p, val, hw);'
       ll_get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw) {'
       ll_getp_args = '(dev_id, subdev_id, tmac, data_p, val, hw);'
       ll_rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val) {'
       rmw_getp_args = '(dev_id, subdev_id, tmac, data_p, &unused_fld, true);'
       rmw_setp_args = '(dev_id, subdev_id, tmac, data_p, val, true);'
       bf_ll_set_args = '(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val, bool hw) {'
       bf_ll_get_args = '(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t *val, bool hw) {'
       bf_ll_rmw_args = '(bf_dev_id_t dev_id, uint32_t mac, uint32_t *data_p, uint32_t val) {'

   template_vals = {

      '__getp_wrapper_func_name__': fn_prefix + '_get', 
      '__setp_wrapper_func_name__': fn_prefix + '_set', 
      '__rmw_wrapper_func_name__': fn_prefix + '_rmw',

      '__block_name__': block_name + '_',
      '__reg_nm__': reg_nm + '_',
      '__fld_nm__': fld_name,
      '__fld_desc__': fld_desc,

      '__set_bf_ll_wrapper_func__': 'bf_ll_' + fn_prefix + '_set',
      '__get_bf_ll_wrapper_func__': 'bf_ll_' + fn_prefix + '_get', 
      '__rmw_bf_ll_wrapper_func__': 'bf_ll_' + fn_prefix + '_rmw',
       
      '__ll_set_args__': ll_set_args,
      '__ll_setp_args__': ll_setp_args,
      '__ll_get_args__': ll_get_args,
      '__ll_getp_args__': ll_getp_args,
      '__ll_rmw_args__': ll_rmw_args,
      '__rmw_getp_args__': rmw_getp_args,
      '__rmw_setp_args__': rmw_setp_args,
      '__bf_ll_set_args__': bf_ll_set_args,
      '__bf_ll_get_args__': bf_ll_get_args,
      '__bf_ll_rmw_args__': bf_ll_rmw_args,
      }

   return bf_ll_wrapper_func_template.safe_substitute(template_vals)

def gen_bf_ll_hdr_func(block_name, grp_nm, reg_nm, reg_chan, fld_name, fld_desc):
   bf_ll_hdr_func_template = string.Template("""
bf_status_t $__set_bf_ll_wrapper_func__$__ll_set_args__
bf_status_t $__get_bf_ll_wrapper_func__$__ll_get_args__
bf_status_t $__rmw_bf_ll_wrapper_func__$__ll_rmw_args__
    """)

   blk = str(block_name)
   blk = blk.replace('_rspec', '')
   if grp_nm == '':
      blk_grp_nm = block_name
      reg_ofs = reg_nm
   else:
      blk_grp_nm = block_name + '_'+ grp_nm 
      reg_ofs = grp_nm + '.' + reg_nm
   
   fn_prefix = tof3_str + blk_grp_nm + '_'+ reg_nm + '_'  + fld_name

   if reg_chan:
       ll_set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);'
       ll_get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);'
       ll_rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);'
   else:
       ll_set_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);'
       ll_get_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);'
       ll_rmw_args = '(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);'

   template_vals = {
      '__getp_wrapper_func_name__': fn_prefix + '_get', 
      '__setp_wrapper_func_name__': fn_prefix + '_set', 
      '__rmw_wrapper_func_name__': fn_prefix + '_rmw',

      '__block_name__': block_name + '_',
      '__reg_nm__': reg_nm + '_',
      '__fld_nm__': fld_name,
      '__fld_desc__': fld_desc,

      '__set_bf_ll_wrapper_func__': 'bf_ll_' + fn_prefix + '_set',
      '__get_bf_ll_wrapper_func__': 'bf_ll_' + fn_prefix + '_get', 
      '__rmw_bf_ll_wrapper_func__': 'bf_ll_' + fn_prefix + '_rmw',
       
      '__ll_set_args__': ll_set_args,
      '__ll_get_args__': ll_get_args,
      '__ll_rmw_args__': ll_rmw_args,
      }

   return bf_ll_hdr_func_template.safe_substitute(template_vals)

def dump_parsed_data(data):
   print('\n==================================================', end='')
   print('\n Dumping Parsed Data', end='')
   print('\n==================================================', end='\n')

   reg_keys = ['reg_grp', 'reg_nm', 'reg_chan', 'reg_desc']
   fld_keys = ['fld_nm', 'fld_desc']

   reg_val = {'reg_grp':'', 'reg_nm':'', 'reg_chan':'', 'reg_desc':''}
   fld_val = {'fld_nm':'', 'fld_desc':''}

   for rmap in data:
       map_nm = rmap.map_nm
       grp_nm = rmap.grp_nm
       print('\n -----------')
       #if (grp_nm == ''):
       if (1):
           reg_list = rmap.reg_list
           print('Num of reg-list', len(reg_list))
           if (grp_nm != ''):
               print('grp_nm', grp_nm)

           for idx in list(reg_list):
               for key in idx:
                   if key in reg_keys:
                       reg_val[key] = idx[key]
                       if (key == str('reg_desc')):
                           print(reg_val)
                   if key in fld_keys:
                       #print(key, idx[key])
                       fld_val[key] = idx[key]
                       if (key == str('fld_desc')):
                           #print('reg_name:', reg_val['reg_nm'])
                           print(fld_val)

tf3_registers_exclude_list = [
    # Mac registers to exclude
    'scratch',
    'dft_csr',
    'cfg_pcs_base_tx_c49_test_pattern_seed_a_lsb',
    'cfg_pcs_base_tx_c49_test_pattern_seed_a_msb', 
    'cfg_pcs_base_tx_c49_test_pattern_seed_b_lsb', 
    'cfg_pcs_base_tx_c49_test_pattern_seed_b_msb',
    'pcs_rx_map_index0',
    'pcs_rx_map_index1',
    'pcs_rx_map_index2',
    'pcs_rx_map_index3',
    'pcs_rx_map_index4',
    'freeze_enable',
    'inj',
    'en1',
    'en0',
    'stat',

    # No app or sys registers to exclude
]

# Function Generators
def generate_code(data, access_c_fd, access_hdr_fd, bf_c_fd, bf_hdr_fd):
   reg_keys = ['reg_grp', 'reg_nm', 'reg_chan', 'reg_desc']
   fld_keys = ['fld_nm', 'fld_desc']

   reg_val = {'reg_grp':'', 'reg_nm':'', 'reg_chan':'', 'reg_desc':''}
   fld_val = {'fld_nm':'', 'fld_desc':''}

   for rmap in data:
       map_nm = rmap.map_nm
       grp_nm = rmap.grp_nm
       blk_nm = map_nm
       #print('\n xxxxxxxxxxx')
       if (1):
           reg_list = rmap.reg_list
           #print('Num of reg-list', len(reg_list))
           #if (grp_nm != ''):
               #print('grp_nm', grp_nm)

           for idx in list(reg_list):
               for key in idx:
                   if key in reg_keys:
                       reg_val[key] = idx[key]
                       if (key == str('reg_desc')):
                           pass
                           #print(reg_val)
                   if key in fld_keys:
                       #print(key, idx[key])
                       fld_val[key] = idx[key]
                       if (key == str('fld_desc')):
                           reg_nm = reg_val['reg_nm']
                           if str(reg_nm) not in tf3_registers_exclude_list:
                               reg_chan = reg_val['reg_chan']
                               fld_name = fld_val['fld_nm']
                               desc = fld_val['fld_desc']
                               desc = get_clean_desc(desc)
                               access_c_fd.write(gen_access_c_func(blk_nm, grp_nm, reg_nm, reg_chan, fld_name, desc))
                               access_hdr_fd.write(gen_access_func_hdr(blk_nm, grp_nm, reg_nm, reg_chan, fld_name, desc))
                               bf_c_fd.write(gen_bf_ll_c_func(blk_nm, grp_nm, reg_nm, reg_chan, fld_name, desc))
                               bf_hdr_fd.write(gen_bf_ll_hdr_func(blk_nm, grp_nm, reg_nm, reg_chan, fld_name, desc))

csr_dict = {
   'addressmap': re.compile(r'addressmap {'),
   'group': re.compile(r'group {'),
   'register': re.compile(r'register {'),
   'field': re.compile(r'field {'),
   'reg_or_field_end': re.compile(r"[}\[:\]\d+]"),
   'description': re.compile(r'property description ='),
   'channelized': re.compile(r'Per-lane'),
   }

def csr_parse_line(line):
   for key, rgx in csr_dict.items():
      match = rgx.search(line)
      if match:
         return key, match
   # no matches
   return None, None

def parse_csr_file(crs_file_path, block_name):

   data = []  # create an empty list to collect the data

   grpRecordInProgress = False
   regRecordInProgress = False
   fieldRecordInProgress = False
   reg_obj_exist = False
   map_nm = block_name 
   fld_channelized = False
   descInProgress = False
   fld_desc = ''
   match_found = False
   block_match = '} ' + block_name + ';'
   with open(crs_file_path, 'r') as csr_fobj:
      line = csr_fobj.readline()
      while line:
      
         if line_matches_block_name(line, block_match):
            match_found = True
            break
      
         key, match = csr_parse_line(line)
         
         if key == 'addressmap':
            data = []
      
         if key == 'group':
            reg_obj = RegMap(map_nm)

            grpRecordInProgress = True
            regRecordInProgress = False 
            fieldRecordInProgress = False
            descInProgress = False
            grp_desc = ''
      
         if key == 'register':
            if not grpRecordInProgress:
                reg_obj = RegMap(map_nm)
            fld_channelized = False
            fieldRecordInProgress = False
            regRecordInProgress = True
            descInProgress = False 
            reg_desc = ''
      
         if key == 'field':
            fieldRecordInProgress = True
            descInProgress = False 
            fld_desc = ''
      
         if descInProgress:
            if (not fieldRecordInProgress) and regRecordInProgress:
                 reg_desc += line
            else:
                 fld_desc += line

            if desc_ends(line):
                descInProgress = False 
      
         #if key == 'description' and (fieldRecordInProgress  or grpRecordInProgress):
         if key == 'description':
            descInProgress = True
      
         if map_nm == 'eth400g_mac_rspec' and key == 'channelized' and fieldRecordInProgress:
            fld_channelized = True
      
         #reg or field end
         if key == 'reg_or_field_end':
            if line_check_for_fld_or_reg_name(line):
               fld_name = line_extract_fld_name(line)
               if fieldRecordInProgress:
                 # Special cases in csr
                  fld_channelized = is_tf3_fld_channelized(map_nm, fld_name)    
                  reg_obj.add_field_entry(fld_name, fld_desc)
                  fieldRecordInProgress = False
               
               elif fieldRecordInProgress == False and regRecordInProgress:
                  grp_reg = False
                  if grpRecordInProgress:
                     grp_reg = True

                  is_chan = line_reg_is_chan(line)
                  reg_obj.add_reg_entry(grp_reg, fld_name, is_chan, reg_desc)
                  regRecordInProgress = False
                  if not grpRecordInProgress:
                    data.append(reg_obj)
               
               elif regRecordInProgress == False and grpRecordInProgress:
                  reg_obj.add_grp_entry(fld_name)
                  data.append(reg_obj)
                  grpRecordInProgress = False
                         
         line = csr_fobj.readline()
   return data, match_found

def movfiles_to_port_mgr(dst_dir):
    PRT_MGR_DST = dst_dir + '/'
    pwd = os.getcwd()
    CURR_PWD = pwd + "/*"
    for files in glob.glob(CURR_PWD):
        bf = os.path.basename(files)
        f = PRT_MGR_DST + bf 
        if files.endswith(".c") or files.endswith(".h"):
            print('\nMoving file: ', bf, end='')
            if os.path.isfile(f):
               os.remove(f)
            shutil.move(files, PRT_MGR_DST)



# Parse and Generate the code
def main():
   ap  = argparse.ArgumentParser()

   ap.add_argument("-c", "--csr_file",
                      required = True,
                      help= "Specify the CSR file to parse")
   ap.add_argument("-b", "--block_name",
                      required = True,
                      help= "Specify the block or rspec name")
   args = ap.parse_args()
   csr_file = args.csr_file
   block_name = args.block_name
   #eth400g_mac_rspec, eth400g_sys_rspec, eth400g_app_rspec
   rspec_nm = block_name
   #offset_path = 

   # Get the data
   print('\n======================================================', end='')
   print('\nParsing block "{}" : file "{}"'.format(str(block_name), (str(csr_file))), end='')
   print('\n======================================================', end='\n')
   data, match_found = parse_csr_file(csr_file, block_name)
   print('\n** Parsing complete **', end='\n\n')
   if (match_found == False):
      print('Error: No Matching block "{}" found'.format(str(block_name)), end='\n\n')
      return

   print('Matching block "{}" found'.format(str(block_name)), end='\n\n')

   # Dump 
   #dump_parsed_data(data)
   #return

   # Generate the code
   print('==================================================', end='\n')
   print("Generating the code...")
   print('==================================================')
   gen_access_c_file_nm = tof3_str + str(rspec_nm) + str('_access.c')
   gen_access_hdr_file_nm = tof3_str + str(rspec_nm) + str('_access.h')
   gen_bf_c_file_nm = str('bf_ll_') + tof3_str + str(rspec_nm) + str('_if.c')
   gen_bf_hdr_file_nm = str('bf_ll_') + tof3_str + str(rspec_nm) + str('_if.h')
   print(gen_access_c_file_nm)
   print(gen_access_hdr_file_nm)
   print(gen_bf_c_file_nm)
   print(gen_bf_hdr_file_nm)

   access_c_fd = open(gen_access_c_file_nm, "w")
   access_hdr_fd = open(gen_access_hdr_file_nm, "w")

   bf_c_fd = open(gen_bf_c_file_nm, "w")
   bf_hdr_fd = open(gen_bf_hdr_file_nm, "w")

   access_hdr_fd.write("/* clang-format off */\n\n")
   access_c_fd.write("/* clang-format off */\n\n")
   access_c_fd.write("#include \"tof3-autogen-required-headers.h\"\n\n")
   access_c_fd.write("#include <inttypes.h>\n" )
   access_c_fd.write("#include <stddef.h>\n" )
   access_c_fd.write( "#include <bf_types/bf_types.h>\n" )
   access_c_fd.write( "#include <tof3_regs/tof3_reg_drv.h>\n" )
   access_c_fd.write("#include \"port_mgr_tof3.h\"\n\n")

   bf_hdr_fd.write( "/* clang-format off */\n" )
   bf_c_fd.write( "/* clang-format off */\n" )
   bf_c_fd.write( "#include <stdbool.h>\n" )
   bf_c_fd.write( "#include <stdint.h>\n" )
   bf_c_fd.write( "#include <stddef.h>\n" )
   bf_c_fd.write( "#include <bf_types/bf_types.h>\n" )
   bf_c_fd.write( "#include <tof3_regs/tof3_reg_drv.h>\n" )
   bf_c_fd.write("#include \"port_mgr_tof3.h\"\n\n")
   bf_c_fd.write( "#include \"" + gen_access_hdr_file_nm + "\"\n" )
   bf_c_fd.write( "extern bf_status_t bf_map_logical_tmac_to_physical(bf_dev_id_t dev_id,\n")
   bf_c_fd.write( "                                                   bf_subdev_id_t *subdev_id,\n")
   bf_c_fd.write( "                                                   uint32_t logical_tmac,\n")
   bf_c_fd.write( "                                                   uint32_t *physical_tmac);\n")

   generate_code(data, access_c_fd, access_hdr_fd, bf_c_fd, bf_hdr_fd)

   access_c_fd.close()
   access_hdr_fd.close()
   bf_c_fd.close()  
   bf_hdr_fd.close()

   print("\n** Code generation successful **")

   dst_dir = '../port_mgr_tof3'
   print('\n==================================================', end='\n')
   print('Moving files to "{}" '.format(str(dst_dir)), end='\n')
   print('==================================================', end='\n')
   dst_dir = '../port_mgr_tof3/'
   movfiles_to_port_mgr(dst_dir)
   print("\n** Move successful **")

   return
 
# syntax:
#    python ./csr-gen.py <csr_file_name> <rspec name> <offset_path> <function key string>
#
#    where <function key string> is of the form:
#       "bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t key1, uint32_t key2, ..."
#    
#    where <offset_path> is of the form:
#        tof2_regs.<block>.<sub-block>. ...
#
#    e.g.
#    python ./csr-gen.py eth400g_pcs_rspec.csr eth400g_pcs_rspec "tof2_reg.eth400g_p1.eth400g_pcs"
#
if __name__ == "__main__":
   # global board
   main()

