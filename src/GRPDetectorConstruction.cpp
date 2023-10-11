// Copyright 2021-2023
//
// Authors:
// Stanimir Kisyov, Davide Terzani
//
// Creator: Davide Terzani
//
// This file is part of GRAPPA.
//
// License: BSD-3-Clause

#include <GRPDetectorConstruction.hpp>

GRPDetectorConstruction::GRPDetectorConstruction(
    HistandNTupleManager *myanalysismanager)
{
    m_HistoandNtupleManager = myanalysismanager;
    DefineCommands();
}

GRPDetectorConstruction::~GRPDetectorConstruction() {}

G4VPhysicalVolume *GRPDetectorConstruction::Construct()
{
    return ConstructWorldandTarget();
}

G4VPhysicalVolume *GRPDetectorConstruction::ConstructWorldandTarget()
{
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    //
    const G4bool checkOverlaps = true;

    // ================================================ //
    // First, we create the world
    G4Material *w_material = nist->FindOrBuildMaterial(w_material_name);
    // Instead of G4_AIR, G4_Galactic has 1e-25g/cm3 and 21.8 eV

    // Creation of a world box or sphere
    /*
    G4Box* solidWorld =
        new G4Box(w_name, //its name
            0.5 * w_dimensions[0], 0.5 * w_dimensions[1], 0.5 *
    w_dimensions[2]);     //its size
    */

    G4Sphere *solidWorld = new G4Sphere(
        w_name, // its name
        0.,
        w_radius, // its inner and outer radii
        0.,
        360 * deg, // its phi initial and final angles
        0.,
        360 * deg); // its theta initial and final angles

    // Creation of the logical volume (with the shape of the box)
    G4LogicalVolume *logicWorld = new G4LogicalVolume(
        solidWorld, // its solid
        w_material, // its material
        w_name); // its name

    // Creation of the physical volume associated with the logical volume
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
    // Now, we create the solid target
    //
    // In Testcommon there is an example on how to define a command to change
    // detector material from command line

    // Foil is assumed a square
    foil_y = foil_x;

    G4Material *f_material = nist->FindOrBuildMaterial(f_material_name);
    G4ThreeVector f_dimensions(foil_x, foil_y, foil_z);
    const G4ThreeVector f_position(
        foil_center[0], foil_center[1], foil_center[2]);
    G4RotationMatrix rotm = G4RotationMatrix();
    // Foil rotation by a given angle
    rotm.rotateY(foil_angle_y);

    // Foil box
    G4Box *solidfoil = new G4Box(
        f_name, // its name
        0.5 * f_dimensions[0],
        0.5 * f_dimensions[1],
        0.5 * f_dimensions[2]); // its size

    // Creation of the logical volume (with the shape of the box)
    G4LogicalVolume *logicfoil = new G4LogicalVolume(
        solidfoil, // its solid
        f_material, // its material
        f_name); // its name

    // Creation of the phisical foil
    new G4PVPlacement(
        G4Transform3D(rotm, f_position), // G4Transform3D(rotm,G4threevec);
        logicfoil, // its logical volume
        f_name, // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // We create an absorbing layer that coincides with the world
    // that makes us detect particles

    // First absorber is for e+, e- and gamma
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
    m_LogicalAbsorber = new G4LogicalVolume(
        StdsolidAbsorber, // its solid
        a_material, // its material
        std_a_name); // its name

    // Creation of the physical volume associated with the logical volume
    new G4PVPlacement(
        nullptr, // no rotation
        G4ThreeVector(), // at (0,0,0)
        m_LogicalAbsorber, // its logical volume
        std_a_name, // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Set Visualization attributes

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
    m_LogicalAbsorber->SetVisAttributes(absorberVisAtt);

    // Return root volume
    return physWorld;
}

void GRPDetectorConstruction::ConstructSDandField()
{
    // ================================================ //
    // Link and activate Sensitive Detectors

    G4SDManager *SDMpointer = G4SDManager::GetSDMpointer();

    m_StandardAbsorber = new AbsorberSD(
        "/FinalAbsorber/StandardAbsorber", m_HistoandNtupleManager);

    SDMpointer->AddNewDetector(m_StandardAbsorber);
    m_LogicalAbsorber->SetSensitiveDetector(m_StandardAbsorber);
}

void GRPDetectorConstruction::DefineCommands()
{
    // Modifications to geometry are currently possible
    // but they must be done in the PreInit stage.
    // Still working or reinitializing the geometry between runs
    //
    // define command directory using generic messenger class
    m_GenericMessenger = std::make_shared<G4GenericMessenger>(
        this, "/geometry/", "Commands to configure the target");
    m_WMessenger = std::make_shared<G4GenericMessenger>(
        this, "/geometry/world/", "Commands to configure the target");
    m_FMessenger = std::make_shared<G4GenericMessenger>(
        this, "/geometry/foil/", "Commands to configure the target");
    // configure commands

    // Print the current status of the world and foil
    G4GenericMessenger::Command &printcommand =
        m_GenericMessenger->DeclareMethod(
            "list",
            &GRPDetectorConstruction::PrintDetector,
            "List the current world and detector configuration");
    printcommand.SetGuidance(" Print world and foil status ");
    printcommand.SetStates(G4State_PreInit, G4State_Idle);

    // ===============================
    // Reinitialization stil WIP
    // Update the global geometry after modifications have been made
    // G4GenericMessenger::Command& updatecommand =
    // m_GenericMessenger->DeclareMethod("reinitialize",
    // &GRPDetectorConstruction::ReinitializeGeometry,
    //    "Update and reinitialize global geometry");
    // updatecommand.SetGuidance(" Reinitializes global geometry re-constructing
    // the detector, if it has already been constructed ");
    // updatecommand.SetStates(G4State_Idle);
    // ===============================

    // Target foil properties

    G4GenericMessenger::Command &thickcommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "thickness",
            "mm",
            foil_z,
            "Sets the foil size along z (thickness). Default unit is mm.");
    thickcommand.SetGuidance(" Sets the foil thickness ");
    thickcommand.SetStates(G4State_PreInit);
    G4GenericMessenger::Command &sizecommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "size",
            "mm",
            foil_x,
            "Sets the foil size along x and y (foil is assumed squared). "
            "Default unit is mm.");
    sizecommand.SetGuidance(
        " Sets the foil transverse sizes (assuming it is a square) ");
    sizecommand.SetStates(G4State_PreInit);
    G4GenericMessenger::Command &fmaterialcommand =
        m_FMessenger->DeclareProperty(
            "material", f_material_name, "Sets the foil material.");
    fmaterialcommand.SetGuidance(" Sets the foil material ");
    fmaterialcommand.SetStates(G4State_PreInit);
    G4GenericMessenger::Command &centercommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "center",
            "mm",
            foil_center,
            "Sets the foil center. Default unit is mm.");
    centercommand.SetGuidance(" Sets the foil center ");
    centercommand.SetStates(G4State_PreInit);
    G4GenericMessenger::Command &anglecommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "rotation",
            "deg",
            foil_angle_y,
            "Sets the foil rotation around the y axis. Default unit is deg.");
    anglecommand.SetGuidance(" Sets the foil center ");
    anglecommand.SetStates(G4State_PreInit);

    // World properties
    G4GenericMessenger::Command &wradiuscommand =
        m_WMessenger->DeclarePropertyWithUnit(
            "radius",
            "mm",
            w_radius,
            "Sets the world radius. Default unit is mm.");
    wradiuscommand.SetGuidance(" Sets the world radius ");
    wradiuscommand.SetStates(G4State_PreInit);

    G4GenericMessenger::Command &wmaterialcommand =
        m_WMessenger->DeclareProperty(
            "material", w_material_name, "Sets the world material.");
    wmaterialcommand.SetGuidance(" Sets the world material ");
    wmaterialcommand.SetStates(G4State_PreInit);
}

void GRPDetectorConstruction::PrintDetector()
{
    G4cout << " World and target information " << G4endl;
    G4cout << " ------------------------------------------------------------- "
           << G4endl;
    G4cout << G4endl;
    G4cout << " Name of the world: " << w_name << G4endl;
    G4cout << " The world is a sphere centered in 0 0 0 mm" << G4endl;
    G4cout << " Sphere radius [mm] : " << w_radius << G4endl;
    G4cout << " World material is " << w_material_name << G4endl;
    G4cout << G4endl;
    G4cout << " Name of the target: " << f_name << G4endl;
    G4cout << " The target is a foil centered in " << foil_center[0] << " "
           << foil_center[1] << " " << foil_center[2] << " "
           << "mm" << G4endl;
    G4cout << " The foil size is " << G4endl;
    G4cout << " x [mm] : " << foil_x << G4endl;
    G4cout << " y [mm] : " << foil_y << G4endl;
    G4cout << " z [mm] : " << foil_z << G4endl;
    G4cout << " Rotation around the y (vertical axis) [deg] : "
           << foil_angle_y * rad / deg << G4endl;
    G4cout << " Foil material is " << f_material_name << G4endl;
}

void GRPDetectorConstruction::ReinitializeGeometry()
{
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    runmanager->DefineWorldVolume(ConstructWorldandTarget());
    runmanager->ReinitializeGeometry();
}
