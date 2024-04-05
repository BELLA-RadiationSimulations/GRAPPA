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

#pragma once

#include <globals.hh>
#include <G4UserTrackingAction.hh>
#include <G4Track.hh>

#include <GRPHistandNTupleManager.hpp>

class GRPTrackingAction : public G4UserTrackingAction
{
public:
    GRPTrackingAction(HistandNTupleManager *myanalysismanager);
    ~GRPTrackingAction();

    // Action performed at the generation of the particle event track
    virtual void PreUserTrackingAction(const G4Track *);

private:
    HistandNTupleManager *m_HistandNTupleManager;
};
