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

#include <G4VUserActionInitialization.hh>

#include <GRPParticleContainer.hpp>
#include <GRPPrimaryGeneratorAction.hpp>
#include <GRPTrackingAction.hpp>
#include <GRPRunAction.hpp>

class GRPActionInitialization : public G4VUserActionInitialization
{
public:
    // GRPActionInitialization is constructed when the program starts, but
    // it is only built through GRPActionInitialization::Build() when run is
    // initialized.
    GRPActionInitialization(HistandNTupleManager *myanalysismanager);
    virtual ~GRPActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

    GRPParticleContainer *GetContainer() { return m_myParticleContainer; }
    void SetContainer(GRPParticleContainer *mycontainer)
    {
        m_myParticleContainer = mycontainer;
    }

private:
    HistandNTupleManager *m_HistandNTupleManager;
    // Observer pointer to the particle container type
    GRPParticleContainer *m_myParticleContainer;
};
