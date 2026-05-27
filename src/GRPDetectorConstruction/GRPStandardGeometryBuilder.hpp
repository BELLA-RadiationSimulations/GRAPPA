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

#include <G4SystemOfUnits.hh>

#include <GRPGeometryBuilder.hpp>

// Standard geometry builder.
// This class implement the geometry as it has been done since
// GRAPPA's creation. It is hardcoded C++ volumes, with the possibility
// of changing the elements properties.
// This class is not deprecated, but it won't be updated with newer geometries
// in the future and it will not include new detectors. All the new effort will
// go into supporting the more general approach based on GDML input files
class GRPStandardGeometryBuilder : public GRPGeometryBuilder
{

public:
    G4VPhysicalVolume *ConstructWorldandTarget() override;
    void SetFoilThickness(G4double thickness) { m_foil_z = thickness; }
    void SetFoilSize(G4double size) { m_foil_x = m_foil_y = size; }
    void SetFoilCenter(G4ThreeVector center) { m_foil_center = center; }
    void SetFoilMaterial(const G4String &material)
    {
        m_f_material_name = material;
    }
    void SetFoilRotation(G4double angle) { m_foil_angle_y = angle; }
    void SetWorldRadius(G4double radius) { m_w_radius = radius; }
    void SetWorldMaterial(const G4String &material)
    {
        m_w_material_name = material;
    }
    void SetAbsorberThickness(G4double thickness)
    {
        m_absorber_thickness = thickness;
    }
    void PrintDetector();

private:
    // Some useful parameters to construct the world and the detectors
    G4double m_w_radius = 100 * cm;
    G4double m_foil_x = 5 * cm;
    G4double m_foil_y = 5 * cm;
    G4double m_foil_z = 1 * cm;
    G4ThreeVector m_foil_center = G4ThreeVector(0 * mm, 0 * mm, 0 * mm);
    G4double m_foil_angle_y = 0 * rad;
    G4double m_absorber_thickness = 1 * mm;
    G4String m_w_material_name = "G4_Galactic", m_f_material_name = "G4_W",
             m_a_material_name = "G4_Galactic";
};
