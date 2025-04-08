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

    // Storing particles of interest in a vector
    // Particle IDs are defined in GRPParticlePDGID.hpp
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
    G4float time, pz, px, py, theta_mrad, phi, charge, weight;
    G4ThreeVector position, momentum;

    // Access track information
    G4Track *aTrack = step->GetTrack();

    // Access pre-step point information
    G4StepPoint *preStepPoint = step->GetPreStepPoint();

    // Get volume name
    const G4String thisVolumename = aTrack->GetVolume()->GetName();

    // Get particle information
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    const G4int particleID = particle->GetPDGEncoding();
    const G4int pID = aTrack->GetParentID();
    G4String creatorprocessname = "";

    // Check if particle is of interest, i.e. listed in m_ParticleList
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

    // Energy filters for particles
    constexpr G4double pi = CLHEP::pi;

    // Return if analysis is disabled
    if (!analysisManager->IsActive())
    {
        return false;
    }

    if (CurrentProcess)
    {
        // Getting process name
        // For the world absorber we want the 'transportation' type process,
        // which ensures that the particle is only counted onces in a detector
        const G4String &StepProcessName = CurrentProcess->GetProcessName();
        if (StepProcessName == "Transportation")
        {
            // Processing hit when entering the volume
            kineticEnergy = aTrack->GetKineticEnergy();
            position = aTrack->GetPosition();
            momentum = aTrack->GetMomentum();
            charge = static_cast<G4float>(particle->GetPDGCharge());
            time = static_cast<G4float>(aTrack->GetGlobalTime());
            weight = static_cast<G4float>(aTrack->GetWeight());
            // Killing particle tracking after hitting the detector
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
        }
        else
        {
            // Getting histogram and ntuple IDs
            // Positrons
            if (particleID == ParticleID::positronID)
            {
                histeneid = histomanager->GetPositronEneId();
                histoxyid = histomanager->GetPositronxyId();
                histotxtyid = histomanager->GetPositrontxtyId();
                histthetaid = histomanager->GetPositronThetaId();
                histphiid = histomanager->GetPositronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()
                               ->GetPositronId();
            }
            // Gamma
            else if (particleID == ParticleID::gammaID)
            {
                histeneid = histomanager->GetGammaEneId();
                histoxyid = histomanager->GetGammaxyId();
                histotxtyid = histomanager->GetGammatxtyId();
                histthetaid = histomanager->GetGammaThetaId();
                histphiid = histomanager->GetGammaPhiId();
                ntupleid =
                    m_HistoandNtupleManager->GetNTupleManager()->GetGammaId();
            }
            // Electrons
            else if (particleID == ParticleID::electronID)
            {

                histeneid = histomanager->GetElectronEneId();
                histoxyid = histomanager->GetElectronxyId();
                histotxtyid = histomanager->GetElectrontxtyId();
                histthetaid = histomanager->GetElectronThetaId();
                histphiid = histomanager->GetElectronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()
                               ->GetElectronId();
            }
            // Pions
            // Pions of both charges are collected in one ntuple
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
            // Muons
            // Muons of both charges are collected in one ntuple
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

        // Filling histograms
        if (analysisManager->GetH1Activation(histeneid))
        {
            analysisManager->FillH1(histeneid, kineticEnergy, weight);
        }
        if (analysisManager->GetH1Activation(histthetaid))
        {
            analysisManager->FillH1(histthetaid, theta_mrad, weight);
        }
        if (analysisManager->GetH1Activation(histphiid))
        {
            analysisManager->FillH1(histphiid, phi, weight);
        }
        if (analysisManager->GetH2Activation(histoxyid))
        {
            analysisManager->FillH2(histoxyid, position.x(), position.y(), weight);
        }
        if (analysisManager->GetH2Activation(histotxtyid))
        {
            analysisManager->FillH2(
                histotxtyid, std::atan2(px, pz), std::atan2(py, pz), weight);
        }

        // Filling NTuples
        NTupleManager *ntuplemanager =
            m_HistoandNtupleManager->GetNTupleManager();
        const G4bool analysisactive = analysisManager->IsActive();

        const G4bool activationstatus =
            ntuplemanager->GetIdActivation(ntupleid);

        if (activationstatus && analysisactive)
        {
            G4int ncol = 0;
            // Filling the Ntuples
            analysisManager->FillNtupleFColumn(
                ntupleid, 0, static_cast<G4float>(position.x()));
            analysisManager->FillNtupleFColumn(
                ntupleid, 1, static_cast<G4float>(position.y()));
            analysisManager->FillNtupleFColumn(
                ntupleid, 2, static_cast<G4float>(position.z()));
            analysisManager->FillNtupleFColumn(
                ntupleid, 3, static_cast<G4float>(momentum.x()));
            analysisManager->FillNtupleFColumn(
                ntupleid, 4, static_cast<G4float>(momentum.y()));
            analysisManager->FillNtupleFColumn(
                ntupleid, 5, static_cast<G4float>(momentum.z()));
            analysisManager->FillNtupleFColumn(
                ntupleid, 6, static_cast<G4float>(weight));
            analysisManager->FillNtupleFColumn(ntupleid, 7, time);
            ncol = 8;
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
