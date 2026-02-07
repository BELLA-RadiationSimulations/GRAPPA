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

#pragma once

#include <memory>
#include <G4SystemOfUnits.hh>
#include <G4VUserDetectorConstruction.hh>

#include <GRPDetectorConstructionMessenger.hpp>
#include <GRPGeometryBuilder.hpp>

class HistandNTupleManager;

class GRPDetectorConstruction : public G4VUserDetectorConstruction
{

public:
    // Constructor
    GRPDetectorConstruction(HistandNTupleManager *myanalysismanager);

    // Destructor
    virtual ~GRPDetectorConstruction();
    virtual G4VPhysicalVolume *Construct() override;

    // Sensitive Detector Construction
    void ConstructSDandField() override;

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

    void SetUseGDML(G4bool useGDML);
    void SetGDMLFilename(const G4String &filename);
    void SetDumpObjFiles(G4bool dumpobjfile) { m_dumpObjFile = dumpobjfile; }
    void SetDumpObjDirectory(G4String directory)
    {
        m_dumpObjDirectory = directory;
    }

    void PrintDetector() const;

    G4double GetFoilThickness() const { return m_foil_z; }
    G4double GetFoilSizeX() const { return m_foil_x; }
    G4double GetFoilSizeY() const { return m_foil_y; }
    G4ThreeVector GetFoilCenter() const { return m_foil_center; }
    G4String GetFoilMaterial() const { return m_f_material_name; }
    G4double GetFoilRotation() const { return m_foil_angle_y; }
    G4double GetWorldRadius() const { return m_w_radius; }
    G4String GetWorldMaterial() const { return m_w_material_name; }
    G4double GetAbsorberThickness() const { return m_absorber_thickness; }

private:
    void ConstructStandardSDandField();
    void ConstructGDMLSDandField();
    // GDML configuration
    G4bool m_UseGDML = false;
    G4String m_GDMLFilename;
    G4bool m_dumpObjFile = false;
    G4String m_dumpObjDirectory = "geometry";

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

    // Pointer to the geometry builder
    std::unique_ptr<GRPGeometryBuilder> m_builder;

    // Pointer to the physical world
    G4VPhysicalVolume *m_physWorld = nullptr;

    // Pointer to the customized analysis manager
    HistandNTupleManager *m_HistoandNtupleManager;

    // Pointer to the messenger
    std::unique_ptr<GRPDetectorConstructionMessenger>
        m_detector_construction_messenger;
};
