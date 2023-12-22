// Copyright 2021-2023
//
// Authors:
// Stanimir Kisyov, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <GRPActionInitialization.hpp>

GRPActionInitialization::GRPActionInitialization(
    HistandNTupleManager *myanalysismanager)
{
    m_HistandNTupleManager = myanalysismanager;
    m_PSMessenger = std::make_shared<GRPActionInitializationMessenger>(this);
}

GRPActionInitialization::~GRPActionInitialization() {}

void GRPActionInitialization::BuildForMaster() const
{
    GRPRunAction *runAction =
        new GRPRunAction(m_useGPS, m_HistandNTupleManager);
    SetUserAction(runAction);
}

void GRPActionInitialization::Build() const
{
    GRPPrimaryGeneratorAction *primarygeneratoraction =
        new GRPPrimaryGeneratorAction(m_useGPS, m_filename);
    SetUserAction(primarygeneratoraction);

    GRPRunAction *runAction =
        new GRPRunAction(m_useGPS, m_HistandNTupleManager);
    SetUserAction(runAction);

    GRPTrackingAction *trackingAction =
        new GRPTrackingAction(m_HistandNTupleManager);
    SetUserAction(trackingAction);
}

void GRPActionInitialization::ParticlesFromFile(G4bool particlesfromfile)
{
    m_useGPS = !particlesfromfile;
}

void GRPActionInitialization::ParticlesFileName(G4String filename)
{
    m_filename = filename;
}

GRPActionInitializationMessenger::GRPActionInitializationMessenger(
    GRPActionInitialization *myactionpointer)
{
    m_myactionpointer = myactionpointer;
    m_CMDDirectory = std::make_shared<G4UIdirectory>("/particle_source/");
    m_CMDDirectory->SetGuidance("Set of commands to pick the particle source");

    m_CMDifpartsfromfile = std::make_shared<G4UIcmdWithABool>(
        "/particle_source/ParticlesFromFile", this);
    m_CMDifpartsfromfile->SetGuidance(
        " If true, particles will be read from a binary files and generated "
        "using a particle Gun ");
    m_CMDifpartsfromfile->SetParameterName("ParticlesFromFile", false, true);
    m_CMDifpartsfromfile->SetDefaultValue(false);
    m_CMDifpartsfromfile->AvailableForStates(G4State_PreInit);

    m_CMDpartsfilename =
        std::make_shared<G4UIcmdWithAString>("/particle_source/FileName", this);
    m_CMDpartsfilename->SetGuidance(
        " Binary file containing the particles to be simulated. ");
    m_CMDpartsfilename->SetParameterName("FileName", false, true);
    m_CMDpartsfilename->SetDefaultValue("InitialPhaseSpace");
    m_CMDifpartsfromfile->AvailableForStates(G4State_PreInit);
}

GRPActionInitializationMessenger::~GRPActionInitializationMessenger() {}

void GRPActionInitializationMessenger::SetNewValue(
    G4UIcommand *command, G4String newValues)
{
    if (command == m_CMDifpartsfromfile.get())
    {
        const G4bool flag = m_CMDifpartsfromfile->GetNewBoolValue(newValues);
        m_myactionpointer->ParticlesFromFile(flag);
    }
    if (command == m_CMDpartsfilename.get())
    {
        m_myactionpointer->ParticlesFileName(newValues);
    }
}

G4String GRPActionInitializationMessenger::GetCurrentValue(G4UIcommand *command)
{
    G4String cv;

    if (command == m_CMDifpartsfromfile.get())
    {
        cv = m_CMDifpartsfromfile->ConvertToString(
            m_myactionpointer->GetIfParticlesFromFile());
    }
    else if (command == m_CMDpartsfilename.get())
    {
        cv = m_myactionpointer->GetParticlesFileName();
    }

    return cv;
}
