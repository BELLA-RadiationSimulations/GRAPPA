
#pragma once

#include <G4VPhysicsConstructor.hh>

class GRPEmPhysics : public G4VPhysicsConstructor
{
public:
    explicit GRPEmPhysics(G4int ver = 1, const G4String &name = "");

    ~GRPEmPhysics() override;

    void ConstructParticle() override;
    void ConstructProcess() override;
};
