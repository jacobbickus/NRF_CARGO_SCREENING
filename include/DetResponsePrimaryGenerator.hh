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

#ifndef DetResponsePrimaryGenerator_h
#define DetResponsePrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "globals.hh"
#include <vector>
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "eventInformation.hh"
#include "SourceInformation.hh"
#include "DetectorInformation.hh"
#include "InputFileManager.hh"

#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TRandom2.h"
#include "TSystem.h"
#include "TMath.h"

class G4Event;
class Analysis;

class DetResponsePrimaryGenerator : public PrimaryGeneratorAction
{

public:
DetResponsePrimaryGenerator();
virtual ~DetResponsePrimaryGenerator();

public:
virtual void GeneratePrimaries(G4Event*);

private:
  G4double beam_size_x, beam_size_y;
  G4ParticleGun* fParticleGun;

};

#endif
