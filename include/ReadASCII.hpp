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

#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include <G4ParticleMomentum.hh>
#include <G4ThreeVector.hh>

typedef std::pair<G4ThreeVector, G4ParticleMomentum> GRAPPAParticle;
class ASCIIReader
{
private:
    std::string m_filename;
    std::ifstream m_filestream;
    std::shared_ptr<std::vector<GRAPPAParticle>> m_particlecollection;

public:
    ASCIIReader();
    ASCIIReader(std::string filename);
    ~ASCIIReader();
    G4int CountParticles();
    void ReadParticles();
    inline std::shared_ptr<std::vector<GRAPPAParticle>> GetParticles()
    {
        return m_particlecollection;
    };
    inline std::string GetFileName() { return m_filename; };
};
