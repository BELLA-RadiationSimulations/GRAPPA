/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyRunAction.hpp>

MyRunAction::MyRunAction(HistandNTupleManager* myanalysismanager)
    : G4UserRunAction()
    // Reference member has to be intialized with declaration
{

    // Register accumulable to the accumulable manager
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(tfoil);

    // Associate the histrogram and ntuple manager
    m_HistoandNtupleManager = myanalysismanager;
    m_HistoandNtupleManager->Book();

}

MyRunAction::~MyRunAction()
{

}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    // reset accumulables to their initial values
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

    accumulableManager->Reset();

    // Open the analysis file
    m_HistoandNtupleManager->OpenFile();

}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    // Merge accumulables 
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    // Write and close analysis files
    m_HistoandNtupleManager->FinishAnalysis();

}

void MyRunAction::ComputeTFoil(G4double tg)
{
    tfoil = std::max(tfoil.GetValue(), tg);
}
