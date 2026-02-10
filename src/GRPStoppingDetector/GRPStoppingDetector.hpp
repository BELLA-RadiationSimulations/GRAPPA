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

#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>

#include <GRPStoppingHit.hpp>

class GRPStoppingDetector : public G4VSensitiveDetector
{
public:
    GRPStoppingDetector(const G4String &name);
    ~GRPStoppingDetector() override;

    void Initialize(G4HCofThisEvent *HCE) override;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
    void EndOfEvent(G4HCofThisEvent *HCE) override final;

private:
    G4int m_hcID = 0;
    G4int m_stoppedPositionNtupleNumber = 0;
    GRPStoppingHitCollection *m_HitCollection = nullptr;
};
