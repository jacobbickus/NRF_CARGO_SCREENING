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

#include "DetResponsePrimaryGenerator.hh"

extern G4long seed;
extern G4String inFile;
extern G4double chosen_energy;
extern G4bool SampleEnergyRangebool;
extern G4double uniform_width;
extern G4bool debug;

DetResponsePrimaryGenerator::DetResponsePrimaryGenerator()
        : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fFileOpen(false)
{
  fParticleGun = new G4ParticleGun(1);
  fParticleGun->SetParticleDefinition(G4Gamma::Definition());
  beamStart = -10.0;
  beam_size_x = 30.0*cm;
  beam_size_y = 60.0*cm;
  G4cout << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator -> Particle Type set to Gamma!" << G4endl;


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
    tBrems  = (TGraph*) fin->Get("Graph_from_hBrems");

    if(!tBrems)
    {
      G4cerr << "DetResponsePrimaryGenerator::PrimaryActionGenerator FATAL ERROR -> hBrems Fail." << G4endl;
      exit(1);
    }
    SetBeamMax(TMath::MaxElement(tBrems->GetN(), tBrems->GetX()));
    if(!inFile.compare("brems_distributions.root"))
    {
        file_check = false;
        gSample = (TGraph*) fin->Get("Graph_from_hSample_short");
        hSample = (TH1D*) fin->Get("hSample_long");

        if(!gSample || !hSample)
        {
          G4cerr << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator() -> FATAL ERROR Failure to grab TGraphs from File: " << inFile << G4endl;
          exit(1);
        }
        G4cout << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator -> Reading SAMPLED Distribution from: " << inFile << G4endl;
    }
    else
    {
      file_check = true;

      if(debug)
        std::cout << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator -> Calling CreateInputSpectrum..." << std::endl;

      CreateInputSpectrum(tBrems);
      G4cout << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator -> Reading NON-SAMPLED Distribution from: " << inFile << G4endl;
    }

  } // end of chosen_energy < 0
  else
  {
    file_check = false;
    G4cout << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
    SetBeamMax(chosen_energy);
  }

  G4cout << G4endl << "User Macro Inputs" << G4endl;
  G4cout << "----------------------------------------------------------------------" << G4endl;
}

DetResponsePrimaryGenerator::~DetResponsePrimaryGenerator()
{
  delete fParticleGun;
}

void DetResponsePrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  // ******************** Energy Sampling Options *************************** //

    if(debug && anEvent->GetEventID() == 0)
      std::cout << "DetResponsePrimaryGenerator::GeneratePrimaries -> First Primary Generated." << std::endl;

    G4double w = 1.;
    if(chosen_energy < 0)
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
        G4double dNdE = tBrems->Eval(energy);
        G4double importanceSampling = gSample->Eval(energy);
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

// ************************************************************************************ //
// Input Handling Complete
// ************************************************************************************ //

    // Set the energy
    fParticleGun->SetParticleEnergy(energy);

    const float pi=acos(-1);

    // Set beam position
    G4double x_r = beam_size_x*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
    G4double y_r = beam_size_y*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
    fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator

    // Set beam momentum
    G4double rand_y_dir = cos(180.*G4UniformRand()*CLHEP::deg);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, rand_y_dir, 1)); // along z axis
    fParticleGun->GeneratePrimaryVertex(anEvent);

// Pass the event information
    eventInformation *anInfo = new eventInformation(anEvent);
    anInfo->SetWeight(w);
    anInfo->SetBeamEnergy(energy);
    anEvent->SetUserInformation(anInfo);

    if(debug && anEvent->GetEventID() == 0)
      std::cout << "DetResponsePrimaryGenerator::GeneratePrimaries -> First Primary Complete." << std::endl;
}

G4double DetResponsePrimaryGenerator::SampleEnergyRange(double center_energy, double width=0.005)
{
  return Random.Uniform(center_energy-width, center_energy+width);
}

void DetResponsePrimaryGenerator::CreateInputSpectrum(TGraph* tBrems_in)
{
  if(debug)
    std::cout << "DetResponsePrimaryGenerator::CreateInputSpectrum -> Creating Input Spectrum..." << std::endl;

  std::vector<double> dNdEv;

  // create energies vector with 5eV spacing
  // find max
  double dx = 5.0e-6;
  G4double maxE = TMath::MaxElement(tBrems_in->GetN(), tBrems_in->GetX());
  SetBeamMax(maxE);
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
    std::cout << "DetResponsePrimaryGenerator::CreateInputSpectrum -> Interpolating..." << std::endl;

  for(unsigned int i=1;i<dNdEv.size();++i)
  {
    double yAvg = 0.5*(dNdEv.at(i) + dNdEv.at(i - 1));
    N.push_back(N.at(i - 1) + dx*yAvg);
  }
  if(debug)
  {
    for(int i=0;i<10;++i)
    {
      std::cout << "DetResponsePrimaryGenerator::CreateInputSpectrum -> Energies: "
      << energies[i] << std::endl;
      std::cout << "DetResponsePrimaryGenerator::CreateInputSpectrum -> dNdE: "
      << dNdEv[i] << std::endl;
      std::cout << "DetResponsePrimaryGenerator::CreateInputSpectrum -> N: "
      << N[i] << std::endl;
    }
  }
}

void DetResponsePrimaryGenerator::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "DetResponsePrimaryGenerator::CheckFile -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}
