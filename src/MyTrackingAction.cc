/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyTrackingAction.hpp>

MyTrackingAction::MyTrackingAction(HistandNTupleManager* myanalysismanager) :G4UserTrackingAction()
{
    m_HistandNTupleManager = myanalysismanager;
}


MyTrackingAction::~MyTrackingAction()
{

}

void MyTrackingAction::PreUserTrackingAction(const G4Track* track)
{

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    HistoManager* histoman = m_HistandNTupleManager->GetHistoManager();

    G4int pID = track->GetParentID();

    // fill ntuple of primary particles
    if (pID == 0) {

        G4ThreeVector vertex = track->GetPosition();
        G4ThreeVector momentum = track->GetMomentum();
        G4double x = vertex.x(), y = vertex.y(), z = vertex.z();
        G4double px = momentum.x(), py = momentum.y(), pz = momentum.z();
        G4double kineticenergy = track->GetKineticEnergy();

        G4int histoxyid = histoman->GetPrimaryInitialxyId();
        G4int histoEid = histoman->GetPrimaryInitialEneId();
        G4int histotxtyid = histoman->GetPrimaryInitialtxtyId();
        G4int histoxpxid = histoman->GetPrimaryInitialxpxId();
        G4int histoypyid = histoman->GetPrimaryInitialypyId();
        G4int ntupleid = m_HistandNTupleManager->GetNTupleManager()->GetPrimaryInitialId();
        G4double pz_inv = 1 / pz;

        // Fill NTuple with initial particles
        analysisManager->FillNtupleDColumn(ntupleid, 0, x);
        analysisManager->FillNtupleDColumn(ntupleid, 1, y);
        analysisManager->FillNtupleDColumn(ntupleid, 2, z);
        analysisManager->FillNtupleDColumn(ntupleid, 3, px);
        analysisManager->FillNtupleDColumn(ntupleid, 4, py);
        analysisManager->FillNtupleDColumn(ntupleid, 5, pz);
        analysisManager->AddNtupleRow(ntupleid);

        // Fill Histograms with initial particles
        analysisManager->FillH2(histoxyid, x, y);
        analysisManager->FillH2(histotxtyid, px * pz_inv, py * pz_inv);
        analysisManager->FillH2(histoxpxid, x, px);
        analysisManager->FillH2(histoypyid, y, py);
        analysisManager->FillH1(histoEid, kineticenergy);

    }
}
