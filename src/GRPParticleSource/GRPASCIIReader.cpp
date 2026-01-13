// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <GRPASCIIReader.hpp>

ASCIIReader::ASCIIReader(std::string filename)
{
    m_filename = filename;
    m_filestream.open(m_filename);
    if (!m_filestream.is_open())
    {
        G4ExceptionDescription msg;
        msg << "Impossible to open " << m_filename;
        msg << " Please check if file exists and is accessible";
        G4Exception(
            "ASCIIReader::ASCIIReader()",
            "GRAPPA::FILE_NOT_OPEN",
            JustWarning,
            msg);
        m_filegood = false;
    }
    else
    {
        m_filegood = true;
    }
}

ASCIIReader::~ASCIIReader() {}

G4int ASCIIReader::CountParticles()
{
    G4int nlines = 0;
    std::string dumpline;

    while (m_filestream.good())
    {
        std::getline(m_filestream, dumpline);
        nlines++;
    }
    m_filestream.clear();
    m_filestream.seekg(0);
    return nlines;
}

std::vector<std::pair<G4ThreeVector, G4ParticleMomentum>>
ASCIIReader::GetParticles()
{
    double x, y, z, px, py, pz;
    std::vector<std::pair<G4ThreeVector, G4ParticleMomentum>>
        particlecollection;
    while (m_filestream.good())
    {
        m_filestream >> x;
        m_filestream >> y;
        m_filestream >> z;
        m_filestream >> px;
        m_filestream >> py;
        m_filestream >> pz;

        const std::pair<G4ThreeVector, G4ParticleMomentum> nextparticle = {
            G4ThreeVector(x, y, z), G4ParticleMomentum(px, py, pz)};
        particlecollection.push_back(nextparticle);
    }
    m_filestream.clear();
    m_filestream.seekg(0);
    return particlecollection;
}
