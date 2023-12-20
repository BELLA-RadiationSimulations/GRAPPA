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

#include <iostream>
#include <ctime>

#include <globals.hh>
#include <FTFP_BERT.hh>
#include <QGSP_BERT.hh>
#include <G4PhysListFactory.hh>
#include <G4RunManagerFactory.hh>
#include <G4Timer.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>

#include <GRPDetectorConstruction.hpp>
#include <GRPPrimaryGeneratorAction.hpp>
#include <GRPRunAction.hpp>
#include <GRPActionInitialization.hpp>
#include <GRPSignalHandler.hpp>