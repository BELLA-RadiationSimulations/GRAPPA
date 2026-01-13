// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#pragma once

// Macro to transform preprocessor variables in strings
#define MAKE_STR(x) _MAKE_STR(x)
#define _MAKE_STR(x) #x

// Geant4
#include <G4ios.hh>
#include <G4SystemOfUnits.hh>
#include <G4Timer.hh>

void PrintWelcomeMessage();

void PrintStartMessage(const G4Timer &timer);

void PrintFinalMessage(const G4Timer &timer);
