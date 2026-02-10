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

#include <GRPStoppingHit.hpp>

G4ThreadLocal G4Allocator<GRPStoppingHit> *GRPStoppingHitAllocator = nullptr;

GRPStoppingHit::~GRPStoppingHit() {}

GRPStoppingHit::GRPStoppingHit(
    G4float Ekin,
    G4ThreeVector position,
    G4float weight,
    G4int partID,
    G4float time,
    G4int trackID)
    : m_Ekin(Ekin)
    , m_Position(std::move(position))
    , m_weight(weight)
    , m_PartID(partID)
    , m_Time(time)
    , m_TrackID(trackID)
{}

void GRPStoppingHit::Draw()
{
    G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager)
    {
        G4Circle circle(m_Position);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        const G4Colour colour(1., 0., 0.);
        const G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

void GRPStoppingHit::Print()
{
    G4cout << " TrackID: " << m_TrackID << ", "
           << " PartID: " << m_PartID << ", "
           << " Pos: " << std::setw(7) << G4BestUnit(m_Position, "Length")
           << G4endl;
}
