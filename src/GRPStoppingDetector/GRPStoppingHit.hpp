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

class GRPStoppingHit : public G4VHit
{
public:
    GRPStoppingHit() = default;
    GRPStoppingHit(const GRPStoppingHit &right) = default;
    GRPStoppingHit(
        G4float Ekin,
        G4ThreeVector position,
        G4float weigth,
        G4int partID,
        G4float time,
        G4int trackID);
    ~GRPStoppingHit() override;

    GRPStoppingHit &operator=(const GRPStoppingHit &right) = default;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void Draw() override;
    void Print() override;

    G4float GetEkin() const { return m_Ekin; }
    G4int GetPartID() const { return m_PartID; }
    G4float GetGlobalTime() const { return m_Time; }
    G4ThreeVector GetPosition() const { return m_Position; }
    G4int GetTrackID() const { return m_TrackID; }
    G4float GetWeight() const { return m_weight; }
    G4float GetX() const { return m_Position.x(); }
    G4float GetY() const { return m_Position.y(); }
    G4float GetZ() const { return m_Position.z(); }

    void SetEkin(G4float de) { m_Ekin = de; }
    void SetPartID(G4int partID) { m_PartID = partID; }
    void SetTime(G4float Time) { m_Time = Time; }
    void SetPosition(G4ThreeVector position) { m_Position = position; }
    void SetTrackID(G4int trackID) { m_TrackID = trackID; }
    void SetWeight(G4float weigth) { m_weight = weigth; }

private:
    G4float m_Ekin = 0.;
    G4ThreeVector m_Position;
    G4float m_weight;
    G4int m_PartID;
    G4float m_Time;
    G4int m_TrackID = -1;
};

using GRPStoppingHitCollection = G4THitsCollection<GRPStoppingHit>;

extern G4ThreadLocal G4Allocator<GRPStoppingHit> *GRPStoppingHitAllocator;

inline void *GRPStoppingHit::operator new(size_t)
{
    if (!GRPStoppingHitAllocator)
    {
        GRPStoppingHitAllocator = new G4Allocator<GRPStoppingHit>;
    }
    return (void *)GRPStoppingHitAllocator->MallocSingle();
}

inline void GRPStoppingHit::operator delete(void *aHit)
{
    if (!GRPStoppingHitAllocator)
    {
        GRPStoppingHitAllocator = new G4Allocator<GRPStoppingHit>;
    }
    GRPStoppingHitAllocator->FreeSingle((GRPStoppingHit *)aHit);
}
