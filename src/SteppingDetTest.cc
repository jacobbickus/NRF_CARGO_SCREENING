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

#include "SteppingDetTest.hh"

extern G4bool output;
extern G4bool debug;
extern G4String inFile;
extern G4long seed;


SteppingDetTest::SteppingDetTest(EventAction* event)
: G4UserSteppingAction(), kevent(event), WEIGHTED(false)
{
  fExpectedNextStatus = Undefined;
  if(!inFile.compare("brems_distributions.root"))
    WEIGHTED = true;
}

SteppingDetTest::~SteppingDetTest()
{}

void SteppingDetTest::UserSteppingAction(const G4Step* aStep)
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

    G4String nextStep_VolumeName = endPoint->GetPhysicalVolume()->GetName();
    G4String previousStep_VolumeName = startPoint->GetPhysicalVolume()->GetName();

    RunInformation* krun = RunInformation::Instance();

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

    // *********************************************** Track Shielding Interactions  **************************************************** //

    // Track particles incident shielding from world

    if(nextStep_VolumeName.compare(0,5,"Atten") == 0
        && previousStep_VolumeName.compare("World") == 0)
    {
      manager->FillNtupleIColumn(0,0, eventID);
      manager->FillNtupleIColumn(0,1, seed);
      manager->FillNtupleIColumn(0,2, trackID);
      manager->FillNtupleDColumn(0,3, energy);
      manager->FillNtupleDColumn(0,4, beamEnergy);
      manager->FillNtupleDColumn(0,5, theTrack->GetGlobalTime());
      manager->FillNtupleSColumn(0,6, CPName);
      if(WEIGHTED)
        manager->FillNtupleDColumn(0,7, weight);

      manager->AddNtupleRow(0);
      return;
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

      manager->FillNtupleIColumn(1,0, eventID);
      manager->FillNtupleIColumn(1,1, seed);
      manager->FillNtupleIColumn(1,2, trackID);
      manager->FillNtupleDColumn(1,3, energy);
      manager->FillNtupleDColumn(1,4, beamEnergy);
      manager->FillNtupleDColumn(1,5, theTrack->GetGlobalTime());
      manager->FillNtupleDColumn(1,6, theta);
      manager->FillNtupleDColumn(1,7, phi);
      manager->FillNtupleSColumn(1,8, CPName);
      if(WEIGHTED)
        manager->FillNtupleDColumn(1,9, weight);

      manager->AddNtupleRow(1);
      return;
    }
    // While in water keep track of cherenkov and pass number of cherenkov to EventAction
    if(startPoint->GetPhysicalVolume()->GetName().compare("Water")==0)
    {
      const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep();
      if(secondaries->size()>0)
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
                  kevent->ScintillationEnergy(energy);
                  kevent->ScintillationAddSecondary();
                  // for individual scintillation photon data
                  manager->FillNtupleIColumn(3,0, eventID);
                  manager->FillNtupleDColumn(3,1, secondaries->at(i)->GetKineticEnergy()/(MeV));
                  G4ThreeVector p_scint = secondaries->at(i)->GetMomentum();
                  G4double phi_scint = std::asin(p_scint.y()/p_scint.mag());
                  G4double theta_scint = std::asin(std::sqrt(std::pow(p_scint.x(),2)+std::pow(p_scint.y(),2))/p_scint.mag());
                  manager->FillNtupleDColumn(3,2, phi_scint);
                  manager->FillNtupleDColumn(3,3, theta_scint);
                  if(WEIGHTED)
                    manager->FillNtupleDColumn(3,4, weight);

                  manager->AddNtupleRow(3);
                  krun->AddScintillationEnergy(secondaries->at(i)->GetKineticEnergy());
                  krun->AddScintillation();
                }
                if(secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov")
                {
                  // for event level cherenkov photon data
                  kevent->CherenkovEnergy(energy);
                  kevent->CherenkovAddSecondary();
                  // for individual cherenkov photon data
                  manager->FillNtupleIColumn(5,0, eventID);
                  manager->FillNtupleDColumn(5,1,secondaries->at(i)->GetKineticEnergy()/(MeV));
                  G4ThreeVector p_cher = secondaries->at(i)->GetMomentum();
                  G4double phi_cher = std::asin(p_cher.y()/p_cher.mag());
                  manager->FillNtupleDColumn(5,2,phi_cher);

                  if(WEIGHTED)
                    manager->FillNtupleDColumn(5,3, weight);

                  manager->AddNtupleRow(5);

                  krun->AddCerenkovEnergy(secondaries->at(i)->GetKineticEnergy());
                  krun->AddCerenkov();
                } // end of if secondaries->at(i)->GetCreatorProcess
              } // end of if secondaries->at(i)->GetDynamicParticle()
          } // end of if secondaries->at(i)->GetParentID()
        } // end of for loop
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

        if(opProc)
        {
          theStatus = opProc->GetStatus();
          if (theStatus == Detection)
          {
            manager->FillNtupleIColumn(6,0,eventID);
            manager->FillNtupleDColumn(6,1, theParticle->GetKineticEnergy()/(MeV));
            manager->FillNtupleDColumn(6,2, beamEnergy);
            manager->FillNtupleDColumn(6,3, loc.x()/(cm));
            manager->FillNtupleDColumn(6,4, loc.y()/(cm));
            G4String creatorProcess;

            if(theTrack->GetCreatorProcess() !=0)
                creatorProcess = theTrack->GetCreatorProcess()->GetProcessName();
            else
                creatorProcess = "Brem";

            manager->FillNtupleSColumn(6,5, creatorProcess);
            manager->FillNtupleDColumn(6,6, theTrack->GetGlobalTime()); // time units is nanoseconds
            manager->FillNtupleIColumn(6,7, seed);

            if(WEIGHTED)
              manager->FillNtupleDColumn(6,8, weight);

            manager->AddNtupleRow(6);
          } // for if status == Detection
        } // for if opProc
      } // for for loop
    } // for if statement if first time in photocathode
  } // for if at boundary
} // end of SteppingDetTest::UserSteppingAction
