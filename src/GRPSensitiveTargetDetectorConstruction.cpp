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

#include <GRPSensitiveTargetDetectorConstruction.hpp>

GRPSTDetectorConstruction::GRPSTDetectorConstruction(
    HistandNTupleManager *myanalysismanager)
{
    m_HistoandNtupleManager = myanalysismanager;
}

GRPSTDetectorConstruction::~GRPSTDetectorConstruction() {}

G4VPhysicalVolume *GRPSTDetectorConstruction::Construct()
{
    return ConstructWorldandSTarget();
}

G4VPhysicalVolume *GRPSTDetectorConstruction::ConstructWorldandSTarget()
{
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    //
    const G4bool checkOverlaps = true;

    // ================================================ //
    // Creating the WORLD volume
    // Defining material
    G4Material *w_material = nist->FindOrBuildMaterial(w_material_name);
    // Creating the world solid
    G4Sphere *solidWorld = new G4Sphere(
        w_name, // its name
        0.,
        w_radius, // its inner and outer radii
        0.,
        360 * deg, // its phi initial and final angles
        0.,
        360 * deg); // its theta initial and final angles
    // Creating logical world volume
    G4LogicalVolume *logicWorld = new G4LogicalVolume(
        solidWorld, // its solid
        w_material, // its material
        w_name); // its name
    // Creating physical world volume
    physWorld = new G4PVPlacement(
        nullptr, // no rotation
        G4ThreeVector(), // at (0,0,0)
        logicWorld, // its logical volume
        w_name, // its name
        nullptr, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Creating FOIL ABSORBER volume
    // Defining material
    G4Material *f_material = nist->FindOrBuildMaterial(f_material_name);
    // Defining dimension
    G4ThreeVector f_dimensions(foil_x, foil_y, foil_z);
    // Defining position
    const G4ThreeVector f_position(foil_center[0], foil_center[1], foil_center[2]);
    // Creating foil solid
    G4Box *solidfoil = new G4Box(
        f_name, // its name
        0.5 * f_dimensions[0],
        0.5 * f_dimensions[1],
        0.5 * f_dimensions[2]); // its size
    // Creation foil logical volume
    m_LogicalAbsorberFoil = new G4LogicalVolume(
        solidfoil, // its solid
        f_material, // its material
        f_name); // its name
    // Creating foil physical volume
    new G4PVPlacement(
        nullptr, //  no rotation
        f_position, // position
        m_LogicalAbsorberFoil, // its logical volume
        f_name, // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Creating WOLRD SHEATH ABSORBER volume
    //
    const G4double a_dimensions = w_radius;
    G4Material *a_material = w_material;

    G4Sphere *StdsolidAbsorber = new G4Sphere(
        std_a_name, // its name
        a_dimensions - absorber_thickness,
        a_dimensions, // its inner and outer radii
        0.,
        360 * deg, // its phi initial and final angles
        0.,
        180 * deg); // its theta initial and final angles

    // Creation of the logical volume (with the shape of the sphere)
    m_LogicalAbsorberWorld = new G4LogicalVolume(
        StdsolidAbsorber, // its solid
        a_material, // its material
        std_a_name); // its name

    // Creation of the physical volume associated with the logical volume
    new G4PVPlacement(
        nullptr, // no rotation
        G4ThreeVector(), // at (0,0,0)
        m_LogicalAbsorberWorld, // its logical volume
        std_a_name, // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // Return root volume
    return physWorld;
}

void GRPSTDetectorConstruction::ConstructSDandField()
{
    // ================================================ //
    // Link and activate Sensitive Detectors

    G4SDManager *SDMpointer = G4SDManager::GetSDMpointer();

    m_SDAbsorber = new WorldAbsorberSD(
        "/FinalAbsorber/Absorber", m_HistoandNtupleManager);
    //m_SDAbsorberFoil = new AbsorberSD(
    //    "/FinalAbsorber/AbsorberFoil", m_HistoandNtupleManager);

    SDMpointer->AddNewDetector(m_SDAbsorber);

    m_LogicalAbsorberWorld->SetSensitiveDetector(m_SDAbsorber);
    m_LogicalAbsorberFoil->SetSensitiveDetector(m_SDAbsorber);
}

void GRPSTDetectorConstruction::ReinitializeGeometry()
{
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    runmanager->DefineWorldVolume(ConstructWorldandSTarget());
    runmanager->ReinitializeGeometry();
}
