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

#include <GRPAbsorberHit.hpp>

G4ThreadLocal G4Allocator<GRPAbsorberHit> *GRPAbsorberHitAllocator = nullptr;

GRPAbsorberHit::~GRPAbsorberHit() {}

GRPAbsorberHit::GRPAbsorberHit(
    G4float Ekin,
    G4ThreeVector position,
    G4ThreeVector momentum,
    G4float weight,
    G4int partID,
    G4float time,
    G4int trackID,
    G4int parentID,
    G4String CreatorProcess)
    : m_Ekin(Ekin)
    , m_Position(std::move(position))
    , m_Momentum(std::move(momentum))
    , m_weight(weight)
    , m_PartID(partID)
    , m_Time(time)
    , m_TrackID(trackID)
    , m_parentID(parentID)
    , m_CreatorProcess(std::move(CreatorProcess))
{}

void GRPAbsorberHit::Draw()
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

void GRPAbsorberHit::Print()
{
    G4cout << " TrackID: " << m_TrackID << ", "
           << " PartID: " << m_PartID << ", "
           << " Creator process: " << m_CreatorProcess << ", "
           << " Pos: " << std::setw(7) << G4BestUnit(m_Position, "Length")
           << G4endl;
}
