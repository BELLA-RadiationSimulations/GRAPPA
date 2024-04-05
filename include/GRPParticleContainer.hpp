// Copyright 2021-2024
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#pragma once

#include <G4GenericMessenger.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>

#include <GRPASCIIReader.hpp>

// A "particle" contains the position and the momentum
typedef std::pair<G4ThreeVector, G4ParticleMomentum> GRPParticle;

class GRPParticleContainer
{
public:
    GRPParticleContainer();
    ~GRPParticleContainer();

    void AddTotalParticlesSimulated(G4int addparticles);
    void DefineCommands();
    std::tuple<G4ThreeVector, G4double, G4ThreeVector>
    GetNextPositionEnergyMomentumdirection(G4int particle) const;
    G4int GetNParticlesInFile() const { return m_NpartsInFile; }
    G4String GetParticleName() const { return m_particlename; }
    G4ParticleDefinition *GetParticleDefinition() const
    {
        return m_particledefinition;
    }
    G4int GetTotalParticlesSimulated() const
    {
        return m_TotalParticlesSimulated;
    }
    G4bool GetUseFile() const { return m_useFile; }
    G4bool Load(G4String filename);
    void Print();
    void SetFilename(G4String filename);
    void SetParticle(G4String particle);
    void SetUseFile(G4bool usefile) { m_useFile = usefile; }

private:
    // Stores the number of particles used in the file
    G4int m_TotalParticlesSimulated = 0;
    // Simulated particle name
    G4String m_particlename = "geantino";
    // Simulated particle definition
    G4ParticleDefinition *m_particledefinition;
    // Simulated particle mass
    G4double m_particle_mass;
    // Simulated particle mass squared
    G4double m_particle_mass_squared;
    // Unit for particle position
    G4double m_input_position_units = CLHEP::mm;
    // Unit for particle momentum
    G4double m_input_momentum_units = CLHEP::MeV;
    // Vector containing the particles read from the source
    std::vector<GRPParticle> m_particlecollection;
    // Input file name with particle phase space
    G4String m_filename;
    // Number of particles in the file
    G4int m_NpartsInFile = 0;
    // If the file is used
    G4bool m_useFile = false;
    // Pointer to the generic messengers
    std::shared_ptr<G4GenericMessenger> m_CMDpartsfromfile;
};
