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

#include "DetectorResponseFunction.hh"

extern G4String response_function_file;
extern G4bool debug;

DetectorResponseFunction *DetectorResponseFunction::instance = 0;

DetectorResponseFunction::DetectorResponseFunction()
{
  CheckFile(response_function_file.c_str());
  fin = TFile::Open(response_function_file.c_str());
  fin->cd();

  if(debug)
    std::cout << "DetectorResponseFunction::DetectorResponseFunction -> Grabbing Response Function..." << std::endl;

  tdet_response = (TProfile*) fin->Get("DetectorResponse");

  if(debug)
    std::cout << "DetectorResponseFunction::DetectorResponseFunction -> Detector Response: " << tdet_response << std::endl;

  if(debug && tdet_response == NULL)
    std::cerr << "DetectorResponseFunction::DetectorResponseFunction -> Detector Response TProfile NULL... Check Detector Response Function Input." << std::endl;

  if(tdet_response == NULL)
    G4cerr << "DetectorResponseFunction::DetectorResponseFunction -> Detector Response TProfile NULL... Check Detector Response Function Input." << G4endl;

  tdet_scintillation_response = (TProfile*) fin->Get("ScintillationResponse");
  tdet_cherenkov_response = (TProfile*) fin->Get("CherenkovResponse");
  if(debug)
    std::cout << "DetectorResponseFunction::DetectorResponseFunction -> Response Functions Obtained." << std::endl;

  G4cout << "DetectorResponseFunction::DetectorResponseFunction -> Response Functions Obtained." << G4endl;
}

DetectorResponseFunction::~DetectorResponseFunction()
{}

G4double DetectorResponseFunction::GetDetectorPhotoelectrons(G4double incident_energy)
{
  return tdet_response->Interpolate(incident_energy);
}

G4double DetectorResponseFunction::GetScintillationResponse(G4double incident_energy)
{
  return tdet_scintillation_response->Interpolate(incident_energy);
}

G4double DetectorResponseFunction::GetCherenkovResponse(G4double incident_energy)
{
  return tdet_cherenkov_response->Interpolate(incident_energy);
}

void DetectorResponseFunction::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "DetectorResponseFunction::CheckFile -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}
