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

# ---- flow sets ----
# m8e2: 2 elephants + 8 mice (original layout)
SET_M8E2 = {
    "e1": (11, 21), "e2": (12, 22),
    "m1": (11, 25), "m2": (12, 26), "m3": (13, 27), "m4": (14, 28),
    "m5": (15, 25), "m6": (16, 26), "m7": (17, 25), "m8": (18, 26),
}
# m16e3: 3 elephants + 16 mice (2 per sender, shuffled dsts, 2 mice per dst)
SET_M16E3 = {
    "e1": (11, 21), "e2": (12, 22), "e3": (13, 23),
    "m01": (11, 24), "m02": (11, 27), "m03": (12, 25), "m04": (12, 28),
    "m05": (13, 26), "m06": (13, 21), "m07": (14, 27), "m08": (14, 22),
    "m09": (15, 28), "m10": (15, 23), "m11": (16, 21), "m12": (16, 24),
    "m13": (17, 22), "m14": (17, 25), "m15": (18, 23), "m16": (18, 26),
}
# m32e4: 4 elephants + 32 mice (4 per sender, shuffled dsts, 4 mice per dst)
SET_M32E4 = {
    "e1": (11, 21), "e2": (12, 22), "e3": (13, 23), "e4": (14, 24),
    "m01": (11, 22), "m02": (11, 23), "m03": (11, 24), "m04": (11, 25),
    "m05": (12, 23), "m06": (12, 24), "m07": (12, 25), "m08": (12, 26),
    "m09": (13, 24), "m10": (13, 25), "m11": (13, 26), "m12": (13, 27),
    "m13": (14, 25), "m14": (14, 26), "m15": (14, 27), "m16": (14, 28),
    "m17": (15, 26), "m18": (15, 27), "m19": (15, 28), "m20": (15, 21),
    "m21": (16, 27), "m22": (16, 28), "m23": (16, 21), "m24": (16, 22),
    "m25": (17, 28), "m26": (17, 21), "m27": (17, 22), "m28": (17, 23),
    "m29": (18, 21), "m30": (18, 22), "m31": (18, 23), "m32": (18, 24),
}
# m59e5: 5 elephants + 59 mice. "8 mice per sender" would collide with the
# elephant of the same (src,dst) pair in disp_idx, so the 5 elephant senders
# (h1-h5) each send 7 mice skipping their elephant dst; h6-h8 send 8 each.
# Dst balance: .21-.25 receive 7 mice, .26-.28 receive 8.
SET_M59E5 = {
    "e1": (11, 21), "e2": (12, 22), "e3": (13, 23), "e4": (14, 24), "e5": (15, 25),
    "m01": (11, 24), "m02": (11, 27), "m03": (11, 22), "m04": (11, 25), "m05": (11, 28), "m06": (11, 23), "m07": (11, 26),
    "m08": (12, 21), "m09": (12, 26), "m10": (12, 23), "m11": (12, 28), "m12": (12, 25), "m13": (12, 24), "m14": (12, 27),
    "m15": (13, 22), "m16": (13, 25), "m17": (13, 24), "m18": (13, 27), "m19": (13, 26), "m20": (13, 21), "m21": (13, 28),
    "m22": (14, 23), "m23": (14, 28), "m24": (14, 25), "m25": (14, 26), "m26": (14, 21), "m27": (14, 27), "m28": (14, 22),
    "m29": (15, 26), "m30": (15, 21), "m31": (15, 28), "m32": (15, 24), "m33": (15, 27), "m34": (15, 22), "m35": (15, 23),
    "m36": (16, 27), "m37": (16, 22), "m38": (16, 25), "m39": (16, 21), "m40": (16, 28), "m41": (16, 23), "m42": (16, 26), "m43": (16, 24),
    "m44": (17, 28), "m45": (17, 23), "m46": (17, 26), "m47": (17, 22), "m48": (17, 21), "m49": (17, 24), "m50": (17, 25), "m51": (17, 27),
    "m52": (18, 21), "m53": (18, 24), "m54": (18, 27), "m55": (18, 23), "m56": (18, 22), "m57": (18, 26), "m58": (18, 28), "m59": (18, 25),
}
# m64e5: 5 elephants + 64 mice, strict "8 mice per sender": every sender
# h1-h8 sends one mouse to EACH dst .21-.28 (8 per dst as well). The 5
# elephant senders' own mouse to their elephant dst shares that elephant's
# disp_idx, so those 5 indices carry merged elephant+mouse counts and are
# named "eN+mMM"; per-flow mice skew stats use the 59 clean mice only.
SET_M64E5 = {
    "e1+m08": (11, 21), "e2+m16": (12, 22), "e3+m24": (13, 23),
    "e4+m32": (14, 24), "e5+m40": (15, 25),
    "m01": (11, 24), "m02": (11, 27), "m03": (11, 22), "m04": (11, 25),
    "m05": (11, 28), "m06": (11, 23), "m07": (11, 26),
    "m09": (12, 26), "m10": (12, 23), "m11": (12, 28), "m12": (12, 25),
    "m13": (12, 24), "m14": (12, 27), "m15": (12, 21),
    "m17": (13, 25), "m18": (13, 24), "m19": (13, 27), "m20": (13, 26),
    "m21": (13, 21), "m22": (13, 28), "m23": (13, 22),
    "m25": (14, 28), "m26": (14, 25), "m27": (14, 26), "m28": (14, 21),
    "m29": (14, 27), "m30": (14, 22), "m31": (14, 23),
    "m33": (15, 21), "m34": (15, 28), "m35": (15, 24), "m36": (15, 27),
    "m37": (15, 22), "m38": (15, 23), "m39": (15, 26),
    "m41": (16, 27), "m42": (16, 22), "m43": (16, 25), "m44": (16, 21),
    "m45": (16, 28), "m46": (16, 23), "m47": (16, 26), "m48": (16, 24),
    "m49": (17, 28), "m50": (17, 23), "m51": (17, 26), "m52": (17, 22),
    "m53": (17, 21), "m54": (17, 24), "m55": (17, 25), "m56": (17, 27),
    "m57": (18, 21), "m58": (18, 24), "m59": (18, 27), "m60": (18, 23),
    "m61": (18, 22), "m62": (18, 26), "m63": (18, 28), "m64": (18, 25),
}
FLOWS = SET_M8E2
IDX2FLOW = {((s << 8) | d) & 0xFFF: n for n, (s, d) in FLOWS.items()}
ORDER = ["e1", "e2", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8"]
IDX_OF = {n: ((s << 8) | d) & 0xFFF for n, (s, d) in FLOWS.items()}
# h7/h8 sit on device ports 128/129 = pipe 1; tofino-model reads back pipe-1
# SALU counts shifted 8 bits (counts land in byte1, not byte0). Verified with
# fixed-count pings: idx 0x119/0x21A store count<<8, all pipe-0 idx store count.
def _shifts(flows):
    return {n: 8 for n, (s, d) in flows.items() if s >= 17}
SHIFT = _shifts(FLOWS)


def connect():
    interface = gc.ClientInterface(grpc_addr=GRPC_ADDR, client_id=0, device_id=0)
    interface.bind_pipeline_config(P4_NAME)
    bfrt_info = interface.bfrt_info_get()
    target = gc.Target(device_id=0, pipe_id=0xffff)
    return interface, bfrt_info, target


def main():
    flow_set = "m8e2"
    args = sys.argv[1:]
    if "--set" in args:
        flow_set = args[args.index("--set") + 1]
    if flow_set == "m16e3":
        FLOWS_g = SET_M16E3
    elif flow_set == "m32e4":
        FLOWS_g = SET_M32E4
    elif flow_set == "m59e5":
        FLOWS_g = SET_M59E5
    elif flow_set == "m64e5":
        FLOWS_g = SET_M64E5
    else:
        FLOWS_g = SET_M8E2
    globals()["FLOWS"] = FLOWS_g
    globals()["IDX2FLOW"] = {((s << 8) | d) & 0xFFF: n for n, (s, d) in FLOWS_g.items()}
    globals()["ORDER"] = list(FLOWS_g)
    globals()["IDX_OF"] = {n: ((s << 8) | d) & 0xFFF for n, (s, d) in FLOWS_g.items()}
    globals()["SHIFT"] = _shifts(FLOWS_g)
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
    # group skew summary: clean mice vs merged elephant+mouse indices
    sk = {}
    for n in ORDER:
        idx = IDX_OF[n]
        a = counts["spine1"].get(idx, 0)
        b = counts["spine2"].get(idx, 0)
        if a + b:
            sk[n] = abs(a - b) / (a + b)
    clean = [v for n, v in sk.items() if n.startswith("m")]
    merged = [v for n, v in sk.items() if "+" in n]
    if clean:
        print(f"clean-mice skew: n={len(clean)} mean={sum(clean)/len(clean):.2f} "
              f"range={min(clean):.2f}-{max(clean):.2f} pinned={sum(v == 1.0 for v in clean)}")
    if merged:
        print(f"merged(e+m) skew: n={len(merged)} mean={sum(merged)/len(merged):.2f} "
              f"pinned={sum(v == 1.0 for v in merged)}")
    interface._die = True


if __name__ == "__main__":
    main()
