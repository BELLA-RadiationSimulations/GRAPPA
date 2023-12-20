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

ParticleSource::ParticleSource() {}

ParticleSource::~ParticleSource() {}

void ParticleSource::SetPart(G4ParticleDefinition *part)
{
    m_part = part;
    m_particle_mass = m_part->GetPDGMass();
    m_particle_mass_squared = std::pow(m_particle_mass, 2);
}

ParticleSourceGPS::ParticleSourceGPS()
{
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    SetPart(particle_table->FindParticle("e-"));
    m_GPS = std::make_shared<G4GeneralParticleSource>();
    m_GPS->SetParticleDefinition(m_part);
    m_GPS->SetVerbosity(0);
}

ParticleSourceGPS::~ParticleSourceGPS() { m_part = nullptr; }

ParticleSourceGun::ParticleSourceGun(G4String filename)
{
#pragma message(__FILE__ "(" MAKE_STR(                                         \
    __LINE__) "): \
To Do: fix the particle gun.")
    G4ExceptionDescription warning;
    warning << "The use of particles from files assumes that the run is not "
               "interrupted before its end,";
    warning << " otherwise the actual number of particles in the run is "
               "counted incorrectly.";
    G4Exception(
        "ParticleSourceGun::ParticleSourceGun()",
        "GRAPPA::INCORRECT_PARTICLE_COUNT",
        JustWarning,
        warning);
    using namespace CLHEP;
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    SetPart(particle_table->FindParticle("e-"));
    m_Gun = std::make_shared<G4ParticleGun>();
    m_Gun->SetParticleDefinition(m_part);
    m_filename = filename;
    reader = std::make_shared<ASCIIReader>(filename);
    m_NpartsInFile = reader->CountParticles();
    reader->ReadParticles();
    m_particlecollection = reader->GetParticles();
    if (m_NpartsInFile != static_cast<G4int>(m_particlecollection->size()))
    {
        G4ExceptionDescription msg;
        msg << "The number of particles in file " << m_NpartsInFile;
        msg << " is not equal to the number of particles in the array "
            << m_particlecollection->size() << ".";
        msg << " Please check.";
        G4Exception(
            "ParticleSourceGun::ParticleSourceGun()",
            "GRAPPA::DIFFERENT_NUMBER_OF_PARTICLES",
            FatalException,
            msg);
    }
}

ParticleSourceGun::~ParticleSourceGun() { m_part = nullptr; }

void ParticleSourceGun::LoadNextParticle(int eventNumber)
{
    GRAPPAParticle particle;
    double Kenergy;
    G4ParticleMomentum momentumdirection;
    const int eventModulo =
        (eventNumber + m_TotalParticlesSimulated) % m_NpartsInFile;

    particle = m_particlecollection->at(eventModulo);

    m_NextPosition = particle.first;
    m_NextMomentum = particle.second;
    Kenergy = std::sqrt(m_NextMomentum.mag2() + m_particle_mass_squared) -
        m_particle_mass;
    momentumdirection = m_NextMomentum / m_NextMomentum.mag();
    m_Gun->SetParticlePosition(m_NextPosition);
    m_Gun->SetParticleEnergy(Kenergy);
    m_Gun->SetParticleMomentumDirection(momentumdirection);
}