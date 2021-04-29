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
extern G4long seed;

DetectorResponseFunction *DetectorResponseFunction::instance = 0;

DetectorResponseFunction::DetectorResponseFunction(G4double maxE)
{
  CheckFile(response_function_file.c_str());
  fin = TFile::Open(response_function_file.c_str());
  fin->cd();

  gRandom->SetSeed(seed);

  if(debug)
    std::cout << "DetectorResponseFunction::DetectorResponseFunction -> Grabbing Response Function..." << std::endl;

  tdet_response = (TProfile*) fin->Get("DetectorResponse");
  std::string hn = "projY_";
  int x_bins = maxE/10e-3;
  for(int i=0;i<x_bins+1;++i)
  {
    std::string h_name = hn + std::to_string(i) + "_" + std::to_string(i+1);
    TH1D* proj_tmp = new TH1D();
    proj_tmp = (TH1D*) fin->Get(h_name.c_str());
    projections.push_back(proj_tmp);
  }

  if(debug)
  {
    for(int i=0;i<5;++i)
      projections[i]->Print();

    projections[projections.size()-1]->Print();
  }

  if(debug)
    std::cout << "DetectorResponseFunction::DetectorResponseFunction -> Number of Projections: " << projections.size() << std::endl;


  hdet_response = (TH2D*) fin->Get("hDetectorResponse");
  xAxis = hdet_response->GetXaxis();

  if(debug && tdet_response == NULL)
    std::cerr << "DetectorResponseFunction::DetectorResponseFunction -> Detector Response TProfile NULL... Check Detector Response Function Input." << std::endl;

  if(tdet_response == NULL)
  {
    G4cerr << "DetectorResponseFunction::DetectorResponseFunction -> Detector Response TProfile NULL... Check Detector Response Function Input." << G4endl;
    exit(1);
  }

  tdet_scintillation_response = (TProfile*) fin->Get("ScintillationResponse");
  tdet_cherenkov_response = (TProfile*) fin->Get("CherenkovResponse");
  if(debug)
    std::cout << "DetectorResponseFunction::DetectorResponseFunction -> Response Functions Obtained." << std::endl;

  G4cout << "DetectorResponseFunction::DetectorResponseFunction -> Response Functions Obtained." << G4endl;
}

DetectorResponseFunction::~DetectorResponseFunction()
{}

G4int DetectorResponseFunction::GetBin(G4double incident_energy)
{
  return xAxis->FindBin(incident_energy);
}

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

G4double DetectorResponseFunction::GetProjectedPhotoelectrons(G4double incident_energy)
{
  G4int high_bin = GetBin(incident_energy);
  G4int low_bin = high_bin - 1;

  TH1D* theProjection = projections.at(low_bin);
  G4double numPE = theProjection->GetRandom();

  if(debug)
  {
    std::cout << "DetectorResponseFunction::GetProjectedPhotoelectrons -> Incident Energy:  "
                << incident_energy << " MeV" << std::endl;
    std::cout << "DetectorResponseFunction::GetProjectedPhotoelectrons -> Bin Number:       "
                << low_bin << std::endl;
    std::cout << "DetectorResponseFunction::GetProjectedPhotoelectrons -> Photoelectrons:   "
                << numPE << std::endl;
  }

  return numPE;
}

void DetectorResponseFunction::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "DetectorResponseFunction::CheckFile -> File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}
