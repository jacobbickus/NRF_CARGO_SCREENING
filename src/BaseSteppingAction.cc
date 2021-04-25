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

#include "BaseSteppingAction.hh"

extern G4bool debug;
extern G4String inFile;
extern G4long seed;
extern G4bool resonanceTest;

BaseSteppingAction::BaseSteppingAction()
: drawChopperIncDataFlag(0), drawChopperOutDataFlag(0), drawNRFDataFlag(0),
  drawIntObjInDataFlag(0), drawIntObjOutDataFlag(0), drawShieldingIncDataFlag(0),
  drawPlexiIncDataFlag(0), drawWaterIncDataFlag(0),
  drawScintillationDataFlag(0), drawScintillation2DataFlag(0),
  drawCherenkovDataFlag(0), drawCherenkov2DataFlag(0), drawDetDataFlag(0),
  WEIGHTED(false),
  stepM(NULL)
{
  if(debug)
    std::cout << "BaseSteppingAction::BaseSteppingAction Initialized." << std::endl;

  stepM = new StepMessenger(this);
  krun = RunInformation::Instance();
  kdet = DetectorInformation::Instance();
  manager = G4AnalysisManager::Instance();
  fExpectedNextStatus = Undefined;

  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;

  if(resonanceTest)
    WEIGHTED = false;
}

BaseSteppingAction::~BaseSteppingAction()
{
  delete stepM;
}


void BaseSteppingAction::FillChopperInc(G4int num, G4double locx, G4double locy)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleDColumn(num,1, energy);
  manager->FillNtupleDColumn(num,2, locx/(cm));
  manager->FillNtupleDColumn(num,3, locy/(cm));

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,4, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillChopperOut(G4int num)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleDColumn(num,1, energy);
  manager->FillNtupleDColumn(num,2, theta);
  manager->FillNtupleDColumn(num,3, phi);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,4, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillNRF(G4int num, G4double locz, const G4TrackVector* emitted_nrf)
{
  for(unsigned int i=0;i<emitted_nrf->size();++i)
  {
    if(emitted_nrf->at(i)->GetCreatorProcess()->GetProcessName() == "NRF")
    {
      manager->FillNtupleIColumn(num,0, eventID);
      manager->FillNtupleDColumn(num,1, energy);
      manager->FillNtupleSColumn(num,2, nextStep_VolumeName);
      manager->FillNtupleDColumn(num,3, locz/(cm));
      G4ThreeVector p_nrf = emitted_nrf->at(i)->GetMomentum();
      G4double theta_nrf = std::asin(std::sqrt(std::pow(p_nrf.x(),2)+std::pow(p_nrf.y(),2))/p_nrf.mag());
      G4double phi_nrf = std::asin(p_nrf.y()/p_nrf.mag());
      manager->FillNtupleDColumn(num,4, theta_nrf);
      manager->FillNtupleDColumn(num,5, phi_nrf);
      manager->FillNtupleIColumn(num,6, seed);

      if(WEIGHTED)
        manager->FillNtupleDColumn(num,7,weight);

      manager->AddNtupleRow(num);
    } // end if emitted_nrf->at(i)->GetCreatorProcess()
  } // end of for loop
}

void BaseSteppingAction::FillIntObjIn(G4int num)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleIColumn(num,1, trackID);
  manager->FillNtupleDColumn(num,2, energy);
  manager->FillNtupleDColumn(num,3, beamEnergy);
  manager->FillNtupleSColumn(num,4, CPName);
  manager->FillNtupleDColumn(num,5, theta);
  manager->FillNtupleDColumn(num,6, phi);
  manager->FillNtupleDColumn(num,7, gtime);
  manager->FillNtupleIColumn(num,8, seed);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,9, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillIntObjOut(G4int num)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleIColumn(num,1, trackID);
  manager->FillNtupleDColumn(num,2, energy);
  manager->FillNtupleDColumn(num,3, beamEnergy);
  manager->FillNtupleSColumn(num,4, CPName);
  manager->FillNtupleDColumn(num,5, theta);
  manager->FillNtupleDColumn(num,6, phi);
  manager->FillNtupleDColumn(num,7, gtime);
  manager->FillNtupleIColumn(num,8,seed);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,9, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillShielding(G4int num)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleIColumn(num,1, seed);
  manager->FillNtupleIColumn(num,2, trackID);
  manager->FillNtupleDColumn(num,3, energy);
  manager->FillNtupleDColumn(num,4, beamEnergy);
  manager->FillNtupleDColumn(num,5, gtime);
  manager->FillNtupleSColumn(num,6, CPName);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,7, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillPlexi(G4int num)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleIColumn(num,1, seed);
  manager->FillNtupleIColumn(num,2, trackID);
  manager->FillNtupleDColumn(num,3, energy);
  manager->FillNtupleDColumn(num,4, beamEnergy);
  manager->FillNtupleDColumn(num,5, gtime);
  manager->FillNtupleDColumn(num,6, theta);
  manager->FillNtupleDColumn(num,7, phi);
  manager->FillNtupleSColumn(num,8, CPName);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,9, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillWater(G4int num)
{
  manager->FillNtupleIColumn(num,0, eventID);
  manager->FillNtupleIColumn(num,1, trackID);
  manager->FillNtupleDColumn(num,2, energy);
  manager->FillNtupleSColumn(num,3, CPName);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,4, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillDetected(G4int num, G4double p_energy, G4String creatorProcess)
{
  manager->FillNtupleIColumn(num,0,eventID);
  manager->FillNtupleDColumn(num,1, p_energy);
  manager->FillNtupleDColumn(num,2, beamEnergy);
  manager->FillNtupleSColumn(num,3, creatorProcess);
  manager->FillNtupleDColumn(num,4, gtime); // time units is nanoseconds
  manager->FillNtupleIColumn(num,5, seed);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,6, weight);

  manager->AddNtupleRow(num);
}

void BaseSteppingAction::FillIncDetector(G4int num, G4double p_energy)
{
  manager->FillNtupleIColumn(num,0,eventID);
  manager->FillNtupleDColumn(num,1, p_energy);
  manager->FillNtupleDColumn(num,2, beamEnergy);
  manager->FillNtupleSColumn(num,3, procCount);
  manager->FillNtupleIColumn(num,4,seed);

  if(WEIGHTED)
    manager->FillNtupleDColumn(num,5, weight);

  manager->AddNtupleRow(num);
}
