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


import logging

import ptf.testutils as testutils
from bfruntime_client_base_tests import BfRuntimeTest
import bfrt_grpc.bfruntime_pb2 as bfruntime_pb2
import bfrt_grpc.client as client
import base64
import itertools

logger = logging.getLogger('Test')
if not len(logger.handlers):
    logger.addHandler(logging.StreamHandler())


class MeterBytecountAdjustTest(BfRuntimeTest):
    def setUp(self):
        client_id = 0
        p4_name = "tna_meter_bytecount_adjust"
        BfRuntimeTest.setUp(self, client_id, p4_name)

    def runTest(self):
        # Get bfrt_info and set it as part of the test
        bfrt_info = self.interface.bfrt_info_get()
        meter_table = bfrt_info.table_get("SwitchIngress.meter")
        direct_meter_color_table = bfrt_info.table_get("SwitchIngress.direct_meter_color")

        target = client.Target(device_id=0, pipe_id=0xffff)

        logger.info("set byte count adjust for meter table")
        meter_table.attribute_meter_bytecount_adjust_set(target, -12)
        resp = meter_table.attribute_get(target, "MeterByteCountAdjust")
        for d in resp:
            assert d["byte_count_adjust"] == -12

        logger.info("set byte count adjust for MatchAction_Direct meter table")
        direct_meter_color_table.attribute_meter_bytecount_adjust_set(target, 20)
        resp = direct_meter_color_table.attribute_get(target, "MeterByteCountAdjust")
        for d in resp:
            assert d["byte_count_adjust"] == 20

class IndirectMeterBytecountAdjustPerPipeTest(BfRuntimeTest):
    def setUp(self):
        client_id = 0
        p4_name = "tna_meter_bytecount_adjust"
        BfRuntimeTest.setUp(self, client_id, p4_name)

    def runTest(self):
        # Get bfrt_info and set it as part of the test
        bfrt_info = self.interface.bfrt_info_get()
        meter_table = bfrt_info.table_get("SwitchIngress.meter")
        meter_color_table = bfrt_info.table_get("SwitchIngress.meter_color")

        target = client.Target(device_id=0, pipe_id=0xffff)
        try:
            # Set the table as Asymmetric
            logger.info("set meter_color table to Asymmetric")
            mode = bfruntime_pb2.Mode.SINGLE
            meter_color_table.attribute_entry_scope_set(target,
                                            predefined_pipe_scope=True,
                                            predefined_pipe_scope_val=mode)
            pipe = [0,1]
            byte_count = [-12,-16]
            for pipe,bc in zip(pipe,byte_count):
                target = client.Target(device_id=0, pipe_id=pipe)
                logger.info("set byte count adjust to %d meter table pipe:%d",bc,pipe)
                meter_table.attribute_meter_bytecount_adjust_set(target, bc)
            logger.info("Get Bytecount value of pipe0")
            target = client.Target(device_id=0, pipe_id=0)
            resp = meter_table.attribute_get(target, "MeterByteCountAdjust")
            for d in resp:
                logger.info("Expected byte count : -12 , Returned byte count adjust %d", d["byte_count_adjust"])
                assert d["byte_count_adjust"] == -12
        finally:
            target = client.Target(device_id=0, pipe_id=0xffff)
            meter_color_table.attribute_entry_scope_set(target, predefined_pipe_scope=True,
                                                    predefined_pipe_scope_val=bfruntime_pb2.Mode.ALL)
        
class DirectMeterBytecountAdjustPerPipeTest(BfRuntimeTest):
    def setUp(self):
        client_id = 0
        p4_name = "tna_meter_bytecount_adjust"
        BfRuntimeTest.setUp(self, client_id, p4_name)

    def runTest(self):
        try:
            # Get bfrt_info and set it as part of the test
            bfrt_info = self.interface.bfrt_info_get()
            direct_meter_color_table = bfrt_info.table_get("SwitchIngress.direct_meter_color")

            target = client.Target(device_id=0, pipe_id=0xffff)
            # Set the table as Asymmetric
            logger.info("set meter_color table to Asymmetric")
            mode = bfruntime_pb2.Mode.SINGLE
            direct_meter_color_table.attribute_entry_scope_set(target,
                                            predefined_pipe_scope=True,
                                            predefined_pipe_scope_val=mode)
            pipe = [0,1]
            byte_count = [-12,-16]
            for pipe,bc in zip(pipe,byte_count):
                target = client.Target(device_id=0, pipe_id=pipe)
                logger.info("set byte count adjust to %d meter table pipe:%d",bc,pipe)
                direct_meter_color_table.attribute_meter_bytecount_adjust_set(target, bc)
            logger.info("Get Bytecount value of pipe0")
            target = client.Target(device_id=0, pipe_id=0)
            resp = direct_meter_color_table.attribute_get(target, "MeterByteCountAdjust")
            for d in resp:
                logger.info("Expected byte count : -12 , Returned byte count adjust %d", d["byte_count_adjust"])
                assert d["byte_count_adjust"] == -12
        finally:
            target = client.Target(device_id=0, pipe_id=0xffff)
            direct_meter_color_table.attribute_entry_scope_set(target, predefined_pipe_scope=True,
                                                    predefined_pipe_scope_val=bfruntime_pb2.Mode.ALL)
