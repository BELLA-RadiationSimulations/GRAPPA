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

class GRPEmPhysics : public G4VPhysicsConstructor
{
public:
    explicit GRPEmPhysics(G4int ver = 1, const G4String &name = "");

    ~GRPEmPhysics() override;

    void ConstructParticle() override;
    void ConstructProcess() override;

    G4double GetMuPairCrossSection() const { return m_gmumuxsfactor; }
    void SetMuPairCrossSection(G4double newcrosssection);

private:
    G4double m_gmumuxsfactor = 1;
};
