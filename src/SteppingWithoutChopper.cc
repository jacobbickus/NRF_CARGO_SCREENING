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

#include "SteppingWithoutChopper.hh"

extern G4bool output;
extern G4long seed;
extern G4bool debug;


SteppingWithoutChopper::SteppingWithoutChopper(EventAction* event)
: G4UserSteppingAction(), BaseSteppingAction(), kevent(event)
{
  fExpectedNextStatus = Undefined;
  WEIGHTED = false;
  if(debug)
    std::cout << "SteppingWithoutChopper::SteppingWithoutChopper Initialized." << std::endl;
}

SteppingWithoutChopper::~SteppingWithoutChopper()
{}

void SteppingWithoutChopper::UserSteppingAction(const G4Step* aStep)
{
  if(!output)
    return;

  G4StepPoint* endPoint   = aStep->GetPostStepPoint();
  G4StepPoint* startPoint = aStep->GetPreStepPoint();
  G4Track* theTrack       = aStep->GetTrack();

  // Run Logical Checks
  if(endPoint == NULL)
    return; // at the end of the world
  else if(endPoint->GetPhysicalVolume()==NULL)
    return;

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

  CPName = "beam";
  if(theTrack->GetCreatorProcess() !=0)
    CPName = theTrack->GetCreatorProcess()->GetProcessName();

  G4ThreeVector p = startPoint->GetMomentum();
  theta = std::asin(std::sqrt(std::pow(p.x(),2)+std::pow(p.y(),2))/p.mag());
  phi = std::asin(p.y()/p.mag());
  G4ThreeVector loc = theTrack->GetPosition();

// **************************************************** Track NRF Materials **************************************************** //

  const G4VProcess* process = endPoint->GetProcessDefinedStep();

  if(process->GetProcessName() == "NRF")
  {
    krun->AddNRF();
    const G4TrackVector* emitted_nrf = aStep->GetSecondary();
    FillNRF(0, loc.z(), emitted_nrf);
  } // end of process->GetProcessName() == "NRF"

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
      FillPlexi(4);
      return;
    }
    else
      return;
  }

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
                creatorProcess = "Beam";

            procCount = "Det";
            FillDetected(5, theParticle->GetKineticEnergy()/(MeV), creatorProcess);
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
            FillIncDetector(6, theParticle->GetKineticEnergy()/(MeV));
          } // for if keeping track of detector process data

        } // for if opProc
      } // for for loop
    } // for if statement if first time in photocathode
  } // for if at boundary
}
