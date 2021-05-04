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

#ifndef BaseEventAction_h
#define BaseEventAction_h 1

#include "globals.hh"
#include "G4Types.hh"
#include "G4EventManager.hh"
#include "EventInformation.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include "EventMessenger.hh"
#include "DetectorResponseFunction.hh"

class G4Event;
class EventMessenger;

class BaseEventAction
{
public:
  BaseEventAction();
  ~BaseEventAction();

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

protected:
  void EventActionPrint();
  void FillDetectorResponse();
  void FillScintillationPerEvent(G4double);
  void FillCherenkovPerEvent(G4double);

  G4AnalysisManager* manager;
  G4int eventInfoFreq, runID;
  G4int eventID;
  G4double runTime, prevRunTime, eventsPerSec, totalEventsToRun, timeToFinish;
  EventMessenger* eventM;
  G4int c_secondaries, s_secondaries, s_detected, c_detected, number_detected;
  G4double incident_energy;
  G4int energy_counter;
  std::vector<double> cherenkov_energyv, scintillation_energyv;
};

#endif
