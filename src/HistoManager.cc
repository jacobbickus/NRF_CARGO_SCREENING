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

#include "HistoManager.hh"

extern G4String gOutName;
extern G4bool bremTest;
extern G4bool debug;
extern G4String inFile;

HistoManager::HistoManager() : fFactoryOn(false)
{
  start_time = time(0);
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book()
{
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  manager->SetVerboseLevel(0);
  // open output file
  G4bool fileOpen = manager->OpenFile(gOutName);

  if(!fileOpen)
  {
    G4cerr << "HistoManager::Book(): Cannot Open " <<manager->GetFileName()<<G4endl;
    return;
  }

  // Create ID 0 Ntuple for Brem Beam Emission
  manager->CreateNtuple("Brem","Bremsstrahlung Beam");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Theta");
  manager->CreateNtupleDColumn("Phi");
  //manager->CreateNtupleSColumn("CProcess");
  manager->FinishNtuple();

  // Create ID 1 Ntuple for Incident Chopper Data
  manager->CreateNtuple("ChopIn", "Chopper Wheel Incident Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  if(!inFile.compare(0,24,"brems_distributions.root"))
    manager->CreateNtupleDColumn("Weight");
  manager->FinishNtuple();

  // Create ID 2 Ntuple for Exiting Chopper Data
  manager->CreateNtuple("ChopOut", "Chopper Wheel Exiting Radiation Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleIColumn("isNRF");
  manager->CreateNtupleDColumn("Theta");
  manager->CreateNtupleDColumn("Phi");
  if(!inFile.compare(0,24,"brems_distributions.root"))
    manager->CreateNtupleDColumn("Weight");
  manager->FinishNtuple();

  if(!bremTest)
  {
    // Create ID 3 Ntuple for NRF Materials
    manager->CreateNtuple("NRF","NRF Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("Material");
    manager->CreateNtupleDColumn("ZPos");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

    // Create Testing TTree ID 4 for Incident Air just prior to Interrogation Object
    //manager->CreateNtuple("AirIn","Incident Air Pocket Just Prior to Int Obj");
    //manager->CreateNtupleDColumn("Energy");
    //manager->CreateNtupleSColumn("CreatorProcess");
    //manager->CreateNtupleDColumn("Angle");
    //manager->CreateNtupleDColumn("Time");
    //manager->CreateNtupleIColumn("EventID");
    //if(!inFile.compare(0,24,"brems_distributions.root"))
    //  manager->CreateNtupleDColumn("Weight");
    //manager->FinishNtuple();

    // Create ID 4 NTuple for Incident Interrogation Object Information
    manager->CreateNtuple("IntObjIn","Incident Interrogation Object Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("CreatorProcess");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->CreateNtupleDColumn("Time");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

    // Create ID 5 Ntuple for IntObj Emission Data
    manager->CreateNtuple("IntObjOut","Interrogation Object Emission Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("CreatorProcess");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->CreateNtupleDColumn("Time");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

    // Create ID 6 Ntuple for Incident Water Tank Data
    // Only Run with smaller runs this ntuple can get quite large
    manager->CreateNtuple("Water","Incident Water Tank Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("CreatorProcess");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

    // Create ID 7 Ntuple for cherenkov in water
    manager->CreateNtuple("Cherenkov","Cherenkov in Water Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleIColumn("NumSecondaries");
    manager->CreateNtupleDColumn("Time");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

    // Create ID 8 Ntuple for Detected Information
    manager->CreateNtuple("DetInfo","Detected Information");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("X");
    manager->CreateNtupleDColumn("Y");
    manager->CreateNtupleSColumn("CreatorProcess");
    manager->CreateNtupleDColumn("Time");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

    // Create ID 9 Ntuple for Detector Process Information
    manager->CreateNtuple("IncDetInfo","Incident Detector Process Information");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("DetProcess");
    if(!inFile.compare(0,24,"brems_distributions.root"))
      manager->CreateNtupleDColumn("Weight");
    manager->FinishNtuple();

  }

  fFactoryOn = true;
if(debug)
  std::cout << "HistoManager::Book() --> Complete!" << std::endl;
}

void HistoManager::finish()
{
  if(!fFactoryOn)
  {
    G4cerr << "ERROR HistoManager::finish: Failed to write to file" << G4endl;
    return;
  }

  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  manager->Write();
  manager->CloseFile();

  std::cout << std::endl << "HistoManager::finish -> Ntuples are saved." << std::endl;
  G4cout << G4endl << "HistoManager::finish -> Ntuples are saved. " << G4endl;
  stop_time = time(0);
  G4cout << "HistoManager::finish -> Time: ["
          << ((int)std::difftime(stop_time,start_time))/3600 << " h "
          << ((int)std::difftime(stop_time,start_time)%3600)/60 << " m "
          << ((int)std::difftime(stop_time,start_time)%3600)%60 << " s]"
          << G4endl;
  std::cout << "HistoManager::finish -> Time: ["
              << ((int)std::difftime(stop_time,start_time))/3600 << " h "
              << ((int)std::difftime(stop_time,start_time)%3600)/60 << " m "
              << ((int)std::difftime(stop_time,start_time)%3600)%60 << " s]"
              << std::endl;

  delete manager;
  fFactoryOn = false;
}
