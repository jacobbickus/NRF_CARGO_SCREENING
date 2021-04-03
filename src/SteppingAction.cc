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

SteppingAction::SteppingAction(EventAction* event)
        : G4UserSteppingAction(), kevent(event),
        drawChopperIncDataFlag(0), drawChopperOutDataFlag(0), drawNRFDataFlag(0),
        drawIntObjInDataFlag(0), drawIntObjOutDataFlag(0), drawWaterIncDataFlag(0),
        drawCherenkovDataFlag(0), drawDetDataFlag(0),
        stepM(NULL)
{
  stepM = new StepMessenger(this);
  fExpectedNextStatus = Undefined;
}

SteppingAction::~SteppingAction()
{
  delete stepM;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

    G4StepPoint* endPoint   = aStep->GetPostStepPoint();
    G4StepPoint* startPoint = aStep->GetPreStepPoint();
    G4Track* theTrack = aStep->GetTrack();

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
    if(!inFile.compare(0,24,"brems_distributions.root"))
    {
      eventInformation* info = (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
      weight = info->GetWeight();
    }

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
    else if(nextStep_VolumeName.compare(0, 3, "Col") == 0)
    {
      // kill photons in collimator
      theTrack->SetTrackStatus(fStopAndKill);
      krun->AddStatusKilledPosition();
      return;
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

    G4int isNRF = 0;
    G4AnalysisManager* manager = G4AnalysisManager::Instance();

    G4String CPName = "beam";
    // Check if Track is created by NRF
    if(theTrack->GetCreatorProcess() !=0)
    {
      CPName = theTrack->GetCreatorProcess()->GetProcessName();
      if(addNRF)
      {
        if(CPName == "NRF")
        {
          isNRF = 1;
        }
      }
    }

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
      if(nextStep_VolumeName.compare(0,11,"BremBacking") != 0
          && previousStep_VolumeName.compare(0,11,"BremBacking") == 0
          && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        if(std::cos(theta) < 0.94 || CPName != "eBrem")
        {
          theTrack->SetTrackStatus(fStopAndKill);
          krun->AddStatusKilledPosition();
        }
        else
        {
          manager->FillNtupleDColumn(0,0,energy);
          manager->FillNtupleDColumn(0,1, theta);
          manager->FillNtupleDColumn(0,2, phi);
          manager->AddNtupleRow(0);
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
          manager->FillNtupleIColumn(3,0, eventID);
          manager->FillNtupleDColumn(3,1,energy);
          manager->FillNtupleSColumn(3,2,endPoint->GetPhysicalVolume()->GetName());
          manager->FillNtupleDColumn(3,3, loc.z()/(cm));
          if(!inFile.compare(0,24,"brems_distributions.root"))
            manager->FillNtupleDColumn(3,4,weight);
          manager->AddNtupleRow(3);
        }
      }
    }

// *********************************************** Track Chopper Interactions **************************************************** //

    // Chopper Analysis
    if(drawChopperIncDataFlag)
    {
      // Gammas Incident Chopper Wheel
      if(nextStep_VolumeName.compare(0, 4,"Chop") == 0
         && previousStep_VolumeName.compare(0, 4, "Chop") != 0
         && theTrack->GetParticleDefinition() == G4Gamma::Definition())
      {
        manager->FillNtupleIColumn(1,0,eventID);
        manager->FillNtupleDColumn(1,1, energy);
        if(!inFile.compare(0,24,"brems_distributions.root"))
          manager->FillNtupleDColumn(1,2, weight);
        manager->AddNtupleRow(1);
      }
    }

    // Gammas Exiting Chopper Wheel
    if(nextStep_VolumeName.compare(0,4,"Chop") != 0
       && previousStep_VolumeName.compare(0,4,"Chop") == 0)
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
          manager->FillNtupleIColumn(2,2,isNRF);
          manager->FillNtupleDColumn(2,3,theta);
          manager->FillNtupleDColumn(2,4, phi);
          if(!inFile.compare(0,24,"brems_distributions.root"))
            manager->FillNtupleDColumn(2,5, weight);
          manager->AddNtupleRow(2);
        }
      }
    }

// *********************************************** Track Interrogation Object Interactions **************************************************** //

        // Interrogation Object Analysis
        if(!bremTest)
        {
          // Incident Interrogation Object
          if(drawIntObjInDataFlag)
          {
            if(nextStep_VolumeName.compare(0, 6,"IntObj") == 0
               && previousStep_VolumeName.compare(0, 6, "IntObj") != 0)
            {
                manager->FillNtupleIColumn(4,0,eventID);
                manager->FillNtupleIColumn(4,1,trackID);
                manager->FillNtupleDColumn(4,2, energy);
                manager->FillNtupleSColumn(4,3, CPName);
                manager->FillNtupleDColumn(4,4,theta);
                manager->FillNtupleDColumn(4,5,phi);
                manager->FillNtupleDColumn(4,6,theTrack->GetGlobalTime());
                if(!inFile.compare(0,24,"brems_distributions.root"))
                  manager->FillNtupleDColumn(4,7, weight);
                manager->AddNtupleRow(4);
            }
          }
          // Exiting Interrogation Object
          if(nextStep_VolumeName.compare(0, 6,"IntObj") != 0
             && previousStep_VolumeName.compare(0,6,"IntObj") == 0)
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
                manager->FillNtupleIColumn(5,0, eventID);
                manager->FillNtupleIColumn(5,1, trackID);
                manager->FillNtupleDColumn(5,2, energy);
                manager->FillNtupleSColumn(5,3, CPName);
                manager->FillNtupleDColumn(5,4, theta);
                manager->FillNtupleDColumn(5,5, phi);
                manager->FillNtupleDColumn(5,6, theTrack->GetGlobalTime());

                if(!inFile.compare(0,24,"brems_distributions.root"))
                  manager->FillNtupleDColumn(5,7, weight);

                manager->AddNtupleRow(5);
              }// end if drawIntObjOutDataFlag
            }// end else
          }// end if exiting Interrogation Object
        }// end if !bremTest

// *********************************************** Track Water Tank Interactions **************************************************** //

        // Water Analysis
        // First time incident Water keep track of NRF hitting water
        // Be careful here it will keep track of reflections
        if(drawWaterIncDataFlag && !bremTest)
        {
          if(nextStep_VolumeName.compare(0, 5,"Water") == 0
             && previousStep_VolumeName.compare(0, 5, "Water") != 0)
          {
            manager->FillNtupleIColumn(6,0, eventID);
            manager->FillNtupleIColumn(6,1, trackID);
            manager->FillNtupleDColumn(6,2, energy);
            manager->FillNtupleSColumn(6,3, CPName);
            if(!inFile.compare(0,24,"brems_distributions.root"))
              manager->FillNtupleDColumn(6,4, weight);
            manager->AddNtupleRow(6);
          }
        }

// *********************************************** Track Cherenkov Interactions **************************************************** //

        // While in water keep track of cherenkov and pass number of cherenkov to EventAction
        if(startPoint->GetPhysicalVolume()->GetName().compare(0,5,"Water")==0)
        {
                // only care about secondaries that occur in water volume
          if(bremTest)
          {
            theTrack->SetTrackStatus(fStopAndKill); // kill track only intersted in incident chopper Data
            krun->AddStatusKilledPosition();
          }

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
          if(nextStep_VolumeName.compare(0,2,"PC")==0
             && previousStep_VolumeName.compare(0,2,"PC")!=0)
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
                    manager->FillNtupleIColumn(8,0,eventID);
                    manager->FillNtupleDColumn(8,1, theParticle->GetKineticEnergy()/(MeV));
                    manager->FillNtupleDColumn(8,2, loc.x()/(cm));
                    manager->FillNtupleDColumn(8,3, loc.y()/(cm));
                    G4String creatorProcess;

                    if(theTrack->GetCreatorProcess() !=0)
                        creatorProcess = theTrack->GetCreatorProcess()->GetProcessName();
                    else
                        creatorProcess = "Brem";

                    manager->FillNtupleSColumn(8,4, creatorProcess);
                    manager->FillNtupleDColumn(8,5, theTrack->GetGlobalTime()); // time units is nanoseconds
                    if(!inFile.compare(0,24,"brems_distributions.root"))
                      manager->FillNtupleDColumn(8,6, weight);
                    manager->AddNtupleRow(8);
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
                  manager->FillNtupleIColumn(9,0,eventID);
                  manager->FillNtupleDColumn(9,1, theParticle->GetKineticEnergy()/(MeV));
                  manager->FillNtupleSColumn(9,2, procCount);
                  if(!inFile.compare(0,24,"brems_distributions.root"))
                    manager->FillNtupleDColumn(9,3, weight);
                  manager->AddNtupleRow(9);
                } // for if keeping track of detector process data

              } // for if opProc
            } // for for loop
          } // for if statement if first time in photocathode
        } // for if at boundary
} // end of user stepping action function
