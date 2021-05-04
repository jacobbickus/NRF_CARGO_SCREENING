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

#include "SteppingBremTest.hh"

extern G4bool output;
extern G4bool debug;
extern G4bool addNRF;
extern G4bool WEIGHTED;

SteppingBremTest::SteppingBremTest()
: G4UserSteppingAction(), BaseSteppingAction()
{
  if(debug)
    std::cout << "SteppingBremTest::SteppingBremTest Initialized." << std::endl;
}

SteppingBremTest::~SteppingBremTest()
{}

void SteppingBremTest::UserSteppingAction(const G4Step* aStep)
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
  EventInformation* info =
            (EventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
  beamEnergy = info->GetBeamEnergy()/(MeV);

  nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
  previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();

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

// **************************************************** Track Linac Interactions **************************************************** //
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

    FillBremRadiator(1);
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
      FillBremBacking(2);
      return;
    } // end else
  } // end if exiting brem backing

// **************************************************** Track Chopper Interactions  **************************************************** //
  // Gammas Incident Chopper Wheel
  if(nextStep_VolumeName.compare("Chop") == 0
     && previousStep_VolumeName.compare("Chop") != 0
     && theTrack->GetParticleDefinition() == G4Gamma::Definition())
  {

    if(CPName.compare("eBrem") !=0)
    {
      krun->AddStatusKilledProcess();
      theTrack->SetTrackStatus(fStopAndKill);
      return;
    }

    FillChopperInc(3, loc.x(), loc.y());
    return;
  }

  // Gammas Exiting Chopper Wheel
  if(nextStep_VolumeName.compare("Chop") != 0
     && previousStep_VolumeName.compare("Chop") == 0)
  {
    FillChopperOut(4);
    theTrack->SetTrackStatus(fStopAndKill);
    krun->AddStatusKilledPosition();
    return;
  }

}// end of user stepping function
