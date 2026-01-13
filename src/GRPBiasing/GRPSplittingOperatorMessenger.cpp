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

#include <G4RunManager.hh>

#include <GRPPhysicsList.hpp>
#include <GRPSplittingOperator.hpp>
#include <GRPSplittingOperatorMessenger.hpp>

GRPSplittingOperatorMessenger::GRPSplittingOperatorMessenger(
    GRPSplittingOperator *splitting_operator)
    : G4UImessenger(), m_splitting_operator(splitting_operator)
{
    m_splitting_directory = std::make_unique<G4UIdirectory>("/GRAPPA/biasing/");
    m_splitting_directory->SetGuidance("Personalize GRAPPA biasing operation");

    m_splitting_directory_pions =
        std::make_unique<G4UIdirectory>("/GRAPPA/biasing/pionDecay/");
    m_splitting_directory_pions->SetGuidance(
        "Change settings of the pion decay splitting biasing");

    m_splitting_directory_muons =
        std::make_unique<G4UIdirectory>("/GRAPPA/biasing/muonPair/");
    m_splitting_directory_muons->SetGuidance(
        "Change settings of the pair production generation of muon biasing");

    // Commands for muon biasing
    m_set_muon_factor = std::make_unique<G4UIcmdWithAnInteger>(
        "/GRAPPA/biasing/muonPair/setSplittingFactor", this);
    m_set_muon_factor->SetGuidance(
        "Define the muon pair production splitting factor");
    m_set_muon_factor->AvailableForStates(G4State_Idle);

    m_get_muon_factor = std::make_unique<G4UIcmdWithoutParameter>(
        "/GRAPPA/biasing/muonPair/getSplittingFactor", this);
    m_get_muon_factor->SetGuidance(
        "Print the muon pair production splitting factor");
    m_get_muon_factor->SetToBeBroadcasted(false);
    m_get_muon_factor->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);

    m_set_muon_weight_scale = std::make_unique<G4UIcmdWithABool>(
        "/GRAPPA/biasing/muonPair/setScaleMuonWeigthWithCrossSection", this);
    m_set_muon_weight_scale->SetGuidance(
        "Choose if the muon weight must also be scaled by the artificial "
        "cross section increase (default is true).");
    m_set_muon_weight_scale->AvailableForStates(G4State_Idle);

    m_get_muon_weight_scale = std::make_unique<G4UIcmdWithoutParameter>(
        "/GRAPPA/biasing/muonPair/getScaleMuonWeigthWithCrossSection", this);
    m_get_muon_weight_scale->SetGuidance(
        "Print if the muon weight is scaled by the artificial "
        "cross section increase.");
    m_get_muon_weight_scale->SetToBeBroadcasted(false);
    m_get_muon_weight_scale->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);

    // Commands for pion biasing
    m_set_pion_factor = std::make_unique<G4UIcmdWithAnInteger>(
        "/GRAPPA/biasing/pionDecay/setSplittingFactor", this);
    m_set_pion_factor->SetGuidance("Define the pion decay splitting factor");
    m_set_pion_factor->AvailableForStates(G4State_Idle);

    m_get_pion_factor = std::make_unique<G4UIcmdWithoutParameter>(
        "/GRAPPA/biasing/pionDecay/getSplittingFactor", this);
    m_get_pion_factor->SetGuidance("Print the pion decay splitting factor");
    m_get_pion_factor->SetToBeBroadcasted(false);
    m_get_pion_factor->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);
}

GRPSplittingOperatorMessenger::~GRPSplittingOperatorMessenger() = default;

void GRPSplittingOperatorMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{
    G4RunManager *rman = G4RunManager::GetRunManager();
    const GRPPhysicsList *custom_physics_list =
        dynamic_cast<const GRPPhysicsList *>(rman->GetUserPhysicsList());
    const G4bool isbiasingactive = custom_physics_list->GetAddBiasing();
    const G4bool enableprint = isbiasingactive;

    if (!isbiasingactive)
    {
        G4ExceptionDescription msg;
        msg << "Biasing is not active. Commands will be skipped.";
        G4Exception(
            "GRPSplittingOperatorMessenger::SetNewValue()",
            "GRAPPA::BIASING_NOT_ACTIVE",
            JustWarning,
            msg);
    }
    if (command == m_set_muon_factor.get())
    {
        if (!m_splitting_operator)
        {
            G4ExceptionDescription msg;
            msg << "No splitting operator is defined!";
            G4Exception(
                "GRPSplittingOperatorMessenger::SetNewValue()",
                "GRAPPA::SPLITTING_OPERATOR_UNDEFINED",
                FatalException,
                msg);
        }
        const G4int muon_factor = m_set_muon_factor->GetNewIntValue(newValue);
        m_splitting_operator->SetMuonSplittingFactor(muon_factor);
    }
    if (command == m_get_muon_factor.get())
    {
        if (!m_splitting_operator)
        {
            G4ExceptionDescription msg;
            msg << "No splitting operator is defined!";
            G4Exception(
                "GRPSplittingOperatorMessenger::SetNewValue()",
                "GRAPPA::SPLITTING_OPERATOR_UNDEFINED",
                FatalException,
                msg);
        }
        m_splitting_operator->PrintMuonSplittingFactor(enableprint);
    }
    if (command == m_set_muon_weight_scale.get())
    {
        if (!m_splitting_operator)
        {
            G4ExceptionDescription msg;
            msg << "No splitting operator is defined!";
            G4Exception(
                "GRPSplittingOperatorMessenger::SetNewValue()",
                "GRAPPA::SPLITTING_OPERATOR_UNDEFINED",
                FatalException,
                msg);
        }
        const G4bool muon_scale_with_weight =
            m_set_muon_weight_scale->GetNewBoolValue(newValue);
        m_splitting_operator->SetScaleMuonWeightByCrossSection(
            muon_scale_with_weight);
    }
    if (command == m_get_muon_weight_scale.get())
    {
        if (!m_splitting_operator)
        {
            G4ExceptionDescription msg;
            msg << "No splitting operator is defined!";
            G4Exception(
                "GRPSplittingOperatorMessenger::SetNewValue()",
                "GRAPPA::SPLITTING_OPERATOR_UNDEFINED",
                FatalException,
                msg);
        }
        m_splitting_operator->PrintScaleWeightByCrossSection(enableprint);
    }
    if (command == m_set_pion_factor.get())
    {
        if (!m_splitting_operator)
        {
            G4ExceptionDescription msg;
            msg << "No splitting operator is defined!";
            G4Exception(
                "GRPSplittingOperatorMessenger::SetNewValue()",
                "GRAPPA::SPLITTING_OPERATOR_UNDEFINED",
                FatalException,
                msg);
        }
        const G4int pion_factor = m_set_pion_factor->GetNewIntValue(newValue);
        m_splitting_operator->SetPionSplittingFactor(pion_factor);
    }
    if (command == m_get_pion_factor.get())
    {
        if (!m_splitting_operator)
        {
            G4ExceptionDescription msg;
            msg << "No splitting operator is defined!";
            G4Exception(
                "GRPSplittingOperatorMessenger::SetNewValue()",
                "GRAPPA::SPLITTING_OPERATOR_UNDEFINED",
                FatalException,
                msg);
        }
        m_splitting_operator->PrintPionSplittingFactor(enableprint);
    }
}
