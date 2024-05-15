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

#include <GRPHistandNTupleManager.hpp>

HistandNTupleManager::HistandNTupleManager() : fFileName("Data_output")
{
    m_HistoManager = new HistoManager();
    m_NtupleManager = new NTupleManager();
}

HistandNTupleManager::~HistandNTupleManager() {}

void HistandNTupleManager::FinishAnalysis()
{
    // Close Analysis and write files
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive())
    {
        analysisManager->Write();
        analysisManager->CloseFile();
    }
}

void HistandNTupleManager::OpenFile()
{
    // Open the analysis
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive())
    {
        analysisManager->OpenFile();
    }
}

void HistandNTupleManager::Book()
{
    // Introduce an analysis manager
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(0);

    analysisManager->SetDefaultFileType("root");
    // Note: merging ntuples is available only with Root output
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetFileName(fFileName);

    // This option enables the manual activation and deactivation of
    // single histograms and ntuple. A little more verbose in the code, but
    // could potentially allow finer control in macro files
    analysisManager->SetActivation(true);

    m_HistoManager->Book();
    m_NtupleManager->Book();
}

HistoManager::HistoManager() {}

HistoManager::~HistoManager() {}

void HistoManager::Book()
{
    // Introduce an analysis manager for histogram generation
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetHistoDirectoryName("histo");

    // Define histograms of interest
    // Once a particle source is defined (e.g. GPS), histogram limits can be
    // changed based on that

    // 1D
    // HistogramId = 0
    m_primaryeneid = analysisManager->CreateH1(
        "FinalPrimaryEnergy",
        "Final Energy of primaries",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(m_primaryeneid, m_activatehistograms);
    // HistogramId = 1
    m_posieneid = analysisManager->CreateH1(
        "FinalPositronEnergy",
        "Final Energy of positrons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(m_posieneid, m_activatehistograms);
    // HistogramId = 2
    m_electroneneid = analysisManager->CreateH1(
        "FinalElectronEnergy",
        "Final Energy of electrons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(m_electroneneid, m_activatehistograms);
    // HistogramId = 3
    m_gammaeneid = analysisManager->CreateH1(
        "FinalGammaEnergy",
        "Final Energy of photons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(m_gammaeneid, m_activatehistograms);
    // HistogramId = 4
    m_pioneneid = analysisManager->CreateH1(
        "FinalPionEnergy",
        "Final Energy of pions",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(m_pioneneid, m_activatehistograms);
    // HistogramId = 5
    m_muoneneid = analysisManager->CreateH1(
        "FinalMuonEnergy",
        "Final Energy of muons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(m_muoneneid, m_activatehistograms);
    // HistogramId = 6
    m_initialprimaryeneid = analysisManager->CreateH1(
        "InitialPrimaryEnergy",
        "Initial Energy of primaries",
        50,
        9.9 * GeV,
        10.1 * GeV,
        "GeV");
    analysisManager->SetH1Activation(
        m_initialprimaryeneid, m_activatehistograms);

    // Final theta distributions
    // HistogramId = 7
    m_primarytheta = analysisManager->CreateH1(
        "PrimaryTheta",
        "Final #theta of primaries",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_primarytheta, m_activatehistograms);
    // HistogramId = 8
    m_electrontheta = analysisManager->CreateH1(
        "ElectronTheta",
        "Final #theta of electrons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_electrontheta, m_activatehistograms);
    // HistogramId = 9
    m_positrontheta = analysisManager->CreateH1(
        "PositronTheta",
        "Final #theta of positrons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_positrontheta, m_activatehistograms);
    // HistogramId = 10
    m_gammatheta = analysisManager->CreateH1(
        "GammaTheta",
        "Final #theta of photons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_gammatheta, m_activatehistograms);
    // HistogramId = 11
    m_piontheta = analysisManager->CreateH1(
        "PionTheta",
        "Final #theta of pions",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_piontheta, m_activatehistograms);
    // HistogramId = 12
    m_muontheta = analysisManager->CreateH1(
        "MuonTheta",
        "Final #theta of muons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_muontheta, m_activatehistograms);

    // Final phi distributions
    // HistogramId = 13
    m_primaryphi = analysisManager->CreateH1(
        "PrimaryPhi",
        "Final #phi of primaries",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_primaryphi, m_activatehistograms);
    // HistogramId = 14
    m_electronphi = analysisManager->CreateH1(
        "ElectronPhi",
        "Final #phi of electrons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_electronphi, m_activatehistograms);
    // HistogramId = 15
    m_positronphi = analysisManager->CreateH1(
        "PositronPhi",
        "Final #phi of positrons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_positronphi, m_activatehistograms);
    // HistogramId = 16
    m_gammaphi = analysisManager->CreateH1(
        "GammaPhi",
        "Final #phi of photons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_gammaphi, m_activatehistograms);
    // HistogramId = 17
    m_pionphi = analysisManager->CreateH1(
        "PionPhi",
        "Final #phi of pions",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_pionphi, m_activatehistograms);
    // HistogramId = 18
    m_muonphi = analysisManager->CreateH1(
        "MuonPhi",
        "Final #phi of muons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(m_muonphi, m_activatehistograms);

    // 2D
    // HistogramId = 0
    // Initial primary transverse distribution
    m_initialprimaryxyid = analysisManager->CreateH2(
        "InitialPrimaryXY",
        "Initial primary transverse distribution",
        15,
        -100 * um,
        100 * um,
        15,
        -100 * um,
        100 * um,
        "um",
        "um");
    analysisManager->SetH2Activation(
        m_initialprimaryxyid, m_activatehistograms);
    // HistogramId = 1
    // Initial primary longitudinal distribution
    m_initialprimaryzxid = analysisManager->CreateH2(
        "InitialPrimaryZX",
        "Initial primary longitudinal distribution",
        15,
        -100 * um,
        100 * um,
        15,
        -100 * um,
        100 * um,
        "um",
        "um");
    analysisManager->SetH2Activation(
        m_initialprimaryzxid, m_activatehistograms);
    // Initial primary angular distribution
    // HistogramId = 2
    m_initialprimarytxtyid = analysisManager->CreateH2(
        "InitialPrimaryangles",
        "Initial primary angle distribution",
        10,
        -20 * mrad,
        20 * mrad,
        10,
        -20 * mrad,
        20 * mrad,
        "mrad",
        "mrad");
    analysisManager->SetH2Activation(
        m_initialprimarytxtyid, m_activatehistograms);
    // Initial primary trasverse x-phasespace
    // HistogramId = 3
    m_initialprimaryxpxid = analysisManager->CreateH2(
        "InitialPrimaryXPhaseSpace",
        "Initial X phase space of primaries",
        20,
        -20 * um,
        20 * um,
        20,
        -20 * MeV,
        20 * MeV,
        "um",
        "MeV");
    analysisManager->SetH2Activation(
        m_initialprimaryxpxid, m_activatehistograms);
    // Initial primary trasverse y-phasespace
    // HistogramId = 4
    m_initialprimaryypyid = analysisManager->CreateH2(
        "InitialPrimaryYPhaseSpace",
        "Initial Y phase space of primaries",
        20,
        -20 * um,
        20 * um,
        20,
        -20 * MeV,
        20 * MeV,
        "um",
        "MeV");
    analysisManager->SetH2Activation(
        m_initialprimaryypyid, m_activatehistograms);

    // Final primary transverse distribution
    // HistogramId = 5
    m_finalprimaryxyid = analysisManager->CreateH2(
        "FinalPrimaryXY",
        "Final primary transverse distribution",
        40,
        -1.5 * mm,
        1.5 * mm,
        40,
        -1.5 * mm,
        1.5 * mm,
        "mm",
        "mm");
    analysisManager->SetH2Activation(m_finalprimaryxyid, m_activatehistograms);
    // Final electron transverse distribution
    // HistogramId = 6
    m_finalelectronxyid = analysisManager->CreateH2(
        "FinalElectronXY",
        "Final electron transverse distribution",
        40,
        -1.5 * mm,
        1.5 * mm,
        40,
        -1.5 * mm,
        1.5 * mm,
        "mm",
        "mm");
    analysisManager->SetH2Activation(m_finalelectronxyid, m_activatehistograms);
    // Final positron transverse distribution
    // HistogramId = 7
    m_finalpositronxyid = analysisManager->CreateH2(
        "FinalPositronXY",
        "Final positron transverse distribution",
        40,
        -1.5 * mm,
        1.5 * mm,
        40,
        -1.5 * mm,
        1.5 * mm,
        "mm",
        "mm");
    analysisManager->SetH2Activation(m_finalpositronxyid, m_activatehistograms);
    // Final gamma transverse distribution
    // HistogramId = 8
    m_finalgammaxyid = analysisManager->CreateH2(
        "FinalGammaXY",
        "Final photon transverse distribution",
        40,
        -1.5 * mm,
        1.5 * mm,
        40,
        -1.5 * mm,
        1.5 * mm,
        "mm",
        "mm");
    analysisManager->SetH2Activation(m_finalgammaxyid, m_activatehistograms);
    // Final pion transverse distribution
    // HistogramId = 9
    m_finalpionxyid = analysisManager->CreateH2(
        "FinalPionXY",
        "Final pion transverse distribution",
        40,
        -1.5 * mm,
        1.5 * mm,
        40,
        -1.5 * mm,
        1.5 * mm,
        "mm",
        "mm");
    analysisManager->SetH2Activation(m_finalpionxyid, m_activatehistograms);
    // Final muon transverse distribution
    // HistogramId = 10
    m_finalmuonxyid = analysisManager->CreateH2(
        "FinalMuonXY",
        "Final muon transverse distribution",
        40,
        -1.5 * mm,
        1.5 * mm,
        40,
        -1.5 * mm,
        1.5 * mm,
        "mm",
        "mm");
    analysisManager->SetH2Activation(m_finalmuonxyid, m_activatehistograms);

    // Final primary angle distribution
    // HistogramId = 11
    m_finalprimarytxtyid = analysisManager->CreateH2(
        "FinalPrimaryAngles",
        "Final primary angle distribution",
        40,
        -1.5 * rad,
        1.5 * rad,
        40,
        -1.5 * rad,
        1.5 * rad,
        "rad",
        "rad");
    analysisManager->SetH2Activation(
        m_finalprimarytxtyid, m_activatehistograms);
    // Final electron angle distribution
    // HistogramId = 12
    m_finalelectrontxtyid = analysisManager->CreateH2(
        "FinalElectronAngles",
        "Final electron angle distribution",
        40,
        -1.5 * rad,
        1.5 * rad,
        40,
        -1.5 * rad,
        1.5 * rad,
        "rad",
        "rad");
    analysisManager->SetH2Activation(
        m_finalelectrontxtyid, m_activatehistograms);
    // Final positron angle distribution
    // HistogramId = 13
    m_finalpositrontxtyid = analysisManager->CreateH2(
        "FinalPositronAngles",
        "Final positron angle distribution",
        40,
        -1.5 * rad,
        1.5 * rad,
        40,
        -1.5 * rad,
        1.5 * rad,
        "rad",
        "rad");
    analysisManager->SetH2Activation(
        m_finalpositrontxtyid, m_activatehistograms);
    // Final gamma angle distribution
    // HistogramId = 14
    m_finalgammatxtyid = analysisManager->CreateH2(
        "FinalGammaAngles",
        "Final photon angle distribution",
        40,
        -1.5 * rad,
        1.5 * rad,
        40,
        -1.5 * rad,
        1.5 * rad,
        "rad",
        "rad");
    analysisManager->SetH2Activation(m_finalgammatxtyid, m_activatehistograms);
    // Final gamma angle distribution
    // HistogramId = 15
    m_finalpiontxtyid = analysisManager->CreateH2(
        "FinalPionAngles",
        "Final pion angle distribution",
        40,
        -1.5 * rad,
        1.5 * rad,
        40,
        -1.5 * rad,
        1.5 * rad,
        "rad",
        "rad");
    analysisManager->SetH2Activation(m_finalpiontxtyid, m_activatehistograms);
    // Final gamma angle distribution
    // HistogramId = 16
    m_finalmuontxtyid = analysisManager->CreateH2(
        "FinalMuonAngles",
        "Final muon angle distribution",
        40,
        -1.5 * rad,
        1.5 * rad,
        40,
        -1.5 * rad,
        1.5 * rad,
        "rad",
        "rad");
    analysisManager->SetH2Activation(m_finalmuontxtyid, m_activatehistograms);
}

NTupleManager::NTupleManager() {}

NTupleManager::~NTupleManager() {}

void NTupleManager::Book()
{
    // Introduce an analysis manager for Ntuple generation
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleDirectoryName("ntuple");
    // Creation on ntuples

    // NTuple 0: Phase space of particles generated at source
    m_primaryinitialid = analysisManager->CreateNtuple(
        "InitialPrimary", "Initial primary particles phase space");
    analysisManager->CreateNtupleFColumn(m_primaryinitialid, "x");
    analysisManager->CreateNtupleFColumn(m_primaryinitialid, "y");
    analysisManager->CreateNtupleFColumn(m_primaryinitialid, "z");
    analysisManager->CreateNtupleFColumn(m_primaryinitialid, "px");
    analysisManager->CreateNtupleFColumn(m_primaryinitialid, "py");
    analysisManager->CreateNtupleFColumn(m_primaryinitialid, "pz");
    analysisManager->FinishNtuple(m_primaryinitialid);
    analysisManager->SetNtupleActivation(
        m_primaryinitialid, m_primaryinitialidactive);

    // NTuple 1: Phase space of final primaries
    m_primaryid = analysisManager->CreateNtuple(
        "FinalPrimary", "Final primary particles phase space");
    analysisManager->CreateNtupleFColumn(m_primaryid, "x");
    analysisManager->CreateNtupleFColumn(m_primaryid, "y");
    analysisManager->CreateNtupleFColumn(m_primaryid, "z");
    analysisManager->CreateNtupleFColumn(m_primaryid, "px");
    analysisManager->CreateNtupleFColumn(m_primaryid, "py");
    analysisManager->CreateNtupleFColumn(m_primaryid, "pz");
    analysisManager->CreateNtupleFColumn(m_primaryid, "t");
    analysisManager->CreateNtupleSColumn(m_primaryid, "detector");
    analysisManager->FinishNtuple(m_primaryid);
    analysisManager->SetNtupleActivation(m_primaryid, m_primaryactive);

    // NTuple 2: Phase space of final positrons
    m_posiid = analysisManager->CreateNtuple(
        "FinalPositron", "Final positrons phase space");
    analysisManager->CreateNtupleFColumn(m_posiid, "x");
    analysisManager->CreateNtupleFColumn(m_posiid, "y");
    analysisManager->CreateNtupleFColumn(m_posiid, "z");
    analysisManager->CreateNtupleFColumn(m_posiid, "px");
    analysisManager->CreateNtupleFColumn(m_posiid, "py");
    analysisManager->CreateNtupleFColumn(m_posiid, "pz");
    analysisManager->CreateNtupleFColumn(m_posiid, "t");
    analysisManager->CreateNtupleSColumn(m_posiid, "detector");
    analysisManager->CreateNtupleSColumn(m_posiid, "process");
    analysisManager->FinishNtuple(m_posiid);
    analysisManager->SetNtupleActivation(m_posiid, m_posiactive);

    // NTuple 3: Phase space of final electrons
    m_electronid = analysisManager->CreateNtuple(
        "FinalElectron", "Final electrons phase space");
    analysisManager->CreateNtupleFColumn(m_electronid, "x");
    analysisManager->CreateNtupleFColumn(m_electronid, "y");
    analysisManager->CreateNtupleFColumn(m_electronid, "z");
    analysisManager->CreateNtupleFColumn(m_electronid, "px");
    analysisManager->CreateNtupleFColumn(m_electronid, "py");
    analysisManager->CreateNtupleFColumn(m_electronid, "pz");
    analysisManager->CreateNtupleFColumn(m_electronid, "t");
    analysisManager->CreateNtupleSColumn(m_electronid, "detector");
    analysisManager->CreateNtupleSColumn(m_electronid, "process");
    analysisManager->FinishNtuple(m_electronid);
    analysisManager->SetNtupleActivation(m_electronid, m_electronactive);

    // NTuple 4: Phase space of final photons
    m_gammaid = analysisManager->CreateNtuple(
        "FinalPhoton", "Final photons phase space");
    analysisManager->CreateNtupleFColumn(m_gammaid, "x");
    analysisManager->CreateNtupleFColumn(m_gammaid, "y");
    analysisManager->CreateNtupleFColumn(m_gammaid, "z");
    analysisManager->CreateNtupleFColumn(m_gammaid, "px");
    analysisManager->CreateNtupleFColumn(m_gammaid, "py");
    analysisManager->CreateNtupleFColumn(m_gammaid, "pz");
    analysisManager->CreateNtupleFColumn(m_gammaid, "t");
    analysisManager->CreateNtupleSColumn(m_gammaid, "detector");
    analysisManager->FinishNtuple(m_gammaid);
    analysisManager->SetNtupleActivation(m_gammaid, m_gammaactive);

    // NTuple 5: Phase space of final pions
    m_pionid =
        analysisManager->CreateNtuple("FinalPion", "Final pions phase space");
    analysisManager->CreateNtupleFColumn(m_pionid, "x");
    analysisManager->CreateNtupleFColumn(m_pionid, "y");
    analysisManager->CreateNtupleFColumn(m_pionid, "z");
    analysisManager->CreateNtupleFColumn(m_pionid, "px");
    analysisManager->CreateNtupleFColumn(m_pionid, "py");
    analysisManager->CreateNtupleFColumn(m_pionid, "pz");
    analysisManager->CreateNtupleFColumn(m_pionid, "t");
    analysisManager->CreateNtupleSColumn(m_pionid, "detector");
    analysisManager->CreateNtupleFColumn(m_pionid, "q");
    analysisManager->CreateNtupleSColumn(m_pionid, "process");
    analysisManager->FinishNtuple(m_pionid);
    analysisManager->SetNtupleActivation(m_pionid, m_pionactive);

    // NTuple 6: Phase space of final muons
    m_muonid =
        analysisManager->CreateNtuple("FinalMuon", "Final muons phase space");
    analysisManager->CreateNtupleFColumn(m_muonid, "x");
    analysisManager->CreateNtupleFColumn(m_muonid, "y");
    analysisManager->CreateNtupleFColumn(m_muonid, "z");
    analysisManager->CreateNtupleFColumn(m_muonid, "px");
    analysisManager->CreateNtupleFColumn(m_muonid, "py");
    analysisManager->CreateNtupleFColumn(m_muonid, "pz");
    analysisManager->CreateNtupleFColumn(m_muonid, "t");
    analysisManager->CreateNtupleSColumn(m_muonid, "detector");
    analysisManager->CreateNtupleFColumn(m_muonid, "q");
    analysisManager->CreateNtupleSColumn(m_muonid, "process");
    analysisManager->FinishNtuple(m_muonid);
    analysisManager->SetNtupleActivation(m_muonid, m_muonactive);

    // NTuple 7: Particle count
    m_primarycountid =
        analysisManager->CreateNtuple("PrimaryCount", "Counting primaries");
    analysisManager->CreateNtupleIColumn(m_primarycountid, "N");
    analysisManager->FinishNtuple(m_primarycountid);
    analysisManager->SetNtupleActivation(
        m_primarycountid, m_primarycountactive);
}

void NTupleManager::ListNtuples()
{
    G4cout << "List of all the available NTuples" << G4endl;
    G4cout << " ID   Name   ActivationStatus" << G4endl;
    G4cout << " " << m_primaryinitialid << "  "
           << "InitialPrimary"
           << "  " << m_primaryinitialidactive << G4endl;
    G4cout << " " << m_primaryid << "  "
           << "FinalPrimary"
           << "  " << m_primaryactive << G4endl;
    G4cout << " " << m_posiid << "  "
           << "FinalPositron"
           << "  " << m_posiactive << G4endl;
    G4cout << " " << m_electronid << "  "
           << "FinalElectron"
           << "  " << m_electronactive << G4endl;
    G4cout << " " << m_gammaid << "  "
           << "FinalPhoton"
           << "  " << m_gammaactive << G4endl;
    G4cout << " " << m_pionid << "  "
           << "FinalPion"
           << "  " << m_pionactive << G4endl;
    G4cout << " " << m_muonid << "  "
           << "FinalMuon"
           << "  " << m_muonactive << G4endl;
    G4cout << " " << m_primarycountid << "  "
           << "PrimaryCount"
           << "  " << m_primarycountactive << G4endl;
}

void NTupleManager::SetNtupleDump(G4int ID, G4bool ifdump)
{
    if (ID == m_primaryinitialid)
    {
        m_primaryinitialidactive = ifdump;
    }
    else if (ID == m_primaryid)
    {
        m_primaryactive = ifdump;
    }
    else if (ID == m_posiid)
    {
        m_posiactive = ifdump;
    }
    else if (ID == m_electronid)
    {
        m_electronactive = ifdump;
    }
    else if (ID == m_gammaid)
    {
        m_gammaactive = ifdump;
    }
    else if (ID == m_muonid)
    {
        m_muonactive = ifdump;
    }
    else if (ID == m_pionid)
    {
        m_pionactive = ifdump;
    }
    else if (ID == m_primarycountid)
    {
        m_primarycountactive = ifdump;
    }
    else
    {
        G4ExceptionDescription msg;
        msg << "No NTuple found with id = " << ID;
        G4Exception(
            "NTupleManager::SetNtupleDump",
            "GRAPPA::NO_NTUPLE_FOUND",
            G4ExceptionSeverity::JustWarning,
            msg);
    }
}

G4bool NTupleManager::GetIdActivation(const G4int ID) const
{
    if (ID == m_primaryinitialid)
    {
        return GetPrimaryInitialActivation();
    }
    else if (ID == m_primaryid)
    {
        return GetPrimaryActivation();
    }
    else if (ID == m_posiid)
    {
        return GetPositronActivation();
    }
    else if (ID == m_electronid)
    {
        return GetElectronActivation();
    }
    else if (ID == m_gammaid)
    {
        return GetGammaActivation();
    }
    else if (ID == m_muonid)
    {
        return GetMuonActivation();
    }
    else if (ID == m_pionid)
    {
        return GetPionActivation();
    }
    else if (ID == m_primarycountid)
    {
        return GetPrimaryCountActivation();
    }
    else
    {
        G4ExceptionDescription msg;
        msg << "No NTuple found with id = " << ID;
        G4Exception(
            "NTupleManager::SetNtupleDump",
            "GRAPPA::NO_NTUPLE_FOUND",
            G4ExceptionSeverity::JustWarning,
            msg);
        return false;
    }
}
