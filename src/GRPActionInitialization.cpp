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

#include <GRPActionInitialization.hpp>

GRPActionInitialization::GRPActionInitialization(
    HistandNTupleManager *myanalysismanager)
{
    m_HistandNTupleManager = myanalysismanager;
}

GRPActionInitialization::~GRPActionInitialization() {}

// Defining User Run Action of Master thread (only used in multi-threaded mode).
// Not invoked in sequential mode.
void GRPActionInitialization::BuildForMaster() const
{
    GRPRunAction *runAction = new GRPRunAction(m_HistandNTupleManager);
    SetUserAction(runAction);
    runAction->SetContainer(m_myParticleContainer);
}

// Defining User Run Action in sequential mode as well as for the workers in multi-threaded mode.
void GRPActionInitialization::Build() const
{
    // Mandatory user action class
    // Instantiating primary particle generator
    GRPPrimaryGeneratorAction *primarygeneratoraction =
        new GRPPrimaryGeneratorAction(m_myParticleContainer->GetUseFile());
    SetUserAction(primarygeneratoraction);

    GRPRunAction *runAction = new GRPRunAction(m_HistandNTupleManager);
    SetUserAction(runAction);
    runAction->SetContainer(m_myParticleContainer);
    GRPTrackingAction *trackingAction =
        new GRPTrackingAction(m_HistandNTupleManager);
    SetUserAction(trackingAction);
}
