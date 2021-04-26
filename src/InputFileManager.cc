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


void InputFileManager::ReadWeightedInput(const char* inFile, TGraph* &g_input, TGraph* &g_sample, TH1D* &h_sample)
{
  if(debug)
    std::cout << "InputFileManager::ReadWeightedBremInput -> Importing weighted data..." << std::endl;

  CheckFile(inFile);
  fin = TFile::Open(inFile);
  fFileOpen = true;
  fin->cd();
  //fin->ls();
  fin->GetObject("Graph_from_h_input",g_input);
  fin->GetObject("Graph_from_h_sample_short",g_sample);
  fin->GetObject("h_sample_long",h_sample);
}

void InputFileManager::ReadNonWeightedInput(const char* inFile, TGraph* &g_input)
{
  if(debug)
    std::cout << "InputFileManager::ReadNonWeightedBremInput -> Importing non-weighted data..." << std::endl;

  CheckFile(inFile);
  fin = TFile::Open(inFile);
  fFileOpen = true;
  fin->cd();
  fin->GetObject("Graph_from_h_input",g_input);
}

void InputFileManager::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "InputFileManager::CheckFile -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}
