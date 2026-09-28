#!/usr/bin/env python3
"""Parse rdma_fct_test.sh results.
usage: rdma_fct_parse.py <dir> [<dir>...]
Prints per-run: elephant BW/wall, mouse completion wall ms, retrans stats
from the pcap (duplicate PSN per QPN = go-back-N / timer retransmissions).
"""
import glob, os, subprocess, sys
from collections import defaultdict

def fmt_wall(path):
    t0, t1, rc = open(path).read().split()[:3]
    return (float(t1)-float(t0))*1000, int(rc)

def parse(dirp):
    print(f"===== {dirp} =====")
    for f in sorted(glob.glob(dirp+"/ele_*.out")):
        i = os.path.basename(f)[4:-4]
        wall, rc = fmt_wall(dirp+f"/ele_{i}.wall")
        bw = "?"
        for ln in open(f):
            p = ln.split()
            if len(p) >= 5 and p[0].isdigit():
                bw = p[3]; break
        ok = "OK " if rc == 0 else "FAIL"
        print(f"  elephant{i}: {ok} wall={wall:8.0f}ms bw={bw}MB/s")

    mice_w, mice_ok, mice_tot = [], 0, 0
    for f in sorted(glob.glob(dirp+"/mouse_*.out"),
                    key=lambda p: int(os.path.basename(p).split('_')[1].split('.')[0])):
        i = os.path.basename(f)[6:-4]
        t0, t1, rc, tries = open(dirp+f"/mouse_{i}.wall").read().split()
        wall = (float(t1)-float(t0))*1000
        mice_tot += 1
        done = "OK " if rc == "0" else "FAIL"
        if rc == "0": mice_ok += 1; mice_w.append(wall)
        print(f"  mouse{i:>2}: {done} wall={wall:7.0f}ms tries={tries}")
    if mice_w:
        mice_w.sort()
        n = len(mice_w)
        print(f"  mice: {mice_ok}/{mice_tot} complete  FCT p50={mice_w[n//2]:.0f}ms "
              f"max={mice_w[-1]:.0f}ms")

    pcap = dirp+"/roce.pcap"
    if os.path.exists(pcap) and os.path.getsize(pcap) > 0:
        # RoCE BTH sits 12 bytes into the UDP payload (8B UDP + 4B unused):
        # BTH byte0=opcode, QPN=bytes3..5, PSN=bytes6..8 (network order).
        # Parse raw with tcpdump -X would be clumsy; use tshark if present,
        # else fall back to raw python parsing of the pcap via dpkt-less
        # manual record walk below.
        try:
            out = subprocess.run(
                ["tcpdump", "-r", pcap, "-nn", "udp port 4791", "-c", "5"],
                capture_output=True, text=True, timeout=60)
        except Exception as e:
            print(f"  pcap read error: {e}"); return
        dup, seen = defaultdict(int), defaultdict(set)
        total = 0
        qpn_map = {}   # udp sport -> QPN (first seen)
        # manual pcap walk: 24B global hdr, per-rec 16B hdr
        data = open(pcap, "rb").read()
        if len(data) < 24: return
        off = 24
        while off + 16 <= len(data):
            caplen = int.from_bytes(data[off+8:off+12], "little")
            pkt = data[off+16:off+16+caplen]
            off += 16 + caplen
            total += 1
            # eth(14) ip(20) udp(8) unused(4) -> BTH at 46; qpn off+3..6, psn +6..9
            if len(pkt) < 55: continue
            bth = pkt[46:55]
            qpn = int.from_bytes(bth[3:6], "big")
            psn = int.from_bytes(bth[6:9], "big") & 0xffffff
            if psn in seen[qpn]: dup[qpn] += 1
            seen[qpn].add(psn)
        tot_dup = sum(dup.values())
        print(f"  pcap: {total} roce pkts, {len(seen)} QPs, "
              f"dup-PSN pkts (retrans) = {tot_dup}")

for d in sys.argv[1:]:
    parse(d)
