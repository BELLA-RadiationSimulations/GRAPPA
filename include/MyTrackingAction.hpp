/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <globals.hh>
#include <G4UserTrackingAction.hh>
#include <G4Track.hh>

#include <MyHistandNTupleManager.hpp>

class MyTrackingAction : public G4UserTrackingAction
{
public:
    MyTrackingAction(HistandNTupleManager* myanalysismanager);
    ~MyTrackingAction();

    // Action performed at the generation of the particle event track
    virtual void  PreUserTrackingAction(const G4Track*);
private:
    HistandNTupleManager* m_HistandNTupleManager;
};