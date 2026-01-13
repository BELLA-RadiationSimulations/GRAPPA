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

#include <G4GeneralParticleSource.hh>
#include <G4ParticleGun.hh>

#include <GRPParticleContainer.hpp>

class GRPParticleSource
{
public:
    GRPParticleSource();
    virtual ~GRPParticleSource();

    // method to access particle gun
    virtual G4VPrimaryGenerator *GetGun() = 0;
    virtual void LoadNextParticle(const GRPParticleContainer &, G4int) {};
};

class GRPParticleSourceGPS : public GRPParticleSource
{
public:
    GRPParticleSourceGPS();
    virtual ~GRPParticleSourceGPS();

    // method to access particle gun
    virtual G4GeneralParticleSource *GetGun() override { return m_GPS.get(); };

private:
    std::unique_ptr<G4GeneralParticleSource> m_GPS;
};

class GRPParticleSourceGun : public GRPParticleSource
{
public:
    GRPParticleSourceGun();
    virtual ~GRPParticleSourceGun();
    void LoadNextParticle(
        const GRPParticleContainer &mycontainer, G4int eventNumber) override;

    // method to access particle gun
    virtual G4ParticleGun *GetGun() override { return m_Gun.get(); };

private:
    std::unique_ptr<G4ParticleGun> m_Gun;
};
