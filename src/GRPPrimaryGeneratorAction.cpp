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

using CLHEP::GeV;
using CLHEP::MeV;
using CLHEP::mm;

GRPPrimaryGeneratorAction::GRPPrimaryGeneratorAction(bool useGPS, G4String filename)
{
    if (useGPS)
    {
        m_PSource = std::make_shared<ParticleSourceGPS>();
    }
    else
    {
        m_PSource = std::make_shared<ParticleSourceGun>(filename);
    }
}

GRPPrimaryGeneratorAction::~GRPPrimaryGeneratorAction()
{
}

void GRPPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    int eventID = anEvent->GetEventID();

    m_PSource->LoadNextParticle(eventID);
    m_PSource->GetGun()->GeneratePrimaryVertex(anEvent);
}
