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

#include "InputFileManager.hh"

extern G4bool debug;

InputFileManager *InputFileManager::instance = 0;

InputFileManager::InputFileManager()
: fFileOpen(false)
{
  if(debug)
    std::cout << "InputFileManager::InputFileManager -> Instatiated." << std::endl;
}

InputFileManager::~InputFileManager()
{

}


void InputFileManager::ReadWeightedInput(const char* inFile, TGraph* &tBrems, TGraph* &tSample, TH1D* &hSample)
{
  if(debug)
    std::cout << "InputFileManager::ReadWeightedInput -> Importing weighted data..." << std::endl;

  CheckFile(inFile);
  fin = TFile::Open(inFile);
  fFileOpen = true;
  fin->cd();
  fin->GetObject("Graph_from_hBrems",tBrems);
  fin->GetObject("Graph_from_hSample_short",tSample);
  fin->GetObject("hSample_long",hSample);
  CloseInputFile();

  if(debug)
    tBrems->Print();

  fFileOpen = false;
}

void InputFileManager::ReadNonWeightedInput(const char* inFile, TGraph* &tBrems)
{
  if(debug)
    std::cout << "InputFileManager::ReadNonWeightedInput -> Importing non-weighted data..." << std::endl;

  CheckFile(inFile);
  fin = TFile::Open(inFile);
  fFileOpen = true;
  fin->cd();
  fin->GetObject("Graph_from_hBrems",tBrems);
  CloseInputFile();

  if(debug)
    tBrems->Print();

  fFileOpen = false;
}

void InputFileManager::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "InputFileManager::CheckFile -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}
