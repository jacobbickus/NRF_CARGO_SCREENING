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
extern G4String inFile;
extern G4bool addNRF;
extern G4long seed;
extern G4bool output;

SteppingWResponseFunction::SteppingWResponseFunction(EventActionWResponseFunction* event)
        : G4UserSteppingAction(), kevent(event),
        drawChopperIncDataFlag(0), drawChopperOutDataFlag(0), drawNRFDataFlag(0),
        drawIntObjInDataFlag(0), drawIntObjOutDataFlag(0), drawShieldingIncDataFlag(0),
        drawPlexiIncDataFlag(0), drawWaterIncDataFlag(0),
        drawScintillationDataFlag(0), drawScintillation2DataFlag(0),
        drawCherenkovDataFlag(0), drawCherenkov2DataFlag(0), drawDetDataFlag(0),
        WEIGHTED(false),
        stepM(NULL)
{
  stepM = new StepMessenger(this);
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;
}

SteppingWResponseFunction::~SteppingWResponseFunction()
{
  delete stepM;
}

void SteppingWResponseFunction::UserSteppingAction(const G4Step* aStep)
{
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
    G4double beamEnergy = info->GetBeamEnergy()/(MeV);

    if(WEIGHTED)
      weight = info->GetWeight();

    RunInformation* krun = RunInformation::Instance();
    DetectorInformation* kdet = DetectorInformation::Instance();

    G4String nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
    G4String previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();
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

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = theTrack->GetTrackID();
    G4double energy = theTrack->GetKineticEnergy()/(MeV);
// ************************************************* Checks and Cuts Complete ************************************************** //
    G4AnalysisManager* manager = G4AnalysisManager::Instance();

    G4String CPName = "beam";
    if(theTrack->GetCreatorProcess() !=0)
      CPName = theTrack->GetCreatorProcess()->GetProcessName();

    G4ThreeVector p = startPoint->GetMomentum();
    // sin(theta) = (vector magnitude in XY plane)/(total vector magnitude)
    // polar angle measured between the positive Z axis and the vector
    G4double theta = std::asin(std::sqrt(std::pow(p.x(),2)+std::pow(p.y(),2))/p.mag());
    // sin(phi) -> angle in the XY plane reference to the positive X axis
    G4double phi = std::asin(p.y()/p.mag());
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
          for(unsigned int i=0;i<emitted_nrf->size();++i)
          {
            if(emitted_nrf->at(i)->GetCreatorProcess()->GetProcessName() == "NRF")
            {
              manager->FillNtupleIColumn(0,0, eventID);
              manager->FillNtupleDColumn(0,1, energy);
              manager->FillNtupleSColumn(0,2, nextStep_VolumeName);
              manager->FillNtupleDColumn(0,3, loc.z()/(cm));
              G4ThreeVector p_nrf = emitted_nrf->at(i)->GetMomentum();
              G4double theta_nrf = std::asin(std::sqrt(std::pow(p_nrf.x(),2)+std::pow(p_nrf.y(),2))/p_nrf.mag());
              G4double phi_nrf = std::asin(p_nrf.y()/p_nrf.mag());
              manager->FillNtupleDColumn(0,4, theta_nrf);
              manager->FillNtupleDColumn(0,5, phi_nrf);
              manager->FillNtupleIColumn(0,6, seed);

              if(WEIGHTED)
                manager->FillNtupleDColumn(0,7,weight);

              manager->AddNtupleRow(0);
            } // end if emitted_nrf->at(i)->GetCreatorProcess()
          } // end of for loop
        } // end of process->GetProcessName() == "NRF"
      } // end of if drawNRFDataFlag
    } // end of if addNRF

// *********************************************** Track Chopper Interactions **************************************************** //

    // Chopper Analysis
    if(drawChopperIncDataFlag)
    {
      // Gammas Incident Chopper Wheel
      if(nextStep_VolumeName.compare("Chop") == 0
         && previousStep_VolumeName.compare("Chop") != 0
         && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        manager->FillNtupleIColumn(1,0, eventID);
        manager->FillNtupleDColumn(1,1, energy);
        manager->FillNtupleDColumn(1,2, loc.x());
        manager->FillNtupleDColumn(1,3, loc.y());

        if(WEIGHTED)
          manager->FillNtupleDColumn(1,4, weight);

        manager->AddNtupleRow(1);
      }
    }

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
      {
        if(drawChopperOutDataFlag)
        {
          manager->FillNtupleIColumn(2,0,eventID);
          manager->FillNtupleDColumn(2,1, energy);
          manager->FillNtupleDColumn(2,2,theta);
          manager->FillNtupleDColumn(2,3, phi);

          if(WEIGHTED)
            manager->FillNtupleDColumn(2,4, weight);

          manager->AddNtupleRow(2);
          return;
        }
        else
          return;
      }
    }

// *********************************************** Track Interrogation Object Interactions **************************************************** //

    // Incident Interrogation Object
    if(drawIntObjInDataFlag)
    {
      if(nextStep_VolumeName.compare("IntObj") == 0
         && previousStep_VolumeName.compare("IntObj") != 0)
      {
          manager->FillNtupleIColumn(3,0,eventID);
          manager->FillNtupleIColumn(3,1,trackID);
          manager->FillNtupleDColumn(3,2, energy);
          manager->FillNtupleDColumn(3,3, beamEnergy);
          manager->FillNtupleSColumn(3,4, CPName);
          manager->FillNtupleDColumn(3,5,theta);
          manager->FillNtupleDColumn(3,6,phi);
          manager->FillNtupleDColumn(3,7,theTrack->GetGlobalTime());
          manager->FillNtupleIColumn(3,8,seed);

          if(WEIGHTED)
            manager->FillNtupleDColumn(3,9, weight);

          manager->AddNtupleRow(3);
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
          manager->FillNtupleIColumn(4,0, eventID);
          manager->FillNtupleIColumn(4,1, trackID);
          manager->FillNtupleDColumn(4,2, energy);
          manager->FillNtupleDColumn(4,3, beamEnergy);
          manager->FillNtupleSColumn(4,4, CPName);
          manager->FillNtupleDColumn(4,5, theta);
          manager->FillNtupleDColumn(4,6, phi);
          manager->FillNtupleDColumn(4,7, theTrack->GetGlobalTime());
          manager->FillNtupleIColumn(4,8,seed);

          if(WEIGHTED)
            manager->FillNtupleDColumn(4,9, weight);

          manager->AddNtupleRow(4);
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
        manager->FillNtupleIColumn(5,0, eventID);
        manager->FillNtupleIColumn(5,1, seed);
        manager->FillNtupleIColumn(5,2, trackID);
        manager->FillNtupleDColumn(5,3, energy);
        manager->FillNtupleDColumn(5,4, beamEnergy);
        manager->FillNtupleDColumn(5,5, theTrack->GetGlobalTime());
        manager->FillNtupleSColumn(5,6, CPName);
        if(WEIGHTED)
          manager->FillNtupleDColumn(5,7, weight);

        manager->AddNtupleRow(5);
        return;
      }
    }

// *********************************************** Track Plexiglass Interactions **************************************************** //

    if(nextStep_VolumeName.compare(0,4,"Plex") == 0
        && previousStep_VolumeName.compare(0,4,"LowZ") == 0)
    {
      manager->FillNtupleIColumn(6,0, eventID);
      manager->FillNtupleIColumn(6,1, seed);
      manager->FillNtupleIColumn(6,2, trackID);
      manager->FillNtupleDColumn(6,3, energy);
      manager->FillNtupleDColumn(6,4, beamEnergy);
      manager->FillNtupleDColumn(6,5, theTrack->GetGlobalTime());
      manager->FillNtupleDColumn(6,6, theta);
      manager->FillNtupleDColumn(6,7, phi);
      manager->FillNtupleSColumn(6,8, CPName);

      if(WEIGHTED)
        manager->FillNtupleDColumn(6,9, weight);

      manager->AddNtupleRow(6);

      kevent->SetIncidentEnergy(energy);

      krun->AddStatusKilledPosition();
      theTrack->SetTrackStatus(fStopAndKill);
      return;
    }

} // end of user stepping action function
