// Copyright 2021-2025
//
// Authors:
// Stanimir Kisyov, Sarah Schröder, Davide Terzani
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

#include <GRPParticlePDGID.hpp>

class HistoManager
{
public:
    HistoManager();
    ~HistoManager();

    G4int GetPrimaryEneId() const { return m_primaryeneid; }
    G4int GetPrimaryxyId() const { return m_finalprimaryxyid; }
    G4int GetPrimarytxtyId() const { return m_finalprimarytxtyid; }
    G4int GetPrimaryInitialxyId() const { return m_initialprimaryxyid; }
    G4int GetPrimaryInitialzxId() const { return m_initialprimaryzxid; }
    G4int GetPrimaryInitialEneId() const { return m_initialprimaryeneid; }
    G4int GetPrimaryInitialtxtyId() const { return m_initialprimarytxtyid; }
    G4int GetPrimaryInitialxpxId() const { return m_initialprimaryxpxid; }
    G4int GetPrimaryInitialypyId() const { return m_initialprimaryypyid; }
    G4int GetPrimaryThetaId() const { return m_primarytheta; }
    G4int GetPrimaryPhiId() const { return m_primaryphi; }
    G4int GetEneID(G4int particleID) const;
    G4int GetxyID(G4int particleID) const;
    G4int GetthetaxtehtayID(G4int particleID) const;
    G4int GetthetaID(G4int particleID) const;
    G4int GetphiID(G4int particleID) const;
    void Book();

private:
    // Histogram id numbers

    // Initial primary energy
    G4int m_initialprimaryeneid;
    // Initial primary xy
    G4int m_initialprimaryxyid;
    // Initial primary zx
    G4int m_initialprimaryzxid;
    // Initial primary theta_x-theta_y
    G4int m_initialprimarytxtyid;
    // Initial primary x-p_x
    G4int m_initialprimaryxpxid;
    // Initial primary y-p_y
    G4int m_initialprimaryypyid;
    // Final primary theta distribution
    G4int m_primarytheta;
    // Final electron theta distribution
    G4int m_electrontheta;
    // Final positron theta distribution
    G4int m_positrontheta;
    // Final gamma theta distribution
    G4int m_gammatheta;
    // Final pions theta distribution
    G4int m_piontheta;
    // Final muons theta distribution
    G4int m_muontheta;
    // Final primary phi distribution
    G4int m_primaryphi;
    // Final electron phi distribution
    G4int m_electronphi;
    // Final positron phi distribution
    G4int m_positronphi;
    // Final gamma phi distribution
    G4int m_gammaphi;
    // Final pions phi distribution
    G4int m_pionphi;
    // Final muon phi distribution
    G4int m_muonphi;
    // Final primary energy
    G4int m_primaryeneid;
    // Final positron energy
    G4int m_posieneid;
    // Final electron energy
    G4int m_electroneneid;
    // Final gamma energy
    G4int m_gammaeneid;
    // Final pions energy
    G4int m_pioneneid;
    // Final muons energy
    G4int m_muoneneid;
    // Final primary transverse distribution
    G4int m_finalprimaryxyid;
    // Final electron transverse distribution
    G4int m_finalelectronxyid;
    // Final positron transverse distribution
    G4int m_finalpositronxyid;
    // Final gamma transverse distribution
    G4int m_finalgammaxyid;
    // Final pions transverse distribution
    G4int m_finalpionxyid;
    // Final muons transverse distribution
    G4int m_finalmuonxyid;
    // Final primary transverse angle
    G4int m_finalprimarytxtyid;
    // Final electron transverse angle
    G4int m_finalelectrontxtyid;
    // Final positron transverse angle
    G4int m_finalpositrontxtyid;
    // Final gamma transverse angle
    G4int m_finalgammatxtyid;
    // Final pions transverse angle
    G4int m_finalpiontxtyid;
    // Final muons transverse angle
    G4int m_finalmuontxtyid;
    // Flag to default activate the histograms
    // This needs to be set to 'true' for an implementation issue
    // that requires additional investigation.
    // The analysis manager activation depends on the *histograms*
    // activation, regardless the activation status of the ntuples.
    // Therefore, if the histograms are not activated, the ntuples are also not
    // written. Because histograms are not computationally intensive and memory
    // demanding, we should just keep them active to allow ntuples to be
    // written.
    G4bool m_activatehistograms = true;
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
    // Get Ntuple index
    G4int GetNtupleID(G4int particleID) const;
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
