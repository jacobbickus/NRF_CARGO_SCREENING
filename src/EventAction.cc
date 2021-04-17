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
extern G4bool detTest;
extern G4String inFile;
extern G4long seed;

EventAction::EventAction()
:eventInfoFreq(100000), runID(0),runTime(0.), prevRunTime(0.), eventsPerSec(0.),
totalEventsToRun(0.), timeToFinish(0.), WEIGHTED(false), eventM(NULL)
{
  eventM = new EventMessenger(this);
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;
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

    // Reset values
    number_detected = 0;
    energy_counter = 0;
    s_detected = 0;
    c_detected = 0;
    incident_energy = 0.;
    s_secondaries = 0;
    c_secondaries = 0;
    scintillation_energyv.clear();
    cherenkov_energyv.clear();

    if(debug)
        std::cout << "EventAction::BeginOfEventAction -> Ending" << std::endl;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    if(debug)
        std::cout << "EventAction::EndOfEventAction -> Beginning" << std::endl;

    eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    G4double weight = info->GetWeight();
    G4AnalysisManager* manager = G4AnalysisManager::Instance();

    // Deal With Scintillation per Event
    if(s_secondaries > 0)
    {
      // Grab Max Energy
      G4double maxE = *std::max_element(scintillation_energyv.begin(), scintillation_energyv.end());

      // Fill the Tree
      if(detTest)
      {
        manager->FillNtupleIColumn(2,0,anEvent->GetEventID());
        manager->FillNtupleDColumn(2,1,maxE);
        manager->FillNtupleIColumn(2,2,s_secondaries);

        if(WEIGHTED)
          manager->FillNtupleDColumn(2,3,weight);

        manager->AddNtupleRow(2);
      }
      else
      {
        manager->FillNtupleIColumn(8,0,anEvent->GetEventID());
        manager->FillNtupleDColumn(8,1,maxE);
        manager->FillNtupleIColumn(8,2,s_secondaries);

        if(WEIGHTED)
          manager->FillNtupleDColumn(8,3,weight);

        manager->AddNtupleRow(8);
      }
    }

    // Deal With Cherenkov per Event
    if(c_secondaries > 0)
    {
      // Grab Max Energy
      G4double maxE = *std::max_element(cherenkov_energyv.begin(),cherenkov_energyv.end());

      // Fill the TTree
      if(detTest)
      {
        manager->FillNtupleIColumn(4,0,anEvent->GetEventID());
        manager->FillNtupleDColumn(4,1,maxE);
        manager->FillNtupleIColumn(4,2,c_secondaries);

        if(WEIGHTED)
          manager->FillNtupleDColumn(4,3, weight);

        manager->AddNtupleRow(4);
      }
      else
      {
        manager->FillNtupleIColumn(10,0,anEvent->GetEventID());
        manager->FillNtupleDColumn(10,1,maxE);
        manager->FillNtupleIColumn(10,2,c_secondaries);

        if(WEIGHTED)
          manager->FillNtupleDColumn(10,3, weight);

        manager->AddNtupleRow(10);
      }
    }

    // Deal with Detected per Event
    if(detTest)
    {
      if(incident_energy > 0)
      {
        manager->FillNtupleIColumn(7,0, anEvent->GetEventID());
        manager->FillNtupleDColumn(7,1, incident_energy/(MeV));
        manager->FillNtupleIColumn(7,2, number_detected);
        manager->FillNtupleIColumn(7,3, s_detected);
        manager->FillNtupleIColumn(7,4, c_detected);
        manager->FillNtupleIColumn(7,5, seed);

        if(WEIGHTED)
          manager->FillNtupleDColumn(7,6, weight);

        manager->AddNtupleRow(7);
      }
    }

    if(debug)
        std::cout << "EventAction::EndOfEventAction() --> Ending!" << std::endl;
}
