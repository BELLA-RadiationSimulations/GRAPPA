/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <algorithm>
#include <vector>

#include <G4ParticleTable.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VProcess.hh>
#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>

#include <MyHistandNTupleManager.hpp>

//
//  Here we use the Particle Data Group (PDG) convention
//  for particle numbering. Each particle has a unique integer ID.
//  Name: e-, id: 11
//  Name: e+, id: -11
//  Name: gamma, id: 22
//  Name: mu-, id: 13
//  Name: mu+, id: -13
//  Name: pi+, id: 211
//  Name: pi-, id: -211

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
    std::vector<G4int> m_ParticleList;
    G4int m_electronID;
    G4int m_positronID;
    G4int m_gammaID;
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
    std::vector<G4int> m_ParticleList;
    G4int m_muonminusID;
    G4int m_muonplusID;
    G4int m_pionminusID;
    G4int m_pionplusID;
};