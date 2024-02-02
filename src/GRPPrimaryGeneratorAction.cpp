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

#include <GRPPrimaryGeneratorAction.hpp>

GRPPrimaryGeneratorAction::GRPPrimaryGeneratorAction(
    bool useFile)
{
    if (useFile)
    {
        m_PSource = std::make_shared<GRPParticleSourceGun>();
    }
    else
    {
        m_PSource = std::make_shared<GRPParticleSourceGPS>();
    }
}

GRPPrimaryGeneratorAction::~GRPPrimaryGeneratorAction() {}

void GRPPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    const int eventID = anEvent->GetEventID();
    auto run = static_cast<const GRPRun *>(runmanager->GetCurrentRun());
    auto container = run->GetContainer();
    m_PSource->LoadNextParticle(*container, eventID);
    m_PSource->GetGun()->GeneratePrimaryVertex(anEvent);
}
