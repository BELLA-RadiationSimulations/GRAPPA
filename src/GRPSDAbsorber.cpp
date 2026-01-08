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

    const G4String HCname = "AbsorberHC";
    collectionName.insert(HCname);
}

AbsorberSD::~AbsorberSD() {}

void AbsorberSD::Initialize(G4HCofThisEvent *HCE)
{

    // Create hit collection
    m_HitCollection =
        new GRPAbsorberHitCollection(this->GetName(), collectionName[0]);

    // Add this collection in hce

    m_hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(m_hcID, m_HitCollection);
}

G4bool AbsorberSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // Analysis manager for histograms
    auto analysisManager = G4AnalysisManager::Instance();

    // Access track information
    G4Track *aTrack = step->GetTrack();
    const G4int trackID = aTrack->GetTrackID();
    G4String CreatorProcessName = "Primary";

    // Access pre-step point information
    G4StepPoint *preStepPoint = step->GetPreStepPoint();

    // Get particle information
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    const G4int particleID = particle->GetPDGEncoding();
    const G4int parentID = aTrack->GetParentID();

    // Return if analysis is disabled
    if (!analysisManager->IsActive())
    {
        return false;
    }

    // Loop where we check if the particle has already hit the SD.
    // In that case we skip the count and we return from the function
    // If the loop goes through it means that the particle is hitting
    // the SD for the first time
    const std::vector<GRPAbsorberHit *> &HCVector =
        (*m_HitCollection->GetVector());

    for (GRPAbsorberHit *aPreviousHit : HCVector)
    {
        if (aPreviousHit->GetTrackID() == trackID)
        {
            return false;
        }
    }

    const G4float kineticEnergy =
        static_cast<G4float>(preStepPoint->GetKineticEnergy());
    const G4ThreeVector position = preStepPoint->GetPosition();
    const G4ThreeVector momentum = preStepPoint->GetMomentum();
    const G4float time = static_cast<G4float>(aTrack->GetGlobalTime());
    const G4float weight = static_cast<G4float>(aTrack->GetWeight());

    const G4VProcess *creatorprocess = aTrack->GetCreatorProcess();

    if (creatorprocess)
    {
        CreatorProcessName = creatorprocess->GetProcessName();
    }

    GRPAbsorberHit *aNewHit = new GRPAbsorberHit(
        kineticEnergy,
        position,
        momentum,
        weight,
        particleID,
        time,
        trackID,
        parentID,
        CreatorProcessName);

    m_HitCollection->insert(aNewHit);

    return true;
}

void AbsorberSD::EndOfEvent(G4HCofThisEvent *HCE)
{

    auto analysisManager = G4AnalysisManager::Instance();
    HistoManager *myhistomanager = m_HistoandNtupleManager->GetHistoManager();
    NTupleManager *myntuplemanager =
        m_HistoandNtupleManager->GetNTupleManager();

    GRPAbsorberHitCollection *aHC =
        static_cast<GRPAbsorberHitCollection *>(HCE->GetHC(m_hcID));

    constexpr G4float pi = static_cast<G4float>(CLHEP::pi);

    if (analysisManager->IsActive() && aHC)
    {
        for (unsigned int j = 0; j < aHC->entries(); j++)
        {
            GRPAbsorberHit *thisHit = (*aHC)[j];
            const G4int particleID = thisHit->GetPartID();
            const G4int parentID = thisHit->GetParentID();
            const G4float kineticEnergy = thisHit->GetEkin();
            const G4float weight = thisHit->GetWeight();
            const G4float x = thisHit->GetX();
            const G4float y = thisHit->GetY();
            const G4float z = thisHit->GetZ();
            const G4ThreeVector momentum = thisHit->GetMomentum();
            const G4float px = thisHit->GetPX();
            const G4float py = thisHit->GetPY();
            const G4float pz = thisHit->GetPZ();
            const G4float hittime = thisHit->GetGlobalTime();
            const G4float theta_mrad =
                static_cast<G4float>(pi - momentum.theta());
            const G4float phi = static_cast<G4float>(momentum.phi());
            const G4String creatorprocessname = thisHit->GetCreatorProcess();

            const bool isMuorPi =
                (particleID == ParticleID::pionminusID ||
                 particleID == ParticleID::pionplusID ||
                 particleID == ParticleID::muonminusID ||
                 particleID == ParticleID::muonplusID);
            const bool isPrimary = parentID == 0;

            // Check if particle is of interest, i.e. listed in m_ParticleList
            const G4bool particleinvector =
                (std::find(
                     m_ParticleList.begin(),
                     m_ParticleList.end(),
                     particleID) != m_ParticleList.end());
            if (isPrimary)
            {
                const G4int histeneid = myhistomanager->GetPrimaryEneId();
                const G4int histoxyid = myhistomanager->GetPrimaryxyId();
                const G4int histotxtyid = myhistomanager->GetPrimarytxtyId();
                const G4int histthetaid = myhistomanager->GetPrimaryThetaId();
                const G4int histphiid = myhistomanager->GetPrimaryPhiId();
                const G4int ntupleid = myntuplemanager->GetPrimaryId();
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
                    analysisManager->FillH2(histoxyid, x, y, weight);
                }
                if (analysisManager->GetH2Activation(histotxtyid))
                {
                    analysisManager->FillH2(
                        histotxtyid,
                        std::atan2(px, pz),
                        std::atan2(py, pz),
                        weight);
                }
                // Filling Ntuple
                if (myntuplemanager->GetIdActivation(ntupleid))
                {
                    // Filling the Ntuples
                    analysisManager->FillNtupleFColumn(ntupleid, 0, x);
                    analysisManager->FillNtupleFColumn(ntupleid, 1, y);
                    analysisManager->FillNtupleFColumn(ntupleid, 2, z);
                    analysisManager->FillNtupleFColumn(ntupleid, 3, px);
                    analysisManager->FillNtupleFColumn(ntupleid, 4, py);
                    analysisManager->FillNtupleFColumn(ntupleid, 5, pz);
                    analysisManager->FillNtupleFColumn(ntupleid, 6, weight);
                    analysisManager->FillNtupleFColumn(ntupleid, 7, hittime);
                    analysisManager->AddNtupleRow(ntupleid);
                }
            }
            else
            {
                // We only define histograms for particles of interest
                if (particleinvector)
                {
                    const G4int histeneid =
                        myhistomanager->GetEneID(particleID);
                    const G4int histoxyid = myhistomanager->GetxyID(particleID);
                    const G4int histotxtyid =
                        myhistomanager->GetthetaxtehtayID(particleID);
                    const G4int histthetaid =
                        myhistomanager->GetthetaID(particleID);
                    const G4int histphiid =
                        myhistomanager->GetphiID(particleID);

                    // Filling histograms
                    if (analysisManager->GetH1Activation(histeneid))
                    {
                        analysisManager->FillH1(
                            histeneid, kineticEnergy, weight);
                    }
                    if (analysisManager->GetH1Activation(histthetaid))
                    {
                        analysisManager->FillH1(
                            histthetaid, theta_mrad, weight);
                    }
                    if (analysisManager->GetH1Activation(histphiid))
                    {
                        analysisManager->FillH1(histphiid, phi, weight);
                    }
                    if (analysisManager->GetH2Activation(histoxyid))
                    {
                        analysisManager->FillH2(histoxyid, x, y, weight);
                    }
                    if (analysisManager->GetH2Activation(histotxtyid))
                    {
                        analysisManager->FillH2(
                            histotxtyid,
                            std::atan2(px, pz),
                            std::atan2(py, pz),
                            weight);
                    }
                }
                // Filling Ntuple
                const G4int ntupleid = myntuplemanager->GetNtupleID(particleID);

                if (myntuplemanager->GetIdActivation(ntupleid))
                {
                    // Filling the Ntuples
                    analysisManager->FillNtupleFColumn(ntupleid, 0, x);
                    analysisManager->FillNtupleFColumn(ntupleid, 1, y);
                    analysisManager->FillNtupleFColumn(ntupleid, 2, z);
                    analysisManager->FillNtupleFColumn(ntupleid, 3, px);
                    analysisManager->FillNtupleFColumn(ntupleid, 4, py);
                    analysisManager->FillNtupleFColumn(ntupleid, 5, pz);
                    analysisManager->FillNtupleFColumn(ntupleid, 6, weight);
                    analysisManager->FillNtupleFColumn(ntupleid, 7, hittime);
                    analysisManager->FillNtupleSColumn(
                        ntupleid, 8, creatorprocessname);
                    if (isMuorPi)
                    {
                        const G4float charge =
                            (particleID == ParticleID::muonminusID) ||
                                (particleID == ParticleID::pionminusID)
                            ? -1
                            : 1;
                        analysisManager->FillNtupleFColumn(ntupleid, 9, charge);
                    }
                    else if (!particleinvector)
                    {
                        analysisManager->FillNtupleIColumn(
                            ntupleid, 9, particleID);
                    }
                    analysisManager->AddNtupleRow(ntupleid);
                }
            }
        }
    }
}
