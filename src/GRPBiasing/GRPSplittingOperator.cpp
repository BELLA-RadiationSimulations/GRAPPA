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
#include <G4Decay.hh>
#include <G4EmProcessSubType.hh>
#include <G4PionMinus.hh>
#include <G4PionPlus.hh>
#include <G4ProcessManager.hh>

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

    m_pionDecayFactorMessenger = std::make_unique<G4GenericMessenger>(
        this,
        "/GRAPPA/biasing/pionDecay/",
        "Biasing control for the pion decay");
    G4GenericMessenger::Command &setPionSplittingFactorCmd =
        m_pionDecayFactorMessenger->DeclareProperty(
            "setSplittingFactor",
            m_pionSplittingFactor,
            "Define the pion decay splitting factor");
    setPionSplittingFactorCmd.SetStates(G4State_Idle);
    G4GenericMessenger::Command &getPionSplittingFactorCmd =
        m_pionDecayFactorMessenger->DeclareMethod(
            "getSplittingFactor",
            &GRPSplittingOperator::PrintPionSplittingFactor,
            "Print the pion decay splitting factor");
    getPionSplittingFactorCmd.SetStates(G4State_Idle);

    m_muonPairFactorMessenger = std::make_unique<G4GenericMessenger>(
        this,
        "/GRAPPA/biasing/muonPair/",
        "Biasing control for the muon pair production");
    G4GenericMessenger::Command &setMuonSplittingFactorCmd =
        m_muonPairFactorMessenger->DeclareProperty(
            "setSplittingFactor",
            m_muonSplittingFactor,
            "Define the muon pair production splitting factor");
    setMuonSplittingFactorCmd.SetStates(G4State_Idle);
    G4GenericMessenger::Command &getMuonSplittingFactorCmd =
        m_muonPairFactorMessenger->DeclareMethod(
            "getSplittingFactor",
            &GRPSplittingOperator::PrintMuonSplittingFactor,
            "Print the muon pair production splitting factor");
    getMuonSplittingFactorCmd.SetStates(G4State_Idle);
    G4GenericMessenger::Command &setScaleMuonWeightWithCrossSection =
        m_muonPairFactorMessenger->DeclareProperty(
            "setScaleMuonWeigthWithCrossSection",
            m_scaleMuonWeightByCrossSection,
            "Choose if the muon weight must also be scaled by the artificial "
            "cross section increase (default is true).");
    setScaleMuonWeightWithCrossSection.SetStates(G4State_Idle);
    G4GenericMessenger::Command &getScaleMuonWeightWithCrossSection =
        m_muonPairFactorMessenger->DeclareMethod(
            "getScaleMuonWeigthWithCrossSection",
            &GRPSplittingOperator::PrintScaleWeightByCrossSection,
            "Print if the muon weight is scaled by the artificial "
            "cross section increase.");
    getScaleMuonWeightWithCrossSection.SetStates(G4State_Idle);
}

void GRPSplittingOperator::StartRun()
{
    m_pionDecaySplittingOperation->SetSplittingFactor(m_pionSplittingFactor);
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
        G4cout
            << GetName()
            << " : starting run with muon pair production splitting factor = "
            << m_muonSplittingFactor << ". ";
        if (m_scaleMuonWeightByCrossSection)
        {
            G4cout << "The weight is scaled by the artificial cross section "
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
