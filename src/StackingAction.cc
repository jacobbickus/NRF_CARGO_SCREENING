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

#include "StackingAction.hh"


StackingAction::StackingAction()
{
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* currentTrack)
{
  DetectorInformation* detInfo = DetectorInformation::Instance();
  RunInformation* runInfo = RunInformation::Instance();
  // if a new track is created beyond interogation material kill it
  G4double EndIntObj = detInfo->getEndIntObj();
  G4double trackZ = currentTrack->GetPosition().z();

  if(trackZ/(cm) > EndIntObj/(cm))
  {
    runInfo->AddStatusKilledPosition();
    return fKill;
  }
  
  G4ParticleDefinition *pdef = currentTrack->GetDefinition();
  // kill neutrons (probably not important)
  if(pdef == G4Neutron::Definition()) return fKill;
  return fUrgent;
}