/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyHistandNTupleManager.hpp>

HistandNTupleManager::HistandNTupleManager() : fFileName("Analysis")
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
#if defined(GEANT4_HAS_HDF5)
    analysisManager->SetDefaultFileType("hdf5");
#else
    analysisManager->SetDefaultFileType("root");
    // Note: merging ntuples is available only with Root output
    // analysisManager->SetNtupleMerging(true);
#endif
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
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetHistoDirectoryName("histo");

    // Define histograms of interest
    // Once a particle source is defined (e.g. GPS), histogram limits can be changed based on that

    // 1D
    // HistogramId = 0
    primaryeneid = analysisManager->CreateH1("FinalPrimaryEnergy", "Final Energy of primaries", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(primaryeneid, false);
    // HistogramId = 1
    posieneid = analysisManager->CreateH1("FinalPositronEnergy", "Final Energy of positrons", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(posieneid, false);
    // HistogramId = 2
    electroneneid = analysisManager->CreateH1("FinalElectronEnergy", "Final Energy of electrons", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(electroneneid, false);
    // HistogramId = 3
    gammaeneid = analysisManager->CreateH1("FinalGammaEnergy", "Final Energy of photons", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(gammaeneid, false);
    // HistogramId = 4
    initialprimaryeneid = analysisManager->CreateH1("InitialPrimaryEnergy", "Initial Energy of primaries", 50, 9.9 * GeV, 10.1 * GeV, "GeV");
    analysisManager->SetH1Activation(initialprimaryeneid, false);

    // 2D
    // HistogramId = 0
    // Initial primary transverse distribution
    initialprimaryxyid = analysisManager->CreateH2("InitialPrimaryXY", "Initial primary transverse distribution", 15, -100 * um, 100 * um,
                                                   15, -100 * um, 100 * um, "um", "um");
    analysisManager->SetH2Activation(initialprimaryxyid, false);
    // Initial primary angular distribution
    // HistogramId = 1
    initialprimarytxtyid = analysisManager->CreateH2("InitialPrimaryangles", "Initial primary angle distribution", 10, -20 * mrad, 20 * mrad,
                                                     10, -20 * mrad, 20 * mrad, "mrad", "mrad");
    analysisManager->SetH2Activation(initialprimarytxtyid, false);
    // Initial primary trasverse x-phasespace
    // HistogramId = 2
    initialprimaryxpxid = analysisManager->CreateH2("InitialPrimaryXPhaseSpace", "Initial X phase space of primaries", 20, -20 * um, 20 * um,
                                                    20, -20 * MeV, 20 * MeV, "um", "MeV");
    analysisManager->SetH2Activation(initialprimaryxpxid, false);
    // Initial primary trasverse y-phasespace
    // HistogramId = 3
    initialprimaryypyid = analysisManager->CreateH2("InitialPrimaryYPhaseSpace", "Initial Y phase space of primaries", 20, -20 * um, 20 * um,
                                                    20, -20 * MeV, 20 * MeV, "um", "MeV");
    analysisManager->SetH2Activation(initialprimaryypyid, false);

    // Final primary transverse distribution
    // HistogramId = 4
    finalprimaryxyid = analysisManager->CreateH2("FinalPrimaryXY", "Final primary transverse distribution", 40, -1.5 * mm, 1.5 * mm,
                                                 40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalprimaryxyid, false);
    // Final electron transverse distribution
    // HistogramId = 5
    finalelectronxyid = analysisManager->CreateH2("FinalElectronXY", "Final electron transverse distribution", 40, -1.5 * mm, 1.5 * mm,
                                                  40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalelectronxyid, false);
    // Final positron transverse distribution
    // HistogramId = 6
    finalpositronxyid = analysisManager->CreateH2("FinalPositronXY", "Final positron transverse distribution", 40, -1.5 * mm, 1.5 * mm,
                                                  40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalpositronxyid, false);
    // Final gamma transverse distribution
    // HistogramId = 7
    finalgammaxyid = analysisManager->CreateH2("FinalGammaXY", "Final photon transverse distribution", 40, -1.5 * mm, 1.5 * mm,
                                               40, -1.5 * mm, 1.5 * mm, "mm", "mm");
    analysisManager->SetH2Activation(finalgammaxyid, false);

    // Final primary angle distribution
    // HistogramId = 8
    finalprimarytxtyid = analysisManager->CreateH2("FinalPrimaryAngles", "Final primary angle distribution", 40, -1.5 * rad, 1.5 * rad,
                                                   40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalprimarytxtyid, false);
    // Final electron angle distribution
    // HistogramId = 9
    finalelectrontxtyid = analysisManager->CreateH2("FinalElectronAngles", "Final electron angle distribution", 40, -1.5 * rad, 1.5 * rad,
                                                    40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalelectrontxtyid, false);
    // Final positron angle distribution
    // HistogramId = 10
    finalpositrontxtyid = analysisManager->CreateH2("FinalPositronAngles", "Final positron angle distribution", 40, -1.5 * rad, 1.5 * rad,
                                                    40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalpositrontxtyid, false);
    // Final gamma angle distribution
    // HistogramId = 11
    finalgammatxtyid = analysisManager->CreateH2("FinalGammaAngles", "Final photon angle distribution", 40, -1.5 * rad, 1.5 * rad,
                                                 40, -1.5 * rad, 1.5 * rad, "rad", "rad");
    analysisManager->SetH2Activation(finalgammatxtyid, false);

    analysisManager->SetH1Plotting(primaryeneid, false);
    analysisManager->SetH1Plotting(posieneid, false);
    analysisManager->SetH1Plotting(electroneneid, false);
    analysisManager->SetH1Plotting(gammaeneid, false);
    analysisManager->SetH1Plotting(initialprimaryeneid, false);
    analysisManager->SetH2Plotting(initialprimaryxyid, false);
    analysisManager->SetH2Plotting(initialprimarytxtyid, false);
    analysisManager->SetH2Plotting(initialprimaryxpxid, false);
    analysisManager->SetH2Plotting(initialprimaryypyid, false);
    analysisManager->SetH2Plotting(finalprimaryxyid, false);
    analysisManager->SetH2Plotting(finalelectronxyid, false);
    analysisManager->SetH2Plotting(finalpositronxyid, false);
    analysisManager->SetH2Plotting(finalgammaxyid, false);
    analysisManager->SetH2Plotting(finalprimarytxtyid, false);
    analysisManager->SetH2Plotting(finalelectrontxtyid, false);
    analysisManager->SetH2Plotting(finalpositrontxtyid, false);
    analysisManager->SetH2Plotting(finalgammatxtyid, false);
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
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetNtupleDirectoryName("ntuple");
    // Creation on ntuples

    // NTuple 0: Phase space of particles generated at source
    primaryinitialid = analysisManager->CreateNtuple("InitialPrimary", "Initial primary particle phase space");
    analysisManager->SetNtupleActivation(primaryinitialid, false);
    analysisManager->CreateNtupleDColumn(primaryinitialid, "X");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Y");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Z");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Px");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Py");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "Pz");
    analysisManager->FinishNtuple(primaryinitialid);

    // NTuple 1: Phase space of final primaries
    primaryid = analysisManager->CreateNtuple("FinalPrimary", "Final primary particles phase space");
    analysisManager->SetNtupleActivation(primaryid, false);
    analysisManager->CreateNtupleDColumn(primaryid, "X");
    analysisManager->CreateNtupleDColumn(primaryid, "Y");
    analysisManager->CreateNtupleDColumn(primaryid, "Z");
    analysisManager->CreateNtupleDColumn(primaryid, "Px");
    analysisManager->CreateNtupleDColumn(primaryid, "Py");
    analysisManager->CreateNtupleDColumn(primaryid, "Pz");
    analysisManager->CreateNtupleDColumn(primaryid, "t");
    analysisManager->FinishNtuple(primaryid);

    // NTuple 2: Phase space of final positrons
    posiid = analysisManager->CreateNtuple("FinalPositron", "Final secondary positron phase space");
    analysisManager->SetNtupleActivation(posiid, false);
    analysisManager->CreateNtupleDColumn(posiid, "X");
    analysisManager->CreateNtupleDColumn(posiid, "Y");
    analysisManager->CreateNtupleDColumn(posiid, "Z");
    analysisManager->CreateNtupleDColumn(posiid, "Px");
    analysisManager->CreateNtupleDColumn(posiid, "Py");
    analysisManager->CreateNtupleDColumn(posiid, "Pz");
    analysisManager->CreateNtupleDColumn(posiid, "t");
    analysisManager->FinishNtuple(posiid);

    // NTuple 3: Phase space of final electrons
    electronid = analysisManager->CreateNtuple("FinalElectron", "Final secondary electron phase space");
    analysisManager->SetNtupleActivation(electronid, false);
    analysisManager->CreateNtupleDColumn(electronid, "X");
    analysisManager->CreateNtupleDColumn(electronid, "Y");
    analysisManager->CreateNtupleDColumn(electronid, "Z");
    analysisManager->CreateNtupleDColumn(electronid, "Px");
    analysisManager->CreateNtupleDColumn(electronid, "Py");
    analysisManager->CreateNtupleDColumn(electronid, "Pz");
    analysisManager->CreateNtupleDColumn(electronid, "t");
    analysisManager->FinishNtuple(electronid);

    // NTuple 4: Phase space of final photons
    gammaid = analysisManager->CreateNtuple("FinalPhoton", "Final secondary photon phase space");
    analysisManager->SetNtupleActivation(gammaid, false);
    analysisManager->CreateNtupleDColumn(gammaid, "X");
    analysisManager->CreateNtupleDColumn(gammaid, "Y");
    analysisManager->CreateNtupleDColumn(gammaid, "Z");
    analysisManager->CreateNtupleDColumn(gammaid, "Px");
    analysisManager->CreateNtupleDColumn(gammaid, "Py");
    analysisManager->CreateNtupleDColumn(gammaid, "Pz");
    analysisManager->CreateNtupleDColumn(gammaid, "t");
    analysisManager->FinishNtuple(gammaid);

}