//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "BasePGA.hh"

extern G4long seed;
extern G4bool debug;
extern G4String inFile;
extern G4double chosen_energy;
extern G4bool detTest;

BasePGA::BasePGA()
: pgaM(NULL), fParticleGun(0)
{
  fParticleGun = new G4ParticleGun(1);
  fParticleGun->SetParticleTime(0.0*ns);
  gRandom->SetSeed(seed);
  sInfo = SourceInformation::Instance();
  detInfo = DetectorInformation::Instance();
}

BasePGA::~BasePGA()
{
  delete pgaM;
  delete fParticleGun;
}

void BasePGA::CallMessenger()
{
  pgaM = new PGAMessenger(this);
}

void BasePGA::CreateInputSpectrum(TGraph* tBrems_in)
{
  if(debug)
    std::cout << "BasePGA::CreateInputSpectrum -> Creating Input Spectrum..." << std::endl;

  std::vector<double> dNdEv;

  // create energies vector with 5eV spacing
  // find max
  double dx = 5.0e-6;
  G4double maxE = TMath::MaxElement(tBrems_in->GetN(), tBrems_in->GetX());
  SourceInformation* sInfo = SourceInformation::Instance();
  sInfo->SetBeamMax(maxE);
  G4int nbins = maxE/dx; // 5eV Spacing
  G4double counter = 0;
  for(int i=0;i<nbins;++i)
  {
    counter+= dx;
    energies.push_back(counter);
  }

  // Evaluate Closest Energies in tgraph
  for(unsigned int i=0;i<energies.size();++i)
  {
    dNdEv.push_back(tBrems_in->Eval(energies[i]));
  }

  N.push_back(0);

  if(debug)
    std::cout << "BasePGA::CreateInputSpectrum -> Interpolating..." << std::endl;

  for(unsigned int i=1;i<dNdEv.size();++i)
  {
    double yAvg = 0.5*(dNdEv.at(i) + dNdEv.at(i - 1));
    N.push_back(N.at(i - 1) + dx*yAvg);
  }
  if(debug)
  {
    for(int i=0;i<10;++i)
    {
      std::cout << "BasePGA::CreateInputSpectrum -> Energies: "
      << energies[i] << std::endl;
      std::cout << "BasePGA::CreateInputSpectrum -> dNdE: "
      << dNdEv[i] << std::endl;
      std::cout << "BasePGA::CreateInputSpectrum -> N: "
      << N[i] << std::endl;
    }
  }
}

G4double BasePGA::SampleEnergyRange(double center_energy, double width=0.005)
{
  return Random.Uniform(center_energy-width, center_energy+width);
}

G4double BasePGA::SampleUResonances()
 {
    std::vector<double> er;
    er.push_back(1.65624253132*MeV);
    er.push_back(1.7335537285*MeV);
    er.push_back(1.86232584382*MeV);

    G4int idx = Random.Integer(er.size());
    G4double de = 25.0*eV;

    return Random.Uniform(er[idx]-de, er[idx]+de);
}

void BasePGA::StartUserMacroInputs()
{
  G4cout << G4endl << "BasePGA::StartUserMacroInputs" << G4endl;
  G4cout << "----------------------------------------------------------------------" << G4endl;
}

void BasePGA::ReadWeighted()
{
  InputFileManager* ifm = InputFileManager::Instance();

  tBrems = 0;
  tSample = 0;
  hSample = 0;

  ifm->ReadWeightedInput(inFile.c_str(), tBrems, tSample, hSample);
  if(!tSample || !hSample)
  {
    G4cerr << "BasePG::ReadWeighted() -> FATAL ERROR Failure to grab TGraphs from File: " << inFile << G4endl;
    exit(1);
  }
  G4cout << "BasePG::ReadWeighted() -> Reading SAMPLED Distribution from: " << inFile << G4endl;
}

void BasePGA::ReadNonWeighted()
{
  InputFileManager* ifm = InputFileManager::Instance();
  ifm->ReadNonWeightedInput(inFile.c_str(), tBrems);

  if(debug)
    std::cout << "BasePG::ReadNonWeighted() -> Calling CreateInputSpectrum..." << std::endl;

  CreateInputSpectrum(tBrems);
  sInfo->SetBeamMax(TMath::MaxElement(tBrems->GetN(), tBrems->GetX()));
  G4cout << "BasePG::ReadNonWeighted() -> Reading NON-SAMPLED Distribution from: " << inFile << G4endl;
}

void BasePGA::SetUserEnergy()
{
  G4cout << "BasePG::SetUserEnergy() Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
  sInfo->SetBeamMax(chosen_energy);
}

void BasePGA::SetupNonBremTest()
{
  beam_size = 10.0*mm;
  beam_size_x = 10.0*mm;
  beam_size_y = 10.0*mm;
  fParticleGun->SetParticleDefinition(G4Gamma::Definition());
  if(!detTest)
    beamStart = -100.;
  else
    beamStart = -50.;
  G4cout << "BasePGA::SetupNonBremTest -> Particle Type set to Gamma!" << G4endl;
  sInfo->SetBeamMax(-1.);
  sInfo->SetSourceZPosition(beamStart);
  detInfo->setShiftFactor(beamStart);
}
