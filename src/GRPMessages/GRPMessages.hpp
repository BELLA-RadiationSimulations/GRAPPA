// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause-LBNL

#pragma once

// Macro to transform preprocessor variables in strings
#define MAKE_STR(x) _MAKE_STR(x)
#define _MAKE_STR(x) #x

#include <G4Timer.hh>

// Prints the header message with the application name,
// version, Geant4 linked version, etc...
void PrintWelcomeMessage();

// Prints the initial timer message
void PrintStartMessage(const G4Timer &timer);

// Prints the closing message for the application,
// which includes the execution time.
void PrintFinalMessage(const G4Timer &timer);
