/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyParticleSource.hpp>


ParticleSource::ParticleSource()
{
    m_Gun = std::make_shared<G4GeneralParticleSource>();
    G4ParticleTable* particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* part
        = particle_table->FindParticle("e-");
    m_Gun->SetParticleDefinition(part);
    m_Gun->SetVerbosity(0);
}

ParticleSource::~ParticleSource()
{
}