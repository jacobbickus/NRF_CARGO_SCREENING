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

HistoManager::HistoManager() : fFactoryOn(false), WEIGHTED(false)
{
  start_time = time(0);
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;
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

  // Create ID 0 Ntuple for Incident Chopper Data
  manager->CreateNtuple("ChopIn", "Chopper Wheel Incident Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("X");
  manager->CreateNtupleDColumn("Y");

  if(WEIGHTED)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();

  if(bremTest)
  {
    // Create ID 1 Ntuple for Brem Radiator Emission
    manager->CreateNtuple("Radiator","Bremsstrahlung Beam from Radiator");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    //manager->CreateNtupleSColumn("CreatorProcess");
    manager->FinishNtuple();

    // Create ID 2 Ntuple for Brem Backing Emission
    manager->CreateNtuple("Backing","Bremsstrahlung Beam from Backing");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->FinishNtuple();
  }

  else
  {
    // Create ID 1 Ntuple for Exiting Chopper Data
    manager->CreateNtuple("ChopOut", "Chopper Wheel Exiting Radiation Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 2 Ntuple for NRF Materials
    manager->CreateNtuple("NRF","NRF Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("Material");
    manager->CreateNtupleDColumn("ZPos");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->CreateNtupleIColumn("Seed");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 3 NTuple for Incident Interrogation Object Information
    manager->CreateNtuple("IntObjIn","Incident Interrogation Object Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("BeamEnergy");
    manager->CreateNtupleSColumn("CreatorProcess");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->CreateNtupleDColumn("Time");
    manager->CreateNtupleIColumn("Seed");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 4 Ntuple for IntObj Emission Data
    manager->CreateNtuple("IntObjOut","Interrogation Object Emission Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("BeamEnergy");
    manager->CreateNtupleSColumn("CreatorProcess");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->CreateNtupleDColumn("Time");
    manager->CreateNtupleIColumn("Seed");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 5 Ntuple for Incident Shielding Layer Data
    manager->CreateNtuple("Shielding","Incident Outer Shielding Layer");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("Seed");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("BeamEnergy");
    manager->CreateNtupleDColumn("Time");
    manager->CreateNtupleSColumn("CreatorProcess");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 6 Ntuple for Incident Plexiglass Layer (Prior to Water)
    manager->CreateNtuple("Plexiglass", "Incident Plexiglass Layer");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("Seed");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("BeamEnergy");
    manager->CreateNtupleDColumn("Time");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->CreateNtupleSColumn("CreatorProcess");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 7 Ntuple for Incident Water Tank Data
    // Only Run with smaller runs this ntuple can get quite large
    manager->CreateNtuple("Water","Incident Water Tank Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleIColumn("TrackID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleSColumn("CreatorProcess");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 8 Ntuple for cherenkov in water tracking Creation Event
    manager->CreateNtuple("Cherenkov","Optical Photons Scinitllation and Cherenkov in Water Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleIColumn("NumSecondaries");
    manager->CreateNtupleDColumn("Time");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 9 Ntuple for Cherenkov in water tracking optical photons
    manager->CreateNtuple("Cherenkov2","Cherenkov in Water Optical Photon Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("Phi");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 10 Ntuple for Detected Information
    manager->CreateNtuple("DetInfo","Detected Information");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("BeamEnergy");
    manager->CreateNtupleDColumn("X");
    manager->CreateNtupleDColumn("Y");
    manager->CreateNtupleSColumn("CreatorProcess");
    manager->CreateNtupleDColumn("Time");
    manager->CreateNtupleIColumn("Seed");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    // Create ID 11 Ntuple for Detector Process Information
    manager->CreateNtuple("IncDetInfo","Incident Detector Process Information");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("BeamEnergy");
    manager->CreateNtupleSColumn("DetProcess");
    manager->CreateNtupleIColumn("Seed");

    if(WEIGHTED)
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
