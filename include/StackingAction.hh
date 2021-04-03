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

#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "RunInformation.hh"
#include "DetectorInformation.hh"
#include "G4Neutron.hh"
#include "RunAction.hh"

class StackingAction : public G4UserStackingAction
{
public:
StackingAction();
virtual ~StackingAction();

public:
virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
};

#endif
