/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyHistandNTupleManager.hpp>


HistandNTupleManager::HistandNTupleManager() :fFileName("Analysis")
{
    m_HistoManager = new HistoManager();
    m_NtupleManager = new NTupleManager();
}

HistandNTupleManager::~HistandNTupleManager()
{

}

void HistandNTupleManager::FinishAnalysis()
{
    // Close Analysis and write files
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive()) {
        analysisManager->Write();
        analysisManager->CloseFile();
    }

}

void HistandNTupleManager::OpenFile()
{
    // Open the analysis 
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive()) {
        analysisManager->OpenFile();
    }
}

void HistandNTupleManager::Book()
{
    // Introduce an analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(0);
    analysisManager->SetDefaultFileType("hdf5");
    analysisManager->SetFileName(fFileName);

    // This option enables the manual activation and deactivation of
    // single histograms and ntuple. A little more verbose in the code, but could potentially
    // allow finer control in macro files
    analysisManager->SetActivation(true);

    m_HistoManager->Book();
    m_NtupleManager->Book();
}

HistoManager::HistoManager()
{

}

HistoManager::~HistoManager()
{
}

void HistoManager::Book()
{
    // Introduce an analysis manager for histogram generation
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetHistoDirectoryName("histo");

    // Define histograms of interest
    // Once a particle source is defined (e.g. GPS), histogram limits can be changed based on that

    // 1D
    // HistogramId = 0
    primaryeneid = analysisManager->CreateH1("FinalPrimaryEnergy", "Final Energy of primaries", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(primaryeneid, true);
    // HistogramId = 1
    posieneid = analysisManager->CreateH1("FinalPositronEnergy", "Final Energy of positrons", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(posieneid, true);
    // HistogramId = 2
    electroneneid = analysisManager->CreateH1("FinalElectronEnergy", "Final Energy of electrons", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(electroneneid, true);
    // HistogramId = 3
    gammaeneid = analysisManager->CreateH1("FinalGammaEnergy", "Final Energy of photons", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(gammaeneid, true);
    // HistogramId = 4
    initialprimaryeneid = analysisManager->CreateH1("InitialPrimaryEnergy", "Initial Energy of primaries", 50, 9.9 * GeV, 10.1 * GeV, "GeV");
    analysisManager->SetH1Activation(initialprimaryeneid, true);

    // 2D
    // HistogramId = 0
    // Initial primary transverse distribution
    initialprimaryxyid = analysisManager->CreateH2("InitialPrimaryXY", "Initial primary transverse distribution", 15, -100 * um, 100 * um,
        15, -100 * um, 100 * um, "um", "um");
    analysisManager->SetH2Activation(initialprimaryxyid, true);
    // Initial primary angular distribution
    // HistogramId = 1
    initialprimarytxtyid = analysisManager->CreateH2("InitialPrimaryangles", "Initial primary angle distribution", 10, -20 * mrad, 20 * mrad,
        10, -20 * mrad, 20 * mrad, "mrad", "mrad");
    analysisManager->SetH2Activation(initialprimarytxtyid, true);
    // Initial primary trasverse x-phasespace
    // HistogramId = 2
    initialprimaryxpxid = analysisManager->CreateH2("InitialPrimaryXPhaseSpace", "Initial X phase space of primaries", 20, -20 * um, 20 * um,
        20, -20 * MeV, 20 * MeV, "um", "MeV");
    analysisManager->SetH2Activation(initialprimaryxpxid, true);
    // Initial primary trasverse y-phasespace
    // HistogramId = 3
    initialprimaryypyid = analysisManager->CreateH2("InitialPrimaryYPhaseSpace", "Initial Y phase space of primaries", 20, -20 * um, 20 * um,
        20, -20 * MeV, 20 * MeV, "um", "MeV");
    analysisManager->SetH2Activation(initialprimaryypyid, true);

    // Final primary transverse distribution
    // HistogramId = 4
    finalprimaryxyid = analysisManager->CreateH2("FinalPrimaryXY", "Final primary transverse distribution", 40, -1.5 * mm, 1.5 * mm,
        40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalprimaryxyid, true);
    // Final electron transverse distribution
    // HistogramId = 5
    finalelectronxyid = analysisManager->CreateH2("FinalElectronXY", "Final electron transverse distribution", 40, -1.5 * mm, 1.5 * mm,
        40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalelectronxyid, true);
    // Final positron transverse distribution
    // HistogramId = 6
    finalpositronxyid = analysisManager->CreateH2("FinalPositronXY", "Final positron transverse distribution", 40, -1.5 * mm, 1.5 * mm,
        40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalpositronxyid, true);
    // Final gamma transverse distribution
    // HistogramId = 7
    finalgammaxyid = analysisManager->CreateH2("FinalGammaXY", "Final photon transverse distribution", 40, -1.5 * mm, 1.5 * mm,
        40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalgammaxyid, true);

    // Final primary angle distribution
    // HistogramId = 8
    finalprimarytxtyid = analysisManager->CreateH2("FinalPrimaryAngles", "Final primary angle distribution", 40, -1.5 * rad, 1.5 * rad,
        40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalprimarytxtyid, true);
    // Final electron angle distribution
    // HistogramId = 9
    finalelectrontxtyid = analysisManager->CreateH2("FinalElectronAngles", "Final electron angle distribution", 40, -1.5 * rad, 1.5 * rad,
        40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalelectrontxtyid, true);
    // Final positron angle distribution
    // HistogramId = 10
    finalpositrontxtyid = analysisManager->CreateH2("FinalPositronAngles", "Final positron angle distribution", 40, -1.5 * rad, 1.5 * rad,
        40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalpositrontxtyid, true);
    // Final gamma angle distribution
    // HistogramId = 11
    finalgammatxtyid = analysisManager->CreateH2("FinalGammaAngles", "Final photon angle distribution", 40, -1.5 * rad, 1.5 * rad,
        40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalgammatxtyid, true);

    analysisManager->SetH1Plotting(primaryeneid, true);
    analysisManager->SetH1Plotting(posieneid, true);
    analysisManager->SetH1Plotting(electroneneid, true);
    analysisManager->SetH1Plotting(gammaeneid, true);
    analysisManager->SetH1Plotting(initialprimaryeneid, true);
    analysisManager->SetH2Plotting(initialprimaryxyid, true);
    analysisManager->SetH2Plotting(initialprimarytxtyid, true);
    analysisManager->SetH2Plotting(initialprimaryxpxid, true);
    analysisManager->SetH2Plotting(initialprimaryypyid, true);
    analysisManager->SetH2Plotting(finalprimaryxyid, true);
    analysisManager->SetH2Plotting(finalelectronxyid, true);
    analysisManager->SetH2Plotting(finalpositronxyid, true);
    analysisManager->SetH2Plotting(finalgammaxyid, true);
    analysisManager->SetH2Plotting(finalprimarytxtyid, true);
    analysisManager->SetH2Plotting(finalelectrontxtyid, true);
    analysisManager->SetH2Plotting(finalpositrontxtyid, true);
    analysisManager->SetH2Plotting(finalgammatxtyid, true);
}

NTupleManager::NTupleManager()
{

}

NTupleManager::~NTupleManager()
{
}

void NTupleManager::Book()
{
    // Introduce an analysis manager for Ntuple generation
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetNtupleDirectoryName("ntuple");

    // Creation on ntuples

    // NTuple 0: Phase space of particles generated at source
    primaryinitialid = analysisManager->CreateNtuple("InitialParticles", "Initial primary particle phase space");
    analysisManager->SetNtupleActivation(primaryinitialid, true);
    analysisManager->CreateNtupleDColumn(primaryinitialid, "X [mm]");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Y [mm]");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Z [mm]");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Px [MeV]");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Py [MeV]");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Pz [MeV]");
    analysisManager->FinishNtuple(primaryinitialid);

    // NTuple 1: Phase space of final primaries
    primaryid = analysisManager->CreateNtuple("FinalPrimary", "Final primary particles phase space");
    analysisManager->SetNtupleActivation(primaryid, true);
    analysisManager->CreateNtupleDColumn(primaryid, "X [mm]");
    analysisManager->CreateNtupleDColumn(primaryid, "Y [mm]");
    analysisManager->CreateNtupleDColumn(primaryid, "Z [mm]");
    analysisManager->CreateNtupleDColumn(primaryid, "Px [MeV]");
    analysisManager->CreateNtupleDColumn(primaryid, "Py [MeV]");
    analysisManager->CreateNtupleDColumn(primaryid, "Pz [MeV]");
    analysisManager->CreateNtupleDColumn(primaryid, "t [ns]");
    analysisManager->FinishNtuple(primaryid);

    // NTuple 2: Phase space of final positrons
    posiid = analysisManager->CreateNtuple("FinalPositron", "Final secondary positron phase space");
    analysisManager->SetNtupleActivation(posiid, true);
    analysisManager->CreateNtupleDColumn(posiid, "X [mm]");
    analysisManager->CreateNtupleDColumn(posiid, "Y [mm]");
    analysisManager->CreateNtupleDColumn(posiid, "Z [mm]");
    analysisManager->CreateNtupleDColumn(posiid, "Px [MeV]");
    analysisManager->CreateNtupleDColumn(posiid, "Py [MeV]");
    analysisManager->CreateNtupleDColumn(posiid, "Pz [MeV]");
    analysisManager->CreateNtupleDColumn(posiid, "t [ns]");
    analysisManager->FinishNtuple(posiid);

    // NTuple 3: Phase space of final electrons
    electronid = analysisManager->CreateNtuple("FinalElectron", "Final secondary electron phase space");
    analysisManager->SetNtupleActivation(electronid, true);
    analysisManager->CreateNtupleDColumn(electronid, "X [mm]");
    analysisManager->CreateNtupleDColumn(electronid, "Y [mm]");
    analysisManager->CreateNtupleDColumn(electronid, "Z [mm]");
    analysisManager->CreateNtupleDColumn(electronid, "Px [MeV]");
    analysisManager->CreateNtupleDColumn(electronid, "Py [MeV]");
    analysisManager->CreateNtupleDColumn(electronid, "Pz [MeV]");
    analysisManager->CreateNtupleDColumn(electronid, "t [ns]");
    analysisManager->FinishNtuple(electronid);

    // NTuple 4: Phase space of final photons
    gammaid = analysisManager->CreateNtuple("FinalPhoton", "Final secondary photon phase space");
    analysisManager->SetNtupleActivation(gammaid, true);
    analysisManager->CreateNtupleDColumn(gammaid, "X [mm]");
    analysisManager->CreateNtupleDColumn(gammaid, "Y [mm]");
    analysisManager->CreateNtupleDColumn(gammaid, "Z [mm]");
    analysisManager->CreateNtupleDColumn(gammaid, "Px [MeV]");
    analysisManager->CreateNtupleDColumn(gammaid, "Py [MeV]");
    analysisManager->CreateNtupleDColumn(gammaid, "Pz [MeV]");
    analysisManager->CreateNtupleDColumn(gammaid, "t [ns]");
    analysisManager->FinishNtuple(gammaid);

}