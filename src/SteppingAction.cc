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

#include "SteppingAction.hh"
extern G4bool bremTest;
extern G4bool debug;
extern G4String inFile;
extern G4bool addNRF;
extern G4long seed;
extern G4bool output;

SteppingAction::SteppingAction(EventAction* event)
        : G4UserSteppingAction(), kevent(event),
        drawChopperIncDataFlag(0), drawChopperOutDataFlag(0), drawNRFDataFlag(0),
        drawIntObjInDataFlag(0), drawIntObjOutDataFlag(0), drawShieldingIncDataFlag(0),
        drawPlexiIncDataFlag(0), drawWaterIncDataFlag(0),
        drawCherenkovDataFlag(0), drawCherenkov2DataFlag(0), drawDetDataFlag(0),
        WEIGHTED(false),
        stepM(NULL)
{
  stepM = new StepMessenger(this);
  fExpectedNextStatus = Undefined;
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;
}

SteppingAction::~SteppingAction()
{
  delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
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
    eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
    G4double beamEnergy = info->GetBeamEnergy()/(MeV);

    if(WEIGHTED)
      weight = info->GetWeight();

    RunInformation* krun = RunInformation::Instance();
    DetectorInformation* kdet = DetectorInformation::Instance();

    G4String nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
    G4String previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();
    // kill photons past IntObj
    G4double EndIntObj = kdet->getEndIntObj();

    // Run Cuts
    if(bremTest)
    {
        G4double EndChop = kdet->getEndChop();
        if(theTrack->GetPosition().z() > EndChop)
        {
          theTrack->SetTrackStatus(fStopAndKill);
          krun->AddStatusKilledPosition();
          return;
        }
    }

    if(theTrack->GetPosition().z() > EndIntObj)
    {
      // kill photons that go beyond the interrogation object
      theTrack->SetTrackStatus(fStopAndKill);
      krun->AddStatusKilledPosition();
      return;
    }

    if(!bremTest)
    {
      if(nextStep_VolumeName.compare(0, 3, "Col") == 0)
      {
        // kill photons in collimator
        theTrack->SetTrackStatus(fStopAndKill);
        krun->AddStatusKilledPosition();
        return;
      }
    }

    // Run Time Cut
    if(theTrack->GetGlobalTime() > 250) // cut placed on particles time greater than 1500 ns
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

    G4ThreeVector p = aStep->GetPreStepPoint()->GetMomentum();
    // sin(theta) = (vector magnitude in XY plane)/(total vector magnitude)
    // polar angle measured between the positive Z axis and the vector
    G4double theta = std::asin(std::sqrt(std::pow(p.x(),2)+std::pow(p.y(),2))/p.mag());
    // sin(phi) -> angle in the XY plane reference to the positive X axis
    G4double phi = std::asin(p.y()/p.mag());
    G4ThreeVector loc = theTrack->GetPosition();

// *********************************** Track Bremsstrahlung Beam for Brem Test ***************************************** //

    if(bremTest)
    {
      // exiting Brem Radiator cuts
      if(nextStep_VolumeName.compare("Brem") != 0
          && previousStep_VolumeName.compare("Brem") == 0)

      {
        if(std::cos(phi) < 0.)
        {
          krun->AddStatusKilledPhiAngle();
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }
        if(std::cos(theta) < 0.)
        {
          krun->AddStatusKilledThetaAngle();
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }

        manager->FillNtupleIColumn(1,0, eventID);
        manager->FillNtupleDColumn(1,1, energy);
        manager->FillNtupleDColumn(1,2, theta);
        manager->FillNtupleDColumn(1,3, phi);
        manager->AddNtupleRow(1);
      }
      // exiting BremBacking
      if(nextStep_VolumeName.compare("CBack") != 0
          && previousStep_VolumeName.compare("CBack") == 0
          && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        krun->AddBremBackingHit();
        if(CPName.compare("eBrem") !=0)
        {
          krun->AddStatusKilledProcess();
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }
        else if(std::cos(phi) < 0.)
        {
          krun->AddStatusKilledPhiAngle();
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }
        else if(std::cos(theta) < 0.)
        {
          krun->AddStatusKilledThetaAngle();
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }
        else
        {
          manager->FillNtupleIColumn(2,0, eventID);
          manager->FillNtupleDColumn(2,1, energy);
          manager->FillNtupleDColumn(2,2, theta);
          manager->FillNtupleDColumn(2,3, phi);
          //manager->FillNtupleSColumn(0,4, CPName);
          manager->AddNtupleRow(2);
          return;
        }
      }
    }

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
          manager->FillNtupleIColumn(2,0, eventID);
          manager->FillNtupleDColumn(2,1, energy);
          manager->FillNtupleSColumn(2,2, nextStep_VolumeName);
          manager->FillNtupleDColumn(2,3, loc.z()/(cm));
          manager->FillNtupleDColumn(2,4, theta);
          manager->FillNtupleDColumn(2,5, phi);
          manager->FillNtupleIColumn(2,6, seed);

          if(WEIGHTED)
            manager->FillNtupleDColumn(2,7,weight);

          manager->AddNtupleRow(2);
        }
      }
    }

// *********************************************** Track Chopper Interactions **************************************************** //

    // Chopper Analysis
    if(drawChopperIncDataFlag)
    {
      // Gammas Incident Chopper Wheel
      if(nextStep_VolumeName.compare("Chop") == 0
         && previousStep_VolumeName.compare("Chop") != 0
         && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        if(bremTest)
        {
          if(CPName.compare("eBrem") !=0)
          {
            krun->AddStatusKilledProcess();
            theTrack->SetTrackStatus(fStopAndKill);
            return;
          }
        }

        manager->FillNtupleIColumn(0,0, eventID);
        manager->FillNtupleDColumn(0,1, energy);
        manager->FillNtupleDColumn(0,2, loc.x());
        manager->FillNtupleDColumn(0,3, loc.y());
        if(WEIGHTED)
          manager->FillNtupleDColumn(0,4, weight);
        manager->AddNtupleRow(0);
        // Once Incident Chopper record data and kill for brem test
        if(bremTest)
        {
          theTrack->SetTrackStatus(fStopAndKill);
          return;
        }
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
          manager->FillNtupleIColumn(1,0,eventID);
          manager->FillNtupleDColumn(1,1, energy);
          manager->FillNtupleDColumn(1,2,theta);
          manager->FillNtupleDColumn(1,3, phi);
          if(WEIGHTED)
            manager->FillNtupleDColumn(1,4, weight);
          manager->AddNtupleRow(1);
          return;
        }
      }
    }

// *********************************************** Track Interrogation Object Interactions **************************************************** //

    // Incident Interrogation Object
    if(drawIntObjInDataFlag && !bremTest)
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
        if(drawIntObjOutDataFlag && !bremTest)
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
      }// end else
    }// end if exiting Interrogation Object

// *********************************************** Track Shielding Interactions  **************************************************** //

    // Track particles incident shielding from world
    if(drawShieldingIncDataFlag && !bremTest)
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
      if(drawPlexiIncDataFlag && !bremTest)
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
        return;
      }
    }

// *********************************************** Track Water Tank Interactions **************************************************** //

    // Water Analysis
    // First time incident Water keep track of NRF hitting water
    // Be careful here it will keep track of reflections
    if(drawWaterIncDataFlag && !bremTest)
    {
      if(nextStep_VolumeName.compare("Water") == 0
         && previousStep_VolumeName.compare("Water") != 0)
      {
        manager->FillNtupleIColumn(7,0, eventID);
        manager->FillNtupleIColumn(7,1, trackID);
        manager->FillNtupleDColumn(7,2, energy);
        manager->FillNtupleSColumn(7,3, CPName);
        if(WEIGHTED)
          manager->FillNtupleDColumn(7,4, weight);
        manager->AddNtupleRow(7);
        return;
      }
    }

// *********************************************** Track Cherenkov Interactions **************************************************** //

    // While in water keep track of cherenkov and pass number of cherenkov to EventAction
    if(startPoint->GetPhysicalVolume()->GetName().compare("Water")==0)
    {
      // only care about secondaries that occur in water volume

      const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
      if(secondaries->size()>0)
      {
        if(drawCherenkovDataFlag)
        {
          kevent->CherenkovEnergy(energy);
          kevent->CherenkovSecondaries(secondaries->size());
          kevent->CherenkovTime(theTrack->GetGlobalTime());
        }

        for(unsigned int i=0; i<secondaries->size(); ++i)
        {
          if(secondaries->at(i)->GetParentID()>0)
          {
              if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
              {
                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation")
                {
                  krun->AddScintillationEnergy(secondaries->at(i)->GetKineticEnergy());
                  krun->AddScintillation();
                }
                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov")
                {
                  // for total run
                  if(drawCherenkov2DataFlag)
                  {
                    manager->FillNtupleIColumn(9,0, eventID);
                    manager->FillNtupleDColumn(9,1,secondaries->at(i)->GetKineticEnergy()/(MeV));
                    G4ThreeVector p_cher = secondaries->at(i)->GetMomentum();
                    G4double phi_cher = std::asin(p_cher.y()/p_cher.mag());
                    manager->FillNtupleDColumn(9,2,phi_cher);

                    if(WEIGHTED)
                      manager->FillNtupleDColumn(9,3, weight);

                    manager->AddNtupleRow(9);
                  }
                  krun->AddCerenkovEnergy(secondaries->at(i)->GetKineticEnergy());
                  krun->AddCerenkov();
                }
              }
          }
        }
      } // end of optical photons if statement
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

          if(opProc && !bremTest)
          {
            theStatus = opProc->GetStatus();

            if(theStatus == Transmission)
            {
                    procCount = "Trans";
            }
            else if(theStatus == FresnelRefraction)
            {
                    procCount = "Refr";
            }
            else if (theStatus == TotalInternalReflection)
            {
                    procCount = "Int_Refl";
            }
            else if (theStatus == LambertianReflection)
            {
                    procCount = "Lamb";
            }
            else if (theStatus == LobeReflection)
            {
                    procCount = "Lobe";
            }
            else if (theStatus == SpikeReflection)
            {
                    procCount = "Spike";
            }
            else if (theStatus == BackScattering)
            {
                    procCount = "BackS";
            }
            else if (theStatus == Absorption)
            {
                    procCount = "Abs";
            }
            // Keep track of detected photons
            else if (theStatus == Detection)
            {
                procCount = "Det";
                manager->FillNtupleIColumn(10,0,eventID);
                manager->FillNtupleDColumn(10,1, theParticle->GetKineticEnergy()/(MeV));
                manager->FillNtupleDColumn(10,2, beamEnergy);
                manager->FillNtupleDColumn(10,3, loc.x()/(cm));
                manager->FillNtupleDColumn(10,4, loc.y()/(cm));
                G4String creatorProcess;

                if(theTrack->GetCreatorProcess() !=0)
                    creatorProcess = theTrack->GetCreatorProcess()->GetProcessName();
                else
                    creatorProcess = "Brem";

                manager->FillNtupleSColumn(10,5, creatorProcess);
                manager->FillNtupleDColumn(10,6, theTrack->GetGlobalTime()); // time units is nanoseconds
                manager->FillNtupleIColumn(10,7, seed);
                if(WEIGHTED)
                  manager->FillNtupleDColumn(10,8, weight);
                manager->AddNtupleRow(10);
            }
            else if (theStatus == NotAtBoundary)
            {
                    procCount = "NotAtBoundary";
            }
            else if (theStatus == SameMaterial)
            {
                    procCount = "SameMaterial";
            }
            else if (theStatus == StepTooSmall)
            {
                    procCount = "SteptooSmall";
            }
            else if (theStatus == NoRINDEX)
            {
                    procCount = "NoRINDEX";
            }
            else
            {
                    procCount = "noStatus";
            }
            // Keep track of Detector Process Data
            if(drawDetDataFlag && !bremTest)
            {
              manager->FillNtupleIColumn(11,0,eventID);
              manager->FillNtupleDColumn(11,1, theParticle->GetKineticEnergy()/(MeV));
              manager->FillNtupleDColumn(11,2, beamEnergy);
              manager->FillNtupleSColumn(11,3, procCount);
              manager->FillNtupleIColumn(11,4,seed);

              if(WEIGHTED)
                manager->FillNtupleDColumn(11,5, weight);

              manager->AddNtupleRow(11);
            } // for if keeping track of detector process data

          } // for if opProc
        } // for for loop
      } // for if statement if first time in photocathode
    } // for if at boundary
} // end of user stepping action function
