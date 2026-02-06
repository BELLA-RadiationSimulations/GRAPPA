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

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>
#include <G4Sphere.hh>
#include <G4VisAttributes.hh>
#include <G4VUserDetectorConstruction.hh>

#include <GRPStandardGeometryBuilder.hpp>

G4VPhysicalVolume *GRPStandardGeometryBuilder::ConstructWorldandTarget()
{
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    const G4bool checkOverlaps = true;

    // ================================================ //
    // Creating the world

    G4Sphere *solidWorld = new G4Sphere(
        "WorldSphere", // its name
        0.,
        m_w_radius, // its inner and outer radii
        0.,
        360 * deg, // its phi initial and final angles
        0.,
        180 * deg); // its theta initial and final angles

    // Defining material of the world volume
    G4Material *w_material = nist->FindOrBuildMaterial(m_w_material_name);

    // Creating world logical volume
    G4LogicalVolume *logicWorld = new G4LogicalVolume(
        solidWorld, // its solid
        w_material, // its material
        "WorldLogical"); // its name

    // Creating physical world volume, i.e. positioning world volume
    G4VPhysicalVolume *physWorld = new G4PVPlacement(
        nullptr, // no rotation
        G4ThreeVector(), // at (0,0,0)
        logicWorld, // its logical volume
        "World", // its name
        nullptr, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Creating foil target
    //
    // In Testcommon there is an example on how to define a command to change
    // detector material from command line

    // Defining foil dimension and material
    // Foil is a square
    G4Material *f_material = nist->FindOrBuildMaterial(m_f_material_name);
    G4ThreeVector f_dimensions(m_foil_x, m_foil_y, m_foil_z);
    const G4ThreeVector f_position = m_foil_center;
    G4RotationMatrix rotm = G4RotationMatrix();
    // Foil rotation by a given angle
    rotm.rotateY(m_foil_angle_y);

    // Creating foil solid
    G4Box *solidfoil = new G4Box(
        "FoilBox", // its name
        0.5 * f_dimensions[0],
        0.5 * f_dimensions[1],
        0.5 * f_dimensions[2]); // its size

    // Creating foil logical volume
    G4LogicalVolume *logicfoil = new G4LogicalVolume(
        solidfoil, // its solid
        f_material, // its material
        "FoilLogical"); // its name

    // Creating physical foil volume, i.e. positioning foil volume into world
    // volume
    new G4PVPlacement(
        G4Transform3D(rotm, f_position), // G4Transform3D(rotm,G4threevec);
        logicfoil, // its logical volume
        "Foil", // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Creating a thin absorbing layer that coincides with the world
    // to detect particles

    const G4double a_dimensions = m_w_radius;
    G4Material *a_material = w_material;

    // Creating sensitive detector solid
    G4Sphere *StdsolidAbsorber = new G4Sphere(
        "AbsorberSphere", // its name
        a_dimensions - m_absorber_thickness,
        a_dimensions, // its inner and outer radii
        0.,
        360 * deg, // its phi initial and final angles
        0.,
        180 * deg); // its theta initial and final angles

    // Creating sensitive detector logical volume (with the shape of a sphere)
    G4LogicalVolume *logicalAbsorber = new G4LogicalVolume(
        StdsolidAbsorber, // its solid
        a_material, // its material
        "AbsorberLogical"); // its name

    // Creating the absorber physical volume, i.e. placing its logical volume
    // into the world
    new G4PVPlacement(
        nullptr, // no rotation
        G4ThreeVector(), // at (0,0,0)
        logicalAbsorber, // its logical volume
        "Absorber", // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Setting visualization attributes

    G4VisAttributes *worldVisAtt =
        new G4VisAttributes(false); // Setting world visible
    // worldVisAtt->SetForceWireframe();
    // worldVisAtt->SetLineWidth(0.05 * mm);
    // Setting target visible, with a blueish color
    G4VisAttributes *targetVisAtt =
        new G4VisAttributes(G4Colour(204 / 255., 204 / 255., 255 / 255., 0.2));
    targetVisAtt->SetForceSolid();
    // Setting absorber lightly visible with a white color
    G4VisAttributes *absorberVisAtt =
        new G4VisAttributes(G4Colour(1., 1., 1., 0.1));

    logicWorld->SetVisAttributes(worldVisAtt);
    logicfoil->SetVisAttributes(targetVisAtt);
    logicalAbsorber->SetVisAttributes(absorberVisAtt);

    // Return root volume
    return physWorld;
}

void GRPStandardGeometryBuilder::PrintDetector()
{
    G4cout << " World and target information " << G4endl;
    G4cout << " ------------------------------------------------------------- "
           << G4endl;
    G4cout << G4endl;
    G4cout << " The world is a sphere centered in 0 0 0 mm" << G4endl;
    G4cout << " Sphere radius [mm] : " << m_w_radius << G4endl;
    G4cout << " World material is " << m_w_material_name << G4endl;
    G4cout << G4endl;
    G4cout << " The absorber that surrounds the world has a thickness "
           << m_absorber_thickness << " mm" << G4endl;
    G4cout << G4endl;
    G4cout << " Name of the target: Foil" << G4endl;
    G4cout << " The target is a foil centered in " << m_foil_center[0] << " "
           << m_foil_center[1] << " " << m_foil_center[2] << " "
           << "mm" << G4endl;
    G4cout << " The foil size is " << G4endl;
    G4cout << " x [mm] : " << m_foil_x << G4endl;
    G4cout << " y [mm] : " << m_foil_y << G4endl;
    G4cout << " z [mm] : " << m_foil_z << G4endl;
    G4cout << " Rotation around the y (vertical axis) [deg] : "
           << m_foil_angle_y / deg << G4endl;
    G4cout << " Foil material is " << m_f_material_name << G4endl;
}
