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

#include "BaseEventAction.hh"
#include <ctime>

extern G4bool debug;
extern G4bool printEvents;
extern G4String inFile;

BaseEventAction::BaseEventAction()
:eventInfoFreq(100000), runID(0),runTime(0.), prevRunTime(0.), eventsPerSec(0.),
totalEventsToRun(0.), timeToFinish(0.), WEIGHTED(false), eventM(NULL)
{
  if(debug)
    std::cout << "BaseEventAction::BaseEventAction Initialized." << std::endl;

  eventM = new EventMessenger(this);
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;

  manager = G4AnalysisManager::Instance();
}

BaseEventAction::~BaseEventAction()
{
  delete eventM;
}

void BaseEventAction::EventActionPrint()
{
  if(eventID == 0)
  {
    std::cout << "Tracking Events... " << std::endl;
    G4cout << "Tracking Events: " << G4endl;
    totalEventsToRun = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
  }
  else if(eventID % eventInfoFreq == 0)
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
    eventsPerSec = eventID*1.0/runTime;  // [s]
    timeToFinish = (totalEventsToRun-eventID)/eventsPerSec; // [s]

    // Output the event variables in scientific notation using
    // std::stringstreams to avoid screwing up G4cout formatting
    std::stringstream eventSS;
    eventSS.precision(3);
    eventSS << std::scientific << (double)eventID;
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
  } // end else if eventID %
} // end of Print function

void BaseEventAction::FillDetectorResponse()
{
  if(incident_energy > 0)
  {
    manager->FillNtupleIColumn(2,0, eventID);
    manager->FillNtupleDColumn(2,1, incident_energy/(MeV));
    manager->FillNtupleIColumn(2,2, number_detected);
    manager->FillNtupleIColumn(2,3, s_detected);
    manager->FillNtupleIColumn(2,4, c_detected);
    manager->AddNtupleRow(2);
  }
}

void BaseEventAction::FillScintillationPerEvent(G4double weight=1.)
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

void BaseEventAction::FillCherenkovPerEvent(G4double weight=1.)
{
  if(c_secondaries > 0)
  {
    // Grab Max Energy
    G4double maxE = *std::max_element(cherenkov_energyv.begin(),cherenkov_energyv.end());

    manager->FillNtupleIColumn(10,0, eventID);
    manager->FillNtupleDColumn(10,1,maxE);
    manager->FillNtupleIColumn(10,2,c_secondaries);

    if(WEIGHTED)
      manager->FillNtupleDColumn(10,3, weight);

    manager->AddNtupleRow(10);
  }
}
