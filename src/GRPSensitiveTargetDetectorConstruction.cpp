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
    DefineCommands();
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
    // Creating foil solid
    G4Box *solidfoillayer = new G4Box(
        f_name, // its name
        0.5 * foil_x,
        0.5 * foil_y,
        0.5 * foil_z/n_layers); // its size
    // Creation foil logical volume
    m_LogicalAbsorberTarget = new G4LogicalVolume(
        solidfoillayer, // its solid
        f_material, // its material
        f_name); // its name
    // Defining stack of target layers that form the whole target
    for ( G4int i=0; i<n_layers; i++ )
    {
        // Defining position
        const G4ThreeVector f_position(foil_center[0], foil_center[1], foil_center[2]+i*foil_z/n_layers);
        // Creating foil physical volume
        new G4PVPlacement(
            nullptr, //  no rotation
            f_position, // position
            m_LogicalAbsorberTarget, // its logical volume
            f_name, // its name
            logicWorld, // its mother  volume
            false, // no boolean operation
            i, // copy number starting at 1. Copy number 0 is reserved for world volume.
            checkOverlaps); // overlaps checking
    }


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

    m_SDAbsorberWorld =
        new WorldAbsorberSD("/SDAbsorber/World", m_HistoandNtupleManager);
    m_SDAbsorberTarget =
        new TargetAbsorberSD("/SDAbsorber/Target", m_HistoandNtupleManager);

    SDMpointer->AddNewDetector(m_SDAbsorberWorld);
    SDMpointer->AddNewDetector(m_SDAbsorberTarget);

    m_LogicalAbsorberWorld->SetSensitiveDetector(m_SDAbsorberWorld);
    m_LogicalAbsorberTarget->SetSensitiveDetector(m_SDAbsorberTarget);
}

void GRPSTDetectorConstruction::DefineCommands()
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
    m_AMessenger = std::make_shared<G4GenericMessenger>(
        this,
        "/geometry/absorber/",
        "Commands to configure the particles absorber");
    // configure commands

    // Print the current status of the world and foil
    G4GenericMessenger::Command &printcommand =
        m_GenericMessenger->DeclareMethod(
            "list",
            &GRPSTDetectorConstruction::PrintDetector,
            "List the current world and detector configuration");
    printcommand.SetGuidance(" Print world and foil status ");
    printcommand.SetStates(G4State_PreInit, G4State_Idle);

    // ===============================
    // Reinitialization still WIP
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
    G4GenericMessenger::Command &layercommand =
        m_FMessenger->DeclarePropertyWithUnit(
            "layers",
            "#",
            n_layers,
            "Number of sensitive layers constituting the foil.");
    layercommand.SetGuidance(" Sets the number of foil layers ");
    layercommand.SetStates(G4State_PreInit);
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

void GRPSTDetectorConstruction::PrintDetector()
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
    G4cout << " number of layers : " << n_layers << G4endl;
    G4cout << " Foil material is " << f_material_name << G4endl;
}

// Functionality doesn't work yet. WIP
//void GRPSTDetectorConstruction::ReinitializeGeometry()
//{
//   G4RunManager *runmanager = G4RunManager::GetRunManager();
//    runmanager->DefineWorldVolume(ConstructWorldandSTarget());
//    runmanager->ReinitializeGeometry();
//}
