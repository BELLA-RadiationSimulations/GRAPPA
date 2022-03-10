/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <filesystem>
#include <globals.hh>
#include <G4Accumulable.hh>
#include <G4AccumulableManager.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4UnitsTable.hh>
#include <G4UserRunAction.hh>

#include <MyPrimaryGeneratorAction.hpp>
#include <MyHistandNTupleManager.hpp>

class G4Run;

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction(HistandNTupleManager* myanalysismanager);
    virtual ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;

    void ComputeTFoil(G4double tf);

    inline HistandNTupleManager* GetHistandNTupleManager() { return m_HistoandNtupleManager; }
private:
    G4Accumulable<G4double> tfoil = G4Accumulable("Foil_time", 0.0, G4MergeMode::kMaximum);
    G4double* tfoilM;
    HistandNTupleManager* m_HistoandNtupleManager;
};
