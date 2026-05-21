########################################################################
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


"""
Thrift PD interface basic tests
"""
from collections import OrderedDict

import time
import datetime
import sys
import logging
import math
import re

import unittest

import pd_base_tests

from ptf import config
from ptf.testutils import *
from ptf.thriftutils import *

import os

from perf_test_alpm.p4_pd_rpc.ttypes import *
from res_pd_rpc.ttypes import *

import random

this_dir = os.path.dirname(os.path.abspath(__file__))


seed = random.randint(0, 65536)
random.seed(seed)
print('Seed used %d' % seed)
sys.stdout.flush()

def readData():
    global alpm_file_size

    data_file = os.path.join(this_dir, '../alpm_test/data.txt')
    f = open(data_file, 'r')
    lines = f.readlines()
    f.close()
    alpm_file_size = len(lines)

    test_matrix = []
    for line in lines:
        line = re.split('/|\t', line.strip())
        test_matrix.append((line[0], int(line[1]), (int(line[2]) % 8) + 1))

    return test_matrix

def run_tests(test, num_entries):
    print()
    sess_hdl = test.conn_mgr.client_init()

    dev_tgt = DevTarget_t(0, hex_to_i16(0xFFFF))

    print(datetime.datetime.now(), "  Creating test data")
    test_matrix = readData()

    vrfs = [(random.randint(1, 2000)) for x in range(num_entries)]
    nhop_indices = [(random.randint(1, 5000)) for x in range(num_entries)]

    test.client.perf_alpm_immediate_action_table_bulk_init(sess_hdl, dev_tgt, num_entries)

    print(datetime.datetime.now(), "  Setting up entries")

    entries = random.sample(range(alpm_file_size), num_entries)
    for i in range(num_entries):
        (ip_dst, match_len, port) = test_matrix[entries[i]]
        match_spec = perf_test_alpm_perf_alpm_immediate_action_match_spec_t(
                vrfs[i],
                ipv4Addr_to_i32(ip_dst),
                match_len)

        action_spec = perf_test_alpm_fib_hit_nexthop_action_spec_t(nhop_indices[i])

        test.client.perf_alpm_immediate_action_table_add_with_fib_hit_nexthop_bulk_setup(sess_hdl,
                                                                                         dev_tgt,
                                                                                         match_spec,
                                                                                         action_spec)

    print(datetime.datetime.now(), "  Starting test")
    rate = test.client.perf_alpm_immediate_action_table_perf_test_execute()
    print(datetime.datetime.now(), "  Completed test")
    test.conn_mgr.complete_operations(sess_hdl)
    test.conn_mgr.client_cleanup(sess_hdl)
    print("Rate (Operations/second) : %d" % (rate))
    return rate

class TestAlpmPerfImmediateActionHalf(pd_base_tests.ThriftInterfaceDataPlane):
    def __init__(self):
        pd_base_tests.ThriftInterfaceDataPlane.__init__(self, ["perf_test_alpm"])

    """ Alpm perf test with immediate action """
    def runTest(self):
        if test_param_get('target') == "hw":
            min_rate = 5300
        else:
            min_rate = 7400

        results = []
        for _ in range(10):
            results.append( run_tests(self, 65000) )
        results.sort()
        print("Half:", results)
        sys.stdout.flush()
        for rate in results[2:]:
            self.assertGreater(rate, min_rate)


class TestAlpmPerfImmediateActionFull(pd_base_tests.ThriftInterfaceDataPlane):
    def __init__(self):
        pd_base_tests.ThriftInterfaceDataPlane.__init__(self, ["perf_test_alpm"])

    """ Alpm perf test with immediate action """
    def runTest(self):
        if test_param_get('target') == "hw":
            min_rate = 2700
        else:
            min_rate = 3200

        results = []
        for _ in range(10):
            results.append( run_tests(self, 115000) )
        results.sort()
        print("Full:", results)
        sys.stdout.flush()
        for rate in results[2:]:
            self.assertGreater(rate, min_rate)
