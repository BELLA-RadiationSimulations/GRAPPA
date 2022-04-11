/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <G4Threading.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UIMessenger.hh>
#include <G4VUserActionInitialization.hh>

#include <MyActionInitialization.hpp>
#include <MyDetectorConstruction.hpp>
#include <MyPrimaryGeneratorAction.hpp>
#include <MyTrackingAction.hpp>
#include <MyRunAction.hpp>

class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization(HistandNTupleManager *myanalysismanager);
    virtual ~MyActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
    G4bool GetIfParticlesFromFile() const { return !m_useGPS; }
    G4String GetParticlesFileName() { return m_filename; }
    void ParticlesFromFile(G4bool particlesfromfile);
    void ParticlesFileName(G4String filename);

private:
    HistandNTupleManager *m_HistandNTupleManager;
    // Flag that can be changed if one wants to use particle gun instead of a GPS
    // MyActionInitialization is constructed when the program starts,
    // but it is only built (MyActionInitialization::Build() called) when run is initialized.
    G4bool m_useGPS = true;
    G4String m_filename = "InitialPhaseSpace.dat";
    // Pointer to the generic messengers
    std::shared_ptr<G4UImessenger> m_PSMessenger;
};

class MyActionInitializationMessenger : public G4UImessenger
{
public:
    MyActionInitializationMessenger(MyActionInitialization *myactionpointer);
    virtual ~MyActionInitializationMessenger();
    void SetNewValue(G4UIcommand *command, G4String newValues);
    G4String GetCurrentValue(G4UIcommand *command);

private:
    MyActionInitialization *m_myactionpointer;
    std::shared_ptr<G4UIdirectory> m_CMDDirectory;
    std::shared_ptr<G4UIcmdWithABool> m_CMDifpartsfromfile;
    std::shared_ptr<G4UIcmdWithAString> m_CMDpartsfilename;
};