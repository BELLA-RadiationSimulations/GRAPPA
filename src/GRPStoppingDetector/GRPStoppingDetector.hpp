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

#pragma once

#include <memory>

#include <G4ParticleDefinition.hh>
#include <G4SDManager.hh>
#include <G4VSensitiveDetector.hh>

#include <GRPStoppingDetectorMessenger.hpp>
#include <GRPStoppingHit.hpp>

class GRPStoppingDetector : public G4VSensitiveDetector
{
public:
    GRPStoppingDetector(const G4String &name);
    ~GRPStoppingDetector() override;

    void Initialize(G4HCofThisEvent *HCE) override;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
    void EndOfEvent(G4HCofThisEvent *HCE) override final;

    std::vector<G4ParticleDefinition *> GetSensitiveParticles() const
    {
        return m_sensitiveParticles;
    }
    std::vector<G4String> GetSensitiveProcesses() const
    {
        return m_sensitiveProcesses;
    }
    void SetSensitiveParticles(std::vector<G4String> sensitiveParticles);
    void SetSensitiveProcesses(std::vector<G4String> sensitiveProcesses);

private:
    G4int m_hcID = -1;
    G4int m_stoppedPositionNtupleNumber = 0;
    GRPStoppingHitCollection *m_HitCollection = nullptr;
    std::vector<G4ParticleDefinition *> m_sensitiveParticles;
    std::vector<G4String> m_sensitiveProcesses;
    std::unique_ptr<GRPStoppingDetectorMessenger> m_stoppingDetectorMessenger;
};
