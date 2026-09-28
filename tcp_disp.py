#!/usr/bin/env python3
"""Per-flow (inner TCP 4-tuple) spine dispersion from raw pcaps.
usage: tcp_disp.py <spine1.pcap> <spine2.pcap>
Counts VXLAN-decapsulated packets per inner flow per spine; forward
(client->server) direction only by default (inner src in 10.100.1.0/24).
"""
import struct, sys
from collections import defaultdict

def read_pcap(path):
    with open(path, "rb") as f:
        gh = f.read(24)
        if len(gh) < 24 or gh[:4] not in (b"\xd4\xc3\xb2\xa1", b"\xa1\xb2\xc3\xd4"):
            raise SystemExit(f"{path}: not a pcap")
        le = gh[:4] == b"\xd4\xc3\xb2\xa1"
        end = "<" if le else ">"
        linktype = struct.unpack(end + "I", gh[20:24])[0]
        assert linktype == 1, f"{path}: linktype {linktype} != EN10MB"
        while True:
            ph = f.read(16)
            if len(ph) < 16:
                return
            caplen = struct.unpack(end + "I", ph[8:12])[0]
            yield f.read(caplen)

def flow_key(pkt):
    # outer: eth(14) + ip(20) + udp(8) + vxlan(8) = 50
    if len(pkt) < 50 + 34:
        return None
    if struct.unpack("!H", pkt[12:14])[0] != 0x0800:
        return None
    oip = pkt[14:]
    if (oip[0] >> 4) != 4 or oip[9] != 17:
        return None
    oh = (oip[0] & 0xF) * 4
    ou = oip[oh:]
    if struct.unpack("!H", ou[2:4])[0] != 4789:
        return None
    vni = struct.unpack("!I", ou[8:11] + b"\0")[0]
    inner = ou[16:]                      # inner ethernet
    if struct.unpack("!H", inner[12:14])[0] != 0x0800:
        return None
    ip = inner[14:]
    if (ip[0] >> 4) != 4 or ip[9] != 6:  # inner TCP
        return None
    ihl = (ip[0] & 0xF) * 4
    tcp = ip[ihl:]
    sport, dport = struct.unpack("!HH", tcp[0:4])
    src = ".".join(map(str, ip[12:16]))
    dst = ".".join(map(str, ip[16:20]))
    payload = struct.unpack("!H", ip[2:4])[0] - ihl - ((tcp[12] >> 4) * 4)
    return src, dst, sport, dport, vni, payload

stats = defaultdict(lambda: [0, 0, 0])   # pkts_sp1, pkts_sp2, bytes_fwd
for spine, path in enumerate(sys.argv[1:3]):
    for pkt in read_pcap(path):
        k = flow_key(pkt)
        if not k:
            continue
        src, dst, sport, dport, vni, payload = k
        fwd = src.startswith("10.100.1.")
        key = (src, dst)
        stats[key][spine] += 1
        if fwd:
            stats[key][2] += max(payload, 0)

print(f"{'flow':>25} {'spine1':>7} {'spine2':>7} {'sp1%':>6} {'fwd KB':>7}")
t1 = t2 = tb = 0
for (src, dst) in sorted(stats):
    a, b, byt = stats[(src, dst)]
    t1 += a; t2 += b; tb += byt
    tot = a + b
    print(f"{src+' > '+dst:>25} {a:>7} {b:>7} {100*a/tot:>5.0f}% {byt/1024:>7.0f}")
T = t1 + t2
print("-" * 60)
print(f"{'TOTAL':>25} {t1:>7} {t2:>7} {100*t1/T if T else 0:>5.1f}% {tb/1024:>7.0f}")
