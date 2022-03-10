/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyPrimaryGeneratorAction.hpp>

using CLHEP::MeV;
using CLHEP::GeV;
using CLHEP::mm;

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
{
    m_PSource = std::make_shared<ParticleSource>();
    
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{

}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    m_PSource->GetGun()->GeneratePrimaryVertex(anEvent);
}
