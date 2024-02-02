// Copyright 2022-2024
//
// Authors:
// Stanimir Kisyov, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of BELLABD.
//
// License: BSD-3-Clause

#include <GRPRun.hpp>

void GRPRun::Merge(const G4Run *aRun) { G4Run::Merge(aRun); }

void GRPRun::RecordEvent(const G4Event *anEvent)
{
    G4Run::RecordEvent(anEvent);
}
