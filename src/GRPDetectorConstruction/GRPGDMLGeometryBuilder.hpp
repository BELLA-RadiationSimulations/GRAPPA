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

#include <GRPGeometryBuilder.hpp>

#if defined(GRAPPA_USE_GDML)
#include <G4GDMLParser.hh>

class GRPGDMLGeometryBuilder : public GRPGeometryBuilder
{
public:
    explicit GRPGDMLGeometryBuilder(const G4String &filename);

    G4VPhysicalVolume *ConstructWorldandTarget() override;
    virtual const SDMapping ReturnSensitiveDetectors() const override;

    void SetDumpObjFiles(G4bool dumpobjfile) { m_dumpObjFiles = dumpobjfile; }
    void SetDumpObjDirectory(G4String directory)
    {
        m_dumpObjDirectory = directory;
    }

private:
    G4String m_filename;
    std::unique_ptr<G4GDMLParser> m_parser;
    G4bool m_dumpObjFiles = false;
    G4String m_dumpObjDirectory;
};
#endif
