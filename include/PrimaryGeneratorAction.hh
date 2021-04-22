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

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "PGAMessenger.hh"
#include "globals.hh"
#include <vector>
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
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
class PGAMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
PrimaryGeneratorAction();
virtual ~PrimaryGeneratorAction();

public:
virtual void GeneratePrimaries(G4Event*);
G4ParticleGun* GetParticleGun()
{
  return fParticleGun;
};
void SetBeamSize(G4double x)
{
  beam_size = x;
  G4cout << "PrimaryGeneratorAction::BeamSize set to: " << beam_size << " mm" << G4endl;
}

void SetBeamMax(G4double maxE){beamMax = maxE;}
G4double GetBeamMax()const {return beamMax;}

private:
  void CreateInputSpectrum(TGraph*);
  G4double SampleUResonances();
  G4double SampleEnergyRange(double,double);

private:
  PGAMessenger* pgaM;
  G4ParticleGun* fParticleGun;
  G4double beamStart, beamMax;
  G4double beam_size, energy;
  G4bool file_check;
  std::vector<double> energies, N;
  // ROOT
  TRandom2 Random;
  TGraph *tBrems;
  TGraph *tSample;
  TH1D* hSample;
};

#endif
