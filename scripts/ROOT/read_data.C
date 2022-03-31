// To execute this MACRO file in root
// 1) open root
// 2a) .L read_data.C
// 2b) .L read_data.C+ (to run a compiled version of this library)

// This macro file extracts data from root file.
// At the moment it assumes the file structure from GRAPPA
//  Analysis.root (file, name can be changed)
//      |---> hist (histogram folder)
//      |---> ntuple (ntuple folder)
//              |---> InitialParticles (Primary particles initial ntuple)
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

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <Math/Vector3D.h>
#include <Math/Vector4D.h>
#include <TCanvas.h>
#include <TDatabasePDG.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TMath.h>
#include <TNtuple.h>
#include <TParticlePDG.h>
#include <TROOT.h>

namespace PhysicalConstants
{
  constexpr double c_m_s = 299792458.;
  constexpr double c_mm_ns = c_m_s * 1.e3 / 1.e9;
  constexpr double c_mu_fs = c_m_s * 1.e6 / 1.e15;
}

class PhaseSpace
{
private:
  std::vector<ROOT::Math::XYZVector> m_position;
  std::vector<ROOT::Math::PxPyPzMVector> m_massmomentum;
  std::vector<double> m_time;
  int m_NofParticles = 0;
  double m_reference_time = std::numeric_limits<double>::lowest();
  std::string m_ntuplename = "";
  TParticlePDG *m_part = nullptr;

public:
  PhaseSpace();
  PhaseSpace(int i);
  ~PhaseSpace();
  inline std::vector<ROOT::Math::XYZVector> GetPosition() { return m_position; }
  inline std::vector<ROOT::Math::PxPyPzMVector> GetMomentum() { return m_massmomentum; }
  inline std::vector<double> t() { return m_time; }
  TParticlePDG *GetPart() { return m_part; }
  inline int NofParticles() { return m_NofParticles; }
  inline std::string GetNTupleName() { return m_ntuplename; }
  inline double Mass() { return m_part->Mass() * 1.e3; }
  void SetPart(TParticlePDG *part);
  void SetNtupleName(std::string ntuplename);

  friend PhaseSpace ExtractPhasespace(std::string, std::string, std::string);
};

PhaseSpace::PhaseSpace()
{
}

PhaseSpace::PhaseSpace(int i)
{
  m_NofParticles = i;
  m_position.reserve(i);
  m_massmomentum.reserve(i);
  m_time.reserve(i);
}

PhaseSpace::~PhaseSpace()
{
}

void PhaseSpace::SetPart(TParticlePDG *part)
{
  m_part = part;
}

void PhaseSpace::SetNtupleName(std::string ntuplename)
{
  m_ntuplename = ntuplename;
}

PhaseSpace ExtractPhasespace(std::string particlename, std::string ntuplename, std::string filename = "Analysis.root")
{
  using namespace ROOT::Math;

  double x, y, z, px, py, pz, t, deltat, cdeltat, betax, betay, betaz, gamma,
      gamma_inv, mass_squared, mom_squared;
  int i;
  PxPyPzMVector mmomvec;

  // Pointer to file. WARNING: no error checking is performed
  std::shared_ptr<TFile> myFile(TFile::Open(filename.c_str()));
  if (!myFile)
  {
    std::cout << "ERROR: file " << filename << " provided does not exist or is not accessible" << std::endl;
    std::cout << "Returning an empty phasespace" << std::endl;
    return PhaseSpace();
  }
  // Get ntuple directory
  std::shared_ptr<TDirectory> dir((TDirectory *)myFile->Get("ntuple"));
  // Get the specified ntuple. WARNING: no error checking is performed
  std::shared_ptr<TNtuple> ntuple((TNtuple *)dir->Get(ntuplename.c_str()));
  if (!ntuple)
  {
    std::cout << "ERROR: ntuple " << ntuplename << " provided does not exist or is not accessible" << std::endl;
    std::cout << "Returning an empty phasespace" << std::endl;
    return PhaseSpace();
  }
  // Assigning the branches address
  ntuple->SetBranchAddress("X", &x);
  ntuple->SetBranchAddress("Y", &y);
  ntuple->SetBranchAddress("Z", &z);
  ntuple->SetBranchAddress("Px", &px);
  ntuple->SetBranchAddress("Py", &py);
  ntuple->SetBranchAddress("Pz", &pz);
  ntuple->SetBranchAddress("t", &t);

  // Construct a new PhaseSpace object
  PhaseSpace ps = PhaseSpace(ntuple->GetEntries());
  ps.SetNtupleName(ntuplename);
  ps.SetPart(TDatabasePDG::Instance()->GetParticle(particlename.c_str()));
  mass_squared = TMath::Power(ps.Mass(), 2);
  // First cycle to find reference time
  for (i = 0; ntuple->GetEntry(i) > 0; i++)
  {
    ps.m_reference_time = std::max(t, ps.m_reference_time);
  }

  // Second cycle to uniform particle positions
  for (i = 0; ntuple->GetEntry(i) > 0; i++)
  {
    mmomvec = PxPyPzMVector(px, py, pz, ps.Mass());
    gamma_inv = 1 / mmomvec.Gamma();
    betax = px * gamma_inv;
    betay = py * gamma_inv;
    betaz = pz * gamma_inv;
    deltat = ps.m_reference_time - t;
    cdeltat = PhysicalConstants::c_mm_ns * deltat;
    // Backpropagate particles
    ps.m_position.push_back(XYZVector(x + betax * cdeltat, y + betay * cdeltat, z + betaz * cdeltat));
    ps.m_massmomentum.push_back(mmomvec);
    ps.m_time.push_back(-deltat);
  }
  return ps;
}

void plot(std::string ntuplename, std::string filename = "Analysis.root")
{
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  std::shared_ptr<TFile> myFile(TFile::Open(filename.c_str()));

  while (!myFile)
  {
    std::cout << filename << " is an invalid file, please provide a valid file: ";
    std::cin >> filename;
    myFile.reset((TFile *)TFile::Open(filename.c_str()));
  }

  // Create a canvas and divide it into 2x2 pads
  std::shared_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2, 2);

  // Get ntuple directory
  std::shared_ptr<TDirectory> dir((TDirectory *)myFile->Get("ntuple"));

  std::shared_ptr<TNtuple> ntuple((TNtuple *)dir->Get(ntuplename.c_str()));
  while (!ntuple)
  {
    std::cout << "Provided name " << ntuplename << " is not valid." << std::endl;
    std::cout << "Please provide another one: ";
    std::cin >> ntuplename;
    ntuple.reset((TNtuple *)dir->Get(ntuplename.c_str()));
  }

  std::cout << "Plotting Ntuple " << ntuplename << std::endl;
  // Get ntuple

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  ntuple->Draw("X");

  // Draw Labs histogram in the pad 2
  c1->cd(2);
  ntuple->Draw("Z");

  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y  ?? how to do this?
  c1->cd(3);
  gPad->SetLogy(1);
  ntuple->Draw("t");

  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y  ?? how to do this?
  c1->cd(4);
  gPad->SetLogy(1);
  gPad->SetLogx(1);
  ntuple->Draw("Pz");
}