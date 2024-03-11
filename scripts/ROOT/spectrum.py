# Copyright 2021-2024
#
# Authors:
# Stanimir Kisyov, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

import uproot as up
import numpy as np
from pathlib import Path
from scipy.constants import physical_constants
import argparse

MeV = 1
GeV = 1000 * MeV

_m_e = physical_constants["electron mass energy equivalent in MeV"][0] * MeV
_m_mu = physical_constants["muon mass energy equivalent in MeV"][0] * MeV
up.default_library = "np"

def particlespectrum(file: Path, particle: str, muonfactor: float):
    from uproot import open
    from numpy import sum, histogram

    if particle == "muon":
        ntuplename = "FinalMuon"
        mass = _m_mu
    elif particle == "positron":
        ntuplename = "FinalPositron"
        mass = _m_e
        muonfactor = 1
    else:
        raise NameError("Particle name {} incorrect".format(particle))
    
    ntuple = open(file)["ntuple"]
    part = ntuple[ntuplename]
    cp = ntuple["PrimaryCount"]
    ns = cp["N"].array()
    Nprimaries = sum(ns)

    if particle == "muon":
        procs = part["process"].array()
        Nmupairidx = np.where(procs == "GammaToMuPair")[0]
        px = part["px"].array()[Nmupairidx]
        py = part["py"].array()[Nmupairidx]
        pz = part["pz"].array()[Nmupairidx]
    else:
        px = part["px"].array()
        py = part["py"].array()
        pz = part["pz"].array()

    ekin = np.sqrt(mass**2 + px**2 + py**2 + pz**2) - mass
    ntuple.close()
    hist, bin_edges = histogram(ekin, bins=200)
    hist = hist / (Nprimaries * muonfactor)
    return hist, bin_edges

def plotspectrum(file: Path, particle: str, muonfactor: float):
    import matplotlib as mpl
    from matplotlib.pyplot import subplots, rc

    mpl.use("pgf")
    params = {
        "font.size": 18,
        "lines.linewidth": 3,
        "font.family": "serif",
        # "font.style": "normal",
        "legend.fontsize": 18,
        # "legend.frameon": False,
        # "legend.numpoints": 1,
        "xtick.labelsize": 18,
        "ytick.labelsize": 18,
        "figure.figsize": [8, 6],
        "axes.labelsize": 18,
        "text.usetex": True,
    }
    mpl.rcParams.update(params)
    rc("text.latex", preamble=r"\usepackage{amsmath}")

    hist, bin_edges = particlespectrum(file, particle, muonfactor)

    edges = 0.5*(bin_edges[1:] + bin_edges[:-1])
    fig, ax = subplots(layout="constrained")
    ax.plot(edges / GeV, hist, color="black", linewidth=3)
    ax.set_xlabel("$E_{kin} [GeV]$")
    ax.set_ylabel("$1/N_0 dN / dE \,[1/GeV]$")
    ax.set_yscale("log")
    fig.savefig("spectrum_{}_{}.png".format(file.stem, particle), dpi=200)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="spectrum.py",
        description="Produce the particle spectrum.",
    )
    parser.add_argument(
        "particle",
        metavar="muon",
        type=str,
        nargs=1,
        help="Particle type",
    )
    parser.add_argument(
        "file",
        metavar="filename1.root filename2.root",
        type=str,
        nargs="+",
        help="File from which plots are extracted",
    )
    parser.add_argument(
        "--muonfactor",
        metavar="muonfactor",
        type=float,
        nargs=1,
        default=[1],
        help="Muon cross section increase.",
        required=False,
        dest="muonfactor",
    )

    args = parser.parse_args()
    inputfiles = args.file
    particle = args.particle[0]
    muonfactor = args.muonfactor[0]

    if particle not in ["muon", "positron"]:
        raise NameError("Particle name {} incorrect".format(particle))
    
    for ff in inputfiles:
        print("Working on file", ff)
        pathff = Path(ff)
        plotspectrum(pathff, particle, muonfactor)
