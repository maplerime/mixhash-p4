#!/usr/bin/env python3
"""Paper figure: MixHash vs Classic ECMP — spine dispersion + FCT comparison.

Data source: leaf-spine/results.md (R1 m8e2, R2 m16e3, R3 m32e4,
2026-09-28). Output: leaf-spine/dispersion_fct.pdf (+ .png preview).
Palette: validated 2-slot categorical (blue #2a78d6 / orange #eb6834) on white.
"""
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

# ---- palette / ink (validated on white surface) ----
MIX, CLS = "#2a78d6", "#eb6834"          # slot 1 / slot 2, fixed order
INK, INK2, MUTED = "#0b0b0b", "#52514e", "#898781"
GRID, BASE = "#e1e0d9", "#c3c2b7"

plt.rcParams.update({
    "font.family": "sans-serif",
    "font.sans-serif": ["DejaVu Sans"],
    "font.size": 8.5,
    "axes.edgecolor": BASE,
    "axes.linewidth": 0.8,
    "xtick.color": INK2, "ytick.color": INK2,
    "xtick.labelcolor": INK2, "ytick.labelcolor": INK2,
    "axes.labelcolor": INK2,
    "pdf.fonttype": 42,
})

def style_ax(ax, ymax):
    ax.set_axisbelow(True)
    ax.yaxis.grid(True, color=GRID, lw=0.7)
    ax.set_axisbelow(True)
    for s in ("top", "right"):
        ax.spines[s].set_visible(False)
    ax.spines["left"].set_visible(False)
    ax.tick_params(length=0)
    ax.set_ylim(0, ymax)

def bars(ax, x, vals, color, w=0.32):
    # white edge = 2px-style gap between adjacent bars per mark spec
    return ax.bar(x, vals, w, color=color, edgecolor="white", linewidth=1.2, zorder=3)

def label_bars(ax, rects, fmt, dy):
    # white halo: labels are wider than their bars and graze the neighbour
    # bar's body; the pad keeps the ink off the colour
    for r in rects:
        ax.annotate(fmt.format(r.get_height()),
                    (r.get_x() + r.get_width() / 2, r.get_height()),
                    xytext=(0, dy), textcoords="offset points",
                    ha="center", va="bottom", fontsize=7.5, color=INK2,
                    bbox=dict(boxstyle="square,pad=0.15", facecolor="white",
                              edgecolor="none"))

fig, axes = plt.subplots(1, 3, figsize=(7.0, 2.3))

# ---- (a) per-flow spine skew (mean) ----
ax = axes[0]
rounds = ["R1\n(10 flows)", "R2\n(19 flows)", "R3\n(36 flows)"]
x = range(3)
mix_skew = [0.19, 0.29, 0.27]
cls_skew = [1.00, 1.00, 1.00]
b1 = bars(ax, [i - 0.17 for i in x], mix_skew, MIX)
b2 = bars(ax, [i + 0.17 for i in x], cls_skew, CLS)
label_bars(ax, b1, "{:.2f}", 2); label_bars(ax, b2, "{:.2f}", 2)
ax.set_xticks(x); ax.set_xticklabels(rounds, fontsize=7.5)
ax.set_xlim(-0.7, 2.75)
style_ax(ax, 1.22)
ax.set_title("(a) Per-flow spine skew (mean)", fontsize=8.5, color=INK, pad=8)

# ---- (b) aggregate sp1 share ----
ax = axes[1]
mix_agg = [46.0, 59.8, 53.2]
cls_agg = [60.0, 46.7, 66.7]
b1 = bars(ax, [i - 0.17 for i in x], mix_agg, MIX)
b2 = bars(ax, [i + 0.17 for i in x], cls_agg, CLS)
label_bars(ax, b1, "{:.0f}%", 2); label_bars(ax, b2, "{:.0f}%", 2)
even_ln = ax.axhline(50, color=MUTED, lw=0.9, ls=(0, (4, 3)), zorder=2)
ax.set_xticks(x); ax.set_xticklabels(rounds, fontsize=7.5)
ax.set_xlim(-0.7, 2.75)
style_ax(ax, 82)
ax.set_title("(b) Aggregate traffic on spine 1", fontsize=8.5, color=INK, pad=8)

# ---- (c) FCT avg ----
ax = axes[2]
groups = ["R2\neleph.", "R2\nmice", "R3\neleph.", "R3\nmice"]
mix_fct = [31.807, 18.070, 74.676, 50.403]   # seconds
cls_fct = [32.436, 18.916, 82.451, 58.886]
xg = range(4)
b1 = bars(ax, [i - 0.17 for i in xg], mix_fct, MIX)
b2 = bars(ax, [i + 0.17 for i in xg], cls_fct, CLS)
# no per-bar labels: labels collide inside each pair; the delta callouts
# below carry the comparison and tab:fct in the paper carries exact values
# delta callouts: R3 = classic's imbalanced draw (mixhash wins, green)
for i, (m, c) in enumerate(zip(mix_fct, cls_fct)):
    if i < 2:
        continue
    d = 100 * (m - c) / c
    good = d < 0  # negative = mixhash faster
    ax.annotate(f"{d:+.1f}%", (i, max(m, c)), xytext=(0, 14),
                textcoords="offset points", ha="center", fontsize=7.5,
                color="#006300" if good else MUTED,
                fontweight="bold" if good else "normal")
ax.set_xticks(xg); ax.set_xticklabels(groups, fontsize=7.5)
ax.set_xlim(-0.7, 3.85)
style_ax(ax, 100)
ax.set_ylabel("FCT avg (s)", fontsize=8)
ax.set_title("(c) Flow completion time (avg)", fontsize=8.5, color=INK, pad=8)

# shared legend, top-level (2 series + panel-b reference line)
from matplotlib.lines import Line2D
even_ref = Line2D([0], [0], color=MUTED, lw=0.9, ls=(0, (4, 3)))
fig.legend([b1, b2, even_ref],
           ["MixHash (per-packet)", "Classic ECMP (per-flow)",
            "even split (50%)"],
           loc="upper center", bbox_to_anchor=(0.5, 1.13), ncol=3,
           frameon=False, fontsize=8, handlelength=1.2, handleheight=0.9,
           columnspacing=1.6)

fig.tight_layout(pad=0.6)
fig.savefig("dispersion_fct.pdf", bbox_inches="tight")
fig.savefig("dispersion_fct.png", dpi=200, bbox_inches="tight")
print("saved")
