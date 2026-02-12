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

#include <G4LogicalVolumeStore.hh>

#include <GRPDetectorConstruction.hpp>

#include <GRPStoppingDetector.hpp>

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
    const G4bool checkOverlaps = true;

    // ================================================ //
    // Creating the world
    // Creating the world solid: optional a box or a sphere

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

    // Defining material of the world volume
    G4Material *w_material = nist->FindOrBuildMaterial(w_material_name);

    // Creating world logical volume
    G4LogicalVolume *logicWorld = new G4LogicalVolume(
        solidWorld, // its solid
        w_material, // its material
        w_name); // its name

    // Creating physical world volume, i.e. positioning world volume
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
    // Creating foil target
    //
    // In Testcommon there is an example on how to define a command to change
    // detector material from command line

    // Defining foil dimension and material
    // Foil is a square
    foil_y = foil_x;
    G4Material *f_material = nist->FindOrBuildMaterial(f_material_name);
    G4ThreeVector f_dimensions(foil_x, foil_y, foil_z);
    const G4ThreeVector f_position(
        foil_center[0], foil_center[1], foil_center[2]);
    G4RotationMatrix rotm = G4RotationMatrix();
    // Foil rotation by a given angle
    rotm.rotateY(foil_angle_y);

    // Creating foil solid
    G4Box *solidfoil = new G4Box(
        f_name, // its name
        0.5 * f_dimensions[0],
        0.5 * f_dimensions[1],
        0.5 * f_dimensions[2]); // its size

    // Creating foil logical volume
    G4LogicalVolume *logicfoil = new G4LogicalVolume(
        solidfoil, // its solid
        f_material, // its material
        f_name); // its name

    // Creating physical foil volume, i.e. positioning foil volume into world
    // volume
    new G4PVPlacement(
        G4Transform3D(rotm, f_position), // G4Transform3D(rotm,G4threevec);
        logicfoil, // its logical volume
        f_name, // its name
        logicWorld, // its mother  volume
        false, // no boolean operation
        0, // copy number
        checkOverlaps); // overlaps checking

    // ================================================ //
    // Creating a thin absorbing layer that coincides with the world
    // to detect particles

    // First absorber is for e+, e- and gamma
    const G4double a_dimensions = w_radius;
    G4Material *a_material = w_material;

    // Creating sensitive detector solid
    G4Sphere *StdsolidAbsorber = new G4Sphere(
        std_a_name, // its name
        a_dimensions - absorber_thickness,
        a_dimensions, // its inner and outer radii
        0.,
        360 * deg, // its phi initial and final angles
        0.,
        180 * deg); // its theta initial and final angles

    // Creating sensitive detector logical volume (with the shape of a sphere)
    G4LogicalVolume *logicalAbsorber = new G4LogicalVolume(
        StdsolidAbsorber, // its solid
        a_material, // its material
        std_a_name); // its name

    // Creating the absorber physical volume, i.e. placing its logical volume
    // into the world
    new G4PVPlacement(
        nullptr, // no rotation
        G4ThreeVector(), // at (0,0,0)
        logicalAbsorber, // its logical volume
        std_a_name, // its name
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

void GRPDetectorConstruction::ConstructSDandField()
{
    // ================================================ //
    // Link and activate sensitive detectors

    G4SDManager *SDMpointer = G4SDManager::GetSDMpointer();

    AbsorberSD *standardAbsorber = new AbsorberSD(
        "/FinalAbsorber/StandardAbsorber", m_HistoandNtupleManager);
    GRPStoppingDetector *stoppingDetector =
        new GRPStoppingDetector("StoppingDetector");

    G4LogicalVolume *logicalFoil =
        G4LogicalVolumeStore::GetInstance()->GetVolume(f_name);
    G4LogicalVolume *logicalAbsorber =
        G4LogicalVolumeStore::GetInstance()->GetVolume(std_a_name);
    SDMpointer->AddNewDetector(standardAbsorber);
    SDMpointer->AddNewDetector(stoppingDetector);
    logicalAbsorber->SetSensitiveDetector(standardAbsorber);

    logicalFoil->SetSensitiveDetector(stoppingDetector);

    // ===================================================
    // Attach biasing operator to the beam dump, the world,
    // the magnet and the power optics.
    // The world is needed for pion decay mainly, the other element for
    // pair production.
    // WARNING: with the current implementation of biasing, the new weight
    // of the particle also takes into account the artificial cross section
    // of the muon pair production. This means that we have to be careful
    // to include all the regions where a significant number of muon pairs
    // is generated, otherwise in some regions the number of muon generated,
    // artificially increased, is not appropriately compensated by their weight
    GRPSplittingOperator *splittingOperator = new GRPSplittingOperator();
    G4LogicalVolume *logicalWorld =
        G4LogicalVolumeStore::GetInstance()->GetVolume(w_name);

    splittingOperator->AttachTo(logicalWorld);
    splittingOperator->AttachTo(logicalFoil);
}

void GRPDetectorConstruction::DefineCommands()
{
    // Modifications to geometry are currently possible
    // but they must be done in the PreInit stage.
    // Still working or reinitializing the geometry between runs
    //
    // define command directory using generic messenger class
    m_GenericMessenger = std::make_shared<G4GenericMessenger>(
        this, "/geometry/", "Commands to configure the geometry");
    m_WMessenger = std::make_shared<G4GenericMessenger>(
        this, "/geometry/world/", "Commands to configure the world");
    m_FMessenger = std::make_shared<G4GenericMessenger>(
        this, "/geometry/foil/", "Commands to configure the target");
    m_AMessenger = std::make_shared<G4GenericMessenger>(
        this,
        "/geometry/absorber/",
        "Commands to configure the particle absorber");
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

    // Target foil thickness
    G4GenericMessenger::Command &thickcommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "thickness",
            "mm",
            foil_z,
            "Sets the foil size along z (thickness). Default unit is mm.");
    thickcommand.SetGuidance(" Sets the foil thickness ");
    thickcommand.SetStates(G4State_PreInit);

    // Target foil transverse size
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

    // Target foil material
    G4GenericMessenger::Command &fmaterialcommand =
        m_FMessenger->DeclareProperty(
            "material", f_material_name, "Sets the foil material.");
    fmaterialcommand.SetGuidance(" Sets the foil material ");
    fmaterialcommand.SetStates(G4State_PreInit);

    // Target foil positioning
    G4GenericMessenger::Command &centercommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "center",
            "mm",
            foil_center,
            "Sets the foil center. Default unit is mm.");
    centercommand.SetGuidance(" Sets the foil center ");
    centercommand.SetStates(G4State_PreInit);

    // Target foil rotation
    G4GenericMessenger::Command &anglecommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "rotation",
            "deg",
            foil_angle_y,
            "Sets the foil rotation around the y axis. Default unit is deg.");
    anglecommand.SetGuidance(" Sets the foil center ");
    anglecommand.SetStates(G4State_PreInit);

    // World dimension
    G4GenericMessenger::Command &wradiuscommand =
        m_WMessenger->DeclarePropertyWithUnit(
            "radius",
            "mm",
            w_radius,
            "Sets the world radius. Default unit is mm.");
    wradiuscommand.SetGuidance(" Sets the world radius ");
    wradiuscommand.SetStates(G4State_PreInit);

    // World material
    G4GenericMessenger::Command &wmaterialcommand =
        m_WMessenger->DeclareProperty(
            "material", w_material_name, "Sets the world material.");
    wmaterialcommand.SetGuidance(" Sets the world material ");
    wmaterialcommand.SetStates(G4State_PreInit);

    // Absorber thickness
    G4GenericMessenger::Command &absthicknesscommand =
        m_AMessenger->DeclarePropertyWithUnit(
            "thickness",
            "mm",
            absorber_thickness,
            "Sets the thickness of the absorber. Default unit is mm.");
    absthicknesscommand.SetGuidance(
        " Change the thickness of the final particles absorber that surrounds "
        "the world.");
    absthicknesscommand.SetStates(G4State_PreInit);
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
    G4cout << " The absorber that surrounds the world has a thickness "
           << absorber_thickness << " mm" << G4endl;
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
