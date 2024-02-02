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

#include <GRPParticleSource.hpp>

GRPParticleSource::GRPParticleSource() {}

GRPParticleSource::~GRPParticleSource() {}

GRPParticleSourceGPS::GRPParticleSourceGPS()
{
    m_GPS = std::make_unique<G4GeneralParticleSource>();
    m_GPS->SetVerbosity(0);
    m_GPS->SetParticlePosition(G4ThreeVector());
}

GRPParticleSourceGPS::~GRPParticleSourceGPS() { }

GRPParticleSourceGun::GRPParticleSourceGun()
{
    m_Gun = std::make_unique<G4ParticleGun>();
}

GRPParticleSourceGun::~GRPParticleSourceGun() { }

void GRPParticleSourceGun::LoadNextParticle(const GRPParticleContainer &mycontainer, G4int eventNumber)
{
    if (mycontainer.GetNParticlesInFile() <= 0)
        return;

    auto quantities =
        mycontainer.GetNextPositionEnergyMomentumdirection(eventNumber);

    const G4ThreeVector NextPosition = std::get<0>(quantities);
    const G4double Kenergy = std::get<1>(quantities);
    const G4ThreeVector momentumdirection = std::get<2>(quantities);
    m_Gun->SetParticlePosition(NextPosition);
    m_Gun->SetParticleEnergy(Kenergy);
    m_Gun->SetParticleMomentumDirection(momentumdirection);
}
