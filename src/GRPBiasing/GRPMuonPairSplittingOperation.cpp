// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <G4BiasingProcessInterface.hh>
#include <G4Gamma.hh>
#include <G4GammaConversionToMuons.hh>
#include <G4GammaGeneralProcess.hh>
#include <G4MuonMinus.hh>
#include <G4MuonPlus.hh>

#include <GRPMuonPairSplittingOperation.hpp>

// Utitily functions to know if a particle is a muon or a pion
inline G4bool IsMuon(const G4Track &track)
{
    return (
        track.GetDefinition() == G4MuonMinus::MuonMinus() ||
        track.GetDefinition() == G4MuonPlus::MuonPlus());
}

GRPMuonPairSplittingOperation::GRPMuonPairSplittingOperation(G4String name)
    : G4VBiasingOperation(name), m_SplittingFactor(1)
{}

GRPMuonPairSplittingOperation::~GRPMuonPairSplittingOperation() {}

G4VParticleChange *GRPMuonPairSplittingOperation::ApplyFinalStateBiasing(
    const G4BiasingProcessInterface *callingProcess,
    const G4Track *track,
    const G4Step *step,
    G4bool &)
{
    constexpr G4bool verbose = false;
    G4VParticleChange *processFinalState;

    // Local pointer to the wrapped object
    G4GammaGeneralProcess *gammaGeneralProc =
        dynamic_cast<G4GammaGeneralProcess *>(
            callingProcess->GetWrappedProcess());
    if (!gammaGeneralProc)
    {
        return callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    }
    // Collect wrapped process (muon pair production) final state:
    processFinalState = gammaGeneralProc->PostStepDoIt(*track, *step);
    if (!processFinalState)
    {
        const G4String pname = gammaGeneralProc->GetProcessName();
        G4ExceptionDescription msg;
        msg << "The called process " << pname
            << " is not returning the expected type of G4VParticleChange."
            << G4endl;

        G4Exception(
            "GRPMuonPairSplittingOperation::ApplyFinalStateBiasing",
            "GRAPPA::INCORRECT_PARTICLE_CHANGE",
            G4ExceptionSeverity::JustWarning,
            msg);

        return callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    }

    // Only act if the particle is a photon
    if (track->GetDefinition() != G4Gamma::Gamma())
    {
        return processFinalState;
    }

    // Only bias GammaToMuPair (process Type=2 (electromagnetic), process
    // SubType=15)
    const G4VProcess *selectedProcess =
        step->GetPostStepPoint()->GetProcessDefinedStep();
    const G4GammaConversionToMuons *gammaToMuPair =
        dynamic_cast<const G4GammaConversionToMuons *>(selectedProcess);
    if (!gammaToMuPair)
    {
        return processFinalState;
    }

    // If no splitting and/or cross section factor requested, let the process
    // return directly its generated final state:
    const G4int crossSectionFactor = gammaToMuPair->GetCrossSecFactor();
    if (m_SplittingFactor * crossSectionFactor == 1 ||
        (m_SplittingFactor == 1 && !m_scaleMuonWeightByCrossSection))
        return processFinalState;

    // Check that the number of secondaries is two (mu+ and mu-)
    // If not warn the user and return unaltered process
    if (processFinalState->GetNumberOfSecondaries() != 2)
    {
        const G4String pname = gammaToMuPair->GetProcessName();
        const G4int nsecondaries = processFinalState->GetNumberOfSecondaries();
        G4ExceptionDescription msg;
        msg << "The process " << pname << " is counting " << nsecondaries
            << " secondaries. ";
        msg << "The expected value is 2. No biasing is performed." << G4endl;

        G4Exception(
            "GRPMuonPairSplittingOperation::ApplyFinalStateBiasing",
            "GRAPPA::INCORRECT_NUMBER_OF_SECONDARIES",
            G4ExceptionSeverity::JustWarning,
            msg);

        return processFinalState;
    }

    // For the biasing the goal is to multiply the number of muons created in
    // the pair production To do so, we first copy the primary particle (photon)
    // into the new particle change and we mark its deletion, like the original
    // process. Then we need to copy the first two secondaries from the process.
    m_ParticleChange.Initialize(*track);
    // Store gamma final state:
    m_ParticleChange.ProposeMomentumDirection(G4ThreeVector());
    m_ParticleChange.ProposeEnergy(0.);
    m_ParticleChange.ProposeTrackStatus(fStopAndKill);

    // Define the secondary's weight, taking into account both the splitting
    // factor and the cross section factor. This way we can increase the cross
    // section without worrying that the particle weight is not consistently
    // scaled
    G4double muweight = track->GetWeight() / m_SplittingFactor;
    if (m_scaleMuonWeightByCrossSection)
    {
        muweight /= crossSectionFactor;
    }

    // inform we will have m_SplittingFactor muons:
    m_ParticleChange.SetNumberOfSecondaries(2 * m_SplittingFactor);
    // inform we take care of secondaries weight (otherwise these
    // secondaries are by default given the primary weight).
    m_ParticleChange.SetSecondaryWeightByProcess(true);

    // Copy the original muons
    G4Track *firstmuontrack = processFinalState->GetSecondary(0);
    G4Track *secondmuontrack = processFinalState->GetSecondary(1);

    firstmuontrack->SetWeight(muweight);
    secondmuontrack->SetWeight(muweight);

    m_ParticleChange.AddSecondary(firstmuontrack);
    m_ParticleChange.AddSecondary(secondmuontrack);

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
        processFinalState = gammaGeneralProc->PostStepDoIt(*track, *step);

        // Check if the process was the correct one (GammaToMuPair)
        const G4int nSec = processFinalState->GetNumberOfSecondaries();
        // In case the number of secondaries is not 2, the process is definitely
        // wrong Here we make use of C++ if short-circuit
        if (nSec == 2 && IsMuon(*(processFinalState->GetSecondary(0))) &&
            IsMuon(*(processFinalState->GetSecondary(1))))
        {
            // Process is correct
            firstmuontrack = processFinalState->GetSecondary(0);
            secondmuontrack = processFinalState->GetSecondary(1);
            firstmuontrack->SetWeight(muweight);
            secondmuontrack->SetWeight(muweight);
            m_ParticleChange.AddSecondary(firstmuontrack);
            m_ParticleChange.AddSecondary(secondmuontrack);
            nCalls++;
        }
        else
        {
            // If the process is not correct, we have to manually delete the
            // created secondaries to avoid accumulating them in memory
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
        G4cout << "Generating " << m_SplittingFactor << " muons took "
               << nWhileLoopIterations << " while loop iterations" << G4endl;
    }
    return &m_ParticleChange;
}
