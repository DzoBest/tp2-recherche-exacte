#!/usr/bin/env python3
"""Trace temps = f(m) pour chaque alphabet, et le meilleur algorithme par (sigma, m)."""
import csv, collections
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

data = collections.defaultdict(lambda: collections.defaultdict(dict))
for r in csv.DictReader(open("resultats.csv")):
    data[int(r["alphabet"])][r["algo"]][int(r["m"])] = float(r["temps"])

fig, axes = plt.subplots(2, 2, figsize=(14, 10))
for ax, s in zip(axes.flat, sorted(data)):
    for algo, pts in data[s].items():
        ms = sorted(pts)
        ax.plot(ms, [pts[m] for m in ms], marker="o", ms=3, label=algo,
                linestyle="--" if algo.startswith("naif") else "-")
    ax.set_title(f"Alphabet de taille {s}")
    ax.set_xlabel("longueur du mot m"); ax.set_ylabel("temps total (s), 100 mots")
    ax.set_yscale("log"); ax.grid(True, alpha=.3)
axes.flat[0].legend(fontsize=7)
plt.tight_layout(); plt.savefig("courbes.png", dpi=150)

print("Meilleur algorithme par (alphabet, m) :")
for s in sorted(data):
    best = {}
    for m in sorted(next(iter(data[s].values()))):
        best[m] = min(data[s], key=lambda a: data[s][a][m])
    print(f"  sigma={s}: " + ", ".join(f"m={m}:{a}" for m, a in best.items()))
