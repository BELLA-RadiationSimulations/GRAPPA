// Copyright 2021-2024
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

#include <G4Event.hh>
#include <G4RunManager.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

#include <GRPParticleSource.hpp>
#include <GRPRun.hpp>

class GRPPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    GRPPrimaryGeneratorAction(bool useGPS);
    virtual ~GRPPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event *anEvent);

    GRPParticleSource *GetSource() const { return m_PSource.get(); };

private:
    std::shared_ptr<GRPParticleSource> m_PSource;
};
