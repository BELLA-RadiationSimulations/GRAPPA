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

#include <GRPRun.hpp>

void GRPRun::Merge(const G4Run *aRun) { G4Run::Merge(aRun); }

void GRPRun::RecordEvent(const G4Event *anEvent)
{
    G4Run::RecordEvent(anEvent);
}
