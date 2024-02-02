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

#include <grappa.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    // Welcome message
    PrintWelcomeMessage();
    // Detect interactive mode (if no arguments) and define UI session

    G4UIExecutive *ui = nullptr;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Initialize visualization
    //
    G4VisManager *visManager = new G4VisExecutive("Quiet");
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Getting run start time
    G4Timer timer = G4Timer();
    timer.Start();
    G4cout << " Program started at \t" << timer.GetClockTime() << G4endl;

#ifdef G4MULTITHREADED
    G4RunManager *runManager = G4RunManagerFactory::CreateRunManager();
#else
    G4RunManager *runManager =
        G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);
#endif

    runManager->SetVerboseLevel(0);

    // Registering signals for program safe interruption
    RegisterSignals();

    // Constructing the custom analysis manager used to create histograms
    // and ntuples in the Sensitive Detectors.
    HistandNTupleManager *myanalysismanager = new HistandNTupleManager();

    // Constructing the physics list
    // The last characters in the physics list represent the ElectroMagnetic
    // component By default, FTFP_BERT constructs the standart EM.
    // "_LIV_" stands for Livermore, "_PEN" to Penelope.
    //
    G4PhysListFactory factory;
    G4VModularPhysicsList *physicsList =
        factory.GetReferencePhysList("FTFP_BERT_EMZ");
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);
    // FTFP_BERT should be used instead if primary articles energy is <5GeV;


    // Introducing a particle container
    // that serves if we need to read particles from file
    GRPParticleContainer myParticleContainer = GRPParticleContainer();

    // Mandatory class
    // Constructing actions
    // It takes as input a pointer to the custom analysis manager
    GRPActionInitialization *myActionInitialization =
        new GRPActionInitialization(myanalysismanager);
    myActionInitialization->SetContainer(&myParticleContainer);
    runManager->SetUserInitialization(myActionInitialization);

    // Mandatory class
    // Constructing the detectors.
    // It takes as input a pointer to the custom analysis manager
    runManager->SetUserInitialization(
        new GRPDetectorConstruction(myanalysismanager));

    if (ui)
    {
        // interactive mode
        ui->SessionStart();
        delete ui;
    }
    else
    {
        // batch mode
        const G4String command = "/control/execute ";
        const G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // Getting run end time
    timer.Stop();

    // Final cleanup
    // Must delete: run manager, visualization manager, analysis manager
    delete runManager;
    delete visManager;
    delete myanalysismanager;

    PrintFinalMessage(timer);

    return 0;
}
