// Copyright 2022 Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause
// 

#include <GRPRunAction.hpp>

GRPRunAction::GRPRunAction(G4bool useGPS, HistandNTupleManager *myanalysismanager)
    : G4UserRunAction()
{
    // Associate the histrogram and ntuple manager
    m_HistoandNtupleManager = myanalysismanager;
    m_HistoandNtupleManager->Book();
    m_partsfromfile = !useGPS;
    m_timer = std::make_unique<G4Timer>();
    DefineCommands();
}

GRPRunAction::~GRPRunAction()
{
}

void GRPRunAction::BeginOfRunAction(const G4Run *)
{
    // Getting the run manager
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    // inform the runManager to save random number seed
    runmanager->SetRandomNumberStore(false);
    m_timer->Start();

    // Checking if particles are read from file and issuing a warning if beamon requests more
    // particles than available
    const GRPPrimaryGeneratorAction *myprimarygenerationpointer =
        static_cast<const GRPPrimaryGeneratorAction *>(runmanager->GetUserPrimaryGeneratorAction());
    // Note: if condition necessary since there is no action object for master when in MT mode

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
                G4Exception("GRPRunAction::BeginOfRunAction()",
                            "GRAPPA::MANY_PARTICLES_REQUESTED", JustWarning, msg);
            }
        }
    }
    G4AnalysisManager::Instance()->Reset();
    //   Open the analysis file
    m_HistoandNtupleManager->OpenFile();
}

void GRPRunAction::EndOfRunAction(const G4Run *run)
{
    
    // Stopping the run timer
    m_timer->Stop();

    // Getting the run manager
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0)
        return;
    const GRPPrimaryGeneratorAction *myprimarygenerationpointer =
        static_cast<const GRPPrimaryGeneratorAction *>(runmanager->GetUserPrimaryGeneratorAction());

    if (myprimarygenerationpointer)
    {
        myprimarygenerationpointer->GetSource()->AddTotalParticlesSimulated(m_Numberofeventsthisrun);
    }
    // Write and close analysis files
    m_HistoandNtupleManager->FinishAnalysis();

    // Print the run timing
    if (IsMaster())
    {
        G4cout << " Finished Run "<< runmanager->GetCurrentRun()->GetRunID() << G4endl;
        G4cout << "==========================================================================" << G4endl;
        G4cout << " Timing for the current Run " << runmanager->GetCurrentRun()->GetRunID() << ":" << G4endl;
        G4cout << "    User elapsed time   => " << m_timer->GetUserElapsed() / 3600 << " h   = "
            << m_timer->GetUserElapsed() / 60 << " min   = " << m_timer->GetUserElapsed() << " s." << G4endl;
        G4cout << "    Real elapsed time   => " << m_timer->GetRealElapsed() / 3600 << " h   = "
            << m_timer->GetRealElapsed() / 60 << " min   = " << m_timer->GetRealElapsed() << " s." << G4endl;
        G4cout << "    System elapsed time => " << m_timer->GetSystemElapsed() / 3600 << " h   = "
            << m_timer->GetSystemElapsed() / 60 << " min   = " << m_timer->GetSystemElapsed() << " s." << G4endl;
        G4cout << "==========================================================================" << G4endl;
    }
}

void GRPRunAction::DefineCommands()
{
    // Messenger class with custom ntuple commands
    //
    // define command directory using generic messenger class
    m_AMessenger = std::make_shared<G4GenericMessenger>(this, "/ntuplecontrol/", "Custom commands to personalize ntuples");

    // Print the list of ntuples
    G4GenericMessenger::Command &printcommand = m_AMessenger->DeclareMethod("list", &GRPRunAction::ListNtuples,
                                                                                  "List the available ntuples");
    printcommand.SetGuidance(" List the available ntuples' ID, Name and ActivationStatus");
    printcommand.SetStates(G4State_PreInit, G4State_Idle, G4State_Init);

    G4GenericMessenger::Command &setdumpcommand = m_AMessenger->DeclareMethod("setDump", &GRPRunAction::SetNtupleDump,
                                                                                  "Set the activation status of a given Ntuple ID");
    setdumpcommand.SetGuidance(" Set the activation status of a given Ntuple ID ");
    setdumpcommand.SetStates(G4State_PreInit, G4State_Idle, G4State_Init);

}
