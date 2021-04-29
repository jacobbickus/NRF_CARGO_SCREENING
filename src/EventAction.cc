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

extern G4bool debug;
extern G4bool detTest;

EventAction::EventAction()
: G4UserEventAction(), BaseEventAction()
{
  if(debug)
    std::cout << "EventAction::EventAction Initialized." << std::endl;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* anEvent)
{
  eventID = anEvent->GetEventID();

  if(debug && eventID == 0)
      std::cout << "EventAction::BeginOfEventAction -> Beginning" << std::endl;

  EventActionPrint();
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

  if(debug && eventID == 0)
      std::cout << "EventAction::BeginOfEventAction -> Ending" << std::endl;
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    if(debug && eventID == 0)
        std::cout << "EventAction::EndOfEventAction -> Beginning" << std::endl;

    eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    G4double weight = info->GetWeight();
    G4AnalysisManager* manager = G4AnalysisManager::Instance();

    // Deal with Detected per Event
    if(detTest)
    {
      FillDetectorResponse();
    }
    else
    {
      FillScintillationPerEvent(weight);
      FillCherenkovPerEvent(weight);
    }

    if(debug && eventID == 0)
        std::cout << "EventAction::EndOfEventAction() --> Ending!" << std::endl;
}
