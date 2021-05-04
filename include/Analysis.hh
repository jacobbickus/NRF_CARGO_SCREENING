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

#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"
#include "g4root.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "time.h"


class Analysis
{
public:
Analysis();
~Analysis();

void finish();     // close root file
void Book();

private:
void IncidentChopper(G4bool Weight=false)
{
  manager->CreateNtuple("ChopIn", "Chopper Wheel Incident Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("X");
  manager->CreateNtupleDColumn("Y");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void EmissionChopper(G4bool Weight=false)
{
  // Create ID 1 Ntuple for Exiting Chopper Data
  manager->CreateNtuple("ChopOut", "Chopper Wheel Exiting Radiation Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Theta");
  manager->CreateNtupleDColumn("Phi");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void NRF(G4bool Weight=false)
{
  // Create ID 2 Ntuple for NRF Materials
  manager->CreateNtuple("NRF","NRF Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleSColumn("Material");
  manager->CreateNtupleDColumn("ZPos");
  manager->CreateNtupleDColumn("Theta");
  manager->CreateNtupleDColumn("Phi");
  manager->CreateNtupleIColumn("Seed");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void IncidentIntObj(G4bool Weight=false)
{
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

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void EmissionIntObj(G4bool Weight=false)
{
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

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void IncidentShielding(G4bool Weight=false)
{
  manager->CreateNtuple("Shielding","Incident Outer Shielding Layer");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleIColumn("Seed");
  manager->CreateNtupleIColumn("TrackID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("BeamEnergy");
  manager->CreateNtupleDColumn("Time");
  manager->CreateNtupleSColumn("CreatorProcess");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void IncidentPlexiglass(G4bool Weight=false)
{
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

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void IncidentWater(G4bool Weight=false)
{
  manager->CreateNtuple("Water","Incident Water Tank Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleIColumn("TrackID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleSColumn("CreatorProcess");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void ScintillationPerEvent(G4bool Weight=false)
{
  manager->CreateNtuple("Scintillation","Scintillation per Event Water Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleIColumn("NumSecondaries");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void SteppingScintillation(G4bool Weight=false)
{
  manager->CreateNtuple("Scintillation2","Scintillation in Water Optical Photon Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Phi");
  manager->CreateNtupleDColumn("Theta");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void CherenkovPerEvent(G4bool Weight=false)
{
  manager->CreateNtuple("Cherenkov","Cherenkov per Event Water Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleIColumn("NumSecondaries");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void SteppingCherenkov(G4bool Weight=false)
{
  manager->CreateNtuple("Cherenkov2","Cherenkov in Water Optical Photon Data");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Phi");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void PCDetection(G4bool Weight=false)
{
  manager->CreateNtuple("DetInfo","Detected Information");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("BeamEnergy");
  manager->CreateNtupleSColumn("CreatorProcess");
  manager->CreateNtupleDColumn("Time");
  manager->CreateNtupleIColumn("Seed");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void IncidentPC(G4bool Weight=false)
{
  manager->CreateNtuple("IncDetInfo","Incident Detector Process Information");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("BeamEnergy");
  manager->CreateNtupleSColumn("DetProcess");
  manager->CreateNtupleIColumn("Seed");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void BremBacking()
{
  manager->CreateNtuple("Backing","Bremsstrahlung Beam from Backing");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Theta");
  manager->CreateNtupleDColumn("Phi");
  manager->FinishNtuple();
}

void BremRadiator()
{
  manager->CreateNtuple("Radiator","Bremsstrahlung Beam from Radiator");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Theta");
  manager->CreateNtupleDColumn("Phi");
  manager->FinishNtuple();
}

void DetectorEstimatedInfo(G4bool Weight=false)
{
  manager->CreateNtuple("DetInfo","Estimation of Detected Counts from Response Function");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("IncidentEnergy");
  manager->CreateNtupleDColumn("BeamEnergy");
  manager->CreateNtupleDColumn("NumPE");
  manager->CreateNtupleDColumn("NumPE2");
  manager->CreateNtupleDColumn("NumScintillation");
  manager->CreateNtupleDColumn("NumCherenkov");

  if(Weight)
    manager->CreateNtupleDColumn("Weight");

  manager->FinishNtuple();
}

void DetectorResponse()
{
  manager->CreateNtuple("DetResponse","Detector Response Function");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleDColumn("IncidentEnergy");
  manager->CreateNtupleIColumn("NumPE");
  manager->CreateNtupleIColumn("NumScintillation");
  manager->CreateNtupleIColumn("NumCherenkov");
  manager->FinishNtuple();
}

private:
G4bool fFactoryOn;
G4int start_time, stop_time;
G4AnalysisManager* manager;
};

#endif
