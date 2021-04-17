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

class DetectorResponseFunction
{
  static DetectorResponseFunction *instance;

  DetectorResponseFunction();
public:

  static DetectorResponseFunction *Instance()
  {
    if(!instance)
    {
      instance = new DetectorResponseFunction;
    }
    return instance;
  }

  ~DetectorResponseFunction();

  void CheckFile(const char*);
  G4double GetDetectorPhotoelectrons(G4double);
  G4double GetScintillationResponse(G4double);
  G4double GetCherenkovResponse(G4double);
  void CloseInputFile()
  {
    if(fin->IsOpen())
      fin->Close();
  }

private:
  TProfile* tdet_response;
  TProfile* tdet_scintillation_response;
  TProfile* tdet_cherenkov_response;
  TFile* fin;
};

#endif
