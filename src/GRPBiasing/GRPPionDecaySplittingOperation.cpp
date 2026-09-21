// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause-LBNL

#include <G4BiasingProcessInterface.hh>
#include <G4MuonMinus.hh>
#include <G4MuonPlus.hh>
#include <G4PionMinus.hh>
#include <G4PionPlus.hh>

#include <GRPPionDecaySplittingOperation.hpp>

// Utitily functions to know if a particle is a muon or a pion
inline G4bool IsMuon(const G4Track &track)
{
    return (
        track.GetDefinition() == G4MuonMinus::MuonMinus() ||
        track.GetDefinition() == G4MuonPlus::MuonPlus());
}

inline G4bool IsPion(const G4Track &track)
{
    return (
        track.GetDefinition() == G4PionMinus::PionMinus() ||
        track.GetDefinition() == G4PionPlus::PionPlus());
}

GRPPionDecaySplittingOperation::GRPPionDecaySplittingOperation(G4String name)
    : G4VBiasingOperation(name), m_SplittingFactor(1)
{}

GRPPionDecaySplittingOperation::~GRPPionDecaySplittingOperation() {}

G4VParticleChange *GRPPionDecaySplittingOperation::ApplyFinalStateBiasing(
    const G4BiasingProcessInterface *callingProcess,
    const G4Track *track,
    const G4Step *step,
    G4bool &)
{
    constexpr G4bool verbose = false;
    // Collect wrapped process (pion Decay) final state:
    G4VParticleChange *processFinalState =
        callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);

    // If no splitting requested, let the process return directly its
    // generated final state:
    if (m_SplittingFactor == 1)
        return processFinalState;

    // Only act if the particle is a pion
    if (!IsPion(*track))
        return processFinalState;

    // Check that the number of secondaries is two (mu and neutrino)
    // If not warn the user and return unaltered process
    if (processFinalState->GetNumberOfSecondaries() != 2)
    {

        const G4String pname =
            callingProcess->GetWrappedProcess()->GetProcessName();
        const G4int nsecondaries = processFinalState->GetNumberOfSecondaries();
        G4ExceptionDescription msg;
        msg << "The process " << pname << " is counting " << nsecondaries
            << " secondaries. ";
        msg << "The expected value is 2. No biasing is performed." << G4endl;

        G4Exception(
            "GRPPionDecaySplittingOperation::ApplyFinalStateBiasing",
            "GRAPPA::INCORRECT_NUMBER_OF_SECONDARIES",
            G4ExceptionSeverity::JustWarning,
            msg);

        return processFinalState;
    }

    // Check that the seconday outcome is what it is expected (i.e., exclude
    // rare decays into other particles)
    G4Track *firstsecondarytrack = processFinalState->GetSecondary(0);
    G4Track *secondsecondarytrack = processFinalState->GetSecondary(1);
    if (!(IsMuon(*firstsecondarytrack) || IsMuon(*secondsecondarytrack)))
    {
        return processFinalState;
    }

    // For the biasing the goal is to multiply the number of muons created in
    // the decay. To do so, we first copy the primary particle (pion)
    // into the new particle change and we mark its deletion, like the original
    // process. Then we need to copy the secondaries from the process.
    // IMPORTANT: weighting rationale.
    //
    // We split only the muon: N muons are produced, each from an independent
    // re-sampling of the pion decay, each carrying weight w/N (muweight). The
    // expectation is preserved since N * (w/N) = w, where w is the primary
    // (pion) weight.
    //
    // The neutrino, by contrast, is NOT split. We keep exactly ONE neutrino
    // (from this first decay) and it must retain the PRIMARY weight w, not
    // muweight. Reason: unbiased expectation for a neutrino tally is a single
    // neutrino at weight w, and 1 * w = w. If this neutrino were instead given
    // muweight (= w/N), the neutrino sample would be under-weighted by a factor
    // of N and any neutrino-based tally would be biased.
    //
    // Therefore: DO NOT call SetWeight(muweight) on the neutrino track. It
    // keeps the weight propagated to it by the wrapped decay process, which is
    // the primary weight w.
    //
    // Caveat: only this one neutrino is correlated with its partner muon
    // (muon #1). Muons #2..N have no associated neutrino. This is acceptable
    // for downstream muon tallies, but muon-neutrino correlation studies would
    // be biased by this scheme.

    m_ParticleChange.Initialize(*track);
    // Store final state:
    m_ParticleChange.ProposeMomentumDirection(0., 0., 0.);
    m_ParticleChange.ProposeEnergy(0.);
    m_ParticleChange.ProposeTrackStatus(fStopAndKill);

    // Define the secondary's weigth
    const G4double muweight = track->GetWeight() / m_SplittingFactor;

    // Inform we will have m_SplittingFactor muons + 1 neutrino.
    // Note that we only multiply the number of muons, not the number of
    // neutrinos. So, the number of secondaries is going to be m_SplittingFactor
    // + 1
    m_ParticleChange.SetNumberOfSecondaries(m_SplittingFactor + 1);
    // inform we take care of secondaries weight (otherwise these
    // secondaries are by default given the primary weight).
    m_ParticleChange.SetSecondaryWeightByProcess(true);

    if (IsMuon(*firstsecondarytrack))
    {
        firstsecondarytrack->SetWeight(muweight);
    }
    else
    {
        secondsecondarytrack->SetWeight(muweight);
    }

    m_ParticleChange.AddSecondary(firstsecondarytrack);
    // Keep neutrino at PRIMARY weight w (do NOT set muweight) -- see note
    // above.
    m_ParticleChange.AddSecondary(secondsecondarytrack);

    processFinalState->Clear();

    // Now multiply the secondary particles
    G4int nCalls = 1;
    G4int nWhileLoopIterations = 0;
    while (nCalls < m_SplittingFactor)
    {
        nWhileLoopIterations++;
        // This is a call to the original PostStepDoIt process, renewed
        // every time in the loop.
        // This way new secondaries are created at every iteration
        processFinalState =
            callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);

        // First, we need to check that the process was correct.
        // Second, we need to manually delete all the particles
        // that are not included in the particle change as
        // otherwise they are retained in memory until the end of the program
        const G4int nSec = processFinalState->GetNumberOfSecondaries();
        if (nSec == 2 &&
            (IsMuon(*processFinalState->GetSecondary(0)) ||
             IsMuon(*processFinalState->GetSecondary(1))))
        {
            firstsecondarytrack = processFinalState->GetSecondary(0);
            secondsecondarytrack = processFinalState->GetSecondary(1);

            // Because the expected outcome is a muon and a neutrino,
            // If the first particle is a muon then the second cannot be
            if (IsMuon(*firstsecondarytrack))
            {
                firstsecondarytrack->SetWeight(muweight);
                m_ParticleChange.AddSecondary(firstsecondarytrack);
                delete secondsecondarytrack;
            }
            else
            {
                secondsecondarytrack->SetWeight(muweight);
                m_ParticleChange.AddSecondary(secondsecondarytrack);
                delete firstsecondarytrack;
            }
            nCalls++;
        }
        else
        {
            for (G4int secidx = 0; secidx < nSec; secidx++)
            {
                G4Track *secpointer = processFinalState->GetSecondary(secidx);
                delete secpointer;
            }
        }
        processFinalState->Clear();
    }
    processFinalState->Clear();
    if (verbose)
    {
        G4cout << "Returning biased particles" << G4endl;
        G4cout << "Generating " << m_SplittingFactor
               << " muons via pion decay took " << nWhileLoopIterations
               << " while loop iterations" << G4endl;
    }
    return &m_ParticleChange;
}
