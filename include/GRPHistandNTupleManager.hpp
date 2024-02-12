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

#pragma once

#if defined(GRAPPA_USE_BACKWARDS_ANALYSIS)
#include <G4GenericAnalysisManager.hh>
using G4AnalysisManager = G4GenericAnalysisManager;
#else
#include <G4AnalysisManager.hh>
#endif
#include <G4SystemOfUnits.hh>

class HistoManager
{
public:
    HistoManager();
    ~HistoManager();

    G4int GetPrimaryEneId() const { return primaryeneid; }
    G4int GetPositronEneId() const { return posieneid; }
    G4int GetElectronEneId() const { return electroneneid; }
    G4int GetGammaEneId() const { return gammaeneid; }
    G4int GetPionEneId() const { return pioneneid; }
    G4int GetMuonEneId() const { return muoneneid; }
    G4int GetPrimaryxyId() const { return finalprimaryxyid; }
    G4int GetPositronxyId() const { return finalpositronxyid; }
    G4int GetElectronxyId() const { return finalelectronxyid; }
    G4int GetGammaxyId() const { return finalgammaxyid; }
    G4int GetPionxyId() const { return finalpionxyid; }
    G4int GetMuonxyId() const { return finalmuonxyid; }
    G4int GetPrimarytxtyId() const { return finalprimarytxtyid; }
    G4int GetPositrontxtyId() const { return finalpositrontxtyid; }
    G4int GetElectrontxtyId() const { return finalelectrontxtyid; }
    G4int GetGammatxtyId() const { return finalgammatxtyid; }
    G4int GetPiontxtyId() const { return finalpiontxtyid; }
    G4int GetMuontxtyId() const { return finalmuontxtyid; }
    G4int GetPrimaryInitialxyId() const { return initialprimaryxyid; }
    G4int GetPrimaryInitialzxId() const { return initialprimaryzxid; }
    G4int GetPrimaryInitialEneId() const { return initialprimaryeneid; }
    G4int GetPrimaryInitialtxtyId() const { return initialprimarytxtyid; }
    G4int GetPrimaryInitialxpxId() const { return initialprimaryxpxid; }
    G4int GetPrimaryInitialypyId() const { return initialprimaryypyid; }
    G4int GetPrimaryThetaId() const { return primarytheta; }
    G4int GetElectronThetaId() const { return electrontheta; }
    G4int GetPositronThetaId() const { return positrontheta; }
    G4int GetGammaThetaId() const { return gammatheta; }
    G4int GetPionThetaId() const { return piontheta; }
    G4int GetMuonThetaId() const { return muontheta; }
    G4int GetPrimaryPhiId() const { return primaryphi; }
    G4int GetElectronPhiId() const { return electronphi; }
    G4int GetPositronPhiId() const { return positronphi; }
    G4int GetGammaPhiId() const { return gammaphi; }
    G4int GetPionPhiId() const { return pionphi; }
    G4int GetMuonPhiId() const { return muonphi; }
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

    // Get index for the initial primaries Ntuple
    G4int GetPrimaryInitialId() const { return m_primaryinitialid; }
    // Get index for the final primaries Ntuple
    G4int GetPrimaryId() const { return m_primaryid; }
    // Get index for the positrons Ntuple
    G4int GetPositronId() const { return m_posiid; }
    // Get index for the electrons Ntuple
    G4int GetElectronId() const { return m_electronid; }
    // Get index for the photons Ntuple
    G4int GetGammaId() const { return m_gammaid; }
    // Get index for the pions Ntuple
    G4int GetPionId() const { return m_pionid; }
    // Get index for the muons Ntuple
    G4int GetMuonId() const { return m_muonid; }
    // Get index for the primary count Ntuple
    G4int GetPrimaryCountId() const { return m_primarycountid; }
    // Get if the Ntuple for the initial primaries is active
    inline G4bool GetPrimaryInitialActivation() const
    {
        return m_primaryinitialidactive;
    }
    // Get if the Ntuple for the final primaries is active
    G4bool GetPrimaryActivation() const { return m_primaryactive; }
    // Get if the Ntuple for the positrons is active
    G4bool GetPositronActivation() const { return m_posiactive; }
    // Get if the Ntuple for the electrons is active
    G4bool GetElectronActivation() const { return m_electronactive; }
    // Get if the Ntuple for the gammas is active
    G4bool GetGammaActivation() const { return m_gammaactive; }
    // Get if the Ntuple for the pions is active
    G4bool GetPionActivation() const { return m_pionactive; }
    // Get if the Ntuple for the muons is active
    G4bool GetMuonActivation() const { return m_muonactive; }
    // Get if the Ntuple for the primary count is active
    G4bool GetPrimaryCountActivation() const { return m_primarycountactive; }
    // Get if the Ntuple with ID is active
    G4bool GetIdActivation(const G4int ID) const;
    void Book();
    void ListNtuples();
    void SetNtupleDump(G4int ID, G4bool ifdump);

private:
    // Phase space of initial primaries
    G4int m_primaryinitialid;
    // Phase space of final primaries
    G4int m_primaryid;
    // Phase space of final positrons
    G4int m_posiid;
    // Phase space of final electrons
    G4int m_electronid;
    // Phase space of final photons
    G4int m_gammaid;
    // Phase space of final pions
    G4int m_pionid;
    // Phase space of final muons
    G4int m_muonid;
    // Events executed by each thread
    G4int m_primarycountid;

    G4bool m_primaryinitialidactive = true;
    G4bool m_primaryactive = true;
    G4bool m_posiactive = true;
    G4bool m_electronactive = true;
    G4bool m_gammaactive = true;
    G4bool m_pionactive = true;
    G4bool m_muonactive = true;
    G4bool m_primarycountactive = true;
};

class HistandNTupleManager
{
public:
    HistandNTupleManager();
    ~HistandNTupleManager();

    void FinishAnalysis();
    void OpenFile();
    void Book();

    HistoManager *GetHistoManager() { return m_HistoManager; }

    NTupleManager *GetNTupleManager() { return m_NtupleManager; }

private:
    G4String fFileName;
    HistoManager *m_HistoManager;
    NTupleManager *m_NtupleManager;
};
