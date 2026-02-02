# Copyright 2021-2026
#
# Authors:
# Stanimir Kisyov, Sarah Schröder, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import sys
import uproot as up
from scipy.constants import physical_constants
import hepunits.units as un
from mpi4py import MPI
import time as timelibrary

params = {
    "font.size": 16,
    "lines.linewidth": 3,
    "legend.fontsize": 16,
    "xtick.labelsize": 16,
    "ytick.labelsize": 16,
    "axes.labelsize": 16,
    "text.usetex": True,
    "mathtext.fontset": "cm",
    "svg.fonttype": "none",
    "font.family": "serif",
}
mpl.rcParams.update(params)
mpl.use("Cairo")

# Import latex
m_mu = physical_constants["muon mass energy equivalent in MeV"][0] * un.MeV


def get_task_indices(N, M, rank):
    """
    Divides N events among M tasks, returning start/end indices for a given rank.

    Args:
        N: Total number of events
        M: Total number of tasks
        rank: Current task ID (0-indexed)

    Returns:
        (start_idx, end_idx): Start (inclusive) and end (exclusive) indices
    """
    base = N // M  # Integer division for base events per task
    remainder = N % M  # Remaining events to distribute

    # First 'remainder' tasks get (base+1) events
    # Remaining tasks get (base) events
    if rank < remainder:
        start_idx = rank * (base + 1)
        end_idx = start_idx + base + 1
    else:
        start_idx = remainder * (base + 1) + (rank - remainder) * base
        end_idx = start_idx + base

    return start_idx, end_idx


def mu_pair_decay(filename, chunk_size="200MB", n_workers=1):

    # Initialize MPI environment
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    # Only rank 0 prints general information
    if rank == 0:
        print(f"Running with {size} MPI processes")
        print("STARTED")
        sys.stdout.flush()

    start_execution = timelibrary.time() * un.s

    # All ranks need to know the primary count
    with up.open(filename, num_workers=1)["ntuple"] as ntuple:
        local_N0 = np.sum(ntuple["PrimaryCount"]["N"].array(library="np"))
        # Sum across all ranks
        N0 = local_N0

    if rank == 0:
        print(f"\tNumber of primaries: {N0:.2e}")
        sys.stdout.flush()

    # Counting the number of entries
    with up.open(filename, num_workers=1)["ntuple/FinalMuon"] as fmuons:
        number_of_entries = fmuons.num_entries

    # The end index is exclusive
    start_idx, end_idx = get_task_indices(number_of_entries, size, rank)

    chunk_count = 0
    # Define the bins for the histogram
    x_bins = (
        np.linspace(0, 10, 150) * un.GeV
    )  # 150 bins for x-axis, including both endpoints
    # Initialize histograms to 0
    local_histpair = np.zeros(len(x_bins) - 1, dtype=np.float32)
    local_histdecay = np.zeros(len(x_bins) - 1, dtype=np.float32)
    local_histpairdiv = np.zeros(len(x_bins) - 1, dtype=np.float32)
    local_histdecaydiv = np.zeros(len(x_bins) - 1, dtype=np.float32)
    # Initialize weight sums to 0
    local_weights_sum_pair = 0
    local_weights_sum_decay = 0
    local_weights_sum_pairdiv = 0
    local_weights_sum_decaydiv = 0
    with up.open(filename, num_workers=1)["ntuple/FinalMuon"] as fmuons:
        for chunk in fmuons.iterate(
            ["px", "py", "pz", "w", "process"],
            entry_start=start_idx,
            entry_stop=end_idx,
            step_size=chunk_size,
            library="np",
            how=list,
        ):
            chunk_count += 1

            E = np.sqrt(m_mu**2 + chunk[0] ** 2 + chunk[1] ** 2 + chunk[2] ** 2)
            weight = chunk[3]
            angle = np.arctan2(np.hypot(chunk[0], chunk[1]), -chunk[2])
            process = chunk[4]

            pair_mask = process == "biasWrapper(GammaGeneralProc)"
            decay_mask = process == "biasWrapper(Decay)"
            pair_lowdiv_mask = (angle < 50 * un.mrad) & pair_mask
            decay_lowdiv_mask = (angle < 50 * un.mrad) & decay_mask

            histpairtmp, _ = np.histogram(
                E[pair_mask], bins=x_bins, density=True, weights=weight[pair_mask]
            )
            histdecaytmp, _ = np.histogram(
                E[decay_mask], bins=x_bins, density=True, weights=weight[decay_mask]
            )
            histpairdivtmp, _ = np.histogram(
                E[pair_lowdiv_mask],
                bins=x_bins,
                density=True,
                weights=weight[pair_lowdiv_mask],
            )
            histdecaydivtmp, _ = np.histogram(
                E[decay_lowdiv_mask],
                bins=x_bins,
                density=True,
                weights=weight[decay_lowdiv_mask],
            )

            # Sum the histograms and the weights
            ss = np.sum(weight[pair_mask])
            local_histpair += histpairtmp * ss
            local_weights_sum_pair += ss
            ss = np.sum(weight[decay_mask])
            local_histdecay += histdecaytmp * ss
            local_weights_sum_decay += ss
            ss = np.sum(weight[pair_lowdiv_mask])
            local_histpairdiv += histpairdivtmp * ss
            local_weights_sum_pairdiv += ss
            ss = np.sum(weight[decay_lowdiv_mask])
            local_histdecaydiv += histdecaydivtmp * ss
            local_weights_sum_decaydiv += ss

    # Now merge data across ranks
    weights_sum_pair = comm.allreduce(local_weights_sum_pair, op=MPI.SUM)
    weights_sum_decay = comm.allreduce(local_weights_sum_decay, op=MPI.SUM)
    weights_sum_pairdiv = comm.allreduce(local_weights_sum_pairdiv, op=MPI.SUM)
    weights_sum_decaydiv = comm.allreduce(local_weights_sum_decaydiv, op=MPI.SUM)

    histpair = np.zeros_like(local_histpair)
    comm.Allreduce(local_histpair, histpair, op=MPI.SUM)
    histdecay = np.zeros_like(local_histdecay)
    comm.Allreduce(local_histdecay, histdecay, op=MPI.SUM)
    histpairdiv = np.zeros_like(local_histpairdiv)
    comm.Allreduce(local_histpairdiv, histpairdiv, op=MPI.SUM)
    histdecaydiv = np.zeros_like(local_histdecaydiv)
    comm.Allreduce(local_histdecaydiv, histdecaydiv, op=MPI.SUM)

    # Set the plot size with a 1:1 aspect ratio
    if rank == 0:
        fig, ax = plt.subplots(figsize=(7, 4.33), layout="constrained")

        bincenters = 0.5 * (x_bins[1:] + x_bins[:-1])

        # Plot the spectrum
        print(
            "Number of muons from pair per initial electron {:2.2e}".format(
                weights_sum_pair / N0
            )
        )
        sys.stdout.flush()
        ax.plot(
            bincenters / un.GeV,
            histpair / N0 / (1 / un.GeV),
            c="black",
            label=r"$\mu_{Pair}$",
        )
        ax.plot(
            bincenters / un.GeV,
            histdecay / N0 / (1 / un.GeV),
            c="red",
            label=r"$\mu_{Decay}$",
        )
        ax.plot(
            bincenters / un.GeV,
            histpairdiv / N0 / (1 / un.GeV),
            c="black",
            linestyle="dashed",
            label=r"$\mu_{Pair}\,(<100\, mrad)$",
        )
        ax.plot(
            bincenters / un.GeV,
            histdecaydiv / N0 / (1 / un.GeV),
            c="red",
            linestyle="dashed",
            label=r"$\mu_{Decay}\,(<100\, mrad)$",
        )

        # Labeling and title
        ax.set_xlabel(r"Energy, $E\,[GeV]$")
        ax.set_ylabel(r"$1/N_0 dN_\mu/dE\,[1/GeV]$")
        ax.set_title(r"10 GeV primary electron")
        ax.legend()

        ax.grid(True)

        # Set the y-scale to logarithmic with a maximum value of 1
        ax.set_yscale("log")
        # plt.ylim(0.005, 1)  # Set the maximum value to 1 (0.01 is the minimum for log scale)

        # Save the plot as an .svg image
        # plt.savefig('mu_plot.svg', format='svg', bbox_inches='tight', dpi=300)
        fig.savefig("mu_plot_nopion.svg", format="svg", dpi=300)
        fig.savefig("mu_plot_nopion.png", dpi=300)

    end_execution = timelibrary.time() * un.s
    total_execution = end_execution - start_execution
    if rank == 0:
        print("The execution took {:2.2f} minutes".format(total_execution / un.minute))
        sys.stdout.flush()


if __name__ == "__main__":
    mu_pair_decay("Data_output_10GeV_W_1cm_biasing.root")
