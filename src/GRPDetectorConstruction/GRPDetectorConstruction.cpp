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

#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>

#include <GRPDetectorConstruction.hpp>
#include <GRPHistandNTupleManager.hpp>
#include <GRPSDAbsorber.hpp>
#include <GRPSplittingOperator.hpp>
#include <GRPStandardGeometryBuilder.hpp>

#if defined(GRAPPA_USE_GDML)
#include <GRPGDMLGeometryBuilder.hpp>
#endif

GRPDetectorConstruction::GRPDetectorConstruction(
    HistandNTupleManager *myanalysismanager)
    : m_HistoandNtupleManager(myanalysismanager)
{
    m_detector_construction_messenger =
        std::make_unique<GRPDetectorConstructionMessenger>(this);
}

GRPDetectorConstruction::~GRPDetectorConstruction() = default;

G4VPhysicalVolume *GRPDetectorConstruction::Construct()
{
    if (m_UseGDML)
    {
#if defined(GRAPPA_USE_GDML)
        m_builder = std::make_unique<GRPGDMLGeometryBuilder>(m_GDMLFilename);
#else
        G4ExceptionDescription msg;

        msg << "WARNING: GDML requested but not available in this build. ";
        msg << "Falling back to standard geometry." << G4endl;
        G4Exception(
            "GRPDetectorConstruction::Construct",
            "GRAPPA::GDML_UNSUPPORTED",
            G4ExceptionSeverity::JustWarning,
            msg);
        m_UseGDML = false;
#endif
    }

    if (!m_UseGDML)
    {
        auto standardBuilder = std::make_unique<GRPStandardGeometryBuilder>();
        standardBuilder->SetFoilThickness(m_foil_z);
        standardBuilder->SetFoilSize(m_foil_x); // assumes square
        standardBuilder->SetFoilCenter(m_foil_center);
        standardBuilder->SetFoilMaterial(m_f_material_name);
        standardBuilder->SetFoilRotation(m_foil_angle_y);
        standardBuilder->SetWorldRadius(m_w_radius);
        standardBuilder->SetWorldMaterial(m_w_material_name);
        standardBuilder->SetAbsorberThickness(m_absorber_thickness);
        m_builder = std::move(standardBuilder);
    }

    m_physWorld = m_builder->ConstructWorldandTarget();
    return m_physWorld;
}

void GRPDetectorConstruction::ConstructSDandField()
{
    // ================================================ //
    // Link and activate sensitive detectors

    G4SDManager *SDMpointer = G4SDManager::GetSDMpointer();

    G4VSensitiveDetector *previousdetector =
        SDMpointer->FindSensitiveDetector("/FinalAbsorber/StandardAbsorber");
    if (!previousdetector)
    {
        AbsorberSD *standardAbsorber = new AbsorberSD(
            "/FinalAbsorber/StandardAbsorber", m_HistoandNtupleManager);

        SDMpointer->AddNewDetector(standardAbsorber);
        SetSensitiveDetector("AbsorberLogical", standardAbsorber);
    }
    // ===================================================
    // Attach biasing operator to the target and the world.
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
        G4LogicalVolumeStore::GetInstance()->GetVolume("WorldLogical");
    G4LogicalVolume *logicalFoil =
        G4LogicalVolumeStore::GetInstance()->GetVolume("FoilLogical");

    splittingOperator->AttachTo(logicalWorld);
    splittingOperator->AttachTo(logicalFoil);
}

void GRPDetectorConstruction::PrintDetector() const
{
    if (m_UseGDML)
    {
        G4cout << "Geometry loaded from GDML file: " << m_GDMLFilename
               << G4endl;
    }
    else
    {
        G4cout << " World and target information " << G4endl;
        G4cout
            << " ------------------------------------------------------------- "
            << G4endl;
        G4cout << G4endl;
        G4cout << " The world is a sphere centered in 0 0 0 mm" << G4endl;
        G4cout << " Sphere radius: " << m_w_radius / mm << " mm" << G4endl;
        G4cout << " World material: " << m_w_material_name << G4endl;
        G4cout << G4endl;
        G4cout << " Absorber thickness: " << m_absorber_thickness / mm << " mm"
               << G4endl;
        G4cout << G4endl;
        G4cout << " Target: Foil" << G4endl;
        G4cout << " Center: " << m_foil_center / mm << " mm" << G4endl;
        G4cout << " Size x: " << m_foil_x / mm << " mm" << G4endl;
        G4cout << " Size y: " << m_foil_y / mm << " mm" << G4endl;
        G4cout << " Size z: " << m_foil_z / mm << " mm" << G4endl;
        G4cout << " Rotation (y): " << m_foil_angle_y / deg << " deg" << G4endl;
        G4cout << " Material: " << m_f_material_name << G4endl;
    }
}

void GRPDetectorConstruction::SetUseGDML(G4bool useGDML)
{
#if defined(GRAPPA_USE_GDML)
    m_UseGDML = useGDML;
#else
            G4ExceptionDescription msg;

        msg << "WARNING: GDML requested but not available in this build. ";
        msg << "Falling back to standard geometry." << G4endl;
        G4Exception(
            "GRPDetectorConstruction::SetUseGDML",
            "GRAPPA::GDML_UNSUPPORTED",
            G4ExceptionSeverity::JustWarning,
            msg);
        m_UseGDML = false;
#endif
}

void GRPDetectorConstruction::SetGDMLFilename(const G4String &filename)
{
    #if defined(GRAPPA_USE_GDML)
    m_GDMLFilename = filename;
#else
            G4ExceptionDescription msg;

        msg << "WARNING: GDML requested but not available in this build. ";
        msg << "Falling back to standard geometry." << G4endl;
        G4Exception(
            "GRPDetectorConstruction::SetUseGDML",
            "GRAPPA::GDML_UNSUPPORTED",
            G4ExceptionSeverity::JustWarning,
            msg);
        m_UseGDML = false;
#endif
}
