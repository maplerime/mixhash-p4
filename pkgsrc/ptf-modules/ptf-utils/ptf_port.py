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
#################################################################################\

import sys
import os
import json
import six
import subprocess
from ptf import config

cur_dir = os.path.dirname(os.path.realpath(__file__))

def take_port_down(port_num):
    device = 0
    pm = config['port_map']
    veth = pm[(device, port_num)]
    veth_idx = veth.strip('veth')
    veth_num = int(veth_idx)
    veth_pair = "veth%d" % (veth_num - 1)
    six.print_(port_num, veth, veth_pair)
    subprocess.call(['port_ifdown', str(veth_pair)])

def bring_port_up(port_num):
    device = 0
    pm = config['port_map']
    veth = pm[(device, port_num)]
    veth_idx = veth.strip('veth')
    veth_num = int(veth_idx)
    veth_pair = "veth%d" % (veth_num - 1)
    subprocess.call(['port_ifup', str(veth_pair)])
