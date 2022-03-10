/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <G4VUserActionInitialization.hh>
#include <G4Threading.hh>

#include <MyDetectorConstruction.hpp>
#include <MyPrimaryGeneratorAction.hpp>
#include <MyActionInitialization.hpp>
#include <MyTrackingAction.hpp>
#include <MyRunAction.hpp>

class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization(HistandNTupleManager* myanalysismanager);
    virtual ~MyActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
    G4double* tfoilM = new G4double;
    HistandNTupleManager* m_HistandNTupleManager;
};