/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <G4VPhysicalVolume.hh>
#include <G4VProcess.hh>
#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>

#include <MyHistandNTupleManager.hpp>

 // Class that defines the behaviour of the absorbing layer around the world.
 // We treat it as a Sensitive Detector in order to be able to count and manage the
 // incoming particles hits.
 //

class AbsorberSD : public G4VSensitiveDetector
{
public:
    AbsorberSD(G4String name, HistandNTupleManager* myanalysismanager);
    ~AbsorberSD();
    void Initialize(G4HCofThisEvent* HCE) override;
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;

private:
    HistandNTupleManager* m_HistoandNtupleManager;
};