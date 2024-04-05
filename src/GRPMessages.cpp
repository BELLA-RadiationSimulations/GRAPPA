// Copyright 2021-2024
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
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
    G4cout << "                        GRAPPA                       " << G4endl;
    G4cout << " =================================================== " << G4endl;
#if defined(GRAPPA_VERSION)
    G4cout << " Running GRAPPA version " << MAKE_STR(GRAPPA_VERSION)
           << ", compiled using Geant4 version " << MAKE_STR(Geant4_VERSION)
           << G4endl;
    G4cout << G4endl;
#endif
#if defined(GRAPPA_DESCRIPTION) && defined(GRAPPA_URL)
    G4cout << " GRAPPA: " << MAKE_STR(GRAPPA_DESCRIPTION) << G4endl;
    G4cout << " More info at " << MAKE_STR(GRAPPA_URL) << G4endl;
#endif
}

void PrintFinalMessage(const G4Timer &timer)
{

    G4cout
        << "==================================================================="
        << G4endl;
    G4cout << " Simulation timing: " << G4endl;
    G4cout << "    User elapsed time   => "
           << timer.GetUserElapsed() * (CLHEP::second / CLHEP::hour)
           << " h   = "
           << timer.GetUserElapsed() * (CLHEP::second / CLHEP::minute)
           << " min   = " << timer.GetUserElapsed() << " s." << G4endl;
    G4cout << "    Real elapsed time   => "
           << timer.GetRealElapsed() * (CLHEP::second / CLHEP::hour)
           << " h   = "
           << timer.GetRealElapsed() * (CLHEP::second / CLHEP::minute)
           << " min   = " << timer.GetRealElapsed() << " s." << G4endl;
    G4cout << "    System elapsed time => "
           << timer.GetSystemElapsed() * (CLHEP::second / CLHEP::hour)
           << " h   = "
           << timer.GetSystemElapsed() * (CLHEP::second / CLHEP::minute)
           << " min   = " << timer.GetSystemElapsed() << " s." << G4endl;

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
