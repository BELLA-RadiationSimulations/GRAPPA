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

#include <GRPMessages.hpp>

void PrintWelcomeMessage()
{
    G4cout << " =================================================== " << G4endl;
    G4cout << "  Geant4 RApid Pair Production Application (GRAPPA)  " << G4endl;
    G4cout << " =================================================== " << G4endl;
    G4cout << G4endl;
    G4cout << " =================================================== " << G4endl;
    G4cout << "\tGRAPPA compiled for " << MAKE_STR(CAVE) << " analysis "
           << G4endl;
    G4cout << " =================================================== " << G4endl;
#if defined(GRAPPA_VERSION)
    G4cout << " Running GRAPPA version " << MAKE_STR(GRAPPA_VERSION) << G4endl;
    G4cout << G4endl;
#endif
#if defined(GRAPPA_DESCRIPTION) && defined(GRAPPA_URL)
    G4cout << " GRAPPA: " << MAKE_STR(GRAPPA_DESCRIPTION) << G4endl;
    G4cout << " More info at " << MAKE_STR(GRAPPA_URL) << G4endl;
    G4cout << G4endl;
#endif
}

void PrintFinalMessage(std::shared_ptr<G4Timer> timer)
{

    G4cout
        << "==================================================================="
        << G4endl;
    G4cout << " Simulation timing: " << G4endl;
    G4cout << "    User elapsed time   => " << timer->GetUserElapsed() / 3600
           << " h   = " << timer->GetUserElapsed() / 60
           << " min   = " << timer->GetUserElapsed() << " s." << G4endl;
    G4cout << "    Real elapsed time   => " << timer->GetRealElapsed() / 3600
           << " h   = " << timer->GetRealElapsed() / 60
           << " min   = " << timer->GetRealElapsed() << " s." << G4endl;
    G4cout << "    System elapsed time => " << timer->GetSystemElapsed() / 3600
           << " h   = " << timer->GetSystemElapsed() / 60
           << " min   = " << timer->GetSystemElapsed() << " s." << G4endl;

    G4cout << " ================================================== " << G4endl;
    G4cout << "        Program GRAPPA ran successfully  " << G4endl;
    G4cout << " ================================================== " << G4endl;
}

void PrintFinalMessage()
{

    G4cout << " ================================================== " << G4endl;
    G4cout << "        Program GRAPPA ran successfully  " << G4endl;
    G4cout << " ================================================== " << G4endl;
}
