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

#include <G4VPhysicsConstructor.hh>

// This class is a simplified copy of G4EmExtraPhysics
// where we implemented only the default processes.
// The reason is that this basic class also implemented a messenger
// which we don't want because we need to take control of the
// gamma conversion to muon process for biasing reasons
class GRPEmExtraPhysics : public G4VPhysicsConstructor
{
public:
    GRPEmExtraPhysics(G4int ver = 0);

    ~GRPEmExtraPhysics() override;

    void ConstructParticle() override;
    void ConstructProcess() override;

private:
    void ConstructGammaElectroNuclear();
    G4double m_GNLowEnergyLimit;
    G4int m_verbose;
};
