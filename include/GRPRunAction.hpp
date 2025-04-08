// Copyright 2021-2025
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
#include <memory>
#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4Timer.hh>
#include <G4UnitsTable.hh>
#include <G4UserRunAction.hh>

#include <GRPActionInitialization.hpp>
#include <GRPMessages.hpp>
#include <GRPPrimaryGeneratorAction.hpp>
#include <GRPRun.hpp>
#include <GRPHistandNTupleManager.hpp>

class G4Run;

class GRPRunAction : public G4UserRunAction
{
public:
    GRPRunAction(HistandNTupleManager *myanalysismanager);
    virtual ~GRPRunAction();

    G4Run *GenerateRun() override;
    virtual void BeginOfRunAction(const G4Run *) override;
    virtual void EndOfRunAction(const G4Run *) override;

    inline HistandNTupleManager *GetHistandNTupleManager()
    {
        return m_HistoandNtupleManager;
    }
    void DefineCommands();
    void ListNtuples()
    {
        if (m_HistoandNtupleManager)
            m_HistoandNtupleManager->GetNTupleManager()->ListNtuples();
    }
    void SetNtupleDump(G4int ID, G4bool ifdump)
    {
        if (m_HistoandNtupleManager)
            m_HistoandNtupleManager->GetNTupleManager()->SetNtupleDump(
                ID, ifdump);
    }
    GRPParticleContainer *GetContainer() const { return m_myParticleContainer; }
    void SetContainer(GRPParticleContainer *mycontainerptr)
    {
        m_myParticleContainer = mycontainerptr;
    }

private:
    HistandNTupleManager *m_HistoandNtupleManager;
    std::unique_ptr<G4Timer> m_timer;
    // Pointer to the generic messengers
    std::shared_ptr<G4GenericMessenger> m_AMessenger;
    GRPParticleContainer *m_myParticleContainer;
};
