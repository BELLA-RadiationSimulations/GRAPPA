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

#include <GRPDetectorConstructionMessenger.hpp>
#include <GRPDetectorConstruction.hpp>

GRPDetectorConstructionMessenger::GRPDetectorConstructionMessenger(
    GRPDetectorConstruction *detectorconstruction)
    : m_detectorconstruction(detectorconstruction)
{

    m_foil_dir = std::make_unique<G4UIdirectory>("/geometry/foil/", this);
    m_foil_dir->SetGuidance("Set foil parameters");
    m_world_dir = std::make_unique<G4UIdirectory>("/geometry/world/", this);
    m_world_dir->SetGuidance("Set world parameters");
    m_absorber_dir =
        std::make_unique<G4UIdirectory>("/geometry/absorber/", this);
    m_absorber_dir->SetGuidance("Set absorber parameters");
    // Modifications to geometry are currently possible
    // but they must be done in the PreInit stage.
    // Still working or reinitializing the geometry between runs

    // Print the current status of the world and foil
    m_geometrylist_cmd =
        std::make_unique<G4UIcmdWithoutParameter>("/geometry/list", this);
    m_geometrylist_cmd->SetGuidance(
        "List the current geometry, world and detector configuration");
    m_geometrylist_cmd->SetToBeBroadcasted(false);
    m_geometrylist_cmd->AvailableForStates(
        G4State_PreInit, G4State_Init, G4State_Idle, G4State_GeomClosed);

    // Target foil thickness
    m_target_thickness = std::make_unique<G4UIcmdWithADoubleAndUnit>(
        "/geometry/foil/thickness", this);
    m_target_thickness->SetGuidance("Sets the foil size along z (thickness).");
    m_target_thickness->AvailableForStates(G4State_PreInit);
    m_target_thickness->SetUnitCategory("Length");

    // Target foil transverse size
    m_target_size = std::make_unique<G4UIcmdWithADoubleAndUnit>(
        "/geometry/foil/size", this);
    m_target_size->SetGuidance(
        "Sets the foil size along x and y (foil is assumed squared).");
    m_target_size->AvailableForStates(G4State_PreInit);
    m_target_size->SetUnitCategory("Length");

    // Target foil material
    m_target_material =
        std::make_unique<G4UIcmdWithAString>("/geometry/foil/material", this);
    m_target_material->SetGuidance("Sets the foil material");
    m_target_material->AvailableForStates(G4State_PreInit);

    // Target foil positioning
    m_target_center = std::make_unique<G4UIcmdWith3VectorAndUnit>(
        "/geometry/foil/center", this);
    m_target_center->SetGuidance("Sets the foil center.");
    m_target_center->AvailableForStates(G4State_PreInit);
    m_target_center->SetUnitCategory("Length");

    // Target foil rotation
    m_target_angle = std::make_unique<G4UIcmdWithADoubleAndUnit>(
        "/geometry/foil/rotation", this);
    m_target_angle->SetGuidance("Sets the foil rotation around the y axis.");
    m_target_angle->AvailableForStates(G4State_PreInit);
    m_target_angle->SetUnitCategory("Angle");

    // World dimension
    m_world_radius = std::make_unique<G4UIcmdWithADoubleAndUnit>(
        "/geometry/world/radius", this);
    m_world_radius->SetGuidance("Sets the world radius.");
    m_world_radius->AvailableForStates(G4State_PreInit);
    m_world_radius->SetUnitCategory("Length");

    // World material
    m_world_material =
        std::make_unique<G4UIcmdWithAString>("/geometry/world/material", this);
    m_world_material->SetGuidance("Sets the world material");
    m_world_material->AvailableForStates(G4State_PreInit);

    // Absorber thickness
    m_absorber_thickness = std::make_unique<G4UIcmdWithADoubleAndUnit>(
        "/geometry/absorber/thickness", this);
    m_absorber_thickness->SetGuidance("Sets the absorber thickness.");
    m_absorber_thickness->AvailableForStates(G4State_PreInit);
    m_absorber_thickness->SetUnitCategory("Length");

    m_use_gdml_cmd =
        std::make_unique<G4UIcmdWithABool>("/geometry/useGDML", this);
    m_use_gdml_cmd->SetGuidance("Enable geometry loading from GDML file");
    m_use_gdml_cmd->SetParameterName("useGDML", false);
    m_use_gdml_cmd->AvailableForStates(G4State_PreInit);

    m_gdml_file_cmd =
        std::make_unique<G4UIcmdWithAString>("/geometry/gdmlFile", this);
    m_gdml_file_cmd->SetGuidance("Path to GDML geometry file");
    m_gdml_file_cmd->SetParameterName("filename", false);
    m_gdml_file_cmd->AvailableForStates(G4State_PreInit);
}

GRPDetectorConstructionMessenger::~GRPDetectorConstructionMessenger() = default;

void GRPDetectorConstructionMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{

    if (!m_detectorconstruction)
    {
        G4ExceptionDescription msg;
        msg << "No detector construction is defined!";
        G4Exception(
            "GRPDetectorConstructionMessenger::SetNewValue",
            "GRAPPA::DETECTOR_CONSTRUCTION_UNDEFINED",
            FatalException,
            msg);
    }

    if (command == m_geometrylist_cmd.get())
    {
        m_detectorconstruction->PrintDetector();
    }
    if (command == m_target_thickness.get())
    {
        const G4double thickness =
            m_target_thickness->GetNewDoubleValue(newValue);
        m_detectorconstruction->SetFoilThickness(thickness);
    }
    if (command == m_target_size.get())
    {
        const G4double size = m_target_size->GetNewDoubleValue(newValue);
        m_detectorconstruction->SetFoilSize(size);
    }
    if (command == m_target_center.get())
    {
        const G4ThreeVector center =
            m_target_center->GetNew3VectorValue(newValue);
        m_detectorconstruction->SetFoilCenter(center);
    }
    if (command == m_target_angle.get())
    {
        const G4double angle = m_target_angle->GetNewDoubleValue(newValue);
        m_detectorconstruction->SetFoilRotation(angle);
    }
    if (command == m_target_material.get())
    {
        m_detectorconstruction->SetFoilMaterial(newValue);
    }
    if (command == m_world_radius.get())
    {
        const G4double radius = m_world_radius->GetNewDoubleValue(newValue);
        m_detectorconstruction->SetWorldRadius(radius);
    }
    if (command == m_world_material.get())
    {
        m_detectorconstruction->SetWorldMaterial(newValue);
    }
    if (command == m_absorber_thickness.get())
    {
        const G4double thickness =
            m_absorber_thickness->GetNewDoubleValue(newValue);
        m_detectorconstruction->SetAbsorberThickness(thickness);
    }
    if (command == m_use_gdml_cmd.get()) {
        m_detectorconstruction->SetUseGDML(m_use_gdml_cmd->GetNewBoolValue(newValue));
    }
    else if (command == m_gdml_file_cmd.get()) {
        m_detectorconstruction->SetGDMLFilename(newValue);
    }
}
