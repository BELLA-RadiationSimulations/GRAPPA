/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyPrimaryGeneratorAction.hpp>

using CLHEP::GeV;
using CLHEP::MeV;
using CLHEP::mm;

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction(bool useGPS, G4String filename)
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

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    int eventID = anEvent->GetEventID();

    m_PSource->LoadNextParticle(eventID);
    m_PSource->GetGun()->GeneratePrimaryVertex(anEvent);
}
