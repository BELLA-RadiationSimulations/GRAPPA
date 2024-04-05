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

void GRPActionInitialization::BuildForMaster() const
{
    GRPRunAction *runAction = new GRPRunAction(m_HistandNTupleManager);
    SetUserAction(runAction);
    runAction->SetContainer(m_myParticleContainer);
}

void GRPActionInitialization::Build() const
{
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
