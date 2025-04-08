// Copyright 2021-2025
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <GRPParticleContainer.hpp>

GRPParticleContainer::GRPParticleContainer()
{
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle =
        particle_table->FindParticle(m_particlename);
    m_particledefinition = particle;
    m_particle_mass = particle->GetPDGMass();
    m_particle_mass_squared = std::pow(m_particle_mass, 2);
    DefineCommands();
}

GRPParticleContainer::~GRPParticleContainer() {}

void GRPParticleContainer::AddTotalParticlesSimulated(G4int addparticles)
{
    m_TotalParticlesSimulated += addparticles;
    if (m_NpartsInFile > 0)
    {
        m_TotalParticlesSimulated = m_TotalParticlesSimulated % m_NpartsInFile;
    }
}

void GRPParticleContainer::DefineCommands()
{
    m_CMDpartsfromfile = std::make_shared<G4GenericMessenger>(
        this, "/particlesource/", "Set if GRAPPA reads particles from file.");

    G4GenericMessenger::Command ifpartsfromfile, isfromfile, setpart, filename,
        list;

    ifpartsfromfile = m_CMDpartsfromfile->DeclareProperty(
        "particlesFromFile", m_useFile, "Read particles from a file.");
    ifpartsfromfile.SetGuidance(
        "If true, particles will be read from a binary files and generated "
        "using a particle Gun.");
    ifpartsfromfile.SetStates(G4State_PreInit);

    setpart = m_CMDpartsfromfile->DeclareMethod(
        "setParticle",
        &GRPParticleContainer::SetParticle,
        "Set the particle type.");
    setpart.SetGuidance(
        "Set the particle type associated with the particles in the input "
        "file.");
    setpart.SetStates(G4State_PreInit, G4State_Idle);

    filename = m_CMDpartsfromfile->DeclareMethod(
        "setFileName",
        &GRPParticleContainer::SetFilename,
        "Set name of input file for particles.");
    filename.SetGuidance(
        "Set the name of the file from which particles are read");
    filename.SetStates(G4State_PreInit, G4State_Idle);

    list = m_CMDpartsfromfile->DeclareMethod(
        "print",
        &GRPParticleContainer::Print,
        "Prints the current status of particle input.");
    list.SetGuidance(
        "Prints a message with the status of the particle input. It specifies "
        "if particles are taken from an input file and, in case, it prints the "
        "name");
    list.SetStates(G4State_PreInit, G4State_Idle);
}

std::tuple<G4ThreeVector, G4double, G4ThreeVector>
GRPParticleContainer::GetNextPositionEnergyMomentumdirection(
    G4int eventNumber) const
{
    const int eventModulo =
        (eventNumber + m_TotalParticlesSimulated) % m_NpartsInFile;
    const GRPParticle particle = m_particlecollection[eventModulo];
    const G4ThreeVector NextPosition = particle.first * m_input_position_units;
    const G4ParticleMomentum NextMomentum =
        particle.second * m_input_momentum_units;
    // Default units in Geant4 for momentum and PDG mass are MeV
    const G4double Kenergy =
        std::sqrt(NextMomentum.mag2() + m_particle_mass_squared) -
        m_particle_mass;
    const G4ThreeVector NextMomentumDirection =
        NextMomentum / NextMomentum.mag();

    return std::make_tuple(NextPosition, Kenergy, NextMomentumDirection);
}

G4bool GRPParticleContainer::Load(G4String filename)
{
    ASCIIReader reader = ASCIIReader(filename);

    if (!reader.IsFileGood())
        return false;
    m_NpartsInFile = reader.CountParticles();
    m_particlecollection = reader.GetParticles();
    if (m_NpartsInFile != static_cast<G4int>(m_particlecollection.size()))
    {
        G4ExceptionDescription msg;
        msg << "The number of particles in file " << m_NpartsInFile;
        msg << " is not equal to the number of particles in the array "
            << m_particlecollection.size() << ".";
        msg << " Please check.";
        G4Exception(
            "ParticleSourceGun::Load()",
            "GRAPPA::DIFFERENT_NUMBER_OF_PARTICLES",
            FatalException,
            msg);
        return false;
    }
    if (m_NpartsInFile == 0)
    {
        G4ExceptionDescription msg;
        msg << "The file " << m_filename;
        msg << " has zero particles. ";
        msg << " Please check.";
        G4Exception(
            "ParticleSourceGun::Load()",
            "GRAPPA::ZERO_PARTICLES",
            JustWarning,
            msg);
    }
    m_TotalParticlesSimulated = 0;
    return true;
}

void GRPParticleContainer::Print()
{
    if (m_useFile)
    {
        G4cout << G4endl;
        G4cout << " GRAPPA will read particles from an input file. " << G4endl;
        G4cout << " File name: " << m_filename << G4endl;
        G4cout << " Particle Name: " << m_particlename << G4endl;
        G4cout << G4endl;
    }
    else
    {
        G4cout << G4endl;
        G4cout << " GRAPPA will use a GPS to generate particles. " << G4endl;
        G4cout << G4endl;
    }
}

void GRPParticleContainer::SetFilename(G4String filename)
{
    const G4bool isloadgood = Load(filename);
    if (isloadgood)
        m_filename = filename;
}

void GRPParticleContainer::SetParticle(const G4String particlename)
{
    if (!m_useFile)
    {
        G4ExceptionDescription msg;
        msg << "This command is only meant to change the particle type read "
               "from the file. "
               "If using the GPS, change the particle type using the "
               "appropriate command.";
        G4Exception(
            "ParticleSourceGun::SetParticle",
            "GRAPPA::USING_GPS",
            JustWarning,
            msg);
        return;
    }
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particle_table->FindParticle(particlename);
    if (!particle)
    {
        G4ExceptionDescription msg;
        msg << "Particle " << particlename
            << " not found. Particle is not changed";
        G4Exception(
            "ParticleSourceGun::SetParticle",
            "GRAPPA::PARTICLE_NOT_FOUND",
            JustWarning,
            msg);
    }
    else
    {
        m_particlename = particlename;
        m_particledefinition = particle;
        m_particle_mass = particle->GetPDGMass();
        m_particle_mass_squared = std::pow(m_particle_mass, 2);
    }
}
