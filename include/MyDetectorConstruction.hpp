/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <memory>
#include <CLHEP/Units/SystemOfUnits.h>
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

#include <MySDAbsorber.hpp>

class MyDetectorConstruction : public G4VUserDetectorConstruction
{

public:
    //Constructor
    MyDetectorConstruction(HistandNTupleManager* myanalysismanager);
    //Destructor
    virtual ~MyDetectorConstruction();
    virtual G4VPhysicalVolume* Construct() override;

    // Sensitive Detector Construction
    void ConstructSDandField() override;

private:
    // Function to define the custom commands for the UI
    void DefineCommands();
    void PrintDetector();
    G4VPhysicalVolume* ConstructWorldandTarget();
    void ReinitializeGeometry();

    // Some useful data about the world and the detectors
    G4double w_radius = 50 * cm;
    G4double foil_x = 5 * cm;
    G4double foil_y = 5 * cm;
    G4double foil_z = 1 * cm;
    G4ThreeVector foil_center = G4ThreeVector(0 * mm, 0 * mm, 0 * mm);
    // Foil angle in radians
    G4double foil_angle_y = 0 * rad;
    G4double absorber_thickness = 0.02 * w_radius;
    G4String w_name = "World", f_name = "High-Z_foil", a_name = "Absorber";
    G4String w_material_name = "G4_Galactic", f_material_name = "G4_W", a_material_name = "G4_Galactic";
    // Pointer to the physical world
    G4VPhysicalVolume* physWorld;
    // Pointer to the logical absorber
    G4LogicalVolume* logicAbsorber;
    // Pointer to the customized analysis manager
    HistandNTupleManager* m_HistoandNtupleManager;
    // Pointer to the SD
    AbsorberSD* absorber;

    // Pointer to the generic messengers
    std::shared_ptr<G4GenericMessenger> m_WMessenger;
    std::shared_ptr<G4GenericMessenger> m_FMessenger;
    std::shared_ptr<G4GenericMessenger> m_GenericMessenger;
};
