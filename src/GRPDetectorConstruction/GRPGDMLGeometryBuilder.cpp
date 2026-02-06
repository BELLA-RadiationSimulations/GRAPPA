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

#include <GRPGDMLGeometryBuilder.hpp>

GRPGDMLGeometryBuilder::GRPGDMLGeometryBuilder(const G4String & filename)
        : m_filename(filename)
{
    m_parser = std::make_unique<G4GDMLParser>();
    m_parser->Read(m_filename);
}

G4VPhysicalVolume * GRPGDMLGeometryBuilder::ConstructWorldandTarget()
 {
        return m_parser->GetWorldVolume();
    }

const SDMapping & GRPGDMLGeometryBuilder::ReturnSensitiveDetectors() const
{
    SDMapping SDmap;
    const G4GDMLAuxMapType* auxmap = m_parser->GetAuxMap();
    // Volume iterator
  for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin(); iter != auxmap->end(); iter++) {
    // Auxiliary information iterator (could be multiple per volume)
    G4LogicalVolume * volume = iter->first;
    for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
         vit != (*iter).second.end(); vit++)
    {
        const G4String auxtype = vit->type;
        if (auxtype == "SensitiveDetector")
        {
            SDmap.insert({volume, vit->value});
        }
    }
  }

    return SDmap;

}
