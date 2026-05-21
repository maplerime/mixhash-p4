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

from ptf.testutils import *
import ptf.dataplane as dataplane

try:
    import scapy.config
    import scapy.route
    import scapy.layers.l2
    import scapy.layers.inet
    import scapy.main
    from scapy.all import Packet
except ImportError:
    sys.exit("Need to install scapy for packet parsing")

try:
    scapy.main.load_contrib("bfd")
    BFD = scapy.contrib.bfd.BFD
except:
    e = sys.exc_info()[0]
    sys.exit("Scapy loading error: %s" % e)

def bfd_ipv4_packet(pktlen=66,
                        eth_dst='00:01:02:03:04:05',
                        eth_src='00:06:07:08:09:0a',
                        dl_vlan_enable=False,
                        vlan_vid=0,
                        vlan_pcp=0,
                        dl_vlan_cfi=0,
                        ip_src='192.168.0.1',
                        ip_dst='192.168.0.2',
                        ip_tos=0,
                        ip_ttl=255,
                        ip_id=0x0001,
                        udp_sport=1234,
                        udp_dport=3784,
                        with_udp_chksum=False,
                        ip_ihl=None,
                        ip_options=False,
                        version = 1,
                        diag = 0,
                        sta = 3,
                        flags = 0x00,
                        detect_mult = 0x03,
                        bfdlen =  24,
                        my_discriminator = 0x11111111,
                        your_discriminator = 0x22222222,
                        min_tx_interval = 1000000000,
                        min_rx_interval = 1000000000,
                        echo_rx_interval = 1000000000):
    """
    Return an IPv4 BFD packet
    """
    bfd_hdr = BFD(
                    version = version,
                    diag = diag,
                    sta = sta,
                    flags = flags,
                    detect_mult = detect_mult,
                    len = bfdlen,
                    my_discriminator = my_discriminator,
                    your_discriminator = your_discriminator,
                    min_tx_interval = min_tx_interval,
                    min_rx_interval = min_rx_interval,
                    echo_rx_interval = echo_rx_interval
                 )
    bfd_pkt = simple_udp_packet(
        pktlen=pktlen,
        eth_dst=eth_dst,
        eth_src=eth_src,
        ip_dst=ip_dst,
        ip_src=ip_src,
        ip_ttl=ip_ttl,
        ip_tos=ip_tos,
        udp_sport=udp_sport,
        udp_dport=udp_dport,
        with_udp_chksum=with_udp_chksum,
        udp_payload=bfd_hdr
    )


    return bfd_pkt
