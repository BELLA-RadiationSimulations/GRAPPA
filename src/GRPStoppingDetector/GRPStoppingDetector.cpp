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

#include <G4AnalysisManager.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>

#include <GRPStoppingDetector.hpp>

// Primaries, electrons, positrons and photons absorbing layer
GRPStoppingDetector::GRPStoppingDetector(const G4String &name)
    : G4VSensitiveDetector(name)
{
    //=============================================
    // This script extracts the name of the detector
    // just in case it is defined as a "path"
    size_t pos = name.find_last_of("/");
    G4String finalname;
    if (pos != G4String::npos)
    {
        finalname = name.substr(pos + 1);
    }
    else
    {
        finalname = name;
    }
    //=============================================
    const G4String HCname = "HC_" + finalname;
    collectionName.insert(HCname);
    // Booking the Ntuple from the analysis manager
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    m_stoppedPositionNtupleNumber = analysisManager->CreateNtuple(
        finalname, "Stopping position of particles");
    analysisManager->CreateNtupleIColumn("PartID");
    analysisManager->CreateNtupleFColumn("Ekin");
    analysisManager->CreateNtupleFColumn("x");
    analysisManager->CreateNtupleFColumn("y");
    analysisManager->CreateNtupleFColumn("z");
    analysisManager->CreateNtupleFColumn("time");
    analysisManager->CreateNtupleFColumn("weight");
    analysisManager->FinishNtuple(m_stoppedPositionNtupleNumber);
    analysisManager->SetNtupleActivation(m_stoppedPositionNtupleNumber, true);
}

GRPStoppingDetector::~GRPStoppingDetector() = default;

void GRPStoppingDetector::Initialize(G4HCofThisEvent *HCE)
{

    // Create hit collection
    m_HitCollection =
        new GRPStoppingHitCollection(this->GetName(), collectionName[0]);

    // Add this collection in hce

    m_hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(m_hcID, m_HitCollection);
}

G4bool GRPStoppingDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{

    // Access track information
    const G4Track *aTrack = aStep->GetTrack();
    const G4int trackID = aTrack->GetTrackID();
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    const G4int particleID = particle->GetPDGEncoding();
    const G4float weight = static_cast<G4float>(aTrack->GetWeight());
    const G4float stoptime = static_cast<G4float>(aTrack->GetGlobalTime());
    G4TrackStatus trackStatus = aTrack->GetTrackStatus();

    const G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    // Want to know the kinetic energy before the particle stops
    const G4float ekin = static_cast<G4float>(preStepPoint->GetKineticEnergy());
    const G4ThreeVector position = postStepPoint->GetPosition();

    if (trackStatus == G4TrackStatus::fStopAndKill)
    {
        // Do not save the same hit twice, so check the trackID
        // Although this should be trivially true as particles are killed
        // after they're registered

        const std::vector<GRPStoppingHit *> HCVector =
            (*m_HitCollection->GetVector());

        for (GRPStoppingHit *aPreviousHit : HCVector)
        {
            if (aPreviousHit->GetTrackID() == trackID)
            {
                return false;
            }
        }

        GRPStoppingHit *aNewHit = new GRPStoppingHit(
            ekin, position, weight, particleID, stoptime, trackID);
        m_HitCollection->insert(aNewHit);
        return true;
    }

    return false;
}

void GRPStoppingDetector::EndOfEvent(G4HCofThisEvent *HCE)
{

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    GRPStoppingHitCollection *aHC =
        static_cast<GRPStoppingHitCollection *>(HCE->GetHC(m_hcID));

    const G4int runID =
        G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();

    // Second round: accumulate results
    // And write on the time-Energy ntuple
    if (aHC)
    {
        if (analysisManager->IsActive())
        {
            for (unsigned int j = 0; j < aHC->entries(); j++)
            {
                const GRPStoppingHit *thisHit = (*aHC)[j];
                const G4int particleID = thisHit->GetPartID();
                const G4float ekin = thisHit->GetEkin();
                const G4float weight = thisHit->GetWeight();
                const G4float time = thisHit->GetGlobalTime();
                const G4float x = thisHit->GetPosition().x();
                const G4float y = thisHit->GetPosition().y();
                const G4float z = thisHit->GetPosition().z();

                analysisManager->FillNtupleIColumn(
                    m_stoppedPositionNtupleNumber, 0, particleID);
                analysisManager->FillNtupleFColumn(
                    m_stoppedPositionNtupleNumber, 1, ekin);
                analysisManager->FillNtupleFColumn(
                    m_stoppedPositionNtupleNumber, 2, x);
                analysisManager->FillNtupleFColumn(
                    m_stoppedPositionNtupleNumber, 3, y);
                analysisManager->FillNtupleFColumn(
                    m_stoppedPositionNtupleNumber, 4, z);
                analysisManager->FillNtupleFColumn(
                    m_stoppedPositionNtupleNumber, 5, time);
                analysisManager->FillNtupleFColumn(
                    m_stoppedPositionNtupleNumber, 5, weight);
                analysisManager->AddNtupleRow(m_stoppedPositionNtupleNumber);
            }
        }
    }
}
