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

#include "PrimaryGeneratorAction.hh"

extern G4long seed;
extern G4String inFile;
extern G4double chosen_energy;
extern G4bool resonanceTest;
extern G4bool bremTest;
extern G4bool SampleEnergyRangebool;
extern G4double uniform_width;
extern G4bool debug;

PrimaryGeneratorAction::PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction(), pgaM(NULL),
        fParticleGun(0),fFileOpen(false), cutE(-1.), lowImportance(-1.)
{
  fParticleGun = new G4ParticleGun(1);
  if(!bremTest)
    beam_size = 44.0*mm; // optimized beam size for a 4.5cm radius interrogation object
  else
    beam_size = 1.3*mm;

  // Call messenger after default beams are set that way user can change default
  pgaM = new PGAMessenger(this);

  if(bremTest)
  {
    fParticleGun->SetParticleDefinition(G4Electron::Definition());
    beamStart = 129.9;
    G4cout << "Particle Type set to Electron!" << G4endl;
    file_check = false;
  }
  else
  {
    fParticleGun->SetParticleDefinition(G4Gamma::Definition());
    beamStart = 139.9;
    G4cout << "Particle Type set to Gamma!" << G4endl;
  }

  SourceInformation* sInfo = SourceInformation::Instance();
  sInfo->SetSourceZPosition(beamStart);
  // Default Kinematics
  fParticleGun->SetParticleTime(0.0*ns);

  if(chosen_energy < 0)
  {
    gRandom->SetSeed(seed);

    CheckFile(inFile.c_str());
    fin = TFile::Open(inFile.c_str());
    fFileOpen = true;
    fin->cd();
    hBrems  = (TH1D*) fin->Get("hBrems");

    if(debug)
      hBrems->Print();

    if(!hBrems)
    {
      G4cerr << "PrimaryGeneratorAction::PrimaryActionGenerator FATAL ERROR -> hBrems Fail." << G4endl;
      exit(1);
    }

    if(!inFile.compare("brems_distributions.root"))
    {
        file_check = false;
        gBrems = (TGraph*) fin->Get("Graph_from_hBrems");
        hSample = (TH1D*) fin->Get("hSample");
        gSample = (TGraph*) fin->Get("Graph_from_hSample");

        if(debug)
          hSample->Print();

        if(!hSample || !gSample || !gBrems)
        {
          G4cerr << "PrimaryGeneratorAction::PrimaryGeneratorAction() -> FATAL ERROR Failure to grab TGraphs from File: " << inFile << G4endl;
          exit(1);
        }
        G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Reading SAMPLED Distribution from: " << inFile << G4endl;
    }
    else
    {
      file_check = true;

      if(debug)
        std::cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Calling CreateInputSpectrum..." << std::endl;

      CreateInputSpectrum(hBrems);
      G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Reading NON-SAMPLED Distribution from: " << inFile << G4endl;
    }

  } // end of chosen_energy < 0
  else
  {
    file_check = false;
    G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
  }

  G4cout << G4endl << "User Macro Inputs" << G4endl;
  G4cout << "----------------------------------------------------------------------" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete pgaM;
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // ******************** Energy Sampling Options *************************** //

    G4double w = 1.;
    if(!resonanceTest && chosen_energy < 0)
    {
      // User IS USING importance sampling
      if(!file_check)
      {
        // Grab random from importance sampling distribution
        energy = hSample->GetRandom()*MeV;
        // Grab contents of bin closest
        // TGraph::Eval -> Description: linear interpolation between the
        // two points close to x is computed. If x is outside the graph
        // range, a linear extrapolation is computed. Eval here returns the
        // probability per 5 eV for each respective distribution
        G4double dNdE = gBrems->Eval(energy);
        G4double importanceSampling = 0.;
        // This hopes to quicken simulation by avoiding TGraph::Eval()
        if(cutE > 0.1)
          if(energy < cutE)
            importanceSampling = lowImportance;
          else
            importanceSampling = gSample->Eval(energy);
        else
          importanceSampling = gSample->Eval(energy);

        // Create importance weighting based on the two distributions probability
        w = dNdE/importanceSampling;
      }
      // User IS NOT USING importance sampling
      else
      {
        double random = G4UniformRand()*N[N.size() - 1];
        for(unsigned int i=0;i<N.size();++i)
        {
          if(N[i] > random)
          {
            double f = (random - N[i - 1]) / (N[i] - N[i - 1]);
            energy = f*energies[i] + (1 - f)*energies[i - 1];
            break;
          } // end of if(N[i] > random )
        } // end of for
      } // end of else not using importance sampling !filecheck
      energy = energy*MeV;
    } // end of !resonance
    // The user has selected a mono-energetic beam
    else if(chosen_energy > 0 && !SampleEnergyRangebool)
      energy = chosen_energy*MeV;
    // User wishes to sample from Uranium resonance energies. dNdE will not
    // be used here
    else if(chosen_energy > 0 && SampleEnergyRangebool)
      energy = SampleEnergyRange(chosen_energy,uniform_width);
    else
      energy = SampleUResonances();

// ************************************************************************************ //
// Input Handling Complete
// ************************************************************************************ //

    // Set the energy
    fParticleGun->SetParticleEnergy(energy);

    const float pi=acos(-1);

    // Set beam position
    G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
    G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
    fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator

    // Set beam momentum
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1)); // along z axis

    fParticleGun->GeneratePrimaryVertex(anEvent);

// Pass the event information
    eventInformation *anInfo = new eventInformation(anEvent);
    anInfo->SetWeight(w);
    anInfo->SetBeamEnergy(energy);
    anEvent->SetUserInformation(anInfo);
}

G4double PrimaryGeneratorAction::SampleUResonances()
 {
    std::vector<double> er;
    er.push_back(1.65624253132*MeV);
    er.push_back(1.7335537285*MeV);
    er.push_back(1.86232584382*MeV);

    G4int idx = Random.Integer(er.size());
    G4double de = 25.0*eV;

    return Random.Uniform(er[idx]-de, er[idx]+de);
}

G4double PrimaryGeneratorAction::SampleEnergyRange(double center_energy, double width=0.005)
{
  return Random.Uniform(center_energy-width, center_energy+width);
}

void PrimaryGeneratorAction::CreateInputSpectrum(TH1D* hBrems_in)
{
  if(debug)
    std::cout << "PrimaryGeneratorAction::CreateInputSpectrum -> Creating Input Spectrum..." << std::endl;

  std::vector<double> dNdEv;

  if(debug)
  {
    std::cout << "PrimaryGeneratorAction::CreateInputSpectrum -> Bin Center 0: " << hBrems_in->GetXaxis()->GetBinCenter(0)
     << std::endl << "Bin Center 1: " << hBrems_in->GetXaxis()->GetBinCenter(1)
     << std::endl << "Bin Content 0: " << hBrems_in->GetBinContent(0)
     << std::endl << "Bin Content 1: " << hBrems_in->GetBinContent(1) << std::endl;
  }

  for(int i=1;i<hBrems_in->GetNbinsX();++i)
  {
    // set energies
    energies.push_back(hBrems_in->GetXaxis()->GetBinCenter(i));
    // set dNdE
    dNdEv.push_back(hBrems_in->GetBinContent(i));
  }

  double dx = energies[1] - energies[0];
  N.push_back(0);

  if(debug)
    std::cout << "PrimaryGeneratorAction::CreateInputSpectrum -> Interpolating..." << std::endl;

  for(unsigned int i=1;i<dNdEv.size();++i)
  {
    double yAvg = 0.5*(dNdEv.at(i) + dNdEv.at(i - 1));
    N.push_back(N.at(i - 1) + dx*yAvg);
  }
  if(debug)
  {
    for(int i=0;i<10;++i)
    {
      std::cout << "PrimaryGeneratorAction::CreateInputSpectrum -> Energies: "
      << energies[i] << std::endl;
      std::cout << "PrimaryGeneratorAction::CreateInputSpectrum -> dNdE: "
      << dNdEv[i] << std::endl;
      std::cout << "PrimaryGeneratorAction::CreateInputSpectrum -> N: "
      << N[i] << std::endl;
    }
  }
}

void PrimaryGeneratorAction::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "PrimaryGeneratorAction::CheckFile -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}
