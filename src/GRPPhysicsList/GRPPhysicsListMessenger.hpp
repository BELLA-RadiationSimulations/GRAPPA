// Copyright 2021-2025
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

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UImessenger.hh>

// Forward declaration of the physics list to enable access in this class
class GRPPhysicsList;

class GRPPhysicsListMessenger : public G4UImessenger
{
public:
    explicit GRPPhysicsListMessenger(GRPPhysicsList *);
    ~GRPPhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    GRPPhysicsList *m_physics_list;
    std::unique_ptr<G4UIdirectory> m_physics_directory;
    std::unique_ptr<G4UIcmdWithAString> m_add_cmd;
};
