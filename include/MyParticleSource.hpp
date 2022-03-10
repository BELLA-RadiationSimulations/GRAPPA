/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <memory>
#include <G4ParticleTable.hh>
#include <G4GeneralParticleSource.hh>

class ParticleSource
{
public:
	ParticleSource();
	~ParticleSource();

	// method to access particle gun
	G4GeneralParticleSource* GetGun() { return m_Gun.get(); }
private:

	std::shared_ptr<G4GeneralParticleSource> m_Gun;

};
