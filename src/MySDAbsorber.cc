/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MySDAbsorber.hpp>

// Primaries, electrons, positrons and photons absorbing layer
AbsorberSD::AbsorberSD(G4String name, HistandNTupleManager *myanalysismanager) : G4VSensitiveDetector(name)
{
    m_HistoandNtupleManager = myanalysismanager;
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
    const G4String name = particle->GetParticleName();
    // Check if particle is in the particle list
    G4int pID = aTrack->GetParentID();
    if ((m_ParticleList.find(name) == m_ParticleList.end()) && (pID != 0))
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

    // Divergence filters for positrons
    constexpr G4double theta1 = 1 * mrad;
    constexpr G4double theta2 = 5 * mrad;
    constexpr G4double theta3 = 10 * mrad;
    constexpr G4double theta4 = 20 * mrad;
    constexpr G4double theta5 = 50 * mrad;
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
            if (kineticEnergy < MinPrimaryEnergy)
            {
                FillNtuple = false;
            }
        }
        else
        {

            if (name == "e+")
            {
                G4int pcut1, pcut2, pcut3, pcut4, pcut5;
                histeneid = histomanager->GetPositronEneId();
                histoxyid = histomanager->GetPositronxyId();
                histotxtyid = histomanager->GetPositrontxtyId();
                histthetaid = histomanager->GetPositronThetaId();
                histphiid = histomanager->GetPositronPhiId();
                pcut1 = histomanager->GetPositronEnergyCut1Id();
                pcut2 = histomanager->GetPositronEnergyCut2Id();
                pcut3 = histomanager->GetPositronEnergyCut3Id();
                pcut4 = histomanager->GetPositronEnergyCut4Id();
                pcut5 = histomanager->GetPositronEnergyCut5Id();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPositronId();
                if (kineticEnergy < MinPositronEnergy)
                {
                    FillNtuple = false;
                }
                // Filling divergence cuts here since they are only analyzing positrons
                if (FillHistogram)
                {
                    if (analysisManager->GetH1Activation(pcut1))
                    {
                        if (theta_mrad <= theta1)
                        {
                            analysisManager->FillH1(pcut1, kineticEnergy);
                        }
                    }
                    if (analysisManager->GetH1Activation(pcut2))
                    {
                        if (theta_mrad <= theta2)
                        {
                            analysisManager->FillH1(pcut2, kineticEnergy);
                        }
                    }
                    if (analysisManager->GetH1Activation(pcut3))
                    {
                        if (theta_mrad <= theta3)
                        {
                            analysisManager->FillH1(pcut3, kineticEnergy);
                        }
                    }
                    if (analysisManager->GetH1Activation(pcut4))
                    {
                        if (theta_mrad <= theta4)
                        {
                            analysisManager->FillH1(pcut4, kineticEnergy);
                        }
                    }
                    if (analysisManager->GetH1Activation(pcut5))
                    {
                        if (theta_mrad <= theta5)
                        {
                            analysisManager->FillH1(pcut5, kineticEnergy);
                        }
                    }
                }
            }
            else if (name == "gamma")
            {
                histeneid = histomanager->GetGammaEneId();
                histoxyid = histomanager->GetGammaxyId();
                histotxtyid = histomanager->GetGammatxtyId();
                histthetaid = histomanager->GetGammaThetaId();
                histphiid = histomanager->GetGammaPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetGammaId();
                if (kineticEnergy < MinPhotonEnergy)
                {
                    FillNtuple = false;
                }
            }
            else if (name == "e-")
            {

                histeneid = histomanager->GetElectronEneId();
                histoxyid = histomanager->GetElectronxyId();
                histotxtyid = histomanager->GetElectrontxtyId();
                histthetaid = histomanager->GetElectronThetaId();
                histphiid = histomanager->GetElectronPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetElectronId();
                if (kineticEnergy < MinElectronEnergy)
                {
                    FillNtuple = false;
                }
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
    const G4String name = particle->GetParticleName();
    G4int pID = aTrack->GetParentID();
    // Check if particle is in the particle list
    if (m_ParticleList.find(name) == m_ParticleList.end())
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
            if (name == "pi+" || name == "pi-")
            {
                histeneid = histomanager->GetPionEneId();
                histoxyid = histomanager->GetPionxyId();
                histotxtyid = histomanager->GetPiontxtyId();
                histthetaid = histomanager->GetPionThetaId();
                histphiid = histomanager->GetPionPhiId();
                ntupleid = m_HistoandNtupleManager->GetNTupleManager()->GetPionId();
            }
            // Muons are collected in a single ntuple
            else if (name == "mu+" || name == "mu-")
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
