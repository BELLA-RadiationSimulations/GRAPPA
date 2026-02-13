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

#include <map>

#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>

class HistandNTupleManager;

using SDMapping = std::map<G4LogicalVolume *, std::pair<G4String, G4String>>;
using BiasedMapping = std::map<G4LogicalVolume *, G4bool>;

// Base class that handles the two possible geometry builders.
// This is an abstract class.
// Possible derived classes are the Standard builder and the GDML builder
class GRPGeometryBuilder
{
public:
    virtual ~GRPGeometryBuilder() = default;
    virtual G4VPhysicalVolume *ConstructWorldandTarget() = 0;
    virtual void ConstructSDandField() {};
    virtual const SDMapping ReturnSensitiveDetectors() const
    {
        G4ExceptionDescription msg;

        msg << "Trying to return a sensitive detector of a GDML geometry ";
        msg << "but GDML is not supported in this build." << G4endl;
        G4Exception(
            "GRPGeometryBuilder::ReturnSensitiveDetectors",
            "GRAPPA::GDML_UNSUPPORTED",
            G4ExceptionSeverity::FatalException,
            msg);
        return {};
    };
    virtual const BiasedMapping ReturnBiasedVolumes() const
    {
        G4ExceptionDescription msg;

        msg << "Trying to return a biased mapping of a GDML geometry ";
        msg << "but GDML is not supported in this build." << G4endl;
        G4Exception(
            "GRPGeometryBuilder::ReturnBiasedVolumes",
            "GRAPPA::GDML_UNSUPPORTED",
            G4ExceptionSeverity::FatalException,
            msg);
        return {};
    };
    virtual void
    SetHistandNtupleManager(HistandNTupleManager *histandntuplemanager)
    {
        m_HistoandNtupleManager = histandntuplemanager;
    }

protected:
    // Pointer to the customized analysis manager
    HistandNTupleManager *m_HistoandNtupleManager = nullptr;
};
