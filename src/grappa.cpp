// Copyright 2022 Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause
// 

#include <grappa.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    // Welcome message
    G4cout << " =================================================== " << G4endl;
    G4cout << "  Geant4 RApid Pair Production Application (GRAPPA)  " << G4endl;
    G4cout << " =================================================== " << G4endl;
    // Detect interactive mode (if no arguments) and define UI session

    G4UIExecutive* ui = 0;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive("Quiet");
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Getting run start time
    std::shared_ptr<G4Timer> timer = std::make_shared<G4Timer>();
    timer->Start();
    G4cout << " Program started at \t" << timer->GetClockTime() << G4endl;

#ifdef G4MULTITHREADED
    G4RunManager* runManager = G4RunManagerFactory::CreateRunManager();
#else
    G4RunManager* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);
#endif

    runManager->SetVerboseLevel(0);

    // Constructing the custom analysis manager used to create histograms
    // and ntuples in the Sensitive Detectors.
    HistandNTupleManager* myanalysismanager = new HistandNTupleManager();

    // Constructing the physics list
    // The last characters in the physics list represent the ElectroMagnetic component
    // By default, FTFP_BERT constructs the standart EM.
    // "_LIV_" stands for Livermore, "_PEN" to Penelope.
    //
    G4PhysListFactory factory;
    G4VModularPhysicsList* physicsList = factory.GetReferencePhysList("FTFP_BERT_EMZ");
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    //FTFP_BERT should be used instead if primary articles energy is <5GeV;

    // Mandatory class
    // Constructing actions
    // It takes as input a pointer to the custom analysis manager
    GRPActionInitialization* myActionInitialization =
        new GRPActionInitialization(myanalysismanager);
    runManager->SetUserInitialization(myActionInitialization);

    // Mandatory class
    // Constructing the detectors.
    // It takes as input a pointer to the custom analysis manager
    runManager->SetUserInitialization(new GRPDetectorConstruction(myanalysismanager));

    // Last, setting the verbosity of the run manager
    // RunManager prints a signal every everyevent events.
    G4int everyevent = 200;
    runManager->SetPrintProgress(everyevent);

    if (ui) {
        //interactive mode
        ui->SessionStart();
        delete ui;
    }
    else {
        //batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // Getting run end time
    timer->Stop();

    // Final cleanup
    // Must delete: run manager, visualization manager, analysis manager
    delete runManager;
    delete visManager;
    delete myanalysismanager;

    G4cout << "===================================================================" << G4endl;
    G4cout << " Simulation timing: " << G4endl;
    G4cout << "    User elapsed time   => " << timer->GetUserElapsed() / 3600 << " h   = "
        << timer->GetUserElapsed() / 60 << " min   = " << timer->GetUserElapsed() << " s." << G4endl;
    G4cout << "    Real elapsed time   => " << timer->GetRealElapsed() / 3600 << " h   = "
        << timer->GetRealElapsed() / 60 << " min   = " << timer->GetRealElapsed() << " s." << G4endl;
    G4cout << "    System elapsed time => " << timer->GetSystemElapsed() / 3600 << " h   = "
        << timer->GetSystemElapsed() / 60 << " min   = " << timer->GetSystemElapsed() << " s." << G4endl;

    G4cout << " ================================= " << G4endl;
    G4cout << "  Program GRAPPA ran successfully  " << G4endl;
    G4cout << " ================================= " << G4endl;

    return 0;
}
