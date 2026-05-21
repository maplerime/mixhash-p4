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

import os
from ptf.testutils import *
from ptf.packet import *
from ptf.dataplane import DataPlane
import ptf.mask

try:
    from pal_rpc.ttypes import *
except ImportError:
    pass

import logging

"""
Note: This change is temporary. Scapy and bf_pktpy will only be available 
      during the transition period. After this period, the default 
      tool will be bf-pktpy.
"""
pktpy_tool = (os.environ.get("PKTPY", "true")).lower()

if pktpy_tool == "false":
    print('Using Scapy..')
    from .utils_scapy import *
else:
    print('Using PKTPY..')
    from .utils_pktpy import *


# Util functions independent of bf_pktpy/Scapy
def generate_mac_addresses(no_of_addr):
    """
        Generate list of different mac addresses

        Args:
            no_of_addr (int): number of requested MAC addresses (max 256^4)

        Return:
            list: mac_list with generated MAC addresses
    """
    mac_list = []
    i = 0
    for first_grp in range(0, 256):
        for second_grp in range(0, 256):
            for third_grp in range(0, 256):
                for fourth_grp in range(0, 256):
                    mac_list.append('00:00:' +
                                    ('%02x' % first_grp) + ':' +
                                    ('%02x' % second_grp) + ':' +
                                    ('%02x' % third_grp) + ':' +
                                    ('%02x' % fourth_grp))
                    i += 1
                    if i == no_of_addr:
                        return mac_list
    return mac_list
