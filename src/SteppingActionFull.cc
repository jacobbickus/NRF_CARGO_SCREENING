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

#include "SteppingActionFull.hh"

extern G4bool output;
extern G4bool addNRF;

SteppingActionFull::SteppingActionFull(EventAction* event)
: G4UserSteppingAction(), BaseSteppingAction(), kevent(event)
{}

SteppingActionFull::~SteppingActionFull()
{}

void SteppingActionFull::UserSteppingAction(const G4Step* aStep)
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
    gtime = theTrack->GetGlobalTime();
// ************************************************* Checks and Cuts Complete ************************************************** //

    CPName = "beam";
    if(theTrack->GetCreatorProcess() !=0)
      CPName = theTrack->GetCreatorProcess()->GetProcessName();

    G4ThreeVector p = startPoint->GetMomentum();
    // sin(theta) = (vector magnitude in XY plane)/(total vector magnitude)
    // polar angle measured between the positive Z axis and the vector
    theta = std::asin(std::sqrt(std::pow(p.x(),2)+std::pow(p.y(),2))/p.mag());
    // sin(phi) -> angle in the XY plane reference to the positive X axis
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
    if(drawChopperIncDataFlag)
    {
      // Gammas Incident Chopper Wheel
      if(nextStep_VolumeName.compare("Chop") == 0
         && previousStep_VolumeName.compare("Chop") != 0
         && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        FillChopperInc(1, loc.x(), loc.y());
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
          FillChopperOut(2);
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
        FillIntObjIn(3);
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
          FillIntObjOut(4);
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
        FillShielding(5);
        return;
      }
    }

// *********************************************** Track Plexiglass Interactions **************************************************** //


    if(nextStep_VolumeName.compare(0,4,"Plex") == 0
        && previousStep_VolumeName.compare(0,4,"Last") == 0)
    {
      if(std::cos(phi) < 0.6)
      {
        krun->AddStatusKilledPhiAngle();
        theTrack->SetTrackStatus(fStopAndKill);
        return;
      }
      if(std::cos(theta) < 0.2)
      {
        krun->AddStatusKilledThetaAngle();
        theTrack->SetTrackStatus(fStopAndKill);
        return;
      }
      if(drawPlexiIncDataFlag)
      {
        FillPlexi(6);
        return;
      }
      else
        return;
    }

// *********************************************** Track Water Tank Interactions **************************************************** //

    // Water Analysis
    // First time incident Water keep track of NRF hitting water
    // Be careful here it will keep track of reflections
    if(drawWaterIncDataFlag)
    {
      if(nextStep_VolumeName.compare("Water") == 0
         && previousStep_VolumeName.compare("Water") != 0)
      {
        FillWater(7);
        return;
      }
    }

// *********************************************** Track Cherenkov Interactions **************************************************** //

    // While in water keep track of cherenkov and pass number of cherenkov to EventAction
    if(startPoint->GetPhysicalVolume()->GetName().compare("Water")==0)
    {
      const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
      if(secondaries->size()>0)
      {
        FillScintAndCherenkov(9,11, secondaries);
      }
    } // end of if loop while inside water

// *********************************************** Track Photocathode Interactions **************************************************** //

        // Photocathode Analysis

    if(endPoint->GetStepStatus() == fGeomBoundary)
    {
      const G4DynamicParticle* theParticle = theTrack->GetDynamicParticle();
      G4OpBoundaryProcessStatus theStatus = Undefined;
      G4ProcessManager* OpManager =
              G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
      G4int MAXofPostStepLoops =
              OpManager->GetPostStepProcessVector()->entries();
      G4ProcessVector* postStepDoItVector =
              OpManager->GetPostStepProcessVector(typeDoIt);
      // incident photocathode
      if(nextStep_VolumeName.compare("PC")==0
         && previousStep_VolumeName.compare("PC")!=0)
      {
        krun->AddTotalSurface();

        for (G4int i=0; i<MAXofPostStepLoops; ++i)
        {
          G4VProcess* currentProcess = (*postStepDoItVector)[i];

          G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);

          if(opProc)
          {
            theStatus = opProc->GetStatus();
            // Keep track of detected photons
            if(theStatus == Detection)
            {
              G4String creatorProcess;

              if(theTrack->GetCreatorProcess() !=0)
                  creatorProcess = theTrack->GetCreatorProcess()->GetProcessName();
              else
                  creatorProcess = "Brem";

              procCount = "Det";
              FillDetected(12, theParticle->GetKineticEnergy()/(MeV), creatorProcess);
            }
            else if(theStatus == Transmission)
              procCount = "Trans";
            else if(theStatus == FresnelRefraction)
              procCount = "Refr";
            else if (theStatus == TotalInternalReflection)
              procCount = "Int_Refl";
            else if (theStatus == LambertianReflection)
              procCount = "Lamb";
            else if (theStatus == LobeReflection)
              procCount = "Lobe";
            else if (theStatus == SpikeReflection)
              procCount = "Spike";
            else if (theStatus == BackScattering)
              procCount = "BackS";
            else if (theStatus == Absorption)
              procCount = "Abs";
            else if (theStatus == NotAtBoundary)
              procCount = "NotAtBoundary";
            else if (theStatus == SameMaterial)
              procCount = "SameMaterial";
            else if (theStatus == StepTooSmall)
              procCount = "SteptooSmall";
            else if (theStatus == NoRINDEX)
              procCount = "NoRINDEX";
            else
              procCount = "noStatus";
            // Keep track of Detector Process Data
            if(drawDetDataFlag)
            {
              FillIncDetector(13, theParticle->GetKineticEnergy()/(MeV));
            } // for if keeping track of detector process data

          } // for if opProc
        } // for for loop
      } // for if statement if first time in photocathode
    } // for if at boundary
} // end of user stepping action function

void SteppingActionFull::FillScintAndCherenkov(G4int num, G4int num2, const std::vector<const G4Track*>* secondaries)
{
  for(unsigned int i=0; i<secondaries->size(); ++i)
  {
    if(secondaries->at(i)->GetParentID()>0)
    {
        if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
        {
          if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation")
          {
            // for event level scintillation photon data
            if(drawScintillationDataFlag)
            {
              kevent->ScintillationEnergy(energy);
              kevent->ScintillationAddSecondary();
            }
            // for individual scintillation photon data
            if(drawScintillation2DataFlag)
            {
              manager->FillNtupleIColumn(num,0, eventID);
              manager->FillNtupleDColumn(num,1, secondaries->at(i)->GetKineticEnergy()/(MeV));
              G4ThreeVector p_scint = secondaries->at(i)->GetMomentum();
              G4double phi_scint = std::asin(p_scint.y()/p_scint.mag());
              G4double theta_scint = std::asin(std::sqrt(std::pow(p_scint.x(),2)+std::pow(p_scint.y(),2))/p_scint.mag());
              manager->FillNtupleDColumn(num,2, phi_scint);
              manager->FillNtupleDColumn(num,3, theta_scint);
              if(WEIGHTED)
                manager->FillNtupleDColumn(num,4, weight);

              manager->AddNtupleRow(num);
            }
            krun->AddScintillationEnergy(secondaries->at(i)->GetKineticEnergy());
            krun->AddScintillation();
          }
          if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov")
          {
            // for event level cherenkov photon data
            if(drawCherenkovDataFlag)
            {
              kevent->CherenkovEnergy(energy);
              kevent->CherenkovAddSecondary();
            }
            // for individual cherenkov photon data
            if(drawCherenkov2DataFlag)
            {
              manager->FillNtupleIColumn(num2,0, eventID);
              manager->FillNtupleDColumn(num2,1,secondaries->at(i)->GetKineticEnergy()/(MeV));
              G4ThreeVector p_cher = secondaries->at(i)->GetMomentum();
              G4double phi_cher = std::asin(p_cher.y()/p_cher.mag());
              manager->FillNtupleDColumn(num2,2,phi_cher);

              if(WEIGHTED)
                manager->FillNtupleDColumn(num2,3, weight);

              manager->AddNtupleRow(num2);
            }
            krun->AddCerenkovEnergy(secondaries->at(i)->GetKineticEnergy());
            krun->AddCerenkov();
          }// end if cherenkov

        } // end if optical photon
      } // end if parentID > 0
    } // end for loop
} // end of Fill function
