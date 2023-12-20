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

#pragma once

// Macro to transform preprocessor variables in strings
#define MAKE_STR(x) _MAKE_STR(x)
#define _MAKE_STR(x) #x

// C++
#include <memory>

// Geant4
#include <G4ios.hh>
#include <G4Timer.hh>

void PrintWelcomeMessage();

void PrintFinalMessage(std::shared_ptr<G4Timer> timer);

void PrintFinalMessage();
