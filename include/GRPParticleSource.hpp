// Copyright 2022 Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause
// 

#pragma once

#include <fstream>
#include <memory>

#include <G4ParticleTable.hh>
#include <G4ParticleGun.hh>
#include <G4GeneralParticleSource.hh>

#include <ReadASCII.hpp>

class ParticleSource
{
public:
	ParticleSource();
	virtual ~ParticleSource();

	// method to access particle gun
	virtual G4VPrimaryGenerator *GetGun() = 0;
	void AddTotalParticlesSimulated(G4int addparticles) { m_TotalParticlesSimulated += addparticles; }
	virtual void LoadNextParticle(G4int){};
	void SetPart(G4ParticleDefinition *part);
	virtual G4int GetNParticlesInFile() { return 0; }
	inline G4int GetTotalParticlesSimulated() { return m_TotalParticlesSimulated; }

protected:
	G4ParticleDefinition *m_part = nullptr;
	G4double m_particle_mass = 0;
	G4double m_particle_mass_squared = 0;
	G4int m_TotalParticlesSimulated = 0;
};

class ParticleSourceGPS : public ParticleSource
{
public:
	ParticleSourceGPS();
	virtual ~ParticleSourceGPS();

	// method to access particle gun
	G4GeneralParticleSource *GetGun() override { return m_GPS.get(); };

private:
	std::shared_ptr<G4GeneralParticleSource> m_GPS;
};

class ParticleSourceGun : public ParticleSource
{
public:
	ParticleSourceGun(G4String filename);
	virtual ~ParticleSourceGun();
	void LoadNextParticle(int eventNumber) override;

	// method to access particle gun
	G4ParticleGun *GetGun() override { return m_Gun.get(); };
	G4int GetNParticlesInFile() override { return m_NpartsInFile; };

private:
	G4ThreeVector m_NextPosition = G4ThreeVector(0., 0., 0.);
	G4ParticleMomentum m_NextMomentum = G4ParticleMomentum(0., 0., 0.);
	std::shared_ptr<std::vector<GRAPPAParticle>> m_particlecollection;
	std::string m_filename;
	std::shared_ptr<G4ParticleGun> m_Gun;
	G4int m_NpartsInFile = 0;
	std::shared_ptr<ASCIIReader> reader;
};