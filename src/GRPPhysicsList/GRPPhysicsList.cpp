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

#include <G4DecayPhysics.hh>
#include <G4HadronElasticPhysicsHP.hh>
#include <G4HadronicParameters.hh>
#include <G4IonPhysics.hh>
#include <G4MuonicAtomDecayPhysics.hh>
#include <G4NeutronTrackingCut.hh>
#include <G4StepLimiterPhysics.hh>
#include <G4StoppingPhysics.hh>
#include <G4SystemOfUnits.hh>
#include <G4ProcessTable.hh>

#include <GRPEmPhysics.hpp>
#include <GRPEmExtraPhysics.hpp>
#include <GRPPhysicsList.hpp>

GRPPhysicsList::GRPPhysicsList() : G4VModularPhysicsList(), m_biasing(true)
{

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
        250 * eV, 1 * GeV);
    SetDefaultCutValue(0.7 * mm);
    DumpCutValuesTable();

    // Add the electromagnetic physics list
    // The default list is the default in Geant4 physics lists (option 0)
    m_em_physics = new GRPEmPhysics();
    RegisterPhysics(m_em_physics);

    GRPEmExtraPhysics *extraphysics = new GRPEmExtraPhysics();
    RegisterPhysics(extraphysics);

    // Register decay of muonic atoms
    RegisterPhysics(new G4MuonicAtomDecayPhysics());

    // Decays
    RegisterPhysics(new G4DecayPhysics());

    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysicsHP());
#if defined(GRAPPA_ENABLED_BERTINI_ROLLBACK)
    G4HadronicParameters::Instance()->SetBertiniAs11_2(true);
#endif
    // Stopping Physics
    // This include the capture of muon minus (mu- + p -> n + nu_mu)
    // We set it to false because it otherwise shadows the more detailed muonic
    // atom decay physics
    G4StoppingPhysics *stoppingphysics = new G4StoppingPhysics();
    stoppingphysics->SetMuonMinusCapture(false);
    RegisterPhysics(stoppingphysics);

    // Ion Physics
    RegisterPhysics(new G4IonPhysics());

    // Neutron tracking cut
    RegisterPhysics(new G4NeutronTrackingCut());

    // Add the step limiter physics
    RegisterPhysics(new G4StepLimiterPhysics());

    // Introduce biasing
    m_biasingPhysics = new G4GenericBiasingPhysics();
    // We need to bias GammaGeneralProc because it's the only process
    // explicitly
    // declared. Selection is then performed in the biasing operation.
    std::vector<G4String> processToBias{"GammaToMuPair"};
    m_biasingPhysics->PhysicsBias("gamma", processToBias);
    // Bias pion decay
    processToBias = {"Decay"};
    m_biasingPhysics->PhysicsBias("pi+", processToBias);
    m_biasingPhysics->PhysicsBias("pi-", processToBias);

    RegisterPhysics(m_biasingPhysics);

    // Add the physics list messenger to have UI commands
    m_pl_messenger =
        std::make_unique<GRPPhysicsListMessenger>(this, m_em_physics);
}

GRPPhysicsList::~GRPPhysicsList() = default;

void GRPPhysicsList::AddBiasing()
{
    if (m_biasingPhysics)
    {
        ReplacePhysics(m_biasingPhysics);
        m_biasing = true;
    }
}

void GRPPhysicsList::RemoveBiasing()
{
    if (m_biasingPhysics)
    {
        RemovePhysics(m_biasingPhysics);
        m_biasing = false;
    }
}

void GRPPhysicsList::SetAddBiasing(G4bool ifaddbiasing)
{
    if (ifaddbiasing)
    {
        AddBiasing();
    }
    else
    {
        RemoveBiasing();
    }
}
