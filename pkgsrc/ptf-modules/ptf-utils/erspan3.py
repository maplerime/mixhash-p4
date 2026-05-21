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

import logging
import ptf.dataplane as dataplane

def match_erspan3_pkt(exp_pkt, pkt, ignore_tstamp=True):
    """
    Compare ERSPAN_III packets, ignore the timestamp value. Just make sure
    it is non-zero
    """
    if ignore_tstamp:
        erspan3 = pkt.getlayer(ERSPAN_III)
        if erspan3 == None:
            #self.logger.error("No ERSPAN pkt received")
            return False

        if erspan3.timestamp == 0:
            #self.logger.error("Invalid ERSPAN timestamp")
            return False

        #fix the exp_pkt timestamp and compare
        exp_erspan3 = exp_pkt.getlayer(ERSPAN_III)
        if exp_erspan3 == None:
            #self.logger.error("Test user error - exp_pkt is not ERSPAN_III packet")
            return False

        exp_erspan3.timestamp = 0
        erspan3.timestamp = 0
        pkt[IP].len = 0
        exp_pkt[IP].len = 0
        pkt[IP].chksum = 0
        exp_pkt[IP].chksum = 0

    return dataplane.match_exp_pkt(exp_pkt, pkt)

def verify_erspan3_packet(test, pkt, port):
    """
    Check that an expected packet is received
    """
    logging.debug("Checking for pkt on port %r", port)
    (_, rcv_port, rcv_pkt, pkt_time) = test.dataplane.poll(port_number=port, timeout=2, exp_pkt=None)
    test.assertTrue(rcv_pkt != None, "Did not receive pkt on %r" % port)
    # convert rcv_pkt string back to layered pkt
    nrcv = pkt.__class__(rcv_pkt)
    #pkt.show2()
    #nrcv.show2()
    #print hexdump(pkt)
    #print hexdump(nrcv)
    test.assertTrue(match_erspan3_pkt(pkt, nrcv), "Received packet did not match expected packet")

def verify_erspan3_any_packet_any_port(test, pkts=[], ports=[]):
    """
    Check that _any_ of the packets is received on _any_ of the specified ports belonging to
    the given device (default device_number is 0).
    """
    received = False
    match_index = 0

    logging.debug("Checking for pkt on port %r", ports)
    for port in ports:
        (_, rcv_port, rcv_pkt, pkt_time) = test.dataplane.poll(port_number=port, timeout=2)
        for pkt in pkts:
            nrcv = pkt.__class__(rcv_pkt)
            if match_erspan3_pkt(pkt, nrcv):
                match_index = ports.index(rcv_port)
                received = True
                break

    test.assertTrue(received == True, "Did not receive expected pkt(s) on any of ports %r" % ports)

    return match_index


