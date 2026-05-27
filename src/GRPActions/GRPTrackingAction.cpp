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

#include <GRPTrackingAction.hpp>

GRPTrackingAction::GRPTrackingAction(HistandNTupleManager *myanalysismanager)
{
    m_HistandNTupleManager = myanalysismanager;
}

GRPTrackingAction::~GRPTrackingAction() {}

void GRPTrackingAction::PreUserTrackingAction(const G4Track *track)
{

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    HistoManager *histoman = m_HistandNTupleManager->GetHistoManager();

    const G4int pID = track->GetParentID();

    G4bool isanalysisactive, isntupleactive;

    // Check if analysis is active
    isanalysisactive = analysisManager->IsActive();

    // Fill histograms and ntuple of primary particles
    if ((pID == 0) && isanalysisactive)
    {
        // Extracting particle values
        const G4ThreeVector vertex = track->GetPosition();
        const G4ThreeVector momentum = track->GetMomentum();
        const G4float x = static_cast<G4float>(vertex.x());
        const G4float y = static_cast<G4float>(vertex.y());
        const G4float z = static_cast<G4float>(vertex.z());
        const G4float px = static_cast<G4float>(momentum.x());
        const G4float py = static_cast<G4float>(momentum.y());
        const G4float pz = static_cast<G4float>(momentum.z());
        const G4double kineticenergy = track->GetKineticEnergy();
        const G4double pz_inv = 1 / pz;

        // Getting histogram and ntuple IDs
        const G4int histoxyid = histoman->GetPrimaryInitialxyId();
        const G4int histozxid = histoman->GetPrimaryInitialzxId();
        const G4int histoEid = histoman->GetPrimaryInitialEneId();
        const G4int histotxtyid = histoman->GetPrimaryInitialtxtyId();
        const G4int histoxpxid = histoman->GetPrimaryInitialxpxId();
        const G4int histoypyid = histoman->GetPrimaryInitialypyId();
        const G4int ntupleid =
            m_HistandNTupleManager->GetNTupleManager()->GetPrimaryInitialId();

        // Fill nuple with primary particles
        isntupleactive =
            m_HistandNTupleManager->GetNTupleManager()->GetIdActivation(
                ntupleid);
        if (isntupleactive)
        {
            analysisManager->FillNtupleFColumn(ntupleid, 0, x);
            analysisManager->FillNtupleFColumn(ntupleid, 1, y);
            analysisManager->FillNtupleFColumn(ntupleid, 2, z);
            analysisManager->FillNtupleFColumn(ntupleid, 3, px);
            analysisManager->FillNtupleFColumn(ntupleid, 4, py);
            analysisManager->FillNtupleFColumn(ntupleid, 5, pz);
            analysisManager->AddNtupleRow(ntupleid);
        }

        // Fill histograms with primary particles
        if (analysisManager->GetH2Activation(histoxyid))
        {
            analysisManager->FillH2(histoxyid, x, y);
        }
        if (analysisManager->GetH2Activation(histozxid))
        {
            analysisManager->FillH2(histozxid, z, x);
        }
        if (analysisManager->GetH2Activation(histotxtyid))
        {
            analysisManager->FillH2(histotxtyid, px * pz_inv, py * pz_inv);
        }
        if (analysisManager->GetH2Activation(histoxpxid))
        {
            analysisManager->FillH2(histoxpxid, x, px);
        }
        if (analysisManager->GetH2Activation(histoypyid))
        {
            analysisManager->FillH2(histoypyid, y, py);
        }
        if (analysisManager->GetH1Activation(histoEid))
        {
            analysisManager->FillH1(histoEid, kineticenergy);
        }
    }
}
