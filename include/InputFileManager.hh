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

#ifndef InputFileManager_h
#define InputFileManager_h 1

#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TH1.h"
#include "TGraph.h"


class InputFileManager
{
  static InputFileManager *instance;

  InputFileManager();
public:
  static InputFileManager *Instance()
  {
    if(!instance)
    {
      instance = new InputFileManager;
    }
    return instance;
  }

  void CloseInputFile(){if(fFileOpen) fin->Close();}
  void CheckFile(const char*);
  void ReadWeightedInput(const char*, TGraph*, TGraph*, TH1D*);
  void ReadNonWeightedInput(const char*, TGraph*);

private:

TFile *fin;
G4bool fFileOpen;

~InputFileManager();
};

#endif
