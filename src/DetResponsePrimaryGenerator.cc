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
        : PrimaryGeneratorAction(), fParticleGun(0)
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

    InputFileManager* ifm = InputFileManager::Instance();

    if(!inFile.compare("brems_distributions.root"))
    {
      tBrems = 0;
      tSample = 0;
      hSample = 0;

      ifm->ReadWeightedInput(inFile.c_str(), tBrems, tSample, hSample);

      file_check = false;

      if(!tSample || !hSample)
      {
        G4cerr << "PrimaryGeneratorAction::PrimaryGeneratorAction() -> FATAL ERROR Failure to grab TGraphs from File: " << inFile << G4endl;
        exit(1);
      }
      G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Reading SAMPLED Distribution from: " << inFile << G4endl;
    }
    else
    {
      ifm->ReadNonWeightedInput(inFile.c_str(), tBrems);
      file_check = true;

      if(debug)
        std::cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Calling CreateInputSpectrum..." << std::endl;

      CreateInputSpectrum(tBrems);
      G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Reading NON-SAMPLED Distribution from: " << inFile << G4endl;
    }

  } // end of chosen_energy < 0
  else
  {
    file_check = false;
    G4cout << "DetResponsePrimaryGenerator::DetResponsePrimaryGenerator Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
    sInfo->SetBeamMax(chosen_energy);
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
      if(!file_check)
      {
        energy = hSample->GetRandom()*MeV;
        G4double dNdE = tBrems->Eval(energy);
        G4double importanceSampling = tSample->Eval(energy);
        w = dNdE/importanceSampling;
      }
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
    else if(chosen_energy > 0 && !SampleEnergyRangebool)
      energy = chosen_energy*MeV;
    else if(chosen_energy > 0 && SampleEnergyRangebool)
      energy = SampleEnergyRange(chosen_energy,uniform_width);

// ************************************************************************************ //
// Input Handling Complete
// ************************************************************************************ //

    // Set the energy
    fParticleGun->SetParticleEnergy(energy);

    const float pi=acos(-1);
    G4double x_r = beam_size_x*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
    G4double y_r = beam_size_y*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
    fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator

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
