/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MySDAbsorber.hpp>

//
//  Here we use the Particle Data Group (PDG) convention
//  for particle numbering. Each particle has a unique integer ID.
//  Name: e-, id: 11
//  Name: e+, id: -11
//  Name: gamma, id: 22
//  Name: mu-, id: 13
//  Name: mu+, id: -13
//  Name: pi+, id: 211
//  Name: pi-, id: -211

// Primaries, electrons, positrons and photons absorbing layer
AbsorberSD::AbsorberSD(G4String name, HistandNTupleManager *myanalysismanager) : G4VSensitiveDetector(name)
{
    m_HistoandNtupleManager = myanalysismanager;
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();

    const G4ParticleDefinition *electron = particle_table->FindParticle("e-");
    m_electronID = electron->GetPDGEncoding();
    m_ParticleList.push_back(m_electronID);

    const G4ParticleDefinition *positron = particle_table->FindParticle("e+");
    m_positronID = positron->GetPDGEncoding();
    m_ParticleList.push_back(m_positronID);

    const G4ParticleDefinition *gamma = particle_table->FindParticle("gamma");
    m_gammaID = gamma->GetPDGEncoding();
    m_ParticleList.push_back(m_gammaID);

}

AbsorberSD::~AbsorberSD()
{
}

void AbsorberSD::Initialize(G4HCofThisEvent *)
{
}

G4bool AbsorberSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // Analysis manager for histograms
    auto analysisManager = G4AnalysisManager::Instance();
    G4double kineticEnergy, time, pz, px, py, theta_mrad, phi;
    G4ThreeVector position, momentum;

    // Access track information
    G4Track *aTrack = step->GetTrack();

    // Getting pre step point information and volume name
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4String thisVolumename = aTrack->GetVolume()->GetName();
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    const G4int particleID = particle->GetPDGEncoding();
    G4int pID = aTrack->GetParentID();
    // Check if particle is in the particle list
    G4bool particleinvector =
        ( std::find(m_ParticleList.begin(), m_ParticleList.end(), particleID) != m_ParticleList.end() );
    if (!particleinvector && (pID != 0))
    {
        return false;
    }
    G4int histeneid, histoxyid, histotxtyid, histthetaid, histphiid, ntupleid;

    // Pointer to current process
    const G4VProcess *CurrentProcess = preStepPoint->GetProcessDefinedStep();

    // Boolean flags for histogram and Ntuple filling
    G4bool FillHistogram = true;
    G4bool FillNtuple = true;

    // Energy filters for particles
    constexpr G4double MinPrimaryEnergy = 4 * MeV;
    constexpr G4double MinPhotonEnergy = 4 * MeV;
    constexpr G4double MinElectronEnergy = 1 * MeV;
    constexpr G4double MinPositronEnergy = 1 * MeV;
    constexpr G4double pi = CLHEP::pi;

    if (!analysisManager->IsActive())
    {
        return false;
    }
    if (CurrentProcess)
    {
        // Getting process name (it should match "Transportation")
        const G4String &StepProcessName = CurrentProcess->GetProcessName();
        if (StepProcessName == "Transportation")
        {
            // processing hit when entering the volume
            kineticEnergy = aTrack->GetKineticEnergy();
            position = aTrack->GetPosition();
            momentum = aTrack->GetMomentum();
            time = aTrack->GetGlobalTime();
            aTrack->SetTrackStatus(fStopAndKill);
            pz = std::abs(momentum.z());
            px = momentum.x();
            py = momentum.y();
            theta_mrad = pi - momentum.theta();
            phi = momentum.phi();
        }
        else
        {
            return false;
        }

        HistoManager *histomanager = m_HistoandNtupleManager->GetHistoManager();

        if (pID == 0)
        {
            histeneid = histomanager->GetPrimaryEneId();
            histoxyid = histomanager->GetPrimaryxyId();
            histotxtyid = histomanager->GetPrimarytxtyId();
            histthetaid = histomanager->GetPrimaryThetaId();
            histphiid = histomanager->GetPrimaryPhiId();
            ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPrimaryId();
            FillNtuple = !(kineticEnergy < MinPrimaryEnergy);
        }
        else
        {

            if (particleID == m_positronID)
            {
                histeneid = histomanager->GetPositronEneId();
                histoxyid = histomanager->GetPositronxyId();
                histotxtyid = histomanager->GetPositrontxtyId();
                histthetaid = histomanager->GetPositronThetaId();
                histphiid = histomanager->GetPositronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPositronId();
                FillNtuple = !(kineticEnergy < MinPositronEnergy);
            }
            else if (particleID == m_gammaID)
            {
                histeneid = histomanager->GetGammaEneId();
                histoxyid = histomanager->GetGammaxyId();
                histotxtyid = histomanager->GetGammatxtyId();
                histthetaid = histomanager->GetGammaThetaId();
                histphiid = histomanager->GetGammaPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetGammaId();
                FillNtuple = !(kineticEnergy < MinPhotonEnergy);
            }
            else if (particleID == m_electronID)
            {

                histeneid = histomanager->GetElectronEneId();
                histoxyid = histomanager->GetElectronxyId();
                histotxtyid = histomanager->GetElectrontxtyId();
                histthetaid = histomanager->GetElectronThetaId();
                histphiid = histomanager->GetElectronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetElectronId();
                FillNtuple = !(kineticEnergy < MinElectronEnergy);
            }
            else
            {
                return false;
            }
        }

        if (FillHistogram)
        {
            // Filling the correct histogram
            if (analysisManager->GetH1Activation(histeneid))
            {
                analysisManager->FillH1(histeneid, kineticEnergy);
            }
            if (analysisManager->GetH1Activation(histthetaid))
            {
                analysisManager->FillH1(histthetaid, theta_mrad);
            }
            if (analysisManager->GetH1Activation(histphiid))
            {
                analysisManager->FillH1(histphiid, phi);
            }
            if (analysisManager->GetH2Activation(histoxyid))
            {
                analysisManager->FillH2(histoxyid, position.x(), position.y());
            }
            if (analysisManager->GetH2Activation(histotxtyid))
            {
                analysisManager->FillH2(histotxtyid, std::atan2(px, pz), std::atan2(py, pz));
            }
        }

        FillNtuple = (FillNtuple && analysisManager->GetNtupleActivation(ntupleid));
        if (FillNtuple)
        {
            // Filling the correct Ntuple
            analysisManager->FillNtupleFColumn(ntupleid, 0, position.x());
            analysisManager->FillNtupleFColumn(ntupleid, 1, position.y());
            analysisManager->FillNtupleFColumn(ntupleid, 2, position.z());
            analysisManager->FillNtupleFColumn(ntupleid, 3, momentum.x());
            analysisManager->FillNtupleFColumn(ntupleid, 4, momentum.y());
            analysisManager->FillNtupleFColumn(ntupleid, 5, momentum.z());
            analysisManager->FillNtupleFColumn(ntupleid, 6, time);
            analysisManager->AddNtupleRow(ntupleid);
        }

        return true;
    }

    return false;
}

// Pions and muons absorbing layer
PiandMuAbsorberSD::PiandMuAbsorberSD(G4String name, HistandNTupleManager *myanalysismanager) : G4VSensitiveDetector(name)
{
    m_HistoandNtupleManager = myanalysismanager;
    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();


    const G4ParticleDefinition *muonminus = particle_table->FindParticle("mu-");
    m_muonminusID = muonminus->GetPDGEncoding();
    m_ParticleList.push_back(m_muonminusID);

    const G4ParticleDefinition *muonplus = particle_table->FindParticle("mu+");
    m_muonplusID = muonplus->GetPDGEncoding();
    m_ParticleList.push_back(m_muonplusID);

    const G4ParticleDefinition *pionminus = particle_table->FindParticle("pi-");
    m_pionminusID = pionminus->GetPDGEncoding();
    m_ParticleList.push_back(m_pionminusID);

    const G4ParticleDefinition *pionplus = particle_table->FindParticle("pi+");
    m_pionplusID = pionplus->GetPDGEncoding();
    m_ParticleList.push_back(m_pionplusID);

}

PiandMuAbsorberSD::~PiandMuAbsorberSD()
{
}

void PiandMuAbsorberSD::Initialize(G4HCofThisEvent *)
{
}

G4bool PiandMuAbsorberSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // Analysis manager for histograms
    auto analysisManager = G4AnalysisManager::Instance();
    G4double kineticEnergy, time, pz, px, py, theta_mrad, phi, charge;
    G4ThreeVector position, momentum;

    // Access track information
    G4Track *aTrack = step->GetTrack();

    // Getting pre step point information and volume name
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4String thisVolumename = aTrack->GetVolume()->GetName();
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    const G4int particleID = particle->GetPDGEncoding();
    G4int pID = aTrack->GetParentID();
    // Check if particle is in the particle list
    G4bool particleinvector =
        ( std::find(m_ParticleList.begin(), m_ParticleList.end(), particleID) != m_ParticleList.end() );
    if (!particleinvector )
    {
        return false;
    }
    G4int histeneid, histoxyid, histotxtyid, histthetaid, histphiid, ntupleid;

    // Pointer to current process
    const G4VProcess *CurrentProcess = preStepPoint->GetProcessDefinedStep();

    // Boolean flags for histogram and Ntuple filling
    G4bool FillHistogram = true;
    G4bool FillNtuple = true;
    constexpr G4double pi = CLHEP::pi;

    if (!analysisManager->IsActive())
    {
        return false;
    }
    if (CurrentProcess)
    {
        // Getting process name (it should match "Transportation")
        const G4String &StepProcessName = CurrentProcess->GetProcessName();
        if (StepProcessName == "Transportation")
        {
            // processing hit when entering the volume
            kineticEnergy = aTrack->GetKineticEnergy();
            position = aTrack->GetPosition();
            momentum = aTrack->GetMomentum();
            charge = particle->GetPDGCharge();
            time = aTrack->GetGlobalTime();
            aTrack->SetTrackStatus(fStopAndKill);
            pz = std::abs(momentum.z());
            px = momentum.x();
            py = momentum.y();
            theta_mrad = pi - momentum.theta();
            phi = momentum.phi();
        }
        else
        {
            return false;
        }

        HistoManager *histomanager = m_HistoandNtupleManager->GetHistoManager();

        if (pID == 0)
        {
            // This is forced. There shouldn't in fact be any primary pions or muons in
            // the simulation, so every particle that gets to here should have a pID != 0
            // In the future, we may consider doing something else here.
            return false;
        }
        else
        {
            // Pions are collected in a single ntuple
            if (particleID == m_pionplusID || particleID == m_pionminusID)
            {
                histeneid = histomanager->GetPionEneId();
                histoxyid = histomanager->GetPionxyId();
                histotxtyid = histomanager->GetPiontxtyId();
                histthetaid = histomanager->GetPionThetaId();
                histphiid = histomanager->GetPionPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPionId();
            }
            // Muons are collected in a single ntuple
            else if (particleID == m_muonplusID || particleID == m_muonminusID)
            {
                histeneid = histomanager->GetMuonEneId();
                histoxyid = histomanager->GetMuonxyId();
                histotxtyid = histomanager->GetMuontxtyId();
                histthetaid = histomanager->GetMuonThetaId();
                histphiid = histomanager->GetMuonPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetMuonId();
            }
            else
            {
                return false;
            }
        }

        if (FillHistogram)
        {
            // Filling the correct histogram
            if (analysisManager->GetH1Activation(histeneid))
            {
                analysisManager->FillH1(histeneid, kineticEnergy);
            }
            if (analysisManager->GetH1Activation(histthetaid))
            {
                analysisManager->FillH1(histthetaid, theta_mrad);
            }
            if (analysisManager->GetH1Activation(histphiid))
            {
                analysisManager->FillH1(histphiid, phi);
            }
            if (analysisManager->GetH2Activation(histoxyid))
            {
                analysisManager->FillH2(histoxyid, position.x(), position.y());
            }
            if (analysisManager->GetH2Activation(histotxtyid))
            {
                analysisManager->FillH2(histotxtyid, std::atan2(px, pz), std::atan2(py, pz));
            }
        }

        FillNtuple = (FillNtuple && analysisManager->GetNtupleActivation(ntupleid));
        if (FillNtuple)
        {
            // Filling the correct Ntuple
            analysisManager->FillNtupleFColumn(ntupleid, 0, position.x());
            analysisManager->FillNtupleFColumn(ntupleid, 1, position.y());
            analysisManager->FillNtupleFColumn(ntupleid, 2, position.z());
            analysisManager->FillNtupleFColumn(ntupleid, 3, momentum.x());
            analysisManager->FillNtupleFColumn(ntupleid, 4, momentum.y());
            analysisManager->FillNtupleFColumn(ntupleid, 5, momentum.z());
            analysisManager->FillNtupleFColumn(ntupleid, 6, time);
            analysisManager->FillNtupleFColumn(ntupleid, 7, charge);
            analysisManager->AddNtupleRow(ntupleid);
        }

        return true;
    }

    return false;
}
