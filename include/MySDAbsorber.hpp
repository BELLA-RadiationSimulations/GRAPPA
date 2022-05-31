/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <set>

#include <G4VPhysicalVolume.hh>
#include <G4VProcess.hh>
#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>

#include <MyHistandNTupleManager.hpp>

// Class that defines the behaviour of the absorbing layer around the world.
// We treat it as a Sensitive Detector in order to be able to count and manage the
// incoming particles hits.
// This absorber counts primary particles, electrons, positrons and photons
class AbsorberSD : public G4VSensitiveDetector
{
public:
    AbsorberSD(G4String name, HistandNTupleManager *myanalysismanager);
    ~AbsorberSD();
    void Initialize(G4HCofThisEvent *HCE) override;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;

private:
    HistandNTupleManager *m_HistoandNtupleManager;
    // Set containing the particles included in this absorber
    std::set<G4String> m_ParticleList = {"e+", "e-", "gamma"};
};

// Class that defines the behaviour of the absorbing layer around the world.
// We treat it as a Sensitive Detector in order to be able to count and manage the
// incoming particles hits.
// This absorber counts pions and muons
class PiandMuAbsorberSD : public G4VSensitiveDetector
{
public:
    PiandMuAbsorberSD(G4String name, HistandNTupleManager *myanalysismanager);
    ~PiandMuAbsorberSD();
    void Initialize(G4HCofThisEvent *HCE) override;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;

private:
    HistandNTupleManager *m_HistoandNtupleManager;
    // Set containing the particles included in this absorber
    std::set<G4String> m_ParticleList = {"pi+", "pi-", "pi0", "mu+", "mu-"};
};