/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#pragma once

#include <G4AnalysisManager.hh>
#include <G4SystemOfUnits.hh>

class HistoManager
{
public:
    HistoManager();
    ~HistoManager();

    inline int GetPrimaryEneId() { return primaryeneid; }
    inline int GetPositronEneId() { return posieneid; }
    inline int GetElectronEneId() { return electroneneid; }
    inline int GetGammaEneId() { return gammaeneid; }
    inline int GetPrimaryxyId() { return finalprimaryxyid; }
    inline int GetPositronxyId() { return finalpositronxyid; }
    inline int GetElectronxyId() { return finalelectronxyid; }
    inline int GetGammaxyId() { return finalgammatxtyid; }
    inline int GetPrimarytxtyId() { return finalprimarytxtyid; }
    inline int GetPositrontxtyId() { return finalpositrontxtyid; }
    inline int GetElectrontxtyId() { return finalelectrontxtyid; }
    inline int GetGammatxtyId() { return finalgammaxyid; }
    inline int GetPrimaryInitialxyId() { return initialprimaryxyid; }
    inline int GetPrimaryInitialEneId() { return initialprimaryeneid; }
    inline int GetPrimaryInitialtxtyId() { return initialprimarytxtyid; }
    inline int GetPrimaryInitialxpxId() { return initialprimaryxpxid; }
    inline int GetPrimaryInitialypyId() { return initialprimaryypyid; }
    inline int GetPositronEnergyCut1() { return positronenergycut1; }
    inline int GetPositronEnergyCut2() { return positronenergycut2; }
    inline int GetPositronEnergyCut3() { return positronenergycut3; }
    inline int GetPositronEnergyCut4() { return positronenergycut4; }
    inline int GetPositronEnergyCut5() { return positronenergycut5; }
    void Book();

private:
    // Histogram id numbers

    // Initial primary energy
    G4int initialprimaryeneid;
    // Initial primary xy
    G4int initialprimaryxyid;
    // Initial primary theta_x-theta_y
    G4int initialprimarytxtyid;
    // Initial primary x-p_x
    G4int initialprimaryxpxid;
    // Initial primary y-p_y
    G4int initialprimaryypyid;

    // Final primary energy
    G4int primaryeneid;
    // Final positron energy
    G4int posieneid;
    // Final electron energy
    G4int electroneneid;
    // Final gamma energy
    G4int gammaeneid;
    // Final primary transverse distribution
    G4int finalprimaryxyid;
    // Final electron transverse distribution
    G4int finalelectronxyid;
    // Final positron transverse distribution
    G4int finalpositronxyid;
    // Final gamma transverse distribution
    G4int finalgammaxyid;
    // Final primary transverse angle
    G4int finalprimarytxtyid;
    // Final electron transverse angle
    G4int finalelectrontxtyid;
    // Final positron transverse angle
    G4int finalpositrontxtyid;
    // Final gamma transverse angle
    G4int finalgammatxtyid;

    // Some histograms with divergence cuts
    G4int positronenergycut1;
    G4int positronenergycut2;
    G4int positronenergycut3;
    G4int positronenergycut4;
    G4int positronenergycut5;

};


class NTupleManager
{
public:
    NTupleManager();
    ~NTupleManager();

    inline int GetPrimaryInitialId() { return primaryinitialid; }
    inline int GetPrimaryId() { return primaryid; }
    inline int GetPositronId() { return posiid; }
    inline int GetElectronId() { return electronid; }
    inline int GetGammaId() { return gammaid; }
    void Book();

private:
    G4int primaryinitialid, primaryid, posiid, electronid, gammaid;
};

class HistandNTupleManager
{
public:
    HistandNTupleManager();
    ~HistandNTupleManager();

    void FinishAnalysis();
    void OpenFile();
    void Book();

    HistoManager* GetHistoManager() {
        return m_HistoManager;
    }

    NTupleManager* GetNTupleManager() {
        return m_NtupleManager;
    }

private:

    G4String fFileName;
    HistoManager* m_HistoManager;
    NTupleManager* m_NtupleManager;
};