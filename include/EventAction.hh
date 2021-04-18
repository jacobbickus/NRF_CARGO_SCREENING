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

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Types.hh"
#include "G4EventManager.hh"
#include "eventInformation.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include "EventMessenger.hh"

class G4Event;
class EventMessenger;

class EventAction : public G4UserEventAction
{
public:
EventAction();
~EventAction();

public:
void BeginOfEventAction(const G4Event*);
void EndOfEventAction(const G4Event*);

void ScintillationEnergy(G4double energy){scintillation_energyv.push_back(energy);}
void CherenkovEnergy(G4double energy){cherenkov_energyv.push_back(energy);}
void ScintillationAddSecondary(){s_secondaries++;}
void CherenkovAddSecondary(){c_secondaries++;}
void SetEventInfoFreq(G4int freq){eventInfoFreq = freq;}
void AddDetectedScintillation(){s_detected++;}
void AddDetectedCherenkov(){c_detected++;}
void AddDetected(){number_detected++;}
void SetIncidentEnergy(G4double e)
{
  if(energy_counter == 0)
  {
    incident_energy = e;
    energy_counter++;
  }
}
private:

void FillDetectorResponse()
{
  if(incident_energy > 0)
  {
    manager->FillNtupleIColumn(1,0, eventID);
    manager->FillNtupleDColumn(1,1, incident_energy/(MeV));
    manager->FillNtupleIColumn(1,2, number_detected);
    manager->FillNtupleIColumn(1,3, s_detected);
    manager->FillNtupleIColumn(1,4, c_detected);
    manager->FillNtupleIColumn(1,5, seed);
    manager->AddNtupleRow(1);
  }
}

void FillScintillationPerEvent(G4double weight=0.)
{
  if(s_secondaries > 0)
  {
    // Grab Max Energy
    G4double maxE = *std::max_element(scintillation_energyv.begin(), scintillation_energyv.end());

    manager->FillNtupleIColumn(8,0,eventID);
    manager->FillNtupleDColumn(8,1,maxE);
    manager->FillNtupleIColumn(8,2,s_secondaries);

    if(WEIGHTED)
      manager->FillNtupleDColumn(8,3,weight);

    manager->AddNtupleRow(8);
  }
}

void FillCherenkovPerEvent(G4double weight=0.)
{
  if(c_secondaries > 0)
  {
    // Grab Max Energy
    G4double maxE = *std::max_element(cherenkov_energyv.begin(),cherenkov_energyv.end());

    manager->FillNtupleIColumn(10,0,anEvent->GetEventID());
    manager->FillNtupleDColumn(10,1,maxE);
    manager->FillNtupleIColumn(10,2,c_secondaries);

    if(WEIGHTED)
      manager->FillNtupleDColumn(10,3, weight);

    manager->AddNtupleRow(10);
  }
}

private:
G4int eventInfoFreq, runID;
G4int eventID;
G4double runTime, prevRunTime, eventsPerSec, totalEventsToRun, timeToFinish;
G4bool WEIGHTED;
EventMessenger* eventM;
G4int c_secondaries, s_secondaries, s_detected, c_detected, number_detected;
G4double incident_energy;
G4int energy_counter;
std::vector<double> cherenkov_energyv, scintillation_energyv;
};

#endif
