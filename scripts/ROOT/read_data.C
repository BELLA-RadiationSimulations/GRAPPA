// To execute this MACRO file in root
// 1) open root
// 2a) .L read_data.C
// 2b) .L read_data.C+ (to run a compiled version of this library)

// This macro file extracts data from root file.
// At the moment it assumes the file structure from GRAPPA
//  Analysis.root (file, name can be changed)
//      |---> hist (histogram folder)
//      |---> ntuple (ntuple folder)
//              |---> InitialPrimary (Primary particles initial ntuple)
//              |---> FinalElectron (Electron final ntuple)
//              |---> FinalPositron (Positron final ntuple)
//              |---> FinalPhoton (Photon ntuple)
//              |---> FinalPrimary (Primary final ntuple)
//

// PhaseSpace collects the phase space of all the particles
// in the ntuple, automatically bringing them to a fixed relative time.
// After extraction, particles are moved such that their own absolute time
// coincides with the highest absolute time available.
// That time is taken as the phasespace reference time.

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <Math/Vector3D.h>
#include <Math/RotationY.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TDatabasePDG.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TH2.h>
#include <TMath.h>
#include <TNtuple.h>
#include <TParticlePDG.h>
#include <TROOT.h>
#include <TStyle.h>

namespace PhysicalConstants
{
  constexpr double c_m_s = 299792458.;
  constexpr double c_mm_ns = c_m_s * 1.e3 / 1.e9;
  constexpr double c_mu_fs = c_m_s * 1.e6 / 1.e15;
}

class ReferenceVariable
{

public:
  ReferenceVariable(){};
  ~ReferenceVariable(){};
  void AddEntry(double entry);
  void ComputeStatistics();
  inline double Cutoff() { return this->cutoff; }
  inline double Mean() { return this->mean; }
  inline double Max() { return this->Mean() + this->Cutoff(); }
  inline double Min() { return this->Mean() - this->Cutoff(); }
  inline double MinusOneSigma() { return this->Mean() - this->Std(); }
  inline double MinusTwoSigma() { return this->Mean() - 2. * this->Std(); }
  inline double MinusThreeSigma() { return this->Mean() - 3. * this->Std(); }
  inline int Nentries() { return this->nentries; }
  inline double OneSigma() { return this->Mean() + this->Std(); }
  void Reset();
  inline double Std() { return this->std; }
  inline double TwoSigma() { return this->Mean() + 2. * this->Std(); }
  inline double ThreeSigma() { return this->Mean() + 3. * this->Std(); }
  inline double Var() { return this->var; }
  void Write();

private:
  double cutoff = 0;
  double mean = 0;
  double mean_accumulator = 0;
  int nentries = 0;
  double std = 0;
  double var;
  double var_accumulator = 0;
};

void ReferenceVariable::AddEntry(double entry)
{
  this->nentries += 1;
  this->mean_accumulator += entry;
  this->var_accumulator += entry * entry;
}

void ReferenceVariable::ComputeStatistics()
{
  if (this->nentries == 0)
  {
    this->cutoff = 0;
    this->mean = 0;
    this->std = 0;
    this->var = 0;
  }
  else
  {
    this->mean = this->mean_accumulator / this->nentries;
    this->var = this->var_accumulator / this->nentries - this->mean * this->mean;
    this->std = TMath::Sqrt(this->var);
    this->cutoff = 5 * this->std;
  }
}

void ReferenceVariable::Reset()
{
  this->cutoff = 0;
  this->mean = 0;
  this->mean_accumulator = 0;
  this->nentries = 0;
  this->std = 0;
  this->var = 0;
  this->var_accumulator = 0;
}

void ReferenceVariable::Write()
{
  std::cout << " Mean = " << this->Mean() << std::endl;
  std::cout << " Variance = " << this->Var() << std::endl;
  std::cout << " Standard deviation = " << this->Std() << std::endl;
  std::cout << " Cutoff = " << this->Cutoff() << std::endl;
  std::cout << " Nentries = " << this->Nentries() << std::endl;
}

bool AcceptParticle(ROOT::Math::RhoZPhiVectorF momentum, double t, ReferenceVariable tref)
{
  constexpr double pi_4 = TMath::PiOver4();
  bool accept = true;
  accept = accept && (TMath::Abs(t - tref.Mean()) < tref.Cutoff());
  accept = accept && (momentum.Theta() < pi_4);
  return accept;
}

bool FilterVariable(double variable, ReferenceVariable reference, double cutoff = 3)
{
  return (TMath::Abs(variable - reference.Mean()) < (cutoff * reference.Std()));
}

bool FilterPosition(double x, double y, double z, ReferenceVariable referencex,
                    ReferenceVariable referencey, ReferenceVariable referencez, double cutoff = 3)
{
  bool accept = true;
  accept = accept && (TMath::Abs(x - referencex.Mean()) < (cutoff * referencex.Std()));
  accept = accept && (TMath::Abs(y - referencey.Mean()) < (cutoff * referencey.Std()));
  accept = accept && (TMath::Abs(z - referencez.Mean()) < (cutoff * referencez.Std()));
  return accept;
}

ReferenceVariable GetReferenceTime(std::shared_ptr<TNtuple> ntuple)
{
  Long64_t nentries;
  int i;
  long double mean = 0, std = 0;
  double t;
  ReferenceVariable tref;
  auto start = std::chrono::high_resolution_clock::now();

  ntuple->SetBranchAddress("t", &t);
  TBranch *b_time = ntuple->GetBranch("t");
  nentries = ntuple->GetEntries();

  b_time->SetAddress(&t);

  for (i = 0; i < nentries; i++)
  {
    b_time->GetEntry(i);
    tref.AddEntry(t);
  }

  tref.ComputeStatistics();

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Time used to find time for " << ntuple->GetName()
            << " is " << duration.count() * 1.e-3 << " s" << std::endl;

  ntuple->ResetBranchAddresses();
  return tref;
}

ROOT::Math::XYZVectorF BackPropagateParticle(ROOT::Math::XYZVectorF position, ROOT::Math::RhoZPhiVectorF momentum, float m2, float deltat)
{
  float gamma_inv = 1.f / std::sqrt(momentum.Mag2() + m2);
  float px = momentum.x();
  float py = momentum.y();
  float pz = momentum.z();
  float x = position.x();
  float y = position.y();
  float z = position.z();
  float betax = px * gamma_inv;
  float betay = py * gamma_inv;
  float betaz = pz * gamma_inv;
  float cdeltat = static_cast<float>(PhysicalConstants::c_mm_ns * deltat);

  return ROOT::Math::XYZVectorF(x + betax * cdeltat, y + betay * cdeltat, z + betaz * cdeltat);
}

std::shared_ptr<TFile> OpenFile(std::string filename = "Ntuple.root", Option_t *option = "READ")
{
  std::shared_ptr<TFile> myFile;
  myFile.reset(TFile::Open(filename.c_str(), option));
  if (!myFile)
  {
    std::cout << "ERROR: file " << filename << " provided does not exist or is not accessible" << std::endl;
    return nullptr;
  }

  if (myFile->IsZombie())
  {
    std::cout << "ERROR: file " << filename << " results as zombie" << std::endl;
    return nullptr;
  }

  return myFile;
}

std::shared_ptr<TNtuple> GetNtuple(std::shared_ptr<TFile> myFile_in, std::string ntuplename)
{
  // Pointer to file. WARNING: no error checking is performed
  std::shared_ptr<TNtuple> ntuple;

  // Get the specified ntuple. WARNING: no error checking is performed
  ntuple.reset((TNtuple *)((TDirectory *)myFile_in->Get("ntuple"))->Get(ntuplename.c_str()));
  return ntuple;
}

void WriteSametimeParticles()
{
  using namespace ROOT::Math;
  // File names
  std::string filename = "Ntuple.root";
  std::string outputfilename = "Backpropagated.root";
  std::string ntupleposiname = "FinalPositron";
  std::string ntupleelename = "FinalElectron";
  std::string ntupleinitialname = "InitialPrimary";
  std::string positreename = "PositronSameTime";
  std::string eletreename = "ElectronSameTime";
  std::string primtreename = "Primary";

  ReferenceVariable reference_time_ele, reference_time_posi;
  TParticlePDG *positron, *electron, *photon, *primary;
  double x, y, z, px, py, pz, t;
  float xback, yback, zback;
  XYZVectorF position, backposition;
  RhoZPhiVectorF momentum;
  float deltat, charge, mass, mass_squared, tref;
  int i, nentries, kept_particles, rejected_particles;
  double rotation_y_angle = TMath::Pi();
  RotationY rotationy = RotationY(rotation_y_angle);

  std::cout << "Called function WriteSametimeParticles " << std::endl;

  std::shared_ptr<TFile> myFile = OpenFile(filename);
  assert(myFile != nullptr);

  // Getting positron ntuple
  positron = TDatabasePDG::Instance()->GetParticle("e+");
  auto originalntupleposi = GetNtuple(myFile, ntupleposiname);
  assert(originalntupleposi != nullptr);
  // Getting electron ntuple
  electron = TDatabasePDG::Instance()->GetParticle("e-");
  auto originalntupleele = GetNtuple(myFile, ntupleelename);
  assert(originalntupleele != nullptr);
  // Getting primary ntuple:
  // WARNING: Change particle type associated with primary
  photon = TDatabasePDG::Instance()->GetParticle("gamma");
  auto originalntupleinitial = GetNtuple(myFile, ntupleinitialname);
  assert(originalntupleinitial != nullptr);

  std::cout << "Getting reference time for final positrons" << std::endl;
  reference_time_posi = GetReferenceTime(originalntupleposi);
  std::cout << "Reference time for positrons" << std::endl;
  reference_time_posi.Write();
  std::cout << "Getting reference time for final electrons" << std::endl;
  reference_time_ele = GetReferenceTime(originalntupleele);
  std::cout << "Reference time for electron" << std::endl;
  reference_time_posi.Write();

  std::cout << "Opening a new file" << std::endl;
  std::shared_ptr<TFile> myBackFile = OpenFile(outputfilename.c_str(), "RECREATE");
  std::shared_ptr<TTree> finalntupleposi =
      std::make_shared<TTree>(positreename.c_str(),
                              "Final backpropagated Positron distribution");
  std::shared_ptr<TTree> finalntupleele =
      std::make_shared<TTree>(eletreename.c_str(),
                              "Final backpropagated Electron distribution");
  std::shared_ptr<TTree> finalntupleprimary =
      std::make_shared<TTree>(primtreename.c_str(),
                              "Rotated primary distribution");

  // ===============================================================================================================//
  // Positron
  std::cout << "===================================" << std::endl;
  std::cout << "Now starting analysis for positrons" << std::endl;
  std::cout << "===================================" << std::endl;

  mass = positron->Mass() * 1.e3;
  mass_squared = TMath::Power(mass, 2);
  charge = positron->Charge() / 3.; // charge is in units of e / 3

  std::cout << "Now backpropagating positrons" << std::endl;

  originalntupleposi->SetBranchAddress("X", &x);
  originalntupleposi->SetBranchAddress("Y", &y);
  originalntupleposi->SetBranchAddress("Z", &z);
  originalntupleposi->SetBranchAddress("Px", &px);
  originalntupleposi->SetBranchAddress("Py", &py);
  originalntupleposi->SetBranchAddress("Pz", &pz);
  originalntupleposi->SetBranchAddress("t", &t);

  finalntupleposi->Branch("X", &backposition);
  finalntupleposi->Branch("P", &momentum);
  finalntupleposi->Branch("t", &deltat);

  nentries = originalntupleposi->GetEntries();
  kept_particles = 0;
  rejected_particles = 0;

  for (i = 0; i < nentries; i++)
  {
    originalntupleposi->GetEntry(i);
    momentum.SetXYZ(px, py, pz);
    position.SetXYZ(x, y, z);

    momentum = rotationy * momentum;
    position = rotationy * position;
    if (!AcceptParticle(momentum, t, reference_time_posi))
    {
      rejected_particles += 1;
      continue;
    }
    deltat = reference_time_posi.Mean() - t;
    backposition = BackPropagateParticle(position, momentum, mass_squared, deltat);
    kept_particles += 1;
    finalntupleposi->Fill();
  }
  std::cout << "Finished backpropagating positrons" << std::endl;
  std::cout << "Kept particles: " << kept_particles << std::endl;
  std::cout << "Rejected particles: " << rejected_particles << std::endl;
  tref = reference_time_posi.Mean();
  auto b_mass_posi = finalntupleposi->Branch("m", &mass);
  auto b_charge_posi = finalntupleposi->Branch("q", &charge);
  auto b_reference_time_posi = finalntupleposi->Branch("reference time", &tref);
  b_mass_posi->Fill();
  b_charge_posi->Fill();
  b_reference_time_posi->Fill();
  std::cout << "Now dumping backpropagated positrons into file" << std::endl;
  finalntupleposi->Write();
  finalntupleposi->ResetBranchAddresses();
  originalntupleposi->ResetBranchAddresses();
  std::cout << "End positrons section" << std::endl;
  std::cout << std::endl;

  // ===============================================================================================================//
  // Electrons
  std::cout << "===================================" << std::endl;
  std::cout << "Now starting analysis for electrons" << std::endl;
  std::cout << "===================================" << std::endl;

  mass = electron->Mass() * 1.e3;
  mass_squared = TMath::Power(mass, 2);
  charge = electron->Charge() / 3.; // charge is in units of e / 3

  std::cout << "Now backpropagating electrons" << std::endl;

  originalntupleele->SetBranchAddress("X", &x);
  originalntupleele->SetBranchAddress("Y", &y);
  originalntupleele->SetBranchAddress("Z", &z);
  originalntupleele->SetBranchAddress("Px", &px);
  originalntupleele->SetBranchAddress("Py", &py);
  originalntupleele->SetBranchAddress("Pz", &pz);
  originalntupleele->SetBranchAddress("t", &t);

  finalntupleele->Branch("X", &backposition);
  finalntupleele->Branch("P", &momentum);
  finalntupleele->Branch("t", &deltat);

  nentries = originalntupleele->GetEntries();
  kept_particles = 0;
  rejected_particles = 0;

  for (i = 0; i < nentries; i++)
  {
    originalntupleele->GetEntry(i);
    momentum.SetXYZ(px, py, pz);
    position.SetXYZ(x, y, z);

    momentum = rotationy * momentum;
    position = rotationy * position;
    if (!AcceptParticle(momentum, t, reference_time_ele))
    {
      rejected_particles += 1;
      continue;
    }
    deltat = reference_time_ele.Mean() - t;
    backposition = BackPropagateParticle(position, momentum, mass_squared, deltat);
    kept_particles += 1;
    finalntupleele->Fill();
  }
  std::cout << "Finished backpropagating electrons" << std::endl;
  std::cout << "Kept particles: " << kept_particles << std::endl;
  std::cout << "Rejected particles: " << rejected_particles << std::endl;
  tref = reference_time_ele.Mean();
  auto b_mass_ele = finalntupleele->Branch("m", &mass);
  auto b_charge_ele = finalntupleele->Branch("q", &charge);
  auto b_reference_time_ele = finalntupleele->Branch("reference time", &tref);
  b_mass_ele->Fill();
  b_charge_ele->Fill();
  b_reference_time_ele->Fill();
  std::cout << "Now dumping backpropagated electrons into file" << std::endl;
  finalntupleele->Write();
  finalntupleele->ResetBranchAddresses();
  originalntupleele->ResetBranchAddresses();
  std::cout << "End electrons section" << std::endl;
  std::cout << std::endl;

  // ===============================================================================================================//
  // Primaries
  std::cout << "===================================" << std::endl;
  std::cout << "Now starting analysis for primaries" << std::endl;
  std::cout << "===================================" << std::endl;

  primary = electron;
  mass = primary->Mass() * 1.e3;
  mass_squared = TMath::Power(mass, 2);
  charge = primary->Charge() / 3.; // charge is in units of e / 3

  std::cout << "Now extracting primaries" << std::endl;

  originalntupleinitial->SetBranchAddress("X", &x);
  originalntupleinitial->SetBranchAddress("Y", &y);
  originalntupleinitial->SetBranchAddress("Z", &z);
  originalntupleinitial->SetBranchAddress("Px", &px);
  originalntupleinitial->SetBranchAddress("Py", &py);
  originalntupleinitial->SetBranchAddress("Pz", &pz);

  finalntupleprimary->Branch("X", &position);
  finalntupleprimary->Branch("P", &momentum);

  nentries = originalntupleinitial->GetEntries();

  for (i = 0; i < nentries; i++)
  {
    originalntupleinitial->GetEntry(i);
    momentum.SetXYZ(px, py, pz);
    position.SetXYZ(x, y, z);
    momentum = rotationy * momentum;
    position = rotationy * position;
    finalntupleprimary->Fill();
  }

  std::cout << "Finished extracting primaries" << std::endl;
  std::cout << "Number of particles: " << nentries << std::endl;
  auto b_mass_primary = finalntupleprimary->Branch("m", &mass);
  auto b_charge_primary = finalntupleprimary->Branch("q", &charge);
  b_mass_primary->Fill();
  b_charge_primary->Fill();
  std::cout << "Now dumping rotated primaries into file" << std::endl;
  finalntupleprimary->Write();
  finalntupleprimary->ResetBranchAddresses();
  originalntupleinitial->ResetBranchAddresses();
  std::cout << "End primary section" << std::endl;
  std::cout << std::endl;

  std::cout << "All particles have been extracted, analysis finished" << std::endl;
  std::cout << "End function WriteSametimeParticles " << std::endl;
}

void PlotParticles()
{
  using namespace ROOT::Math;
  gROOT->Reset();
  gStyle->SetPalette(kBird);
  std::string filename = "Backpropagated.root";
  std::string positreename = "PositronSameTime";
  std::string eletreename = "ElectronSameTime";
  std::string primtreename = "Primary";
  double prefactor_time = 1;
  double prefactor_pos = 2;
  float x, y, z;
  float t, mass, mass_squared, charge, ref_time;
  int nprimaries, nentries;
  int i;
  std::shared_ptr<XYZVectorF> position;
  std::shared_ptr<RhoZPhiVectorF> momentum;
  TBranch *b_pos, *b_mom, *b_time, *b_mass, *b_charge;
  ReferenceVariable xref, yref, zref, tref;

  std::cout << "Called function PlotParticles " << std::endl;
  auto myFile = OpenFile(filename);

  std::shared_ptr<TTree> positrons((TTree *)myFile->Get(positreename.c_str()));
  std::shared_ptr<TTree> electrons((TTree *)myFile->Get(eletreename.c_str()));
  std::shared_ptr<TTree> primaries((TTree *)myFile->Get(primtreename.c_str()));

  nprimaries = primaries->GetEntries();

  // Plotting positrons

  nentries = positrons->GetEntries();

  b_pos = positrons->GetBranch("X");
  b_mom = positrons->GetBranch("P");
  b_time = positrons->GetBranch("t");
  b_mass = positrons->GetBranch("m");
  b_charge = positrons->GetBranch("q");
  b_pos->SetAddress(&position);
  b_mom->SetAddress(&momentum);
  b_time->SetAddress(&t);
  b_mass->SetAddress(&mass);
  b_charge->SetAddress(&charge);

  for (i = 0; i < nentries; i++)
  {
    b_pos->GetEntry(i);
    b_time->GetEntry(i);
    xref.AddEntry(position->X());
    yref.AddEntry(position->Y());
    zref.AddEntry(position->Z());
    tref.AddEntry(t);
  }
  xref.ComputeStatistics();
  yref.ComputeStatistics();
  zref.ComputeStatistics();
  tref.ComputeStatistics();

  TH1 *htheta = new TH1D("theta", "theta distribution", 100, 0, 0.1);
  TH1 *hene = new TH1D("Energy", "energy distribution", 100, 0, 1000);
  TH2 *xyspace = new TH2D("xy", "X-Y space", 300, xref.MinusOneSigma(), xref.OneSigma(),
                          300, yref.MinusOneSigma(), yref.OneSigma());
  TH2 *zxspace = new TH2D("zx", "Z-X space", 300, zref.MinusOneSigma(), zref.OneSigma(),
                          300, xref.MinusOneSigma(), xref.OneSigma());

  // Fetching particle mass (MeV)
  b_mass->GetEntry(0);
  mass_squared = TMath::Power(mass, 2);

  for (i = 0; i < nentries; i++)
  {
    b_pos->GetEntry(i);
    b_mom->GetEntry(i);
    b_time->GetEntry(i);
    x = position->X();
    y = position->Y();
    z = position->Z();
    if (!FilterPosition(x, y, z, xref, yref, zref, 1.5))
    {
      continue;
    }
    if (!FilterVariable(t, tref))
    {
      continue;
    }
    htheta->Fill(momentum->Theta());
    hene->Fill(TMath::Sqrt(momentum->Mag2() + mass_squared));
    xyspace->Fill(x, y);
    zxspace->Fill(z, x);
  }

  TCanvas *c1 = new TCanvas("Positrons", "Positrons", 20, 20, 1000, 1000);
  c1->Divide(2, 2);
  c1->cd(1);
  c1->SetLogz(1);
  xyspace->SetMinimum(200);
  xyspace->DrawCopy("COLZ2");
  c1->cd(2);
  c1->SetLogz(1);
  zxspace->SetMinimum(400);
  zxspace->DrawCopy("COLZ2");
  c1->cd(3);
  htheta->DrawCopy();
  c1->cd(4);
  c1->SetLogx(1);
  c1->SetLogy(1);
  hene->DrawCopy();

  positrons->ResetBranchAddresses();
  std::cout << "End function PlotParticles " << std::endl;
}

void read_data()
{
  WriteSametimeParticles();
  PlotParticles();
}