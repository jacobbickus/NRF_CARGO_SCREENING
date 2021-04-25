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

#ifndef DetBuildResPGA_h
#define DetBuildResPGA_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetBuildResPGAMessenger.hh"

class DetBuildResPGAMessenger;

class DetBuildResPGA : public PrimaryGeneratorAction
{

public:
DetBuildResPGA();
virtual ~DetBuildResPGA();

public:
virtual void GeneratePrimaries(G4Event*);
void SetBeamSizeX(G4double val){beam_size_x = val;}
void SetBeamSizeY(G4double val){beam_size_y = val;}

private:
  G4ParticleGun* fParticleGun;
  G4double beam_size_x, beam_size_y;
  DetBuildResPGAMessenger* pgaM;

};

#endif
