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

SteppingBremTest::SteppingBremTest(EventAction* event)
: G4UserSteppingAction()
{}
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
  eventInformation* info =
            (eventInformation*)(G4RunManager::GetRunManager()->GetCurrentEvent()->GetUserInformation());
  G4double beamEnergy = info->GetBeamEnergy()/(MeV);

  RunInformation* krun = RunInformation::Instance();
  DetectorInformation* kdet = DetectorInformation::Instance();

  G4String nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
  G4String previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();

  G4double EndChop = kdet->getEndChop();
  if(theTrack->GetPosition().z() > EndChop)
  {
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

    manager->FillNtupleIColumn(0,0, eventID);
    manager->FillNtupleDColumn(0,1, energy);
    manager->FillNtupleDColumn(0,2, theta);
    manager->FillNtupleDColumn(0,3, phi);
    manager->AddNtupleRow(0);
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
      manager->FillNtupleIColumn(1,0, eventID);
      manager->FillNtupleDColumn(1,1, energy);
      manager->FillNtupleDColumn(1,2, theta);
      manager->FillNtupleDColumn(1,3, phi);
      manager->AddNtupleRow(1);
      return;
    } // end else
  } // end if exiting brem backing

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

    manager->FillNtupleIColumn(2,0, eventID);
    manager->FillNtupleDColumn(2,1, energy);
    manager->FillNtupleDColumn(2,2, loc.x());
    manager->FillNtupleDColumn(2,3, loc.y());
    manager->AddNtupleRow(2);
    // Once Incident Chopper record data and kill for brem test
    theTrack->SetTrackStatus(fStopAndKill);
    return;
  }
}// end of user stepping function
