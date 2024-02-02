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
#include <vector>

#include <G4ParticleMomentum.hh>
#include <G4ThreeVector.hh>

class ASCIIReader
{

public:
    ASCIIReader() = default;
    ASCIIReader(std::string filename);
    ~ASCIIReader();
    G4int CountParticles();
    G4bool IsFileGood() const { return m_filegood; }
    std::vector<std::pair<G4ThreeVector, G4ParticleMomentum>> GetParticles();
    std::string GetFileName() const { return m_filename; };

private:
    std::string m_filename;
    std::ifstream m_filestream;
    G4bool m_filegood = true;
};
