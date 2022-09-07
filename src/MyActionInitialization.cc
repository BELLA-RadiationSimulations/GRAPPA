/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyActionInitialization.hpp>

MyActionInitialization::MyActionInitialization(HistandNTupleManager *myanalysismanager)
    : G4VUserActionInitialization()
{
    m_HistandNTupleManager = myanalysismanager;
    m_PSMessenger = std::make_shared<MyActionInitializationMessenger>(this);
}

MyActionInitialization::~MyActionInitialization()
{

}

void MyActionInitialization::BuildForMaster() const
{
    MyRunAction *runAction = new MyRunAction(m_useGPS, m_HistandNTupleManager);
    SetUserAction(runAction);
}

void MyActionInitialization::Build() const
{
    MyPrimaryGeneratorAction *primarygeneratoraction =
        new MyPrimaryGeneratorAction(m_useGPS, m_filename);
    SetUserAction(primarygeneratoraction);

    MyRunAction *runAction = new MyRunAction(m_useGPS, m_HistandNTupleManager);
    SetUserAction(runAction);

    MyTrackingAction *trackingAction = new MyTrackingAction(m_HistandNTupleManager);
    SetUserAction(trackingAction);
}

void MyActionInitialization::ParticlesFromFile(G4bool particlesfromfile)
{
    m_useGPS = !particlesfromfile;
}

void MyActionInitialization::ParticlesFileName(G4String filename)
{
    m_filename = filename;
}

MyActionInitializationMessenger::MyActionInitializationMessenger(MyActionInitialization *myactionpointer)
{
    m_myactionpointer = myactionpointer;
    m_CMDDirectory = std::make_shared<G4UIdirectory>("/particle_source/");
    m_CMDDirectory->SetGuidance("Set of commands to pick the particle source");

    m_CMDifpartsfromfile = std::make_shared<G4UIcmdWithABool>("/particle_source/ParticlesFromFile", this);
    m_CMDifpartsfromfile->SetGuidance(" If true, particles will be read from a binary files and generated using a particle Gun ");
    m_CMDifpartsfromfile->SetParameterName("ParticlesFromFile", false, true);
    m_CMDifpartsfromfile->SetDefaultValue(false);
    m_CMDifpartsfromfile->AvailableForStates(G4State_PreInit);

    m_CMDpartsfilename = std::make_shared<G4UIcmdWithAString>("/particle_source/FileName", this);
    m_CMDpartsfilename->SetGuidance(" Binary file containing the particles to be simulated. ");
    m_CMDpartsfilename->SetParameterName("FileName", false, true);
    m_CMDpartsfilename->SetDefaultValue("InitialPhaseSpace");
    m_CMDifpartsfromfile->AvailableForStates(G4State_PreInit);
}

MyActionInitializationMessenger::~MyActionInitializationMessenger()
{

}

void MyActionInitializationMessenger::SetNewValue(
    G4UIcommand *command, G4String newValues)
{
    if (command == m_CMDifpartsfromfile.get())
    {
        G4bool flag = m_CMDifpartsfromfile->GetNewBoolValue(newValues);
        m_myactionpointer->ParticlesFromFile(flag);
    }
    if (command == m_CMDpartsfilename.get())
    {
        m_myactionpointer->ParticlesFileName(newValues);
    }
}

G4String MyActionInitializationMessenger::GetCurrentValue(G4UIcommand *command)
{
    G4String cv;

    if (command == m_CMDifpartsfromfile.get())
    {
        cv = m_CMDifpartsfromfile->ConvertToString(
            m_myactionpointer->GetIfParticlesFromFile()
        );
    }
    else if (command == m_CMDpartsfilename.get())
    {
        cv = m_myactionpointer->GetParticlesFileName();
    }

    return cv;
}