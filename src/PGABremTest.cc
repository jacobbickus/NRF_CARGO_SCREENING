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

#include "PGABremTest.hh"

PGABremTest::PGABremTest()
: G4VUserPrimaryGeneratorAction(), BasePGA()
{
  beam_size = 0.5*mm;
  fParticleGun->SetParticleDefinition(G4Electron::Definition());
  beamStart = 0.; // start brem beam 1 cm behind brem radiator
  G4cout << "BasePGA::SetBremTestPGA -> Particle Type set to Electron!" << G4endl;
  SetUserEnergy();
  StartUserMacroInputs();
}

PGABremTest::~PGABremTest()
{
}

void PGABremTest::GeneratePrimaries(G4Event* anEvent)
{
  energy = chosen_energy*MeV;
  fParticleGun->SetParticleEnergy(energy);
  G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
  G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
  fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1)); // along z axis
  fParticleGun->GeneratePrimaryVertex(anEvent);

  eventInformation *anInfo = new eventInformation(anEvent);
  anInfo->SetWeight(w);
  anInfo->SetBeamEnergy(energy);
  anEvent->SetUserInformation(anInfo);

  if(debug && anEvent->GetEventID() == 0)
    std::cout << "PGABremTest::GeneratePrimaries -> First Primary Complete." << std::endl;
}
