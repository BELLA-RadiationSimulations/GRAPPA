/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MySDAbsorber.hpp>

AbsorberSD::AbsorberSD(G4String name, HistandNTupleManager* myanalysismanager) :G4VSensitiveDetector(name)
{
    m_HistoandNtupleManager = myanalysismanager;
}

AbsorberSD::~AbsorberSD()
{

}

void AbsorberSD::Initialize(G4HCofThisEvent*)
{

}

G4bool AbsorberSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Analysis manager for histograms
    auto analysisManager = G4AnalysisManager::Instance();
    G4double kineticEnergy, time, pz_inv;
    G4ThreeVector position, momentum;

    // Access track information
    G4Track* aTrack = step->GetTrack();

    // Getting pre step point information and volume name
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4String thisVolumename = aTrack->GetVolume()->GetName();
    const G4ParticleDefinition* particle = aTrack->GetParticleDefinition();
    G4String name = particle->GetParticleName();
    G4int pID = aTrack->GetParentID();
    G4int histeneid, histoxyid, histotxtyid, ntupleid;

    // Pointer to current process
    const G4VProcess* CurrentProcess = preStepPoint->GetProcessDefinedStep();

    // Boolean flags for histogram and Ntuple filling
    G4bool FillHistogram = true;
    G4bool FillNtuple = true;

    // Energy filters for particles
    G4double MinPrimaryEnergy = 4 * MeV;
    G4double MinPhotonEnergy = 4 * MeV;
    G4double MinElectronEnergy = 1 * MeV;
    G4double MinPositronEnergy = 1 * MeV;

    if (CurrentProcess)
    {
        // Getting process name (it should match "Transportation")
        const G4String& StepProcessName = CurrentProcess->GetProcessName();
        if (StepProcessName == "Transportation") {
            // processing hit when entering the volume
            kineticEnergy = aTrack->GetKineticEnergy();
            position = aTrack->GetPosition();
            momentum = aTrack->GetMomentum();
            time = aTrack->GetGlobalTime();
            aTrack->SetTrackStatus(fStopAndKill);
            pz_inv = 1 / momentum.z();
        }
        else
        {
            return false;
        }

        if (pID == 0)
        {
            histeneid = m_HistoandNtupleManager->GetHistoManager()->GetPrimaryEneId();
            histoxyid = m_HistoandNtupleManager->GetHistoManager()->GetPrimaryxyId();
            histotxtyid = m_HistoandNtupleManager->GetHistoManager()->GetPrimarytxtyId();
            ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPrimaryId();
            if (kineticEnergy < MinPrimaryEnergy)
            {
                FillNtuple = false;
            }
        }
        else
        {

            if (name == "e+")
            {
                histeneid = m_HistoandNtupleManager->GetHistoManager()->GetPositronEneId();
                histoxyid = m_HistoandNtupleManager->GetHistoManager()->GetPositronxyId();
                histotxtyid = m_HistoandNtupleManager->GetHistoManager()->GetPositrontxtyId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPositronId();
                if (kineticEnergy < MinPositronEnergy)
                {
                    FillNtuple = false;
                }
            }
            else if (name == "gamma")
            {
                histeneid = m_HistoandNtupleManager->GetHistoManager()->GetGammaEneId();
                histoxyid = m_HistoandNtupleManager->GetHistoManager()->GetGammaxyId();
                histotxtyid = m_HistoandNtupleManager->GetHistoManager()->GetGammatxtyId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetGammaId();
                if (kineticEnergy < MinPhotonEnergy)
                {
                    FillNtuple = false;
                }
            }
            else if (name == "e-")
            {

                histeneid = m_HistoandNtupleManager->GetHistoManager()->GetElectronEneId();
                histoxyid = m_HistoandNtupleManager->GetHistoManager()->GetElectronxyId();
                histotxtyid = m_HistoandNtupleManager->GetHistoManager()->GetElectrontxtyId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetElectronId();
                if (kineticEnergy < MinElectronEnergy)
                {
                    FillNtuple = false;
                }

            }
            else
            {
                return false;

            }
        }

        if (FillHistogram)
        {
            // Filling the correct histogram
            analysisManager->FillH1(histeneid, kineticEnergy);
            analysisManager->FillH2(histoxyid, position.x(), position.y());
            analysisManager->FillH2(histotxtyid, momentum.x() * pz_inv, momentum.y() * pz_inv);
        }

        if (FillNtuple)
        {
            // Filling the correct Ntuple
            analysisManager->FillNtupleDColumn(ntupleid, 0, position.x());
            analysisManager->FillNtupleDColumn(ntupleid, 1, position.y());
            analysisManager->FillNtupleDColumn(ntupleid, 2, position.z());
            analysisManager->FillNtupleDColumn(ntupleid, 3, momentum.x());
            analysisManager->FillNtupleDColumn(ntupleid, 4, momentum.y());
            analysisManager->FillNtupleDColumn(ntupleid, 5, momentum.z());
            analysisManager->FillNtupleDColumn(ntupleid, 6, time);
            analysisManager->AddNtupleRow(ntupleid);
        }

        return true;
    }

    return false;

}
