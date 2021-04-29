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

extern G4bool debug;
extern G4bool printEvents;
extern G4String inFile;

EventActionWResponseFunction::EventActionWResponseFunction()
: G4UserEventAction(), BaseEventAction()
{
  if(debug)
    std::cout << "EventActionWResponseFunction::EventActionWResponseFunction Initialized." << std::endl;
}

EventActionWResponseFunction::~EventActionWResponseFunction()
{}

void EventActionWResponseFunction::BeginOfEventAction(const G4Event* anEvent)
{
  eventID = anEvent->GetEventID();
  if(debug && eventID == 0)
      std::cout << "EventActionWResponseFunction::BeginOfEventAction -> Beginning" << std::endl;

  EventActionPrint();
  incident_energy = 0.;
  energy_counter = 0;

  if(debug && eventID == 0)
    std::cout << "EventActionWResponseFunction::BeginOfEventActionWResponseFunction -> Ending" << std::endl;
}

void EventActionWResponseFunction::EndOfEventAction(const G4Event* anEvent)
{
  if(debug && eventID == 0)
      std::cout << "EventActionWResponseFunction::EndOfEventActionWResponseFunction -> Beginning" << std::endl;

  if(incident_energy > 0.00001)
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
