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

#pragma once

#include <G4Threading.hh>

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>
#include <G4VUserActionInitialization.hh>

#include <GRPDetectorConstruction.hpp>
#include <GRPPrimaryGeneratorAction.hpp>
#include <GRPTrackingAction.hpp>
#include <GRPRunAction.hpp>

class GRPActionInitialization : public G4VUserActionInitialization
{
public:
    GRPActionInitialization(HistandNTupleManager *myanalysismanager);
    virtual ~GRPActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
    G4bool GetIfParticlesFromFile() const { return !m_useGPS; }
    G4String GetParticlesFileName() { return m_filename; }
    void ParticlesFromFile(G4bool particlesfromfile);
    void ParticlesFileName(G4String filename);

private:
    HistandNTupleManager *m_HistandNTupleManager;
    // Flag that can be changed if one wants to use particle gun instead of a
    // GPS GRPActionInitialization is constructed when the program starts, but
    // it is only built (GRPActionInitialization::Build() called) when run is
    // initialized.
    G4bool m_useGPS = true;
    G4String m_filename = "InitialPhaseSpace.dat";
    // Pointer to the generic messengers
    std::shared_ptr<G4UImessenger> m_PSMessenger;
};

class GRPActionInitializationMessenger : public G4UImessenger
{
public:
    GRPActionInitializationMessenger(GRPActionInitialization *myactionpointer);
    virtual ~GRPActionInitializationMessenger();
    void SetNewValue(G4UIcommand *command, G4String newValues);
    G4String GetCurrentValue(G4UIcommand *command);

private:
    GRPActionInitialization *m_myactionpointer;
    std::shared_ptr<G4UIdirectory> m_CMDDirectory;
    std::shared_ptr<G4UIcmdWithABool> m_CMDifpartsfromfile;
    std::shared_ptr<G4UIcmdWithAString> m_CMDpartsfilename;
};
