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
    void AddPhysicsList(const G4String &name);

private:
    std::unique_ptr<GRPPhysicsListMessenger> m_pl_messenger;
};
