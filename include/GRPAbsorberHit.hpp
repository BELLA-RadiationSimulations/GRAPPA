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

#include <utility>

#include <G4Allocator.hh>
#include <G4Circle.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4UnitsTable.hh>
#include <G4VHit.hh>
#include <G4VisAttributes.hh>
#include <G4VVisManager.hh>

class GRPAbsorberHit : public G4VHit
{
public:
    GRPAbsorberHit() = default;
    GRPAbsorberHit(const GRPAbsorberHit &right) = default;
    GRPAbsorberHit(
        G4float Ekin,
        G4ThreeVector position,
        G4ThreeVector momentum,
        G4float weigth,
        G4int partID,
        G4float time,
        G4int trackID,
        G4int parentID,
        G4String creatorProcess);
    ~GRPAbsorberHit() override;

    GRPAbsorberHit &operator=(const GRPAbsorberHit &right) = default;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw() override;
    void Print() override;

    G4String GetCreatorProcess() const { return m_CreatorProcess; }
    G4float GetEkin() const { return m_Ekin; }
    G4int GetPartID() const { return m_PartID; }
    G4float GetGlobalTime() const { return m_Time; }
    G4ThreeVector GetMomentum() const { return m_Momentum; }
    G4ThreeVector GetPosition() const { return m_Position; }
    G4int GetParentID() const { return m_parentID; }
    G4int GetTrackID() const { return m_TrackID; }
    G4float GetWeight() const { return m_weight; }
    G4float GetX() const { return m_Position.x(); }
    G4float GetY() const { return m_Position.y(); }
    G4float GetZ() const { return m_Position.z(); }
    G4float GetPX() const { return m_Momentum.x(); }
    G4float GetPY() const { return m_Momentum.y(); }
    G4float GetPZ() const { return m_Momentum.z(); }

    void SetCreatorProcess(G4String cprocess) { m_CreatorProcess = cprocess; }
    void SetEkin(G4float de) { m_Ekin = de; }
    void SetPartID(G4int partID) { m_PartID = partID; }
    void SetTime(G4float Time) { m_Time = Time; }
    void SetMomentum(G4ThreeVector momentum) { m_Momentum = momentum; }
    void SetParentID(G4int parentID) { m_parentID = parentID; }
    void SetPosition(G4ThreeVector position) { m_Position = position; }
    void SetTrackID(G4int trackID) { m_TrackID = trackID; }
    void SetWeight(G4float weigth) { m_weight = weigth; }

private:
    G4float m_Ekin = 0.;
    G4ThreeVector m_Position;
    G4ThreeVector m_Momentum;
    G4float m_weight;
    G4int m_PartID;
    G4float m_Time;
    G4int m_TrackID = -1;
    G4int m_parentID = 0;
    G4String m_CreatorProcess;
};

using GRPAbsorberHitCollection = G4THitsCollection<GRPAbsorberHit>;

extern G4ThreadLocal G4Allocator<GRPAbsorberHit> *GRPAbsorberHitAllocator;

inline void *GRPAbsorberHit::operator new(size_t)
{
    if (!GRPAbsorberHitAllocator)
    {
        GRPAbsorberHitAllocator = new G4Allocator<GRPAbsorberHit>;
    }
    return (void *)GRPAbsorberHitAllocator->MallocSingle();
}

inline void GRPAbsorberHit::operator delete(void *aHit)
{
    if (!GRPAbsorberHitAllocator)
    {
        GRPAbsorberHitAllocator = new G4Allocator<GRPAbsorberHit>;
    }
    GRPAbsorberHitAllocator->FreeSingle((GRPAbsorberHit *)aHit);
}
