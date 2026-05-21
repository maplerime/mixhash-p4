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


import re
import time
import unittest

from ptf.testutils import *

from pal_rpc.ttypes import *


def get_swtype(client):
    board_type = client.pltfm_pm.pltfm_pm_board_type_get()
    swtype = ""
    if re.search("0x0234|0x1234|0x4234|0x5234", hex(board_type)):
        swtype = "mavericks"
    elif re.search("0x2234|0x3234", hex(board_type)):
        swtype = "montara"
    return swtype


def portAdd(client, dev, ports, speed, fec_type, an_mode="", statusCheck=1):
    for i in ports:
        client.pal.pal_port_add(dev, int(i), speed, fec_type)
        if an_mode != "":
            client.pal.pal_port_an_set(dev, int(i), an_mode)
        client.pal.pal_port_enable(dev, int(i))

    if statusCheck:
        checkStatus(client, dev, ports)


def portAddAll(client, dev, speed, fec_type, an_mode=""):
    client.pal.pal_port_add_all(dev, speed, fec_type)
    if an_mode != "":
        client.pal.pal_port_an_set_all(dev, an_mode)
    client.pal.pal_port_enable_all(dev)


def portDel(client, dev, ports):
    for i in ports:
        client.pal.pal_port_del(dev, int(i))


def portDelAll(client, dev):
    client.pal.pal_port_del_all(dev)


def portOperStatus(client, dev, port):
    status = 0
    try:
        status = client.pal.pal_port_oper_status_get(dev, int(port))
    except InvalidPalOperation:
        pass
    return status


def checkStatus(client, dev, ports):
    portsdown = ""

    loopCntr = 10
    for i in range(loopCntr):
        portsdown = ""
        time.sleep(15)
        for j in ports:
            status = portOperStatus(client, dev, j)
            print("Loop %s: Oper status for port %s is %s" % (i, j, status))
            if status == 0:
                portsdown += str(j) + " "
        if portsdown == "":
            break

    assert portsdown == "", "Ports did not come up"
    return portsdown
