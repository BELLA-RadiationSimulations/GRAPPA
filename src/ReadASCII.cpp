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

#include <ReadASCII.hpp>

ASCIIReader::ASCIIReader()
{
    m_particlecollection = std::make_shared<std::vector<GRAPPAParticle>>();
}

ASCIIReader::ASCIIReader(std::string filename)
{
    m_filename = filename;
    m_filestream.open(m_filename);
    if (!m_filestream.is_open())
    {
        G4ExceptionDescription msg;
        msg << "Impossible to open " << m_filename;
        msg << " Please check if file exists and is accessible";
        G4Exception("ASCIIReader::ASCIIReader()",
                    "GRAPPA::FILE_NOT_OPEN", FatalException, msg);
    }
    m_particlecollection = std::make_shared<std::vector<GRAPPAParticle>>();
}

ASCIIReader::~ASCIIReader()
{
    if (m_filestream.is_open())
    {
        m_filestream.close();
    }
}

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

void ASCIIReader::ReadParticles()
{
    double x, y, z, px, py, pz;
    G4ThreeVector nextposition;
    G4ParticleMomentum nextmomentum;
    GRAPPAParticle nextparticle;
    while (m_filestream.good())
    {
        m_filestream >> x;
        m_filestream >> y;
        m_filestream >> z;
        m_filestream >> px;
        m_filestream >> py;
        m_filestream >> pz;

        nextposition = G4ThreeVector(x, y, z);
        nextmomentum = G4ParticleMomentum(px, py, pz);

        nextparticle = {nextposition, nextmomentum};
        m_particlecollection->push_back(nextparticle);
    }
}