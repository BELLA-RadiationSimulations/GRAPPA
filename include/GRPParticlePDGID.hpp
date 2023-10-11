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

namespace ParticleID
{
    constexpr G4int electronID = 11;
    constexpr G4int positronID = -11;
    constexpr G4int gammaID = 22;
    constexpr G4int muonminusID = 13;
    constexpr G4int muonplusID = -13;
    constexpr G4int pionplusID = 211;
    constexpr G4int pionminusID = -211;
} // namespace ParticleID