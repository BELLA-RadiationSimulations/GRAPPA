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

#include <G4Run.hh>

#include <GRPParticleContainer.hpp>

class GRPRun : public G4Run
{
public:
    GRPRun(){};
    ~GRPRun() override = default;

    void RecordEvent(const G4Event *anEvent) override;
    void Merge(const G4Run *aRun) override;

    GRPParticleContainer *GetContainer() const { return m_myParticleContainer; }
    void SetContainer(GRPParticleContainer *mycontainerptr)
    {
        m_myParticleContainer = mycontainerptr;
    }

private:
    GRPParticleContainer *m_myParticleContainer;
};
