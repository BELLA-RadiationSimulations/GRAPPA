// Copyright 2021-2025
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#pragma once

#include <memory>

#include <G4GenericMessenger.hh>
#include <G4VBiasingOperator.hh>

#include <GRPMuonPairSplittingOperation.hpp>
#include <GRPPionDecaySplittingOperation.hpp>

class GRPSplittingOperator : public G4VBiasingOperator
{
public:
    GRPSplittingOperator();
    ~GRPSplittingOperator() {};
    // -------------------------
    // Optional from base class:
    // -------------------------
    // Call at run start:
    virtual void StartRun();
    // Call at each track starting:
    // this call would be useful in case we needed to accumulate results
    // over tracking, e.g. the number of times the process was applied
    virtual void StartTracking(const G4Track *) {};

    void PrintMuonSplittingFactor()
    {
        G4cout << "The splitting factor for muon pair production is "
               << m_muonSplittingFactor << G4endl;
    }
    void PrintPionSplittingFactor()
    {
        G4cout << "The splitting factor for pion decay is "
               << m_pionSplittingFactor << G4endl;
    }
    void PrintScaleWeightByCrossSection()
    {
        G4cout << "Scale weight with artificial cross section: "
               << (m_scaleMuonWeightByCrossSection ? "true" : "false")
               << G4endl;
    }

private:
    // -----------------------------
    // Mandatory from base class:
    // -----------------------------
    // Unused:
    virtual G4VBiasingOperation *ProposeNonPhysicsBiasingOperation(
        const G4Track * /* track */,
        const G4BiasingProcessInterface * /* callingProcess */)
    {
        return 0;
    }
    virtual G4VBiasingOperation *ProposeOccurenceBiasingOperation(
        const G4Track * /* track */,
        const G4BiasingProcessInterface * /* callingProcess */)
    {
        return 0;
    }
    // Used:
    virtual G4VBiasingOperation *ProposeFinalStateBiasingOperation(
        const G4Track *, const G4BiasingProcessInterface *);

    std::shared_ptr<GRPPionDecaySplittingOperation>
        m_pionDecaySplittingOperation;
    std::shared_ptr<GRPMuonPairSplittingOperation> m_muonPairSplittingOperation;
    G4int m_muonSplittingFactor;
    G4int m_pionSplittingFactor;
    G4bool m_scaleMuonWeightByCrossSection = true;
    // Messengers to change the commands
    std::unique_ptr<G4GenericMessenger> m_pionDecayFactorMessenger;
    std::unique_ptr<G4GenericMessenger> m_muonPairFactorMessenger;
};
