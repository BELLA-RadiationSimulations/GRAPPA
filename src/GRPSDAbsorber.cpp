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

#include <GRPSDAbsorber.hpp>

//
//  Here we use the Particle Data Group (PDG) convention
//  for particle numbering. Each particle has a unique integer ID.
//  Name: e-, id: 11
//  Name: e+, id: -11
//  Name: gamma, id: 22
//  Name: mu-, id: 13
//  Name: mu+, id: -13
//  Name: pi+, id: 211
//  Name: pi-, id: -211

// Primaries, electrons, positrons and photons absorbing layer
AbsorberSD::AbsorberSD(G4String name, HistandNTupleManager *myanalysismanager)
    : G4VSensitiveDetector(name)
{
    m_HistoandNtupleManager = myanalysismanager;

    m_ParticleList.push_back(ParticleID::electronID);

    m_ParticleList.push_back(ParticleID::positronID);

    m_ParticleList.push_back(ParticleID::gammaID);

    m_ParticleList.push_back(ParticleID::muonminusID);

    m_ParticleList.push_back(ParticleID::muonplusID);

    m_ParticleList.push_back(ParticleID::pionminusID);

    m_ParticleList.push_back(ParticleID::pionplusID);
}

AbsorberSD::~AbsorberSD() {}

void AbsorberSD::Initialize(G4HCofThisEvent *) {}

G4bool AbsorberSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // Analysis manager for histograms
    auto analysisManager = G4AnalysisManager::Instance();
    G4double kineticEnergy;
    G4float time, pz, px, py, theta_mrad, phi, charge;
    G4ThreeVector position, momentum;

    // Access track information
    G4Track *aTrack = step->GetTrack();

    // Getting pre step point information and volume name
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    const G4String thisVolumename = aTrack->GetVolume()->GetName();
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    const G4int particleID = particle->GetPDGEncoding();
    const G4int pID = aTrack->GetParentID();
    G4String creatorprocessname = "";

    // Check if particle is in the particle list
    const G4bool particleinvector =
        (std::find(m_ParticleList.begin(), m_ParticleList.end(), particleID) !=
         m_ParticleList.end());
    if (!particleinvector && (pID != 0))
    {
        return false;
    }
    G4int histeneid, histoxyid, histotxtyid, histthetaid, histphiid, ntupleid;

    // Pointer to current process
    const G4VProcess *CurrentProcess = preStepPoint->GetProcessDefinedStep();

    // Boolean flags for histogram and Ntuple filling
    const G4bool FillHistogram = true;
    G4bool FillNtuple = true;

    // Energy filters for particles
    constexpr G4double MinPrimaryEnergy = 4 * MeV;
    constexpr G4double MinPhotonEnergy = 4 * MeV;
    constexpr G4double MinElectronEnergy = 1 * MeV;
    constexpr G4double MinPositronEnergy = 1 * MeV;
    constexpr G4double pi = CLHEP::pi;

    if (!analysisManager->IsActive())
    {
        return false;
    }
    if (CurrentProcess)
    {
        // Getting process name (it should match "Transportation")
        const G4String &StepProcessName = CurrentProcess->GetProcessName();
        if (StepProcessName == "Transportation")
        {
            // processing hit when entering the volume
            kineticEnergy = aTrack->GetKineticEnergy();
            position = aTrack->GetPosition();
            momentum = aTrack->GetMomentum();
            charge = static_cast<G4float>(particle->GetPDGCharge());
            time = static_cast<G4float>(aTrack->GetGlobalTime());
            aTrack->SetTrackStatus(fStopAndKill);
            pz = static_cast<G4float>(std::abs(momentum.z()));
            px = static_cast<G4float>(momentum.x());
            py = static_cast<G4float>(momentum.y());
            theta_mrad = static_cast<G4float>(pi - momentum.theta());
            phi = static_cast<G4float>(momentum.phi());
        }
        else
        {
            return false;
        }

        HistoManager *histomanager = m_HistoandNtupleManager->GetHistoManager();

        const bool isMuorPi =
            (particleID == ParticleID::pionminusID ||
             particleID == ParticleID::pionplusID ||
             particleID == ParticleID::muonminusID ||
             particleID == ParticleID::muonplusID);
        const bool hasCreatorProcess = isMuorPi ||
            particleID == ParticleID::positronID ||
            (particleID == ParticleID::electronID && pID != 0);

        if (hasCreatorProcess)
        {
            const G4VProcess *creatorprocess = aTrack->GetCreatorProcess();
            creatorprocessname = creatorprocess->GetProcessName();
        }
        if (pID == 0)
        {
            histeneid = histomanager->GetPrimaryEneId();
            histoxyid = histomanager->GetPrimaryxyId();
            histotxtyid = histomanager->GetPrimarytxtyId();
            histthetaid = histomanager->GetPrimaryThetaId();
            histphiid = histomanager->GetPrimaryPhiId();
            ntupleid =
                m_HistoandNtupleManager->GetNTupleManager()->GetPrimaryId();
            FillNtuple = !(kineticEnergy < MinPrimaryEnergy);
        }
        else
        {

            if (particleID == ParticleID::positronID)
            {
                histeneid = histomanager->GetPositronEneId();
                histoxyid = histomanager->GetPositronxyId();
                histotxtyid = histomanager->GetPositrontxtyId();
                histthetaid = histomanager->GetPositronThetaId();
                histphiid = histomanager->GetPositronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()
                               ->GetPositronId();
                FillNtuple = !(kineticEnergy < MinPositronEnergy);
            }
            else if (particleID == ParticleID::gammaID)
            {
                histeneid = histomanager->GetGammaEneId();
                histoxyid = histomanager->GetGammaxyId();
                histotxtyid = histomanager->GetGammatxtyId();
                histthetaid = histomanager->GetGammaThetaId();
                histphiid = histomanager->GetGammaPhiId();
                ntupleid =
                    m_HistoandNtupleManager->GetNTupleManager()->GetGammaId();
                FillNtuple = !(kineticEnergy < MinPhotonEnergy);
            }
            else if (particleID == ParticleID::electronID)
            {

                histeneid = histomanager->GetElectronEneId();
                histoxyid = histomanager->GetElectronxyId();
                histotxtyid = histomanager->GetElectrontxtyId();
                histthetaid = histomanager->GetElectronThetaId();
                histphiid = histomanager->GetElectronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()
                               ->GetElectronId();
                FillNtuple = !(kineticEnergy < MinElectronEnergy);
            }
            // Pions are collected in a single ntuple
            else if (
                particleID == ParticleID::pionminusID ||
                particleID == ParticleID::pionplusID)
            {
                histeneid = histomanager->GetPionEneId();
                histoxyid = histomanager->GetPionxyId();
                histotxtyid = histomanager->GetPiontxtyId();
                histthetaid = histomanager->GetPionThetaId();
                histphiid = histomanager->GetPionPhiId();
                ntupleid =
                    m_HistoandNtupleManager->GetNTupleManager()->GetPionId();
            }
            // Muons are collected in a single ntuple
            else if (
                particleID == ParticleID::muonminusID ||
                particleID == ParticleID::muonplusID)
            {
                histeneid = histomanager->GetMuonEneId();
                histoxyid = histomanager->GetMuonxyId();
                histotxtyid = histomanager->GetMuontxtyId();
                histthetaid = histomanager->GetMuonThetaId();
                histphiid = histomanager->GetMuonPhiId();
                ntupleid =
                    m_HistoandNtupleManager->GetNTupleManager()->GetMuonId();
            }
            else
            {
                return false;
            }
        }

        if (FillHistogram)
        {
            // Filling the correct histogram
            if (analysisManager->GetH1Activation(histeneid))
            {
                analysisManager->FillH1(histeneid, kineticEnergy);
            }
            if (analysisManager->GetH1Activation(histthetaid))
            {
                analysisManager->FillH1(histthetaid, theta_mrad);
            }
            if (analysisManager->GetH1Activation(histphiid))
            {
                analysisManager->FillH1(histphiid, phi);
            }
            if (analysisManager->GetH2Activation(histoxyid))
            {
                analysisManager->FillH2(histoxyid, position.x(), position.y());
            }
            if (analysisManager->GetH2Activation(histotxtyid))
            {
                analysisManager->FillH2(
                    histotxtyid, std::atan2(px, pz), std::atan2(py, pz));
            }
        }

        NTupleManager *ntuplemanager =
            m_HistoandNtupleManager->GetNTupleManager();
        FillNtuple = (FillNtuple && ntuplemanager->GetIdActivation(ntupleid));

        if (FillNtuple)
        {
            G4int ncol = 0;
            // Filling the correct Ntuple
            analysisManager->FillNtupleFColumn(ntupleid, 0, static_cast<G4float>(position.x()));
            analysisManager->FillNtupleFColumn(ntupleid, 1, static_cast<G4float>(position.y()));
            analysisManager->FillNtupleFColumn(ntupleid, 2, static_cast<G4float>(position.z()));
            analysisManager->FillNtupleFColumn(ntupleid, 3, static_cast<G4float>(momentum.x()));
            analysisManager->FillNtupleFColumn(ntupleid, 4, static_cast<G4float>(momentum.y()));
            analysisManager->FillNtupleFColumn(ntupleid, 5, static_cast<G4float>(momentum.z()));
            analysisManager->FillNtupleFColumn(ntupleid, 6, time);
            ncol = 7;
            if (isMuorPi)
            {
                analysisManager->FillNtupleFColumn(ntupleid, ncol, charge);
                ncol += 1;
            }
            if (hasCreatorProcess)
            {
                analysisManager->FillNtupleSColumn(
                    ntupleid, ncol, creatorprocessname);
                ncol += 1;
            }
            analysisManager->AddNtupleRow(ntupleid);
        }

        return true;
    }

    return false;
}
