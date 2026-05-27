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

#include <G4ParticleDefinition.hh>
#include <G4SDManager.hh>
#include <G4VSensitiveDetector.hh>

#include <GRPStoppingDetectorMessenger.hpp>
#include <GRPStoppingHit.hpp>

// This class defines a sensitive detector
// that registers all the particles *stopping* inside it
// where stopping is defined as particles that are killed
// (a particle that leaves the detector is never registered).
// The detector saves the stopping position, the particle energy when it was
// killed the particle ID and weight, the stopping time and the stopping
// process. This class also defines a messenger through which the user can
// specify a list of particles and processes to be sensitive to.
class GRPStoppingDetector : public G4VSensitiveDetector
{
public:
    GRPStoppingDetector(const G4String &name);
    ~GRPStoppingDetector() override;

    void Initialize(G4HCofThisEvent *HCE) override;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
    void EndOfEvent(G4HCofThisEvent *HCE) override final;

    G4bool IsSensitiveToAllParticles() const { return m_sensitiveAllParticles; }
    G4bool IsSensitiveToAllProcesses() const { return m_sensitiveAllProcesses; }
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
    G4bool m_sensitiveAllParticles = false;
    G4bool m_sensitiveAllProcesses = false;
    std::unique_ptr<GRPStoppingDetectorMessenger> m_stoppingDetectorMessenger;
};
