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

#include <G4BuilderType.hh>
#include <G4ComptonScattering.hh>
#include <G4CoulombScattering.hh>
#include <G4eBremsstrahlung.hh>
#include <G4eCoulombScatteringModel.hh>
#include <G4eIonisation.hh>
#include <G4Electron.hh>
#include <G4EmBuilder.hh>
#include <G4EmModelActivator.hh>
#include <G4EmParameters.hh>
#include <G4eplusAnnihilation.hh>
#include <G4Gamma.hh>
#include <G4GammaConversion.hh>
#include <G4GammaConversionToMuons.hh>
#include <G4GammaGeneralProcess.hh>
#include <G4GenericIon.hh>
#include <G4hIonisation.hh>
#include <G4hMultipleScattering.hh>
#include <G4ionIonisation.hh>
#include <G4KleinNishinaModel.hh>
#include <G4LivermorePhotoElectricModel.hh>
#include <G4LivermorePolarizedRayleighModel.hh>
#include <G4LossTableManager.hh>
#include <G4MuonMinus.hh>
#include <G4MuonPlus.hh>
#include <G4NuclearStopping.hh>
#include <G4ParticleDefinition.hh>
#include <G4PhotoElectricAngularGeneratorPolarized.hh>
#include <G4PhotoElectricEffect.hh>
#include <G4PhysicsListHelper.hh>
#include <G4Positron.hh>
#include <G4ProcessManager.hh>
#include <G4RayleighScattering.hh>
#include <G4SystemOfUnits.hh>
#include <G4UrbanMscModel.hh>
#include <G4WentzelVIModel.hh>

#include <GRPEmPhysics.hpp>

GRPEmPhysics::GRPEmPhysics(G4int ver, const G4String &)
    : G4VPhysicsConstructor("GRPEmCustomPhysics")
{
    SetVerboseLevel(ver);
    G4EmParameters *param = G4EmParameters::Instance();
    param->SetDefaults();
    param->SetVerbose(ver);
    param->SetGeneralProcessActive(false);
    param->SetFluctuationType(fUrbanFluctuation);
    param->SetUseICRU90Data(true);
    SetPhysicsType(bElectromagnetic);
}

GRPEmPhysics::~GRPEmPhysics() = default;

void GRPEmPhysics::ConstructParticle()
{
    // minimal set of particles for EM physics
    G4EmBuilder::ConstructMinimalEmSet();
}

void GRPEmPhysics::ConstructProcess()
{
    if (verboseLevel > 1)
    {
        G4cout << "### " << GetPhysicsName() << " Construct Processes "
               << G4endl;
    }
    G4EmBuilder::PrepareEMPhysics();

    G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
    G4EmParameters *param = G4EmParameters::Instance();

    // processes used by several particles
    G4hMultipleScattering *hmsc = new G4hMultipleScattering("ionmsc");

    // nuclear stopping is enabled if th eenergy limit above zero
    const G4double nielEnergyLimit = param->MaxNIELEnergy();
    G4NuclearStopping *pnuc = nullptr;
    if (nielEnergyLimit > 0.0)
    {
        pnuc = new G4NuclearStopping();
        pnuc->SetMaxKinEnergy(nielEnergyLimit);
    }

    // high energy limit for e+- scattering models and bremsstrahlung
    const G4double highEnergyLimit = param->MscEnergyLimit();

    // Add gamma EM Processes
    G4ParticleDefinition *particle = G4Gamma::Gamma();
    const G4bool polar = param->EnablePolarisation();

    // Photoelectric
    G4PhotoElectricEffect *pe = new G4PhotoElectricEffect();
    G4VEmModel *peModel = new G4LivermorePhotoElectricModel();
    pe->SetEmModel(peModel);
    if (polar)
    {
        peModel->SetAngularDistribution(
            new G4PhotoElectricAngularGeneratorPolarized());
    }

    // Compton scattering
    G4ComptonScattering *cs = new G4ComptonScattering;
    if (polar)
    {
        cs->SetEmModel(new G4KleinNishinaModel());
    }

    // default Rayleigh scattering is Livermore
    G4RayleighScattering *rl = new G4RayleighScattering();
    if (polar)
    {
        rl->SetEmModel(new G4LivermorePolarizedRayleighModel());
    }

    if (G4EmParameters::Instance()->GeneralProcessActive())
    {
        G4GammaGeneralProcess *sp = new G4GammaGeneralProcess();
        sp->AddEmProcess(pe);
        sp->AddEmProcess(cs);
        sp->AddEmProcess(new G4GammaConversion());
        sp->AddEmProcess(rl);
        G4LossTableManager::Instance()->SetGammaGeneralProcess(sp);
        ph->RegisterProcess(sp, particle);
    }
    else
    {
        ph->RegisterProcess(pe, particle);
        ph->RegisterProcess(cs, particle);
        ph->RegisterProcess(new G4GammaConversion(), particle);
        ph->RegisterProcess(rl, particle);
    }

    // Here we add the remaining discrete processes of interest for gammas.
    // Discrete processes cannot be added via AddEmProcess because they have a
    // different interface And they should be added directly to the particle
    // manager

    G4ProcessManager *pmanager = particle->GetProcessManager();

    G4GammaConversionToMuons *gmumu = new G4GammaConversionToMuons();
    gmumu->SetCrossSecFactor(m_gmumuxsfactor);
    pmanager->AddDiscreteProcess(gmumu);

    // e-
    particle = G4Electron::Electron();

    G4UrbanMscModel *msc1 = new G4UrbanMscModel();
    G4WentzelVIModel *msc2 = new G4WentzelVIModel();
    msc1->SetHighEnergyLimit(highEnergyLimit);
    msc2->SetLowEnergyLimit(highEnergyLimit);
    G4EmBuilder::ConstructElectronMscProcess(msc1, msc2, particle);

    G4eCoulombScatteringModel *ssm = new G4eCoulombScatteringModel();
    G4CoulombScattering *ss = new G4CoulombScattering();
    ss->SetEmModel(ssm);
    ss->SetMinKinEnergy(highEnergyLimit);
    ssm->SetLowEnergyLimit(highEnergyLimit);
    ssm->SetActivationLowEnergyLimit(highEnergyLimit);

    ph->RegisterProcess(new G4eIonisation(), particle);
    ph->RegisterProcess(new G4eBremsstrahlung(), particle);
    ph->RegisterProcess(ss, particle);

    // e+
    particle = G4Positron::Positron();

    msc1 = new G4UrbanMscModel();
    msc2 = new G4WentzelVIModel();
    msc1->SetHighEnergyLimit(highEnergyLimit);
    msc2->SetLowEnergyLimit(highEnergyLimit);
    G4EmBuilder::ConstructElectronMscProcess(msc1, msc2, particle);

    ssm = new G4eCoulombScatteringModel();
    ss = new G4CoulombScattering();
    ss->SetEmModel(ssm);
    ss->SetMinKinEnergy(highEnergyLimit);
    ssm->SetLowEnergyLimit(highEnergyLimit);
    ssm->SetActivationLowEnergyLimit(highEnergyLimit);

    // annihilation
    auto anni = new G4eplusAnnihilation();

    ph->RegisterProcess(new G4eIonisation(), particle);
    ph->RegisterProcess(new G4eBremsstrahlung(), particle);
    ph->RegisterProcess(anni, particle);
    ph->RegisterProcess(ss, particle);

    // generic ion
    particle = G4GenericIon::GenericIon();
    G4ionIonisation *ionIoni = new G4ionIonisation();
    ph->RegisterProcess(hmsc, particle);
    ph->RegisterProcess(ionIoni, particle);
    if (nullptr != pnuc)
    {
        ph->RegisterProcess(pnuc, particle);
    }

    // This function construct the basic processes (ionization, multiple
    // scattering, bremsstrahlung) for muons, hadrons and ions
    G4EmBuilder::ConstructCharged(hmsc, pnuc);

    // extra configuration
    const G4EmModelActivator mact(GetPhysicsName());
}

void GRPEmPhysics::SetMuPairCrossSection(G4double newcrosssection)
{
    if (newcrosssection <= 0)
    {
        G4ExceptionDescription msg;
        msg << "Cross section factor for muon pair production cannot be <= 0."
            << G4endl;
        msg << "The provided value is " << newcrosssection
            << ". Ignoring the value.";
        G4Exception(
            "GRPEmPhysics::SetMuPairCrossSection",
            "GRAPPA::INCORRECT_CROSS_SECTION",
            JustWarning,
            msg);
    }
    else
    {
        m_gmumuxsfactor = newcrosssection;
    }
}
