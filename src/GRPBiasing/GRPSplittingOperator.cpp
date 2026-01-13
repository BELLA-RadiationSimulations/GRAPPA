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
#include <G4Decay.hh>
#include <G4PionMinus.hh>
#include <G4PionPlus.hh>
#include <G4ProcessManager.hh>
#include <G4RunManager.hh>

#include <GRPPhysicsList.hpp>
#include <GRPSplittingOperator.hpp>

GRPSplittingOperator::GRPSplittingOperator()
    : G4VBiasingOperator("SplittingOperator")
    , m_muonSplittingFactor(1)
    , m_pionSplittingFactor(1)
{

    m_pionDecaySplittingOperation =
        std::make_shared<GRPPionDecaySplittingOperation>(
            "PionDecaySplittingOperation");
    m_muonPairSplittingOperation =
        std::make_shared<GRPMuonPairSplittingOperation>(
            "MuonPairSplittingOperation");

    // Define the biasing messenger
    m_splitting_messenger =
        std::make_unique<GRPSplittingOperatorMessenger>(this);
}

void GRPSplittingOperator::StartRun()
{
    G4RunManager *rman = G4RunManager::GetRunManager();
    const GRPPhysicsList *custom_physics_list =
        dynamic_cast<const GRPPhysicsList *>(rman->GetUserPhysicsList());
    const G4bool isbiasingactive = custom_physics_list->GetAddBiasing();

    // We guard these commands and only execute them if biasing is active.
    // For the purpose of code functioning, this is actually not needed as, if
    // biasing is inactive it is not called during the processing and the
    // commands here are ineffective. However, it's important to guard the print
    // statements so that users do not get confused by reading that biasing is
    // working when it's not
    if (isbiasingactive)
    {
        m_pionDecaySplittingOperation->SetSplittingFactor(
            m_pionSplittingFactor);
        m_muonPairSplittingOperation->SetSplittingFactor(m_muonSplittingFactor);
        m_muonPairSplittingOperation->SetScaleWeightByCrossSection(
            m_scaleMuonWeightByCrossSection);
        if (m_pionSplittingFactor > 1)
        {
            G4cout << GetName()
                   << " : starting run with pion decay splitting factor = "
                   << m_pionSplittingFactor;
            G4cout << " . " << G4endl;
        }
        if (m_muonSplittingFactor > 1 || m_scaleMuonWeightByCrossSection)
        {
            G4cout << GetName()
                   << " : starting run with muon pair production splitting "
                      "factor = "
                   << m_muonSplittingFactor << ". ";
            if (m_scaleMuonWeightByCrossSection)
            {
                G4cout
                    << "The weight is scaled by the artificial cross section "
                       "factor."
                    << G4endl;
            }
            else
            {
                G4cout << "The weight is not scaled by the artificial cross "
                          "section factor."
                       << G4endl;
            }
        }
    }
}

G4VBiasingOperation *GRPSplittingOperator::ProposeFinalStateBiasingOperation(
    const G4Track *track, const G4BiasingProcessInterface *)
{
    G4ParticleDefinition *pDef = track->GetDefinition();
    if (pDef == G4Gamma::Gamma())
    {
        return m_muonPairSplittingOperation.get();
    }
    else if (pDef == G4PionMinus::PionMinus() || pDef == G4PionPlus::PionPlus())
    {
        return m_pionDecaySplittingOperation.get();
    }
    else
    {
        G4ExceptionDescription msg;
        msg << "WARNING: In the biasing operation only gamma and pions should "
               "be "
               "allowed.";
        G4Exception(
            "GRPSplittingOperator::ProposeFinalStateBiasingOperation",
            "GRAPPA::INCORRECT_PARTICLE_IN_BIASING",
            G4ExceptionSeverity::JustWarning,
            msg);
        return nullptr;
    }
}
