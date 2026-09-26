#!/usr/bin/env python3
# Parse fct_test.sh results. usage: fct_parse.py <tag> [<tag>...]
import json, sys, glob, os

def load(tag):
    out = {}
    d = f'/tmp/fct_{tag}'
    for f in sorted(glob.glob(d+'/big_*.json')):
        h = os.path.basename(f)[4:-5]
        try:
            j = json.load(open(f))
            sent = j['end']['sum_sent']; rcv = j['end']['sum_received']
            out[('big', h)] = dict(
                fct_s=rcv['seconds'], send_s=sent['seconds'],
                bytes=sent['bytes'], mbps=rcv['bits_per_second']/1e6,
                retrans=sent.get('retransmits', -1))
        except Exception as e:
            out[('big', h)] = dict(err=str(e))
    for f in sorted(glob.glob(d+'/mice_*.json'), key=lambda p: int(os.path.basename(p).split('_')[1])):
        base = os.path.basename(f)[:-5]
        try:
            j = json.load(open(f))
            rcv = j['end']['sum_received']
            out[('mice', base)] = dict(fct_ms=rcv['seconds']*1000, bytes=rcv['bytes'])
        except Exception as e:
            out[('mice', base)] = dict(err=str(e))
    return out

for tag in sys.argv[1:]:
    r = load(tag)
    print(f'===== {tag} =====')
    print('-- big flows (8MB) --')
    for (k, h), v in sorted(r.items()):
        if k != 'big': continue
        if 'err' in v: print(f'{h}: ERROR {v["err"]}'); continue
        print(f'{h}: FCT={v["fct_s"]:.1f}s  send_done={v["send_s"]:.1f}s  {v["mbps"]:.2f}Mbps  retrans={v["retrans"]}')
    print('-- mice (8KB) --')
    ms = [v['fct_ms'] for (k,h),v in r.items() if k=='mice' and 'fct_ms' in v]
    errs = [(h,v['err']) for (k,h),v in r.items() if k=='mice' and 'err' in v]
    for (k, h), v in sorted(r.items()):
        if k=='mice' and 'fct_ms' in v: print(f'{h}: {v["fct_ms"]:.0f} ms')
    if errs: print('errors:', errs)
    if ms:
        ms.sort()
        print(f'summary: n={len(ms)} min={ms[0]:.0f} med={ms[len(ms)//2]:.0f} p90={ms[int(len(ms)*0.9)-1 if len(ms)>1 else 0]:.0f} max={ms[-1]:.0f} mean={sum(ms)/len(ms):.0f} (ms)')
