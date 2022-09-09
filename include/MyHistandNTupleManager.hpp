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
    inline int GetPionEneId() {return pioneneid;}
    inline int GetMuonEneId() {return muoneneid;}
    inline int GetPrimaryxyId() { return finalprimaryxyid; }
    inline int GetPositronxyId() { return finalpositronxyid; }
    inline int GetElectronxyId() { return finalelectronxyid; }
    inline int GetGammaxyId() { return finalgammaxyid; }
    inline int GetPionxyId() { return finalpionxyid; }
    inline int GetMuonxyId() { return finalmuonxyid; }
    inline int GetPrimarytxtyId() { return finalprimarytxtyid; }
    inline int GetPositrontxtyId() { return finalpositrontxtyid; }
    inline int GetElectrontxtyId() { return finalelectrontxtyid; }
    inline int GetGammatxtyId() { return finalgammatxtyid; }
    inline int GetPiontxtyId() { return finalpiontxtyid; }
    inline int GetMuontxtyId() { return finalmuontxtyid; }
    inline int GetPrimaryInitialxyId() { return initialprimaryxyid; }
    inline int GetPrimaryInitialzxId() { return initialprimaryzxid; }
    inline int GetPrimaryInitialEneId() { return initialprimaryeneid; }
    inline int GetPrimaryInitialtxtyId() { return initialprimarytxtyid; }
    inline int GetPrimaryInitialxpxId() { return initialprimaryxpxid; }
    inline int GetPrimaryInitialypyId() { return initialprimaryypyid; }
    inline int GetPrimaryThetaId() { return primarytheta; }
    inline int GetElectronThetaId() { return electrontheta; }
    inline int GetPositronThetaId() { return positrontheta; }
    inline int GetGammaThetaId() { return gammatheta; }
    inline int GetPionThetaId() {return piontheta;}
    inline int GetMuonThetaId() {return muontheta;}
    inline int GetPrimaryPhiId() { return primaryphi; }
    inline int GetElectronPhiId() { return electronphi; }
    inline int GetPositronPhiId() { return positronphi; }
    inline int GetGammaPhiId() { return gammaphi; }
    inline int GetPionPhiId() {return pionphi;}
    inline int GetMuonPhiId() {return muonphi;}
    void Book();

private:
    // Histogram id numbers

    // Initial primary energy
    G4int initialprimaryeneid;
    // Initial primary xy
    G4int initialprimaryxyid;
    // Initial primary zx
    G4int initialprimaryzxid;
    // Initial primary theta_x-theta_y
    G4int initialprimarytxtyid;
    // Initial primary x-p_x
    G4int initialprimaryxpxid;
    // Initial primary y-p_y
    G4int initialprimaryypyid;
    // Final primary theta distribution
    G4int primarytheta;
    // Final electron theta distribution
    G4int electrontheta;
    // Final positron theta distribution
    G4int positrontheta;
    // Final gamma theta distribution
    G4int gammatheta;
    // Final pions theta distribution
    G4int piontheta;
    // Final muons theta distribution
    G4int muontheta;
    // Final primary phi distribution
    G4int primaryphi;
    // Final electron phi distribution
    G4int electronphi;
    // Final positron phi distribution
    G4int positronphi;
    // Final gamma phi distribution
    G4int gammaphi;
    // Final pions phi distribution
    G4int pionphi;
    // Final muon phi distribution
    G4int muonphi;
    // Final primary energy
    G4int primaryeneid;
    // Final positron energy
    G4int posieneid;
    // Final electron energy
    G4int electroneneid;
    // Final gamma energy
    G4int gammaeneid;
    // Final pions energy
    G4int pioneneid;
    // Final muons energy
    G4int muoneneid;
    // Final primary transverse distribution
    G4int finalprimaryxyid;
    // Final electron transverse distribution
    G4int finalelectronxyid;
    // Final positron transverse distribution
    G4int finalpositronxyid;
    // Final gamma transverse distribution
    G4int finalgammaxyid;
    // Final pions transverse distribution
    G4int finalpionxyid;
    // Final muons transverse distribution
    G4int finalmuonxyid;
    // Final primary transverse angle
    G4int finalprimarytxtyid;
    // Final electron transverse angle
    G4int finalelectrontxtyid;
    // Final positron transverse angle
    G4int finalpositrontxtyid;
    // Final gamma transverse angle
    G4int finalgammatxtyid;
    // Final pions transverse angle
    G4int finalpiontxtyid;
    // Final muons transverse angle
    G4int finalmuontxtyid;

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
    inline int GetPionId() { return pionid; }
    inline int GetMuonId() { return muonid; }
    void Book();

private:
    G4int primaryinitialid, primaryid, posiid, electronid,
        gammaid, pionid, muonid;
};

class HistandNTupleManager
{
public:
    HistandNTupleManager();
    ~HistandNTupleManager();

    void FinishAnalysis();
    void OpenFile();
    void Book();

    HistoManager *GetHistoManager()
    {
        return m_HistoManager;
    }

    NTupleManager *GetNTupleManager()
    {
        return m_NtupleManager;
    }

private:
    G4String fFileName;
    HistoManager *m_HistoManager;
    NTupleManager *m_NtupleManager;
};