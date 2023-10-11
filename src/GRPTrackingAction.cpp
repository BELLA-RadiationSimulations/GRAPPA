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

#include <GRPTrackingAction.hpp>

GRPTrackingAction::GRPTrackingAction(HistandNTupleManager *myanalysismanager)
    : G4UserTrackingAction()
{
    m_HistandNTupleManager = myanalysismanager;
}

GRPTrackingAction::~GRPTrackingAction() {}

void GRPTrackingAction::PreUserTrackingAction(const G4Track *track)
{

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    HistoManager *histoman = m_HistandNTupleManager->GetHistoManager();

    G4int pID = track->GetParentID();

    G4bool isanalysisactive, isntupleactive;

    // Check if analysis is active
    isanalysisactive = analysisManager->IsActive();

    // fill ntuple of primary particles
    if ((pID == 0) && isanalysisactive)
    {
        G4ThreeVector vertex = track->GetPosition();
        G4ThreeVector momentum = track->GetMomentum();
        G4float x = vertex.x(), y = vertex.y(), z = vertex.z();
        G4float px = momentum.x(), py = momentum.y(), pz = momentum.z();
        G4double kineticenergy = track->GetKineticEnergy();

        G4int histoxyid = histoman->GetPrimaryInitialxyId();
        G4int histozxid = histoman->GetPrimaryInitialzxId();
        G4int histoEid = histoman->GetPrimaryInitialEneId();
        G4int histotxtyid = histoman->GetPrimaryInitialtxtyId();
        G4int histoxpxid = histoman->GetPrimaryInitialxpxId();
        G4int histoypyid = histoman->GetPrimaryInitialypyId();
        G4int ntupleid =
            m_HistandNTupleManager->GetNTupleManager()->GetPrimaryInitialId();
        G4double pz_inv = 1 / pz;

        // Fill NTuple with initial particles
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

        // Fill Histograms with initial particles
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
