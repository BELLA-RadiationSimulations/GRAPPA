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

#include <GRPEmPhysics.hpp>
#include <GRPPhysicsListMessenger.hpp>
#include <GRPPhysicsList.hpp>

GRPPhysicsListMessenger::GRPPhysicsListMessenger(
    GRPPhysicsList *physics_list, GRPEmPhysics *em_physics)
    : m_physics_list(physics_list), m_em_physics(em_physics)
{

    m_physics_directory =
        std::make_unique<G4UIdirectory>("/GRAPPA/physicsList/");
    m_physics_directory->SetGuidance("Personalize the GRAPPA physics list");

    // Command to add the biasing physics
    m_add_biasing_cmd = std::make_unique<G4UIcmdWithABool>(
        "/GRAPPA/physicsList/setBiasing", this);
    m_add_biasing_cmd->SetGuidance(
        "Add or remove biasing to the physics list.");
    m_add_biasing_cmd->SetParameterName("ifbiasing", false);
    m_add_biasing_cmd->AvailableForStates(G4State_PreInit);

    // Command to change the biasing cross section
    m_set_muonxs_cmd = std::make_unique<G4UIcmdWithADouble>(
        "/GRAPPA/physicsList/setMuonPairCrossSectionFactor", this);
    m_set_muonxs_cmd->SetGuidance(
        "Change the cross section factor to the muon pair production.");
    m_set_muonxs_cmd->SetParameterName("crossSectionFactor", false);
    m_set_muonxs_cmd->AvailableForStates(G4State_PreInit);

    // Command to check the biasing cross section
    m_get_muonxs_cmd = std::make_unique<G4UIcmdWithoutParameter>(
        "/GRAPPA/physicsList/getMuonPairCrossSectionFactor", this);
    m_get_muonxs_cmd->SetGuidance(
        "Check the cross section factor to the muon pair production.");
    m_get_muonxs_cmd->AvailableForStates(
        G4State_PreInit, G4State_Init, G4State_Idle, G4State_GeomClosed);

    // Command to list the current physics list status
    m_list_physics_cmd = std::make_unique<G4UIcmdWithoutParameter>(
        "/GRAPPA/physicsList/list", this);
    m_list_physics_cmd->SetGuidance(
        "List the current status of the custom physics list");
    m_list_physics_cmd->SetToBeBroadcasted(false);
    m_list_physics_cmd->AvailableForStates(
        G4State_PreInit, G4State_Init, G4State_Idle, G4State_GeomClosed);
}

GRPPhysicsListMessenger::~GRPPhysicsListMessenger() = default;

void GRPPhysicsListMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{
    if (command == m_add_biasing_cmd.get())
    {
        if (!m_physics_list)
        {
            G4ExceptionDescription msg;
            msg << "No physics list is defined!";
            G4Exception(
                "GRPPhysicsListMessenger::SetNewValue",
                "GRAPPA::PHYSICS_LIST_UNDEFINED",
                FatalException,
                msg);
        }
        const G4bool ifaddbiasing =
            m_add_biasing_cmd->GetNewBoolValue(newValue);
        m_physics_list->SetAddBiasing(ifaddbiasing);
    }

    if (command == m_set_muonxs_cmd.get())
    {
        if (!m_em_physics)
        {
            G4ExceptionDescription msg;
            msg << "No electromagnetic physics is defined!";
            G4Exception(
                "GRPPhysicsListMessenger::SetNewValue()",
                "GRAPPA::PHYSICS_LIST_UNDEFINED",
                FatalException,
                msg);
        }
        const G4double newxs = m_set_muonxs_cmd->GetNewDoubleValue(newValue);
        m_em_physics->SetMuPairCrossSection(newxs);
    }
    if (command == m_get_muonxs_cmd.get())
    {
        if (!m_em_physics)
        {
            G4ExceptionDescription msg;
            msg << "No electromagnetic physics is defined!";
            G4Exception(
                "GRPPhysicsListMessenger::SetNewValue()",
                "GRAPPA::PHYSICS_LIST_UNDEFINED",
                FatalException,
                msg);
        }
        const G4double muonxs = m_em_physics->GetMuPairCrossSection();
        G4cout << "Muon production is increased artificially by a factor "
               << muonxs << G4endl;
    }
    if (command == m_list_physics_cmd.get())
    {
        if (!m_physics_list)
        {
            G4ExceptionDescription msg;
            msg << "No physics list is defined!";
            G4Exception(
                "GRPPhysicsListMessenger::SetNewValue()",
                "GRAPPA::PHYSICS_LIST_UNDEFINED",
                FatalException,
                msg);
        }
        if (!m_em_physics)
        {
            G4ExceptionDescription msg;
            msg << "No electromagnetic physics is defined!";
            G4Exception(
                "GRPPhysicsListMessenger::SetNewValue()",
                "GRAPPA::PHYSICS_LIST_UNDEFINED",
                FatalException,
                msg);
        }
        const G4bool ifaddbiasing = m_physics_list->GetAddBiasing();
        const G4double muonxs = m_em_physics->GetMuPairCrossSection();

        G4cout
            << "GRAPPA is using a custom physics list. The list components are:"
            << G4endl;
        G4cout << G4endl;
        G4cout << "- Electromagnetic list (option 0) including muon pair "
                  "production"
               << G4endl;
        if (muonxs > 1)
        {
            G4cout
                << "\t(Muon production is increased artificially by a factor "
                << muonxs << ")" << G4endl;
        }
        // We keep this commented for now, need to investigate if list is broken
        // G4cout << "- G4MuonicAtomDecayPhysics for decay of muonic atoms"
        //        << G4endl;
        G4cout << "- G4DecayPhysics for decay of general ions" << G4endl;
        G4cout << "- G4HadronElasticPhysicsHP high precision elastic "
                  "scattering model"
               << G4endl;
        G4cout << "- G4StoppingPhysics to model particle stopping, including "
                  "muon capture"
               << G4endl;
        G4cout << "- G4IonPhysics to model generic ions" << G4endl;
        G4cout << "- G4NeutronTrackingCut to enable tracking cuts on neutrons"
               << G4endl;
        G4cout << "- G4StepLimiterPhysics to enable general production cuts"
               << G4endl;
        if (ifaddbiasing)
        {
            G4cout << "- Biasing physics for muon production via Bethe-Heitler "
                      "and pion decay"
                   << G4endl;
            G4cout << "  Both biasings are performed via production splitting."
                   << G4endl;
            G4cout << "  More information about the biasing can be found in "
                      "the /GRAPPA/biasing/ commands."
                   << G4endl;
        }
    }
}
