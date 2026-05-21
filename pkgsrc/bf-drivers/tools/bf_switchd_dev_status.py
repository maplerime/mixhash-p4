#!/usr/bin/env python3

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


import socket
import sys
import argparse
import datetime
import time

# http://stackoverflow.com/questions/6405208/how-to-convert-numeric-string-ranges-to-a-list-in-python
def get_valid_ranges(x):
    result = set()
    for part in x.split(','):
        if '-' in part:
            a, b = part.split('-')
            a, b = int(a), int(b)
            result |= set(range(a, b + 1))
        else:
            a = int(part)
            result.add(a)
    return sorted(result)

parser = argparse.ArgumentParser(description='Query bf_switchd until the device is ready.')
parser.add_argument('--port', type=int, default=7777, help='Port number bf_switchd is listening on, default is 7777')
parser.add_argument('--host', type=str, default='localhost', help='Host to query, default is localhost')
parser.add_argument('--device', type=str, default='0', help='Device id to query (1 or 0-2 or 0,4,6-8)')
parser.add_argument('--timeout', type=int, default=300, help='Timeout (in seconds) before giving up')
args = parser.parse_args()

dev_ids = get_valid_ranges(args.device)
s = None
start_time = datetime.datetime.now()
timeout_time = start_time + datetime.timedelta(seconds=args.timeout)
tmo = timeout_time <= datetime.datetime.now()

print("Connecting to", args.host, "port", args.port, "to check status on these devices:", dev_ids)

last_dev_id = None
while len(dev_ids) and not tmo:
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect( (args.host, args.port) )
        s.settimeout(1)
        dev_id = dev_ids[0]
        if last_dev_id != dev_id:
            last_dev_id = dev_id
            print("Waiting for device", dev_id, "to be ready")
        s.sendall(str(dev_id).encode())
        r = s.recv(1)
        s.close()
        s = None
        if r.decode() == '1':
            dev_ids.remove(dev_id)
        else:
            time.sleep(1)

    except:
        if s:
            s.close()
            s = None
        time.sleep(1)
    tmo = timeout_time <= datetime.datetime.now()


if len(dev_ids):
    print("Timeout or error while waiting for devices to be ready")
    sys.exit(1)
else:
    sys.exit(0)
