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

#ifndef EventActionWResponseFunction_h
#define EventActionWResponseFunction_h 1

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
#include "DetectorResponseFunction.hh"

class G4Event;
class EventMessenger;

class EventActionWResponseFunction : public G4UserEventAction
{
public:
EventActionWResponseFunction();
~EventActionWResponseFunction();

public:
void BeginOfEventAction(const G4Event*);
void EndOfEventAction(const G4Event*);

void SetEventInfoFreq(G4int freq){eventInfoFreq = freq;}
void SetIncidentEnergy(G4double e){incident_energy = e;}

private:
G4int eventInfoFreq, runID;
G4int eventID;
G4double runTime, prevRunTime, eventsPerSec, totalEventsToRun, timeToFinish;
G4bool WEIGHTED;
EventMessenger* eventM;
G4double incident_energy;

};

#endif
