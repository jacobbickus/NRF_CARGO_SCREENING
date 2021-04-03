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

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4OpBoundaryProcess.hh"
#include "RunInformation.hh"
#include "DetectorInformation.hh"
#include "StackingAction.hh"
#include "HistoManager.hh"
#include "StepMessenger.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "eventInformation.hh"

#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

class StepMessenger;

class SteppingAction : public G4UserSteppingAction
{
public:
SteppingAction(EventAction*);
virtual ~SteppingAction();

// method from the base class
virtual void UserSteppingAction(const G4Step*);

public: // for now set to public until I can figure out how to work with protected

void SetChopperIncDataFlag(G4int val){drawChopperIncDataFlag = val;}
void SetChopperOutDataFlag(G4int val){drawChopperOutDataFlag = val;}
void SetNRFDataFlag(G4int val){drawNRFDataFlag = val;}
void SetIntObjInDataFlag(G4int val){drawIntObjInDataFlag = val;}
void SetIntObjOutDataFlag(G4int val){drawIntObjOutDataFlag = val;}
void SetWaterIncDataFlag(G4int val){drawWaterIncDataFlag = val;}
void SetCherenkovDataFlag(G4int val){drawCherenkovDataFlag = val;}
void SetDetDataFlag(G4int val){drawDetDataFlag = val;}

private:
G4double weight;
EventAction* kevent;
G4OpBoundaryProcessStatus fExpectedNextStatus;
G4String procCount;
G4int drawChopperIncDataFlag, drawChopperOutDataFlag, drawNRFDataFlag, drawIntObjInDataFlag, drawIntObjOutDataFlag, drawWaterIncDataFlag, drawCherenkovDataFlag, drawDetDataFlag;
StepMessenger* stepM;
};

#endif
