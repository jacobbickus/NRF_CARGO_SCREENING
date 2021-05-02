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

#ifndef SteppingWithoutChopper_h
#define SteppingWithoutChopper_h 1

#include "G4UserSteppingAction.hh"
#include "BaseSteppingAction.hh"

class SteppingWithoutChopper: public G4UserSteppingAction, public BaseSteppingAction
{
public:
  SteppingWithoutChopper(EventAction*);
  virtual ~SteppingWithoutChopper();
  virtual void UserSteppingAction(const G4Step*);

private:
  EventAction* kevent;

};

#endif
