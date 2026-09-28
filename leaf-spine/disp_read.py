#!/usr/bin/env python3
"""Read / reset the P4 spine-dispersion registers (disp_sp1_reg / disp_sp2_reg).

usage:
  disp_read.py            # dump per-flow spine packet counts + aggregate split
  disp_read.py --reset    # zero both registers at the known flow indices

Flow index = ((src_last_octet << 8) | dst_last_octet) & 0xFFF of the inner
packet at leaf1 (10.100.1.x -> 10.100.2.x), e.g. h1->h9 = 0xB15.
"""
import sys
import bfrt_grpc.client as gc

P4_NAME = "tna_lb_mixhash"
GRPC_ADDR = "localhost:50052"

# test-flow name -> (src host 10.100.1.X, dst host 10.100.2.Y)
FLOWS = {
    "e1": (11, 21), "e2": (12, 22),
    "m1": (11, 25), "m2": (12, 26), "m3": (13, 27), "m4": (14, 28),
    "m5": (15, 25), "m6": (16, 26), "m7": (17, 25), "m8": (18, 26),
}
IDX2FLOW = {((s << 8) | d) & 0xFFF: n for n, (s, d) in FLOWS.items()}
ORDER = ["e1", "e2", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8"]
IDX_OF = {n: ((s << 8) | d) & 0xFFF for n, (s, d) in FLOWS.items()}
# h7/h8 sit on device ports 128/129 = pipe 1; tofino-model reads back pipe-1
# SALU counts shifted 8 bits (counts land in byte1, not byte0). Verified with
# fixed-count pings: idx 0x119/0x21A store count<<8, all pipe-0 idx store count.
SHIFT = {"m7": 8, "m8": 8}


def connect():
    interface = gc.ClientInterface(grpc_addr=GRPC_ADDR, client_id=0, device_id=0)
    interface.bind_pipeline_config(P4_NAME)
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)
    return interface, bfrt_info, target


def main():
    do_reset = "--reset" in sys.argv
    interface, bfrt_info, target = connect()
    regs = {}
    for spine, name in (("spine1", "disp_sp1_reg"), ("spine2", "disp_sp2_reg")):
        regs[spine] = bfrt_info.table_get(f"pipe.SwitchIngress.{name}")

    if do_reset:
        for spine, t in regs.items():
            fname = f"SwitchIngress.disp_{spine.replace('spine', 'sp')}_reg.f1"
            idxs = list(IDX2FLOW)
            keys = [t.make_key([gc.KeyTuple("$REGISTER_INDEX", i)]) for i in idxs]
            datas = [t.make_data([gc.DataTuple(fname, 0)]) for _ in idxs]
            t.entry_mod(target, keys, datas)
        print("dispersion registers reset")
        interface._die = True
        return

    counts = {"spine1": {}, "spine2": {}}
    for spine, t in regs.items():
        idxs = list(IDX2FLOW)
        keys = [t.make_key([gc.KeyTuple("$REGISTER_INDEX", i)]) for i in idxs]
        resp = t.entry_get(target, keys)
        for data, key in resp:
            kd = key.to_dict()
            idx = kd["$REGISTER_INDEX"]["value"]
            dd = data.to_dict()
            val = 0
            for fname, fv in dd.items():
                if fname.endswith(".f1"):
                    # bytes-width fields arrive as little-endian byte lists
                    if isinstance(fv, list):
                        val = sum((b & 0xFF) << (8 * i) for i, b in enumerate(fv))
                    elif isinstance(fv, dict):
                        val = fv["value"]
                    else:
                        val = fv
                    break
            counts[spine][idx] = val >> SHIFT.get(IDX2FLOW.get(idx, ""), 0)

    print(f"{'flow':>6} {'spine1':>8} {'spine2':>8} {'sp1%':>6}   skew")
    t1 = t2 = 0
    for n in ORDER:
        idx = IDX_OF[n]
        a = counts["spine1"].get(idx, 0)
        b = counts["spine2"].get(idx, 0)
        t1 += a
        t2 += b
        tot = a + b
        pct = f"{100*a/tot:.0f}%" if tot else "-"
        # per-flow skew: |a-b|/(a+b), 0 = perfectly balanced across spines
        skew = f"{abs(a-b)/tot:.2f}" if tot else "-"
        print(f"{n:>6} {a:>8} {b:>8} {pct:>6}   {skew}")
    T = t1 + t2
    print("-" * 34)
    if T:
        print(f"{'ALL':>6} {t1:>8} {t2:>8} {100*t1/T:>5.1f}%")
    else:
        print("no data")
    interface._die = True


if __name__ == "__main__":
    main()
