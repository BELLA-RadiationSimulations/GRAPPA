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

#pragma once

#include <memory>

#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

// Forward declaration of the physics list to enable access in this class
class GRPStoppingDetector;

class GRPStoppingDetectorMessenger : public G4UImessenger
{
public:
    explicit GRPStoppingDetectorMessenger(GRPStoppingDetector *);
    ~GRPStoppingDetectorMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    GRPStoppingDetector *m_stoppingDetector;

    std::unique_ptr<G4UIdirectory> m_stoppingDetectorDirectory;
    std::unique_ptr<G4UIcmdWithAString> m_stoppingDetectorParticleList;
    std::unique_ptr<G4UIcmdWithAString> m_stoppingDetectorProcessList;
    std::unique_ptr<G4UIcmdWithoutParameter>
        m_stoppingDetectorListSensitiveParticles;
    std::unique_ptr<G4UIcmdWithoutParameter>
        m_stoppingDetectorListSensitiveProcesses;
};
