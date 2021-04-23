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

#include "EventActionWResponseFunction.hh"
#include <ctime>

extern G4bool debug;
extern G4bool printEvents;
extern G4String inFile;

EventActionWResponseFunction::EventActionWResponseFunction()
:eventInfoFreq(100000), runID(0),runTime(0.), prevRunTime(0.), eventsPerSec(0.),
totalEventsToRun(0.), timeToFinish(0.), WEIGHTED(false), eventM(NULL)
{
  eventM = new EventMessenger(this);
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;
}

EventActionWResponseFunction::~EventActionWResponseFunction()
{
  delete eventM;
}

void EventActionWResponseFunction::BeginOfEventAction(const G4Event* anEvent)
{
  eventID = anEvent->GetEventID();
  if(debug && eventID == 0)
      std::cout << "EventActionWResponseFunction::BeginOfEventAction -> Beginning" << std::endl;

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

  incident_energy = 0.;

  if(debug && eventID == 0)
    std::cout << "EventActionWResponseFunction::BeginOfEventActionWResponseFunction -> Ending" << std::endl;
}

void EventActionWResponseFunction::EndOfEventAction(const G4Event* anEvent)
{
  if(debug && eventID == 0)
      std::cout << "EventActionWResponseFunction::EndOfEventActionWResponseFunction -> Beginning" << std::endl;

  if(incident_energy > 0.)
  {
    eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    G4double beam_energy = info->GetBeamEnergy();
    G4double weight = info->GetWeight();
    DetectorResponseFunction *r_function = DetectorResponseFunction::Instance();
    G4double numPE = r_function->GetDetectorPhotoelectrons(incident_energy);
    G4double numPE2 = r_function->GetProjectedPhotoelectrons(incident_energy);
    G4double numScint = r_function->GetScintillationResponse(incident_energy);
    G4double numCher = r_function->GetCherenkovResponse(incident_energy);

    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    manager->FillNtupleIColumn(5,0, eventID);
    manager->FillNtupleDColumn(5,1, incident_energy/(MeV));
    manager->FillNtupleDColumn(5,2, beam_energy);
    manager->FillNtupleDColumn(5,3, numPE);
    manager->FillNtupleDColumn(5,4, numPE2);
    manager->FillNtupleDColumn(5,5, numScint);
    manager->FillNtupleDColumn(5,6, numCher);

    if(WEIGHTED)
      manager->FillNtupleDColumn(5,7, weight);

    manager->AddNtupleRow(5);

  }

  if(debug && eventID == 0)
      std::cout << "EventActionWResponseFunction::EndOfEventActionWResponseFunction() --> Ending!" << std::endl;
}
