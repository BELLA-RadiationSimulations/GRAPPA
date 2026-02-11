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

#include <G4Tokenizer.hh>

#include <GRPStoppingDetector.hpp>
#include <GRPStoppingDetectorMessenger.hpp>

GRPStoppingDetectorMessenger::GRPStoppingDetectorMessenger(
    GRPStoppingDetector *stoppingDetector)
    : m_stoppingDetector(stoppingDetector)
{

    m_stoppingDetectorDirectory =
        std::make_unique<G4UIdirectory>("/GRAPPA/stoppingDetector/");
    m_stoppingDetectorDirectory->SetGuidance(
        "Personalize the GRAPPA stopping detector");

    m_stoppingDetectorParticleList = std::make_unique<G4UIcmdWithAString>(
        "/GRAPPA/stoppingDetector/setParticleList", this);
    m_stoppingDetectorParticleList->SetGuidance(
        "Set the list of particles to which the detector is sensitive");
    m_stoppingDetectorParticleList->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);

    m_stoppingDetectorProcessList = std::make_unique<G4UIcmdWithAString>(
        "/GRAPPA/stoppingDetector/setProcessList", this);
    m_stoppingDetectorProcessList->SetGuidance(
        "Set the list of processes recorded by the detector");
    m_stoppingDetectorProcessList->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);

    m_stoppingDetectorListSensitiveParticles =
        std::make_unique<G4UIcmdWithoutParameter>(
            "/GRAPPA/stoppingDetector/getParticleList", this);
    m_stoppingDetectorListSensitiveParticles->SetGuidance(
        "List of particles to which the detector is sensitive");
    m_stoppingDetectorListSensitiveParticles->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);

    m_stoppingDetectorListSensitiveProcesses =
        std::make_unique<G4UIcmdWithoutParameter>(
            "/GRAPPA/stoppingDetector/getProcessList", this);
    m_stoppingDetectorListSensitiveProcesses->SetGuidance(
        "List of processes recorded by the detector");
    m_stoppingDetectorListSensitiveProcesses->AvailableForStates(
        G4State_Init, G4State_Idle, G4State_GeomClosed);
}

GRPStoppingDetectorMessenger::~GRPStoppingDetectorMessenger() = default;

void GRPStoppingDetectorMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{
    if (!m_stoppingDetector)
    {
        G4ExceptionDescription msg;
        msg << "No stopping detector is defined!";
        G4Exception(
            "GRPStoppingDetectorMessenger::SetNewValue",
            "GRAPPA::STOPPING_DETECTOR_UNDEFINED",
            FatalException,
            msg);
    }
    if (command == m_stoppingDetectorParticleList.get())
    {
        // Using the provided Geant4 tokenizer to extract the values from the
        // string The () operator on the tokenizer returns the next string
        G4Tokenizer tokenizer(newValue);
        std::vector<G4String> token_vector;
        G4String str;
        while (!(str = tokenizer()).empty())
        {
            token_vector.push_back(str);
        }
        m_stoppingDetector->SetSensitiveParticles(token_vector);
    }
    if (command == m_stoppingDetectorProcessList.get())
    {
        // Using the provided Geant4 tokenizer to extract the values from the
        // string The () operator on the tokenizer returns the next string
        G4Tokenizer tokenizer(newValue);
        std::vector<G4String> token_vector;
        G4String str;
        while (!(str = tokenizer()).empty())
        {
            token_vector.push_back(str);
        }
        m_stoppingDetector->SetSensitiveProcesses(token_vector);
    }
    if (command == m_stoppingDetectorListSensitiveParticles.get())
    {
        G4cout << "List of particles for detector "
               << m_stoppingDetector->GetName() << ": ";
        if (m_stoppingDetector->IsSensitiveToAllParticles())
        {
            G4cout << "all" << " ";
        }
        else
        {
            const std::vector<G4ParticleDefinition *> particleVector =
                m_stoppingDetector->GetSensitiveParticles();
            for (G4ParticleDefinition *aParticle : particleVector)
            {
                G4cout << aParticle->GetParticleName() << " ";
            }
        }
        G4cout << G4endl;
    }
    if (command == m_stoppingDetectorListSensitiveProcesses.get())
    {
        const std::vector<G4String> processVector =
            m_stoppingDetector->GetSensitiveProcesses();
        G4cout << "List of processes for detector "
               << m_stoppingDetector->GetName() << ": ";
        for (const G4String &aProcess : processVector)
        {
            G4cout << aProcess << " ";
        }
        G4cout << G4endl;
    }
}
