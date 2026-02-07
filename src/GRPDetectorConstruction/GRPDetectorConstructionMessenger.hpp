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

#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

class GRPDetectorConstruction;

class GRPDetectorConstructionMessenger : public G4UImessenger
{
public:
    explicit GRPDetectorConstructionMessenger(GRPDetectorConstruction *);
    ~GRPDetectorConstructionMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    GRPDetectorConstruction *m_detectorconstruction;
    std::unique_ptr<G4UIdirectory> m_foil_dir;
    std::unique_ptr<G4UIdirectory> m_world_dir;
    std::unique_ptr<G4UIdirectory> m_absorber_dir;
    std::unique_ptr<G4UIcmdWithoutParameter> m_geometrylist_cmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> m_target_thickness;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> m_target_size;
    std::unique_ptr<G4UIcmdWith3VectorAndUnit> m_target_center;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> m_target_angle;
    std::unique_ptr<G4UIcmdWithAString> m_target_material;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> m_world_radius;
    std::unique_ptr<G4UIcmdWithAString> m_world_material;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> m_absorber_thickness;

    std::unique_ptr<G4UIcmdWithABool> m_use_gdml_cmd;
    std::unique_ptr<G4UIcmdWithAString> m_gdml_file_cmd;
    std::unique_ptr<G4UIcmdWithABool> m_dump_obj;
    std::unique_ptr<G4UIcmdWithAString> m_dump_directory;
};
