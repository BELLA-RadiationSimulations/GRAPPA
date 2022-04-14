/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MySDAbsorber.hpp>

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
    G4String name = particle->GetParticleName();
    G4int pID = aTrack->GetParentID();
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
            analysisManager->FillNtupleDColumn(ntupleid, 0, position.x());
            analysisManager->FillNtupleDColumn(ntupleid, 1, position.y());
            analysisManager->FillNtupleDColumn(ntupleid, 2, position.z());
            analysisManager->FillNtupleDColumn(ntupleid, 3, momentum.x());
            analysisManager->FillNtupleDColumn(ntupleid, 4, momentum.y());
            analysisManager->FillNtupleDColumn(ntupleid, 5, momentum.z());
            analysisManager->FillNtupleDColumn(ntupleid, 6, time);
            analysisManager->AddNtupleRow(ntupleid);
        }

        return true;
    }

    return false;
}
