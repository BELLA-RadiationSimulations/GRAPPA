/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <G4VUserPrimaryGeneratorAction.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Event.hh>

#include <MyParticleSource.hpp>

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    MyPrimaryGeneratorAction(bool useGPS, G4String filename);
    virtual ~MyPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event *anEvent);

    inline std::shared_ptr<ParticleSource> GetSource() const { return m_PSource; };

private:
    std::shared_ptr<ParticleSource> m_PSource;
};