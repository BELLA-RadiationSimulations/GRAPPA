// Copyright 2021-2026
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause-LBNL

#pragma once

#include <memory>

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UImessenger.hh>

// Forward declaration of the physics list to enable access in this class
class GRPPhysicsList;
class GRPEmPhysics;

// Messenger for the GRPPhysicsList class.
// Via this messenger we can personalize options for the physics class
class GRPPhysicsListMessenger : public G4UImessenger
{
public:
    explicit GRPPhysicsListMessenger(GRPPhysicsList *, GRPEmPhysics *);
    ~GRPPhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    GRPPhysicsList *m_physics_list;
    GRPEmPhysics *m_em_physics;
    std::unique_ptr<G4UIdirectory> m_physics_directory;
    std::unique_ptr<G4UIcmdWithABool> m_add_biasing_cmd;
    std::unique_ptr<G4UIcmdWithADouble> m_set_muonxs_cmd;
    std::unique_ptr<G4UIcmdWithoutParameter> m_get_muonxs_cmd;
    std::unique_ptr<G4UIcmdWithoutParameter> m_list_physics_cmd;
};
