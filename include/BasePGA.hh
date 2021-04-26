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

#ifndef BasePGA_h
#define BasePGA_h 1

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

class BasePGA
{
public:
  BasePGA();
  ~BasePGA();

  G4ParticleGun* GetParticleGun()
  {
    return fParticleGun;
  };

  // For PGA Messenger
  void SetBeamSize(G4double x){beam_size = x*cm;}
  void SetBeamSizeX(G4double val){beam_size_x = val*cm;}
  void SetBeamSizeY(G4double val){beam_size_y = val*cm;}

protected:
  void CallMessenger();
  void CreateInputSpectrum(TGraph*);
  G4double SampleEnergyRange(double, double);
  G4double SampleUResonances();
  void StartUserMacroInputs();
  void ReadWeighted();
  void ReadNonWeighted();
  void SetUserEnergy();
  void SetupNonBremTest();
  const float pi=acos(-1);
  DetectorInformation* detInfo;
  SourceInformation* sInfo;
  G4double beam_size, energy, beamStart, beam_size_x, beam_size_y;
  PGAMessenger* pgaM;
  G4ParticleGun* fParticleGun;
  G4bool file_check;
  // ROOT
  TGraph *tBrems;
  TGraph *tSample;
  TH1D   *hSample;
  TRandom2 Random;
  std::vector<double> energies, N;

};

#endif
