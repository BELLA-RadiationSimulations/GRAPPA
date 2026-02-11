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

#include <filesystem>

#include <G4Material.hh>
#include <G4VisAttributes.hh>

#include <GRPGDMLGeometryBuilder.hpp>

struct VolumeInfo
{
    G4VSolid *solid;
    G4String name;
    G4String materialName;
    G4AffineTransform transform;
};

void CollectVolumes(
    G4VPhysicalVolume *pv,
    const G4AffineTransform &parentTransform,
    std::vector<VolumeInfo> &volumes,
    bool skipWorld = true)
{
    G4RotationMatrix rot = pv->GetObjectRotationValue();
    G4ThreeVector trans = pv->GetTranslation();
    G4AffineTransform local(rot, trans);
    G4AffineTransform global = parentTransform * local;

    G4LogicalVolume *lv = pv->GetLogicalVolume();

    if (!skipWorld)
    {
        VolumeInfo info;
        info.solid = lv->GetSolid();
        info.name = pv->GetName();
        info.materialName = lv->GetMaterial()->GetName();
        info.transform = global;
        volumes.push_back(info);
    }

    for (size_t i = 0; i < lv->GetNoDaughters(); ++i)
    {
        CollectVolumes(lv->GetDaughter(i), global, volumes, false);
    }
}

void WriteOBJ(const VolumeInfo &vol, const std::string &directory)
{
    auto polyhedron = vol.solid->GetPolyhedron();
    if (!polyhedron)
        return;

    std::string filename = directory + "/" + vol.name + ".obj";
    std::ofstream out(filename);
    out << "# Volume: " << vol.name << "\n";
    out << "# Material: " << vol.materialName << "\n";
    out << "o " << vol.name << "\n";

    int nVerts = polyhedron->GetNoVertices();
    for (int i = 1; i <= nVerts; ++i)
    {
        G4Point3D v = polyhedron->GetVertex(i);
        G4ThreeVector vt(v.x(), v.y(), v.z());
        vt = vol.transform.NetRotation() * vt + vol.transform.NetTranslation();
        out << "v " << vt.x() << " " << vt.y() << " " << vt.z() << "\n";
    }

    int nFacets = polyhedron->GetNoFacets();
    for (int f = 1; f <= nFacets; ++f)
    {
        G4int n;
        G4int nodes[4];
        G4int edgeFlags[4];
        polyhedron->GetFacet(f, n, nodes, edgeFlags);

        out << "f";
        for (int j = 0; j < n; ++j)
        {
            out << " " << std::abs(nodes[j]);
        }
        out << "\n";
    }

    out.close();
}

void ExportAllVolumes(G4VPhysicalVolume *world, const std::string &directory)
{
    std::filesystem::create_directories(directory);

    std::vector<VolumeInfo> volumes;
    G4AffineTransform identity;
    CollectVolumes(world, identity, volumes, true);

    // Write individual files
    for (const auto &vol : volumes)
    {
        WriteOBJ(vol, directory);
    }

    // Write a manifest so you know what's there
    std::ofstream manifest(directory + "/manifest.txt");
    manifest << "# name | material | file\n";
    for (const auto &vol : volumes)
    {
        manifest << vol.name << " | " << vol.materialName << " | " << vol.name
                 << ".obj\n";
    }
    manifest.close();
}

GRPGDMLGeometryBuilder::GRPGDMLGeometryBuilder(const G4String &filename)
    : m_filename(filename)
{
    m_parser = std::make_unique<G4GDMLParser>();
    m_parser->Read(m_filename);

    m_supportedSensitiveDetectors.insert("FinalAbsorber");
}

G4VPhysicalVolume *GRPGDMLGeometryBuilder::ConstructWorldandTarget()
{
    G4VPhysicalVolume *world = m_parser->GetWorldVolume();
    if (m_dumpObjFiles)
    {
        ExportAllVolumes(world, m_dumpObjDirectory);
    }
    G4VisAttributes *worldVisAtt =
        new G4VisAttributes(false); // Setting world visible

    world->GetLogicalVolume()->SetVisAttributes(worldVisAtt);
    return world;
}

const SDMapping GRPGDMLGeometryBuilder::ReturnSensitiveDetectors() const
{
    SDMapping SDmap;
    const G4GDMLAuxMapType *auxmap = m_parser->GetAuxMap();

    // With this loop we check the auxiliary map of the GDML file
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
         iter != auxmap->end();
         iter++)
    {
        // For a given logical volume we check the auxiliary variables
        // We assume it's the definition of a sensitive detector if
        // the "type" is one of the types supported as a sensitive detector.
        // The second variable is then the detector name
        G4LogicalVolume *volume = iter->first;
        for (G4GDMLAuxListType::const_iterator vit = iter->second.begin();
             vit != iter->second.end();
             vit++)
        {
            const G4String auxtype = vit->type;
            const G4String auxvalue = vit->value;

            //
            if (m_supportedSensitiveDetectors.count(auxtype))
            {
                SDmap.insert({
                    volume, {auxtype, auxvalue}
                });
            }
        }
    }

    return SDmap;
}

const BiasedMapping GRPGDMLGeometryBuilder::ReturnBiasedVolumes() const
{
    BiasedMapping BiasMap;
    const G4GDMLAuxMapType *auxmap = m_parser->GetAuxMap();
    // Volume iterator
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
         iter != auxmap->end();
         iter++)
    {
        // Auxiliary information iterator (could be multiple per volume)
        G4LogicalVolume *volume = iter->first;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
             vit != (*iter).second.end();
             vit++)
        {
            const G4String auxtype = vit->type;
            if (auxtype == "Biasing")
            {
                if (vit->value != "true" && vit->value != "false")
                {
                    G4ExceptionDescription msg;
                    msg << "The auxiliary value passed to volume "
                        << volume->GetName()
                        << " for Biasing cannot be parsed as 'true' or 'false'"
                        << G4endl;
                    G4Exception(
                        "GRPGDMLGeometryBuilder::ReturnBiasedVolumes",
                        "GRAPPA::INCORRECT_BOOLEAN",
                        G4ExceptionSeverity::FatalException,
                        msg);
                }
                if (vit->value == "true")
                {
                    BiasMap.insert({volume, true});
                }
            }
        }
    }

    return BiasMap;
}
