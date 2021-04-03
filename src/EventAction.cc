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
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "EventAction.hh"
#include <ctime>

extern G4bool debug;
extern G4bool printEvents;
extern G4bool bremTest;
extern G4String inFile;

EventAction::EventAction()
:eventInfoFreq(100000), runID(0),runTime(0.), prevRunTime(0.), eventsPerSec(0.),
totalEventsToRun(0.), timeToFinish(0.), eventM(NULL)
{
  eventM = new EventMessenger(this);
}

EventAction::~EventAction()
{
  delete eventM;
}

void EventAction::BeginOfEventAction(const G4Event* anEvent)
{
    if(debug)
        std::cout << "EventAction::BeginOfEventAction -> Beginning" << std::endl;

    G4int event = anEvent->GetEventID();
    if(event == 0)
    {
      std::cout << "Tracking Events... " << std::endl;
      G4cout << "Tracking Events: " << G4endl;
      totalEventsToRun = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    }
    else if(event % eventInfoFreq == 0)
    {
      G4RunManager *runMgr = G4RunManager::GetRunManager();
      if(runMgr->GetCurrentRun()->GetRunID()!=runID)
      {
        prevRunTime = clock()*1.0/CLOCKS_PER_SEC;
        runID++;
      }

      // Calculate the rate [particles tracked / s] and the estimated
      // time to completion of the present run [m,s]
      runTime = clock()*1.0/CLOCKS_PER_SEC - prevRunTime;
      eventsPerSec = event*1.0/runTime;  // [s]
      timeToFinish = (totalEventsToRun-event)/eventsPerSec; // [s]

      // Output the event variables in scientific notation using
      // std::stringstreams to avoid screwing up G4cout formatting
      std::stringstream eventSS;
      eventSS.precision(3);
      eventSS << std::scientific << (double)event;
      std::stringstream tEventSS;
      tEventSS.precision(3);
      tEventSS << std::scientific << totalEventsToRun;
      if(printEvents)
      {
        std::cout << "\r**  Event [" << eventSS.str() << "/" << tEventSS.str() << "]    "
                  << std::setprecision(4) << "Rate [" << eventsPerSec << "]    "
                  << std::setprecision(2) << "Time2Finish ["
                  << ((int)timeToFinish)/3600  << "h "
                  << ((int)timeToFinish%3600)/60 << "m "
                  << ((int)timeToFinish%3600)%60 << "s]"
                  << std::setprecision(6) << std::flush;
      }

      G4cout << "\r**  Event [" << eventSS.str() << "/" << tEventSS.str() << "]    "
                << std::setprecision(4) << "Rate [" << eventsPerSec << "]    "
                << std::setprecision(2) << "Time2Finish ["
                << ((int)timeToFinish)/3600  << "h "
                << ((int)timeToFinish%3600)/60 << "m "
                << ((int)timeToFinish%3600)%60 << "s]"
                << std::setprecision(6) << std::flush;
    }

    c_secondaries = 0;
    energyv.clear();
    timev.clear();

    if(debug)
        std::cout << "EventAction::BeginOfEventAction -> Ending" << std::endl;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    if(debug)
        std::cout << "EventAction::EndOfEventAction -> Beginning" << std::endl;

    if(c_secondaries > 0)
    {
      // Grab Max Energy
      G4double maxE = *std::max_element(energyv.begin(),energyv.end());
      // Find Max Energy's Weight
      eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
      G4double weight = info->GetWeight();
      // Find the Average Time
      G4double c_time;
      if(timev.size() > 0)
      {
              c_time = calcAvg();
      }
      else
              c_time = 0;
      // Fill the TTree
      G4AnalysisManager* manager = G4AnalysisManager::Instance();
      manager->FillNtupleIColumn(7,0,anEvent->GetEventID());
      manager->FillNtupleDColumn(7,1,maxE);
      manager->FillNtupleIColumn(7,2,c_secondaries);
      manager->FillNtupleDColumn(7,3,c_time);
      if(!inFile.compare(0,24,"brems_distributions.root"))
        manager->FillNtupleDColumn(7,4, weight);
      manager->AddNtupleRow(7);
    }

    if(debug)
        std::cout << "EventAction::EndOfEventAction() --> Ending!" << std::endl;
}
