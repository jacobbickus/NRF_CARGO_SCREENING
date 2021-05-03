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

#include "PGAIntObj.hh"

extern G4bool WEIGHTED;
extern G4bool debug;

PGAIntObj::PGAIntObj()
: G4VUserPrimaryGeneratorAction(), BasePGA()
{
  SetupNonBremTest();
  beamStart = -20*cm;
  sInfo->SetSourceZPosition(beamStart);
  detInfo->setShiftFactor(beamStart);

  if(WEIGHTED)
  {
    ReadWeighted();
    file_check = false;
  }
  else
  {
    ReadNonWeighted();
    file_check = true;
  }

  CallMessenger();
  StartUserMacroInputs();
}

PGAIntObj::~PGAIntObj()
{}

void PGAIntObj::GeneratePrimaries(G4Event* anEvent)
{
  if(debug && anEvent->GetEventID() == 0)
    std::cout << "PGAIntObj::GeneratePrimaries -> First Primary Generated." << std::endl;

  G4double w = 1.;
  // User IS USING importance sampling
  if(!file_check)
  {
    energy = h_sample->GetRandom()*MeV;
    G4double dNdE = g_input->Eval(energy);
    G4double importanceSampling = g_sample->Eval(energy);
    w = dNdE/importanceSampling;
    // this is dangerous fixing weight...
    if(w > 1.)
      w = 1.;

    int weight_counter = 0;
    while(w < 0)
    {
      weight_counter++;
      if(weight_counter > 5)
      {
        G4cerr << "PGAIntObj::GeneratePrimaries -> ERROR Weight consistently found to be < 0 Breaking Loop and Exiting." << G4endl;
        exit(100);
      }
      energy = h_sample->GetRandom()*MeV;
      dNdE = g_input->Eval(energy);
      importanceSampling = g_sample->Eval(energy);
      w = dNdE/importanceSampling;
    }
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
  } // end else

  energy = energy*MeV;

// ************************************************************************************ //
// Input Handling Complete
// ************************************************************************************ //
  fParticleGun->SetParticleEnergy(energy);
  G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
  G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
  fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
  fParticleGun->GeneratePrimaryVertex(anEvent);

// Pass the event information
  eventInformation *anInfo = new eventInformation(anEvent);
  anInfo->SetWeight(w);
  anInfo->SetBeamEnergy(energy);
  anEvent->SetUserInformation(anInfo);

  if(debug && anEvent->GetEventID() == 0)
    std::cout << "PGAIntObj::GeneratePrimaries -> First Primary Complete." << std::endl;

}
