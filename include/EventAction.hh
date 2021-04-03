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
#include "HistoManager.hh"
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

void CherenkovEnergy(G4double energy)
{
  energyv.push_back(energy);
}
void CherenkovSecondaries(G4int secondaries)
{
  c_secondaries += secondaries;
}
void CherenkovTime(G4double times)
{
  timev.push_back(times);
}

void SetEventInfoFreq(G4int freq){eventInfoFreq = freq;}

private:

G4double calcAvg()
{
  for(std::size_t i=0;i<timev.size();++i)
  {
    sum += timev.at(i);
  }
  return sum/timev.size();
}
G4int eventInfoFreq, runID;
G4double runTime, prevRunTime, eventsPerSec, totalEventsToRun, timeToFinish;
EventMessenger* eventM;
G4int c_secondaries;
G4double sum;
std::vector<double> energyv, timev;
};

#endif
