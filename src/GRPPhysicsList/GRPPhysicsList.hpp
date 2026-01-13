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

#include <memory>

#include <G4VModularPhysicsList.hh>

#include <GRPPhysicsListMessenger.hpp>

// Modular physics used in this application
class GRPPhysicsList : public G4VModularPhysicsList
{
public:
    explicit GRPPhysicsList();
    ~GRPPhysicsList();
    void SetAddBiasing(G4bool ifaddbiasing) { m_biasing = ifaddbiasing; }
    G4bool GetAddBiasing() const { return m_biasing; }

private:
    std::unique_ptr<GRPPhysicsListMessenger> m_pl_messenger;
    GRPEmPhysics *m_em_physics;
    G4bool m_biasing;
};
