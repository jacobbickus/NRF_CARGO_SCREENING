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
extern G4bool detTest;
extern G4bool debug;
extern G4String inFile;
extern G4bool ResponseFunction;

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
  int tuple_counter = 0;
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  manager->SetVerboseLevel(0);
  // open output file
  G4bool fileOpen = manager->OpenFile(gOutName);

  if(!fileOpen)
  {
    G4cerr << "HistoManager::Book(): Cannot Open " <<manager->GetFileName()<<G4endl;
    return;
  }

  if(!detTest)
  {
    // Create ID 0 Ntuple for Incident Chopper Data
    manager->CreateNtuple("ChopIn", "Chopper Wheel Incident Data");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("X");
    manager->CreateNtupleDColumn("Y");

    if(WEIGHTED)
      manager->CreateNtupleDColumn("Weight");

    manager->FinishNtuple();

    if(debug)
      G4cout << "HistoManager::Book -> Ntuple Incident Chopper Data Number: " << tuple_counter << G4endl;

    tuple_counter++;
  }

  if(bremTest)
  {
    // Create ID 1 Ntuple for Brem Radiator Emission
    manager->CreateNtuple("Radiator","Bremsstrahlung Beam from Radiator");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");

    manager->FinishNtuple();

    if(debug)
      G4cout << "HistoManager::Book -> Ntuple Brem Radiator Emission Number: " << tuple_counter << G4endl;

    tuple_counter++;


    // Create ID 2 Ntuple for Brem Backing Emission
    manager->CreateNtuple("Backing","Bremsstrahlung Beam from Backing");
    manager->CreateNtupleIColumn("EventID");
    manager->CreateNtupleDColumn("Energy");
    manager->CreateNtupleDColumn("Theta");
    manager->CreateNtupleDColumn("Phi");
    manager->FinishNtuple();

    if(debug)
      G4cout << "HistoManager::Book -> Ntuple Bremmsstrahlung Beam From Backing: " << tuple_counter << G4endl;

    tuple_counter++;
  }

  else
  {
    if(!detTest)
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

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Chopper Emission: " << tuple_counter << G4endl;

      tuple_counter++;

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

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple NRF: " << tuple_counter << G4endl;

      tuple_counter++;

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

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Incident Interrogation Object: " << tuple_counter << G4endl;

      tuple_counter++;

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

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Interrogation Object Emission: " << tuple_counter << G4endl;

      tuple_counter++;
    }

    // Create ID 5/0 Ntuple for Incident Shielding Layer Data
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

    if(debug)
      G4cout << "HistoManager::Book -> Ntuple Shielding: " << tuple_counter << G4endl;

    tuple_counter++;

    // Create ID 6/1 Ntuple for Incident Plexiglass Layer (Prior to Water)
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

    if(debug)
      G4cout << "HistoManager::Book -> Ntuple Plexiglass: " << tuple_counter << G4endl;

    tuple_counter++;

    if(ResponseFunction)
    {
      // Create ID 7 Ntuple for WResponseFunction Run
      manager->CreateNtuple("DetInfo","Estimation of Detected Counts from Response Function");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleDColumn("IncidentEnergy");
      manager->CreateNtupleDColumn("BeamEnergy");
      manager->CreateNtupleDColumn("NumPE");
      manager->CreateNtupleDColumn("NumScintillation");
      manager->CreateNtupleDColumn("NumCherenkov");

      if(WEIGHTED)
        manager->CreateNtupleDColumn("Weight");

      manager->FinishNtuple();
    }

    // Create ID 7 Ntuple for Incident Water Tank Data
    // Only Run with smaller runs this ntuple can get quite large
    if(!detTest && !ResponseFunction)
    {
      manager->CreateNtuple("Water","Incident Water Tank Data");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleIColumn("TrackID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleSColumn("CreatorProcess");

      if(WEIGHTED)
        manager->CreateNtupleDColumn("Weight");

      manager->FinishNtuple();

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Incident Water Tank: " << tuple_counter << G4endl;

      tuple_counter++;
    }

    if(!ResponseFunctionTest)
    {
      // Create ID 8/2 Ntuple for scinitllation in water per EVENT
      manager->CreateNtuple("Scintillation","Scintillation per Event Water Data");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleIColumn("NumSecondaries");

      if(WEIGHTED)
        manager->CreateNtupleDColumn("Weight");

      manager->FinishNtuple();

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Scintillation Per Event Water Data: " << tuple_counter << G4endl;

      tuple_counter++;

      // Create ID 9/3 Ntuple for Scintillation in water tracking
      manager->CreateNtuple("Scintillation2","Scintillation in Water Optical Photon Data");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("Phi");
      manager->CreateNtupleDColumn("Theta");

      if(WEIGHTED)
        manager->CreateNtupleDColumn("Weight");

      manager->FinishNtuple();

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Scintillation in Water Optical Photon Data: " << tuple_counter << G4endl;

      tuple_counter++;

      // Create ID 10/4 Ntuple for cherenkov in water per EVENT
      manager->CreateNtuple("Cherenkov","Cherenkov per Event Water Data");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleIColumn("NumSecondaries");

      if(WEIGHTED)
        manager->CreateNtupleDColumn("Weight");

      manager->FinishNtuple();

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Cherenkov Per Event Water Data: " << tuple_counter << G4endl;

      tuple_counter++;

      // Create ID 11/5 Ntuple for Cherenkov in water tracking optical photons
      manager->CreateNtuple("Cherenkov2","Cherenkov in Water Optical Photon Data");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("Phi");

      if(WEIGHTED)
        manager->CreateNtupleDColumn("Weight");

      manager->FinishNtuple();

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Cherenkov in Water Optical Photon Data: " << tuple_counter << G4endl;

      tuple_counter++;

      // Create ID 12/6 Ntuple for Detected Information
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

      if(debug)
        G4cout << "HistoManager::Book -> Ntuple Detected Information: " << tuple_counter << G4endl;

      tuple_counter++;

      // Ntuple ID 7 for DetTest
      if(detTest)
      {
        manager->CreateNtuple("DetResponse","Detector Response Function");
        manager->CreateNtupleIColumn("EventID");
        manager->CreateNtupleDColumn("IncidentEnergy");
        manager->CreateNtupleIColumn("NumPE");
        manager->CreateNtupleIColumn("NumScintillation");
        manager->CreateNtupleIColumn("NumCherenkov");
        manager->CreateNtupleIColumn("Seed");

        manager->FinishNtuple();

        if(debug)
          G4cout << "HistoManager::Book -> Ntuple Detector Response Function: " << tuple_counter << G4endl;

        tuple_counter++;

      }

      if(!detTest)
      {
        // Create ID 13 Ntuple for Detector Process Information
        manager->CreateNtuple("IncDetInfo","Incident Detector Process Information");
        manager->CreateNtupleIColumn("EventID");
        manager->CreateNtupleDColumn("Energy");
        manager->CreateNtupleDColumn("BeamEnergy");
        manager->CreateNtupleSColumn("DetProcess");
        manager->CreateNtupleIColumn("Seed");

        if(WEIGHTED)
          manager->CreateNtupleDColumn("Weight");

        manager->FinishNtuple();

        if(debug)
          G4cout << "HistoManager::Book -> Ntuple Incident Detector Process Information: " << tuple_counter << G4endl;

        tuple_counter++;
      }

    }
  }

  fFactoryOn = true;

  if(debug)
    std::cout << "HistoManager::Book() --> Complete!" << std::endl;

  if(debug)
    G4cout << "HistoManager::Book -> TOTAL NTUPLES: " << tuple_counter << G4endl;

}// end of HistoManager::Book

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
