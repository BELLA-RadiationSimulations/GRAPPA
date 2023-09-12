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

#include <globals.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4Timer.hh>
#include <G4UnitsTable.hh>
#include <G4UserRunAction.hh>

#include <GRPActionInitialization.hpp>
#include <GRPPrimaryGeneratorAction.hpp>
#include <GRPHistandNTupleManager.hpp>

class G4Run;

class GRPRunAction : public G4UserRunAction
{
public:
    GRPRunAction(G4bool useGPS, HistandNTupleManager *myanalysismanager);
    virtual ~GRPRunAction();

    virtual void BeginOfRunAction(const G4Run *) override;
    virtual void EndOfRunAction(const G4Run *) override;

    inline HistandNTupleManager *GetHistandNTupleManager() { return m_HistoandNtupleManager; }
    void DefineCommands();
    void ListNtuples() { if(m_HistoandNtupleManager) m_HistoandNtupleManager->GetNTupleManager()->ListNtuples(); }
    void SetNtupleDump(G4int ID, G4bool ifdump) { if(m_HistoandNtupleManager) m_HistoandNtupleManager->GetNTupleManager()->SetNtupleDump(ID, ifdump); }
private:
    HistandNTupleManager *m_HistoandNtupleManager;
    G4bool m_partsfromfile = false;
    G4int m_Numberofeventsthisrun = 0;
    std::unique_ptr<G4Timer> m_timer;
    // Pointer to the generic messengers
    std::shared_ptr<G4GenericMessenger> m_AMessenger;
};
