// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause-LBNL

#include <G4AnnihiToMuPair.hh>
#include <G4BuilderType.hh>
#include <G4CascadeInterface.hh>
#include <G4CrossSectionDataSetRegistry.hh>
#include <G4Electron.hh>
#include <G4ElectronNuclearProcess.hh>
#include <G4ElectroVDNuclearModel.hh>
#include <G4ExcitedStringDecay.hh>
#include <G4Gamma.hh>
#include <G4GammaNuclearXS.hh>
#include <G4GammaParticipants.hh>
#include <G4GeneratorPrecompoundInterface.hh>
#include <G4HadronicParameters.hh>
#include <G4HadronInelasticProcess.hh>
#include <G4LowEGammaNuclearModel.hh>
#include <G4MuonMinus.hh>
#include <G4MuonMinusAtomicCapture.hh>
#include <G4MuonNuclearProcess.hh>
#include <G4MuonPlus.hh>
#include <G4MuonToMuonPairProduction.hh>
#include <G4MuonVDNuclearModel.hh>
#include <G4Positron.hh>
#include <G4PositronNuclearProcess.hh>
#include <G4ProcessManager.hh>
#include <G4QGSMFragmentation.hh>
#include <G4QGSModel.hh>
#include <G4TheoFSGenerator.hh>

#include <GRPEmExtraPhysics.hpp>

GRPEmExtraPhysics::GRPEmExtraPhysics(G4int ver)
    : G4VPhysicsConstructor("GRPExtraPhysics")
    , m_GNLowEnergyLimit(200 * CLHEP::MeV)
    , m_verbose(ver)
{
    SetPhysicsType(bEmExtra);
    if (m_verbose > 1)
        G4cout << "### G4EmExtraPhysics" << G4endl;
}

GRPEmExtraPhysics::~GRPEmExtraPhysics() = default;

void GRPEmExtraPhysics::ConstructParticle()
{
    G4Gamma::Gamma();
    G4Electron::Electron();
    G4Positron::Positron();
    G4MuonPlus::MuonPlus();
    G4MuonMinus::MuonMinus();
}

void GRPEmExtraPhysics::ConstructProcess()
{
    G4ParticleDefinition *muonplus = G4MuonPlus::MuonPlus();
    G4ParticleDefinition *muonminus = G4MuonMinus::MuonMinus();

    G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

    ConstructGammaElectroNuclear();

    G4MuonNuclearProcess *muNucProcess = new G4MuonNuclearProcess();
    G4MuonVDNuclearModel *muNucModel = new G4MuonVDNuclearModel();
    muNucProcess->RegisterMe(muNucModel);
    ph->RegisterProcess(muNucProcess, muonplus);
    ph->RegisterProcess(muNucProcess, muonminus);

    // We leave this as a template to be reactivated if needed
    if (m_activate_muon_to_muon_pair)
    {
        auto proc = new G4MuonToMuonPairProduction();
        ph->RegisterProcess(proc, muonplus);
        ph->RegisterProcess(proc, muonminus);
    }

    // Add muon capturing in atom
    // Warning: maybe broken process, need to investigate
    // Sometimes it returns errors like a muonic particle being defined twice.

    // G4ProcessManager *pmanager = muonminus->GetProcessManager();

    // pmanager->AddRestProcess(new G4MuonMinusAtomicCapture());
}

void GRPEmExtraPhysics::ConstructGammaElectroNuclear()
{
    G4HadronicParameters *param = G4HadronicParameters::Instance();
    G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

    G4HadronInelasticProcess *gnuc =
        new G4HadronInelasticProcess("photonNuclear", G4Gamma::Gamma());
    auto xsreg = G4CrossSectionDataSetRegistry::Instance();
    G4VCrossSectionDataSet *xs = nullptr;
    xs = xsreg->GetCrossSectionDataSet("GammaNuclearXS");
    if (nullptr == xs)
        xs = new G4GammaNuclearXS();

    gnuc->AddDataSet(xs);

    G4QGSModel<G4GammaParticipants> *theStringModel =
        new G4QGSModel<G4GammaParticipants>;
    auto theStringDecay = new G4ExcitedStringDecay(new G4QGSMFragmentation());
    theStringModel->SetFragmentationModel(theStringDecay);

    auto theCascade = new G4GeneratorPrecompoundInterface();
    auto theModel = new G4TheoFSGenerator();
    theModel->SetTransport(theCascade);
    theModel->SetHighEnergyGenerator(theStringModel);

    // Bertini cascade for moderate energies
    auto cascade = new G4CascadeInterface();

    // added low-energy gamma nuclear model LEND disabled
    if (m_GNLowEnergyLimit > 0.0)
    {
        G4LowEGammaNuclearModel *lemod = new G4LowEGammaNuclearModel();
        lemod->SetMaxEnergy(m_GNLowEnergyLimit);
        gnuc->RegisterMe(lemod);
        cascade->SetMinEnergy(m_GNLowEnergyLimit - CLHEP::MeV);
    }
    cascade->SetMaxEnergy(param->GetMaxEnergyTransitionFTF_Cascade());
    gnuc->RegisterMe(cascade);
    theModel->SetMinEnergy(param->GetMinEnergyTransitionFTF_Cascade());
    theModel->SetMaxEnergy(param->GetMaxEnergy());
    gnuc->RegisterMe(theModel);

    ph->RegisterProcess(gnuc, G4Gamma::Gamma());
    auto enuc = new G4ElectronNuclearProcess();
    auto pnuc = new G4PositronNuclearProcess();
    auto eModel = new G4ElectroVDNuclearModel();

    enuc->RegisterMe(eModel);
    pnuc->RegisterMe(eModel);
    ph->RegisterProcess(enuc, G4Electron::Electron());
    ph->RegisterProcess(pnuc, G4Positron::Positron());
}
