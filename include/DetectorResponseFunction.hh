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

#ifndef DetectorResponseFunction_h
#define DetectorResponseFunction_h 1

#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"

#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include <vector>
#include "TRandom2.h"

class DetectorResponseFunction
{
  static DetectorResponseFunction *instance;

  DetectorResponseFunction(G4double);
public:

  static DetectorResponseFunction *Instance(G4double maxE=1.8)
  {
    if(!instance)
    {
      instance = new DetectorResponseFunction(maxE);
    }
    return instance;
  }

  ~DetectorResponseFunction();

  void CheckFile(const char*);
  G4double GetDetectorPhotoelectrons(G4double);
  G4double GetScintillationResponse(G4double);
  G4double GetCherenkovResponse(G4double);
  G4int GetBin(G4double);
  //void SetBeamMax(G4double maximumE){maxE = maximumE;}
  //G4double GetBeamMax()const {return maxE;}
  G4double GetProjectedPhotoelectrons(G4double);
  void CloseInputFile()
  {
    if(fin->IsOpen())
      fin->Close();
  }

private:
  TProfile* tdet_response;
  TH2D* hdet_response;
  TProfile* tdet_scintillation_response;
  TProfile* tdet_cherenkov_response;
  TFile* fin;
  TAxis* xAxis;
  std::vector<TH1D*> projections;
};

#endif
