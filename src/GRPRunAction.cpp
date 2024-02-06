// Copyright 2021-2024
//
// Authors:
// Stanimir Kisyov, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <GRPRunAction.hpp>

GRPRunAction::GRPRunAction(HistandNTupleManager *myanalysismanager)
{
    // Associate the histrogram and ntuple manager
    m_HistoandNtupleManager = myanalysismanager;
    m_HistoandNtupleManager->Book();
    m_timer = std::make_unique<G4Timer>();
    DefineCommands();
}

GRPRunAction::~GRPRunAction() {}

G4Run *GRPRunAction::GenerateRun() { return new GRPRun(); }

void GRPRunAction::BeginOfRunAction(const G4Run *)
{
    // Getting the run manager
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    // inform the runManager to save random number seed
    runmanager->SetRandomNumberStore(false);
    m_timer->Start();

    GRPRun *run = static_cast<GRPRun *>(runmanager->GetNonConstCurrentRun());
    run->SetContainer(m_myParticleContainer);

    if (m_myParticleContainer->GetUseFile())
    {
        const G4int Numberofeventsthisrun =
            runmanager->GetNumberOfEventsToBeProcessed();
        const G4int nparts = m_myParticleContainer->GetNParticlesInFile();
        if (Numberofeventsthisrun > nparts)
        {
            G4ExceptionDescription msg;
            msg << "The number of particles requested ("
                << runmanager->GetNumberOfEventsToBeProcessed();
            msg << ") is greater than the particles available in the input "
                   "file ("
                << nparts << ").";
            msg << " Stored particles will be thus used more than once";
            G4Exception(
                "GRPRunAction::BeginOfRunAction()",
                "GRAPPA::MANY_PARTICLES_REQUESTED",
                JustWarning,
                msg);
        }
    }
    // Only reset the analysis manager if the
    // analysis is not set to backward compatibility
    // i.e., Geant4 version >= 11.
    // In previous versions Reset is not public
#if !defined(GRAPPA_USE_BACKWARDS_ANALYSIS)
    G4AnalysisManager::Instance()->Reset();
#endif
    //   Open the analysis file
    m_HistoandNtupleManager->OpenFile();
}

void GRPRunAction::EndOfRunAction(const G4Run *run)
{

    // Stopping the run timer
    m_timer->Stop();

    // Getting the run manager
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    G4AnalysisManager * analysismanager = G4AnalysisManager::Instance();
    const G4int nofEvents = run->GetNumberOfEvent();

    if (!IsMaster())
    {
        const G4int primarycountid = m_HistoandNtupleManager->GetNTupleManager()->GetPrimaryCountId();
        const G4bool primarycountactive = m_HistoandNtupleManager->GetNTupleManager()->GetIdActivation(primarycountid);
        if (primarycountactive)
        {
            analysismanager->FillNtupleIColumn(primarycountid, 0, nofEvents);
            analysismanager->AddNtupleRow(primarycountid);
        }
    }
    // Print the run timing
    if (IsMaster())
    {
        m_myParticleContainer->AddTotalParticlesSimulated(nofEvents);
        G4cout << " Finished Run " << runmanager->GetCurrentRun()->GetRunID()
               << G4endl;
        G4cout << "============================================================"
                  "=============="
               << G4endl;
        G4cout << " Timing for the current Run "
               << runmanager->GetCurrentRun()->GetRunID() << ":" << G4endl;
        G4cout << "    User elapsed time   => "
               << m_timer->GetUserElapsed() * (CLHEP::second / CLHEP::hour)
               << " h   = "
               << m_timer->GetUserElapsed() * (CLHEP::second / CLHEP::minute)
               << " min   = " << m_timer->GetUserElapsed() << " s." << G4endl;
        G4cout << "    Real elapsed time   => "
               << m_timer->GetRealElapsed() * (CLHEP::second / CLHEP::hour)
               << " h   = "
               << m_timer->GetRealElapsed() * (CLHEP::second / CLHEP::minute)
               << " min   = " << m_timer->GetRealElapsed() << " s." << G4endl;
        G4cout << "    System elapsed time => "
               << m_timer->GetSystemElapsed() * (CLHEP::second / CLHEP::hour)
               << " h   = "
               << m_timer->GetSystemElapsed() * (CLHEP::second / CLHEP::minute)
               << " min   = " << m_timer->GetSystemElapsed() << " s." << G4endl;
        G4cout << "============================================================"
                  "=============="
               << G4endl;
    }

    // Write and close analysis files
    m_HistoandNtupleManager->FinishAnalysis();
}

void GRPRunAction::DefineCommands()
{
    // Messenger class with custom ntuple commands
    //
    // define command directory using generic messenger class
    m_AMessenger = std::make_shared<G4GenericMessenger>(
        this, "/ntuplecontrol/", "Custom commands to personalize ntuples");

    // Print the list of ntuples
    G4GenericMessenger::Command &printcommand = m_AMessenger->DeclareMethod(
        "list", &GRPRunAction::ListNtuples, "List the available ntuples");
    printcommand.SetGuidance(
        " List the available ntuples' ID, Name and ActivationStatus");
    printcommand.SetStates(G4State_PreInit, G4State_Idle, G4State_Init);

    G4GenericMessenger::Command &setdumpcommand = m_AMessenger->DeclareMethod(
        "setDump",
        &GRPRunAction::SetNtupleDump,
        "Set the activation status of a given Ntuple ID");
    setdumpcommand.SetGuidance(
        " Set the activation status of a given Ntuple ID ");
    setdumpcommand.SetStates(G4State_PreInit, G4State_Idle, G4State_Init);
}
