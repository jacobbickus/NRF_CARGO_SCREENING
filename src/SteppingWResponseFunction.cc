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

#include "SteppingWResponseFunction.hh"

extern G4bool debug;
extern G4bool addNRF;
extern G4long seed;
extern G4bool output;

SteppingWResponseFunction::SteppingWResponseFunction(EventActionWResponseFunction* event)
: G4UserSteppingAction(), BaseSteppingAction(), kevent(event)
{
}

SteppingWResponseFunction::~SteppingWResponseFunction()
{
}

void SteppingWResponseFunction::UserSteppingAction(const G4Step* aStep)
{
  //std::cout << "SteppingWResponseFunction::UserSteppingAction -> HERE." << std::endl;
  if(!output)
    return;

  G4StepPoint* endPoint   = aStep->GetPostStepPoint();
  G4StepPoint* startPoint = aStep->GetPreStepPoint();
  G4Track* theTrack       = aStep->GetTrack();

    // Run Logical Checks
    if(endPoint == NULL)
    {
      return; // at the end of the world
    }
    else if(endPoint->GetPhysicalVolume()==NULL)
    {
      return;
    }

    // Grab Relevant event information including the particle weight
    eventInformation* info =
              (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    beamEnergy = info->GetBeamEnergy()/(MeV);

    if(WEIGHTED)
      weight = info->GetWeight();

    nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
    previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();
    // kill photons past IntObj
    G4double EndIntObj = kdet->getEndIntObj();

    if(theTrack->GetPosition().z() > EndIntObj)
    {
      // kill photons that go beyond the interrogation object
      theTrack->SetTrackStatus(fStopAndKill);
      krun->AddStatusKilledPosition();
      return;
    }


    if(nextStep_VolumeName.compare(0, 3, "Col") == 0)
    {
      // kill photons in collimator
      theTrack->SetTrackStatus(fStopAndKill);
      krun->AddStatusKilledPosition();
      return;
    }

    // Run Time Cut
    if(theTrack->GetGlobalTime() > 250) // cut placed on particles time greater than 250 ns
    {
      theTrack->SetTrackStatus(fStopAndKill);
      krun->AddStatusKilledTime();
      return;
    }

    eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    trackID = theTrack->GetTrackID();
    energy = theTrack->GetKineticEnergy()/(MeV);
// ************************************************* Checks and Cuts Complete ************************************************** //

    CPName = "beam";
    if(theTrack->GetCreatorProcess() !=0)
      CPName = theTrack->GetCreatorProcess()->GetProcessName();

    G4ThreeVector p = startPoint->GetMomentum();
    theta = std::asin(std::sqrt(std::pow(p.x(),2)+std::pow(p.y(),2))/p.mag());
    phi = std::asin(p.y()/p.mag());
    G4ThreeVector loc = theTrack->GetPosition();
// **************************************************** Track NRF Materials **************************************************** //

    const G4VProcess* process = endPoint->GetProcessDefinedStep();

    if(addNRF)
    {
      // Keep track of Any NRF Created
      if(drawNRFDataFlag)
      {
        if(process->GetProcessName() == "NRF")
        {
          krun->AddNRF();
          const G4TrackVector* emitted_nrf = aStep->GetSecondary();
          FillNRF(0, loc.z(), emitted_nrf);
        } // end of process->GetProcessName() == "NRF"
      } // end of if drawNRFDataFlag
    } // end of if addNRF

// *********************************************** Track Chopper Interactions **************************************************** //

    // Chopper Analysis

    // Gammas Exiting Chopper Wheel
    if(nextStep_VolumeName.compare("Chop") != 0
       && previousStep_VolumeName.compare("Chop") == 0)
    {
      if(std::abs(theta) > 0.1)
      {
        theTrack->SetTrackStatus(fStopAndKill);
        krun->AddStatusKilledThetaAngle();
        return;
      }
      else if(std::abs(phi) > 0.1)
      {
        theTrack->SetTrackStatus(fStopAndKill);
        krun->AddStatusKilledPhiAngle();
        return;
      }
      else
        return;
    }

// *********************************************** Track Interrogation Object Interactions **************************************************** //

    // Incident Interrogation Object
    if(drawIntObjInDataFlag)
    {
      if(nextStep_VolumeName.compare("IntObj") == 0
         && previousStep_VolumeName.compare("IntObj") != 0)
      {
        FillIntObjIn(1);
        return;
      }
    }

    // Exiting Interrogation Object
    if(nextStep_VolumeName.compare("IntObj") != 0
       && previousStep_VolumeName.compare("IntObj") == 0)
    {
      if(std::abs(phi) > 1)
      {
        theTrack->SetTrackStatus(fStopAndKill);
        krun->AddStatusKilledPhiAngle();
        return;
      }
      else
      {
        if(drawIntObjOutDataFlag)
        {
          FillIntObjOut(2);
          return;
        }// end if drawIntObjOutDataFlag
        else
          return;
      }// end else
    }// end if exiting Interrogation Object

// *********************************************** Track Shielding Interactions  **************************************************** //

    // Track particles incident shielding from world
    if(drawShieldingIncDataFlag)
    {
      if(nextStep_VolumeName.compare(0,5,"Atten") == 0
          && previousStep_VolumeName.compare("World") == 0)
      {
        FillShielding(3);
        return;
      }
    }

// *********************************************** Track Plexiglass Interactions **************************************************** //

    if(nextStep_VolumeName.compare(0,4,"Plex") == 0
        && previousStep_VolumeName.compare(0,4,"Last") == 0)
    {
      FillPlexi(4);
      kevent->SetIncidentEnergy(energy);
      krun->AddStatusKilledPosition();
      theTrack->SetTrackStatus(fStopAndKill);
      return;
    }

} // end of user stepping action function
