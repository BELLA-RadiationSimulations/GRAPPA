// Copyright 2021-2026
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

#include <G4VPhysicalVolume.hh>

class GRPGeometryBuilder
{
public:
    virtual ~GRPGeometryBuilder() = default;
    virtual G4VPhysicalVolume *ConstructWorldandTarget() = 0;
};
