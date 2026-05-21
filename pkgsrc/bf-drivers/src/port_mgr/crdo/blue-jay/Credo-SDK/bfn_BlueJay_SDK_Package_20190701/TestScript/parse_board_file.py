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

"""
QSFPDD_Port,QSFPDD_Lane,Octal,Tile,TX_Lane,TX_PN_Swap,RX_Lane,RX_PN_Swap,Schematics Net Name

QSFPDD_Port,QSFPDD_Lane,Tile,Tile_Octal,Tile_TX_Lane,PCB_TX_Lane,TX_PN_Swap,Tile_RX_Lane,PCB_RX_Lane,RX_PN_Swap,JBAY Package Net Name,Schematics Net Name
"""

fp_map_ = dict()
tx_map_ = dict()
rx_map_ = dict()

def parse_csv (filename):

    with open(filename, "rb") as csv_file:
        csv_reader = csv.DictReader(csv_file)
        row_num = 0
        for row in csv_reader:
            QSFPDD_Port = row["QSFPDD_Port"]
            QSFPDD_Lane = row["QSFPDD_Lane"].replace(" ","")
            #Octal       = row["Octal"]
            Octal       = row["Tile_Octal"]
            Tile        = row["Tile"]
            #TX_Lane     = row["TX_Lane"]
            TX_Lane     = row["Tile_TX_Lane"]
            TX_PN_Swap  = row["TX_PN_Swap"]
            #RX_Lane     = row["RX_Lane"]
            RX_Lane     = row["Tile_RX_Lane"]
            RX_PN_Swap  = row["RX_PN_Swap"]
            net_name    = row["Schematics Net Name"]

            # fix port 33 Octal value
            if QSFPDD_Port == "33":
              Octal = "8"

            if QSFPDD_Port == "": QSFPDD_Port = cur_QSFPDD_Port
            else:                 cur_QSFPDD_Port = QSFPDD_Port
            if Octal == "": Octal = cur_Octal
            else:           cur_Octal = Octal
            if Tile == "": Tile = cur_Tile
            else:          cur_Tile = Tile
            if net_name == "": net_name = cur_net_name
            else:              cur_net_name = net_name
            if False:
              print(QSFPDD_Port + ","),
              print(QSFPDD_Lane + ","),
              print(Octal + ","),
              print(Tile + ","),
              print(TX_Lane + ","),
              print(TX_PN_Swap + ","),
              print(RX_Lane + ","),
              print(RX_PN_Swap + ","),
              print(net_name + ","),
              print("")
            # skip improperly defined cpu port
            #if (int(QSFPDD_Port) == 33): continue
            #group = (int(Octal) % 8)
            group = int(Octal)
            # add front-port to serdes/tile map entry
            key = (int(QSFPDD_Port), int(QSFPDD_Lane))
            value = { 'group':int(group), 'tile':int(Tile), 'phy_tx':int(TX_Lane), 'tx_pn':TX_PN_Swap, 'phy_rx':int(RX_Lane), 'rx_pn':RX_PN_Swap, 'name':net_name }
            #print("key: " + str(key) + " value: " + str(value))
            fp_map_[key] = value
            key = (int(Tile), int(group), int(QSFPDD_Lane))
            value = { 'phy_tx':int(TX_Lane), 'tx_pn':TX_PN_Swap }
            tx_map_[key] = value
            key = (int(Tile), int(group), int(QSFPDD_Lane))
            value = { 'phy_rx':int(RX_Lane), 'rx_pn':RX_PN_Swap }
            rx_map_[key] = value

            #print("CMAP: {" + QSFPDD_Port + ", " + QSFPDD_Lane + ", " + Tile + ", " + Octal + ", " + TX_Lane + ", " + TX_PN_Swap + ", " + RX_Lane + ", " + RX_PN_Swap + "},")

        csv_file.close()   
    return

def lane_PN_swap_get(tile, g, ln):
    tx_pn_yn = tx_map_[ (tile, g, ln) ]['tx_pn']
    rx_pn_yn = rx_map_[ (tile, g, ln) ]['rx_pn']
    if tx_pn_yn == 'Y':
      tx_pn = 1
    else:
      tx_pn = 0
    if rx_pn_yn == 'Y':
      rx_pn = 1
    else:
      rx_pn = 0
    return tx_pn, rx_pn

def front_port_to_tile_grp(fp, lane):
    key = (fp, lane)
    return fp_map_[ key ]['tile'], fp_map_[ key ]['group']

def lane_map_get(tile, g, print_en=0):
    # 0x80000 is Tx lns 0-3
    tx0 = tx_map_[ (tile, g, 0) ]['phy_tx']
    tx1 = tx_map_[ (tile, g, 1) ]['phy_tx']
    tx2 = tx_map_[ (tile, g, 2) ]['phy_tx']
    tx3 = tx_map_[ (tile, g, 3) ]['phy_tx']
    r80000 = (tx0 << 12) | (tx1 << 8) | (tx2 << 4) | tx3
    if print_en: print("0x80000 = " + "%04x"%r80000),
    if print_en: print(" : "),
    if g < 8:
      # 0x80001 is Tx lns 4-7
      tx4 = tx_map_[ (tile, g, 4) ]['phy_tx']
      tx5 = tx_map_[ (tile, g, 5) ]['phy_tx']
      tx6 = tx_map_[ (tile, g, 6) ]['phy_tx']
      tx7 = tx_map_[ (tile, g, 7) ]['phy_tx']
      r80001 = (tx4 << 12) | (tx5 << 8) | (tx6 << 4) | tx7
      if print_en: print("0x80001 = " + "%04x"%r80001),
      if print_en: print(" : "),
    else:
      r80001 = 0x7654

    # 0x800002 is Rx lns 0-3
    rx0 = rx_map_[ (tile, g, 0) ]['phy_rx']
    rx1 = rx_map_[ (tile, g, 1) ]['phy_rx']
    rx2 = rx_map_[ (tile, g, 2) ]['phy_rx']
    rx3 = rx_map_[ (tile, g, 3) ]['phy_rx']
    r80002 = (rx0 << 12) | (rx1 << 8) | (rx2 << 4) | rx3
    if print_en: print("0x80002 = " + "%04x"%r80002),
    if print_en: print(" : "),
    if g < 8:
      # 0x80003 is Rx lns 4-7
      rx4 = rx_map_[ (tile, g, 4) ]['phy_rx']
      rx5 = rx_map_[ (tile, g, 5) ]['phy_rx']
      rx6 = rx_map_[ (tile, g, 6) ]['phy_rx']
      rx7 = rx_map_[ (tile, g, 7) ]['phy_rx']
      r80003 = (rx4 << 12) | (rx5 << 8) | (rx6 << 4) | rx7
      if print_en: print("0x80003 = " + "%04x"%r80003)
    else:
      r80003 = 0x7654

    return r80000, r80001, r80002, r80003

#if __name__ == "__main__":
def parse_board_map(print_en=0):
  parse_csv("Newport_P0A_Port_Mapping_1_31_2019.csv")
  for fp in range(1,34):
      if fp == 33:
          group_range=range(4)
      else:
          group_range=range(8)
      for channel in group_range:
          if print_en: print(str(fp) + "/" + str(channel) + ": "),
          if print_en: print( fp_map_[ (fp, channel) ] )
      if print_en: print("")


