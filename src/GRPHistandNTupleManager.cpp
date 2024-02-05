// Copyright 2021-2024
//
// Authors:
// Stanimir Kisyov, Davide Terzani
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
    primaryeneid = analysisManager->CreateH1(
        "FinalPrimaryEnergy",
        "Final Energy of primaries",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(primaryeneid, false);
    // HistogramId = 1
    posieneid = analysisManager->CreateH1(
        "FinalPositronEnergy",
        "Final Energy of positrons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(posieneid, false);
    // HistogramId = 2
    electroneneid = analysisManager->CreateH1(
        "FinalElectronEnergy",
        "Final Energy of electrons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(electroneneid, false);
    // HistogramId = 3
    gammaeneid = analysisManager->CreateH1(
        "FinalGammaEnergy",
        "Final Energy of photons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(gammaeneid, false);
    // HistogramId = 4
    pioneneid = analysisManager->CreateH1(
        "FinalPionEnergy",
        "Final Energy of pions",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(pioneneid, false);
    // HistogramId = 5
    muoneneid = analysisManager->CreateH1(
        "FinalMuonEnergy",
        "Final Energy of muons",
        200,
        10 * MeV,
        1 * GeV,
        "GeV",
        "none",
        "log");
    analysisManager->SetH1Activation(muoneneid, false);
    // HistogramId = 6
    initialprimaryeneid = analysisManager->CreateH1(
        "InitialPrimaryEnergy",
        "Initial Energy of primaries",
        50,
        9.9 * GeV,
        10.1 * GeV,
        "GeV");
    analysisManager->SetH1Activation(initialprimaryeneid, false);

    // Final theta distributions
    // HistogramId = 7
    primarytheta = analysisManager->CreateH1(
        "PrimaryTheta",
        "Final #theta of primaries",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(primarytheta, false);
    // HistogramId = 8
    electrontheta = analysisManager->CreateH1(
        "ElectronTheta",
        "Final #theta of electrons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(electrontheta, false);
    // HistogramId = 9
    positrontheta = analysisManager->CreateH1(
        "PositronTheta",
        "Final #theta of positrons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(positrontheta, false);
    // HistogramId = 10
    gammatheta = analysisManager->CreateH1(
        "GammaTheta",
        "Final #theta of photons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(gammatheta, false);
    // HistogramId = 11
    piontheta = analysisManager->CreateH1(
        "PionTheta",
        "Final #theta of pions",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(piontheta, false);
    // HistogramId = 12
    muontheta = analysisManager->CreateH1(
        "MuonTheta",
        "Final #theta of muons",
        200,
        0 * mrad,
        10 * mrad,
        "mrad",
        "none",
        "linear");
    analysisManager->SetH1Activation(muontheta, false);

    // Final phi distributions
    // HistogramId = 13
    primaryphi = analysisManager->CreateH1(
        "PrimaryPhi",
        "Final #phi of primaries",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(primaryphi, false);
    // HistogramId = 14
    electronphi = analysisManager->CreateH1(
        "ElectronPhi",
        "Final #phi of electrons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(electronphi, false);
    // HistogramId = 15
    positronphi = analysisManager->CreateH1(
        "PositronPhi",
        "Final #phi of positrons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(positronphi, false);
    // HistogramId = 16
    gammaphi = analysisManager->CreateH1(
        "GammaPhi",
        "Final #phi of photons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(gammaphi, false);
    // HistogramId = 17
    pionphi = analysisManager->CreateH1(
        "PionPhi",
        "Final #phi of pions",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(pionphi, false);
    // HistogramId = 18
    muonphi = analysisManager->CreateH1(
        "MuonPhi",
        "Final #phi of muons",
        30,
        -CLHEP::pi * rad,
        CLHEP::pi * rad,
        "rad",
        "none",
        "linear");
    analysisManager->SetH1Activation(muonphi, false);

    // 2D
    // HistogramId = 0
    // Initial primary transverse distribution
    initialprimaryxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(initialprimaryxyid, false);
    // HistogramId = 1
    // Initial primary longitudinal distribution
    initialprimaryzxid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(initialprimaryzxid, false);
    // Initial primary angular distribution
    // HistogramId = 2
    initialprimarytxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(initialprimarytxtyid, false);
    // Initial primary trasverse x-phasespace
    // HistogramId = 3
    initialprimaryxpxid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(initialprimaryxpxid, false);
    // Initial primary trasverse y-phasespace
    // HistogramId = 4
    initialprimaryypyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(initialprimaryypyid, false);

    // Final primary transverse distribution
    // HistogramId = 5
    finalprimaryxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalprimaryxyid, false);
    // Final electron transverse distribution
    // HistogramId = 6
    finalelectronxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalelectronxyid, false);
    // Final positron transverse distribution
    // HistogramId = 7
    finalpositronxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalpositronxyid, false);
    // Final gamma transverse distribution
    // HistogramId = 8
    finalgammaxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalgammaxyid, false);
    // Final pion transverse distribution
    // HistogramId = 9
    finalpionxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalpionxyid, false);
    // Final muon transverse distribution
    // HistogramId = 10
    finalmuonxyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalmuonxyid, false);

    // Final primary angle distribution
    // HistogramId = 11
    finalprimarytxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalprimarytxtyid, false);
    // Final electron angle distribution
    // HistogramId = 12
    finalelectrontxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalelectrontxtyid, false);
    // Final positron angle distribution
    // HistogramId = 13
    finalpositrontxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalpositrontxtyid, false);
    // Final gamma angle distribution
    // HistogramId = 14
    finalgammatxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalgammatxtyid, false);
    // Final gamma angle distribution
    // HistogramId = 15
    finalpiontxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalpiontxtyid, false);
    // Final gamma angle distribution
    // HistogramId = 16
    finalmuontxtyid = analysisManager->CreateH2(
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
    analysisManager->SetH2Activation(finalmuontxtyid, false);
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
    primaryinitialid = analysisManager->CreateNtuple(
        "InitialPrimary", "Initial primary particle phase space");
    analysisManager->CreateNtupleFColumn(primaryinitialid, "x");
    analysisManager->CreateNtupleFColumn(primaryinitialid, "y");
    analysisManager->CreateNtupleFColumn(primaryinitialid, "z");
    analysisManager->CreateNtupleFColumn(primaryinitialid, "px");
    analysisManager->CreateNtupleFColumn(primaryinitialid, "py");
    analysisManager->CreateNtupleFColumn(primaryinitialid, "pz");
    analysisManager->FinishNtuple(primaryinitialid);
    analysisManager->SetNtupleActivation(
        primaryinitialid, m_primaryinitialidactive);

    // NTuple 1: Phase space of final primaries
    primaryid = analysisManager->CreateNtuple(
        "FinalPrimary", "Final primary particles phase space");
    analysisManager->CreateNtupleFColumn(primaryid, "x");
    analysisManager->CreateNtupleFColumn(primaryid, "y");
    analysisManager->CreateNtupleFColumn(primaryid, "z");
    analysisManager->CreateNtupleFColumn(primaryid, "px");
    analysisManager->CreateNtupleFColumn(primaryid, "py");
    analysisManager->CreateNtupleFColumn(primaryid, "pz");
    analysisManager->CreateNtupleFColumn(primaryid, "t");
    analysisManager->FinishNtuple(primaryid);
    analysisManager->SetNtupleActivation(primaryid, m_primaryidactive);

    // NTuple 2: Phase space of final positrons
    posiid = analysisManager->CreateNtuple(
        "FinalPositron", "Final positron phase space");
    analysisManager->CreateNtupleFColumn(posiid, "x");
    analysisManager->CreateNtupleFColumn(posiid, "y");
    analysisManager->CreateNtupleFColumn(posiid, "z");
    analysisManager->CreateNtupleFColumn(posiid, "px");
    analysisManager->CreateNtupleFColumn(posiid, "py");
    analysisManager->CreateNtupleFColumn(posiid, "pz");
    analysisManager->CreateNtupleFColumn(posiid, "t");
    analysisManager->CreateNtupleSColumn(posiid, "process");
    analysisManager->FinishNtuple(posiid);
    analysisManager->SetNtupleActivation(posiid, m_posiidactive);

    // NTuple 3: Phase space of final electrons
    electronid = analysisManager->CreateNtuple(
        "FinalElectron", "Final electron phase space");
    analysisManager->CreateNtupleFColumn(electronid, "x");
    analysisManager->CreateNtupleFColumn(electronid, "y");
    analysisManager->CreateNtupleFColumn(electronid, "z");
    analysisManager->CreateNtupleFColumn(electronid, "px");
    analysisManager->CreateNtupleFColumn(electronid, "py");
    analysisManager->CreateNtupleFColumn(electronid, "pz");
    analysisManager->CreateNtupleFColumn(electronid, "t");
    analysisManager->CreateNtupleSColumn(electronid, "process");
    analysisManager->FinishNtuple(electronid);
    analysisManager->SetNtupleActivation(electronid, m_electronidactive);

    // NTuple 4: Phase space of final photons
    gammaid = analysisManager->CreateNtuple(
        "FinalPhoton", "Final photon phase space");
    analysisManager->CreateNtupleFColumn(gammaid, "x");
    analysisManager->CreateNtupleFColumn(gammaid, "y");
    analysisManager->CreateNtupleFColumn(gammaid, "z");
    analysisManager->CreateNtupleFColumn(gammaid, "px");
    analysisManager->CreateNtupleFColumn(gammaid, "py");
    analysisManager->CreateNtupleFColumn(gammaid, "pz");
    analysisManager->CreateNtupleFColumn(gammaid, "t");
    analysisManager->FinishNtuple(gammaid);
    analysisManager->SetNtupleActivation(gammaid, m_gammaidactive);

    // NTuple 5: Phase space of final pions
    pionid =
        analysisManager->CreateNtuple("FinalPions", "Final pions phase space");
    analysisManager->CreateNtupleFColumn(pionid, "x");
    analysisManager->CreateNtupleFColumn(pionid, "y");
    analysisManager->CreateNtupleFColumn(pionid, "z");
    analysisManager->CreateNtupleFColumn(pionid, "px");
    analysisManager->CreateNtupleFColumn(pionid, "py");
    analysisManager->CreateNtupleFColumn(pionid, "pz");
    analysisManager->CreateNtupleFColumn(pionid, "t");
    analysisManager->CreateNtupleFColumn(pionid, "q");
    analysisManager->CreateNtupleSColumn(pionid, "process");
    analysisManager->FinishNtuple(pionid);
    analysisManager->SetNtupleActivation(pionid, m_pionidactive);

    // NTuple 6: Phase space of final muons
    muonid =
        analysisManager->CreateNtuple("FinalMuon", "Final muons phase space");
    analysisManager->CreateNtupleFColumn(muonid, "x");
    analysisManager->CreateNtupleFColumn(muonid, "y");
    analysisManager->CreateNtupleFColumn(muonid, "z");
    analysisManager->CreateNtupleFColumn(muonid, "px");
    analysisManager->CreateNtupleFColumn(muonid, "py");
    analysisManager->CreateNtupleFColumn(muonid, "pz");
    analysisManager->CreateNtupleFColumn(muonid, "t");
    analysisManager->CreateNtupleFColumn(muonid, "q");
    analysisManager->CreateNtupleSColumn(muonid, "process");
    analysisManager->FinishNtuple(muonid);
    analysisManager->SetNtupleActivation(muonid, m_muonidactive);
}

void NTupleManager::ListNtuples()
{
    G4cout << "List of all the available NTuples" << G4endl;
    G4cout << " ID   Name   ActivationStatus" << G4endl;
    G4cout << " " << primaryinitialid << "  "
           << "InitialPrimary"
           << "  " << m_primaryinitialidactive << G4endl;
    G4cout << " " << primaryid << "  "
           << "FinalPrimary"
           << "  " << m_primaryidactive << G4endl;
    G4cout << " " << posiid << "  "
           << "FinalPositron"
           << "  " << m_posiidactive << G4endl;
    G4cout << " " << electronid << "  "
           << "FinalElectron"
           << "  " << m_electronidactive << G4endl;
    G4cout << " " << gammaid << "  "
           << "FinalPhoton"
           << "  " << m_gammaidactive << G4endl;
    G4cout << " " << pionid << "  "
           << "FinalPions"
           << "  " << m_pionidactive << G4endl;
    G4cout << " " << muonid << "  "
           << "FinalMuon"
           << "  " << m_muonidactive << G4endl;
}

void NTupleManager::SetNtupleDump(G4int ID, G4bool ifdump)
{
    if (ID == primaryinitialid)
    {
        m_primaryinitialidactive = ifdump;
    }
    else if (ID == primaryid)
    {
        m_primaryidactive = ifdump;
    }
    else if (ID == posiid)
    {
        m_posiidactive = ifdump;
    }
    else if (ID == electronid)
    {
        m_electronidactive = ifdump;
    }
    else if (ID == gammaid)
    {
        m_gammaidactive = ifdump;
    }
    else if (ID == muonid)
    {
        m_muonidactive = ifdump;
    }
    else if (ID == pionid)
    {
        m_pionidactive = ifdump;
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
    if (ID == primaryinitialid)
    {
        return GetPrimaryInitialIdActivation();
    }
    else if (ID == primaryid)
    {
        return GetPrimaryIdActivation();
    }
    else if (ID == posiid)
    {
        return GetPositronIdActivation();
    }
    else if (ID == electronid)
    {
        return GetElectronIdActivation();
    }
    else if (ID == gammaid)
    {
        return GetGammaIdActivation();
    }
    else if (ID == muonid)
    {
        return GetMuonIdActivation();
    }
    else if (ID == pionid)
    {
        return GetPionIdActivation();
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
