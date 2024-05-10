// Copyright 2021-2024
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
#include <G4SystemOfUnits.hh>
#include <G4Box.hh>
#include <G4GenericMessenger.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>
#include <G4RunManager.hh>
#include <G4Sphere.hh>
#include <G4VisAttributes.hh>
#include <G4VUserDetectorConstruction.hh>

#include <GRPSDAbsorber.hpp>

class GRPSTDetectorConstruction : public G4VUserDetectorConstruction
{

public:
    // Constructor
    GRPSTDetectorConstruction(HistandNTupleManager *myanalysismanager);
    // Destructor
    virtual ~GRPSTDetectorConstruction();
    virtual G4VPhysicalVolume *Construct() override;

    // Sensitive Detector Construction
    void ConstructSDandField() override;

private:
    G4VPhysicalVolume *ConstructWorldandSTarget();
    void ReinitializeGeometry();

    // Some useful parameters to construct the world and the detectors
    G4double w_radius = 100 * cm;
    G4double foil_x = 5 * cm;
    G4double foil_y = 5 * cm;
    G4double foil_z = 1 * cm;
    G4ThreeVector foil_center = G4ThreeVector(0 * mm, 0 * mm, 0 * mm);
    G4double absorber_thickness = 1 * mm;
    G4String w_name = "World", f_name = "Foil", std_a_name = "Absorber";
    G4String w_material_name = "G4_Galactic", f_material_name = "G4_W",
             a_material_name = "G4_Galactic";

    // Pointer to the physical world
    G4VPhysicalVolume *physWorld;
    // Pointer to the logical absorbers for sensitive detectors
    G4LogicalVolume *m_LogicalAbsorberWorld;
    G4LogicalVolume *m_LogicalAbsorberFoil;
    // Pointer to the sensitive detectors
    AbsorberSD *m_SDAbsorberWorld;
    AbsorberSD *m_SDAbsorberFoil;

    // Pointer to the customized analysis manager
    HistandNTupleManager *m_HistoandNtupleManager;
};
