/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyRunAction.hpp>

MyRunAction::MyRunAction(HistandNTupleManager *myanalysismanager)
    : G4UserRunAction()
{
    // Associate the histrogram and ntuple manager
    m_HistoandNtupleManager = myanalysismanager;
    m_HistoandNtupleManager->Book();
}

MyRunAction::~MyRunAction()
{
}

void MyRunAction::BeginOfRunAction(const G4Run *)
{
    // Getting the run manager
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    // inform the runManager to save random number seed
    runmanager->SetRandomNumberStore(false);

    // Checking if particles are read from file and issuing a warning if beamon requests more
    // particles than available
    const MyActionInitialization *myactinitpointer =
        static_cast<const MyActionInitialization *>(runmanager->GetUserActionInitialization());
    const MyPrimaryGeneratorAction *myprimarygenerationpointer =
        static_cast<const MyPrimaryGeneratorAction *>(runmanager->GetUserPrimaryGeneratorAction());
    // Note: if condition necessary since there is no action object for master when in MT mode
    if (myactinitpointer)
    {
        m_partsfromfile = myactinitpointer->GetIfParticlesFromFile();
    }

    m_Numberofeventsthisrun = runmanager->GetNumberOfEventsToBeProcessed();
    if (myprimarygenerationpointer)
    {
        if (m_partsfromfile)
        {
            G4int nparts = myprimarygenerationpointer->GetSource()->GetNParticlesInFile();
            if (m_Numberofeventsthisrun > nparts)
            {
                G4ExceptionDescription msg;
                msg << "The number of particles requested (" << runmanager->GetNumberOfEventsToBeProcessed();
                msg << ") is greater than the particles available in the input file (" << nparts << ").";
                msg << " Stored particles will be thus used more than once";
                G4Exception("MyRunAction::BeginOfRunAction()",
                            "GRAPPA::MANY_PARTICLES_REQUESTED", JustWarning, msg);
            }
        }
    }
    //   Open the analysis file
    m_HistoandNtupleManager->OpenFile();
}

void MyRunAction::EndOfRunAction(const G4Run *run)
{
    // Getting the run manager
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0)
        return;
    const MyPrimaryGeneratorAction *myprimarygenerationpointer =
        static_cast<const MyPrimaryGeneratorAction *>(runmanager->GetUserPrimaryGeneratorAction());

    if (myprimarygenerationpointer)
    {
        myprimarygenerationpointer->GetSource()->AddTotalParticlesSimulated(m_Numberofeventsthisrun);
    }
    // Write and close analysis files
    m_HistoandNtupleManager->FinishAnalysis();
}
