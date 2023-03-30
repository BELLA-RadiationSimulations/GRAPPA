/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <GRPParticleSource.hpp>

ParticleSource::ParticleSource()
{
}

ParticleSource::~ParticleSource()
{
}

void ParticleSource::SetPart(G4ParticleDefinition *part)
{
    m_part = part;
    m_particle_mass = m_part->GetPDGMass();
    m_particle_mass_squared = std::pow(m_particle_mass, 2);
}

ParticleSourceGPS::ParticleSourceGPS() : ParticleSource()
{
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    SetPart(particle_table->FindParticle("e-"));
    m_GPS = std::make_shared<G4GeneralParticleSource>();
    m_GPS->SetParticleDefinition(m_part);
    m_GPS->SetVerbosity(0);
}

ParticleSourceGPS::~ParticleSourceGPS()
{
    m_part = nullptr;
}

ParticleSourceGun::ParticleSourceGun(G4String filename) : ParticleSource()
{
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
        msg << " is not equal to the number of particles in the array " << m_particlecollection->size() << ".";
        msg << " Please check.";
        G4Exception("ParticleSourceGun::ParticleSourceGun()",
                    "GRAPPA::DIFFERENT_NUMBER_OF_PARTICLES", FatalException, msg);
    }
}

ParticleSourceGun::~ParticleSourceGun()
{
    m_part = nullptr;
}

void ParticleSourceGun::LoadNextParticle(int eventNumber)
{
    GRAPPAParticle particle;
    double energy;
    G4ParticleMomentum momentumdirection;
    int eventModulo = (eventNumber + m_TotalParticlesSimulated) % m_NpartsInFile;

    particle = m_particlecollection->at(eventModulo);

    m_NextPosition = particle.first;
    m_NextMomentum = particle.second;
    energy = std::sqrt(m_NextMomentum.mag2() + m_particle_mass_squared);
    momentumdirection = m_NextMomentum / m_NextMomentum.mag();
    m_Gun->SetParticlePosition(m_NextPosition);
    m_Gun->SetParticleEnergy(energy);
    m_Gun->SetParticleMomentumDirection(momentumdirection);
}