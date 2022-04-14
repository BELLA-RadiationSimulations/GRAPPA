/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyHistandNTupleManager.hpp>

HistandNTupleManager::HistandNTupleManager() : fFileName("Data_output")
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

#if GRAPPA_USE_HDF5
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

    // Final theta distributions
    // HistogramId = 5
    primarytheta =
        analysisManager->CreateH1("PrimaryTheta", "Final #theta of primaries", 200, 0 * mrad, 10 * mrad, "mrad", "none", "linear");
    analysisManager->SetH1Activation(primarytheta, false);
    // HistogramId = 6
    electrontheta =
        analysisManager->CreateH1("ElectronTheta", "Final #theta of electrons", 200, 0 * mrad, 10 * mrad, "mrad", "none", "linear");
    analysisManager->SetH1Activation(electrontheta, false);
    // HistogramId = 7
    positrontheta =
        analysisManager->CreateH1("PositronTheta", "Final #theta of positrons", 200, 0 * mrad, 10 * mrad, "mrad", "none", "linear");
    analysisManager->SetH1Activation(positrontheta, false);
    // HistogramId = 8
    gammatheta =
        analysisManager->CreateH1("GammaTheta", "Final #theta of photons", 200, 0 * mrad, 10 * mrad, "mrad", "none", "linear");
    analysisManager->SetH1Activation(gammatheta, false);

    // Some 1D histograms to analyze particles after a cut in divergence (i.e. collimator)

    // HistogramId = 9
    positronenergycut1 =
        analysisManager->CreateH1("PositronEnergyCut1", "Positron energy (#theta < 1 mrad)", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(positronenergycut1, false);
    // HistogramId = 10
    positronenergycut2 =
        analysisManager->CreateH1("PositronEnergyCut2", "Positron energy (#theta < 5 mrad)", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(positronenergycut2, false);
    // HistogramId = 11
    positronenergycut3 =
        analysisManager->CreateH1("PositronEnergyCut3", "Positron energy (#theta < 10 mrad)", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(positronenergycut3, false);
    // HistogramId = 12
    positronenergycut4 =
        analysisManager->CreateH1("PositronEnergyCut4", "Positron energy (#theta < 20 mrad)", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(positronenergycut4, false);
    // HistogramId = 13
    positronenergycut5 =
        analysisManager->CreateH1("PositronEnergyCut5", "Positron energy (#theta < 50 mrad)", 200, 10 * MeV, 1 * GeV, "GeV", "none", "log");
    analysisManager->SetH1Activation(positronenergycut5, false);

    // Final phi distributions
    // HistogramId = 14
    primaryphi =
        analysisManager->CreateH1("PrimaryPhi", "Final #phi of primaries", 30, -CLHEP::pi * rad, CLHEP::pi * rad, "rad", "none", "linear");
    analysisManager->SetH1Activation(primaryphi, false);
    // HistogramId = 15
    electronphi =
        analysisManager->CreateH1("ElectronPhi", "Final #phi of electrons", 30, -CLHEP::pi * rad, CLHEP::pi * rad, "rad", "none", "linear");
    analysisManager->SetH1Activation(electronphi, false);
    // HistogramId = 16
    positronphi =
        analysisManager->CreateH1("PositronPhi", "Final #phi of positrons", 30, -CLHEP::pi * rad, CLHEP::pi * rad, "rad", "none", "linear");
    analysisManager->SetH1Activation(positronphi, false);
    // HistogramId = 17
    gammaphi =
        analysisManager->CreateH1("GammaPhi", "Final #phi of photons", 30, -CLHEP::pi * rad, CLHEP::pi * rad, "rad", "none", "linear");
    analysisManager->SetH1Activation(gammaphi, false);

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
    analysisManager->CreateNtupleDColumn(primaryinitialid, "x");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "y");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "z");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "px");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "py");
    analysisManager->CreateNtupleDColumn(primaryinitialid, "pz");
    analysisManager->FinishNtuple(primaryinitialid);

    // NTuple 1: Phase space of final primaries
    primaryid = analysisManager->CreateNtuple("FinalPrimary", "Final primary particles phase space");
    analysisManager->SetNtupleActivation(primaryid, false);
    analysisManager->CreateNtupleDColumn(primaryid, "x");
    analysisManager->CreateNtupleDColumn(primaryid, "y");
    analysisManager->CreateNtupleDColumn(primaryid, "z");
    analysisManager->CreateNtupleDColumn(primaryid, "px");
    analysisManager->CreateNtupleDColumn(primaryid, "py");
    analysisManager->CreateNtupleDColumn(primaryid, "pz");
    analysisManager->CreateNtupleDColumn(primaryid, "t");
    analysisManager->FinishNtuple(primaryid);

    // NTuple 2: Phase space of final positrons
    posiid = analysisManager->CreateNtuple("FinalPositron", "Final secondary positron phase space");
    analysisManager->SetNtupleActivation(posiid, false);
    analysisManager->CreateNtupleDColumn(posiid, "x");
    analysisManager->CreateNtupleDColumn(posiid, "y");
    analysisManager->CreateNtupleDColumn(posiid, "z");
    analysisManager->CreateNtupleDColumn(posiid, "px");
    analysisManager->CreateNtupleDColumn(posiid, "py");
    analysisManager->CreateNtupleDColumn(posiid, "pz");
    analysisManager->CreateNtupleDColumn(posiid, "t");
    analysisManager->FinishNtuple(posiid);

    // NTuple 3: Phase space of final electrons
    electronid = analysisManager->CreateNtuple("FinalElectron", "Final secondary electron phase space");
    analysisManager->SetNtupleActivation(electronid, false);
    analysisManager->CreateNtupleDColumn(electronid, "x");
    analysisManager->CreateNtupleDColumn(electronid, "y");
    analysisManager->CreateNtupleDColumn(electronid, "z");
    analysisManager->CreateNtupleDColumn(electronid, "px");
    analysisManager->CreateNtupleDColumn(electronid, "py");
    analysisManager->CreateNtupleDColumn(electronid, "pz");
    analysisManager->CreateNtupleDColumn(electronid, "t");
    analysisManager->FinishNtuple(electronid);

    // NTuple 4: Phase space of final photons
    gammaid = analysisManager->CreateNtuple("FinalPhoton", "Final secondary photon phase space");
    analysisManager->SetNtupleActivation(gammaid, false);
    analysisManager->CreateNtupleDColumn(gammaid, "x");
    analysisManager->CreateNtupleDColumn(gammaid, "y");
    analysisManager->CreateNtupleDColumn(gammaid, "z");
    analysisManager->CreateNtupleDColumn(gammaid, "px");
    analysisManager->CreateNtupleDColumn(gammaid, "py");
    analysisManager->CreateNtupleDColumn(gammaid, "pz");
    analysisManager->CreateNtupleDColumn(gammaid, "t");
    analysisManager->FinishNtuple(gammaid);
}